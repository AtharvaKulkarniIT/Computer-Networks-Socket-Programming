#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080
#define BUFFER_SIZE 50

void binary_division(char *dividend, char *divisor, char *quotient, char *remainder) {
    int dividend_len = strlen(dividend);
    int divisor_len = strlen(divisor);

    char temp_dividend[100];
    strcpy(temp_dividend,dividend);

    // Perform binary long division
    int i,j;
    for (i = 0; i <= dividend_len - divisor_len; i++) {
        if (temp_dividend[i] == '1') {
            for (j = 0; j < divisor_len; j++) {
                if (temp_dividend[i + j] == divisor[j]) {
                    temp_dividend[i + j] = '0';
                } else {
                    temp_dividend[i + j] = '1';
                }
            }
            quotient[i] = '1';
        } else {
            quotient[i] = '0';
        }
    }
    quotient[dividend_len - divisor_len + 1] = '\0';
    strcpy(remainder, &temp_dividend[dividend_len - divisor_len + 1]);
}


int main()
{
int serversocket,client_socket,bindstatus,valread,i,isAllZeroes=1;
char divisor[BUFFER_SIZE] = {0};
char checksum[BUFFER_SIZE] = {0};
 char quotient[50], remainder[50];
struct sockaddr_in serveraddress;

serversocket=socket(AF_INET,SOCK_STREAM,0);
if(serversocket<0)
printf("socket failed");
serveraddress.sin_family=AF_INET;
serveraddress.sin_port=htons(9000);
//serveraddress.sin_addr.s_addr= htons("192.168.56.1");
serveraddress.sin_addr.s_addr= INADDR_ANY;

bindstatus=bind(serversocket,(struct sockaddr *)&serveraddress, sizeof(serveraddress));
if(bindstatus<0)
printf("binding failed\n");
else
printf("binding is successful\n");

listen(serversocket,3);
printf("Send reply to the client\n");

//clientsocket=accept(serversocket,( struct sockaddr *)&clientsocket, sizeof(clientsocket));
client_socket=accept(serversocket,NULL,NULL);
if(client_socket<0)
printf("connection is rejected by server\n");
else
printf("connection is accepted by server\n");
printf("\n");


// Read data from client
    valread = read(client_socket, divisor, BUFFER_SIZE);
    printf("Received CRC Key from client: %s\n", divisor);
    valread = read(client_socket, checksum , BUFFER_SIZE);
    printf("Received data from client: %s\n", checksum);
    
    binary_division(checksum, divisor, quotient, remainder);
    printf("Reminder=%s\n",remainder);
    for (i = 0; i < strlen(remainder); i++) {
        if (remainder[i] != '0') {
            isAllZeroes = 0;
            break;
        }
    }
    if (isAllZeroes) {
        printf("Accepted\n");
    } else {
        printf("Not Accepted\n");
    }

    
close(client_socket);
close(serversocket);
return 0;

}
    
    
    
    
    
    
    
    
    
    
    
