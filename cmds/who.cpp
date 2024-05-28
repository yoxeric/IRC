
#include "../inc/Server.hpp"

void Server::who(Client& sender, std::string buffer)
{
	std::stringstream ss;
	std::istringstream input(buffer);
	std::string target;
	getline(input, target, ' ');

	if (!target.empty() && target.at(0) == '#')
	{
		target.erase(target.begin()); // delete first charcater  ( # )

		Channel* chan = find_channel(target); 
		if (chan == NULL)
		{
			send_err(403, sender, target, "No such channel");
			return ;
		}

		list_channel(sender, *chan);
	}
	else
	{
		Client* target_client = find_client(target);
		if (target_client == NULL)
		{
			send_err(401, sender, target, "No such nick/channel");
			return ;
		}

		list_user(sender, *target_client);
	}
	 
	// get all names if target empty
}
