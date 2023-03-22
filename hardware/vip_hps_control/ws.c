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
    CURLcode res;
    CURLcode rec_res = CURLE_AGAIN;
    curl_socket_t sockfd;
    size_t nsent_total = 0;
    FILE * file = NULL; 
    char request[1024];
    memset(request, 0, sizeof(request));
    sprintf(request, "GET /production HTTP/1.1\r\n"
        // "Host: 192.168.137.1:8080\r\n"
        "Host: 7nbl97eho0.execute-api.us-east-1.amazonaws.com \r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        // "Sec-WebSocket-Protocol: chat, superchat\r\n"
        "\r\n");
    curl = curl_easy_init();
    if (curl)
    {
       // curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.137.1:8080"); 
       curl_easy_setopt(curl, CURLOPT_URL, URL);
       curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
       curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
       curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
       // curl_easy_setopt(curl, CURLOPT_HEADER, 0L);  // Don't include response headers in output
       // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);  // Disable response body handling
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
            while (rec_res == CURLE_AGAIN) rec_res = curl_easy_recv(curl, recvBuf, sizeof(recvBuf), &recBuf);
            rec_res = CURLE_AGAIN;
            memset(recvBuf, 0, recBuf);
            char sendHeader[] = {0x81, 0x85, 0x00, 0x00, 0x00, 0x00};
            char toSend[1024];
            memset(toSend, 0, sizeof(toSend));
            for (int i = 0; i < sizeof(sendHeader); i++)
                toSend[i] = sendHeader[i];
            toSend[1] = (0x80) | sprintf(toSend + sizeof(sendHeader), "{\"action\": \"connect_to_roomID\", \"roomID\": 8862, \"user\": \"Demo\"}");
            res = curl_easy_send(curl, toSend, sizeof(sendHeader) + toSend[1] & 0x7F, &buf);
            printf("Sent %d vs Should %d\n", buf, sizeof(sendHeader) + toSend[1]);
            wait_on_socket(sockfd, 0, 60000L);
            while (res != CURLE_OK);
            printf("done sending\n");
            while (rec_res == CURLE_AGAIN) rec_res = curl_easy_recv(curl, recvBuf, sizeof(recvBuf), &recBuf);
            printf("done receiving\n");
            fwrite(recvBuf + 2, sizeof(char), recvBuf[1], stdout);
            printf("\n");
            memset(toSend, 0, recBuf);
            memset(recvBuf, 0, recBuf);
            for (int i = 0; i < sizeof(sendHeader); i++)
                toSend[i] = sendHeader[i];
            toSend[1] = (0x80) | sprintf(toSend + sizeof(sendHeader), "{\"action\": \"disconnect_roomID\", \"roomID\": 8862, \"user\": \"Demo\"}");
            res = curl_easy_send(curl, toSend, 0x7F & toSend[1] + sizeof(sendHeader), &buf);
            wait_on_socket(sockfd, 0, 60000L);
            while (res != CURLE_OK);
            printf("done sending\n");
            rec_res = CURLE_AGAIN;
            while (rec_res == CURLE_AGAIN) rec_res = curl_easy_recv(curl, recvBuf, sizeof(recvBuf), &recBuf);
            printf("done receiving %d bytes\n", recBuf);
            fwrite(recvBuf + 2, sizeof(char), recvBuf[1], stdout);
            printf("\n");
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
