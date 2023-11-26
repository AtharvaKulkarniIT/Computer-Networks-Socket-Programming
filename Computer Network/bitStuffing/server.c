#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 1024

int main() {
    int sockfd, newfd, n;
    char buffer[MAXLINE], stuffed_frame[MAXLINE];
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

    // Print the received unstuffed data
    printf("Received unstuffed data: %s\n", buffer);

    // Perform bit stuffing on the received frame (buffer) and store it in stuffed_frame
    int i, j = 0, count = 0;
    for (i = 0; i < n; i++) {
        if (buffer[i] == '1') {
            stuffed_frame[j] = '1';
            count++;
        } else {
            stuffed_frame[j] = '0';
            count = 0;
        }
        j++;
        if (count == 5) {
            stuffed_frame[j] = '0'; // Stuff an extra '0' bit after 5 consecutive '1's
            j++;
            count = 0;
        }
    }
    stuffed_frame[j] = '\0';

    // Send the stuffed frame back to the client
    write(newfd, stuffed_frame, strlen(stuffed_frame));

    // Print the stuffed data
    printf("Sent stuffed data: %s\n", stuffed_frame);

    close(newfd);
    close(sockfd);
    return 0;
}
