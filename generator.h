#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include "node.h"

void recGen(node *, int *, FILE *);
void generator(node *, FILE *);
void staticSemantics(node *, int *, FILE *);
int verifyNode(node *, int *, FILE *);

#endif
