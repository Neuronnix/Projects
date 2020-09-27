// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>     /* inet_ntoa */
#include <netdb.h>         /* gethostname */
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

#include <iostream>

#include "debug.hpp"
#include "socket.hpp"

using std::endl;
using std::cout;



Server::Server(int port, int max_queue)
: server(init_server_addr(port))
, listenfd(set_up_server_socket(server, max_queue))
, maxfd(listenfd)
{
    // initialize selectfds and add listenfd to the
    // set of file descriptors passed into select
    FD_ZERO(&selectfds);
    FD_SET(listenfd, &selectfds);
    // maxfd identifies how far into the set to search
}


const int Server::select()
{
    return ::select(maxfd + 1, &selectfds, NULL, NULL, NULL);
}


const int Server::isset_fd(const int fd) const
{
    return FD_ISSET(fd, &selectfds);
}

void Server::set_fd(const int fd)
{
    FD_SET(fd, &selectfds);
    maxfd = std::max(fd, maxfd);
}

void Server::clr_fd(const int fd)
{
    FD_CLR(fd, &selectfds);
}

void Server::zero_fd(const int fd)
{
    FD_ZERO(&selectfds);
}

/*
 * Initialize a server address associated with the given port.
 */
struct sockaddr_in& Server::init_server_addr(const int& listenport) const 
{
    struct sockaddr_in *addr = new sockaddr_in();
    cout << "Initializing server" << endl;
    cout << "Port is " << listenport << endl;

    //IPv4 Internet address family
    addr->sin_family = AF_INET;

    addr->sin_port = htons(listenport);
    
    memset(&(addr->sin_zero), 0, 8);

    // Listen on all network interfaces.
    addr->sin_addr.s_addr = INADDR_ANY;

    return *addr;
}


const bool Server::has_new_connection(void) const
{
    return isset_fd(listenfd);
}


/*
 * Create and set up a socket for a server to listen on.
 */
const int Server::set_up_server_socket(const struct sockaddr_in &self, const int& num_queue) const 
{
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


/**
 * @brief      Accept new client connection on the server's listening socket
 *
 * @return     client socket of the accepted connection
 */
const int Server::accept_connection() {
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;

    return accept_connection(peer);
}


/**
 * @brief      Accept new client connection on the server's listening socket
 *
 * @param[in]  peer  peer socket structure used to store address of the client
 *
 * @return     socket of the accepted client.
 */
const int Server::accept_connection(const struct sockaddr_in& peer) 
{
    unsigned int peer_len = sizeof(peer);
    int client_socket = accept(listenfd, (struct sockaddr *)&peer, &peer_len);

    if (client_socket < 0) {
        BUGF();
        perror("accept");
        exit(1);
    } else {
        LOG("New connection accepted from %s:%d\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));

        set_fd(client_socket);
        return client_socket;
    }    
}