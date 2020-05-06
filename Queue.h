#include "List.h"

typedef List Queue;

Queue* Queue_Construct(void (*destruct)(void*));

void Queue_Enqueue(Queue*, void* data);

void* Queue_Dequeue(Queue*);

int Queue_Size(Queue*);

void* Queue_Peek(Queue*);

void Queue_Destruct(Queue**);
