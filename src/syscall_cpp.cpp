/*
#include "../h/syscall_c.hpp"
*/
#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/Semaphore.hpp"


using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
    //return mem_alloc(n);
    return mem_alloc(n);
    //return MemoryAllocator::getInstance().mem_alloc( (n + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
    //return __mem_alloc(n);
    //return MemoryAllocator::getInstance().mem_alloc( (n + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

void operator delete(void *p)
{
    //mem_free(p);
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    //mem_free(p);
    mem_free(p);
}


Thread::Thread(void (*body)(void *), void *arg) {
    auto* thread = (thread_t*)MemoryAllocator::getInstance().mem_alloc((sizeof (thread_t)));
    this->body = body;
    this->arg = arg;
    ::thread_create(thread, body, arg);
    myHandle = *thread;
}



Thread::~Thread() {
}

void Thread::wRun(void *t) {
    //printingString("Trci ova nit!\n");
    if(t){
        ((Thread*)t)->run();
    }
}

int Thread::start() {
    //printingString("Usao u start!\n");
    if(pid) return pid;
    else{
        auto* thread = (thread_t*)MemoryAllocator::getInstance().mem_alloc((sizeof (thread_t)));
        pid = ::thread_create(thread, wRun, this);
        myHandle = *thread;
        //printingString("Stigao bar ovde kao\n");
        return pid;
    }
}

void Thread::join() {
    ::thread_join(myHandle);
}

void Thread::dispatch() {
    ::thread_dispatch();
}

int Thread::sleep(time_t) {
    return 0; //TODO
}

int Thread::getThreadId() {
    int id = ::getThreadId();
    return id;
}


Semaphore::Semaphore(unsigned int init) {
    sem_t* sem = (sem_t*)mem_alloc((sizeof (sem_t)));
    this->init = init;
    ::sem_open(sem, init);
    myHandle = *sem;
}

Semaphore::~Semaphore() {
    ::sem_close(myHandle);
}

int Semaphore::wait() {
    ::sem_wait(myHandle);
    return 0;
}

int Semaphore::signal() {
    ::sem_signal(myHandle);
    return 0;
}

char Console::getc() {
    char c =::getc();
    return c;
}

void Console::putc(char c) {
    ::putc(c);
}

int Semaphore::priority_signal(){
    ::priority_signal(myHandle);
    return 0;
}

int Semaphore::timedWait(time_t) {
    return 0;
}

int Semaphore::tryWait() {
    return ::sem_trywait(myHandle);
}
