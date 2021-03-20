#include "translator.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h> 
bool verbose = false;

int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "wrong number of args -- must be filename and -v for verbose")    
    if (argc > 2)
        if (strncmp(argv[2], "-v", strlen(argv[2])) == 0)
            verbose = true;
    char *filename = argv[1];
    int labelCounter = 0;
    int ra = 0;

    char *files[MAX_FILES_IN_DIR];
    struct stat path_stat;
    stat(filename, &path_stat);
    int count = 0;
    FILE *outputFile = NULL;
    if (S_ISDIR(path_stat.st_mode)) {
        
        char *out = calloc(1, strlen(filename) + 10);
        strncpy(out, filename, strlen(filename) + 1);
        strcat(out, "/out.asm");
        out[strlen(filename) + 9] = '\0';
        //printf("OUTPUT: %s\n", out);
        outputFile = fopen(out, "w");

        DIR *dir = opendir(filename);
        struct dirent *dp;
        ASSERT(dir, "directory not open");
        WRITE("@256\n")
        WRITE("D=A\n")
        WRITE("@SP\n")
        WRITE("M=D\n")
        char *command[MAX_COMMAND_LENGTH] = {"call", "Sys.init", "0"};
        doCall(command, outputFile, "SYS", &ra);
        while ((dp = readdir(dir)) != NULL) {
            char fullPath[PATH_MAX ] = {0};
            memcpy(&fullPath, filename, strlen(filename));
            char *ext = strchr(dp->d_name, '.');
            if (ext != NULL && strncmp(ext, ".vm", strlen(".vm")) == 0) {

                strncpy(fullPath, filename, PATH_MAX - 1);
                strncat(fullPath, "/", 2);
                strncat(fullPath, dp->d_name, PATH_MAX - 1);
                fullPath[PATH_MAX - 1] = '\0';

                files[count++] = fullPath;
                if (verbose)
                    fprintf(stdout, "FILENAME: %s\n", fullPath);
                
                //fprintf(stderr, "OPENING: %s\n", fullPath);
                FILE *inputFile = fopen(fullPath, "r");
                ASSERT(inputFile, "not open")
                parseCommands(inputFile, outputFile, &labelCounter, &ra);
                fclose(inputFile);
            }
        }
    } else {
        FILE *inputFile = fopen(filename, "r");
        ASSERT(inputFile, "input file not open in main.c")
        int i;
        for (i = 0; i < strlen(filename); i++) {
            if (filename[i] == '.')
                break;
        }
        char *out = calloc(1, i + 5);
        strncpy(out, filename, i + 1);
        strcat(out, "asm");
        out[i + 4] = '\0';

        outputFile = fopen(out, "w");
        ASSERT(outputFile, "output file not open in main.c")
        ASSERT(inputFile, "inputFile not open")
        parseCommands(inputFile, outputFile, &labelCounter, &ra);
        fclose(inputFile);
    }
    WRITE("(END)\n");
    WRITE("@END\n");
    WRITE("0;JMP\n");
    fclose(outputFile);
    return 0;
}
