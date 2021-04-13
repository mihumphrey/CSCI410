#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "grammar.h"

#define ASSERT(arg, err) if (!(arg)) {fprintf(stderr,"\033[31mError, %s, exiting...\033[0m\n", err); exit(1);} 

#define STREQUALS(arg1, arg2) strncmp((arg1), (arg2), strlen((arg1))) == 0

#define WRITE(...) fprintf(outputFile, __VA_ARGS__);


#define MAX_LINE_LENGTH 256

#endif
