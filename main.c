/* Zachary J Fulliam cs4280-002 project 4 code generation 
Fall 2022 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "generator.h"
#include "parser.h"
#include "testTree.h"



int main(int argc, char *argv[])
{
    if ( argc > 2 ){
        printf( " P4 ERROR: p4 expects one or no arguments. \n" );
        return ( 1 );
    }
    
    FILE *inFilePtr = NULL;
    FILE *outFilePtr = NULL;
    char *fileName = NULL;
    char *inFile = NULL;
    
    const char outFileExt[] = ".file.asm"; // target file extension postfix
    
    if ( argc == 2 ){
        
        
        fileName = (char*)malloc(sizeof(argv[1]));
        strcpy( fileName, argv[1] );
        char inFile[strlen(fileName)];
       // inFile = (char *)malloc(sizeof(argv[1])); //memory allocation for fileName
        strcpy( inFile, fileName );
        
        inFilePtr = fopen(inFile, "r");
        
        if (!inFilePtr ){
            printf(" file error in p4: FILE cannot be opened. \n");
            return ( 1 );
        }
        
        
    }
    else {
        const char outFileName[]= "kb.asm";
        fileName = (char *) malloc(sizeof(outFileName));
        strcpy(fileName, outFileName);
        inFilePtr = stdin;
    }
    
    node *root = parser ( inFilePtr);
       fclose(inFilePtr);
 //   traversePreorder ( root, 0 );
 //   staticSemantics(root, NULL);
 //   printf("\nStatic Semantic Success\n");
    char outFile[strlen(fileName) + strlen(outFileExt)]; // outFile = fileName.input1
    strcpy(outFile, fileName);
    strcat(outFile, outFileExt);
    free(fileName); // Remove allocated memory storage
    outFilePtr = fopen(outFile, "w"); // Open outFile for writing permission and assign to outFilePtr

    if (!outFilePtr) {
        printf("Error in P4: File '%s' could not be opened\n", outFile);
        return ( 1 );
    }
    
    generator(root, outFilePtr); //Call generator.c file
    fclose(outFilePtr);		 // CLose outFilePtr upon success

    if (inFile != NULL) {
        printf("comp %s\n", inFile); //Upon success, print .input1 target
    }
    
    printf("comp %s\n", outFile);    //Upon success, print .asm target 
    free(inFile); // Remove allocated memory storage
    return ( 0 );
}
