#include "Stack.h"

Stack* Stack_Construct(void (*destruct)(void*))
{
    return List_Construct(destruct);
}

void Stack_Destruct(Stack** stack)
{
    List_Destruct(stack);
}

int Stack_Size(Stack* stack)
{
    return List_GetSize(stack);
}

void Stack_Push(Stack* stack, void* data)
{
    List_PushFront(stack, data);
}

void* Stack_Pop(Stack* stack)
{
    return List_PopFront(stack);
}

void* Stack_Peek(Stack* stack)
{
    return List_InspectFront(stack);
}
