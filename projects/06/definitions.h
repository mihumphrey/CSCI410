//
// Created by Michael on 2/19/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INC_06_DEFINITIONS_H
#define INC_06_DEFINITIONS_H

#define ASSERT(expr, err) {if (!(expr)){fprintf(stderr, "Error, %s, exiting... \n", err); exit (1);}}

#define MAX_LINE_LENGTH 256


#endif //INC_06_DEFINITIONS_H
