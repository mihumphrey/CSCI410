#include "tokenizer.h"

TokenList *getTokens(char *input) {
    TokenList *tokens = calloc(1, sizeof(TokenList));
    initList_Token(tokens, 1);
    CharList *stringConst = NULL;
    CharList *otherString = NULL;
    bool inStringConst = false;
    bool inOther = false;
    
    for (int i = 0; i < strlen(input); i++) {
        bool insertSymbol = false, insertString = false;
        Token *symbolToken = NULL;
        Token *stringToken = NULL;
        bool wasInOther = inOther;
        if (inOther && input[i] == ' ' && !inStringConst) {
            inOther = false;

        } else if (isSymbol(input[i])) {
            char *symbol = getSymbol(input[i]);
            symbolToken = malloc(sizeof(Token));
            symbolToken->name = symbol;
            symbolToken->type = T_SYMBOL;
            insertSymbol = true;
            inOther = false;

        } else if (input[i] == '"' && !inStringConst) {
            inStringConst = true;
            stringConst= malloc(sizeof(CharList));
            initList_char(stringConst, 1);
            inOther = false;

        } else if (input[i] == '"' && inStringConst) {
            inStringConst = false;
            stringToken = malloc(sizeof(Token));
            char *constName = calloc(1, stringConst->used + 1);
            strncpy(constName, stringConst->list, stringConst->used );
            constName[stringConst->used] = '\0';
            stringToken->name = constName;
            stringToken->type = T_STRING_CONST;
            insertString = true;
            inOther = false;

        } else if (inStringConst) {
            insertList_char(stringConst, input[i]);
            inOther = false;

        } else if (!inOther) {
            inOther = true;
            otherString = malloc(sizeof(CharList));
            initList_char(otherString, 1);
            if (input[i] != ' ')
                insertList_char(otherString, input[i]);

        } else if (inOther) {
            if (input[i] != ' ')
                insertList_char(otherString, input[i]);
        } 
        
        if (wasInOther && !inOther) {
            bool isspace = false;
            char *otherChar = malloc(otherString->used + 1);
            strncpy(otherChar, otherString->list, otherString->used);
            otherChar[otherString->used] = '\0';
            if (STREQUALS(otherChar, " ")) {
                isspace = true;
                printf("BAHSALKFJALSKDJFL");
            }

            if (!isspace) {
                Token *t = malloc(sizeof(Token));
                if (isKeyword(otherChar))
                    t->type = T_KEYWORD;
                else if (isNum(otherChar))
                    t->type = T_INT_CONST;
                else
                    t->type = T_IDENTIFIER;
        
                t->name = otherChar;
                insertList_Token(tokens, t);
            } else free(otherChar);
            freeList_char(otherString);
        }

        if (insertSymbol) {
            insertList_Token(tokens, symbolToken);
        }
        if (insertString) {
            insertList_Token(tokens, stringToken);
            freeList_char(stringConst);
        }
        
    }     
    return tokens;    
}

void writeTokens(TokenList *tokens, FILE *outputFile) {
    WRITE("<tokens>\n")
    for (int i = 0; i < tokens->used; i++) {
        char *name = tokens->list[i]->name;
        char *type = getTokenType(tokens->list[i]->type);
        WRITE("<%s> %s </%s>\n", type, name, type)
    }
    WRITE("</tokens>")
}

