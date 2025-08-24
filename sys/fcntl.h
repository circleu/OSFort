#ifndef _SYS_FCNTL_H
#define _SYS_FCNTL_H

#include "stdint.h"

typedef struct {
    int type;
    pid_t pid;
} f_owner_ex;
typedef struct {
    int16_t l_type;
    int16_t l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
} flock;

/* From <sys/types.h> */
typedef uint16_t mode_t;
typedef int64_t off_t;
typedef int pid_t;

/* 
 * Numbered as they've been ordered in POSIX.1-2024.
 *
 * I searched some Unix-like OSes like Linux, FreeBSD, Opensolaris, etc.,
 * but they numbered the constants all different to each other.
 * So I just do like this.
 */
#define F_DUPFD 0
#define F_DUPFD_CLOEXEC 1
#define F_PUDFD_CLOFORK 2
#define F_GETFD 3
#define F_SETFD 4
#define F_GETFL 5
#define F_SETFL 6
#define F_GETLK 7
#define F_SETLK 8
#define F_SETLKW 9
#define F_OFD_GETLK 10
#define F_OFD_SETLK 11
#define F_OFD_SETLKW 12
#define F_GETOWN 13
#define F_GETOWN_EX 14
#define F_SETOWN 15
#define F_SETOWN_EX 16

#define FD_CLOEXEC 0x01
#define FD_CLOFORK 0x02

/* Following Opensolaris */
#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2

#define F_OWNER_PID 0
#define F_OWNER_PGRP 1

/* From <stdio.h> */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/*
 * Numbered as they've been ordered in POSIX.1-2024.
 *
 * Many of the OSes set the values not starting from 0b1.
 * I don't know why now, so I just do like this.
 */
#define O_CLOEXEC 0x0001
#define O_CLOFORK 0x0002
#define O_CREAT 0x0004
#define O_DIRECTORY 0x0008
#define O_EXCL 0x0010
#define O_NOCTTY 0x0020
#define O_NOFOLLOW 0x0040
#define O_TRUNC 0x0080
#define O_TTY_INIT 0x0100

/* 
 * Numbered as they've been ordered in POSIX.1-2024.
 * 
 * Same reason with oflag constants.
 */
#define O_APPEND 0
#define O_DSYNC 1
#define O_NONBLOCK 2
#define O_RSYNC 3
#define O_SYNC 4

/* Is this the same thing with S_IAMB? */
#define O_ACCMODE 0x1FF

/* Following Opensolaris 0~2 */
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_EXEC 3
#define O_SEARCH O_EXEC

/* 
 * From <sys/stat.h>
 * 
 * Following conventional values.
 */
#define S_IFMT 0xF000
#define S_IFBLK 0x6000
#define S_IFCHR 0x2000
#define S_IFIFO 0x1000
#define S_IFREG 0x8000
#define S_IFDIR 0x4000
#define S_IFLNK 0xA000
#define S_IFSOCK 0xC000

/* Following FreeBSD */
#define AT_FDCWD (-100)

/* I dont know why they were sequenctially numbered */
#define AT_EACCESS 0x01

#define AT_SYMLINK_NOFOLLOW 0x02

#define AT_SYMLINK_FOLLOW 0x04

#define AT_REMOVEDIR 0x08

/* Following conventional? values */
#define POSIX_FADV_NORMAL 0
#define POSIX_FADV_RANDOM 1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED 3
#define POSIX_FADV_DONTNEED 4
#define POSIX_FADV_NOREUSE 5


int creat(const char* path, mode_t mode);
int fcntl(int fildes, int cmd, ...);
int open(const char* path, int oflag, ...);
int openat(int fd, const char* path, int oflag, ...);
int posix_fadvise(int fd, off_t offset, off_t len, int advice);
int posix_fallocate(int fd, off_t offset, off_t len);


#endif