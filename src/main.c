#include <stdio.h>

#include <plisp/gc.h>
#include <plisp/read.h>
#include <plisp/print.h>

int main() {
    plisp_GC_init();

    plisp_t *obj = plisp_read(stdin);
    plisp_write(stdout, obj);
    putchar('\n');

    return 0;
}
