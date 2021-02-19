#include "assembler.h"
#include "definitions.h"

int main(int argc, char *argv[]) {
    ASSERT(argc == 2, "wrong number of arguments (must be 1 -- filename)");
    struct assembler *assemb = assembler();
    FILE *file = fopen(argv[1], "r");
    firstPass(assemb, file);
    fclose(file);
    printf("Hello, World!\n");
    return 0;
}