#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>    /* Internet domain header, for struct sockaddr_in */


class Server {
public:
	Server(int, int);

	const int accept_connection(void);
	const int accept_connection(const struct sockaddr_in& peer);
	const bool has_new_connection(void) const;
	const int select(void);
	const int isset_fd(const int fd) const;
	void set_fd(const int fd);
	void clr_fd(const int fd);
	void zero_fd(const int fd);
private:
	struct sockaddr_in& server;
	int listenfd;
	int maxfd;
	fd_set selectfds;
	struct sockaddr_in& init_server_addr(const int&) const;
	const int set_up_server_socket(const struct sockaddr_in &self, const int& num_queue) const;
};
#endif
