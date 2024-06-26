
#include "inc/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int server_socket;
	Server server;

	server_socket = server.pool.make_server_socket(atoi(av[1]));
	if (server_socket < 0)
		return 1;

    server.pool.init_poll(server_socket);
    server.init_server(av[2]);

    while (1)
    {
        // Poll sockets to see if they are ready (2 second timeout)
    	int ready_fds = poll(server.pool.get_poll(), server.pool.get_size(), -1);
		if (ready_fds == 0)
    	{
			std::cout << "waiting..." << std::endl;
			continue ;
    	}
    	else if (ready_fds < 0)
    	{
			std::cout << "Poll Error :" << strerror(errno) << std::endl;
			return 1;
		}
		// server.print();
		if (server.pool.check_pollin(0))
		{
			// std::cout << "new connection..." << std::endl;
			int client_socket = server.pool.accept_new_connection(server_socket);
			server.add_client(client_socket);
			server.send_msg(client_socket, "connected\n");
			// std::cout << "<" << client_socket << "> client connected" << std::endl;
		}
		else
		{
			for (int i = 1; i < server.pool.get_size(); i++)
			{
				int client_socket = server.pool.get_socket(i);
				if (server.pool.check_pollhup(i))
				{
					// std::cout << "[" << i << "]<" << client_socket << "> Disconnected" << std::endl;
					server.quit( *server.find_client(client_socket), "QUIT : client died");
				}
				else if (server.pool.check_pollin(i))
				{
					// std::cout << "[" << i << "]<" << client_socket << "> ready to receive msg..." << std::endl;
					std::string msg = server.pool.read_data(i);
					// std::cout << "got message = \n\"" << msg << "\"\n";
					server.split_input(i, msg);
				}
			}
		}
	}
	close(server_socket);
	return 0;
}
