#include <stdio.h>
#include "thread.h"

void foo(int th);

int main() {
    twine_init();
    twine_create(foo, 1);
    twine_create(foo, 2);

    int i;
    for (i = 0; i < 5; i++) {
        printf("** thread 0 working...\n");
        twine_yield();
    }
}

void foo(int th) {
    int i;
    for (i = 0; i < 5; i++) {
        printf("** thread %d working...\n", th);
        twine_yield();
    }
}
