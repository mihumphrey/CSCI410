#ifndef ANALYZER_H
#define ANALYZER_H

#include "grammar.h"
#include "parser.h"
#include "symbolTable.h"

typedef struct __COMPILER__ {
    TokenList *tokens;
    FILE *outputFile;
    SymbolTable *classTable;
    SymbolTable *subroutineTable;
    char *className;
    char *subroutineName;
    int labelNum;
    int numExpressions;
} Compiler;

void compileClass(Compiler *compiler);
void compileClassVarDec(Compiler *compiler);
void compileSubroutineDec(Compiler *compiler);
void compileSubroutineBody(Compiler *compiler);
void compileParameterList(Compiler *compiler);
void compileVarDec(Compiler *compiler);
void compileStatements(Compiler *compiler);
void compileDo(Compiler *compiler);
void compileLet(Compiler *compiler);
void compileWhile(Compiler *compiler);
void compileReturn(Compiler *compiler);
void compileIf(Compiler *compiler);
void compileExpression(Compiler *compiler);
void compileTerm(Compiler *compiler);
void compileSubroutineCall(Compiler *compiler);
void compileExpressionList(Compiler *compiler);

void advance(Compiler *compiler);
bool currentSymbolEQ(Compiler *compiler, char symbolName);
char currentSymbol(Compiler *compiler);
char *currentTokenWord(Compiler *compiler);
bool currentTokenWordEQ(Compiler *compiler, char *name);


SymbolEntry *getEntry(Compiler *compiler, char *name);
#endif
