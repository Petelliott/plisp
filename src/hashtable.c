#include <plisp/hashtable.h>
#include <plisp/object.h>
#include <plisp/gc.h>

#include <string.h>
#include <plisp/superfasthash.h>

const char *ht_name = "hash-table";

const size_t HT_DEFAULT_SIZE = 64;

plisp_t *plisp_make_hashtable(enum HT_KEY keytype) {
    struct pl_hashtable *ht = plisp_alloc(sizeof(struct pl_hashtable));
    ht->len = HT_DEFAULT_SIZE;
    ht->elems = 0;
    ht->buckets = plisp_alloc(sizeof(struct ht_bucket *) * ht->len);
    memset(ht->buckets, 0, sizeof(struct ht_bucket *) * ht->len);

    plisp_t *ht_obj = alloc_plisp_obj();
    ht_obj->tid = 0;
    ht_obj->data.custom.nameid = ht_name;
    ht_obj->data.custom.obj = ht;

    return ht_obj;
}

plisp_t *plisp_hashtable_find(plisp_t *ht, plisp_t *key);
plisp_t *plisp_hashtable_insert(plisp_t *ht, plisp_t *key, plisp_t *value);
plisp_t *plisp_hashtable_delete(plisp_t *ht, plisp_t *key);

plisp_hash_t plisp_hash_eq(plisp_t *obj) {
    return SuperFastHash(&obj, sizeof(obj));
}

plisp_hash_t plisp_hash_eqv(plisp_t *obj) {
    // this weird padding zeroing is necessary
    // C does not verify that data in padding is zero
    if (obj->tid == TID_INT || obj->tid == TID_FLOAT || obj->tid == TID_CUSTOM) {
        plisp_t pl;
        memset(&pl, 0, sizeof(pl));
        pl.tid = obj->tid;

        if (obj->tid == TID_INT) {
            pl.data.i = obj->data.i;
        } else if (obj->tid == TID_FLOAT) {
            pl.data.f = obj->data.f;
        } else if (obj->tid == TID_CUSTOM) {
            pl.data.custom.nameid = obj->data.custom.nameid;
            pl.data.custom.obj = obj->data.custom.obj;
        }

        return SuperFastHash(&pl, sizeof(pl));
    }

    return plisp_hash_eq(obj);
}

plisp_hash_t plisp_hash_equal(plisp_t *obj) {
    if (obj->tid == TID_STRING || obj->tid == TID_SYMBOL) {
        return SuperFastHash(obj->data.string.base, obj->data.string.len);
    } else if (obj->tid == TID_CONS) {
        return plisp_hash_equal(obj->data.cons.car) +
               plisp_hash_equal(obj->data.cons.cdr);
    }

    return plisp_hash_eqv(obj);
}
