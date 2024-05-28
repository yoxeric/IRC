
#include "../inc/Server.hpp"

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

	std::cout << "nickname = " << name << "."<< std::endl;
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
	// give it a random value
	
	sender.set_registred(sender.is_registred() + 1);
}
