
#include "../inc/Server.hpp"

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
