#ifndef QT_ATOMICS_H
#define QT_ATOMICS_H

#include <sys/time.h>

#include <qthread/common.h>
#include <qthread/qthread.h>

#ifdef QTHREAD_NEEDS_IA64INTRIN
# ifdef HAVE_IA64INTRIN_H
#  include <ia64intrin.h>
# elif defined(HAVE_IA32INTRIN_H)
#  include <ia32intrin.h>
# endif
#endif

#ifdef QTHREAD_OVERSUBSCRIPTION
# ifdef HAVE_PTHREAD_YIELD
#  define SPINLOCK_BODY() do { COMPILER_FENCE; pthread_yield(); } while (0)
# elif HAVE_SCHED_YIELD
#  include <sched.h> /* for sched_yield(); */
#  define SPINLOCK_BODY() do { COMPILER_FENCE; sched_yield(); } while (0)
# else
#  error Cannot support efficient oversubscription on this platform.
# endif
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
# define SPINLOCK_BODY() do { COMPILER_FENCE; __asm__ __volatile__ ("pause" ::: "memory"); } while (0)
#else // ifdef QTHREAD_OVERSUBSCRIPTION
# define SPINLOCK_BODY() do { COMPILER_FENCE; } while (0)
#endif // ifdef QTHREAD_OVERSUBSCRIPTION

#if defined(__tile__)
# include <tmc/sync.h>
# define QTHREAD_FASTLOCK_ATTRVAR
# define QTHREAD_FASTLOCK_SETUP()     do { } while (0)
# define QTHREAD_FASTLOCK_INIT(x)     tmc_sync_mutex_init(&(x))
# define QTHREAD_FASTLOCK_INIT_PTR(x) tmc_sync_mutex_init((x))
# define QTHREAD_FASTLOCK_LOCK(x)     tmc_sync_mutex_lock((x))
# define QTHREAD_FASTLOCK_UNLOCK(x)   tmc_sync_mutex_unlock((x))
# define QTHREAD_FASTLOCK_DESTROY(x)
# define QTHREAD_FASTLOCK_DESTROY_PTR(x)
# define QTHREAD_FASTLOCK_TYPE        tmc_sync_mutex_t
# define QTHREAD_FASTLOCK_INITIALIZER TMC_SYNC_MUTEX_INIT
#elif defined(USE_INTERNAL_SPINLOCK) && USE_INTERNAL_SPINLOCK
# define QTHREAD_FASTLOCK_SETUP() do { } while (0)
# define QTHREAD_FASTLOCK_ATTRVAR
typedef struct qt_spin_exclusive_s { /* added to allow fast critical section ordering */
    aligned_t enter;                 /* and not call pthreads spin_lock -- hard to debug */
    aligned_t exit;                  /* near the lock under gdb -- 4/1/11 akp */
} qt_spin_exclusive_t;
void qt_spin_exclusive_lock(qt_spin_exclusive_t *);
void qt_spin_exclusive_unlock(qt_spin_exclusive_t *);
# define QTHREAD_FASTLOCK_INIT(x)     { (x).enter = 0; (x).exit = 0; }
# define QTHREAD_FASTLOCK_INIT_PTR(x) { (x)->enter = 0; (x)->exit = 0; }
# define QTHREAD_FASTLOCK_LOCK(x)     { aligned_t val = qthread_incr(&(x)->enter, 1); \
                                        while (val != (x)->exit) SPINLOCK_BODY(); /* spin waiting for my turn */ }
# define QTHREAD_FASTLOCK_UNLOCK(x)   do { COMPILER_FENCE; \
                                           (x)->exit++; /* allow next guy's turn */ } while (0)
# define QTHREAD_FASTLOCK_DESTROY(x)
# define QTHREAD_FASTLOCK_DESTROY_PTR(x)
# define QTHREAD_FASTLOCK_TYPE        qt_spin_exclusive_t
# define QTHREAD_FASTLOCK_INITIALIZER (qt_spin_exclusive_t) {0, 0 }
#elif defined(HAVE_PTHREAD_SPIN_INIT) && !defined(QTHREAD_OVERSUBSCRIPTION)
# include <pthread.h>
# define QTHREAD_FASTLOCK_ATTRVAR
# define QTHREAD_FASTLOCK_SETUP()        do { } while (0)
# define QTHREAD_FASTLOCK_INIT(x)        pthread_spin_init(&(x), PTHREAD_PROCESS_PRIVATE)
# define QTHREAD_FASTLOCK_INIT_PTR(x)    pthread_spin_init((x), PTHREAD_PROCESS_PRIVATE)
# define QTHREAD_FASTLOCK_LOCK(x)        pthread_spin_lock((x))
# define QTHREAD_FASTLOCK_UNLOCK(x)      pthread_spin_unlock((x))
# define QTHREAD_FASTLOCK_DESTROY(x)     pthread_spin_destroy(&(x))
# define QTHREAD_FASTLOCK_DESTROY_PTR(x) pthread_spin_destroy((x))
# define QTHREAD_FASTLOCK_TYPE        pthread_spinlock_t
# define QTHREAD_FASTLOCK_INITIALIZER PTHREAD_SPINLOCK_INITIALIZER
#else /* fallback */
# include <pthread.h>
# ifndef QTHREAD_FASTLOCK_ATTR
#  define QTHREAD_FASTLOCK_ATTR
extern pthread_mutexattr_t _fastlock_attr;
#  define QTHREAD_FASTLOCK_ATTRVAR pthread_mutexattr_t _fastlock_attr
# endif
# define QTHREAD_FASTLOCK_SETUP()        do {                                   \
        pthread_mutexattr_init(&_fastlock_attr);                                \
        pthread_mutexattr_setpshared(&_fastlock_attr, PTHREAD_PROCESS_PRIVATE); \
} while (0)
# define QTHREAD_FASTLOCK_INIT(x)        pthread_mutex_init(&(x), &_fastlock_attr)
# define QTHREAD_FASTLOCK_INIT_PTR(x)    pthread_mutex_init((x), &_fastlock_attr)
# define QTHREAD_FASTLOCK_LOCK(x)        pthread_mutex_lock((x))
# define QTHREAD_FASTLOCK_UNLOCK(x)      pthread_mutex_unlock((x))
# define QTHREAD_FASTLOCK_DESTROY(x)     pthread_mutex_destroy(&(x))
# define QTHREAD_FASTLOCK_DESTROY_PTR(x) pthread_mutex_destroy((x))
# define QTHREAD_FASTLOCK_TYPE        pthread_mutex_t
# define QTHREAD_FASTLOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#endif /* */

// Trylock declarations

#if defined(__tile__)
# include <tmc/sync.h>
# define QTHREAD_TRYLOCK_INIT(x)     tmc_sync_mutex_init(&(x))
# define QTHREAD_TRYLOCK_INIT_PTR(x) tmc_sync_mutex_init((x))
# define QTHREAD_TRYLOCK_LOCK(x)     tmc_sync_mutex_lock((x))
# define QTHREAD_TRYLOCK_TRY(x)      (tmc_sync_mutex_trylock((x)) == 0)
# define QTHREAD_TRYLOCK_UNLOCK(x)   tmc_sync_mutex_unlock((x))
# define QTHREAD_TRYLOCK_DESTROY(x)
# define QTHREAD_TRYLOCK_DESTROY_PTR(x)
# define QTHREAD_TRYLOCK_TYPE        tmc_sync_mutex_t
# define QTHREAD_TRYLOCK_INITIALIZER TMC_SYNC_MUTEX_INIT

/* For the followimg implementation of try-locks,
 * it is necessary that qthread_incr() be defined on
 * haligned_t types. This requirement is satisfied when
 * defined(QTHREAD_ATOMIC_INCR), it remains to be determined
 * whether it is satisfied in some circumstances when
 * !defined(QTHREAD_ATOMIC_INCR).
 */
#elif defined(USE_INTERNAL_SPINLOCK) && USE_INTERNAL_SPINLOCK \
    && defined(QTHREAD_ATOMIC_INCR)                           \
    && !defined(QTHREAD_MUTEX_INCREMENT)

typedef union qt_spin_trylock_s {
    aligned_t u;
    struct {
        haligned_t ticket;
        haligned_t users;
    } s;
} Q_ALIGNED(QTHREAD_ALIGNMENT_ALIGNED_T) qt_spin_trylock_t;

# define QTHREAD_TRYLOCK_TYPE qt_spin_trylock_t
# define QTHREAD_TRYLOCK_INIT(x)     { (x).u = 0; }
# define QTHREAD_TRYLOCK_INIT_PTR(x) { (x)->u = 0; }
# define QTHREAD_TRYLOCK_LOCK(x)     { uint32_t val = qthread_incr(&(x)->s.users, 1); \
                                       while (val != (x)->s.ticket) SPINLOCK_BODY(); /* spin waiting for my turn */ }
# define QTHREAD_TRYLOCK_UNLOCK(x)   do { COMPILER_FENCE;                                               \
                                          qthread_incr(&(x)->s.ticket, 1); /* allow next guy's turn */ \
} while (0)
# define QTHREAD_TRYLOCK_DESTROY(x)
# define QTHREAD_TRYLOCK_DESTROY_PTR(x)

static inline int QTHREAD_TRYLOCK_TRY(qt_spin_trylock_t *x)
{
    qt_spin_trylock_t newcmp, cmp = *(x);

    if (cmp.s.users != cmp.s.ticket) {
        return 0;
    }

    newcmp         = cmp;
    newcmp.s.users = newcmp.s.ticket + 1;
    return(qthread_cas(&(x->u), cmp.u, newcmp.u) == cmp.u);
}

#elif defined(HAVE_PTHREAD_SPIN_INIT) && !defined(QTHREAD_OVERSUBSCRIPTION)

# define QTHREAD_TRYLOCK_TYPE pthread_spinlock_t
# define QTHREAD_TRYLOCK_INIT(x)        pthread_spin_init(&(x), PTHREAD_PROCESS_PRIVATE)
# define QTHREAD_TRYLOCK_INIT_PTR(x)    pthread_spin_init((x), PTHREAD_PROCESS_PRIVATE)
# define QTHREAD_TRYLOCK_LOCK(x)        pthread_spin_lock((x))
# define QTHREAD_TRYLOCK_UNLOCK(x)      pthread_spin_unlock((x))
# define QTHREAD_TRYLOCK_DESTROY(x)     pthread_spin_destroy(&(x))
# define QTHREAD_TRYLOCK_DESTROY_PTR(x) pthread_spin_destroy((x))
# define QTHREAD_TRYLOCK_TRY(x)         (pthread_spin_trylock((x)) == 0)

#else /* fallback */

# include <pthread.h>
# ifndef QTHREAD_FASTLOCK_ATTR
#  define QTHREAD_FASTLOCK_ATTR
extern pthread_mutexattr_t _fastlock_attr;
#  ifdef QTHREAD_FASTLOCK_ATTRVAR
#   undef QTHREAD_FASTLOCK_ATTRVAR
#  endif
#  define QTHREAD_FASTLOCK_ATTRVAR pthread_mutexattr_t _fastlock_attr
# endif
# define QTHREAD_TRYLOCK_TYPE pthread_mutex_t
# define QTHREAD_TRYLOCK_INIT(x)        pthread_mutex_init(( & (x)), &_fastlock_attr)
# define QTHREAD_TRYLOCK_INIT_PTR(x)    pthread_mutex_init((x), &_fastlock_attr)
# define QTHREAD_TRYLOCK_LOCK(x)        pthread_mutex_lock((x))
# define QTHREAD_TRYLOCK_UNLOCK(x)      pthread_mutex_unlock((x))
# define QTHREAD_TRYLOCK_DESTROY(x)     pthread_mutex_destroy( & (x))
# define QTHREAD_TRYLOCK_DESTROY_PTR(x) pthread_mutex_destroy((x))
# define  QTHREAD_TRYLOCK_TRY(x)        (pthread_mutex_trylock((x)) == 0)

#endif // if defined(__tile__)

#include <pthread.h>
#define QTHREAD_COND_DECL(c)   pthread_cond_t c; pthread_mutex_t c ## _lock
#define QTHREAD_COND_INIT_SOLO(c) do { \
    { \
	pthread_mutexattr_t tmp_attr; \
	qassert(pthread_mutexattr_init(&tmp_attr), 0); \
	qassert(pthread_mutexattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0); \
	qassert(pthread_mutexattr_destroy(&tmp_attr), 0); \
    } { \
	pthread_condattr_t tmp_attr; \
	qassert(pthread_condattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0); \
	qassert(pthread_cond_init(&(c), &tmp_attr), 0); \
	qassert(pthread_condattr_destroy(&tmp_attr), 0); \
    } \
} while (0)
#define QTHREAD_COND_INIT_SOLO_PTR(c) do { \
    { \
	pthread_mutexattr_t tmp_attr; \
	qassert(pthread_mutexattr_init(&tmp_attr), 0); \
	qassert(pthread_mutexattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0); \
	qassert(pthread_mutexattr_destroy(&tmp_attr), 0); \
    } { \
	pthread_condattr_t tmp_attr; \
	qassert(pthread_condattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0); \
	qassert(pthread_cond_init((c), &tmp_attr), 0); \
	qassert(pthread_condattr_destroy(&tmp_attr), 0); \
    } \
} while (0)
#define QTHREAD_COND_INIT(c) do { \
    { \
	pthread_mutexattr_t tmp_attr; \
	qassert(pthread_mutexattr_init(&tmp_attr), 0); \
	qassert(pthread_mutexattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0); \
	qassert(pthread_mutex_init(&(c ## _lock), &tmp_attr), 0); \
	qassert(pthread_mutexattr_destroy(&tmp_attr), 0); \
    } { \
	pthread_condattr_t tmp_attr; \
	qassert(pthread_condattr_setpshared(&tmp_attr, PTHREAD_PROCESS_PRIVATE), 0); \
	qassert(pthread_cond_init(&(c), &tmp_attr), 0); \
	qassert(pthread_condattr_destroy(&tmp_attr), 0); \
    } \
} while (0)
#define QTHREAD_COND_LOCK(c)   qassert(pthread_mutex_lock(&(c ## _lock)), 0)
#define QTHREAD_COND_UNLOCK(c) qassert(pthread_mutex_unlock(&(c ## _lock)), 0)
#define QTHREAD_INITLOCK(l)    do { if (pthread_mutex_init(l, NULL) != 0) { return QTHREAD_PTHREAD_ERROR; } } while(0)
#define QTHREAD_LOCK(l)        qassert(pthread_mutex_lock(l), 0)
#define QTHREAD_UNLOCK(l)      qassert(pthread_mutex_unlock(l), 0)
#define QTHREAD_DESTROYLOCK(l) qassert(pthread_mutex_destroy(l), 0)
#define QTHREAD_DESTROYCOND(l) qassert(pthread_cond_destroy(l), 0)
#define QTHREAD_COND_SIGNAL(c) qassert(pthread_cond_signal(&(c)), 0)
#define QTHREAD_COND_BCAST(l)  qassert(pthread_cond_broadcast(&(l)), 0)
#define QTHREAD_COND_DESTROY(c) do { \
    qassert(pthread_cond_destroy(&(c)), 0); \
    qassert(pthread_mutex_destroy(&(c ## _lock)), 0); \
} while (0)
#define QTHREAD_COND_WAIT(c) do { \
    struct timespec t; \
    struct timeval n; \
    gettimeofday(&n, NULL); \
    t.tv_nsec = (n.tv_usec * 1000) + 500000000; \
    t.tv_sec = n.tv_sec + ((t.tv_nsec >= 1000000000)?1:0); \
    t.tv_nsec -= ((t.tv_nsec >= 1000000000)?1000000000:0); \
    qassert(pthread_cond_timedwait(&(c), &(c ## _lock), &t), 0); \
} while (0)
#define QTHREAD_COND_WAIT_DUO(c, m) do { \
    struct timespec t; \
    struct timeval n; \
    gettimeofday(&n, NULL); \
    t.tv_nsec = (n.tv_usec * 1000) + 500000000; \
    t.tv_sec = n.tv_sec + ((t.tv_nsec >= 1000000000)?1:0); \
    t.tv_nsec -= ((t.tv_nsec >= 1000000000)?1000000000:0); \
    qassert(pthread_cond_timedwait(&(c), &(m), &t), 0); \
} while (0)
#ifdef QTHREAD_MUTEX_INCREMENT
# define QTHREAD_CASLOCK(var)                var; QTHREAD_FASTLOCK_TYPE var ## _caslock
# define QTHREAD_CASLOCK_STATIC(var)         var; static QTHREAD_FASTLOCK_TYPE var ## _caslock
# define QTHREAD_CASLOCK_EXPLICIT_DECL(name) QTHREAD_FASTLOCK_TYPE name;
# define QTHREAD_CASLOCK_EXPLICIT_INIT(name) QTHREAD_FASTLOCK_INIT(name)
# define QTHREAD_CASLOCK_INIT(var, i)        var = i; QTHREAD_FASTLOCK_INIT(var ## _caslock)
# define QTHREAD_CASLOCK_DESTROY(var)        QTHREAD_FASTLOCK_DESTROY(var ## _caslock)
# define QTHREAD_CASLOCK_READ(var)           (void *)qt_cas_read_ui((uintptr_t *) & (var), & (var ## _caslock))
# define QTHREAD_CASLOCK_READ_UI(var)        qt_cas_read_ui((uintptr_t *) & (var), & (var ## _caslock))
# define QT_CAS(var, oldv, newv)             qt_cas((void **) & (var), (void *)(oldv), (void *)(newv), & (var ## _caslock))
# define QT_CAS_(var, oldv, newv, caslock)   qt_cas((void **) & (var), (void *)(oldv), (void *)(newv), & (caslock))
static QINLINE void *qt_cas(void **const           ptr,
                            void *const            oldv,
                            void *const            newv,
                            QTHREAD_FASTLOCK_TYPE *lock)
{
    void *ret;

    QTHREAD_FASTLOCK_LOCK(lock);
    ret = *ptr;
    if (*ptr == oldv) {
        *ptr = newv;
    }
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return ret;
}

static QINLINE uintptr_t qt_cas_read_ui(uintptr_t *const       ptr,
                                        QTHREAD_FASTLOCK_TYPE *mutex)
{
    uintptr_t ret;

    QTHREAD_FASTLOCK_LOCK(mutex);
    ret = *ptr;
    QTHREAD_FASTLOCK_UNLOCK(mutex);
    return ret;
}

#else /* ifdef QTHREAD_MUTEX_INCREMENT */
# define QTHREAD_CASLOCK(var)        (var)
# define QTHREAD_CASLOCK_STATIC(var) (var)
# define QTHREAD_CASLOCK_EXPLICIT_DECL(name)
# define QTHREAD_CASLOCK_EXPLICIT_INIT(name)
# define QTHREAD_CASLOCK_INIT(var, i) (var) = i
# define QTHREAD_CASLOCK_DESTROY(var)
# define QTHREAD_CASLOCK_READ(var)         (var)
# define QTHREAD_CASLOCK_READ_UI(var)      (var)
# define QT_CAS(var, oldv, newv)           qt_cas((void **) & (var), (void *)(oldv), (void *)(newv))
# define QT_CAS_(var, oldv, newv, caslock) qt_cas((void **) & (var), (void *)(oldv), (void *)(newv))
# ifdef QTHREAD_ATOMIC_CAS_PTR
#  define qt_cas(P, O, N) (void *)__sync_val_compare_and_swap((P), (O), (N))
# else
static QINLINE void *qt_cas(void **const ptr,
                            void *const  oldv,
                            void *const  newv)
{   /*{{{*/
#  if defined(HAVE_GCC_INLINE_ASSEMBLY)
#   if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    void *result;
    __asm__ __volatile__ ("A_%=:\n\t"
                          "lwarx  %0,0,%3\n\t"
                          "cmpw   %0,%1\n\t"
                          "bne    B_%=\n\t"
                          "stwcx. %2,0,%3\n\t"
                          "bne-   A_%=\n"
                          "B_%=:"
                          : "=&b" (result)
                          : "r" (oldv), "r" (newv), "r" (ptr)
                          : "cc", "memory");
    return result;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    void *result;
    __asm__ __volatile__ ("A_%=:\n\t"
                          "ldarx  %0,0,%3\n\t"
                          "cmpw   %0,%1\n\t"
                          "bne    B_%=\n\t"
                          "stdcx. %2,0,%3\n\t"
                          "bne-   A_%=\n"
                          "B_%=:"
                          : "=&b" (result)
                          : "r" (oldv), "r" (newv), "r" (ptr)
                          : "cc", "memory");
    return result;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    void *nv = newv;
    __asm__ __volatile__ ("cas [%1], %2, %0"
                          : "=&r" (nv)
                          : "r" (ptr), "r" (oldv)
#    if !defined(__SUNPRO_C) && !defined(__SUNPRO_CC)
                          , "0" (nv)
#    endif
                          : "cc", "memory");
    return nv;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    void *nv = newv;
    __asm__ __volatile__ ("casx [%1], %2, %0"
                          : "=&r" (nv)
                          : "r" (ptr), "r" (oldv)
#    if !defined(__SUNPRO_C) && !defined(__SUNPRO_CC)
                          , "0" (nv)
#    endif
                          : "cc", "memory");
    return nv;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    void **retval;
    __asm__ __volatile__ ("mov ar.ccv=%0;;" : : "rO" (oldv));
    __asm__ __volatile__ ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
                          : "=r" (retval)
                          : "r" (ptr), "r" (newv)
                          : "memory");
    return retval;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    void **retval;

    /* note that this is GNU/Linux syntax (aka AT&T syntax), not Intel syntax.
     * Thus, this instruction has the form:
     * [lock] cmpxchg reg, reg/mem
     *                src, dest
     *
     * NOTE: this is valid even on 64-bit architectures, because AMD64
     * instantiates cmpxchg for 8-byte registers, and IA32 never has 64-bit
     * pointers
     */
#    if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) && defined(__PGI)
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)\n\t"
                          "mov %%rax,(%0)"
                          ::"r" (&retval),
                          "r" (newv), "r" (ptr),
                          "a" (oldv) /* load into RAX */
                          : "cc", "memory");
#    else
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)"
                          : "=a" (retval) /* store from RAX */
                          : "r" (newv), "r" (ptr),
                          "a" (oldv) /* load into RAX */
                          : "cc", "memory");
#    endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) && defined(__PGI) */
    return retval;

#   else /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) */
#    error "Don't have a qt_cas implementation for this architecture"
#   endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) */
#  else /* if defined(HAVE_GCC_INLINE_ASSEMBLY) */
#   error "CAS needs inline assembly OR __sync_val_compare_and_swap"
#  endif /* if defined(HAVE_GCC_INLINE_ASSEMBLY) */
} /*}}}*/

# endif /* ATOMIC_CAS_PTR */
#endif  /* MUTEX_INCREMENT */

#ifndef QTHREAD_MUTEX_INCREMENT
# define qthread_internal_atomic_read_s(op, lock) (*op)
# define qthread_internal_incr(op, lock, val)     qthread_incr(op, val)
# define qthread_internal_incr_s(op, lock, val)   qthread_incr(op, val)
# define qthread_internal_decr(op, lock)          qthread_incr(op, -1)
# define qthread_internal_incr_mod(op, m, lock)   qthread_internal_incr_mod_(op, m)
# define QTHREAD_OPTIONAL_LOCKARG
#else
# define qthread_internal_incr_mod(op, m, lock) qthread_internal_incr_mod_(op, m, lock)
# define QTHREAD_OPTIONAL_LOCKARG , QTHREAD_FASTLOCK_TYPE * lock
static QINLINE aligned_t qthread_internal_incr(aligned_t             *operand,
                                               QTHREAD_FASTLOCK_TYPE *lock,
                                               int                    val)
{                                      /*{{{ */
    aligned_t retval;

    QTHREAD_FASTLOCK_LOCK(lock);
    retval    = *operand;
    *operand += val;
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return retval;
}                                      /*}}} */

static QINLINE saligned_t qthread_internal_incr_s(saligned_t            *operand,
                                                  QTHREAD_FASTLOCK_TYPE *lock,
                                                  int                    val)
{                                      /*{{{ */
    saligned_t retval;

    QTHREAD_FASTLOCK_LOCK(lock);
    retval    = *operand;
    *operand += val;
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return retval;
}                                      /*}}} */

static QINLINE saligned_t qthread_internal_atomic_read_s(saligned_t            *operand,
                                                         QTHREAD_FASTLOCK_TYPE *lock)
{                                      /*{{{ */
    saligned_t retval;

    QTHREAD_FASTLOCK_LOCK(lock);
    retval = *operand;
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return retval;
}                                      /*}}} */

#endif /* ifndef QTHREAD_MUTEX_INCREMENT */

static QINLINE aligned_t qthread_internal_incr_mod_(aligned_t             *operand,
                                                    const unsigned int max QTHREAD_OPTIONAL_LOCKARG)
{                                      /*{{{ */
    aligned_t retval;

#if QTHREAD_ATOMIC_CAS && (QTHREAD_SIZEOF_ALIGNED_T == 4)
    uint32_t oldval, newval;

    newval = *operand;
    do {
        retval  = oldval = newval;
        newval  = oldval + 1;
        newval *= (newval < max);

        newval = __sync_val_compare_and_swap((uint32_t *)operand, oldval, newval);
    } while (oldval != newval);
#elif QTHREAD_ATOMIC_CAS && (QTHREAD_SIZEOF_ALIGNED_T == 8)
    uint64_t oldval, newval;

    newval = *operand;
    do {
        retval  = oldval = newval;
        newval  = oldval + 1;
        newval *= (newval < max);

        newval = __sync_val_compare_and_swap((uint64_t *)operand, oldval, newval);
    } while (oldval != newval);
#elif defined(HAVE_GCC_INLINE_ASSEMBLY)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) && (QTHREAD_SIZEOF_ALIGNED_T == 4))

    unsigned int incrd = incrd;        /* these don't need to be initialized */
    unsigned int compd = compd;        /* they're just tmp variables */

    /* the minus in bne- means "this bne is unlikely to be taken" */
    asm volatile ("A_%=:\n\t"             /* local label */
                  "lwarx  %0,0,%3\n\t"    /* load operand */
                  "addi   %2,%0,1\n\t"    /* increment it into incrd */
                  "cmplw  7,%2,%4\n\t"    /* compare incrd to the max */
                  "mfcr   %1\n\t"         /* move the result into compd */
                  "rlwinm %1,%1,29,1\n\t" /* isolate the result bit */
                  "mullw  %2,%2,%1\n\t"   /* incrd *= compd */
                  "stwcx. %2,0,%3\n\t"    /* *operand = incrd */
                  "bne-   A_%=\n\t"       /* if it failed, go to label 1 back */
                  "isync"                 /* make sure it wasn't all a dream */

                  /* = means this operand is write-only (previous value is discarded)
                   * & means this operand is an earlyclobber (i.e. cannot use the same register as any of the input operands)
                   * b means this is a register but must not be r0 */
                  : "=&b"   (retval), "=&r" (compd), "=&r" (incrd)
                  : "r"     (operand), "r" (max)
                  : "cc", "memory");

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint64_t incrd = incrd;
    uint64_t compd = compd;

    asm volatile ("A_%=:\n\t"             /* local label */
                  "ldarx  %0,0,%3\n\t"    /* load operand */
                  "addi   %2,%0,1\n\t"    /* increment it into incrd */
                  "cmpl   7,1,%2,%4\n\t"  /* compare incrd to the max */
                  "mfcr   %1\n\t"         /* move the result into compd */
                  "rlwinm %1,%1,29,1\n\t" /* isolate the result bit */
                  "mulld  %2,%2,%1\n\t"   /* incrd *= compd */
                  "stdcx. %2,0,%3\n\t"    /* *operand = incrd */
                  "bne-   A_%=\n\t"       /* if it failed, to to label 1 back */
                  "isync"                 /* make sure it wasn't all just a dream */
                  : "=&b"   (retval), "=&r" (compd), "=&r" (incrd)
                  : "r"     (operand), "r" (max)
                  : "cc", "memory");

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
    ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64) && (QTHREAD_SIZEOF_ALIGNED_T == 4))

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
        retval  = oldval = *operand;
        newval  = oldval + 1;
        newval *= (newval < max);

        /* if (*operand == oldval)
         * swap(newval, *operand)
         * else
         * newval = *operand
         */
        __asm__ __volatile__ ("cas [%1] , %2, %0"        /* */
                              : "=&r"  (newval)
                              : "r"    (operand), "r" (oldval), "0" (newval)
                              : "memory");
    } while (oldval != newval);

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    aligned_t oldval, newval;

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
        retval  = oldval = *operand;
        newval  = oldval + 1;
        newval *= (newval < max);

        /* if (*operand == oldval)
         * swap(newval, *operand)
         * else
         * newval = *operand
         */
        __asm__ __volatile__ ("casx [%1] , %2, %0" : "=&r" (newval)
                              : "r"    (operand), "r" (oldval)
#  if !defined(__SUNPRO_CC) && !defined(__SUNPRO_C)
                              , "0" (newval)
#  endif
                              : "memory");
    } while (oldval != newval);

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
#  if QTHREAD_SIZEOF_ALIGNED_T == 8
    int64_t res, old, new;

    do {
        old  = *operand;               /* atomic, because operand is aligned */
        new  = old + 1;
        new *= (new < max);
        asm volatile ("mov ar.ccv=%0;;" :        /* no output */
                      : "rO"    (old));

        /* separate so the compiler can insert its junk */
        asm volatile ("cmpxchg8.acq %0=[%1],%2,ar.ccv" : "=r" (res)
                      : "r"     (operand), "r" (new)
                      : "memory");
    } while (res != old);              /* if res==old, new is out of date */
    retval = old;

#  else /* 32-bit aligned_t */
    int32_t res, old, new;

    do {
        old  = *operand;               /* atomic, because operand is aligned */
        new  = old + 1;
        new *= (new < max);
        asm volatile ("mov ar.ccv=%0;;" :        /* no output */
                      : "rO"    (old));

        /* separate so the compiler can insert its junk */
        asm volatile ("cmpxchg4.acq %0=[%1],%2,ar.ccv" : "=r" (res)
                      : "r"     (operand), "r" (new)
                      : "memory");
    } while (res != old);              /* if res==old, new is out of date */
    retval = old;
#  endif /* if QTHREAD_SIZEOF_ALIGNED_T == 8 */

# elif ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) && (QTHREAD_SIZEOF_ALIGNED_T == 4)) || \
    ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) && (QTHREAD_SIZEOF_ALIGNED_T == 4))

    unsigned int oldval, newval;

    do {
        oldval  = *operand;
        newval  = oldval + 1;
        newval *= (newval < max);
        asm volatile ("lock; cmpxchgl %1, (%2)"
                      : "=&a" (retval)
                      : "r"     (newval), "r" (operand), "0" (oldval)
                      : "memory");
    } while (retval != oldval);

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union {
        uint64_t i;
        struct {
            /* note: the ordering of these is important and counter-intuitive; welcome to little-endian! */
            uint32_t l;
            uint32_t h;
        } s;
    } oldval, newval;
    char test;

    do {
#  ifdef __PIC__
        /* this saves off %ebx to make PIC code happy :P */
#   define QTHREAD_INTERNAL_PIC_PREFIX /*"xchg %%ebx, %4\n\t"*/ "pushl %%ebx\n\tmovl %4, %%ebx\n\t"
        /* this restores it */
#   define QTHREAD_INTERNAL_PIC_SUFFIX /*"\n\txchg %%ebx, %4"*/ "\n\tpopl %%ebx"
#   define QTHREAD_INTERNAL_PIC_REG_4  "m"
#  else
#   define QTHREAD_INTERNAL_PIC_PREFIX
#   define QTHREAD_INTERNAL_PIC_SUFFIX
#   define QTHREAD_INTERNAL_PIC_REG_4 "b"
#  endif
        oldval.i  = *operand;
        newval.i  = oldval.i + 1;
        newval.i *= (newval.i < max);
        __asm__ __volatile__ (QTHREAD_INTERNAL_PIC_PREFIX
                              "lock; cmpxchg8b (%1)\n\t"
                              "setne %0" /* test = (ZF==0) */
                              QTHREAD_INTERNAL_PIC_SUFFIX
                              : "=q" (test)
                              : "r"    (operand), /*EAX*/ "a" (oldval.s.l),
                              /*EDX*/ "d" (oldval.s.h),
                              /*EBX*/ QTHREAD_INTERNAL_PIC_REG_4(newval.s.l),
                              /*ECX*/ "c" (newval.s.h)
                              : "memory");
    } while (test);
    retval = oldval.i;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    unsigned long oldval, newval;

    do {
        oldval  = *operand;
        newval  = oldval + 1;
        newval *= (newval < max);
        asm volatile ("lock; cmpxchgq %1, (%2)"
                      : "=a" (retval)
                      : "r"     (newval), "r" (operand), "0" (oldval)
                      : "memory");
    } while (retval != oldval);

# else /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) && (QTHREAD_SIZEOF_ALIGNED_T == 4)) */
#  error "Unimplemented assembly architecture"
# endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) && (QTHREAD_SIZEOF_ALIGNED_T == 4)) */

#elif defined(QTHREAD_MUTEX_INCREMENT)
    QTHREAD_FASTLOCK_LOCK(lock);
    retval    = (*operand)++;
    *operand *= (*operand < max);
    QTHREAD_FASTLOCK_UNLOCK(lock);

#elif QTHREAD_ATOMIC_CAS
    aligned_t oldval, newval;

    retval = *operand;
    do {
        oldval  = retval;
        newval  = retval + 1;
        newval *= (newval < max);
        retval  = __sync_val_compare_and_swap(operand, oldval, newval);
    } while (retval != oldval);
#else /* if defined(HAVE_GCC_INLINE_ASSEMBLY) */
# error "Neither atomic or mutex increment enabled"
#endif /* if defined(HAVE_GCC_INLINE_ASSEMBLY) */

    return retval;
}                                      /*}}} */

static QINLINE void *qt_internal_atomic_swap_ptr(void **addr,
                                                 void  *newval)
{   /*{{{*/
    void *oldval = *addr;
    void *tmp;

    while ((tmp = qthread_cas_ptr(addr, oldval, newval)) != oldval) {
        oldval = tmp;
    }
    return oldval;
} /*}}}*/

#endif /* ifndef QT_ATOMICS_H */

/* vim:set expandtab */
