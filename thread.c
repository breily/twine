#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include "thread.h"

#define THREAD_EXIT  0
#define THREAD_YIELD 1

// stack size for our threads
#define STACK_SIZE 1048576
// the current running thread
static twine_thread *current;
// keep track of tail for convenience
static twine_thread *tail;
// set thread timing info (10ms time slices, repeat alarm every .1ms)
static struct timeval interval = { 0, 100   };
static struct timeval value    = { 0, 10000 };
static struct itimerval timev;

void twine_mutex_init(twine_mutex *lockVar) {
    lockVar->value = 0;         // 0 = unlocked
}

void twine_mutex_lock(twine_mutex *lockVar) {
    sighold(SIGALRM);           // stop signals for atomic operation
    while (lockVar->value) {    // wait while lock is 1 (locked)
        sigrelse(SIGALRM);
        sighold(SIGALRM);
    }
    lockVar->value = 1;         // 1 = locked
    sigrelse(SIGALRM);
}

void twine_mutex_unlock(twine_mutex *lockVar) {
    sighold(SIGALRM);
    lockVar->value = 0;         // 0 = unlocked
    sigrelse(SIGALRM);
}

void handle_alarm(int sig) {
    twine_yield();
}

void start_timer() {
    getitimer(0, &timev);
    // repeat alarm every .1ms
    timev.it_interval = interval;
    // give each thread 10ms
    timev.it_value = value;
    setitimer(0, &timev, NULL);
}

void twine_init() {
    // set up our main (current) thread
    twine_thread *t = (twine_thread *) malloc(sizeof(twine_thread));
    t->id = 0;
    t->next = NULL;

    // initialize the two pointers
    current = t;
    tail = t;

    // signal handler
    signal(SIGALRM, handle_alarm);
}

int twine_create(void (*func)(int), int val) {
    // set up the thread, allocate stack, etc
    twine_thread *t = (twine_thread *) malloc(sizeof(twine_thread));
    int ret = getcontext(&t->ctx);
    if (ret == -1) die("twine_create: getcontext()");
    t->ctx.uc_stack.ss_sp = malloc(STACK_SIZE);
    t->ctx.uc_stack.ss_size = STACK_SIZE;
    t->ctx.uc_link = &tail->ctx;
    makecontext(&t->ctx, (void (*)(void)) func, 1, val);

    // set other information, add to end of queue
    t->id = tail->id + 1;
    t->next = NULL;
    enqueue(t);

    // return the id
    return t->id;
}

void twine_yield() {
    // can't switch if theres nothing to switch to
    if (current->next == NULL) return;
    // get the next thread in the queue
    twine_thread *t;
    t = current->next;

    // add the current thread to the end of the queue
    current->next = NULL;
    enqueue(current);
    current = t;
    start_timer();
    // switch threads
    int ret = swapcontext(&tail->ctx, &current->ctx);
    if (ret == -1) die("twine_yield: swapcontext()");
}

void twine_exit() {
    // get the current thread, set current to the next one
    twine_thread *old;
    old = current;
    current = current->next;

    // free our allocated space - frees the stack too?
    free(old);
    if (current == NULL) exit(0);

    start_timer();   
    // set the context to the next thread
    // no need to get or swap contexts because we're exiting
    // this thread
    int ret = setcontext(&current->ctx);
    if (ret == -1) die("twine_exit: setcontext()");
}

void enqueue(thread *t) {
    twine_thread *c;
    c = tail;
    // make the last thread's next point to *t
    c->next = t;
    // make tail point to *t
    tail = t;
}

void die(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
