#include "List.h"

typedef List Stack;

Stack* Stack_Construct(void (*destruct)(void*));

void Stack_Destruct(Stack**);

int Stack_Size(Stack*);

void Stack_Push(Stack*, void* data);

void* Stack_Pop(Stack*);

void* Stack_Peek(Stack*);
