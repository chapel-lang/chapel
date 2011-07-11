#ifndef QTHREAD_INNARDS_H
#define QTHREAD_INNARDS_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <math.h>

#ifdef QTHREAD_HAVE_HWLOC
# include <hwloc.h>
# if (HWLOC_API_VERSION < 0x00010000)
#  error HWLOC version unrecognized
# endif
#endif

#include "qt_visibility.h"

#include "qthread_asserts.h"
#include "qt_atomics.h"

#include "qt_context.h"

#ifdef QTHREAD_DEBUG
# include <sys/syscall.h>         /* for SYS_accept and others */
# ifdef HAVE_UNISTD_H
#  include <unistd.h>                  /* for write() */
# endif
# include <stdarg.h>                   /* for va_start and va_end */
#endif
#include <pthread.h>
#include <qt_hash.h>

/* this function allows a qthread to retrieve its qthread_t pointer if it has
 * been lost for some reason */
qthread_t *qthread_internal_self(void);

extern unsigned int QTHREAD_LOCKING_STRIPES;

typedef struct qlib_s {
    unsigned int          nshepherds;
    unsigned int          nshepherds_active;
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_TYPE nshepherds_active_lock;
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    unsigned int               nworkers_active;
    unsigned int               nworkerspershep;
#endif
    struct qthread_shepherd_s *shepherds;

    unsigned                   qthread_stack_size;
    unsigned                   master_stack_size;
    unsigned                   max_stack_size;

    unsigned                   qthread_argcopy_size;
    unsigned                   qthread_tasklocal_size;

    qthread_t                 *mccoy_thread; /* free when exiting */

    void                      *master_stack;
    qt_context_t               master_context;
#ifdef QTHREAD_USE_VALGRIND
    unsigned int               valgrind_masterstack_id;
#endif

    /* assigns a unique thread_id mostly for debugging! */
    aligned_t             max_thread_id;
    QTHREAD_FASTLOCK_TYPE max_thread_id_lock;

    /* round robin scheduler - can probably be smarter */
    aligned_t             sched_shepherd;
    QTHREAD_FASTLOCK_TYPE sched_shepherd_lock;

#if defined(QTHREAD_MUTEX_INCREMENT) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    QTHREAD_FASTLOCK_TYPE *atomic_locks;
# ifdef QTHREAD_COUNT_THREADS
    aligned_t             *atomic_stripes;
    QTHREAD_FASTLOCK_TYPE *atomic_stripes_locks;
# endif
#endif
    /* this is how we manage FEB-type locks
     * NOTE: this can be a major bottleneck and we should probably create
     * multiple hashtables to improve performance. The current hashing is a bit
     * of a hack, but improves the bottleneck a bit
     */
    qt_hash               *locks;
#ifdef QTHREAD_COUNT_THREADS
    aligned_t             *locks_stripes;
# ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_TYPE *locks_stripes_locks;
# endif
#endif
    /* these are separated out for memory reasons: if you can get away with
     * simple locks, then you can use a little less memory. Subject to the same
     * bottleneck concerns as the above hashtable, though these are slightly
     * better at shrinking their critical section. FEBs have more memory
     * overhead, though. */
    qt_hash               *FEBs;
#ifdef QTHREAD_COUNT_THREADS
    aligned_t             *febs_stripes;
# ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_TYPE *febs_stripes_locks;
# endif
#endif
    /* this is for holding syncvar waiters... similar to the FEBs (perhaps
     * should have the _stripes business as well... maybe later) */
    qt_hash *syncvars;
} *qlib_t;

#ifndef QTHREAD_SST_PRIMITIVES
extern qlib_t qlib;
#endif

/* These are the internal functions that futurelib should be allowed to get at */
unsigned int INTERNAL qthread_isfuture(void);

void INTERNAL qthread_assertfuture(qthread_t *t);

void INTERNAL qthread_assertnotfuture(void);

int INTERNAL qthread_fork_future_to(const qthread_f             f,
                                    const void                 *arg,
                                    aligned_t                  *ret,
                                    const qthread_shepherd_id_t shepherd);
unsigned int INTERNAL qthread_internal_shep_to_node(const qthread_shepherd_id_t shep);
void INTERNAL         qthread_exec(qthread_t    *t,
                                   qt_context_t *c);

#define QTHREAD_NO_NODE ((unsigned int)(-1))
#ifdef QTHREAD_SST_PRIMITIVES
# define qthread_shepherd_count()                              PIM_readSpecial(PIM_CMD_LOC_COUNT)
# define qthread_fork_syncvar_future_to(me, f, arg, ret, shep) qthread_fork_syncvar_to(f, arg, ret, shep)
#else
# define qthread_shepherd_count() (qlib->nshepherds)
int INTERNAL qthread_fork_syncvar_future(const qthread_f f,
                                         const void     *arg,
                                         syncvar_t      *ret);
int INTERNAL qthread_fork_syncvar_future_to(const qthread_f             f,
                                            const void                 *arg,
                                            syncvar_t                  *ret,
                                            const qthread_shepherd_id_t shepherd);
int qthread_fork_syncvar_copyargs(const qthread_f   f,
                                  const void *const arg,
                                  const size_t      arg_size,
                                  syncvar_t *const  ret);
int qthread_fork_syncvar_copyargs_to(const qthread_f   f,
                                     const void *const arg,
                                     const size_t      arg_size,
                                     syncvar_t *const  ret,
                                     const qthread_shepherd_id_t
                                     preferred_shep);
#endif // ifdef QTHREAD_SST_PRIMITIVES

/* functions added by akp to hand openMP task completion
 */
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
void qthread_getTaskListLock(void);
void qthread_releaseTaskListLock(void);

extern int __qthreads_temp;
void INTERNAL qthread_reset_forCount(void);

int INTERNAL            qthread_forCount(int inc);
taskSyncvar_t *qthread_getTaskRetVar(void);
void qthread_setTaskRetVar(taskSyncvar_t *v);
void qt_move_to_orig(void); /* reschedule this thread on shepherd 0 worker 0 -- for termination 4/1/11 AKP */
#endif // ifdef QTHREAD_USE_ROSE_EXTENSIONS
#ifdef STEAL_PROFILE
void INTERNAL qthread_steal_stat(void);
#endif

/* internal initialization functions */
void INTERNAL qt_feb_barrier_internal_init(void);
void INTERNAL qthread_internal_cleanup(void (*function)(void));
void INTERNAL qthread_internal_cleanup_early(void (*function)(void));

# ifdef QTHREAD_RCRTOOL 
/* allow environment variable to control whether dynamic thread count
 *  adjustment active - akp 5/26/11
 */
extern int rcrSchedulingOff;
#endif

/* for debugging */
#ifdef QTHREAD_DEBUG
enum qthread_debug_levels {
    NONE = 0,
    THREAD_BEHAVIOR, LOCK_BEHAVIOR, ALL_CALLS, ALL_FUNCTIONS,
    THREAD_DETAILS, LOCK_DETAILS, ALL_DETAILS
};

extern enum qthread_debug_levels debuglevel;

extern QTHREAD_FASTLOCK_TYPE output_lock;

# ifdef QTHREAD_RCRTOOL
#  ifdef HAVE_GNU_VAMACROS
#   define qthread_debug(level, format, args ...) qthread_debug_(level, debuglevel, "QDEBUG: ", "%s(%u): " format, __FUNCTION__, __LINE__, ## args)
#   define rcrtool_debug(level, format, args ...) qthread_debug_(level, rcrtoollevel, "RCRDEBUG: ", "%s(%u): " format, __FUNCTION__, __LINE__, ## args)
static QINLINE void qthread_debug_(int         level,
                                   int         debuglevel,
                                   const char *msgPrefix,
                                   const char *format,
                                   ...)
#  elif defined(HAVE_C99_VAMACROS)
#   define qthread_debug(level, format, ...) qthread_debug_(level, debuglevel, "QDEBUG: ", "%s(%u): " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#   define rcrtool_debug(level, format, ...) qthread_debug_(level, rcrtoollevel, "RCRDEBUG: ", "%s(%u): " format, __FUNCTION__, __LINE__, __VA_ARGS__)
static QINLINE void qthread_debug_(int         level,
                                   int         debuglevel,
                                   const char *msgPrefix,
                                   const char *format,
                                   ...)
#  else // ifdef HAVE_GNU_VAMACROS
#   define qthread_debug(level, format, ...) qthread_debug_(level, debuglevel, "QDEBUG: ", format, __FUNCTION__, __LINE__, __VA_ARGS__)
#   define rcrtool_debug(level, format, ...) qthread_debug_(level, rcrtoollevel, "RCRDEBUG: ", format, __FUNCTION__, __LINE__, __VA_ARGS__)
static QINLINE void qthread_debug_(int         level,
                                   int         debuglevel,
                                   const char *msgPrefix,
                                   const char *format,
                                   ...)
#  endif // ifdef HAVE_GNU_VAMACROS
# else // ifdef QTHREAD_RCRTOOL
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
# endif  // ifdef QTHREAD_RCRTOOL
{                                      /*{{{ */
    va_list args;

    if ((level <= debuglevel) || (level == 0)) {
        static char buf[1024];  /* protected by the output_lock */
        char       *head = buf;
        char        ch;

        QTHREAD_FASTLOCK_LOCK(&output_lock);

# ifdef QTHREAD_RCRTOOL
        while (syscall(SYS_write, 2, msgPrefix, 8) != 8) ;
# else
        while (syscall(SYS_write, 2, "QDEBUG: ", 8) != 8) ;
# endif

        va_start(args, format);
        /* avoiding the obvious method, to save on memory
         * vfprintf(stderr, format, args); */
        while ((ch = *format++)) {
            assert(head < (buf + 1024));
            if (ch == '%') {
                ch = *format++;
                switch (ch) {
                    case 's':
                    {
                        char *str = va_arg(args, char *);

                        qassert(syscall(SYS_write, 2, buf, head - buf), head - buf);
                        head = buf;
                        qassert(syscall(SYS_write, 2, str, strlen(str)), strlen(str));
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
                    case 'x':
                        *head++ = '0';
                        *head++ = 'x';
                    case 'u':
                    case 'd':
                    case 'i':
                    {
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
        /* XXX: not checking for extra long values of head */
        qassert(syscall(SYS_write, 2, buf, head - buf), head - buf);
        va_end(args);
        /*fflush(stderr); */

        QTHREAD_FASTLOCK_UNLOCK(&output_lock);
    }
}                                      /*}}} */

#else // ifdef QTHREAD_DEBUG
# define qthread_debug(...) do { } while(0)
#endif // ifdef QTHREAD_DEBUG

#define QTHREAD_CHOOSE_STRIPE(addr) (((size_t)addr >> 4) & (QTHREAD_LOCKING_STRIPES - 1))

/* this is a function to check the input preconds for a nascent thread. */
int INTERNAL qthread_check_precond(qthread_t *t);

#endif // ifndef QTHREAD_INNARDS_H
/* vim:set expandtab: */
