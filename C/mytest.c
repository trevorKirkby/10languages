#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main()
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	portno = 1234;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Error opening socket.");
	}

	server = gethostbyname("localhost");
	bzero((char *) & serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy(
		(char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length
	);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		error("Error connecting");

	while (1) //Infinite interactive loop until keyboard interrupt. I could code in text based exit, but w/e.
	{
		printf("Please enter text to send: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0)
		{
			error("Error writing to socket.");
		}

		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0)
		{ //not using brackets in some cases is messy.
			error("Error reading from socket.");
		}
		printf("%s",buffer);
	}

	return 0;
}
