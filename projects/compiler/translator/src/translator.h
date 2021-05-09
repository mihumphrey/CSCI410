
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "includes.h"

typedef struct Translator {
    FILE *inputFile;
    FILE *outputFile;
    int labelNum;
    int ra;
    char *currClass;
    char *currCommand[MAX_COMMAND_LENGTH];
    char *currFunc;
    char *line;
} Translator;

// All functions used in this program
void doPush(Translator *translator);
void doPop(Translator *translator);
void doArithmetic(Translator *translator);
void doLabel(Translator *translator);
void doIfGoto(Translator *translator);
void doGoto(Translator *translator);
void doFunc(Translator *translator);
void doCall(Translator *translator);
void doReturn(Translator *translator);
char *toASM(char *loc);
void getOffset(char *segment, char *offset);

#endif
