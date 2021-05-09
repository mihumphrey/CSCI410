#include "assembler.h"

extern bool verbose;

//********************************************************************************************************************//
//* Constructor assembler                                                                                            *//
//*     Input:                                                                                                       *//
//*         NONE                                                                                                     *//
//*     Creates an assembler object                                                                                  *//
//*     Loads the object with all predefined values in the symbol table                                              *//
//*     Returns:                                                                                                     *//
//*         assembler object                                                                                         *//
//********************************************************************************************************************//
struct assembler *assembler() {
    struct assembler *assemble = malloc(sizeof(struct assembler));
    assemble -> st = malloc(sizeof(struct symbolTable));
    assemble -> st -> symbolList = malloc(4 * sizeof(struct symbol));
    assemble -> st -> size = 4;
    assemble -> st -> used = 0;
    assemble -> st -> nextRamAddr = 16;
    
    insertSymbol(assemble -> st, symbol("R0", 0, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R1", 1, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R2", 2, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R3", 3, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R4", 4, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R5", 5, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R6", 6, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R7", 7, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R8", 8, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R9", 9, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R10", 10, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R11", 11, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R12", 12, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R13", 13, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R14", 14, PREDEFINED));
    insertSymbol(assemble -> st, symbol("R15", 15, PREDEFINED));

    insertSymbol(assemble -> st, symbol("SP", 0, PREDEFINED));
    insertSymbol(assemble -> st, symbol("LCL", 1, PREDEFINED));
    insertSymbol(assemble -> st, symbol("ARG", 2, PREDEFINED));
    insertSymbol(assemble -> st, symbol("THIS", 3, PREDEFINED));
    insertSymbol(assemble -> st, symbol("THAT", 4, PREDEFINED));

    insertSymbol(assemble -> st, symbol("SCREEN", 16384, PREDEFINED));
    insertSymbol(assemble -> st, symbol("KBD", 24576, PREDEFINED));
    return assemble;
}

//********************************************************************************************************************//
//* Constructor symbol                                                                                               *//
//*     Input:                                                                                                       *//
//*         name: char *                                                                                             *//
//*         memAddr: int                                                                                             *//
//*         mt: enum memType                                                                                         *//
//*     Creates a symbol object                                                                                      *//
//*     Loads the object with the values in the parameters                                                           *//
//*     Returns:                                                                                                     *//
//*         symbol object                                                                                            *//
//********************************************************************************************************************//
struct symbol symbol(char *name, uint16_t memAddr, enum memType mt) {
    struct symbol s;
    s.name = name;
    s.memAddr = memAddr;
    s.mt = mt;

    return s;
}

//********************************************************************************************************************//
//* Function getComp                                                                                                 *//
//*     Input:                                                                                                       *//
//*         name: char *                                                                                             *//
//*     Compares the input to all possible inputs                                                                    *//
//*     Returns:                                                                                                     *//
//*         6 character char * representing the output mapping                                                       *//
//********************************************************************************************************************//
char *getComp(char *name) {
    ASSERT(name, "comp cannot be null")
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
    printf("COMP: %s\n", name);
    ASSERT(1 == 0, "could not find comp")
}

//********************************************************************************************************************//
//* Function getDest                                                                                                 *//
//*     Input:                                                                                                       *//
//*         name: char *                                                                                             *//
//*     Compares the input to all possible inputs                                                                    *//
//*     Returns:                                                                                                     *//
//*         3 character char * representing the output mapping                                                       *//
//********************************************************************************************************************//
char *getDest(char *name) {
    if (name == NULL) return "000";
    if (strncmp(name, "M", strlen(name)) == 0) return "001";
    if (strncmp(name, "D", strlen(name)) == 0) return "010";
    if (strncmp(name, "MD", strlen(name)) == 0) return "011";
    if (strncmp(name, "A", strlen(name)) == 0) return "100";
    if (strncmp(name, "AM", strlen(name)) == 0) return "101";
    if (strncmp(name, "AD", strlen(name)) == 0) return "110";
    if (strncmp(name, "AMD", strlen(name)) == 0) return "111";
    ASSERT(1 == 0, "could not find dest")
}

//********************************************************************************************************************//
//* Function getJump                                                                                                 *//
//*     Input:                                                                                                       *//
//*         name: char *                                                                                             *//
//*     Compares the input to all possible inputs                                                                    *//
//*     Returns:                                                                                                     *//
//*         3 character char * representing the output mapping                                                       *//
//********************************************************************************************************************//
char *getJump(char *name) {
    if (name == NULL) return "000";
    if (strncmp(name, "JGT", strlen(name)) == 0) return "001";
    if (strncmp(name, "JEQ", strlen(name)) == 0) return "010";
    if (strncmp(name, "JGE", strlen(name)) == 0) return "011";
    if (strncmp(name, "JLT", strlen(name)) == 0) return "100";
    if (strncmp(name, "JNE", strlen(name)) == 0) return "101";
    if (strncmp(name, "JLE", strlen(name)) == 0) return "110";
    if (strncmp(name, "JMP", strlen(name)) == 0) return "111";
    ASSERT(1 == 0, "could not find jump")
}

//********************************************************************************************************************//
//* Function freeAssembler                                                                                           *//
//*     Input:                                                                                                       *//
//*         as: struct assembler                                                                                     *//
//*     Frees all memory used by the assembler object                                                                *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
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

//********************************************************************************************************************//
//* Function insertSymbol                                                                                            *//
//*     Input:                                                                                                       *//
//*         st: struct symbolTable                                                                                   *//
//*         symbol: struct symbol                                                                                    *//
//*     Dynamically inserts a symbol into st                                                                         *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void insertSymbol(struct symbolTable *st, struct symbol symbol) {
    if (st -> used == st -> size) {
        st -> size *= 2;
        st -> symbolList = realloc(st -> symbolList, st -> size * sizeof(struct symbol));
    }
    st -> symbolList[st -> used++] = symbol;
}

//********************************************************************************************************************//
//* Function tableContains                                                                                           *//
//*     Input:                                                                                                       *//
//*         st: struct symbolTable                                                                                   *//
//*         label: char *                                                                                            *//
//*         memAddr: uint16_t                                                                                        *//
//*     Checks if st contains label, and sets memAddr to label's corresponding value if it does                      *//
//*     Returns:                                                                                                     *//
//*         Boolean found or not                                                                                     *//
//********************************************************************************************************************//
bool tableContains(struct symbolTable *st, char *label, uint16_t *memAddr) {
    for (int i = 0; i < st -> used; i++) {
        if (strncmp(st -> symbolList[i].name, label, strlen(label)) == 0) {
            *memAddr = st -> symbolList[i].memAddr;
            return true;
        }
    }
    return false;
}

//********************************************************************************************************************//
//* Function toBinary                                                                                                *//
//*     Input:                                                                                                       *//
//*         input: uint16_t                                                                                          *//
//*     Converts numerical input to string binary output                                                             *//
//*     Returns:                                                                                                     *//
//*         char * binary version of input                                                                           *//
//********************************************************************************************************************//
char *toBinary(uint16_t input) {

    char *bin = malloc(17);
    int count = 0;
    for (unsigned i = (1u << 15u); i > 0; i = (i >> 1u)) {
        if (input &i) 
            bin[count] = '1';
        else bin[count] = '0';
        count++;
    }
    bin[16] = '\0';
    return bin;
}

//********************************************************************************************************************//
//* Function firstPass                                                                                               *//
//*     Input:                                                                                                       *//
//*         as: struct assembler                                                                                     *//
//*         inputFile: FILE *                                                                                        *//
//*         memAddr: uint16_t                                                                                        *//
//*     Conducts the first pass of assembly, storing all L-instructions in the symbol table                          *//
//*         Reads a line of the program                                                                              *//
//*             If line is blank or comment, skip it                                                                 *//
//*             Reads until first non-whitespace character                                                           *//
//*                 If this character begins with '(', it is an L instruction                                        *//
//*                     Store the value in the symbolTable                                                           *//
//*                 Else skip the line                                                                               *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void firstPass(struct assembler *as, FILE *inputFile) {
    ASSERT(inputFile != NULL, "input inputFile not open in first pass")

    char line[MAX_LINE_LENGTH];
    int PC = 0;

    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

        bool added = false;
        int i;
        for (i = 0; i < strlen(line); i++) {
            if (line[i] == '/')
                break;
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
                        if (verbose)
                            printf("\t* Inserting symbol: %s with PC: %d\n", label, PC);
                        insertSymbol(as -> st, symbol(label, PC, ROM));
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

//********************************************************************************************************************//
//* Function secondPass                                                                                              *//
//*     Input:                                                                                                       *//
//*         as: struct assembler                                                                                     *//
//*         inputFile: FILE *                                                                                        *//
//*         outputFile: FILE *                                                                                       *//
//*         memAddr: uint16_t                                                                                        *//
//*     Conducts the second pass of assembly, converting all A and C instructions into binary                        *//
//*         Reads a line of the program                                                                              *//
//*             If line is blank or comment, skip it                                                                 *//
//*             Reads until first non-whitespace character                                                           *//
//*                 If this character begins with '(', skip it                                                       *//
//*                 If this character is a '@'                                                                       *//
//*                     Call handleAInstruction                                                                      *//
//*                 If this character is anything else                                                               *//
//*                     Call handleCInstruction                                                                      *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void secondPass(struct assembler *as, FILE *inputFile, FILE *outputFile) {
    ASSERT(inputFile, "input file not open in second pass")
    rewind(inputFile);
    ASSERT(outputFile, "input file not open in second pass")
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '(')
                break;
            else if (line[i] == '@') {
                if (verbose)
                    printf("\t* Found A-Instruction: %s", line);
                handleAInstruction(as, line, i, outputFile);
                break;
            } else if (line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
                if (verbose)
                    printf("\t* Found C-Instruction: %s", line);
                handleCInstr(line, i, outputFile);
                break;
            }
        } 
    }
}

//********************************************************************************************************************//
//* Function handleAInstruction                                                                                      *//
//*     Input:                                                                                                       *//
//*         as: struct assembler                                                                                     *//
//*         line: char *                                                                                             *//
//*         outputFile: FILE *                                                                                       *//
//*     Parses line for the label of the A instruction                                                               *//
//*     Checks if this label is already in the assembler                                                             *//
//*         If so, retrieve its value, then write its binary version to outputFile                                   *//
//*         If not, insert it into the assembler, then write its new value (binary) to outputFile                    *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void handleAInstruction(struct assembler *as, char *line, int i, FILE *outputFile) {
    for (int j = i + 1; j < strlen(line); j++) {
        if (line[j] == ' ' || line[j] == '\n' || line[j] == '\0' || line[j] == '/' || line[i] == '\t') {
            size_t len = ((size_t)(j - 1) - (i + 1));
            char *label = malloc(len +1);
            strncpy(label, &line[i + 1], len);
            label[len] = '\0';
            if (strlen(label) == 1 && label[0] == '0') {
                char *bin = toBinary(0);
                if (verbose)
                    printf("\t\t* Writing instruction to outputFile\n");
                fprintf(outputFile, "%s\n", bin);
                free(bin);
                free(label);
                break;
            }
            if (verbose)
                printf("\t\t* Checking if label is a constant\n");
            uint16_t num = (uint16_t)strtol(label, NULL, 10);
            if (num != 0L) {
                char *bin = toBinary(num);
                if (verbose)
                    printf("\t\t* Writing instruction to outputFile\n");
                fprintf(outputFile, "%s\n", bin);
                free(bin);
                free(label);
                break;
            }

            uint16_t ramAddr = 0;
            if (verbose)
                printf("\t\t* Checking if symbol table contains symbol: %s\n", label);
            bool contains = tableContains(as -> st, label, &ramAddr);
            if (!contains) {
                ramAddr = as -> st -> nextRamAddr;
                if (verbose)
                    printf("\t\t* Inserting symbol: %s\n", label);
                insertSymbol(as -> st, symbol(label, as->st->nextRamAddr++, RAM));
            } else free(label);
            if (verbose)
                printf("\t\t* Converting instruction to binary\n");
            char *bin = toBinary(ramAddr);
            if (verbose)
                printf("\t\t* Writing instruction to outputFile\n");
            fprintf(outputFile, "%s\n", bin);
            free(bin);
            break;
        }
    }
}

//********************************************************************************************************************//
//* Function handleCInstruction                                                                                      *//
//*     Input:                                                                                                       *//
//*         line: char *                                                                                             *//
//*         start: int                                                                                               *//
//*         outputFile: FILE *                                                                                       *//
//*     Parses line for the dest, comp, a, and jump fields                                                           *//
//*         If line contains '=', use dest and comp; jump is null                                                    *//
//*         If line contains ';', use jump and comp; dest is null                                                    *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void handleCInstr(char *line, int start, FILE *outputFile) {
    int i;
    char a = '0';
    char *dest;
    char *comp;
    char *jump;

    for (i = start; i < strlen(line); i++) {
        if (line[i] == ' ' || line[i] == '/' || line[i] == '\0' || line[i] == '\n' || line[i] == '\r')
            break;
    }

    char instr[(i - start) + 1];
    strncpy(instr, &line[start], (i) - start);
    instr[(i - start)] = '\0';

    bool doDest = false;
    int sepLoc = -1;
    if (verbose)
        printf("\t\t* Locating separator (= or ;)\n");
    for (i = 0; i < strlen(instr); i++) {
        if (instr[i] == '=') {
            sepLoc = i;
            doDest = true;
            break;
        }
        if (instr[i] == ';') {
            sepLoc = i;
            break;
        }
    }

    ASSERT(sepLoc != -1, "separator (';', '=') not found")

    if (doDest) {
        if (verbose)
            printf("\t\t* Instruction found to be a dest=comp instruction\n");
        dest = handleDest(instr, sepLoc);
        jump = getJump(NULL);
    } else {
        if (verbose)
            printf("\t\t* Instruction found to be a comp;jump instruction\n");
        if (verbose)
            printf("\t\t* Handling jump instruction\n");
        jump = handleJump(instr, sepLoc);
        if (verbose)
            printf("\t\t* Handling dest instruction\n");
        dest = getDest(NULL);
    }
    if (verbose)
        printf("\t\t* Handling comp instruction\n");
    comp = handleComp(instr, sepLoc, doDest, &a);

    if (verbose)
        printf("\t\t* Writing instruction to outputFile\n");
    fprintf(outputFile, "111");
    fprintf(outputFile, "%c", a);
    fprintf(outputFile, "%s", comp);
    fprintf(outputFile, "%s", dest);
    fprintf(outputFile, "%s\n", jump);

}

//********************************************************************************************************************//
//* Function handleDest                                                                                              *//
//*     Input:                                                                                                       *//
//*         instr: char *                                                                                            *//
//*         sep: int                                                                                                 *//
//*     Parses the instruction for the dest field                                                                    *//
//*     Returns:                                                                                                     *//
//*         char * representing the dest ASM field (binary)                                                          *//
//********************************************************************************************************************//
char *handleDest(char *instr, int sep) {
    char before[sep + 1];
    strncpy(before, instr, sep);
    before[sep] = '\0';

    if (verbose)
        printf("\t\t\t* Obtaining dest bits\n");
    char *dest = getDest(before);
    return dest;
}

//********************************************************************************************************************//
//* Function handleJump                                                                                              *//
//*     Input:                                                                                                       *//
//*         instr: char *                                                                                            *//
//*         sep: int                                                                                                 *//
//*     Parses the instruction for the jump field                                                                    *//
//*     Returns:                                                                                                     *//
//*         char * representing the jump ASM field (binary)                                                          *//
//********************************************************************************************************************//
char *handleJump(char *instr, int sep) {
    char before[((strlen(instr))- (sep + 1)) + 1];
    strncpy(before, &instr[sep + 1], (strlen(instr)) - (sep + 1));
    before[(strlen(instr)) - (sep + 1)] = '\0';

    if (verbose)
        printf("\t\t\t* Obtaining jump bits\n");
    char *jump = getJump(before);
    return jump;
}

//********************************************************************************************************************//
//* Function handleComp                                                                                              *//
//*     Input:                                                                                                       *//
//*         instr: char *                                                                                            *//
//*         sep: int                                                                                                 *//
//*         doDest: bool                                                                                             *//
//*         a: char * (pointer)                                                                                      *//
//*     Parses the instruction for the comp field                                                                    *//
//*         Uses doDest to determine whether the comp field is before or after the separator                         *//
//*     Returns:                                                                                                     *//
//*         char * representing the comp ASM field (binary)                                                          *//
//********************************************************************************************************************//
char *handleComp(char *instr, int sep, bool doDest, char *a) {
    if (doDest) {
        char before[((strlen(instr)) - (sep + 1)) + 1];
        strncpy(before, &instr[sep + 1], (strlen(instr)) - (sep + 1));
        before[(strlen(instr)) - (sep + 1)] = '\0';

        for (int i = 0; i < strlen(before); i++) {
            if (before[i] == 'M')
                *a = '1';
        }

        if (verbose)
            printf("\t\t\t* Obtaining comp bits\n");
        char *comp = getComp(before);
        return comp;

    } else {
        char before[sep + 1];
        strncpy(before, instr, sep);
        before[sep] = '\0';
        for (int i = 0; i < strlen(before); i++)
            if (before[i] == 'M')
                *a = '1';


        if (verbose)
            printf("\t\t\t* Obtaining comp bits\n");
        char *comp = getComp(before);
        return comp;
    }
}
