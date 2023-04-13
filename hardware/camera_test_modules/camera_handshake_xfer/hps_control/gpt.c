#include <stdio.h>
#include <curl/curl.h>

#define SERVER_URL "192.168.137.1:8080"  // Change this to your server's WebSocket URL

int main() {
CURL *curl;
CURLcode res;
struct curl_slist *headers = NULL;

// Initialize libcurl
curl = curl_easy_init();
if (!curl) {
fprintf(stderr, "Failed to initialize libcurl\n");
return 1;
}

// Set WebSocket headers
headers = curl_slist_append(headers, "Upgrade: websocket");
headers = curl_slist_append(headers, "Connection: Upgrade");
headers = curl_slist_append(headers, "Sec-WebSocket-Version: 13");
headers = curl_slist_append(headers, "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==");
headers = curl_slist_append(headers, "Content-Type: text/plain");

// Set libcurl options
curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
curl_easy_setopt(curl, CURLOPT_HEADER, 0L);  // Don't include response headers in output
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);  // Disable response body handling

// Upgrade connection to WebSocket
res = curl_easy_perform(curl);
if (res != CURLE_OK) {
fprintf(stderr, "Failed to upgrade connection to WebSocket: %s\n", curl_easy_strerror(res));
curl_slist_free_all(headers);
curl_easy_cleanup(curl);
return 1;
}

// Send WebSocket frame with payload of 12 bytes
char payload[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\0'};
res = curl_easy_send(curl, payload, sizeof(payload) - 1, NULL);
if (res != CURLE_OK) {
fprintf(stderr, "Failed to send WebSocket frame: %s\n", curl_easy_strerror(res));
curl_slist_free_all(headers);
curl_easy_cleanup(curl);
return 1;
}

// Clean up
curl_slist_free_all(headers);
curl_easy_cleanup(curl);

return 0;
}
