
#include "../inc/Server.hpp"

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
				if(std::strtod(arg.c_str(), NULL) < 1)
					continue;
				chan->set_limit(static_cast<int>(std::strtod(arg.c_str(), NULL)));
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
			else if(mode.at(i) == 'o' && oper == '+')
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

