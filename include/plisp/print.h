#ifndef PLISP_PRINT_H
#define PLISP_PRINT_H

#include <plisp/object.h>

#include <stdio.h>

void plisp_write(FILE *f, plisp_t *obj);

#endif
