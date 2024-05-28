
#include "../inc/Server.hpp"

void 	Server::pass(Client &sender, std::string buffer)
{
	std::istringstream input(buffer);
	std::string pswd;

	// std::cout << "-----PASS-----" << std::endl;

	getline(input, pswd, ' ');
	if(!sender.get_pass().empty())
	{
		send_err(462, sender, "You may not reregister");
		return ;
	}
	if(pswd.empty())
	{
		send_err(461, sender, "PASS", "Not enough parameters"); //return("ERR_NEEDMOREPARAMS");
		return ;
	}	
	if (pswd != password)
	{
		send_err(464, sender, "Password incorrect");
		sender.set_registred(sender.is_registred() - 1);
		return ;
	}
	if (pswd == password)
	{
		sender.set_pass(pswd);
		sender.set_registred(1);
		std::cout << "password is correct !!! " << std::endl;
	}
	// send_err(462, sender, "You may not reregister");
}