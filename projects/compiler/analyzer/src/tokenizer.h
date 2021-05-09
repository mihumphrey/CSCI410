#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "includes.h"
#include "grammar.h"
#include "parser.h"

TokenList *getTokens(char *input);
void writeTokens(TokenList *tokens, FILE *outputFile);

void initList_char(CharList *tokens, size_t initialSize);
void insertList_char(CharList *tokens, char element);
void freeList_char(CharList *tokens);
void initList_Token(TokenList *tokens, size_t initialSize);
void insertList_Token(TokenList *tokens, Token *element);
void freeList_Token(TokenList *tokens);



#endif
