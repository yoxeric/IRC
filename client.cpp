
#include "inc/Client.hpp"


void		Client::add_mode(char m)
{
	for (int i = 0; i < (int) mode.size(); ++i)
	{
		if (mode[i] == m)
			return;
	}
	mode.push_back(m);
}

void		Client::remove_mode(char m)
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

int			Client::is_mode(char m)
{
	for (int i = 0; i < (int) mode.size(); ++i)
	{
		if (mode[i] == m)
			return (1);
	}
	return (0);
}

// ----------------------  Setter -----------------------


void		Client::set_mode(std::string str)
{
	mode = str;
}

void 		Client::set_operator(bool val)
{
	is_operator = val;
}

void 		Client::set_socket(int s)
{
	socket = s;
}

void 		Client::set_nickname(std::string str)
{
	nickname = str;
}

void 		Client::set_username(std::string str)
{
	username = str;
}

void 		Client::set_realname(std::string str)
{
	realname = str;
}

void 		Client::set_address(std::string str)
{
	address = str;
}

void 		Client::set_pass(std::string str)
{
	pass = str;
}


// ----------------------  Getter -----------------------


std::string Client::get_mode()
{
	return (mode);
}

bool 		Client::get_operator()
{
	return (is_operator);
}

int 		Client::get_socket()
{
	return (socket);
}

std::string Client::get_nickname()
{
	return (nickname);
}

std::string	Client::get_username()
{
	return (username);
}

std::string	Client::get_realname()
{
	return (realname);
}

std::string Client::get_address()
{
	return (address);
}

std::string Client::get_pass()
{
	return (pass);
}



// ----------------------  Debug -----------------------


void Client::print()
{
	std::cout << "@socket : " << socket << std::endl;
	std::cout << "nickname : " << nickname << std::endl;
	std::cout << "username : " << username << std::endl;
	std::cout << "address  : " << address << std::endl;
	std::cout << "pass     : " << pass << std::endl;
	std::cout << "is op    : " << is_operator << std::endl;
}

