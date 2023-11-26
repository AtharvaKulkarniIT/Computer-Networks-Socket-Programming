#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}


/*  EXPLAINATION :- 
This C code is an example of a simple client-server communication using sockets over the TCP/IP protocol. The code creates a client that connects to a server running on the local machine (IP address 127.0.0.1) and communicates with it through a TCP connection.

Let's go through the code step-by-step:

1. Include Headers:
The code includes several necessary header files for working with sockets, networking, and standard I/O functions.

2. Macro Definitions:
The code defines some constants using preprocessor macros. These constants are used to define the maximum buffer size (MAX) for data communication and the port number (PORT) on which the server is listening.

3. Function `func(int sockfd)`:
This function is responsible for handling the client-server communication. It takes a socket file descriptor (sockfd) as an argument, which is used for communication with the server.

4. Socket Creation (`main()` function):
The `main()` function starts by creating a socket using the `socket()` system call. The `socket()` function returns a socket file descriptor (sockfd) that is used in subsequent socket operations.

5. Server Address (`struct sockaddr_in servaddr`):
A `struct sockaddr_in` is used to store the server's address and port information. The IP address is set to "127.0.0.1" (localhost), and the port is set to the value defined by the `PORT` macro.

6. Connect to Server:
The `connect()` function is used to establish a connection to the server using the socket file descriptor (sockfd) and the server address (`servaddr`). If the connection is successful, the client is connected to the server.

7. Chat Loop:
The `func()` function implements a loop for client-server communication. Inside the loop, the client repeatedly sends a message to the server and receives a response until the message "exit" is sent.

   a. The client clears the buffer using `bzero()` and then prompts the user to enter a string.
   b. The entered string is read character by character until a newline character ('\n') is encountered, and it is stored in the buffer `buff`.
   c. The client sends the message to the server using the `write()` function.
   d. The client waits for a response from the server using the `read()` function and stores it in the same buffer `buff`.
   e. The client then prints the received message from the server.
   f. If the received message is "exit," the client breaks out of the loop and exits.

8. Close Socket:
After the chat loop is done (when the user enters "exit"), the client closes the socket using the `close()` function.

This code creates a simple client that connects to the server and allows the user to exchange messages with the server until the user decides to exit by entering "exit". Keep in mind that this code is a basic example and lacks error handling and more robust features, which would be essential in a production-level client-server application.*/
