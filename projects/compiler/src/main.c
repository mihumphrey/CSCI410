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
    bool compiler = false, analyzer = false, translator = false, assembler = false;
    bool all = true;
    char *filename;

    int opt;
    while((opt = getopt(argc, argv, "kvscta?:")) != -1) {
        switch(opt) {
        case 'k': keep = true; break;
        case 'v': verbose = true; break;
        case 'c': compiler = true; all = false; break;
        case 's': analyzer = true; all = false; break;
        case 't': translator = true; all = false; break;
        case 'a': assembler= true; all = false; break;
        default:
            fprintf(stderr, "Usage: %s [-kv] [directory]\n", argv[0]);
        }
    }
    
    filename = argv[optind];

    char command[1024];
    if (all) {
        sprintf(command, "./bin/compiler %s %s", filename, verbose ? "-v" : "");
        system(command);
        sprintf(command, "./bin/analyzer %s %s", filename, verbose ? "-v" : "");
        system(command);
        sprintf(command, "./bin/translator %s %s", filename, verbose ? "-v" : "");
        system(command);
        sprintf(command, "./bin/assembler %s %s", filename, verbose ? "-v" : "");
        system(command);
    }

    if (compiler) {
        sprintf(command, "./bin/compiler %s %s", filename, verbose ? "-v" : "");
        system(command);
    }

    if (analyzer) {
        sprintf(command, "./bin/analyzer %s %s", filename, verbose ? "-v" : "");
        system(command);
    }

    if (translator) {
        sprintf(command, "./bin/translator %s %s", filename, verbose ? "-v" : "");
        system(command);
    }

    if (assembler) {
        sprintf(command, "./bin/assembler %s %s", filename, verbose ? "-v" : "");
        system(command);
    }

    if (!keep) {
        sprintf(command, "rm -f %s/*.vm", filename);
        system(command);
        sprintf(command, "rm -f %s/*.xml", filename);
        system(command);
        sprintf(command, "rm -f %s/*.asm", filename);
        system(command);
    }
}
