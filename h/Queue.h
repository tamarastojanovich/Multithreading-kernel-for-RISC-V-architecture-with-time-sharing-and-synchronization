//
// Created by os on 6/26/22.
//

#ifndef NEKI_DODATNI_FAJLOVI_QUEUE_H
#define NEKI_DODATNI_FAJLOVI_QUEUE_H

#include "PCB.hpp"

struct El {
    PCB* p;
    El* next;
    El(PCB *data, El *next) : p(data), next(next) {
        MemoryAllocator::getInstance()._mem_alloc_(sizeof(El)+MEM_BLOCK_SIZE-1);
    }
    ~El(){
        MemoryAllocator::getInstance()._mem_free_(this,sizeof(this));
    }
};

class Queue {
public:
    void add(PCB* p);
    PCB* get();
    bool isEmpty();
    static void* operator new(size_t sz);
    static void operator delete(void* v);
private:
    El* head=0,*tail=0;
};


#endif //NEKI_DODATNI_FAJLOVI_QUEUE_H
