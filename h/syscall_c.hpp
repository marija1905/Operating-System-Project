#include "../lib/hw.h"

void* mem_alloc (size_t size);

int mem_free (void*);

class TCB;
typedef TCB* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_exit ();

void thread_dispatch ();

void thread_join (thread_t handle);

class Semaphore_;
typedef Semaphore_* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

int sem_trywait (sem_t id);

char getc ();

void putc (char);

int getThreadId();

int priority_signal(sem_t id);

int open_r (sem_t* handle, unsigned value);
int close_r (sem_t handle);
int wait_r (sem_t id, unsigned value);
int signal_r (sem_t id, unsigned value);
void join_all();