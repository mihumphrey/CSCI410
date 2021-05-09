#include "assembler.h"
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

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
 
    struct stat path_stat;
    stat(filename, &path_stat);
    char *filenameCPY = malloc(strlen(filename) + 1);
    memcpy(filenameCPY, filename, strlen(filename));
    filenameCPY[strlen(filename)] = '\0';
    struct assembler *assemble = assembler();
    if (S_ISDIR(path_stat.st_mode)) {
        
        DIR *dir = opendir(filename);
        struct dirent *dp;
        ASSERT(dir, "directory not open");

        while ((dp = readdir(dir)) != NULL) {
            char input[PATH_MAX ] = {0};
            memcpy(&input, filename, strlen(filename));
            char *ext = strchr(dp->d_name, '.');
            if (ext != NULL && strncmp(ext, ".asm", strlen(".asm")) == 0) {
                
                strncpy(input, filename, PATH_MAX - 1);
                strncat(input, "/", 2);
                strncat(input, dp->d_name, PATH_MAX - 1);
                input[PATH_MAX - 1] = '\0';
        
                if (verbose)
                    fprintf(stderr, "\t* Assembling File: %s\n", input);
                char out[PATH_MAX + 1] = {0};
                memcpy(out, input, strlen(input) - strlen(ext));
                strcat(out, ".hack");
                out[strlen(input) + 1] = '\0';

                if (verbose)
                    fprintf(stderr, "\t* Assembler output file: %s\n", out);
                FILE *outputFile = fopen(out, "w");
    
                ASSERT(outputFile, "cannot open output file to write")

                FILE *inputFile = fopen(input, "r");
                ASSERT(inputFile, "cannot open input file to read")

                if (verbose)
                    printf("* Currently beginning first pass\n");
                firstPass(assemble, inputFile);
                if (verbose)
                    printf("* Currently beginning second pass\n");
                secondPass(assemble, inputFile, outputFile);
                if (verbose)
                    printf("* Freeing all assembler data\n");
                fclose(inputFile);
                fclose(outputFile);
            }
        }
    }
    freeAssembler(assemble);
    return 0;
}
