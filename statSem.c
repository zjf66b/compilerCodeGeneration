#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statSem.h"

static const int STACK_SIZE = 100;  // maximum number of identifiers on the stack
static identifierStack stack = { .size = 0, .top = NULL };

void push(token tk) {
    if (stack.size == STACK_SIZE) {
        printf("STACK OVERFLOW\n");
        exit(stack.size);
    }

    identifierElement *newIdElem = (identifierElement *) malloc(sizeof(identifierElement));
    newIdElem->tk.type = tk.type;
    strcpy(newIdElem->tk.instance, tk.instance);
    newIdElem->tk.lineNum = tk.lineNum;

    newIdElem->next = stack.top;
    stack.top = newIdElem;
    stack.size++;
}

void pop() {
    if (stack.size > 0) {
        identifierElement *temp = stack.top;
        stack.top = temp->next;
        free(temp);
        stack.size--;
    }
    else {
        printf("STACK EMPTY\n");
        exit(stack.size);
    }
}

int find(token tk, int error) {
    int distance;  // distance from top of stack
    identifierElement *curIdElem = stack.top;

    for (distance = 0; distance < stack.size; distance++) {
        if (strcmp(tk.instance, curIdElem->tk.instance) == 0) {
            if (error == 1) {
                printf("Static Semantics Error: Variable '%s' previously defined on line %d (line %d)\n", tk.instance, curIdElem->tk.lineNum, tk.lineNum);
                exit(curIdElem->tk.lineNum);
            }
            if (error == 2) {
                return distance;
            }
        }

        curIdElem = curIdElem->next;
    }
    if (error == 2) {
        printf("Static Semantics Error: Variable '%s' used prior to definition (line %d)\n", tk.instance, tk.lineNum);
        exit(tk.lineNum);
    }
    return -1;  // variable not found in stack
}

void printStack() {
    int distance;
    identifierElement *curIdElem = stack.top;

    printf("STACK\n");

    for (distance = 0; distance < stack.size; distance++) {
        printf("%s %d\n", curIdElem->tk.instance, distance);

        curIdElem = curIdElem->next;
    }
}
