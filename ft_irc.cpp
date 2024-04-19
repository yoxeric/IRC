 
#include "ft_irc.hpp"

int main(int ac, char **av)
{
	if (ac < 3)
	{
		std::cout << "usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int sockfd, newsockfd, portnb, n;
	char buffer[255];

	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_len;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cout << "Error openning server socket" << std::endl;
		return 1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portnb = atoi(av[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnb);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Binding Failed" << std::endl;
		return 1;
	}

	listen(sockfd, 5);
	client_len = sizeof(client_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
	if (newsockfd < 0)
	{
		std::cout << "Error accepting client" << std::endl;
		return 1;
	}

	while (1)
	{
		bzero(buffer, 255);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
		{
			std::cout << "Error reading" << std::endl;
			return 1;
		}

		printf("client : %s\n", buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);

		n = write(newsockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			std::cout << "Error writing" << std::endl;
			return 1;
		}

		if (strncmp("close", buffer, 5))
			break ;
	}

	close(newsockfd);
	close(sockfd);
	return 0;
}