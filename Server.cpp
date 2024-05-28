
#include "inc/Server.hpp"

std::string get_time()
{
	time_t rawtime;
  	time (&rawtime);
  	struct tm *timeinfo = localtime (&rawtime);

    char buffer [100];
  	strftime(buffer, sizeof(buffer), "%X %h %e %Y",timeinfo);
  	return (std::string(buffer));
}

// -----------------------------------  Server --------------------------------------

void	Server::init_server(char* pass)
{
	password = pass;
	networkname = std::string("Scale Factor Communication");
	servername = std::string("ScaleFactor.ma");
	datetime = get_time();
	version = std::string("1.0.1");
}

// ----------------------------------- Channel -------------------------------------------


Channel*	Server::add_channel(std::string name)
{
	Channel chan;
	chan.set_name(name);
	// chan.set_topic("hot topic");
	// chan.set_mode("");
	chan.set_limit(1024);

	channels.push_back(chan);
	return (&channels.back());
}

Channel*	Server::find_channel(std::string name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->get_name() == name)
		{
			return (&(*it));
		}
	}
	return (NULL);
}

void	Server::remove_channel(Channel &chan)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->get_name() == chan.get_name())
		{
			channels.erase(it);
			break ;
		}
	}
}


int			Server::channel_count()
{
	return (channels.size());
}


// ----------------------------------- Client -------------------------------------------



Client*	Server::add_client(int socket)
{
	Client client;
	client.set_socket(socket);
	client.set_registred(0);
	// client.set_mode("");

	clients.push_back(client);
	return (&clients.back());
}

Client*	Server::find_client(int socket)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->get_socket() == socket)
		{
			return (&(*it));
		}
	}
	return (NULL);
}

Client*	Server::find_client(std::string name)
{
	// check if anyone has the same username
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->get_nickname() == name)
		{
			return (&(*it));
		}
	}
	return (NULL);
}

void	Server::remove_client(Client &client)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->get_socket() == client.get_socket())
		{
			clients.erase(it);
			break ;
		}
	}
	pool.remove_from_poll(client.get_socket());
}

int			Server::client_count()
{
	return (clients.size());
}

int			Server::operator_count()
{
	int count = 0;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->is_mode('o'))
			count++;
	}
	return (count);
}


// ----------------------------------- Debug -------------------------------------------



void Server::print()
{
	std::cout << "networkname : " << networkname << std::endl;
	std::cout << "servername : " << servername << std::endl;
	std::cout << "datetime : " << datetime << std::endl;
	std::cout << "version : " << version << std::endl;
	std::cout << "------ all clients" << std::endl;
	int i = 0;

	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		std::cout << "---client nb." << i << std::endl;
		it->print();
		i++;
	}
}


// yes


