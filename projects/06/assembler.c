#include "assembler.h"

//
// Created by Michael on 2/19/2021.
//
struct assembler *assembler() {
    struct assembler *asmb = malloc(sizeof(struct assembler));
    asmb -> st = malloc(sizeof(struct symbolTable));
    asmb -> st -> symbolList = malloc(4 * sizeof(struct symbol));
    asmb -> st -> size = 4;
    asmb -> st -> used = 0;
    asmb -> st -> nextRamAddr = 0;
    
    insertSymbol(asmb -> st, createSymbol("R0", 0, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R1", 1, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R2", 2, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R3", 3, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R4", 4, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R5", 5, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R6", 6, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R7", 7, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R8", 8, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R9", 9, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R10", 10, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R11", 11, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R12", 12, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R13", 13, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R14", 14, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("R15", 15, PREDEFINED));

    insertSymbol(asmb -> st, createSymbol("SCREEN", 16384, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("KBD", 24576, PREDEFINED));
    return asmb;
}


struct symbol createSymbol(char *name, int memAddr, enum memType mt) {
    struct symbol s;
    s.name = name;
    s.memAddr = memAddr;
    s.mt = mt;

    return s;
}

void freeAssembler(struct assembler *as) {
   for (int i = 0; i < as -> st -> used; i++) {
        free(as -> st -> symbolList[i].name);
    } 
    free(as -> st -> symbolList);
    free (as -> st);
    free(as);
}

void insertSymbol(struct symbolTable *st, struct symbol symbol) {
    if (st -> used == st -> size) {
        st -> size *= 2;
        st -> symbolList = realloc(st -> symbolList, st -> size * sizeof(struct symbol));
    }
    st -> symbolList[st -> used++] = symbol;
}

bool tableContains(struct symbolTable *st, char *label, uint16_t *memAddr) {
    printf("FINDING: %s\n\n", label);
    for (int i = 0; i < st -> used; i++) {
        printf("NAME: %s\n", st -> symbolList[i].name);
        if (strncmp(st -> symbolList[i].name, label, strlen(label)) == 0) {
            printf("\nVAL IN: %u\n\n", st -> symbolList[i].memAddr);
            return true;
        }
    }
    return false;
}

char *toBinary(uint16_t input) {
    printf("VALUE: %u\n\n", input);
    for (unsigned i = 1 << 15; i > 0; i = i >> 1)
        (input & i) ? printf("1") : printf("0");
    printf("\n");
    return "";
}

void firstPass(struct assembler *as, FILE *file) {
    ASSERT(file != NULL, "file not open");

    char line[MAX_LINE_LENGTH];
    int PC = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] != '(')
                continue;
            else {
                for (int j = i + 1; j < strlen(line); j++) {
                    if (line[j] == ')') {
                        size_t len = ((size_t)j - (i + 1));
                        char *label = malloc(len + 1);
                        strncpy(label, &line[i + 1], len);
                        label[len] = '\0';
                        insertSymbol(as -> st, createSymbol(label, PC, ROM));
                        break;
                    }
                }
                break;
            }
        }
        printf("%s", line);
        PC++;
    }
}

void secondPass(struct assembler *as, FILE *file, FILE *out) {
    ASSERT(file, "file not open");
    rewind(file);
    ASSERT(file, "file not open");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '(')
                continue;
            if (line[i] == '@') {
                //Processing L-Type Command
                for (int j = i + 1; j < strlen(line); j++) {
                    if (line[j] == ' ' || line[j] == '\n' || line[j] == '\0') {
                        size_t len = ((size_t)j - (i + 1));
                        char *label = malloc(len + 1);
                        strncpy(label, &line[i + 1], len);
                        label[len] = '\0';
                        uint16_t ramAddr;
                        if (tableContains(as -> st, label, &ramAddr)) {
                            printf("FOUND");
                            char * bin = toBinary(ramAddr);                  
                        } else {
                            insertSymbol(as -> st, createSymbol(label, as -> st -> nextRamAddr++, RAM));
                            
                            char * bin = toBinary(ramAddr);                  
                        }
                        break;  
                    }
                }
                break;
            }
        } 
    }
    
}
