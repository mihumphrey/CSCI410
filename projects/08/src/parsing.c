#include "translator.h"
#include "parsing.h"

extern bool verbose;
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
void parseCommands(Translator *translator) {
        
    ASSERT(translator->inputFile, "input file not open on parse")
    ASSERT(translator->outputFile, "output file not open on parse")
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, MAX_LINE_LENGTH, translator->inputFile)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

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
    
        strncpy(translator->line, line, eol-sol);
        translator->line[eol - sol] = '\0';
        WRITE("//%s\n", translator->line)
        if (verbose)
            fprintf(stderr, "* Found instruction: %s\n", translator->line);

        parseCommand(translator);
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
void parseCommand(Translator *translator) {
    char *command[MAX_COMMAND_LENGTH];
    int i = 1;
    char *token = strtok(translator->line, " ");
    command[0] = token;
    while(token != NULL) {
        token = strtok(NULL, " ");
        if (token == NULL) break;
        command[i] = token;
        i++;
    }
    
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        translator->currCommand[i] = command[i];
    }

    char *action = command[0];
    if (STREQUALS(action, "push")) {
        doPush(translator);
    } else if (STREQUALS(action, "pop")) {
        doPop(translator);
    } else if (STREQUALS(action, "label")) {
        doLabel(translator);
    } else if (STREQUALS(action, "if-goto")) {
        doIfGoto(translator);
    } else if (STREQUALS(action, "goto")) {
        doGoto(translator);
    } else if (STREQUALS(action, "function")) {
        doFunc(translator);
    } else if (STREQUALS(action, "call")) {
        doCall(translator);
    } else if (STREQUALS(action, "return")) {
        doReturn(translator);
    } else doArithmetic(translator);
}
