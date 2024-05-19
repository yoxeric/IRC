
#include "inc/ft_irc.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	IRCServer	server;
	int 		server_socket;
	int			chosen_fd;

	server_socket = server.pool.make_server_socket(atoi(av[1]));
	if (server_socket < 0)
		return 1;
    server.init_server(av[2]);
    server.pool.init_poll(server_socket);
    while (true)
    {
        // Poll sockets to see if they are ready (2 second timeout)
    	chosen_fd = poll(server.pool.get_poll(), server.pool.get_count(), 2000);
    	if (chosen_fd == 0)
    	{
			std::cout << "waiting..." << std::endl;
			continue ;
    	}
    	else if (chosen_fd < 0)
    	{
			std::cout << "Poll Error" << strerror(errno) << std::endl;
			return 1;
		}
		// server.pool.print();
		if (server.pool.check_pollin(0))
		{
			// std::cout << "[" << i << "]<" << socket << "> new connection..." << std::endl;
			int client_socket = server.pool.accept_new_connection(server_socket);
			Client* client = server.add_client(client_socket);
			if (server.operator_count() == 0)
				client->set_mode("o");
			server.send_msg(client_socket, "connected\n");
			std::cout << client_socket << "> client connected" << std::endl;
		}
		else
		{
			for (int i = 1; i < server.pool.get_count(); i++)
			{
				if (server.pool.check_pollin(i))
				{
					if (server.clients[i-1].get_username().empty() == true)
					{
						std::cout << "msg 11111" << std::endl;
						std::string msg = server.pool.read_data(i);
						server.clients[i-1].set_username("username");
						std::cout << msg << std::endl;
					}
					else
					{
						std::cout << "msg 22222" << std::endl;
						std::string msg = server.pool.read_data(i);
						std::cout << msg << std::endl;
					}
				}
				// else if (server.pool.check_pollout(i))
				// {
				// 	// send data
				// }
				// else if (server.pool.check_pollhup(i))
				// {
				// 	// client disconnected
				// 	// send message to all channels this client is not here anymore abb
				// 	// remove the client from pollfd and delete his classs  && close socket fd 
				// }
			}
		}
		std::cout << "poll ended " << std::endl;
    }
	close(server_socket);
	return 0;
}
