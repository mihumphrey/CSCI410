#ifndef GRAMMAR_H
#define GRAMMAR_H


#define KEYWORD_LIST_LEN 21

#define SYMBOL_LIST_LEN 19

typedef enum __TOKEN_TYPE__ {
    KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST
} TokenType;

typedef struct __TOKEN__ {
    char *value;
    char *tokenName;
    TokenType type;
} Token;

const char *KEYWORD_LIST[KEYWORD_LIST_LEN] = {
    "class",
    "method",
    "function", 
    "constructor",
    "int",
    "boolean",
    "char", 
    "void", 
    "var", 
    "static", 
    "field",
    "let",
    "do",
    "if",
    "else",
    "while",
    "return", 
    "true",
    "false",
    "null",
    "this",
};

const char SYMBOL_LIST[SYMBOL_LIST_LEN] = {
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    '.',
    ',',
    ';',
    '+',
    '-',
    '*',
    '/',
    '&',
    '|',
    '<',
    '>',
    '=',
    '-',
};


#endif
