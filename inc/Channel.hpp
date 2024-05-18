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

	/**
	 * Sum numbers in a vector.
	 *
	 * @param client Container whose values are summed.
	 * @return sum of `values`, or 0.0 if `values` is empty.
	 */
	void		add_membre(Client& client);
	int			remove_membre(Client& client);
	int			is_membre(Client& client);

	int			count_membres();


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