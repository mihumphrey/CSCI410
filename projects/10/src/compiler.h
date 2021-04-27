#ifndef ANALYZER_H
#define ANAlYZER_h

#include "grammar.h"
#include "parser.h"

#define CURR_WORD tokens->list[tokens->iter]->name
#define CURR_WORD_EQ(word) STREQUALS(tokens->list[tokens->iter]->name, word)

void compileClass(TokenList *tokens, FILE *outputFile);
void compileClassVarDec(TokenList *tokens, FILE *outputFile, int indent);
void compileSubroutineDec(TokenList *tokens, FILE *outputFile, int indent);
void compileSubroutineBody(TokenList *tokens, FILE *outputFile, int indent);
void compileParameterList(TokenList *tokens, FILE *outputFile, int indent);
void compileVarDec(TokenList *tokens, FILE *outputFile, int indent);
void compileStatements(TokenList *tokens, FILE *outputFile, int indent);
void compileDo(TokenList *tokens, FILE *outputFile, int indent);
void compileLet(TokenList *tokens, FILE *outputFile, int indent);
void compileWhile(TokenList *tokens, FILE *outputFile, int indent);
void compileReturn(TokenList *tokens, FILE *outputFile, int indent);
void compileIf(TokenList *tokens, FILE *outputFile, int indent);
void compileExpression(TokenList *tokens, FILE *outputFile, int indent);
void compileTerm(TokenList *tokens, FILE *outputFile, int indent);
void compileSubroutineCall(TokenList *tokens, FILE *outputFile, int indent);
void compileExpressionList(TokenList *tokens, FILE *outputFile, int indent);

void writeToken(TokenList *tokens, FILE *outputFile, int indent);
void writeTag(char *tag, FILE *outputFile, int indent);
void advance(TokenList *tokens);
bool currentSymbolEQ(TokenList *tokens, char symbolName);
char currentSymbol(TokenList *tokens);
char *currentTokenWord(TokenList *tokens);
bool currentTokenWordEQ(TokenList *tokens, char *name);

typedef struct __COMPILER__ {
    FILE *outputFile;
    TokenList *tokens;
    
}

#endif
