
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
		send_err(461, sender, "OPER", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}	
	Client* target_client = find_client(name);
	if (target_client != NULL)
	{
		send_err(431, sender, "Nickname is already in use");
		return ;
	}
	// if (pswd != password)
	// {
	// 	send_err(464, sender, "Password incorrect");
	// 	return ;
	// }
	// if (sender.is_mode('o'))
	// {
	// 	target_client->set_mode("o");
		// "<client> :You are now an IRC operator"
	// }

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
	
	sender.set_mode("o");
}
