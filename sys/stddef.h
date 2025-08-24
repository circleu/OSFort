#ifndef _SYS_STDDEF_H
#define _SYS_STDDEF_H

#include "stdint.h"

typedef long double max_align_t;
typedef int64_t ptrdiff_t;
typedef uint16_t wchar_t;
typedef uint64_t size_t;

#define NULL ((void*)0)
#define offsetof(s, m) (size_t)&(((s*)0)->m)

#endif