#include "queue.h"

void QInit(struct Queue* q, int size){
    q->size = size;
    q->queueIn = q->queueOut = 0;

}

int QPut(struct Queue* q, int e){
    if(q->queueIn == ((q->queueOut -1 + q->size) % q->size)){
        return -1; //Queue full
    }
    
    q->elems[q->queueIn] = e;
    q->queueIn = (q->queueIn + 1) % q->size;
    return 0;
}

int QGet(struct Queue* q){
    int e = q->elems[q->queueOut];
    q->queueOut = (q->queueOut + 1) % q->size;
    return e;
}

int QEmpty(struct Queue* q){
    if(q->queueIn == q->queueOut)
        return 1;
    return 0;
}
