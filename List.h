#pragma once

#include <stdbool.h> 
#include <stdlib.h>
#include <string.h>

typedef enum
{
    CONTINUE, STOP, DELETE
}
Action;

typedef enum
{
    HEAD, TAIL
}
End;
    
typedef Action (*Iterate)(void* data, void* args);

typedef void (*Destruct)(void* data);

typedef struct Node
{
    void* data;
    struct Node* a;
    struct Node* b;
}
Node;

Node* Node_Init(void* data)
{
    Node* node = malloc(sizeof(*node));
    node->data = data;
    node->b = NULL;
    node->a = NULL;
    return node;
}

typedef struct
{
    Node* head;
    Node* tail;
    Destruct destruct;
    size_t size;
}
List;

List* List_Init(Destruct destruct)
{
    List* self = malloc(sizeof(*self));
    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
    self->destruct = destruct;
    return self;
}

void List_Push(List* self, void* data, End end)
{
    Node* node = Node_Init(data);
    if(self->size == 0)
    {
        self->head = node;
        self->tail = node;
    }
    else
    {
        if(end == HEAD)
        {
            node->b = self->head;
            node->b->a = node;
            self->head = node;
        }
        if(end == TAIL)
        {
            node->a = self->tail;
            node->a->b = node;
            self->tail = node;
        }
    }
    self->size += 1;
}

void* List_Pop(List* self, Node* node, bool destroy)
{
    Node* a = node->a;
    Node* b = node->b;
    void* data = node->data;
    if(node == self->head) self->head = b;
    if(node == self->tail) self->tail = a;
    if(a) a->b = b;
    if(b) b->a = a;
    if(destroy)
    {
        self->destruct(data);
        data = NULL;
    }
    free(node);
    self->size -= 1;
    return data;
}

Node* List_For(List* self, Iterate iterate, void* args)
{
    Node* node = self->head;
    while(node)
    {
        Node* b = node->b;
        switch(iterate(node->data, args))
        {
            case DELETE:
                List_Pop(self, node, true);
            case CONTINUE:
                break;
            case STOP:
                return node;
        }
        node = b;
    }
    return NULL;
}

void List_Clear(List* self)
{
    while(self->size > 0)
        List_Pop(self, self->head, true);
}

void List_Free(List* self)
{
    List_Clear(self);
    free(self);
}
