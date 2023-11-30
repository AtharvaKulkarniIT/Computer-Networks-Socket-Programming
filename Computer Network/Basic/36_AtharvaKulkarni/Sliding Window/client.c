#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024
double pow(double x, double y)  
{  
    int power = 1, i; // declare variables  
    for (i = 1; i <= y; ++i)  
    {  
        power = power * x;  
          
    }  
    return power;  
}  

double floor(double x) {
    int temp = (int)x;
    double result;
    if (temp > x) {
        result = (double)(temp - 1);
    } else {
        result = (double)temp;
    }
    return result;
}

double log2(double n) {
    int i = 0;
    while (n > 1) {
        n = n / 2;
        i++;
    }
    return i;
}

int main(int argc, char const *argv[]) {
    system("clear");
    int nf, tp, tt, nb, np, w, timer = 0, tr = 0;
    float a;
    char buffer[MAX_BUFFER_SIZE] = {0};

    printf("Enter the number of frames: ");
    scanf("%d", &nf);

    printf("Transmission delay (in ms): ");
    scanf("%d", &tt);

    printf("Propagation delay (in ms): ");
    scanf("%d", &tp);

    a = tp / tt;
    np = 1 + (2 * a);
    nb = floor(log2(np));

    if (pow(2, nb) < np) {
        w = pow(2, nb);
    } else {
        w = np;
    }

    printf("\n%d frames would be sent at a time.\n\n", w);

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char message[MAX_BUFFER_SIZE] = {0};
    char received_message[MAX_BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    for (int i = 1; i <= nf; i++) {
        if (i % w == 0) {
            printf("%d\n", i);
            printf("Above frames are sent and acknowledgement is received!!\n\n");
            sprintf(message, "%d", i);
            send(sock, message, strlen(message), 0);
            valread = read(sock, received_message, MAX_BUFFER_SIZE);
            printf("%s\n", received_message);
            timer += (tt + tp) * w;
            printf("timer: %d\n",timer);
            tr++;
        } else {
            printf("%d ", i);
        }
    }

    if (nf % w != 0) {
        tr++;
        printf("\nAbove frames are sent and acknowledgement is received!!\n");
        sprintf(message, "%d", nf);
        send(sock, message, strlen(message), 0);
        valread = read(sock, received_message, MAX_BUFFER_SIZE);
        printf("%s\n", received_message);
    }

    printf("Thus the total number of transmissions is: %d\n", tr);
    printf("Thus the total transmission time taken is: %d\n", timer);

    close(sock);
    return 0;
}
