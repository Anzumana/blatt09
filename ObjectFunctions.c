#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Object.h"
#include "List.h"
#include "Formula.h"
#include "ObjectFunctions.h"

char *toCString(void *o)
{
	Object *obj = (Object*) o;
	switch (obj->type) {
		case TYPE_DOUBLE:
			return doubleToCString(obj);
		case TYPE_STRING:
			return stringToCString(obj);
		case TYPE_LIST:
			return listToCString(obj);
		case TYPE_COORD:
			return coordToCString(obj);
		case TYPE_RANGE:
			return rangeToCString(obj);
		case TYPE_CALL:
			return callToCString(obj);
		case TYPE_CELL:
			return cellToCString(obj);
		default:
			return "toCString: unknown type";
	}
}

char *typeName(void *o)
{
	Object *obj = (Object*) o;
	switch (obj->type) {
		case TYPE_DOUBLE:
			return "Double";
		case TYPE_STRING:
			return "String";
		case TYPE_LIST:
			return "List";
		case TYPE_NODE:
			return "Node";
		case TYPE_COORD:
			return "Coord";
		case TYPE_RANGE:
			return "Range";
		case TYPE_CALL:
			return "Call";
		case TYPE_CELL:
			return "Cell";
		default:
			return "typeName: unknown type";
	}
}

char *toCStringTyped(void *o)
{
	Object *obj = (Object*) o;
	char *s;
	switch (obj->type) {
		case TYPE_DOUBLE:
			s = doubleToCString(obj);
			break;
		case TYPE_STRING:
			s = stringToCString(obj);
			break;
		case TYPE_LIST:
			s = listToCString(obj);
			break;
		case TYPE_COORD:
			s = coordToCString(obj);
			break;
		case TYPE_RANGE:
			s = rangeToCString(obj);
			break;
		case TYPE_CALL:
			s = callToCString(obj);
			break;
		case TYPE_CELL:
			s = cellToCString(obj);
			break;
		default:
			s = "toCStringTyped: unknown type";
			break;
	}
	char *t = typeName(obj);
	char *result = malloc(strlen(t) + strlen(s) + 3);
	sprintf(result, "%s(%s)", t, s);
	return result;
}


