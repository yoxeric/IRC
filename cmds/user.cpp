
#include "../inc/Server.hpp"

int Server::user(Client& sender, std::string buffer)
{
	// USER username hostname us.undernet.org :realname in (RFC 1459)
	// USER username 0 * :realname in ( RFC 2812)
	std::istringstream input(buffer);
	std::string username;
	getline(input, username, ' ');

	std::string hostname;
	getline(input, hostname, ' ');

	std::string addr;
	getline(input, addr, ' ');

	std::string realname;
	getline(input, realname, ' ');

	// std::cout << "-----USER-----" << std::endl;


	if(hostname.empty() || hostname.find(':') != std::string::npos ||
		addr.empty() || addr.find(':') != std::string::npos ||
		username.empty() || username.find(':') != std::string::npos ||
		realname.front() != ':' || realname.size() < 2)
	{
		send_err(461, sender, "USER", "Not enough parameters");
		return 1;
	}
	if(!sender.get_username().empty())
	{
		send_err(462, sender, "You may not reregister");
		return 1;
	}
	realname = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
	sender.set_username(username);
	sender.set_hostname(hostname);
	sender.set_address(addr); // may have error here if adresse not added correctly
	sender.set_realname(realname);
	return 0;
}
