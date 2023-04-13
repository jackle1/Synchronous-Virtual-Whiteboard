#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define COLOUR_PIXEL_URL "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391"
#define COLOUR_PIXEL_BODY_LEN 130 


size_t writeFile(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE * file = (FILE *) userdata;
    return fwrite(ptr, size, nmemb, file);
}

size_t writeBuf(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    char * buf = (char *) userdata;
    for (size_t i = 0; i < nmemb; i++)
        buf[i] = ptr[i];
}

void putRoomPixels(unsigned short roomID, unsigned short x, unsigned short y, unsigned short colour)
{
    CURL *curl;
    CURLcode res;
    char * buf[COLOUR_PIXEL_BODY_LEN];
    sprintf(buf, "{\"member\": \"De1-Soc\",\"roomID\": %d,\"R-values\" : %d,\"G-values\" : %d,\"B-values\" : %d,\"request-for\": 1,\"x\": %d,\"y\": %d}",
            roomID, colour >> 11, (colour >> 5) & 0x3F, colour & 0x1F, x, y);            
    printf(buf);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, COLOUR_PIXEL_URL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);    
    } else
        fprintf(stderr, "CURL FAILED!!!\n");

}

int main(void)
{
    CURL *curl;
    CURLcode res;
    FILE * file = NULL; 

    curl = curl_easy_init();
    /**
    if (curl)
    {
        volatile char * buf = (char *) malloc(1000 * sizeof(char));
        memset((void *) buf, 0, sizeof(buf));
        curl_easy_setopt(curl, CURLOPT_URL, "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=4187");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeBuf);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
            curl_easy_cleanup(curl);    
        }
        while (buf[0] == 0);
        printf("%d %s\n", sizeof(buf),  buf);
    }
    putRoomPixels(4187, 0, 0, 0);
    **/

    // Write HTTP response to file
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.137.1:8080/file");
        file = fopen("get.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    fclose(file);
    return 0;
}
