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
    bool src = false;
    bool compiler = false, analyzer = false, translator = false, assembler = false;
    bool all = true;
    bool help = false;
    char *filename;

    int opt;
    while((opt = getopt(argc, argv, "hkxvscta?:")) != -1) {
        switch(opt) {
        case 'k': keep = true; break;
        case 'v': verbose = true; break;
        case 'c': compiler = true; all = false; break;
        case 'x': analyzer = true; all = false; break;
        case 't': translator = true; all = false; break;
        case 'a': assembler = true; all = false; break;
        case 's': src = true; break;
        case 'h': help = true; break;
        default:
            printf("Usage: %s [args] [directory]\n\n", argv[0]);

            printf("Args: \n");
            printf("\t -k(eep)           Keeps all intermediate files (in given directory)\n");
            printf("\t -v(erbose)        Gives a verbose output of compiler (**WARNING** this may produce > 1M output lines for a large program with the OS loaded)\n");
            printf("\t -s(rc)            Compiles only the source files given (ignores the OS files)\n");
            printf("\t -c(ompiler)       Runs only the compiler on the given directory (.jack -> .vm)\n");
            printf("\t -x(ml analyzer)   Runs only the syntax analyzer on the given directory (.jack -> .xml)\n");
            printf("\t -t(ranslator)     Runs only the translator on the given directory (.vm -> .asm)\n");
            printf("\t -a(ssembler)      Runs only the assembler on the given directory (.asm -> .hack)\n");
            printf("\t -h(elp)           Displays this help menu\n");
            exit(1);
        }
    }

    if (help) {
        printf("\nAuthor: Michael Humphrey\n");
        printf("Professor: Dr. Christopher Painter-Wakefield\n");
        printf("Course: CSCI410\n");
        printf("Date: Spring 2021\n\n");

        printf("This compiler was built to compile nand2tetris .jack files to .hack files\n");
        printf("This is done by compiling each .jack file in the given directory into .vm files, which are then translated into one .asm file, which is finally assembled into ont .hack file\n");
        printf("This compiler may be run with any of the following arguments, but defaults by compiling all the way from .jack to .hack, as well as the syntax analyzer.\n");
        printf("By default, it does not keep intermediate files, and grabs all .jack files located in the OS directory of the cwd to compile\n\n");

        printf("Usage: %s [args] [directory]\n\n", argv[0]);

        printf("Args: \n");
        printf("\t -k(eep)           Keeps all intermediate files (in given directory)\n");
        printf("\t -v(erbose)        Gives a verbose output of compiler (**WARNING** this may produce > 1M output lines for a large program with the OS loaded)\n");
        printf("\t -s(rc)            Compiles only the source files given (ignores the OS files)\n");
        printf("\t -c(ompiler)       Runs only the compiler on the given directory (.jack -> .vm)\n");
        printf("\t -x(ml analyzer)   Runs only the syntax analyzer on the given directory (.jack -> .xml)\n");
        printf("\t -t(ranslator)     Runs only the translator on the given directory (.vm -> .asm)\n");
        printf("\t -a(ssembler)      Runs only the assembler on the given directory (.asm -> .hack)\n");
        printf("\t -h(elp)           Displays this help menu\n");
        exit(0);
    }
    
    filename = argv[optind];
    if (!filename) {
        fprintf(stderr, "No directory specified, exiting...\n");    
        printf("Usage: %s [args] [directory]\n\n", argv[0]);

        printf("Args: \n");
        printf("\t -k(eep)           Keeps all intermediate files (in given directory)\n");
        printf("\t -v(erbose)        Gives a verbose output of compiler (**WARNING** this may produce > 1M output lines for a large program with the OS loaded)\n");
        printf("\t -s(rc)            Compiles only the source files given (ignores the OS files)\n");
        printf("\t -c(ompiler)       Runs only the compiler on the given directory (.jack -> .vm)\n");
        printf("\t -x(ml analyzer)   Runs only the syntax analyzer on the given directory (.jack -> .xml)\n");
        printf("\t -t(ranslator)     Runs only the translator on the given directory (.vm -> .asm)\n");
        printf("\t -a(ssembler)      Runs only the assembler on the given directory (.asm -> .hack)\n");
        printf("\t -h(elp)           Displays this help menu\n");
 
        exit(-1);
    }

    char command[1024];
    if (all) {
    
        if (src)
            sprintf(command, "./bin/compiler %s %s -s", filename, verbose ? "-v" : "");
        else
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
        if (src)
            sprintf(command, "./bin/compiler %s %s -s", filename, verbose ? "-v" : "");
        else
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
