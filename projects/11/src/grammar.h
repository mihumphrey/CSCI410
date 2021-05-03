#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "includes.h"

#define KEYWORD_LIST_LEN 21

#define SYMBOL_LIST_LEN 19

#define CLASS_VAR_OPENINGS_LEN 2
#define SUBROUTINE_OPENINGS_LEN 3
#define STATEMENT_OPENINGS_LEN 5
#define OPERATORS_LEN 9
#define UNARY_OPS_LEN 2

typedef enum __TOKEN_TYPE__ {
    KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST
} TokenType;

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
    size_t iter;
} TokenList;




const char *KEYWORD_LIST[KEYWORD_LIST_LEN];
const char SYMBOL_LIST[SYMBOL_LIST_LEN];
const char *CLASS_VAR_OPENINGS[CLASS_VAR_OPENINGS_LEN];
const char *SUBROUTINE_OPENINGS[SUBROUTINE_OPENINGS_LEN];
const char *STATEMENT_OPENINGS[STATEMENT_OPENINGS_LEN];
const char *OPERATORS[OPERATORS_LEN];
const char UNARY_OPS[UNARY_OPS_LEN];

bool isClassVarOpening(const char *keyword);
bool isSubroutineOpening(const char *keyword);
bool isStatementOpening(char *keyword);
bool isOperator(const char *keyword);
bool isUnaryOp(const char op);
bool isSymbol(char in);
char *getSymbol(char in);
bool isKeyword(char *keyword);
bool isNum(char *in);
char *getTokenType(TokenType type);

const char *getArithCommand(char *command);
const char *getUnaryArithCommand(char command);
#endif
