#define QUEUE_C_
#include "queue.h"

void QueueInit(struct FifoQueue *Queue)
{
    Queue->front = Queue->rear;
    Queue->count = 0;
}
// Queue In
uint8 QueueIn(struct FifoQueue *Queue,uint8 sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == QueueSize))
    {                    // full
        return QueueFull;
    }else
    {                    // in
        Queue->dat[Queue->rear] = sdat;
        Queue->rear = (Queue->rear + 1) % QueueSize;
        Queue->count = Queue->count + 1;
        return QueueOperateOk;
    }
}
// Queue Out
uint8 QueueOut(struct FifoQueue *Queue,uint8 *sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == 0))
    {                    // empty
        return QueueEmpty;
    }else
    {                    // out
        *sdat = Queue->dat[Queue->front];
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
        return QueueOperateOk;
    }
}
void QueueCheckOut(struct FifoQueue *Queue,uint8 sdat)
{
	uint8 temp = QueueOperateOk;
	if((Queue->front == Queue->rear) && (Queue->count == 0))
	{                    // empty
	       return;
	}
	while(Queue->dat[Queue->front] != sdat && temp!=QueueEmpty)
	{
		temp = QueueOut(Queue,&temp);
	}
}