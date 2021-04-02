#ifndef PARSER_H
#define PARSER_H

#include "includes.h"
#include "grammar.h"

char *parse(FILE *inputFile, FILE *outputFile);

void initList_char(CharList *tokens, size_t initialSize);
void insertList_char(CharList *tokens, char element);
void freeList_char(CharList *tokens);
void initList_Token(TokenList *tokens, size_t initialSize);
void insertList_Token(TokenList *tokens, Token *element);
void freeList_Token(TokenList *tokens);

#endif
