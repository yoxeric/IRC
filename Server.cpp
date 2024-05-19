
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

void	IRCServer::init_server(char* pass)
{
	password = pass;
	networkname = std::string("Scale Factor Communication");
	servername = std::string("ScaleFactor.ma");
	datetime = get_time();
	version = std::string("1.0.1");
}

// ----------------------------------- Channel -------------------------------------------


Channel*	IRCServer::add_channel(std::string name)
{
	Channel chan;
	chan.set_name(name);
	// chan.set_topic("hot topic");
	// chan.set_mode("");
	chan.set_limit(1024);

	channels.push_back(chan);
	return (&channels.back());
}

Channel*	IRCServer::find_channel(std::string name)
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

void	IRCServer::remove_channel(Channel &chan)
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


int			IRCServer::channel_count()
{
	return (channels.size());
}


// ----------------------------------- Client -------------------------------------------



Client*	IRCServer::add_client(int socket)
{
	Client client;
	client.set_socket(socket);
	client.set_realname("realname");
	// client.set_mode("");

	clients.push_back(client);
	return (&clients.back());
}

Client*	IRCServer::find_client(int socket)
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

Client*	IRCServer::find_client(std::string name)
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


void	IRCServer::remove_client(Client &client)
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

int			IRCServer::client_count()
{
	return (clients.size());
}

int			IRCServer::operator_count()
{
	int count = 0;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->is_mode('o'))
			count++;
	}
	return (count);
}


// ----------------------------------------- Messages -------------------------------------



int	IRCServer::send_msg_channel(Client& sender, Channel &chan, std::string msg)
{
	// send msg to all user in the channel
	std::cout << "sending to membres ..." << std::endl;
	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		std::cout << " mbr = "<< it->get_nickname() << std::endl;
		if (sender.get_socket() != it->get_socket())
			send_msg(it->get_socket(), msg);
	}
	return 0;
}

int	IRCServer::send_msg(int dest_fd, std::string msg)
{
	std::cout << "msg >> "<< msg;
	if (send(dest_fd, msg.c_str(), msg.size(), 0) == -1)
	{
		std::cout << "Send error to client [" << dest_fd << "]";
		return 1;
	}
	return 0;
}

std::string	IRCServer::create_tag(Client& client)
{
	std::stringstream s;

	s << client.get_nickname() << "!" << client.get_username() << "@" << client.get_address() << ".IP";

	return s.str();
}

void	IRCServer::send_reply(int code, Client &client, std::string arg, std::string msg)
{
	std::stringstream s;

	if (arg.empty())
		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code 
	<< " " << client.get_nickname() << " :" << msg << std::endl;
	else if (msg.empty())
		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code 
	<< " " << client.get_nickname() << " " << arg << std::endl;
	else
		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code 
	<< " " << client.get_nickname() << " " << arg << " :" << msg << std::endl;

	send_msg(client.get_socket(), s.str());
}



// ----------------------------------- Debug -------------------------------------------



void IRCServer::print()
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


