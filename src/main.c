#include "List.h"
#include "Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ------------------------------------------------------------- *
 * GENERIC PERSON OBJECT USED FOR VALIDATING ALL DATA STRUCTURES *
 * ------------------------------------------------------------- */

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

bool MatchAge(void* data)
{
    Person* person = (Person*) data;
    return person->age == 99;
}

/* ------------------------------------------------------------- *
 *                          LIST                                 *
 * ------------------------------------------------------------- */

void test_list(void)
{
    // ENSURE NOTHING BREAKS WHEN LIST IS NULL.
    List* bork = List_Construct(NULL);
    List_Destruct(NULL);
    List_GetSize(NULL);
    List_Delete(NULL, 0);
    List_DeleteMatch(NULL, NULL);
    List_PushFront(NULL, NULL);
    List_InspectFront(NULL);
    List_MapForward(NULL, NULL);
    List_PopFront(NULL);
    List_InspectBack(NULL);
    List_PopBack(NULL);
    List_MapBackward(NULL, NULL);
    List_Destruct(&bork); // CLEANUP.
    // GENERAL TESTS.
    List* list = List_Construct(Person_Destruct);
    assert(List_GetSize(list) == 0);
    const Person persons[] = {
        {  24, 180 },
        {  23, 170 },
        {  22, 160 },
        {  21, 150 },
        {  20, 140 },
        {  99, 140 },
        {  99, 141 },
        {  99, 142 },
        {  99, 143 },
        { 100, 144 },
    };
    const int len = LEN(persons);
    FOREACH(Person, p, persons)
        List_PushFront(list, Person_Construct(p.age, p.height));
    END
    Person* front = List_PopFront(list);
    Person* back = List_PopBack(list);
    assert(front->age == 100);
    assert(back->age == 24);
    Person_Destruct(front);
    Person_Destruct(back);
    assert(List_GetSize(list) == len - 2);
    List_Delete(list, 2);
    assert(List_GetSize(list) == len - 3);
    List_DeleteMatch(list, MatchAge);
    assert(List_GetSize(list) == len - 6);
    List_Destruct(&list);
    assert(list == NULL);
}

int main(void)
{
    test_list();
}