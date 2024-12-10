//
// Created by marko on 20.4.22..
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../lib/mem.h"
#include "../h/print.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Semaphore.hpp"

TCB *TCB::running = nullptr;
int TCB::tID = 0;
int TCB::numOfnotB = 0;
TCB* TCB::coroutines[TCB::NUM_OF_T] = { nullptr };
List<TCB> TCB::allThreads;
int TCB::max_threads = 0;
Semaphore_ *TCB::sem = nullptr;
int TCB::staticP = 0;

TCB *TCB::createCoroutine()
{
    return (TCB*)MemoryAllocator::getInstance().mem_alloc((sizeof(TCB)));
}

void TCB::yield()
{
    Riscv::pushRegisters();

    TCB::thread_dispatch();

    Riscv::popRegisters();
}

void TCB::callBodyWithArg(void *arg) {
    //printingString("THREAD WRAPPER\n");
    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    Riscv::popSppSpie();
    TCB::running->body(TCB::running->arg);
    printString("Thread::");
    printInt( TCB::running->actualTID);
    printString("\n");
    TCB::running->setFinished(true);
    //sem->Semaphore_::sem_signal(sem);
    /*
    numOfnotB--;
    Scheduler::put(Scheduler::getB());*/
    //printingString("dosaoo ja\n");
    ::thread_dispatch();

}


void TCB::thread_dispatch()
{
    TCB *old = running;

    /*
    printingString("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printingString("DOSLA NIT: ");
    printInteger(running->actualTID);
    printingString("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");*/


    if ((!old->isFinished()) && !old->isBlocked()) {
        //printInt(sem->getInit());
        //printString("\n");
        //sem->Semaphore_::sem_wait(sem);
        Scheduler::put(old);

    }

    running = Scheduler::get();
    //Scheduler::printBlocked();
    //Scheduler::printScheduler();

    if(running == nullptr){
        printingString("RUNNING JE NULL\n");
    }

    /*
    printingString("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printingString("IDE NIT: ");
    printInteger(running->actualTID);
    printingString("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");*/


    TCB::contextSwitch(&old->context, &running->context);

}

int TCB::thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, uint64* stack){
    if(handle == nullptr) return -1;

    //__asm__ volatile("mv a0, %0" : : "r" (arg));

    TCB* coroutine = TCB::createCoroutine();

    coroutine->body = start_routine;
    coroutine->stack = coroutine->body != nullptr ? stack: nullptr;
    coroutine->arg = arg;
    coroutine->context = {
            (uint64) &callBodyWithArg,
            coroutine->stack  != nullptr ? (uint64) &coroutine->stack[DEFAULT_STACK_SIZE] : 0};
    coroutine->finished = false;
    coroutine->blocked = false;
    coroutine->actualTID = tID;


    coroutine->sem0 = (Semaphore_*)mem_alloc((sizeof (sem_t)));
    Semaphore_::sem_open(&coroutine->sem0, 1);
    coroutine->sem1 = (Semaphore_*)mem_alloc((sizeof (sem_t)));
    Semaphore_::sem_open(&coroutine->sem1, 0);

    coroutine->message = nullptr;

    if(coroutine->body != nullptr) {
        Scheduler::put(coroutine);
        /*
        if(numOfnotB < max_threads) {
            Scheduler::put(coroutine);
            numOfnotB++;
        }
        else{
            Scheduler::putB(coroutine);
        }*/
    }

    if(tID > 0){
        if(running->actualTID == 0){
            coroutine->parent = staticP + 1;
            staticP++;
        }
        else{
            coroutine->parent = running->parent;
        }
    }

    *handle = coroutine;
    coroutines[tID] = coroutine;
    allThreads.addLast(coroutine);
    tID++;


    return 0; // Thread creation successful
}

int TCB::thread_exit(){
    TCB::running->setFinished(true);
    // MENJALA
    TCB::thread_dispatch();
    return 0;
}

void TCB::thread_join(thread_t handle){
    if(handle == nullptr) return;
    while(!handle->isFinished()){
        TCB::thread_dispatch();
    }
}


int TCB::getThreadId() {
    TCB::thread_dispatch();
    int id = running->actualTID;
    printString("Hello! Thread::");
    printInt(id);
    printString("\n");
    return id;
}

void TCB::setMaximumThreads(int num_of_threads) {
    //sem = (Semaphore_*)mem_alloc((sizeof (sem_t)));
    max_threads = num_of_threads + 1;
    //Semaphore_::sem_open(&sem, max_threads);
}

void TCB::join_all() {
    printInt(tID);
    printString("\n************\n");
    for(int i=0; i<tID; i++){
        printInt(coroutines[i]->actualTID);
        printInt(coroutines[i]->parent);
        printString(" ");
    }
    printString("\n***************\n");
    TCB* captureRunning = running;
    for(int i=captureRunning->actualTID + 1; i<tID; i++){

        if(captureRunning != nullptr && coroutines[i] != nullptr){

            if(captureRunning->parent == 0 || captureRunning->parent == coroutines[i]->parent){

                printInt(coroutines[i]->actualTID);
                printInt(coroutines[i]->parent);
                printString(" ");
                TCB::thread_join(coroutines[i]);
            }
        }
    }
}

void TCB::send(TCB* id, char* message){
    sem_wait(this->sem0);
    printString("Send message: ");
    printString(message);
    printString("\n");
    this->message = message;
    sem_signal(this->sem1);
}

void TCB::recive(){
    sem_wait(this->sem1);
    printString("Recive message: ");
    printString(message);
    printString("\n");
    this->message = nullptr;
    sem_signal(this->sem0);
}


