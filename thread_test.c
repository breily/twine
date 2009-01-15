#include <stdio.h>
#include "thread.h"

// Our function that we'll create threads of
void foo(int th);

int main() {
    // Initialize Twine
    twine_init();

    // Create two threads and add them to the queue
    twine_create(foo, 1);
    twine_create(foo, 2);

    int i;
    for (i = 0; i < 5; i++) {
        // main thread is thread 0
        printf("** thread 0 working...\n");
        // Yields to the next thread in the queue
        twine_yield();
    }
}

void foo(int th) {
    int i;
    for (i = 0; i < 5; i++) {
        printf("** thread %d working...\n", th);
        // Yields to the next thread in the queue
        twine_yield();
    }
}
