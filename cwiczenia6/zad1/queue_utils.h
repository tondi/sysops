#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include "utils.h"

struct Queue {
    int items[500];
    int size;
    int in;
    int out;
};

int queueGet(struct Queue* q);
int queuePut(struct Queue* q, int e);
void queueInit(struct Queue* q, int size);
int queueEmpty(struct Queue* q);

#endif
