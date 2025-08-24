#ifndef _KERNEL_PROC_H
#define _KERNEL_PROC_H

#include "../../sys/stdint.h"


typedef struct {
    uint64_t pid;
    uint64_t* page_tbl;
    void* entry;
    void* usr_stack;
}__attribute__((packed)) PROCESS;


#endif