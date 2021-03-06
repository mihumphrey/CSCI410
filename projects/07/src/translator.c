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
    return "";
}

char *getOffset(char *segment, char *offset) {
    int off = atoi(offset);
    if (STREQUALS(segment, "pointer"))
        off += 3;
    else if (STREQUALS(segment, "temp"))
        off += 5;
    else if (STREQUALS(segment, "static"))
        off += 16;

    sprintf(offset, "%d", off);
    return offset;
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
    } else if (STREQUALS(segment, "static") || STREQUALS(segment, "temp") || STREQUALS(segment, "pointer")) {
        offset = getOffset(segment, offset);
        WRITE("@%s\n", offset)
        WRITE("D=M\n")
    } else if (STREQUALS(segment, "this") || STREQUALS(segment, "that") || STREQUALS(segment, "argument") || STREQUALS(segment, "local")) {
        WRITE("@%s\n", toASM(segment))
        WRITE("D=M\n")
        WRITE("@%s\n", offset)
        WRITE("A=D+A\n")
        WRITE("D=M\n")
    } else ASSERT(0 == 1, "segment not found in push instruction")
    WRITE("@SP\n")
    WRITE("A=M\n")
    WRITE("M=D\n")
    WRITE("@SP\n")
    WRITE("M=M+1\n")
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
        } else if (STREQUALS(segment, "this") || STREQUALS(segment, "that") || STREQUALS(segment, "argument") || STREQUALS(segment, "local")) {
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
        } else if (STREQUALS(segment, "pointer") || STREQUALS(segment, "temp") || STREQUALS(segment, "static")) {
            offset = getOffset(segment, offset);
            WRITE("@SP\n")
            WRITE("AM=M-1\n")
            WRITE("D=M\n")
            WRITE("@%s\n", offset)
            WRITE("M=D\n")
        } else ASSERT(0 == 1, "segment not found in pop instruction")
}

void doArithmetic(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, int *labelNum) {
    char *action = command[0];
        WRITE("@SP\n")
    if (STREQUALS(action, "add") || STREQUALS(action, "sub") || STREQUALS(action, "and") || STREQUALS(action, "or")) {
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
    }
    if (STREQUALS(action, "add")) {
        WRITE("M=M+D\n")
    } else if (STREQUALS(action, "sub")) {
        WRITE("M=M-D\n")
    } else if (STREQUALS(action, "neg")) {
        WRITE("A=M-1\n")
        WRITE("M=-M\n")
    } else if (STREQUALS(action, "and")) {
        WRITE("M=M&D\n")
    } else if (STREQUALS(action, "or")) {
        WRITE("M=M|D\n")
    } else if (STREQUALS(action, "not")) {
        WRITE("A=M-1\n")
        WRITE("M=!M\n")
    } else if (STREQUALS(action, "eq") || STREQUALS(action, "lt") || STREQUALS(action, "gt")) {
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
        WRITE("D;%s\n", STREQUALS(action, "eq") ? "JEQ" : STREQUALS(action, "gt") ? "JGT" : "JLT")
        WRITE("@SP\n")
        WRITE("A=M-1\n")
        WRITE("M=0\n")
        WRITE("(%s)\n", label)
        free(label);
    } else ASSERT(0 == 1, "arithmetic command not implemented")
}
