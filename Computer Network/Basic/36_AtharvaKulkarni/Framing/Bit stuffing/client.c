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
    char data[100],b[100];
    int n,count=0,p=-1;
    printf("\nEnter Data:");
    scanf("%s",data);
    n=strlen(data);
   

    char flag[10]="01111110";
    char esc[2]="0";
    char  data1[100];
    int k=n;
    printf("\nflag =%s",flag);
    char ch[10]="0";
    while(k%8!=0)
    {
        strcat(data1,ch);
        k=n+strlen(data1);
        
    }
     
    strcat(data1,data);
     
    n=strlen(data1);
 
for(int i=0;i<n;i++)
{
    if(data1[i]=='0'&&data1[i+1]=='1')
    {
       
         k=i;
        count=-1;
        for(int j=0;j<9;j++)
        { 

            if(data1[k]==flag[j])
            {
            count++;
          
            k++;
            }
        }
    }
  
    if(count==8)
    {
         int m=0;
        while(m<6)
        {
            p++;
            b[p]=data1[i];
            i++;
            m++;
           

        }
        count=0;
        i--;
        p++;
        m=0;
        b[p]='0';
        while(m<2)
        {
            p++;
            b[p]=data1[i];
            i++;
            m++;
          
        }
        
               
        
    }
    
    
    if(count!=8)
    {
       
           p++; 
          b[p]=data1[i];
         
         
    }
    
    
}
char final[200];


strcat(final,flag);
strcat(final,b);
strcat(final,flag);
printf("\n%s",final);

int client_scoket;
client_scoket=socket(AF_INET,SOCK_STREAM,0);
if(client_scoket<0)
printf("\nproblem in socket creation");
struct sockaddr_in client_address;
client_address.sin_family=AF_INET;
client_address.sin_addr.s_addr=INADDR_ANY;
client_address.sin_port=htons(9000);
int con_status=connect(client_scoket,(struct sockaddr*)&client_address,sizeof(client_address));
if(con_status<0)
printf("\nproblem in connection");
write(client_scoket,final,100);
}