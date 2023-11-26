#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 1024

void crc(char *data, char *CRC_Key, char *remainder);
int verify_remainder(char *remainder);

void crc(char *data, char *CRC_Key, char *remainder) {
    char check_value[30]; // Remainder
    int N = strlen(CRC_Key);

    // Initialize check value with data
    strncpy(check_value, data, N);

    // Perform CRC error checking
    int i = 0;
    do {
        if (check_value[0] == '1') {
            for (int j = 0; j < N; j++) {
                check_value[j] = (check_value[j] == CRC_Key[j]) ? '0' : '1';
            }
        }

        for (int j = 0; j < N - 1; j++)
            check_value[j] = check_value[j + 1];

        check_value[N - 1] = data[i++];
    } while (i <= strlen(data));

    // Copy the remainder to the 'remainder' parameter
    strncpy(remainder, check_value, N - 1);
    remainder[N - 1] = '\0';

    // Append the calculated CRC checksum to the data
    strncpy(data + strlen(data), check_value, N - 1);
    data[strlen(data) + N - 1] = '\0';
}

int verify_remainder(char *remainder) {
    int N = strlen(remainder);
    for (int i = 0; i < N; i++) {
        if (remainder[i] == '1') {
            return 0; // Not all zeros, error detected
        }
    }
    return 1; // All zeros, no error detected
}

int main() {
    int sockfd, newfd;
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

    char CRC_Key[30];
    char remainder[30]; // To store the remainder

    // Get user input for CRC key
    printf("Enter the CRC Key: ");
    scanf("%s", CRC_Key);

    while (1) {
        char data[MAXLINE];

        // Receive data from the client
        int n = read(newfd, data, sizeof(data));
        if (n <= 0) {
            printf("Connection closed by the client.\n");
            break;
        }

        printf("Received data: %s\n", data);

        // Perform CRC error checking for received data
        crc(data, CRC_Key, remainder);

        // Display the remainder on the server side
        printf("Remainder: %s\n", remainder);

        // Verify the received data
        int error = verify_remainder(remainder);

        if (error) {
            printf("No error detected\n\n");
        } else {
            printf("Error detected\n\n");
        }

        // Send the received data with the appended remainder back to the client
        write(newfd, data, strlen(data));
    }

    close(newfd);
    close(sockfd);
    return 0;
}
