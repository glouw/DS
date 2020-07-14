#include "List.h"
#include "Dict.h"

#include <stdio.h> 
#include <assert.h>

typedef struct
{
    char* const name; // USED AS A KEY.
    double age;
    double weight;
    double height;
}
Person;

Person* Person_Init(char* name, double age, double weight, double height)
{
    Person* self = malloc(sizeof(*self));
    *(char**) &self->name = name;
    self->age = age;
    self->weight = weight;
    self->height = height;
    return self;
}

void Person_Free(void* p)
{
    Person* self = p;
    free(self);
}

Action Person_Print(void* data, void* args)
{
    Person* self = data;
    char* prefix = args;
    if(prefix)
        printf("%s: ", prefix);
    printf("%s : %.2f %.2f %.2f\n", self->name, self->age, self->weight, self->height);
    return CONTINUE;
}

bool Person_Match(Person* a, Person* b)
{
    return a->age == b->age
        && a->weight == b->weight
        && a->height == b->height
        && strcmp(a->name, b->name) == 0;
}

Action Person_MatchName(void* data, void* args)
{
    Person* person = data;
    return strcmp(person->name, args) == 0 ? STOP : CONTINUE;
}

Action Person_Find(void* data, void* args)
{
    return Person_Match(data, args) ? STOP : CONTINUE;
}

Action Person_Delete(void* data, void* args)
{
    Person* a = data;
    Person* b = args;
    return Person_Match(a, b) ? DELETE : CONTINUE;
}

void Test_List(void)
{
    List* list = List_Init(Person_Free);
    Person persons[] = {
        { "AA", 1, 1, 1 },
        { "AB", 7, 7, 7 },
        { "AC", 2, 2, 2 },
        { "AD", 2, 2, 2 },
        { "AE", 2, 2, 2 },
        { "AO", 2, 2, 2 },
        { "AP", 8, 8, 8 },
        { "AQ", 9, 9, 9 },
    };
    for(size_t i = 0; i < sizeof(persons) / sizeof(*persons); i++)
    {
        Person p = persons[i];
        List_Push(list, Person_Init("GUSTAV LOUW", p.age, p.weight, p.height), TAIL);
    }
    List_For(list, Person_Print, "[TEST] PRINTING");
    Person find = { "GUSTAV LOUW", 8, 8, 8};
    Node* found = List_For(list, Person_Find, &find);
    if(found)
    {
        Person_Print(found->data, "[TEST] FOUND AND DELETING");
        List_Pop(list, found, true);
    }
    Person delete = { "GUSTAV LOUW", 2, 2, 2 };
    List_For(list, Person_Delete, &delete);
    List_For(list, Person_Print, "[TEST] FINAL");
    List_Clear(list);
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->size == 0);
    List_Free(list);
}

void Test_Dict(void)
{
    Dict* dict = Dict_Init(1699, Person_MatchName, Person_Free);
    /* SET */
    {
        Person* person = Person_Init("Gustav Louw", 123, 456, 789);
        if(!Dict_Insert(dict, person->name, strlen(person->name), person))
            Person_Free(person);
    }
    /* GET */
    {
        char* name = "Gustav Louw";
        Node* found = Dict_Get(dict, name, strlen(name));
        if(found)
            Person_Print(found->data, "FOUND PERSON");
    }
    Dict_Free(&dict);
}

int main(void)
{
    Test_List();
    Test_Dict();
    return 0;
}
