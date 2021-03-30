#include "tokenizer.h"

char **parse(FILE *inputFile, FILE *outputFile) {
    char *input[MAX_LINE_LENGTH];
    bool isInComment = false;
    ASSERT(inputFile, "input file not open for reading")
    ASSERT(outputFile, "output file not open for writing")

    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        bool cont = true;
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
            if (line[eol] == '/' && line[eol + 1] == '/') {
                cont = false;
                break;
            }
        
            if (line[eol] == '/' && line[eol + 1] == '*') {
                isInComment = true;
                cont = false;
            }

            if (line[eol] == '*' && line[eol + 1] == '/') {
                isInComment = false;
                cont = false;
            }

            if (line[eol] == '\0' || line[eol] == '\n' || line[eol] == '\r' || (line[eol] == '/' && line[eol + 1] == '/'))
                break;
        }
        
        if (cont && !isInComment) {
            char *command = calloc(1, (eol - sol) + 1);
            strncpy(command, &line[sol], eol - sol);
            command[eol - sol] = '\0';
            printf("COMMAND: %s\n", command);
        }
    }
    return input;
}

Token *getTokens(FILE *inputfile) {
    return NULL;    
}
