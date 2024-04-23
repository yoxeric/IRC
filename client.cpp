
#include "client.hpp"

void s_client::cap(std::string str)
{
	(void)(str);
}

void s_client::nick(std::string str)
{
	nickname = str;
}

void s_client::user(std::string user, std::string addr)
{

	username = user;
	address = addr;
	
}

void s_client::join(std::string str)
{
	chan.push_back(str);
}


void s_client::print()
{
	std::cout << "nickname :" << nickname << std::endl;
	std::cout << "username :" << username << std::endl;
	std::cout << "address  :" << address << std::endl;
	// std::cout << "pass     :" << pass << std::endl;

	// std::cout << "chan    :" << chan << std::endl;
	// std::cout << "chanops :" << chanops << std::endl;
}
