#pragma once

#include <stdlib.h>

typedef struct ListNode
{
    void* data;
    struct ListNode* next;
}
ListNode;

typedef struct
{
    ListNode* node;
}
List;

ListNode* ListNode_Construct(void* data)
{
    ListNode* node = calloc(1, sizeof(*node));
    node->data = data;
    return node;
}

List List_Construct(void)
{
    static List zero;
    return zero;
}

void List_PushFront(List* list, ListNode* node)
{
    node->next = list->node;
    list->node = node;
}

void List_Iterate(List* list, void func(void*))
{
    for(ListNode* node = list->node; node; node = node->next)
        func(node->data);
}

void List_Destruct(List* list, void destruct(void*))
{
    ListNode* node = list->node;
    while(node)
    {
        ListNode* next = node->next;
        destruct(node->data);
        free(node);
        node = next;
    }
    static List zero;
    *list = zero;
}
