//
// Created by os on 5/19/22.
//

#ifndef NEKI_DODATNI_FAJLOVI_THREAD_H
#define NEKI_DODATNI_FAJLOVI_THREAD_H


#include "../lib/hw.h"
#include "syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"


class Scheduler;

//mozemo da definisemo nekakvu listu PCBA i onda da trazimo kome odg thread_t i tako pokrenemo odgovarajuci ili izbacimo iz liste?
class PCB {
public:
    ~PCB(){
    MemoryAllocator m=MemoryAllocator::getInstance();
    m._mem_free_((uint64*)stack,sizeof(stack));
    stack=nullptr;}
    bool isFinished() const {return finished; }
    void setFinished(bool val){finished=val;}

    bool isBlocked() const{return blocked;}
    void setBlocked(bool val){blocked=true;}

    bool OnClose() const{return signalOnClose;}
    void SetSignal(bool val){signalOnClose=val;}


    using Body=void(*)(void*);
    using Body1=void(*)();
    static PCB *createPCB(Body bod,void* arg);
    PCB* createPCB1(Thread* thr);

    bool BodyNull(){if(body==nullptr) return true;
    else return false;}



    static void threadWrapper();

    static void yield();
    static PCB* idle;

    static PCB* running;
    static int time;

    static void* operator new(size_t size);
    static void operator delete(void* v);

    int getTimeSlice();
    struct Context{
        uint64 ra;
        uint64 sp;
    };

    Context * getContext(){return &context;}

    static void contextSwitch(Context* oldContext,Context* newContext);
private:
    friend class Thread;
    explicit PCB(Body bod,void* arg);
    explicit PCB(Thread* thr);


    static bool first;
    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    int timeslice=DEFAULT_TIME_SLICE;
    bool finished=false;
    bool blocked=false;
    bool signalOnClose=false;




    static void dispatch();

    static uint64 constexpr STACK_SIZE=1024;
};



#endif //NEKI_DODATNI_FAJLOVI_THREAD_H
