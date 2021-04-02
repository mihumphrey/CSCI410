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
    if (in == '<') return "&lt;";
    else if (in == '>') return "&gt;";
    else if (in == '&') return "&amp;";
    else {
        char *out = calloc(1, 2); 
        strncpy(out, &in, 1); 
        out[1] = '\0';
        return out;
    }
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
