#pragma once

#include <stdbool.h>

typedef struct Node Node;

struct Node
{
    void* data;
    struct Node* next;
    struct Node* prev;
};

typedef struct List List;

struct List
{
    struct Node* head;
    struct Node* tail;
    void (*destruct)(void* data);
    bool (*match)(void*, void*);
    int size;
};

List* List_Construct(void (*destruct)(void*), bool (*match)(void*, void*));

void List_Destruct(List**);

int List_GetSize(List*);

void List_PushFront(List*, void*);

void List_PushBack(List*, void*);

void* List_InspectFront(List*);

void List_MapForward(List*, void (*func)(void*));

void* List_Match(List*, void* data);

void List_DeleteMatch(List*, void* data);

void* List_PopFront(List*);

void* List_InspectBack(List*);

void* List_PopBack(List*);

void List_MapBackward(List*, void (*func)(void*));

List* List_Copy(List*);
