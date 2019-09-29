#ifndef PLISP_EVAL_H
#define PLISP_EVAL_H

#include <plisp/object.h>

extern plisp_t *toplevel;

void plisp_init_eval(void);

void plisp_toplevel_def(plisp_t *sym, plisp_t *obj);
void plisp_def_subr(const char *name, void *fp, short nargs, bool rest);

plisp_t *plisp_eval(plisp_t *form);


#endif
