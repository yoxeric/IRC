
#include "../inc/Server.hpp"

int Server::nick(Client& sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string name;
	getline(input, name, ' ');
	if (name.empty())
	{
		send_err(431, sender, "No nickname given");
		return 1;
	}
	if(name.find_first_not_of("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[]{}/|_-^") != std::string::npos)
	{
		send_err(431, sender, name, "Erroneus nickname");
		return 1;
	}
	Client* target_client = find_client(name);
	if (target_client != NULL && target_client->get_socket() != sender.get_socket())
	{
		send_err(431, sender, name, "Nickname is already in use");
		return 1;
	}

	// std::cout << "nickname = " << name << "."<< std::endl;
	// change the client name inside the joined channel
	for (std::vector<Channel>::iterator ch = channels.begin(); ch != channels.end(); ch++)
	{
		for (std::vector<Client>::iterator cl = ch->members.begin(); cl != ch->members.end(); cl++)
		{
			if (sender.get_nickname() == cl->get_nickname())
			{
				cl->set_nickname(name);
				break ;
			}
		}
	}
	sender.set_nickname(name);
	return 0;
}
