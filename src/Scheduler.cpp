//
// Created by os on 5/20/22.
//

#include "../h/Scheduler.h"


List<PCB> Scheduler::ThreadQueue;

PCB* Scheduler::get() {
    return Scheduler::ThreadQueue.removeFirst();
}
void Scheduler::put(PCB *p) {
    Scheduler::ThreadQueue.addLast(p);

}

PCB* Scheduler::peekFirst() {
    return ThreadQueue.peekFirst();
}

