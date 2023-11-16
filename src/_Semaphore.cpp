//
// Created by os on 6/26/22.
//
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"

void Semaphore::wait() {
    sem_wait(myHandle);
}

void Semaphore::signal() {
    sem_signal(myHandle);
}

Semaphore::Semaphore(unsigned int init) {
    mem_alloc(sizeof(Semaphore)+MEM_BLOCK_SIZE-1);
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    myHandle=0;
}

void* Semaphore::operator new(size_t size) {
    Semaphore* sem=(Semaphore*) mem_alloc(size);
    return sem;
}

void Semaphore::operator delete(void *v) {
    mem_free(v);
}