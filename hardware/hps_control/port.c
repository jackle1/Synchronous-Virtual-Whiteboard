#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


void * foo(void * fd)
{
    int socket = *((int *) fd);
    int size = 11;
    char buf[size];
    memset(buf, 0, size);
    while (1)
    {
        int n = recv(socket, buf, size, 0);
        unsigned short x, y, colour;
        fwrite(buf, sizeof(char), size, stdout);
        printf("\n");
        sscanf(buf, "%03d%03d%d", &x, &y, &colour);
        printf("(%d, %d) %d\n", x, y, colour);
        memset(buf, 0, size);
    }
}


int main (void)
{
    struct sockaddr_in sck;
    int socket_fd = 0;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    sck.sin_family = AF_INET;
    sck.sin_port = htons(3001);
    sck.sin_addr.s_addr = inet_addr("192.168.137.1"); 
    printf("Starting connection\n");
    int ret = connect(socket_fd, (struct sockaddr *) &sck, sizeof(sck));
    if (ret < 0)
        printf("Socket failed :(");
    else
    {
        printf("Socket success\n");
        pthread_t thread;
        pthread_create(&thread, NULL, foo, (void *) &socket_fd);
        char buf[1024] = "HI";
        send(socket_fd, buf, strlen(buf), 0);
        memset(buf, 0, sizeof(buf)); 
        int i = 0;
        while (1)
        {
           sleep(1);
           sprintf(buf, "Send %d", i++); 
           send(socket_fd, buf, strlen(buf), 0);
        }
    }
    close(socket_fd);
    return 0;
}
