
#include "inc/ft_irc.hpp"

// << PRIVMSG @%#bunny,Angel :Hi! I have a problem!
void get_targets(std::string str, std::vector<std::string> &target, std::vector<int> &type)
{
	int i = 1;
	std::istringstream input(str);
	
	std::cout << "str [" << str << "] "<< std::endl;
	for (; i < (std::count(str.begin(), str.end(), ',') + 1);i++)
	{
		getline(input, target[i], ',');
		type[i] = 1 * (target[i][0] == '#') + 1;
		std::cout << "i [" << i << "] "<< std::endl;
		if(type[i] == 2)
	        target[i] = target[i].substr(1, target[i].length() - 1);
	}
	getline(input, target[0], ' ');
	type[0] = 1 * (target[0][0] == '#') + 1;
	if(type[0] == 2)
	    target[0] = target[0].substr(1, target[0].length() - 1);
}

std::string getword(std::string str, int start)
{
	std::size_t i;
	std::size_t j;
	std::string word;

	i = str.find(" ", start);
	j = str.find("\n", start);
	if (i != std::string::npos || j != std::string::npos)
	{
		if (j < i)
			i = j;
		word = str.substr(start, i - start);
		// std::cout << start << " - " << i << " word :" << word << std::endl;
	}

	if (word.back() == '\r')
		word.pop_back();

	return (word);
}

// To get what command to call, do a map with a string as a key (command name) and a pointer to function (it’s cool)

std::string parse(Server& server, int sender_socket, std::string buffer)
{
	std::size_t i;
	// int index = server.find_client(sender_socket);
	// Client &sender = server.clients[ index ];
	Client &sender = *server.find_client(sender_socket);


	std::cout << "parsing ... " << std::endl;

	i = buffer.find("CAP ");
	if (i != std::string::npos)
	{
		std::string cap = buffer.substr(i + 4, buffer.length() - i - 1);

		server.cap(sender, cap);
	}

	i = buffer.find("NICK ");
	if (i != std::string::npos)
	{
		std::string nick = getword(buffer, i + 5);

		server.nick(sender, nick);
	}

	i = buffer.find("USER ");
	if (i != std::string::npos)
	{
		std::string user;
		std::string addr;

		user = getword(buffer, i + 5);

		i = buffer.find(" ", i + 5);
		user = getword(buffer, i + 1);

		i = buffer.find(" ", i + 1);
		addr = getword(buffer, i + 1);

		server.user(sender, user, addr);
	}
	
	i = buffer.find("JOIN ");
	if (i != std::string::npos)
	{
		std::string ch;

		i = buffer.find("#");
		if (i != std::string::npos)
		{

			ch = getword(buffer, i + 1);

			server.join(sender, ch);

		}
	}

	i = buffer.find("WHO ");
	if (i != std::string::npos)
	{
		std::string target = getword(buffer, i + 4);

		server.who(sender, target);

	}

	i = buffer.find("MODE ");
	if (i != std::string::npos)
	{
		std::string target = getword(buffer, i + 5);

		std::string mode = "";

		server.mode(sender, target, mode);
	}

	if (buffer.find("PRIVMSG ") == 0 || buffer.find("NOTICE") == 0)
	{
		std::vector<std::string> target(std::count(buffer.begin(), buffer.begin() + buffer.find(":"), ',') + 1);
		std::vector<int> type(target.size());
		std::string msg = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
		// channel type 2, user type 1
		get_targets(buffer.substr(buffer.find(" ") + 1, buffer.find(":") - buffer.find(" ") - 1), target, type);
		server.prvmsg(sender, target, type, msg);

		
		// std::string target = getword(buffer, i + 8);

		// i = buffer.find(":");
		// std::string msg = buffer.substr(i + 1, buffer.length() - i - 1);

		// server.prvmsg(sender, target, msg);
	}

	i = buffer.find("QUIT ");
	if (i != std::string::npos)
	{
	}

	std::cout << "----- sender = " << sender.get_nickname() << std::endl;
	sender.print();

	// split msg after :

	i = buffer.find(":");
	if (i != std::string::npos)
		return (buffer.substr(i + 1, buffer.length() - i - 1));
	else
		return (buffer);
}
