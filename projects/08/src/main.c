#include "translator.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
# include <begemot.h>
bool verbose = false;

int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "wrong number of args -- must be filename and -v for verbose")    
    char *filename = argv[1];

    struct stat path_stat;
    stat(filename, &path_stat);
    if (S_ISDIR(path_stat.st_mode)) {
        DIR *dir = opendir(filename);
        struct dirent *dp;
        ASSERT(dir, "directory not open");
        while ((dp = readdir(dir)) != NULL) {
            printf("FILENAME: %s\n", dp->d_name);
            char *ext = strnchr(dp->d_name, ".", strlen(dp->d_name));
            if (strncmp(ext, ".vm", strlen(ext)) == 0) {
                printf("FILENAME: %s\n", dp->d_name);
    
            }
        }
    } else {
        
    }


    if (argc > 2)
        if (strncmp(argv[2], "-v", strlen(argv[2])) == 0)
            verbose = true;
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
