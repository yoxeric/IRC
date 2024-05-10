
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

void	Server::init_server()
{
	networkname = std::string("Scale Factor Communication");
	servername = std::string("ScaleFactor.ma");
	datetime = get_time();
	version = std::string("1.0.1");
}

void	Server::welcome_server(Client& client)
{

	std::stringstream ss1;
	ss1 << "Welcome to the " << networkname << " IRC Network " <<  create_tag(client);
	send_reply(001, client, "", ss1.str());


	std::stringstream ss2;
	ss2 << "Your host is " << servername << " running version " << version;
	send_reply(002, client, "", ss2.str());


	std::stringstream ss3;
	ss3 << "This server was created " << datetime;
	send_reply(003, client, "", ss3.str());

	list_users(client);
	modt_server(client);

}

void	Server::modt_server(Client& client)
{

	send_reply(375, client, "", "- ScaleFactor.ma Message of the Day - ");

	// send_reply(372, client, "", "<!--          .-.           .-             .-.                         -->");
	// send_reply(372, client, "", "<!--    .--.-'             /              / -'           /             -->");
	// send_reply(372, client, "", "<!--   (  (_).-.  .-.     /   .-.     . -/--.-.  .-. ---/---.-._.).--. -->");
	// send_reply(372, client, "", "<!--    `-. (    (  |    /  ./.-'_   /  /  (  | (      /   (   )/      -->");
	// send_reply(372, client, "", "<!--  _    ) `---'`-'-'_/_.-(__.'    `.'    `-'-'`---'/     `-'/       -->");
	// send_reply(372, client, "", "<!-- (_.--'                                                            -->");

	// send_reply(372, client, "", "<!--              # #                      .-                                    .-   -->");
	// send_reply(372, client, "", "<!--    .--.      .-.    /      /         /  /        /           /             /  /  -->");
	// send_reply(372, client, "", "<!--   (    '    (  )   /      /.-.  #   /  /    .   /    .-.    /    .--.     /  /   -->");
	// send_reply(372, client, "", "<!--    `-.  (     /   /     /    ) /   /  /    /   /    (   )  /    \\  /    /  /    -->");
	// send_reply(372, client, "", "<!--       )  `---'   ' ----'`---- '---'  /    `._ /  ---'`--' / ___ _\\/ _ -'  /     -->");
	// send_reply(372, client, "", "<!-- (_.--'                                                                           -->");

	send_reply(372, client, "", "<!--          # #                      .-                                  -->");
	send_reply(372, client, "", "<!--          .-.    /      /         /  .-         /           /          -->");
	send_reply(372, client, "", "<!--         (  )   /      /.-.  #   /  /      .   /    .-.    /           -->");
	send_reply(372, client, "", "<!--   .       /   /     /    ) /   /  /      /   /    (   )  /       .-.  -->");
	send_reply(372, client, "", "<!--  (-------'   ' ----'`---- '---'  /      `._ /  ---'`--' / ___ __(___  -->");
	send_reply(372, client, "", "<!--                                 /                                     -->");
	send_reply(372, client, "", "- ");
	send_reply(372, client, "", "-  Welcome to the ScaleFactor.ma server of the Scale Factor IRC network ");
	send_reply(372, client, "", "-  ");
	send_reply(372, client, "", "-  The server is designed for quiet communication.");
	send_reply(372, client, "", "-  other use not according to the intended purpose will not be tolerated   ");
	send_reply(372, client, "", "-  You cannot change the network rules with any command !   ");
	send_reply(372, client, "", "-  ");

	send_reply(376, client, "", "End of /MOTD command.");

	// list(client);

}

void	Server::list_users(Client& client)
{

	std::stringstream ss1;
	ss1 << "There are " << client_count() << " users on " << 1 << " servers " << datetime;
	send_reply(251, client, "", ss1.str());

	std::stringstream ss2;
	ss2 << operator_count();
	send_reply(252, client, ss2.str(), "operator(s) online");

	std::stringstream ss3;
	ss3 << channel_count();
	send_reply(254, client, ss3.str(), "channels formed");

	std::stringstream ss4;
	ss4 << "I have " << client_count() << " clients and " << 1 << " servers " << datetime;
	send_reply(255, client, "", ss4.str());

}

void	Server::list(Client& client)
{

	std::cout << "---LISTING-----" << std::endl;

	send_reply(321, client, "Channel", "Users Name");


	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::stringstream ss2;
		ss2 << "#"  << it->get_name() << " " << it->count_membres();
		send_reply(322, client, ss2.str(), it->get_topic());
	}
	
	send_reply(323, client, "", "End of /LIST");

}


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


	welcome_server(client);
}

void Server::prvmsg(Client& client, std::vector<std::string> target, std::vector<int> type, std::string msg)
{
	std::ostringstream s;

	std::cout << "---PRVMSG-----" << std::endl;

	for (int i = 0; i < (int) target.size(); ++i)
	{
		/* code */
		std::cout << "target = "  << target[i] << "." << std::endl;
		std::cout << "type = "  << type[i] << "." << std::endl;
		if (type[i] == 2)
		{

			// target[i].erase(target[i].begin()); // delete first charcater  ( # )
			Channel* chan = find_channel(target[i]); 
			std::cout << "chan = " << chan << std::endl;

			if (chan != nullptr)
			{

				std::cout << "found = " << chan->get_name() << std::endl;

				s << ":" << create_tag(client) << " PRIVMSG #" << target[i] << " :" << msg << std::endl;

				send_msg_channel(client, *chan, s.str());

			}
			else
			{
				std::cout << "Error : " << target[i] << " channel not found" << std::endl;
			}

		}
		else if (type[i] == 1)
		{

			Client* dest_client = find_client(target[i]);
			std::cout << "cl = " << dest_client << std::endl;

			if (dest_client != nullptr)
			{
				std::cout << "found = " << dest_client->get_nickname() << std::endl;

				// std::cout << "privmsg usr = " << target << std::endl;

				s << ":" << create_tag(client) << " PRIVMSG " << dest_client->get_nickname() << " :" << msg << std::endl ;

				send_msg(dest_client->get_socket(), s.str());
			}
			else
			{
				std::cout << "Error :" << target[i] << "  client not found" << std::endl;
			}
		}
	}

	// std::cout << "Message >> " << s.str() << std::endl ;
	// send_msg(client.get_socket(), s.str());

	// std::cout << "done" << std::endl;

	// :Angel PRIVMSG Wiz :Hello are you receiving this message ?

  	// :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!

	// nickname!username@address.IP PRIVMSG #coolpeople :Hi everyone!
}


void Server::join(Client& client, std::string chan_name)
{

	std::cout << "---JOIN-----" << std::endl;

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin());

	Channel* chan = find_channel(chan_name);

	if (chan != nullptr)
	{
		chan->add_membre(client);
	}
	else
	{
		chan = add_channel(chan_name);

		chan->add_membre(client);
	}

	std::ostringstream s;

	s << ":" << create_tag(client) << " JOIN #" << chan_name << std::endl;

	send_msg(client.get_socket(), s.str());


	// :yh1!~yh2@60CCA6D2.A5193C4D.9C9BAFEA.IP JOIN :#kosmosas
	// nickname!username@address.IP JOIN #ch
}


void Server::who(Client& client, std::string target)
{
	std::ostringstream s;

	if (target.at(0) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target); 
		if (chan != nullptr)
		{

			std::stringstream ss1;
			ss1 << "#" << chan->get_name();
			send_reply(322, client, ss1.str(), chan->get_topic());

			for (std::vector<Client>::iterator it = chan->members.begin(); it != chan->members.end(); it++)
			{
				std::stringstream ss1;
				ss1 << "~" << it->get_username() << " " << it->get_address() << " " << servername
				<< " " << it->get_nickname() << " " <<  it->get_mode();
				send_reply(352, client, ss1.str(), it->get_realname());
			}
			
			send_reply(323, client, "", "End of /WHO list");

		}
		else
		{
			std::cout << "Error : " << target << " channel not found" << std::endl;
		}
	}
	else
	{

		Client* target_client = find_client(target);
		if (target_client != nullptr)
		{

			std::ostringstream ss2;

			ss2 << "~" << target_client->get_username() << " " << target_client->get_address() << " " << servername
			<< " " << target_client->get_nickname() << " " <<  target_client->get_mode();
			send_reply(352, client, s.str(), target_client->get_realname());

			send_reply(323, client, "", "End of /WHO list");

		}
		else
		{
			std::cout << "Error : " << target << "  client not found" << std::endl;
		}
	}
}

void Server::mode(Client& client, std::string target, std::string mode)
{
	(void)(client);
	if (target.at(0) == '#')
	{
		Channel* chan = find_channel(target); 
		if (chan != nullptr)
		{
			if (mode.at(0) == '+')
			{
				for (int i = 0; i < (int) target.size(); ++i)
				{
					chan->add_mode(mode[i]);
				}
			}
			else if (mode.at(0) == '-')
			{
				for (int i = 0; i < (int) target.size(); ++i)
				{
					chan->remove_mode(mode[i]);
				}
			}
			else
			{
				chan->set_mode(mode);
			}
		}
		else
		{
			std::cout << "Error : " << target << " channel not found" << std::endl;
		}
	}
	else
	{
		Client* cli = find_client(target);
		if (cli != nullptr)
		{
			if (mode.at(0) == '+')
			{
				for (int i = 0; i < (int) target.size(); ++i)
				{
					cli->add_mode(mode[i]);
				}
			}
			else if (mode.at(0) == '-')
			{
				for (int i = 0; i < (int) target.size(); ++i)
				{
					cli->remove_mode(mode[i]);
				}
			}
			else
			{
				cli->set_mode(mode);
			}
		}
		else
		{
			std::cout << "Error : " << target << "  client not found" << std::endl;
		}
	}
}


// void Server::topic(Client& client, std::string target, s std::string topic)
// {

// 	if (target.at(1) == '#')
// 	{
// 		Channel chan = find_client(target);
// 		if (client.is_ope)
// 		{
// 			for (int i = 0; i < target.size(); ++i)
// 			{
// 				chan.add_mode(mode[i]);
// 			}
// 		}
// 		else if (mode.at(0) == '-')
// 		{
// 			for (int i = 0; i < target.size(); ++i)
// 			{
// 				chan.remove_mode(mode[i]);
// 			}
// 		}
// 		else
// 		{
// 			chan.set_mode(mode);
// 		}
// 	}
// }

// ------------------------------------------------------------------------------



int	Server::send_msg_channel(Client& sender, Channel &chan, std::string msg)
{
	// send msg to all user in the channel
	std::cout << "sending to membres ..."<< std::endl;
	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		std::cout << " mbr = "<< it->get_nickname() << std::endl;
		// std::cout << " chans test :" << pool->clients[j].is_membre(channel) << std::endl;
		if (sender.get_socket() != it->get_socket())
			send_msg(it->get_socket(), msg);
	}
	return 0;
}

// int	Server::send_msg_client(Client &client, std::string msg)
// {
// 	send_msg(client.get_socket(), msg);
// 	return 0;
// }

int	Server::send_msg(int dest_fd, std::string msg)
{
	std::cout << "msg >> "<< msg;
	if (send(dest_fd, msg.c_str(), msg.size(), 0) == -1)
	{
		std::cout << "Send error to client [" << dest_fd << "]";
		return 1;
	}
	return 0;
}


std::string	Server::create_tag(Client& client)
{
	std::stringstream s;

	s << client.get_nickname() << "!" << client.get_username() << "@" << client.get_address() << ".IP";

	return s.str();
}

// std::string	Server::create_reply(int code, std::string nick, std::string arg, std::string msg)
// {
// 	std::stringstream s;

// 	if (arg.empty())
// 		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code << " " << nick << " :" << msg << "\n";
// 	else
// 		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code << " " << nick << " " << arg << " :" << msg << "\n";

// 	return s.str();
// }

void	Server::send_reply(int code, Client &client, std::string arg, std::string msg)
{
	std::stringstream s;

	if (arg.empty())
		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code 
	<< " " << client.get_nickname() << " :" << msg << std::endl;
	else
		s << ":" << servername << " " << std::setw(3) << std::setfill('0') << code 
	<< " " << client.get_nickname() << " " << arg << " :" << msg << std::endl;

	send_msg(client.get_socket(), s.str());
}


// ------------------------------------------------------------------------------



Client*	Server::add_client(int socket)
{
	Client client;
	client.set_socket(socket);
	client.set_realname("realname");
	client.set_mode("M");

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
	return (nullptr);
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
	return (nullptr);
}

Channel*	Server::add_channel(std::string name)
{
	Channel chan;
	chan.set_name(name);
	chan.set_topic("hot topic");
	chan.set_mode("M");
	
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
	return (nullptr);
}

int			Server::client_count()
{
	int count = 0;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		count++;
	}
	return (count);
}

int			Server::operator_count()
{
	int count = 0;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->get_operator())
			count++;
	}
	return (count);
}

int			Server::channel_count()
{
	int count = 0;
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		count++;
	}
	return (count);
}


// ------------------------------------------------------------------------------


void Server::print_clients()
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

void Server::print()
{
	std::cout << "networkname : " << networkname << std::endl;
	std::cout << "servername : " << servername << std::endl;
	std::cout << "datetime : " << datetime << std::endl;
	std::cout << "version : " << version << std::endl;
}



// yes


