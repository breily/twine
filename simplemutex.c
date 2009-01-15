#include <stdio.h>
#include "thread.h"

// Our mutex lock
twine_mutex mutex;
// A value we will use to test the mutex locks
int mutex_test = 7;
// Function to create threads
void foo(int th);

int main() {
    // Initialize Twine
    twine_init();
    // Initialize our mutex lock
    twine_mutex_init(&mutex);
    // Create and enqueue two threads
    twine_create(foo, 1);
    twine_create(foo, 2);

    int i;
    for (i = 0; i < 5; i++) {
        // Lock mutex - Atomic operation
        twine_mutex_lock(&mutex);
        printf("** thread 0 working...\n");
        mutex_test = 0;
        // Yield to another thread
        twine_yield();
        // Lock shoud have prevented changes
        if (mutex_test != 0) printf("!! thread 0 mutex failed\n");
        else printf("** thread 0 mutex passed\n");
        // Unlock the mutex
        twine_mutex_unlock(&mutex);
    }
}

void foo(int th) {
    int i;
    for (i = 0; i < 5; i++) {
        // Lock mutex
        twine_mutex_lock(&mutex);
        printf("** thread %d working...\n", th);
        mutex_test = th;
        // Yield to another thread
        twine_yield();
        // Hopefully no change
        if (mutex_test != th) printf("!! thread %d mutex failed\n", th);
        else printf("** thread %d mutex passed\n", th);
        // Unlock mutex
        twine_mutex_unlock(&mutex);
    }
}
