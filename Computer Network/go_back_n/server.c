#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{
int serversocket, bindstatus, client_socket;
struct sockaddr_in serveraddress;
char servermessage[256] = "Hello everyone!!! Welcome to socket programming!!!!!\n";
serversocket = socket(AF_INET, SOCK_STREAM, 0);
serveraddress.sin_family = AF_INET;
serveraddress.sin_port = htons(9000);
serveraddress.sin_addr.s_addr = INADDR_ANY;
bindstatus = bind(serversocket, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
if(bindstatus < 0)
{
printf("Binding Failed");
}
else
{
printf("Binding Successfull\n");
}
listen(serversocket, 3);
printf("Send reply to the client\n");
client_socket = accept(serversocket, NULL, NULL);
//main code
send(client_socket, servermessage, sizeof(servermessage), 0);
int frame_no, win_size, error_index, i, index=0, ack=0, flag=0, j;
int array[3], temp[1];
printf("\nEnter the number of frames you want to transmit: ");
scanf("%d", &frame_no);
printf("\nEnter the window size: ");
scanf("%d", &win_size);
printf("Enter the index of the frame where error is to be shown: ");
scanf("%d", &error_index);
array[0] = frame_no;
array[1] = win_size;
array[2] = error_index;
write(client_socket, array, 3);
for(i = 1; i <= win_size; i++)
{
temp[0] = i;
write(client_socket, temp, 1);
printf("\nSending the client frame no: %d", i);
}
index = win_size + 1;
while(index <= frame_no)
{
if(ack == error_index || flag == 0)
{
index = ack;
printf("\nNo Acknowledgement received from the client: ");
for(j = ack+1; j <= ack+win_size; j++)
{
temp[0] = j;
write(client_socket, temp, 1);
printf("\nSending the client frame no: %d", j);
index++;
}
flag = 1;
}
if(ack != error_index || flag == 1)
{
printf("\nReceived Acknowledgement for frame no: %d", ack+1);
printf("\nSending the client frame no: %d", index+1);
temp[0] = index;
write(client_socket, temp, 1);
i++;
ack++;
index++;
}
}
ack--;
while(ack < frame_no)
{
printf("\nReceived Acknowledgement for frame no: %d", ack+1);
ack++;
}
printf("\n");
//end of main code
close(serversocket);
return 0;
}
