#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void datarev(char *data,int l)
{
    l=l-1;
    int n=l;
    char temp;
    while(n>l/2)
    {
        temp=*(data+n);
        (data+n)=(data+(l%n));
        *(data+(l%n))=temp;
        n--;
    }
    *(data+l+1)='\0';
}

int chekbi(int i)
{
    int j = (i) & (i-1);
    return j;
}

int countpowtwo(char *main,int l)
{
    int c=0;
    for(int i=0;i<l;i++)
    {
        if(*(main+i)=='x')
        {
            c++;
        }
    }

        return c;

}

int main()
{
    char *ip = "127.0.0.1";
    int port = 5566;
    int n;
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t serveraddrsize, clientaddrsize;
    char data[1024];
    char data1[1024];
    char maindata[1024];
    int p = 1;
    int k = 0;
    int ldata;
    int c;
    int count = 0;
    int count1 = 0;
    char temp[1024];

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("Socket Error");
        exit(0);
    }
    printf("Server Socket Created Succesfully\n");
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    serveraddrsize = sizeof(server_addr);
    n = bind(server_sock, (struct sockaddr *)&server_addr, serveraddrsize);
    if (n < 0)
    {
        perror("Binding Error");
        exit(0);
    }
    printf("Socket Bind Successfully on port %d\n", port);
    printf("Listening.....\n");
    listen(server_sock, 5);
    while (1)
    {
        clientaddrsize = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &clientaddrsize);
        if (client_sock < 0)
        {
            perror("Accepting error");
            exit(0);
        }
        printf("The Client Connected Succesfully\n");
        bzero(buffer, 1024);
        strcpy(buffer, "Hello Client");
        send(client_sock, buffer, sizeof(buffer), 0);
        bzero(buffer, 1024);
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client : ");
        puts(buffer);
        printf("\n");

        // Hamming code for server 
        printf("Enter the data : ");
        scanf(" %s", data);
        ldata = strlen(data);
        printf("\nEntered data %s", data);
        datarev(&data[0], ldata);
        printf("\nReverse data %s", data);

        while (data[k] != '\0')
        {
            if (!chekbi(p))
            {
                maindata[p] = 'x';
            }
            else
            {
                maindata[p] = data[k];
                k++;
            }
            p++;
        }
        maindata[p] = '\0';
        int t = p;
        p = 1;


        c = countpowtwo(&maindata[0], strlen(maindata));
        printf(" %d", c);
        while (count < c)
        {
            count1 = 0;
            if (!chekbi(p))
            {
                for (int k = p; k < t; k += (2 * p))
                {
                    for (int j = k; j < (k + p); j++)
                    {
                        if (maindata[j] == '1')
                        {
                            count1++;
                        }
                    }
                }
                if (count1 % 2 == 0)
                {
                    // Even pairity
                    maindata[p] = '0';
                }
                else
                {
                    maindata[p] = '1';
                }
                count++;
            }
            p++;
        }
        printf("\n Data send = ");
        for (int i = strlen(maindata)-1; i >=1; i--)
        {
            printf("%c", maindata[i]);
        }
        send(client_sock,maindata,sizeof(maindata),0);
        printf("\n");

        close(client_sock);
        printf("Client DIsconneted\n");
    }

    return 0;
}