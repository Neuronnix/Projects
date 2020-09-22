#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>    /* Internet domain header, for struct sockaddr_in */

struct sockaddr_in& init_server_addr(int port);
int set_up_server_socket(const struct sockaddr_in &self, int num_queue);
int accept_connection(const int listenfd);

#endif
