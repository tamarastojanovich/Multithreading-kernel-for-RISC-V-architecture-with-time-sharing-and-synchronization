//
// Created by os on 6/28/22.
//
#include "../h/syscall_cpp.hpp"
#include "../lib/console.h"
char Console::getc() {
    return __getc();
}

void Console::putc(char c) {
    __putc(c);
}