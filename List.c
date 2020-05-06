#include "List.h"
#include "Util.h"

#include <stdio.h>
#include <assert.h>

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

void test_print(List* list)
{
    puts("");
    List_MapForward(list, Person_Print);
    puts("");
    List_MapBackward(list, Person_Print);
    puts("");
}

bool Match(void* data)
{
    Person* person = (Person*) data;
    return person->age == 99;
}

void test(void)
{
    List list = List_Construct(Person_Destruct);
    const Person persons[] = {
        { 24, 180 },
        { 23, 170 },
        { 22, 160 },
        { 21, 150 },
        { 20, 140 },
        { 99, 140 },
        { 99, 141 },
        { 99, 142 },
        { 99, 143 },
        { 99, 144 },
    };
    FOREACH(Person, p, persons)
        List_PushFront(&list, ListNode_Construct(Person_Construct(p.age, p.height)));
    END
    test_print(&list);
    void* front = List_PopFront(&list);
    void* back = List_PopBack(&list);
    Person_Destruct(front);
    Person_Destruct(back);
    List_Delete(&list, 2);
    List_DeleteMatch(&list, Match);
    List_MapForward(&list, Person_Print);
    List_Destruct(&list);
}

int main(void)
{
    test();
}
