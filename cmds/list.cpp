
#include "../inc/Server.hpp"

void	Server::list(Client& sender)
{

	std::cout << "---LISTING-----" << std::endl;

	send_reply(321, sender, "Channel", "Users Name");


	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::stringstream ss;
		ss << "#"  << it->get_name() << " " << it->count_membres();
		send_reply(322, sender, ss.str(), it->get_topic());
	}
	
	send_reply(323, sender, "", "End of /LIST");

}
