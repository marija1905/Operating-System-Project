//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/riscv.hpp"
#include "syscall_c.hpp"
#include "../h/print.hpp"

class TCB;
typedef TCB* thread_t;


class TCB
{
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    using Body = void (*)(void*);

    static TCB *createCoroutine();

    static int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg, uint64* stack);

    static void thread_dispatch();

    static int thread_exit();

    static void thread_join (thread_t handle);

    static void join_all();

    static void yield();

    static int getThreadId();

    static void setMaximumThreads(int num_of_threads = 5);

    void send(TCB* id, char* message);

    void recive();

    static TCB *running;

    int actualTID = 0;


private:

    static Semaphore_ *sem;

    static int max_threads;

    char* message;

    Semaphore_* sem0;

    Semaphore_* sem1;

    static const int NUM_OF_T = 150;

    static TCB* coroutines[NUM_OF_T];

    List<TCB> children;

    static List<TCB> allThreads;

    int parent = 0;

    static int tID;

    static int staticP;

    static int numOfnotB;


    struct Context
    {
        uint64 ra;
        uint64 sp;
    };


    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    bool finished;
    bool blocked;

    static void contextSwitch(Context *oldContext, Context *runningContext);


    static void callBodyWithArg(void* arg);

    //static uint64 constexpr STACK_SIZE = 1024;

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
