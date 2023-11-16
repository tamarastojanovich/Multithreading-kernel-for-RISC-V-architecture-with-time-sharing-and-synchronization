//
// Created by os on 6/26/22.
//

#include "../h/Semaphor.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.h"
#include "../h/printing.hpp"

Semaphor* Semaphor::createSemaphore(unsigned short init) {
    Semaphor* sem=(Semaphor*)MemoryAllocator::getInstance()._mem_alloc_(sizeof(Semaphor)+MEM_BLOCK_SIZE-1);
    sem->val=init;
  sem->blocked=new Queue();
  return sem;
}


int Semaphor::wait() {
    int ret=0;
    if(--val<0){
        PCB* old=PCB::running;
        blocked->add(old);
        PCB::running=Scheduler::get();
        while(PCB::running->BodyNull()){
            Scheduler::put(PCB::running);
            PCB::running=Scheduler::get();
        }
        PCB::contextSwitch((old->getContext()),PCB::running->getContext());

    }
    if(PCB::running->OnClose()) ret=-1;
    return ret;
}

int Semaphor::signal() {
   if(++val<=0 && (!blocked->isEmpty())){
       PCB* p=blocked->get();
       Scheduler::put((PCB*)p);
   }
    return 0;
}

int Semaphor::closeSem() {
    while(val<=0 && (!blocked->isEmpty())){
        val++;
        PCB* p=blocked->get();
        p->SetSignal(true);
        Scheduler::put(p);
    }
    return 0;
}
void* Semaphor::operator new(size_t size) {
    Semaphor* sem=(Semaphor*)MemoryAllocator::getInstance()._mem_alloc_(sizeof(Semaphor)+MEM_BLOCK_SIZE-1);
    return sem;
}

void Semaphor::operator delete(void *v) {
    MemoryAllocator::getInstance()._mem_free_(v,sizeof(v));
}