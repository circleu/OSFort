#include "gdt.h"


GDT gdt = {
{0x0000, 0, 0, 0x00, 0b0000, 0b0000, 0},
{0xffff, 0, 0, 0x9a, 0b1111, 0b1010, 0},
{0xffff, 0, 0, 0x92, 0b1111, 0b1100, 0},
{0xffff, 0, 0, 0xfa, 0b1111, 0b1010, 0},
{0xffff, 0, 0, 0xf2, 0b1111, 0b1100, 0}
};
GDTR gdtr = {
    sizeof(GDT) - 1,
    (uint64_t)&gdt
};