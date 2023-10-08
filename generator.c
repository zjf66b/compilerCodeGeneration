#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testTree.h"
#include "generator.h"
#include "token.h"
#include "node.h"
#include "statSem.h"
#define LENGTH 5

static int labelCount = 0;      // counting unique labels generated
static int tempVarCount = 0;    // counting unique temps generated
static char name[LENGTH];       // for creation of unique names

typedef enum {
    VAR,                // for unique  Vars
    LABEL               // for nodeType label
} nameType;


static char *newName(nameType type) {
    if (type == VAR) {
        sprintf(name, "V%d", tempVarCount++); // increment tempVarCount if VAR
    }
    else if (type == LABEL) {
        sprintf(name, "L%d", labelCount++);   // increment labelCount if Label
    }
    return name;
}

void recGen(node *n, int *varCount, FILE *outFilePtr) {

    int distance;
    char label[LENGTH];
    char label2[LENGTH];
    char arg[LENGTH];
    distance = verifyNode(n, varCount, outFilePtr);

    // Setup switch/case statement to traverse nodeType 
    // and perform Static Semantics and Code Generation
    // for corresponding Labels
    switch (n->label) {

	// <expr>        ->      <A> + <expr> | <A> - <expr> | <A>
        case EXPRnd:   if (n->token1.type != NULLtk) {
                           staticSemantics(n->child2, varCount, outFilePtr);
                           strcpy(arg, newName(VAR));
                           fprintf(outFilePtr, "STORE %s\n", arg);
                          
 			   staticSemantics(n->child1, varCount, outFilePtr);
                           if (n->token1.type == PLUStk) {
                               fprintf(outFilePtr, "ADD %s\n", arg);
                           }
                           else if (n->token1.type == MINUStk) {
                               fprintf(outFilePtr, "SUB %s\n", arg);
                           }
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;

	// <A>             ->        <N> * <A> | <N>
        case And:      if (n->token1.type != NULLtk) {
                           staticSemantics(n->child2, varCount, outFilePtr);
                           strcpy(arg, newName(VAR));
                           fprintf(outFilePtr, "STORE %s\n", arg);

                           staticSemantics(n->child1, varCount, outFilePtr);
                           if (n->token1.type == MULTtk) {
                               fprintf(outFilePtr, "MULT %s\n", arg);
                           }
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;

	// <N>             ->       <M> / <N> | <M>	
        case Nnd:
                       if (n->token1.type != NULLtk) {
                           staticSemantics(n->child2, varCount, outFilePtr);
                           strcpy(arg, newName(VAR));

                           fprintf(outFilePtr, "STORE %s\n", arg);
                           staticSemantics(n->child1, varCount, outFilePtr);
                           if (n->token1.type == DIVtk) {
                               fprintf(outFilePtr, "DIV %s\n", arg);
                           }
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;

	// <M>              ->     - <M> |  <R>
        case Mnd:      staticSemantics(n->child1, varCount, outFilePtr);
                       if (n->child1->label == Mnd ) {
                           fprintf(outFilePtr, "MULT -1\n");
                       }
                       break;

	// <R>              ->      [ <expr> ] | Identifier | Integeri
	// For <R> nodes with Local IDtk  = STACKR
	// 		      Local INTtk = LOAD
        case Rnd:      if (n->child1 == NULL) {
                           if (n->token1.type == IDtk) {
                               fprintf(outFilePtr, "STACKR %d\n", distance);
                           }
                           else if (n->token1.type == INTEGERtk) {//CHANGED THIS
                               fprintf(outFilePtr, "LOAD %s\n", n->token1.instance);
                           }
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;

	// <in>              ->      Read [ Identifier ]  
	// for <in> nodes with Local Variables
	// create tempory READ, LOAD, STACKW#
	// to target file .asm
        case INnd:     strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "READ %s\n", arg);
                       fprintf(outFilePtr, "LOAD %s\n", arg);
                       fprintf(outFilePtr, "STACKW %d\n", find(n->token1, 2));
                       break;

	// <out>            ->      Output [ <expr> ]
        case OUTnd:    staticSemantics(n->child1, varCount, outFilePtr);
                       strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "STORE %s\n", arg);
                       fprintf(outFilePtr, "WRITE %s\n", arg);
                       break;

	// <if>               ->      IFF [ <expr> <RO> <expr> ] <stat>
        case IFnd:     staticSemantics(n->child3, varCount, outFilePtr);
                       strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "STORE %s\n", arg);

                       staticSemantics(n->child1, varCount, outFilePtr);
                       fprintf(outFilePtr, "SUB %s\n", arg);
                       strcpy(label, newName(LABEL));

		       // Setup switch statement for Relational Operator Tokens
                       switch (n->child2->token1.type) {
                           case GREATERtk: n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRNEG %s\n", label) : fprintf(outFilePtr, "BRZNEG %s\n", label);
                                           break;
                           case LESStk:    n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRPOS %s\n", label) : fprintf(outFilePtr, "BRZPOS %s\n", label);
                                           break;
                           case EQtk:      n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label, label);
                                           break;
                           default:        n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label, label);
                                           break;
                       }
                       staticSemantics(n->child4, varCount, outFilePtr);
                       fprintf(outFilePtr, "%s: NOOP\n", label);
                       break;

	// <loop>          ->      Loop [ <expr> <RO> <expr> ] <stat>
        case LOOPnd:   strcpy(label, newName(LABEL));
                       fprintf(outFilePtr, "%s: NOOP\n", label);
                       staticSemantics(n->child3, varCount, outFilePtr);
                       strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "STORE %s\n", arg);

                       staticSemantics(n->child1, varCount, outFilePtr);
                       fprintf(outFilePtr, "SUB %s\n", arg);
                       strcpy(label2, newName(LABEL));

		       // Setup switch statement for Relational Operator Tokens
                       switch (n->child2->token1.type) {
                           case GREATERtk: n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRNEG %s\n", label2) : fprintf(outFilePtr, "BRZNEG %s\n", label2);
                                           break;
                           case LESStk:    n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRPOS %s\n", label2) : fprintf(outFilePtr, "BRZPOS %s\n", label2);
                                           break;
                           case EQtk:      n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label2) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label2, label2);
                                           break;
                           default:        n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label2) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label2, label2);
                                           break;
                       }
                       staticSemantics(n->child4, varCount, outFilePtr);
                       fprintf(outFilePtr, "BR %s\n", label);
                       fprintf(outFilePtr, "%s: NOOP\n", label2);
                       break;

	// <assign>       ->      Identifier  = <expr>  
        case ASSIGNnd: staticSemantics(n->child1, varCount, outFilePtr);
                       fprintf(outFilePtr, "STACKW %d\n", distance);
                       break;

        default:       staticSemantics(n->child1, varCount, outFilePtr);
                       staticSemantics(n->child2, varCount, outFilePtr);
                       staticSemantics(n->child3, varCount, outFilePtr);
                       staticSemantics(n->child4, varCount, outFilePtr);
                       break;
    }
}

void generator(node *program, FILE *outFilePtr) {

    if (program == NULL || program->label != PROGRAMnd) {
        printf("Generator Error: Invalid parse tree received\n");
        exit(program->label);
    }

    staticSemantics(program, NULL, outFilePtr); //If program exists, call this function to proceed//

    fprintf(outFilePtr, "STOP\n"); //Return after staticSemantics fxn processed
    int i;
    for (i = 0; i < tempVarCount; i++) {
        fprintf(outFilePtr, "V%d 0\n", i);
    }
}

void staticSemantics(node *n, int *varCount, FILE *outFilePtr) {
    if (n != NULL) { //If not empty, proceed
        if (n->label == BLOCKnd || n->label == PROGRAMnd) { //PROGRAM or BLOCK
            int *varCount = (int *) malloc(sizeof(int));
            *varCount = 0;

            recGen(n, varCount, outFilePtr); //Call main recGen fxn above
	    int i;
            for (i = 0; i < *varCount; i++) {
                pop();	// Print POP to target file upon calling pop()
                fprintf(outFilePtr, "POP\n");
            }
            free(varCount); //Free allocated memory
        }
        else {
            recGen(n, varCount, outFilePtr);
        }
    }
}

int verifyNode(node *n, int *varCount, FILE *outFilePtr) {

    int distance = (-1);
    if (n->label == VARSnd) {
        if (n->token1.type == IDtk) {
            if (*varCount > 0) {
                distance = find(n->token1, 1);  // set error reporting to type 1
            }
            push(n->token1); 	// Print PUSH to target file upon calling push()
            fprintf(outFilePtr, "PUSH\n");
            (*varCount)++;
            return distance;
        }
    }
    else if (n->label == Rnd || n->label == INnd || n->label == ASSIGNnd) {
        if (n->token1.type == IDtk) {
            distance = find(n->token1, 2);  // set error reporting to type 2
            return distance;
        }
    }
    return distance;
}
