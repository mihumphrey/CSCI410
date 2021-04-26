#include "symbolTable.h"
//********************************************************************************************************************//
//* Helper Function initList_char                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: CharList * -- List of tokens to append to                                                        *//
//*         initialSize: size_t -- size to initialize list to                                                        *//
//*     Initializes a dynamic string                                                                                 *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void initList_symbolTable(SymbolTable *tokens, size_t initialSize) {
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
void insertList_symbolTable(SymbolTable *tokens, SymbolEntry element) {
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
void freeList_symbolTable(SymbolTable *tokens) {
    free(tokens->list);
    tokens->list = NULL;
    tokens->used = tokens->size = 0;
    free(tokens);
}

SymbolEntry *get(SymbolTable *tokens, char *name) {
    for (int i = 0; i < tokens->used; i++) {
        if (STREQUALS(tokens->list[i].name, name)) 
            return &tokens->list[i];
    }
    return NULL;
}

bool contains(SymbolTable *tokens, char *name) {
    for (int i = 0; i < tokens->used; i++) {
        if (STREQUALS(tokens->list[i].name, name)) 
            return true;
    }
    return false;
}

void clear(SymbolTable *tokens) {
    tokens->list = realloc(tokens->list, 4);
    tokens->used = 0;
    tokens->size = 4;
}
