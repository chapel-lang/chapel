#ifndef _QTHREAD_H_
#define _QTHREAD_H_

#include <errno.h> /* for ENOMEM */

#if defined(__cplusplus) && __cplusplus < 202302L
#include <atomic>
#define QT_Atomic(T) std::atomic<T>
using std::atomic_load_explicit;
using std::memory_order_relaxed;
#else
#include <stdatomic.h>
#define QT_Atomic(T) _Atomic(T)
#endif

#include <limits.h> /* for UINT_MAX (C89) */
#include <stdbool.h>
#include <stdint.h>
#include <string.h> /* for memcpy() */

#include "common.h"

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
#define QTHREAD_SUCCESS 0
#define QTHREAD_BADARGS -1
#define QTHREAD_PTHREAD_ERROR -2
#define QTHREAD_NOT_ALLOWED -3
#define QTHREAD_MALLOC_ERROR ENOMEM
#define QTHREAD_THIRD_PARTY_ERROR -4
#define QTHREAD_TIMEOUT                                                        \
  -5 /* neither ETIME nor ETIMEDOUT seem appropriate, strictly speaking */
#ifdef EOVERFLOW
#define QTHREAD_OVERFLOW EOVERFLOW
#else
#define QTHREAD_OVERFLOW -6
#endif
#define QTHREAD_OPFAIL -7
#define NO_SHEPHERD ((qthread_shepherd_id_t) - 1)
#define NO_WORKER ((qthread_worker_id_t) - 1)

#define QTHREAD_RELEASE_TYPE_ALPHA 0
#define QTHREAD_RELEASE_TYPE_BETA 1
#define QTHREAD_RELEASE_TYPE_RC 2
#define QTHREAD_RELEASE_TYPE_PATCH 3

/* QTHREAD_VERSION = [MAJ].[MIN].[REV][EXT][EXT_NUMBER]
 * Example: QTHREAD_VERSION = 1.17.1rc1 represents
 *          MAJ = 1
 *          MIN = 17
 *          REV = 1
 *          EXT = rc
 *          EXT_NUMBER = 1
 */
#define QTHREAD_VERSION "1.23"

/* * QTHREAD_NUMVERSION = [MAJ] * 10000000 + [MIN] * 100000 + [REV] * 1000
 *                      + [EXT] * 100 + [EXT_NUMBER]
 * where [EXT] is converted to the following format number:
 *    ALPHA (a) = 0 (ABT_RELEASE_TYPE_ALPHA)
 *    BETA (b)  = 1 (ABT_RELEASE_TYPE_BETA)
 *    RC (rc)   = 2 (ABT_RELEASE_TYPE_RC)
 *    PATCH (p) = 3 (ABT_RELEASE_TYPE_PATCH)

 *
 * QTHREAD_NUMVERSION has 2 digit for MAJ, 2 digits for MIN, 2 digits for REV, 1
 * digit for EXT, and 2 digits for EXT_NUMBER.  For example, 1.17.1rc1 is
 * converted to the numeric version 011701201.
 */
#define QTHREAD_NUMVERSION 012300300

#define QTHREADS_GET_VERSION(MAJOR, MINOR, REVISION, TYPE, PATCH)              \
  (((MAJOR) * 10000000) + ((MINOR) * 100000) + ((REVISION) * 1000) +           \
   ((TYPE) * 100) + (PATCH))

#include "macros.h"

#define MACHINE_FENCE atomic_thread_fence(memory_order_acq_rel);

#if QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64
#define QTHREAD_SWAPS_IMPLY_ACQ_REL_FENCES
#endif

Q_STARTCXX /* */
/* NOTE!!!!!!!!!!!
 * Reads and writes operate on aligned_t-size segments of memory.
 *
 * FEB locking only works on aligned addresses.
 * On all supported platforms sizeof(aligned_t) == alignof(aligned_t)
 * anyway so we're fine. */

#if UINTPTR_MAX == UINT32_MAX
#define QTHREAD_BITS 32
#elif UINTPTR_MAX == UINT64_MAX
#define QTHREAD_BITS 64
#else
#error Unrecognized bit-ness
#endif

#if QTHREAD_BITS == 32
  typedef uint32_t aligned_t;
typedef uint16_t haligned_t;
typedef int32_t saligned_t;
#elif QTHREAD_BITS == 64
  typedef uint64_t aligned_t;
typedef uint32_t haligned_t;
typedef int64_t saligned_t;
#else
#error "Don't know type for sizeof aligned_t"
#endif

#define QTHREAD_ALIGNMENT_ALIGNED_T (sizeof(aligned_t))

#define QALIGN(d, s) (s) = (d)

typedef struct _syncvar_s {
  union {
    uint64_t w;

    struct {
#ifdef BITFIELD_ORDER_FORWARD
      uint64_t data : 60;
      unsigned state : 3;
      unsigned lock : 1;
#else
      unsigned lock : 1;
      unsigned state : 3;
      uint64_t data : 60;
#endif
    } s;
  } u;
} syncvar_t;

#define SYNCVAR_STATIC_INITIALIZER {{0}}
#define SYNCVAR_STATIC_EMPTY_INITIALIZER                                       \
  {.u.s = {.data = 0, .state = 2, .lock = 0}}
#define SYNCVAR_STATIC_INITIALIZE_TO(value)                                    \
  {.u.s = {.data = value, .state = 0, .lock = 0}}
#define SYNCVAR_STATIC_EMPTY_INITIALIZE_TO(value)                              \
  {.u.s = {.data = value, .state = 2, .lock = 0}}
#define SYNCVAR_INITIALIZER ((syncvar_t)SYNCVAR_STATIC_INITIALIZER)
#define SYNCVAR_EMPTY_INITIALIZER ((syncvar_t)SYNCVAR_STATIC_EMPTY_INITIALIZER)
#define SYNCVAR_INITIALIZE_TO(value)                                           \
  ((syncvar_t)SYNCVAR_STATIC_INITIALIZE_TO(value))
#define SYNCVAR_EMPTY_INITIALIZE_TO(value)                                     \
  ((syncvar_t)SYNCVAR_STATIC_EMPTY_INITIALIZE_TO(value))

#define INT64TOINT60(x) ((uint64_t)((x) & (uint64_t)0xfffffffffffffffULL))
#define INT60TOINT64(x)                                                        \
  ((int64_t)(((x) & (uint64_t)0x800000000000000ULL)                            \
               ? ((x) | (uint64_t)0xf800000000000000ULL)                       \
               : (x)))
#define DBL64TODBL60(in, out)                                                  \
  do {                                                                         \
    memcpy(&(out), &(in), 8);                                                  \
    out >>= 4;                                                                 \
  } while (0)
#define DBL60TODBL64(in, out)                                                  \
  do {                                                                         \
    in <<= 4;                                                                  \
    memcpy(&(out), &(in), 8);                                                  \
  } while (0)

typedef unsigned short qthread_shepherd_id_t;
typedef unsigned short qthread_worker_id_t;

/* for convenient arguments to qthread_fork */
typedef aligned_t (*qthread_f)(void *arg);

/* use this function to initialize the qthreads environment before spawning any
 * qthreads. The argument to this function used to specify the number of
 * pthreads that will be spawned to shepherd the qthreads. This number is now
 * ignored, the qthread_init() function is deprecated, and qthread_initialize()
 * now takes its place. If you MUST specify the number of shepherds, use the
 * environment variable QTHREAD_NUM_SHEPHERDS. */
int qthread_init(qthread_shepherd_id_t nshepherds);
int qthread_initialize(void);
void qthread_call_method(qthread_f f, void *arg, void *ret, uint16_t flags);

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
int qthread_disable_shepherd(qthread_shepherd_id_t shep);
void qthread_enable_shepherd(qthread_shepherd_id_t shep);
/* add calls to allow workers in addition to shepherds to be disabled and
 * renumber */
int qthread_disable_worker(qthread_worker_id_t worker);
void qthread_enable_worker(qthread_worker_id_t worker);

/* this function allows a qthread to specifically give up control of the
 * processor even though it has not blocked. This is useful for things like
 * busy-waits or cooperative multitasking. Without this function, threads will
 * only ever allow other threads assigned to the same pthread to execute when
 * they block. */
#define qthread_yield()                                                        \
  do { qthread_yield_(0); } while (0)
#define qthread_yield_near()                                                   \
  do { qthread_yield_(1); } while (0)
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
int qthread_fork(qthread_f f, void const *arg, aligned_t *ret);

int qthread_fork_net(qthread_f f, void const *arg, aligned_t *ret);

int qthread_fork_precond(
  qthread_f f, void const *arg, aligned_t *ret, int npreconds, ...);
int qthread_fork_syncvar(qthread_f f, void const *arg, syncvar_t *ret);
int qthread_fork_to(qthread_f f,
                    void const *arg,
                    aligned_t *ret,
                    qthread_shepherd_id_t shepherd);

int qthread_fork_precond_to(qthread_f f,
                            void const *arg,
                            aligned_t *ret,
                            qthread_shepherd_id_t shepherd,
                            int npreconds,
                            ...);
int qthread_fork_syncvar_to(qthread_f f,
                            void const *arg,
                            syncvar_t *ret,
                            qthread_shepherd_id_t shepherd);
int qthread_fork_copyargs(qthread_f f,
                          void const *arg,
                          size_t arg_size,
                          aligned_t *ret);
int qthread_fork_copyargs_to(qthread_f f,
                             void const *arg,
                             size_t arg_size,
                             syncvar_t *ret,
                             qthread_shepherd_id_t preferred_shep);
int qthread_fork_syncvar_copyargs(qthread_f f,
                                  void const *arg,
                                  size_t arg_size,
                                  syncvar_t *ret);
int qthread_fork_syncvar_copyargs_simple(qthread_f f,
                                         void const *arg,
                                         size_t arg_size,
                                         syncvar_t *ret);
int qthread_fork_copyargs_precond(qthread_f f,
                                  void const *arg,
                                  size_t arg_size,
                                  syncvar_t *ret,
                                  int npreconds,
                                  ...);
int qthread_fork_precond_simple(
  qthread_f f, void const *arg, aligned_t *ret, int npreconds, ...);

enum _qthread_features {
  SPAWN_PARENT,
  SPAWN_SIMPLE,
  SPAWN_NEW_TEAM,
  SPAWN_NEW_SUBTEAM,
  SPAWN_RET_SYNCVAR_T,
  SPAWN_RET_SINC,
  SPAWN_RET_SINC_VOID,
  SPAWN_PC_SYNCVAR_T,
  SPAWN_COUNT,
  SPAWN_LOCAL_PRIORITY,
  SPAWN_NETWORK
};

#define QTHREAD_SPAWN_PARENT (1 << SPAWN_PARENT)
#define QTHREAD_SPAWN_SIMPLE (1 << SPAWN_SIMPLE)
#define QTHREAD_SPAWN_NEW_TEAM (1 << SPAWN_NEW_TEAM)
#define QTHREAD_SPAWN_NEW_SUBTEAM (1 << SPAWN_NEW_SUBTEAM)
#define QTHREAD_SPAWN_RET_SYNCVAR_T (1 << SPAWN_RET_SYNCVAR_T)
#define QTHREAD_SPAWN_RET_SINC (1 << SPAWN_RET_SINC)
#define QTHREAD_SPAWN_RET_SINC_VOID (1 << SPAWN_RET_SINC_VOID)
#define QTHREAD_SPAWN_PC_SYNCVAR_T (1 << SPAWN_PC_SYNCVAR_T)
#define QTHREAD_SPAWN_LOCAL_PRIORITY (1 << SPAWN_LOCAL_PRIORITY)
#define QTHREAD_SPAWN_NETWORK (1 << SPAWN_NETWORK)

int qthread_spawn(qthread_f f,
                  void const *arg,
                  size_t arg_size,
                  void *ret,
                  size_t npreconds,
                  void *preconds,
                  qthread_shepherd_id_t target_shep,
                  unsigned int feature_flag);

/* This is a function to move a thread from one shepherd to another. */
int qthread_migrate_to(qthread_shepherd_id_t const shepherd);

/* Resets the default shepherd spawn order for tasks that use NO_SHEPHERD */
void qthread_reset_target_shep(void);

/* these are accessor functions for use by the qthreads to retrieve information
 * about themselves */
#define QTHREAD_NULL_TASK_ID UINT_MAX
#define QTHREAD_NON_TASK_ID 0
#define QTHREAD_DEFAULT_TEAM_ID 1
#define QTHREAD_NON_TEAM_ID 0
#define QTHREAD_NULL_TEAM_ID UINT_MAX
unsigned qthread_id(void);
qthread_shepherd_id_t qthread_shep(void);
qthread_worker_id_t qthread_worker(qthread_shepherd_id_t *s);
qthread_worker_id_t qthread_worker_unique(qthread_shepherd_id_t *s);
qthread_worker_id_t qthread_worker_local(qthread_shepherd_id_t *s);

void *qthread_get_tasklocal(unsigned int);
unsigned qthread_size_tasklocal(void);

void *qthread_tos(void);
void *qthread_bos(void);

size_t qthread_stackleft(void);
aligned_t *qthread_retloc(void);
int qthread_shep_ok(void);
void qthread_shep_next(qthread_shepherd_id_t *shep);
void qthread_shep_prev(qthread_shepherd_id_t *shep);
void qthread_shep_next_local(qthread_shepherd_id_t *shep);
void qthread_shep_prev_local(qthread_shepherd_id_t *shep);

/* returns the distance from one shepherd to another */
int qthread_distance(qthread_shepherd_id_t const src,
                     qthread_shepherd_id_t const dest);
/* returns a list of shepherds, sorted by their distance from either this
 * qthread or the specified shepherd */
qthread_shepherd_id_t const *qthread_sorted_sheps(void);
qthread_shepherd_id_t const *
qthread_sorted_sheps_remote(qthread_shepherd_id_t const src);
/* returns the number of actively-scheduling shepherds */
qthread_shepherd_id_t qthread_num_shepherds(void);
qthread_worker_id_t
qthread_num_workers(void); /* how many kernel-level threads are running */
qthread_worker_id_t
qthread_num_workers_local(qthread_shepherd_id_t shepherd_id);

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
void qt_team_critical_section(qt_team_critical_section_t boundary);

#define qthread_fork_new_team(f, a, r)                                         \
  qthread_spawn((f), (a), 0, (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_TEAM)
#define qthread_fork_new_subteam(f, a, r)                                      \
  qthread_spawn(                                                               \
    (f), (a), 0, (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_SUBTEAM)
#define qthread_fork_new_team_to(f, a, r, p)                                   \
  qthread_spawn((f), (a), 0, (r), 0, NULL, (p), QTHREAD_SPAWN_NEW_TEAM)
#define qthread_fork_syncvar_new_team(f, a, r)                                 \
  qthread_spawn((f),                                                           \
                (a),                                                           \
                0,                                                             \
                (r),                                                           \
                0,                                                             \
                NULL,                                                          \
                NO_SHEPHERD,                                                   \
                QTHREAD_SPAWN_NEW_TEAM | QTHREAD_SPAWN_RET_SYNCVAR_T)
#define qthread_fork_syncvar_new_subteam(f, a, r)                              \
  qthread_spawn((f),                                                           \
                (a),                                                           \
                0,                                                             \
                (r),                                                           \
                0,                                                             \
                NULL,                                                          \
                NO_SHEPHERD,                                                   \
                QTHREAD_SPAWN_NEW_SUBTEAM | QTHREAD_SPAWN_RET_SYNCVAR_T)
#define qthread_fork_copyargs_new_team(f, a, z, r)                             \
  qthread_spawn(                                                               \
    (f), (a), (z), (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_TEAM)
#define qthread_fork_copyargs_new_subteam(f, a, z, r)                          \
  qthread_spawn(                                                               \
    (f), (a), (z), (r), 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NEW_SUBTEAM)
#define qthread_fork_syncvar_copyargs_to(f, a, z, r, p)                        \
  qthread_spawn((f), (a), (z), (r), 0, NULL, (p), QTHREAD_SPAWN_RET_SYNCVAR_T)

struct qthread_queue_s;
typedef struct qthread_queue_s *qthread_queue_t;

#define QTHREAD_QUEUE_NO_SYNCH (1 << 0)
#define QTHREAD_QUEUE_MULTI_JOIN (1 << 1)
#define QTHREAD_QUEUE_MULTI_JOIN_LENGTH (1 << 2)
#define QTHREAD_QUEUE_CAPPED (1 << 3)

qthread_queue_t qthread_queue_create(uint8_t flags, aligned_t length);
int qthread_queue_join(qthread_queue_t q);
aligned_t qthread_queue_length(qthread_queue_t q);
int qthread_queue_release_one(qthread_queue_t q);
int qthread_queue_release_all(qthread_queue_t q);
int qthread_queue_destroy(qthread_queue_t q);

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

#define QTHREAD_SPINLOCK_IS_RECURSIVE (-1)
#define QTHREAD_SPINLOCK_IS_NOT_RECURSIVE (-2)

typedef union qt_spin_trylock_s {
  aligned_t u;

  struct {
    QT_Atomic(haligned_t) ticket;
    QT_Atomic(haligned_t) users;
  } s;
} qt_spin_trylock_t;

typedef struct {
  int64_t s;
  int64_t count;
} qthread_spinlock_state_t;

typedef struct {
  qt_spin_trylock_t lock;
  qthread_spinlock_state_t state;
} qthread_spinlock_t;

/* This function is just to assist with debugging; it returns 1 if the address
 * is full, and 0 if the address is empty */
int qthread_feb_status(aligned_t const *addr);
int qthread_syncvar_status(syncvar_t *const v);

/* The empty/fill functions merely assert the empty or full state of the given
 * address. */
int qthread_empty(aligned_t const *dest);
int qthread_syncvar_empty(syncvar_t *restrict dest);
int qthread_fill(aligned_t const *dest);
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
int qthread_writeEF(aligned_t *restrict dest, aligned_t const *restrict src);
int qthread_writeEF_const(aligned_t *dest, aligned_t const src);
int qthread_syncvar_writeEF(syncvar_t *restrict dest,
                            uint64_t const *restrict src);
int qthread_syncvar_writeEF_const(syncvar_t *restrict dest, uint64_t src);

/* This function is a cross between qthread_fill() and qthread_writeEF(). It
 * does not wait for memory to become empty, but performs the write and sets
 * the state to full atomically with respect to other FEB-based actions. Data
 * is read from src and written to dest.
 *
 * The semantics of writeF are:
 * 1 - data is copied from src to destination
 * 2 - the destination's FEB state gets set to full
 */
int qthread_writeF(aligned_t *dest, aligned_t const *src);
int qthread_writeF_const(aligned_t *dest, aligned_t src);
int qthread_syncvar_writeF(syncvar_t *restrict dest,
                           uint64_t const *restrict src);
int qthread_syncvar_writeF_const(syncvar_t *restrict dest, uint64_t src);

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
int qthread_purge_to(aligned_t *restrict dest, aligned_t const *restrict src);
int qthread_purge_to_const(aligned_t *dest, aligned_t purgeVal);

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
int qthread_writeFF(aligned_t *restrict dest, aligned_t const *restrict src);
int qthread_writeFF_const(aligned_t *dest, aligned_t src);
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
int qthread_readFF(aligned_t *dest, aligned_t const *src);
int qthread_syncvar_readFF(uint64_t *restrict dest, syncvar_t *restrict src);

/* These functions wait for memory to become full, and then empty it. When
 * memory becomes full, only one thread blocked like this will be awoken. Data
 * is read from src and written to dest.
 *
 * The semantics of readFE are:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 * 3 - the src's FEB bits get changed from full to empty when the data is copied
 */
int qthread_readFE(aligned_t *dest, aligned_t const *src);
int qthread_syncvar_readFE(uint64_t *restrict dest, syncvar_t *restrict src);

/* This function ignores the FEB state. Data is read from src and written to
 * dest.
 *
 * The semantics of readXX are:
 * 1 - src's FEB state is ignored
 * 2 - data is copied from src to destination
 */
int qthread_readXX(aligned_t *dest, aligned_t const *src);
// NOTE: There is no syncvar version of readXX

/* functions to implement FEB-ish locking/unlocking
 *
 * These are atomic and functional, but do not have the same semantics as full
 * FEB locking/unlocking (namely, unlocking cannot block), however because of
 * this, they have lower overhead.
 */
int qthread_lock(aligned_t const *a);
int qthread_unlock(aligned_t const *a);
int qthread_trylock(aligned_t const *a);

int qthread_spinlock_init(qthread_spinlock_t *a, bool const is_recursive);
int qthread_spinlock_destroy(qthread_spinlock_t *a);
int qthread_spinlock_lock(qthread_spinlock_t *a);
int qthread_spinlock_unlock(qthread_spinlock_t *a);
int qthread_spinlock_trylock(qthread_spinlock_t *a);

int qthread_spinlocks_init(qthread_spinlock_t *a, bool const is_recursive);
int qthread_spinlocks_destroy(qthread_spinlock_t *a);

#define QTHREAD_SPINLOCK_IS_RECURSIVE (-1)
#define QTHREAD_SPINLOCK_IS_NOT_RECURSIVE (-2)

#define QTHREAD_MUTEX_INITIALIZER                                              \
  {{.s = {0, 0}}, {QTHREAD_SPINLOCK_IS_NOT_RECURSIVE, 0}}
#define QTHREAD_RECURSIVE_MUTEX_INITIALIZER                                    \
  {{.s = {0, 0}}, {QTHREAD_SPINLOCK_IS_RECURSIVE, 0}}

/* functions to implement spinlock-based locking/unlocking
 * if qthread_lock_init(adr) is called, subsequent locking over adr
 * uses spin locking instead of FEBs. Support recursive locking.
 */
int qthread_lock_init(aligned_t const *a, bool const is_recursive);
int qthread_lock_destroy(aligned_t *a);

/* the following three functions implement variations on atomic increment. It
 * is done with architecture-specific assembly (on supported architectures,
 * when possible) and does NOT use FEB's or lock/unlock unless the architecture
 * is unsupported or cannot perform atomic operations at the right granularity.
 * All of these functions return the value of the contents of the operand
 * *after* incrementing.
 */
static inline float qthread_fincr(float *operand, float incr) {
  union {
    float f;
    uint32_t i;
  } oldval, newval, res;

  do {
    oldval.f =
      atomic_load_explicit((QT_Atomic(float) *)operand, memory_order_relaxed);
    newval.f = oldval.f + incr;
    res.i =
      __sync_val_compare_and_swap((uint32_t *)operand, oldval.i, newval.i);
  } while (res.i != oldval.i); /* if res!=old, the calc is out of date */
  return oldval.f;
}

static inline double qthread_dincr(double *operand, double incr) {
  union {
    uint64_t i;
    double d;
  } oldval, newval, res;

  do {
    oldval.d =
      atomic_load_explicit((QT_Atomic(double) *)operand, memory_order_relaxed);
    newval.d = oldval.d + incr;
    res.i =
      __sync_val_compare_and_swap((uint64_t *)operand, oldval.i, newval.i);
  } while (res.i != oldval.i); /* if res!=old, the calc is out of date */
  return oldval.d;
}

static inline uint32_t qthread_incr32(uint32_t *operand, uint32_t incr) {
  return __sync_fetch_and_add(operand, incr);
}

static inline uint64_t qthread_incr64(uint64_t *operand, uint64_t incr) {
  return __sync_fetch_and_add(operand, incr);
}

static inline int64_t qthread_incr_xx(void *addr, int64_t incr, size_t length) {
  switch (length) {
    case 4: return qthread_incr32((uint32_t *)addr, incr);

    case 8: return qthread_incr64((uint64_t *)addr, incr);

    default: QTHREAD_TRAP();
  }
  return 0; /* compiler check */
}

uint64_t qthread_syncvar_incrF(syncvar_t *restrict operand, uint64_t inc);

#define qthread_cas(ADDR, OLDV, NEWV)                                          \
  __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
#define qthread_cas32(ADDR, OLDV, NEWV)                                        \
  __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
#define qthread_cas64(ADDR, OLDV, NEWV)                                        \
  __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
#ifdef __INTEL_COMPILER
#define qthread_cas_ptr(ADDR, OLDV, NEWV)                                      \
  (void *)__sync_val_compare_and_swap(                                         \
    (ADDR), (uintptr_t)(OLDV), (uintptr_t)(NEWV))
#else
#define qthread_cas_ptr(ADDR, OLDV, NEWV)                                      \
  (void *)__sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
#endif

Q_ENDCXX /* */

#ifndef __cplusplus

#define qthread_incr(ADDR, INCVAL) __sync_fetch_and_add(ADDR, INCVAL)

#else /* ifdef __cplusplus */
  extern "C++" {
#include "qthread.hpp"
}
#endif /* __cplusplus */

#endif /* _QTHREAD_H_ */
  /* vim:set expandtab: */
