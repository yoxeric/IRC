#pragma once

#include <iostream>
#include <sstream> 
#include <vector>

#include "poll.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
private:
	std::string			networkname;
	std::string			servername;
	std::string			datetime;
	std::string			version;
	std::string			tokens;


public:
	s_poll					pool;
	std::vector<Client>		clients;
	std::vector<Channel>	channels;


	// Server();
	// ~Server();
	
	// std::string parse (int sender_socket, std::string buffer);

	Channel&	add_channel(std::string name);
	Channel&	find_channel(std::string name);
	// void		remove_channel(std::string name);

	Client&		add_client(int socket);
	Client&		find_client(int socket);
	Client&		find_client(std::string name);
	// void		remove_client(int socket);


	void 		cap(Client &client, std::string str);
	void 		nick(Client &client, std::string str);
	void 		user(Client &client, std::string str, std::string adrr);
	void 		join(Client &client, std::string str);
	void 		mode(Client &client, std::string str);
	void 		who(Client &client, std::string str);
	void 		prvmsg(Client &client, std::string target, std::string msg);
	void 		quit(Client &client, std::string str);

	// int			do_channel_exist(std::string chan);
	// int 		is_channel_membre(Client client);

	int			send_msg_client(Client &client, std::string msg);
	int			send_msg_channel(Channel &chan, std::string msg);

	int			send_msg(int dest_fd, std::string msg);
	// int			send_msg_index(int dest_index, std::string msg);

	std::string	creat_reply(int code, std::string nick, std::string arg, std::string msg);

	void 		list_clients();
};
