#include <stdio.h> 
#include <sys/time.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

void Node_Free(Node* node)
{
    free(node);
}

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
    Node_Free(node);
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

Node* Dict_Get(Dict* self, size_t hash, void* data)
{
    List* list = self->list[hash];
    Node* found = List_For(list, self->iterate, data);
    return found;
}

bool Dict_Insert(Dict* self, size_t hash, void* data)
{
    Node* found = Dict_Get(self, hash, data);
    if(found)
        return false;
    List* list = self->list[hash];
    List_Push(list, data, TAIL);
    return true;
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

typedef struct
{
    char* name;
    double age;
    double weight;
    double height;
}
Person;

Person* Person_Init(char* name, double age, double weight, double height)
{
    Person* self = malloc(sizeof(*self));
    self->name = name;
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
    Person* person = Person_Init("Gustav Louw", 123, 123, 123);
    size_t hash = Dict_Hash(dict, person->name, strlen(person->name));
    bool inserted = Dict_Insert(dict, hash, person);
    if(!inserted)
        Person_Free(person);
    Node* found = Dict_Get(dict, hash, person->name);
    if(found)
        Person_Print(found->data, "FOUND PERSON");
    Dict_Free(&dict);
}

int main(void)
{
    Test_List();
    Test_Dict();
    return 0;
}
