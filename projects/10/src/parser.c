#include "parser.h"

void initList_char(CharList *tokens, size_t initialSize) {
    tokens->list = malloc(initialSize);
    tokens->used = 0;
    tokens->size = initialSize;
}

void insertList_char(CharList *tokens, char element) {
    if (tokens->used == tokens->size) {
        tokens->size *= 2;
        tokens->list = realloc(tokens->list, tokens->size);
    }
    tokens->list[tokens->used++] = element;
}

void freeList_char(CharList *tokens) {
    free(tokens->list);
    tokens->list = NULL;
    tokens->used = tokens->size = 0;
    free(tokens);
}

void initList_Token(TokenList *tokens, size_t initialSize) {
    tokens->list = calloc(1, initialSize * sizeof(Token));
    tokens->used = 0;
    tokens->size = initialSize;
    tokens->iter = 0;
}

void insertList_Token(TokenList *tokens, Token *element) {
    if (tokens->used == tokens->size) {
        tokens->size *= 2;
        tokens->list = realloc(tokens->list, tokens->size * sizeof(Token));
    }
    tokens->list[tokens->used++] = element;
}

void freeList_Token(TokenList *tokens) {
    for (int i = 0; i < tokens->used; i++) {
        free(tokens->list[i] ->name);
        free(tokens->list[i]);
    }
    free(tokens->list);
    tokens->list = NULL;
    tokens->used = tokens->size = 0;
    free(tokens);
}

char *parse(FILE *inputFile, FILE *outputFile) {
    bool isInComment = false;
    ASSERT(inputFile, "input file not open for reading")
    ASSERT(outputFile, "output file not open for writing")

        
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
            free(command);
        }
    }
    inputChar = malloc(input->used + 1);
    memcpy(inputChar, input->list, input->used);
    inputChar[input->used] = '\0';
    freeList_char(input);
    return inputChar;
}
