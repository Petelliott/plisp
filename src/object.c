#include <plisp/object.h>
#include <plisp/gc.h>

#include <string.h>

plisp_t nil = { .tid = TID_NIL };

plisp_t *plist_make_nil(void) {
    return &nil;
}

plisp_t *plisp_make_cons(plisp_t *car, plisp_t *cdr) {
    plisp_t *obj = alloc_plisp_obj();
    obj->tid = TID_CONS;
    obj->data.cons.car = car;
    obj->data.cons.cdr = cdr;
    return obj;
}

plisp_t *plisp_make_int(long i) {
    plisp_t *obj = alloc_plisp_obj();
    obj->tid = TID_INT;
    obj->data.i = i;
    return obj;
}

plisp_t ht_t = { .tid = TID_BOOL, .data = { .b = true}};
plisp_t ht_f = { .tid = TID_BOOL, .data = { .b = false}};

plisp_t *plisp_make_bool(bool b) {
    return (b)? &ht_t : &ht_f;
}

plisp_t *plisp_make_float(double f) {
    plisp_t *obj = alloc_plisp_obj();
    obj->tid = TID_FLOAT;
    obj->data.f = f;
    return obj;
}

plisp_t *plisp_make_string(const char *str) {
    size_t len = strlen(str);
    char *nstr = plisp_alloc_atomic(len+1);

    memcpy(nstr, str, len+1);
    plisp_t *obj = alloc_plisp_obj();
    obj->tid = TID_STRING;
    obj->data.string.len  = len;
    obj->data.string.base = nstr;

    return obj;
};

plisp_t *plisp_make_symbol(const char *name) {
    plisp_t *obj = plisp_make_string(name);
    obj->tid = TID_SYMBOL;
    return obj;
}

plisp_t *plisp_make_custom(const char *name, void *obj) {
    plisp_t *pobj = alloc_plisp_obj();
    pobj->tid = TID_CUSTOM;
    pobj->data.custom.obj = obj;

    return pobj;
}
