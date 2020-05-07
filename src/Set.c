#include "Set.h"

#include <stdlib.h>

Set* Set_Construct(void (*destruct)(void*), bool (*match)(void*, void*))
{
    return List_Construct(destruct, match);
}

bool Set_IsMember(Set* set, void* data)
{
    return List_Match(set, data) != NULL;
}

bool Set_Insert(Set* set, void* data)
{
    if(!Set_IsMember(set, data))
    {
        List_PushFront(set, data);
        return true;
    }
    return false;
}

Set* Set_CreateUnion(Set* a, Set* b)
{
    List* out = List_Copy(a);
    for(Node* node = b->head; node; node = node->next)
        Set_Insert(out, node->data);
    return out;
}

Set* Set_CreateIntersection(Set* a, Set* b)
{
    List* out = Set_Construct(a->destruct, a->match);
    for(Node* node = a->head; node; node = node->next)
        if(Set_IsMember(b, node->data))
            Set_Insert(out, node->data);
    return out;
}

Set* Set_CreateDifference(Set* a, Set* b)
{
    List* out = Set_Construct(a->destruct, a->match);
    for(Node* node = a->head; node; node = node->next)
        if(!Set_IsMember(b, node->data))
            Set_Insert(out, node->data);
    return out;
}

bool Set_IsSubset(Set* a, Set* b)
{
    if(Set_GetSize(a) > Set_GetSize(b))
        return false;
    for(Node* node = a->head; node; node = node->next)
        if(!Set_IsMember(b, node->data))
            return false;
    return true;
}

bool Set_IsEqual(Set* a, Set* b)
{
    return Set_GetSize(a) == Set_GetSize(b) && Set_IsSubset(a, b);
}

void Set_Remove(Set* set, void* data)
{
    List_DeleteMatch(set, data);
}

void Set_Destruct(Set** set)
{
    List_Destruct(set);
}

int Set_GetSize(Set* set)
{
    return List_GetSize(set);
}
