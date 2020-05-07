#include "List.h"

typedef List Queue;

Queue* Queue_Construct(void (*destruct)(void*), bool (*match)(void*, void*));

void Queue_Destruct(Queue**);

int Queue_Size(Queue*);

void Queue_Enqueue(Queue*, void* data);

void* Queue_Dequeue(Queue*);

void* Queue_Peek(Queue*);
