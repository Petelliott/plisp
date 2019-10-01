#ifndef PLISP_BUILTIN_H
#define PLISP_BUILTIN_H

#include <plisp/object.h>

#include <stdbool.h>

bool plisp_c_eq(plisp_t *a, plisp_t *b);
bool plisp_c_eqv(plisp_t *a, plisp_t *b);
bool plisp_c_equal(plisp_t *a, plisp_t *b);

plisp_t *plisp_eq(plisp_t *a, plisp_t *b);
plisp_t *plisp_eqv(plisp_t *a, plisp_t *b);
plisp_t *plisp_equal(plisp_t *a, plisp_t *b);

plisp_t *plisp_cons(plisp_t *car, plisp_t *cdr);

plisp_t *plisp_car(plisp_t *obj);
plisp_t *plisp_cdr(plisp_t *obj);

plisp_t *plisp_cddr(plisp_t *obj);
plisp_t *plisp_cdddr(plisp_t *obj);
plisp_t *plisp_cadr(plisp_t *obj);
plisp_t *plisp_caddr(plisp_t *obj);
plisp_t *plisp_cadddr(plisp_t *obj);

plisp_t *plisp_plus(plisp_t *lst);
plisp_t *plisp_minus(plisp_t *lst);

bool plisp_c_null(plisp_t *obj);
plisp_t *plisp_null(plisp_t *obj);
bool plisp_c_not(plisp_t *obj);
plisp_t *plisp_not(plisp_t *obj);

plisp_t *plisp_write(plisp_t *obj);
plisp_t *plisp_newline();

size_t plisp_c_length(plisp_t *obj);
plisp_t *plisp_length(plisp_t *obj);

void plisp_builtin_init(void);

#endif
