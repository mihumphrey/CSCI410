#include "includes.h"
#include "parsing.h"
#include "translator.h"

bool verbose = false;

int main(int argc, char *argv[]) {
    ASSERT(argc >= 2, "wrong number of args -- must be filename and -v for verbose")    
    if (argc > 2)
        if (strncmp(argv[2], "-v", strlen(argv[2])) == 0)
            verbose = true;
    char *filename = argv[1];

    Translator *translator = malloc(sizeof(Translator));

    translator->labelNum = 0;
    translator->ra = 0;
    translator->currClass = "main";
    translator->currFunc = "main";

    struct stat path_stat;
    stat(filename, &path_stat);
    FILE *outputFile = NULL;
    char *filenameCPY = malloc(strlen(filename) + 1);
    memcpy(filenameCPY, filename, strlen(filename));
    filenameCPY[strlen(filename)] = '\0';
    if (S_ISDIR(path_stat.st_mode)) {
        
        char *ptr, *last = NULL;
        ptr = strtok(filenameCPY, "/");
        while (ptr != NULL) {
            last = ptr;
            ptr = strtok(NULL, "/");
        }
        char out[PATH_MAX + 1];
        strcpy(out, filename);
        strcat(out, "/");
        strcat(out, last);
        strcat(out, ".asm");
        out[PATH_MAX] = '\0';
        outputFile = fopen(out, "w");
        translator->outputFile = outputFile;

        DIR *dir = opendir(filename);
        struct dirent *dp;
        ASSERT(dir, "directory not open");
        WRITE("@256\n")
        WRITE("D=A\n")
        WRITE("@SP\n")
        WRITE("M=D\n")
        translator->currCommand[0] = "call";
        translator->currCommand[1] = "Sys.init";
        translator->currCommand[2] = "0";

        doCall(translator);
        while ((dp = readdir(dir)) != NULL) {
            char fullPath[PATH_MAX ] = {0};
            memcpy(&fullPath, filename, strlen(filename));
            char *ext = strchr(dp->d_name, '.');
            if (ext != NULL && strncmp(ext, ".vm", strlen(".vm")) == 0) {

                strncpy(fullPath, filename, PATH_MAX - 1);
                strncat(fullPath, "/", 2);
                strncat(fullPath, dp->d_name, PATH_MAX - 1);
                fullPath[PATH_MAX - 1] = '\0';

                translator->currClass = dp->d_name;

                if (verbose)
                    fprintf(stdout, "\n\nFILENAME: %s\n", fullPath);
                
                FILE *inputFile = fopen(fullPath, "r");
                translator->inputFile = inputFile;
                ASSERT(inputFile, "not open")
                parseCommands(translator);
                fclose(inputFile);
            }
        }
    } else {
        FILE *inputFile = fopen(filename, "r");
        translator->inputFile = inputFile;
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
        translator->outputFile = outputFile;
        ASSERT(outputFile, "output file not open in main.c")
        ASSERT(inputFile, "inputFile not open")
        parseCommands(translator);//inputFile, outputFile, &labelCounter, &ra, "main");
        fclose(inputFile);
    }
    free(filenameCPY);
    free(translator);
    fclose(outputFile);
    
    return 0;
}
