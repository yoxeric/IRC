#pragma once

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include <iostream>
#include <sstream> 
#include <vector>

#include <poll.h>


#define INIT_CLIENTS 10
#define BUFFER_SIZE 255
typedef struct sockaddr_in ipv4_saddr;

class s_poll
{
private:

	std::vector<struct pollfd>	fds; // Array of socket file descriptors
    int 						count;

public:

	void				init_poll(int sockfd);

	void 				add_to_poll(int new_fd);
	void 				remove_from_poll(int fd);


	// ----------------------  Networking -----------------------


	int					make_server_socket(int portnb);
	std::string			read_data(int client_index);
	int					accept_new_connection(int server_socket);
	void				set_to_nonblocking(int);

	int					check_pollin(int index);
	int					check_pollout(int index);
	int					check_pollhup(int index);


	// ----------------------  Getter -----------------------


	struct pollfd*		get_poll();
	int					get_socket(int index);
	int					get_count();


	// ----------------------  Debug -----------------------


	void				print();

};
