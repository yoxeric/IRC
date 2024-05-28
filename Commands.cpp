
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

void Server::who(Client& sender, std::string buffer)
{
	std::stringstream ss;
	std::istringstream input(buffer);
	std::string target;
	getline(input, target, ' ');

	if (!target.empty() && target.at(0) == '#')
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
	 
	// get all names if target empty
}

void Server::oper(Client& sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string name;
	getline(input, name, ' ');

	std::string pswd;
	getline(input, pswd, ' ');

	if(pswd.empty() || name.empty())
	{
		send_err(461, sender, "OPER", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}	
	Client* target_client = find_client(name);
	if (target_client != NULL)
	{
		send_err(431, sender, "Nickname is already in use");
		return ;
	}
	if (pswd != password)
	{
		send_err(464, sender, "Password incorrect");
		return ;
	}

	// if (sender.is_mode('o'))
	// {
	// 	target_client->set_mode("o");
		// "<client> :You are now an IRC operator"
	// }

	if (name == "admin")
	{
		sender.set_mode("o");
	}
}

void 	Server::pass(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string pswd;

	getline(input, pswd, ' ');
	if(registred  > 0)
	{
		send_err(462, sender, "You may not reregister");
		return ;
	}
	if(pswd.empty())
	{
		send_err(461, sender, "PASS", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}	
	if (pswd != password)
	{
		send_err(464, sender, "Password incorrect");
		return ;
	}
	if (pswd == password)
	{
		sender.set_pass(pswd);
		registred += 1;
		std::cout << "password is correct !!! " << std::endl;
	}
	// send_err(462, sender, "You may not reregister");
}

void Server::cap(Client& sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string subcommand;
	std::stringstream ss;

	getline(input, subcommand, ' ');
	std::cout << "subcommand =" << subcommand << "."<< std::endl;
	std::cout << "buffer =" << buffer << "."<< std::endl;
	if(subcommand.empty())
	{
		send_err(410, sender, subcommand, "Invalid CAP command");
		return;
	}
	if(!subcommand.compare("LIST") || !subcommand.compare("LS"))
	{
		ss << "CAP * " << subcommand << " :"<< std::endl;
		send_msg(sender.get_socket(), ss.str());
	}
	else if(!subcommand.compare("REQ"))
	{
		ss << "CAP * ACK " << buffer.substr(buffer.find(":"), buffer.length() - buffer.find(":")) << std::endl;
		send_msg(sender.get_socket(), ss.str());
	}
	else if(!subcommand.compare("END"))
	{
		return;
	}
	else
	{
		send_err(410, sender, subcommand, "Invalid CAP command");
		return;
	}
}

void Server::nick(Client& sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string nick;
	getline(input, nick, ' ');
	if (nick.empty())
	{
		send_err(431, sender, "No nickname given");
		return ;
	}
	if(nick.find_first_not_of("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[]{}/|_-^") != std::string::npos)
	{
		send_err(431, sender, nick, "Erroneus nickname");
		return ;
	}
	Client* target_client = find_client(nick);
	if (target_client != NULL && target_client->get_socket() != sender.get_socket())
	{
		send_err(431, sender, nick, "Nickname is already in use");
		return ;
	}
	sender.set_nickname(nick);
	// give it a random value
}

void Server::user(Client& sender, std::string buffer)
{
	// USER username hostname us.undernet.org :realname in (RFC 1459)
	// USER username 0 * :realname in ( RFC 2812)
	std::istringstream input(buffer);
	std::string username;
	getline(input, username, ' ');

	std::string hostname;
	getline(input, hostname, ' ');

	std::string addr;
	getline(input, addr, ' ');

	std::string realname;
	getline(input, realname, ' ');

	if(hostname.empty() || hostname.find(':') != std::string::npos ||
		addr.empty() || addr.find(':') != std::string::npos ||
		username.empty() || username.find(':') != std::string::npos ||
		realname.front() != ':' || realname.size() < 2)
	{
		send_err(461, sender, "USER", "Not enough parameters");
		return ;
	}
	if(registred > 2)
	{
		send_err(462, sender, "You may not reregister");
		return ;
	}
	realname = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
	(void)(hostname);
	sender.set_username(username);
	sender.set_address(addr); // may have error here if adresse not added correctly
	sender.set_realname(realname);
	registred += 2;
	welcome_server(sender);
}


void Server::prvmsg(Client& sender, std::string buffer)
{
	std::ostringstream s;
	std::string all_targets;
	std::istringstream input(buffer);
	getline(input, all_targets, ' ');
	if(all_targets.empty() || all_targets.find_first_not_of(',') == std::string::npos)
	{
		send_err(461, sender, "PRVMG", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}
	if(all_targets.find(',') != std::string::npos)
		all_targets = all_targets.substr(all_targets.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
	
	std::vector<std::string> target;
	std::string msg;

	getline(input, msg, ' ');
	if(msg.front() != ':')
	{
		send_err(412, sender, "No text to send");
		return ;
	}
	msg = buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
	if(msg.empty())
	{
		send_err(412, sender, "No text to send");
		return ;
	}
	get_targets(all_targets, target);
	if(target.size() == 0)
	{
		send_err(411, sender, "No recipient given"); // ?????????
		return ;
	}
	// std::cout << "-----PRVMSG-----" << std::endl;

	for (int i = 0; i < (int) target.size(); ++i)
	{
		// std::cout << "target = "  << target[i] << "." << std::endl;
		// std::cout << "type = "  << type[i] << "." << std::endl;
		if (target[i].front() == '#')
		{
			target[i].erase(0, 1);
			Channel* chan = find_channel(target[i]); 
			if (chan == NULL)
			{
				send_err(403, sender, target[i], "No such channel");
				continue ;
			}
			if (!chan->is_membre(sender.get_nickname()))
			{
				send_err(404, sender, target[i], "Cannot send to channel (no external message)");
				continue ;
			}
			
			// std::cout << "found = " << chan->get_name() << std::endl;

			s << ":" << create_tag(sender) << " PRIVMSG #" << target[i] << " :" << msg;// << std::endl;

			send_msg_channel(sender, *chan, s.str());

		}
		else if (target[i].front() != '#')
		{
			Client* target_client = find_client(target[i]);
			if (target_client == NULL)
			{
				send_err(401, sender, target[i], "No such nick/channel");
				continue ;
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

void 	Server::join(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string all_targets;
	getline(input, all_targets, ' ');

	if(all_targets.empty() || all_targets.find_first_not_of(',') == std::string::npos)
	{
		send_err(461, sender, "Not enough parameters");
		return ;
	}

	if(all_targets.find(',') != std::string::npos)
		all_targets = all_targets.substr(all_targets.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
	
	std::vector<std::string> chan_name;
	get_targets(all_targets, chan_name);

	std::string all_keys;
	getline(input, all_keys, ' ');

	std::vector<std::string> key;
	if(!all_keys.empty() && all_keys.find_first_not_of(',') != std::string::npos)
	{
		if(all_keys.find(',') != std::string::npos)
			all_keys = all_keys.substr(all_keys.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
		get_targets(all_keys, key);
	}
  // std::cout << "--------------------" << all_targets << "." << all_targets.find_first_not_of(',') << "." << std::endl;
  // todo : (461) "<client> <command> :Not enough parameters"

	for(int i = 0; i < (int) chan_name.size(); i++)
	{
		std::cout << "chan_name = "  << chan_name[i] << "." << std::endl;
		if( (int) key.size() > i)
			std::cout << "key = "  << key[i] << "." << std::endl; 
		if (chan_name[i].at(0) != '#')
		{
			send_err(403, sender, chan_name[i], "No such channel");
			continue;
		}
		else
			chan_name[i].erase(chan_name[i].begin());

		Channel* chan = find_channel(chan_name[i]);
		if (chan != NULL)
		{
			if (chan->is_mode('i') && !chan->is_invited(sender.get_nickname()))
			{
				send_err(473, sender, chan_name[i], "Cannot join channel (invite-only)");
				continue ;
			}
			if (chan->is_mode('l') && chan->count_membres() >= chan->get_limit())
			{
				send_err(471, sender, chan_name[i], "Cannot join channel (users limit reached)");
				continue ;
			}
			if (chan->is_mode('k'))
			{
				if (key.empty())
				{
					send_err(475, sender, chan_name[i], "Cannot join channel (key not supplied)");
					continue ;
				}
				else if ( (int) key.size() <= i || chan->get_key() != key[i])
				{
					send_err(475, sender, chan_name[i], "Cannot join channel (incorrect key)");
					continue ;
				}
			}
		}
		else
		{
			chan = add_channel(chan_name[i]);
		}

		chan->add_membre(sender);
		if (chan->is_mode('i'))
			chan->remove_invited(sender.get_nickname());
		if (chan->count_operators() == 0)
			chan->add_membre_mode(sender, 'o');

		std::ostringstream s;

		s << ":" << create_tag(sender) << " JOIN #" << chan_name[i] << std::endl;

		send_msg(sender.get_socket(), s.str());
	}

	// nickname!username@address.IP JOIN #ch
}

void 	Server::part(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string all_chans;
	getline(input, all_chans, ' ');
	if(all_chans.empty() || all_chans.find_first_not_of(',') == std::string::npos)
	{
		send_err(461, sender, "PART", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}
	if(all_chans.find(',') != std::string::npos)
		all_chans = all_chans.substr(all_chans.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
	std::vector<std::string> chan_name;
	get_targets(all_chans, chan_name);

	std::string reason;
	getline(input, reason, ' ');

	for(int i = 0; i < (int) chan_name.size(); i++)
	{
		std::stringstream s;

		if (chan_name[i].at(0) != '#')
		{
			send_err(403, sender, chan_name[i], "No such channel");
			continue ;
		}
		else
			chan_name[i].erase(chan_name[i].begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(chan_name[i]); 
		if (chan == NULL)
		{
			send_err(403, sender, chan_name[i], "No such channel");
			continue ;
		}
		if (!chan->is_membre(sender.get_nickname()))
		{
			send_err(442, sender, chan_name[i], "You're not on that channel");
			continue ;
		}

		chan->remove_membre(sender);
		if (chan->count_membres() == 0)
			remove_channel(*chan);
		
		s << ":" << create_tag(sender) << " PART #" << chan_name[i] ;
		if(reason.size() > 1 && reason.front() == ':')
			s << " :" << buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		s << std::endl ;
		send_msg(sender.get_socket(), s.str());
	}
}

void Server::mode(Client& sender, std::string buffer)
{
// . i: Set/remove Invite-only channel ------- no arg 
// · t: Set/remove the restrictions of the TOPIC command to channel operators ------- no arg 
// · k: Set/remove the channel key (password) ------- 1 arg == key
// · o: Give/take channel operator privilege ---- 1 arg == user
// · l: Set/remove the user limit to channel ------- 1 arg == number

	std::istringstream input(buffer);
	std::string target;
	getline(input, target, ' ');

	if(target.empty())
	{
		send_err(461, sender, "MODE", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}

	std::string mode;
	getline(input, mode, ' ');

	std::cout << "target = "  << target << "." << std::endl;
	std::cout << "mode = "  << mode << "." << std::endl;

	char oper = '+';
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
		for (int i = 0; i < (int) mode.size(); i++)
		{
			if (mode.at(i) == '-')
			{
				oper = '-';
				continue;
			}
			else if(mode.at(i) == '+')
			{
				oper = '+';
				continue;
			}

			if (mode[i] == 'o')
			{
				std::string arg;
				getline(input, arg, ' ');
				if (!arg.empty())
				{
					Client* target_client = find_client(arg);
					if (target_client == NULL)
					{
						send_err(401, sender, target, "No such nick/channel");
						return ;
					}
					if (target_client != NULL)
					{
						if(oper == '-')
							chan->remove_membre_mode(*target_client, 'o');
						else if(oper == '+')
							chan->add_membre_mode(*target_client, 'o');
					}
				}
			}
			else if (mode[i] == 'k')
			{
				std::string arg;
				getline(input, arg, ' ');
				if (arg.empty())
				{
					send_err(461, sender, "MODE", "Not enough parameters");
				}
				chan->set_key(arg);
				if(oper == '+')
					chan->add_mode('k');
				if(oper == '-')
					chan->remove_mode('k');
			}
			else if (mode[i] == 'l' && oper == '+')
			{
				std::string arg;
				getline(input, arg, ' ');
				if(std::stoi(arg) < 1)
					continue;
				chan->set_limit(std::stoi(arg));
				chan->add_mode('l');
			}
			else if((mode.at(i) == 'i' || mode.at(i) == 't') && oper == '+')
			{
				chan->add_mode(mode[i]);
			}
			else if((mode.at(i) == 'i' || mode.at(i) == 't' || mode[i] == 'l') && oper == '-')
			{
				chan->remove_mode(mode[i]);
			}
			else
			{
				send_err(472, sender, std::string(1,mode.at(i)), "Unknown MODE flag");
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
		for (int i = 0; i < (int) mode.size(); i++)
		{
			if (mode.at(i) == '-')
			{
				oper = '-';
				continue;
			}
			else if(mode.at(i) == '+')
			{
				oper = '+';
				continue;
			}
			if(mode.at(i) == 'o' && oper == '-')
			{
				target_client->remove_mode(mode[i]);
			}
			else if(mode.at(i) == 'o' && oper == '-')
			{
				target_client->add_mode(mode[i]);
			}
			else
			{
				send_err(472, sender, std::string(1,mode.at(i)), "Unknown MODE flag");
			}
		}
	}
}


void Server::topic(Client& sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::stringstream ss;
	std::stringstream _ss_;

	std::string chan_name;
	getline(input, chan_name, ' ');
	
	if(chan_name.empty())
	{
		send_err(461, sender, "TOPIC", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}

	std::string topic;
	getline(input, topic, ' ');

	if(topic.size() > 1 && topic.front() == ':')
		topic = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));


	if (chan_name.at(0) != '#')
	{
		send_err(403, sender, chan_name, "No such channel");
		return ;
	}
	else
		chan_name.erase(chan_name.begin()); // delete first charcater  ( # )

	Channel* chan = find_channel(chan_name); 
	if (chan == NULL)
	{
		send_err(403, sender, chan_name, "No such channel");
		return ;
	}

	std::cout << "chan_name : " << chan_name << std::endl;
	std::cout << "topic : " << topic << std::endl;

	if (chan->is_mode('t'))
	{
		if (!chan->is_membre(sender.get_nickname()))
		{
			send_err(442, sender, chan_name, "You're not on that channel");
			return ;
		}
		if (!chan->is_membre_mode(sender, 'o'))
		{
			send_err(482, sender, chan_name, "You're not channel operator");
			return ;
		}
	}
	if(topic == ":")
	{
		chan->set_topic(topic);
	}
	else
	{
		chan->set_topic("");
	}
	if(topic.empty())
	{
		ss << "#" << chan->get_name() << " " << chan->count_membres();
		_ss_ << chan->get_topic();
		send_reply(332, sender, ss.str(), _ss_.str());
		return ;
	}
}

void 	Server::kick(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::stringstream s;
	std::string chan_name;
	getline(input, chan_name, ' ');

	std::string target;
	getline(input, target, ' ');

	if(chan_name.empty() || target.empty())
	{
		send_err(461, sender, "KICK", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}

	std::string reason;
	getline(input, reason, ' ');

	if (chan_name.at(0) != '#')
	{
		send_err(403, sender, chan_name, "No such channel");
		return ;
	}
	else
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

		s << ":" << create_tag(sender) << " KICK #" << chan->get_name() << " " << baduser->get_nickname();
		if(reason.size() > 1 && reason.front() == ':')
			s << " :" << buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		s << std::endl ;
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

		s << ":" << create_tag(sender) << " KICK : you have been kicked" << std::endl ;
		if(reason.size() > 1 && reason.front() == ':')
			s << " :" << buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		s << std::endl ;
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

void 	Server::invite(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string nick;
	std::stringstream s;
	getline(input, nick, ' ');

	std::string chan_name;
	getline(input, chan_name, ' ');

	if(chan_name.empty() || nick.empty())
	{
		send_err(461, sender, "INVITE", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}	

	if (chan_name.at(0) != '#')
	{
		send_err(403, sender, chan_name, "No such channel");
		return ;
	}
	else
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
		send_err(482, sender, nick, "You're not channel operator");
		return ;
	}
	Client* target_client = find_client(nick);
	if (target_client == NULL)
	{
		send_err(401, sender, nick, "No such nick/channel");
		return ;
	}
	if (chan->is_membre(target_client->get_nickname()))
	{
		send_err(443, sender, nick, chan_name, "is already on channel");
		return ;
	}

	chan->add_invited(target_client->get_nickname());

	s << ":" << create_tag(sender) << " INIVTE " << nick << " #" << chan_name << std::endl;

	send_msg(target_client->get_socket(), s.str());

	// :dan-!d@localhost INVITE Wiz #test
	// :Angel!wings@irc.org INVITE Wiz #Dust
}

void 	Server::ping(Client &sender, std::string buffer)
{
	std::stringstream s;
	std::istringstream input(buffer);
	std::string token;
	getline(input, token, ' ');
	if(token.empty() || !token.compare(":"))
	{
		send_err(409, sender, "No origin specified"); // ERR_NOORIGIN
		return ;
	}
	if(token.front() == ':')
		token.erase(0,1);
	s << ":" << create_tag(sender) << " PONG :" << token << std::endl;
	send_msg(sender.get_socket(), s.str());

}

void 	Server::quit(Client &sender, std::string buffer)
{
	std::stringstream s;
	std::istringstream input(buffer);
	std::string reason;
	getline(input, reason, ' ');

	s << ":" << create_tag(sender) << " QUIT";
	if(reason.size() > 1 && reason.front() == ':')
	{
		reason = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
		s << " :" << reason;
	}
	s << std::endl;
	for (std::vector<Channel>::iterator ch = channels.begin(); ch != channels.end(); ch++)
	{
		for (std::vector<Client>::iterator cl = ch->members.begin(); cl != ch->members.end(); cl++)
		{
			if (sender.get_nickname() == cl->get_nickname())
			{
				send_msg_channel(sender, *ch, s.str());
				break ;
			}
		}
	}
	send_msg(sender.get_socket(), s.str());

	remove_client(sender);

	// :dan-!d@localhost QUIT :Quit: Bye for now!
}
