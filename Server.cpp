

#include "inc/Server.hpp"


void Server::cap(Client& client, std::string str)
{
	(void)(client);
	(void)(str);
}

void Server::nick(Client& client, std::string str)
{
	// std::cout << "nickname = " << str << std::endl;
	client.set_nickname(str);
}

void Server::user(Client& client, std::string user, std::string addr)
{

	client.set_username(user);
	client.set_address(addr);

}

void Server::prvmsg(Client& client, std::string target, std::string msg)
{
	std::ostringstream s;


	if (target.at(0) == '#')
	{

		Channel chan = find_channel(target); // todo: remove #

		s << ":" << client.get_nickname() << "!" << client.get_username() << "@" << client.get_address() 
		<< ".IP PRIVMSG #" << target << " :" << msg << std::endl ;

		send_msg_channel(chan, msg);

	}
	else
	{

		Client dest_client = find_client(target);

		// std::cout << "privmsg usr = " << target << std::endl;

		s << ":" << client.get_nickname() << "!" << client.get_username() << "@" << client.get_address() 
		<< ".IP PRIVMSG " << dest_client.get_nickname() << " :" << msg << std::endl ;

		send_msg_client(client, s.str());


	}

	// std::cout << "Message >> " << s.str() << std::endl ;
	// send_msg(client.get_socket(), s.str());



	// :Angel PRIVMSG Wiz :Hello are you receiving this message ?

  	// :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!

	// nickname!username@address.IP PRIVMSG #coolpeople :Hi everyone!
}


void Server::join(Client& client, std::string chan_name)
{
	//if (do_channel_exist(chan_name))
	if (1)
	{
		Channel chan = add_channel(chan_name);

		chan.add_membre(client);
		
	}
	else
	{
		Channel chan = find_channel(chan_name);

		chan.add_membre(client);

	}

	std::ostringstream s;

	s << ":" << client.get_nickname() << "!" << client.get_username() << "@" << client.get_address() 
	<< ".IP JOIN #" << chan_name << std::endl ;
	send_msg(client.get_socket(), s.str());

	
	// :yh1!~yh2@60CCA6D2.A5193C4D.9C9BAFEA.IP JOIN :#kosmosas
	// nickname!username@address.IP JOIN #ch
}



// ------------------------------------------------------------------------------



int	Server::send_msg_channel(Channel &chan, std::string msg)
{
	// send msg to all user in the channel
	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		// std::cout << " idx = "<< sender_index << " --> " << j << std::endl;
		// std::cout << " chans test :" << pool->clients[j].is_membre(channel) << std::endl;
		send_msg_client(*it, msg);
	}
	return 0;
}

int	Server::send_msg_client(Client &client, std::string msg)
{
	send_msg(client.get_socket(), msg);
	return 0;
}

int	Server::send_msg(int dest_fd, std::string msg)
{
	std::cout << "msg >> "<< msg << std::endl;
	if (send(dest_fd, msg.c_str(), msg.size(), 0) == -1)
	{
		std::cout << "Send error to client [" << dest_fd << "]";
		return 1;
	}
	return 0;
}


std::string	Server::creat_reply(int code, std::string nick, std::string arg, std::string msg)
{
	std::ostringstream s;

	s << ":" << servername << " " << code << " " << nick << " " << arg << " :" << msg << std::endl ;
	return s.str();
}



// ------------------------------------------------------------------------------



Client&	Server::add_client(int socket)
{
	Client client;
	client.set_socket(socket);

	clients.push_back(client);
	return (*clients.rbegin());
}

Client& Server::find_client(int socket)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->get_socket() == socket)
		{
			return (*it);
		}
	}
	return (*clients.end());
}

Client& Server::find_client(std::string name)
{
	// check if anyone has the same username
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->get_nickname() == name)
		{
			return (*it);
		}
	}
	return (*clients.end());
}

Channel&	Server::add_channel(std::string name)
{
	Channel chan;
	chan.set_name(name);
	
	channels.push_back(chan);
	return (*channels.rbegin());
}

Channel& Server::find_channel(std::string name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->get_name() == name)
		{
			return (*it);
		}
	}
	return (*channels.end());
}


// ------------------------------------------------------------------------------


void Server::list_clients()
{

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


