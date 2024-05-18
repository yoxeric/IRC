
#include "inc/Server.hpp"

// ----------------------------------- Commands -------------------------------------------

void	Server::list(Client& sender)
{

	std::cout << "---LISTING-----" << std::endl;

	send_reply(321, sender, "Channel", "Users Name");


	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::stringstream ss;
		ss << "#"  << it->get_name() << " " << it->count_membres();
		send_reply(322, sender, ss.str(), it->get_topic());
	}
	
	send_reply(323, sender, "", "End of /LIST");

}

void Server::who(Client& sender, std::string target)
{
	std::stringstream ss;

	if (target.at(0) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target); 
		if (chan == NULL)
		{
			send_err(403, sender, target, "No such channel");
			return ;
		}

		list_channel(sender, *chan);
	}
	else
	{
		Client* target_client = find_client(target);
		if (target_client == NULL)
		{
			send_err(401, sender, target, "No such nick/channel");
			return ;
		}

		list_user(sender, *target_client);
	}
}


void Server::cap(Client& sender, std::string str)
{
	(void)(sender);
	(void)(str);
}

// todo : PASS cmd
// todo : OPER cmd

void Server::nick(Client& sender, std::string str)
{
	// std::cout << "nickname = " << str << std::endl;
	if (str.empty())
	{
		send_err(431, sender, "No nickname given");
		return ;
	}
	Client* target_client = find_client(str);
	if (target_client != NULL && target_client->get_socket() != sender.get_socket())
	{
		send_err(431, sender, str, "Nickname is already in use");
		return ;
	}

	sender.set_nickname(str);
}

void Server::user(Client& sender, std::string user, std::string addr)
{

	// todo: "<client> :You may not reregister"   (after first ime)

	sender.set_username(user);
	sender.set_address(addr);

	welcome_server(sender);
}

void Server::prvmsg(Client& sender, std::vector<std::string> target, std::vector<int> type, std::string msg)
{
	std::ostringstream s;

	// std::cout << "-----PRVMSG-----" << std::endl;

	for (int i = 0; i < (int) target.size(); ++i)
	{
		// std::cout << "target = "  << target[i] << "." << std::endl;
		// std::cout << "type = "  << type[i] << "." << std::endl;
		if (type[i] == 2)
		{
			Channel* chan = find_channel(target[i]); 
			if (chan == NULL)
			{
				send_err(403, sender, target[i], "No such channel");
				return ;
			}
			if (!chan->is_membre(sender.get_nickname()))
			{
				send_err(404, sender, target[i], "Cannot send to channel (no external message)");
				return ;
			}
			
			// std::cout << "found = " << chan->get_name() << std::endl;

			s << ":" << create_tag(sender) << " PRIVMSG #" << target[i] << " :" << msg;// << std::endl;

			send_msg_channel(sender, *chan, s.str());

		}
		else if (type[i] == 1)
		{

			Client* target_client = find_client(target[i]);
			if (target_client == NULL)
			{
				send_err(401, sender, target[i], "No such nick/channel");
				return ;
			}
			
			// std::cout << "found = " << target_client->get_nickname() << std::endl;

			s << ":" << create_tag(sender) << " PRIVMSG " << target[i] << " :" << msg;// << std::endl;

			send_msg(target_client->get_socket(), s.str());
		}
	}

	// :Angel PRIVMSG Wiz :Hello are you receiving this message ?

  	// :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!

	// nickname!username@address.IP PRIVMSG #coolpeople :Hi everyone!
}

void Server::join(Client& sender, std::string chan_name, std::string key)
{
	(void)(key);
	

  // todo : (461) "<client> <command> :Not enough parameters"

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin());

	Channel* chan = find_channel(chan_name);
	if (chan != NULL)
	{
		if (chan->is_mode('i') && !chan->is_invited(sender.get_nickname()))
		{
			send_err(473, sender, chan_name, "Cannot join channel (invite-only)");
			return ;
		}
		if (chan->is_mode('l') && chan->count_membres() >= chan->get_limit())
		{
			send_err(471, sender, chan_name, "Cannot join channel (users limit reached)");
			return ;
		}
		if (chan->is_mode('k'))
		{
			if (key.empty())
			{
				send_err(475, sender, chan_name, "Cannot join channel (key not supplied)");
				return ;
			}
			else if (chan->get_key() != key)
			{
				send_err(475, sender, chan_name, "Cannot join channel (incorrect key)");
				return ;
			}
		}
	}
	else
	{
		chan = add_channel(chan_name);
	}

	chan->add_membre(sender);
	if (chan->is_mode('i'))
		chan->remove_invited(sender.get_nickname());
	if (chan->count_operators() == 0)
		chan->add_membre_mode(sender, 'o');

	std::ostringstream s;

	s << ":" << create_tag(sender) << " JOIN #" << chan_name << std::endl;

	send_msg(sender.get_socket(), s.str());

	// nickname!username@address.IP JOIN #ch
}

void 	Server::part(Client &sender, std::string chan_name, std::string msg)
{
	std::stringstream s;

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )

	Channel* chan = find_channel(chan_name); 
	if (chan == NULL)
	{
		send_err(403, sender, chan_name, "No such channel");
		return ;
	}
	if (!chan->is_membre(sender.get_nickname()))
	{
		send_err(442, sender, chan_name, "You're not on that channel");
		return ;
	}


	chan->remove_membre(sender);
	if (chan->count_membres() == 0)
		remove_channel(*chan);
	
	s << ":" << create_tag(sender) << " PART #" << chan_name << " :" << msg << std::endl;
	
	send_msg(sender.get_socket(), s.str());
}

void Server::mode(Client& sender, std::string target, std::string mode, std::string arg)
{
// . i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// · l: Set/remove the user limit to channel

	std::cout << "target = "  << target << "." << std::endl;
	std::cout << "mode = "  << mode << "." << std::endl;
	std::cout << "arg = "  << arg << "." << std::endl;


	(void)(arg);

	if (target.at(0) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target);
		if (chan == NULL)
		{
			std::cout << "Error : #" << target << " channel not found" << std::endl;
			send_err(403, sender, target, "No such channel");
			return ;
		}
		if (mode.empty())
		{
			std::stringstream ss;
			ss << "+"  << chan->get_mode();
			std::stringstream ch;
			ch << "#"  << target;

			send_reply(324, sender, ch.str(), ss.str());

			// :irc.example.com 324 dan #foobar +nrt
	  		// :irc.example.com 329 dan #foobar 1620807422
		}
		else if (mode.at(0) == '+')
		{
			for (int i = 1; i < (int) mode.size(); ++i)
			{
				if (mode[i] == 'o')
				{
					if (!arg.empty())
					{
						Client* target_client = find_client(arg);
						if (target_client != NULL)
							chan->add_membre_mode(*target_client, 'o');
					}
					else
					{
						std::cout << "Error : +o mode missing argument" << std::endl;
						// send_err(403, client, target, "No such channel");
					}
				}
				else if (mode[i] == 'l')
				{
					chan->set_limit(std::stoi(arg));
					chan->add_mode('l');
				}
				else if (mode[i] == 'k')
				{
					chan->set_key(arg);
					chan->add_mode('k');
				}
				else
				{
					chan->add_mode(mode[i]);
				}
			}
		}
		else if (mode.at(0) == '-')
		{
			for (int i = 1; i < (int) mode.size(); ++i)
			{
				if (mode[i] == 'o')
				{
					if (!arg.empty())
					{
						Client* target_client = find_client(arg);
						if (target_client != NULL)
							chan->remove_membre_mode(*target_client, 'o');
					}
					else
					{
						std::cout << "Error : +o mode missing argument" << std::endl;
						// send_err(403, client, target, "No such channel");
					}
				}
				if (mode[i] == 'l')
				{
					// chan->set_limit(std::stoi(arg));
					chan->remove_mode('l');
					// :WiZ!jto@tolsun.oulu.fi MODE #eu-opers -l
				}
				if (mode[i] == 'k')
				{
					chan->set_key(arg);
					chan->remove_mode('k');
				}
				else
				{
					chan->remove_mode(mode[i]);
				}

			}
		}
			// else
			// {
			// 	chan->set_mode(mode);
			// }

			chan->print();
	}
	else
	{
		Client* target_client = find_client(target);
		if (target_client == NULL)
		{
			send_err(401, sender, target, "No such nick/channel");
			return ;
		}
		if (target_client->get_nickname() != sender.get_nickname())
		{
			send_err(502, sender, target, "Cant change mode for other users");
			return ;
		}

		if (mode.at(0) == '+')
		{
			for (int i = 1; i < (int) target.size(); ++i)
			{
				target_client->add_mode(mode[i]);
			}
		}
		else if (mode.at(0) == '-')
		{
			for (int i = 1; i < (int) target.size(); ++i)
			{
				target_client->remove_mode(mode[i]);
			}
		}
		// else
		// {
		// 	target_client->set_mode(mode);
		// }
		
	}
}


void Server::topic(Client& sender, std::string target, std::string topic)
{
	std::cout << "target : " << target << std::endl;
	std::cout << "topic : " << topic << std::endl;

	if (target.at(0) == '#')
		target.erase(target.begin()); // delete first charcater  ( # )

	Channel* chan = find_channel(target); 
	if (chan == NULL)
	{
		send_err(403, sender, target, "No such channel");
		return ;
	}

	if (chan->is_mode('t'))
	{
		if (!chan->is_membre(sender.get_nickname()))
		{
			send_err(442, sender, target, "You're not on that channel");
			return ;
		}
		if (!chan->is_membre_mode(sender, 'o'))
		{
			send_err(482, sender, target, "You're not channel operator");
			return ;
		}

		chan->set_topic(topic);
	}
	else
	{
		chan->set_topic(topic);
	}
}

void 	Server::kick(Client &sender, std::string chan_name, std::string target)
{
	std::stringstream s;

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )

	Channel* chan = find_channel(chan_name); 
	if (chan != NULL)
	{
		Client* baduser = find_client(target);
		if (baduser == NULL)
		{
			send_err(401, sender, target, "No such nick/channel");
			return ;
		}
		if (!chan->is_membre_mode(sender, 'o'))
		{
			send_err(482, sender, target, "You're not channel operator");
			return ;
		}

		chan->remove_membre(*baduser);
		if (chan->count_membres() == 0)
			remove_channel(*chan);

		s << ":" << create_tag(sender) << " KICK #" << chan->get_name() << " " << baduser->get_nickname() << std::endl ;// << " " << msg << std::endl ;

		send_msg(baduser->get_socket(), s.str());

	}
	else if (chan_name == "local")
	{
		Client* baduser = find_client(target);
		if (baduser == NULL)
		{
			send_err(401, sender, target, "No such nick/channel");
			return ;
		}
		if (!sender.is_mode('o'))
		{
			std::cout << "Error : #" << target << " You're not operator" << std::endl;
			send_err(482, sender, target, "You're not operator");
			return ;
		}

		s << ":" << create_tag(sender) << " KICK : you have been kicked" << std::endl ;;//<< msg << std::endl ;
		
		send_msg(baduser->get_socket(), s.str());

		remove_client(*baduser);
	}
	else
	{
		std::cout << "Error : " << chan_name << " channel not found" << std::endl;
		send_err(403, sender, chan_name, "No such channel");
	}

	 // :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
}

void 	Server::invite(Client &sender, std::string chan_name, std::string target)
{
	std::stringstream s;

	if (chan_name.at(0) == '#')
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )

	Channel* chan = find_channel(chan_name); 
	if (chan == NULL)
	{
		send_err(403, sender, chan_name, "No such channel");
		return ;
	}
	if (!chan->is_membre(sender.get_nickname()))
	{
		send_err(442, sender, chan_name, "You're not on that channel");
		return ;
	}
	if (!chan->is_membre_mode(sender, 'o'))
	{
		send_err(482, sender, target, "You're not channel operator");
		return ;
	}
	Client* target_client = find_client(target);
	if (target_client == NULL)
	{
		send_err(401, sender, target, "No such nick/channel");
		return ;
	}
	if (chan->is_membre(target_client->get_nickname()))
	{
		send_err(443, sender, target, chan_name, "is already on channel");
		return ;
	}

	chan->add_invited(target_client->get_nickname());

	s << ":" << create_tag(sender) << " INIVTE " << target << " #" << chan_name << std::endl;

	send_msg(target_client->get_socket(), s.str());

	// :dan-!d@localhost INVITE Wiz #test
	// :Angel!wings@irc.org INVITE Wiz #Dust
}

void 	Server::ping(Client &sender, std::string msg)
{
	std::stringstream s;

	s << ":" << create_tag(sender) << " PONG :" << msg << std::endl;
		
	send_msg(sender.get_socket(), s.str());

}


void 	Server::quit(Client &sender, std::string msg)
{
	std::stringstream s;

	s << ":" << create_tag(sender) << " QUIT :" << msg << std::endl ;

	send_msg(sender.get_socket(), s.str());

	remove_client(sender);

	// :dan-!d@localhost QUIT :Quit: Bye for now!
}
