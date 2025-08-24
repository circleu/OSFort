#ifndef _KERNEL_FORT_H
#define _KERNEL_FORT_H

#include "../../sys/stdint.h"
#include "../../sys/stddef.h"

#define FONT_WIDTH 8 // px
#define FONT_HEIGHT 18 // px


typedef uint8_t charbitmap[FONT_HEIGHT];

uint8_t* char_to_bitmap(const char c);


#endif