#include "translator.h"

int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "wrong number of args -- must be 1")    
    char *filename = argv[1];
    FILE *inputFile = fopen(filename, "r");
    
    int i;
    for (i = 0; i < strlen(filename); i++) {
        if (filename[i] == '.')
            break;
    }
    char *out = calloc(1, i + 5);
    strncpy(out, filename, i + 1);
    strcat(out, "asm");
    out[i + 4] = '\0';

    FILE *outputFile = fopen(out, "w");
    int labelCounter = 0;
    parseCommands(inputFile, outputFile, &labelCounter);
    return 0;
}
