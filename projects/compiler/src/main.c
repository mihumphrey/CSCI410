#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>  
#include <sys/wait.h>
#include <string.h>

#define ASSERT(arg, err) if (!(arg)) {fprintf(stderr,"\033[31mError, %s, exiting...\033[0m\n", err); exit(1);} 

int main(int argc, char **argv) {
    bool verbose = false;
    bool keep = false;
    char *filename;

    int opt;
    while((opt = getopt(argc, argv, "kv?:")) != -1) {
        switch(opt) {
        case 'k': keep = true; break;
        case 'v': verbose = true; break;
        default:
            fprintf(stderr, "Usage: %s [-kv] [directory]\n", argv[0]);
        }
    }
    
    filename = argv[optind];

    char command[1024];
    system("rm -rf temp");

    if (keep)
        system("mkdir temp");

    sprintf(command, "./bin/compiler %s %s", filename, verbose ? "-v" : "");
    system(command);
    sprintf(command, "./bin/analyzer %s %s", filename, verbose ? "-v" : "");
    system(command);
    sprintf(command, "./bin/translator %s %s", filename, verbose ? "-v" : "");
    system(command);
    sprintf(command, "./bin/assembler %s %s", filename, verbose ? "-v" : "");
    system(command);
    sprintf(command, "%s %s/*.vm %s", keep ? "mv" : "rm", filename, keep ? "temp/" : "");
    system(command);
    sprintf(command, "%s %s/*.xml %s", keep ? "mv" : "rm", filename, keep ? "temp/" : "");
    system(command);
    sprintf(command, "%s %s/*.asm %s", keep ? "mv" : "rm", filename, keep ? "temp/" : "");
    system(command);
}
