
#include "../inc/Server.hpp"

int 	Server::pass(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string pswd;

	// std::cout << "-----PASS-----" << std::endl;

	getline(input, pswd, ' ');
	if(!sender.get_pass().empty())
	{
		send_err(462, sender, "You may not reregister");
		return 1;
	}
	if(pswd.empty())
	{
		send_err(461, sender, "PASS", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return 1;
	}	
	if (pswd != password)
	{
		send_err(464, sender, "Password incorrect");
		return 1;
	}
	sender.set_pass(pswd);
	return 0;
}