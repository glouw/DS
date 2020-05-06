#include "List.h"

#include <stdlib.h>

typedef struct Node Node;

struct Node
{
    void* data;
    struct Node* next;
    struct Node* prev;
};

struct List
{
    struct Node* head;
    struct Node* tail;
    void (*destruct)(void* data);
    int size;
};

static Node* Node_Construct(void* data)
{
    Node* node = calloc(1, sizeof(*node));
    node->data = data;
    return node;
}

List* List_Construct(void (*destruct)(void*))
{
    List* list = calloc(1, sizeof(*list));
    list->destruct = destruct;
    return list;
}

void List_Destruct(List** list)
{
    if(list && *list)
    {
        Node* node = (*list)->head;
        while(node)
        {
            Node* next = node->next;
            if((*list)->destruct)
                (*list)->destruct(node->data);
            free(node);
            node = next;
        }
        free(*list);
        *list = NULL;
    }
}

int List_GetSize(List* list)
{
    return list ? list->size : -1;
}

static void Remove(List* list, Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;
    if(list->destruct)
        list->destruct(node->data);
    free(node);
    list->size -= 1;
    if(prev)
        prev->next = next;
    if(next)
        next->prev = prev;
    if(node == list->head)
        list->head = next;
    if(node == list->tail)
        list->tail = prev;
}

void List_Delete(List* list, const int index)
{
    if(list && index < list->size)
    {
        int i = 0;
        for(Node* node = list->head; node; node = node->next)
        {
            if(index == i)
            {
                Remove(list, node);
                return;
            }
            i += 1;
        }
    }
}

void List_DeleteMatch(List* list, bool (*match)(void*))
{
    if(list)
    {
        Node* node = list->head;
        while(node)
        {
            Node* next = node->next;
            if(match(node->data))
                Remove(list, node);
            node = next;
        }
    }
}

void List_PushFront(List* list, void* data)
{
    if(list)
    {
        Node* node = Node_Construct(data);
        node->next = list->head;
        list->head = node;
        list->size += 1;
        if(list->size == 1)
            list->tail = list->head;
        if(list->head->next)
            list->head->next->prev = list->head;
    }
}

void* List_InspectFront(List* list)
{
    return list ? list->head->data : NULL;
}

void List_MapForward(List* list, void (*func)(void*))
{
    if(list)
        for(Node* node = list->head; node; node = node->next)
            func(node->data);
}

void* List_PopFront(List* list)
{
    if(list)
    {
        void* data = List_InspectFront(list);
        Node* next = list->head->next;
        free(list->head);
        list->head = next;
        list->head->prev = NULL;
        list->size -= 1;
        if(list->size == 0)
            list->tail = NULL;
        return data;
    }
    else
        return NULL;
}

void* List_InspectBack(List* list)
{
    return list ? list->tail->data : NULL;
}

void* List_PopBack(List* list)
{
    if(list)
    {
        void* data = List_InspectBack(list);
        Node* prev = list->tail->prev;
        free(list->tail);
        list->tail = prev;
        list->tail->next = NULL;
        list->size -= 1;
        if(list->size == 0)
            list->head = NULL;
        return data;
    }
    else
        return NULL;
}

void List_MapBackward(List* list, void (*func)(void*))
{
    if(list)
        for(Node* node = list->tail; node; node = node->prev)
            func(node->data);
}
