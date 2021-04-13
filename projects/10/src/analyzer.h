#ifndef ANALYZER_H
#define ANAlYZER_h

#include "grammar.h"
#include "parser.h"

#define CURR_LETTER tokens->list[tokens->iter]->name[0]
#define CURR_LETTER_EQ(letter) tokens->list[tokens->iter]->name[0] == letter ? true : false
#define CURR_WORD tokens->list[tokens->iter]->name
#define CURR_WORD_EQ(word) STREQUALS(tokens->list[tokens->iter]->name, word)

void analyzeClass(TokenList *tokens, FILE *outputFile);
void analyzeClassVarDec(TokenList *tokens, FILE *outputFile, int indent);
void analyzeSubroutineDec(TokenList *tokens, FILE *outputFile, int indent);
void analyzeSubroutineBody(TokenList *tokens, FILE *outputFile, int indent);
void analyzeParameterList(TokenList *tokens, FILE *outputFile, int indent);
void analyzeVarDec(TokenList *tokens, FILE *outputFile, int indent);
void analyzeStatements(TokenList *tokens, FILE *outputFile, int indent);
void analyzeDo(TokenList *tokens, FILE *outputFile, int indent);
void analyzeLet(TokenList *tokens, FILE *outputFile, int indent);
void analyzeWhile(TokenList *tokens, FILE *outputFile, int indent);
void analyzeReturn(TokenList *tokens, FILE *outputFile, int indent);
void analyzeIf(TokenList *tokens, FILE *outputFile, int indent);
void analyzeExpression(TokenList *tokens, FILE *outputFile, int indent);
void analyzeTerm(TokenList *tokens, FILE *outputFile, int indent);
void analyzeSubroutineCall(TokenList *tokens, FILE *outputFile, int indent);
void analyzeExpressionList(TokenList *tokens, FILE *outputFile, int indent);

void writeToken(TokenList *tokens, FILE *outputFile, int indent);
void writeTag(char *tag, FILE *outputFile, int indent);
void advance(TokenList *tokens);
#endif
