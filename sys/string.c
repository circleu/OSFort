#include "string.h"


void* memset(void* s, int c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((unsigned char*)s)[i] = (unsigned char)c;
    }

    return s;
}
int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; (i < n) && (s1[i] || s2[i]); i++) {
        if (s1[i] != s2[i]) {
            if (s1[i] > s2[i]) return 1;
            else return -1;
        }
    }
    return 0;
}