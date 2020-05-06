#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct ListNode
{
    void* data;
    struct ListNode* next;
    struct ListNode* prev;
}
ListNode;

typedef struct
{
    ListNode* head;
    ListNode* tail;
    void (*destruct)(void* data);
    int count;
}
List;

ListNode*
ListNode_Construct(void* data)
{
    ListNode* node = calloc(1, sizeof(*node));
    node->data = data;
    return node;
}

List
List_Construct(void (*destruct)(void*))
{
    static List zero;
    List list = zero;
    list.destruct = destruct;
    return list;
}

void
List_Remove(List* list, ListNode* node)
{
    ListNode* prev = node->prev;
    ListNode* next = node->next;
    if(list->destruct)
        list->destruct(node->data);
    free(node);
    if(prev)
        prev->next = next;
    if(next)
        next->prev = prev;
    if(node == list->head)
        list->head = next;
    if(node == list->tail)
        list->tail = prev;
}

void
List_Delete(List* list, const int index)
{
    if(index < list->count)
    {
        int i = 0;
        for(ListNode* node = list->head; node; node = node->next)
        {
            if(index == i)
            {
                List_Remove(list, node);
                return;
            }
            i += 1;
        }
    }
}

void
List_DeleteMatch(List* list, bool (*match)(void*))
{
    ListNode* node = list->head;
    while(node)
    {
        ListNode* next = node->next;
        if(match(node->data))
            List_Remove(list, node);
        node = next;
    }
}

void
List_PushFront(List* list, ListNode* node)
{
    node->next = list->head;
    list->head = node;
    list->count += 1;
    if(list->count == 1)
        list->tail = list->head;
    if(list->head->next)
        list->head->next->prev = list->head;
}

void*
List_InspectFront(List* list)
{
    return list->head->data;
}

void
List_MapForward(List* list, void (*func)(void*))
{
    for(ListNode* node = list->head; node; node = node->next)
        func(node->data);
}

void*
List_PopFront(List* list)
{
    void* data = List_InspectFront(list);
    ListNode* next = list->head->next;
    free(list->head);
    list->head = next;
    list->head->prev = NULL;
    list->count -= 1;
    if(list->count == 0)
        list->tail = NULL;
    return data;
}

void*
List_InspectBack(List* list)
{
    return list->tail->data;
}

void*
List_PopBack(List* list)
{
    void* data = List_InspectBack(list);
    ListNode* prev = list->tail->prev;
    free(list->tail);
    list->tail = prev;
    list->tail->next = NULL;
    list->count -= 1;
    if(list->count == 0)
        list->head = NULL;
    return data;
}

void
List_MapBackward(List* list, void (*func)(void*))
{
    for(ListNode* node = list->tail; node; node = node->prev)
        func(node->data);
}

void
List_Destruct(List* list)
{
    ListNode* node = list->head;
    while(node)
    {
        ListNode* next = node->next;
        if(list->destruct)
            list->destruct(node->data);
        free(node);
        node = next;
    }
    static List zero;
    *list = zero;
}
