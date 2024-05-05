

#include "inc/Channel.hpp"


void Channel::add_membre(Client client)
{
	members.push_back(client);
}


int Channel::is_membre(Client client)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			return 1;
		}
	}
	return 0;
	// std::vector<Client>::iterator it = std::find(members.begin(), members.end(), client);
	// if (it != membres.end())
	// 	return (1);
	// return (0);
}


std::string Channel::get_name()
{
	return (name);
}

void Channel::set_name(std::string str)
{
	name = str;
}
