#pragma once

#include <stdbool.h>

typedef struct List List;

List* List_Construct(void (*destruct)(void*));

void List_Destruct(List**);

int List_GetSize(List*);

void List_Delete(List*, const int index);

void List_DeleteMatch(List*, bool (*match)(void*));

void List_PushFront(List*, void*);

void* List_InspectFront(List*);

void List_MapForward(List*, void (*func)(void*));

void* List_PopFront(List*);

void* List_InspectBack(List*);

void* List_PopBack(List*);

void List_MapBackward(List*, void (*func)(void*));
