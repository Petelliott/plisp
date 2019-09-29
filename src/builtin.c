#include <plisp/builtin.h>

#include <string.h>


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
