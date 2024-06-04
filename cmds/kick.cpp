
#include "../inc/Server.hpp"

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
	if (chan == NULL)
	{
		send_err(403, sender, chan_name, "No such channel");
	}
	else if (chan != NULL)
	{
		Client* baduser = find_client(target);
		if (baduser == NULL)
		{
			send_err(401, sender, target, "No such nick/channel");
			return ;
		}
		if (!chan->is_membre_mode(sender, 'o'))
		{
			send_err(482, sender, sender.get_nickname(), "You're not channel operator");
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
			send_err(482, sender, sender.get_nickname(), "You're not operator");
			return ;
		}

		s << ":" << create_tag(sender) << " KICK : you have been kicked" << std::endl ;
		if(reason.size() > 1 && reason.front() == ':')
			s << " :" << buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		s << std::endl ;
		send_msg(baduser->get_socket(), s.str());

		remove_client(*baduser);
	}

	 // :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
}
