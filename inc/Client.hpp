#pragma once

#include <iostream>
#include <sstream> 
#include <vector>

class Client
{
private:
	std::string	message;
	std::string nickname;
	std::string username;
	std::string realname;
	std::string address;
	std::string pass;
	std::string mode;

	int 		socket;

public:


	// ----------------------  Mode -----------------------


	void		add_mode(char m);
	void		remove_mode(char m);
	int			is_mode(char m);


	// ----------------------  Setter -----------------------

	void		set_mode(std::string);
	void 		set_nickname(std::string);
	void 		set_username(std::string);
	void 		set_realname(std::string);
	void 		set_address(std::string);
	void 		set_pass(std::string);
	void		set_message(std::string);
	void 		set_socket(int);

	// ----------------------  Getter -----------------------

	std::string	get_mode();
	std::string get_nickname();
	std::string get_username();
	std::string get_realname();
	std::string get_address();
	std::string get_pass();

	int 		get_socket();

	// ----------------------  Debug -----------------------

	void 		print();

};
