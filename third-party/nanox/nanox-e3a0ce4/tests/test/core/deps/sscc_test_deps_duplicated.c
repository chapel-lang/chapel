/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

/*
<testinfo>
test_generator=gens/api-generator
</testinfo>
*/


const char *__nanos_family __attribute__((weak))  = "master";
int __nanos_version __attribute__((weak))  = 5002;
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;
typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct 
{
        int __val[2];
} __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef int __daddr_t;
typedef long int __swblk_t;
typedef int __key_t;
typedef int __clockid_t;
typedef void *__timer_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;
typedef long int __ssize_t;
typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;
typedef long int __intptr_t;
typedef unsigned int __socklen_t;
typedef __ssize_t ssize_t;
typedef long unsigned int size_t;
typedef __gid_t gid_t;
typedef __uid_t uid_t;
typedef __off_t off_t;
typedef __useconds_t useconds_t;
typedef __pid_t pid_t;
typedef __intptr_t intptr_t;
typedef __socklen_t socklen_t;
extern int access(__const char *__name, int __type) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int faccessat(int __fd, __const char *__file, int __type, int __flag) __attribute__((__nothrow__)) __attribute__((__nonnull__(2)));
extern __off_t lseek(int __fd, __off_t __offset, int __whence) __attribute__((__nothrow__));
extern int close(int __fd);
extern ssize_t read(int __fd, void *__buf, size_t __nbytes);
extern ssize_t write(int __fd, __const void *__buf, size_t __n);
extern int pipe(int __pipedes[2]) __attribute__((__nothrow__));
extern unsigned int alarm(unsigned int __seconds) __attribute__((__nothrow__));
extern unsigned int sleep(unsigned int __seconds);
extern __useconds_t ualarm(__useconds_t __value, __useconds_t __interval) __attribute__((__nothrow__));
extern int usleep(__useconds_t __useconds);
extern int pause(void);
extern int chown(__const char *__file, __uid_t __owner, __gid_t __group) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int fchown(int __fd, __uid_t __owner, __gid_t __group) __attribute__((__nothrow__));
extern int lchown(__const char *__file, __uid_t __owner, __gid_t __group) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int fchownat(int __fd, __const char *__file, __uid_t __owner, __gid_t __group, int __flag) __attribute__((__nothrow__)) __attribute__((__nonnull__(2)));
extern int chdir(__const char *__path) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int fchdir(int __fd) __attribute__((__nothrow__));
extern char *getcwd(char *__buf, size_t __size) __attribute__((__nothrow__));
extern char *getwd(char *__buf) __attribute__((__nothrow__)) __attribute__((__nonnull__(1))) __attribute__((__deprecated__));
extern int dup(int __fd) __attribute__((__nothrow__));
extern int dup2(int __fd, int __fd2) __attribute__((__nothrow__));
extern char **__environ;
extern int execve(__const char *__path, char *__argv[], char *__envp[]) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int fexecve(int __fd, char *__argv[], char *__envp[]) __attribute__((__nothrow__));
extern int execv(__const char *__path, char *__argv[]) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int execle(__const char *__path, __const char *__arg, ...) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int execl(__const char *__path, __const char *__arg, ...) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int execvp(__const char *__file, char *__argv[]) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int execlp(__const char *__file, __const char *__arg, ...) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int nice(int __inc) __attribute__((__nothrow__));
extern void _exit(int __status) __attribute__((__noreturn__));
enum 
{
    _PC_LINK_MAX, 
    _PC_MAX_CANON, 
    _PC_MAX_INPUT, 
    _PC_NAME_MAX, 
    _PC_PATH_MAX, 
    _PC_PIPE_BUF, 
    _PC_CHOWN_RESTRICTED, 
    _PC_NO_TRUNC, 
    _PC_VDISABLE, 
    _PC_SYNC_IO, 
    _PC_ASYNC_IO, 
    _PC_PRIO_IO, 
    _PC_SOCK_MAXBUF, 
    _PC_FILESIZEBITS, 
    _PC_REC_INCR_XFER_SIZE, 
    _PC_REC_MAX_XFER_SIZE, 
    _PC_REC_MIN_XFER_SIZE, 
    _PC_REC_XFER_ALIGN, 
    _PC_ALLOC_SIZE_MIN, 
    _PC_SYMLINK_MAX, 
    _PC_2_SYMLINKS
};
enum 
{
    _SC_ARG_MAX, 
    _SC_CHILD_MAX, 
    _SC_CLK_TCK, 
    _SC_NGROUPS_MAX, 
    _SC_OPEN_MAX, 
    _SC_STREAM_MAX, 
    _SC_TZNAME_MAX, 
    _SC_JOB_CONTROL, 
    _SC_SAVED_IDS, 
    _SC_REALTIME_SIGNALS, 
    _SC_PRIORITY_SCHEDULING, 
    _SC_TIMERS, 
    _SC_ASYNCHRONOUS_IO, 
    _SC_PRIORITIZED_IO, 
    _SC_SYNCHRONIZED_IO, 
    _SC_FSYNC, 
    _SC_MAPPED_FILES, 
    _SC_MEMLOCK, 
    _SC_MEMLOCK_RANGE, 
    _SC_MEMORY_PROTECTION, 
    _SC_MESSAGE_PASSING, 
    _SC_SEMAPHORES, 
    _SC_SHARED_MEMORY_OBJECTS, 
    _SC_AIO_LISTIO_MAX, 
    _SC_AIO_MAX, 
    _SC_AIO_PRIO_DELTA_MAX, 
    _SC_DELAYTIMER_MAX, 
    _SC_MQ_OPEN_MAX, 
    _SC_MQ_PRIO_MAX, 
    _SC_VERSION, 
    _SC_PAGESIZE, 
    _SC_RTSIG_MAX, 
    _SC_SEM_NSEMS_MAX, 
    _SC_SEM_VALUE_MAX, 
    _SC_SIGQUEUE_MAX, 
    _SC_TIMER_MAX, 
    _SC_BC_BASE_MAX, 
    _SC_BC_DIM_MAX, 
    _SC_BC_SCALE_MAX, 
    _SC_BC_STRING_MAX, 
    _SC_COLL_WEIGHTS_MAX, 
    _SC_EQUIV_CLASS_MAX, 
    _SC_EXPR_NEST_MAX, 
    _SC_LINE_MAX, 
    _SC_RE_DUP_MAX, 
    _SC_CHARCLASS_NAME_MAX, 
    _SC_2_VERSION, 
    _SC_2_C_BIND, 
    _SC_2_C_DEV, 
    _SC_2_FORT_DEV, 
    _SC_2_FORT_RUN, 
    _SC_2_SW_DEV, 
    _SC_2_LOCALEDEF, 
    _SC_PII, 
    _SC_PII_XTI, 
    _SC_PII_SOCKET, 
    _SC_PII_INTERNET, 
    _SC_PII_OSI, 
    _SC_POLL, 
    _SC_SELECT, 
    _SC_UIO_MAXIOV, 
    _SC_IOV_MAX = _SC_UIO_MAXIOV, 
    _SC_PII_INTERNET_STREAM, 
    _SC_PII_INTERNET_DGRAM, 
    _SC_PII_OSI_COTS, 
    _SC_PII_OSI_CLTS, 
    _SC_PII_OSI_M, 
    _SC_T_IOV_MAX, 
    _SC_THREADS, 
    _SC_THREAD_SAFE_FUNCTIONS, 
    _SC_GETGR_R_SIZE_MAX, 
    _SC_GETPW_R_SIZE_MAX, 
    _SC_LOGIN_NAME_MAX, 
    _SC_TTY_NAME_MAX, 
    _SC_THREAD_DESTRUCTOR_ITERATIONS, 
    _SC_THREAD_KEYS_MAX, 
    _SC_THREAD_STACK_MIN, 
    _SC_THREAD_THREADS_MAX, 
    _SC_THREAD_ATTR_STACKADDR, 
    _SC_THREAD_ATTR_STACKSIZE, 
    _SC_THREAD_PRIORITY_SCHEDULING, 
    _SC_THREAD_PRIO_INHERIT, 
    _SC_THREAD_PRIO_PROTECT, 
    _SC_THREAD_PROCESS_SHARED, 
    _SC_NPROCESSORS_CONF, 
    _SC_NPROCESSORS_ONLN, 
    _SC_PHYS_PAGES, 
    _SC_AVPHYS_PAGES, 
    _SC_ATEXIT_MAX, 
    _SC_PASS_MAX, 
    _SC_XOPEN_VERSION, 
    _SC_XOPEN_XCU_VERSION, 
    _SC_XOPEN_UNIX, 
    _SC_XOPEN_CRYPT, 
    _SC_XOPEN_ENH_I18N, 
    _SC_XOPEN_SHM, 
    _SC_2_CHAR_TERM, 
    _SC_2_C_VERSION, 
    _SC_2_UPE, 
    _SC_XOPEN_XPG2, 
    _SC_XOPEN_XPG3, 
    _SC_XOPEN_XPG4, 
    _SC_CHAR_BIT, 
    _SC_CHAR_MAX, 
    _SC_CHAR_MIN, 
    _SC_INT_MAX, 
    _SC_INT_MIN, 
    _SC_LONG_BIT, 
    _SC_WORD_BIT, 
    _SC_MB_LEN_MAX, 
    _SC_NZERO, 
    _SC_SSIZE_MAX, 
    _SC_SCHAR_MAX, 
    _SC_SCHAR_MIN, 
    _SC_SHRT_MAX, 
    _SC_SHRT_MIN, 
    _SC_UCHAR_MAX, 
    _SC_UINT_MAX, 
    _SC_ULONG_MAX, 
    _SC_USHRT_MAX, 
    _SC_NL_ARGMAX, 
    _SC_NL_LANGMAX, 
    _SC_NL_MSGMAX, 
    _SC_NL_NMAX, 
    _SC_NL_SETMAX, 
    _SC_NL_TEXTMAX, 
    _SC_XBS5_ILP32_OFF32, 
    _SC_XBS5_ILP32_OFFBIG, 
    _SC_XBS5_LP64_OFF64, 
    _SC_XBS5_LPBIG_OFFBIG, 
    _SC_XOPEN_LEGACY, 
    _SC_XOPEN_REALTIME, 
    _SC_XOPEN_REALTIME_THREADS, 
    _SC_ADVISORY_INFO, 
    _SC_BARRIERS, 
    _SC_BASE, 
    _SC_C_LANG_SUPPORT, 
    _SC_C_LANG_SUPPORT_R, 
    _SC_CLOCK_SELECTION, 
    _SC_CPUTIME, 
    _SC_THREAD_CPUTIME, 
    _SC_DEVICE_IO, 
    _SC_DEVICE_SPECIFIC, 
    _SC_DEVICE_SPECIFIC_R, 
    _SC_FD_MGMT, 
    _SC_FIFO, 
    _SC_PIPE, 
    _SC_FILE_ATTRIBUTES, 
    _SC_FILE_LOCKING, 
    _SC_FILE_SYSTEM, 
    _SC_MONOTONIC_CLOCK, 
    _SC_MULTI_PROCESS, 
    _SC_SINGLE_PROCESS, 
    _SC_NETWORKING, 
    _SC_READER_WRITER_LOCKS, 
    _SC_SPIN_LOCKS, 
    _SC_REGEXP, 
    _SC_REGEX_VERSION, 
    _SC_SHELL, 
    _SC_SIGNALS, 
    _SC_SPAWN, 
    _SC_SPORADIC_SERVER, 
    _SC_THREAD_SPORADIC_SERVER, 
    _SC_SYSTEM_DATABASE, 
    _SC_SYSTEM_DATABASE_R, 
    _SC_TIMEOUTS, 
    _SC_TYPED_MEMORY_OBJECTS, 
    _SC_USER_GROUPS, 
    _SC_USER_GROUPS_R, 
    _SC_2_PBS, 
    _SC_2_PBS_ACCOUNTING, 
    _SC_2_PBS_LOCATE, 
    _SC_2_PBS_MESSAGE, 
    _SC_2_PBS_TRACK, 
    _SC_SYMLOOP_MAX, 
    _SC_STREAMS, 
    _SC_2_PBS_CHECKPOINT, 
    _SC_V6_ILP32_OFF32, 
    _SC_V6_ILP32_OFFBIG, 
    _SC_V6_LP64_OFF64, 
    _SC_V6_LPBIG_OFFBIG, 
    _SC_HOST_NAME_MAX, 
    _SC_TRACE, 
    _SC_TRACE_EVENT_FILTER, 
    _SC_TRACE_INHERIT, 
    _SC_TRACE_LOG, 
    _SC_LEVEL1_ICACHE_SIZE, 
    _SC_LEVEL1_ICACHE_ASSOC, 
    _SC_LEVEL1_ICACHE_LINESIZE, 
    _SC_LEVEL1_DCACHE_SIZE, 
    _SC_LEVEL1_DCACHE_ASSOC, 
    _SC_LEVEL1_DCACHE_LINESIZE, 
    _SC_LEVEL2_CACHE_SIZE, 
    _SC_LEVEL2_CACHE_ASSOC, 
    _SC_LEVEL2_CACHE_LINESIZE, 
    _SC_LEVEL3_CACHE_SIZE, 
    _SC_LEVEL3_CACHE_ASSOC, 
    _SC_LEVEL3_CACHE_LINESIZE, 
    _SC_LEVEL4_CACHE_SIZE, 
    _SC_LEVEL4_CACHE_ASSOC, 
    _SC_LEVEL4_CACHE_LINESIZE, 
    _SC_IPV6 = _SC_LEVEL1_ICACHE_SIZE + 50, 
    _SC_RAW_SOCKETS, 
    _SC_V7_ILP32_OFF32, 
    _SC_V7_ILP32_OFFBIG, 
    _SC_V7_LP64_OFF64, 
    _SC_V7_LPBIG_OFFBIG, 
    _SC_SS_REPL_MAX, 
    _SC_TRACE_EVENT_NAME_MAX, 
    _SC_TRACE_NAME_MAX, 
    _SC_TRACE_SYS_MAX, 
    _SC_TRACE_USER_EVENT_MAX, 
    _SC_XOPEN_STREAMS, 
    _SC_THREAD_ROBUST_PRIO_INHERIT, 
    _SC_THREAD_ROBUST_PRIO_PROTECT
};
enum 
{
    _CS_PATH, 
    _CS_V6_WIDTH_RESTRICTED_ENVS, 
    _CS_GNU_LIBC_VERSION, 
    _CS_GNU_LIBPTHREAD_VERSION, 
    _CS_V5_WIDTH_RESTRICTED_ENVS, 
    _CS_V7_WIDTH_RESTRICTED_ENVS, 
    _CS_LFS_CFLAGS = 1000, 
    _CS_LFS_LDFLAGS, 
    _CS_LFS_LIBS, 
    _CS_LFS_LINTFLAGS, 
    _CS_LFS64_CFLAGS, 
    _CS_LFS64_LDFLAGS, 
    _CS_LFS64_LIBS, 
    _CS_LFS64_LINTFLAGS, 
    _CS_XBS5_ILP32_OFF32_CFLAGS = 1100, 
    _CS_XBS5_ILP32_OFF32_LDFLAGS, 
    _CS_XBS5_ILP32_OFF32_LIBS, 
    _CS_XBS5_ILP32_OFF32_LINTFLAGS, 
    _CS_XBS5_ILP32_OFFBIG_CFLAGS, 
    _CS_XBS5_ILP32_OFFBIG_LDFLAGS, 
    _CS_XBS5_ILP32_OFFBIG_LIBS, 
    _CS_XBS5_ILP32_OFFBIG_LINTFLAGS, 
    _CS_XBS5_LP64_OFF64_CFLAGS, 
    _CS_XBS5_LP64_OFF64_LDFLAGS, 
    _CS_XBS5_LP64_OFF64_LIBS, 
    _CS_XBS5_LP64_OFF64_LINTFLAGS, 
    _CS_XBS5_LPBIG_OFFBIG_CFLAGS, 
    _CS_XBS5_LPBIG_OFFBIG_LDFLAGS, 
    _CS_XBS5_LPBIG_OFFBIG_LIBS, 
    _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS, 
    _CS_POSIX_V6_ILP32_OFF32_CFLAGS, 
    _CS_POSIX_V6_ILP32_OFF32_LDFLAGS, 
    _CS_POSIX_V6_ILP32_OFF32_LIBS, 
    _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS, 
    _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS, 
    _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS, 
    _CS_POSIX_V6_ILP32_OFFBIG_LIBS, 
    _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS, 
    _CS_POSIX_V6_LP64_OFF64_CFLAGS, 
    _CS_POSIX_V6_LP64_OFF64_LDFLAGS, 
    _CS_POSIX_V6_LP64_OFF64_LIBS, 
    _CS_POSIX_V6_LP64_OFF64_LINTFLAGS, 
    _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS, 
    _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS, 
    _CS_POSIX_V6_LPBIG_OFFBIG_LIBS, 
    _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS, 
    _CS_POSIX_V7_ILP32_OFF32_CFLAGS, 
    _CS_POSIX_V7_ILP32_OFF32_LDFLAGS, 
    _CS_POSIX_V7_ILP32_OFF32_LIBS, 
    _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS, 
    _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS, 
    _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS, 
    _CS_POSIX_V7_ILP32_OFFBIG_LIBS, 
    _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS, 
    _CS_POSIX_V7_LP64_OFF64_CFLAGS, 
    _CS_POSIX_V7_LP64_OFF64_LDFLAGS, 
    _CS_POSIX_V7_LP64_OFF64_LIBS, 
    _CS_POSIX_V7_LP64_OFF64_LINTFLAGS, 
    _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS, 
    _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS, 
    _CS_POSIX_V7_LPBIG_OFFBIG_LIBS, 
    _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS
};
extern long int pathconf(__const char *__path, int __name) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern long int fpathconf(int __fd, int __name) __attribute__((__nothrow__));
extern long int sysconf(int __name) __attribute__((__nothrow__));
extern size_t confstr(int __name, char *__buf, size_t __len) __attribute__((__nothrow__));
extern __pid_t getpid(void) __attribute__((__nothrow__));
extern __pid_t getppid(void) __attribute__((__nothrow__));
extern __pid_t getpgrp(void) __attribute__((__nothrow__));
extern __pid_t __getpgid(__pid_t __pid) __attribute__((__nothrow__));
extern int setpgid(__pid_t __pid, __pid_t __pgid) __attribute__((__nothrow__));
extern int setpgrp(void) __attribute__((__nothrow__));
extern __pid_t setsid(void) __attribute__((__nothrow__));
extern __uid_t getuid(void) __attribute__((__nothrow__));
extern __uid_t geteuid(void) __attribute__((__nothrow__));
extern __gid_t getgid(void) __attribute__((__nothrow__));
extern __gid_t getegid(void) __attribute__((__nothrow__));
extern int getgroups(int __size, __gid_t __list[]) __attribute__((__nothrow__));
extern int setuid(__uid_t __uid) __attribute__((__nothrow__));
extern int setreuid(__uid_t __ruid, __uid_t __euid) __attribute__((__nothrow__));
extern int seteuid(__uid_t __uid) __attribute__((__nothrow__));
extern int setgid(__gid_t __gid) __attribute__((__nothrow__));
extern int setregid(__gid_t __rgid, __gid_t __egid) __attribute__((__nothrow__));
extern int setegid(__gid_t __gid) __attribute__((__nothrow__));
extern __pid_t fork(void) __attribute__((__nothrow__));
extern __pid_t vfork(void) __attribute__((__nothrow__));
extern char *ttyname(int __fd) __attribute__((__nothrow__));
extern int ttyname_r(int __fd, char *__buf, size_t __buflen) __attribute__((__nothrow__)) __attribute__((__nonnull__(2)));
extern int isatty(int __fd) __attribute__((__nothrow__));
extern int ttyslot(void) __attribute__((__nothrow__));
extern int link(__const char *__from, __const char *__to) __attribute__((__nothrow__)) __attribute__((__nonnull__(1, 2)));
extern int linkat(int __fromfd, __const char *__from, int __tofd, __const char *__to, int __flags) __attribute__((__nothrow__)) __attribute__((__nonnull__(2, 4)));
extern int symlink(__const char *__from, __const char *__to) __attribute__((__nothrow__)) __attribute__((__nonnull__(1, 2)));
extern ssize_t readlink(__const char *__path, char *__buf, size_t __len) __attribute__((__nothrow__)) __attribute__((__nonnull__(1, 2)));
extern int symlinkat(__const char *__from, int __tofd, __const char *__to) __attribute__((__nothrow__)) __attribute__((__nonnull__(1, 3)));
extern ssize_t readlinkat(int __fd, __const char *__path, char *__buf, size_t __len) __attribute__((__nothrow__)) __attribute__((__nonnull__(2, 3)));
extern int unlink(__const char *__name) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int unlinkat(int __fd, __const char *__name, int __flag) __attribute__((__nothrow__)) __attribute__((__nonnull__(2)));
extern int rmdir(__const char *__path) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern __pid_t tcgetpgrp(int __fd) __attribute__((__nothrow__));
extern int tcsetpgrp(int __fd, __pid_t __pgrp_id) __attribute__((__nothrow__));
extern char *getlogin(void);
extern int getlogin_r(char *__name, size_t __name_len) __attribute__((__nonnull__(1)));
extern int setlogin(__const char *__name) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;
extern int getopt(int ___argc, char **___argv, const char *__shortopts) __attribute__((__nothrow__));
extern int gethostname(char *__name, size_t __len) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int sethostname(__const char *__name, size_t __len) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int sethostid(long int __id) __attribute__((__nothrow__));
extern int getdomainname(char *__name, size_t __len) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int setdomainname(__const char *__name, size_t __len) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int vhangup(void) __attribute__((__nothrow__));
extern int revoke(__const char *__file) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int profil(unsigned short int *__sample_buffer, size_t __size, size_t __offset, unsigned int __scale) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int acct(__const char *__name) __attribute__((__nothrow__));
extern char *getusershell(void) __attribute__((__nothrow__));
extern void endusershell(void) __attribute__((__nothrow__));
extern void setusershell(void) __attribute__((__nothrow__));
extern int daemon(int __nochdir, int __noclose) __attribute__((__nothrow__));
extern int chroot(__const char *__path) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern char *getpass(__const char *__prompt) __attribute__((__nonnull__(1)));
extern int fsync(int __fd);
extern long int gethostid(void);
extern void sync(void) __attribute__((__nothrow__));
extern int getpagesize(void) __attribute__((__nothrow__)) __attribute__((__const__));
extern int getdtablesize(void) __attribute__((__nothrow__));
extern int truncate(__const char *__file, __off_t __length) __attribute__((__nothrow__)) __attribute__((__nonnull__(1)));
extern int ftruncate(int __fd, __off_t __length) __attribute__((__nothrow__));
extern int brk(void *__addr) __attribute__((__nothrow__));
extern void *sbrk(intptr_t __delta) __attribute__((__nothrow__));
extern long int syscall(long int __sysno, ...) __attribute__((__nothrow__));
extern int lockf(int __fd, int __cmd, __off_t __len);
extern int fdatasync(int __fildes);
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef struct _IO_FILE __FILE;
typedef struct 
{
        int __count;
        union 
        {
                unsigned int __wch;
                char __wchb[4];
        } __value;
} __mbstate_t;
typedef struct 
{
        __off_t __pos;
        __mbstate_t __state;
} _G_fpos_t;
typedef struct 
{
        __off64_t __pos;
        __mbstate_t __state;
} _G_fpos64_t;
typedef int _G_int16_t __attribute__((__mode__(__HI__)));
typedef int _G_int32_t __attribute__((__mode__(__SI__)));
typedef unsigned int _G_uint16_t __attribute__((__mode__(__HI__)));
typedef unsigned int _G_uint32_t __attribute__((__mode__(__SI__)));
typedef __builtin_va_list __gnuc_va_list;
struct _IO_jump_t;
struct _IO_FILE;
typedef void _IO_lock_t;
struct _IO_marker
{
        struct _IO_marker *_next;
        struct _IO_FILE *_sbuf;
        int _pos;
};
enum __codecvt_result
{
    __codecvt_ok, 
    __codecvt_partial, 
    __codecvt_error, 
    __codecvt_noconv
};
struct _IO_FILE
{
        int _flags;
        char *_IO_read_ptr;
        char *_IO_read_end;
        char *_IO_read_base;
        char *_IO_write_base;
        char *_IO_write_ptr;
        char *_IO_write_end;
        char *_IO_buf_base;
        char *_IO_buf_end;
        char *_IO_save_base;
        char *_IO_backup_base;
        char *_IO_save_end;
        struct _IO_marker *_markers;
        struct _IO_FILE *_chain;
        int _fileno;
        int _flags2;
        __off_t _old_offset;
        unsigned short _cur_column;
        signed char _vtable_offset;
        char _shortbuf[1];
        _IO_lock_t *_lock;
        __off64_t _offset;
        void *__pad1;
        void *__pad2;
        void *__pad3;
        void *__pad4;
        size_t __pad5;
        int _mode;
        char _unused2[15 * sizeof(int) - 4 * sizeof(void *) - sizeof(size_t)];
};
typedef struct _IO_FILE _IO_FILE;
struct _IO_FILE_plus;
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
typedef __ssize_t __io_read_fn(void *__cookie, char *__buf, size_t __nbytes);
typedef __ssize_t __io_write_fn(void *__cookie, __const char *__buf, size_t __n);
typedef int __io_seek_fn(void *__cookie, __off64_t *__pos, int __w);
typedef int __io_close_fn(void *__cookie);
extern int __underflow(_IO_FILE *);
extern int __uflow(_IO_FILE *);
extern int __overflow(_IO_FILE *, int);
extern int _IO_getc(_IO_FILE *__fp);
extern int _IO_putc(int __c, _IO_FILE *__fp);
extern int _IO_feof(_IO_FILE *__fp) __attribute__((__nothrow__));
extern int _IO_ferror(_IO_FILE *__fp) __attribute__((__nothrow__));
extern int _IO_peekc_locked(_IO_FILE *__fp);
extern void _IO_flockfile(_IO_FILE *) __attribute__((__nothrow__));
extern void _IO_funlockfile(_IO_FILE *) __attribute__((__nothrow__));
extern int _IO_ftrylockfile(_IO_FILE *) __attribute__((__nothrow__));
extern int _IO_vfscanf(_IO_FILE *, const char *, __gnuc_va_list, int *);
extern int _IO_vfprintf(_IO_FILE *, const char *, __gnuc_va_list);
extern __ssize_t _IO_padn(_IO_FILE *, int, __ssize_t);
extern size_t _IO_sgetn(_IO_FILE *, void *, size_t);
extern __off64_t _IO_seekoff(_IO_FILE *, __off64_t, int, int);
extern __off64_t _IO_seekpos(_IO_FILE *, __off64_t, int);
extern void _IO_free_backup_area(_IO_FILE *) __attribute__((__nothrow__));
typedef _G_fpos_t fpos_t;
extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;
extern int remove(__const char *__filename) __attribute__((__nothrow__));
extern int rename(__const char *__old, __const char *__new) __attribute__((__nothrow__));
extern int renameat(int __oldfd, __const char *__old, int __newfd, __const char *__new) __attribute__((__nothrow__));
extern FILE *tmpfile(void);
extern char *tmpnam(char *__s) __attribute__((__nothrow__));
extern char *tmpnam_r(char *__s) __attribute__((__nothrow__));
extern char *tempnam(__const char *__dir, __const char *__pfx) __attribute__((__nothrow__)) __attribute__((__malloc__));
extern int fclose(FILE *__stream);
extern int fflush(FILE *__stream);
extern int fflush_unlocked(FILE *__stream);
extern FILE *fopen(__const char *__filename, __const char *__modes);
extern FILE *freopen(__const char *__filename, __const char *__modes, FILE *__stream);
extern FILE *fdopen(int __fd, __const char *__modes) __attribute__((__nothrow__));
extern FILE *fmemopen(void *__s, size_t __len, __const char *__modes) __attribute__((__nothrow__));
extern FILE *open_memstream(char **__bufloc, size_t *__sizeloc) __attribute__((__nothrow__));
extern void setbuf(FILE *__stream, char *__buf) __attribute__((__nothrow__));
extern int setvbuf(FILE *__stream, char *__buf, int __modes, size_t __n) __attribute__((__nothrow__));
extern void setbuffer(FILE *__stream, char *__buf, size_t __size) __attribute__((__nothrow__));
extern void setlinebuf(FILE *__stream) __attribute__((__nothrow__));
extern int fprintf(FILE *__stream, __const char *__format, ...);
extern int printf(__const char *__format, ...);
extern int sprintf(char *__s, __const char *__format, ...) __attribute__((__nothrow__));
extern int vfprintf(FILE *__s, __const char *__format, __gnuc_va_list __arg);
extern int vprintf(__const char *__format, __gnuc_va_list __arg);
extern int vsprintf(char *__s, __const char *__format, __gnuc_va_list __arg) __attribute__((__nothrow__));
extern int snprintf(char *__s, size_t __maxlen, __const char *__format, ...) __attribute__((__nothrow__)) __attribute__((__format__(__printf__, 3, 4)));
extern int vsnprintf(char *__s, size_t __maxlen, __const char *__format, __gnuc_va_list __arg) __attribute__((__nothrow__)) __attribute__((__format__(__printf__, 3, 0)));
extern int vdprintf(int __fd, __const char *__fmt, __gnuc_va_list __arg) __attribute__((__format__(__printf__, 2, 0)));
extern int dprintf(int __fd, __const char *__fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
extern int fscanf(FILE *__stream, __const char *__format, ...);
extern int scanf(__const char *__format, ...);
extern int sscanf(__const char *__s, __const char *__format, ...) __attribute__((__nothrow__));
extern int fscanf(FILE *__stream, __const char *__format, ...) __asm__ ("""__isoc99_fscanf");
extern int scanf(__const char *__format, ...) __asm__ ("""__isoc99_scanf");
extern int sscanf(__const char *__s, __const char *__format, ...) __asm__ ("""__isoc99_sscanf") __attribute__((__nothrow__));
extern int vfscanf(FILE *__s, __const char *__format, __gnuc_va_list __arg) __attribute__((__format__(__scanf__, 2, 0)));
extern int vscanf(__const char *__format, __gnuc_va_list __arg) __attribute__((__format__(__scanf__, 1, 0)));
extern int vsscanf(__const char *__s, __const char *__format, __gnuc_va_list __arg) __attribute__((__nothrow__)) __attribute__((__format__(__scanf__, 2, 0)));
extern int vfscanf(FILE *__s, __const char *__format, __gnuc_va_list __arg) __asm__ ("""__isoc99_vfscanf") __attribute__((__format__(__scanf__, 2, 0)));
extern int vscanf(__const char *__format, __gnuc_va_list __arg) __asm__ ("""__isoc99_vscanf") __attribute__((__format__(__scanf__, 1, 0)));
extern int vsscanf(__const char *__s, __const char *__format, __gnuc_va_list __arg) __asm__ ("""__isoc99_vsscanf") __attribute__((__nothrow__)) __attribute__((__format__(__scanf__, 2, 0)));
extern int fgetc(FILE *__stream);
extern int getc(FILE *__stream);
extern int getchar(void);
extern int getc_unlocked(FILE *__stream);
extern int getchar_unlocked(void);
extern int fgetc_unlocked(FILE *__stream);
extern int fputc(int __c, FILE *__stream);
extern int putc(int __c, FILE *__stream);
extern int putchar(int __c);
extern int fputc_unlocked(int __c, FILE *__stream);
extern int putc_unlocked(int __c, FILE *__stream);
extern int putchar_unlocked(int __c);
extern int getw(FILE *__stream);
extern int putw(int __w, FILE *__stream);
extern char *fgets(char *__s, int __n, FILE *__stream);
extern char *gets(char *__s);
extern __ssize_t __getdelim(char **__lineptr, size_t *__n, int __delimiter, FILE *__stream);
extern __ssize_t getdelim(char **__lineptr, size_t *__n, int __delimiter, FILE *__stream);
extern __ssize_t getline(char **__lineptr, size_t *__n, FILE *__stream);
extern int fputs(__const char *__s, FILE *__stream);
extern int puts(__const char *__s);
extern int ungetc(int __c, FILE *__stream);
extern size_t fread(void *__ptr, size_t __size, size_t __n, FILE *__stream);
extern size_t fwrite(__const void *__ptr, size_t __size, size_t __n, FILE *__s);
extern size_t fread_unlocked(void *__ptr, size_t __size, size_t __n, FILE *__stream);
extern size_t fwrite_unlocked(__const void *__ptr, size_t __size, size_t __n, FILE *__stream);
extern int fseek(FILE *__stream, long int __off, int __whence);
extern long int ftell(FILE *__stream);
extern void rewind(FILE *__stream);
extern int fseeko(FILE *__stream, __off_t __off, int __whence);
extern __off_t ftello(FILE *__stream);
extern int fgetpos(FILE *__stream, fpos_t *__pos);
extern int fsetpos(FILE *__stream, __const fpos_t *__pos);
extern void clearerr(FILE *__stream) __attribute__((__nothrow__));
extern int feof(FILE *__stream) __attribute__((__nothrow__));
extern int ferror(FILE *__stream) __attribute__((__nothrow__));
extern void clearerr_unlocked(FILE *__stream) __attribute__((__nothrow__));
extern int feof_unlocked(FILE *__stream) __attribute__((__nothrow__));
extern int ferror_unlocked(FILE *__stream) __attribute__((__nothrow__));
extern void perror(__const char *__s);
extern int sys_nerr;
extern __const char *sys_errlist[];
extern int fileno(FILE *__stream) __attribute__((__nothrow__));
extern int fileno_unlocked(FILE *__stream) __attribute__((__nothrow__));
extern FILE *popen(__const char *__command, __const char *__modes);
extern int pclose(FILE *__stream);
extern char *ctermid(char *__s) __attribute__((__nothrow__));
extern void flockfile(FILE *__stream) __attribute__((__nothrow__));
extern int ftrylockfile(FILE *__stream) __attribute__((__nothrow__));
extern void funlockfile(FILE *__stream) __attribute__((__nothrow__));
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long int uint_least64_t;
typedef signed char int_fast8_t;
typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long int uint_fast64_t;
typedef unsigned long int uintptr_t;
typedef long int intmax_t;
typedef unsigned long int uintmax_t;
typedef long int ptrdiff_t;
typedef int wchar_t;
typedef struct 
{
        void **address;
        ptrdiff_t offset;
        struct 
        {
                _Bool input : 1;
                _Bool output : 1;
                _Bool can_rename : 1;
                _Bool aa : 1;
        } flags;
        size_t size;
} nanos_dependence_internal_t;
typedef enum 
{
    NANOS_PRIVATE, 
    NANOS_SHARED
} nanos_sharing_t;
typedef struct 
{
        uint64_t address;
        nanos_sharing_t sharing;
        struct 
        {
                _Bool input : 1;
                _Bool output : 1;
        } flags;
        size_t size;
} nanos_copy_data_internal_t;
typedef nanos_dependence_internal_t nanos_dependence_t;
typedef nanos_copy_data_internal_t nanos_copy_data_t;
typedef struct 
{
        int _lower;
        int _upper;
        int _step;
        int _chunk;
        int _sign;
} nanos_slicer_data_for_internal_t;
typedef nanos_slicer_data_for_internal_t nanos_slicer_data_for_t;
typedef void *nanos_thread_t;
typedef void *nanos_wd_t;
typedef struct 
{
        int nsect;
        nanos_wd_t lwd[];
} nanos_compound_wd_data_t;
typedef struct 
{
        int lower;
        int upper;
        int step;
        _Bool last;
} nanos_loop_info_t;
typedef struct 
{
        _Bool mandatory_creation : 1;
        _Bool tied : 1;
        _Bool reserved0 : 1;
        _Bool reserved1 : 1;
        _Bool reserved2 : 1;
        _Bool reserved3 : 1;
        _Bool reserved4 : 1;
        _Bool reserved5 : 1;
        nanos_thread_t tie_to;
        unsigned int priority;
} nanos_wd_props_t;
typedef struct 
{
        void *(*factory)(void *prealloc, void *arg);
        size_t dd_size;
        void *arg;
} nanos_device_t;
typedef enum 
{
    NANOS_STATE_START, 
    NANOS_STATE_END, 
    NANOS_SUBSTATE_START, 
    NANOS_SUBSTATE_END, 
    NANOS_BURST_START, 
    NANOS_BURST_END, 
    NANOS_PTP_START, 
    NANOS_PTP_END, 
    NANOS_POINT, 
    EVENT_TYPES
} nanos_event_type_t;
typedef enum 
{
    NANOS_NOT_CREATED, 
    NANOS_NOT_TRACED, 
    NANOS_STARTUP, 
    NANOS_SHUTDOWN, 
    NANOS_ERROR, 
    NANOS_IDLE, 
    NANOS_RUNTIME, 
    NANOS_RUNNING, 
    NANOS_SYNCHRONIZATION, 
    NANOS_SCHEDULING, 
    NANOS_CREATION, 
    NANOS_MEM_TRANSFER, 
    NANOS_CACHE, 
    NANOS_YIELD, 
    NANOS_EVENT_STATE_TYPES
} nanos_event_state_value_t;
typedef enum 
{
    NANOS_WD_DOMAIN, 
    NANOS_WD_DEPENDENCY
} nanos_event_domain_t;
typedef long long nanos_event_id_t;
typedef unsigned int nanos_event_key_t;
typedef unsigned long long nanos_event_value_t;
typedef struct 
{
        nanos_event_key_t key;
        nanos_event_value_t value;
} nanos_event_burst_t;
typedef struct 
{
        nanos_event_state_value_t value;
} nanos_event_state_t;
typedef struct 
{
        unsigned int nkvs;
        nanos_event_key_t *keys;
        nanos_event_value_t *values;
} nanos_event_point_t;
typedef struct 
{
        nanos_event_domain_t domain;
        nanos_event_id_t id;
        unsigned int nkvs;
        nanos_event_key_t *keys;
        nanos_event_value_t *values;
} nanos_event_ptp_t;
typedef struct 
{
        nanos_event_type_t type;
        union 
        {
                nanos_event_burst_t burst;
                nanos_event_state_t state;
                nanos_event_point_t point;
                nanos_event_ptp_t ptp;
        } info;
} nanos_event_t;
typedef enum 
{
    NANOS_LOCK_FREE = 0, 
    NANOS_LOCK_BUSY = 1
} nanos_lock_state_t;
typedef struct nanos_lock_t
{
        volatile nanos_lock_state_t _state;
} nanos_lock_t;
typedef void *nanos_wg_t;
typedef void *nanos_team_t;
typedef void *nanos_sched_t;
typedef void *nanos_slicer_t;
typedef void *nanos_slicer_data_t;
typedef void *nanos_dd_t;
typedef void *nanos_sync_cond_t;
typedef unsigned int nanos_copy_id_t;
typedef struct 
{
        int nthreads;
        void *arch;
} nanos_constraint_t;
typedef enum 
{
    NANOS_OK = 0, 
    NANOS_UNKNOWN_ERR, 
    NANOS_UNIMPLEMENTED
} nanos_err_t;
typedef struct 
{
        void (*outline)(void *);
} nanos_smp_args_t;
nanos_wd_t nanos_current_wd(void);
int nanos_get_wd_id(nanos_wd_t wd);
nanos_slicer_t nanos_find_slicer(const char *slicer);
nanos_err_t nanos_create_wd(nanos_wd_t *wd, size_t num_devices, nanos_device_t *devices, size_t data_size, int align, void **data, nanos_wg_t wg, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t **copies);
nanos_err_t nanos_create_sliced_wd(nanos_wd_t *uwd, size_t num_devices, nanos_device_t *devices, size_t outline_data_size, int align, void **outline_data, nanos_wg_t uwg, nanos_slicer_t slicer, size_t slicer_data_size, int slicer_align, void **slicer_data, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t **copies);
nanos_err_t nanos_submit(nanos_wd_t wd, size_t num_deps, nanos_dependence_t *deps, nanos_team_t team);
nanos_err_t nanos_create_wd_and_run(size_t num_devices, nanos_device_t *devices, size_t data_size, int align, void *data, size_t num_deps, nanos_dependence_t *deps, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t *copies);
nanos_err_t nanos_create_for(void);
nanos_err_t nanos_set_internal_wd_data(nanos_wd_t wd, void *data);
nanos_err_t nanos_get_internal_wd_data(nanos_wd_t wd, void **data);
nanos_err_t nanos_yield(void);
nanos_err_t nanos_slicer_get_specific_data(nanos_slicer_t slicer, void **data);
nanos_err_t nanos_create_team(nanos_team_t *team, nanos_sched_t sg, unsigned int *nthreads, nanos_constraint_t *constraints, _Bool reuse, nanos_thread_t *info);
nanos_err_t nanos_create_team_mapped(nanos_team_t *team, nanos_sched_t sg, unsigned int *nthreads, unsigned int *mapping);
nanos_err_t nanos_leave_team();
nanos_err_t nanos_end_team(nanos_team_t team);
nanos_err_t nanos_team_barrier(void);
nanos_err_t nanos_single_guard(_Bool *);
nanos_err_t nanos_wg_wait_completion(nanos_wg_t wg);
nanos_err_t nanos_create_int_sync_cond(nanos_sync_cond_t *sync_cond, volatile int *p, int condition);
nanos_err_t nanos_create_bool_sync_cond(nanos_sync_cond_t *sync_cond, volatile _Bool *p, _Bool condition);
nanos_err_t nanos_sync_cond_wait(nanos_sync_cond_t *sync_cond);
nanos_err_t nanos_sync_cond_signal(nanos_sync_cond_t *sync_cond);
nanos_err_t nanos_destroy_sync_cond(nanos_sync_cond_t *sync_cond);
nanos_err_t nanos_wait_on(size_t num_deps, nanos_dependence_t *deps);
nanos_err_t nanos_init_lock(nanos_lock_t **lock);
nanos_err_t nanos_set_lock(nanos_lock_t *lock);
nanos_err_t nanos_unset_lock(nanos_lock_t *lock);
nanos_err_t nanos_try_lock(nanos_lock_t *lock, _Bool *result);
nanos_err_t nanos_destroy_lock(nanos_lock_t *lock);
nanos_err_t nanos_get_addr(nanos_copy_id_t copy_id, void **addr);
nanos_err_t nanos_copy_value(void *dst, nanos_copy_id_t copy_id);
nanos_err_t nanos_get_num_running_tasks(int *num);
void nanos_handle_error(nanos_err_t err);
void *nanos_smp_factory(void *prealloc, void *args);
extern const size_t nanos_smp_dd_size;
void *nanos_gpu_factory(void *prealloc, void *args);
extern const size_t nanos_gpu_dd_size;
nanos_err_t nanos_instrument_register_key(nanos_event_key_t *event_key, const char *key, const char *description, _Bool abort_when_registered);
nanos_err_t nanos_instrument_register_value(nanos_event_value_t *event_value, const char *key, const char *value, const char *description, _Bool abort_when_registered);
nanos_err_t nanos_instrument_get_key(const char *key, nanos_event_key_t *event_key);
nanos_err_t nanos_instrument_get_value(const char *key, const char *value, nanos_event_value_t *event_value);
nanos_err_t nanos_instrument_events(unsigned int num_events, nanos_event_t events[]);
nanos_err_t nanos_instrument_enter_state(nanos_event_state_value_t state);
nanos_err_t nanos_instrument_leave_state(void);
nanos_err_t nanos_instrument_enter_burst(nanos_event_key_t key, nanos_event_value_t value);
nanos_err_t nanos_instrument_leave_burst(nanos_event_key_t key);
nanos_err_t nanos_instrument_point_event(unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values);
nanos_err_t nanos_instrument_ptp_start(nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values);
nanos_err_t nanos_instrument_ptp_end(nanos_event_domain_t domain, nanos_event_id_t id, unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values);
nanos_err_t nanos_instrument_disable_state_events(nanos_event_state_value_t state);
nanos_err_t nanos_instrument_enable_state_events(void);
void task(int *a, int *b)
{
}
typedef struct _nx_data_env_0_t_tag
{
        int *__tmp_0_0;
        int *__tmp_1_0;
} _nx_data_env_0_t;
int main();
static void _smp__ol_main_0(_nx_data_env_0_t *_args)
{
    {
        task((_args->__tmp_0_0), (_args->__tmp_1_0));
    }
}
int main()
{
    int a, b, i;
    for (i = 0;
        i < 100;
        i++)
    {
        {
            int *__tmp_0 = &a;
            int *__tmp_1 = &b;
            {
                /* SMP device descriptor */
                nanos_smp_args_t _ol_main_0_smp_args = {(void (*)(void *)) _smp__ol_main_0};
                nanos_device_t _ol_main_0_devices[] = {{
                    nanos_smp_factory,
                    nanos_smp_dd_size,
                    &_ol_main_0_smp_args
                }};
                _nx_data_env_0_t *ol_args = (_nx_data_env_0_t *) 0;
                nanos_wd_t wd = (nanos_wd_t) 0;
                nanos_wd_props_t props;
                __builtin_memset(&props, 0, sizeof (props));
                props.tied = 1;
                nanos_err_t err;
                err = nanos_create_wd(&wd, 1, _ol_main_0_devices, sizeof(_nx_data_env_0_t), __alignof__(_nx_data_env_0_t), (void **) &ol_args, nanos_current_wd(), &props, 0, (nanos_copy_data_t **) 0);
                if (err != NANOS_OK)
                    nanos_handle_error(err);
                if (wd != (nanos_wd_t) 0)
                {
                    ol_args->__tmp_0_0 = __tmp_0;
                    ol_args->__tmp_1_0 = __tmp_1;
                    nanos_dependence_t _dependences[2] = {
                        {
                            (void **) &ol_args->__tmp_0_0,
                            ((char *) ((__tmp_0)) - (char *) ol_args->__tmp_0_0),
                            {
                                1,
                                1,
                                1,
                                0
                            },
                            sizeof(int)
                        },
                        {
                            (void **) &ol_args->__tmp_1_0,
                            ((char *) ((__tmp_1)) - (char *) ol_args->__tmp_1_0),
                            {
                                1,
                                1,
                                1,
                                0
                            },
                            sizeof(int)
                        }
                    };
                    err = nanos_submit(wd, 2, (nanos_dependence_t *) _dependences, (nanos_team_t) 0);
                    if (err != NANOS_OK)
                        nanos_handle_error(err);
                }
                else
                {
                    _nx_data_env_0_t imm_args;
                    imm_args.__tmp_0_0 = __tmp_0;
                    imm_args.__tmp_1_0 = __tmp_1;
                    nanos_dependence_t _dependences[2] = {
                        {
                            (void **) &imm_args.__tmp_0_0,
                            ((char *) ((__tmp_0)) - (char *) imm_args.__tmp_0_0),
                            {
                                1,
                                1,
                                1,
                                0
                            },
                            sizeof(int)
                        },
                        {
                            (void **) &imm_args.__tmp_1_0,
                            ((char *) ((__tmp_1)) - (char *) imm_args.__tmp_1_0),
                            {
                                1,
                                1,
                                1,
                                0
                            },
                            sizeof(int)
                        }
                    };
                    err = nanos_create_wd_and_run(1, _ol_main_0_devices, sizeof(_nx_data_env_0_t), __alignof__(_nx_data_env_0_t),  &imm_args, 2, (nanos_dependence_t *) _dependences, &props, 0, (nanos_copy_data_t *) 0);
                    if (err != NANOS_OK)
                        nanos_handle_error(err);
                }
            }
        }
    }
    nanos_wg_wait_completion(nanos_current_wd());
}
