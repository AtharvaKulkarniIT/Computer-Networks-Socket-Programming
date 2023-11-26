#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 2048  // Adjust the buffer size according to your needs

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(45678);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Use the server's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error connecting to server");
        close(clientSocket);
        exit(1);
    }

    printf("Connected to server.\n");

    char buffer[1024];
    int bytesRead;

    while (1) {
        printf("Enter the Message to be Sent (or 'bye' to exit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline character from input
        buffer[strlen(buffer) - 1] = '\0';

        char stuffed[2048];
        int stuffedIndex = 0;

        for (int i = 0; i < strlen(buffer); i++) {
            if (buffer[i] == 'F') {
                stuffed[stuffedIndex++] = 'E';
                stuffed[stuffedIndex++] = 'F';
            } else if (buffer[i] == 'E') {
                stuffed[stuffedIndex++] = 'E';
                stuffed[stuffedIndex++] = 'E';
            } else {
                stuffed[stuffedIndex++] = buffer[i];
            }
        }

        stuffed[stuffedIndex] = '\0';

        // Data in each frame is stuffed by 'F' at the beginning and end
        char data[MAX_BUFFER_SIZE];  // Use a larger buffer size

        // Check the result of snprintf and handle it accordingly
        int snprintf_result = snprintf(data, sizeof(data), "F%sF", stuffed);

        if (snprintf_result < 0 || snprintf_result >= sizeof(data)) {
            fprintf(stderr, "Error: snprintf overflowed the buffer.\n");
            break;
        }

        printf("The data being sent (with byte stuffed) is: %s\n", data);

        // Send the stuffed data to the server
        send(clientSocket, data, strlen(data), 0);
        printf("Sending Message...\n");

        // Receive acknowledgment from the server
        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            perror("Error reading from server");
            break;
        } else if (bytesRead == 0) {
            printf("Server disconnected.\n");
            break;
        }

        if (strcmp(buffer, "success") == 0) {
            printf("Thanks for the Feedback Server!!\n");
        }

        if (strcmp(buffer, "bye") == 0) {
            printf("Messaging is over.....EXITING\n");
            // Close the client socket and exit the loop
            close(clientSocket);
            break;
        }
    }

    return 0;
}

