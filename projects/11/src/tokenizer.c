#include "tokenizer.h"

extern bool verbose;
//********************************************************************************************************************//
//* Function getTokens                                                                                               *//
//*     Input:                                                                                                       *//
//*         input: char * -- flat array string of the program to compile                                             *//
//*     Tokenizes input based upon grammar given in book                                                             *//
//*     Returns:                                                                                                     *//
//*         TokenList * -- list of tokens                                                                            *//
//********************************************************************************************************************//
TokenList *getTokens(char *input) {
    if (verbose)
        fprintf(stderr, "\t\t* Beginning Tokenizing\n");
    
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
            symbolToken->type = SYMBOL;
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
            stringToken->type = STRING_CONST;
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
            }

            if (!isspace) {
                Token *t = malloc(sizeof(Token));
                if (isKeyword(otherChar))
                    t->type = KEYWORD;
                else if (isNum(otherChar))
                    t->type = INT_CONST;
                else
                    t->type = IDENTIFIER;
        
                t->name = otherChar;
                if (verbose)
                    fprintf(stderr, "\t\t\t* Inserting Token -- NAME: %s\t     TYPE: %s\n", t->name, getTokenType(t->type)); 
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
    if (verbose)
        fprintf(stderr, "\t\t* Done Tokenizing\n");
    return tokens;    
}

//********************************************************************************************************************//
//* Helper Function initList_char                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: CharList * -- List of tokens to append to                                                        *//
//*         initialSize: size_t -- size to initialize list to                                                        *//
//*     Initializes a dynamic string                                                                                 *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void initList_char(CharList *tokens, size_t initialSize) {
    tokens->list = malloc(initialSize);
    tokens->used = 0;
    tokens->size = initialSize;
}

//********************************************************************************************************************//
//* Helper Function insertList_char                                                                                  *//
//*     Input:                                                                                                       *//
//*         tokens: CharList * -- List of tokens to append to                                                        *//
//*         element: char -- element to append                                                                       *//
//*     Appends to charList (dynamically)                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void insertList_char(CharList *tokens, char element) {
    if (tokens->used == tokens->size) {
        tokens->size *= 2;
        tokens->list = realloc(tokens->list, tokens->size);
    }
    tokens->list[tokens->used++] = element;
}

//********************************************************************************************************************//
//* Helper Function freeList_char                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: CharList * -- List of tokens to free                                                             *//
//*     Frees all data used by CharList                                                                             *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void freeList_char(CharList *tokens) {
    free(tokens->list);
    tokens->list = NULL;
    tokens->used = tokens->size = 0;
    free(tokens);
}

//********************************************************************************************************************//
//* Helper Function initList_Token                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: tokenList * -- List of tokens to append to                                                       *//
//*         initialSize: size_t -- size to initialize list to                                                        *//
//*     Initializes a dynamic TokenList                                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void initList_Token(TokenList *tokens, size_t initialSize) {
    tokens->list = calloc(1, initialSize * sizeof(Token));
    tokens->used = 0;
    tokens->size = initialSize;
    tokens->iter = 0;
}

//********************************************************************************************************************//
//* Helper Function insertList_Token                                                                                 *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens to append to                                                       *//
//*         element: Token * -- element to append                                                                    *//
//*     Appends to TokenList (dynamically)                                                                           *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void insertList_Token(TokenList *tokens, Token *element) {
    if (tokens->used == tokens->size) {
        tokens->size *= 2;
        tokens->list = realloc(tokens->list, tokens->size * sizeof(Token));
    }
    tokens->list[tokens->used++] = element;
}

//********************************************************************************************************************//
//* Helper Function freeList_Token                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens to free                                                            *//
//*     Frees all data used by TokenList                                                                             *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void freeList_Token(TokenList *tokens) {
    for (int i = 0; i < tokens->used; i++) {
        free(tokens->list[i] ->name);
        free(tokens->list[i]);
    }
    free(tokens->list);
    tokens->list = NULL;
    tokens->used = tokens->size = 0;
    free(tokens);
}
