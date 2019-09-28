#include <plisp/hashtable.h>
#include <plisp/object.h>

#include <string.h>
#include <plisp/superfasthash.h>

plisp_t *plisp_make_hashtable(enum HT_KEY keytype);

plisp_t *plisp_hashtable_find(plisp_t *ht, plisp_t *key);
plisp_t *plisp_hashtable_insert(plisp_t *ht, plisp_t *key, plisp_t *value);

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
plisp_hash_t plisp_hash_equal(plisp_t *obj);
