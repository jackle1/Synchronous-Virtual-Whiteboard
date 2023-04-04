#include "terasic_includes.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define RING_BUF_SIZE (1024)

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
} ring_buffer_t;

ring_buffer_t * ring_buffer_init(void)
{
    ring_buffer_t * ptr = (ring_buffer_t *) malloc(sizeof(ring_buffer_t));
    ptr->count = 0;
    pthread_mutex_init(&ptr->lock, NULL);
    return ptr;
}

bool ring_buffer_add(ring_buffer_t * rb, uint16_t x, uint16_t y, uint32_t colour)
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

bool ring_buffer_remove(ring_buffer_t * rb, pixel_t * pixel)
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

int ring_buffer_size(ring_buffer_t * rb)
{
    return rb->count;
}