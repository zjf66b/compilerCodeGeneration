#ifndef STATSEMANTICS_H
#define STATSEMANTICS_H

#include "token.h"

#define SIZE 9 // LENGTH + 1

typedef struct identifierElement {
    token tk;
    struct identifierElement *next;
} identifierElement;

typedef struct {
    int size;
    identifierElement *top;
} identifierStack;

void push(token);
void pop();
int find(token, int);
void printStack();

#endif
