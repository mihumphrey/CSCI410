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
    return asmb;
}

void insertSymbol(struct symbolTable *st, struct symbol symbol) {
    if (st -> used == st -> size) {
        st -> size *= 2;
        st -> symbolList = realloc(st -> symbolList, st -> size * sizeof(struct symbol));
    }
    st -> symbolList[st -> used++] = symbol;
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
                        struct symbol s;
                        s.name = label;
                        s.memAddr = PC;
                        s.mt = ROM;
                        insertSymbol(as -> st, s);
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
