#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>

#include "lib.h"

struct Queue{
    int items[500];
    int size;
    int queueIn;
    int queueOut;
};

void QueueInit(struct Queue* q, int size);
int QueuePut(struct Queue* q, int e);
int QueueGet(struct Queue* q);
int QueueEmpty(struct Queue* q);

#endif
