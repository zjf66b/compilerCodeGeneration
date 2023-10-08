#ifndef TESTTREE_H
#define TESTTREE_H
#include "node.h"

node * createNode(nodeType);
void traversePreorder(node *, int);
void visitNode(node *, int);
//void staticSemantics(node *, int *);
//void preorder(node *, int *);
//void verifyNode(node *, int *);
extern char *tokenNames[];

#endif
