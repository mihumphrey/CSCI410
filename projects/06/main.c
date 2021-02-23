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
    

    struct assembler *assemb = assembler();
    FILE *file = fopen(filename, "r");
    FILE *newFile = fopen(out, "w");
    firstPass(assemb, file);
    secondPass(assemb, file, newFile);
    fclose(file);
    fclose(newFile);
    freeAssembler(assemb);
    
//    free(out);
    return 0;
}
