/**
 * References from libcurl library
 * curl_easy_send() & curl_easy_recv() from
 *  https://curl.se/libcurl/c/sendrecv.html
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "terasic_includes.h"
#include "ring_buffer.c"

#define WS_DEBUG 1

enum res_state_e
{
    CLOUD_WAIT,
    CLOUD_DONE,
    CLOUD_ERROR
};

#define GET_ROOM_PIXEL_URL "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID="
#define GET_ROOM_PIXEL_URL_LEN 79
#define COLOUR_PIXEL_URL "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"
#define COLOUR_PIXEL_BODY_LEN 130
#define BULK_PIXEL_FILE "bulk_send.json"
#define WS_URL "https://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production"

#define WS_SIZE_BYTE 1
#define WS_SIZE_MASK (0x7F)
#define WS_REC_HEADER_SIZE (2)
#define WS_ROOM_CONNECT_SIZE (45)
char WS_HEADER[] = {0x81, 0x85, 0x00, 0x00, 0x00, 0x00}; // 2nd Byte is (0x80 | Size) for mask and size
#define WS_PIXEL_SEND_F "{\"action\": \"post\", \"roomID\": %d, \"user\": \"User\", \"RGB\": RGB, %hu: x, \"y\": %hu}"
#define WS_PIXEL_RECV_SIZE (WS_REC_HEADER_SIZE + 10 + 3 + 3 + 2) // 10-Digit RGB + 3 Digit X + 3 Digit Y + 2 Commas + 2 Spaces

typedef struct
{
    char done;
    FILE *file;
} http_res_t;

void getRoomPixels(volatile http_res_t *res, uint16_t roomID);
static size_t getRoomPixelsCallBack(char *ptr, size_t size, size_t nmemb, void *userdata);
void putRoomPixels(uint16_t roomID, uint16_t x, uint16_t y, uint32_t colour);
void sendBulkPixel(pixel_t *head, uint16_t roomID);
CURL *initWSConn(uint16_t roomID);
void *readWSConn(void * args_pointer);
void sendWSPixel(uint16_t x, uint16_t y, uint32_t colour, CURL * curl);

static size_t resToBuf(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    char *buf = (char *)userdata;
    for (int i = 0; i < nmemb; i++)
        buf[i] = ptr[i];
    return size * nmemb;
}

static size_t getRoomPixelsCallBack(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    http_res_t *buf = (http_res_t *)userdata;
    fwrite(ptr, size, nmemb, buf->file);
    buf->done = CLOUD_DONE;
    return size * nmemb;
}

void getRoomPixels(volatile http_res_t *res, uint16_t roomID)
{
    CURL *curl;
    CURLcode http_res;
    char url[GET_ROOM_PIXEL_URL_LEN];
    sprintf(url, "%s%d", GET_ROOM_PIXEL_URL, roomID);
    printf("getRoomPixels URL: %s\n", url);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getRoomPixelsCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);
        http_res = curl_easy_perform(curl);
        if (http_res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(http_res));
            res->done = CLOUD_ERROR;
        }
        curl_easy_cleanup(curl);
    }
    else
        fprintf(stderr, "CURL FAILED!!!\n");
}

void putRoomPixels(uint16_t roomID, uint16_t x, uint16_t y, uint32_t colour)
{
    CURL *curl;
    CURLcode res;
    char buf[COLOUR_PIXEL_BODY_LEN];
    char res_buf[1024];
    memset(res_buf, 0, sizeof(res_buf));
    sprintf(buf, "{\"member\": \"De1-Soc\",\"roomID\": %d,\"RGB\" : %d,\"request-for\": 1,\"x\": %d,\"y\": %d}",
            roomID, colour, x, y);
    printf("%s\n", buf);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, COLOUR_PIXEL_URL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resToBuf);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, res_buf);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
            printf("%s\n", res_buf);
        curl_easy_cleanup(curl);
    }
    else
        fprintf(stderr, "CURL FAILED!!!\n");
}

void sendCameraPic(const char *path)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    FILE *fp;
    char *url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391";

    // Initialize curl
    curl = curl_easy_init();
    if (curl)
    {
        // Open the file containing the request body data
        fp = fopen(path, "r");
        if (fp)
        {
            // Set curl options
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_READDATA, fp);
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Perform the request
            res = curl_easy_perform(curl);

            // Check for errors
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            // Clean up
            fclose(fp);
        }
        else
        {
            fprintf(stderr, "Failed to open file '%s'\n", path);
        }

        curl_easy_cleanup(curl);
    }
}

CURL *initWSConn(uint16_t roomID)
{
    char request[1024] = {0};
    size_t frame_size;
    CURL *curl = NULL;
    size_t recBuf;
    CURLcode res = CURLE_AGAIN;
    printf("Init Web Socket Connection\n");

    curl = curl_easy_init();
    frame_size = sprintf(request, "GET /production HTTP/1.1\r\n"
                                  "Host: 7nbl97eho0.execute-api.us-east-1.amazonaws.com \r\n"
                                  "Upgrade: websocket\r\n"
                                  "Connection: Upgrade\r\n"
                                  "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
                                  "Sec-WebSocket-Version: 13\r\n"
                                  "\r\n");
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, WS_URL);
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#if WS_DEBUG == 1
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
        res = curl_easy_perform(curl);
        res = curl_easy_send(curl, request, frame_size, &recBuf); // Upgrade to WS Connection
        res = CURLE_AGAIN;
        memset(request, 0, frame_size);
        do {res = curl_easy_recv(curl, request, sizeof(request), &recBuf);} while (res != CURLE_OK);
#if WS_DEBUG == 1
        printf("Upgrade Command got %zu\n", recBuf);
        fwrite(request + WS_REC_HEADER_SIZE, sizeof(char), recBuf - WS_REC_HEADER_SIZE, stdout);
        fflush(stdout);
        printf("\n");
#endif
        memset(request, 0, recBuf);
        for (int i = 0; i < sizeof(WS_HEADER); i++)
            request[i] = WS_HEADER[i];
        request[WS_SIZE_BYTE] = (0x80) |
                                sprintf(request + sizeof(WS_HEADER), "{\"action\": \"connect_to_roomID\", \"roomID\": %hu, \"user\": \"De1\"}", roomID);
        res = curl_easy_send(curl, request, sizeof(WS_HEADER) + (request[WS_SIZE_BYTE] & WS_SIZE_MASK), &recBuf);
        res = CURLE_AGAIN;
        // Only want to receive header + connect message - DONT want to include potential pixels
        do {res = curl_easy_recv(curl, request, WS_ROOM_CONNECT_SIZE + WS_REC_HEADER_SIZE, &recBuf);} while (res != CURLE_OK);
#if WS_DEBUG == 1
        printf("Room Connect Command got %zu\n", recBuf);
        fwrite(request + WS_REC_HEADER_SIZE, sizeof(char), recBuf - WS_REC_HEADER_SIZE, stdout);
        fflush(stdout);
        printf("\n");
#endif
        return curl;
    }
    else
    {
        printf("Curl Init Failed For Socket???\n");
        curl_easy_cleanup(curl);
        return NULL;
    }
}

void *readWSConn(void * args_pointer)
{
    void ** args = (void **) args_pointer;
    CURL * curl = (CURL *) args[0];
    volatile uint32_t * vga_base = (volatile uint32_t *) args[1];
    volatile uint8_t * stop = (volatile uint8_t *) args[2];
    ring_buffer_t * rb = (ring_buffer_t *) args[3];
    
    CURLcode rec = CURLE_AGAIN;
    size_t rec_size, send_size;

    if (curl == NULL)
    {
        printf("CURL Object For WS Connection is NULL\n");
    } else
    {
        printf("Waiting on WS Connection for pixels...\n");
        while (1)
        {
            uint8_t recv[WS_PIXEL_RECV_SIZE] = {0};
            rec = curl_easy_recv(curl, recv, sizeof(recv), &rec_size);
            if (rec == CURLE_OK)
            {
                uint16_t x, y;
                uint32_t colour;
                fwrite(recv, sizeof(char), rec_size, stdout);
                printf("\n");
                if (3 == sscanf(recv + WS_REC_HEADER_SIZE, "%u,%hu,%hu", &colour, &x, &y))
                    vga_base[x + VGA_X * y] = colour;
            }
            // } else if (ring_buffer_size(rb) > 0)
            // {
            //     pixel_t pixel;
            //     ring_buffer_remove(rb, &pixel);
            //     int coord_colour_size = 3 + 3 + 10;
            //     char send_buf[sizeof(WS_HEADER) + sizeof(WS_PIXEL_SEND_F) + coord_colour_size + 1];
            //     for (int i = 0; i < sizeof(WS_HEADER); i++)
            //         send_buf[i] = WS_HEADER[i];
            //     size_t buf_len = sprintf(send_buf + sizeof(WS_HEADER), WS_PIXEL_SEND_F, pixel.x, pixel.y, pixel.colour);
            //     rec = curl_easy_send(curl, send_buf, sizeof(WS_HEADER) + buf_len, &send_size);
            //     if (rec != CURLE_OK)
            //     {
            //         printf("ERROR: Something went wrong with sending the pixel");
            //     }
            // }
        }
    }
}

/**
int main(void)
{
    sendCameraPic("post.txt");
    volatile http_res_t * res = (http_res_t *) malloc(sizeof(http_res_t));
    res->done = CLOUD_WAIT;
    res->file = fopen("testget.txt", "w");
    getRoomPixels(res, 8862);
    // putRoomPixels(8862, 0, 0, 0);
}
**/
