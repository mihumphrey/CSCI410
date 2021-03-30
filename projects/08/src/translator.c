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
//*         void                                                                                                     *//
//********************************************************************************************************************//
void getOffset(char *segment, char *offset) {
    int off = atoi(offset);
    if (STREQUALS(segment, "pointer"))
        off += 3;
    else if (STREQUALS(segment, "temp"))
        off += 5;
    else if (STREQUALS(segment, "static"))
        off += 16;

    sprintf(offset, "%d", off);
}

//********************************************************************************************************************//
//* Helper Function pushASM                                                                                          *//
//*     Input:                                                                                                       *//
//*         segment: char * -- memory segment                                                                        *//
//*     Translates push segment to assembly name                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void pushCallStack(char *segment, Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t\t* Pushing %s\n", segment);
    WRITE("@%s\n", segment)
    WRITE("D=M\n")
    WRITE("@SP\n")
    WRITE("AM=M+1\n")
    WRITE("A=A-1\n")
    WRITE("M=D\n")
}

//********************************************************************************************************************//
//* Helper Function popASM                                                                                           *//
//*     Input:                                                                                                       *//
//*         segment: char * -- memory segment                                                                        *//
//*     Translates pop segment to assembly name                                                                      *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void popCallStack(char *segment, Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t\t* Popping %s\n", segment);
    WRITE("@13\n")
    WRITE("AM=M-1\n")
    WRITE("D=M\n")
    WRITE("@%s\n", segment)
    WRITE("M=D\n")
}

//********************************************************************************************************************//
//* Function doPush                                                                                                  *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Handles all push operations to all memory segments                                                           *//
//*         Writes ASM code for all types of push arguments and exits gracefully if it does not know a memory segment*//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doPush(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doPush\n");
    char *segment = translator->currCommand[1];
    char *offset = translator->currCommand[2];
    if (verbose) {
        fprintf(stderr, "\t\t* Action: %s\n", translator->currCommand[0]);
        fprintf(stderr, "\t\t* Segment: %s\n", translator->currCommand[1]);
        fprintf(stderr, "\t\t* Offset: %s\n", translator->currCommand[2]);
    }
    ASSERT(segment, "segment cannot be null")
    ASSERT(offset, "offset cannot be null")
    if (STREQUALS(segment, "constant")) {
        WRITE("@%s\n", offset)
        WRITE("D=A\n")
    } else if (STREQUALS(segment, "static") || STREQUALS(segment, "temp") || STREQUALS(segment, "pointer")) {
        getOffset(segment, offset);
        if (STREQUALS(segment, "static"))
            WRITE("@%s.%s\n", translator->currClass, offset)
        else
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
//*         Translator                                                                                               *//
//*     Handles all pop operations to all memory segments                                                            *//
//*         Writes ASM code for all types of pop arguments and exits gracefully if it does not know a memory segment *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doPop(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doPop\n");
    char *segment = translator->currCommand[1];
    char *offset = translator->currCommand[2];
    if (verbose) {
        fprintf(stderr, "\t\t* Action: %s\n", translator->currCommand[0]);
        fprintf(stderr, "\t\t* Segment: %s\n", translator->currCommand[1]);
        fprintf(stderr, "\t\t* Offset: %s\n", translator->currCommand[2]);
    }
    ASSERT(segment, "segment cannot be null")
    ASSERT(offset, "offset cannot be null")
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
        getOffset(segment, offset);
        WRITE("@SP\n")
        WRITE("AM=M-1\n")
        WRITE("D=M\n")
        if (STREQUALS(segment, "static"))
            WRITE("@%s.%s\n", translator->currClass, offset)
        else
            WRITE("@%s\n", offset)
        WRITE("M=D\n")
    } else ASSERT(0 == 1, "segment not found in pop instruction")
    //free(offset);
}

//********************************************************************************************************************//
//* Function doArithmetic                                                                                            *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*         labelNum: int * -- current label number for all global labels                                            *//
//*     Handles all arithmetic operations from memory                                                                *//
//*         Writes ASM code for all types of arithmetic operations and exits gracefully if it does not know a command*//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doArithmetic(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doArithmetic\n");
    char *action = translator->currCommand[0];
    if (verbose) {
        fprintf(stderr, "\t\t* Action: %s\n", action);
        fprintf(stderr, "\t\t* Segment: %s\n", "NULL");
        fprintf(stderr, "\t\t* Offset: %s\n", "NULL");
    }
    ASSERT(action, "action cannot be null")
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
        sprintf(labelNumchar, "%d", translator->labelNum++);
        int len = strlen(labelNumchar) + 1;
        char *label = calloc(1, len + 1);
        label[0] = 'L';
        strcat(&label[1], labelNumchar);
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
        //free(label);
    } else ASSERT(0 == 1, "arithmetic command not implemented")
}

//********************************************************************************************************************//
//* Function doLabel                                                                                                 *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Adds a new label to the ASM                                                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doLabel(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doLabel\n");
    char *label = translator->currCommand[1];
    if (verbose)
        fprintf(stderr, "\t\t* Setting label: %s\n", label);
    ASSERT(label, "label cannot be null")
    WRITE("(%s$%s)\n", translator->currFunc, label) 
}

//********************************************************************************************************************//
//* Function doIfGoto                                                                                                *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Writes the ASM code to handle an if-goto statement                                                           *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doIfGoto(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doIfGoto\n");
    char *label = translator->currCommand[1];
    if (verbose)
        fprintf(stderr, "\t\t* Setting label: %s\n", label);
    ASSERT(label, "label cannot be null")
    WRITE("@SP\n")
    WRITE("AM=M-1\n")
    WRITE("D=M\n")
    WRITE("@%s$%s\n", translator->currFunc, label)
    WRITE("D;JNE\n")
}

//********************************************************************************************************************//
//* Function doGoto                                                                                                  *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Handles Writing ASM code for goto statements                                                                 *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doGoto(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doGoto\n");
    char *label = translator->currCommand[1];
    if (verbose)
        fprintf(stderr, "\t\t* Setting label: %s\n", label);
    ASSERT(label, "label cannot be null")
    WRITE("@%s$%s\n", translator->currFunc, label)
    WRITE("0;JMP\n")
}

//********************************************************************************************************************//
//* Function doFunc                                                                                                  *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Handles all func commands, writing ASM code for a new func                                                   *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doFunc(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doFunc\n");
    char *label = translator->currCommand[1];
    char *numVars = translator->currCommand[2];
    int nVars = atoi(numVars);
    if (verbose)
        fprintf(stderr, "\t\t* Setting currFunc: %s\n", translator->currFunc);
    strcpy(translator->currFunc, label);
    translator->currFunc[strlen(label)] = '\0';
    ASSERT(label, "label cannot be null")
    ASSERT(numVars, "numVars cannot be null")
    WRITE("(%s)\n", translator->currFunc)
    for (int i = 0; i < nVars; i++) {
        WRITE("@SP\n")
        WRITE("A=M\n")
        WRITE("M=0\n")
        WRITE("@SP\n")
        WRITE("M=M+1\n")
    }
}

//********************************************************************************************************************//
//* Function doCall                                                                                                  *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Handles all call commands                                                                                    *//
//*         Writes ASM code to push/store all memory for after the call                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doCall(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doCall\n");
    char *label = translator->currCommand[1];
    int nArgs = atoi(translator->currCommand[2]);

    ASSERT(label, "label can not be null")
    ASSERT(nArgs >= 0, "invalid number of args")
    if (verbose)
        fprintf(stderr, "\t\t* Pushing RA\n");
    WRITE("@return%s%d\n", translator->currFunc, translator->ra)
    WRITE("D=A\n")
    WRITE("@SP\n")
    WRITE("AM=M+1\n")
    WRITE("A=A-1\n")
    WRITE("M=D\n")

    pushCallStack("LCL", translator);
    pushCallStack("ARG", translator);
    pushCallStack("THIS", translator);
    pushCallStack("THAT", translator);

    if (verbose)
        fprintf(stderr, "\t\t* Pushing ARG = SP - (nArgs + 5)\n");
    WRITE("@SP\n")
    WRITE("D=M\n")
    WRITE("@%d\n", nArgs + 5)
    WRITE("D=D-A\n")
    WRITE("@ARG\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t\t* Pushing LCL = SP\n");
    WRITE("@SP\n")
    WRITE("D=M\n")
    WRITE("@LCL\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t\t* GOTO function\n");
    WRITE("@%s\n", label)
    WRITE("0;JMP\n")
    if (verbose)
        fprintf(stderr, "\t\t* Setting Call label: (return%s%d)\n", translator->currFunc, translator->ra );
    WRITE("(return%s%d)\n", translator->currFunc, translator->ra++)
}

//********************************************************************************************************************//
//* Function doCall                                                                                                  *//
//*     Input:                                                                                                       *//
//*         Translator                                                                                               *//
//*     Handles all return commands                                                                                  *//
//*         Writes ASM code to pop/load all memory for after the call                                                *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void doReturn(Translator *translator) {
    ASSERT(translator, "translator object null")
    if (verbose)
        fprintf(stderr, "\t* In doReturn\n");

    if (verbose)
        fprintf(stderr, "\t\t* Setting FRAME = LCL\n");
    WRITE("@LCL\n")
    WRITE("D=M\n")
    WRITE("@13\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t\t* Setting RET = FRAME - 5\n");
    WRITE("@5\n")
    WRITE("A=D-A\n")
    WRITE("D=M\n")
    WRITE("@14\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t\t* Popping ARG\n");
    WRITE("@SP\n")
    WRITE("AM=M-1\n")
    WRITE("D=M\n")
    WRITE("@ARG\n")
    WRITE("A=M\n")
    WRITE("M=D\n")

    if (verbose)
        fprintf(stderr, "\t\t* Setting SP = ARG + 1\n");
    WRITE("@ARG\n")
    WRITE("D=M+1\n")
    WRITE("@SP\n")
    WRITE("M=D\n")

    popCallStack("THAT", translator);
    popCallStack("THIS", translator);
    popCallStack("ARG", translator);
    popCallStack("LCL", translator);

    if (verbose)
        fprintf(stderr, "\t\t* Writing GOTO return\n");
    WRITE("@14\n")
    WRITE("A=M\n")
    WRITE("0;JMP\n")
}
