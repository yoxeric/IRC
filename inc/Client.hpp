#pragma once

#include <iostream>
#include <sstream> 
#include <vector>

class Client
{
private:
	std::string nickname;
	std::string username;
	std::string realname;
	std::string address;
	std::string pass;
	std::string mode;

	int 		socket;
	bool		is_operator;

public:

	void 		print();

	int			is_mode(char m);
	std::string	get_mode();

	int 		get_socket();
	bool		get_operator();
	std::string get_nickname();
	std::string get_username();
	std::string get_realname();
	std::string get_address();
	std::string get_pass();

	void		set_mode(std::string str);
	void		add_mode(char m);
	void		remove_mode(char m);

	void 		set_socket(int socket);
	void		set_operator(bool val);
	void 		set_nickname(std::string str);
	void 		set_username(std::string str);
	void 		set_realname(std::string str);
	void 		set_address(std::string str);
	void 		set_pass(std::string str);
};
