#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#define ASSERT(arg, err) if (!(arg)) {fprintf(stderr,"\033[31mError, %s, exiting...\033[0m\n", err); exit(1);} 
#define STREQUALS(arg1, arg2) strncmp((arg1), (arg2), strlen((arg1))) == 0
#define WRITE(...) fprintf(outputFile, __VA_ARGS__);
#define MAX_LINE_LENGTH 256
#define MAX_COMMAND_LENGTH 3


void parseCommands(FILE *inputFile, FILE *outputFile, int *labelNum);
void parseCommand(char *instr, FILE *outputFile, int *labelNum);
void doPush(char *command[MAX_COMMAND_LENGTH], FILE *outputFile);
void doPop(char *command[MAX_COMMAND_LENGTH], FILE *outputFile);
void doArithmetic(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, int *labelNum);

#endif
