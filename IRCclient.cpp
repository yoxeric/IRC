
#include "ft_irc.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "usage: ./irc_client <hosname> <port>" << std::endl;
		return 1;
	}

	int sockfd, portnb, n;
	char buffer[255];

	struct sockaddr_in serv_addr;
	struct hostent *server;

	portnb = atoi(av[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cout << "Error openning server socket" << std::endl;
		return 1;
	}

	server = gethostbyname(av[1]);
	if (server == NULL)
	{
		std::cout << "Error : no such host" << std::endl;
		return 1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr,(char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portnb);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Error connecting" << std::endl;
		return 1;
	}

	while (1)
	{
		bzero(buffer, 255);

		//write message to client
		fgets(buffer, 255, stdin);

		//write message in socket
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			std::cout << "Error writing" << std::endl;
			return 1;
		}

		bzero(buffer, 255);

		//read client message from socket
		n = read(sockfd, buffer, 255);
		if (n < 0)
		{
			std::cout << "Error reading" << std::endl;
			return 1;
		}
		printf("client : %s\n", buffer);


		if (strncmp("close", buffer, 5) == 0)
			break ;
	}

	close(sockfd);
	return 0;
}