#include "List.h"

typedef List Set;

Set* Set_Construct(void (*destruct)(void*), bool (*match)(void*, void*));

bool Set_IsMember(Set*, void* data);

Set* Set_CreateUnion(Set* a, Set* b);

Set* Set_CreateIntersection(Set* a, Set* b);

Set* Set_CreateDifference(Set* a, Set* b);

bool Set_IsSubset(Set* a, Set* b);

bool Set_IsEqual(Set* a, Set* b);

void Set_Remove(Set*, void* data);

void Set_Destruct(Set**);

int Set_GetSize(Set*);
