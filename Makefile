NAME = ircserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ft_irc.cpp poll.cpp client.cpp parse.cpp

HEADER = ft_irc.hpp poll.hpp client.hpp

OBJ = ${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all