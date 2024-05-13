NAME = ircserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ft_irc.cpp poll.cpp Client.cpp Channel.cpp Server.cpp parse.cpp Commands.cpp Notices.cpp

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
	