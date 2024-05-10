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
	std::string mode;
	
	int limit;

public:
	std::vector<Client> members;
	std::vector<std::string> operators;
	std::vector<std::string> invited;


	void 		print();

	void		add_membre(Client& client);
	void		remove_membre(Client& client);
	int			is_membre(Client& client);

	int			count_membres();

	int			is_mode(char m);
	std::string	get_mode();

	std::string	get_name();
	std::string	get_topic();
	std::string	get_key();
	int			get_limit();

	void		set_mode(std::string str);
	void		add_mode(char m);
	void		remove_mode(char m);

	void		set_name(std::string str);
	void		set_topic(std::string str);
	void		set_key(std::string str);
	void		set_limit(int limit);
};