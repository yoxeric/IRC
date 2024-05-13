

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
	int count = 0;
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		count++;
	}
	return count;
}


void Channel::add_operator(Client& client)
{
	operators.push_back(client);
}

int Channel::remove_operator(Client& client)
{
	for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			operators.erase(it);
			break ;
		}
	}
	if (operators.size() == 1)
		return (2);
	return 0;
}

int Channel::is_operator(Client& client)
{
	for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			return 1;
		}
	}
	return 0;
}



void Channel::add_invited(Client& client)
{
	invited.push_back(client);
}

int Channel::remove_invited(Client& client)
{
	for (std::vector<Client>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
		{
			invited.erase(it);
			break ;
		}
	}
	if (invited.size() == 1)
		return (2);
	return 0;
}

int Channel::is_invited(Client& client)
{
	for (std::vector<Client>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		if (it->get_nickname() == client.get_nickname())
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

	std::cout << "---ops : " << std::endl;
	for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		std::cout << " ["<< it->get_socket() << "] --> " << it->get_nickname() << std::endl;
	}
	std::cout << "---membres : " << std::endl;
	for (std::vector<Client>::iterator it = members.begin(); it != members.end(); it++)
	{
		std::cout << " ["<< it->get_socket() << "] --> " << it->get_nickname() << std::endl;
	}
	std::cout << "---invited : " << std::endl;
	for (std::vector<Client>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		std::cout << " ["<< it->get_socket() << "] --> " << it->get_nickname() << std::endl;
	}
}