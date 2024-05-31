
#include "../inc/Server.hpp"

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
			chan->set_topic_info(create_tag(sender), get_timestamp());
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
