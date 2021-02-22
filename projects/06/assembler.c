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
    asmb -> st -> nextRamAddr = 16;
    
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

    insertSymbol(asmb -> st, createSymbol("SP", 0, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("LCL", 1, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("ARG", 2, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("THIS", 3, PREDEFINED));
    insertSymbol(asmb -> st, createSymbol("THAT", 4, PREDEFINED));

    

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

char *getComp(char *name) {
    ASSERT(name != NULL, "comp cannot be null");
    if (strncmp(name, "0", strlen(name)) == 0) return "101010";
    if (strncmp(name, "1", strlen(name)) == 0) return "111111";
    if (strncmp(name, "-1", strlen(name)) == 0) return "111010";
    if (strncmp(name, "D", strlen(name)) == 0) return "001100";
    if (strncmp(name, "A", strlen(name)) == 0 || strncmp(name, "M", strlen(name)) == 0) return "110000";
    if (strncmp(name, "!D", strlen(name)) == 0) return "001101";
    if (strncmp(name, "!A", strlen(name)) == 0 || strncmp(name, "!M", strlen(name)) == 0) return "110001";
    if (strncmp(name, "-D", strlen(name)) == 0) return "001111";
    if (strncmp(name, "-A", strlen(name)) == 0 || strncmp(name, "-M", strlen(name)) == 0) return "110011";
    if (strncmp(name, "D+1", strlen(name)) == 0) return "011111";
    if (strncmp(name, "A+1", strlen(name)) == 0 || strncmp(name, "M+1", strlen(name)) == 0) return "110111";
    if (strncmp(name, "D-1", strlen(name)) == 0) return "001110";
    if (strncmp(name, "A-1", strlen(name)) == 0 || strncmp(name, "M-1", strlen(name)) == 0) return "110010";
    if (strncmp(name, "D+A", strlen(name)) == 0 || strncmp(name, "D+M", strlen(name)) == 0) return "000010";
    if (strncmp(name, "D-A", strlen(name)) == 0 || strncmp(name, "D-M", strlen(name)) == 0) return "010011";
    if (strncmp(name, "A-D", strlen(name)) == 0 || strncmp(name, "M-D", strlen(name)) == 0) return "000111";
    if (strncmp(name, "D&A", strlen(name)) == 0 || strncmp(name, "D&M", strlen(name)) == 0) return "000000";
    if (strncmp(name, "D|A", strlen(name)) == 0 || strncmp(name, "D|M", strlen(name)) == 0) return "010101";
    ASSERT(1 == 0, "could not find comp");
}

char *getDest(char *name) {
    if (name == NULL) return "000";
    if (strncmp(name, "M", strlen(name)) == 0) return "001";
    if (strncmp(name, "D", strlen(name)) == 0) return "010";
    if (strncmp(name, "MD", strlen(name)) == 0) return "011";
    if (strncmp(name, "A", strlen(name)) == 0) return "100";
    if (strncmp(name, "AM", strlen(name)) == 0) return "101";
    if (strncmp(name, "AD", strlen(name)) == 0) return "110";
    if (strncmp(name, "AMD", strlen(name)) == 0) return "111";
    ASSERT(1 == 0, "could not find dest");
}

char *getJump(char *name) {
    if (name == NULL) return "000";
    if (strncmp(name, "JGT", strlen(name)) == 0) return "001";
    if (strncmp(name, "JEQ", strlen(name)) == 0) return "010";
    if (strncmp(name, "JGE", strlen(name)) == 0) return "011";
    if (strncmp(name, "JLT", strlen(name)) == 0) return "100";
    if (strncmp(name, "JNE", strlen(name)) == 0) return "101";
    if (strncmp(name, "JLE", strlen(name)) == 0) return "110";
    if (strncmp(name, "JMP", strlen(name)) == 0) return "111";
    ASSERT(1 == 0, "could not find jump");
}

void freeAssembler(struct assembler *as) {
    for (int i = 0; i < as -> st -> used; i++) {
        if (as -> st -> symbolList[i].mt != PREDEFINED) {
            free(as -> st -> symbolList[i].name);
        }
    }
    free(as -> st -> symbolList);
    free(as -> st);
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
    for (int i = 0; i < st -> used; i++) {
        if (strncmp(st -> symbolList[i].name, label, strlen(label)) == 0) {
            *memAddr = st -> symbolList[i].memAddr;
            return true;
        }
    }
    return false;
}

char *toBinary(uint16_t input) {
    char *bin = malloc(17);
    int count = 0;
    for (unsigned i = 1 << 15; i > 0; i = i >> 1) {
        if (input &i) 
            bin[count] = '1';
        else bin[count] = '0';
        count++;
    }
    bin[16] = '\0';
    return bin;
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

        bool added;
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] != '(') {
                continue;
            } else {
                added = true;
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
        if (!added) PC++;
        added = false;
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
                break;
            else if (line[i] == '@') {
                for (int j = i + 1; j < strlen(line); j++) {
                    if (line[j] == ' ' || line[j] == '\n' || line[j] == '\0') {
                        size_t len = ((size_t)(j - 1) - (i + 1));
                        char *label = malloc(len +1);
                        strncpy(label, &line[i + 1], len);
                        label[len] = '\0';
                        if (strlen(label) == 1 && label[0] == '0') {
                            char *bin = toBinary(0);
                            fprintf(out, "%s\n", bin);
                            free(bin);
                            free(label);
                            break;
                        }
                        int num;
                        if (num = strtol(label, NULL, 10)) {
                            char *bin = toBinary(num);
                            fprintf(out, "%s\n", bin);
                            free(bin);
                            free(label);
                            break;
                        }
                    
                        uint16_t ramAddr = 0;
                        bool contains = tableContains(as -> st, label, &ramAddr);
                        if (!contains) {
                            ramAddr = as -> st -> nextRamAddr;
                            insertSymbol(as -> st, createSymbol(label, as -> st -> nextRamAddr++, RAM));
                        } else free(label);
                        char *bin = toBinary(ramAddr);                  
                        fprintf(out, "%s\n", bin);
                        free(bin);
                        break;  
                    }
                }
                break;
            } else if (line[i] != ' ' && line[i] != '\t' && line[i] != '\0' && line[i] != '(') {
            
                for (int j = i; j < strlen(line); j++) {
                    if (line[j] == '/' || line[j] == '\n' || line[j] == '\0') {
                        size_t len = ((size_t)(j - 1) - i);
                        char instr[len + 1];
                        strncpy(instr, &line[i], len);
                        instr[len] = '\0';       
                        char a = '0';
                        char *comp;
                        char *dest;
                        char *jump;
                        int ii;
                        bool hasEQ = false;
                        for (ii = 0; ii < strlen(instr); ii++) {
                            if (instr[ii] == '=') {
                                hasEQ = true;
                                jump = getJump(NULL);
                                size_t lenBefore = (size_t)ii;
                                char destASM[lenBefore + 1];
                                strncpy(destASM, instr, lenBefore);
                                destASM[lenBefore] = '\0';
                                dest = getDest(destASM);
                                break;
                            }
                        }
                        if (hasEQ) {
                            for (int k = ii; k < strlen(instr); k++)
                                if (instr[k] == 'M')
                                    a = '1';
                            char compASM[(strlen(instr) - ii) + 1];
                            strncpy(compASM, &instr[ii + 1], strlen(instr) - (ii));
                            compASM[strlen(instr) - (ii)] = '\0';
                            comp = getComp(compASM);
                            if (instr[ii] == 'M')
                                a = '1';
                            jump = getJump(NULL);
                        } else {
                            for (ii = 0; ii < strlen(instr); ii++) {
                                if (instr[ii] == ';') {
                                    break;
                                }
                            }
                            dest = getDest(NULL);
                            char compASM[ii + 1]; 
                            strncpy(compASM, instr, ii);
                            compASM[ii] = '\0';
                            comp = getComp(compASM);

                            char jumpASM[(strlen(instr) - ii) + 1];
                            strncpy(jumpASM, &instr[ii + 1], strlen(instr) - ii);
                            jumpASM[strlen(instr) - ii] = '\0';
                            jump = getJump(jumpASM);
                        }

                        fwrite("111", 3, 1, out);
                        fwrite(&a, 1, 1, out);
                        fwrite(comp, strlen(comp), 1, out);
                        fwrite(dest, strlen(dest), 1, out);
                        fwrite(jump, strlen(jump), 1, out);
                        fwrite("\n", 1, 1, out);
                        break;
                    }   
                }
                
                break;
            }
        } 
    }
    
}
