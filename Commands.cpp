
#include "inc/Server.hpp"

// ----------------------------------- Commands -------------------------------------------

void	Server::list(Client& client)
{

	std::cout << "---LISTING-----" << std::endl;

	send_reply(321, client, "Channel", "Users Name");


	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::stringstream ss;
		ss << "#"  << it->get_name() << " " << it->count_membres();
		send_reply(322, client, ss.str(), it->get_topic());
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

	std::cout << "-----PRVMSG-----" << std::endl;

	for (int i = 0; i < (int) target.size(); ++i)
	{
		// std::cout << "target = "  << target[i] << "." << std::endl;
		// std::cout << "type = "  << type[i] << "." << std::endl;
		if (type[i] == 2)
		{
			Channel* chan = find_channel(target[i]); 
			if (chan != NULL)
			{
				if (chan->is_membre(client))
				{
					// std::cout << "found = " << chan->get_name() << std::endl;

					s << ":" << create_tag(client) << " PRIVMSG #" << target[i] << " :" << msg << std::endl;

					send_msg_channel(client, *chan, s.str());

				}
				else
				{
					std::cout << "Error :" << target[i] << "  Cannot send to channel (no external message)" << std::endl;
					send_err(404, client, target[i], "Cannot send to channel");
					// :themis.sorcery.net 404 you #alice :Cannot send to channel (no external message)

				}
			}
			else
			{
				std::cout << "Error : " << target[i] << " channel not found" << std::endl;
				send_err(403, client, target[i], "No such channel");
			}

		}
		else if (type[i] == 1)
		{

			Client* dest_client = find_client(target[i]);
			if (dest_client != NULL)
			{
				// std::cout << "found = " << dest_client->get_nickname() << std::endl;

				s << ":" << create_tag(client) << " PRIVMSG " << dest_client->get_nickname() << " :" << msg << std::endl ;

				send_msg(dest_client->get_socket(), s.str());
			}
			else
			{
				std::cout << "Error :" << target[i] << "  client not found" << std::endl;
				send_err(401, client, target[i], "No such nick/channel");
			}
		}
	}

	// :Angel PRIVMSG Wiz :Hello are you receiving this message ?

  	// :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!

	// nickname!username@address.IP PRIVMSG #coolpeople :Hi everyone!
}

void Server::join(Client& client, std::string chan_name)
{
	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin());

	Channel* chan = find_channel(chan_name);

	if (chan != NULL)
	{
		if (chan->is_mode('i'))
		{
			if (chan->is_invited(client))
				chan->add_membre(client);
			else
			{
				std::cout << "Error :" << client.get_nickname() << " not invited" << std::endl;
				send_err(473, client, chan_name, "Cannot join channel (invite-only)");
				return ;
			}
		}
		else
			chan->add_membre(client);
	}
	else
	{
		chan = add_channel(chan_name);

		chan->add_membre(client);
		client.add_mode('o');
	}

	std::ostringstream s;

	s << ":" << create_tag(client) << " JOIN #" << chan_name << std::endl;

	send_msg(client.get_socket(), s.str());

	// nickname!username@address.IP JOIN #ch
}


void Server::who(Client& client, std::string target)
{
	std::stringstream ss;

	if (target.at(0) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target); 
		if (chan != NULL)
		{

			list_channel(client, *chan);

		}
		else
		{
			std::cout << "Error : " << target << " channel not found" << std::endl;
			send_err(403, client, target, "No such channel");
		}
	}
	else
	{

		Client* target_client = find_client(target);
		if (target_client != NULL)
		{

			list_user(client, *target_client);

		}
		else
		{
			std::cout << "Error : " << target << "  client not found" << std::endl;
			send_err(401, client, target, "No such nick/channel");
		}
	}
}


void Server::topic(Client& client, std::string target, std::string topic)
{
	(void)(client);

	if (target.at(1) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target); 
		if (chan != NULL)
		{
			chan->set_topic(topic);
		}
		else
		{
			std::cout << "Error : #" << target << " channel not found" << std::endl;
			send_err(403, client, target, "No such channel");
		}
	}
}


void Server::mode(Client& client, std::string target, std::string mode)
{
// . i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// · l: Set/remove the user limit to channel

	std::cout << "target = "  << target << "." << std::endl;
	std::cout << "mode = "  << mode << "." << std::endl;

	if (target.at(0) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target); 
		if (chan != NULL)
		{
			if (mode.at(0) == '+')
			{
				for (int i = 1; i < (int) mode.size(); ++i)
				{
					chan->add_mode(mode[i]);
				}
			}
			else if (mode.at(0) == '-')
			{
				for (int i = 1; i < (int) mode.size(); ++i)
				{
					chan->remove_mode(mode[i]);
				}
			}
			else
			{
				chan->set_mode(mode);
			}

			chan->print();
		}
		else
		{
			std::cout << "Error : #" << target << " channel not found" << std::endl;
			send_err(403, client, target, "No such channel");
		}
	}
	else
	{
		Client* cli = find_client(target);
		if (cli != NULL)
		{
			if (mode.at(0) == '+')
			{
				for (int i = 1; i < (int) target.size(); ++i)
				{
					cli->add_mode(mode[i]);
				}
			}
			else if (mode.at(0) == '-')
			{
				for (int i = 1; i < (int) target.size(); ++i)
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
			send_err(401, client, target, "No such nick/channel");
		}
	}
}

void 	Server::kick(Client &client, std::string chan_name, std::string target)
{
	std::stringstream s;

	if (chan_name.at(0) == '#')
	{
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(chan_name); 
		if (chan != NULL)
		{
			Client* baduser = find_client(target);
			if (baduser != NULL)
			{
				chan->remove_membre(*baduser);

				s << ":" << create_tag(client) << " KICK #" << chan->get_name() << " " << baduser->get_nickname() << std::endl ;// << " " << msg << std::endl ;

				send_msg(baduser->get_socket(), s.str());

			}
			else
			{
				std::cout << "Error : " << target << "  client not found" << std::endl;
				send_err(401, client, target, "No such nick/channel");
			}
		}
		else
		{
			std::cout << "Error : " << chan_name << " channel not found" << std::endl;
			send_err(403, client, chan_name, "No such channel");
		}
	}

	 // :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
}

void 	Server::invite(Client &client, std::string chan_name, std::string target)
{
	std::stringstream s;

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )

	Channel* chan = find_channel(chan_name); 
	if (chan != NULL)
	{
		Client* user = find_client(target);
		if (user != NULL)
		{
			chan->add_invited(*user);

			s << ":" << create_tag(client) << " INIVTE " << target << " #" << chan_name << std::endl;

			send_msg(user->get_socket(), s.str());
		}
		else
		{
			std::cout << "Error : " << target << "  client not found" << std::endl;
			send_err(401, client, target, "No such nick/channel");
		}
	}
	else
	{
		std::cout << "Error : " << chan_name << " channel not found" << std::endl;
		send_err(403, client, chan_name, "No such channel");
	}



	// :dan-!d@localhost INVITE Wiz #test

	// :Angel!wings@irc.org INVITE Wiz #Dust

}

void 	Server::ping(Client &client, std::string msg)
{
	std::stringstream s;

	s << ":" << create_tag(client) << " PONG :" << msg << std::endl;
		
	send_msg(client.get_socket(), s.str());

}

void 	Server::part(Client &client, std::string chan_name, std::string msg)
{
	std::stringstream s;

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )


	Channel* chan = find_channel(chan_name); 
	if (chan != NULL)
	{
		chan->remove_membre(client);
		
		s << ":" << create_tag(client) << " PART #" << chan_name << " :" << msg << std::endl;
		
		send_msg(client.get_socket(), s.str());
	}
	else
	{
		std::cout << "Error : " << chan_name << " channel not found" << std::endl;
		send_err(403, client, chan_name, "No such channel");
	}
}

void 	Server::quit(Client &sender, std::string msg)
{
	std::stringstream s;

	s << ":" << create_tag(sender) << " QUIT :" << msg << std::endl ;

	// for (int i = 0; i < (int) channels.size(); ++i)
	// {
	// 	if (channels[i]->is_membre(client))
	// 			{
	// send_msg(sender.get_socket(), s.str());

	remove_client(sender);

	// :dan-!d@localhost QUIT :Quit: Bye for now!
}