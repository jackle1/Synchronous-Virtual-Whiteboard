#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
char *host = "192.168.137.1";
int port = 8080;
char *message = "GET / HTTP/1.1\r\nHost: 192.168.137.1\r\n\r\n";
struct sockaddr_in server_addr;
int sockfd, bytes, total_bytes = 0;
char buffer[1024];

// Create a socket
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) {
perror("Error creating socket");
exit(EXIT_FAILURE);
}

// Set server address and port
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(port);
if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
perror("Error setting server address");
exit(EXIT_FAILURE);
}

// Connect to the server
if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
perror("Error connecting to server");
exit(EXIT_FAILURE);
}

// Send the HTTP request
bytes = send(sockfd, message, strlen(message), 0);
if (bytes < 0) {
perror("Error sending message");
exit(EXIT_FAILURE);
}

// Receive the response from the server
while ((bytes = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
total_bytes += bytes;
fwrite(buffer, 1, bytes, stdout);
}
if (bytes < 0) {
perror("Error receiving message");
exit(EXIT_FAILURE);
}

// Close the socket
close(sockfd);

return 0;
}
