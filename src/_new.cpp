#include "../h/syscall_cpp.hpp"

#include "../lib/mem.h"


void* operator new(size_t n)
{
    return mem_alloc(n);
}

void* operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p)
{
    mem_free(p);
}

void operator delete[](void *p)
{
    mem_free(p);
}