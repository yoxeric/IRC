
#include "ft_irc.hpp"

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

std::string parse(int sender_index, char *buffer, s_poll *pool)
{
	std::cout << "----- sender index = " << sender_index << std::endl;

	std::string str(buffer);
	std::size_t i;

	i = str.find("CAP ");
	if (i != std::string::npos)
	{
		std::string cap = str.substr(i + 4, str.length() - i - 1);

		pool->clients[sender_index].cap(cap);
	}

	i = str.find("NICK ");
	if (i != std::string::npos)
	{
		std::string nick = getword(str, i + 5);

		pool->clients[sender_index].nick(nick);
	}

	i = str.find("USER ");
	if (i != std::string::npos)
	{
		i = i + 5;

		std::string user;
		std::string addr;

		user = getword(str, i);

		i = str.find(" ", i);
		user = getword(str, i + 1);

		i = str.find(" ", i + 1);
		addr = getword(str, i + 1);

		pool->clients[sender_index].user(user, addr);
	}
	
	i = str.find("JOIN ");
	if (i != std::string::npos)
	{
		std::string ch;

		i = str.find("#");
		if (i != std::string::npos)
		{
			ch = getword(str, i + 1);

			pool->channels.push_back(ch);
			pool->clients[sender_index].join(ch, pool->fds[sender_index].fd);

			pool->clients[sender_index].chan.push_back(ch);
		}
	}

	i = str.find("WHO ");
	if (i != std::string::npos)
	{
	}

	i = str.find("MODE ");
	if (i != std::string::npos)
	{
	}

	i = str.find("PRIVMSG ");
	if (i != std::string::npos)
	{

		i = str.find("#", i + 8);
		if (i != std::string::npos)
		{
			std::string ch = getword(str, i + 1);

			std::cout << "priv ch = " << ch << std::endl;

			std::string msg;

			i = str.find(":");
			if (i != std::string::npos)
				msg = str.substr(i+ 1, str.length() - i - 1);

			private_msg_channel(sender_index, ch, msg, pool);
		}
		else
		{
			i = str.find("PRIVMSG ");
			std::string usr = getword(str, i + 8);

			std::cout << "priv usr = " << usr << std::endl;


			std::string msg;

			i = str.find(":");
			if (i != std::string::npos)
				msg = str.substr(i+ 1, str.length() - i - 1);

			private_msg_user(sender_index, usr, msg, pool);
		}
	}

	i = str.find("QUIT ");
	if (i != std::string::npos)
	{
	}

	pool->clients[sender_index].print();

	std::string msg;

	i = str.find(":");
	// std::cout << "pos1 " << i<< std::endl;
	// std::cout << "pos2 "<< str.length() - i<< std::endl;
	if (i != std::string::npos)
	{
		msg = str.substr(i+ 1, str.length() - i - 1);
		return (msg);
	}
	else
	{
		msg = str;
		return (msg);
	}
}
