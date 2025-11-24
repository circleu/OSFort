#ifndef _KERNEL_PRINT_H
#define _KERNEL_PRINT_H

#include "../../sys/stdarg.h"
#include "bootinfo.h"
#include "font.h"
#include "bitmap.h"


typedef struct {
    uint64_t framebuffer_base;
    size_t framebuffer_size;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
    uint32_t x;
    uint32_t y;
}__attribute__((packed)) GRAPHICS;

void kputc(const char ch);
void kputs(const char* str);
void kprintf(const char* str, ...);
void clear_line(unsigned int line);
void clear_screen(uint32_t color);
uint64_t pow(uint64_t a, uint64_t b);
char* itoa(uint64_t num);
char* hex(uint64_t num);
char* hexbyte(uint8_t num);


#endif