#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main()
{

int socket_server , clientsocketfd, bindstatus;

struct sockaddr_in serveraddress , clientaddress;

socket_server = socket(AF_INET , SOCK_STREAM , 0);
serveraddress.sin_family = AF_INET;
serveraddress.sin_port = htons(9000);
serveraddress.sin_addr.s_addr = INADDR_ANY;

bindstatus = bind( socket_server , 
                   (struct sockaddr *)&serveraddress , 
                    sizeof(serveraddress) 
                  );
                  
if (bindstatus<0)
printf("Binding Failed\n");
else
printf("Binding is successful\n");

listen(socket_server , 10);
printf("Send reply to the client\n");


int cliaddlen = sizeof(clientaddress);
clientsocketfd = accept(socket_server , (struct sockaddr *)&clientaddress, &cliaddlen );






    char data1[100],crc_g[100];
    
    read(clientsocketfd , data1 , 100);
    read(clientsocketfd , crc_g, 100);

 
 
    int n, length_data1;
    n = strlen(crc_g);
    length_data1 = strlen(data1);



    for(int i=0; i<=(length_data1-n);)
    {
        for (int j=0 ; j<n; j++)
        {
            data1[i+j] = data1[i+j]==crc_g[j] ? '0' : '1';
        }
        while(i<length_data1 && data1[i]!='1')
        {
            i++;
        } 
    }

 
    printf("\n\nFinal Answer is %s",data1);
    for(int i=0; i<length_data1;i++)
    {
        if (data1[i]=='1')
        { 
            char msg[256] = "Error in Communication! Can't proceed further:(";
            write(clientsocketfd , msg , sizeof(msg));
            break;
            
        }    
        else if(data1[length_data1-1]!='1' && data1[length_data1]!='1')
        {
            char msg[256] = "NO Error in Communication! Can be proceed further:)";
            write(clientsocketfd , msg , sizeof(msg));
        }   
    } 

    printf("\n\n");

 
close(socket_server);

return 0;

}
