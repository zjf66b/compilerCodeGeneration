#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "node.h"

node * parser(FILE *);

node * program();

node * block();

node * vars();

node * expr();

node * N();

node * A();

node * M();

node * R();

node * stats();

node * mStat();

node * stat();

node * label();

node * in();

node * out();

node * If();

node * loop();

node * assign();

node * RO();

node * GOTO();

void error();

void error2();

#endif
