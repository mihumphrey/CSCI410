#include "grammar.h"

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
    '~',
};

bool isSymbol(char in) {
    for (int i = 0; i < SYMBOL_LIST_LEN; i++) {
        if (SYMBOL_LIST[i] == in)
            return true;
    }
    return false;
}
char *getSymbol(char in) {
    char *out;
    if (in == '<') {
        out = malloc(5);
        strcpy(out, "&lt;");//return "&lt;";
    } else if (in == '>') {
        out = malloc(5);
        strcpy(out, "&gt;");
        out[4] = '\0';
    }//return "&gt;";
    else if (in == '&') {
        out = malloc(6);
        strcpy(out, "&amp;");
        out[5] = '\0';
    }//return "&amp;";
    else {
        out = malloc(2);
        strncpy(out, &in, 1); 
        out[1] = '\0';
    }
    return out;
}

bool isKeyword(char *keyword) {
    for (int i = 0; i < KEYWORD_LIST_LEN; i++) {
        if (STREQUALS(KEYWORD_LIST[i], keyword))
            return true;
    }
    return false;
}

bool isNum(char *in) {
    for (int i = 0; i < strlen(in); i++) {
        if (in[i] < 48 || in[i] > 57)
            return false;
    }
    return true;
}

char *getTokenType(TokenType type) {
    switch(type) {
        case T_KEYWORD: return "keyword";
        case T_IDENTIFIER: return "identifier";
        case T_INT_CONST: return "integerConstant";
        case T_STRING_CONST: return "stringConstant";
        case T_SYMBOL: return "symbol";
    }
    return "";
}
