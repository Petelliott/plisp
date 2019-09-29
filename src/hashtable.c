#include <plisp/hashtable.h>
#include <plisp/object.h>
#include <plisp/gc.h>
#include <plisp/builtin.h>
#include <plisp/superfasthash.h>

#include <string.h>
#include <assert.h>

const char *ht_name = "hash-table";

const size_t HT_DEFAULT_SIZE = 64;

struct pl_hashtable *plisp_object_to_hashtable(plisp_t *obj) {
    assert(obj->tid == TID_CUSTOM);
    assert(obj->data.custom.nameid == ht_name);

    return obj->data.custom.obj;
}

plisp_t *plisp_make_hashtable(enum HT_KEY keytype) {
    struct pl_hashtable *ht = plisp_alloc(sizeof(struct pl_hashtable));
    ht->len = HT_DEFAULT_SIZE;
    ht->elems = 0;
    ht->keytype = keytype;
    ht->buckets = plisp_alloc(sizeof(struct ht_bucket *) * ht->len);
    memset(ht->buckets, 0, sizeof(struct ht_bucket *) * ht->len);

    plisp_t *ht_obj = alloc_plisp_obj();
    ht_obj->tid = TID_CUSTOM;
    ht_obj->data.custom.nameid = ht_name;
    ht_obj->data.custom.obj = ht;

    return ht_obj;
}

plisp_t *plisp_hashtable_find(plisp_t *ht, plisp_t *key) {
    struct pl_hashtable *table = plisp_object_to_hashtable(ht);

    plisp_hash_t hash = plisp_hash(key, table->keytype) % table->len;

    struct ht_bucket *hb;
    for (hb = table->buckets[hash]; hb != NULL; hb = hb->next) {
        if (plisp_compare(key, hb->key, table->keytype)) {
            return hb->value;
        }
    }

    return NULL;
}

void plisp_hashtable_insert(plisp_t *ht, plisp_t *key, plisp_t *value) {
    struct pl_hashtable *table = plisp_object_to_hashtable(ht);

    //TODO: grow table

    plisp_hash_t hash = plisp_hash(key, table->keytype) % table->len;

    struct ht_bucket **hb;
    for (hb = &(table->buckets[hash]); *hb != NULL; hb = &((*hb)->next)) {
        if (plisp_compare(key, (*hb)->key, table->keytype)) {
            (*hb)->value = value;
            return;
        }
    }
    *hb = plisp_alloc(sizeof(struct ht_bucket));
    (*hb)->key = key;
    (*hb)->value = value;
    (*hb)->next = NULL;
    table->elems++;
}

plisp_t *plisp_hashtable_delete(plisp_t *ht, plisp_t *key);

plisp_hash_t plisp_hash(plisp_t *obj, enum HT_KEY keytype) {
    switch (keytype) {
        case HT_EQ:
            return plisp_hash_eq(obj);
        case HT_EQV:
            return plisp_hash_eqv(obj);
        case HT_EQUAL:
            return plisp_hash_equal(obj);
    }

    return 0;
}

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

bool plisp_compare(plisp_t *obja, plisp_t *objb, enum HT_KEY keytype) {
    switch (keytype) {
        case HT_EQ:
            return plisp_c_eq(obja, objb);
        case HT_EQV:
            return plisp_c_eqv(obja, objb);
        case HT_EQUAL:
            return plisp_c_equal(obja, objb);
    }

    return false;
}
