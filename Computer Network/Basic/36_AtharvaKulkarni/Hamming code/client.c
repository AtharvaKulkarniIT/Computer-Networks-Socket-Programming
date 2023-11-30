#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int chekbi(int i) {
    int j = (i) & (i - 1);
    return j;
}

int power(int ba, int in) {
    int res = 1;
    for (int i = 1; i <= in; i++) {
        res = res * ba;
    }

    return res;
}

int convbinary(int *dig, int l) {
    int sum = 0;
    for (int i = l - 1; i >= 0; i--) {
        sum = (power(2, i) * (*(dig + i))) + sum;
    }

    return sum;
}

void error(char *d,int l)
{
    int po = rand()%l-1;
    if(*(d+po)=='0')
    {
        *(d+po)='1';
    }
    else
    {
         *(d+po)='0';
    }
    printf("\nerror = %d\n",po);
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 5566;
    int n;
    int client_sock;
    struct sockaddr_in client_addr;
    char buffer[1024];
    socklen_t clientaddrsize;
    char data[1024];
    int p = 1;
    int o;
    int l = 0;
    int ldata;
    int c = 10; 
    int binary[8];
    int count = 0;
    int count1 = 0;
    char temp[1024];
    count = 0;
    p = 1;
    client_sock = socket(AF_INET,SOCK_STREAM,0);
    if(client_sock <0)
    {
        perror("Socket Error");
        exit(0);
    }
    printf("Server Socket Created Succesfully\n");
    memset(&client_addr,'\0',sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_port = port;
    client_addr.sin_addr.s_addr=inet_addr(ip);

    clientaddrsize=sizeof(client_addr);
    n=connect(client_sock,(struct sockaddr*)&client_addr,clientaddrsize);
    if(n<0)
    {
        perror("Connection Error");
        exit(0);
    }

    printf("You Connected to server on port %d\n",port);
    bzero(buffer,1024);
    recv(client_sock,buffer,sizeof(buffer),0);
    printf("Server : ");
    puts(buffer);
    printf("\n");
    bzero(buffer,1024);
    strcpy(buffer,"Hello Server");
    send(client_sock,buffer,sizeof(buffer),0);
    bzero(data,100);
    recv(client_sock,data,sizeof(data),0);
    ldata = strlen(data);

    error(&data[0],ldata);


    printf("Server : ");
    for (int i = strlen(data)-1; i >=1; i--)
        {
            printf("%c", data[i]);
        }

    
    count = 0;
    p = 1;
    bzero(binary,8);

    while (count < c) {
        count1 = 0;
        if (!chekbi(p)) {
            for (int i = p; i < strlen(data); i += (2 * p)) {
                for (int j = i; j < (i + p); j++) {
                    if (data[j] == '1') {
                        count1++;
                    }
                }
            }

            if (count1 % 2 == 0) {
                binary[l] = 0;
                l++;
            } else {
                binary[l] = 1;
                l++;
            }
        }
        p++; // Increment p
        count++; // Increment count
    }
    o = convbinary(&binary[0], l); // Pass the correct index
    if (o == 0) {
        printf("\nData received is Correct : ");
    } else {
        printf("\nData Received is not correct");
        printf("\nError is at %d bit", ldata-o);
        printf("\nCorrect Data : ");
        if (data[o] == '1') {
            data[o] = '0';
        } else {
            data[o] = '1';
        }
    }
    for (int i = strlen(data)-1; i>0; i--) {
        printf("%c", data[i]);
    }
    printf("\n");

    
    close(client_sock);
    printf("You Disconnected from Server\n");
    return 0;





}