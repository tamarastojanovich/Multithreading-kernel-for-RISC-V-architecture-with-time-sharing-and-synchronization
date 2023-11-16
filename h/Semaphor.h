//
// Created by os on 6/26/22.
//

#ifndef NEKI_DODATNI_FAJLOVI_SEMAPHOR_H
#define NEKI_DODATNI_FAJLOVI_SEMAPHOR_H


#include "../h/Queue.h"
class PCB;

class Semaphor {
public:
    static Semaphor* createSemaphore(unsigned short init);
    int wait();
    int signal();
    int getVal(){return val;}
    int closeSem();


    void* operator new(size_t size);

    void operator delete(void* v);
private:

    int val;
    Queue* blocked;
};


#endif //NEKI_DODATNI_FAJLOVI_SEMAPHOR_H
