

#include "inc/Channel.hpp"

void Channel::print()
{
	std::cout << "nickname : " << name  << std::endl;
	std::cout << "username : " << topic  << std::endl;
	std::cout << "address  : " << key  << std::endl;
	std::cout << "pass     : " << limit << std::endl;

	std::cout << "---membres     : " << std::endl;
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		std::cout << " ["<< it->get_socket() << "] --> " << it->get_nickname() << std::endl;
	}
}

void Channel::add_membre(Client& client)
{
	members.push_back(client);
}

int Channel::is_membre(Client& client)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			return 1;
		}
	}
	return 0;
}

int	Channel::count_membres()
{
	int count = 0;
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		count++;
	}
	return count;
}

int			Channel::is_mode(char m)
{
	for (int i = 0; i < (int) mode.size(); ++i)
	{
		if (mode[i] == m)
			return (1);
	}
	return (0);
}

std::string Channel::get_mode()
{
	return (mode);
}

std::string Channel::get_name()
{
	return (name);
}

std::string Channel::get_topic()
{
	return (topic);
}

std::string Channel::get_key()
{
	return (key);
}

int Channel::get_limit()
{
	return (limit);
}

void		Channel::add_mode(char m)
{
	for (int i = 0; i < (int) mode.size(); ++i)
	{
		if (mode[i] == m)
			return;
	}
	mode.push_back(m);
}

void		Channel::remove_mode(char m)
{
	int i = 0;
	for (; i < (int) mode.size(); ++i)
	{
		if (mode[i] == m)
		{
			mode.erase(i);
			return ;
		}
	}
}

void	Channel::set_mode(std::string str)
{
	mode = str;
}

void	Channel::set_name(std::string str)
{
	name = str;
}

void	Channel::set_topic(std::string str)
{
	topic = str;
}

void	Channel::set_key(std::string str)
{
	key = str;
}

void	Channel::set_limit(int l)
{
	limit = l;
}
