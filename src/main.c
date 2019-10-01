#include <stdio.h>

#include <plisp/gc.h>
#include <plisp/read.h>
#include <plisp/print.h>
#include <plisp/hashtable.h>
#include <plisp/builtin.h>
#include <plisp/eval.h>

int main(int argc, char *argv[]) {
    plisp_GC_init();
    plisp_init_reader();
    plisp_init_eval();
    plisp_builtin_init();

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");

        while (1) {
            plisp_t *obj = plisp_read(file);
            if (obj == NULL) {
                break;
            }

            plisp_eval(obj, toplevel);
        }
    } else {
        while (1) {
            printf("> ");
            plisp_t *obj = plisp_read(stdin);
            if (obj == NULL) {
                putchar('\n');
                break;
            }

            plisp_t *result = plisp_eval(obj, toplevel);
            if (result != NULL) {
                plisp_c_write(stdout, result);
                putchar('\n');
            }
        }
    }


    return 0;
}
