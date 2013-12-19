///////////////////////////////////////////////////////////
// fundamental classes

#ifndef OBJECT_H
#define OBJECT_H

// #define DEBUG

#define TYPE_UNKNOWN 0
#define TYPE_DOUBLE  1
#define TYPE_STRING  2

struct Object;

typedef struct Object {
	int type;
} Object;

typedef struct Double {
	int type;
	double value;
} Double;

Double *newDouble(double d);
char *doubleToCString(Object *obj);
void doubleDelete(Object *obj);

typedef struct String {
	int type;
	char *value;
} String;

String *newString(char *s);
String *newStringN(char *s, int n);
char *stringToCString(Object *obj);
void stringDelete(Object *obj);

#endif
