#include "translator.h"

extern bool verbose;

//********************************************************************************************************************//
//* Helper Function toASM                                                                                            *//
//*     Input:                                                                                                       *//
//*         loc: char * -- "local", "this", "that", or "argument"                                                    *//
//*     Translates verbose name to assembly name                                                                     *//
//*     Returns:                                                                                                     *//
//*         ASM name of location: char *                                                                             *//
//********************************************************************************************************************//
char *toASM(char *loc) {
    if (STREQUALS(loc, "local"))
        return "LCL";
    if (STREQUALS(loc, "this"))
        return "THIS";
    if (STREQUALS(loc, "that"))
        return "THAT";
    if (STREQUALS(loc, "argument"))
        return "ARG";
    ASSERT(0 == 1, "location not found")
    return "";
}

//********************************************************************************************************************//
//* Helper Function getOffset                                                                                        *//
//*     Input:                                                                                                       *//
//*         segment: char * -- "pointer", "temp", "static"                                                           *//
//*         offset: char * -- current offset                                                                         *//
//*     Increments the current offset by a set amount based on what the segment is                                   *//
//*     Returns:                                                                                                     *//
//*         New Offset: char *                                                                                       *//
//********************************************************************************************************************//
char *getOffset(char *segment, char *offset) {
    char *offStr = malloc(sizeof(offset));
    int off = atoi(offset);
    if (STREQUALS(segment, "pointer"))
        off += 3;
    else if (STREQUALS(segment, "temp"))
        off += 5;
    else if (STREQUALS(segment, "static"))
        off += 16;

    sprintf(offStr, "%d", off);
    return offStr;
}

void pushCallStack(char *segment, FILE *outputFile) {
    WRITE("@%s\n", segment)
    WRITE("D=M\n")
    WRITE("@SP\n")
    WRITE("AM=M+1\n")
    WRITE("A=A-1\n")
    WRITE("M=D\n")
}

void popCallStack(char *segment, FILE *outputFile) {
    WRITE("@13\n")
    WRITE("AM=M-1\n")
    WRITE("D=M\n")
    WRITE("@%s\n", segment)
    WRITE("M=D\n")
}

//********************************************************************************************************************//
//* Function parseCommands                                                                                           *//
//*     Input:                                                                                                       *//
//*         inputFile: FILE * -- file descriptor for the current source file                                         *//
//*         outputFile: FILE * -- file descriptor for the current output file                                        *//
//*         labelNum: int * -- current label number for all global labels                                            *//
//*     Parses all commands in a file simply by using fgets to get an entire line                                    *//
//*         Stores each current line in a char * to be further parsed                                                *//
//*         Uses '/' or '\0' or '\r' or '\n' to find the end of a useful line                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void parseCommands(FILE *inputFile, FILE *outputFile, int *labelNum, int *ra) {
        
    ASSERT(inputFile, "input file not open on parse")
    ASSERT(outputFile, "output file not open on parse")
    char line[MAX_LINE_LENGTH];
    char *currFunct = "main";
    
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
        if (verbose)
            fprintf(stderr, "* Found instruction: %s\n", instr);

        parseCommand(instr, outputFile, labelNum, currFunct, ra);
    }
}

//********************************************************************************************************************//
//* Function parseCommand                                                                                            *//
//*     Input:                                                                                                       *//
//*         instr: char * -- string representation of the current instruction                                        *//
//*         outputFile: FILE * -- file descriptor for the current output file                                        *//
//*         labelNum: int * -- current label number for all global labels                                            *//
//*     Parses single command using strtok to break apart a declaration and calling functions to handle the command  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void parseCommand(char *instr, FILE *outputFile, int *labelNum, char *currFunc, int *ra) {
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
    } else if (STREQUALS(action, "label")) {
        doLabel(command, outputFile, currFunc);
    } else if (STREQUALS(action, "if-goto")) {
        doIfGoto(command, outputFile, currFunc);
    } else if (STREQUALS(action, "goto")) {
        doGoto(command, outputFile, currFunc);
    } else if (STREQUALS(action, "function")) {
        doFunct(command, outputFile, currFunc);
    } else if (STREQUALS(action, "call")) {
        doCall(command, outputFile, currFunc, ra);
    } else if (STREQUALS(action, "return")) {
        doReturn(command, outputFile, currFunc, ra);
    } else doArithmetic(command, outputFile, labelNum);
}

//********************************************************************************************************************//
//* Function doPush                                                                                                  *//
//*     Input:                                                                                                       *//
//*         command: char ** -- array representation of the command                                                  *//
//*         outputFile: FILE * -- file descriptor for the current output file                                        *//
//*     Handles all push operations to all memory segments                                                           *//
//*         Writes ASM code for all types of push arguments and exits gracefully if it does not know a memory segment*//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doPush(char *command[MAX_COMMAND_LENGTH], FILE *outputFile) {
    char *segment = command[1];
    char *offset = command[2];
    if (verbose) {
        fprintf(stderr, "\t* Action: %s\n", command[0]);
        fprintf(stderr, "\t* Segment: %s\n", command[1]);
        fprintf(stderr, "\t* Offset: %s\n", command[2]);
    }
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

//********************************************************************************************************************//
//* Function doPop                                                                                                   *//
//*     Input:                                                                                                       *//
//*         command: char ** -- array representation of the command                                                  *//
//*         outputFile: FILE * -- file descriptor for the current output file                                        *//
//*     Handles all pop operations to all memory segments                                                            *//
//*         Writes ASM code for all types of pop arguments and exits gracefully if it does not know a memory segment *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doPop(char *command[MAX_COMMAND_LENGTH], FILE *outputFile) {
    char *segment = command[1];
    char *offset = command[2];
    if (verbose) {
        fprintf(stderr, "\t* Action: %s\n", command[0]);
        fprintf(stderr, "\t* Segment: %s\n", command[1]);
        fprintf(stderr, "\t* Offset: %s\n", command[2]);
    }
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

//********************************************************************************************************************//
//* Function doArithmetic                                                                                            *//
//*     Input:                                                                                                       *//
//*         command: char ** -- array representation of the command                                                  *//
//*         outputFile: FILE * -- file descriptor for the current output file                                        *//
//*         labelNum: int * -- current label number for all global labels                                            *//
//*     Handles all arithmetic operations from memory                                                                *//
//*         Writes ASM code for all types of arithmetic operations and exits gracefully if it does not know a command*//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doArithmetic(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, int *labelNum) {
    char *action = command[0];
    if (verbose) {
        fprintf(stderr, "\t* Action: %s\n", command[0]);
        fprintf(stderr, "\t* Segment: %s\n", "NULL\n");
        fprintf(stderr, "\t* Offset: %s\n", "NULL\n");
    }
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

void doLabel(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc) {
    char *label = command[1];
    WRITE("(%s$%s)\n", currFunc, label) 
}

void doIfGoto(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc) {
    char *label = command[1];
    WRITE("@SP\n")
    WRITE("AM=M-1\n")
    WRITE("D=M\n")
    WRITE("@%s$%s\n", currFunc, label)
    WRITE("D;JNE\n")
}

void doGoto(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc) {
    char *label = command[1];
    WRITE("@%s$%s\n", currFunc, label)
    WRITE("0;JMP\n")
}

void doFunct(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc) {
    char *label = command[1];
    char *numVars = command[2];
    int nVars = atoi(numVars);
    printf("NVARS: %d\n", nVars);
    currFunc = label;
    for (int i = 0; i < nVars; i++) {
        WRITE("@SP\n")
        WRITE("A=M\n")
        WRITE("M=0\n")
        WRITE("@SP\n")
        WRITE("M=M+1\n")
    }
}

void doCall(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc, int *ra) {
    if (verbose)
        fprintf(stderr, "* In doCall\n");
    char *label = command[1];
    int nArgs = atoi(command[2]);
    if (verbose)
        fprintf(stderr, "\t* Pushing RA\n");
    WRITE("@return%s%d\n", currFunc, *ra)
    WRITE("D=A\n")
    WRITE("@SP\n")
    WRITE("AM=M+1\n")
    WRITE("A=A-1\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t* Pushing LCL\n");
    pushCallStack("LCL", outputFile);
    if (verbose)
        fprintf(stderr, "\t* Pushing ARG\n");
    pushCallStack("ARG", outputFile);
    if (verbose)
        fprintf(stderr, "\t* Pushing THIS\n");
    pushCallStack("THIS", outputFile);
    if (verbose)
        fprintf(stderr, "\t* Pushing THAT\n");
    pushCallStack("THAT", outputFile);

    if (verbose)
        fprintf(stderr, "\t* Pushing ARG = SP - (nArgs + 5)\n");
    WRITE("@SP\n")
    WRITE("D=M\n")
    WRITE("@%d\n", nArgs + 5)
    WRITE("D=D-A\n")
    WRITE("@ARG\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t* Pushing LCL = SP\n");
    WRITE("@SP\n")
    WRITE("D=M\n")
    WRITE("@LCL\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t* GOTO function\n");
    WRITE("@%s\n", label)
    WRITE("0;JMP\n")
    if (verbose)
        fprintf(stderr, "\t* Setting Call label: (return%s%d)\n",currFunc, *ra );
    WRITE("(return%s%d)\n", currFunc, (*ra)++)
}

void doReturn(char *command[MAX_COMMAND_LENGTH], FILE *outputFile, char *currFunc, int *ra) {
    WRITE("@LCL\n")
    WRITE("D=M\n")
    WRITE("@13\n")
    WRITE("M=D\n")
    WRITE("@5\n")
    WRITE("A=D-A\n")
    WRITE("D=M\n")
    WRITE("@14\n")
    WRITE("M=D\n")

    WRITE("@SP\n")
    WRITE("AM=M-1\n")
    WRITE("D=M\n")
    WRITE("@ARG\n")
    WRITE("A=M\n")
    WRITE("M=D\n")

    WRITE("@ARG\n")
    WRITE("D=M+1\n")
    WRITE("@SP\n")
    WRITE("M=D\n")

    popCallStack("THAT", outputFile);
    popCallStack("THIS", outputFile);
    popCallStack("ARG", outputFile);
    popCallStack("LCL", outputFile);

    WRITE("@14\n")
    WRITE("A=M\n")
    WRITE("0;JMP\n")
    
}
