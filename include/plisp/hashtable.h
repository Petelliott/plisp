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

extern const char *ht_name;

struct ht_bucket {
    plisp_t *key;
    void *value;
};

struct pl_hashtable {
    enum HT_KEY keytype;
    struct ht_bucket *buckets;
    size_t len;
    size_t elems;
};

plisp_t *plisp_make_hashtable(enum HT_KEY keytype);
plisp_t *plisp_hashtable_clone(plisp_t *ht);

void *plisp_hashtable_find(plisp_t *ht, plisp_t *key);
void plisp_hashtable_insert(plisp_t *ht, plisp_t *key, void *value);
void *plisp_hashtable_delete(plisp_t *ht, plisp_t *key);

plisp_hash_t plisp_hash(plisp_t *obj, enum HT_KEY keytype);
plisp_hash_t plisp_hash_eq(plisp_t *obj);
plisp_hash_t plisp_hash_eqv(plisp_t *obj);
plisp_hash_t plisp_hash_equal(plisp_t *obj);

bool plisp_compare(plisp_t *obja, plisp_t *objb, enum HT_KEY keytype);

#endif
