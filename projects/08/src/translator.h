#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

/*
 * Macros used in main program to increase readability
 */
// Assertion to simplify exiting gracefully
#define ASSERT(arg, err) if (!(arg)) {fprintf(stderr,"\033[31mError, %s, exiting...\033[0m\n", err); exit(1);} 
// String equality to simplify comparing strings 
#define STREQUALS(arg1, arg2) strncmp((arg1), (arg2), strlen((arg1))) == 0
// Write to write assembly to the output file 
#define WRITE(...) fprintf(outputFile, __VA_ARGS__);

// Definition to declare that this will not handle a line linger than 256 chars
#define MAX_LINE_LENGTH 256
#define MAX_COMMAND_LENGTH 3
#define MAX_FILES_IN_DIR 64

// All functions used in this program
void parseCommands(FILE *inputFile, FILE *outputFile, int *labelNum, int *ra);
void parseCommand(char *instr, FILE *outputFile, int *labelNum, char *currFunc, int *ra);
void doPush(char *command[MAX_COMMAND_LENGTH], FILE *outputFile);
void doPop(char *command[MAX_COMMAND_LENGTH], FILE *outputFile);
void doArithmetic(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, int *labelNum);
void doLabel(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc);
void doIfGoto(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc);
void doGoto(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc);
void doFunct(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc);
void doCall(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc, int *ra);
void doReturn(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc, int *ra);
char *toASM(char *loc);
char *getOffset(char *segment, char *offset);

#endif
