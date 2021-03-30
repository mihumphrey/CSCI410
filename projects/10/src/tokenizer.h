#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "includes.h"
#include "grammar.h"

char **parse(FILE *inputFile, FILE *outputFile);
Token *getTokens(FILE *inputfile);
/*
typedef struct __VECTOR__ {
    char **vector;
    size_t used;
    size_t size;
} Vector;

void initVector(Vector *vector, size_t initialSize) {
    vector->vector = malloc(initialSize * MAX_LINE_LENGTH);
    for (int i = 0; i < initialSize; i++) {
        vector->vector = malloc(MAX_LINE_LENGTH);
    }
    vector->used = 0;
    vector->size = initialSize;
}

void insertVector(Vector *vector, char *element) {
    if (vector->used == vector->size) {
        vector->size *= 2;
        vector->vector = realloc(vector->vector, vector->size * MAX_LINE_LENGTH);
    }
    vector->vector[vector->used++] = element;
}

void freeVector(Vector *vector) {
    free(vector->vector);
   // vector->vector = NULL;
    vector->used = vector->size = 0;
}


*/
#endif
