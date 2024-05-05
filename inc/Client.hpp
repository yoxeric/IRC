#pragma once

#include <iostream>
#include <sstream> 
#include <vector>

class Client
{
private:
	std::string nickname;
	std::string username;
	std::string address;
	std::string pass;

	int 		socket;

public:

	void 		print();

	void 		set_socket(int socket);
	int 		get_socket();

	std::string get_nickname();
	std::string get_username();
	std::string get_address();
	std::string get_pass();


	void		get_socket(int socket);

	void 		set_nickname(std::string str);
	void 		set_username(std::string str);
	void 		set_address(std::string str);
	void 		set_pass(std::string str);
};
