#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int sock, val, i, count, port;
    char recvdata[50], sentdata[50];
    struct sockaddr_in server_addr;

    printf("\n\n Client Running ...... ");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    printf("\nEnter the port number: ");
    scanf("%d", &port);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Connect");
        exit(1);
    }

    while (1)
    {
        // get the packet number from client
        printf("\nEnter packet number: ");
        scanf("%d", &val);

        // send the value to server
        send(sock, &val, sizeof(val), 0);

        // get the data from the user
        printf("\n\nEnter data: ");
        scanf("%s", sentdata);

        // send the data to server
        send(sock, sentdata, strlen(sentdata), 0);

        if (strcmp(sentdata, "end") == 0)
            break;

        // receive the result from the server
        recv(sock, &count, sizeof(count), 0);
        i = recv(sock, recvdata, 50, 0);
        recvdata[i] = '\0';
        printf("\n%s %d", recvdata, count);
    }

    close(sock);
    return 0;
}
