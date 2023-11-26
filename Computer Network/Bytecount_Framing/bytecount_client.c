#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define MAXLINE 1024

int main() {
    int sockfd, n;
    char buffer[MAXLINE], frame[MAXLINE];
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed!");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);

    // Convert the server IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed!");
        exit(1);
    }

    // Get user input for the binary data
    printf("Enter the binary data: ");
    scanf("%s", buffer);

    // Calculate the character count and format it as a string
    int dataLength = strlen(buffer);

    // Allocate memory for the frame dynamically
    char *frame = (char *)malloc(dataLength + 16); // Adding some extra space for count and delimiter

    if (frame == NULL) {
        perror("Memory allocation failed!");
        exit(1);
    }

    snprintf(frame, dataLength + 16, "%d,%s", dataLength, buffer);

    // Send the frame to the server
    write(sockfd, frame, strlen(frame));

    // Free dynamically allocated memory
    free(frame);

    close(sockfd);
    return 0;
}
