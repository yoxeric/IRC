
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

void s_client::join(std::string ch, int serv_fd)
{
	chan.push_back(ch);

	std::ostringstream s;

	s << ":" << nickname << "!" << username << "@" << address << ".IP JOIN #" << ch << std::endl ;
	std::cout << "Message >> " << s.str() << std::endl ;
	if (send(serv_fd, s.str().c_str(), s.str().size(), 0) == -1)
	{
		std::cout << "Send error to client [" << serv_fd << "]";
		return ;
	}

	// :yh1!~yh2@60CCA6D2.A5193C4D.9C9BAFEA.IP JOIN :#kosmosas
	// nickname!username@address.IP JOIN #ch
}

void s_client::print()
{
	std::cout << "nickname :" << nickname << std::endl;
	std::cout << "username :" << username << std::endl;
	std::cout << "address  :" << address << std::endl;
	std::cout << "pass     :" << pass << std::endl;

	// std::cout << "chan    :" << chan[0] << std::endl;
	// std::cout << "chanops :" << chanops[0] << std::endl;
}
