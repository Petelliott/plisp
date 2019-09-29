#include <plisp/print.h>

void plisp_write_cons(FILE *f, plisp_t *obj) {
        plisp_t *car = obj->data.cons.car;
        plisp_t *cdr = obj->data.cons.cdr;

        plisp_write(f, car);

        if (cdr->tid == TID_NIL) {
            return;
        } else if (cdr->tid == TID_CONS) {
            fprintf(f, " ");
            plisp_write_cons(f, cdr);
        } else {
            fprintf(f, " . ");
            plisp_write(f, cdr);
        }
}

void plisp_write(FILE *f, plisp_t *obj) {
    if (obj->tid == TID_CONS) {
        fprintf(f, "(");
        plisp_write_cons(f, obj);
        fprintf(f, ")");
    } else if (obj->tid == TID_INT) {
        fprintf(f, "%li", obj->data.i);
    } else if (obj->tid == TID_SYMBOL) {
        fprintf(f, "%s", obj->data.string.base);
    } else if (obj->tid == TID_NIL) {
        fprintf(f, "NIL");
    } else if (obj->tid == TID_CUSTOM) {
        fprintf(f, "#<%s>", obj->data.custom.nameid);
    } else {
        fprintf(f, "#?");
    }
}
