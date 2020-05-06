#include "Queue.h"

Queue* Queue_Construct(void (*destruct)(void*))
{
    return List_Construct(destruct);
}

void Queue_Enqueue(Queue* queue, void* data)
{
    List_PushFront(queue, data);
}

void* Queue_Dequeue(Queue* queue)
{
    return List_PopBack(queue);
}

int Queue_Size(Queue* queue)
{
    return List_GetSize(queue);
}

void* Queue_Peek(Queue* queue)
{
    return List_InspectBack(queue);
}

void Queue_Destruct(Queue** queue)
{
    List_Destruct(queue);
}
