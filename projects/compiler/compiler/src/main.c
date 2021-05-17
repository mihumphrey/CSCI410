#include "includes.h"
#include "parser.h"
#include "tokenizer.h"
#include "compiler.h"

bool verbose = false;

//********************************************************************************************************************//
//* Function main                                                                                                    *//
//*     Input:                                                                                                       *//
//*         argc: int -- number of inputs + 1, argv: char** -- input flags                                           *//
//*     Parses input, and creates output files + calls all other functions                                           *//
//*     Returns:                                                                                                     *//
//*         int -- exit status of program                                                                            *//
//********************************************************************************************************************//
int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "Too few arguments -- must contain FILENAME, may contain -v(erbose), -t(okenize), -p(arse) -- ")

    bool OS = true;
    if (argc > 2 && STREQUALS(argv[2], "-v")) verbose = true;    
    if (argc > 2 && STREQUALS(argv[2], "-s")) OS = false;

    if (argc > 3 && STREQUALS(argv[3], "-v")) verbose = true;
    if (argc > 3 && STREQUALS(argv[3], "-s")) OS = false;

    char *filename = argv[1];
    ASSERT(filename, "Input filename not found")

    if (verbose)
        fprintf(stderr, "* Compiling: %s\n", filename);

    Compiler *compiler = malloc(sizeof(Compiler));
    compiler->labelNum = 0;

    compiler->classTable = malloc(sizeof(SymbolTable));
    //compiler->subroutineTable = malloc(sizeof(SymbolTable));

    initList_symbolTable(compiler->classTable, 2);
    //initList_symbolTable(compiler->subroutineTable, 2);


    struct stat path_stat;
    stat(filename, &path_stat);
    compiler->outputFile = NULL;
    char *filenameCPY = malloc(strlen(filename) + 1);
    memcpy(filenameCPY, filename, strlen(filename));
    filenameCPY[strlen(filename)] = '\0';
    if (S_ISDIR(path_stat.st_mode)) {
        
        DIR *dir = opendir(filename);
        struct dirent *dp;
        ASSERT(dir, "directory not open");

        while ((dp = readdir(dir)) != NULL) {
            char input[PATH_MAX ] = {0};
            memcpy(&input, filename, strlen(filename));
            char *ext = strchr(dp->d_name, '.');
            if (ext != NULL && strncmp(ext, ".jack", strlen(".jack")) == 0) {
                
                strncpy(input, filename, PATH_MAX - 1);
                strncat(input, "/", 2);
                strncat(input, dp->d_name, PATH_MAX - 1);
                input[PATH_MAX - 1] = '\0';
        
                if (verbose)
                    fprintf(stderr, "\t* Compiling File: %s\n", input);
                char outCompile[PATH_MAX + 1] = {0};
                memcpy(outCompile, input, strlen(input) - strlen(ext));
                strcat(outCompile, ".vm");
                outCompile[strlen(input)] = '\0';

                if (verbose)
                    fprintf(stderr, "\t* Compiler output file: %s\n", outCompile);
                compiler->outputFile = fopen(outCompile, "w");
    
                ASSERT(compiler->outputFile, "cannot open compile output file to write")

                FILE *inputFile = fopen(input, "r");
                ASSERT(inputFile, "cannot open input file to read")
  
                char *out = parse(inputFile);
                compiler->tokens = getTokens(out);
                
                compileClass(compiler);

                if (verbose)
                    fprintf(stderr, "* Freeing all data used\n");

                fclose(inputFile);
                fclose(compiler->outputFile);
                freeList_Token(compiler->tokens);
                free(out);
            }        
        }
        closedir(dir);
    }
    if (OS) {
        stat("OS", &path_stat);
        compiler->outputFile = NULL;
        memcpy(filenameCPY, filename, strlen(filename));
        filenameCPY[strlen(filename)] = '\0';
        if (S_ISDIR(path_stat.st_mode)) {
        
            DIR *dir = opendir("OS");
            struct dirent *dp;
            ASSERT(dir, "directory not open");

            while ((dp = readdir(dir)) != NULL) {
                char input[PATH_MAX ] = {0};
                memcpy(&input, "OS", strlen("OS"));
                char *ext = strchr(dp->d_name, '.');
                if (ext != NULL && strncmp(ext, ".jack", strlen(".jack")) == 0) {
                
                    strncpy(input, "OS", PATH_MAX - 1);
                    strncat(input, "/", 2);
                    strncat(input, dp->d_name, PATH_MAX - 1);
                    input[PATH_MAX - 1] = '\0';
        
                    if (verbose)
                        fprintf(stderr, "\t* Compiling File: %s\n", input);

                    FILE *inputFile = fopen(input, "r");
                    ASSERT(inputFile, "cannot open input file to read")
  
                    char outCompile[PATH_MAX + 1] = {0};
                    strncpy(input, filename, PATH_MAX - 1);
                    strncat(input, "/", 2);
                    strncat(input, dp->d_name, PATH_MAX - 1);
                    input[PATH_MAX - 1] = '\0';
                    memcpy(outCompile, input, strlen(input) - strlen(ext));
                    strcat(outCompile, ".vm");
                    outCompile[strlen(input)] = '\0';

                    if (verbose)
                        fprintf(stderr, "\t* Compiler output file: %s\n", outCompile);
                    compiler->outputFile = fopen(outCompile, "w");
    
                    ASSERT(compiler->outputFile, "cannot open compile output file to write")

                    char *out = parse(inputFile);
                    compiler->tokens = getTokens(out);
                
                    compileClass(compiler);

                    if (verbose)
                        fprintf(stderr, "* Freeing all data used\n");

                    fclose(inputFile);
                    fclose(compiler->outputFile);
                    freeList_Token(compiler->tokens);
                    free(out);
                }        
            }
            closedir(dir);
        }
    }
 
    free(filenameCPY);
    freeList_symbolTable(compiler->classTable);
    //freeList_symbolTable(compiler->subroutineTable);
    free(compiler);
    return 0;
}
