#include <plisp/print.h>

void plisp_write(FILE *f, plisp_t *obj) {
    if (obj->tid == TID_CONS) {
        fprintf(f, "(");
        plisp_write(f, obj->data.cons.car);
        fprintf(f, " . ");
        plisp_write(f, obj->data.cons.cdr);
        fprintf(f, ")");
    } else if (obj->tid == TID_INT) {
        fprintf(f, "%li", obj->data.i);
    } else if (obj->tid == TID_NIL) {
        fprintf(f, "NIL");
    } else {
        fprintf(f, "#?");
    }
}
