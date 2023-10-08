#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct {
        char value;
        int tableColNum;
        int lineNum;
} character;

typedef enum {
	LOWER, UPPER, INT, WS, EQ, LESS, GREATER, COL, PLUS, MINUS, MULT, DIV, CAR, DOT, LPAREN,
	RPAREN, COMMA, LBRACE, RBRACE, SEMICOL, LBRACK, RBRACK, EXCLA, BAR, AMP,UNDR, ENDOFFILE, COMMENT
} characterType;


#endif
