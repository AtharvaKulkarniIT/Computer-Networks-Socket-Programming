#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

// Calculate the CRC checksum for binary data
unsigned char calculateCRC(const unsigned char *data, size_t length, unsigned char crc_key) {
    unsigned char crc = crc_key;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
    }
    return crc;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept a client connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("Accepting connection failed");
        close(server_socket);
        exit(1);
    }

    // Receive binary data and CRC key from the client
    unsigned char received_data[1024];
    ssize_t bytes_received = recv(client_socket, received_data, sizeof(received_data), 0);
    if (bytes_received <= 0) {
        perror("Error receiving data");
        close(client_socket);
        close(server_socket);
        exit(1);
    }

    // Extract the CRC key from the received data
    unsigned char crc_key = received_data[bytes_received - 1];
    bytes_received--;

    // Calculate CRC for the received data
    unsigned char calculated_crc = calculateCRC(received_data, bytes_received, crc_key);

    // Check if the calculated CRC is zero
    if (calculated_crc == 0) {
        printf("Binary data received successfully: %s\n", received_data);
    } else {
        printf("Binary data received with errors.\n");
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
