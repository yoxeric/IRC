#pragma once

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <sstream> 
#include <vector>

#include <poll.h>


#define INIT_CLIENTS 10
#define BUFFER_SIZE 255

class s_poll
{
private:

	std::vector<struct pollfd>	fds; // Array of socket file descriptors
    int 						count;

public:


	void				init_poll(int sockfd);

	void 				add_to_poll(int new_fd);
	void 				remove_from_poll(int fd);

	int					make_server_socket(int portnb);
	std::string			read_data(int client_index);
	int					accept_new_connection(int server_socket);

	int					check_pollin(int index);
	int					check_pollout(int index);

	struct pollfd*		get_poll();
	int					get_socket(int index);
	int					get_count();
};
