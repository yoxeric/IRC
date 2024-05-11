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
	std::vector<Client> operators;
	std::vector<Client> invited;


	void		add_membre(Client& client);
	int			remove_membre(Client& client);
	int			is_membre(Client& client);
	int			is_membre(std::string name);

	int			count_membres();

	void		add_operator(Client& client);
	int			remove_operator(Client& client);
	int			is_operator(Client& client);

	void		add_invited(Client& client);
	int			remove_invited(Client& client);
	int			is_invited(Client& client);


	// ----------------------  Mode -----------------------


	void		add_mode(char m);
	void		remove_mode(char m);
	int			is_mode(char m);


	// ----------------------  Setter -----------------------


	void		set_mode(std::string str);
	void		set_name(std::string str);
	void		set_topic(std::string str);
	void		set_key(std::string str);
	void		set_limit(int limit);


	// ----------------------  Getter -----------------------


	std::string	get_mode();
	std::string	get_name();
	std::string	get_topic();
	std::string	get_key();
	int			get_limit();


	// ----------------------  Debug -----------------------


	void 		print();

};