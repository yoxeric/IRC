
#include "inc/ft_irc.hpp"

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

std::string parse(Server& server, int sender_socket, std::string buffer)
{
	// int index = server.find_client(sende   r_socket);
	// Client &sender = server.clients[ index ];
	Client &sender = *server.find_client(sender_socket);
	std::cout << "----- sender = " << sender.get_nickname() << std::endl;
	sender.print();

	if(buffer[0] == ' ')
		buffer = buffer.substr(buffer.find(" "), buffer.length());
	if(buffer.back() == '\n')
		buffer.pop_back();
	std::istringstream input(buffer);
	std::string comand;
	getline(input, comand, ' ');

	if (!comand.compare("CAP")) //done
	{
		std::string subcommand;
		getline(input, subcommand, ' ');
		if(!subcommand[0])
			return("ERR_NEEDMOREPARAMS");
		if(!subcommand.compare("END") || !subcommand.compare("LIST") || !subcommand.compare("LS"))
			server.cap(sender, subcommand);
		else if(!subcommand.compare("REQ"))
		{
			std::string option;
			getline(input, option, ' ');
			if(option[0] != ':')
				return("ERR_INVALIDCAPCMD");
			server.cap(sender, buffer.substr(buffer.find(":"), buffer.length() - buffer.find(":")));
		}
		else
			return("ERR_INVALIDCAPCMD");
	}
	else if (!comand.compare("NICK")) // done
	{
		std::string nick;
		getline(input, nick, ' ');
		if(!nick[0])
			return("ERR_NONICKNAMEGIVEN");
		if(nick.find_first_not_of("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[]{}/|_-^") != std::string::npos)
			return("ERR_ERRONEUSNICKNAME");
		server.nick(sender, nick);

	}
	else if (!comand.compare("USER"))  //done
	{
	// << USER username username us.undernet.org :realname
	/*
		CAP LS
		NICK ohachami
		USER ohachami ohachami 127.0.0.1 :realname
	*/
		std::string username;
		getline(input, username, ' ');

		std::string second_param;
		getline(input, second_param, ' ');

		std::string third_param;
		getline(input, third_param, ' ');

		std::string last_param;
		getline(input, last_param, ' ');

		if(!third_param[0] || third_param.find(':') != std::string::npos ||
			!second_param[0] || second_param.find(':') != std::string::npos ||
			!username[0] || username.find(':') != std::string::npos ||
			last_param[0] != ':' || last_param.size() < 2)
				return("ERR_NEEDMOREPARAMS");
		std::string realname = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
		server.user(sender, username, second_param, third_param, realname);
	}
	else if (!comand.compare("WHO")) //done
	{
		std::string token;
		getline(input, token, ' ');
		server.who(sender, token);
	}
	else if (!comand.compare("PING")) //done
	{
		std::string token;
		getline(input, token, ' ');
		//token.pop_back();
		if(!token[0] || !token.compare(":"))
			return("ERR_NOORIGIN");
		if(token[0] == ':')
			token.erase(0,1);
		server.ping(sender, token);
	}
	else if (!comand.compare("PRIVMSG") || !comand.compare("NOTICE")) //done
	{
		std::string all_targets;
		getline(input, all_targets, ' ');
		if(!all_targets[0] || all_targets.find_first_not_of(',') == std::string::npos)
			return("ERR_NORECIPIENT");
		if(all_targets.find(',') != std::string::npos)
			all_targets = all_targets.substr(all_targets.find_first_not_of(',') , buffer.length() - buffer.find_first_not_of(','));
		std::vector<std::string> target;
		std::vector<int> type;
		std::string msg;
		getline(input, msg, ' ');
		if(msg[0] != ':')
			return("ERR_NOTEXTTOSEND");
		msg = buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		if(!msg[0])
			return("ERR_NOTEXTTOSEND");
		get_targets(all_targets, target, type);
		if(target.size() == 0)
			return("ERR_NORECIPIENT");
		server.prvmsg(sender, target, type, msg);
	}
	else if (!comand.compare("QUIT")) //done
	{
		std::string reason;
		getline(input, reason, ' ');
		reason.pop_back();
		if(reason[0] != ':' )
			return("ERR_NONICKNAMEGIVEN");
		server.quit(sender, buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":")));
	}
	else if (!comand.compare("LIST")) //done
		server.list(sender);
	else if (!comand.compare("MODE"))
	{
		std::string target;

		std::string mode;

		server.mode(sender, target, mode);
	}
	else if (!comand.compare("JOIN"))
	{
		std::string ch;
		server.join(sender, ch);

	}
	else
		return ("ERR_UNKNOWNCOMMAND");
	return ("");
}
