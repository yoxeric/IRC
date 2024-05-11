#pragma once

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include <unistd.h>

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>

#include <iostream>
#include <sstream> 
#include <vector>

// #include <poll.h>

#include "Server.hpp"


std::string parse(Server& server, int client_index, std::string buffer);


// void	parse_CAP ();
// void	parse_NICK ();
// void	parse_USER ();
// void	parse_JOIN ();
// void	parse_WHO ();
// void	parse_MODE ();
// void	parse_PRVMAG ();
// void	parse_QUIT ();