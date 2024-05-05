#pragma once

#include <iostream>
#include <sstream> 
#include <vector>

#include "Client.hpp"

class Channel
{
private:
	std::string name;
	std::string topic;
	std::string key;
	std::string limit;

public:
	std::vector<Client> members;
	std::vector<Client> operators;
	std::vector<Client> invited;


	void add_membre(Client client);
	int is_membre(Client client);

	std::string get_name();

	void set_name(std::string str);
};