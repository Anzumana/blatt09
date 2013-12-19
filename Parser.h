#ifndef PARSER_H
#define PARSER_H

#include "Object.h"

char* removeSpaces(const char *s);

int isInteger(const char *s);
int isDouble(const char *s);
int isCoordinate(const char *s);
int isRange(const char *s);

Object *parse(String *st);
Object *parseExpression(String *s);

#endif
