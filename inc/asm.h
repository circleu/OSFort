#ifndef _IO_H
#define _IO_H

#include "../sys/stdint.h"


void outb(uint8_t origin, uint16_t dest);
uint8_t inb(uint16_t origin);
void outl(uint32_t origin, uint16_t dest);
uint32_t inl(uint16_t origin);
void io_wait();
uint64_t rdmsr(uint32_t msr);
void wrmsr(uint32_t msr, uint64_t value);
void invlpg(uint64_t vir_addr);


#endif