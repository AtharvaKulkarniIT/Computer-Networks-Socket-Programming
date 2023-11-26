#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<arpa/inet.h>

#define MAXLINE 1024

int main() {
    int sockfd, n;
    char buffer[MAXLINE], unstuffed_frame[MAXLINE];
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed!");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed!");
        exit(1);
    }

    printf("Enter the data frame (0s and 1s): ");
    scanf("%s", buffer);

    write(sockfd, buffer, strlen(buffer));

    n = read(sockfd, unstuffed_frame, sizeof(unstuffed_frame));
    unstuffed_frame[n] = '\0';

    printf("Received unstuffed frame: %s\n", unstuffed_frame);

    close(sockfd);
    return 0;
}
