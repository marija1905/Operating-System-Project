#include "..//lib/hw.h"
#include "..//h/syscall_c.hpp"
#include "..//lib/console.h"
#include "..//lib/mem.h"
#include "../h/MemoryAllocator.hpp"

const uint32 MEM_ALLOC = 0x01;
const uint32 MEM_FREE = 0x02;
const uint32 THREAD_C = 0x11;
const uint32 THREAD_E = 0x12;
const uint32 THREAD_D = 0x13;
const uint32 THREAD_J = 0x14;
const uint32 SEM_O = 0x21;
const uint32 SEM_C = 0x22;
const uint32 SEM_W = 0x23;
const uint32 SEM_S = 0x24;
const uint32 SEM_TDW = 0x25;
const uint32 SEM_TW = 0x26;
const uint32 GETC = 0x41;
const uint32 PUTC = 0x42;
const uint32 THREAD_ID = 0x43;
const uint32 SEM_PRIORITY = 0x44;

const uint32 SEM_O_R = 0x45;
const uint32 SEM_C_R = 0x46;
const uint32 SEM_W_R = 0x47;
const uint32 SEM_S_R = 0x48;
const uint32 JA = 0x49;
const int EOF = -1;

void* mem_alloc (size_t size){

    __asm__ volatile("mv a1, %0" : : "r" ((size)));
    __asm__ volatile("mv a0, %0" : : "r" (MEM_ALLOC));

    void* ptr;

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0": "=r" (ptr));

    return ptr;

}

int mem_free (void* ptr){
    uint64 a0 = MEM_FREE;

    __asm__ volatile("mv a1, %0" : : "r" (ptr));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0": "=r" (a0));

    return (int)a0;

}


// prilikom smestanja arg  u a registre dolazi do pogresnog upisa zbog THREAD_C !!!!
int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){

    uint64 *stack;
    //stack = (uint64*)__mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64));
    if(start_routine != nullptr) {
        //stack = (uint64*)MemoryAllocator::getInstance().mem_alloc((DEFAULT_STACK_SIZE * sizeof(uint64) + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE);
        stack = (uint64*)mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
        //stack = (uint64*) __mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64));
    }
    else{
        stack = nullptr;
    }
    __asm__ volatile("mv a4, %0" : : "r" (stack));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (THREAD_C));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int thread_exit (){
    uint64 a0 = THREAD_E;

    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");

    return 0;
}

void thread_dispatch (){
    uint64 a0 = THREAD_D;

    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
}

void thread_join (thread_t handle){
    uint64 a0 = THREAD_J;

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
}

int sem_open(sem_t* handle, unsigned init){
    uint64 a0 = SEM_O;

    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int sem_close (sem_t handle){
    uint64 a0 = SEM_C;

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int sem_wait (sem_t id){
    uint64 a0 = SEM_W;

    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (a0));


    __asm__ volatile ("ecall");

    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int sem_signal (sem_t id){
    uint64 a0 = SEM_S;

    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int sem_trywait (sem_t id){
    uint64 a0 = SEM_TW;

    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

char getc (){
    uint64 a0 = GETC;
    __asm__ volatile("mv a0, %0" : : "r" (a0));
    __asm__ volatile ("ecall");
    char v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

void putc (char c){
    uint64 a0 = PUTC;

    __asm__ volatile("mv a1, %0" : : "r" (c));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");

}

int getThreadId(){

    uint64 a0 = THREAD_ID;
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    int i;

    __asm__ volatile ("ecall");
    __asm__ volatile("mv %0, a0": "=r" (i));

    return i;
}

int priority_signal(sem_t id){
    uint64 a0 = SEM_PRIORITY;

    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    int i;

    __asm__ volatile ("ecall");
    __asm__ volatile("mv %0, a0": "=r" (i));
    return i;
}

int open_r(sem_t* handle, unsigned init){
    uint64 a0 = SEM_O_R;

    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int close_r (sem_t handle){
    uint64 a0 = SEM_C_R;

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int wait_r (sem_t id, unsigned init){
    uint64 a0 = SEM_W_R;

    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (a0));


    __asm__ volatile ("ecall");

    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

int signal_r (sem_t id, unsigned init){
    uint64 a0 = SEM_S_R;

    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
    int v;
    __asm__ volatile("mv %0, a0": "=r" (v));
    return v;
}

void join_all(){
    uint64 a0 = JA;
    __asm__ volatile("mv a0, %0" : : "r" (a0));

    __asm__ volatile ("ecall");
};