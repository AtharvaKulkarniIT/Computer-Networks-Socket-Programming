#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(45678);

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding");
        close(serverSocket);
        exit(1);
    }

    // Listen for connections
    if (listen(serverSocket, 5) < 0) {
        perror("Error listening");
        close(serverSocket);
        exit(1);
    }

    printf("Waiting for a client to connect...\n");

    // Accept a client connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    if (clientSocket < 0) {
        perror("Error accepting client connection");
        close(serverSocket);
        exit(1);
    }

    printf("Client connected.\n");

    char buffer[1024];
    int bytesRead;

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive stuffed message from client
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            perror("Error reading from client");
            break;
        } else if (bytesRead == 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Message Received...Successfully!!!\n");
        printf("The Stuffed Message is : %s\n", buffer);

        char destuffed[1024];
        int destuffedIndex = 0;

        for (int i = 1; i < bytesRead - 1; i++) {
            if ((buffer[i] == 'E' && buffer[i + 1] == 'F') || (buffer[i] == 'F' && i == bytesRead - 2)) {
                destuffed[destuffedIndex++] = 'F';
                i++;
            } else if (buffer[i] == 'E' && buffer[i + 1] == 'E') {
                destuffed[destuffedIndex++] = 'E';
                i++;
            } else {
                destuffed[destuffedIndex++] = buffer[i];
            }
        }

        destuffed[destuffedIndex] = '\0';

        printf("The Destuffed Message is : %s\n", destuffed);

        // Send acknowledgment to the client
        send(clientSocket, "success", strlen("success"), 0);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}

