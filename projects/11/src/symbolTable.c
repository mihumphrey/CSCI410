#include "symbolTable.h"
void initList_symbolTable(SymbolTable *tokens, size_t initialSize) {
    tokens->list = calloc(1, initialSize * sizeof(Token));
    tokens->used = 0;
    tokens->size = initialSize;
    tokens->isConstructor = false;
    tokens->isMethod = false;
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
void insertList_symbolTable(SymbolTable *tokens, SymbolEntry *element) {
    element->offset = getNextOffset(tokens, element->segment);
    if (tokens->used == tokens->size) {
        tokens->size *= 2;
        tokens->list = realloc(tokens->list, tokens->size * sizeof(SymbolEntry));
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
void freeList_symbolTable(SymbolTable *tokens) {
    for (int i = 0; i < tokens->used; i++) {
        if (strlen(tokens->list[i]->name) > 0)
        free(tokens->list[i]);
    }
    free(tokens->list);
    tokens->list = NULL;
    tokens->used = tokens->size = 0;
//    free(tokens);
}

/*SymbolEntry *getSymbolEntry(SymbolTable *tokens, char *name) {
    for (int i = 0; i < tokens->used; i++) {
        if (STREQUALS(name, tokens->list[i]->name)) 
            return tokens->list[i];
    }
    return NULL;
}*/

bool contains(SymbolTable *tokens, char *name) {
    for (int i = 0; i < tokens->used; i++) {
        if (STREQUALS(tokens->list[i]->name, name)) 
            return true;
    }
    return false;
}

void clear(SymbolTable *tokens) {
    freeList_symbolTable(tokens);
    initList_symbolTable(tokens, 2);
//    tokens->list = realloc(tokens->list, 4);
//    tokens->used = 0;
//    tokens->size = 4;
}

int getNextOffset(SymbolTable *tokens, Segment segment) {
    int maxCount = -1;
    for (int i = 0; i < tokens->used; i++) {
        if (tokens->list[i]->segment == segment && tokens->list[i]->offset > maxCount)
            maxCount = tokens->list[i]->offset;
    }
    return maxCount + 1;
}

void printTable(SymbolTable *table) {
    for (int i = 0; i < table->used; i++) {
        printf("\t\tNAME: %s", table->list[i]->name);
        printf("\t\tTYPE: %s", table->list[i]->type);
        printf("\t\tSEGMENT: %s", getSymbolSegment(table->list[i]->segment));
        printf("\t\tOFFSET: %d\n", table->list[i]->offset);
    }
}

char *getSymbolSegment(Segment segment) {
    switch(segment) {
    case STATIC :
        return "static";
    case THIS :
        return "this";
    case ARG :
        return "argument";
    case LOCAL : 
        return "local";
    case THAT : 
        return "that";
    case POINTER :
        return "pointer";
    case TEMP :
        return "temp";
    default:
        ASSERT(0 == 1, "invalid memory segment")
    }
    return NULL;
}
