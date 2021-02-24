#include "assembler.h"
#include "definitions.h"

int main(int argc, char *argv[]) {
    ASSERT(argc == 2, "wrong number of arguments (must be 1 -- filename)");
    char *filename = argv[1];
     
    int i;
    for (i = 0; i < strlen(filename); i++) {
        if (filename[i] == '.')
            break;
    
    }
    
    char *out = calloc(1, i + 6);
    strncpy(out, filename, i + 1);
    strncat(out, "hack", sizeof("hack"));
    out[i + 5] = '\0';

    printf("OPENING FILE: %s\n", out);
    

    struct assembler *assemb = assembler();
    FILE *file = fopen(filename, "r");
    FILE *newFile = fopen(out, "w");
    firstPass(assemb, file);
    for (int i = 0; i < assemb -> st -> used; i++) {
        printf("IN TABLE: %s, WITH MEM: %d\n", assemb -> st -> symbolList[i].name, assemb -> st -> symbolList[i].memAddr);
    }
    secondPass(assemb, file, newFile);
    fclose(file);
    fclose(newFile);
    freeAssembler(assemb);
    free(out);

    return 0;
}
