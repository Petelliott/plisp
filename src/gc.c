#include <plisp/gc.h>
#include <gc.h>

void plisp_GC_init(void) {
    GC_INIT();
}

void *plisp_alloc(size_t len) {
    return GC_MALLOC(len);
}

void *plisp_realloc(void *old, size_t len) {
    return GC_REALLOC(old, len);
}

void *plisp_alloc_atomic(size_t len) {
    return GC_MALLOC_ATOMIC(len);
}

plisp_t *alloc_plisp_obj(void) {
    return plisp_alloc(sizeof(plisp_t));
}
