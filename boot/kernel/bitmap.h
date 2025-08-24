#ifndef _KERNEL_BITMAP_H
#define _KERNEL_BITMAP_H

#include "../../sys/stdint.h"
#include "../../sys/stddef.h"


typedef struct {
    uint8_t* buffer;
    size_t size;
}__attribute__((packed)) BITMAP;

char get_bitmap(BITMAP* bitmap, size_t index);
char set_bitmap(BITMAP* bitmap, size_t index, uint8_t value);


#endif