#ifndef PLISP_HASHTABLE_H
#define PLISP_HASHTABLE_H

#include <plisp/object.h>
#include <stdint.h>

typedef uint32_t plisp_hash_t;

enum HT_KEY {
    HT_EQ,
    HT_EQV,
    HT_EQUAL
};

const char *ht_name = "hash-table";

struct ht_bucket {
    plisp_t *key;
    plisp_t *value;
    struct ht_bucket *next;
};

struct pl_hashtable {
    enum HT_KEY keytype;
    plisp_t **block;
    size_t len;
    size_t elems;
};

plisp_t *plisp_make_hashtable(enum HT_KEY keytype);

plisp_t *plisp_hashtable_find(plisp_t *ht, plisp_t *key);
plisp_t *plisp_hashtable_insert(plisp_t *ht, plisp_t *key, plisp_t *value);

plisp_hash_t plisp_hash_eq(plisp_t *obj);
plisp_hash_t plisp_hash_eqv(plisp_t *obj);
plisp_hash_t plisp_hash_equal(plisp_t *obj);

#endif
