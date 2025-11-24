#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

#include "../../sys/stdint.h"
#include "../../sys/stddef.h"
#include "../../sys/string.h"
#include "bootinfo.h"
#include "bitmap.h"
#include "font.h"
#include "gdt.h"
#include "paging.h"
#include "idt.h"
#include "syscall.h"
#include "../../dev/pic/pic.h"
#include "../../dev/pci/pcie.h"
#include "../../dev/acpi/acpi.h"
#include "../../dev/ahci/ahci.h"
#include "../../dev/nvme/nvme.h"
#include "asm.h"
#include "print.h"

#define KERNEL_START 0x0000FFFF00000000


void enter_usr_mode(void* entry, void* usr_stack) {
    __asm__ volatile (
        "cli;"
        "mov %0, %%rsp;"
        "pushq $0x20;"
        "pushq %%rsp;"
        "pushq $0x202;"
        "pushq $0x18;"
        "pushq %1;"
        "iretq"
        ::"r"(usr_stack), "r"(entry)
    );
}

extern size_t _kernel_start;
extern size_t _kernel_end;
extern GDT gdt;
extern GDTR gdtr;
extern IDT idt;
extern IDTR idtr;
extern PAGE_BITMAP page_bitmap;
extern GRAPHICS graphics;
extern uint8_t asq[NVME_ADMIN_QUEUE_ENTRIES * NVME_SQE_SIZE];
extern uint8_t acq[NVME_ADMIN_QUEUE_ENTRIES * NVME_CQE_SIZE];
extern NVME_IDENTIFY_CONTROLLER_DATA_STRUCTURE identify_buf;
extern __attribute__((sysv_abi)) void jmp_hh();


#endif