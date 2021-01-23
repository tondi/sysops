#include "queue_utils.h"

void queueInit(struct Queue *q, int size)
{
    q->size = size;
    q->in = q->out = 0;
}

int queuePut(struct Queue *q, int e)
{
    if (q->in == ((q->out - 1 + q->size) % q->size))
    {
        // Kolejka pełna
        return -1;
    }

    q->items[q->in] = e;
    q->in = (q->in + 1) % q->size;
    return 0;
}

int queueGet(struct Queue *q)
{
    int e = q->items[q->out];
    q->out = (q->out + 1) % q->size;
    return e;
}

int queueEmpty(struct Queue *q)
{
    if (q->in == q->out)
        return 1;
    return 0;
}
