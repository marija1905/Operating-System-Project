//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyCoroutineQueue;
    static List<TCB> blockedThreads;
    static Scheduler* iPtr;

public:
    static TCB *get();

    static void put(TCB *ccb);

    static TCB *getB();

    static void putB(TCB *ccb);

    static void printScheduler();

    static void printBlocked();

    static Scheduler* getInstance() {
        if (iPtr == nullptr) {
            static Scheduler ins = Scheduler();
            iPtr = &ins;
            return &ins;
        }
        return iPtr;
    }

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP