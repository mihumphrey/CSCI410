#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "includes.h"

#define KEYWORD_LIST_LEN 21

#define SYMBOL_LIST_LEN 19

typedef enum __TOKEN_TYPE__ {
    T_KEYWORD, T_SYMBOL, T_IDENTIFIER, T_INT_CONST, T_STRING_CONST
} TokenType;

typedef enum __ANALYZER_TYPE__ {
    A_CLASS, A_KEYWORD, A_IDENTIFIER, A_SYMBOL, A_INT_CONST, A_STRING_CONST, A_SUBROUTINE_DEC, A_SUBROUTINE_BODY, A_PARAMETER_LIST, A_VAR_DEC, A_LET_STATEMENT, A_EXPRESSION, A_EXPRESSION_LIST, A_TERM, A_WHILE_STATEMENT, A_DO_STATEMENT, A_IF_STATEMENT, A_RETURN_STATEMENT
} AnalyzerType;

typedef struct __TOKEN__ {
    char *name;
    TokenType type;
} Token;

typedef struct __CHAR_LIST__ {
    char *list;
    size_t used;
    size_t size;
} CharList;
typedef struct __TOKEN_LIST__ {
    Token **list;
    size_t used;
    size_t size;
} TokenList;




const char *KEYWORD_LIST[KEYWORD_LIST_LEN];
const char SYMBOL_LIST[SYMBOL_LIST_LEN];


bool isSymbol(char in);
char *getSymbol(char in);
bool isKeyword(char *keyword);
bool isNum(char *in);
char *getTokenType(TokenType type);

#endif
