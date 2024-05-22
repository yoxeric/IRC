
#include "inc/Server.hpp"

void get_targets(std::string str, std::vector<std::string> &target, std::vector<int> &type)
{
	std::string arg;
	std::istringstream sub_input(str);
	getline(sub_input, arg, ',');
	for (int i = 0; i < (std::count(str.begin(), str.end(), ',') + 1);i++)
	{
		if(!arg[0])
		{
			getline(sub_input, arg, ',');
			continue;
		}
		target.push_back(arg);
		type.push_back(1 * (target.back().at(0) == '#') + 1);
		if(type.back() == 2)
	        target.back() = target.back().substr(1, target.back().length() - 1);
		getline(sub_input, arg, ',');
	}
}

/*
CAP LS
NICK yh1
USER yh2 yh2 irc.data.lt :realname
CAP REQ :account-notify away-notify multi-prefix userhost-in-names
CAP END
PRIVMSG ,Angel,,,Devil, :Hi! I have a problem!
PRIVMSG #Angel :Hi! I have a problem!
PRIVMSG #Angel,Devil :Hi! :I have :a problem!
*/

int is_not_space(char let)
{
	return !std::isspace(let);
}

/*void get_raw(std::string &str)
{
	//int i = 1;
	std::istringstream input(str);
}*/

std::string getword(std::string str, int start)
{
	std::size_t i;
	std::size_t j;
	std::string word;

	i = str.find(" ", start);
	j = str.find("\n", start);
	if (i != std::string::npos || j != std::string::npos)
	{
		if (j < i)
			i = j;
		word = str.substr(start, i - start);
		// std::cout << start << " - " << i << " word :" << word << std::endl;
	}

	if (word.back() == '\r')
		word.pop_back();

	return (word);
}

// To get what command to call, do a map with a string as a key (command name) and a pointer to function (it’s cool)

std::string Server::parse(int sender_socket, std::string buffer)
{
	// int index = find_client(sende   r_socket);
	// Client &sender = server.clients[ index ];
	Client &sender = *find_client(sender_socket);
	std::cout << "--- command line = " << buffer << "\n";
	std::cout << "----- sender = " << sender.get_nickname() << std::endl;
	sender.print();

	if(buffer.front() == ' ')
		buffer = buffer.substr(buffer.find(" "), buffer.length());
	if(buffer.back() == '\r')
		buffer.pop_back();
	//else
	//	return ("ERR_UNKNOWNCOMMAND");
	std::istringstream input(buffer);
	std::string comand;
	getline(input, comand, ' ');

	if (!comand.compare("CAP")) //done
	{
		std::string subcommand;
		getline(input, subcommand, ' ');
		if(subcommand.empty())
		{
			send_err(461, sender, "CAP", "Not enough parameters");
			return("ERR_NEEDMOREPARAMS");
		}
		// std::cout << "-----------------------." << subcommand << ". " << '\n';
		if(!subcommand.compare("END") || !subcommand.compare("LIST") || !subcommand.compare("LS"))
			cap(sender, subcommand);
		else if(!subcommand.compare("REQ"))
		{
			std::string option;
			getline(input, option, ' ');

			if(option.front() != ':')
			{
				send_err(500, sender, "CAP", "invalid command");
				return("ERR_INVALIDCAPCMD");
			}
			cap(sender, buffer.substr(buffer.find(":"), buffer.length() - buffer.find(":")));

		}
		// else
		// 	return("ERR_INVALIDCAPCMD");
	}
	else if (!comand.compare("PASS"))
	{
		if (!sender.get_pass().empty())
		{
			send_err(462, sender, "You may not reregister");
			return("ERR_ALREADYREGISTERED");
		}
		std::string password;
		getline(input, password, ' ');

		pass(sender, password);
	}
	else if (!comand.compare("NICK")) // done
	{

		std::string nickname;
		getline(input, nickname, ' ');
		if(nickname.empty())
		{
			send_err(431, sender, "No nickname given");
			return("ERR_NONICKNAMEGIVEN");
		}
		if(nickname.find_first_not_of("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[]{}/|_-^") != std::string::npos)
		{			
			send_err(432, sender, nickname, "Erroneus nickname");
			return("ERR_ERRONEUSNICKNAME");
		}
		nick(sender, nickname);
	}
	else if (!comand.compare("USER"))  //done
	{
	// << USER username username us.undernet.org :realname
	/*
		CAP LS
		NICK ohachami
		USER ohachami ohachami 127.0.0.1 :realname
	*/
		if (!sender.get_username().empty())
		{
			send_err(462, sender, "You may not reregister");
			return("ERR_ALREADYREGISTERED");
		}
		std::string username;
		getline(input, username, ' ');

		std::string second_param;
		getline(input, second_param, ' ');

		std::string third_param;
		getline(input, third_param, ' ');

		std::string last_param;
		getline(input, last_param, ' ');

		if(third_param.empty() || third_param.find(':') != std::string::npos ||
			second_param.empty() || second_param.find(':') != std::string::npos ||
			username.empty() || username.find(':') != std::string::npos ||
			last_param.front() != ':' || last_param.size() < 2)
		{
				send_err(461, sender, "USER", "Not enough parameters");
				return("ERR_NEEDMOREPARAMS");
		}
		std::string realname = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
		user(sender, username, second_param, third_param, realname);
	}
	else if (!comand.compare("WHO")) //done
	{
		std::string token;
		getline(input, token, ' ');
		who(sender, token);
	}
	else if (!comand.compare("PING")) //done
	{
		std::string token;
		getline(input, token, ' ');
		//token.pop_back();
		if(token.empty() || !token.compare(":"))
			return("ERR_NOORIGIN");
		if(token.front() == ':')
			token.erase(0,1);
		ping(sender, token);
		
	}
	else if (!comand.compare("PRIVMSG") || !comand.compare("NOTICE")) //done
	{
		std::string all_targets;
		getline(input, all_targets, ' ');
		if(all_targets.empty() || all_targets.find_first_not_of(',') == std::string::npos)
			return("ERR_NORECIPIENT");
		if(all_targets.find(',') != std::string::npos)
			all_targets = all_targets.substr(all_targets.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
		std::vector<std::string> target;
		std::vector<int> type;
		std::string msg;
		getline(input, msg, ' ');

		if(msg.front() != ':')
		{
			send_err(412, sender, "No text to send");
			return("ERR_NOTEXTTOSEND");
		}
		msg = buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		if(msg.empty())
		{
			send_err(412, sender, "No text to send");
			return("ERR_NOTEXTTOSEND");
		}
		get_targets(all_targets, target, type);
		if(target.size() == 0)
			return("ERR_NORECIPIENT");
		prvmsg(sender, target, type, msg);
	}
	else if (!comand.compare("LIST")) //done
		list(sender);
	else if (!comand.compare("MODE"))
	{
		std::string target;
		getline(input, target, ' ');

		std::string modestr;
		getline(input, modestr, ' ');

		std::string arg;
		getline(input, arg, ' ');

		mode(sender, target, modestr, arg);
	}
	else if (!comand.compare("JOIN"))
	{
		std::string all_targets;
		getline(input, all_targets, ' ');
		std::cout << "--------------------" << all_targets << "." << all_targets.find_first_not_of(',') << "." << std::endl;
		if(all_targets.empty() || all_targets.find_first_not_of(',') == std::string::npos)
			return("ERR_NORECIPIENT");
		if(all_targets.find(',') != std::string::npos)
			all_targets = all_targets.substr(all_targets.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
		//std::vector<std::string> target;
		// get_targets(all_targets, target, type);
		// if(target.size() == 0)
		//	return("ERR_NORECIPIENT");

		std::string all_keys;
		getline(input, all_keys, ' ');
		//if(all_keys.empty() || all_keys.find_first_not_of(',') == std::string::npos)
		//	return("ERR_NORECIPIENT");
		if(all_keys.find(',') != std::string::npos)
			all_keys = all_keys.substr(all_keys.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
		//std::vector<std::string> key;
		// get_targets(all_keys, key, type);
		// if(key.size() == 0)
		//	return("ERR_NORECIPIENT");

		join(sender, all_targets, all_keys);
  	}
	else if (!comand.compare("QUIT")) //done
	{
		std::string reason;
		getline(input, reason, ' ');
		//reason.pop_back();
		if(reason.front() != ':' )
		{
			send_err(431, sender, "No nickname given");
			return("ERR_NONICKNAMEGIVEN");
		}
		quit(sender, buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":")));
	}
	else if (!comand.compare("KICK"))
	{
		std::string chan;
		getline(input, chan, ' ');

		std::string user;
		getline(input, user, ' ');

		kick(sender, chan, user);
	}
	else if (!comand.compare("INVITE"))
	{
		std::string chan;
		getline(input, chan, ' ');

		std::string user;
		getline(input, user, ' ');

		invite(sender, chan, user);
	}
	else if (!comand.compare("TOPIC"))
	{
		std::string chan;
		getline(input, chan, ' ');

		std::string topic_str;
		getline(input, topic_str, ' ');
		if(topic_str.front() != ':')
			return("ERR");
		topic_str = buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		if(topic_str.empty())
			return("ERR");
		topic(sender, chan, topic_str);
	}
	else if (!comand.compare("OPER"))
	{
		std::string name;
		getline(input, name, ' ');

		std::string password;
		getline(input, password, ' ');

		oper(sender, name, password);
	}
	else if (!comand.compare("PART"))
	{
		std::string all_chans;
		getline(input, all_chans, ' ');
		if(all_chans.empty() || all_chans.find_first_not_of(',') == std::string::npos)
			return("ERR_NORECIPIENT");
		if(all_chans.find(',') != std::string::npos)
			all_chans = all_chans.substr(all_chans.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
		// std::vector<std::string> target;
		// std::vector<int> type;
		std::string msg;
		getline(input, msg, ' ');
		if(msg.front() != ':')
			msg = buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		 // get_targets(all_targets, target, type);
		part(sender, all_chans, msg);
	}
	else
		return ("ERR_UNKNOWNCOMMAND");
	return ("");
}
