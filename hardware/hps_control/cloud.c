#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

enum res_state_e {CLOUD_WAIT, CLOUD_DONE, CLOUD_ERROR};

#define GET_ROOM_PIXEL_URL "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID="
#define GET_ROOM_PIXEL_URL_LEN 79
#define COLOUR_PIXEL_URL "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"
#define COLOUR_PIXEL_BODY_LEN 130 
#define BULK_PIXEL_FILE "bulk_send.json"
#define WS_URL "https://7nbl97eho0.execute-api.us-east-1.amazonaws.com/production"

typedef struct 
{
    char done;
    FILE * file;
} http_res_t;

typedef struct Pixel
{
    uint16_t x;
    uint16_t y;
    uint32_t colour;
    struct Pixel * next;
} pixel_t;

typedef struct WS_Conn
{
    CURL * curl;
    curl_socket_t sockfd;
} ws_conn_t;

void getRoomPixels(volatile http_res_t * res, uint16_t roomID);
static size_t getRoomPixelsCallBack(char *ptr, size_t size, size_t nmemb, void *userdata);
void putRoomPixels(uint16_t roomID, uint16_t x, uint16_t y, uint32_t colour);
void sendBulkPixel(pixel_t * head, uint16_t roomID);
ws_conn_t initWSConn(uint16_t roomID);

static size_t resToBuf(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    char * buf = (char *) userdata;
    for (int i = 0; i < nmemb; i++)
        buf[i] = ptr[i];
    return size * nmemb;
}


static size_t getRoomPixelsCallBack(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    http_res_t * buf = (http_res_t *) userdata;
    fwrite(ptr, size, nmemb, buf->file);
    buf->done = CLOUD_DONE; 
    return size * nmemb;
}

void getRoomPixels(volatile http_res_t * res, uint16_t roomID)
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
    } else
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
        } else 
            printf("%s\n", res_buf);
        curl_easy_cleanup(curl);    
    } else
        fprintf(stderr, "CURL FAILED!!!\n");

}

void sendCameraPic(const char * path)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    FILE *fp;
    char *url = "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391";

    // Initialize curl
    curl = curl_easy_init();
    if(curl) {
        // Open the file containing the request body data
        fp = fopen(path, "r");
        if(fp) 
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
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            // Clean up
            fclose(fp);
        } else {
            fprintf(stderr, "Failed to open file '%s'\n", path);
        }

        curl_easy_cleanup(curl);
    }
}

void sendBulkPixel(pixel_t * head, uint16_t roomID)
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
    pixel_t * ptr = head;
    int i = 0;

    fp = fopen(BULK_PIXEL_FILE, "w");
    char buf[50] = {0};
    int ret = sprintf(buf, "{\"member\":\"De1\",\"roomID\":%d,\"request-for\":1,", roomID);
    if (fp)
    {
        fwrite(buf, sizeof(char), ret, fp); // Write header
        // Write RGB values first
        char rgb[] = "\"RGB\":[";
        fwrite(rgb, sizeof(char), sizeof(rgb) - 1, fp);
        while (ptr != NULL)
        {
            if (ptr->next != NULL)
                fprintf(fp, "%hu,", ptr->colour);
            else
                fprintf(fp, "%hu],", ptr->colour);
            ptr = ptr->next;
            i++;
        }
        printf("List length: %i\n", i);

        // Write x values
        ptr = head;
        char x[] = "\"x\":[";
        fwrite(x, sizeof(char), sizeof(x) - 1, fp);
        while (ptr != NULL)
        {
            if (ptr->next != NULL)
                fprintf(fp, "%hu,", ptr->x);
            else
                fprintf(fp, "%hu],", ptr->x);
            ptr = ptr->next;
        }

        // Write y values
        ptr = head;
        char y[] = "\"y\":[";
        fwrite(y, sizeof(char), sizeof(y) - 1, fp);
        while (ptr != NULL)
        {
            if (ptr->next != NULL)
                fprintf(fp, "%hu,", ptr->y);
            else
                fprintf(fp, "%hu]", ptr->y);
            pixel_t * done = ptr;
            ptr = ptr->next;
            free((void *) done);
        }
        fprintf(fp, "}");
        fflush(fp);
        fclose(fp);
        sendCameraPic(BULK_PIXEL_FILE);
    }
}

ws_conn_t initWSConn(uint16_t roomID)
{
    char request[1024];
    int frame_size;
    ws_conn_t * conn = NULL;
    size_t recBuf;

    conn = (ws_conn_t *) malloc(sizeof(ws_conn_t));
    conn->curl = curl_easy_init();
    memset(request, 0, sizeof(request));
    frame_size = sprintf(request, "GET /production HTTP/1.1\r\n"
        "Host: 7nbl97eho0.execute-api.us-east-1.amazonaws.com/production \r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n");
    if (conn->curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, WS_URL);
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);  // Don't include response headers in output
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);  // Disable response body handling
        res = curl_easy_perform(curl);
        res = curl_easy_send(curl, request, frame_size, &recBuf);
    } else
    {
        printf("Curl Init Failed For Socket???\n");
        curl_easy_cleanup(conn->curl);
        free(conn);
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
