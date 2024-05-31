
#include "inc/poll.hpp"

void s_poll::init_poll(int server_socket)
{
	struct pollfd	tmp;
	set_to_nonblocking(server_socket);
    tmp.fd = server_socket;
    tmp.events = POLLIN;
    tmp.revents = 0;
	fds.push_back(tmp);
    count = 1;
}

void s_poll::add_to_poll(int new_fd)
{
	struct pollfd	tmp;
	set_to_nonblocking(new_fd);
    tmp.fd = new_fd;
    tmp.events = POLLIN;
    tmp.revents = 0;
	fds.push_back(tmp);
	count++;
}

void s_poll::remove_from_poll(int fd)
{
	for (int i = 0; i < count; ++i)
	{
		if (fds[i].fd == fd)
		{
			fds[i] = fds[count - 1];
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
	ipv4_saddr serv_addr;

	//opening server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		std::cout << "Error openning server socket :" << strerror(errno) << std::endl;
		return (-1);
	}

	//clear server address
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnb);

	int enable = 1;
	//set socket options
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)))
	{
		std::cout << "socket settings Failed :" << strerror(errno) << std::endl;
		return (-1);
	}
	//bind server address
	if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Binding Failed :" << strerror(errno) << std::endl;
		return (-1);
	}
	if (listen(server_socket, INIT_CLIENTS) == -1)
	{
		std::cout << "Error listening : " << strerror(errno) << std::endl;
		return (-1);
	}
	std::cout << "listening for clients..." << std::endl;
	return (server_socket);
}

int	s_poll::accept_new_connection(int server_socket)
{
	int client_fd;

	client_fd = accept(server_socket, 0, 0);
	if (client_fd < 0)
	{
		std::cout << "Error accepting client:" << strerror(errno) << std::endl;
		return (-1);
	}
	add_to_poll(client_fd);
	return (client_fd);
}

void	s_poll::set_to_nonblocking(int s_fd)
{
	int flags;
	
	flags = fcntl(s_fd, F_GETFD);
	flags |= O_NONBLOCK;
	fcntl(s_fd, F_SETFD, flags);
}

std::string	s_poll::read_data(int client_index)
{
	char buffer[BUFFER_SIZE];
	int bytes_read;
	int send_fd;

	send_fd = get_socket(client_index);
	bzero(&buffer, BUFFER_SIZE);
	bytes_read = recv(send_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read == 0)
	{
			std::cout << "receive error :" << strerror(errno) << std::endl;
	}
	else if (bytes_read < 0)
	{
		std::cout << "[" << client_index << "]<" << send_fd << "> client socket closed" << std::endl;
		return ("QUIT : client died");
	}
	else
	{
		return (std::string(buffer));
	}
	return ("");
}

void			s_poll::set_pollin(int index)
{
    fds[index].events = POLLIN;
    fds[index].revents = 0;
}

void			s_poll::set_pollout(int index)
{
    fds[index].events = POLLOUT;
    fds[index].revents = 0;
}

// ----------------------  Checker -----------------------


int			s_poll::check_pollin(int index)
{
	return (fds[index].revents & POLLIN);
}

int			s_poll::check_pollout(int index)
{
	return (fds[index].revents & POLLOUT);
}

int			s_poll::check_pollhup(int index)
{
	return (fds[index].revents & POLLHUP);
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
