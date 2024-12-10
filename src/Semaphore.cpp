#include "../h/Semaphore.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../lib/mem.h"


//int Semaphore_::numOfBlocked = 0;
int Semaphore_::numOfTimeout = 0;
List<Semaphore_> Semaphore_::semWaitingTimeout;
//List<TCB> Semaphore_::readyBlockedQueue;

static const int SEMDEAD = -1;
static const int TIMEOUT = -2;


void Semaphore_::block(){
    TCB::running->setBlocked(true);
    /*
    printInt(id->numOfBlocked);
    printString("\n************************\n");*/

    if(numOfBlocked == 0 ){
        readyBlockedQueue.setEmpty();
    }
    //printString("USAO U BLOCKED\n");
    //id->readyBlockedQueue.printList();
    readyBlockedQueue.addLast(TCB::running);
    numOfBlocked++;
    /*
    printString("\nBLOKIRANA NIT: ");
    printInt(TCB::running->actualTID);
    printString("\n");*/
    TCB::thread_dispatch();
}

void Semaphore_::unblock() {
    TCB* t = readyBlockedQueue.removeFirst();
    t->setBlocked(false);
    Scheduler::put(t);
    numOfBlocked--;
    /*
    printString("\nODBLOKIRANA NIT: ");
    printInt(t->actualTID);
    printString("\n");*/
}

int Semaphore_::sem_open(sem_t *handle, unsigned int init) {
    //size_t size = (sizeof(Semaphore) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    sem_t s;
    //s = (Semaphore*)MemoryAllocator::getInstance().mem_alloc(size);
    s = (Semaphore_*)MemoryAllocator::getInstance().mem_alloc((sizeof(Semaphore_)));
    if(s == nullptr) return -1;
    s->init = (int)init;
    s->numOfBlocked = 0;
    s->readyBlockedQueue.setEmpty();
    *handle = s;
    return 0;
}

int Semaphore_::sem_wait(sem_t id) {
    if(id == nullptr) return -1;
    //if(numOfBlocked == 0) return -2; // ukoliko se zatvorio semafor, znaci da su sve niti odblokirane i wait vraca neg vr
    if(--(id->init) < 0){
        //printString("U IFU\n");
        //readyBlockedQueue.printList();
        id->block();
    }

    return 0;
}

int Semaphore_::sem_signal(sem_t id) {
    if(id == nullptr) return -1;
    //int num = (int)id->init;
    if(++(id->init) <= 0) {
        id->unblock();
    }
    return 0;
}

int Semaphore_::sem_close (sem_t handle){
    if(handle == nullptr) return -1;
    for(int i = 0; i < handle->numOfBlocked; i++) {
        handle->unblock();
    }
    MemoryAllocator::getInstance().mem_free((void*)handle);
    handle = nullptr;
    return 0;
}

int Semaphore_::priority_signal(sem_t id){
    if(id == nullptr) return -1;
    //int num = (int)id->init;
    if(++(id->init) <= 0) {
        priority_unblock();
    }
    return 0;
}

void Semaphore_::priority_unblock() {
    TCB* t_best = nullptr;
    TCB* tmp = nullptr;
    TCB* last = readyBlockedQueue.peekLast();

    while(tmp != last){
        tmp = readyBlockedQueue.removeFirst();
        if(t_best == nullptr) {
            t_best = tmp;
            continue;
        }
        if(tmp->getThreadId() < t_best->getThreadId()){
            if(t_best != nullptr) {
                readyBlockedQueue.addLast(t_best);
            }
            t_best = tmp;
        }
        else{
            readyBlockedQueue.addLast(tmp);
        }
    }
    //TCB* t = readyBlockedQueue.removeFirst();
    t_best->setBlocked(false);
    numOfBlocked--;
    Scheduler::put(t_best);
}

int Semaphore_::sem_timedwait(sem_t id, time_t timeout) {
    if (id == nullptr) return SEMDEAD;
    this->handle = &id;
    this->timeout = timeout;
    this->init = 0;
    numOfTimeout++;
    semWaitingTimeout.addLast(this);
    sem_wait(id);
    if (timeout == 0) return TIMEOUT;

    for(int i=0; i < numOfTimeout; i++){
        Semaphore_* sem = semWaitingTimeout.removeFirst();
        if (sem != id) {
            semWaitingTimeout.addLast(sem);
        }
    }
    return 0;
}

int Semaphore_::sem_trywait(sem_t id) {
    if(id == nullptr) return -1;
    if(id->init < 1){
        return 0;
    }
    else{
        return 1;
    }
}


int Semaphore_::open_r(sem_t *handle, unsigned int value) {
    sem_t s;
    s = (Semaphore_*)MemoryAllocator::getInstance().mem_alloc((sizeof(Semaphore_)));
    if(s == nullptr) return -1;
    s->init = (int)value;

    printInt(s->init);
    printString("\n");
    s->numOfBlocked = 0;
    s->readyBlockedQueue.setEmpty();
    s->values.setEmpty();
    *handle = s;
    return 0;
}

int Semaphore_::wait_r(sem_t id, unsigned value) {
    if(id == nullptr) return -1;
    if(id->init < (int)value){
        int v = ((int)value);
        id->values.addLast(&v);
        id->block();
    }
    else{
        id->init -= (int)value;
    }
    return 0;
}

int Semaphore_::signal_r(sem_t id, unsigned value) {
    if(id == nullptr) return -1;
    int* curr = id->values.removeFirst();

    id->init += value;
    while (curr != nullptr && id->init >= *curr) {
        id->unblock();
        id->init -= *curr;
        curr = id->values.removeFirst();
    }
    if(curr != nullptr){
        id->values.addFirst(curr);
    }

    return 0;
}

int Semaphore_::close_r (sem_t handle){
    if(handle == nullptr) return -1;
    for(int i = 0; i < handle->numOfBlocked; i++) {
        handle->unblock();
    }
    MemoryAllocator::getInstance().mem_free((void*)handle);
    handle = nullptr;
    return 0;
}