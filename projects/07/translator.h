#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#define ASSERT(arg, err) if (!(arg)) {fprintf(stderr,"\033[31mError, %s, exiting...\033[0m\n", err); exit(1);} 
#define MAX_LINE_LENGTH 256


struct StackNode {
    int data;
    struct StackNode *next;
    
};

struct Translator {
    struct StackNode *Stack;
    int *stat;
    
};

struct StackNode* newNode(int data);
bool isEmpty(struct StackNode *root);
void push(struct StackNode** root, int data);
int pop(struct StackNode** root); 
int peek(struct StackNode* root);
void print(struct StackNode *root);

void parseCommands(struct StackNode **root, FILE *inputFile, FILE *outputFile);
void handleStackOp(struct StackNode **root, char *instr, FILE *outputFile);
void handleFirstToken(char *token);

#endif
