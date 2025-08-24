#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include "stdint.h"

typedef int64_t blkcnt_t;
typedef int32_t blksize_t;
typedef int32_t clockid_t;
typedef uint32_t dev_t;
typedef uint64_t fsblkcnt_t;
typedef uint64_t fsfilcnt_t;
typedef uint32_t gid_t;
typedef int64_t id_t;
typedef uint64_t ino_t;
typedef int32_t key_t;
typedef uint16_t mode_t;
typedef uint64_t nlink_t;
typedef int64_t off_t;
typedef int32_t pid_t;
typedef struct {uint8_t dummy[64];} pthread_attr_t;
typedef struct {uint8_t dummy[32];} pthread_barrier_t;
typedef struct {uint8_t dummy[8];} pthread_barrierattr_t;
typedef struct {uint8_t dummy[64];} pthread_cond_t;
typedef struct {uint8_t dummy[8];} pthread_condattr_t;
typedef uint32_t pthread_key_t;
typedef struct {uint8_t dummy[64];} pthread_mutex_t;
typedef struct {uint8_t dummy[8];} pthread_mutexattr_t;
typedef int32_t pthread_once_t;
typedef struct {uint8_t dummy[64];} pthread_rwlock_t;
typedef struct {uint8_t dummy[8];} pthread_rwlockattr_t;
typedef struct {uint8_t dummy[8];} pthread_spinlock_t;
typedef uint64_t pthread_t;
typedef uint64_t reclen_t;
typedef uint64_t size_t;
typedef int64_t ssize_t;
typedef int64_t suseconds_t;
typedef int64_t time_t;
typedef void* timer_t;
typedef uint32_t uid_t;

#endif