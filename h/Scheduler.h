//
// Created by os on 5/20/22.
//

#ifndef NEKI_DODATNI_FAJLOVI_SCHEDULER_H
#define NEKI_DODATNI_FAJLOVI_SCHEDULER_H

#include "List.h"

class PCB;

class Scheduler {
private:
    static List<PCB> ThreadQueue;
public:

    static PCB* get();
    static void put(PCB* p);
    static PCB* peekFirst();
};


#endif //NEKI_DODATNI_FAJLOVI_SCHEDULER_H
