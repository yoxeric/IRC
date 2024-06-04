#pragma once

#include <iostream>
#include <sstream> 
#include <vector>

class Client
{
private:
	std::string nickname;
	std::string username;
	std::string hostname;
	std::string realname;
	std::string address;
	std::string pass;
	std::string mode;

	// std::stringstream message;
	// int			destination;

	int 		socket;
	int			registred;

public:

	Client();
	~Client();

	// ----------------------  Mode -----------------------


	void		add_mode(char m);
	void		remove_mode(char m);
	int			is_mode(char m);


	// ----------------------  Setter -----------------------

	void		set_mode(std::string str);
	void 		set_nickname(std::string str);
	void 		set_username(std::string str);
	void 		set_hostname(std::string str);
	void 		set_realname(std::string str);
	void 		set_address(std::string str);
	void 		set_pass(std::string str);

	// void 		set_message(std::string str);
	// void 		reset_message(std::string str);
	// void 		set_destination(int fd);

	void 		set_socket(int socket);
	void		set_registred(int i);
	void		add_registred(int i);

	// ----------------------  Getter -----------------------

	std::string	get_mode();
	std::string get_nickname();
	std::string get_username();
	std::string get_hostname();
	std::string get_realname();
	std::string get_address();
	std::string get_pass();

	// std::string get_message();
	// int			get_destination();

	int 		get_socket();
	int			get_registred();

	// ----------------------  Debug -----------------------

	void 		print();

};
