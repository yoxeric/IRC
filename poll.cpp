
#include "poll.hpp"

void s_poll::add(int new_fd)
{
	fds[count].fd = new_fd;
	fds[count].events = POLLIN;
	count++;
}

void s_poll::remove(int i)
{
	fds[i] = fds[count - 1];
	count--;
}

s_poll::s_poll(int sockfd, int size)
{
	fds.reserve(size);
	clients.reserve(size);
	// chans.reserve(size);

	// Add the listening server socket to array
	// with notification when the socket can be read
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    count = 1;
}