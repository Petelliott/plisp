#include <plisp/hashtable.h>
#include <plisp/object.h>
#include <plisp/gc.h>
#include <plisp/builtin.h>
#include <plisp/superfasthash.h>

#include <string.h>
#include <assert.h>
#include <stdio.h>

const char *ht_name = "hash-table";

const size_t HT_DEFAULT_SIZE = 31;

struct pl_hashtable **plisp_object_to_hashtable(plisp_t *obj) {
    assert(obj->tid == TID_CUSTOM);
    assert(obj->data.custom.nameid == ht_name);

    return (struct pl_hashtable **) &(obj->data.custom.obj);
}

plisp_t *plisp_make_hashtable(enum HT_KEY keytype) {
    struct pl_hashtable *ht = plisp_alloc(sizeof(struct pl_hashtable));
    ht->len = HT_DEFAULT_SIZE;
    ht->elems = 0;
    ht->keytype = keytype;
    ht->buckets = plisp_alloc(sizeof(struct ht_bucket) * ht->len);
    memset(ht->buckets, 0, sizeof(struct ht_bucket) * ht->len);

    plisp_t *ht_obj = alloc_plisp_obj();
    ht_obj->tid = TID_CUSTOM;
    ht_obj->data.custom.nameid = ht_name;
    ht_obj->data.custom.obj = ht;

    return ht_obj;
}

plisp_t *plisp_hashtable_clone(plisp_t *ht) {
    struct pl_hashtable *table = *plisp_object_to_hashtable(ht);

    struct pl_hashtable *nht = plisp_alloc(sizeof(struct pl_hashtable));
    nht->len = table->len;
    nht->elems = table->elems;
    nht->keytype = table->keytype;
    nht->buckets = plisp_alloc(sizeof(struct ht_bucket) * table->len);

    memcpy(nht->buckets, table->buckets, sizeof(struct ht_bucket) * table->len);

    plisp_t *ht_obj = alloc_plisp_obj();
    ht_obj->tid = TID_CUSTOM;
    ht_obj->data.custom.nameid = ht_name;
    ht_obj->data.custom.obj = nht;

    return ht_obj;
}

void *plisp_hashtable_find(plisp_t *ht, plisp_t *key) {
    struct pl_hashtable *table = *plisp_object_to_hashtable(ht);

    plisp_hash_t hash = plisp_hash(key, table->keytype) % table->len;

    for (plisp_hash_t i = hash;; i = (i+1) % table->len) {
        struct ht_bucket *bkt = &(table->buckets[i]);
        if (bkt->key == NULL) {
            break;
        } else if (plisp_compare(key, bkt->key, table->keytype)) {
            assert(bkt->value != NULL);
            return bkt->value;
        }
    }

    return NULL;
}

static void plisp_ht_insert(struct pl_hashtable **tp, plisp_t *key, void *value) {
    assert(value != NULL);
    struct pl_hashtable *table = *tp;

    if (3*(table->elems+1) >= 2*table->len) {
        struct pl_hashtable *nht = plisp_alloc(sizeof(struct pl_hashtable));
        nht->len = table->len * 2;
        nht->elems = 0;
        nht->keytype = table->keytype;
        nht->buckets = plisp_alloc(sizeof(struct ht_bucket) * nht->len);
        memset(nht->buckets, 0, sizeof(struct ht_bucket) * nht->len);

        for (plisp_hash_t i = 0; i < table->len; ++i) {
            struct ht_bucket *bkt = &(table->buckets[i]);
            if (bkt->key != NULL) {
                plisp_ht_insert(&nht, bkt->key, bkt->value);
            }
        }

        *tp = nht;
        table = *tp;
    }

    plisp_hash_t hash = plisp_hash(key, table->keytype) % table->len;

    for (plisp_hash_t i = hash;; i = (i+1) % table->len) {
        struct ht_bucket *bkt = &(table->buckets[i]);
        if (bkt->key == NULL) {
            bkt->key = key;
            bkt->value = value;
            table->elems++;
            break;
        } else if (plisp_compare(key, bkt->key, table->keytype)) {
            bkt->value = value;
            break;
        }
    }
}

void plisp_hashtable_insert(plisp_t *ht, plisp_t *key, void *value) {
    struct pl_hashtable **table = plisp_object_to_hashtable(ht);
    plisp_ht_insert(table, key, value);
}

void *plisp_hashtable_delete(plisp_t *ht, plisp_t *key);

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
