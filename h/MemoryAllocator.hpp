



#include "../lib/hw.h"




//
// Created by os on 5/17/22.
//

#ifndef NEKI_DODATNI_FAJLOVI_MEMORYALLOCATOR_H
#define NEKI_DODATNI_FAJLOVI_MEMORYALLOCATOR_H


struct FreeMem{
    FreeMem *next,*prev;
    size_t sz;
};

struct Mem{
    Mem *next,*prev;
    size_t sz;
    char* addr;
    Mem():next(0),prev(0),addr(0){}
};

class MemoryAllocator {
public:
  static MemoryAllocator getInstance();

    void* _mem_alloc_(size_t sz);
    int _mem_free_(void* v,size_t sz);
     static Mem* free;

private:
    MemoryAllocator(){
        free=(Mem*)HEAP_START_ADDR;
        free->sz=(size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
        free->addr=(char*)HEAP_START_ADDR;
    }




};


#endif
