#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("connection with the server failed");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX];
    printf("Enter the expression\n");
    printf("Type 'exit' to quit.\n");

    while (1) {
        fgets(buffer, MAX, stdin);
        write(sockfd, buffer, sizeof(buffer));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        bzero(buffer, MAX);
        read(sockfd, buffer, sizeof(buffer));
        printf("Result from server: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}

