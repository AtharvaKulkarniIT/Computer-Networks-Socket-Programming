
# include<stdlib.h>

# include<netinet/in.h>

# include<time.h>

# include<arpa/inet.h>

# include<string.h>

# include<unistd.h>

# include<netdb.h>

# include<stdio.h>

#include<sys/types.h>

#include<sys/socket.h>

#define PORT 8080

int main(int argc, char *argv[])

{

struct sockaddr_in serversocket;

int listensocket,i,connsd,size;

char sendbuff[100];

int j,keylen,msglen;

char input[100], key[30],temp[30],quot[100],rem[30],key1[30];

printf("\tThis is Chat Client.\n");

bzero((char *)&serversocket,sizeof(serversocket));

listensocket=socket(AF_INET,SOCK_STREAM,0);

if(listensocket<0)

printf("Socket Could Not Be Created");

else

printf("Socket Created Succesfully");

printf("\nListensocket Value : %d",listensocket);

serversocket.sin_family=AF_INET;

serversocket.sin_port=htons(PORT);

serversocket.sin_addr.s_addr=INADDR_ANY;

i = connect(listensocket,(struct sockaddr *)&serversocket,sizeof(serversocket));

if(i==0)

printf("\nConnect SUCCESS\n");

else

printf("\nConnect ERROR\n");

printf("Enter 8 bit Data: ");

fgets(input,sizeof(input),stdin);

printf("Enter Key: ");

fgets(key,sizeof(key),stdin);

keylen=strlen(key);

msglen=strlen(input);

strcpy(key1,key);

for (i=0;i<keylen-1;i++)

{

input[msglen+i]='0';

}

for (i=0;i<keylen;i++)

temp[i]=input[i];

for (i=0;i<msglen;i++)

{

quot[i]=temp[0];

if(quot[i]=='0')

for (j=0;j<keylen;j++)

key[j]='0';

else

for (j=0;j<keylen;j++)

key[j]=key1[j];

for (j=keylen-1;j>0;j--)

{

if(temp[j]==key[j])

rem[j-1]='0';

else

rem[j-1]='1';

}

rem[keylen-1]=input[i+keylen];

strcpy(temp,rem);

}

strcpy(rem,temp);

printf("\nQuotient is ");

for (i=0;i<msglen;i++)

printf("%c",quot[i]);

printf("\nRemainder is ");

for (i=0;i<keylen-1;i++)

printf("%c",rem[i]);

bzero(sendbuff,100);

printf("\nFinal data is: ");

for (i=0,j=0;i<msglen;j++,i++)

{

printf("%c",input[i]);

sendbuff[j]=input[i];

}

for (i=0,j=msglen;i<keylen-1;j++,i++)

{

printf("%c",rem[i]);

sendbuff[j]=rem[i];

}

printf("\nWriting the Final data...\n");

write(listensocket,sendbuff,strlen(sendbuff));

close(listensocket);

}

