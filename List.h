///////////////////////////////////////////////////////////
// fundamental clses

#ifndef LIST_H
#define LIST_H

#define TYPE_LIST	5
#define TYPE_NODE	6

#include "Object.h"

struct Node;

typedef struct List {
	int type;
	struct Node *first;
	struct Node *last;
} List;

typedef struct Node {
	int type;
	Object *value;
	struct Node *next;
	struct Node *prev;
} Node;

List *newList(void);
char *listToCString(Object *obj);

Node *newNode(Object *value);
char *nodeToCString(Object *obj);

///////////////////////////////////////////////////////////
// list implementation

int isEmpty(List *list);
void append(List *list, void *element);
Object *get(List *list, int i);
int length(List *list);
Object *pop(List *list);
Object *popn(List *list, int i);
void removeAll(List *list);
void removeObject(List *list, void *obj);
int contains(List *list, void *obj);

///////////////////////////////////////////////////////////
// higher-order list functions

typedef Object* (*MapFunction)(Object* element);
typedef void (*ForEachFunction)(Object* element);
typedef int (*FilterFunction)(Object* element);
typedef Object* (*ReduceFunction)(Object* result, Object* element);

List* map(MapFunction f, List* list);
void forEach(ForEachFunction f, List* list);
List* filter(FilterFunction predicate, List *list);
Object* reduce(ReduceFunction f, List *list, Object *initVal);
List* flatMap(MapFunction f, List* list);
List* flatMapInto(List* result, MapFunction f, List* list);

#endif
