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
#include "qt_qthread_t.h"
#include "qt_threadqueues.h"

#include <pthread.h>
#include <qt_hash.h>

/* this function allows a qthread to retrieve its qthread_t pointer if it has
 * been lost for some reason */
qthread_t *qthread_internal_self(void);

extern unsigned int QTHREAD_LOCKING_STRIPES;

#ifdef CAS_STEAL_PROFILE
// stripe this array across a cache line    
#define CAS_STEAL_PROFILE_LENGTH (CACHELINE_WIDTH / sizeof(uint64_t))
typedef struct uint64_strip_s { 
    uint64_t fields[CAS_STEAL_PROFILE_LENGTH];
}   uint64_strip_t;
#endif

typedef struct qlib_s {
    unsigned int          nshepherds;
    aligned_t             nshepherds_active;
#ifdef QTHREAD_MUTEX_INCREMENT
    QTHREAD_FASTLOCK_TYPE nshepherds_active_lock;
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    unsigned int               nworkers_active;
#endif
    unsigned int               nworkerspershep;
    struct qthread_shepherd_s *shepherds;
    qt_threadqueue_t          **threadqueues;  

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
#ifdef CAS_STEAL_PROFILE
    uint64_strip_t            *cas_steal_profile;
#endif

    /* assigns a unique thread_id mostly for debugging! */
    aligned_t             max_thread_id;
    QTHREAD_FASTLOCK_TYPE max_thread_id_lock;

    /* assigns a unique team id */
    aligned_t             max_team_id;
    QTHREAD_FASTLOCK_TYPE max_team_id_lock;

    /* assigns a unique worker_id */
    aligned_t             max_unique_id;
    QTHREAD_FASTLOCK_TYPE max_unique_id_lock;

    /* round robin scheduler - can probably be smarter */
    aligned_t             sched_shepherd;
    QTHREAD_FASTLOCK_TYPE sched_shepherd_lock;

#if defined(QTHREAD_MUTEX_INCREMENT) ||             \
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
int qthread_fork_track_syncvar_copyargs(const qthread_f   f,
                                  const void *const arg,
                                  const size_t      arg_size,
                                  syncvar_t *const  ret);
int qthread_fork_track_syncvar_copyargs_to(const qthread_f   f,
                                     const void *const arg,
                                     const size_t      arg_size,
                                     syncvar_t *const  ret,
                                     const qthread_shepherd_id_t
                                     preferred_shep);
#endif // ifdef QTHREAD_SST_PRIMITIVES

/* functions added by akp to handle OpenMP task completion
 */
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
extern int __qthreads_temp;
void INTERNAL qthread_reset_forCount(void);

int INTERNAL qthread_forCount(int inc);
#endif // ifdef QTHREAD_USE_ROSE_EXTENSIONS

/* internal initialization functions */
void INTERNAL qt_feb_barrier_internal_init(void);
void INTERNAL qthread_internal_cleanup(void (*function)(void));
void INTERNAL qthread_internal_cleanup_early(void (*function)(void));

#ifdef QTHREAD_RCRTOOL
/* allow environment variable to control whether dynamic thread count
 *  adjustment active - akp 5/26/11
 */
extern int rcrSchedulingOff;
#endif

#define QTHREAD_CHOOSE_STRIPE(addr) (((size_t)addr >> 4) & (QTHREAD_LOCKING_STRIPES - 1))

/* this is a function to check the input preconds for a nascent thread. */
int INTERNAL qthread_check_precond(qthread_t *t);

#endif // ifndef QTHREAD_INNARDS_H
/* vim:set expandtab: */
