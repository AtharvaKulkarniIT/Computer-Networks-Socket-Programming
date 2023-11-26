#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SA struct sockaddr
#define PORT 8080

void main()
{
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
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
		read(sockfd, &reply, sizeof(reply));

		ch = rand() % 2;

		switch (ch)
		{
			case 0:	printf("Acknowledgement sent successfully.\n");
					write(sockfd, &ch, sizeof(ch));
					break;

			case 1:	printf("Acknowledgement destroyed during transmission.\n");
					write(sockfd, &ch, sizeof(ch));
					break;
		}

		sleep(3);
	}
}
