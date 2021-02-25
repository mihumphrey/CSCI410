#include "assembler.h"

bool verbose = false;

//********************************************************************************************************************//
//* Function main                                                                                                    *//
//*     Input:                                                                                                       *//
//*         argc: int                                                                                                *//
//*         argv: char **                                                                                            *//
//*     Creates an assembler object                                                                                  *//
//*     Calls firstPass and secondPass                                                                               *//
//*     Returns:                                                                                                     *//
//*         0                                                                                                        *//
//********************************************************************************************************************//
int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "wrong number of arguments (must be 1 or 2 -- filename & (-v)?)")
    char *filename = argv[1];
    if (argc > 2)
        if (strncmp(argv[2], "-v", strlen(argv[2])) == 0)
            verbose = true;
    int i;
    for (i = 0; i < strlen(filename); i++) {
        if (filename[i] == '.')
            break;
    }
    
    char *out = calloc(1, i + 6);
    strncpy(out, filename, i + 1);
    strcat(out, "hack");
    out[i + 5] = '\0';

    struct assembler *assemble = assembler();
    FILE *file = fopen(filename, "r");
    FILE *newFile = fopen(out, "w");

    if (verbose)
        printf("* Currently beginning first pass\n");
    firstPass(assemble, file);
    if (verbose)
        printf("* Currently beginning second pass\n");
    secondPass(assemble, file, newFile);
    fclose(file);
    fclose(newFile);
    if (verbose)
        printf("* Freeing all assembler data\n");
    freeAssembler(assemble);
    free(out);

    return 0;
}
