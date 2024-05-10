#pragma once

#include <iostream>
#include <sstream> 
#include <vector>
#include <iomanip>

#include "poll.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
private:
	std::string			networkname;
	std::string			servername; // address
	std::string			datetime;
	std::string			version;

public:
	s_poll					pool;
	std::vector<Client>		clients;
	std::vector<Channel>	channels;


	void		init_server();
	void		welcome_server(Client& client);
	void		modt_server(Client& client);
	void 		list_users(Client &client);
	void 		list(Client &client);

	// std::string parse (int sender_socket, std::string buffer);

	Channel*	add_channel(std::string name);
	Channel*	find_channel(std::string name);
	// void		remove_channel(std::string name);

	Client*		add_client(int socket);
	Client*		find_client(int socket);
	Client*		find_client(std::string name);
	// void		remove_client(int socket);

	int			client_count();
	int			operator_count();
	int			channel_count();

	int			do_channel_exist(std::string chan_name);
	// int 		is_channel_membre(Client client);


	void 		cap(Client &client, std::string str);
	void 		nick(Client &client, std::string nick);
	void 		user(Client &client, std::string user, std::string adrr);
	void 		join(Client &client, std::string chan_name);
	void 		mode(Client& client, std::string target, std::string mode);
	void 		who(Client& client, std::string target);
	void		prvmsg(Client& client, std::vector<std::string> target, std::vector<int> type, std::string msg);
	void 		quit(Client &client, std::string str);


	// int			send_msg_client(Client &client, std::string msg);
	int			send_msg_channel(Channel &chan, std::string msg);
	int			send_msg(int dest_fd, std::string msg);

	std::string	create_tag(Client& client);
	// std::string	create_reply(int code, std::string nick, std::string arg, std::string msg);
	void		send_reply(int code, Client &client, std::string arg, std::string msg);


	void		print_clients();
	void		print();
};
