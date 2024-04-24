#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <poll.h>

#include "client.hpp"

class s_poll
{
public:
	// struct pollfd *fds; // Array of socket file descriptors
    // int size; // Size of descriptor array
    // int count;

	std::vector<struct pollfd> fds; // Array of socket file descriptors
	std::vector<s_client> clients;
	std::vector<std::string> channels;
    int count;

	void add(int new_fd);
	void remove(int i);

	s_poll(int sockfd, int size);
};
