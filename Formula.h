///////////////////////////////////////////////////////////
// fundamental classes

#ifndef FORMULA_H
#define FORMULA_H

#define TYPE_COORD 7
#define TYPE_RANGE 8
#define TYPE_CALL  9
#define TYPE_CELL  10

#include "Object.h"
#include "List.h"

typedef struct Coord {
	int type;
	int row;
	int column;
} Coord;

typedef struct Range {
	int type;
	Coord *c1;
	Coord *c2;
} Range;

typedef struct Call {
	int type;
	String *function;
	List *arguments;
} Call;

typedef struct Cell {
	int type;
	int row;
	int column;
	char *input; // the string the user typed
	Object *formula; // the formula, if any (parsed user input)
	Object *value; // the evaluated formula, if applicable (formula applied to spreadsheet data)
	List *listeners; // list of other cells that depend on this cell
	List *listeningTo; // list of other cells that this cell depends on
} Cell;

Coord *newCoord(int row, int column);
char *coordToCString(Object *obj);

Range *newRange(Coord *c1, Coord *c2);
char *rangeToCString(Object *obj);

Call *newCall(String *function, List *arguments);
char *callToCString(Object *obj);

Cell *newCell(void);
char *cellToCString(Object *obj);

#endif
