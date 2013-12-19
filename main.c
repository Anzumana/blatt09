#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Gui.h"
#include "Object.h"
#include "ObjectFunctions.h"
#include "List.h"
#include "Formula.h"
#include "Parser.h"
#include "main.h"

static struct Cell *cells[ROWS][COLUMNS];
static const char *currentFileName = "spreadsheet.txt";



void initCells(void)
{
	// printf("initCells\n");
	int i, j;
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLUMNS; j++) {
			struct Cell *c = newCell();
			c->row = i;
			c->column = j;
			cells[i][j] = c;
		}
	}
}



Double *sum(Double *a, Double *b)
{
	return newDouble(a->value + b->value);
}



Double *sub(Double *a, Double *b)
{
	return newDouble(a->value - b->value);
}



Double *mul(Double *a, Double *b)
{
	return newDouble(a->value * b->value);
}



Double *division(Double *a, Double *b)
{
	return newDouble(a->value / b->value);
}



Double *min(Double *a, Double *b)
{
	return newDouble(a->value <= b->value ? a->value : b->value);
}



Double *max(Double *a, Double *b)
{
	return newDouble(a->value >= b->value ? a->value : b->value);
}



int isDoubleObject(Object *obj) 
{
	return obj->type == TYPE_DOUBLE;
}



ReduceFunction getOperation(String *f) 
{
	if (strcmp(f->value, "sum") == 0) return (ReduceFunction) sum;
	if (strcmp(f->value, "add") == 0) return (ReduceFunction) sum;
	if (strcmp(f->value, "plus") == 0) return (ReduceFunction) sum;
	if (strcmp(f->value, "+") == 0) return (ReduceFunction) sum;
	if (strcmp(f->value, "sub") == 0) return (ReduceFunction) sub;
	if (strcmp(f->value, "minus") == 0) return (ReduceFunction) sub;
	if (strcmp(f->value, "-") == 0) return (ReduceFunction) sub;
	if (strcmp(f->value, "mul") == 0) return (ReduceFunction) mul;
	if (strcmp(f->value, "*") == 0) return (ReduceFunction) mul;
	if (strcmp(f->value, "div") == 0) return (ReduceFunction) division;
	if (strcmp(f->value, "/") == 0) return (ReduceFunction) division;
	if (strcmp(f->value, "min") == 0) return (ReduceFunction) min;
	if (strcmp(f->value, "max") == 0) return (ReduceFunction) max;
	return NULL;
}



/*
#define TYPE_UNKNOWN 0
#define TYPE_DOUBLE  1
#define TYPE_STRING  2
#define TYPE_LIST	5
#define TYPE_NODE	6
#define TYPE_COORD 7
#define TYPE_RANGE 8
#define TYPE_CALL  9
#define TYPE_CELL  10
*/

static EvalFunc evaluationFunctions[] = { // indexed by type, must fit into this range
	NULL, 		// TYPE_UNKNOWN
	doubleEval,	// TYPE_DOUBLE
	stringEval,	// TYPE_STRING
	NULL, 		// not used
	NULL, 		// not used
	NULL,		// TYPE_LIST
	NULL, 		// TYPE_NODE
	coordEval, 	// TYPE_COORD
	rangeEval, 	// TYPE_RANGE
	callEval, 	// TYPE_CALL
	NULL, 		// TYPE_CELL
};



Object *evaluate(Object *obj)
{
	EvalFunc f = evaluationFunctions[obj->type];
	return (f != NULL) ? f(obj) : NULL;
}



Object *doubleEval(Object *obj) {
	return obj; // a Double evaluates to itself
}



Object *stringEval(Object *obj) {
	return obj; // a String evaluates to itself
}



Object *coordEval(Object *obj) {
	Coord *c = (Coord*) obj;
	return cells[c->row][c->column]->value; // a Coord evaluates to the value of the cell it references (which has already been evaluated)
}



Object *rangeEval(Object *obj) { // a Range evaluates to the list of referenced cell values (which have already been evaluated)
	Range *range = (Range*) obj;
	List *result = newList();
	int row, col;
	for (row = range->c1->row; row <= range->c2->row; row++) {
		for (col = range->c1->column; col <= range->c2->column; col++) {
			Cell *cell = cells[row][col];
			if (cell->value != NULL) {
				append(result, cell->value);
			}
		}
	}
	return (Object*) result;
}



Object *callEval(Object *obj) { // a Call evaluates to the result of the call
	Call *c = (Call*) obj;
	ReduceFunction reduceFunc = getOperation(c->function);
	if (reduceFunc != NULL) {
		List *argvals = flatMap(evaluate, c->arguments);
		argvals = filter(isDoubleObject, argvals); // only Doubles
		return reduce(reduceFunc, argvals, NULL); // combine the arguments using the reduce function
	}
	return NULL;
}



void printIt(Object *o)
{
	Cell *c = (Cell*) o;
	printf("%c%d\n", 'A' + c->column, c->row + 1);
}



void processInput(char *input, int row, int column) 
{
	Cell *cell = cells[row][column];
    free(cell->input); // free old input
    cell->input = strdup(input); // copy input string
	printf("cell->input: %s (row %d, column %d) (strlen = %d)\n", 
		cell->input, row, column, (int)strlen(input));

	// *** todo, rest of function ***
	// new input, old cell references are obsolete



	// gegeben
	updateReferencedCells(cell);

	// *** todo ***
	saveFile(currentFileName);
}


	// *** todo ***
void updateReferencedCells(Cell *cell)
{
	// todo
}



// *** todo ***
/* Adds cells that obj (which is a formula) references to result list. 
 * result contains Cell objects. 
 * obj is a formula (String, Double, Coord, Range, or Call)
 */
List *references(List *result, Object *obj) 
{
// todo
}



// *** todo ***
void loadFile(const char *fileName) 
{
// todo
}



// *** todo ***
/**
 * Describe your implementation here
 */
void saveFile(const char *fileName) 
{
 // todo
}



int main(???) // *** todo ***
{
	initCells();

	// *** todo *** Load file here when passed to program
	

	while (1) {
		GuiInput input = guiGetInput();
		processInput(input.value, input.row, input.column);
	}

	return 0;
}
