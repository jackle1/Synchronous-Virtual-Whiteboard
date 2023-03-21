#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define URL "https://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production"

static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
    struct timeval tv;
    fd_set infd, outfd, errfd;
    int res;

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    FD_ZERO(&infd);
    FD_ZERO(&outfd);
    FD_ZERO(&errfd);

    FD_SET(sockfd, &errfd); /* always check for error */

    if (for_recv)
    {
        FD_SET(sockfd, &infd);
    }
    else
    {
        FD_SET(sockfd, &outfd);
    }

    /* select() returns the number of signalled sockets or -1 */
    res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
    return res;
}


int main(void)
{
    CURL *curl;
    volatile CURLcode res, rec_res;
    curl_socket_t sockfd;
    size_t nsent_total = 0;
    FILE * file = NULL; 
    char request[1024];
    memset(request, 0, sizeof(request));
    sprintf(request, "GET /production HTTP/1.1\r\n"
        "Host: 192.168.137.1:8080\r\n"
        // "Host: 7nbl97eho0.execute-api.us-east-1.amazonaws.com/production \r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        // "Sec-WebSocket-Protocol: chat, superchat\r\n"
        "\r\n");
    curl = curl_easy_init();
    if (curl)
    {
       curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.137.1:8080"); 
       // curl_easy_setopt(curl, CURLOPT_URL, URL);
       curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
       curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
       curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
       curl_easy_setopt(curl, CURLOPT_HEADER, 0L);  // Don't include response headers in output
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);  // Disable response body handling
       res = curl_easy_perform(curl);
       if (res != CURLE_OK)
       {
           fprintf(stderr, "curl_easy_perform() failed: %s\n",
           curl_easy_strerror(res));
       } else {
            res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
            size_t buf = 0;
            size_t recBuf = 0;
            char recvBuf[1024] = {0};
            printf("header len %d\n", strlen(request));
            res = curl_easy_send(curl, request, strlen(request), &buf);
            wait_on_socket(sockfd, 0, 60000L);
            char toSend[] = {0x81, 0x85, 0x00, 0x00, 0x00, 0x00, 'h', 'e', 'l', 'l', 'o'};
            // char toSend[] = {129, 136, 136, 35, 93, 205, 231, 85, 56, 191, 177, 19, 109, 253};
            res = curl_easy_send(curl, toSend, sizeof(toSend), &buf);
            wait_on_socket(sockfd, 0, 60000L);
            while (res != CURLE_OK);
            printf("done sending\n");
            rec_res = curl_easy_recv(curl, recvBuf, sizeof(recvBuf), &recBuf);
            wait_on_socket(sockfd, 0, 60000L);
            while(rec_res == CURLE_AGAIN);
            printf("done receiving\n");
            fwrite(recvBuf, sizeof(char), sizeof(recvBuf), stdout);
       }
       curl_easy_cleanup(curl);    
    }
    return 0;
}
/**
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() 
{
    struct sockaddr_in server_addr;
    int socket_fd, n;
    char buffer[1024];

    // Create a socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
    printf("Failed to create socket\n");
    return 1;
    }

    // Set the server address and port
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "50.17.61.0", &server_addr.sin_addr) <= 0) {
    printf("Invalid address/ Address not supported\n");
    return 1;
    }

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
    printf("Failed to connect to server\n");
    return 1;
    }

    // Send the WebSocket upgrade request
    char request[1024];
    sprintf(request, "GET /websocket HTTP/1.1\r\n"
    "Host: localhost:8080\r\n"
    "Upgrade: websocket\r\n"
    "Connection: Upgrade\r\n"
    "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
    "Sec-WebSocket-Version: 13\r\n"
    "\r\n");
    send(socket_fd, request, strlen(request), 0);

    // Receive the server's WebSocket upgrade response
    n = recv(socket_fd, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    printf("Received:\n%s\n", buffer);

    // Send a WebSocket message to the server
    char message[1024];
    sprintf(message, "\x81\x05\x48\x65\x6c\x6c\x6f");
    send(socket_fd, message, strlen(message), 0);

    // Receive a WebSocket message from the server
    n = recv(socket_fd, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    printf("Received:\n%s\n", buffer);

    // Close the connection
    close(socket_fd);

    return 0;
}
**/
