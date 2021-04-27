#ifndef ANALYZER_H
#define ANAlYZER_h

#include "grammar.h"
#include "parser.h"

#define CURR_WORD tokens->list[tokens->iter]->name
#define CURR_WORD_EQ(word) STREQUALS(tokens->list[tokens->iter]->name, word)

void compileClass(TokenList *tokens, FILE *outputFile);
void compileClassVarDec(TokenList *tokens, FILE *outputFile);
void compileSubroutineDec(TokenList *tokens, FILE *outputFile);
void compileSubroutineBody(TokenList *tokens, FILE *outputFile);
void compileParameterList(TokenList *tokens, FILE *outputFile);
void compileVarDec(TokenList *tokens, FILE *outputFile);
void compileStatements(TokenList *tokens, FILE *outputFile);
void compileDo(TokenList *tokens, FILE *outputFile);
void compileLet(TokenList *tokens, FILE *outputFile);
void compileWhile(TokenList *tokens, FILE *outputFile);
void compileReturn(TokenList *tokens, FILE *outputFile);
void compileIf(TokenList *tokens, FILE *outputFile);
void compileExpression(TokenList *tokens, FILE *outputFile);
void compileTerm(TokenList *tokens, FILE *outputFile);
void compileSubroutineCall(TokenList *tokens, FILE *outputFile);
void compileExpressionList(TokenList *tokens, FILE *outputFile);

void writeToken(TokenList *tokens, FILE *outputFile);
void writeTag(char *tag, FILE *outputFile);

void advance(TokenList *tokens);
bool currentSymbolEQ(TokenList *tokens, char symbolName);
char currentSymbol(TokenList *tokens);
char *currentTokenWord(TokenList *tokens);
bool currentTokenWordEQ(TokenList *tokens, char *name);

typedef struct __COMPILER__ {
    TokenList *tokens;
    FILE *outputFile;
    SymbolTable *classTable;
    SymbolTable *subroutineTable;
}
#endif
