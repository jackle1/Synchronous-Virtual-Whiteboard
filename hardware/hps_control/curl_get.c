#include <stdio.h>
#include <curl/curl.h>

size_t writeFile(char *ptr, size_t size, size_t nmemb, void *userdata)
{
FILE * file = (FILE *) userdata;
return fwrite(ptr, size, nmemb, file);
}

int main(void)
{
CURL *curl;
CURLcode res;
FILE * file = NULL; 

curl = curl_easy_init();
if (curl)
{
curl_easy_setopt(curl, CURLOPT_URL, "https://hbzwo0rl65.execute-api.us-east-1.amazonaws.com/dev/cpen391?RoomID=2361");
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
res = curl_easy_perform(curl);
if (res != CURLE_OK)
{
fprintf(stderr, "curl_easy_perform() failed: %s\n",
curl_easy_strerror(res));
curl_easy_cleanup(curl);    
}
}
// Write HTTP response to file
/**
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
**/
return 0;
}

