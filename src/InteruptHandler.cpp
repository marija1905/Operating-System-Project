//
// Created by os on 5/23/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/Semaphore.hpp"
#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"

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

const uint32 SEM_O_R = 0x45;
const uint32 SEM_C_R = 0x46;
const uint32 SEM_W_R = 0x47;
const uint32 SEM_S_R = 0x48;
const uint32 JA = 0x49;

void (*funcArray[1000])(uint64*);

void mem_alloc_ (uint64* args) {

    MemoryAllocator::getInstance().mem_alloc(size_t(args[1]));

}

void mem_free_(uint64* args){

   MemoryAllocator::getInstance().mem_free((void*)args[1]);

}

void thread_create_(uint64* args){

    TCB::thread_create((thread_t*)args[1], (void (*)(void *))args[2], (void*)args[3], (uint64*)args[4]);

}

void thread_exit_(uint64* args){

    TCB::thread_exit();
}

void thread_dispatch_(uint64* args){

    TCB::thread_dispatch();

}

void thread_join_(uint64* args) {

    TCB::thread_join((thread_t) args[1]);

}

void sem_open_(uint64* args) {

    Semaphore_::sem_open((sem_t *)args[1], (unsigned int)args[2]);

}

void sem_close_(uint64* args) {

    Semaphore_::sem_close((sem_t)args[1]);

}

void sem_wait_(uint64* args) {

    ((Semaphore_*) args[1])->Semaphore_::sem_wait((sem_t)args[1]);
    //Semaphore_::sem_wait((sem_t)args[1]);
}

void sem_signal_(uint64* args) {

    ((Semaphore_*) args[1])->Semaphore_::sem_signal((sem_t)args[1]);
    //Semaphore_::sem_signal((sem_t)args[1]);

}

void open_r_(uint64* args) {

    Semaphore_::open_r((sem_t *)args[1], (unsigned int)args[2]);

}

void close_r_(uint64* args) {

    Semaphore_::close_r((sem_t)args[1]);

}

void wait_r_(uint64* args) {

    ((Semaphore_*) args[1])->Semaphore_::wait_r((sem_t)args[1], (unsigned int)args[2]);
    //Semaphore_::sem_wait((sem_t)args[1]);
}

void signal_r_(uint64* args) {

    ((Semaphore_*) args[1])->Semaphore_::signal_r((sem_t)args[1], (unsigned int)args[2]);
    //Semaphore_::sem_signal((sem_t)args[1]);

}

void join_all_(uint64* args) {

    TCB::join_all();

}

void getThreadID_(uint64* args) {

    TCB::getThreadId();

}

//TODO

inline void trywait_(uint64* args){

    ((Semaphore_*) args[1])->Semaphore_::sem_trywait((sem_t)args[1]);
    //Semaphore_::sem_trywait((sem_t)args[1]);
}

inline void getc_ (uint64* args){

    __getc();


}

inline void putc_ (uint64* args){

    __putc(char(args[1]));

}

void init(){
    funcArray[MEM_ALLOC] = &mem_alloc_;
    funcArray[MEM_FREE] = &mem_free_;
    funcArray[THREAD_C] = &thread_create_;
    funcArray[THREAD_E] = &thread_exit_;
    funcArray[THREAD_D] = &thread_dispatch_;
    funcArray[THREAD_J] = &thread_join_;
    funcArray[SEM_O] = &sem_open_;
    funcArray[SEM_C] = &sem_close_;
    funcArray[SEM_W] = &sem_wait_;
    funcArray[SEM_S] = &sem_signal_;
    funcArray[GETC] = &getc_;
    funcArray[PUTC] = &putc_;
    funcArray[SEM_TW] = &trywait_;
    funcArray[SEM_O_R] = &open_r_;
    funcArray[SEM_C_R] = &close_r_;
    funcArray[SEM_W_R] = &wait_r_;
    funcArray[SEM_S_R] = &signal_r_;
    funcArray[JA] = &join_all_;
    funcArray[THREAD_ID] = &getThreadID_;
}

void fillArgs(uint64* args, uint64 a1, uint64 a2, uint64 a3, uint64 a4){
    args[1] = a1;
    args[2] = a2;
    args[3] = a3;
    args[4] = a4;

}
extern "C" void handleSupervisorTrap(){

    if (funcArray[MEM_ALLOC] == nullptr){
        init();
    }

    uint64 scauseVar;
    uint64 volatile sstatus;
    uint64 a0;

    __asm__ volatile("ld t3, 10*8(fp)");
    __asm__ volatile("mv %0, t3" : "=r"(a0)); // smestanje koda fje u prom a0 kako bi se znalo koji sys poziv je u pitanju

    __asm__ volatile ("csrr %[ime], scause": [ime] "=r" (scauseVar));
    uint64 volatile sepc = Riscv::r_sepc() + 4;
    __asm__ volatile ("csrr %[ime], sstatus": [ime] "=r" (sstatus));

    uint64 a1;
    __asm__ volatile ("ld t3, 11*8(fp)");
    __asm__ volatile("mv %0, t3": "=r" (a1));
    uint64 a2;
    __asm__ volatile ("ld t3, 12*8(fp)");
    __asm__ volatile("mv %0, t3": "=r" (a2));
    uint64 a3;
    __asm__ volatile ("ld t3, 13*8(fp)");
    __asm__ volatile("mv %0, t3": "=r" (a3));
    uint64 a4;
    __asm__ volatile ("ld t3, 14*8(fp)");
    __asm__ volatile("mv %0, t3": "=r" (a4));

    uint64 args[20];

    fillArgs(args, a1, a2, a3, a4);

    if (scauseVar == (0UL << 63 | 0x09 ) || scauseVar == (0UL << 63 | 0x08)) {
        /*
        switch(a0){
            case 0x01:{
                mem_alloc_(a1,a2,a3,a4);
                break;
            }
            case 0x02:{
                mem_free_(a1,a2,a3,a4);
                break;
            }
            case 0x11:{
                thread_create_(a1,a2,a3,a4);
                break;
            }
            case 0x12:{
                thread_exit_(a1,a2,a3,a4);
                break;
            }
            case 0x13:{
                thread_dispatch_(a1,a2,a3,a4);
                break;
            }
            case 0x14:{
                thread_join_(a1,a2,a3,a4);
                break;
            }
            case 0x21:{
                sem_open_(a1,a2,a3,a4);
                break;
            }
            case 0x22:{
                sem_close_(a1,a2,a3,a4);
                break;
            }
            case 0x23:{
                sem_wait_(a1,a2,a3,a4);
                break;
            }
            case 0x24:{
                sem_signal_(a1,a2,a3,a4);
                break;
            }
            case 0x41:{
                getc_(a1,a2,a3,a4);
                break;
            }
            case 0x42:{
                putc_(a1,a2,a3,a4);
                break;
            }

        }*/
        funcArray[a0](args);
        __asm__ volatile("sd a0, 10*8(fp)");
        Riscv::w_sepc(sepc);
        Riscv::w_sstatus(sstatus);

    }
    else if (scauseVar == 0x8000000000000009UL){
        console_handler();
    }
    else if (scauseVar == 0x8000000000000001UL){
        uint64 m = 0x02;
        /*
        for (int i = 0; i < Semaphore_::getNumOfTimeout(); i++) {
            Semaphore_* sem = Semaphore_::semWaitingTimeout.removeFirst();
            sem->decrementTimeout();
            if (sem->getTimeout() == 0){
                sem->sem_signal(sem->getSemID());
            }
            else {
                Semaphore_::semWaitingTimeout.addLast(sem);
            }
            //Semaphore_::sem_signal(sem->getSemID());
        }*/
        __asm__ volatile("csrc sip, %[mask]" : : [mask] "r"(m));
    }
    else{
        printingString("Scause: ");
        printInteger(scauseVar);
        printingString("\n");
        printingString("Sepc: ");
        printInteger(Riscv::r_sepc());
        printingString("\n");

    }

}