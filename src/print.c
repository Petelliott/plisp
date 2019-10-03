#include <plisp/print.h>

#include <ctype.h>

void plisp_c_write_cons(FILE *f, plisp_t *obj) {
    plisp_t *car = obj->data.cons.car;
    plisp_t *cdr = obj->data.cons.cdr;

    plisp_c_write(f, car);

    if (cdr->tid == TID_NIL) {
        return;
    } else if (cdr->tid == TID_CONS) {
        fprintf(f, " ");
        plisp_c_write_cons(f, cdr);
    } else {
        fprintf(f, " . ");
        plisp_c_write(f, cdr);
    }
}

static void plisp_c_write_string(FILE *f, plisp_t *obj) {
    fputc('"',f);
    for (size_t i = 0; i < obj->data.string.len; ++i) {
        char ch = obj->data.string.base[i];
        if (ch == '"') {
            fprintf(f, "\\\"");
        } else if (ch == '\\') {
            fprintf(f, "\\\\");
        } else if (ch == '\n') {
            fprintf(f, "\\n");
        } else if (ch == '\r') {
            fprintf(f, "\\r");
        } else if (ch == '\e') {
            fprintf(f, "\\e");
        } else if (ch == '\t') {
            fprintf(f, "\\t");
        } else if (isprint(ch)) {
            fputc(ch, f);
        } else {
            fprintf(f, "\\x%X", (unsigned char)ch);
        }
    }
    fputc('"', f);
}

void plisp_c_write(FILE *f, plisp_t *obj) {
    if (obj->tid == TID_CONS) {
        fprintf(f, "(");
        plisp_c_write_cons(f, obj);
        fprintf(f, ")");
    } else if (obj->tid == TID_INT) {
        fprintf(f, "%li", obj->data.i);
    } else if (obj->tid == TID_BOOL) {
        if (obj->data.b) {
            fprintf(f, "#t");
        } else {
            fprintf(f, "#f");
        }
    } else if (obj->tid == TID_STRING) {
        plisp_c_write_string(f, obj);
    } else if (obj->tid == TID_SYMBOL) {
        fprintf(f, "%s", obj->data.string.base);
    } else if (obj->tid == TID_NIL) {
        fprintf(f, "NIL");
    } else if (obj->tid == TID_CUSTOM) {
        fprintf(f, "#<%s>", obj->data.custom.nameid);
    } else if (obj->tid == TID_PRIM_FN) {
        fprintf(f, "#<builtin function>");
    } else if (obj->tid == TID_CLOSURE) {
        fprintf(f, "#<interpreted closure>");
    } else if (obj->tid == TID_COMPILED) {
        fprintf(f, "#<compiled closure>");
    } else {
        fprintf(f, "#?");
    }
}
