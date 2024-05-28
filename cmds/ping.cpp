
#include "../inc/Server.hpp"

void 	Server::ping(Client &sender, std::string buffer)
{
	std::stringstream s;
	std::istringstream input(buffer);
	std::string token;
	getline(input, token, ' ');
	if(token.empty() || !token.compare(":"))
	{
		send_err(409, sender, "No origin specified"); // ERR_NOORIGIN
		return ;
	}
	if(token.front() == ':')
		token.erase(0,1);
	s << ":" << create_tag(sender) << " PONG :" << token << std::endl;
	send_msg(sender.get_socket(), s.str());

}