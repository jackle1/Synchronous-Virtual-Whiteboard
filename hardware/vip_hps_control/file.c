#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VGA_X (640)
int main(void)
{
    unsigned short colour;
    char read;
    int i = 0;
    FILE * file_fd = NULL;
    char * file = "pixel_file.txt";

    file_fd = fopen(file, "r");
    if (file_fd != NULL)
    {
        printf("Starting file read\n");
        // Skip header bytes 
        do
        {
            fscanf(file_fd, "%c", &read);
            printf("%c", read);
        } while (read != '[');
        printf("Found first array\n");
        fscanf(file_fd, "%c", &read); // get [ - start of array
        while (i < VGA_X * 2)
        {
            int ret = fscanf(file_fd, "%hu, ", &colour);
            if (ret > 0)
                printf("A: %d %hu\n", i, colour);
            else
            {
                ret = fscanf(file_fd, "%hu], [", &colour);
                printf("B: %d %hu\n", i, colour);
                read = '0';
                while (read != '[') fscanf(file_fd, "%c", &read);
            }
            i++;
        }
    }
}
