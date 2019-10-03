#ifndef PLISP_READ_H
#define PLISP_READ_H

#include <plisp/object.h>

#include <stdio.h>
#include <stddef.h>

extern plisp_t *intern_table;

void plisp_init_reader(void);

plisp_t *make_interned_symbol(const char *text);

plisp_t *plisp_read(FILE *f);


#endif
