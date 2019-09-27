#ifndef PLISP_OBJECT_H
#define PLISP_OBJECT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum TYPEID {
    TID_NIL,
    TID_CONS,
    TID_INT,
    TID_BOOL,
    TID_FLOAT,
    TID_STRING,
    TID_SYMBOL,
    TID_CUSTOM,
};

typedef struct pl_object plisp_t;

struct pl_cons {
    plisp_t *car;
    plisp_t *cdr;
};

struct pl_string {
    size_t len;
    char *base;
};

struct pl_custom {
    // name id serves as both a name and the id.
    // all objects of the same type must have the exact same nameid ptr.
    const char *nameid;
    void *obj;
};

union pl_obj_data {
    struct pl_cons cons;
    long i;
    bool b;
    double f;
    struct pl_string string;
    struct pl_custom  custom;
};

struct pl_object {
    int tid;
    union pl_obj_data data;
};

plisp_t *plist_make_nil(void);
plisp_t *plisp_make_cons(plisp_t *car, plisp_t *cdr);
plisp_t *plisp_make_int(long i);
plisp_t *plisp_make_bool(bool b);
plisp_t *plisp_make_float(double f);
plisp_t *plisp_make_string(const char *str);
plisp_t *plisp_make_symbol(const char *name);
plisp_t *plisp_make_custom(const char *name, void *obj);

#endif
