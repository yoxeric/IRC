
#include "inc/ft_irc.hpp"

void get_targets(std::string str, std::vector<std::string> &target)
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
		//type.push_back(1 * (target.back().at(0) == '#') + 1);
		//if(type.back() == 2)
	    //    target.back() = target.back().substr(1, target.back().length() - 1);
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

/*
int is_not_space(char let)
{
	return !std::isspace(let);
}*/

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

/*
Connection Registration

CAP LS 302
PASS
NICK and USER
Capability Negotiation
SASL (if negotiated)
CAP END
std::string parse_registration(Server& server, int sender_socket, std::string buffer)
*/


std::string parse(Server& server, int sender_socket, std::string buffer)
{
	// int index = server.find_client(sende   r_socket);
	// Client &sender = server.clients[ index ];
	Client &sender = *server.find_client(sender_socket);
	std::cout << "----- sender = " << sender.get_nickname() << std::endl;
	sender.print();

	if(buffer.front() == ' ')
		buffer = buffer.substr(buffer.find_first_not_of(" "), buffer.length());
	if(buffer.back() == '\r')
		buffer.pop_back();
	std::istringstream input(buffer);
	std::string comand;
	getline(input, comand, ' ');
	
	buffer.erase(0,comand.size());
	if(buffer.front() == ' ')
		buffer = buffer.substr(buffer.find_first_not_of(" "), buffer.length());

		/*
NICK ohachami
PASS password
USER ohachami ohachami 127.0.0.1 :realname
JOIN #chan
TOPIC #chan :semting
		*/

	if (!comand.compare("PASS")) //done
		server.pass(sender, buffer);
	else if (!comand.compare("CAP")) //done
		server.cap(sender, buffer);
	else if (!comand.compare("USER") && server.get_registred() > 0)  //done
		server.user(sender, buffer);
	else if (!comand.compare("NICK") && server.get_registred() > 0) // done
		server.nick(sender, buffer);
	else if (server.get_registred() < 3) // done
		server.send_err(421, sender, "You have not registered");
	else if (!comand.compare("WHO")) //done
		server.who(sender, buffer);
	else if (!comand.compare("PING")) //done
		server.ping(sender, buffer);
	else if (!comand.compare("PRIVMSG")) //done
		server.prvmsg(sender, buffer);
	else if (!comand.compare("LIST"))
		server.list(sender);
	else if (!comand.compare("MODE"))
		server.mode(sender, buffer);
	else if (!comand.compare("JOIN"))
		server.join(sender, buffer);
	else if (!comand.compare("QUIT")) //done not tested
		server.quit(sender, buffer);
	else if (!comand.compare("KICK")) // done not tested
		server.kick(sender, buffer);
	else if (!comand.compare("INVITE")) // done  not tested
		server.invite(sender, buffer); 
	else if (!comand.compare("TOPIC")) // done
		server.topic(sender, buffer);
	else if (!comand.compare("OPER")) //done
		server.oper(sender, buffer);
	else if (!comand.compare("PART")) //done 
		server.part(sender, buffer);
	else
		server.send_err(421, sender, comand, "Unknown command");
	return ("");
}
