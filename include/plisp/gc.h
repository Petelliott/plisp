#ifndef PLISP_GC_H
#define PLISP_GC_H

#include <plisp/object.h>

void plisp_GC_init(void);

void *plisp_alloc(size_t len);
void *plisp_alloc_atomic(size_t len);

plisp_t *alloc_plisp_obj(void);

#endif
