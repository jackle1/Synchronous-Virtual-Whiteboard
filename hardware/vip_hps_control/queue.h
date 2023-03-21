#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct{
    uint32_t num;
    uint32_t front;
    uint32_t rear;
    uint32_t data[0];
    uint32_t mydata[2]; // custom data
} QUEUE_STRUCT;

QUEUE_STRUCT* QUEUE_New(int nQueueNum);
void QUEUE_Delete(QUEUE_STRUCT *pQueue);
bool QUEUE_IsEmpty(QUEUE_STRUCT *pQueue);
bool QUEUE_IsFull(QUEUE_STRUCT *pQueue);
bool QUEUE_Push(QUEUE_STRUCT *pQueue, uint32_t data32);
uint32_t QUEUE_Pop(QUEUE_STRUCT *pQueue);
void QUEUE_Empty(QUEUE_STRUCT *pQueue);

#endif