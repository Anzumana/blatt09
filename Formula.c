#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ObjectFunctions.h"
#include "Formula.h"

#define TYPE_COORD 7
#define TYPE_RANGE 8
#define TYPE_CALL  9
#define TYPE_CELL  10



Coord *newCoord(int row, int column) 
{
	Coord *result = calloc(1, sizeof(struct Coord));
	result->type = TYPE_COORD;
	result->row = row;
	result->column = column;
	return result;
}



char *coordToCString(Object *obj) 
{
	Coord *c = (Coord*)obj;
	char *buffer = malloc(32); // should be enough
	sprintf(buffer, "%c%d", 'A' + c->column, 1 + c->row);
	return buffer;
}



Range *newRange(Coord *c1, Coord *c2)
{
	Range *result = calloc(1, sizeof(struct Range));
	result->type = TYPE_RANGE;
	result->c1 = c1;
	result->c2 = c2;
	return result;
}



char *rangeToCString(Object *obj) 
{
	Range *r = (Range*)obj;
	char *buffer = malloc(64); // should be enough
	sprintf(buffer, "%s:%s", toCString(r->c1), toCString(r->c2));
	return buffer;
}



Call *newCall(String *function, List *arguments)
{
	Call *result = calloc(1, sizeof(struct Call));
	result->type = TYPE_CALL;
	result->function = function;
	result->arguments = arguments;
	return result;
}



char *callToCString(Object *obj)
{
	Call *c = (Call*) obj;
	char *sf = toCString(c->function);
	char *sa = toCString(c->arguments);
	char *buffer = malloc(strlen(sf) + strlen(sa) + 1);
	strcpy(buffer, sf);
	strcat(buffer, sa);
	return buffer;	
}



Cell *newCell(void)
{
	// printf("newCell\n");
	Cell *result = calloc(1, sizeof(struct Cell));
	result->type = TYPE_CELL;
	result->listeners = newList();
	result->listeningTo = newList();
	return result;
}



char *cellToCString(Object *obj)
{
	Cell *c = (Cell*) obj;
	char *buffer = malloc(4);
	sprintf(buffer, "%c%d", 'A' + c->column, 1 + c->row);
	return buffer;
}


