
#include "inc/Client.hpp"


void Client::print()
{
	std::cout << "@socket :" << socket << std::endl;
	std::cout << "nickname :" << nickname << std::endl;
	std::cout << "username :" << username << std::endl;
	std::cout << "address  :" << address << std::endl;
	std::cout << "pass     :" << pass << std::endl;
}

void 		Client::set_socket(int s)
{
	socket = s;
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

std::string Client::get_address()
{
	return (address);
}

std::string Client::get_pass()
{
	return (pass);
}


void 		Client::set_nickname(std::string str)
{
	nickname = str;
}

void 		Client::set_username(std::string str)
{
	username = str;
}

void 		Client::set_address(std::string str)
{
	address = str;
}

void 		Client::set_pass(std::string str)
{
	pass = str;
}
