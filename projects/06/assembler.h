//
// Created by Michael on 2/19/2021.
//

#ifndef INC_06_ASSEMBLER_H
#define INC_06_ASSEMBLER_H

#include "definitions.h"

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
char *getDest(char *name);
char *getComp(char *name);
char *getJump(char *name);
void handleCInstr(char *line, int sep, FILE *out);
char *handleDest(char *instr, int sep);
char *handleJump(char *instr, int sep);
char *handleComp(char *instr, int sep, bool doDest, char *a);



#endif //INC_06_ASSEMBLER_H
