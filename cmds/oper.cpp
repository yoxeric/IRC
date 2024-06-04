
#include "../inc/Server.hpp"

void Server::oper(Client& sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string name;
	getline(input, name, ' ');

	std::string pswd;
	getline(input, pswd, ' ');

	if(pswd.empty() || name.empty())
	{
		send_err(461, sender, "OPER", "Not enough parameters");
		return ;
	}	
	Client* target_client = find_client(name);
	if (target_client != NULL)
	{
		send_err(431, sender, "Nickname is already in use");
		return ;
	}


	if (name != "admin")
	{
		send_err(464, sender, "user incorrect");
		return ;
	}
	if (pswd != "admin")
	{
		send_err(464, sender, "Password incorrect");
		return ;
	}
	
	sender.add_mode('o');
	for (std::vector<Channel>::iterator ch = channels.begin(); ch != channels.end(); ch++)
	{
		if (ch->is_membre(sender.get_nickname()))
			ch->add_membre_mode(sender, 'o');;
	}
}
