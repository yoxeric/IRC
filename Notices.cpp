
#include "inc/Server.hpp"


void	Server::welcome_server(Client& client)
{
	std::stringstream ss;

	ss << "Welcome to the " << networkname << " IRC Network " <<  create_tag(client);
	send_reply(001, client, "", ss.str());

	ss.str("");

	ss << "Your host is " << servername << " running version " << version;
	send_reply(002, client, "", ss.str());


	ss.str("");

	ss << "This server was created " << datetime;
	send_reply(003, client, "", ss.str());

	list_server(client);
	modt_server(client);

}

void	Server::list_server(Client& client)
{
	std::stringstream ss;

	ss << "There are " << client_count() << " users on " << 1 << " servers " << datetime;
	send_reply(251, client, "", ss.str());

	ss.str("");

	ss << operator_count();
	send_reply(252, client, ss.str(), "operator(s) online");

	ss.str("");

	ss << channel_count();
	send_reply(254, client, ss.str(), "channels formed");

	ss.str("");

	ss << "I have " << client_count() << " clients and " << 1 << " servers " << datetime;
	send_reply(255, client, "", ss.str());

}

void	Server::modt_server(Client& client)
{

	send_reply(375, client, "", "- ScaleFactor.ma Message of the Day - ");

	// send_reply(372, client, "", "<!--          .-.           .-             .-.                         -->");
	// send_reply(372, client, "", "<!--    .--.-'             /              / -'           /             -->");
	// send_reply(372, client, "", "<!--   (  (_).-.  .-.     /   .-.     . -/--.-.  .-. ---/---.-._.).--. -->");
	// send_reply(372, client, "", "<!--    `-. (    (  |    /  ./.-'_   /  /  (  | (      /   (   )/      -->");
	// send_reply(372, client, "", "<!--  _    ) `---'`-'-'_/_.-(__.'    `.'    `-'-'`---'/     `-'/       -->");
	// send_reply(372, client, "", "<!-- (_.--'                                                            -->");

	// send_reply(372, client, "", "<!--              # #                      .-                                    .-   -->");
	// send_reply(372, client, "", "<!--    .---.     .-.    /      /         /  .        /           /             /  .  -->");
	// send_reply(372, client, "", "<!--   (     '   (  )   /      /.-.  #   /  /   .    /    .-.    /    .- -.    /  /   -->");
	// send_reply(372, client, "", "<!--    `-.  (     /   /     /    ) /   /  /   (    /    (   )  /     |  /    /  /    -->");
	// send_reply(372, client, "", "<!-- (     )  `---'   ' ----'`---- '---'  /    '._ /  ---'`--' / ___ _| / __ '  /     -->");
	// send_reply(372, client, "", "<!--  '---'                              /                                     /      -->");

	send_reply(372, client, "", "<!--          # #                       .-                                  -->");
	send_reply(372, client, "", "<!--         .-.     /      /          /  .-         /           /          -->");
	send_reply(372, client, "", "<!--  .     (   )   /      /.-.   #   /  /     .    /    .-.    /           -->");
	send_reply(372, client, "", "<!-- (         /   /     /    )  /   /  /     (    /    (   )  /       .-.  -->");
	send_reply(372, client, "", "<!--  '-------'   ' ----'`---- -'---'  /      '._ /  ---'`--' / ___ __(___  -->");
	send_reply(372, client, "", "<!--                                  /                                     -->");
	send_reply(372, client, "", "- ");
	send_reply(372, client, "", "-  Welcome to the ScaleFactor.ma server of the Scale Factor IRC network ");
	send_reply(372, client, "", "-  ");
	send_reply(372, client, "", "-  The server is designed for quiet communication.");
	send_reply(372, client, "", "-  other use not according to the intended purpose will not be tolerated   ");
	send_reply(372, client, "", "-  You cannot change the network rules with any command !   ");
	send_reply(372, client, "", "-  ");

	send_reply(376, client, "", "End of /MOTD command.");

}

void	Server::list_channel(Client& client, Channel &chan)
{
	std::stringstream ss;

	ss << "#" << chan.get_name();
	send_reply(322, client, ss.str(), chan.get_topic());

	// send_reply(366, client, "", "End of /NAMES list.");

	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		ss.str("");
		ss << "#" << chan.get_name() << " ~" << it->get_username() << " " << it->get_address() << ".IP " << servername
		<< " " << it->get_nickname() << " " <<  it->get_mode();
		send_reply(352, client, ss.str(), it->get_realname());
	}
	
	send_reply(323, client, "", "End of /WHO list");

}

void	Server::list_user(Client& client, Client &target_client)
{
	std::stringstream ss;

	ss << "~" << target_client.get_username() << " " << target_client.get_address() << ".IP " << servername
	<< " " << target_client.get_nickname() << " " <<  target_client.get_mode();
	send_reply(352, client, ss.str(), target_client.get_realname());

	send_reply(323, client, "", "End of /WHO list");

}


// ------------------------------------- Error --------------------------------------------------------------------------


void	Server::send_err(int code, Client &sender, std::string arg1, std::string msg)
{
	std::stringstream s;

	s << ":" << servername << " " << code << " " << sender.get_nickname() << " " << arg1 << " :" << msg << "\n";

	send_msg(sender.get_socket(), s.str());
}


void	Server::send_err(int code, Client &sender, std::string arg1, std::string arg2, std::string msg)
{
	std::stringstream s;
	
	s << ":" << servername << " " << code << " " << sender.get_nickname() << " " << arg1 << " " << arg2 << " :" << msg << "\n";

	send_msg(sender.get_socket(), s.str());
}


void	Server::send_err(int code, Client &sender, std::string arg1, std::string arg2, std::string arg3, std::string msg)
{
	std::stringstream s;

	// if (arg2.empty() && arg3.empty() )
	// 	s << ":" << servername << " " << code << " " << sender.get_nickname() << " " << arg1 << " :" << msg << "\n";
	// else if (arg3.empty())
	// 	s << ":" << servername << " " << code << " " << sender.get_nickname() << " " << arg1 << " " << arg2 << " :" << msg << "\n";
	// else
	// 	s << ":" << servername << " " << code << " " << sender.get_nickname() << " " << arg1 << " " << arg2 << " " << arg3 << " :" << msg << "\n";

	s << ":" << servername << " " << code << " " << sender.get_nickname() << " " << arg1 << " " << arg2 << " " << arg3 << " :" << msg << "\n";

	send_msg(sender.get_socket(), s.str());
}


// :themis.sorcery.net 404 you #alice :Cannot send to channel (no external message)


// ERR_UNKNOWNERROR (400)
//   "<client> <command>{ <subcommand>} :<info>

// This numeric indicates a very generalised error (which <info> should further explain).
// If there is another more specific numeric which represents the error occuring, that should be used instead.





// ERR_NOSUCHNICK (401)
//   "<client> <nickname> :No such nick/channel

// ERR_NOSUCHSERVER (402)
//   "<client> <server name> :No such server

// ERR_NOSUCHCHANNEL (403)
//   "<client> <channel> :No such channel

// ERR_CANNOTSENDTOCHAN (404)
//   "<client> <channel> :Cannot send to channel

// ERR_TOOMANYCHANNELS (405)
//   "<client> <channel> :You have joined too many channels

// ERR_WASNOSUCHNICK (406)
//   "<client> :There was no such nickname

// ERR_NOORIGIN (409)
//   "<client> :No origin specified

// ERR_NORECIPIENT (411)
//   "<client> :No recipient given (<command>)

// ERR_NOTEXTTOSEND (412)
//   "<client> :No text to send

// ERR_INPUTTOOLONG (417)
//   "<client> :Input line was too long

// ERR_UNKNOWNCOMMAND (421)
//   "<client> <command> :Unknown command

// ERR_NOMOTD (422)
//   "<client> :MOTD File is missing

// ERR_NONICKNAMEGIVEN (431)
//   "<client> :No nickname given

// ERR_ERRONEUSNICKNAME (432)
//   "<client> <nick> :Erroneus nickname

// ERR_NICKNAMEINUSE (433)
//   "<client> <nick> :Nickname is already in use

// ERR_NICKCOLLISION (436)
//   "<client> <nick> :Nickname collision KILL from <user>@<host>

// ERR_USERNOTINCHANNEL (441)
//   "<client> <nick> <channel> :They aren't on that channel

// ERR_NOTONCHANNEL (442)
//   "<client> <channel> :You're not on that channel

// ERR_USERONCHANNEL (443)
//   "<client> <nick> <channel> :is already on channel

// ERR_NOTREGISTERED (451)
//   "<client> :You have not registered

// ERR_NEEDMOREPARAMS (461)
//   "<client> <command> :Not enough parameters

// ERR_ALREADYREGISTERED (462)
//   "<client> :You may not reregister

// ERR_PASSWDMISMATCH (464)
//   "<client> :Password incorrect

// ERR_YOUREBANNEDCREEP (465)
//   "<client> :You are banned from this server.

// ERR_CHANNELISFULL (471)
//   "<client> <channel> :Cannot join channel (+l)

// ERR_UNKNOWNMODE (472)
//   "<client> <modechar> :is unknown mode char to me

// ERR_INVITEONLYCHAN (473)
//   "<client> <channel> :Cannot join channel (+i)

// ERR_BANNEDFROMCHAN (474)
//   "<client> <channel> :Cannot join channel (+b)

// ERR_BADCHANNELKEY (475)
//   "<client> <channel> :Cannot join channel (+k)

// ERR_BADCHANMASK (476)
//   "<channel> :Bad Channel Mask

// ERR_NOPRIVILEGES (481)
//   "<client> :Permission Denied- You're not an IRC operator

// ERR_CHANOPRIVSNEEDED (482)
//   "<client> <channel> :You're not channel operator

// ERR_CANTKILLSERVER (483)
//   "<client> :You cant kill a server!

// ERR_NOOPERHOST (491)
//   "<client> :No O-lines for your host