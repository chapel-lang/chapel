/* System Headers */
/* Portions of this file are Copyright (c) 2025 Tactical Computing Labs, LLC;
 * see COPYING */
#include <limits.h> /* for INT_MAX */
#include <stdint.h>

/* API Headers */
#include "qthread/qthread.h"

/* Internal Syncvar API */
#include "qt_syncvar.h"

/* Internal Headers */
#include "qt_addrstat.h"
#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_blocking_structs.h"
#include "qt_hash.h"
#include "qt_initialized.h" // for qthread_library_initialized
#include "qt_profiling.h"
#include "qt_qthread_mgmt.h"
#include "qt_qthread_struct.h"
#include "qt_subsystems.h"
#include "qt_threadqueues.h"
#include "qthread_innards.h"

/* Internal Prototypes */
static inline void qthread_syncvar_gotlock_fill(qthread_shepherd_t *shep,
                                                qthread_addrstat_t *m,
                                                syncvar_t *maddr,
                                                uint64_t const ret);
static inline void qthread_syncvar_gotlock_empty(qthread_shepherd_t *shep,
                                                 qthread_addrstat_t *m,
                                                 syncvar_t *maddr,
                                                 uint64_t const ret);
static inline void qthread_syncvar_remove(void *maddr);

/* Internal Structs */
typedef struct {
  unsigned int cf : 1; // there was a timeout
  unsigned int zf : 1; // unused
  unsigned int of : 1;
  unsigned int pf : 1;
  unsigned int sf : 1;
} eflags_t;

typedef enum bt {
  WRITEEF,
  WRITEEF_NB,
  WRITEF,
  READFF,
  READFF_NB,
  READFE,
  READFE_NB,
  FILL,
  EMPTY,
  INCR
} blocker_type;

typedef struct {
  pthread_mutex_t lock;
  pthread_cond_t condition;
  uint32_t completed;
  void *a;
  void *b;
  blocker_type type;
  int retval;
} qthread_syncvar_blocker_t;

/* Internal Variables */
static qt_hash *syncvars;
#ifdef QTHREAD_COUNT_THREADS
extern aligned_t *febs_stripes;
#endif
extern unsigned int QTHREAD_LOCKING_STRIPES;

/* Internal Macros */
#define BUILD_UNLOCKED_SYNCVAR(data, state) (((data) << 4) | ((state) << 1))
#define QTHREAD_CHOOSE_STRIPE(addr)                                            \
  (((size_t)addr >> 4) & (QTHREAD_LOCKING_STRIPES - 1))

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64 ||                                 \
     QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM ||                                   \
     QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64 ||                             \
     QTHREAD_ASSEMBLY_ARCH == QTHREAD_RISCV)
#define UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked)                         \
  do {                                                                         \
    atomic_store_explicit(                                                     \
      (_Atomic uint64_t *)&(addr)->u.w, (unlocked), memory_order_relaxed);     \
  } while (0)
#define UNLOCK_THIS_MODIFIED_SYNCVAR(addr, val, state)                         \
  do {                                                                         \
    MACHINE_FENCE;                                                             \
    atomic_store_explicit((_Atomic uint64_t *)&(addr)->u.w,                    \
                          BUILD_UNLOCKED_SYNCVAR(val, state),                  \
                          memory_order_relaxed);                               \
  } while (0)
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) ||                          \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) ||                              \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
#define UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked)                         \
  do {                                                                         \
    atomic_store_explicit(                                                     \
      (_Atomic uint64_t *)&(addr)->u.w, (unlocked), memory_order_relaxed);     \
  } while (0)
#define UNLOCK_THIS_MODIFIED_SYNCVAR(addr, val, state)                         \
  do {                                                                         \
    MACHINE_FENCE;                                                             \
    atomic_store_explicit((_Atomic uint64_t *)&(addr)->u.w,                    \
                          BUILD_UNLOCKED_SYNCVAR(val, state),                  \
                          memory_order_relaxed);                               \
  } while (0)
#else /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64 || QTHREAD_ASSEMBLY_ARCH   \
         == QTHREAD_ARM || QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64) */
#define UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked)                         \
  do {                                                                         \
    /* this has its own pthread mutex, so does not need memory synch */        \
    qthread_cas64(&((addr)->u.w), (addr)->u.w, (unlocked));                    \
  } while (0)
#define UNLOCK_THIS_MODIFIED_SYNCVAR(addr, val, state)                         \
  do {                                                                         \
    /* this has its own pthread mutex, so does not need memory synch */        \
    qthread_cas64(                                                             \
      &((addr)->u.w), (addr)->u.w, BUILD_UNLOCKED_SYNCVAR(val, state));        \
  } while (0)
#endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64 || QTHREAD_ASSEMBLY_ARCH  \
          == QTHREAD_ARM || QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64) */

static uint64_t qthread_mwaitc(syncvar_t *restrict const addr,
                               unsigned char const statemask,
                               unsigned int timeout,
                               eflags_t *restrict const err) {
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
  syncvar_t unlocked;
#endif
  syncvar_t locked;
  eflags_t e;

  assert(timeout > 0);
  assert(addr != NULL);
  assert(err != NULL);

  e = *err;
  e.zf = 0u;
  e.cf = 1u;
  do {
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    /* This applies for any 32-bit architecture with a valid 32-bit CAS
     * (though I'm making some big-endian assumptions at the moment) */
    uint32_t low_unlocked, low_locked;
    uint32_t *addrptr = (uint32_t *)addr;
    do {
    loop_start:
      low_unlocked = addrptr[1]; // atomic read
      if ((low_unlocked & 1) != 0) {
        if (timeout-- <= 0) { goto errexit; }
        SPINLOCK_BODY();
        goto loop_start;
      }
      low_locked = low_unlocked | 1;
      if (qthread_cas32(&(addrptr[1]), low_unlocked, low_locked) ==
          low_unlocked) {
        break;
      }
      if (timeout-- <= 0) { goto errexit; }
    } while (1);
    locked.u.w = addr->u.w; // I locked it, so I can read it
#else  /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) */
    {
      syncvar_t tmp;
    loop_start:
      tmp.u.w =
        atomic_load_explicit((_Atomic uint64_t *)addr, memory_order_relaxed);
      do {
        unlocked = tmp; // may be locked or unlocked, we don't know
        if (unlocked.u.s.lock == 1) {
          if (timeout-- <= 0) { goto errexit; }
          SPINLOCK_BODY();
          goto loop_start;
        }
        locked = unlocked;
        locked.u.s.lock = 1; // create the locked version
        if ((tmp.u.w = qthread_cas64(
               (uint64_t *)addr, unlocked.u.w, locked.u.w)) == unlocked.u.w) {
          break;
        }
        if (timeout-- <= 0) { goto errexit; }
      } while (1);
    }
#endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) */
    /***************************************************
     * now locked == unlocked, and the lock bit is set *
     ***************************************************/
    if (statemask & (1 << locked.u.s.state)) {
      /* this is a state of interest, so fill the err struct */
      e.cf = 0u;
      e.sf = (unsigned char)(locked.u.s.state & 1u);
      e.pf = (unsigned char)((locked.u.s.state >> 1u) & 1u);
      e.of = (unsigned char)((locked.u.s.state >> 2u) & 1u);
      *err = e;
      return locked.u.s.data;
    } else {
      /* this is NOT a state of interest, so unlock the locked bit */
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
      MACHINE_FENCE;
      addrptr[1] = low_unlocked;
#else
      UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked.u.w);
#endif
    }
  } while (timeout-- > 0);
errexit:
  *err = e;
  return 0;
}

static void qt_syncvar_subsystem_shutdown(void) {
  for (unsigned i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
    qt_hash_destroy_deallocate(syncvars[i],
                               (qt_hash_deallocator_fn)qthread_addrstat_delete);
  }
  FREE(syncvars, sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
}

void INTERNAL qt_syncvar_subsystem_init(uint_fast8_t need_sync) {
  syncvars = MALLOC(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
  assert(syncvars);
  for (unsigned i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
    syncvars[i] = qt_hash_create(need_sync);
    assert(syncvars[i]);
  }
  qthread_internal_cleanup_late(qt_syncvar_subsystem_shutdown);
}

int API_FUNC qthread_syncvar_status(syncvar_t *const v) {
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  unsigned int realret;

  /* If I've done an atomic read
   * and the syncvar is unlocked, then I figure I can trust
   * that state and do not need to do a locked atomic operation of any
   * kind (e.g. cas) */
  syncvar_t local_copy_of_v;
  local_copy_of_v.u.w =
    atomic_load_explicit((_Atomic uint64_t *)v, memory_order_relaxed);
  if (local_copy_of_v.u.s.lock == 0) {
    /* short-circuit */
    return (local_copy_of_v.u.s.state & 0x2) ? 0 : 1;
  }
  (void)qthread_mwaitc(v, 0xff, INT_MAX, &e);
  qassert_ret(e.cf == 0u,
              QTHREAD_TIMEOUT); /* there better not have been a timeout */
  local_copy_of_v.u.w =
    atomic_load_explicit((_Atomic uint64_t *)v, memory_order_relaxed);
  realret = local_copy_of_v.u.s.state;
  UNLOCK_THIS_UNMODIFIED_SYNCVAR(
    v,
    BUILD_UNLOCKED_SYNCVAR(local_copy_of_v.u.s.data,
                           local_copy_of_v.u.s.state));
  return (realret & 0x2) ? 0 : 1;
}

static aligned_t qthread_syncvar_nonblocker_thread(void *arg) {
  qthread_syncvar_blocker_t *restrict const a =
    (qthread_syncvar_blocker_t *)arg;

  switch (a->type) {
    case WRITEF: a->retval = qthread_syncvar_writeF(a->a, a->b); break;
    case FILL: a->retval = qthread_syncvar_fill(a->a); break;
    case EMPTY: a->retval = qthread_syncvar_empty(a->a); break;
    default: return 1;
  }
  return 0;
}

static aligned_t qthread_syncvar_blocker_thread(void *arg) {
  qthread_syncvar_blocker_t *restrict const a =
    (qthread_syncvar_blocker_t *)arg;

  switch (a->type) {
    case READFE: a->retval = qthread_syncvar_readFE(a->a, a->b); break;
    case READFE_NB: a->retval = qthread_syncvar_readFE_nb(a->a, a->b); break;
    case READFF: a->retval = qthread_syncvar_readFF(a->a, a->b); break;
    case READFF_NB: a->retval = qthread_syncvar_readFF_nb(a->a, a->b); break;
    case WRITEEF: a->retval = qthread_syncvar_writeEF(a->a, a->b); break;
    case WRITEEF_NB: a->retval = qthread_syncvar_writeEF_nb(a->a, a->b); break;
    case WRITEF: a->retval = qthread_syncvar_writeF(a->a, a->b); break;
    case FILL: a->retval = qthread_syncvar_fill(a->a); break;
    case EMPTY: a->retval = qthread_syncvar_empty(a->a); break;
    case INCR: a->retval = qthread_syncvar_incrF(a->a, *(int64_t *)a->b); break;
  }
  pthread_mutex_lock(&a->lock);
  a->completed = 1u;
  pthread_cond_signal(&a->condition);
  pthread_mutex_unlock(&a->lock);
  return 0;
}

static int
qthread_syncvar_nonblocker_func(void *dest, void *src, blocker_type t) {
  qthread_syncvar_blocker_t args = {PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_COND_INITIALIZER,
                                    0u,
                                    dest,
                                    src,
                                    t,
                                    QTHREAD_SUCCESS};

  qthread_fork(qthread_syncvar_nonblocker_thread, &args, NULL);
  return args.retval;
}

static int qthread_syncvar_blocker_func(void *dest, void *src, blocker_type t) {
  qthread_syncvar_blocker_t args = {PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_COND_INITIALIZER,
                                    0u,
                                    dest,
                                    src,
                                    t,
                                    QTHREAD_SUCCESS};

  pthread_mutex_lock(&args.lock);
  qthread_fork(qthread_syncvar_blocker_thread, &args, NULL);
  while (!args.completed) pthread_cond_wait(&args.condition, &args.lock);
  pthread_mutex_unlock(&args.lock);
  pthread_cond_destroy(&args.condition);
  pthread_mutex_destroy(&args.lock);
  return args.retval;
}

/* state 0: full, no waiters
 * state 1: full, queued waiters (who are waiting for it to be empty)
 * state 2: empty, no waiters
 * state 3: empty, queued waiters (who are waiting for it to be full)
 * state 4-7: undefined
 */
/* ops */
/* fff */
#define SYNCFEB_FULL_NOWAIT 0x1   /* 000 */
#define SYNCFEB_FULL_WAITERS 0x2  /* 001 */
#define SYNCFEB_FULL 0x3          /* 00x */
#define SYNCFEB_EMPTY_NOWAIT 0x4  /* 010 */
#define SYNCFEB_EMPTY_WAITERS 0x8 /* 011 */
#define SYNCFEB_EMPTY 0xc         /* 01x */
#define SYNCFEB_ANY 0xf           /* 0xx */
#define INITIAL_TIMEOUT 100
#define SYNCFEB_STATE_FULL_NO_WAITERS 0x0
#define SYNCFEB_STATE_FULL_WITH_WAITERS 0x1
#define SYNCFEB_STATE_EMPTY_NO_WAITERS 0x2
#define SYNCFEB_STATE_EMPTY_WITH_WAITERS 0x3

int API_FUNC qthread_syncvar_readFF(uint64_t *restrict dest,
                                    syncvar_t *restrict src) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret;
  qthread_t *me = qthread_internal_self();
  assert(src);

  if (!me) { return qthread_syncvar_blocker_func(dest, src, READFF); }

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||                                \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) ||                              \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM) ||                                    \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)
  {
    /* I'm being optimistic here; this only works if a basic 64-bit load is
     * atomic (on most platforms it is). Thus, if I've done an atomic read
     * and the syncvar is both unlocked and full, then I figure I can trust
     * that state and do not need to do a locked atomic operation of any
     * kind (e.g. cas) */
    syncvar_t local_copy_of_src;
    local_copy_of_src.u.w =
      atomic_load_explicit((_Atomic uint64_t *)src, memory_order_relaxed);
    if ((local_copy_of_src.u.s.lock == 0) &&
        ((local_copy_of_src.u.s.state & 2) == 0)) { /* full and unlocked */
      /* short-circuit */
      if (dest) { *dest = local_copy_of_src.u.s.data; }
      return QTHREAD_SUCCESS;
    }
  }
#endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||                       \
             (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) ||                   \
             (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM) ||                         \
             (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)) */
  ret = qthread_mwaitc(src, SYNCFEB_FULL, INITIAL_TIMEOUT, &e);
  if (e.cf) { /* there was a timeout */
    int const lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_addrstat_t *m;
    qthread_addrres_t *X;

    ret = qthread_mwaitc(src, SYNCFEB_ANY, INT_MAX, &e);
    qassert_ret(e.cf == 0u,
                QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if (e.pf == 0u) {             /* it got full! */
      goto locked_full;
    }
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)src, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)src))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else  /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    if (!m) {
      m = qthread_addrstat_new();
      assert(m);
      if (!m) { return QTHREAD_MALLOC_ERROR; }
      qassertnot(qt_hash_put(syncvars[lockbin], (void *)src, m), 0);
    }
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif /* ifdef LOCK_FREE_FEBS */
    UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_WITH_WAITERS);
    X = ALLOC_ADDRRES();
    assert(X);
    if (!X) {
      qthread_syncvar_remove(src);
      return ENOMEM;
    }
    X->addr = (aligned_t *)dest;
    X->waiter = me;
    X->next = m->FFQ;
    m->FFQ = X;
    atomic_store_explicit(
      &me->thread_state, QTHREAD_STATE_FEB_BLOCKED, memory_order_relaxed);
    me->rdata->blockedon.addr = m;
    qthread_back_to_master(me);
  } else {
  locked_full:
    /* at this point, the syncvar is locked and e.pf should be 0 */
    assert(e.pf == 0);
    UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, e.sf);
    if (dest) { *dest = ret; }
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_readFF_nb(uint64_t *restrict dest,
                                       syncvar_t *restrict src) {
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret;
  qthread_t *me = qthread_internal_self();

  assert(src);

  if (!me) { return qthread_syncvar_blocker_func(dest, src, READFF_NB); }

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||                                \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) ||                              \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM) ||                                    \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)
  {
    /* I'm being optimistic here; this only works if a basic 64-bit load is
     * atomic (on most platforms it is). Thus, if I've done an atomic read
     * and the syncvar is both unlocked and full, then I figure I can trust
     * that state and do not need to do a locked atomic operation of any
     * kind (e.g. cas) */
    syncvar_t local_copy_of_src = *src;
    if ((local_copy_of_src.u.s.lock == 0) &&
        ((local_copy_of_src.u.s.state & 2) == 0)) { /* full and unlocked */
      /* short-circuit */
      if (dest) { *dest = local_copy_of_src.u.s.data; }
      return QTHREAD_SUCCESS;
    }
  }
#endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||                       \
             (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) ||                   \
             (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM) ||                         \
             (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64) */
  ret = qthread_mwaitc(src, SYNCFEB_FULL, 1, &e);
  if (e.cf) { /* there was a timeout */
    return QTHREAD_OPFAIL;
  } else {
    /* at this point, the syncvar is locked and e.pf should be 0 */
    assert(e.pf == 0u);
    UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, e.sf);
    if (dest) { *dest = ret; }
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_fill(syncvar_t *restrict addr) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret;
  qthread_shepherd_t *shep = qthread_internal_getshep();

  assert(addr);

  if (!shep) { return qthread_syncvar_nonblocker_func(addr, NULL, FILL); }
  ret = qthread_mwaitc(addr, SYNCFEB_ANY, INT_MAX, &e);
  qassert_ret(e.cf == 0u,
              QTHREAD_TIMEOUT); /* there better not have been a timeout */
  if (e.pf == 1u) {   /* currently empty, so it needs to change state */
    if (e.sf == 1u) { /* waiters! */
      int const lockbin = QTHREAD_CHOOSE_STRIPE(addr);
      qthread_addrstat_t *m;

      QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
      e.sf = 0u; // I'm releasing waiters
      e.pf = 0u; // I'm going to mark this as full
#ifdef LOCK_FREE_FEBS
      do {
        m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)addr);
      got_m:
        if (!m) {
          m = qthread_addrstat_new();
          if (!m) { return QTHREAD_MALLOC_ERROR; }
          QTHREAD_FASTLOCK_LOCK(&m->lock);
          qassertnot(qt_hash_put(syncvars[lockbin], (void *)addr, m), 0);
        } else {
          qthread_addrstat_t *m2;
          hazardous_ptr(0, m);
          if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)addr))) {
            m = m2;
            goto got_m;
          }
          if (!m->valid) { continue; }
          QTHREAD_FASTLOCK_LOCK(&m->lock);
          if (!m->valid) {
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            continue;
          }
        }
        break;
      } while (1);
#else  /* ifdef LOCK_FREE_FEBS */
      /* Note that locking the hash table is unnecessary because we have
       * locked the syncvar itself. */
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)addr);
      assert(m); // otherwise there weren't really any waiters
      QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif /* ifdef LOCK_FREE_FEBS */
      if (m->FEQ) {
        e.pf = 1u; // back to being empty
        if (m->FEQ->next) {
          e.sf = 1u; // only one will be dequeued, so it'll still have waiters
        }
      }
      UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, (e.pf << 1) | e.sf);
      assert(m->FFQ || m->FEQ); // otherwise there weren't really any waiters
      assert(m->EFQ == NULL);   // someone snuck in!
      qthread_syncvar_gotlock_fill(shep, m, addr, ret);
    } else {
      assert(e.sf == 0u); /* no waiters */
      UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, 0);
    }
  } else { /* already full, so just release the lock */
    assert(e.pf == 0u);
    UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, e.sf);
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_empty(syncvar_t *restrict addr) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret;
  qthread_shepherd_t *shep = qthread_internal_getshep();

  assert(addr);

  if (!shep) { return qthread_syncvar_nonblocker_func(addr, NULL, EMPTY); }
  ret = qthread_mwaitc(addr, SYNCFEB_ANY, INT_MAX, &e);
  qassert_ret(e.cf == 0u,
              QTHREAD_TIMEOUT); /* there better not have been a timeout */
  if (e.pf == 0u) {   /* currently full, so it needs to change state */
    if (e.sf == 1u) { /* waiters! */
      int const lockbin = QTHREAD_CHOOSE_STRIPE(addr);
      qthread_addrstat_t *m;

      QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
      e.sf = 0u; // released!
                 // wanted to mark it empty, but the waiters will fill it
#ifdef LOCK_FREE_FEBS
      do {
        m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)addr);
      got_m:
        if (!m) {
          m = qthread_addrstat_new();
          if (!m) { return QTHREAD_MALLOC_ERROR; }
          QTHREAD_FASTLOCK_LOCK(&m->lock);
          qassertnot(qt_hash_put(syncvars[lockbin], (void *)addr, m), 0);
        } else {
          qthread_addrstat_t *m2;
          hazardous_ptr(0, m);
          if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)addr))) {
            m = m2;
            goto got_m;
          }
          if (!m->valid) { continue; }
          QTHREAD_FASTLOCK_LOCK(&m->lock);
          if (!m->valid) {
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            continue;
          }
        }
        break;
      } while (1);
#else                 /* ifdef LOCK_FREE_FEBS */
      /* Note that locking the hash table is unnecessary because we have
       * locked the syncvar itself. */
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)addr);
      assert(m); // otherwise, there weren't any waiters
      QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif                /* ifdef LOCK_FREE_FEBS */
      assert(m->EFQ); // otherwise there weren't really any waiters
      assert(m->FFQ == NULL && m->FEQ == NULL); // someone snuck in!
      if (m->EFQ->next) { e.sf = 1u; }
      // addr->u.w = BUILD_UNLOCKED_SYNCVAR(ret, e.sf); // this must be done by
      // gotlock_empty so we know what value to write
      qthread_syncvar_gotlock_empty(shep, m, addr, e.sf);
    } else {
      assert(e.sf == 0u); /* no waiters */
      UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS);
    }
  } else { /* already empty, so just release the lock */
    assert(e.pf == 1u);
    UNLOCK_THIS_MODIFIED_SYNCVAR(
      addr, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS | e.sf);
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_readFE(uint64_t *restrict dest,
                                    syncvar_t *restrict src) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret;
  int const lockbin = QTHREAD_CHOOSE_STRIPE(src);
  qthread_t *me = qthread_internal_self();

  assert(src);

  if (!me) { return qthread_syncvar_blocker_func(dest, src, READFE); }

  assert(me->rdata);
  assert(me->rdata->shepherd_ptr);
  assert(me->rdata->shepherd_ptr->ready);
  assert(me->rdata->shepherd_ptr->shepherd_id < qthread_num_shepherds());

  ret = qthread_mwaitc(src, SYNCFEB_FULL, INITIAL_TIMEOUT, &e);
  if (e.cf) { /* there was a timeout */
    qthread_addrstat_t *m;
    qthread_addrres_t *X;

    ret = qthread_mwaitc(src, SYNCFEB_ANY, INT_MAX, &e);
    qassert_ret(e.cf == 0u,
                QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if (e.pf == 0u) {             /* it got full! */
      if (e.sf == 1u) {           /* it got full with waiters! */
        goto locked_full_waiters;
      } else {
        goto locked_full;
      }
    }
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)src, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)src))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else  /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    qt_hash_lock(syncvars[lockbin]);
    {
      m = (qthread_addrstat_t *)qt_hash_get_locked(syncvars[lockbin],
                                                   (void *)src);
      if (!m) {
        m = qthread_addrstat_new();
        assert(m);
        if (!m) { return ENOMEM; }
        qassertnot(qt_hash_put_locked(syncvars[lockbin], (void *)src, m), 0);
      }
      QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(syncvars[lockbin]);
#endif /* ifdef LOCK_FREE_FEBS */
    UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_WITH_WAITERS);
    X = ALLOC_ADDRRES();
    assert(X);
    if (!X) {
      qthread_syncvar_remove(src);
      return ENOMEM;
    }
    X->addr = (aligned_t *)&ret;
    X->waiter = me;
    X->next = m->FEQ;
    m->FEQ = X;
    atomic_store_explicit(
      &me->thread_state, QTHREAD_STATE_FEB_BLOCKED, memory_order_relaxed);
    me->rdata->blockedon.addr = m;
    qthread_back_to_master(me);
  } else if (e.sf == 1u) { /* waiters! */
    qthread_addrstat_t *m;

  locked_full_waiters:
    assert(e.pf == 0u); // otherwise we should have gotten a timeout
    e.sf = 0u;          // released!
    // wanted to mark it empty (pf=1), but the waiters will fill it
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    got_m2:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)src, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)src))) {
          m = m2;
          goto got_m2;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else               /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    assert(m); // otherwise, there weren't any waiters
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif              /* ifdef LOCK_FREE_FEBS */
    assert(m->EFQ); // otherwise there weren't really any waiters
    assert(m->FFQ == NULL && m->FEQ == NULL); // someone snuck in!
    if (m->EFQ->next) { // there will be a waiter still waiting
      e.sf = 1u;
    }
    // src->u.w = BUILD_UNLOCKED_SYNCVAR(ret, e.sf); // this must be done by
    // gotlock_empty so we know what value to write
    qthread_syncvar_gotlock_empty(me->rdata->shepherd_ptr, m, src, e.sf);
  } else {
  locked_full:
    assert(e.pf == 0u); // otherwise this isn't really full
    UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS);
  }
  if (dest) { *dest = ret; }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_readFE_nb(uint64_t *restrict dest,
                                       syncvar_t *restrict src) {
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret;
  int const lockbin = QTHREAD_CHOOSE_STRIPE(src);
  qthread_t *me = qthread_internal_self();

  assert(src);

  if (!me) { return qthread_syncvar_blocker_func(dest, src, READFE_NB); }

  assert(me->rdata);
  assert(me->rdata->shepherd_ptr);
  assert(me->rdata->shepherd_ptr->ready);
  assert(me->rdata->shepherd_ptr->shepherd_id < qthread_num_shepherds());

  ret = qthread_mwaitc(src, SYNCFEB_FULL, 1, &e);
  if (e.cf) { /* there was a timeout */
    return QTHREAD_OPFAIL;
  } else if (e.sf == 1u) { /* waiters! */
    qthread_addrstat_t *m;

    assert(e.pf == 0u); // otherwise we should have gotten a timeout
    e.sf = 0u;          // released!
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    // wanted to mark it empty (pf=1), but the waiters will fill it
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)src, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)src))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else               /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)src);
    assert(m); // otherwise, there weren't any waiters
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif              /* ifdef LOCK_FREE_FEBS */
    assert(m->EFQ); // otherwise there weren't really any waiters
    assert(m->FFQ == NULL && m->FEQ == NULL); // someone snuck in!
    if (m->EFQ->next) { // there will be a waiter still waiting
      e.sf = 1u;
    }
    // src->u.w = BUILD_UNLOCKED_SYNCVAR(ret, e.sf); // this must be done by
    // gotlock_empty so we know what value to write
    qthread_syncvar_gotlock_empty(me->rdata->shepherd_ptr, m, src, e.sf);
  } else {
    assert(e.pf == 0u); // otherwise this isn't really full
    UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS);
  }
  if (dest) { *dest = ret; }
  return QTHREAD_SUCCESS;
}

static inline void qthread_syncvar_schedule(qthread_t *waiter,
                                            qthread_shepherd_t *shep) {
  assert(waiter);
  assert(shep);
  atomic_store_explicit(
    &waiter->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
  if (atomic_load_explicit(&waiter->flags, memory_order_relaxed) &
      QTHREAD_UNSTEALABLE) {
    qt_threadqueue_enqueue(waiter->rdata->shepherd_ptr->ready, waiter);
  } else {
    qt_threadqueue_enqueue(shep->ready, waiter);
  }
}

static inline void qthread_syncvar_remove(void *maddr) {
  int const lockbin = QTHREAD_CHOOSE_STRIPE(maddr);
  qthread_addrstat_t *m;

#ifdef LOCK_FREE_FEBS
  {
    qthread_addrstat_t *m2;
    m = qt_hash_get(syncvars[lockbin], (void *)maddr);
  got_m:
    if (!m) { return; }
    hazardous_ptr(0, m);
    if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)maddr))) {
      m = m2;
      goto got_m;
    }
    if (!m->valid) { return; }
    QTHREAD_FASTLOCK_LOCK(&m->lock);
    if (!m->valid) { return; }
    if ((m->FEQ == NULL) && (m->EFQ == NULL) && (m->FFQ == NULL)) {
      m->valid = 0;
      qassertnot(qt_hash_remove(syncvars[lockbin], (void *)maddr), 0);
    } else {
      QTHREAD_FASTLOCK_UNLOCK(&m->lock);
      return;
    }
  }
  assert(m);
#else  /* ifdef LOCK_FREE_FEBS */
  qt_hash_lock(syncvars[lockbin]);
  m =
    (qthread_addrstat_t *)qt_hash_get_locked(syncvars[lockbin], (void *)maddr);
  if (m) {
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
    if ((m->FEQ == NULL) && (m->EFQ == NULL) && (m->FFQ == NULL)) {
      qt_hash_remove_locked(syncvars[lockbin], (void *)maddr);
    } else {
      QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
      m = NULL;
    }
  }
  qt_hash_unlock(syncvars[lockbin]);
#endif /* ifdef LOCK_FREE_FEBS */
  if (m != NULL) {
    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
#ifdef LOCK_FREE_FEBS
    hazardous_release_node((hazardous_free_f)qthread_addrstat_delete, m);
#else
    qthread_addrstat_delete(m);
#endif
  }
}

static inline void qthread_syncvar_gotlock_empty(qthread_shepherd_t *shep,
                                                 qthread_addrstat_t *m,
                                                 syncvar_t *maddr,
                                                 uint64_t const sf) {
  qthread_addrres_t *X = NULL;
  int removeable;

  m->full = 0;
  if (m->EFQ != NULL) {
    /* dequeue one EFQ, do its operation, and schedule the thread */
    X = m->EFQ;
    m->EFQ = X->next;
    /* op */
    if (maddr && (maddr != (syncvar_t *)X->addr)) {
      UNLOCK_THIS_MODIFIED_SYNCVAR(maddr, *((uint64_t *)X->addr), sf);
    }
    qthread_syncvar_schedule(X->waiter, shep);
    FREE_ADDRRES(X);
  }
  if ((m->EFQ == NULL) && (m->FEQ == NULL) && (m->FFQ == NULL)) {
    removeable = 1;
  } else {
    removeable = 0;
  }
  QTHREAD_FASTLOCK_UNLOCK(&m->lock);
  if (removeable) { qthread_syncvar_remove(maddr); }
}

static inline void qthread_syncvar_gotlock_fill(qthread_shepherd_t *shep,
                                                qthread_addrstat_t *m,
                                                syncvar_t *maddr,
                                                uint64_t const ret) {
  qthread_addrres_t *X = NULL;
  int removeable;

  m->full = 1;
  /* dequeue all FFQ, do their operation, and schedule them */
  while (m->FFQ != NULL) {
    /* dQ */
    X = m->FFQ;
    m->FFQ = X->next;
    /* op */
    if (X->addr) { *(uint64_t *)X->addr = ret; }
    /* schedule */
    qthread_syncvar_schedule(X->waiter, shep);
    FREE_ADDRRES(X);
  }
  if (m->FEQ != NULL) {
    /* dequeue one FEQ, do their operation, and reschedule them */
    X = m->FEQ;
    m->FEQ = X->next;
    /* op */
    if (X->addr) { *(uint64_t *)X->addr = ret; }
    qthread_syncvar_schedule(X->waiter, shep);
    FREE_ADDRRES(X);
  }
  if ((m->EFQ == NULL) && (m->FEQ == NULL) && (m->FFQ == NULL)) {
    removeable = 1;
  } else {
    removeable = 0;
  }
  QTHREAD_FASTLOCK_UNLOCK(&m->lock);
  if (removeable) { qthread_syncvar_remove(maddr); }
}

int API_FUNC qthread_syncvar_writeF(syncvar_t *restrict dest,
                                    uint64_t const *restrict src) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t ret = *src;
  qthread_shepherd_t *shep = qthread_internal_getshep();

  qassert_ret((*src >> 60) == 0, QTHREAD_OVERFLOW);

  if (!shep) {
    return qthread_syncvar_nonblocker_func(dest, (void *)src, WRITEF);
  }
  qthread_mwaitc(dest, SYNCFEB_ANY, INT_MAX, &e);
  qassert_ret(e.cf == 0u,
              QTHREAD_TIMEOUT);       /* there better not have been a timeout */
  if ((e.pf == 1u) && (e.sf == 1u)) { /* there are waiters to release */
    int const lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_addrstat_t *m;

    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    e.sf = 0u; // I'm releasing waiters
    e.pf = 0u; // I'm going to mark this as full
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)dest, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)dest))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else  /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    assert(m); // otherwise there weren't really any waiters
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif /* ifdef LOCK_FREE_FEBS */
    if (m->FEQ) {
      e.pf = 1u; // back to being empty
      if (m->FEQ->next) {
        e.sf = 1u; // only one will be dequeued, so it'll still have waiters
      }
    }
    UNLOCK_THIS_MODIFIED_SYNCVAR(dest, ret, (e.pf << 1u) | e.sf);
    assert(m->FFQ || m->FEQ); // otherwise there weren't really any waiters
    assert(m->EFQ == NULL);   // someone snuck in!
    qthread_syncvar_gotlock_fill(shep, m, dest, ret);
  } else {
    UNLOCK_THIS_MODIFIED_SYNCVAR(dest, ret, 0);
  }

  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_writeF_const(syncvar_t *restrict dest,
                                          uint64_t const src) {
  assert(qthread_library_initialized);
  return qthread_syncvar_writeF(dest, &src);
}

int API_FUNC qthread_syncvar_writeEF(syncvar_t *restrict dest,
                                     uint64_t const *restrict src) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  int const lockbin = QTHREAD_CHOOSE_STRIPE(dest);
  qthread_t *me = qthread_internal_self();

  qassert_ret((*src >> 60) == 0, QTHREAD_OVERFLOW);

  if (!me) { return qthread_syncvar_blocker_func(dest, (void *)src, WRITEEF); }
  (void)qthread_mwaitc(dest, SYNCFEB_EMPTY, INITIAL_TIMEOUT, &e);
  if (e.cf) { /* there was a timeout */
    qthread_addrstat_t *m;
    qthread_addrres_t *X;

    uint64_t ret = qthread_mwaitc(dest, SYNCFEB_ANY, INT_MAX, &e);
    qassert_ret(e.cf == 0u,
                QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if (e.pf == 1u) {             /* it got empty! */
      if (e.sf == 1u) {           /* not just empty, but with waiters! */
        goto locked_empty_waiters;
      } else {
        goto locked_empty;
      }
    }
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)dest, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)dest))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else  /* ifdef LOCK_FREE_FEBS */
    qt_hash_lock(syncvars[lockbin]);
    m =
      (qthread_addrstat_t *)qt_hash_get_locked(syncvars[lockbin], (void *)dest);
    if (!m) {
      m = qthread_addrstat_new();
      assert(m);
      if (!m) { return QTHREAD_MALLOC_ERROR; }
      qassertnot(qt_hash_put_locked(syncvars[lockbin], (void *)dest, m), 0);
    }
    /* lock the addrstat (already within the hash) and then unlock the syncvar
     */
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
    qt_hash_unlock(syncvars[lockbin]);
#endif /* ifdef LOCK_FREE_FEBS */
    UNLOCK_THIS_MODIFIED_SYNCVAR(dest, ret, SYNCFEB_STATE_FULL_WITH_WAITERS);
    X = ALLOC_ADDRRES();
    assert(X);
    X->addr = (aligned_t *)src;
    X->waiter = me;
    X->next = m->EFQ;
    m->EFQ = X;
    atomic_store_explicit(
      &me->thread_state, QTHREAD_STATE_FEB_BLOCKED, memory_order_relaxed);
    me->rdata->blockedon.addr = m;
    qthread_back_to_master(me);
  } else if (e.sf == 1u) { /* there are waiters to release! */
    qthread_addrstat_t *m;

  locked_empty_waiters:
    assert(e.pf == 1u); // otherwise it wasn't really empty
    e.pf = 0u;          // mark full
    e.sf = 0u;          // released!
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    got_m2:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)dest, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)dest))) {
          m = m2;
          goto got_m2;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else                         /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    assert(m); // otherwise there weren't really any waiters
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif                        /* ifdef LOCK_FREE_FEBS */
    assert(m->FFQ || m->FEQ); // otherwise there weren't really any waiters
    assert(m->EFQ == NULL);   // someone snuck in!
    if (m->FEQ) {
      e.pf = 1u;
      if (m->FEQ->next) { e.sf = 1u; }
    }
    {
      uint64_t val = *src;
      UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, (e.pf << 1u) | e.sf);
      qthread_syncvar_gotlock_fill(me->rdata->shepherd_ptr, m, dest, val);
    }
  } else {
    uint64_t val;
  locked_empty:
    assert(e.pf == 1u); // otherwise it wasn't really empty
    assert(e.sf == 0u);
    val = *src;
    // e.pf = 0u;                    // now mark it full
    UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, SYNCFEB_STATE_FULL_NO_WAITERS);
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_writeEF_const(syncvar_t *restrict dest,
                                           uint64_t const src) {
  assert(qthread_library_initialized);
  return qthread_syncvar_writeEF(dest, &src);
}

int qthread_syncvar_writeEF_nb(syncvar_t *restrict dest,
                               uint64_t const *restrict src) {
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  int const lockbin = QTHREAD_CHOOSE_STRIPE(dest);
  qthread_t *me = qthread_internal_self();

  qassert_ret((*src >> 60) == 0, QTHREAD_OVERFLOW);

  if (!me) {
    return qthread_syncvar_blocker_func(dest, (void *)src, WRITEEF_NB);
  }
  (void)qthread_mwaitc(dest, SYNCFEB_EMPTY, 1, &e);
  if (e.cf) { /* there was a timeout */
    return QTHREAD_OPFAIL;
  } else if (e.sf == 1u) { /* there are waiters to release! */
    qthread_addrstat_t *m;

    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    assert(e.pf == 1u); // otherwise it wasn't really empty
    e.pf = 0u;          // mark full
    e.sf = 0u;          // released!
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)dest, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)dest))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else                         /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)dest);
    assert(m); // otherwise there weren't really any waiters
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif                        /* ifdef LOCK_FREE_FEBS */
    assert(m->FFQ || m->FEQ); // otherwise there weren't really any waiters
    assert(m->EFQ == NULL);   // someone snuck in!
    if (m->FEQ) {
      e.pf = 1u;
      if (m->FEQ->next) { e.sf = 1u; }
    }
    {
      uint64_t val = *src;
      UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, (e.pf << 1u) | e.sf);
      qthread_syncvar_gotlock_fill(me->rdata->shepherd_ptr, m, dest, val);
    }
  } else {
    uint64_t val;
    assert(e.pf == 1u); // otherwise it wasn't really empty
    assert(e.sf == 0u);
    val = *src;
    // e.pf = 0u;                    // now mark it full
    UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, SYNCFEB_STATE_FULL_NO_WAITERS);
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_syncvar_writeEF_const_nb(syncvar_t *restrict dest,
                                              uint64_t const src) {
  return qthread_syncvar_writeEF_nb(dest, &src);
}

uint64_t API_FUNC qthread_syncvar_incrF(syncvar_t *restrict operand,
                                        uint64_t const inc) {
  assert(qthread_library_initialized);
  eflags_t e = {0u, 0u, 0u, 0u, 0u};
  uint64_t newv;
  qthread_t *me = qthread_internal_self();

  assert(operand);
  if (!me) { return qthread_syncvar_blocker_func(operand, (void *)&inc, INCR); }
  qthread_mwaitc(operand, SYNCFEB_ANY, INT_MAX, &e);
  qassert_ret(
    e.cf == 0u,
    (uint64_t)QTHREAD_TIMEOUT);       /* there better not have been a timeout */
  if ((e.pf == 1u) && (e.sf == 1u)) { /* there are waiters to release */
    int const lockbin = QTHREAD_CHOOSE_STRIPE(operand);
    qthread_addrstat_t *m;

    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    e.sf = 0u; // I'm releasing waiters
    e.pf = 0u; // I'm going to mark this as full
#ifdef LOCK_FREE_FEBS
    do {
      m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)operand);
    got_m:
      if (!m) {
        m = qthread_addrstat_new();
        if (!m) { return QTHREAD_MALLOC_ERROR; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        qassertnot(qt_hash_put(syncvars[lockbin], (void *)operand, m), 0);
      } else {
        qthread_addrstat_t *m2;
        hazardous_ptr(0, m);
        if (m != (m2 = qt_hash_get(syncvars[lockbin], (void *)operand))) {
          m = m2;
          goto got_m;
        }
        if (!m->valid) { continue; }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
        if (!m->valid) {
          QTHREAD_FASTLOCK_UNLOCK(&m->lock);
          continue;
        }
      }
      break;
    } while (1);
#else  /* ifdef LOCK_FREE_FEBS */
    /* Note that locking the hash table is unnecessary because we have
     * locked the syncvar itself. */
    m = (qthread_addrstat_t *)qt_hash_get(syncvars[lockbin], (void *)operand);
    assert(m); // otherwise there weren't really any waiters
    QTHREAD_FASTLOCK_LOCK(&(m->lock));
#endif /* ifdef LOCK_FREE_FEBS */
    if (m->FEQ) {
      e.pf = 1u; // back to being empty
      if (m->FEQ->next) {
        e.sf = 1u; // only one will be dequeued, so it'll still have waiters
      }
    }
    newv = operand->u.s.data + inc;
    UNLOCK_THIS_MODIFIED_SYNCVAR(operand, newv, (e.pf << 1u) | e.sf);
    assert(m->FFQ || m->EFQ); // otherwise there weren't really any waiters
    assert(m->FEQ == NULL);   // someone snuck in!
    qthread_syncvar_gotlock_fill(me->rdata->shepherd_ptr, m, operand, newv);
  } else {
    newv = operand->u.s.data + inc;
    UNLOCK_THIS_MODIFIED_SYNCVAR(operand, newv, (e.pf << 1u) | e.sf);
  }

  return newv;
}

static filter_code qt_syncvar_tf_call_cb(qt_key_t const addr,
                                         qthread_t *restrict const waiter,
                                         void *restrict tf_arg) {
  qt_syncvar_callback_f f = (qt_syncvar_callback_f)((void **)tf_arg)[0];
  void *f_arg = ((void **)tf_arg)[1];
  void *tls;

  if (waiter->rdata->tasklocal_size <= qlib->qthread_tasklocal_size) {
    if (atomic_load_explicit(&waiter->flags, memory_order_relaxed) &
        QTHREAD_BIG_STRUCT) {
      tls = &waiter->data[qlib->qthread_argcopy_size];
    } else {
      tls = waiter->data;
    }
  } else {
    if (atomic_load_explicit(&waiter->flags, memory_order_relaxed) &
        QTHREAD_BIG_STRUCT) {
      tls = *(void **)&waiter->data[qlib->qthread_argcopy_size];
    } else {
      tls = *(void **)&waiter->data[0];
    }
  }
  f((void *)addr,
    waiter->f,
    waiter->arg,
    waiter->ret,
    waiter->thread_id,
    tls,
    f_arg);
  return IGNORE_AND_CONTINUE;
}

static void
qt_syncvar_call_tf(qt_key_t const addr, qthread_addrstat_t *m, void *arg) {
  qt_syncvar_taskfilter_f tf = (qt_syncvar_taskfilter_f)((void **)arg)[0];
  void *const f_arg = ((void **)arg)[1];
  uintptr_t const sync = (uintptr_t)(((void **)arg)[2]);

  if (sync) { QTHREAD_FASTLOCK_LOCK(&m->lock); }
  for (int i = 0; i < 3; i++) {
    qthread_addrres_t *curs, **base;
    switch (i) {
      case 0:
        curs = m->EFQ;
        base = &m->EFQ;
        break;
      case 1:
        curs = m->FEQ;
        base = &m->FEQ;
        break;
      case 2:
        curs = m->FFQ;
        base = &m->FFQ;
        break;
    }
    for (; curs != NULL; curs = curs->next) {
      qthread_t *waiter = curs->waiter;
      switch (tf(addr, waiter, f_arg)) {
        case 0: // ignore, move to the next one
          base = &curs->next;
          break;
        case 2: // remove, move to the next one
        {
          *base = curs->next;
          FREE_ADDRRES(curs);
          break;
        }
        default: QTHREAD_TRAP();
      }
    }
  }
  if (sync) { QTHREAD_FASTLOCK_UNLOCK(&m->lock); }
}

void INTERNAL qthread_syncvar_taskfilter_serial(qt_syncvar_taskfilter_f tf,
                                                void *arg) {
  void *pass[3] = {tf, arg, NULL};

  for (unsigned int i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
    qt_hash_callback(
      syncvars[i], (qt_hash_callback_fn)qt_syncvar_call_tf, pass);
  }
}

void INTERNAL qthread_syncvar_taskfilter(qt_syncvar_taskfilter_f tf,
                                         void *arg) {
  void *pass[3] = {tf, arg, (void *)(uintptr_t)1};

  for (unsigned int i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
    qt_hash_callback(
      syncvars[i], (qt_hash_callback_fn)qt_syncvar_call_tf, pass);
  }
}

void API_FUNC qthread_syncvar_callback(qt_syncvar_callback_f cb, void *arg) {
  void *pass[2] = {cb, arg};

  qthread_syncvar_taskfilter(qt_syncvar_tf_call_cb, pass);
}

/* vim:set expandtab: */
