// main.h

#ifndef MAIN_H
#define MAIN_H

#include "Formula.h"

#define ROWS 100
#define COLUMNS 26

void initCells(void);

Object *evaluate(Object *obj);

typedef Object* (*EvalFunc)(Object *obj);

Object *doubleEval(Object *obj);
Object *stringEval(Object *obj);
Object *coordEval(Object *obj);
Object *callEval(Object *obj);
Object *rangeEval(Object *obj);

Double *sum(Double *a, Double *b);
Double *mul(Double *a, Double *b);
Double *division(Double *a, Double *b);
Double *min(Double *a, Double *b);
Double *max(Double *a, Double *b);
int isDoubleObject(Object *obj);

void processInput(char *input, int row, int column);
void updateReferencedCells(Cell *cell);

List *references(List *result, Object *obj);

void loadFile(const char *fileName);
void saveFile(const char *fileName);

#endif
