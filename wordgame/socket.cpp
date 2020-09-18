// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>     /* inet_ntoa */
#include <netdb.h>         /* gethostname */
#include <sys/socket.h>

#include <iostream>

#include "socket.hpp"

using std::endl;
using std::cout;

/*
 * Initialize a server address associated with the given port.
 */
struct sockaddr_in& init_server_addr(int port) {
    struct sockaddr_in *addr = new sockaddr_in();

    // Allow sockets across machines.
    addr->sin_family = AF_INET;

    // The port the process will listen on.
    addr->sin_port = htons(port);
    
    // Clear this field; sin_zero is used for padding for the struct.
    memset(&(addr->sin_zero), 0, 8);

    // Listen on all network interfaces.
    addr->sin_addr.s_addr = INADDR_ANY;

    return *addr;
}


/*
 * Create and set up a socket for a server to listen on.
 */
int set_up_server_socket(const struct sockaddr_in &self, int num_queue) {
    const int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0) {
        perror("socket");
        exit(1);
    }

    // Make sure we can reuse the port immediately after the
    // server terminates. Avoids the "address in use" error
    int on = 1;
    const int status = setsockopt(soc, SOL_SOCKET, SO_REUSEADDR,
        (const char *) &on, sizeof(on));
    if (status < 0) {
        perror("setsockopt");
        exit(1);
    }

    // Associate the process with the address and a port
    if (bind(soc, (struct sockaddr *)&self, sizeof(self)) < 0) {
        // bind failed; could be because port is in use.
        perror("bind");
        exit(1);
    }

    // Set up a queue in the kernel to hold pending connections.
    if (listen(soc, num_queue) < 0) {
        // listen failed
        perror("listen");
        exit(1);
    }

    return soc;
}


/*
 * Wait for and accept a new connection.
 * Terminate with exit code 1 if the accept call failed, otherwise return
 * the client's socket descriptor.
 */
int accept_connection(const int listenfd) {
    struct sockaddr_in peer;
    unsigned int peer_len = sizeof(peer);
    peer.sin_family = AF_INET;

    cout << "Waiting for a new connection..." << endl;

    int client_socket = accept(listenfd, (struct sockaddr *)&peer, &peer_len);
    if (client_socket < 0) {
        perror("accept");
        exit(1);
    } else {
        cout << "New connection accepted from" << 
                inet_ntoa(peer.sin_addr) << ntohs(peer.sin_port);
        return client_socket;
    }
}