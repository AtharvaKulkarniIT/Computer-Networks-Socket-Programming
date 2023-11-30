#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

void main()

{
 int server_scoket,client_socket;
 server_scoket=socket(AF_INET,SOCK_STREAM,0);
 if(server_scoket<0)
 {
 printf("\nproblem in socket creation");
 }
 struct sockaddr_in server_address,client_address;
 server_address.sin_family=AF_INET;
 server_address.sin_addr.s_addr=INADDR_ANY;
 server_address.sin_port=htons(9000);
 int bind_status=bind(server_scoket,(struct sockaddr*)&server_address,sizeof(server_address));
 if(bind_status<0)
 printf("\nproblem in connection");
 listen(server_scoket,5);
 int clength=sizeof(client_address);
 client_socket=accept(server_scoket,(struct sockaddr*)&client_address,&clength);
char data[100];
 read(client_socket,data,100);


    char b[100];
    int n,count=0,p=-1;
   
    n=strlen(data);
   

    char flag[10]="01111110";
    char esc[10]="00011011";
    
    int k=n;


    char data1[100];
    strcpy(data1,data);
  
    n=strlen(data1);
    char start[10];
   start[0]=data[0];
   start[1]=data[1];
   start[2]=data[2];
   start[3]=data[3];
   start[4]=data[4];
   start[5]=data[5];
   start[6]=data[6];
   start[7]=data[7];
   
     int pointer=0;
     
int counter=0;
 for(int i=0;i<8;i++)
 { 
     if(start[i]==data[i])
     {
         counter++;
     }
    
 }

 if(counter==8)
 {
     
for(int i=8;i<n;i++)

{

    if(pointer!=1 )
  {

    
    if(data[i]=='0'&&data[i+1]=='1')
    {

       
         k=i;
        count=-1;
        for(int j=0;j<9;j++)
        { 

            if(data[k]==flag[j])
            {
            count++;
          
            k++;
            }

        }
        if(count==8)
        {
            pointer=1;
            break;

        }
    }
    else 
    {   
       
        k=i;
        count=0;
         for(int j=0;j<9;j++)
        {
            if(data1[k]==esc[j])
            {
            count++;
           
            k++;
            }
            else
            {
                break;
            }
           
        }
    }
    if(count==8 && pointer!=1)
    {
        i+=8;
        
    }
    
    if(pointer!=1)
    {
       for(int j=i;j<(i+8);j++)
        {
            p++;
            
          b[p]=data1[j];
         
        }
        i+=7;
    }
  }
    
}
}
else
{
   printf("\nfalied");
}

char final[200];

printf("\nb=%s",b);



}