#include "terasic_includes.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define RING_BUF_SIZE (2056)

typedef struct Pixel
{
    uint16_t x;
    uint16_t y;
    uint32_t colour;
} pixel_t;

typedef struct
{
    pixel_t buf[RING_BUF_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
} pixel_buffer_t;

pixel_buffer_t * pixel_buffer_init(void)
{
    pixel_buffer_t * ptr = (pixel_buffer_t *) malloc(sizeof(pixel_buffer_t));
    ptr->count = 0;
    ptr->head = 0;
    ptr->tail = 0;
    // memset(ptr->buf, 0, sizeof(pixel_t) * RING_BUF_SIZE);
    pthread_mutex_init(&ptr->lock, NULL);
    return ptr;
}

bool pixel_buffer_add(pixel_buffer_t * rb, uint16_t x, uint16_t y, uint32_t colour)
{
    pthread_mutex_lock(&rb->lock);
    if (rb->count == RING_BUF_SIZE)
    {
        pthread_mutex_unlock(&rb->lock);
        return FALSE;
    }
    rb->buf[rb->tail].x = x;
    rb->buf[rb->tail].y = y;
    rb->buf[rb->tail].colour = colour;
    rb->tail = (rb->tail + 1) % RING_BUF_SIZE;
    rb->count++;
    pthread_mutex_unlock(&rb->lock);
    return TRUE;
}

bool pixel_buffer_remove(pixel_buffer_t * rb, pixel_t * pixel)
{
    pthread_mutex_lock(&rb->lock);
    if (rb->count > 0)
    {
        *pixel = rb->buf[rb->head]; 
        rb->head = (rb->head + 1) % RING_BUF_SIZE;
        rb->count--;
        pthread_mutex_unlock(&rb->lock);
        return TRUE;
    } else
    {
        pthread_mutex_unlock(&rb->lock);
        return FALSE;
    }
}

int pixel_buffer_size(pixel_buffer_t * rb)
{
    return rb->count;
}