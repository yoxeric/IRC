
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

	// send_reply(372, client, "", "<!--          # #                       .-                                  -->");
	// send_reply(372, client, "", "<!--         .-.     /      /          /  .-         /           /          -->");
	// send_reply(372, client, "", "<!--  .     (   )   /      /.-.   #   /  /     .    /    .-.    /           -->");
	// send_reply(372, client, "", "<!-- (         /   /     /    )  /   /  /     (    /    (   )  /       .-.  -->");
	// send_reply(372, client, "", "<!--  '-------'   ' ----'`---- -'---'  /      '._ /  ---'`--' / ___ __(___  -->");
	// send_reply(372, client, "", "<!--                                  /                                     -->");

	send_reply(372, client, "", "<!--                       ScaleFactor                     -->");
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
	std::stringstream ss2;

	ss << "#" << chan.get_name() << " " << chan.count_membres();
	ss2 << chan.get_topic();
	send_reply(322, client, ss.str(), ss2.str());


	ss.str("");
	ss << "#" << chan.get_name() << " " << create_tag(client) << " " << "1000000000";
	send_reply(333, client, ss.str(), "");

	ss.str("");
	ss << "= #" << chan.get_name();
	ss2.str("");
	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		if (chan.is_membre_mode(*it, 'o'))
			ss2 << "@";
		// ss2 << create_tag(*it) << " ";
		ss2 << it->get_nickname() << " ";
	}
	send_reply(353, client, ss.str(), ss2.str());

	ss.str("");
	ss << "#" << chan.get_name();
	send_reply(366, client, ss.str(), "End of /NAMES list.");

	ss.str("");
	ss << "#" << chan.get_name() << " +" << chan.get_mode();
	ss2.str("");
	// ss2 << "#"  << target;
	send_reply(324, client, ss.str(), ss2.str());


	ss.str("");
	ss << "#" << chan.get_name() << " " << "1000000000";
	send_reply(329, client, ss.str(), "");

	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		ss.str("");
		ss << "#" << chan.get_name() << " ~" << it->get_username() << " " << it->get_address() << ".IP " << servername
		<< " " << it->get_nickname() << " " <<  it->get_mode();
		send_reply(352, client, ss.str(), it->get_realname());
	}

	ss.str("");
	ss << "#" << chan.get_name();
	send_reply(315, client, ss.str(), "End of /WHO list");
}

// >> :sakura.jp.as.dal.net 332 Guest20411 #all :❤ ωєℓ¢σмє to #ALL LOVє is just like pＲiＮgLєＳOnce yOu pOp, yOu can nєvєr stＯp! ❤ 
// >> :sakura.jp.as.dal.net 333 Guest20411 #all LoL!~LoL@DALnet.LoL 1698805456
// >> :sakura.jp.as.dal.net 353 Guest20411 = #all :Guest20411 @Dom prashant__
// >> :sakura.jp.as.dal.net 366 Guest20411 #all :End of /NAMES list.
// >> :sakura.jp.as.dal.net 324 Guest20411 #all +tn 
// >> :sakura.jp.as.dal.net 329 Guest20411 #all 1611642342
// >> :sakura.jp.as.dal.net 352 Guest20411 #all ~yh2 682f-a2c-e5ff-bf2e-99f9.230.197.ip sakura.jp.as.dal.net Guest20411 H :0 realname
// >> :sakura.jp.as.dal.net 315 Guest20411 #all :End of /WHO list.

// >> :AbbY.MindForge.org 332 you #MindForge-Chat :| Welcome to the MindForge IRC Network | General IRC help and main chat channel. Feel free to talk in your own language and have fun! | Remember to join #MindForge for network help | Site: https://MindForge.org |
// >> :AbbY.MindForge.org 333 you #MindForge-Chat Moot 1487514071
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :you!yh2@BCF3BF7C.5B8307BF.31484AE3.IP Pepebyt67!Android@mf-ADC69216.pool.telefonica.de anko!anko@French-Rencontre.MindForge.org keikoz!keikoz@mf-1B2110A8.subs.proxad.net Alessandro!Alex@mf-737C5965.retail.telecomitalia.it Tanoc!Tanoc@mf-ACCE4AAA.dhcp.inet.fi 
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :%PerfumeOfJasmine!Count3ss@MF-Helpers.MindForge.org st4ndby!Camelot@eMule-Portuguese.MindForge.org @KidCat!Mt@ServicesAdmin.MindForge.org palazzolo!palazzolo@mf-B08D85B8.net.vodafone.it Drakonas!kvirc@eMule-Lithuanian.MindForge.org +Zygo!Minei@vguess.best.player.on.MindForge.org 
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :connann!ircap@93BFA18B.9D11A1B5.3D777FDE.IP @albator!albator@FR.IrcOp.MindForge.org +stop!yo@mf-4197BBAC.dyn.user.ono.com llh!UPP@mf-9C26B8F0.cpe.teksavvy.com Dragonite!Dragonite@E0B62223.299AD3D9.88AC2480.IP Hackerpcs!user@mf-337212DF.vm.okeanos.grnet.gr 
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :smil!smil@mf-6E3C8073.colo.transip.net zer0bitz!zer0bitz@mf-26FF0C9A.dhcp.inet.fi %Beefhart!beefhart@MF-Helpers.MindForge.org Vajb!Vajb@mf-2A85DF4F.elisa-mobile.fi Elven_Wizard!Suntop@eMule-Norwegian.MindForge.org MySkO^!~MySkO@9D87A35.1DB5D0D4.4A6519FD.IP 
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :RedStone!redstone@mf-C09C94C5.rbradford.me +_!Sonic@mf-72009322.aso.wtf @Troopy!Trooper@BR.IrcOp.MindForge.org jkhsjdhjs!jkhsjdhjs@mf-7829E6A7.ink Harambe!Roma@EC032123.3D29DEA.EB32CD3F.IP KexyBiscuit!sid555485@mf-3ED72889.irccloud.com ComputerTech!ComputerTe@70AFA039.BC38E2C2.55B7158E.IP 
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :Wolf!Matt@mf-9C3134EA.to.you.by.bnc4free.com +Russell!russell@mf-EFB8DCB.rbradford.me get!facil@Linux.MindForge.org Camile^away!Camile@El-Melonar.MindForge.org Goyo!ILoveIRC@mf-56476DD5.iloveirc.com @Moot!Mt@ServicesAdmin.MindForge.org @pralinka!pralinka@ServicesAdmin.MindForge.org 
// >> :AbbY.MindForge.org 353 you = #MindForge-Chat :EvilCat!EvilCat@EvilCat.MindForge.org &@+[MindForge]!Bot@Services.MindForge.org LogX!LogX@Bot.MindForge.org @SynX!sx@Bot.MindForge.org 
// >> :AbbY.MindForge.org 366 you #MindForge-Chat :End of /NAMES list.
// >> :[MindForge]!Bot@Services.MindForge.org NOTICE you :[#mindforge-chat] | FAQs: https://MindForge.org/FAQs | WebChat: https://WebChat.MindForge.org | Follow us on https://Facebook.com/MindForgeIRC and https://Twitter.com/MindForgeIRC |
// >> :AbbY.MindForge.org 324 you #MindForge-Chat +ntrCTf [3j#i2,3k#K3,5m#m1]:1
// >> :AbbY.MindForge.org 329 you #MindForge-Chat 1347726470
// >> :AbbY.MindForge.org 352 you #MindForge-Chat yh2 BCF3BF7C.5B8307BF.31484AE3.IP AbbY.MindForge.org you H :0 realname
// >> :AbbY.MindForge.org 315 you #MindForge-Chat :End of /WHO list.

// >> :nexus.nl.eu.mixxnet.net 332 you #the_h00d :Welcome to the World Famous #the_h00d | Some, I assume, Are Good People | You need a psychologist. | Don't pull an NTS. Remember to pull out in time! | Fire's Betas Count: 29 | Holly's Death Count: 63
// >> :nexus.nl.eu.mixxnet.net 333 you #the_h00d holly!~holly@hollygurl.users.mixxnet.net 1614801021
// >> :nexus.nl.eu.mixxnet.net 353 you = #the_h00d :@Bjorn @NTS @iekue_erander @Chreddy @Revox @h00db0t daxson @zero1 @Magnum @D2T you @jstar 
// >> :nexus.nl.eu.mixxnet.net 366 you #the_h00d :End of /NAMES list.
// >> :nexus.nl.eu.mixxnet.net 324 you #the_h00d +djnrt 5 4:8
// >> :nexus.nl.eu.mixxnet.net 329 you #the_h00d 1386242542
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d bjorn bjorn.users.mixxnet.net tnt.il.us.mixxnet.net Bjorn G@ :0 ZNC - https://znc.in
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~nts nts.users.mixxnet.net vega.nj.us.mixxnet.net NTS H@ :0 ZNC - https://znc.in
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d iekue_eran 83-82-205-7.cable.dynamic.v4.ziggo.nl nexus.nl.eu.mixxnet.net iekue_erander H@ :0 ...
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~chreddy chreddy.users.mixxnet.net nexus.nl.eu.mixxnet.net Chreddy H@ :0 Chreddy
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~revox staff.mixxnet.net nexus.nl.eu.mixxnet.net Revox H*@ :0 Revox
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d cservice services.mixxnet.net services.mixxnet.net h00db0t H@ :0 BotServ
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~DaxSon 176.254.244.83 tnt.il.us.mixxnet.net daxson H :0 iNvERSeD
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d limnoria zero1.users.mixxnet.net vega.nj.us.mixxnet.net zero1 H@ :0 zero1
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~zero-one magnum.users.mixxnet.net vega.nj.us.mixxnet.net Magnum H@ :0 zero-one
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~D2T d2t.users.mixxnet.net tnt.il.us.mixxnet.net D2T H@ :0 D2T
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d ~yh2 197.230.24.20 nexus.nl.eu.mixxnet.net you H :0 realname
// >> :nexus.nl.eu.mixxnet.net 352 you #the_h00d sid15183 etn.fm nexus.nl.eu.mixxnet.net jstar G@ :0 Juliet Star
// >> :nexus.nl.eu.mixxnet.net 315 you #the_h00d :End of /WHO list.

//todo : fix channel who show info every time

void	Server::list_channel_short(Client& client, Channel &chan)
{
	std::stringstream ss;
	std::stringstream ss2;

	ss.str("");
	ss << "= #" << chan.get_name();
	ss2.str("");
	for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	{
		if (chan.is_membre_mode(*it, 'o'))
			ss2 << "@";
		// ss2 << create_tag(*it) << " ";
		ss2 << it->get_nickname() << " ";
	}
	send_reply(353, client, ss.str(), ss2.str());

	ss.str("");
	ss << "#" << chan.get_name();
	send_reply(366, client, ss.str(), "End of /NAMES list.");


	// for (std::vector<Client>::iterator it = chan.members.begin(); it != chan.members.end(); it++)
	// {
	// 	ss.str("");
	// 	ss << "#" << chan.get_name() << " ~" << it->get_username() << " " << it->get_address() << ".IP " << servername
	// 	<< " " << it->get_nickname() << " " <<  it->get_mode();
	// 	send_reply(352, client, ss.str(), it->get_realname());
	// }

	// ss.str("");
	// ss << "#" << chan.get_name();
	// send_reply(315, client, ss.str(), "End of /WHO list");
}


void	Server::list_user(Client& client, Client &target_client)
{
	std::stringstream ss;

	ss << "~" << target_client.get_username() << " " << target_client.get_address() << ".IP " << servername
	<< " " << target_client.get_nickname() << " " <<  target_client.get_mode();
	send_reply(352, client, ss.str(), target_client.get_realname());

	send_reply(315, client, "", "End of /WHO list");

}


// ------------------------------------- Error --------------------------------------------------------------------------


void	Server::send_err(int code, Client &sender, std::string msg)
{
	std::stringstream s;

	s << ":" << servername << " " << code << " " << sender.get_nickname() << " :" << msg << "\n";

	send_msg(sender.get_socket(), s.str());
}

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