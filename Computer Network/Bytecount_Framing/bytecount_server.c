#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Include arpa/inet library
#include <unistd.h>

#define MAXLINE 1024

int main() {
    int sockfd, newfd, n;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed!");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed!");
        exit(1);
    }

    listen(sockfd, 2);
    len = sizeof(cliaddr);
    newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);

    n = read(newfd, buffer, sizeof(buffer));
    buffer[n] = '\0';

    // Extract the character count from the received frame
    int count = atoi(buffer);

    // Read the next 'count' characters as the binary data
    n = read(newfd, buffer, count);
    buffer[n] = '\0';

    printf("Received binary data: %s\n", buffer);

    // Process the received data as needed

    close(newfd);
    close(sockfd);
    return 0;
}
