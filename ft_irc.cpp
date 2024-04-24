
#include "ft_irc.hpp"


int	private_msg_channel(int sender_index, std::string channel, std::string msg, s_poll *pool)
{
	int send_fd = pool->fds[sender_index].fd;
	// check if anyone is in the channel
	for (int j = 1; j < pool->count; ++j)
	{
		s_client client = pool->clients[j];
		int dest_fd = pool->fds[j].fd;

		std::cout << " idx = "<< sender_index << " --> " << j << std::endl;
		std::cout << " fds = "<< send_fd << " --> " << dest_fd << std::endl;
		// std::cout << " chans nb :" << pool->channels.size() << std::endl;

		for (int i = 0; i < (int) client.chan.size(); i++)
		{
			std::cout << " ch : " << client.chan[i] << " <=?=> " << channel  << std::endl;
			if (client.chan[i] == channel && dest_fd != send_fd) 
			{
				std::cout << " word :" << msg << std::endl;
				send_msg(dest_fd, send_fd, msg);
			}
		}
	}
	return 0;
}

int	private_msg_user(int sender_index, std::string dest_nickname, std::string msg, s_poll *pool)
{
	int send_fd = pool->fds[sender_index].fd;
	// check if anyone is in the channel
	for (int j = 1; j < pool->count; ++j)
	{
		std::string name = pool->clients[j].nickname;
		int dest_fd = pool->fds[j].fd;

		std::cout << " idx = "<< sender_index << " --> " << j << std::endl;
		std::cout << " fds = "<< send_fd << " --> " << dest_fd << std::endl;
		std::cout << " usr : " << name << " <=?=> " << dest_nickname << std::endl;

		if (name == dest_nickname) 
		{
			std::cout << " word :" << msg << std::endl;
			send_msg(dest_fd, send_fd, msg);
		}
	}
	return 0;
}

int	send_msg(int dest_fd, int send_fd, std::string msg_to_send)
{
	std::ostringstream s;

	s << "[" << send_fd << "] client: " << msg_to_send ;
	if (send(dest_fd, s.str().c_str(), s.str().size(), 0) == -1)
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
		std::cout << "[" << send_fd << "] got message = \n\"" << buffer << "\"" << std::endl;

		msg = parse(i, buffer, pool);

		// send message to evrybody
		for (int j = 0; j < pool->count; ++j)
		{
			dest_fd = pool->fds[j].fd;
			// dont send message to server and sender
			if (dest_fd != sockfd && dest_fd != send_fd) 
			{
				// send_msg(dest_fd, send_fd, msg);
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
	std::cout << "server socket = " << sockfd << std::endl;

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
