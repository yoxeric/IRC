#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream> 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <poll.h>
#include <vector>

#include "poll.hpp"
#include "client.hpp"

#define BUFFER_SIZE 255
#define INIT_CLIENTS 5

int		send_msg(int dest_fd, int send_fd, char *msg_to_send);

int		make_server_socket(int portnb);

void	accept_new_connection(int sockfd, s_poll *pool);

void	read_data(int i, int sockfd, s_poll *pool);
