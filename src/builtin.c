#include <plisp/builtin.h>
#include <plisp/print.h>
#include <plisp/eval.h>

#include <string.h>
#include <stdio.h>


bool plisp_c_eq(plisp_t *a, plisp_t *b) {
    return a == b;
}

bool plisp_c_eqv(plisp_t *a, plisp_t *b) {
    if (plisp_c_eq(a, b)) {
        return true;
    }

    if (a->tid != b->tid) {
        return false;
    }

    if (a->tid == TID_INT) {
        return a->data.i == b->data.i;
    }

    if (a->tid == TID_FLOAT) {
        return a->data.f == b->data.f;
    }

    if (a->tid == TID_CUSTOM) {
        return a->data.custom.nameid == b->data.custom.nameid &&
               a->data.custom.obj == b->data.custom.obj;
    }

    return false;
}

bool plisp_c_equal(plisp_t *a, plisp_t *b) {
    if (plisp_c_eqv(a, b)) {
        return true;
    }

    if (a->tid != b->tid) {
        return false;
    }

    if (a->tid == TID_STRING || a->tid == TID_SYMBOL) {
        if (a->data.string.len != b->data.string.len) {
            return false;
        }
        return strncmp(a->data.string.base, b->data.string.base, a->data.string.len) == 0;
    } else if (a->tid == TID_CONS) {
        return plisp_c_equal(a->data.cons.car, b->data.cons.car) &&
               plisp_c_equal(a->data.cons.cdr, b->data.cons.cdr);
    }

    return false;
}

plisp_t *plisp_eq(plisp_t *a, plisp_t *b) {
    return plisp_make_bool(plisp_c_eq(a, b));
}

plisp_t *plisp_eqv(plisp_t *a, plisp_t *b) {
    return plisp_make_bool(plisp_c_eqv(a, b));
}

plisp_t *plisp_equal(plisp_t *a, plisp_t *b) {
    return plisp_make_bool(plisp_c_equal(a, b));
}

plisp_t *plisp_cons(plisp_t *car, plisp_t *cdr) {
    return plisp_make_cons(car, cdr);
}

plisp_t *plisp_car(plisp_t *obj) {
    if (obj->tid != TID_CONS) {
        plisp_write(stderr, obj);
        fprintf(stderr, " is not a pair\n");
        return plisp_make_nil();
    }

    return obj->data.cons.car;
}

plisp_t *plisp_cdr(plisp_t *obj) {
    if (obj->tid != TID_CONS) {
        plisp_write(stderr, obj);
        fprintf(stderr, " is not a pair\n");
        return plisp_make_nil();
    }

    return obj->data.cons.cdr;
}

plisp_t *plisp_plus(plisp_t *lst);
plisp_t *plisp_minus(plisp_t *lst);

void plisp_builtin_init(void) {
    plisp_def_subr("eq?", plisp_eq, 2, false);
    plisp_def_subr("eqv?", plisp_eqv, 2, false);
    plisp_def_subr("equal?", plisp_equal, 2, false);

    plisp_def_subr("cons", plisp_cons, 2, false);
    plisp_def_subr("car", plisp_car, 1, false);
    plisp_def_subr("cdr", plisp_cdr, 1, false);
}
