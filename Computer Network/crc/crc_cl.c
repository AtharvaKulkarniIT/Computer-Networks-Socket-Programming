#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1024

void crc(char *data, char *CRC_Key, char *remainder);

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

int main() {
    int sockfd;
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

    char data[MAXLINE];
    char CRC_Key[30]; // Declare CRC_Key variable
    char remainder[30]; // To store the remainder

    // Get user input for the data to be sent
    printf("Enter the data to be sent: ");
    scanf("%s", data);

    // Get user input for the CRC key
    printf("Enter the CRC Key: ");
    scanf("%s", CRC_Key);

    // Perform CRC calculation and send data with CRC checksum to the server
    crc(data, CRC_Key, remainder);

    // Send data to the server
    write(sockfd, data, strlen(data));

    // Display the remainder on the client side
    printf("Remainder: %s\n", remainder);

    close(sockfd);
    return 0;
}
