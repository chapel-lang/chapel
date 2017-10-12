#ifndef _QTHREAD_H_
#define _QTHREAD_H_

#include <errno.h>                     /* for ENOMEM */

#include <limits.h>                    /* for UINT_MAX (C89) */
#include <qthread/qthread-int.h>       /* for uint32_t and uint64_t */
#include <qthread/common.h>            /* important configuration options */

#include <string.h>                    /* for memcpy() */

#ifndef QTHREAD_NOALIGNCHECK
# include <stdio.h>                    /* for fprintf() */
#endif

#ifdef QTHREAD_NEEDS_IA64INTRIN
# ifdef HAVE_IA64INTRIN_H
#  include <ia64intrin.h>
# elif defined(HAVE_IA32INTRIN_H)
#  include <ia32intrin.h>
# endif
#endif

/*****************************************************************************
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! *
*                                                                           *
*    The most complete documentaton is going to be in the man pages. The    *
*    documentation here is just to give you a general idea of what each     *
*    function does.                                                         *
*                                                                           *
*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*****************************************************************************/

/* Return Codes */
#define QTHREAD_SUCCESS           0
#define QTHREAD_BADARGS           -1
#define QTHREAD_PTHREAD_ERROR     -2
#define QTHREAD_NOT_ALLOWED       -3
#define QTHREAD_MALLOC_ERROR      ENOMEM
#define QTHREAD_THIRD_PARTY_ERROR -4
#define QTHREAD_TIMEOUT           -5   /* neither ETIME nor ETIMEDOUT seem appropriate, strictly speaking */
#ifdef EOVERFLOW
# define QTHREAD_OVERFLOW EOVERFLOW
#else
# define QTHREAD_OVERFLOW -6
#endif
#define QTHREAD_OPFAIL -7
#define NO_SHEPHERD    ((qthread_shepherd_id_t)-1)
#define NO_WORKER      ((qthread_worker_id_t)-1)

#define QTHREAD_VERSION 1010001

#include <qthread/macros.h>

#ifdef QTHREAD_ALIGNEDDATA_ALLOWED
# define Q_ALIGNED(x) __attribute__((aligned(x)))
#endif

Q_STARTCXX /* */
/* NOTE!!!!!!!!!!!
 * Reads and writes operate on aligned_t-size segments of memory.
 *
 * FEB locking only works on aligned addresses. This is here to try and help a
 * little bit. */
#if QTHREAD_SIZEOF_ALIGNED_T == 4
typedef uint32_t Q_ALIGNED (QTHREAD_ALIGNMENT_ALIGNED_T)      aligned_t;
typedef uint16_t Q_ALIGNED (QTHREAD_ALIGNMENT_ALIGNED_T / 2) haligned_t;
typedef int32_t Q_ALIGNED (QTHREAD_ALIGNMENT_ALIGNED_T)      saligned_t;
#elif QTHREAD_SIZEOF_ALIGNED_T == 8
typedef uint64_t Q_ALIGNED (QTHREAD_ALIGNMENT_ALIGNED_T)      aligned_t;
typedef uint32_t Q_ALIGNED (QTHREAD_ALIGNMENT_ALIGNED_T / 2) haligned_t;
typedef int64_t Q_ALIGNED (QTHREAD_ALIGNMENT_ALIGNED_T)      saligned_t;
#else
# error "Don't know type for sizeof aligned_t"
#endif

#ifndef QTHREAD_NOALIGNCHECK
# define QALIGN(d, s) do {                                                   \
        s = (aligned_t *)(((size_t)d) & (~(sizeof(aligned_t) - 1)));         \
        if (s != d) {                                                        \
            fprintf(stderr,                                                  \
                    "WARNING: %s(): unaligned address %p ... assuming %p\n", \
                    __FUNCTION__, (void *)d, (void *)s);                     \
        }                                                                    \
} while(0)
#else /* QTHREAD_NOALIGNCHECK */
# define QALIGN(d, s) (s) = (d)
#endif /* ifndef QTHREAD_NOALIGNCHECK */

typedef struct _syncvar_s {
    union {
        uint64_t w;
        struct {
#ifdef                        BITFIELD_ORDER_FORWARD
            uint64_t data  : 60;
            unsigned state : 3;
            unsigned lock  : 1;
#else
            unsigned lock  : 1;
            unsigned state : 3;
            uint64_t data  : 60;
#endif
        } s;
    } u;
} syncvar_t;

#define SYNCVAR_STATIC_INITIALIZER                { { 0 } }
#define SYNCVAR_STATIC_EMPTY_INITIALIZER          { .u.s = { .data = 0, .state = 2, .lock = 0 } }
#define SYNCVAR_STATIC_INITIALIZE_TO(value)       { .u.s = { .data = value, .state = 0, .lock = 0 } }
#define SYNCVAR_STATIC_EMPTY_INITIALIZE_TO(value) { .u.s = { .data = value, .state = 2, .lock = 0 } }
#define SYNCVAR_INITIALIZER                       ((syncvar_t)SYNCVAR_STATIC_INITIALIZER)
#define SYNCVAR_EMPTY_INITIALIZER                 ((syncvar_t)SYNCVAR_STATIC_EMPTY_INITIALIZER)
#define SYNCVAR_INITIALIZE_TO(value)              ((syncvar_t)SYNCVAR_STATIC_INITIALIZE_TO(value))
#define SYNCVAR_EMPTY_INITIALIZE_TO(value)        ((syncvar_t)SYNCVAR_STATIC_EMPTY_INITIALIZE_TO(value))

#define INT64TOINT60(x)       ((uint64_t)((x) & (uint64_t)0xfffffffffffffffULL))
#define INT60TOINT64(x)       ((int64_t)(((x) & (uint64_t)0x800000000000000ULL) ? ((x) | (uint64_t)0xf800000000000000ULL) : (x)))
#define DBL64TODBL60(in, out) do { memcpy(&(out), &(in), 8); out >>= 4; } while (0)
#define DBL60TODBL64(in, out) do { in <<= 4; memcpy(&(out), &(in), 8); } while(0)

typedef unsigned short qthread_shepherd_id_t;
typedef unsigned short qthread_worker_id_t;


/* for convenient arguments to qthread_fork */
typedef aligned_t (*qthread_f)(void *arg);
typedef void      (*qthread_agg_f)(int count, qthread_f *f, void **arg, void **ret, uint16_t flags);

/* use this function to initialize the qthreads environment before spawning any
 * qthreads. The argument to this function used to specify the number of
 * pthreads that will be spawned to shepherd the qthreads. This number is now
 * ignored, the qthread_init() function is deprecated, and qthread_initialize()
 * now takes its place. If you MUST specify the number of shepherds, use the
 * environment variable QTHREAD_NUM_SHEPHERDS. */
int qthread_init(qthread_shepherd_id_t nshepherds);
int qthread_initialize(void);
int qthread_initialize_agg(int                                               (*agg_cost)(int count,
                                                                  qthread_f *f,
                                                                  void     **arg),
                           qthread_agg_f                                     agg_f,
                           int                                               max_cost);
int qthread_default_agg_cost (int        count,
                              qthread_f *f,
                              void     **arg);
void qthread_default_agg_f (int        count,
                            qthread_f *f,
                            void     **arg,
                            void     **ret,
                            uint16_t   flags);
void qthread_call_method(qthread_f f,
                         void     *arg,
                         void     *ret,
                         uint16_t  flags);

/* use this function to clean up the qthreads environment after execution of
 * the program is finished. This function will terminate any currently running
 * qthreads, so only use it when you are certain that execution has completed.
 * This function is automatically called when the program exits, so only use if
 * reclaiming resources from the library is necessary before the program exits.
 */
void qthread_finalize(void);

/* use this function to tell a shepherd to stop accepting new threads and to
 * offload its existing threads to nearby shepherds. This latter may not take
 * effect immediately, but may only take effect when the current executing
 * qthread on that shepherd next stops executing */
int  qthread_disable_shepherd(qthread_shepherd_id_t shep);
void qthread_enable_shepherd(qthread_shepherd_id_t shep);
/* add calls to allow workers in addition to shepherds to be disabled and renumber */
int  qthread_disable_worker(qthread_worker_id_t worker);
void qthread_enable_worker(qthread_worker_id_t worker);

/* this function allows a qthread to specifically give up control of the
 * processor even though it has not blocked. This is useful for things like
 * busy-waits or cooperative multitasking. Without this function, threads will
 * only ever allow other threads assigned to the same pthread to execute when
 * they block. */
#define qthread_yield()      do { COMPILER_FENCE; qthread_yield_(0); } while (0)
#define qthread_yield_near() do { COMPILER_FENCE; qthread_yield_(1); } while (0)
void qthread_yield_(int);

/* this function flushes the spawncache */
void qthread_flushsc(void);

/* these are the functions for generating a new qthread.
 *
 * Using qthread_fork() and variants:
 *
 *     The specified function will be run to completion. You can detect that a
 *     thread has finished by specifying a location to store the return value
 *     (which will be stored with a qthread_writeF call). The qthread_fork_to
 *     function spawns the thread to a specific shepherd.
 */
int qthread_fork(qthread_f   f,
                 const void *arg,
                 aligned_t  *ret);

int qthread_fork_net(qthread_f   f,
                 const void *arg,
                 aligned_t  *ret);

int qthread_fork_precond(qthread_f   f,
                         const void *arg,
                         aligned_t  *ret,
                         int         npreconds,
                         ...);
int qthread_fork_syncvar(qthread_f   f,
                         const void *arg,
                         syncvar_t  *ret);
int qthread_fork_to(qthread_f             f,
                    const void           *arg,
                    aligned_t            *ret,
                    qthread_shepherd_id_t shepherd);

#ifdef QTHREAD_LOCAL_PRIORITY
int qthread_fork_to_local_priority(qthread_f             f,
                                   const void           *arg,
                                   aligned_t            *ret,
                                   qthread_shepherd_id_t shepherd);
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */

int qthread_fork_precond_to(qthread_f             f,
                            const void           *arg,
                            aligned_t            *ret,
                            qthread_shepherd_id_t shepherd,
                            int                   npreconds,
                            ...);
int qthread_fork_syncvar_to(qthread_f             f,
                            const void           *arg,
                            syncvar_t            *ret,
                            qthread_shepherd_id_t shepherd);
int qthread_fork_copyargs(qthread_f   f,
                          const void *arg,
                          size_t      arg_size,
                          aligned_t  *ret);
int qthread_fork_copyargs_to(qthread_f             f,
                             const void           *arg,
                             size_t                arg_size,
                             syncvar_t            *ret,
                             qthread_shepherd_id_t preferred_shep);
int qthread_fork_syncvar_copyargs(qthread_f   f,
                                  const void *arg,
                                  size_t      arg_size,
                                  syncvar_t  *ret);
int qthread_fork_syncvar_copyargs_simple(qthread_f   f,
                                         const void *arg,
                                         size_t      arg_size,
                                         syncvar_t  *ret);
int qthread_fork_copyargs_precond(qthread_f   f,
                                  const void *arg,
                                  size_t      arg_size,
                                  syncvar_t  *ret,
                                  int         npreconds,
                                  ...);
int qthread_fork_precond_simple(qthread_f   f,
                                const void *arg,
                                aligned_t  *ret,
                                int         npreconds,
                                ...);

enum _qthread_features {
    SPAWN_PARENT,
    SPAWN_SIMPLE,
    SPAWN_NEW_TEAM,
    SPAWN_NEW_SUBTEAM,
    SPAWN_RET_SYNCVAR_T,
    SPAWN_RET_SINC,
    SPAWN_RET_SINC_VOID,
    SPAWN_PC_SYNCVAR_T,
    SPAWN_AGGREGABLE,
    SPAWN_COUNT,
    SPAWN_LOCAL_PRIORITY,
    SPAWN_NETWORK
};

#define QTHREAD_SPAWN_PARENT        (1 << SPAWN_PARENT)
#define QTHREAD_SPAWN_SIMPLE        (1 << SPAWN_SIMPLE)
#define QTHREAD_SPAWN_NEW_TEAM      (1 << SPAWN_NEW_TEAM)
#define QTHREAD_SPAWN_NEW_SUBTEAM   (1 << SPAWN_NEW_SUBTEAM)
#define QTHREAD_SPAWN_RET_SYNCVAR_T (1 << SPAWN_RET_SYNCVAR_T)
#define QTHREAD_SPAWN_RET_SINC      (1 << SPAWN_RET_SINC)
#define QTHREAD_SPAWN_RET_SINC_VOID (1 << SPAWN_RET_SINC_VOID)
#define QTHREAD_SPAWN_PC_SYNCVAR_T  (1 << SPAWN_PC_SYNCVAR_T)
#define QTHREAD_SPAWN_AGGREGABLE    (1 << SPAWN_AGGREGABLE)
#define QTHREAD_SPAWN_LOCAL_PRIORITY (1 << SPAWN_LOCAL_PRIORITY)
#define QTHREAD_SPAWN_NETWORK (1 << SPAWN_NETWORK)

void qthread_chpl_reset_spawn_order(void);

int qthread_spawn(qthread_f             f,
                  const void           *arg,
                  size_t                arg_size,
                  void                 *ret,
                  size_t                npreconds,
                  void                 *preconds,
                  qthread_shepherd_id_t target_shep,
                  unsigned int          feature_flag);

/* This is a function to move a thread from one shepherd to another. */
int qthread_migrate_to(const qthread_shepherd_id_t shepherd);

/* This function sets the debug level if debugging has been enabled */
int qthread_debuglevel(int);

/* these are accessor functions for use by the qthreads to retrieve information
 * about themselves */
#define QTHREAD_NULL_TASK_ID    UINT_MAX
#define QTHREAD_NON_TASK_ID     0
#define QTHREAD_DEFAULT_TEAM_ID 1
#define QTHREAD_NON_TEAM_ID     0
#define QTHREAD_NULL_TEAM_ID    UINT_MAX
unsigned              qthread_id(void);
qthread_shepherd_id_t qthread_shep(void);
qthread_worker_id_t   qthread_worker(qthread_shepherd_id_t *s);
qthread_worker_id_t   qthread_worker_unique(qthread_shepherd_id_t *s);
qthread_worker_id_t   qthread_worker_local(qthread_shepherd_id_t *s);

void *   qthread_get_tasklocal(unsigned int);
unsigned qthread_size_tasklocal(void);


void* qthread_tos(void);
void* qthread_bos(void);

size_t     qthread_stackleft(void);
aligned_t *qthread_retloc(void);
int        qthread_shep_ok(void);
void       qthread_shep_next(qthread_shepherd_id_t *shep);
void       qthread_shep_prev(qthread_shepherd_id_t *shep);
void       qthread_shep_next_local(qthread_shepherd_id_t *shep);
void       qthread_shep_prev_local(qthread_shepherd_id_t *shep);

/* returns the distance from one shepherd to another */
int qthread_distance(const qthread_shepherd_id_t src,
                     const qthread_shepherd_id_t dest);
/* returns a list of shepherds, sorted by their distance from either this
 * qthread or the specified shepherd */
const qthread_shepherd_id_t *qthread_sorted_sheps(void);
const qthread_shepherd_id_t *qthread_sorted_sheps_remote(const
                                                         qthread_shepherd_id_t
                                                         src);
/* returns the number of actively-scheduling shepherds */
qthread_shepherd_id_t qthread_num_shepherds(void);
qthread_worker_id_t   qthread_num_workers(void); /* how many kernel-level threads are running */
qthread_worker_id_t   qthread_num_workers_local(qthread_shepherd_id_t shepherd_id);
/* queries the current state */
enum introspective_state {
    STACK_SIZE,
    RUNTIME_DATA_SIZE,
    BUSYNESS,
    NODE_BUSYNESS,
    ACTIVE_SHEPHERDS,
    TOTAL_SHEPHERDS,
    ACTIVE_WORKERS,
    TOTAL_WORKERS,
    WORKER_OCCUPATION,
    CURRENT_SHEPHERD,
    CURRENT_WORKER,
    CURRENT_UNIQUE_WORKER,
    CURRENT_TEAM,
    PARENT_TEAM
};
size_t qthread_readstate(const enum introspective_state type);

/* Task team interface. */
typedef enum qt_team_critical_section_e {
    BEGIN,
    END
} qt_team_critical_section_t;

unsigned int qt_team_id(void);
unsigned int qt_team_parent_id(void);
void         qt_team_critical_section(qt_team_critical_section_t boundary);
#ifdef QTHREAD_USE_EUREKAS
void         qt_team_eureka(void);
#endif /* QTHREAD_USE_EUREKAS */

#define qthread_fork_new_team(f, a, r) \
    qthread_spawn((f), (a), 0, (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_TEAM)
#define qthread_fork_new_subteam(f, a, r) \
    qthread_spawn((f), (a), 0, (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_SUBTEAM)
#define qthread_fork_new_team_to(f, a, r, p) \
    qthread_spawn((f), (a), 0, (r), 0, NULL, (p), QTHREAD_SPAWN_NEW_TEAM)
#define qthread_fork_syncvar_new_team(f, a, r) \
    qthread_spawn((f), (a), 0, (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_TEAM | QTHREAD_SPAWN_RET_SYNCVAR_T)
#define qthread_fork_syncvar_new_subteam(f, a, r) \
    qthread_spawn((f), (a), 0, (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_SUBTEAM | QTHREAD_SPAWN_RET_SYNCVAR_T)
#define qthread_fork_copyargs_new_team(f, a, z, r) \
    qthread_spawn((f), (a), (z), (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_TEAM)
#define qthread_fork_copyargs_new_subteam(f, a, z, r) \
    qthread_spawn((f), (a), (z), (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_SUBTEAM)
#define qthread_fork_syncvar_copyargs_to(f, a, z, r, p) \
    qthread_spawn((f), (a), (z), (r), 0, NULL, (p), QTHREAD_SPAWN_RET_SYNCVAR_T)

struct qthread_queue_s;
typedef struct qthread_queue_s *qthread_queue_t;

#define QTHREAD_QUEUE_NO_SYNCH          (1 << 0)
#define QTHREAD_QUEUE_MULTI_JOIN        (1 << 1)
#define QTHREAD_QUEUE_MULTI_JOIN_LENGTH (1 << 2)
#define QTHREAD_QUEUE_CAPPED            (1 << 3)

qthread_queue_t qthread_queue_create(uint8_t   flags,
                                     aligned_t length);
int       qthread_queue_join(qthread_queue_t q);
aligned_t qthread_queue_length(qthread_queue_t q);
int       qthread_queue_release_one(qthread_queue_t q);
int       qthread_queue_release_all(qthread_queue_t q);
int       qthread_queue_destroy(qthread_queue_t q);

/****************************************************************************
 * functions to implement FEB locking/unlocking
 ****************************************************************************
 *
 * These are the FEB functions. All but empty/fill have the potential of
 * blocking until the corresponding precondition is met. All FEB
 * blocking/reading/writing is done on a machine-word basis. Memory is assumed
 * to be full unless otherwise asserted, and as such memory that is full and
 * does not have dependencies (i.e. no threads are waiting for it to become
 * empty) does not require state data to be stored. It is expected that while
 * there may be locks instantiated at one time or another for a very large
 * number of addresses in the system, relatively few will be in a non-default
 * (full, no waiters) state at any one time.
 */

/* This function is just to assist with debugging; it returns 1 if the address
 * is full, and 0 if the address is empty */
int qthread_feb_status(const aligned_t *addr);
int qthread_syncvar_status(syncvar_t *const v);

/* The empty/fill functions merely assert the empty or full state of the given
 * address. */
int qthread_empty(const aligned_t *dest);
int qthread_syncvar_empty(syncvar_t *restrict dest);
int qthread_fill(const aligned_t *dest);
int qthread_syncvar_fill(syncvar_t *restrict dest);

/* These functions wait for memory to become empty, and then fill it. When
 * memory becomes empty, only one thread blocked like this will be awoken. Data
 * is read from src and written to dest.
 *
 * The semantics of writeEF are:
 * 1 - destination's FEB state must be "empty"
 * 2 - data is copied from src to destination
 * 3 - the destination's FEB state gets changed from empty to full
 */
int qthread_writeEF(aligned_t *restrict       dest,
                    const aligned_t *restrict src);
int qthread_writeEF_const(aligned_t      *dest,
                          const aligned_t src);
int qthread_syncvar_writeEF(syncvar_t *restrict      dest,
                            const uint64_t *restrict src);
int qthread_syncvar_writeEF_const(syncvar_t *restrict dest,
                                  uint64_t            src);

/* This function is a cross between qthread_fill() and qthread_writeEF(). It
 * does not wait for memory to become empty, but performs the write and sets
 * the state to full atomically with respect to other FEB-based actions. Data
 * is read from src and written to dest.
 *
 * The semantics of writeF are:
 * 1 - data is copied from src to destination
 * 2 - the destination's FEB state gets set to full
 */
int qthread_writeF(aligned_t *restrict       dest,
                   const aligned_t *restrict src);
int qthread_writeF_const(aligned_t *dest,
                         aligned_t  src);
int qthread_syncvar_writeF(syncvar_t *restrict      dest,
                           const uint64_t *restrict src);
int qthread_syncvar_writeF_const(syncvar_t *restrict dest,
                                 uint64_t            src);

/* This function is essentially qthread_empty, but it also writes 0. It does
 * not wait for memory to become empty, but performs the write and sets the
 * state to empty.
 *
 * The semantics of purge are:
 * 1 - the destination's value is set to 0
 * 2 - the destination's FEB state gets set to empty
 */
int qthread_purge(aligned_t *dest);

// Intended only for Chapel where 0 might not be the default value for a type
int qthread_purge_to(aligned_t *restrict       dest,
                     const aligned_t *restrict src);
int qthread_purge_to_const(aligned_t *dest,
                     aligned_t  purgeVal);

// NOTE: There is no syncvar version of purge

/* This function waits for memory to become full, and leaves it full. When
 * memory becomes full, all threads waiting for it to become full with a
 * writeFF will write their value and be queued to run. Data is read from src
 * and written to dest.

 *
 * The semantics of writeFF are:
 * 1 - destination's FEB state must be "full"
 * 2 - data is copied from src to destination
 */
int qthread_writeFF(aligned_t *restrict       dest,
                    const aligned_t *restrict src);
int qthread_writeFF_const(aligned_t *dest,
                          aligned_t  src);
// NOTE: There is no syncvar version of writeFF or writeFF_const

/* This function waits for memory to become full, and then reads it and leaves
 * the memory as full. When memory becomes full, all threads waiting for it to
 * become full with a readFF will receive the value at once and will be queued
 * to run. Data is read from src and stored in dest.
 *
 * The semantics of readFF are:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 */
int qthread_readFF(aligned_t       *dest,
                   const aligned_t *src);
int qthread_syncvar_readFF(uint64_t *restrict  dest,
                           syncvar_t *restrict src);

/* These functions wait for memory to become full, and then empty it. When
 * memory becomes full, only one thread blocked like this will be awoken. Data
 * is read from src and written to dest.
 *
 * The semantics of readFE are:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 * 3 - the src's FEB bits get changed from full to empty when the data is copied
 */
int qthread_readFE(aligned_t       *dest,
                   const aligned_t *src);
int qthread_syncvar_readFE(uint64_t *restrict  dest,
                           syncvar_t *restrict src);

/* This function ignores the FEB state. Data is read from src and written to
 * dest.
 *
 * The semantics of readXX are:
 * 1 - src's FEB state is ignored
 * 2 - data is copied from src to destination
 */
int qthread_readXX(aligned_t       *dest,
                   const aligned_t *src);
// NOTE: There is no syncvar version of readXX

/* functions to implement FEB-ish locking/unlocking
 *
 * These are atomic and functional, but do not have the same semantics as full
 * FEB locking/unlocking (namely, unlocking cannot block), however because of
 * this, they have lower overhead.
 */
int qthread_lock(const aligned_t *a);
int qthread_unlock(const aligned_t *a);

#if defined(QTHREAD_MUTEX_INCREMENT) ||             \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
uint32_t qthread_incr32_(uint32_t *,
                         int32_t);
uint64_t qthread_incr64_(uint64_t *,
                         int64_t);
float qthread_fincr_(float *,
                     float);
double qthread_dincr_(double *,
                      double);
uint32_t qthread_cas32_(uint32_t *,
                        uint32_t,
                        uint32_t);
uint64_t qthread_cas64_(uint64_t *,
                        uint64_t,
                        uint64_t);
#endif  // if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)

/* the following three functions implement variations on atomic increment. It
 * is done with architecture-specific assembly (on supported architectures,
 * when possible) and does NOT use FEB's or lock/unlock unless the architecture
 * is unsupported or cannot perform atomic operations at the right granularity.
 * All of these functions return the value of the contents of the operand
 * *after* incrementing.
 */
static QINLINE float qthread_fincr(float *operand,
                                   float  incr)
{                                      /*{{{ */
#if defined(QTHREAD_MUTEX_INCREMENT)
    return qthread_fincr_(operand, incr);

#elif QTHREAD_ATOMIC_CAS && \
    (!defined(HAVE_GCC_INLINE_ASSEMBLY) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEGX))
    union {
        float    f;
        uint32_t i;
    } oldval, newval, res;

    do {
        oldval.f = *(volatile float *)operand;
        newval.f = oldval.f + incr;
        res.i    = __sync_val_compare_and_swap((uint32_t *)operand, oldval.i, newval.i);
    } while (res.i != oldval.i);       /* if res!=old, the calc is out of date */
    return oldval.f;

#elif !defined(HAVE_GCC_INLINE_ASSEMBLY)
# error Qthreads requires either mutex increments, inline assembly, or compiler CAS builtins
#else  // if defined(QTHREAD_MUTEX_INCREMENT)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    union {
        float    f;
        uint32_t i;
    } retval;
    float    incremented_value;
    uint32_t scratch_int;
    uint32_t          conversion_memory = conversion_memory;
    __asm__ __volatile__ ("A_%=:\n\t"
                          "lwarx  %0,0,%4\n\t"
                          // convert from int to float
                          "stw    %0,%2\n\t"
                          "lfs    %1,%2\n\t"
                          // do the addition
                          "fadds  %1,%1,%5\n\t"
                          // convert from float to int
                          "stfs   %1,%2\n\t"
                          "lwz    %3,%2\n\t"
                          // store back to original location
                          "stwcx. %3,0,%4\n\t"
                          "bne-   A_%=\n\t"
                          "isync"
                          : "=&b" (retval.i),        /* %0 */
                          "=&f" (incremented_value), /* %1 */
                          "=m"  (conversion_memory), /* %2 */
                          "=&r" (scratch_int)        /* %3 */
                          : "r"   (operand),         /* %4 */
                          "f"   (incr)               /* %5 */
                          : "cc", "memory");

    return retval.f;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    union {
        float    f;
        uint32_t i;
    } oldval, newval;

    /* newval.f = *operand; */
    do {
        /* you *should* be able to move the *operand reference outside the
        * loop and use the output of the CAS (namely, newval) instead.
        * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
        * that, the while() comparison uses a temporary register value for
        * newval that has nothing to do with the output of the CAS
        * instruction. (See how obviously wrong that is?) For some reason that
        * I haven't been able to figure out, moving the *operand reference
        * inside the loop fixes that problem, even at -O2 optimization. */
        oldval.f = *(volatile float *)operand;
        newval.f = oldval.f + incr;
        __asm__ __volatile__
        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
         "cas [%1], %2, %0"
         : "+r" (newval.i)
         : "r"    (operand), "r" (oldval.i)
         : "cc", "memory");
    } while (oldval.i != newval.i);
    return oldval.f;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    union {
        float    f;
        uint32_t i;
    } oldval, newval, res;

    do {
        oldval.f = *(volatile float *)operand;
        newval.f = oldval.f + incr;
        __asm__ __volatile__ ("mov ar.ccv=%0;;" ::"rO" (oldval.i));
        __asm__ __volatile__ ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
                              : "=r" (res.i)
                              : "r"    (operand), "r" (newval.i)
                              : "memory");
    } while (res.i != oldval.i);       /* if res!=old, the calc is out of date */
    return oldval.f;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union {
        float    f;
        uint32_t i;
    } oldval, newval, retval;

    do {
        oldval.f = *(volatile float *)operand;
        newval.f = oldval.f + incr;
        __asm__ __volatile__ ("lock; cmpxchg %1, (%2)"
                              : "=a" (retval.i) /* store from EAX */
                              : "r"    (newval.i),
                              "r" (operand),
                              "0" (oldval.i)     /* load into EAX */
                              : "cc", "memory");
    } while (retval.i != oldval.i);
    return oldval.f;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM)
    union {
        float    f;
        uint32_t i;
    } oldval, newval, retval;
    uint32_t scratch_int = scratch_int;

    retval.f = *(volatile float *)operand;
    do {
        oldval.f = retval.f;
        newval.f = oldval.f + incr;
        __asm__ __volatile__ ("dmb sy\n\t" // memory barrier
                              "A_%=:\n\t"
                              "ldrex %4, [%1]\n\t"
                              "cmp   %4, %2\n\t"
                              "bne   B_%=\n\t"
                              "strex %0, %3, [%1]\n\t"
                              "teq   %0, #0\n\t"
                              "bne   A_%=\n\t"
                              "mov   %0, %2\n\t"
                              "B_%=:\n\t"
                              "dmb sy\n\t"
                              : "=%r" (retval.i)  /* 0 */
                              : "r" (operand),    /* 1 */
                              "r" (oldval.i),     /* 2 */
                              "r" (newval.i),     /* 3 */
                              "r" (scratch_int)   /* 4 */
                              : "cc", "memory");
    } while (retval.i != oldval.i);
    return oldval.i;

# else  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#  error Unsupported assembly architecture for qthread_fincr
# endif  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#endif   // if defined(QTHREAD_MUTEX_INCREMENT)
}                                      /*}}} */

static QINLINE double qthread_dincr(double *operand,
                                    double  incr)
{                                      /*{{{ */
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    return qthread_dincr_(operand, incr);

#elif QTHREAD_ATOMIC_CAS &&                      \
    (!defined(HAVE_GCC_INLINE_ASSEMBLY) ||       \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEGX) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32))
    union {
        uint64_t i;
        double   d;
    } oldval, newval, res;

    do {
        oldval.d = *(volatile double *)operand;
        newval.d = oldval.d + incr;
        res.i    = __sync_val_compare_and_swap((uint64_t *)operand, oldval.i, newval.i);
    } while (res.i != oldval.i);       /* if res!=old, the calc is out of date */
    return oldval.d;

#elif !defined(HAVE_GCC_INLINE_ASSEMBLY)
# error Qthreads requires either mutex increments, inline assembly, or compiler CAS builtins
#else  // if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint64_t scratch_int;
    double   incremented_value;
    union {
        uint64_t i;
        double   d;
    } retval;
    uint64_t conversion_memory = conversion_memory;
    __asm__ __volatile__ ("A_%=:\n\t"
                          "ldarx %0,0,%4\n\t"
                                               /*convert from integer to floating point */
                          "std   %0,%2\n\t"    // %2 is scratch memory (NOT a register)
                          "lfd   %1,%2\n\t"    // %1 is a scratch floating point register
                                               /* do the addition */
                          "fadd  %1,%1,%5\n\t" // %4 is the input increment
                                               /* convert from floating point to integer */
                          "stfd   %1,%2\n\t"
                          "ld     %3,%2\n\t"
                          /* store back to original location */
                          "stdcx. %3,0,%4\n\t"
                          "bne-   A_%=\n\t"
                          "isync"
                          : "=&b" (retval.i),           /* %0 */
                          "=&f" (incremented_value),    /* %1 */
                          "=m"  (conversion_memory),    /* %2 */
                          "=r&" (scratch_int)           /* %3 */
                          : "r"   (operand),            /* %4 */
                          "f"   (incr)                  /* %5 */
                          : "cc", "memory");

    return retval.d;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    union {
        uint64_t i;
        double   d;
    } oldval, newval;

    newval.d = *(volatile double *)operand;
    do {
        /* this allows the compiler to be as flexible as possible with register
         * assignments */
        uint64_t tmp1;
        uint64_t tmp2;

        oldval.d  = newval.d;
        newval.d += incr;
        __asm__ __volatile__ ("ldd %0, %1\n\t"
                              "ldx %4, %2\n\t"
                              "membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
                              "sllx %1, 0x20, %1\n\t"
                              "sllx %2, 0x20, %2\n\t"
                              "casx [%3], %2, %1\n\t"
                              "srlx %1, 0x20, %1\n\t"
                              "std %1, %0"
                              /* h means 64-BIT REGISTER
                               * (probably unnecessary, but why take chances?) */
                              : "=m"   (newval.i), "=h" (tmp1), "=h" (tmp2)
                              : "r"    (operand), "m" (oldval.i)
                              : "memory");
    } while (oldval.i != newval.i);
    return oldval.d;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    union {
        uint64_t i;
        double   d;
    } oldval, newval;

    /*newval.d = *operand; */
    do {
        /* you *should* be able to move the *operand reference outside the
        * loop and use the output of the CAS (namely, newval) instead.
        * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
        * that, the while() comparison uses a temporary register value for
        * newval that has nothing to do with the output of the CAS
        * instruction. (See how obviously wrong that is?) For some reason that
        * I haven't been able to figure out, moving the *operand reference
        * inside the loop fixes that problem, even at -O2 optimization. */
        oldval.d = *(volatile double *)operand;
        newval.d = oldval.d + incr;
        __asm__ __volatile__
        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
         "casx [%1], %2, %0"
         : "+r" (newval.i)
         : "r" (operand), "r" (oldval.i)
         : "memory");
    } while (oldval.d != newval.d);
    return oldval.d;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    union {
        uint64_t i;
        double   d;
    } oldval, newval, res;

    do {
        oldval.d = *(volatile double *)operand;
        newval.d = oldval.d + incr;
        __asm__ __volatile__ ("mov ar.ccv=%0;;" ::"rO" (oldval.i));
        __asm__ __volatile__ ("cmpxchg8.acq %0=[%1],%2,ar.ccv"
                              : "=r" (res.i)
                              : "r"    (operand), "r" (newval.i)
                              : "memory");
    } while (res.i != oldval.i);       /* if res!=old, the calc is out of date */
    return oldval.d;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    union {
        double   d;
        uint64_t i;
    } oldval, newval, retval;

    do {
        /* This volatile cast APPEARS to fix a bug. What happened is that in
         * some situations, without the volatile cast, the compiler allowed
         * itself to load *operand TWICE (once to a floating point register,
         * once to an integer register) for some bizarre reason. That, of
         * course, is a race condition (the value could change between loads).
         * That said, there is no guarantee that the use of volatile *fixes*
         * the issue, since compilers are technically allowed to ignore the
         * keyword. That said, I can't think of a better solution (I could load
         * it in assembly myself, but I don't want to pick a floating point
         * unit to use). */
        oldval.d = *(volatile double *)operand;
        newval.d = oldval.d + incr;
#  ifdef __PGI
        __asm__ __volatile__ ("lock; cmpxchgq %1, (%2)\n\t"
                              "mov %%rax,(%0)"
                              ::"r" (&retval.i),
                              "r" (newval.i), "r" (operand),
                              "a" (oldval.i)
                              : "memory");
#  else
        __asm__ __volatile__ ("lock; cmpxchgq %1, (%2)"
                              : "=a" (retval.i)
                              : "r" (newval.i), "r" (operand),
                              "0" (oldval.i)
                              : "memory");
#  endif  // ifdef __PGI
    } while (retval.i != oldval.i);
    return oldval.d;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union {
        double   d;
        uint64_t i;
        struct {
            /* note: the ordering of these is both important and
             * counter-intuitive; welcome to little-endian! */
            uint32_t l;
            uint32_t h;
        } s;
    } oldval, newval;
    char test;

    do {
#  ifdef __PIC__
        /* this saves off %ebx to make PIC code happy :P */
#   define QTHREAD_PIC_PREFIX "xchg %%ebx, %4\n\t"
        /* this restores it */
#   define QTHREAD_PIC_SUFFIX "\n\txchg %%ebx, %4"
#   define QTHREAD_PIC_REG_4  "r"
#  else
#   define QTHREAD_PIC_PREFIX
#   define QTHREAD_PIC_SUFFIX
#   define QTHREAD_PIC_REG_4 "b"
#  endif
        oldval.d = *(volatile double *)operand;
        newval.d = oldval.d + incr;
        /* Yeah, this is weird looking, but it really makes sense when you
         * understand the instruction's semantics (which make sense when you
         * consider that it's doing a 64-bit op on a 32-bit proc):
         *
         *    Compares the 64-bit value in EDX:EAX with the operand
         *    (destination operand). If the values are equal, the 64-bit value
         *    in ECX:EBX is stored in the destination operand. Otherwise, the
         *    value in the destination operand is loaded into EDX:EAX."
         *
         * So what happens is the oldval is loaded into EDX:EAX and the newval
         * is loaded into ECX:EBX to start with (i.e. as inputs). Then
         * CMPXCHG8B does its business, after which EDX:EAX is guaranteed to
         * contain the value of *operand when the instruction executed. We test
         * the ZF field to see if the operation succeeded. We *COULD* save
         * EDX:EAX back into oldval to save ourselves a step when the loop
         * fails, but that's a waste when the loop succeeds (i.e. in the common
         * case). Optimizing for the common case, in this situation, means
         * minimizing our extra write-out to the one-byte test variable.
         */
        __asm__ __volatile__ (QTHREAD_PIC_PREFIX
                              "lock; cmpxchg8b (%1)\n\t"
                              "setne %0" /* test = (ZF==0) */
                              QTHREAD_PIC_SUFFIX
                              : "=q" (test)
                              : "r" (operand),
                              /*EAX*/ "a" (oldval.s.l),
                              /*EDX*/ "d" (oldval.s.h),
                              /*EBX*/ QTHREAD_PIC_REG_4(newval.s.l),
                              /*ECX*/ "c" (newval.s.h)
                              : "memory");
    } while (test);                    /* if ZF was cleared, the calculation is out of date */
    return oldval.d;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM)
    union {
        double   f;
        uint64_t i;
        struct {
            uint32_t t, b;
        } s;
    } oldval, newval, retval;
    uint32_t tmp = tmp;

    retval.f = *(volatile double *)operand;
    do {
        oldval.f = retval.f;
        newval.f = oldval.f + incr;
        __asm__ __volatile__ ("dmb sy\n\t"
                              "A_%=:\n\t"
                              "ldrexd %0, %1, [%2]\n\t"
                              "cmp    %0, %3\n\t"
                              "it     eq\n\t"
                              "cmpeq  %1, %4\n\t"
                              "bne    B_%=\n\t"
                              "strexd %7, %5, %6, [%2]\n\t"
                              "teq    %7, #0\n\t"
                              "bne    A_%=\n\t"
                              "mov    %0, %3\n\t"
                              "mov    %1, %4\n\t"
                              "B_%=:\n\t"
                              "dmb sy\n\t"
                              : "=&r" (retval.s.t), /* 0 */
                              "=&r" (retval.s.b)    /* 1 */
                              : "r" (operand),      /* 2 */
                              "r" (oldval.s.t),     /* 3 */
                              "r" (oldval.s.b),     /* 4 */
                              "r" (newval.s.t),     /* 5 */
                              "r" (newval.s.b),     /* 6 */
                              "r" (tmp)             /* 7 */
                              : "cc", "memory");
    } while (oldval.i != retval.i);
    return oldval.f;

# else  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#  error Unimplemented assembly architecture for qthread_dincr
# endif  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#endif   // if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
}                                      /*}}} */

static QINLINE uint32_t qthread_incr32(uint32_t *operand,
                                       uint32_t  incr)
{                                      /*{{{ */
#ifdef QTHREAD_MUTEX_INCREMENT
    return qthread_incr32_(operand, incr);

#elif defined(QTHREAD_ATOMIC_INCR)
    return __sync_fetch_and_add(operand, incr);

#elif !defined(HAVE_GCC_INLINE_ASSEMBLY) && QTHREAD_ATOMIC_CAS
    uint32_t oldval, newval;
    do {
        oldval = *operand;
        newval = oldval + incr;
        newval = __sync_val_compare_and_swap(operand, oldval, newval);
    } while (oldval != newval);
    return oldval;

#elif !defined(HAVE_GCC_INLINE_ASSEMBLY)
# error Qthreads requires either mutex increments, inline assembly, or compiler atomic builtins
#else  // ifdef QTHREAD_MUTEX_INCREMENT
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint32_t              retval;
    unsigned int incrd = incrd;        /* no initializing */
    __asm__ __volatile__ ("A_%=:\tlwarx  %0,0,%2\n\t"
                          "add    %1,%0,%3\n\t"
                          "stwcx. %1,0,%2\n\t"
                          "bne-   A_%=\n\t" /* if it failed, try again */
                          "isync"           /* make sure it wasn't all a dream */
                          : "=&b"  (retval), "=&r" (incrd)
                          : "r"    (operand), "r" (incr)
                          : "cc", "memory");

    return retval;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    uint32_t oldval, newval;

    /* newval = *operand; */
    do {
        /* you *should* be able to move the *operand reference outside the
        * loop and use the output of the CAS (namely, newval) instead.
        * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
        * that, the while() comparison uses a temporary register value for
        * newval that has nothing to do with the output of the CAS
        * instruction. (See how obviously wrong that is?) For some reason that
        * I haven't been able to figure out, moving the *operand reference
        * inside the loop fixes that problem, even at -O2 optimization. */
        oldval = *operand;
        newval = oldval + incr;
        /* newval always gets the value of *operand; if it's
         * the same as oldval, then the swap was successful */
        __asm__ __volatile__
        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
         "cas [%1] , %2, %0"
         : "+r"  (newval)
         : "r"    (operand), "r" (oldval)
         : "cc", "memory");
    } while (oldval != newval);
    return oldval;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    uint32_t res;

    if (incr == 1) {
        asm volatile ("fetchadd4.rel %0=[%1],1"
                      : "=r" (res)
                      : "r"  (operand));
    } else {
        uint32_t old, newval;

        do {
            old    = *operand;         /* atomic, because operand is aligned */
            newval = old + incr;
            asm volatile ("mov ar.ccv=%0;;" :    /* no output */
                          : "rO"    (old));

            /* separate so the compiler can insert its junk */
            asm volatile ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
                          : "=r" (res)
                          : "r" (operand), "r" (newval)
                          : "memory");
        } while (res != old);          /* if res!=old, the calc is out of date */
    }
    return res;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)

    uint32_t retval = incr;
    __asm__ __volatile__ ("lock ;  xaddl %0, (%1);"
                          : "+r" (retval)
                          : "r"  (operand)
                          : "memory");

    return retval;

# else  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#  error Unimplemented assembly architecture for qthread_incr32
# endif  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#endif   // ifdef QTHREAD_MUTEX_INCREMENT
}                                      /*}}} */

static QINLINE uint64_t qthread_incr64(uint64_t *operand,
                                       uint64_t  incr)
{                                      /*{{{ */
#if defined(QTHREAD_MUTEX_INCREMENT) ||             \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    return qthread_incr64_(operand, incr);

#elif defined(QTHREAD_ATOMIC_INCR)
    return __sync_fetch_and_add(operand, incr);

#elif !defined(HAVE_GCC_INLINE_ASSEMBLY) && QTHREAD_ATOMIC_CAS
    uint64_t oldval, newval;
    do {
        oldval = *operand;
        newval = oldval + incr;
        newval = __sync_val_compare_and_swap(operand, oldval, newval);
    } while (oldval != newval);
    return oldval;

#elif !defined(HAVE_GCC_INLINE_ASSEMBLY)
# error Qthreads requires either mutex increments, inline assembly, or compiler atomic builtins
#else  // if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint64_t          retval;
    uint64_t incrd = incrd;    /* no initializing */

    asm volatile ("A_%=:\tldarx  %0,0,%2\n\t"
                  "add    %1,%0,%3\n\t"
                  "stdcx. %1,0,%2\n\t"
                  "bne-   A_%=\n\t" /* if it failed, try again */
                  "isync"           /* make sure it wasn't all a dream */
                  : "=&b"   (retval), "=&r" (incrd)
                  : "r"     (operand), "r" (incr)
                  : "cc", "memory");

    return retval;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    uint64_t oldval, newval = *operand;

    do {
        /* this allows the compiler to be as flexible as possible with register
         * assignments */
        uint64_t tmp1 = tmp1;
        uint64_t tmp2 = tmp2;

        oldval  = newval;
        newval += incr;
        /* newval always gets the value of *operand; if it's
         * the same as oldval, then the swap was successful */
        __asm__ __volatile__ ("ldx %0, %1\n\t"
                              "ldx %4, %2\n\t"
                              "membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
                              "casx [%3] , %2, %1\n\t"
                              "stx %1, %0"
                              /* h means 64-BIT REGISTER
                               * (probably unnecessary, but why take chances?) */
                              : "=m"   (newval), "=&h" (tmp1), "=&h" (tmp2)
                              : "r"    (operand), "m" (oldval)
                              : "cc", "memory");
    } while (oldval != newval);
    return oldval;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    uint64_t oldval, newval;

#  ifdef QTHREAD_ATOMIC_CAS
    newval = *operand;
    do {
        oldval = newval;
        newval = __sync_val_compare_and_swap(operand, oldval, oldval + incr);
    } while (oldval != newval);
#  else
    do {
        /* you *should* be able to move the *operand reference outside the
        * loop and use the output of the CAS (namely, newval) instead.
        * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
        * that, the while() comparison uses a temporary register value for
        * newval that has nothing to do with the output of the CAS
        * instruction. (See how obviously wrong that is?) For some reason that
        * I haven't been able to figure out, moving the *operand reference
        * inside the loop fixes that problem, even at -O2 optimization. */
        oldval = *operand;
        newval = oldval + incr;
        /* newval always gets the value of *operand; if it's
         * the same as oldval, then the swap was successful */
        __asm__ __volatile__
        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
         "casx [%1] , %2, %0"
         : "+r" (newval)
         : "r"    (operand), "r" (oldval)
         : "cc", "memory");
    } while (oldval != newval);
#  endif  // ifdef QTHREAD_ATOMIC_CAS
    return oldval;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    uint64_t res;

    if (incr == 1) {
        asm volatile ("fetchadd8.rel %0=%1,1"
                      : "=r" (res)
                      : "m"     (*operand));
    } else {
        uint64_t old, newval;

        do {
            old    = *operand;         /* atomic, because operand is aligned */
            newval = old + incr;
            asm volatile ("mov ar.ccv=%0;;" :    /* no output */
                          : "rO"    (old));

            /* separate so the compiler can insert its junk */
            asm volatile ("cmpxchg8.acq %0=[%1],%2,ar.ccv"
                          : "=r" (res)
                          : "r"     (operand), "r" (newval)
                          : "memory");
        } while (res != old);          /* if res!=old, the calc is out of date */
    }
    return res;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union {
        uint64_t i;
        struct {
            /* note: the ordering of these is both important and
             * counter-intuitive; welcome to little-endian! */
            uint32_t l;
            uint32_t h;
        } s;
    } oldval, newval;
    char test;

    do {
#  ifndef QTHREAD_PIC_PREFIX
#   ifdef __PIC__
        /* should share this code with the dincr stuff */
        /* this saves off %ebx to make PIC code happy :P */
#    define QTHREAD_PIC_PREFIX "pushl %%ebx\n\tmovl %4, %%ebx\n\t"  /* "xchg %%ebx, %4\n\t"*/
        /* this restores it */
#    define QTHREAD_PIC_SUFFIX "\n\tpopl %%ebx"  /*"\n\txchg %%ebx, %4"*/
#    define QTHREAD_PIC_REG_4  "r"
#   else
#    define QTHREAD_PIC_PREFIX
#    define QTHREAD_PIC_SUFFIX
#    define QTHREAD_PIC_REG_4 "b"
#   endif
#  endif  // ifndef QTHREAD_PIC_PREFIX
        oldval.i = *operand;
        newval.i = oldval.i + incr;
        /* Yeah, this is weird looking, but it really makes sense when you
         * understand the instruction's semantics (which make sense when you
         * consider that it's doing a 64-bit op on a 32-bit proc):
         *
         *    Compares the 64-bit value in EDX:EAX with the operand
         *    (destination operand). If the values are equal, the 64-bit value
         *    in ECX:EBX is stored in the destination operand. Otherwise, the
         *    value in the destination operand is loaded into EDX:EAX."
         *
         * So what happens is the oldval is loaded into EDX:EAX and the newval
         * is loaded into ECX:EBX to start with (i.e. as inputs). Then
         * CMPXCHG8B does its business, after which EDX:EAX is guaranteed to
         * contain the value of *operand when the instruction executed. We test
         * the ZF field to see if the operation succeeded. We *COULD* save
         * EDX:EAX back into oldval to save ourselves a step when the loop
         * fails, but that's a waste when the loop succeeds (i.e. in the common
         * case). Optimizing for the common case, in this situation, means
         * minimizing our extra write-out to the one-byte test variable.
         */
        __asm__ __volatile__ (QTHREAD_PIC_PREFIX
                              "lock; cmpxchg8b (%1)\n\t"
                              "setne %0" /* test = (ZF==0) */
                              QTHREAD_PIC_SUFFIX
                              : "=q" (test)
                              : "r"    (operand),
                              /*EAX*/ "a" (oldval.s.l),
                              /*EDX*/ "d" (oldval.s.h),
                              /*EBX*/ QTHREAD_PIC_REG_4(newval.s.l),
                              /*ECX*/ "c" (newval.s.h)
                              : "memory");
    } while (test);                    /* if ZF was cleared, the calculation is out of date */
    return oldval.i;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    uint64_t retval = incr;

#  ifdef __PGI
    /* this is a workaround for a bug in the PGI compiler where the width of
     * retval is not respected and %eax is used instead of %rax */
    __asm__ __volatile__ ("lock xaddq %0, (%2)\n\t"
                          "mov %0,(%1)"
                          ::"r" (incr),
                          "r" (&retval),
                          "r" (operand)
                          : "memory");
#  else
    __asm__ __volatile__ ("lock ; xaddq %0, (%1);"
                          : "+r" (retval)
                          : "r" (operand)
                          : "memory");
#  endif  // ifdef __PGI

    return retval;

# else  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#  error Unimplemented assembly architecture for qthread_incr64
# endif  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#endif   // if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
}                                      /*}}} */

static QINLINE int64_t qthread_incr_xx(void   *addr,
                                       int64_t incr,
                                       size_t  length)
{                                      /*{{{ */
    switch (length) {
        case 4:
            return qthread_incr32((uint32_t *)addr, incr);

        case 8:
            return qthread_incr64((uint64_t *)addr, incr);

        default:
            QTHREAD_TRAP();
    }
    return 0;                          /* compiler check */
}                                      /*}}} */

uint64_t qthread_syncvar_incrF(syncvar_t *restrict operand,
                               uint64_t            inc);

#if !defined(QTHREAD_ATOMIC_CAS) || defined(QTHREAD_MUTEX_INCREMENT)
static QINLINE uint32_t qthread_cas32(uint32_t *operand,
                                      uint32_t  oldval,
                                      uint32_t  newval)
{                                /*{{{ */
# ifdef QTHREAD_MUTEX_INCREMENT  // XXX: this is only valid if you don't read *operand without the lock
    return qthread_cas32_(operand, oldval, newval);

# else
#  if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint32_t result;
    __asm__ __volatile__ ("A_%=:\n\t"
                          "lwarx  %0,0,%3\n\t"
                          "cmpw   %0,%1\n\t"
                          "bne    B_%=\n\t"
                          "stwcx. %2,0,%3\n\t"
                          "bne-   A_%=\n"
                          "B_%=:"
                          "isync" /* make sure it wasn't all a dream */
                          : "=&b" (result)
                          : "r" (oldval), "r" (newval), "r" (operand)
                          : "cc", "memory");
    return result;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    uint32_t newv = newval;
    __asm__ __volatile__
    ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
     "cas [%1], %2, %0"
     : "+r" (newv)
     : "r" (operand), "r" (oldval)
     : "cc", "memory");
    return newv;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    uint32_t retval;
    __asm__ __volatile__ ("mov ar.ccv=%0;;" : : "rO" (oldval));
    __asm__ __volatile__ ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
                          : "=r" (retval)
                          : "r" (operand), "r" (newval)
                          : "memory");
    return retval;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    uint32_t retval;
    /* note that this is GNU/Linux syntax (aka AT&T syntax), not Intel syntax.
     * Thus, this instruction has the form:
     * [lock] cmpxchg reg, reg/mem
     *                src, dest
     */
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)"
                          : "=&a" (retval) /* store from EAX */
                          : "r" (newval), "r" (operand),
                          "0" (oldval) /* load into EAX */
                          : "cc", "memory");
    return retval;

#  else  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#   error Unimplemented assembly architecture for qthread_cas32
#  endif  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
# endif   // ifdef QTHREAD_MUTEX_INCREMENT
}                                      /*}}} */

static QINLINE uint64_t qthread_cas64(uint64_t *operand,
                                      uint64_t  oldval,
                                      uint64_t  newval)
{                                      /*{{{ */
# ifdef QTHREAD_MUTEX_INCREMENT
    return qthread_cas64_(operand, oldval, newval);

# else
#  if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint64_t result;
    __asm__ __volatile__ ("A_%=:\n\t"
                          "ldarx  %0,0,%3\n\t"
                          "cmpw   %0,%1\n\t"
                          "bne    B_%=\n\t"
                          "stdcx. %2,0,%3\n\t"
                          "bne-   A_%=\n"
                          "B_%=:"
                          "isync" /* make sure it wasn't all a dream */
                          : "=&b" (result)
                          : "r" (oldval), "r" (newval), "r" (operand)
                          : "cc", "memory");
    return result;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    uint64_t tmp1 = tmp1;
    uint64_t tmp2 = tmp2;
    uint64_t          newv = newval;
    __asm__ __volatile__
    ("ldx %0, %1\n\t"
     "ldx %4, %2\n\t"
     "membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
     "casx [%3], %2, %1\n\t"
     "stx %1, %0"
     /* h means 64-BIT REGISTER
      * (probably unneecessary, but why take chances?) */
     : "+m" (newv), "=&h" (tmp1), "=&h" (tmp2)
     : "r" (operand), "m" (oldval)
     : "cc", "memory");
    return newv;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    uint64_t newv = newval;
    __asm__ __volatile__
    ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
     "casx [%1], %2, %0"
     : "+r" (newv)
     : "r" (operand), "r" (oldval)
     : "cc", "memory");
    return newv;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    uint32_t retval;
    __asm__ __volatile__ ("mov ar.ccv=%0;;" : : "rO" (oldval));
    __asm__ __volatile__ ("cmpxchg8.acq %0=[%1],%2,ar.ccv"
                          : "=r" (retval)
                          : "r" (operand), "r" (newval)
                          : "memory");
    return retval;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union {
        uint64_t i;
        struct {
            /* note: the ordering of these is both important and
             * counter-intuitive; welcome to little-endian! */
            uint32_t l;
            uint32_t h;
        } s;
    } oldv, newv, ret;
    oldv.i = oldval;
    newv.i = newval;
#   ifndef QTHREAD_PIC_PREFIX
#    ifdef __PIC__
    /* this saves off %ebx to make PIC code happy :P */
#     define QTHREAD_PIC_PREFIX "xchg %%ebx, %4\n\t"
    /* this restores it */
#     define QTHREAD_PIC_SUFFIX "\n\txchg %%ebx, %4"
#     define QTHREAD_PIC_REG_4  "r"
#    else
#     define QTHREAD_PIC_PREFIX
#     define QTHREAD_PIC_SUFFIX
#     define QTHREAD_PIC_REG_4 "b"
#    endif
#   endif  // ifndef QTHREAD_PIC_PREFIX
    /* the PIC stuff is already defined above */
    __asm__ __volatile__ (
                          QTHREAD_PIC_PREFIX
                          "lock; cmpxchg8b (%2)"
                          QTHREAD_PIC_SUFFIX
                          : /*EAX*/ "=a" (ret.s.l),
                          /*EDX*/ "=d" (ret.s.h)
                          : "r" (operand),
                          /*EAX*/ "a" (oldv.s.l),
                          /*EBX*/ QTHREAD_PIC_REG_4(newv.s.l),
                          /*EDX*/ "d" (oldv.s.h),
                          /*ECX*/ "c" (newv.s.h)
                          : "memory");
    return ret.i;

#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    /* note that this is GNU/Linux syntax (aka AT&T syntax), not Intel syntax.
     * Thus, this instruction has the form:
     * [lock] cmpxchg reg, reg/mem
     *                src, dest
     */
#   ifdef __PGI
    /* this is a workaround for a bug in the PGI compiler where the width of
     * retval is not respected and %eax is used instead of %rax */
    uint64_t retval;
    __asm__ __volatile__ ("lock cmpxchg %1,(%2)\n\t"
                          "mov %%rax,(%0)"
                          ::"r" (&retval), "r" (newval), "r" (operand),
                          "a" (oldval) /* load into RAX */
                          : "cc", "memory");
    return retval;

#   else
    uint64_t retval;
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)"
                          : "=a" (retval) /* store from RAX */
                          : "r" (newval), "r" (operand),
                          "a" (oldval) /* load into RAX */
                          : "cc", "memory");
    return retval;
#   endif  // ifdef __PGI
#  elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    /* In general, RISC doesn't provide a way to do 64 bit operations from 32
     * bit code. Sorry! */
    uint64_t retval;

    qthread_lock((aligned_t *)operand);
    retval = *operand;
    if (retval == oldval) {
        *operand = newval;
    }
    qthread_unlock((aligned_t *)operand);
    return retval;

#  else  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#   error Unimplemented assembly architecture for qthread_cas64
#  endif  // if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
# endif   // ifdef QTHREAD_MUTEX_INCREMENT
}                                      /*}}} */

static QINLINE aligned_t qthread_cas_xx(aligned_t *addr,
                                        aligned_t  oldval,
                                        aligned_t  newval,
                                        size_t     length)
{                                      /*{{{ */
    switch (length) {
        case 4:
            return qthread_cas32((uint32_t *)addr, oldval, newval);

        case 8:
            return qthread_cas64((uint64_t *)addr, oldval, newval);

        default:
            /* This should never happen, so deliberately cause a seg fault
             * for corefile analysis */
            QTHREAD_TRAP();
    }
    return 0;                          /* compiler check */
}                                      /*}}} */

static QINLINE void *qthread_cas_ptr_(void **addr,
                                      void  *oldval,
                                      void  *newval)
{   /*{{{*/
# if (SIZEOF_VOIDP == 4)
    return (void *)(uintptr_t)qthread_cas32((uint32_t *)addr,
                                            (uint32_t)(uintptr_t)oldval,
                                            (uint32_t)(uintptr_t)newval);

# elif (SIZEOF_VOIDP == 8)
    return (void *)(uintptr_t)qthread_cas64((uint64_t *)addr,
                                            (uint64_t)(uintptr_t)oldval,
                                            (uint64_t)(uintptr_t)newval);

# else
#  error The size of void* either could not be determined, or is very unusual.
    /* This should never happen, so deliberately cause a seg fault for
     * corefile analysis */
    QTHREAD_TRAP();
    return NULL;                       /* compiler check */
# endif  // if (SIZEOF_VOIDP == 4)
} /*}}}*/

#endif  /* ifdef QTHREAD_ATOMIC_CAS */

#ifdef QTHREAD_ATOMIC_CAS
# define qthread_cas(ADDR, OLDV, NEWV) \
    __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# define qthread_cas32(ADDR, OLDV, NEWV) \
    __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# define qthread_cas64(ADDR, OLDV, NEWV) \
    __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# ifdef __INTEL_COMPILER
#  define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    (void *)__sync_val_compare_and_swap((ADDR), (uintptr_t)(OLDV), (uintptr_t)(NEWV))
# else
#  define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    (void *)__sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# endif
#else  // ifdef QTHREAD_ATOMIC_CAS
# define qthread_cas(ADDR, OLDV, NEWV) \
    qthread_cas_xx((aligned_t *)(ADDR), (aligned_t)(OLDV), (aligned_t)(NEWV), sizeof(*(ADDR)))
# ifdef QTHREAD_ATOMIC_CAS_PTR
#  define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    (void *)__sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# else
#  define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    qthread_cas_ptr_((void **)(ADDR), (void *)(OLDV), (void *)(NEWV))
# endif
#endif  // ifdef QTHREAD_ATOMIC_CAS

Q_ENDCXX /* */

#ifndef __cplusplus

# if defined(QTHREAD_ATOMIC_INCR) && !defined(QTHREAD_MUTEX_INCREMENT)
#  define qthread_incr(ADDR, INCVAL) \
    __sync_fetch_and_add(ADDR, INCVAL)
# else
#  define qthread_incr(ADDR, INCVAL) \
    qthread_incr_xx((void *)(ADDR), (int64_t)(INCVAL), sizeof(*(ADDR)))
# endif

#else  /* ifdef __cplusplus */
extern "C++" {
# include <qthread/qthread.hpp>
}
#endif  /* __cplusplus */

#endif /* _QTHREAD_H_ */
/* vim:set expandtab: */
