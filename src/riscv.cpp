#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../lib/mem.h"
#include "../lib/console.h"
#include "../h/PCB.hpp"
#include "../h/printing.hpp"
#include "../h/Semaphor.h"
using Body=void(*)(void*);

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}
void Riscv::handleSupervisorTrap() {
    volatile uint64 cause = r_scause();
    cause=cause&(0xffffffffffffffff);
    //printString(""+cause);

    if (cause==0x0000000000000008UL || cause==0x0000000000000009UL ) {//mem_alloc
        w_sip(0);
        uint64 function;
        __asm__ volatile("mv %0,a0": "=r"(function));
        switch (function) {
            case (0x01): {
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus = r_sstatus();

                size_t sz;

                __asm__ volatile ("ld s1,11*8(fp)");

                __asm__ volatile("mv %0,s1":"=r"(sz));
               volatile void* ret=MemoryAllocator::getInstance()._mem_alloc_(sz);
               __asm__ volatile("mv a0,%[param]"::[param] "r"(ret));

               ret=nullptr;

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case (0x02): {

                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus = r_sstatus();

                size_t sz;
                void** addr;


                __asm__ volatile ("ld s1,11*8(fp)");
                __asm__ volatile ("ld s2,12*8(fp)");

                __asm__ volatile ("mv %[param],s2":[param] "=r"(addr));
                __asm__ volatile("mv %[param],s1":[param]"=r"(sz));


                int ret=MemoryAllocator::getInstance()._mem_free_(*addr,sz);
               __asm__ volatile ("mv a0,%[param]"::[param] "r"(ret));

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);


                break;
            }
            case(0x11): {//creating a thread
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                Body body;
                void* arg=nullptr;
                thread_t* handle=nullptr;

                __asm__ volatile ("ld s1,11*8(fp)");
                __asm__ volatile ("ld s2,12*8(fp)");
                __asm__ volatile("ld s3,13*8(fp)");


                __asm__ volatile("mv %[param],s3":[param]"=r"(arg));
                __asm__ volatile ("mv %[param],s2":[param] "=r"(body));
                __asm__ volatile("mv %[param],s1":[param]"=r"(handle));

                int ret;
                *handle=(thread_t)PCB::createPCB(body,arg);
                if(*handle) ret=0;
                else ret=-1;

                arg=nullptr;
                __asm__ volatile("mv a0,%0"::"r"(ret));
                w_sstatus((uint64)sstatus);

                w_sepc((uint64)sepc);
                break;
            }
            case (0x12):{//set finished
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                PCB::running->setFinished(true);

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case(0x13):{//yield
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                PCB::yield();

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case(0x21):{//Semaphore create
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                sem_t* handle=nullptr;
                int init;

                __asm__ volatile ("ld s1,11*8(fp)");
                __asm__ volatile("ld s2,12*8(fp)");

                __asm__ volatile("mv %[param],s2":[param]"=r"(init));
                __asm__ volatile("mv %[param],s1":[param]"=r"(handle));

                int ret;

                *handle=(sem_t)Semaphor::createSemaphore(init);
                if(*handle) ret=0;
                else ret=-1;

                __asm__ volatile("mv a0,%0"::"r"(ret));
                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case(0x22):{//semaphore close
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                int ret;
                sem_t* handle;
                __asm__ volatile ("ld s1,11*8(fp)");

                __asm__ volatile("mv %[param],s1":[param]"=r"(handle));

                if(handle==nullptr) ret=-1;
                else ret=((Semaphor*)(*handle))->closeSem();

                __asm__ volatile("mv a0,%[param]"::[param]"r"(ret));

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case(0x23):{//semaphore wait
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                int ret;
                sem_t* handle;
                __asm__ volatile ("ld s1,11*8(fp)");

                __asm__ volatile("mv %[param],s1":[param]"=r"(handle));

                if(handle==nullptr)ret=-1;
                else ret=((Semaphor*)(*handle))->wait();

                __asm__ volatile("mv a0,%[param]"::[param]"r"(ret));
                w_sstatus((uint64)sstatus);

                w_sepc((uint64)sepc);
                break;
            }
            case(0x24):{//semaphore signal
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                int ret;
                sem_t* handle;
                __asm__ volatile ("ld s1,11*8(fp)");

                __asm__ volatile("mv %[param],s1":[param]"=r"(handle));

                if(handle==nullptr)ret=-1;
                else ret=((Semaphor*)(*handle))->signal();

                __asm__ volatile("mv a0,%[param]"::[param]"r"(ret));

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case(0x25):{
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                char c;

                c=__getc();

                __asm__ volatile("mv a0,%0"::"r"(c));

                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);
                break;
            }
            case(0x26):{
                volatile uint64 sepc=r_sepc()+4;
                volatile uint64 sstatus=r_sstatus();

                char* c=nullptr;
                __asm__ volatile ("ld s1,11*8(fp)");

                __asm__ volatile("mv %[param],s1":[param]"=r"(c));


                __putc(*c);


                w_sstatus((uint64)sstatus);
                w_sepc((uint64)sepc);

                break;
            }

            default: {
                break;
            }
        }
    }else if(cause == 0x8000000000000001UL) {
        mc_sip(SIP_SSIP);
    }else if (cause == 0x8000000000000009UL) {
        volatile uint64 sepc=r_sepc();
        volatile uint64 sstatus=r_sstatus();
        console_handler();

        w_sstatus((uint64)sstatus);
        w_sepc((uint64)sepc);
            } else {
                //unexpected interrupt
            }
        }




