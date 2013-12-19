/*
 * List.c
 *
 *  Created on: Jul 28, 2013
 *      Author: michaelrohs
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Object.h"



///////////////////////////////////////////////////////////
// fundamental classes



Double *newDouble(double d) {
#ifdef DEBUG 
	printf("newDouble(%g)\n", d);
#endif
	Double *result = calloc(1, sizeof(struct Double));
	result->type = TYPE_DOUBLE;
	result->value = d;
	return result;
}



void doubleDelete(struct Object *obj)
{
#ifdef DEBUG 
	printf("doubleDelete(%g)\n", ((Double*)obj)->value);
#endif
	free(obj);
}



char *doubleToCString(Object *d) {
	// should be enough for the longest double
	//  e.g. -3.14159e+307, -M_PI * 1e307
	// DBL_MIN in double.h
	char *buffer = malloc(32);
	sprintf(buffer, "%g", ((Double*)d)->value);
	return buffer;
}



String *newString(char *s) {
	int n = (int)strlen(s);
	String *result = calloc(1, sizeof(struct String));
	result->type = TYPE_STRING;
	result->value = strdup(s);
	result->value[n] = '\0';
//	printf("newString: %s (strlen %d)\n", result->value, (int)strlen(result->value));
	return result;
}



String *newStringN(char *s, int n) {
	String *result = calloc(1, sizeof(struct String));
	result->type = TYPE_STRING;
	result->value = malloc(n + 1);
	strncpy(result->value, s, n); // do not take ownership of argument s
	result->value[n] = '\0';
	return result;
}



void stringDelete(struct Object *obj)
{
	String *s = (String*) obj;
#ifdef DEBUG 
	printf("stringDelete(%s)\n", s->value);
#endif
	free(s->value);
	free(s);
}



char *stringToCString(Object *o) {
	String *s = (String*)o;
	return s->value;
}
