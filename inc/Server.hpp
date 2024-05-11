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


	// std::string parse (int sender_socket, std::string buffer);

	// ----------------------  Server -----------------------

	void		init_server();
	void		welcome_server(Client& client);
	void		modt_server(Client& client);
	void 		list_users(Client &client);
	void 		list(Client &client);


	// ----------------------  Channel -----------------------


	Channel*	add_channel(std::string name);
	Channel*	find_channel(std::string name);
	// void		remove_channel(std::string name);
	void		remove_channel(Channel &chan);

	int			do_channel_exist(std::string chan_name);


	// ----------------------  Client -----------------------


	Client*		add_client(int socket);
	Client*		find_client(int socket);
	Client*		find_client(std::string name);
	void		remove_client(Client &client);

	int			client_count();
	int			operator_count();
	int			channel_count();


	// ----------------------  Commands -----------------------


	void 		cap(Client &client, std::string str);
	void 		nick(Client &client, std::string nick);
	void 		user(Client &client, std::string user, std::string adrr);
	void		prvmsg(Client& client, std::vector<std::string> target, std::vector<int> type, std::string msg);
	void 		join(Client &client, std::string chan_name);
	void 		who(Client& client, std::string target);
	void 		topic(Client &client, std::string str);

	void 		mode(Client& client, std::string target, std::string mode);
	void 		kick(Client &client, std::string str);
	void 		invite(Client &client, std::string str);

	void 		quit(Client &client, std::string str);


	// ----------------------  Messages -----------------------


	// int			send_msg_client(Client &client, std::string msg);
	int			send_msg_channel(Client& sender, Channel &chan, std::string msg);
	int			send_msg(int dest_fd, std::string msg);

	std::string	create_tag(Client& client);
	// std::string	create_reply(int code, std::string nick, std::string arg, std::string msg);
	void		send_reply(int code, Client &client, std::string arg, std::string msg);


	// ----------------------  Debug -----------------------


	void		print_clients();
	void		print();
};
