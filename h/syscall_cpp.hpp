#ifndef _syscall_cpp
#define _syscall_cpp
#include "syscall_c.hpp"
#include "../lib/hw.h"
#include "../h/TCB.hpp"

void* operator new (size_t);
void operator delete (void*);

class Thread{
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
    int getThreadId();

protected:
    Thread (): myHandle(nullptr), pid(0){};
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    int pid;
    static void wRun(void* t);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
    int priority_signal();

private:
    sem_t myHandle;
    signed init;
};
/*
class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
};*/
class Console {
public:
    static char getc ();

    static void putc (char);
};

#endif