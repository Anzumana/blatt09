/*
 * List.c
 *
 *  Created on: Jul 28, 2013
 *      Author: michaelrohs
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include "ObjectFunctions.h"
#include "List.h"



///////////////////////////////////////////////////////////
// fundamental classes



List *newList(void) {
#ifdef DEBUG
	printf("newList\n");
#endif
	List *result = calloc(1, sizeof(List));
	result->type = TYPE_LIST;
	return result;
}



char *listToCString(Object *obj) {
#ifdef DEBUG
	printf("listToCString\n");
#endif
	List *list = (List*) obj;
	int n = length(list);
	char **strings = malloc(n * sizeof(char*));
	int i, totalLength = 3; // '(', ')', '\0'
	for (i = 0; i < n; i++) {
		strings[i] = toCString(get(list, i));
		totalLength += strlen(strings[i]) + 2; // string + ",_"
	}
	char *buffer = calloc(totalLength, 1);
	buffer[0] = '(';
	int j = 1;
	for (i = 0; i < n; i++) {
		strcpy(buffer + j, strings[i]);
		j += strlen(strings[i]);
		if (i < n-1) {
			strcpy(buffer + j, ", ");
			j += 2;
		}
	}
	free(strings);
	buffer[j++] = ')';
	buffer[j++] = '\0';
	return buffer;
}



///////////////////////////////////////////////////////////
// Node



Node *newNode(Object *value) 
{
	// printf("newNode\n");
	Node *result = calloc(1, sizeof(Node));
	result->type = TYPE_NODE;
	result->value = value;
	return result;
}



///////////////////////////////////////////////////////////
// list implementation



int isEmpty(List *list) {
	return list->first == NULL;
}



void append(List *list, void *element) {
	if (element == NULL) return;
	Node *node = newNode(element);
	if (list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		Node *last = list->last;
		last->next = node;
		list->last = node;
		node->prev = last;
	}
}



Object *get(List *list, int i) {
	if (i >= 0) {
		Node *node = list->first;
		while (i-- > 0) node = node->next;
		return node->value;
	} else {
		Node *node = list->last;
		while (++i < 0) node = node->prev;
		return node->value;
	}
}



int length(List *list) {
	int i = 0;
	for (Node *node = list->first; node != NULL; node = node->next) {
		i++;
	}
	return i;
}



Object *pop(List *list) {
	Node *first = list->first;
	Node *last = list->last;
	Object *value = last->value;
	if (first == last) {
		list->first = NULL;
		list->last = NULL;
	} else {
		list->last = last->prev;
		list->last->next = NULL;
	}
	return value;
}



Object *popn(List *list, int i) 
{
	Node *first = list->first;
	Node *last = list->last;
	Node *node = NULL;
	if (i >= 0) {
		node = list->first;
		while (i-- > 0) node = node->next;
	} else {
		node = list->last;
		while (++i < 0) node = node->prev;
	}
	Object *value = node->value;
	if (first == last) {
		list->first = NULL;
		list->last = NULL;
	} else {
		if (node == first) {
			list->first = first->next;
			list->first->prev = NULL;
		} else if (node == last) {
			list->last = last->prev;
			list->last->next = NULL;
		} else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	return value;
}



void removeAll(List *list)
{
	Node *p = list->first;
	while (p != NULL) {
		Node *q = p->next;
		p = q;
	}
	list->first = NULL;
	list->last = NULL;
}



void removeObject(List *list, void *toRemove)
{
	int i = 0;
	Node *p = list->first;
	while (p != NULL) {
		if (p->value == toRemove) {
			popn(list, i);
		}
		i++;
		p = p->next;
	}
}



int contains(List *list, void *obj)
{
	Node *p;
	for (p = list->first; p != NULL; p = p->next) {
		if (p->value == obj) {
			return 1;
		}
	}
	return 0;
}



///////////////////////////////////////////////////////////
// higher-order list functions

List* map(MapFunction f, List* list)
{
	List* result = newList();
	Node *node;
	for (node = list->first; node != NULL; node = node->next) {
		append(result, f(node->value));
	}
	return result;
}



void forEach(ForEachFunction f, List* list)
{
	Node *node;
	for (node = list->first; node != NULL; node = node->next) {
		f(node->value);
	}
}



List* filter(FilterFunction predicate, List *list)
{
	List* result = newList();
	Node *node;
	for (node = list->first; node != NULL; node = node->next) {
		if (predicate(node->value)) {
			append(result, node->value);
		}
	}
	return result;
}



Object* reduce(ReduceFunction f, List *list, Object *initVal)
{
	if (isEmpty(list)) return initVal;
	Node *node = list->first;
	Object *result = initVal ? f(initVal, node->value) : node->value;
	node = node->next;
	for (; node != NULL; node = node->next) {
		result = f(result, node->value);
	}
	return result;
}


/**
 * *** todo *** Erläuterung map, flatMap. Unterschied.
 */
List* flatMap(MapFunction f, List* list)
{
	List* result = newList();
	Node *node;
	for (node = list->first; node != NULL; node = node->next) {
		Object *o = f(node->value);
		if (o != NULL) {
			if (o->type == TYPE_LIST) {
				flatMapInto(result, f, (List*)o);
			} else {
				append(result, o);			
			}
		}
	}
	return result;
}



List* flatMapInto(List* result, MapFunction f, List* list)
{
	Node *node;
	for (node = list->first; node != NULL; node = node->next) {
		Object *o = f(node->value);
		if (o != NULL) {
			if (o->type == TYPE_LIST) {
				flatMapInto(result, f, (List*)o);
			} else {
				append(result, o);			
			}
		}
	}
	return result;
}


