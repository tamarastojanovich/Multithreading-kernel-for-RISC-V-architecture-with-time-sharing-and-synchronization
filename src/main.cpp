#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.h"
#include "../h/printing.hpp"
#include "../h/PCB.hpp"
extern void userMain(void * n);

int main() {

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SPIE);
    PCB* pcb[2];
    pcb[0]=PCB::createPCB(nullptr,nullptr);
    pcb[1]=PCB::createPCB(userMain,nullptr);
    PCB::running=pcb[0];
    PCB::yield();
    while(!pcb[1]->isFinished()){ printString("main");PCB::yield();}
    return 0;

}
