//
// Created by os on 5/17/22.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"

Mem* MemoryAllocator::free=nullptr;



MemoryAllocator MemoryAllocator::getInstance() {
    if(free==nullptr){
        free=(Mem*)HEAP_START_ADDR;
        free->sz=(size_t)HEAP_END_ADDR-(size_t)HEAP_END_ADDR;
        free->addr=(char*)HEAP_START_ADDR;
    }
    static MemoryAllocator m;
    return m;
}

void* MemoryAllocator::_mem_alloc_(size_t sz) {
    size_t sz1=(sz+sizeof(Mem))/MEM_BLOCK_SIZE;
    sz1+=sz%MEM_BLOCK_SIZE;
    sz=sz1*MEM_BLOCK_SIZE;
    Mem* curr=free;
    for(;curr!=nullptr;curr=curr->next) {
        if (curr->sz < sz) continue;

        if ((curr->sz - sz) <= sizeof(Mem)) {
            if (curr->prev) curr->prev->next = curr->next;
            else free = curr->next;
            if (curr->next) curr->next->prev = curr->prev;

            return (char*)curr->addr;
        } else {
            Mem *alloc = (Mem *) ((char *) curr + sz+sizeof(Mem));
            if (curr->prev) curr->prev->next = alloc;
            else free = alloc;
            if (curr->next) curr->next->prev = alloc;
            alloc->prev = curr->prev;
            alloc->next = curr->next;
            alloc->sz = curr->sz - sz;
            alloc->addr=curr->addr+sz+sizeof(Mem*);
           alloc=0;
            return (char*)curr->addr;

        }
    }
    curr=0;
    return nullptr;
}
int MemoryAllocator::_mem_free_(void *v,size_t sz) {
   Mem* addr=(Mem*) v;
   if((char*)addr<(char*) HEAP_START_ADDR || (char*)addr>(char*) HEAP_END_ADDR) return -1;
   Mem* curr;
   if(!free || (char*)v<(char*)free){
       curr=nullptr;
   }else{
       curr=free;
       for(;curr->next!=nullptr && (char*)addr>(char*)curr->next;curr=curr->next);
       if(curr!=nullptr && ((char*)curr+curr->sz)==(char*)addr){
        curr->sz+=sz;
        if(curr->next &&((char*)curr+curr->sz)==(char*)curr->next){
            curr->sz+=curr->next->sz;
            curr->next=curr->next->next;
            if(curr->next)curr->next->prev=curr;
        }
        return 0;
       }else{
           Mem* newfree=nullptr;
           if(curr) newfree=curr->next;
           else newfree=free;
           if(newfree && ((char*)addr+sz)==(char*)newfree){
               Mem* addfree=(Mem*) addr;
               addfree->sz+=sz;
               addfree->next=newfree->next;
               addfree->prev=newfree->prev;
               if(addfree->prev)addfree->prev->next=addfree;
               else free=addfree;
               if(addfree->next)addfree->next->prev=addfree;
               return 0;
           }else{
               Mem* newfree=(Mem*) addr;
               newfree->sz=sz;
               newfree->prev=curr;
               if(curr)newfree->next=curr->next;
               else newfree->next=free;
               if(newfree->next) newfree->next->prev=newfree;
               if(curr) curr->next=newfree;
               else free=newfree;
           }
       }
   }
   return 0;
}
