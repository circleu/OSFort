#include "bitmap.h"


char get_bitmap(BITMAP* bitmap, size_t index) {
    if (index > bitmap->size * 8) return 0;
    size_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit = 1 << bit_index;

    if ((bitmap->buffer[byte_index] & bit)) return 1;
    else return 0;
}
char set_bitmap(BITMAP* bitmap, size_t index, uint8_t value) {
    if (index > bitmap->size * 8) return 0;
    size_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit = 1 << bit_index;

    if (value)
        bitmap->buffer[byte_index] |= bit;
    else
        bitmap->buffer[byte_index] &= ~bit;
    
    return 1;
}