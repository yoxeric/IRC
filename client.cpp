
#include "inc/Client.hpp"


Client::Client()
{
	nickname = "";
	username = "";
	hostname = "";
	realname = "";
	address = "";
	pass = "";
	mode = "";

	socket = 0;
	registred = 0;
}

Client::~Client()
{
}

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
			mode.erase(mode.begin() + i);
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

void		Client::set_registred(int i)
{
	registred = i;
}


void		Client::add_registred(int i)
{
	registred += i;
}


void 		Client::set_socket(int s)
{
	socket = s;
}

void		Client::set_mode(std::string str)
{
	mode = str;
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

// void 		Client::set_message(std::string str)
// {
// 	message = str;
// }

// void 		Client::set_destination(int s)
// {
// 	destination = s;
// }

// ----------------------  Getter -----------------------


int		Client::get_registred()
{
	return registred;
}


int 		Client::get_socket()
{
	return (socket);
}

std::string Client::get_mode()
{
	return (mode);
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

// std::string Client::get_message()
// {
// 	return (message);
// }

// int 		Client::get_destination()
// {
// 	return (destination);
// }

// ----------------------  Debug -----------------------


void Client::print()
{
	std::cout << "@socket : <" << socket << ">" <<std::endl;

	std::cout << "nickname : <" << nickname << ">" << std::endl;
	std::cout << "username : <" << username << ">" << std::endl;
	std::cout << "address  : <" << address << ">" << std::endl;
	std::cout << "pass     : <" << pass << ">" << std::endl;
	std::cout << "mode     : <" << mode << ">" << std::endl;
}

