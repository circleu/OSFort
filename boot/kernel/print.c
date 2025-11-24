#include "print.h"


GRAPHICS graphics;

void kputc(const char ch) {
    switch (ch) {
        case '\n': {
            graphics.y++;
            graphics.x = 0;
            
            return;
        }
        case '\r': graphics.x = 0; return;
        case '\t': {
            for (size_t i = 0; i < 4; i++) {
                kputc(' ');
            }
            return;
        }
    }

    BITMAP ch_bitmap;
    ch_bitmap.buffer = char_to_bitmap(ch);
    ch_bitmap.size = FONT_HEIGHT;

    if (ch_bitmap.buffer == NULL) return;

    for (uint8_t i = 0; i < FONT_HEIGHT; i++)
        for (uint8_t j = 0; j < FONT_WIDTH; j++)
            if (get_bitmap(&ch_bitmap, i * FONT_WIDTH + (FONT_WIDTH - j - 1))) *(uint32_t*)(graphics.framebuffer_base + (graphics.pixels_per_scanline * (graphics.y * FONT_HEIGHT + i) * 4) + ((graphics.x * FONT_WIDTH + j) * 4)) = 0xaaaaaaaa;

    graphics.x++;

    if (graphics.x >= graphics.pixels_per_scanline / FONT_WIDTH) {
        graphics.x -= graphics.pixels_per_scanline / FONT_WIDTH;
        graphics.y++;
    }
}
void kputs(const char* str) {
    for (size_t i = 0; str[i] != 0; i++)
        kputc(str[i]);
}
void kprintf(const char* str, ...) {
    va_list ap;

    va_start(ap, str);

    for (size_t i = 0; str[i] != 0; i++) {
        if (str[i] == '%') {
            if (str[i + 1] == 'd') { i++;
                kputs(itoa(va_arg(ap, int)));
            }
            else if (str[i + 1] == 's') { i++;
                kputs(va_arg(ap, char*));
            }
            else if (str[i + 1] == 'x') { i++;
                kputs(hex(va_arg(ap, int)));
            }
        }
        else {
            kputc(str[i]);
        }
    }

    va_end(ap);
}
void clear_line(unsigned int line) {
    graphics.x = 0;
    for (size_t i = 0; i < graphics.width; i++) {
        *(uint32_t*)(graphics.framebuffer_base + graphics.y * graphics.pixels_per_scanline + i) = 0x00000000;
    }
}
void clear_screen(uint32_t color) {
    graphics.x = 0;
    graphics.y = 0;

    for (size_t i = 0; i < graphics.height; i++) {
        for (size_t j = 0; j < graphics.width; j++) {
            *(uint32_t*)(graphics.framebuffer_base + i * graphics.pixels_per_scanline + j) = color;
        }
    }
}
uint64_t pow(uint64_t a, uint64_t b) {
    if (b == 0) return 1;

    uint64_t temp = a;
    while (b - 1 > 0) {
        temp *= a;
        b--;
    }

    return temp;
}
char* itoa(uint64_t num) {
    static char temp[21];
    int k = 0;

    for (int i = 0; i < 21; i++) temp[i] = 0;

    if (num >= pow(10, 19)) temp[0] = num / pow(10, 19) + 48;
    else k++;

    for (uint64_t i = pow(10, 18); num < i; i /= 10) k++;
    
    if (k == 20) {
        temp[0] = '0';
        return (char*)temp;
    } 

    for (int i = 0; i < 20 - k; i++) {
        uint64_t j; j = pow(10, 19 - i - k);
        temp[i] = num % (j * 10) / j + 48;
    }

    return (char*)temp;
}
char* hex(uint64_t num) {
    static char temp[17];
    char hex[17] = "0123456789ABCDEF";

    for (int i = 0; i < 17; i++) temp[i] = 0;

    for (int i = 0; i < 16; i++) {
        uint8_t byte; byte = (num & ((uint64_t)0xf << i * 4)) >> i * 4;

        temp[15 - i] = hex[byte];
    }

    return (char*)temp;
}
char* hexbyte(uint8_t num) {
    static char temp[3];
    char hex[17] = "0123456789ABCDEF";

    for (int i = 0; i < 3; i++) temp[i] = 0;

    for (int i = 0; i < 2; i++) {
        uint8_t byte; byte = (num & ((uint64_t)0xf << i * 4)) >> i * 4;

        temp[1 - i] = hex[byte];
    }

    return (char*)temp;
}