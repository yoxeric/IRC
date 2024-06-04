
#include "inc/Server.hpp"

void 		Server::get_targets(std::string str, std::vector<std::string> &target)
{
	std::string arg;
	std::istringstream sub_input(str);
	getline(sub_input, arg, ',');
	for (int i = 0; i < (std::count(str.begin(), str.end(), ',') + 1);i++)
	{
		if(!arg[0])
		{
			getline(sub_input, arg, ',');
			continue;
		}
		target.push_back(arg);
		getline(sub_input, arg, ',');
	}
}

void		Server::split_input(int index, std::string msg)
{
	std::istringstream input(msg);

	getline(input, msg, '\n');
	while (!msg.empty())
	{
		if(msg.front() == ' ')
			msg = msg.substr(msg.find_first_not_of(" "), msg.length());
		if(msg.back() == '\r')
			msg.pop_back();

		if (parse(index, msg))
			break;

		getline(input, msg, '\n');
	}
}

// To get what command to call, do a map with a string as a key (command name) and a pointer to function (it’s cool)
int Server::parse(int index, std::string buffer)
{
	// Client &sender = clients[ index - 1 ];
	Client &sender = *find_client(pool.get_socket(index));

	std::istringstream input(buffer);
	std::string comand;
	getline(input, comand, ' ');

	buffer.erase(0,comand.size());
	if(buffer.front() == ' ' && buffer.find_first_not_of(" ") != std::string::npos)
		buffer = buffer.substr(buffer.find_first_not_of(" "), buffer.length());

	if (!comand.compare("CAP")) //done
		cap(sender, buffer);
	else if (!comand.compare("PASS")) //done
	{
		if (!pass(sender, buffer))
			sender.set_registred(1);
		else
			sender.add_registred(-1);
	}
	else if (sender.get_registred() < 1) // done
	{
		send_err(421, sender, "You have not registered");
		sender.add_registred(-1);
	}
	else if (!comand.compare("USER"))  //done
	{
		if (!user(sender, buffer))
			sender.add_registred(1);
	}
	else if (!comand.compare("NICK")) // done
	{
		if (!nick(sender, buffer))
			sender.add_registred(1);
	}
	else if (sender.get_registred() < 3) // done
	{
		send_err(421, sender, "You have not registered");
	}
	else if (!comand.compare("WHO")) //done
		who(sender, buffer);
	else if (!comand.compare("PING")) //done
		ping(sender, buffer);
	else if (!comand.compare("PRIVMSG")) //done
		prvmsg(sender, buffer);
	else if (!comand.compare("LIST"))
		list(sender);
	else if (!comand.compare("MODE"))
		mode(sender, buffer);
	else if (!comand.compare("JOIN"))
		join(sender, buffer);
	else if (!comand.compare("KICK")) // done not tested
		kick(sender, buffer);
	else if (!comand.compare("INVITE")) // done  not tested
		invite(sender, buffer); 
	else if (!comand.compare("TOPIC")) // done
		topic(sender, buffer);
	else if (!comand.compare("OPER")) //done
		oper(sender, buffer);
	else if (!comand.compare("PART")) //done 
		part(sender, buffer);
	else if (!comand.compare("QUIT")) //done not tested
	{
		quit(sender, buffer);
		return 1;
	}
	else
		send_err(421, sender, comand, "Unknown command");

	if (sender.get_registred() <= -3)
	{
		remove_client(sender);
		return 1;
	}
	if (sender.get_registred() == 3)
	{
		welcome_server(sender);
		sender.add_registred(1);
	}

	// std::cout << "----- sender = " << sender.get_nickname() << std::endl;
	// sender.print();

	return 0;
}