#pragma once

#include "List.h"

typedef struct
{
    List** list;
    Iterate iterate;
    size_t buckets;
}
Dict;

Dict* Dict_Init(size_t buckets, Iterate iterate, Destruct destruct)
{
    Dict* self = malloc(sizeof(*self));
    self->list = malloc(sizeof(*self->list) * buckets);
    for(size_t i = 0; i < buckets; i++)
        self->list[i] = List_Init(destruct);
    self->iterate = iterate;
    self->buckets = buckets;
    return self;
}

unsigned Dict_Hash(Dict* self, void* key, size_t len)
{
    char* byte = key;
    unsigned value = 0x0;
    for(size_t i = 0; i < len; i++)
    {
        value = (value << 4) + byte[i];
        unsigned temp = value & 0xF0000000;
        if(temp)
        {
            value = value ^ (temp >> 24);
            value = value ^ temp;
        }
    }
    return value % self->buckets;
}

List* Dict_Index(Dict* self, void* key, size_t size)
{
    size_t hash = Dict_Hash(self, key, size);
    return self->list[hash];
}

Node* Dict_Get(Dict* self, void* key, size_t size)
{
    List* list = Dict_Index(self, key, size);
    return List_For(list, self->iterate, key);
}

bool Dict_Insert(Dict* self, void* key, size_t size, void* data)
{
    Node* found = Dict_Get(self, key, size);
    if(found)
        return false;
    else
    {
        List* list = Dict_Index(self, key, size);
        List_Push(list, data, TAIL);
        return true;
    }
}

void Dict_Free(Dict** dict)
{
    Dict* self = *dict;
    for(size_t i = 0; i < self->buckets; i++)
        List_Free(self->list[i]);
    free(self->list);
    free(self);
    *dict = NULL;
}
