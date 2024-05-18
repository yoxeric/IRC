

#include "inc/Channel.hpp"



void Channel::add_membre(Client& client)
{
	members.push_back(client);
}

int Channel::remove_membre(Client& client)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			members.erase(it);
			break ;
		}
	}
	if (members.size() == 1)
		return (2);
	return 0;
}

// int Channel::is_membre(Client& client)
// {
// 	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
// 	{
// 		if (it->get_nickname() == client.get_nickname())
// 		{
// 			return 1;
// 		}
// 	}
// 	return 0;
// }

int Channel::is_membre(std::string name)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == name)
		{
			return 1;
		}
	}
	return 0;
}

int	Channel::count_membres()
{
	return members.size();
}

int	Channel::count_operators()
{
	int count = 0;
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->is_mode('o'))
			count++;
	}
	return count;
}


void Channel::add_membre_mode(Client& client, char m)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			it->add_mode(m);
			break ;
		}
	}
}

void Channel::remove_membre_mode(Client& client, char m)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			it->remove_mode(m);
			break ;
		}
	}
}

int Channel::is_membre_mode(Client& client, char m)
{
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			return (it->is_mode(m));
		}
	}
	return 0;
}



void Channel::add_invited(std::string s)
{
	invited.push_back(s);
}

void Channel::remove_invited(std::string s)
{
	for (std::vector<std::string>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		if ((*it) == s)
		{
			invited.erase(it);
			return ;
		}
	}
}

int Channel::is_invited(std::string s)
{
	for (std::vector<std::string>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		if ((*it) == s)
		{
			return 1;
		}
	}
	return 0;
}

// ----------------------  Mode -----------------------


void		Channel::add_mode(char m)
{
	for (int i = 0; i < (int) mode.size(); ++i)
	{
		// std::cout << "am : " << mode[i] << "==" << m << std::endl;
		if (mode[i] == m)
			return;
	}
	mode.push_back(m);
}

void		Channel::remove_mode(char m)
{
	for (int i = 0; i < (int) mode.size(); ++i)
	{
		// std::cout << "rm : " << mode[i] << "==" << m << std::endl;
		if (mode[i] == m)
		{
			mode.erase(mode.begin() + i);
			return ;
		}
	}
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

// ----------------------  Setter -----------------------



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


// ----------------------  Getter -----------------------


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



// ----------------------  Debug -----------------------


void Channel::print()
{
	std::cout << "----- Channel = #" << name  << std::endl;
	std::cout << "name  : <" << name << ">" << std::endl;
	std::cout << "topic : <" << topic << ">" << std::endl;
	std::cout << "key   : <" << key  << ">" << std::endl;
	std::cout << "mode  : <" << mode << ">" << std::endl;
	std::cout << "limit : <" << limit << ">" << std::endl;

	std::cout << "---membres : " << std::endl;
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		std::cout << " ["<< it->get_socket() << "] --> " << it->get_nickname() << "   |   op=" << it->is_mode('o') << std::endl;
	}
	std::cout << "---invited : " << std::endl;
	for (std::vector<std::string>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		std::cout << "- " << (*it) << std::endl;
	}
}