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
#include <time.h>
#include "terasic_includes.h"
#include "pixel_buffer.c"
#include "main.h"

#define WS_DEBUG 0
#define WS_LOCAL_HOST 0

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
#if WS_LOCAL_HOST == 1
#define WS_URL "http://192.168.137.1:8080"
#else
#define WS_URL "https://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production"
#endif
#define SEND_PIXEL_BUF 16384

#define WS_SIZE_BYTE 1
#define WS_SIZE_MASK (0x7F)
#define WS_REC_HEADER_SIZE (2)
#define WS_ROOM_CONNECT_SIZE (45)
static uint8_t WS_HEADER[] = {0x81, 0x80, 0x00, 0x00, 0x00, 0x00}; // 2nd Byte is (0x80 | Size) for mask and size
#define WS_PIXEL_SEND_F "{\"action\":\"post\",\"roomID\":%d,\"user\":\"De1-%hu\",\"pixel\":["
#define WS_PIXEL_RECV_SIZE 12000
#define WS_EX_PAYLOAD_BASE 8
#define WS_WAIT_TIME 700

typedef struct
{
    char done;
    FILE *file;
} http_res_t;

void getRoomPixels(volatile http_res_t *res, uint16_t roomID);
static size_t getRoomPixelsCallBack(char *ptr, size_t size, size_t nmemb, void *userdata);
void putRoomPixels(uint16_t roomID, uint16_t userID, uint16_t x, uint16_t y, uint32_t colour);
void sendBulkPixel(const char * camera_pic, uint16_t roomID, uint16_t userID);
void sendCameraPic(const char *path);
CURL *initWSConn(uint16_t roomID, uint16_t userID);
void *readWSConn(void * args_pointer);
void sendWSPixel(uint16_t x, uint16_t y, uint32_t colour, CURL * curl);
static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms);
void changeWSMask();
void maskWSMsg(char * buf, size_t buflen);

static uint8_t WS_RECV_BUF[WS_PIXEL_RECV_SIZE];
static uint8_t WS_SEND_BUF[SEND_PIXEL_BUF];
static bool send_get_req = FALSE;


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

void putRoomPixels(uint16_t roomID, uint16_t userID, uint16_t x, uint16_t y, uint32_t colour)
{
    CURL *curl;
    CURLcode res;
    char buf[COLOUR_PIXEL_BODY_LEN];
    char res_buf[1024];
    memset(res_buf, 0, sizeof(res_buf));
    sprintf(buf, "{\"member\": \"De1-%hu\",\"roomID\": %d,\"RGB\" : %d,\"request-for\": 1,\"x\": %d,\"y\": %d}",
            userID, roomID, colour, x, y);
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

void sendBulkPixel(const char * camera_pic, uint16_t roomID, uint16_t userID)
{
    /** Format example
        {
            "member": "De1",
            "roomID": 8862,
            "RGB" : 1,
            "request-for": 1,
            "x": 0,
            "y": 0
        }
    */
    FILE * fp = NULL;
    FILE * camera_fd = NULL;

    fp = fopen(BULK_PIXEL_FILE, "w");
    camera_fd = fopen(camera_pic, "r");
    
    if (fp && camera_fd)
    {
        char buf[50] = {0};
        int ret = sprintf(buf, "{\"member\":\"De1-%hu\",\"roomID\":%d,\"request-for\":1,", userID, roomID);
        fwrite(buf, sizeof(char), ret, fp); // Write header

        // Write RGB values first
        char rgb[] = "\"RGB\":[";
        fwrite(rgb, sizeof(char), sizeof(rgb) - 1, fp);
        uint32_t colour;
        for (int i = 0; i < VGA_X * VGA_Y; i++)
        {
            fscanf(camera_fd, "%u,", &colour);
            if (i < VGA_X * VGA_Y - 1)
                fprintf(fp, "%u,", colour);
            else
                fprintf(fp, "%u],", colour);
        }

        // Write x values
        char x[] = "\"x\":[";
        fwrite(x, sizeof(char), sizeof(x) - 1, fp);
        for (int i = 0; i < VGA_Y; i++)
        {
            for (int j = 0; j < VGA_X; j++)
                if (i == VGA_Y - 1 && j == VGA_X -1)
                    fprintf(fp, "%hu],", j);
                else
                    fprintf(fp, "%hu,", j);
        }

        // Write y values
        char y[] = "\"y\":[";
        fwrite(y, sizeof(char), sizeof(y) - 1, fp);
        for (int i = 0; i < VGA_Y; i++)
        {
            for (int j = 0; j < VGA_X; j++)
                if (i == VGA_Y - 1 && j == VGA_X -1)
                    fprintf(fp, "%hu]", i);
                else
                    fprintf(fp, "%hu,", i);
        }
        fprintf(fp, "}");
        fflush(fp);
        fclose(fp);
        sendCameraPic(BULK_PIXEL_FILE);
        send_get_req = TRUE;
    } else
    {
        printf("ERROR: sendBulkPixel() could not open files\n");
    }
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

void changeWSMask()
{
    uint32_t * mask_ptr = (uint32_t *) (WS_HEADER + 2);
    *mask_ptr = (rand() << 15) | rand();
#if WS_DEBUG == 1
    printf("New mask: %x\n", *mask_ptr);
#endif
}

void maskWSMsg(char * buf, size_t buflen)
{
    int WS_MSK_END = 5;
    int ws_idx = 3;
    for (size_t i = 0; i < buflen; i++)
    {
        buf[i] = buf[i] ^ WS_HEADER[ws_idx];
        ws_idx = (ws_idx == WS_MSK_END) ? 3 : ws_idx + 1;
    }
}

CURL *initWSConn(uint16_t roomID, uint16_t userID)
{
    char request[1024] = {0};
    size_t frame_size;
    CURL *curl = NULL;
    size_t recBuf;
    CURLcode res = CURLE_AGAIN;
    printf("Init Web Socket Connection\n");
    srand(time(NULL));
    curl = curl_easy_init();
    frame_size = sprintf(request, "GET /production HTTP/1.1\r\n"
#if WS_LOCAL_HOST == 1
                                  "Host: 192.168.137.1:8080 \r\n"
#else
                                  "Host: 7nbl97eho0.execute-api.us-east-1.amazonaws.com \r\n"
#endif
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
                                sprintf(request + sizeof(WS_HEADER), "{\"action\": \"connect_to_roomID\", \"roomID\": %hu, \"user\": \"De1-%hu\"}", roomID, userID);
        res = curl_easy_send(curl, request, sizeof(WS_HEADER) + (request[WS_SIZE_BYTE] & WS_SIZE_MASK), &recBuf);
        res = CURLE_AGAIN;
        // Only want to receive header + connect message - DONT want to include potential pixels
        do {res = curl_easy_recv(curl, request, WS_ROOM_CONNECT_SIZE + WS_REC_HEADER_SIZE, &recBuf);} while (res != CURLE_OK);
#if WS_DEBUG == 1
        printf("Room Connect Command got %zu\n", recBuf);
        fwrite(request, sizeof(char), recBuf, stdout);
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
    pixel_buffer_t * rb = (pixel_buffer_t *) args[3];
    volatile uint16_t * roomID = (volatile uint16_t *) args[4];
    volatile uint16_t * userID = (volatile uint16_t *) args[5];
    volatile bool * restartConn = (volatile bool *) args[6];
    
    CURLcode rec;
    size_t rec_size, send_size;
    curl_socket_t sockfd;
    clock_t start = clock();

    rec = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
    if (curl == NULL)
    {
        printf("CURL Object For WS Connection is NULL\n");
    } else
    {
        printf("Waiting on WS Connection for pixels...\n");
        while (1)
        {
            if (*restartConn)
            {
                char buf[100];
                for (int i = 0; i < sizeof(WS_HEADER); i++)
                    buf[i] = WS_HEADER[i];
                size_t buflen = sprintf(buf + sizeof(WS_HEADER), "{\"action\":\"disconnect_roomID\",\"roomID\":%hu,\"user\":\"De1-%hu\"}",
                                        *roomID, *userID);
                buf[1] = 0x80 | buflen;
                curl_easy_send(curl, buf, buflen + sizeof(WS_HEADER), &send_size);
                sleep(1);
                curl_easy_cleanup(curl);
                *restartConn = FALSE;
                while (*restartConn == FALSE)
                    sleep(1);
                printf("INFO: New roomID: %hu and userID: %hu\n", *roomID, *userID);
                curl = initWSConn(*roomID, *userID);
                rec = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
                *restartConn = FALSE;
                printf("INFO: Done new connection!\n");
            }
            
            uint8_t recv[2];
            rec = curl_easy_recv(curl, recv, sizeof(recv), &rec_size);
            size_t payload_size = recv[1];
            if (rec == CURLE_OK)
            {
                if (payload_size == 126)
                {
                    curl_easy_recv(curl, recv, 2, &rec_size);
                    payload_size = (recv[0] << 8) | recv[1];
                }
                int sent_total = 0;
                do
                {
                    rec = curl_easy_recv(curl, WS_RECV_BUF + sent_total, payload_size - sent_total, &rec_size);
                    sent_total += rec_size;
                    if (sent_total == payload_size && WS_RECV_BUF[0] == '"' && WS_RECV_BUF[payload_size - 1] != '"')
                    {
                        curl_easy_recv(curl, recv, sizeof(recv), &rec_size);
                        size_t next_packet_len = recv[1];
                        if (next_packet_len == 126)
                        {
                            curl_easy_recv(curl, recv, 2, &rec_size);
                            payload_size += (recv[0] << 8) | recv[1];
                        }
                    }
                } while (sent_total < payload_size);
                
                // while (got_payload < payload_size)
                // {
                //     curl_easy_recv(curl, recv + got_payload, payload_size - got_payload, &rec_size);
                //     got_payload += rec_size;
                // }
                uint16_t x, y;
                uint32_t colour;
#if WS_DEBUG == 1
                if (rec_size > 3)
                {
                    printf("GOT: ");
                    fwrite(WS_RECV_BUF, sizeof(char), payload_size, stdout);
                    printf("\n");
                }
#endif
                char * START_PIXEL = "\"%hu,%hu,%u@";
                char * MID_PIXEL = "%hu,%hu,%u@";
                if (3 == sscanf(WS_RECV_BUF, START_PIXEL, &x, &y, &colour))
                {
                    // printf("(%hu, %hu) = %u\n", x, y, colour);
                    drawPixel(VGA_BASE, x, y, colour);
                    int i = 1;
                    while (WS_RECV_BUF[i] != '"')
                    {
                        if (WS_RECV_BUF[i] == '@' && WS_RECV_BUF[i + 1] != '"')
                        {
                            i++;
                            sscanf(WS_RECV_BUF + i, MID_PIXEL,  &x, &y, &colour);
                            // printf("(%hu, %hu) = %u\n", x, y, colour);
                            drawPixel(VGA_BASE, x, y, colour);
                        }
                        i++;
                    }
                }
                    
            }
            if (send_get_req)
            {
                send_get_req = FALSE;
                char toSend[100];
                for (int i = 0; i < sizeof(WS_HEADER); i++)
                    toSend[i] = WS_HEADER[i];
                size_t buflen = sprintf(toSend + 6, "{\"action\":\"postDone\",\"roomID\":%hu,\"user\":\"De1-%hu\"}", *roomID, *userID);
                toSend[1] = 0x80 | buflen;
                curl_easy_send(curl, toSend, buflen + sizeof(WS_HEADER), &send_size);
            }
            if (pixel_buffer_size(rb) > 50 && 
                ((clock() - start) * 1000 / CLOCKS_PER_SEC) > WS_WAIT_TIME
            )
            {
                WS_SEND_BUF[0] = 0x81;
                WS_SEND_BUF[1] = 0xFE; // MASK | 126 (payload length)
                for (int i = 4; i < 8; i++) // Set mask as 0
                    WS_SEND_BUF[i] = 0;
                int pixel_count = sprintf(WS_SEND_BUF + WS_EX_PAYLOAD_BASE, WS_PIXEL_SEND_F, *roomID, *userID) + WS_EX_PAYLOAD_BASE;
                while (pixel_buffer_size(rb) > 0 && pixel_count < SEND_PIXEL_BUF - 20)
                {
                    pixel_t pixel;
                    pixel_buffer_remove(rb, &pixel);
                    pixel_count += sprintf(WS_SEND_BUF + pixel_count, "%hu,%hu,%u,", pixel.x, pixel.y, pixel.colour);
                }
                WS_SEND_BUF[pixel_count - 1] = ']';
                WS_SEND_BUF[pixel_count] = '}';
                pixel_count++;
                WS_SEND_BUF[2] = (pixel_count - WS_EX_PAYLOAD_BASE) >> 8;
                WS_SEND_BUF[3] = (pixel_count - WS_EX_PAYLOAD_BASE) & 0xFF;
                
#if WS_DEBUG == 1
                fwrite(send_buffer WS_EX_PAYLOAD_BASE, sizeof(char), pixel_count - WS_EX_PAYLOAD_BASE, stdout);
                printf("\n");
#endif
                // maskWSMsg(send_buf + sizeof(WS_HEADER), buf_len);
                rec = curl_easy_send(curl, WS_SEND_BUF, pixel_count, &send_size);
                start = clock();
                printf("Sent Pixel Bytes: %d - %d\n", send_size, pixel_count);
                // int sent_total = 0;
                // do
                // {
                //     int sent = 0;
                //     rec = curl_easy_send(curl, send_buffer + sent_total, pixel_count - sent_total, &send_size);
                //     sent_total += send_size;
                //     if (rec == CURLE_AGAIN && !wait_on_socket(sockfd, 0, 60000L))
                //     {
                //         printf("ERROR: Timeout!!!\n");
                //         break;
                //     }
                //     if (rec != CURLE_OK && rec != CURLE_AGAIN)
                //     {
                //         printf("ERROR: Something went wrong with sending the pixel -  %s\n", curl_easy_strerror(rec));
                //         curl_easy_cleanup(curl);
                //         curl = initWSConn(*roomID);
                //         rec = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
                //     }
                // } while (rec == CURLE_AGAIN);
                // changeWSMask();
                // sleep_us(500000);
            }
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

/* Auxiliary function that waits on the socket. */
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
 
  if(for_recv) {
    FD_SET(sockfd, &infd);
  }
  else {
    FD_SET(sockfd, &outfd);
  }
 
  /* select() returns the number of signalled sockets or -1 */
  res = select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
  return res;
}
