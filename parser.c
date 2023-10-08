// zachary j fulliam cs4280-002 program translation project 10/25/22 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "node.h"
#include "token.h"
#include "scanner.h"
#include "testTree.h"

static FILE *inFilePtr;
static token tk;
extern char *tokenNames[];


node * parser ( FILE *inputFile)
{
    inFilePtr = inputFile;
    
    node *root;
    
    setLookaheadChar( inFilePtr ); 
    tk = scanner ( inFilePtr ); 
    root = program();
    
    tk = scanner( inFilePtr);  
    if ( tk.type != EOFtk) {
        error2();
    }
    printf( "Parser Success\n");
    return root;
}

//<program>  ->  <vars> program <block>
node * program() {
    node *nodeV = createNode(PROGRAMnd);
  
            nodeV-> child1 = vars();

        if(tk.type == PROGRAMtk){
            nodeV->token1 = tk;
            tk = scanner(inFilePtr);
            
           
            nodeV-> child2 = block();
          
            if ( tk.type != ENDtk) {
                
                error();
            } 
        }
        return nodeV;
}

//<block> ->  begin <vars> <stats> end
node * block() {
    
    if(tk.type == BEGINtk) {
        node *node = createNode(BLOCKnd);
        tk = scanner( inFilePtr);
        
        node-> child1 = vars();
        node-> child2 = stats();
        tk = scanner(inFilePtr);
        
        if (tk.type == ENDtk ) {
           
            return node;
        }
      
        error();
    }
    
    error();
    return NULL;
}

// <vars>  ->    empty | whole Identifier :=  Integer  ;  <vars> 
node * vars() {
    node *node = createNode(VARSnd);
    
        if (tk.type == WHOLEtk) {
            tk = scanner(inFilePtr);

            if (tk.type == IDtk) {
		        node->token1 = tk;
                tk = scanner(inFilePtr);
              if (tk.type == COLEQtk) {
                    tk = scanner ( inFilePtr );
                
                if (tk.type == INTEGERtk) {
                    node->token2 = tk;
                    tk = scanner(inFilePtr);
                    
                    if (tk.type == SEMICOLtk) {
                    node->token3 = tk;
                    tk = scanner(inFilePtr);
                    
                    node->child1 = vars();
                    return node;
                    }
                    error();
                }
                error();
            }
            error();
        } 
        error();
    }
    return node;

}

//<expr> ->      <N> - <expr>  | <N>
node * expr() {
    node *node = createNode(EXPRnd);
        node-> child1 = N();
        
        if(tk.type == MINUStk) {
            node-> token1 = tk;
            tk = scanner ( inFilePtr );
            
            node-> child2 = expr();
            return node;
        }
        else{
             return node;
        }
       
}

//<N>  ->       <A> + <N> | <A> * <N> | <A>
node * N() {
    node *node = createNode(Nnd);
        node-> child1 = A();
        
        if (tk.type == PLUStk || tk.type == MULTtk){
            node->  token1 = tk;
            tk = scanner(inFilePtr);
            
            node->child2 = N();
            return node;
        }
        else {
        return node;
    }
}
// <A>  ->        <A> / <M> | <M>
node * A() {
    node *node = createNode(And);
          node->child1 = M();
       
       if (tk.type == DIVtk ) {
          node-> token1 = tk;
          tk = scanner( inFilePtr);
          
          node->child2 = M();
          return node;
       }
       else{
          
           return node;
       }
           
}

// <M>  ->     : <M> |  <R>
node * M() {
    node *node = createNode(Mnd);
    
    if(tk.type == COLtk){
        node-> token1 = tk;
        tk = scanner ( inFilePtr);
        node-> child1 = M();
        return node;
    }
    else{
        node-> child1 = R();
        return node;
    }
}

//<R>  ->      ( <expr> ) | Identifier | Integer
node * R() {
    node *node = createNode(Rnd);
    
    if(tk.type == LPARENtk){
        node->token1 = tk;
        tk = scanner( inFilePtr );
        
        node->child1 = expr();
        
        if(tk.type == RPARENtk){
            node->token2 = tk;
            tk = scanner( inFilePtr );
            return node;
        }
        error();
    }
    else if(tk.type == IDtk) {
        node-> token1 = tk;
        tk = scanner( inFilePtr );
        return node;
    }
    else if( tk.type == INTEGERtk ) {
        node -> token1 = tk;
        tk = scanner( inFilePtr );
        return node;
    }
    
    error();
    
}

// <stats>   ->      <stat>  <mStat>
node * stats() {
    node *node = createNode(STATSnd);
    node-> child1 = stat();
    
    node-> child2 = mStat();
    return node;
}

//<mStat>     ->      empty |  <stat>  <mStat>
node *mStat() {
    node *node = createNode(MSTATnd);
   
    if( tk.type == INPUTtk || tk.type == OUTPUTtk || tk.type == BEGINtk || tk.type  == IFtk || tk.type == WHILEtk || tk.type == ASSIGNtk || tk.type == WARPtk || tk.type == LABELtk) {
            node-> child1 = stat();
            node-> child2 = mStat();

    }

    return node;
}

// <stat>    ->      <in> ;  | <out> ;  | <block> | <if> ;  | <loop> ;  | <assign> ; | <goto> ; | <label> ;
node *stat() {
    node *node = createNode(STATnd);
    
    if (tk.type == INPUTtk) {
        node->child1 = in();
        tk = scanner( inFilePtr );
        if(tk.type == SEMICOLtk){
            return node; 
        }
    }
    else if(tk.type == OUTPUTtk){
        node->child1 = out();
     //   tk = scanner( inFilePtr ); // y is this here
        if(tk.type == SEMICOLtk){
            return node;
        }
    }
    else if(tk.type == BEGINtk){
        node-> child1 = block();
        return node;
    }
    else if(tk.type == IFtk){
        tk = scanner( inFilePtr );
        if(tk.type == SEMICOLtk){
            node-> child1 = If();
            return node;
        }
    }
    else if(tk.type == WHILEtk ){
     node-> child1 =  loop();
       // removed conditional if here gets past while statement regardless now
       tk=scanner( inFilePtr);
       if(tk.type == SEMICOLtk){
           
           return node;
       }
           // return node;
        
    }
    else if(tk.type == ASSIGNtk){
        node-> child1 = assign();
          
        if(tk.type == SEMICOLtk){
            tk = scanner(inFilePtr );
            return node;
        }
    }
    else if(tk.type == WARPtk){
        tk = scanner( inFilePtr);
        if(tk.type == SEMICOLtk){
            node-> child1 = GOTO();
            return node;
        }
    }
    else if(tk.type == LABELtk){
        tk = scanner( inFilePtr );
        if(tk.type == SEMICOLtk){
            node-> child1 = label();
            return node;
        }
    }
    
    error();
    return NULL;
    
}
//<in>   ->      input  Identifier
node * in() {
    
    node *node = createNode(INnd);
    token newTk = tk;//delte delete
    if(tk.type == INPUTtk){
        tk = scanner(inFilePtr);
        newTk = tk;//delte
        if(tk.type == IDtk){
        node->token1 = tk;
            return node;
            }
        }
  error();  
  return NULL;
} 

// <out>  ->      output <expr>
node * out() {
    node *node = createNode(OUTnd);
    if(tk.type == OUTPUTtk){
        node->token1 = tk;
        tk = scanner(inFilePtr);
        node-> child1 = expr();
        return node;
    }
    error();
    return NULL;
}

//<if>  ->      if [ <expr> <RO> <expr> ] then <stat> |  if [ <expr> <RO> <expr> ] then <stat> pick <stat>
node * If() {
    node *node = createNode(IFnd);
        if(tk.type == IFtk){
            tk = scanner(inFilePtr);
            if(tk.type == LBRACKtk){
                tk = scanner(inFilePtr);
                    node->child1 = expr();
                    node->child2 = RO();
                    node->child3 = expr();
                    
                    if(tk.type == RBRACKtk){
                        tk = scanner( inFilePtr );
                        if(tk.type == THENtk){
                            node-> child4 = stat();
                            return node;
                        }
                    }
                
            }
        }
        else { if(tk.type == IFtk){
            tk = scanner(inFilePtr);
            if(tk.type == LBRACKtk){
                tk = scanner( inFilePtr );
                    node->child1 = expr();
                    node->child2 = RO();
                    node->child3 = expr();
                    
                    if(tk.type == RBRACKtk){
                        tk = scanner( inFilePtr );
                        if(tk.type == THENtk){
                            node-> child4 = stat();
                            tk = scanner ( inFilePtr );
                            if(tk.type == PICKtk){
                                node-> child5 = stat();
                                return node;
                            }
                        }
                    }
                }
                 error();
            }
        }
        return NULL;
}

//<loop>  ->      while  [ <expr> <RO> <expr> ]  <stat>
node * loop() {
    node *node = createNode(LOOPnd);        //changed this and output now working for this, erroring on SEMICOLtk after end.
    
    if (tk.type == WHILEtk ){
        tk = scanner( inFilePtr );
        
        if(tk.type == LBRACKtk) {
            tk = scanner( inFilePtr );
            
            node-> child1 = expr();
            node-> child2 = RO();
            node-> child3 = expr();
            
            if(tk.type == RBRACKtk) {
                tk = scanner( inFilePtr );
                
                node-> child4 = stat();
                return node;
            }
            error();
        }
        error();
    }
    error();
    return NULL;
}

//<assign>  ->      assign Identifier  = <expr> 
node * assign() {
    node *node = createNode(ASSIGNnd);
        if( tk.type == ASSIGNtk){
            node-> token1 = tk;
            tk = scanner(inFilePtr);
            
            if(tk.type == IDtk){
                node->token2 = tk;
                tk = scanner(inFilePtr);
                
                if(tk.type == EQtk){
                node->token2 = tk;
                tk = scanner( inFilePtr );
                
                node->child1 = expr();
                return node;
                }
                ;
                error();
            }
          
            error();
        }
        
        error();
        return NULL;
}
//<RO>    ->       >  | < |  ==  |   [ = ]  (three tokens)  | !=
node * RO() {
    node *node = createNode(ROnd);
    
    if(tk.type == GREATERtk || tk.type == LESStk || tk.type == COMPAREtk  ){
        node->token1 = tk;
        tk = scanner( inFilePtr);
        
        if(tk.type == LBRACKtk){
            node->token1 = tk;
            return node;
            
            tk = scanner( inFilePtr);
            if(tk.type == EQtk){
                node->token2 = tk;
                return node;
                
                tk = scanner( inFilePtr );
                if( tk.type == RBRACKtk){
                    node-> token3 = tk;
                    return node;
                }
            }
            
        }
    }
    return NULL;
}

//<label>   ->   label Identifier

node * label() {
    node *node = createNode(LABELnd);
    if(tk.type == LABELtk){
        node-> child1 = label();
        return node;
    }
    error();
    return NULL;
}
//<goto>  ->    warp Identifier

node * GOTO() {
    node *node = createNode(GOTOnd);
    if(tk.type == WARPtk) {
        node->child1 = GOTO();
        return node;
    }
    error();
    return NULL;
}

void error() {
    printf("Parser Error: Unexpected token '%s' received (line %d)\n", tokenNames[tk.type], tk.lineNum);
    exit(tk.type);
}

void error2() {
    printf("Parser Error: Extra token '%s' received after end of program (line %d)\n", tokenNames[tk.type], tk.lineNum);
    exit(tk.type);
}
