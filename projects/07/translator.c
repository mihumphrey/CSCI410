#include "translator.h"

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
    fprintf(outputFile, "(END)\n");
    fprintf(outputFile, "@END\n");
    fprintf(outputFile, "0;JMP\n");
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
    if (strncmp(action, "push", strlen(action)) == 0) {
        doPush(command, outputFile);
    } else if (strncmp(action, "pop", strlen(action)) == 0) {
        doPop(command, outputFile);
    } else doArithmetic(command, outputFile, labelNum);
}

void doPush(char *command[MAX_COMMAND_LENGTH], FILE *outputFile) {
    char *segment = command[1];
    char *offset = command[2];
    ASSERT(segment, "segment cannot be null")
    ASSERT(offset, "offset cannot be null")

    if (strncmp(segment, "constant", strlen(segment)) == 0) {
        fprintf(outputFile, "@SP\n");  
        fprintf(outputFile, "AM=M-1\n");  
        fprintf(outputFile, "D=M\n");  
        fprintf(outputFile, "@%s\n", offset);  
        fprintf(outputFile, "M=D\n");
    } else if (strncmp(segment, "argument", strlen(segment)) == 0) {
        
    } else if (strncmp(segment, "local", strlen(segment)) == 0) {
    
    } else if (strncmp(segment, "this", strlen(segment)) == 0) {
    
    } else if (strncmp(segment, "that", strlen(segment)) == 0) {
    
    } else if (strncmp(segment, "temp", strlen(segment)) == 0) {

    } else if (strncmp(segment, "pointer", strlen(segment)) == 0) {

    } else if (strncmp(segment, "static", strlen(segment)) == 0) {

    } else ASSERT(0 == 1, "segment not implemented")
}

void doPop(char *command[MAX_COMMAND_LENGTH], FILE *outputFile) {
    ASSERT(0 == 1, "not yet implemented")
}

void doArithmetic(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, int *labelNum) {
    char *action = command[0];
    if (strncmp(action, "eq", strlen(action)) != 0 && strncmp(action, "gt", strlen(action)) != 0 && strncmp(action, "lt", strlen(action)) != 0) {
        fprintf(outputFile, "@SP\n");
        fprintf(outputFile, "M=M-1\n");
        fprintf(outputFile, "A=M\n");
        fprintf(outputFile, "D=M\n");
        fprintf(outputFile, "@SP\n");
        if (strncmp(action, "add", strlen(action)) == 0) {
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "M=D+M\n");
        } else if (strncmp(action, "sub", strlen(action)) == 0) {
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "M=M-D\n");
        } else if (strncmp(action, "neg", strlen(action)) == 0) {
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "M=-M\n");
        } else if (strncmp(action, "and", strlen(action)) == 0) {
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "M=M&D\n");
        } else if (strncmp(action, "or", strlen(action)) == 0) {
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "M=M|D\n");
        } else if (strncmp(action, "not", strlen(action)) == 0) {
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "M=!D\n");
        } else ASSERT(0 == 1, "command not implemented")
    } else {
        if (strncmp(action, "eq", strlen(action)) == 0) {
            char labelNumchar[MAX_LINE_LENGTH];
            sprintf(labelNumchar, "%d", (*labelNum)++);
            int len = strlen(labelNumchar) + 1;
            char *label = calloc(1, len + 1);
            label[0] = 'F';
            strncat(&label[1], labelNumchar, len - 1);
            label[len] = '\0'; 
            
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M-1\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "D=M-D\n");
            fprintf(outputFile, "@%s\n", label); 
            fprintf(outputFile, "D;JEQ\n");
            fprintf(outputFile, "D=1\n");
            fprintf(outputFile, "(%s)\n", label);
            fprintf(outputFile, "    @SP\n");
            fprintf(outputFile, "    A=M-1\n");
            fprintf(outputFile, "    M=!D\n");
            free(label);
        } else if (strncmp(action, "lt", strlen(action)) == 0 || strncmp(action, "gt", strlen(action)) == 0) {
            char labelNumcharF[MAX_LINE_LENGTH];
            sprintf(labelNumcharF, "%d", (*labelNum)++);
            int len = strlen(labelNumcharF) + 1;
            char *labelF = calloc(1, len + 1);
            labelF[0] = 'F';
            strncat(&labelF[1], labelNumcharF, len - 1);
            labelF[len] = '\0'; 

            char labelNumcharT[MAX_LINE_LENGTH];
            sprintf(labelNumcharT, "%d", (*labelNum)++);
            int len2 = strlen(labelNumcharT) + 1;
            char *labelT = calloc(1, len2 + 1);
            labelT[0] = 'T';
            strncat(&labelT[1], labelNumcharT, len2 - 1);
            labelT[len] = '\0'; 

            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "M=M-1\n");
            fprintf(outputFile, "A=M\n");
            fprintf(outputFile, "D=M\n");
            fprintf(outputFile, "@SP\n");
            fprintf(outputFile, "A=M-1\n");
            fprintf(outputFile, "D=M-D\n");
            fprintf(outputFile, "@%s\n", labelT);
            fprintf(outputFile, "D;%s\n", strncmp(action, "lt", strlen(action)) == 0 ? "JLE" : "JGT");
            fprintf(outputFile, "D=0\n");
            fprintf(outputFile, "D=!D\n");
            fprintf(outputFile, "@%s\n", labelF);
            fprintf(outputFile, "0;JMP\n");
            fprintf(outputFile, "(%s)\n", labelT);
            fprintf(outputFile, "   D=0\n");
            fprintf(outputFile, "(%s)\n", labelF);
            fprintf(outputFile, "   @SP\n");
            fprintf(outputFile, "   A=M-1\n");
            fprintf(outputFile, "   M=!D\n");
            free(labelF);
            free(labelT);
        } else ASSERT(0 == 1, "unhandled action type");
    } 
}

