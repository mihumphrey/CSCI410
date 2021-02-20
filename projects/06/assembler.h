//
// Created by Michael on 2/19/2021.
//

#ifndef INC_06_ASSEMBLER_H
#define INC_06_ASSEMBLER_H

#include "definitions.h"

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



void firstPass(struct assembler *as, FILE *file);
void secondPass(struct assembler *as, FILE *file, FILE *out);
struct assembler *assembler();
void freeAssembler(struct assembler *as);
void insertSymbol(struct symbolTable *st, struct symbol symbol);
struct symbol createSymbol(char *name, int memAddr, enum memType mt);
char *toBinary(uint16_t input);
bool tableContains(struct symbolTable *st, char *label, uint16_t *memAddr);


#endif //INC_06_ASSEMBLER_H
