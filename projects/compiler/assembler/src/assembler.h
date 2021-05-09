//
// Created by Michael on 2/19/2021.
//

#ifndef INC_06_ASSEMBLER_H
#define INC_06_ASSEMBLER_H

// All includes for the program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Macros to make debugging easier
#define ASSERT(expr, err) if (!(expr)){fprintf(stderr, "\033[31m Error, %s, exiting... \033[0m\n", err); exit (1);}
#define MAX_LINE_LENGTH 256

enum memType {
    PREDEFINED, RAM, ROM
};

struct symbol {
    char *name;
    uint16_t memAddr;
    enum memType mt;
};

struct symbolTable {
    struct symbol *symbolList;
    uint16_t nextRamAddr;
    size_t used;
    size_t size;
};

struct assembler {
    struct symbolTable *st;
};


// All functions defined in assembler.c
void firstPass(struct assembler *as, FILE *inputFile);
void secondPass(struct assembler *as, FILE *inputFile, FILE *outputFile);
struct assembler *assembler();
void freeAssembler(struct assembler *as);
void insertSymbol(struct symbolTable *st, struct symbol symbol);
struct symbol symbol(char *name, uint16_t memAddr, enum memType mt);
char *toBinary(uint16_t input);
bool tableContains(struct symbolTable *st, char *label, uint16_t *memAddr);
char *getDest(char *name);
char *getComp(char *name);
char *getJump(char *name);
void handleAInstruction(struct assembler *as, char *line, int i, FILE *outputFile);
void handleCInstr(char *line, int sep, FILE *outputFile);
char *handleDest(char *instr, int sep);
char *handleJump(char *instr, int sep);
char *handleComp(char *instr, int sep, bool doDest, char *a);



#endif //INC_06_ASSEMBLER_H
