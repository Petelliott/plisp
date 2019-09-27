#include <stdio.h>

#include <plisp/gc.h>

int main() {
    plisp_GC_init();
    printf("hello world\n");
    return 0;
}
