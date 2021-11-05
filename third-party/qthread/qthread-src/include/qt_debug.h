#ifndef QTHREAD_DEBUG_H
#define QTHREAD_DEBUG_H

#include <stdlib.h> /* for malloc() and friends */
#include "qt_alloc.h"
#ifndef EXTERNAL_ALLOCATOR
#ifdef QTHREAD_MEMORY_SCRIBBLING
#include <string.h> /* for memset(), per C90 */
# define ALLOC_SCRIBBLE(ptr, sz) memset((ptr), 0x55, (sz))
# define FREE_SCRIBBLE(ptr, sz)  memset((ptr), 0x77, (sz))
static QINLINE void *MALLOC(size_t sz)
{
    void *ret = qt_malloc(sz);

    ALLOC_SCRIBBLE(ret, sz);
    return ret;
}
# define FREE(ptr, sz) do { FREE_SCRIBBLE(ptr, sz); qt_free(ptr); } while (0)
#else
# define ALLOC_SCRIBBLE(ptr, sz) do { } while (0)
# define FREE_SCRIBBLE(ptr, sz)  do { } while (0)
# define MALLOC(sz) qt_malloc(sz)
# define FREE(ptr, sz) qt_free(ptr)
#endif
#else
# define ALLOC_SCRIBBLE(ptr, sz) do { } while (0)
# define FREE_SCRIBBLE(ptr, sz)  do { } while (0)
#endif

#ifdef QTHREAD_DEBUG
# define DEBUG_ONLY(x) x

/* System headers */
# include <limits.h>      // for INT_MAX, per C89
# if defined(HAVE_SYS_SYSCALL_H) && defined(HAVE_DECL_SYS_WRITE)
#  include <sys/syscall.h> // for syscall()
#  include <unistd.h>      // for SYS_write
#  define WRITE(fd, ch, len) syscall(SYS_write, (fd), (ch), (len))
# else
#  include <unistd.h>      // for write()
#  define WRITE(fd, ch, len) write((fd), (ch), (len))
# endif
# include <stdarg.h>      // for va_start and friends

/* Internal headers */
# include "qt_atomics.h"
# include "qt_asserts.h"

enum qthread_debug_levels {
    NO_DEBUG_OUTPUT = 0,
    DEBUG_CALLS     = 1,
    DEBUG_FUNCTIONS = 2,
    DEBUG_BEHAVIOR  = 4,
    DEBUG_DETAILS   = 8,
    ALWAYS_OUTPUT   = INT_MAX,
};
extern enum qthread_debug_levels debuglevel;

# ifdef QTHREAD_DEBUG_AFFINITY
#  define AFFINITY_CALLS     DEBUG_CALLS
#  define AFFINITY_FUNCTIONS DEBUG_FUNCTIONS
#  define AFFINITY_BEHAVIOR  DEBUG_BEHAVIOR
#  define AFFINITY_DETAILS   DEBUG_DETAILS
# else
#  define AFFINITY_CALLS     NO_DEBUG_OUTPUT
#  define AFFINITY_FUNCTIONS NO_DEBUG_OUTPUT
#  define AFFINITY_BEHAVIOR  NO_DEBUG_OUTPUT
#  define AFFINITY_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_THREADQUEUES
#  define THREADQUEUE_CALLS     DEBUG_CALLS
#  define THREADQUEUE_FUNCTIONS DEBUG_FUNCTIONS
#  define THREADQUEUE_BEHAVIOR  DEBUG_BEHAVIOR
#  define THREADQUEUE_DETAILS   DEBUG_DETAILS
# else
#  define THREADQUEUE_CALLS     NO_DEBUG_OUTPUT
#  define THREADQUEUE_FUNCTIONS NO_DEBUG_OUTPUT
#  define THREADQUEUE_BEHAVIOR  NO_DEBUG_OUTPUT
#  define THREADQUEUE_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_MPOOL
#  define MPOOL_CALLS     DEBUG_CALLS
#  define MPOOL_FUNCTIONS DEBUG_FUNCTIONS
#  define MPOOL_BEHAVIOR  DEBUG_BEHAVIOR
#  define MPOOL_DETAILS   DEBUG_DETAILS
# else
#  define MPOOL_CALLS     NO_DEBUG_OUTPUT
#  define MPOOL_FUNCTIONS NO_DEBUG_OUTPUT
#  define MPOOL_BEHAVIOR  NO_DEBUG_OUTPUT
#  define MPOOL_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_CORE
#  define CORE_CALLS     DEBUG_CALLS
#  define CORE_FUNCTIONS DEBUG_FUNCTIONS
#  define CORE_BEHAVIOR  DEBUG_BEHAVIOR
#  define CORE_DETAILS   DEBUG_DETAILS
# else
#  define CORE_CALLS     NO_DEBUG_OUTPUT
#  define CORE_FUNCTIONS NO_DEBUG_OUTPUT
#  define CORE_BEHAVIOR  NO_DEBUG_OUTPUT
#  define CORE_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_TEAM
#  define TEAM_CALLS     DEBUG_CALLS
#  define TEAM_FUNCTIONS DEBUG_FUNCTIONS
#  define TEAM_BEHAVIOR  DEBUG_BEHAVIOR
#  define TEAM_DETAILS   DEBUG_DETAILS
# else
#  define TEAM_CALLS     NO_DEBUG_OUTPUT
#  define TEAM_FUNCTIONS NO_DEBUG_OUTPUT
#  define TEAM_BEHAVIOR  NO_DEBUG_OUTPUT
#  define TEAM_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_THREADS
#  define THREAD_CALLS     DEBUG_CALLS
#  define THREAD_FUNCTIONS DEBUG_FUNCTIONS
#  define THREAD_BEHAVIOR  DEBUG_BEHAVIOR
#  define THREAD_DETAILS   DEBUG_DETAILS
# else
#  define THREAD_CALLS     NO_DEBUG_OUTPUT
#  define THREAD_FUNCTIONS NO_DEBUG_OUTPUT
#  define THREAD_BEHAVIOR  NO_DEBUG_OUTPUT
#  define THREAD_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_SYSCALLS
#  define SYSCDEBUG_CALLS     DEBUG_CALLS
#  define SYSCDEBUG_FUNCTIONS DEBUG_FUNCTIONS
#  define SYSCDEBUG_BEHAVIOR  DEBUG_BEHAVIOR
#  define SYSCDEBUG_DETAILS   DEBUG_DETAILS
# else
#  define SYSCDEBUG_CALLS     NO_DEBUG_OUTPUT
#  define SYSCDEBUG_FUNCTIONS NO_DEBUG_OUTPUT
#  define SYSCDEBUG_BEHAVIOR  NO_DEBUG_OUTPUT
#  define SYSCDEBUG_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_FEBS
#  define FEB_CALLS     DEBUG_CALLS
#  define FEB_FUNCTIONS DEBUG_FUNCTIONS
#  define FEB_BEHAVIOR  DEBUG_BEHAVIOR
#  define FEB_DETAILS   DEBUG_DETAILS
# else
#  define FEB_CALLS     NO_DEBUG_OUTPUT
#  define FEB_FUNCTIONS NO_DEBUG_OUTPUT
#  define FEB_BEHAVIOR  NO_DEBUG_OUTPUT
#  define FEB_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_SYNCVARS
#  define SYNCVAR_CALLS     DEBUG_CALLS
#  define SYNCVAR_FUNCTIONS DEBUG_FUNCTIONS
#  define SYNCVAR_BEHAVIOR  DEBUG_BEHAVIOR
#  define SYNCVAR_DETAILS   DEBUG_DETAILS
# else
#  define SYNCVAR_CALLS     NO_DEBUG_OUTPUT
#  define SYNCVAR_FUNCTIONS NO_DEBUG_OUTPUT
#  define SYNCVAR_BEHAVIOR  NO_DEBUG_OUTPUT
#  define SYNCVAR_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_LOOPS
#  define LOOP_CALLS     DEBUG_CALLS
#  define LOOP_FUNCTIONS DEBUG_FUNCTIONS
#  define LOOP_BEHAVIOR  DEBUG_BEHAVIOR
#  define LOOP_DETAILS   DEBUG_DETAILS
# else
#  define LOOP_CALLS     NO_DEBUG_OUTPUT
#  define LOOP_FUNCTIONS NO_DEBUG_OUTPUT
#  define LOOP_BEHAVIOR  NO_DEBUG_OUTPUT
#  define LOOP_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_IO
#  define IO_CALLS     DEBUG_CALLS
#  define IO_FUNCTIONS DEBUG_FUNCTIONS
#  define IO_BEHAVIOR  DEBUG_BEHAVIOR
#  define IO_DETAILS   DEBUG_DETAILS
# else
#  define IO_CALLS     NO_DEBUG_OUTPUT
#  define IO_FUNCTIONS NO_DEBUG_OUTPUT
#  define IO_BEHAVIOR  NO_DEBUG_OUTPUT
#  define IO_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_BARRIER
#  define BARRIER_CALLS     DEBUG_CALLS
#  define BARRIER_FUNCTIONS DEBUG_FUNCTIONS
#  define BARRIER_BEHAVIOR  DEBUG_BEHAVIOR
#  define BARRIER_DETAILS   DEBUG_DETAILS
# else
#  define BARRIER_CALLS     NO_DEBUG_OUTPUT
#  define BARRIER_FUNCTIONS NO_DEBUG_OUTPUT
#  define BARRIER_BEHAVIOR  NO_DEBUG_OUTPUT
#  define BARRIER_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_QARRAY
#  define QARRAY_CALLS     DEBUG_CALLS
#  define QARRAY_FUNCTIONS DEBUG_FUNCTIONS
#  define QARRAY_BEHAVIOR  DEBUG_BEHAVIOR
#  define QARRAY_DETAILS   DEBUG_DETAILS
# else
#  define QARRAY_CALLS     NO_DEBUG_OUTPUT
#  define QARRAY_FUNCTIONS NO_DEBUG_OUTPUT
#  define QARRAY_BEHAVIOR  NO_DEBUG_OUTPUT
#  define QARRAY_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_SHEPHERD
#  define SHEPHERD_CALLS     DEBUG_CALLS
#  define SHEPHERD_FUNCTIONS DEBUG_FUNCTIONS
#  define SHEPHERD_BEHAVIOR  DEBUG_BEHAVIOR
#  define SHEPHERD_DETAILS   DEBUG_DETAILS
# else
#  define SHEPHERD_CALLS     NO_DEBUG_OUTPUT
#  define SHEPHERD_FUNCTIONS NO_DEBUG_OUTPUT
#  define SHEPHERD_BEHAVIOR  NO_DEBUG_OUTPUT
#  define SHEPHERD_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_FUTURELIB
#  define FUTURELIB_CALLS     DEBUG_CALLS
#  define FUTURELIB_FUNCTIONS DEBUG_FUNCTIONS
#  define FUTURELIB_BEHAVIOR  DEBUG_BEHAVIOR
#  define FUTURELIB_DETAILS   DEBUG_DETAILS
# else
#  define FUTURELIB_CALLS     NO_DEBUG_OUTPUT
#  define FUTURELIB_FUNCTIONS NO_DEBUG_OUTPUT
#  define FUTURELIB_BEHAVIOR  NO_DEBUG_OUTPUT
#  define FUTURELIB_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_XOMP
#  define XOMP_CALLS     DEBUG_CALLS
#  define XOMP_FUNCTIONS DEBUG_FUNCTIONS
#  define XOMP_BEHAVIOR  DEBUG_BEHAVIOR
#  define XOMP_DETAILS   DEBUG_DETAILS
# else
#  define XOMP_CALLS     NO_DEBUG_OUTPUT
#  define XOMP_FUNCTIONS NO_DEBUG_OUTPUT
#  define XOMP_BEHAVIOR  NO_DEBUG_OUTPUT
#  define XOMP_DETAILS   NO_DEBUG_OUTPUT
# endif
# ifdef QTHREAD_DEBUG_MULTINODE
#  define MULTINODE_CALLS     DEBUG_CALLS
#  define MULTINODE_FUNCTIONS DEBUG_FUNCTIONS
#  define MULTINODE_BEHAVIOR  DEBUG_BEHAVIOR
#  define MULTINODE_DETAILS   DEBUG_DETAILS
# else
#  define MULTINODE_CALLS     NO_DEBUG_OUTPUT
#  define MULTINODE_FUNCTIONS NO_DEBUG_OUTPUT
#  define MULTINODE_BEHAVIOR  NO_DEBUG_OUTPUT
#  define MULTINODE_DETAILS   NO_DEBUG_OUTPUT
# endif

extern QTHREAD_FASTLOCK_TYPE output_lock;

#  ifdef HAVE_GNU_VAMACROS
#   define qthread_debug(level, format, args ...) qthread_debug_(level, "%s(%u): " format, __FUNCTION__, __LINE__, ## args)
static QINLINE void qthread_debug_(int         level,
                                   const char *format,
                                   ...)
#  elif defined(HAVE_C99_VAMACROS)
#   define qthread_debug(level, format, ...) qthread_debug_(level, "%s(%u): " format, __FUNCTION__, __LINE__, __VA_ARGS__)
static QINLINE void qthread_debug_(int         level,
                                   const char *format,
                                   ...)
#  else
static QINLINE void qthread_debug(int         level,
                                  const char *format,
                                  ...)
#  endif // ifdef HAVE_GNU_VAMACROS
{                                      /*{{{ */
    va_list args;

    if (level & debuglevel || level == ALWAYS_OUTPUT) {
        static char buf[1024];  /* protected by the output_lock */
        char       *head = buf;
        char        ch;

        QTHREAD_FASTLOCK_LOCK(&output_lock);

        while (WRITE(2, "QDEBUG: ", 8) != 8) ;

        va_start(args, format);
        /* avoiding the obvious method, to save on memory
         * vfprintf(stderr, format, args); */
        while ((ch = *format++)) {
            assert(head < (buf + 1024));
            if (ch == '%') {
                char ss = '0'; // size specifier

                ch = *format++;

                // Chomp off the size specifier
                if ('l' == ch || 'L' == ch || 'h' == ch) {
                    ss = ch;
                    ch = *format++;
                }

                switch (ch) {
                    case 'c':
                    {
                        *head++ = ch;
                        break;
                    }
                    case 's':
                    {
                        char *str = va_arg(args, char *);

                        qassert(WRITE(2, buf, head - buf), head - buf);
                        head = buf;
                        if (str == NULL) {
                            qassert(WRITE(2, "(null)", 6), 6);
                        } else {
                            qassert(WRITE(2, str, strlen(str)), (ssize_t)strlen(str));
                        }
                        break;
                    }
                    case 'p':
                        *head++ = '0';
                        *head++ = 'x';
                        {
                            uintptr_t num;

                            num = va_arg(args, uintptr_t);
                            if (!num) {
                                *head++ = '0';
                            } else {
                                /* count places */
                                unsigned  places = 0;
                                uintptr_t tmpnum = num;

                                /* yes, this is dumb, but its guaranteed to take
                                 * less than 10 iterations on 32-bit numbers and
                                 * doesn't involve floating point */
                                while (tmpnum >= 16) {
                                    tmpnum /= 16;
                                    places++;
                                }
                                head  += places;
                                places = 0;
                                while (num >= 16) {
                                    uintptr_t tmp = num % 16;
                                    *(head - places) =
                                        (tmp <
                                         10) ? ('0' + tmp) : ('a' + tmp - 10);
                                    num /= 16;
                                    places++;
                                }
                                num             %= 16;
                                *(head - places) =
                                    (num < 10) ? ('0' + num) : ('a' + num - 10);
                                head++;
                            }
                        }
                        break;
                    case 'z':
                    {
                        size_t num = va_arg(args, size_t);
                        /* count places */
                        unsigned places = 0;
                        size_t   tmpnum = num;
                        /* yes, this is dumb, but its guaranteed to take
                         * less than 10 iterations on 32-bit numbers and
                         * doesn't involve floating point */
                        while (tmpnum >= 10) {
                            tmpnum /= 10;
                            places++;
                        }
                        head  += places;
                        places = 0;
                        while (num >= 10) {
                            uintptr_t tmp = num % 10;
                            *(head - places) = (tmp < 10) ? ('0' + tmp) : ('a' + tmp - 10);
                            num             /= 10;
                            places++;
                        }
                        num             %= 10;
                        *(head - places) = (num < 10) ? ('0' + num) : ('a' + num - 10);
                        head++;
                        break;
                    }
                    case 'x':
                        *head++ = '0';
                        *head++ = 'x';
                    case 'u':
                    case 'd':
                    case 'i':
                    {
                        if ('l' == ss) {
                            unsigned long num;
                            unsigned      base;

                            num  = va_arg(args, unsigned long int);
                            base = (ch == 'x') ? 16 : 10;
                            if (!num) {
                                *head++ = '0';
                            } else {
                                /* count places */
                                unsigned  places = 0;
                                unsigned long tmpnum = num;

                                /* yes, this is dumb, but its guaranteed to take
                                 * less than 10 iterations on 32-bit numbers and
                                 * doesn't involve floating point */
                                while (tmpnum >= base) {
                                    tmpnum /= base;
                                    places++;
                                }
                                head  += places;
                                places = 0;
                                while (num >= base) {
                                    unsigned long tmp = num % base;
                                    *(head - places) = (tmp < 10) ? ('0' + tmp) : ('a' + tmp - 10);
                                    num             /= base;
                                    places++;
                                }
                                num             %= base;
                                *(head - places) = (num < 10) ? ('0' + num) : ('a' + num - 10);
                                head++;
                            }
                            break;
                        } else {
                            unsigned int num;
                            unsigned     base;

                            num  = va_arg(args, unsigned int);
                            base = (ch == 'x') ? 16 : 10;
                            if (!num) {
                                *head++ = '0';
                            } else {
                                /* count places */
                                unsigned  places = 0;
                                uintptr_t tmpnum = num;

                                /* yes, this is dumb, but its guaranteed to take
                                 * less than 10 iterations on 32-bit numbers and
                                 * doesn't involve floating point */
                                while (tmpnum >= base) {
                                    tmpnum /= base;
                                    places++;
                                }
                                head  += places;
                                places = 0;
                                while (num >= base) {
                                    uintptr_t tmp = num % base;
                                    *(head - places) = (tmp < 10) ? ('0' + tmp) : ('a' + tmp - 10);
                                    num             /= base;
                                    places++;
                                }
                                num             %= base;
                                *(head - places) = (num < 10) ? ('0' + num) : ('a' + num - 10);
                                head++;
                            }
                            break;
                        }
                    }
                    case 'X':
                        *head++ = '0';
                        *head++ = 'x';
                    case 'U':
                    case 'D':
                    case 'I':
                    {
                        uint64_t num = va_arg(args, uint64_t);
                        unsigned base;

                        base = (ch == 'X') ? 16 : 10;
                        if (!num) {
                            *head++ = '0';
                        } else {
                            /* count places */
                            unsigned places = 0;
                            uint64_t tmpnum = num;

                            /* yes, this is dumb, but its guaranteed to take
                             * less than 10 iterations on 32-bit numbers and
                             * doesn't involve floating point */
                            while (tmpnum >= base) {
                                tmpnum /= base;
                                places++;
                            }
                            head  += places;
                            places = 0;
                            while (num >= base) {
                                uintptr_t tmp = num % base;
                                *(head - places) = (tmp < 10) ? ('0' + tmp) : ('a' + tmp - 10);
                                num             /= base;
                                places++;
                            }
                            num             %= base;
                            *(head - places) = (num < 10) ? ('0' + num) : ('a' + num - 10);
                            head++;
                        }
                        break;
                    }
                    default:
                        *head++ = '%';
                        *head++ = ch;
                }
            } else {
                *head++ = ch;
            }
        }
        assert(head > buf);
        {
            char *curs = buf;
            int ret = WRITE(2, curs, head - curs);
            assert(ret >= 0);
            while (ret > 0 && (curs+ret) != head) {
                curs += ret;
                assert(head > curs);
                ret = WRITE(2, curs, head - curs);
            }
        }
        va_end(args);

        QTHREAD_FASTLOCK_UNLOCK(&output_lock);
    }
}                                      /*}}} */

#else /* ifdef QTHREAD_DEBUG */
# define DEBUG_ONLY(x)
# define qthread_debug(...) do { } while(0)
#endif /* ifdef QTHREAD_DEBUG */

#endif /* QTHREAD_DEBUG_H */
/* vim:set expandtab: */
