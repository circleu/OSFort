#ifndef _SYS_STRING_H
#define _SYS_STRING_H

#include "stdint.h"

typedef uint64_t size_t;
#define NULL ((void*)0)

/* ... */
void* memcpy(void* restrict s1, const void* restrict s2, size_t n);
void* memset(void* s, int c, size_t n);
/* ... */
int strncmp(const char* s1, const char* s2, size_t n);
/* ... */


#endif