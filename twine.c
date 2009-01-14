#include <stdio.h>
#include "thread.h"

#define NUM_THREADS 3
int test = 255;
twine_mutex_t my_mutex;

void foo(int th);
void bar(int th);

int main(int argc, char *argv[]) {
  twine_init();
  twine_mutex_init(&my_mutex);

  twine_create(foo, 1);
  twine_create(foo, 2);
  twine_create(foo, 3);

  int i; for (i = 0; i < 20; i++) twine_yield();

  printf("main done.\n");
}

void foo(int th) {
  int i, local, new_th;
  for (i = 1; i<= NUM_THREADS; i++) {
    new_th = i*16 + th;
    twine_create(bar, new_th);
  }
  
  twine_mutex_lock(&my_mutex);

  test = th; local = test;

  twine_yield();

  if (local == test) printf("foo - thread %i: PASSED\n", th);
  else               printf("foo - thread %i: FAILED\n", th);

  twine_mutex_unlock(&my_mutex);
  twine_exit();
}

void bar(int th) {
  int local;

  twine_mutex_lock(&my_mutex);

  test = th; local = test;

  twine_yield();

  if (local == test) printf("bar - thread %i: PASSED\n", th);
  else               printf("bar - thread %i: FAILED\n", th);

  twine_mutex_unlock(&my_mutex);
  twine_exit();
}
