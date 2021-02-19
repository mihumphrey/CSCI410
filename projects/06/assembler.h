//
// Created by Michael on 2/19/2021.
//

#ifndef INC_06_ASSEMBLER_H
#define INC_06_ASSEMBLER_H

#include "definitions.h"

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15

#define SCREEN 16384
#define KBD 24576
#define SP 0
#define LCL 1
#define ARG 2
#define THIS 3
#define THAT 4

#define NONE 000
#define JGT 001
#define JEQ 010
#define JGE 011
#define JLT 100
#define JNE 101
#define JLE 110
#define JMP 111



enum memType {
    PREDEFINED, RAM, ROM
};

struct symbol {
    char *name;
    int memAddr;
    enum memType mt;
};

struct symbolTable {
    struct symbol *symbolList;
    size_t used;
    size_t size;
};

struct assembler {
    struct symbolTable *st;
};



void firstPass(struct assembler *as, FILE *file);
struct assembler *assembler();
void insertSymbol(struct symbolTable *st, struct symbol symbol);


#endif //INC_06_ASSEMBLER_H
