#pragma once

#include <iostream>
#include <sstream> 
#include <vector>
#include <iomanip>

#include "poll.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class IRCServer
{
private:
	std::string			password;
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

	void				init_server(char *);
	const	std::string	get_password();
	// ----------------------  Channel -----------------------

	Channel*	add_channel(std::string name);
	Channel*	find_channel(std::string name);
	// void		remove_channel(std::string name);
	void		remove_channel(Channel &chan);

	int			channel_count();

	// ----------------------  Client -----------------------


	Client*		add_client(int socket);
	Client*		find_client(int socket);
	Client*		find_client(std::string name);
	void		remove_client(Client &client);

	int			client_count();
	int			operator_count();

	// ----------------------  Messages -----------------------


	// int			send_msg_client(Client &client, std::string msg);
	int			send_msg_channel(Client& sender, Channel &chan, std::string msg);
	int			send_msg(int dest_fd, std::string msg);

	std::string	create_tag(Client& client);
	void		send_reply(int code, Client &client, std::string arg, std::string msg);


	// ----------------------  Notices -----------------------

	void		welcome_server(Client& client);
	void		modt_server(Client& client);
	void 		list_server(Client &client);
	void		list_channel(Client& client, Channel &chan);
	void		list_channel_short(Client& client, Channel &chan);
	void		list_user(Client& client, Client &target_client);


	void		send_err(int code, Client &sender, std::string msg);
	void		send_err(int code, Client &sender, std::string arg1, std::string msg);
	void		send_err(int code, Client &sender, std::string arg1, std::string arg2, std::string msg);
	void		send_err(int code, Client &sender, std::string arg1, std::string arg2, std::string arg3, std::string msg);

	// ----------------------  Commands -----------------------

	void 		list(Client &client);
	void 		who(Client& client, std::string target);

	void 		cap(Client &client, std::string str);
	void 		nick(Client &client, std::string nick);
	void 		user(Client &client, std::string user, std::string adrr);
	void		prvmsg(Client& client, std::vector<std::string> target, std::vector<int> type, std::string msg);
	void 		join(Client &client, std::string chan_name, std::string key);
	void 		part(Client &client, std::string chan_name, std::string msg);

	void 		mode(Client& client, std::string target, std::string mode, std::string arg);
	
	void 		topic(Client &client, std::string target, std::string topic);
	void 		kick(Client &client, std::string chan_name, std::string target);
	void 		invite(Client &client, std::string chan_name, std::string target);

	void 		ping(Client &client, std::string msg);

	void 		quit(Client &client, std::string str);


	// ----------------------  Debug -----------------------

	void		print();
};
