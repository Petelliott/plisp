#include <stdio.h>

#include <plisp/gc.h>
#include <plisp/read.h>
#include <plisp/print.h>
#include <plisp/hashtable.h>
#include <plisp/builtin.h>

int main() {
    plisp_GC_init();

    plisp_t *ht = plisp_make_hashtable(HT_EQUAL);
    plisp_hashtable_insert(ht, plisp_make_int(1), plisp_make_int(2));
    plisp_hashtable_insert(ht, plisp_make_int(3), plisp_make_int(4));
    plisp_hashtable_insert(ht, plisp_make_int(1), plisp_make_int(8));
    plisp_write(stdout, plisp_hashtable_find(ht, plisp_make_int(1)));
    putchar('\n');
    plisp_write(stdout, plisp_hashtable_find(ht, plisp_make_int(3)));
    putchar('\n');

    //plisp_t *obj = plisp_read(stdin);
    plisp_write(stdout, ht);
    putchar('\n');

    return 0;
}
