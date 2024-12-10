#include "../h/syscall_cpp.hpp"
#include "../test/userMain.hpp"
#include "../h/MemoryAllocator.hpp"


extern "C" void supervisorTrap();


void wrapper(void*) {
    userMain();
}

sem_t semaphore;

// Function to be executed by threads
void thread_function(void* value) {
    printingString("Usao u fju\n");
    void* intPtr = value;
    int id = *(int*)intPtr;
    printingString("Thread " );
    printInteger(id);
    printingString( " is waiting...\n");
    sem_wait(semaphore); // Wait for the semaphore
    printingString("Thread " );
    printInteger(id);
    printingString(" is now in the critical section.\n");
    printingString("Thread ");
    printInteger(id);
    printingString(" finished its work and is releasing the semaphore.\n");
    sem_signal(semaphore); // Release the semaphore
}

void function1(void*){
    //wait_r(semaphore, 5);
    printingString("Thread 1 done!\n");
    getThreadId();
}
void function2(void*){
    //wait_r(semaphore, 2);
    printingString("Thread 2 done!\n");
    getThreadId();
}
void function3(void*){
    wait_r(semaphore, 3);
    printingString("Thread 3 done!\n");
    getThreadId();
}
void function4(void*){
    signal_r(semaphore, 2);
    signal_r(semaphore, 2);
    signal_r(semaphore, 2);
    printingString("Thread 4 done!\n");
    getThreadId();
}

void function5(void*){
    //for(int i=0; i<5; i++)
    //getThreadId();
    //thread_dispatch();
    for(int i=0; i<1e8; i++){
    }
}
void function7(void*){
    //TCB::join_all();
    return;
}
void function6(void*){
    thread_t threads[2];
    thread_create(&threads[0], function7, nullptr);
    thread_create(&threads[1], function7, nullptr);
    //join_all();
}
void main(){

    uint64 m = 0x02;
    __asm__ volatile ("csrw stvec, %[vector]":: [vector] "r" (&supervisorTrap));
    __asm__ volatile ("csrs sstatus, %[m]"::[m]"r"(m));
    //userMain();
    //void* ptr = mem_alloc(65);
    //mem_free(ptr);


    thread_t threads[25];
    //int a = 10;
    //void* p = &a;

    //TCB::setMaximumThreads(3);
    thread_create(&threads[0], nullptr, nullptr);
    TCB::running = threads[0];


    thread_create(&threads[1], wrapper, nullptr);
    printingString("Thread1 created\n");


    if(threads[1] == nullptr){
        printingString("Sta ces\n");
    }

    while(!(threads[1]->isFinished())){
            //printingString("Boze daj da ovo proradi\n");
            thread_dispatch();
    }
    join_all();


    /*
    for (auto & thread : threads) {
        delete thread;
    }*/

    printingString("Finished!\n");

    /*
    Thread* threads[N];
    Data* data[N];

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            mat[i][j] = i;
        }
    }
    for(int i = 0; i < N; i++){
        data[i] = new Data();
        data[i]->id = i;
        data[i]->array = mat[i];
        threads[i] = new Huh(data[i]);
        data[i]->thread = threads[i];
        //printInt(i);
        //printString(". Thread created\n");
    }

    for(int i=0; i<N; i++) {
        //printString("Boze pomozi\n");
        threads[i]->start();
    }

    int sum = 0;
    for(int i=0; i<N; i++){
        threads[i]->join();
    }

    int maxElem = data[0]->maxElem;

    for(int i=0; i<N;i++){
        sum += data[i]->sum;
        if (data[i]->maxElem > maxElem){
            maxElem = data[i]->maxElem;
        }
    }

    for (auto thread: threads) { delete thread; }

    printString("Suma = ");
    printInt(sum);
    printString("\n");
    printString("Max element = ");
    printInt(maxElem);
    printString("\n");
     */

}

/*
 const int N = 10;
int mat[N][N];

struct Data{
    int* array;
    Thread* thread;
    int id;
    int maxElem;
    int sum;
};
class Huh: public Thread {
    void sum(void* arg);
    void max(void* arg);
    void* arg;
public:
    Huh(void* arg):Thread() {
        this->arg = arg;
    }

    void run() override {
        sum(arg);
        max(arg);
    }
};
void Huh::sum(void *arg) {
    struct Data *data = (struct Data *) arg;
    int* array = data->array;
    int id = data->id;

    int sum = 0;
    for(int i=0; i<N;i++){
        sum += array[i];
    }
    data->sum =  sum;
    printInt(id);
    printString(" finished!\n");
}

void Huh::max(void *arg) {
    struct Data *data = (struct Data *) arg;
    int* array = data->array;
    int id = data->id;

    int maxElem = array[0];
    for(int i = 1; i < N; i++){
        if(array[i] > maxElem){
            maxElem = array[i];
        }
    }
    data->maxElem = maxElem;
    printInt(id);
    printString(" finished!\n");
}
 */