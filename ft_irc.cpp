 
#include "ft_irc.hpp"

void add_to_poll(int new_fd, struct pollfd **poll_fds, int *poll_size, int *poll_count)
{
	if (*poll_count > *poll_size)
	{
		*poll_size *= 2;
		*poll_fds = (struct pollfd *) realloc(*poll_fds, sizeof(**poll_fds) * (*poll_size));
	}
	(*poll_fds)[(*poll_count)].fd = new_fd;
	(*poll_fds)[(*poll_count)].events = POLLIN;
	(*poll_count)++;
}

void del_from_poll(int i, struct pollfd **poll_fds, int *poll_count)
{
	(*poll_fds)[i] = (*poll_fds)[(*poll_count) - 1];
	(*poll_count)--;
}

int	send_msg(int dest_fd, int send_fd, char *msg_to_send)
{
	// char msg[BUFFER_SIZE];
	std::ostringstream s;
	int status;

	// memset(&msg, '\0', sizeof(msg));
	// sprintf(msg, "[%d] client: %s ", send_fd, msg_to_send);
	// status = send(dest_fd, msg, strlen(msg), 0);
	s << "[" << send_fd << "] client: " << msg_to_send ;
	status = send(dest_fd, s.str().c_str(), s.str().size(), 0);
	if (status == -1)
	{
		std::cout << "Send error to client [" << dest_fd << "]";
		return 1;
	}
	return 0;
}

int	make_server_socket(int portnb)
{
	int sockfd;
	struct sockaddr_in serv_addr;

	//opening server socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cout << "Error openning server socket :" << strerror(errno) << std::endl;
		return (-1);
	}

	//clear server address
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnb);

	//bind server address
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Binding Failed :" << strerror(errno) << std::endl;
		return (-1);
	}

	return (sockfd);
}

void	accept_new_connection(int sockfd, struct pollfd **poll_fds, int *poll_size, int *poll_count)
{
	int client_fd;
	// socklen_t client_len = sizeof(client_addr);

	client_fd = accept(sockfd, 0, 0);
	if (client_fd < 0)
	{
		std::cout << "Error accepting client:" << strerror(errno) << std::endl;
		return ;
	}

	add_to_poll(client_fd, poll_fds, poll_size, poll_count);

	char msg[11] = "connected\n";
	send_msg(client_fd, client_fd, msg);
	std::cout << "[" << client_fd << "] client connected" << std::endl;
}


void	read_data(int i, int sockfd, struct pollfd** poll_fds, int *poll_count)
{
	char buffer[BUFFER_SIZE];
	int bytes_read;
	int send_fd;
	int dest_fd;

	send_fd = (*poll_fds)[i].fd;
	memset(&buffer, '\0', sizeof(buffer));
	bytes_read = recv(send_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read <= 0)
			std::cout << "[" << send_fd << "] client socket closed connection";
		else
			std::cout << "receive error" << strerror(errno) << std::endl;
		close(send_fd);
		del_from_poll(i, poll_fds, poll_count);
	}
	else
	{
		std::cout << "[" << send_fd << "] got message " << std::endl;
		for (int i = 0; i < *poll_count; ++i)
		{
			dest_fd = (*poll_fds)[i].fd;
			if (dest_fd != sockfd && dest_fd != send_fd)
			{
				send_msg(dest_fd, send_fd, buffer);
			}
		}
	}
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int sockfd, portnb;
	int n;

	struct pollfd *poll_fds; // Array of socket file descriptors
    int poll_size; // Size of descriptor array
    int poll_count;

	//get port
	portnb = atoi(av[1]);

	sockfd = make_server_socket(portnb);
	if (sockfd < 0)
	{
		std::cout << "server socket Error" << std::endl;
		return 1;
	}

	//start listening for 5 clients
	if (listen(sockfd, 5))
	{
		std::cout << "Error listening" << strerror(errno) << std::endl;
		return 1;
	}
	std::cout << "listening for clients..." << std::endl;

	// Prepare the array of file descriptors for poll()
    // We'll start with enough room for 5 fds in the array,
    // we'll reallocate if necessary
    poll_size = 5;
    poll_fds = (struct pollfd *) calloc(poll_size + 1, sizeof *poll_fds);
    if (!poll_fds) {
        return (4);
    }

    // Add the listening server socket to array
    // with notification when the socket can be read
    poll_fds[0].fd = sockfd;
    poll_fds[0].events = POLLIN;
    poll_count = 1;


    while (1)
    {
        // Poll sockets to see if they are ready (2 second timeout)
    	n = poll(poll_fds, poll_count, 2000);
    	if (n == 0)
    	{
			std::cout << "waiting..." << std::endl;
			continue ;
    	}
    	else if (n < 0)
    	{
			std::cout << "Poll Error" << strerror(errno) << std::endl;
			return 1;
		}

		// loop over polls
		for (int i = 0; i < poll_count; ++i)
		{
			if ((poll_fds[i].revents && POLLIN) != 1)
				continue ;
			std::cout << "[" << poll_fds[i].fd << "] ready for I/O operation" << std::endl;
			if (poll_fds[i].fd == sockfd)
			{
				accept_new_connection(sockfd, &poll_fds, &poll_size, &poll_count);
			}
			else
			{
				read_data(i, sockfd, &poll_fds, &poll_count);
			}
		}

    }
	close(sockfd);
	return 0;
}

// int main(int ac, char **av)
// {
// 	if (ac != 3)
// 	{
// 		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
// 		return 1;
// 	}

// 	int sockfd, client_fd, portnb;
// 	int n;
// 	char buffer[BUFFER_SIZE];

// 	struct sockaddr_in serv_addr;
// 	struct sockaddr_in client_addr;
// 	socklen_t client_len;

// 	//get port
// 	portnb = atoi(av[1]);

// 	// //opening server socket
// 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sockfd < 0)
// 	{
// 		std::cout << "Error openning server socket" << std::endl;
// 		return 1;
// 	}

// 	//clear server address
// 	bzero((char *) &serv_addr, sizeof(serv_addr));


// 	//set server address
// 	serv_addr.sin_family = AF_INET;
// 	serv_addr.sin_addr.s_addr = INADDR_ANY;
// 	serv_addr.sin_port = htons(portnb);

// 	//bind server address
// 	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
// 	{
// 		std::cout << "Binding Failed" << std::endl;
// 		return 1;
// 	}

// 	// sockfd = make_server_socket(portnb);
// 	// if (sockfd < 0)
// 	// {
// 	// 	std::cout << "Error openning server socket" << std::endl;
// 	// 	return 1;
// 	// }

// 	//start listening for 5 clients
// 	printf("listening for clients...\n");
// 	listen(sockfd, 5);
// 	client_len = sizeof(client_addr);

// 	//opening client socket
// 	client_fd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
// 	if (client_fd < 0)
// 	{
// 		std::cout << "Error accepting client" << std::endl;
// 		return 1;
// 	}

// 	printf("client connected\n");
// 	// open chat loop
// 	while (1)
// 	{
// 		bzero(buffer, BUFFER_SIZE);

// 		// read client message from socket
// 		n = read(client_fd, buffer, BUFFER_SIZE);
// 		if (n < 0)
// 		{
// 			std::cout << "Error reading" << std::endl;
// 			return 1;
// 		}
// 		printf("client : %s\n", buffer);

// 		bzero(buffer, BUFFER_SIZE);

// 		//write message to client
// 		fgets(buffer, BUFFER_SIZE, stdin);

// 		//write message in socket
// 		// send(client_fd, buffer, BUFFER_SIZE, 0);
// 		n = write(client_fd, buffer, strlen(buffer));
// 		if (n < 0)
// 		{
// 			std::cout << "Error writing" << std::endl;
// 			return 1;
// 		}

// 		if (strncmp("close", buffer, 5) == 0)
// 			break ;
// 	}

// 	close(client_fd);
// 	close(sockfd);
// 	return 0;
}