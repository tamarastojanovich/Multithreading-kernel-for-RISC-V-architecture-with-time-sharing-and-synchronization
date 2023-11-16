//
// Created by os on 6/26/22.
//

#include "../h/Queue.h"

bool Queue::isEmpty() {
    if(head==0) return true;
    else return false;
}

void Queue::add(PCB* p){
    if(head==0){
        head=tail=new El(p,0);
    }else{
        tail->next=new El(p,0);
        tail=tail->next;
    }

}


PCB* Queue::get() {
    if(!head) return 0;
    else{
        El* elem=head;
        head=head->next;
        if(!head) {tail=0;}
        PCB* p=elem->p;
        elem=nullptr;

        return p;
    }
}

void* Queue::operator new(size_t sz) {
    Queue* q=(Queue*)MemoryAllocator::getInstance()._mem_alloc_(sz);
    q->head=0;
    q->tail=0;
    return q;
}

void Queue::operator delete(void *v) {
    MemoryAllocator::getInstance()._mem_free_(v,sizeof(v));
}