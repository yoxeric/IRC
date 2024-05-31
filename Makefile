NAME = ircserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

SRC = ft_irc.cpp poll.cpp Client.cpp Channel.cpp Server.cpp parse.cpp Messages.cpp \
 		cmds/cap.cpp cmds/invite.cpp cmds/join.cpp cmds/kick.cpp cmds/list.cpp \
 		cmds/mode.cpp cmds/oper.cpp cmds/part.cpp cmds/pass.cpp cmds/ping.cpp \
 		cmds/privmsg.cpp cmds/quit.cpp cmds/topic.cpp cmds/user.cpp cmds/who.cpp cmds/nick.cpp 

HEADER = inc/ft_irc.hpp inc/poll.hpp inc/Client.hpp inc/Channel.hpp inc/Server.hpp

OBJ = ${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
	