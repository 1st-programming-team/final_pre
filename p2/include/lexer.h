#ifndef POLY__LEXER_HEADER_FILE
#define POLY__LEXER_HEADER_FILE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LEX_ERROR        (-1)

#define TOK_WORD         (-2)

#define TOK_M_BRAK_O     (-3)
#define TOK_M_BRAK_C     (-4)
#define TOK_S_BRAK_O     (-5)
#define TOK_S_BRAK_C     (-6)

#define TOK_ADD          (-7)
#define TOK_SUB          (-8)
#define TOK_MUL          (-9)
#define TOK_DIV          (-10)

#define TOK_POW          (-11)

typedef struct
{
	int *list;
	int index;
	int size;
} tokens;

tokens lex(char *buffer);

#endif
