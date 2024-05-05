
#include "inc/ft_irc.hpp"

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
	return (word);
}

// To get what command to call, do a map with a string as a key (command name) and a pointer to function (it’s cool)

std::string parse(Server& server, int sender_socket, std::string buffer)
{
	std::size_t i;
	Client sender = server.find_client(sender_socket);

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
	}

	i = buffer.find("MODE ");
	if (i != std::string::npos)
	{
	}

	i = buffer.find("PRIVMSG ");
	if (i != std::string::npos)
	{
		std::string target = getword(buffer, i + 8);

		i = buffer.find(":", i + 8);

		std::string msg = buffer.substr(i, buffer.length() - i - 1);


		server.prvmsg(sender, target, msg);
		
	}

	i = buffer.find("QUIT ");
	if (i != std::string::npos)
	{
	}

	std::cout << "----- sender = " << sender.get_nickname() << std::endl;
	sender.print();

	// msg to send to everybody
	std::string msg;

	i = buffer.find(":");
	// std::cout << "pos1 " << i<< std::endl;
	// std::cout << "pos2 "<< buffer.length() - i<< std::endl;
	if (i != std::string::npos)
	{
		msg = buffer.substr(i+ 1, buffer.length() - i - 1);
		return (msg);
	}
	else
	{
		msg = buffer;
		return (msg);
	}
}
