#pragma once

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

	std::vector<std::string> chan;
	std::vector<std::string> chanops;

	void cap(std::string str);
	void nick(std::string str);
	void user(std::string str, std::string adrr);
	void join(std::string str);

	void print();

};
