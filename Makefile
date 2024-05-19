NAME = ircserv
CC = c++
CPPFLAGS = -fsanitize=address -g -Wall -Wextra -Werror -std=c++98

SRC = ft_irc.cpp poll.cpp Client.cpp Channel.cpp Server.cpp parse.cpp Commands.cpp Notices.cpp

OBJ = ${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
	