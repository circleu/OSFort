#ifndef _SYS_STDINT_H
#define _SYS_STDINT_H

/* Define exact widths */
#define INT8_MIN (-(1 << (8 - 1)))
#define INT8_MAX ((1 << (8 - 1)) - 1)
#define UINT8_MAX ((1 << 8) - 1)
#define INT16_MIN (-(1 << (16 - 1)))
#define INT16_MAX ((1 << (16 - 1)) - 1)
#define UINT16_MAX ((1 << 16) - 1)
#define INT32_MIN (-(1 << (32 - 1)))
#define INT32_MAX ((1 << (32 - 1)) - 1)
#define UINT32_MAX ((1 << 32) - 1)
#define INT64_MIN (-(1 << (64 - 1)))
#define INT64_MAX ((1 << (64 - 1)) - 1)
#define UINT64_MAX ((1 << 64) - 1)

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int int64_t;
typedef unsigned long int uint64_t;

/* Define least widths */
#define INT_LEAST8_MIN (-(1 << (8 - 1)))
#define INT_LEAST8_MAX ((1 << (8 - 1)) - 1)
#define UINT_LEAST8_MAX ((1 << 8) - 1)
#define INT_LEAST16_MIN (-(1 << (16 - 1)))
#define INT_LEAST16_MAX ((1 << (16 - 1)) - 1)
#define UINT_LEAST16_MAX ((1 << 16) - 1)
#define INT_LEAST32_MIN (-(1 << (32 - 1)))
#define INT_LEAST32_MAX ((1 << (32 - 1)) - 1)
#define UINT_LEAST32_MAX ((1 << 32) - 1)
#define INT_LEAST64_MIN (-(1 << (64 - 1)))
#define INT_LEAST64_MAX ((1 << (64 - 1)) - 1)
#define UINT_LEAST64_MAX ((1 << 64) - 1)

typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef signed short int int_least16_t;
typedef unsigned short int uint_least16_t;
typedef signed int int_least32_t;
typedef unsigned int uint_least32_t;
typedef signed long int int_least64_t;
typedef unsigned long int uint_least64_t;

/* Define fastest types */
#define INT_FAST8_MIN (-(1 << (8 - 1)))
#define INT_FAST8_MAX ((1 << (8 - 1)) - 1)
#define UINT_FAST8_MAX ((1 << 8) - 1)
#define INT_FAST16_MIN (-(1 << (16 - 1)))
#define INT_FAST16_MAX ((1 << (16 - 1)) - 1)
#define UINT_FAST16_MAX ((1 << 16) - 1)
#define INT_FAST32_MIN (-(1 << (32 - 1)))
#define INT_FAST32_MAX ((1 << (32 - 1)) - 1)
#define UINT_FAST32_MAX ((1 << 32) - 1)
#define INT_FAST64_MIN (-(1 << (64 - 1)))
#define INT_FAST64_MAX ((1 << (64 - 1)) - 1)
#define UINT_FAST64_MAX ((1 << 64) - 1)

typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef signed long int int_fast16_t;
typedef unsigned long int uint_fast16_t;
typedef signed long int int_fast32_t;
typedef unsigned long int uint_fast32_t;
typedef signed long int int_fast64_t;
typedef unsigned long int uint_fast64_t;

/* Define pointer types */
#define INTPTR_MIN (-(1 << (64 - 1)))
#define INTPTR_MAX ((1 << (64 - 1)) - 1)
#define UINTPTR_MAX ((1 << 64) - 1)

typedef long int intptr_t;
typedef unsigned long int uintptr_t;

/* Define max types */
#define INTMAX_MIN (-(1 << (64 - 1)))
#define INTMAX_MAX ((1 << (64 - 1)) - 1)
#define UINTMAX_MAX ((1 << 64) - 1)

typedef long int intmax_t;
typedef unsigned long int uintmax_t;


#endif