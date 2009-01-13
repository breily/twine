#include <stdio.h>
#include "uthread.h"

#define NUM_THREADS 3
int test = 255;
uthread_mutex_t my_mutex;

void foo(int th);
void bar(int th);

int main(int argc, char *argv[]) {
  uthread_init();
  uthread_mutex_init(&my_mutex);

  uthread_create(foo, 1, 1);
  uthread_create(foo, 2, 2);
  uthread_create(foo, 3, 3);

  int i; for (i = 0; i < 20; i++) uthread_yield();

  printf("main done.\n");
}

void foo(int th) {
  int i, local, new_th;
  for (i = 1; i<= NUM_THREADS; i++) {
    new_th = i*16 + th;
    uthread_create(bar, new_th, 1);
  }
  
  uthread_mutex_lock(&my_mutex);

  test = th; local = test;

  uthread_yield();

  if (local == test) printf("foo - thread %i: PASSED\n", th);
  else               printf("foo - thread %i: FAILED\n", th);

  uthread_mutex_unlock(&my_mutex);
  uthread_exit();
}

void bar(int th) {
  int local;

  uthread_mutex_lock(&my_mutex);

  test = th; local = test;

  uthread_yield();

  if (local == test) printf("bar - thread %i: PASSED\n", th);
  else               printf("bar - thread %i: FAILED\n", th);

  uthread_mutex_unlock(&my_mutex);
  uthread_exit();
}
