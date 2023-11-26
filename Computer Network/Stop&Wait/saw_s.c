#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define SA struct sockaddr
#define PORT 8080

void main()
{
	srand(time(0));

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		printf("Socket creation failed.\n");
		exit(0);
	}
	else
		printf("Socket created successfully.\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("Bind failed.\n");
		exit(0);
	}
	else 
		printf("Bind successful.\n");

	if (listen(sockfd, 5) != 0)
	{
		printf("Listen failed.\n");
		exit(0);
	}
	else
		printf("Listen successful\n");

	len = sizeof(cliaddr);

	connfd = accept(sockfd, (SA *)&cliaddr, &len);

	if (connfd < 0)
	{
		printf("Connection failed.\n");
		exit(0);
	}
	else
		printf("Connection successful.\n");

	int count = 1;
	int ch;
	int reply;

	while (1)
	{
		ch = rand() % 2;

		switch (ch)
		{
			case 0:	printf("Frame %d sent successfully.\n", count);
					write(connfd, &ch, sizeof(ch));
					break;

			case 1:	printf("Frame %d destroyed during transmission.\n", count);
					printf("Resending frame %d.\n", count);
					write(connfd, &ch, sizeof(ch));
					break;
		}

		read(connfd, &reply, sizeof(reply));
		
		switch (reply)
		{
			case 0: printf("Acknowledgement received.\n");
					count++;
					break;

			case 1: printf("Acknowledgement not received.\nResending frame %d\n", count);
					break;
		}

		sleep(2);
	}
}
