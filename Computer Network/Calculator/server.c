#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function to perform calculator operations
double calculate(double num1, double num2, char operator) {
    switch (operator) {
        case '+':
            return num1 + num2;
        case '-':	
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            return num2 != 0 ? num1 / num2 : 0;
        default:
            return 0;
    }
}

void performCalculation(int connfd) 
// This function handles the communication with the client and by evaluating the calculate() function it gives the result back to the client.
{
    char buffer[MAX];
    char operator;
    double num1, num2, result;

    for (;;) {
        bzero(buffer, MAX);
        read(connfd, buffer, sizeof(buffer));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        sscanf(buffer, "%lf %c %lf", &num1, &operator, &num2);
        result = calculate(num1, num2, operator);
        snprintf(buffer, MAX, "%.2f", result);
        write(connfd, buffer, sizeof(buffer));
    }
}

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) != 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening..\n");
    len = sizeof(cli);

    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        perror("server accept failed");
        exit(EXIT_FAILURE);
    }

    printf("server accept the client...\n");
    performCalculation(connfd);
    close(sockfd);

    return 0;
}
