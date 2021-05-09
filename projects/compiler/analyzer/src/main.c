#include "includes.h"
#include "parser.h"
#include "tokenizer.h"
#include "analyzer.h"

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

    if (argc > 2 && STREQUALS(argv[2], "-v")) verbose = true;    

    char *filename = argv[1];
    ASSERT(filename, "Input filename not found")

    if (verbose)
        fprintf(stderr, "* Compiling: %s\n", filename);
    struct stat path_stat;
    stat(filename, &path_stat);
    FILE *tokenOutputFile = NULL;
    FILE *analyzeOutputFile = NULL;
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
                char outToken[PATH_MAX + 1] = {0};
                char outAnalyze[PATH_MAX + 1] = {0};
                memcpy(outToken, input, strlen(input) - strlen(ext));
                memcpy(outAnalyze, input, strlen(input) - strlen(ext));
                strcat(outToken, "T.xml");
                strcat(outAnalyze, ".xml");
                outToken[strlen(input)] = '\0';
                outAnalyze[strlen(input)] = '\0';

                if (verbose)
                    fprintf(stderr, "\t* Tokenizer output file: %s\n", outToken);
                if (verbose)
                    fprintf(stderr, "\t* Analyzer output file: %s\n", outAnalyze);
                tokenOutputFile = fopen(outToken, "w");
                analyzeOutputFile = fopen(outAnalyze, "w");
    
                ASSERT(tokenOutputFile, "cannot open token output file to write")
                ASSERT(analyzeOutputFile, "cannot open analyze output file to write")

                FILE *inputFile = fopen(input, "r");
                ASSERT(inputFile, "cannot open input file to read")
  
                char *out = parse(inputFile);
                TokenList *tokens = getTokens(out);
                writeTokens(tokens, tokenOutputFile);

                analyzeClass(tokens, analyzeOutputFile);

                if (verbose)
                    fprintf(stderr, "* Freeing all data used\n");

                fclose(inputFile);
                fclose(tokenOutputFile);
                fclose(analyzeOutputFile);
                freeList_Token(tokens);
                free(out);
            }        
        }
        closedir(dir);
    }
    free(filenameCPY);
    return 0;
}
