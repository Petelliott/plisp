#include <plisp/builtin.h>
#include <plisp/print.h>
#include <plisp/eval.h>

#include <string.h>
#include <stdio.h>
#include <assert.h>


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
        plisp_c_write(stderr, obj);
        fprintf(stderr, " is not a pair\n");
        return plisp_make_nil();
    }

    return obj->data.cons.car;
}

plisp_t *plisp_cdr(plisp_t *obj) {
    if (obj->tid != TID_CONS) {
        plisp_c_write(stderr, obj);
        fprintf(stderr, " is not a pair\n");
        return plisp_make_nil();
    }

    return obj->data.cons.cdr;
}

plisp_t *plisp_cddr(plisp_t *obj) {
    return plisp_cdr(plisp_cdr(obj));
}

plisp_t *plisp_cdddr(plisp_t *obj) {
    return plisp_cdr(plisp_cddr(obj));
}

plisp_t *plisp_cadr(plisp_t *obj) {
    return plisp_car(plisp_cdr(obj));
}

plisp_t *plisp_caddr(plisp_t *obj) {
    return plisp_car(plisp_cddr(obj));
}

plisp_t *plisp_cadddr(plisp_t *obj) {
    return plisp_car(plisp_cdddr(obj));
}

plisp_t *plisp_plus(plisp_t *obja, plisp_t *objb) {
    assert(obja->tid == TID_INT);
    assert(objb->tid == TID_INT);
    return plisp_make_int(obja->data.i + objb->data.i);
}

plisp_t *plisp_minus(plisp_t *lst);

bool plisp_c_null(plisp_t *obj) {
    return obj->tid == TID_NIL;
}

plisp_t *plisp_null(plisp_t *obj) {
    return plisp_make_bool(plisp_c_null(obj));
}

bool plisp_c_symbolp(plisp_t *obj) {
    return obj->tid == TID_SYMBOL;
}

plisp_t *plisp_symbolp(plisp_t *obj) {
    return plisp_make_bool(plisp_c_symbolp(obj));
}

bool plisp_c_not(plisp_t *obj) {
    return plisp_c_eq(obj, plisp_make_bool(false));
}

plisp_t *plisp_not(plisp_t *obj) {
    return plisp_make_bool(plisp_c_not(obj));
}

plisp_t *plisp_write(plisp_t *obj) {
    plisp_c_write(stdout, obj);
    return plisp_make_nil();
}

plisp_t *plisp_newline() {
    putchar('\n');
    return plisp_make_nil();
}

size_t plisp_c_length(plisp_t *obj) {
    if (obj == NULL || plisp_c_null(obj)) {
        return 0;
    }

    return 1 + plisp_c_length(plisp_cdr(obj));
}

plisp_t *plisp_length(plisp_t *obj) {
    return plisp_make_int(plisp_c_length(obj));
}

plisp_t *plisp_reverse_inner(plisp_t *obj, plisp_t *onto) {
    if (plisp_c_null(obj)) {
        return onto;
    } else {
        return plisp_reverse_inner(plisp_cdr(obj), plisp_cons(plisp_car(obj), onto));
    }
}

plisp_t *plisp_reverse(plisp_t *obj) {
    return plisp_reverse_inner(obj, plisp_make_nil());
}

void plisp_builtin_init(void) {
    plisp_def_subr("eq?", plisp_eq, 2, false);
    plisp_def_subr("eqv?", plisp_eqv, 2, false);
    plisp_def_subr("equal?", plisp_equal, 2, false);

    plisp_def_subr("cons", plisp_cons, 2, false);
    plisp_def_subr("car", plisp_car, 1, false);
    plisp_def_subr("cdr", plisp_cdr, 1, false);

    plisp_def_subr("+", plisp_plus, 2, false);

    plisp_def_subr("cddr", plisp_cddr, 1, false);
    plisp_def_subr("cdddr", plisp_cdddr, 1, false);
    plisp_def_subr("cadr", plisp_cadr, 1, false);
    plisp_def_subr("caddr", plisp_caddr, 1, false);
    plisp_def_subr("cadddr", plisp_cadddr, 1, false);

    plisp_def_subr("null?", plisp_null, 1, false);
    plisp_def_subr("symbol?", plisp_symbolp, 1, false);

    plisp_def_subr("not", plisp_not, 1, false);

    plisp_def_subr("write", plisp_write, 1, false);
    plisp_def_subr("newline", plisp_newline, 0, false);

    plisp_def_subr("length", plisp_length, 1, false);
    plisp_def_subr("reverse", plisp_reverse, 1, false);
}
