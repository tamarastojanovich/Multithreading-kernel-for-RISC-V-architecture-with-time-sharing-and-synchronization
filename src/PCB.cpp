//
// Created by os on 5/19/22.
//

#include "../h/PCB.hpp"
#include "../h/riscv.hpp"
#include "../h/Scheduler.h"
#include "../h/printing.hpp"
#include "../h/MemoryAllocator.hpp"

PCB *PCB::running=nullptr;
bool PCB::first=false;
int PCB::time=0;

PCB *PCB::createPCB(Body bod,void* arg) {
    MemoryAllocator m=MemoryAllocator::getInstance();
    PCB* pcb=(PCB*)m._mem_alloc_(sizeof(PCB)+MEM_BLOCK_SIZE-1);
    pcb->body=bod;
    pcb->arg=arg;
    if(pcb->body!=nullptr) pcb->stack=(uint64*)m._mem_alloc_(sizeof(uint64)*STACK_SIZE);
    else pcb->stack=nullptr;
    if(pcb->body!=nullptr) pcb->context.ra=(uint64)threadWrapper;
    else pcb->context.ra=0;
    if(pcb->stack!=nullptr) {
        pcb->context.sp=(uint64) &pcb->stack[STACK_SIZE];
    }
    else pcb->context.sp=0;
    pcb->finished=false;
    pcb->blocked=false;
    pcb->signalOnClose=false;
    if(bod!=nullptr) Scheduler::put(pcb);
    return pcb;
}
void PCB::yield() {

   Riscv::pushRegisters();

    PCB::dispatch();

   Riscv::popRegisters();
}

void PCB::dispatch() {
    PCB *old=(PCB*)running;
    if(old->isFinished()==false){
        Scheduler::put((PCB*)old);}
    running=(PCB*)Scheduler::get();
    if(running->body==nullptr && Scheduler::peekFirst()){
        PCB* pcb=running;
        running=Scheduler::get();
        Scheduler::put(pcb);
    }
    contextSwitch(&old->context,&running->context);
}

void PCB::threadWrapper() {
   if(running->body!=nullptr){
      running->body(running->arg);
   }
    running->setFinished(true);
    PCB::dispatch();
}

PCB::PCB(Body bod, void *arg) {
    MemoryAllocator m=MemoryAllocator::getInstance();
    body=bod;
    this->arg=arg;
    timeslice=DEFAULT_TIME_SLICE;
    if(body!=nullptr) stack=(uint64*)m._mem_alloc_(sizeof(uint64)*STACK_SIZE);
    else stack=nullptr;
    if(body!=nullptr) context.ra=(uint64)threadWrapper;
    else context.ra=0;
    if(stack!=nullptr) {
        context.sp=(uint64) &stack[STACK_SIZE-1];
    }
    else context.sp=0;
    finished=false;
    if(bod!=nullptr) Scheduler::put(this);
}

void *PCB::operator new(size_t size) {
    MemoryAllocator m=MemoryAllocator::getInstance();
    PCB* pcb=(PCB*)m._mem_alloc_(size);
    return pcb;
}
void PCB::operator delete(void *v) {
    MemoryAllocator::getInstance()._mem_free_(v,sizeof(v));
}
int PCB::getTimeSlice() {
    return timeslice;
}
