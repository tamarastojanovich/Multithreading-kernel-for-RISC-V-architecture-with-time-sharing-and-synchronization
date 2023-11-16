#ifndef _syscall_cpp
#define _syscall_cpp

#include "../h/syscall_c.hpp"

void* operator new (size_t sz);
void operator delete (void* free);
void* operator new[](size_t sz);
void operator delete[](void* free);

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    void start ();
    static void dispatch ();
    static void sleep (time_t);//ovo je tek za 4. zadatak
    static void* operator new(size_t size);
    static void operator delete(void* v);
protected:
    friend class PCB;
    friend void wrapper(void* t);
    Thread ();
    virtual void run () {}

private:
    thread_t myHandle;
};




class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    void wait ();
    void signal ();
    static void* operator new(size_t size);
    static void operator delete(void* v);
private:
    sem_t myHandle;
};


class PeriodicThread : public Thread{
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
};
class Console {
public:
    static char getc ();
    static void putc (char);
};
#endif