#include "translator.h"
char *toASM(char *loc) {
    if (STREQUALS(loc, "local"))
        return "LCL";
    if (STREQUALS(loc, "this"))
        return "THIS";
    if (STREQUALS(loc, "that"))
        return "THAT";
    if (STREQUALS(loc, "argument"))
        return "ARG";
}
void parseCommands(FILE *inputFile, FILE *outputFile, int *labelNum) {
    
    ASSERT(inputFile, "input file not open on parse")
    ASSERT(outputFile, "output file not open on parse")
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

        char *instr;
        int sol;
        for (sol = 0; sol < strlen(line); sol++) {
            if (line[sol] != ' ' && line[sol] != '\t')
                break;
        }
        int eol;
        for (eol = sol; sol < strlen(line); eol++) {
            if (line[eol] == '\0' || line[eol] == '\n' || line[eol] == '/' || line[eol] == '\r')
                break;
        }
    
        instr = malloc(((eol) - sol) + 1);
        strncpy(instr, line, (eol) - sol);
        instr[(eol) - sol] = '\0';

        parseCommand(instr, outputFile, labelNum);
    }
    WRITE("(END)\n");
    WRITE("@END\n");
    WRITE("0;JMP\n");
}

void parseCommand(char *instr, FILE *outputFile, int *labelNum) {
    char *command[MAX_COMMAND_LENGTH];
    int i = 1;
    char *token = strtok(instr, " ");
    command[0] = token;
    while(token != NULL) {
        token = strtok(NULL, " ");
        if (token == NULL) break;
        command[i] = token;
        i++;
    }
    
    char *action = command[0];
    if (STREQUALS(action, "push")) {
        doPush(command, outputFile);
    } else if (STREQUALS(action, "pop")) {
        doPop(command, outputFile);
    } else doArithmetic(command, outputFile, labelNum);
}

void doPush(char *command[MAX_COMMAND_LENGTH], FILE *outputFile) {
    char *segment = command[1];
    char *offset = command[2];
    ASSERT(segment, "segment cannot be null")
    ASSERT(offset, "offset cannot be null")

    if (STREQUALS(segment, "constant")) { 
        WRITE("@%s\n", offset)
        WRITE("D=A\n")
        WRITE("@SP\n")
        WRITE("A=M\n")
        WRITE("M=D\n")
        WRITE("@SP\n")
        WRITE("M=M+1\n")
    } else if (STREQUALS(segment, "pointer")) {
        int off = atoi(offset);
        off += 3;
        sprintf(offset, "%d", off); 
        WRITE("@%s\n", offset) 
        WRITE("D=M\n")
    } else if (STREQUALS(segment, "temp")) {
        int off = atoi(offset);
        off += 5;
        sprintf(offset, "%d", off); 
        WRITE("@%s\n", offset) 
        WRITE("D=M\n")
    } else if (STREQUALS(segment, "static")) {
        int off = atoi(offset);
        off += 16;
        sprintf(offset, "%d", off); 
        WRITE("@%s\n", offset) 
        WRITE("D=M\n")
    } else if (STREQUALS(segment, "argument") || STREQUALS(segment, "local") || STREQUALS(segment, "this") || STREQUALS(segment, "that")) {
        WRITE("@%s\n", toASM(segment))
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("A=D+A\n") 
        WRITE("D=M\n")
    } else ASSERT(0 == 1, "segment not implemented")
}

void doPop(char *command[MAX_COMMAND_LENGTH], FILE *outputFile) {
    char *segment = command[1];
    char *offset = command[2];
    if (STREQUALS(segment, "constant")) {
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("M=D\n")
    } else if (STREQUALS(segment, "pointer")) {
        int off = atoi(offset);
        off += 3;
        sprintf(offset, "%d", off);
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("M=D\n")
    } else if (STREQUALS(segment, "temp")) {
        int off = atoi(offset);
        off += 5;
        sprintf(offset, "%d", off);
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("M=D\n")
    } else if (STREQUALS(segment, "static")) {
        int off = atoi(offset);
        off += 16;
        sprintf(offset, "%d", off);
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("M=D\n")
    } else if (STREQUALS(segment, "argument") || STREQUALS(segment, "local") || STREQUALS(segment, "this") || STREQUALS(segment, "that")) {
        WRITE("@%s\n", toASM(segment))
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("D=D+A\n")
        WRITE("@13\n")
        WRITE("M=D\n")
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@13\n")
        WRITE("A=M\n")
        WRITE("M=D\n")
    } else ASSERT(0 == 1, "not yet implemented")
}

void doArithmetic(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, int *labelNum) {
    char *action = command[0];
    if (STREQUALS(action, "add")) {
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=M+D\n")
    } else if (STREQUALS(action, "sub")) {
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=M-D\n")
    } else if (STREQUALS(action, "neg")) {
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=-M\n")
    } else if (STREQUALS(action, "and")) {
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=M&D\n")
    } else if (STREQUALS(action, "or")) {
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=M|D\n")
    } else if (STREQUALS(action, "not")) {
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=!M\n")
    } else if (STREQUALS(action, "eq")) {
        char labelNumchar[MAX_LINE_LENGTH];
        sprintf(labelNumchar, "%d", (*labelNum)++);
        int len = strlen(labelNumchar) + 1;
        char *label = calloc(1, len + 1);
        label[0] = 'L';
        strncat(&label[1], labelNumchar, len - 1);
        label[len] = '\0'; 
            
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("D=M-D\n")
        WRITE("M=-1\n")
        WRITE("@%s\n", label) 
        WRITE("D;JEQ\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=0\n")
        WRITE("(%s)\n", label)
        free(label);

    } else if (STREQUALS(action, "lt")) {
        char labelNumchar[MAX_LINE_LENGTH];
        sprintf(labelNumchar, "%d", (*labelNum)++);
        int len = strlen(labelNumchar) + 1;
        char *label = calloc(1, len + 1);
        label[0] = 'L';
        strncat(&label[1], labelNumchar, len - 1);
        label[len] = '\0'; 

        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("D=M-D\n")
            
        WRITE("M=-1\n")
        WRITE("@%s\n", label)
        WRITE("D;JLT\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=0\n")
        WRITE("(%s)\n", label)
        free(label);
    } else if (STREQUALS(action, "gt")) {
        char labelNumchar[MAX_LINE_LENGTH];
        sprintf(labelNumchar, "%d", (*labelNum)++);
        int len = strlen(labelNumchar) + 1;
        char *label = calloc(1, len + 1);
        label[0] = 'L';
        strncat(&label[1], labelNumchar, len - 1);
        label[len] = '\0'; 

        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("D=M-D\n")
            
        WRITE("M=-1\n")
        WRITE("@%s\n", label)
        WRITE("D;JGT\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=0\n")
        WRITE("(%s)\n", label)
        free(label);
    } 
}

