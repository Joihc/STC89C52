#ifndef QUEUE_H_
#define QUEUE_H_

#include "config.h"

#ifndef QUEUE_C_
#endif

#define QueueSize      60
#define QueueFull      0
#define QueueEmpty     1
#define QueueOperateOk 2
struct FifoQueue
{
    uint16 front;
    uint16 rear;
    uint16 count;
    uint8  dat[QueueSize];
};
//Queue Initalize
extern void QueueInit(struct FifoQueue *Queue);
// Queue In
extern uint8 QueueIn(struct FifoQueue *Queue,uint8 sdat);
// Queue Out
extern uint8 QueueOut(struct FifoQueue *Queue,uint8 *sdat);
extern void QueueCheckOut(struct FifoQueue *Queue,uint8 sdat);
#endif
