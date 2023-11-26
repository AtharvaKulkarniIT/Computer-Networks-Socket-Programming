#include <stdio.h>
#include <math.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
int input[32];
int code[50];
char code1[100];
int ham_calc(int,int);

// int pow(int base,int power){
//     if(power==0) return 1;
//     return pow(base,power-1)*base;
// }

void main()
{
   int n,i,p_n=0,c_l,j,k;

   int server_socket,client_socket;
   server_socket=socket(AF_INET,SOCK_STREAM,0);
   struct sockaddr_in server_address,client_address;
   server_address.sin_family=AF_INET;
   server_address.sin_addr.s_addr=INADDR_ANY;
   server_address.sin_port=htons(9000);

   int bind_status=bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
   if(bind_status<0)
   printf("\n problem in binding.....");
   else
   printf("\nbinded...........");

   listen(server_socket,5);
   int clength=sizeof(client_address);
   client_socket=accept(server_socket,(struct sockaddr*)&client_address,&clength);
   read(client_socket,code1,sizeof(code1));
   read(client_socket,&p_n,sizeof(p_n));
   read(client_socket,&n,sizeof(n));
   int n1=strlen(code1);
   for(i=0;i<n1;i++)
   {
      if(code1[i]=='1')
      {
         code[i]=1;

      }
      else
      {
         code[i]=0;

      }

   }
   printf("\n p_n=%d,n=%d,n1=%d",p_n,n,n1);
   c_l=p_n+n;
   printf("\nThe receieved Code Word is: ");
   for(i=0;i<n1;i++){
      printf("%d",code[i]);

   }
   printf("\n");
   int error_pos = 0;
   for(i=0;i<p_n;i++)
   {
      int position = (int)pow(2,i);
      int value = ham_calc(position,c_l);
      if(value != 0)
      error_pos+=position;

   }
   if(error_pos == 0)
   printf("The received Code Word is correct.\n");
   else
   printf("Error at bit position: %d\n",error_pos-1);
}
int ham_calc(int position,int c_l)
{
   int count=0,i,j;
   i=position-1;
   while(i<c_l)
   {
      for(j=i;j<i+position;j++)
      {
         if(code[j] == 1)
         count++;

      }
      i=i+2*position;

   }
   if(count%2 == 0)
   return 0;
   else
   return 1;
}
