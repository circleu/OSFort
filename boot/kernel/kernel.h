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
#include "../../inc/asm.h"


typedef uint64_t size_t;
typedef struct {
    uint64_t framebuffer_base;
    size_t framebuffer_size;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
    uint32_t x;
    uint32_t y;
}__attribute__((packed)) GRAPHICS;

void kputc(const char ch, GRAPHICS* graphics) {
    switch (ch) {
        case '\n': graphics->y++; return;
        case '\r': graphics->x = 0; return;
    }

    BITMAP ch_bitmap;
    ch_bitmap.buffer = char_to_bitmap(ch);
    ch_bitmap.size = FONT_HEIGHT;

    if (ch_bitmap.buffer == NULL) return;

    for (uint8_t i = 0; i < FONT_HEIGHT; i++)
        for (uint8_t j = 0; j < FONT_WIDTH; j++)
            if (get_bitmap(&ch_bitmap, i * FONT_WIDTH + (FONT_WIDTH - j - 1))) *(uint32_t*)(graphics->framebuffer_base + (graphics->pixels_per_scanline * (graphics->y * FONT_HEIGHT + i) * 4) + ((graphics->x * FONT_WIDTH + j) * 4)) = 0xaaaaaaaa;

    graphics->x++;

    if (graphics->x >= graphics->pixels_per_scanline / FONT_WIDTH) {
        graphics->x -= graphics->pixels_per_scanline / FONT_WIDTH;
        graphics->y++;
    }
}
void kputs(const char* str, GRAPHICS* graphics) {
    for (size_t i = 0; str[i] != 0; i++)
        kputc(str[i], graphics);
}
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


#endif