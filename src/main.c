#include <stdio.h>

#include <plisp/gc.h>
#include <plisp/read.h>
#include <plisp/print.h>
#include <plisp/hashtable.h>
#include <plisp/builtin.h>

int main() {
    plisp_GC_init();
    plisp_init_reader();

    while (1) {
        printf("> ");
        plisp_t *obj = plisp_read(stdin);
        if (obj == NULL) {
            putchar('\n');
            break;
        }
        plisp_write(stdout, obj);
        putchar('\n');
    }

    return 0;
}
