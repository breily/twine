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
static thread *current;
// keep track of tail for convenience
static thread *tail;
// set thread timing info (10ms time slices, repeat alarm every .1ms)
static struct timeval interval = { 0, 100   };
static struct timeval value    = { 0, 10000 };
static struct itimerval timev;

void uthread_mutex_init(uthread_mutex_t *lockVar) {
    // set lock to 0 - unlocked
    lockVar->value = 0;
}

void uthread_mutex_lock(uthread_mutex_t *lockVar) {
    // stop signals for atomic operation
    sighold(SIGALRM);
    // wait while lock is 1 - locked
    while (lockVar->value) {
        sigrelse(SIGALRM);
        sighold(SIGALRM);
    }
    // set lock to locked
    lockVar->value = 1;
    sigrelse(SIGALRM);
}

void uthread_mutex_unlock(uthread_mutex_t *lockVar) {
    sighold(SIGALRM);
    // set lock to unlocked
    lockVar->value = 0;
    sigrelse(SIGALRM);
}

void handle_alarm(int sig) {
    uthread_yield();
}

void start_timer() {
    getitimer(0, &timev);
    // repeat alarm every .1ms
    timev.it_interval = interval;
    // give each thread 10ms
    timev.it_value = value;
    setitimer(0, &timev, NULL);
}

void uthread_init() {
    // set up our main thread
    thread *t = (thread *) malloc(sizeof(thread));
    t->id = 0;
    t->next = NULL;

    // main should probably be highest priority
    t->pri = 1;

    // initialize the two pointers
    current = t;
    tail = t;

    // signal handler
    signal(SIGALRM, handle_alarm);
}

int uthread_create(void (*func)(int), int val, int pri) {
    // set up the thread, allocate stack, etc
    thread *t = (thread *) malloc(sizeof(thread));
    int ret = getcontext(&t->ctx);
    if (ret == -1) die("uthread_create: getcontext()");
    t->ctx.uc_stack.ss_sp = malloc(STACK_SIZE);
    t->ctx.uc_stack.ss_size = STACK_SIZE;
    t->ctx.uc_link = &tail->ctx;
    makecontext(&t->ctx, (void (*)(void)) func, 1, val);

    // set other information, add to end of queue
    t->pri = pri;
    t->id = tail->id + 1;
    t->next = NULL;
    enqueue(t);
    //tail = t;

    // return the id
    return t->id;
}

void uthread_yield() {
    // can't switch if theres nothing to switch to
    if (current->next == NULL) return;
    // get the next thread in the queue
    thread *t;
    t = current->next;

    // add the current thread to the end of the queue
    current->next = NULL;
    enqueue(current);
    current = t;
    start_timer();
    // switch threads
    int ret = swapcontext(&tail->ctx, &current->ctx);
    if (ret == -1) die("uthread_yield: swapcontext()");
}

void uthread_exit() {
    // get the current thread, set current to the next one
    thread *old;
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
    if (ret == -1) die("uthread_exit: setcontext()");
}

void enqueue(thread *t) {
    thread *c;
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
