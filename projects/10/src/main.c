#include "includes.h"
#include "parser.h"
#include "tokenizer.h"

int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "Too few arguments -- must contain FILENAME, may contain -v(erbose), -t(okenize), -p(arse) -- ")
    char *filename = argv[1];
    ASSERT(filename, "Input filename not found")

    struct stat path_stat;
    stat(filename, &path_stat);
    FILE *tokenOutputFile = NULL;
    FILE *parseOutputFile = NULL;
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
                char outToken[PATH_MAX + 1] = {0};
                char outParse[PATH_MAX + 1] = {0};
                strncpy(outToken, input, strlen(input) - strlen(ext));
                strncpy(outParse, input, strlen(input) - strlen(ext));
                strcat(outToken, "T.xml");
                strcat(outParse, ".xml");
                outToken[strlen(input)] = '\0';
                outParse[strlen(input)] = '\0';

                tokenOutputFile = fopen(outToken, "w");
                parseOutputFile = fopen(outParse, "w");
    
                ASSERT(tokenOutputFile, "cannot open token output file to write")
                ASSERT(parseOutputFile, "cannot open parse output file to write")

                FILE *inputFile = fopen(input, "r");
                ASSERT(inputFile, "cannot open input file to read")
  
                char *out = parse(inputFile, tokenOutputFile);
                printf("OUT: %s\n\n\n", out);
                TokenList *tokens = getTokens(out);
                writeTokens(tokens, tokenOutputFile);

            }        
        }
    }
    return 0;
}
