#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

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

void replaceLetter(char *checksum, int position, char newLetter) {
    if (position >= 0 && position < strlen(checksum)) {
        checksum[position] = newLetter;
    } else {
        printf("Invalid position! The position must be within the range of the string.\n");
    }
}

int main()
{
//create Socket
int client_socket;
struct sockaddr_in serveraddress;

client_socket=socket(AF_INET,SOCK_STREAM,0);// create socket
//SOCKET function returns the integer
if(client_socket<0)
printf("socket is not created\n");
else 
printf("socket is created succesfully\n");

serveraddress.sin_family=AF_INET;
serveraddress.sin_port=htons(9000);
//serveraddress.sin_addr.s_addr= htons("192.168.56.1");
serveraddress.sin_addr.s_addr= INADDR_ANY;

int constatus=connect(client_socket,(struct sockaddr *) &serveraddress, sizeof(serveraddress));
 
 if(constatus==-1)
 printf("there was an error in the connection\n");
 else 
 printf("connection is estableshed\n"); 

 char data[50],dividend[50], divisor[50], quotient[50], remainder[50],zeroes[50],input,checksum[50],newLetter;
    int i,position,choice,isAllZeroes=1;

    printf("Enter the CRC Key: ");
    scanf("%s", divisor);
    send(client_socket, divisor, strlen(divisor), 0);
    printf("Enter the data: ");
    scanf("%s", data);

    strcpy(dividend,data);

    int divisor_len = strlen(divisor);
    for (i = 0; i <  divisor_len- 1; i++) {
        zeroes[i] = '0';
    }

    zeroes[divisor_len - 1] = '\0';
    strcat(dividend,zeroes);

    binary_division(dividend, divisor, quotient, remainder);
    strcpy(checksum,strcat(data,remainder));
    printf("Padded Data: %s\n", dividend);
    printf("do you want to put an error in data \n1.yes\n0.no\n");
    scanf("%d",&choice);
    if(choice)
    {
        printf("enter the position you want to replace=");
        scanf("%d",&position);
        getchar();
            printf("Enter the new bit: ");
            scanf("%c", &newLetter);

            replaceLetter(checksum, position-1, newLetter);

    }
    printf("Transmitted Data: %s\n",checksum);
    send(client_socket, checksum, strlen(checksum), 0);
    
 close(client_socket);
 
 return 0;
 
 }
     
    
    
    
    
    
    
    
