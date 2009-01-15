#include <stdio.h>
#include "thread.h"

twine_mutex mutex;
int mutex_test = 1123;
void foo(int th);

int main() {
    twine_init();
    twine_mutex_init(&mutex);
    twine_create(foo, 1);
    twine_create(foo, 2);

    int backup;
    int i;
    for (i = 0; i < 5; i++) {
        twine_mutex_lock(&mutex);
        printf("** thread 0 working...\n");
        mutex_test = 0;
        twine_yield();
        if (mutex_test != 0) printf("!! thread 0 mutex failed\n");
        else printf("** thread 0 mutex passed\n");
        twine_mutex_unlock(&mutex);
    }
}

void foo(int th) {
    int backup;
    int i;
    for (i = 0; i < 5; i++) {
        twine_mutex_lock(&mutex);
        printf("** thread %d working...\n", th);
        mutex_test = th;
        twine_yield();
        if (mutex_test != th) printf("!! thread %d mutex failed\n", th);
        else printf("** thread %d mutex passed\n", th);
        twine_mutex_unlock(&mutex);
    }
}
