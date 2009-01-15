#include <stdio.h>
#include "thread.h"

void foo(int s);

int main() {
    twine_init();
    twine_create(foo, 1);
    twine_create(foo, 2);

    int i;
    for (i = 0; i < 10; i++) {
        printf("** thread 0\n");
        twine_yield();
    }
}

void foo(int s) {
    int i;
    for (i = 0; i < 10; i++) {
        printf("** thread %d\n", i);
        twine_yield();
    }
}
