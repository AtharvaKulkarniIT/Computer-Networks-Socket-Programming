#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
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
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(1);
    }

    // Input binary data from the user
    printf("Enter binary data (e.g., 010101): ");
    char input_data[1024];
    fgets(input_data, sizeof(input_data), stdin);
    size_t data_length = strlen(input_data);
    if (data_length > 0 && input_data[data_length - 1] == '\n') {
        // Remove the newline character if present
        input_data[data_length - 1] = '\0';
    }

    // Input CRC key from the user
    printf("Enter CRC key (e.g., 101): ");
    char crc_key_input[16];
    fgets(crc_key_input, sizeof(crc_key_input), stdin);
    unsigned char crc_key = (unsigned char)strtol(crc_key_input, NULL, 2);

    // Calculate CRC for the data
    unsigned char crc = calculateCRC((const unsigned char *)input_data, strlen(input_data), crc_key);

    // Send data and CRC key to the server
    unsigned char binary_data[1024];
    strncpy((char *)binary_data, input_data, sizeof(binary_data) - 1);
    binary_data[strlen(input_data)] = crc_key;

    ssize_t bytes_sent = send(client_socket, binary_data, strlen(input_data) + 1, 0);
    if (bytes_sent == -1) {
        perror("Error sending data");
        close(client_socket);
        exit(1);
    }

    printf("Binary data sent to the server: %s\n", input_data);

    close(client_socket);
    return 0;
}
