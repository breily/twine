#ifndef THREAD_H
#define THREAD_H

#include <ucontext.h>

// Hold each thread's id, ucontext, and a pointer to the next thread to run.
typedef struct twine_thread {
    int id;
    ucontext_t ctx;
    struct twine_thread *next;
} twine_thread;

// Holds a mutex lock.
typedef struct twine_mutex {
    int value;
} twine_mutex;

// Initializes the thread library.  Must be called before threads are used.
void twine_init();

// Creates a thread to run function *func.  Returns
// the thread id.  Does not run the thread.
int twine_create(void (*func)(int), int val);

// Causes the current thread to relinquish control.
// Adds this thread to the end of the queue, and runs
// the next thread.
void twine_yield();

// Stops the current thread and removes it from the
// queue.  Frees the space allocated for it, and runs
// the next threead.
void twine_exit();

// Initializes the mutex lock lockVar.
void twine_mutex_init(twine_mutex *lockVar);

// Locks the mutex lock lockVar.  If already locked, the
// calling thread blocks until mutex is available.
void twine_mutex_lock(twine_mutex *lockVar);

// Releases the mutex lock lockVar.
void twine_mutex_unlock(twine_mutex *lockVar);

// Calls twine_yield when SIGALRM is caught.
void handle_alarm(int sig);

// Starts the 10ms timer.
void start_timer();

// Adds thread *t to the end of the queue.
void enqueue(twine_thread *t);

// Indicates an error and exits.
void die(char *msg);

#endif
