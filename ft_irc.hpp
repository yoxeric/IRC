
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sstream> 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <poll.h>

#define BUFFER_SIZE 255

void	add_to_poll(int new_fd, struct pollfd **poll_fds, int *poll_size, int *poll_count);
void	del_from_poll(int i, struct pollfd **poll_fds, int *poll_count);
int		send_msg(int dest_fd, int send_fd, char *msg_to_send);

int		make_server_socket(int portnb);

void	accept_new_connection(int sockfd, struct pollfd **poll_fds, int *poll_size, int *poll_count);
void	read_data(int i, int sockfd, struct pollfd** poll_fds, int *poll_count);
