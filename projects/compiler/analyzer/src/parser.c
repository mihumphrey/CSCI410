#include "parser.h"

extern bool verbose;

//********************************************************************************************************************//
//* Function parse                                                                                                   *//
//*     Input:                                                                                                       *//
//*         inputFile: FILE * -- file to read from                                                                   *//
//*     Parses all data from file into flat char *, while stripping comments and whitespace                          *//
//*     Returns:                                                                                                     *//
//*         char * -- flatt array of class                                                                           *//
//********************************************************************************************************************//
char *parse(FILE *inputFile) {
    if (verbose)
        fprintf(stderr, "\t * Beginning parsing\n");
    bool isInComment = false;
    ASSERT(inputFile, "input file not open for reading")
        
    char line[MAX_LINE_LENGTH];
    CharList *input = malloc(sizeof(CharList));
    char *inputChar;
    initList_char(input, 16);
    
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
            for (int i = 0; i < strlen(command); i++) 
                insertList_char(input, command[i]);
            if (verbose)
                fprintf(stderr, "\t\t\t * Found Command: %s\n", command);
            free(command);
        }
    }
    inputChar = malloc(input->used + 1);
    memcpy(inputChar, input->list, input->used);
    inputChar[input->used] = '\0';
    freeList_char(input);
    if (verbose)
        fprintf(stderr, "\t * Done parsing\n");
    return inputChar;
}
