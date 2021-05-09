#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h> 

#define ASSERT(arg, err) if (!(arg)) {fprintf(stderr,"\033[31mError, %s, exiting...\033[0m\n", err); exit(1);} 

#define STREQUALS(arg1, arg2) strncmp((arg1), (arg2), strlen((arg1))) == 0

#define WRITE(...) fprintf(translator->outputFile, __VA_ARGS__);

#define MAX_LINE_LENGTH 256

#define MAX_COMMAND_LENGTH 3

#define MAX_FILES_IN_DIR 64

#endif
