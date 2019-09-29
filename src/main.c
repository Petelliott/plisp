#include <stdio.h>

#include <plisp/gc.h>
#include <plisp/read.h>
#include <plisp/print.h>
#include <plisp/hashtable.h>
#include <plisp/builtin.h>

int main() {
    plisp_GC_init();
    plisp_init_reader();

    plisp_t *obja = plisp_read(stdin);
    plisp_t *objb = plisp_read(stdin);
    //plisp_write(stdout, obj);
    //putchar('\n');
    printf("%d\n", plisp_c_equal(obja, objb));

    return 0;
}
