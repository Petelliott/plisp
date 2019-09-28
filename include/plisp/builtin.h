#ifndef PLISP_BUILTIN_H
#define PLISP_BUILTIN_H

#include <plisp/object.h>

#include <stdbool.h>

bool plisp_c_eq(plisp_t *a, plisp_t *b);
bool plisp_c_eqv(plisp_t *a, plisp_t *b);
bool plisp_c_equal(plisp_t *a, plisp_t *b);

#endif
