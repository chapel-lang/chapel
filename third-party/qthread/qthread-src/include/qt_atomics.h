/* Portions of this file are Copyright (c) 2025 Tactical Computing Labs, LLC;
 * see COPYING */

#ifndef QT_ATOMICS_H
#define QT_ATOMICS_H

#include <pthread.h>

#include <stdatomic.h>
#include <sys/time.h>

#include <qthread/common.h>
#include <qthread/qthread.h>

#define THREAD_FENCE_MEM_ACQUIRE_IMPL atomic_thread_fence(memory_order_acquire)
#define THREAD_FENCE_MEM_RELEASE_IMPL atomic_thread_fence(memory_order_release)

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) ||                                 \
  (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
#define THREAD_FENCE_MEM_ACQUIRE
#define THREAD_FENCE_MEM_RELEASE
#else
#define THREAD_FENCE_MEM_ACQUIRE THREAD_FENCE_MEM_ACQUIRE_IMPL
#define THREAD_FENCE_MEM_RELEASE THREAD_FENCE_MEM_RELEASE_IMPL
#endif

#if QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32 ||                                   \
  QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64
#define SPINLOCK_BODY()                                                        \
  do { __asm__ __volatile__("pause" ::: "memory"); } while (0)
#elif QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM ||                                  \
  QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64
#define SPINLOCK_BODY()                                                        \
  do { __asm__ __volatile__("yield" ::: "memory"); } while (0)
#elif QTHREAD_ASSEMBLY_ARCH == QTHREAD_RISCV
#define SPINLOCK_BODY()                                                        \
  do { __asm__ __volatile__(".word 0x0100000F" ::: "memory"); } while (0)
#elif QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64 ||                            \
  QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32
// For whatever reason the 29 (mdoio) version of this instruction performed
// better in some back-of-the-envelope benchmarking so we're using that.
#define SPINLOCK_BODY()                                                        \
  do { __asm__ __volatile__("or 29,29,29" ::: "memory"); } while (0)
#else
#define SPINLOCK_BODY()                                                        \
  do { atomic_thread_fence(memory_order_acq_rel); } while (0)
#endif

#define QTHREAD_FASTLOCK_SETUP()                                               \
  do {                                                                         \
  } while (0)
#define QTHREAD_FASTLOCK_ATTRVAR

typedef struct qt_spin_exclusive_s { /* added to allow fast critical section
                                        ordering */
  aligned_t _Atomic enter; /* and not call pthreads spin_lock -- hard to debug
                            */
  aligned_t _Atomic exit;  /* near the lock under gdb -- 4/1/11 akp */
} qt_spin_exclusive_t;

void qt_spin_exclusive_lock(qt_spin_exclusive_t *);
void qt_spin_exclusive_unlock(qt_spin_exclusive_t *);
#define QTHREAD_FASTLOCK_INIT(x)                                               \
  {                                                                            \
    atomic_store_explicit(&(x).enter, 0u, memory_order_release);               \
    atomic_store_explicit(&(x).exit, 0u, memory_order_release);                \
  }
#define QTHREAD_FASTLOCK_INIT_PTR(x)                                           \
  {                                                                            \
    atomic_store_explicit(&(x)->enter, 0u, memory_order_release);              \
    atomic_store_explicit(&(x)->exit, 0u, memory_order_release);               \
  }
#define QTHREAD_FASTLOCK_LOCK(x)                                               \
  {                                                                            \
    aligned_t val =                                                            \
      atomic_fetch_add_explicit(&(x)->enter, 1u, memory_order_relaxed);        \
    while (val != atomic_load_explicit(&(x)->exit, memory_order_acquire))      \
      SPINLOCK_BODY(); /* spin waiting for turn */                             \
  }
#define QTHREAD_FASTLOCK_UNLOCK(x)                                             \
  do {                                                                         \
    atomic_fetch_add_explicit(                                                 \
      &(x)->exit, 1u, memory_order_release); /* notify next waiting thread */  \
  } while (0)
#define QTHREAD_FASTLOCK_DESTROY(x)
#define QTHREAD_FASTLOCK_DESTROY_PTR(x)
#define QTHREAD_FASTLOCK_TYPE qt_spin_exclusive_t
#define QTHREAD_FASTLOCK_INITIALIZER (qt_spin_exclusive_t){0, 0}

// Trylock declarations

#define QTHREAD_TRYLOCK_TYPE qt_spin_trylock_t
#define QTHREAD_TRYLOCK_INIT(x)                                                \
  { (x).u = 0; }
#define QTHREAD_TRYLOCK_INIT_PTR(x)                                            \
  { (x)->u = 0; }
#define QTHREAD_TRYLOCK_LOCK(x)                                                \
  {                                                                            \
    uint32_t val =                                                             \
      atomic_fetch_add_explicit(&(x)->s.users, 1, memory_order_relaxed);       \
    while (val != atomic_load_explicit((_Atomic uint32_t *)&(x)->s.ticket,     \
                                       memory_order_acquire))                  \
      SPINLOCK_BODY();                                                         \
  }
#define QTHREAD_TRYLOCK_UNLOCK(x)                                              \
  do {                                                                         \
    atomic_fetch_add_explicit(&(x)->s.ticket, 1, memory_order_release);        \
  } while (0)
#define QTHREAD_TRYLOCK_DESTROY(x)
#define QTHREAD_TRYLOCK_DESTROY_PTR(x)

static inline int QTHREAD_TRYLOCK_TRY(qt_spin_trylock_t *x) {
  qt_spin_trylock_t newcmp, cmp;
  uint64_t tmp =
    atomic_load_explicit((_Atomic uint64_t *)x, memory_order_relaxed);
  cmp = *(qt_spin_trylock_t *)&tmp;

  if (cmp.s.users != cmp.s.ticket) { return 0; }

  newcmp = cmp;
  newcmp.s.users = newcmp.s.ticket + 1;

  if (qthread_cas(&(x->u), cmp.u, newcmp.u) == cmp.u) { return 1; }
  return 0;
}

#define QTHREAD_COND_DECL(c)                                                   \
  pthread_cond_t c;                                                            \
  pthread_mutex_t c##_lock
#define QTHREAD_COND_INIT_SOLO(c)                                              \
  do {                                                                         \
    {                                                                          \
      pthread_mutexattr_t tmp_attr;                                            \
      qassert(pthread_mutexattr_init(&tmp_attr), 0);                           \
      qassert(                                                                 \
        pthread_mutexattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0);  \
      qassert(pthread_mutexattr_destroy(&tmp_attr), 0);                        \
    }                                                                          \
    {                                                                          \
      pthread_condattr_t tmp_attr;                                             \
      qassert(pthread_condattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), \
              0);                                                              \
      qassert(pthread_cond_init(&(c), &tmp_attr), 0);                          \
      qassert(pthread_condattr_destroy(&tmp_attr), 0);                         \
    }                                                                          \
  } while (0)
#define QTHREAD_COND_INIT_SOLO_PTR(c)                                          \
  do {                                                                         \
    {                                                                          \
      pthread_mutexattr_t tmp_attr;                                            \
      qassert(pthread_mutexattr_init(&tmp_attr), 0);                           \
      qassert(                                                                 \
        pthread_mutexattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0);  \
      qassert(pthread_mutexattr_destroy(&tmp_attr), 0);                        \
    }                                                                          \
    {                                                                          \
      pthread_condattr_t tmp_attr;                                             \
      qassert(pthread_condattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), \
              0);                                                              \
      qassert(pthread_cond_init((c), &tmp_attr), 0);                           \
      qassert(pthread_condattr_destroy(&tmp_attr), 0);                         \
    }                                                                          \
  } while (0)
#define QTHREAD_COND_INIT(c)                                                   \
  do {                                                                         \
    {                                                                          \
      pthread_mutexattr_t tmp_attr;                                            \
      qassert(pthread_mutexattr_init(&tmp_attr), 0);                           \
      qassert(                                                                 \
        pthread_mutexattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0);  \
      qassert(pthread_mutex_init(&(c##_lock), &tmp_attr), 0);                  \
      qassert(pthread_mutexattr_destroy(&tmp_attr), 0);                        \
    }                                                                          \
    {                                                                          \
      pthread_condattr_t tmp_attr;                                             \
      pthread_condattr_init(&tmp_attr);                                        \
      qassert(pthread_condattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), \
              0);                                                              \
      qassert(pthread_cond_init(&(c), &tmp_attr), 0);                          \
      qassert(pthread_condattr_destroy(&tmp_attr), 0);                         \
    }                                                                          \
  } while (0)
#define QTHREAD_COND_LOCK(c) qassert(pthread_mutex_lock(&(c##_lock)), 0)
#define QTHREAD_COND_UNLOCK(c) qassert(pthread_mutex_unlock(&(c##_lock)), 0)
#define QTHREAD_INITLOCK(l)                                                    \
  do {                                                                         \
    if (pthread_mutex_init(l, NULL) != 0) { return QTHREAD_PTHREAD_ERROR; }    \
  } while (0)
#define QTHREAD_LOCK(l) qassert(pthread_mutex_lock(l), 0)
#define QTHREAD_UNLOCK(l) qassert(pthread_mutex_unlock(l), 0)
#define QTHREAD_DESTROYLOCK(l) qassert(pthread_mutex_destroy(l), 0)
#define QTHREAD_DESTROYCOND(l) qassert(pthread_cond_destroy(l), 0)
#define QTHREAD_COND_SIGNAL(c) qassert(pthread_cond_signal(&(c)), 0)
#define QTHREAD_COND_BCAST(l) qassert(pthread_cond_broadcast(&(l)), 0)
#define QTHREAD_COND_DESTROY(c)                                                \
  do {                                                                         \
    qassert(pthread_cond_destroy(&(c)), 0);                                    \
    qassert(pthread_mutex_destroy(&(c##_lock)), 0);                            \
  } while (0)
#ifdef NDEBUG
#define QTHREAD_COND_WAIT(c)                                                   \
  do {                                                                         \
    struct timespec t;                                                         \
    struct timeval n;                                                          \
    gettimeofday(&n, NULL);                                                    \
    t.tv_nsec = (n.tv_usec * 1000) + 500000000;                                \
    t.tv_sec = n.tv_sec + ((t.tv_nsec >= 1000000000) ? 1 : 0);                 \
    t.tv_nsec -= ((t.tv_nsec >= 1000000000) ? 1000000000 : 0);                 \
    pthread_cond_timedwait(&(c), &(c##_lock), &t);                             \
  } while (0)
#else
#define QTHREAD_COND_WAIT(c)                                                   \
  do {                                                                         \
    struct timespec t;                                                         \
    struct timeval n;                                                          \
    gettimeofday(&n, NULL);                                                    \
    t.tv_nsec = (n.tv_usec * 1000) + 500000000;                                \
    t.tv_sec = n.tv_sec + ((t.tv_nsec >= 1000000000) ? 1 : 0);                 \
    t.tv_nsec -= ((t.tv_nsec >= 1000000000) ? 1000000000 : 0);                 \
    int val = pthread_cond_timedwait(&(c), &(c##_lock), &t);                   \
    assert(!(val == EINVAL || val == EPERM));                                  \
  } while (0)
#endif
#define QTHREAD_COND_WAIT_DUO(c, m)                                            \
  do {                                                                         \
    struct timespec t;                                                         \
    struct timeval n;                                                          \
    gettimeofday(&n, NULL);                                                    \
    t.tv_nsec = (n.tv_usec * 1000) + 500000000;                                \
    t.tv_sec = n.tv_sec + ((t.tv_nsec >= 1000000000) ? 1 : 0);                 \
    t.tv_nsec -= ((t.tv_nsec >= 1000000000) ? 1000000000 : 0);                 \
    qassert(pthread_cond_timedwait(&(c), &(m), &t), 0);                        \
  } while (0)

#define QTHREAD_CASLOCK(var) (var)
#define QTHREAD_CASLOCK_STATIC(var) (var)
#define QTHREAD_CASLOCK_EXPLICIT_DECL(name)
#define QTHREAD_CASLOCK_EXPLICIT_INIT(name)
#define QTHREAD_CASLOCK_INIT(var, i) (var) = i
#define QTHREAD_CASLOCK_DESTROY(var)
#define QTHREAD_CASLOCK_READ(var) (var)
#define QTHREAD_CASLOCK_READ_UI(var) (var)
#define QT_CAS(var, oldv, newv)                                                \
  qt_cas((void **)&(var), (void *)(oldv), (void *)(newv))
#define QT_CAS_(var, oldv, newv, caslock)                                      \
  qt_cas((void **)&(var), (void *)(oldv), (void *)(newv))
#define qt_cas(P, O, N) (void *)__sync_val_compare_and_swap((P), (O), (N))

#define qthread_internal_atomic_read_s(op, lock) (*op)
#define qthread_internal_incr(op, lock, val) qthread_incr(op, val)
#define qthread_internal_incr_s(op, lock, val) qthread_incr(op, val)
#define qthread_internal_decr(op, lock) qthread_incr(op, -1)
#define qthread_internal_incr_mod(op, m, lock) qthread_internal_incr_mod_(op, m)
#define QTHREAD_OPTIONAL_LOCKARG

static inline aligned_t
qthread_internal_incr_mod_(aligned_t *operand,
                           unsigned int const max QTHREAD_OPTIONAL_LOCKARG) {
  aligned_t retval;

#if QTHREAD_BITS == 32
  uint32_t oldval, newval;

  newval = *operand;
  do {
    retval = oldval = newval;
    newval = oldval + 1;
    newval *= (newval < max);

    newval = __sync_val_compare_and_swap((uint32_t *)operand, oldval, newval);
  } while (oldval != newval);
#elif QTHREAD_BITS == 64
  uint64_t oldval, newval;

  newval = *operand;
  do {
    retval = oldval = newval;
    newval = oldval + 1;
    newval *= (newval < max);

    newval = __sync_val_compare_and_swap((uint64_t *)operand, oldval, newval);
  } while (oldval != newval);
#else /* if QTHREAD_BITS == 32 */
#error "Unrecognized bit-ness"
#endif /* if QTHREAD_BITS == 32 */

  return retval;
}

static inline void *qt_internal_atomic_swap_ptr(void **addr, void *newval) {
  void *oldval =
    atomic_load_explicit((void *_Atomic *)addr, memory_order_relaxed);
  void *tmp;

  while ((tmp = qthread_cas_ptr(addr, oldval, newval)) != oldval) {
    oldval = tmp;
  }
  return oldval;
}

#endif /* ifndef QT_ATOMICS_H */

/* vim:set expandtab */
