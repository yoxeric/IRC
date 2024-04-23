
#include "ft_irc.hpp"

std::string getword(std::string str, int start)
{
	std::size_t i;
	std::string word;

	i = str.find(" ", start);
	if (i != std::string::npos)
	{
		word = str.substr(start, i - start);
		std::cout << start << " - " << i << " word :" << word << std::endl;
	}
	return (word);
}

std::string parse(int client_index, char *buffer, s_poll *pool)
{
	std::string str(buffer);
	std::size_t i;

	i = str.find("CAP ");
	if (i != std::string::npos)
	{
		std::string cap = str.substr(i + 4, str.length() - i - 1);

		pool->clients[client_index].cap(cap);
	}

	i = str.find("NICK ");
	if (i != std::string::npos)
	{
		std::string nick = str.substr(i + 5, str.length() - i - 1);

		pool->clients[client_index].nick(nick);
	}

	i = str.find("USER ");
	if (i != std::string::npos)
	{
		std::string str2 = str.substr(i + 5, str.length() - i - 1);
		std::cout << "parse usr :" << str2 << std::endl;

		std::size_t i;
		std::string user;
		std::string addr;

		user = getword(str, 0);

		i = str.find(" ", 0);
		user = getword(str, i + 1);

		i = str.find(" ", i + 1);
		addr = getword(str, i + 1);

		pool->clients[client_index].user(user, addr);
	}
	
	i = str.find("JOIN ");
	if (i != std::string::npos)
	{
		std::string str3 = str.substr(i + 5, str.length() - i - 1);
		std::cout << "parse join :" << str3 << std::endl;

		std::size_t i;
		std::string ch;

		i = str.find("#");
		ch = getword(str, i + 1);

		channels.push_back(str);
		pool->clients[client_index].join(ch);
	}

	pool->clients[i].print();

	std::string msg;

	i = str.find(":");
	// std::cout << "pos1 " << i<< std::endl;
	// std::cout << "pos2 "<< str.length() - i<< std::endl;
	if (i != std::string::npos)
	{
		msg = str.substr(i+ 1, str.length() - i - 1);
		return (msg);
	}
	else
	{
		msg = str;
		return (msg);
	}
}

int	send_msg(int dest_fd, int send_fd, std::string msg_to_send)
{
	std::ostringstream s;
	int status;

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

void	accept_new_connection(int sockfd, s_poll *pool)
{
	int client_fd;
	// socklen_t client_len = sizeof(client_addr);

	client_fd = accept(sockfd, 0, 0);
	if (client_fd < 0)
	{
		std::cout << "Error accepting client:" << strerror(errno) << std::endl;
		return ;
	}

	pool->add(client_fd);

	send_msg(client_fd, client_fd, std::string("connected\n"));
	std::cout << "[" << client_fd << "] client connected" << std::endl;
}


void	read_data(int i, int sockfd, s_poll *pool)
{
	char buffer[BUFFER_SIZE];
	std::string msg;
	int bytes_read;
	int send_fd;
	int dest_fd;

	send_fd = pool->fds[i].fd;
	memset(&buffer, '\0', sizeof(buffer));
	bytes_read = recv(send_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read <= 0)
			std::cout << "[" << send_fd << "] client socket closed" << std::endl;
		else
			std::cout << "receive error" << strerror(errno) << std::endl;
		close(send_fd);

		pool->remove(i);

	}
	else
	{
		std::cout << "[" << send_fd << "] got message = " << buffer << std::endl;

		msg = parse(i, buffer, pool);

		// send message to evrybody
		for (int i = 0; i < pool->count; ++i)
		{
			dest_fd = pool->fds[i].fd;
			// dont send message to server and sender
			if (dest_fd != sockfd && dest_fd != send_fd) 
			{
				send_msg(dest_fd, send_fd, msg);
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

	//get port
	portnb = atoi(av[1]);

	sockfd = make_server_socket(portnb);
	if (sockfd < 0)
	{
		std::cout << "server socket Error" << std::endl;
		return 1;
	}

	//start listening for 5 clients
	if (listen(sockfd, INIT_CLIENTS))
	{
		std::cout << "Error listening" << strerror(errno) << std::endl;
		return 1;
	}
	std::cout << "listening for clients..." << std::endl;

    s_poll pool(sockfd, INIT_CLIENTS);

    while (1)
    {
        // Poll sockets to see if they are ready (2 second timeout)
    	n = poll(&pool.fds[0], pool.count, 2000);
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
		for (int i = 0; i < pool.count; ++i)
		{
			if ((pool.fds[i].revents & POLLIN) != 1)
				continue ;
			std::cout << "[" << pool.fds[i].fd << "] ready for I/O operation" << std::endl;
			if (pool.fds[i].fd == sockfd)
			{
				std::cout << "[" << pool.fds[i].fd << "] new connection..." << std::endl;
				accept_new_connection(sockfd, &pool);
			}
			else
			{
				std::cout << "[" << pool.fds[i].fd << "] read_data..." << std::endl;
				read_data(i, sockfd, &pool);
			}
		}
    }
	close(sockfd);
	return 0;
}
