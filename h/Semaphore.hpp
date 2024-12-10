#include "TCB.hpp"

class Semaphore_ {
public:
    Semaphore_(unsigned short value = 1): handle(nullptr), init(value), timeout(0){};

    static int sem_open (sem_t* handle, unsigned init);
    static int sem_close (sem_t handle);
    int sem_wait (sem_t id);
    int sem_signal (sem_t id);
    int sem_timedwait(sem_t id, time_t timeout); ;
    int sem_trywait(sem_t id);
    int priority_signal(sem_t id);
    static List<Semaphore_> semWaitingTimeout;


    static int open_r (sem_t* handle, unsigned value);
    static int close_r (sem_t handle);
    int wait_r (sem_t id, unsigned value);
    int signal_r (sem_t id, unsigned value);

    void decrementTimeout(){
        timeout--;
    }

    time_t getTimeout() {
        return timeout;
    }

    static int getNumOfTimeout(){
        return numOfTimeout;
    }

    sem_t getSemID(){
        return *handle;
    }

    int getInit(){
        return init;
    }

protected:
    void block();
    void unblock();
    void priority_unblock();

private:
    sem_t* handle;
    signed int init;
    //static const int NUM_OF_BLOCKED = 50;
    List<TCB> readyBlockedQueue;
    List<int> values;
    int numOfBlocked;
    time_t timeout;
    static int numOfTimeout;
};