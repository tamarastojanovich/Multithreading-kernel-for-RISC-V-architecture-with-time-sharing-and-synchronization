//
// Created by os on 6/15/22.
//

#include  "../h/Scheduler.h"
#include "../h/printing.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/PCB.hpp"

using Body=void(*)(void*);
Thread::Thread(void (*body)(void *), void *arg) {
    mem_alloc(sizeof(Thread)+MEM_BLOCK_SIZE-1);
    thread_create(&this->myHandle,body,arg);
}

void Thread::start() {
   if(myHandle==0) {
       mem_alloc(sizeof(Thread)+MEM_BLOCK_SIZE-1);
       Thread* t=this;
       thread_create(&myHandle, wrapper,t);
   }
   }

void Thread::dispatch(){
    thread_dispatch();
}

Thread::~Thread() {
    myHandle=0;
}

Thread::Thread() {
   myHandle=0;
}

void* Thread::operator new(size_t size) {
    Thread* thr=(Thread*)mem_alloc(sizeof(Thread));
    return thr;
}

void Thread::operator delete(void *v) {
   mem_free(v);
}