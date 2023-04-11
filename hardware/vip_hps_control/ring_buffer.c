#include "terasic_includes.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

typedef struct Pixel
{
    uint16_t x;
    uint16_t y;
    uint32_t colour;
    Pixel * next;
} pixel_t;

typedef struct
{
    pixel_t * head;
    pixel_t * tail;
    int count;
    pthread_mutex_t lock;
} pixel_buffer_t;

pixel_buffer_t * pixel_buffer_init(void)
{
    ptr->count = 0;
    ptr->head = NULL;
    ptr->tail = NULL;
    pthread_mutex_init(&ptr->lock, NULL);
    return ptr;
}

bool pixel_buffer_add(pixel_buffer_t * rb, uint16_t x, uint16_t y, uint32_t colour)
{
    pixel_t * pixel = (pixel_t *) malloc(sizeof(pixel_t));
    pixel->x = x;
    pixel->y = y;
    pixel->colour = colour;
    pixel->next = NULL;
    pthread_mutex_lock(&rb->lock);
    if (rb->head == NULL)
    {
        rb->head = pixel;
        rb->tail = pixel;
    } else
    {
        rb->tail->next= pixel;
        rb->tail = pixel;
    }
    rb->count++;
    pthread_mutex_unlock(&rb->lock);
    return TRUE;
}

bool pixel_buffer_remove(pixel_buffer_t * rb, pixel_t * pixel)
{
    if (rb->head != NULL)
    {
        pthread_mutex_lock(&rb->lock);
        pixel_t * root = rb->head;
        pixel_t * temp = rb->head->next;
        *pixel = *root;
        rb->head = temp;
        rb->count--;
        pthread_mutex_unlock(&rb->lock);
        free(root);
        return TRUE;
    }
    return FALSE;
}

int pixel_buffer_size(pixel_buffer_t * rb)
{
    return rb->count;
}