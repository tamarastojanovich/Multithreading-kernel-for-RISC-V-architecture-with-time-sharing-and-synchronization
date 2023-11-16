//
// Created by os on 5/20/22.
//
#include "../h/MemoryAllocator.hpp"
#include "../h/PCB.hpp"
#include "../h/printing.hpp"
class Thread;

using Body=void(*)(void*);

void* mem_alloc(size_t size) {
    uint64 call=0x01;
    __asm__ volatile("mv a0,%[param]"::[param]"r"(call));
    __asm__ volatile("mv a1,%[param]" ::[param]"r"(size));
    __asm__ volatile("ecall");
     void* returnval;
    __asm__ volatile("mv %[param],a0" :[param] "=r"(returnval));
    return (char*)returnval;
}

int mem_free(void* v){
    Mem** addr=(Mem**) &v;
    size_t sz=sizeof(v);
    __asm__ volatile("mv a1,%[param]"::[param]"r"(sz));
    __asm__ volatile("mv a2,%[param]" ::[param] "r"(addr));
    __asm__ volatile("mv a0,%[param]" ::[param] "r"(0x02));
    __asm__ volatile("ecall");
    int retur;
    __asm__ volatile("mv %[param],a0" :[param] "=r"(retur));
    return retur;
}

int thread_create(thread_t* handle,
                  void(*start_routine)(void*),
                  void* arg){
    mem_alloc(sizeof(PCB)+MEM_BLOCK_SIZE-1);
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x11));
    __asm__ volatile ("mv a1,%[param]"::[param]"r"(&handle));

    __asm__ volatile("mv a2,%[param]"::[param]"r"(start_routine));
    __asm__ volatile("mv a3,%[param]"::[param]"r"(arg));

    __asm__ volatile("ecall");
    int id;
    __asm__ volatile("mv %[param],a0":[param]"=r"(id));
    return id;
}

void thread_dispatch(){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x13));
    __asm__ volatile("ecall");
}

int thread_exit(){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x12));
    __asm__ volatile("ecall");
    return 0;
}

void wrapper(void* t) {
    Thread* thr=(Thread*)t;
    thr->run();
    return;
}

int sem_open(sem_t *handle,unsigned  init){
    mem_alloc(sizeof(Semaphore)+MEM_BLOCK_SIZE-1);
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x21));
    __asm__ volatile ("mv a1,%[param]"::[param]"r"(handle));
    __asm__ volatile("mv a2,%[param]"::[param]"r"(init));

    __asm__ volatile("ecall");

    int id;
    __asm__ volatile("mv %[param],a0":[param]"=r"(id));
    return id;
}

int sem_close(sem_t handle){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x22));
    sem_t* pok=&handle;
    __asm__ volatile("mv a1,%[param]"::[param]"r"(pok));
    __asm__ volatile("ecall");

    int id;
    __asm__ volatile("mv %[param],a0":[param]"=r"(id));
    return id;
}

int sem_wait(sem_t id){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x23));
    sem_t *pok=&id;
    __asm__ volatile("mv a1,%[param]"::[param]"r"(pok));
    __asm__ volatile("ecall");
    int i;
    __asm__ volatile("mv %[param],a0":[param]"=r"(i));
    return i;
}

int sem_signal(sem_t id){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x24));
    sem_t* pok=&id;
    __asm__ volatile("mv a1,%[param]"::[param]"r"(pok));
    __asm__ volatile("ecall");
    int i;
    __asm__ volatile("mv %[param],a0":[param]"=r"(i));
    return i;
}

char getc(){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x25));
    __asm__ volatile("ecall");
    char i;
    __asm__ volatile("mv %[param],a0":[param]"=r"(i));
    return i;
}

void putc(char c){
    __asm__ volatile("mv a0,%[param]"::[param]"r"(0x26));
    __asm__ volatile("mv a1,%[param]"::[param]"r"(&c));
    __asm__ volatile("ecall");
}


