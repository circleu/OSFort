#ifndef _SYS_UNISTD_H
#define _SYS_UNISTD_H

#include "stdint.h"

#define _POSIX_VERSION 202405L
#define _POSIX2_VERSION 202405L
#define _XOPEN_VERSION 800

#define _POSIX_ADVISORY_INFO (-1)
#define _POSIX_ASYNCHRONOUS_IO 202405L
#define _POSIX_BARRIERS 202405L
#define _POSIX_CHOWN_RESTRICTED (-1)
#define _POSIX_CLOCK_SELECTION 202405L
#define _POSIX_CPUTIME (-1)
#define _POSIX_DEVICE_CONTROL (-1)
#define _POSIX_FSYNC (-1)
#define _POSIX_IPV6 (-1)
#define _POSIX_JOB_CONTROL 1
#define _POSIX_MAPPED_FILES 202405L
#define _POSIX_MEMLOCK (-1)
#define _POSIX_MEMLOCK_RANGE (-1)
#define _POSIX_MEMORY_PROTECTION 202405L
#define _POSIX_MESSAGE_PASSING (-1)
#define _POSIX_MONOTONIC_CLOCK 202405L
#define _POSIX_NO_TRUNC (-1)
#define _POSIX_PRIORITIZED_IO (-1)
#define _POSIX_PRIORITY_SCHEDULING (-1)
#define _POSIX_RAW_SOCKETS (-1)
#define _POSIX_READER_WRITER_LOCKS 202405L
#define _POSIX_REALTIME_SIGNALS 202405L
#define _POSIX_REGEXP 1
#define _POSIX_SAVED_IDS 1
#define _POSIX_SEMAPHORES 202405L
#define _POSIX_SHARED_MEMORY_OBJECTS (-1)
#define _POSIX_SHELL 1
#define _POSIX_SPAWN (-1)
#define _POSIX_SPIN_LOCKS 202405L
#define _POSIX_SPORADIC_SERVER (-1)
#define _POSIX_SYNCHRONIZED_IO (-1)
#define _POSIX_THREAD_ATTR_STACKADDR (-1)
#define _POSIX_THREAD_ATTR_STACKSIZE (-1)
#define _POSIX_THREAD_CPUTIME (-1)
#define _POSIX_THREAD_PRIO_INHERIT (-1)
#define _POSIX_THREAD_PRIO_PROTECT (-1)
#define _POSIX_THREAD_PRIORITY_SCHEDULING (-1)
#define _POSIX_THREAD_PROCESS_SHARED (-1)
#define _POSIX_THREAD_ROBUST_PRIO_INHERIT (-1)
#define _POSIX_THREAD_ROBUST_PRIO_PROTECT (-1)
#define _POSIX_THREAD_SAFE_FUNCTIONS 202405L
#define _POSIX_THREAD_SPORADIC_SERVER (-1)
#define _POSIX_THREADS 202405L
#define _POSIX_TIMEOUTS 202405L
#define _POSIX_TIMERS 202405L
#define _POSIX_TYPED_MEMORY_OBJECTS (-1)
// #define _POSIX_V7_ILP32_OFF32
// #define _POSIX_V7_ILP32_OFFBIG
// #define _POSIX_V7_LP64_OFF64
// #define _POSIX_V7_LPBIG_OFFBIG
// #define _POSIX_V8_ILP32_OFF32
// #define _POSIX_V8_ILP32_OFFBIG
// #define _POSIX_V8_LP64_OFF64
// #define _POSIX_V8_LPBIG_OFFBIG
#define _POSIX2_C_BIND 202405L
#define _POSIX2_C_DEV (-1)
// #define _POSIX2_CHAR_TERM
#define _POSIX2_FORT_RUN (-1)
#define _POSIX2_LOCALEDEF (-1)
#define _POSIX2_SW_DEF (-1)
#define _POSIX2_UPE (-1)
// #define _XOPEN_CRYPT
#define _XOPEN_ENH_I18N 0
// #define _XOPEN_REALTIME
// #define _XOPEN_REALTIME_THREADS
#define _XOPEN_SHM 0
// #define _XOPEN_UNIX
#define _XOPEN_UUCP (-1)

// #define _POSIX_ASYNC_IO
// #define _POSIX_FALLOC
// #define _POSIX_PRIO_IO
// #define _POSIX_SYNC_IO
// #define _POSIX_TIMESTAMP_RESOLUTION
// #define _POSIX2_SYMLINKS

/* From <stddef.h> */
#define NULL ((void*)0)

/* From <fcntl.h> */
#define O_CLOEXEC 0x0001
#define O_CLOFORK 0x0002

/* Following conventional? values */
#define F_OK 0x00
#define X_OK 0x01
#define W_OK 0x02
#define R_OK 0x04

/* Following FreeBSD 1~14 */
#define _CS_PATH 1
#define _CS_POSIX_V8_ILP32_OFF32_CFLAGS 2
#define _CS_POSIX_V8_ILP32_OFF32_LDFLAGS 3
#define _CS_POSIX_V8_ILP32_OFF32_LIBS 4
#define _CS_POSIX_V8_ILP32_OFFBIG_CFLAGS 5
#define _CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS 6
#define _CS_POSIX_V8_ILP32_OFFBIG_LIBS 7
#define _CS_POSIX_V8_LP64_OFF64_CFLAGS 8
#define _CS_POSIX_V8_LP64_OFF64_LDFLAGS 9
#define _CS_POSIX_V8_LP64_OFF64_LIBS 10
#define _CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS 11
#define _CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS 12
#define _CS_POSIX_V8_LPBIG_OFFBIG_LIBS 13
#define _CS_POSIX_V8_WIDTH_RESTRICTED_ENVS 14
#define _CS_POSIX_V8_THREADS_CFLAGS 15
#define _CS_POSIX_V8_THREADS_LDFLAGS 16
#define _CS_V8_ENV 17

/* From <stdio.h> */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* Following FreeBSD */
#define SEEK_DATA 3
#define SEEK_HOLE 4

/* Following Opensolaris */
#define	F_ULOCK	0
#define	F_LOCK 1
#define	F_TLOCK	2
#define	F_TEST 3

/*
 * Following Opensolaris 1~9
 * Numbered as they've been ordered in POSIX.1-2024 10~23
 */
#define	_PC_LINK_MAX 1
#define	_PC_MAX_CANON 2
#define	_PC_MAX_INPUT 3
#define	_PC_NAME_MAX 4
#define	_PC_PATH_MAX 5
#define	_PC_PIPE_BUF 6
#define	_PC_NO_TRUNC 7
#define	_PC_VDISABLE 8
#define	_PC_CHOWN_RESTRICTED 9
#define _PC_2_SYMLINKS 10
#define _PC_ALLOC_SIZE_MIN 11
#define _PC_ASYNC_IO 12
#define _PC_FALLOC 13
#define _PC_FILESIZEBITS 14
#define _PC_PRIO_IO 15
#define _PC_REC_INCR_XFER_SIZE 16
#define _PC_REC_MAX_XFER_SIZE 17
#define _PC_REC_MIN_XFER_SIZE 18
#define _PC_REC_XFER_ALIGN 19
#define _PC_SYMLINK_MAX 20
#define _PC_SYNC_IO 21
#define _PC_TEXTDOMAIN_MAX 22
#define _PC_TIMESTAMP_RESOLUTION 23

/* Numbered as they've been ordered in POSIX.1-2024 10~23 */
#define	_SC_ARG_MAX	1
#define	_SC_CHILD_MAX	2
#define	_SC_CLK_TCK	3
#define	_SC_NGROUPS_MAX 4
#define	_SC_OPEN_MAX	5
#define	_SC_JOB_CONTROL	6
#define	_SC_SAVED_IDS	7
#define	_SC_VERSION	8
#define	_SC_PASS_MAX	9
#define	_SC_LOGNAME_MAX	10
#define	_SC_PAGESIZE	11
#define	_SC_XOPEN_VERSION	12
/* 13 reserved following Opensolaris */
#define _SC_NPROCESSORS_CONF 14
#define _SC_NPROCESSORS_ONLN 15
#define	_SC_STREAM_MAX	16
#define	_SC_TZNAME_MAX	17
#define _SC_2_C_BIND 18
#define _SC_2_C_DEV 19
#define _SC_2_CHAR_TERM 20
#define _SC_2_FORT_RUN 21
#define _SC_2_LOCALEDEF 22
#define _SC_2_SW_DEV 23
#define _SC_2_UPE 24
#define _SC_2_VERSION 25
#define _SC_ADVISORY_INFO 26
#define _SC_AIO_LISTIO_MAX 27
#define _SC_AIO_MAX 28
#define _SC_AIO_PRIO_DELTA_MAX 29
#define _SC_ASYNCHRONOUS_IO 30
#define _SC_ATEXIT_MAX 31
#define _SC_BARRIERS 32
#define _SC_BC_BASE_MAX 33
#define _SC_BC_DIM_MAX 34
#define _SC_BC_SCALE_MAX 35
#define _SC_BC_STRING_MAX 36
#define _SC_CLOCK_SELECTION 37
#define _SC_COLL_WEIGHTS_MAX 38
#define _SC_CPUTIME 39
#define _SC_DELAYTIMER_MAX 40
#define _SC_DEVICE_CONTROL 41
#define _SC_EXPR_NEST_MAX 42
#define _SC_FSYNC 43
#define _SC_GETGR_R_SIZE_MAX 44
#define _SC_GETPW_R_SIZE_MAX 45
#define _SC_HOST_NAME_MAX 46
#define _SC_IOV_MAX 47
#define _SC_IPV6 48
#define _SC_LINE_MAX 49
#define _SC_LOGIN_NAME_MAX 50
#define _SC_MAPPED_FILES 51
#define _SC_MEMLOCK 52
#define _SC_MEMLOCK_RANGE 53
#define _SC_MEMORY_PROTECTION 54
#define _SC_MESSAGE_PASSING 55
#define _SC_MONOTONIC_CLOCK 56
#define _SC_MQ_OPEN_MAX 57
#define _SC_MQ_PRIO_MAX 58
#define _SC_NSIG 59
#define _SC_PAGE_SIZE _SC_PAGESIZE
#define _SC_PRIORITIZED_IO 61
#define _SC_PRIORITY_SCHEDULING 62
#define _SC_RAW_SOCKETS 63
#define _SC_RE_DUP_MAX 64
#define _SC_READER_WRITER_LOCKS 65
#define _SC_REALTIME_SIGNALS 66
#define _SC_REGEXP 67
#define _SC_RTSIG_MAX 68
#define _SC_SEM_NSEMS_MAX 69
#define _SC_SEM_VALUE_MAX 70
#define _SC_SEMAPHORES 71
#define _SC_SHARED_MEMORY_OBJECTS 72
#define _SC_SHELL 73
#define _SC_SIGQUEUE_MAX 74
#define _SC_SPAWN 75
#define _SC_SPIN_LOCKS 76
#define _SC_SPORADIC_SERVER 77
#define _SC_SS_REPL_MAX 78
#define _SC_SYMLOOP_MAX 79
#define _SC_SYNCHRONIZED_IO 80
#define _SC_THREAD_ATTR_STACKADDR 81
#define _SC_THREAD_ATTR_STACKSIZE 82
#define _SC_THREAD_CPUTIME 83
#define _SC_THREAD_DESTRUCTOR_ITERATIONS 84
#define _SC_THREAD_KEYS_MAX 85
#define _SC_THREAD_PRIO_INHERIT 86
#define _SC_THREAD_PRIO_PROTECT 87
#define _SC_THREAD_PRIORITY_SCHEDULING 88
#define _SC_THREAD_PROCESS_SHARED 89
#define _SC_THREAD_ROBUST_PRIO_INHERIT 90
#define _SC_THREAD_ROBUST_PRIO_PROTECT 91
#define _SC_THREAD_SAFE_FUNCTIONS 92
#define _SC_THREAD_SPORADIC_SERVER 93
#define _SC_THREAD_STACK_MIN 94
#define _SC_THREAD_THREADS_MAX 95
#define _SC_THREADS 96
#define _SC_TIMEOUTS 97
#define _SC_TIMER_MAX 98
#define _SC_TIMERS 99
#define _SC_TTY_NAME_MAX 100
#define _SC_TYPED_MEMORY_OBJECTS 101
#define _SC_V8_ILP32_OFF32 102
#define _SC_V8_ILP32_OFFBIG 103
#define _SC_V8_LP64_OFF64 104
#define _SC_V8_LPBIG_OFFBIG 105
#define _SC_V7_ILP32_OFF32 106
#define _SC_V7_ILP32_OFFBIG 107
#define _SC_V7_LP64_OFF64 108
#define _SC_V7_LPBIG_OFFBIG 109
#define _SC_XOPEN_CRYPT 200
#define _SC_XOPEN_ENH_I18N 201
#define _SC_XOPEN_REALTIME 202
#define _SC_XOPEN_REALTIME_THREADS 203
#define _SC_XOPEN_SHM 204
#define _SC_XOPEN_UNIX 205
#define _SC_XOPEN_UUCP 206

#define	STDIN_FILENO 0
#define	STDOUT_FILENO 1
#define	STDERR_FILENO 2

#define	_POSIX_VDISABLE 0

#define POSIX_CLOSE_RESTART 1

/* From <sys/types.h> */
typedef uint64_t size_t;
typedef int64_t ssize_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef int64_t off_t;
typedef int32_t pid_t;
/* From <stdint.h> */
// typedef long int intptr_t;


int access(const char* path, int amode);
unsigned int alarm(unsigned int seconds);
int chdir(const char* path);
int chown(const char* path, uid_t owner, gid_t group);
int close(int fildes);
size_t confstr(int name, char* buf, size_t len);
char* crypt(const char* key, const char* salt);
int dup(int fildes);
int dup2(int fildes, int fildes2);
int dup3(int fildes, int fildes2, int flag);
_Noreturn void _exit(int status);
void encrypt(char block[64], int edflag);
int execl(const char* path, const char* arg0, ... /*, (char*)0 */);
int execle(const char* path, const char* arg0, ... /*, (char*)0, char* const envp[] */);
int execlp(const char* file, const char* arg0, ... /*, (char*)0 */);
int execv(const char* path, char* const argv[]);
int execve(const char* path, char* const argv[], char* const envp[]);
int execvp(const char* file, char* const argv[]);
int faccessat(int fd, const char* path, int amode, int flag);
int fchdir(int fildes);
int fchown(int fildes, uid_t owner, gid_t group);
int fchownat(int fd, const char* path, uid_t owner, gid_t group, int flag);
int fdatasync(int fildes);
int fexecve(int fd, char* const argv[], char* const envp[]);
pid_t _Fork(void);
pid_t fork(void);
long int fpathconf(int fildes, int name);
int fsync(int fildes);
int ftruncate(int fildes, off_t length);
char* getcwd(char* buf, size_t size);
gid_t getegid(void);
int getentropy(void* buffer, size_t length);
uid_t geteuid(void);
gid_t getgid(void);
int getgroups(int gidsetsize, gid_t grouplist[]);
long int gethostid(void);
int gethostname(char* name, size_t namelen);
char* getlogin(void);
int getlogin_r(char* name, size_t namesize);
int getopt(int argc, char* const argv[], const char* optstring);
pid_t getpgid(pid_t pid);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
int getresgid(gid_t* restrict rgid, gid_t* restrict egid, gid_t* restrict sgid);
int getresuid(uid_t* restrict ruid, uid_t* restrict euid, uid_t* restrict suid);
pid_t getsid(pid_t pid);
uid_t getuid(void);
int isatty(int fildes);
int lchown(const char* path, uid_t owner, gid_t group);
int link(const char* path1, const char* path2);
int linkat(int fd1, const char* path1, int fd2, const char* path2, int flag);
int lockf(int fildes, int function, off_t size);
off_t lseek(int fildes, off_t offset, int whence);
int nice(int incr);
long int pathconf(const char* path, int name);
int pause(void);
int pipe(int fildes[2]);
int pipe2(int fildes[2], int flag);
int posix_close(int fildes, int flag);
ssize_t pread(int fildes, void* buf, size_t nbyte, off_t offset);
ssize_t pwrite(int fildes, const void* buf, size_t nbyte, off_t offset);
ssize_t read(int fildes, void* buf, size_t nbyte);
ssize_t readlink(const char* restrict path, char* restrict buf, size_t bufsize);
ssize_t readlinkat(int fd, const char* restrict path, char* restrict buf, size_t bufsize);
int rmdir(const char* path);
int setegid(gid_t gid);
int seteuid(uid_t uid);
int setgid(gid_t gid);
int setpgid(pid_t pid, pid_t pgid);
int setregid(gid_t rgid, gid_t egid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);
int setresuid(uid_t ruid, uid_t euid, uid_t suid);
int setreuid(uid_t ruid, uid_t euid);
pid_t setsid(void);
int setuid(uid_t uid);
unsigned int sleep(unsigned int seconds);
void swab(const void* restrict src, void* restrict dest, ssize_t nbytes);
int symlink(const char* path1, const char* path2);
int symlinkat(const char* path1, int fd, const char* path2);
void sync(void);
long int sysconf(int name);
pid_t tcgetpgrp(int fildes);
int tcsetpgrp(int fildes, pid_t pgid_id);
int truncate(const char* path, off_t length);
char* ttyname(int fildes);
int ttyname_r(int fildes, char* name, size_t namesize);
int unlink(const char* path);
int unlinkat(int fd, const char* path, int flag);
ssize_t write(int fildes, const void* buf, size_t nbyte);

extern char *optarg;
extern int opterr, optind, optopt;


#endif