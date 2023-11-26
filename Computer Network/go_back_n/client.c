#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
int main()
{
int clientsocket;
struct sockaddr_in serveraddress;
clientsocket = socket(AF_INET, SOCK_STREAM, 0);
if(clientsocket < 0)
{
printf("Socket is not created\n");
}
else
{
printf("Socket created Successfully\n");
}
serveraddress.sin_family = AF_INET;
serveraddress.sin_port = htons(9000);
serveraddress.sin_addr.s_addr = INADDR_ANY;
int constatus = connect(clientsocket, (struct sockaddr*)&serveraddress, 
sizeof(serveraddress));
if(constatus == 1)
{
printf("There was an error in connection\n");
}
else
{
printf("Connected Successfully\n");
}
//main code
char serverresponse[256];
recv(clientsocket, &serverresponse, sizeof(serverresponse), 0);
printf("The message from client is:\n");
printf("%s\n", serverresponse);
int array_c[3], temp[1];
int frame_no_c, win_size_c, error_index_c, i, frame, index = 0, ack = 0, flag = 0, j;
read(clientsocket, array_c, 3);
frame_no_c = array_c[0];
win_size_c = array_c[1];
error_index_c = array_c[2];
for(i = 1; i <= win_size_c; i++)
{
read(clientsocket, temp, 1);
frame = temp[0];
printf("\nReceived from the server frame no: %d", frame);
index++;
}
index = win_size_c + 1;
while(index <= frame_no_c)
{
if(ack == error_index_c && flag == 0)
{
index = ack;
for(j = ack; j < ack+win_size_c; j++)
{
read(clientsocket, temp, 1);
frame = temp[0];
index++;
}
flag = 1;
}
if(ack != error_index_c || flag == 1)
{
ack++;
i++;
read(clientsocket, temp, 1);
frame = temp[0];
printf("\nReceived from the server frame no: %d", index+1);
index++;
}
}
printf("\n");
//end of main code
close(clientsocket);
return 0;
}
