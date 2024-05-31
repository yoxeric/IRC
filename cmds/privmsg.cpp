
#include "../inc/Server.hpp"

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

			s << ":" << create_tag(sender) << " PRIVMSG #" << target[i] << " :" << msg << std::endl;

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

			s << ":" << create_tag(sender) << " PRIVMSG " << target[i] << " :" << msg << std::endl;

			send_msg(target_client->get_socket(), s.str());
		}
	}

	// :Angel PRIVMSG Wiz :Hello are you receiving this message ?

  	// :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!

	// nickname!username@address.IP PRIVMSG #coolpeople :Hi everyone!
}
