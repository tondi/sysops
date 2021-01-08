#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>

#include "lib.h"

struct Queue{
    int elems[500];
    int size;
    int queueIn;
    int queueOut;
};

void QInit(struct Queue* q, int size);
int QPut(struct Queue* q, int e);
int QGet(struct Queue* q);
int QEmpty(struct Queue* q);

#endif
