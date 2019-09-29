#include <plisp/eval.h>
#include <plisp/hashtable.h>
#include <plisp/read.h>

#include <stdio.h>

plisp_t *toplevel = NULL;

void plisp_init_eval(void) {
    toplevel = plisp_make_hashtable(HT_EQ);
}

void plisp_toplevel_def(plisp_t *sym, plisp_t *obj) {
    plisp_hashtable_insert(toplevel, sym, obj);
}

void plisp_def_subr(const char *name, void *fp, short nargs, bool rest) {
    plisp_toplevel_def(make_interned_symbol(name), plisp_make_prim_fn(fp, nargs, rest));
}

plisp_t *plisp_eval(plisp_t *form) {
    if (form->tid == TID_CONS) {
        //TODO.
        return form;
    } else if (form->tid == TID_SYMBOL) {
        plisp_t *ret = plisp_hashtable_find(toplevel, form);
        if (ret == NULL) {
            fprintf(stderr, "variable '%s' not found\n", form->data.string.base);
        }

        return ret;
    }

    return form;
}

plisp_t *plisp_call_prim(plisp_t *arg, plisp_t *rest);
