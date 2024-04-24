#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <sstream> 
#include <vector>

class s_client
{
public:
	std::string nickname;
	std::string username;
	std::string address;
	std::string pass;

	std::vector<std::string> chan; // channels this user is part of
	std::vector<std::string> chanops; // channels this user is operatoring

	void cap(std::string str);
	void nick(std::string str);
	void user(std::string str, std::string adrr);
	void join(std::string str, int serv_fd);
	void mode(std::string str, int serv_fd);
	void who(std::string str, int serv_fd);
	void prvmsg(std::string str, int serv_fd);
	void quit(std::string str, int serv_fd);

	void print();

};
