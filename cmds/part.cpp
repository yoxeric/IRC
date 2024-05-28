
#include "../inc/Server.hpp"

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
