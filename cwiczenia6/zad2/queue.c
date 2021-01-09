#include "queue.h"

void QueueInit(struct Queue* q, int size){
    q->size = size;
    q->queueIn = q->queueOut = 0;

}

int QueuePut(struct Queue* q, int e){
    if(q->queueIn == ((q->queueOut -1 + q->size) % q->size)){
        return -1; //Queue full
    }
    
    q->items[q->queueIn] = e;
    q->queueIn = (q->queueIn + 1) % q->size;
    return 0;
}

int QueueGet(struct Queue* q){
    int e = q->items[q->queueOut];
    q->queueOut = (q->queueOut + 1) % q->size;
    return e;
}

int QueueEmpty(struct Queue* q){
    if(q->queueIn == q->queueOut)
        return 1;
    return 0;
}
