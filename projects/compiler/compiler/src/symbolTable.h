#include "includes.h"

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum __SEGMENT__ {
    STATIC, THIS, THAT, LOCAL, POINTER, ARG, TEMP    
} Segment;

typedef struct __SYMBOL_ENTRY__ {
    char *name;
    char *type;
    Segment segment;
    int offset;    
} SymbolEntry;

typedef struct __SYMBOL_TABLE__ {
    SymbolEntry **list;
    size_t used;
    size_t size;
    bool isConstructor;
    bool isMethod;
} SymbolTable;


void initList_symbolTable(SymbolTable *tokens, size_t initialSize);
void insertList_symbolTable(SymbolTable *tokens, SymbolEntry *element);
void freeList_symbolTable(SymbolTable *tokens);
SymbolEntry *getSymbolEntry(SymbolTable *tokens, char *name);
bool contains(SymbolTable *tokens, char *name);
void clear(SymbolTable *tokens);
int getNextOffset(SymbolTable *tokens, Segment segment);

void printTable(SymbolTable *table);
char *getSymbolSegment(Segment segment);
#endif
