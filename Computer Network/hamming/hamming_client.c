#include <stdio.h>
#include <math.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

char input1[32];
int input[32];
int code[32];
char code1[100];
int ham_calc(int,int);
int main()
{
 int n,i,c_l,p_n=0,j,k;
 printf("Please enter the Data Word:\n");
 scanf("%s",input1);
 n=strlen(input1);
 for(i=0;i<n;i++)
 {
     if(input1[i]=='1')
     {
         input[i]=1;

     }
     else
     {
         input[i]=0;

     }

 }
 i=0;
 while(n>(int)pow(2,i)-(i+1))
 {
     p_n++;
     i++;

 }
 c_l = p_n + n;
 j=k=0;
 for(i=0;i<c_l;i++){
     if(i==((int)pow(2,k)-1))
     {
         code[i]=0;
         k++;

     }
     else
     {
         code[i]=input[j];
         j++;

     }

 }
 for(i=0;i<p_n;i++)
 {
     int position = (int)pow(2,i);
     int value = ham_calc(position,c_l);
     code[position-1]=value;

 }
 printf("\nThe calculated Code Word is: ");
 for(i=0;i<c_l;i++)
 printf("%d",code[i]);

 printf("\nEnter your choice\n1.send with error\n2.send without error:");
 int ans;
 scanf("%d",&ans);
 if(ans==1)
 {
   printf("\nEnter the postion where you want the error");
   int pos;
   scanf("%d",&pos);
   if(code[pos]==1)
   {
       code[pos]=0;
   }
   else
   {
       code[pos]=1;
   }
 }
 int n1=sizeof(code)/sizeof(code[0]);
 printf("\n");
 for(i=0;i<c_l;i++)
 {
     if(code[i]==1)
     {
         code1[i]='1';

     }
     else
     {
         code1[i]='0';

     }

 }

   //printf("\n %s",code1);

   printf("final data:\n");
 for(i=0;i<c_l;i++)
 printf("%d",code[i]);

    int client_socket;
    client_socket=socket(AF_INET,SOCK_STREAM,0);
    if(client_socket<0)
    printf("\n Prolem in socket creation");
    else
    printf("\nSocket is created");

    struct sockaddr_in client_address;
    client_address.sin_family=AF_INET;
    client_address.sin_addr.s_addr=INADDR_ANY;
    client_address.sin_port  =htons(9000);

    int con_status=connect(client_socket,(struct sockaddr*)&client_address,sizeof(client_address));
    if(con_status<0)
    printf("\nProblem in connection");
    else
    printf("\nconnect is established\n")  ;
    write(client_socket,code1,sizeof(code1));
    write(client_socket,&p_n,sizeof(p_n));
    write(client_socket,&n,sizeof(n));

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
