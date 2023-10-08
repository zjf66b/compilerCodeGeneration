#ifndef NODE_H
#define NODE_H

#include "token.h"

typedef enum {
    PROGRAMnd, BLOCKnd, VARSnd, EXPRnd, Nnd, And, Mnd, Rnd, STATSnd, MSTATnd,
    STATnd, LABELnd, INnd, OUTnd, IFnd, LOOPnd, ASSIGNnd, ROnd, GOTOnd
    
} nodeType;

typedef struct node {
    nodeType label;
    token token1;
    token token2;
    token token3; 
    
    struct node *child1;
    struct node *child2;
    struct node *child3;
    struct node *child4;
    struct node *child5; 
    
    char* name; //alteration
} node;

#endif 
