#pragma once

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <unistd.h>

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>

// #include <iostream>
// #include <sstream> 
// #include <vector>
// #include <poll.h>

#include "poll.hpp"
#include "client.hpp"

#define BUFFER_SIZE 255
#define INIT_CLIENTS 5

int	private_msg_channel(int sender_index, std::string channel, std::string msg, s_poll *pool);
int	private_msg_user(int sender_index, std::string dest_nickname, std::string msg, s_poll *pool);

int		send_msg(int dest_fd, int send_fd, std::string msg_to_send);

int		make_server_socket(int portnb);

void	accept_new_connection(int sockfd, s_poll *pool);

void	read_data(int i, int sockfd, s_poll *pool);

std::string parse(int client_index, char *buffer, s_poll *pool);

// void	parse_CAP ();
// void	parse_NICK ();
// void	parse_USER ();
// void	parse_JOIN ();
// void	parse_WHO ();
// void	parse_MODE ();
// void	parse_PRVMAG ();
// void	parse_QUIT ();