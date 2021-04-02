#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "includes.h"
#include "grammar.h"
#include "parser.h"

TokenList *getTokens(char *input);
void writeTokens(TokenList *tokens, FILE *outputFile);


#endif
