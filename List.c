#include "List.h"
#include "Util.h"

#include <stdio.h>

typedef struct
{
    int age;
    int height;
}
Person;

Person* Person_Construct(const int age, const int height)
{
    Person* person = calloc(1, sizeof(*person));
    person->age = age;
    person->height = height;
    return person;
}

void Person_Print(void* data)
{
    Person* person = (Person*) data;
    printf("%d %d\n", person->age, person->height);
}

void Person_Destruct(void* data)
{
    Person* person = (Person*) data;
    free(person);
}

int main(void)
{
    List list = List_Construct();
    const Person persons[] = {
        { 24, 180 },
        { 23, 170 },
        { 22, 160 },
        { 21, 150 },
        { 20, 140 },
    };
    FOREACH(Person, p, persons)
        List_PushFront(&list, ListNode_Construct(Person_Construct(p.age, p.height)));
    END
    List_Iterate(&list, Person_Print);
    List_Destruct(&list, Person_Destruct);
}
