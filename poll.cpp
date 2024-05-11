
#include "inc/poll.hpp"

void s_poll::init_poll(int server_socket)
{
	fds.reserve(INIT_CLIENTS);

	// Add the listening server socket to array
	// with notification when the socket can be read
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;
    count = 1;
}

void s_poll::add_to_poll(int new_fd)
{
	fds[count].fd = new_fd;
	fds[count].events = POLLIN;
	count++;
}

void s_poll::remove_from_poll(int fd)
{

	for (int i = 0; i < count; ++i)
	{
		if (fds[i].fd == fd)
		{
			fds[i] = fds[count - 1];
			// fds.erase(fds.begin() + index);
			// std::cout << i << "removed from pool" << std::endl;
			break ;
		}
	}
	
	count--;

	close(fd);
}


// ----------------------  Networking -----------------------


int	s_poll::make_server_socket(int portnb)
{
	int server_socket;
	struct sockaddr_in serv_addr;

	//opening server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
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
	if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Binding Failed :" << strerror(errno) << std::endl;
		return (-1);
	}

	return (server_socket);
}

int	s_poll::accept_new_connection(int server_socket)
{
	int client_fd;
	// socklen_t client_len = sizeof(client_addr);

	client_fd = accept(server_socket, 0, 0);
	if (client_fd < 0)
	{
		std::cout << "Error accepting client:" << strerror(errno) << std::endl;
		return (-1);
	}
	add_to_poll(client_fd);

	return (client_fd);
}

std::string	s_poll::read_data(int client_index)
{
	char buffer[BUFFER_SIZE];
	int bytes_read;
	int send_fd;

	send_fd = get_socket(client_index);
	memset(&buffer, '\0', sizeof(buffer));
	bytes_read = recv(send_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read <= 0)
		{
			std::cout << "[" << client_index << "]<" << send_fd << "> client socket closed" << std::endl;

			return ("QUIT : client died");

		}
		else
			std::cout << "receive error" << strerror(errno) << std::endl;

	}
	else
	{
		return (std::string(buffer));
	}
	return ("");
}

int			s_poll::check_pollin(int index)
{
	return (fds[index].revents & POLLIN);
}

int			s_poll::check_pollout(int index)
{
	return (fds[index].revents & POLLOUT);
}


// ----------------------  Getter -----------------------


struct pollfd*		s_poll::get_poll()
{
	return (&fds[0]);
}

int			s_poll::get_socket(int index)
{
	return (fds[index].fd);
}

int			s_poll::get_count()
{
	return (count);
}


// ----------------------  Debug -----------------------



void		s_poll::print()
{
	std::cout << count << std::endl;
	for (int i = 0; i < count; ++i)
	{
		std::cout << "[" << i << "] socket = " << fds[i].fd << std::endl;
	}
}
