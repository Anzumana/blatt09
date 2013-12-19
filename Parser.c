#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "Formula.h"
#include "ObjectFunctions.h"
#include "Parser.h"

#define MAX_COMMAS 64



char* removeSpaces(const char *s) 
{
	int n = strlen(s);
	char *result = malloc(n);
	char *t = result;
	while (*s != '\0') {
		if (isspace(*s)) s++; // skip whitespace character
		else *t++ = *s++; // copy non-whitespace character
	}
	*t = '\0'; // terminate string
	return result;
}



int isInteger(const char *s)
{
	int state = 0;
	int nextState[][2] = { 
		// d,  -
		{  2,  1 }, // state 0
		{  2, -1 }, // state 1
		{  2, -1 }, // state 2
	};
	char c;
	while ((c = *s++) && (state >= 0)) {
		if (isdigit(c)) {
			state = nextState[state][0];
		} else if (c == '-') {
			state = nextState[state][1];
		} else {
			state = -1;
		}
	}
	return state == 2; // accepting state
}



int isDouble(const char *s)
{
	// printf("isDouble\n");
	int state = 0;
	int nextState[][3] = { 
		// d,  -,  .
		// *** todo ***
		{  2,  1, -1 }, // state 0
		{  2, -1, -1 }, // state 1
		{  2, -1,  3 }, // state 2
		{  4, -1, -1 }, // state 3
		{  4, -1, -1 }, // state 4
	};
	char c;
	while ((c = *s++) && (state >= 0)) {
		if (isdigit(c)) {
			state = nextState[state][0];
		} else if (c == '-') {
			state = nextState[state][1];
		} else if (c == '.') {
			state = nextState[state][2];
		} else {
			state = -1;
		}
	}
	return state == 2 || state == 4; // accepting states
}



int isCoordinate(const char *s) 
{
	// printf("isCoordinate\n");
	int state = 0;
	int nextState[][2] = { 
		// l,  d
		// *** todo ***
		{  1, -1 }, // state 0
		{ -1,  2 }, // state 1
		{ -1,  3 }, // state 2
		{ -1, -1 }, // state 3
	};
	char c;
	while ((c = *s++) && (state >= 0)) {
		c = toupper(c);
		if (c >= 'A' && c <= 'Z') {
			state = nextState[state][0];
		} else if (isdigit(c)) {
			state = nextState[state][1];
		} else {
			state = -1;
		}
	}
	return state >= 2;
}



int isRange(const char *s)
{
	int state = 0;
	int nextState[][3] = { 
		// l,  d,  :
		// *** todo ***
		{  1, -1, -1 }, // state 0
		{ -1,  2, -1 }, // state 1
		{ -1,  3,  4 }, // state 2
		{ -1, -1,  5 }, // state 3
		{  5, -1, -1 }, // state 4
		{ -1,  6, -1 }, // state 5
		{ -1,  7, -1 }, // state 6
		{ -1, -1, -1 }, // state 7
	};
	char c;
	while ((c = *s++) && (state >= 0)) {
		c = toupper(c);
		if (c >= 'A' && c <= 'Z') {
			state = nextState[state][0];
		} else if (isdigit(c)) {
			state = nextState[state][1];
		} else if (c == ':') {
			state = nextState[state][2];
		} else {
			state = -1;
		}
	}
	return state >= 6;
}



Object *parse(String *st)
{
	String *stNoSpaces = newString(st->value);
	char *s = stNoSpaces->value;
	s = removeSpaces(s); // @todo: wrong for strings with spaces, simplifies things
	// printf("parse %s\n", s);

	int n = strlen(s);
	if (n <= 0) return (Object*) newString("");

	if (s[0] == '=') { // an expression?
		// printf("expression: %s (strlen %d)\n", s+1, (int)strlen(s+1));
		return parseExpression(newString(s+1)); // remove '='
	}

	// not an expression, can be either a Double or a String

	if (isDouble(s)) { // a floating point number
		printf("is Double\n");
		double d = atof(s);
		return (Object*) newDouble(d);
	} else { // text
		printf("is String\n");
		return (Object*) st;
	}
}



Object *parseExpression(String *s)
{
	// printf("parseExpression %s %d\n", toCString(s), (int)strlen(s->value));
	// assert: s does not contain whitespace

	char *v = s->value;
	int n = strlen(v);
	int colon = -1; // index of colon
	int openParen = -1; // outermost parenthesis
	int closeParen = -1; // outermost parenthesis
	int parenLevel = 0;
	int commas[MAX_COMMAS]; // indices of commas
	int commasN = 0; // number of commas found
	int i;
	for (i = 0; i < n; i++) {
		char c = v[i];
		if (c == ':') {
			colon = i;
		} else if (c == ',' && parenLevel == 1 && commasN < MAX_COMMAS) {
			commas[commasN++] = i;
		} else if (c == '(') { 
			if (parenLevel == 0) {
				openParen = i;
			}
			parenLevel++;
		}
		else if (c == ')') {
			parenLevel--;
			if (parenLevel == 0) {
				closeParen = i;
				break;
			}
		}
	}
	// printf("openParen = %d, closeParen = %d, n = %d, c = %c, c = %d\n", openParen, closeParen, n, v[n-1], v[n-1]);

	if (openParen > 0 && closeParen > openParen && closeParen == n-1) { // a function call
		printf("is Call\n");
		String *function = newStringN(v, openParen);
		String *expressions = newStringN(v + openParen + 1, closeParen - openParen - 1);
		// printf("expressions: %s %d\n", toCString(expressions), (int)strlen(expressions->value));
		
		List *expressionList = newList();
		char *e = expressions->value;
		if (strlen(e) > 0) {
			char *p = e;
			for (i = 0; i < commasN; i++) {
				e[commas[i]-openParen-1] = '\0';
				append(expressionList, newString(p));
				p = e + commas[i] - openParen;
			}
			append(expressionList, newString(p));
		}
		/*
		printf("expressionList:\n");
		for (i = 0; i < length(expressionList); i++) {
			printf("%s\n", toCString(get(expressionList, i)));
		}
		*/
		List *arguments = map((MapFunction)parseExpression, expressionList);
		return (Object*) newCall(function, arguments);
	} else if (isDouble(v)) { // a number
		printf("is Double\n");
		double d = atof(v);
		return (Object*) newDouble(d);
	} else if (isCoordinate(v)) { // a coordinate
		printf("is Coord\n");
		int col = toupper(v[0]) - 'A';
		int row = atoi(v+1) - 1;
		return (Object*) newCoord(row, col);
	} else if (isRange(v)) { // range
		printf("is Range\n");
		int col1 = toupper(v[0]) - 'A';
		int row1 = atoi(v+1) - 1;
		int col2 = toupper(v[colon+1]) - 'A';
		int row2 = atoi(v+colon+2) - 1;
		return (Object*) newRange(newCoord(row1,col1), newCoord(row2,col2));
	} else { // text
		printf("is String\n");
		return (Object*) newString(v);
	}
}


