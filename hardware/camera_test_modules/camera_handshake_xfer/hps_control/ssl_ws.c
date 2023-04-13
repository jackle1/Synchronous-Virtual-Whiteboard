#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main() {
    struct sockaddr_in server_addr;
    int socket_fd, n;
    char buffer[1024];

    // Initialize SSL library
    SSL_library_init();
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLS_client_method());

    // Create a socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
    printf("Failed to create socket\n");
    return 1;
    }

    // Set the server address and port
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(443);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "7nbl97eho0.execute-api.us-east-1.amazonaws.com", &server_addr.sin_addr) <= 0) {
    printf("Invalid address/ Address not supported\n");
    return 1;
    }

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
    printf("Failed to connect to server\n");
    return 1;
    }

    // Create an SSL connection
    SSL *ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, socket_fd);
    if (SSL_connect(ssl) <= 0) {
    printf("Failed to establish SSL connection\n");
    ERR_print_errors_fp(stderr);
    return 1;
    }

    // Send the WebSocket upgrade request
    char request[1024];
    sprintf(request, "GET /production HTTP/1.1\r\n"
    "Host: 7nbl97eho0.execute-api.us-east-1.amazonaws.com\r\n"
    "Upgrade: websocket\r\n"
    "Connection: Upgrade\r\n"
    "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
    "Sec-WebSocket-Version: 13\r\n"
    "Sec-WebSocket-Protocol: chat\r\n"
    "\r\n");
    SSL_write(ssl, request, strlen(request));

    // Receive the server's WebSocket upgrade response
    n = SSL_read(ssl, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("Received:\n%s\n", buffer);

    // Send a WebSocket message to the server
    char message[1024];
    sprintf(message, "\x81\x05\x48\x65\x6c\x6c\x6f");
    SSL_write(ssl, message, strlen(message));

    // Receive a WebSocket message from the server
    n = SSL_read(ssl, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("Received:\n%s\n", buffer);

    // Close the SSL connection and socket
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(socket_fd);

    return 0;
}
