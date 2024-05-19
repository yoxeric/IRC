
#include "inc/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int server_socket, portnb;
	int n;

	//get port
	portnb = atoi(av[1]);

	Server server;

	// server.clients.reserve(INIT_CLIENTS);
	// server.channels.reserve(INIT_CLIENTS);

	server_socket = server.pool.make_server_socket(portnb);
	if (server_socket < 0)
		return 1;

    server.pool.init_poll(server_socket);

    server.init_server();

	//start listening
	if (listen(server_socket, INIT_CLIENTS))
	{
		std::cout << "Error listening : " << strerror(errno) << std::endl;
		return 1;
	}
	std::cout << "listening for clients..." << std::endl;

    while (1)
    {
        // Poll sockets to see if they are ready (2 second timeout)
    	n = poll(server.pool.get_poll(), server.pool.get_count(), 2000);
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

		// server.pool.print();

		// loop over polls
		for (int i = 0; i < server.pool.get_count(); i++)
		{
			if (server.pool.check_pollin(i) != 1)
				continue ;

			// std::cout << "[" << i << "]<" << socket << "> ready for I/O operation" << std::endl;
			int socket = server.pool.get_socket(i);

			if (socket == server_socket)
			{
				// std::cout << "[" << i << "]<" << socket << "> new connection..." << std::endl;
				int client_socket = server.pool.accept_new_connection(server_socket);

				Client* client = server.add_client(client_socket);
				if (server.operator_count() == 0)
					client->set_mode("o");

				server.send_msg(client_socket, "connected\n");
				std::cout << "[" << i << "]<" << client_socket << "> client connected" << std::endl;

			}
			else
			{
				// std::cout << "[" << i << "]<" << socket << "> read_data..." << std::endl;
				std::string msg = server.pool.read_data(i);
				std::istringstream input(msg);
				getline(input, msg, '\n');
				/*for (int i=0; msg[i]; i++)
					std::cout << "msg =" << msg[i] << "={" << (int)msg[i] << "}" << std::endl;*/
				while (!msg.empty())
				{
					std::cout << "[" << i << "].<" << socket << "> got message = \n\"" << msg << "\"" << std::endl;
					std::string error = server.parse(socket, msg);
					if (error[0])
						std::cout << error << std::endl;
					getline(input, msg, '\n');
					// server.list_clients();
				}
				
			}
		}
		// std::cout << "poll ended " << std::endl;
    }
	close(server_socket);
	return 0;
}
