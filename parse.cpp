
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

std::string Server::parse(int sender_socket, std::string buffer)
{
	// int index = find_client(sende   r_socket);
	// Client &sender = server.clients[ index ];
	Client &sender = *find_client(sender_socket);
	size_t i = 0;
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
			send_err(461, sender, "CAP", "Not enough parameters");
			// return("ERR_NEEDMOREPARAMS");
		if(!subcommand.compare("END") || !subcommand.compare("LIST") || !subcommand.compare("LS"))
			cap(sender, subcommand);
		else if(!subcommand.compare("REQ"))
		{
			std::string option;
			getline(input, option, ' ');
			if(option[0] != ':')
				send_err(500, sender, "CAP", "invalid command");
				// return("ERR_INVALIDCAPCMD");
			cap(sender, buffer.substr(buffer.find(":"), buffer.length() - buffer.find(":")));
		}
		// else
		// 	return("ERR_INVALIDCAPCMD");
	}
	if (!comand.compare("NICK")) // done
	{
		std::string nickname;
		getline(input, nickname, ' ');
		if(!nickname[0])
			send_err(431, sender, "No nickname given");
			// return("ERR_NONICKNAMEGIVEN");
		if(nickname.find_first_not_of("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[]{}/|_-^") != std::string::npos)
			send_err(432, sender, nickname, "Erroneus nickname");
			// return("ERR_ERRONEUSNICKNAME");
		nick(sender, nickname);

	}
	if (!comand.compare("USER"))  //done
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
				send_err(461, sender, "USER", "Not enough parameters");
				// return("ERR_NEEDMOREPARAMS");
		std::string realname = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":"));
		user(sender, username, second_param, third_param, realname);
	}
	if (!comand.compare("WHO")) //done
	{
		std::string token;
		getline(input, token, ' ');
		who(sender, token);
	}
	if (!comand.compare("PING")) //done
	{
		std::string token;
		getline(input, token, ' ');
		//token.pop_back();
		if(!token[0] || !token.compare(":"))
			return("ERR_NOORIGIN");
		if(token[0] == ':')
			token.erase(0,1);
		ping(sender, token);
		
	}

	if (!comand.compare("PRIVMSG") || !comand.compare("NOTICE")) //done
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
			send_err(412, sender, "No text to send");
			// return("ERR_NOTEXTTOSEND");
		msg = buffer.substr(buffer.find(":") + 1, buffer.length() - 1 - buffer.find(":"));
		if(!msg[0])
			send_err(412, sender, "No text to send");
			// return("ERR_NOTEXTTOSEND");
		get_targets(all_targets, target, type);
		if(target.size() == 0)
			return("ERR_NORECIPIENT");
		prvmsg(sender, target, type, msg);
	}
	if (!comand.compare("LIST")) //done
		list(sender);
	if (!comand.compare("MODE"))
	{
		std::string target = getword(buffer, i + 5);

		std::string modestr = "";
		i = buffer.find(" ", i + 5);
		if (i != std::string::npos)
			modestr = getword(buffer, i + 1);

		std::string arg = "";
		i = buffer.find(" ", i + 1);
		if (i != std::string::npos)
			arg = getword(buffer, i + 1);

		mode(sender, target, modestr, arg);
	}
	if (!comand.compare("JOIN"))
	{
			std::string ch = getword(buffer, i + 5);

		std::string key = "";
		i = buffer.find(" ", i + 5);
		if (i != std::string::npos)
			key = getword(buffer, i + 1);

		join(sender, ch, key);
  }
	if (!comand.compare("QUIT")) //done
	{
		std::string reason;
		getline(input, reason, ' ');
		reason.pop_back();
		if(reason[0] != ':' )
			send_err(431, sender, "No nickname given");
			// return("ERR_NONICKNAMEGIVEN");
		quit(sender, buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":")));
	}
	/*else
		return ("ERR_UNKNOWNCOMMAND");*/
  
	i = buffer.find("KICK ");
	if (i != std::string::npos)
	{
		std::string chan = getword(buffer, i + 5);

		i = buffer.find(" ", i + 5);
		std::string user = getword(buffer, i + 1);

		kick(sender, chan, user);
	}

	i = buffer.find("INVITE ");
	if (i != std::string::npos)
	{
		std::string user = getword(buffer, i + 7);

		i = buffer.find(" ", i + 7);
		std::string chan = getword(buffer, i + 1);

		invite(sender, chan, user);
	}

	i = buffer.find("TOPIC ");
	if (i != std::string::npos)
	{
		std::string chan = getword(buffer, i + 6);

		i = buffer.find(":", i + 6);
		std::string topicstr = buffer.substr(i + 1, buffer.length() - i - 3);

		topic(sender, chan, topicstr);
	}
  
	i = buffer.find("PART ");
	if (i != std::string::npos)
	{
		std::string chan = getword(buffer, i + 5);

		i = buffer.find(":", i + 5);
		std::string msg = getword(buffer, i + 1);

		part(sender, chan, msg);
	}

	i = buffer.find("PING ");
	if (i != std::string::npos)
	{
		std::string msg = getword(buffer, i + 5);

		ping(sender, msg);
	}
  
	return ("");
}
