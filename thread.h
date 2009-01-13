#ifndef UTHREAD_H
#define UTHREAD_H

#include <ucontext.h>

/*
 * Simple struct to hold each thread's id, ucontext,
 * and a pointer to the next thread to run.
 */
typedef struct thread {
    int id;
    int pri;
    ucontext_t ctx;
    struct thread *next;
} thread;

/*
 * Holds a mutex lock.
 */
typedef struct uthread_mutex {
    int value;
} uthread_mutex_t;

/*
 * Initializes the thread library.  Must be called.
 */
void uthread_init();

/*
 * Creates a thread to run function *func.  Returns
 * the thread id.  Does not run the thread.
 */
int uthread_create(void (*func)(int), int val, int pri);

/*
 * Causes the current thread to relinquish control.
 * Adds this thread to the end of the queue, and runs
 * the next thread.
 */
void uthread_yield();

/*
 * Stops the current thread and removes it from the
 * queue.  Frees the space allocated for it, and runs
 * the next threead.
 */
void uthread_exit();

/*
 * Initializes the mutex lock lockVar.
 */
void uthread_mutex_init(uthread_mutex_t *lockVar);

/*
 * Locks the mutex lock lockVar.  If already locked, the
 * calling thread blocks until mutex is available.
 */
void uthread_mutex_lock(uthread_mutex_t *lockVar);

/*
 * Releases the mutex lock lockVar.
 */
void uthread_mutex_unlock(uthread_mutex_t *lockVar);

/*
 * Calls uthread_yield when SIGALRM is caught.
 */
void handle_alarm(int sig);

/*
 * Starts the 10ms timer.
 */
void start_timer();

/*
 * Switches current thread to the next lowest priority thread.
 */
void find_next_thread();

/*
 * Adds thread *t to the end of the queue.
 */
void enqueue(thread *t);

/*
 * Indicates an error and exits.  Borrowed from 'man makecontext'.
 */
void die(char *msg);

#endif
