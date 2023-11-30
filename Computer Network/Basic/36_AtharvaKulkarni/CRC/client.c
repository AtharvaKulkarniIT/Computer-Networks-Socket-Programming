#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{

int socket_client , serversocketfd;
struct sockaddr_in serveraddress;
struct hostent *server;

socket_client = socket(AF_INET , SOCK_STREAM , 0);

if(socket_client<0)
printf("Socket is NOT created:(\n");
else 
printf("socket is created succesfully:)\n");

socket_client = socket(AF_INET , SOCK_STREAM , 0);
serveraddress.sin_family = AF_INET;
serveraddress.sin_port = htons(9000);
serveraddress.sin_addr.s_addr= INADDR_ANY;


int connectionstatus = connect(socket_client,
                            (struct sockaddr *) &serveraddress,
                             sizeof(serveraddress));
if(connectionstatus==-1)
printf("There was an error in the connection with server:( Try again!\n");



    char data[100], crc_g[100];

    printf("\n\tEnter Data  : ");
    scanf("%s",data);
    printf("\n\tEnter CRC-G : ");
    scanf("%s",crc_g);
    
    
    int n , length_data, length_new_data;
    char new_data[100];

    n = strlen(crc_g);
    length_data = strlen(data);

    strcat(new_data , data);


    for(int i=1; i<=n-1; i++)
    {
        strcat(new_data , "0"); 
    }

    length_new_data = strlen(new_data); 
    for(int i=0; i<=(length_new_data-n);)
    {
        for (int j=0 ; j<n; j++)
        {
            new_data[i+j] = new_data[i+j]==crc_g[j] ? '0' : '1';
        }
        while(i<length_new_data && new_data[i]!='1')
        {
            i++;
        }
        
    }

 
    strncpy(new_data , &new_data[length_new_data - n +1] ,n);
    new_data[n]='\0';

    char data1[100];
    strcat(data1,data);
    strcat(data1,new_data);
    printf("\n\nData to be send to receiver is %s ",data1);
    int a=5;

 while(a!=2)
 {
    printf("\nDo you want to send error in code \n1=yes  \n0=NO \n2=exit   :");
    scanf("%d",&a);
    if(a==1)
{        printf("\nEnter the position where you want to change data:");
        int pos;
        scanf("%d",&pos);

        if(data1[pos]=='0')
        {
            data1[pos]='1';
        }
        else
        {
            data1[pos]='0';
        }
}
 
printf("\n");    
write(socket_client, data1 , 100);
write(socket_client, crc_g , 100);

printf("\n");

char msg[256];
read(socket_client , msg , 256);
printf("\nMessage from the server is : %s",msg);
printf("\n");
  
    
close(socket_client);


 }

}