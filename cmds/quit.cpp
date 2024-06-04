
#include "../inc/Server.hpp"

void 	Server::quit(Client &sender, std::string buffer)
{
	std::stringstream s;
	std::istringstream input(buffer);
	std::string reason;
	getline(input, reason, ' ');

	s << ":" << create_tag(sender) << " QUIT";
	if(reason.size() > 1 && reason.front() == ':')
	{
		reason = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
		s << " :" << reason;
	}
	s << std::endl;

	for (std::vector<Channel>::iterator ch = channels.begin(); ch != channels.end(); ch++)
	{
		if (ch->is_membre(sender.get_nickname()))
			send_msg_channel(sender, *ch, s.str());
	}
	
	send_msg(sender.get_socket(), s.str());

	remove_client(sender);

	// std::cout << "----- sender = " << &sender << std::endl;

	// :dan-!d@localhost QUIT :Quit: Bye for now!
}