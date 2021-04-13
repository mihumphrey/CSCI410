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

const char *STATEMENT_OPENINGS[STATEMENT_OPENINGS_LEN] = {
    "if",
    "let",
    "do",
    "while",
    "return",
};

const char *CLASS_VAR_OPENINGS[CLASS_VAR_OPENINGS_LEN] = {
    "static",
    "field",
};

const char *SUBROUTINE_OPENINGS[SUBROUTINE_OPENINGS_LEN] = {
    "constructor",
    "function", 
    "method",  
};

const char *OPERATORS[OPERATORS_LEN] = {
    "+",
    "-",
    "*",
    "/",
    "&amp",
    "|",
    "&lt",
    "&gt",
    "=",
};

const char UNARY_OPS[UNARY_OPS_LEN] = {
    '-',
    '~',
};

bool isClassVarOpening(const char *keyword) {
    for (int i = 0; i < CLASS_VAR_OPENINGS_LEN; i++) {
        if (STREQUALS(CLASS_VAR_OPENINGS[i], keyword))
            return true;
    }
    return false;
}

bool isSubroutineOpening(const char *keyword) {
    for (int i = 0; i < SUBROUTINE_OPENINGS_LEN; i++) {
        if (STREQUALS(SUBROUTINE_OPENINGS[i], keyword))
            return true;
    }
    return false;
}

bool isStatementOpening(const char *keyword) {
    for (int i = 0; i < STATEMENT_OPENINGS_LEN; i++) {
        if (STREQUALS(STATEMENT_OPENINGS[i], keyword))
            return true;
    }
    return false;
}

bool isSymbol(char in) {
    for (int i = 0; i < SYMBOL_LIST_LEN; i++) {
        if (SYMBOL_LIST[i] == in)
            return true;
    }
    return false;
}

bool isOperator(const char *keyword) {
    for (int i = 0; i < OPERATORS_LEN; i++) {
        if (STREQUALS(OPERATORS[i], keyword))
            return true;
    }
    return false;
}

bool isUnaryOp(const char op) {
    for (int i = 0; i < UNARY_OPS_LEN; i++) {
        if (op == UNARY_OPS[i])
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
        case KEYWORD: return "keyword";
        case IDENTIFIER: return "identifier";
        case INT_CONST: return "integerConstant";
        case STRING_CONST: return "stringConstant";
        case SYMBOL: return "symbol";
    }
    return "";
}
