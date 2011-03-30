#ifndef QT_ATOMICS_H
#define QT_ATOMICS_H

#include <qthread/common.h>
#include <qthread/qthread.h>

#ifdef QTHREAD_NEEDS_IA64INTRIN
# ifdef HAVE_IA64INTRIN_H
#  include <ia64intrin.h>
# elif defined(HAVE_IA32INTRIN_H)
#  include <ia32intrin.h>
# endif
#endif

#if defined(__tile__)
# include <tmc/sync.h>
# define QTHREAD_FASTLOCK_INIT(x)     tmc_sync_mutex_init(& (x))
# define QTHREAD_FASTLOCK_INIT_PTR(x) tmc_sync_mutex_init((x))
# define QTHREAD_FASTLOCK_LOCK(x)     tmc_sync_mutex_lock((x))
# define QTHREAD_FASTLOCK_UNLOCK(x)   tmc_sync_mutex_unlock((x))
# define QTHREAD_FASTLOCK_DESTROY(x)
# define QTHREAD_FASTLOCK_DESTROY_PTR(x)
# define QTHREAD_FASTLOCK_TYPE        tmc_sync_mutex_t
# define QTHREAD_FASTLOCK_INITIALIZER TMC_SYNC_MUTEX_INIT
#elif defined(HAVE_PTHREAD_SPIN_INIT)
# include <pthread.h>
# define QTHREAD_FASTLOCK_INIT(x)        pthread_spin_init(& (x), PTHREAD_PROCESS_PRIVATE)
# define QTHREAD_FASTLOCK_INIT_PTR(x)    pthread_spin_init((x), PTHREAD_PROCESS_PRIVATE)
# define QTHREAD_FASTLOCK_LOCK(x)        pthread_spin_lock((x))
# define QTHREAD_FASTLOCK_UNLOCK(x)      pthread_spin_unlock((x))
# define QTHREAD_FASTLOCK_DESTROY(x)     pthread_spin_destroy(& (x))
# define QTHREAD_FASTLOCK_DESTROY_PTR(x) pthread_spin_destroy((x))
# define QTHREAD_FASTLOCK_TYPE        pthread_spinlock_t
# define QTHREAD_FASTLOCK_INITIALIZER PTHREAD_SPINLOCK_INITIALIZER
#else /* fallback */
# include <pthread.h>
# define QTHREAD_FASTLOCK_INIT(x)        pthread_mutex_init(& (x), NULL)
# define QTHREAD_FASTLOCK_INIT_PTR(x)    pthread_mutex_init((x), NULL)
# define QTHREAD_FASTLOCK_LOCK(x)        pthread_mutex_lock((x))
# define QTHREAD_FASTLOCK_UNLOCK(x)      pthread_mutex_unlock((x))
# define QTHREAD_FASTLOCK_DESTROY(x)     pthread_mutex_destroy(& (x))
# define QTHREAD_FASTLOCK_DESTROY_PTR(x) pthread_mutex_destroy((x))
# define QTHREAD_FASTLOCK_TYPE        pthread_mutex_t
# define QTHREAD_FASTLOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#endif /* */

#ifdef QTHREAD_MUTEX_INCREMENT
# define QTHREAD_CASLOCK(var)                var; QTHREAD_FASTLOCK_TYPE var ## _caslock
# define QTHREAD_CASLOCK_EXPLICIT_DECL(name) QTHREAD_FASTLOCK_TYPE name
# define QTHREAD_CASLOCK_EXPLICIT_INIT(name) QTHREAD_FASTLOCK_INIT(name)
# define QTHREAD_CASLOCK_INIT(var,i)         var = i; QTHREAD_FASTLOCK_INIT(var ## _caslock)
# define QTHREAD_CASLOCK_DESTROY(var)        QTHREAD_FASTLOCK_DESTROY(var ## _caslock)
# define QTHREAD_CASLOCK_READ(var)           (void *)qt_cas_read_ui((volatile uintptr_t *)& (var), & (var ## _caslock))
# define QTHREAD_CASLOCK_READ_UI(var)        qt_cas_read_ui((volatile uintptr_t *)& (var), & (var ## _caslock))
# define QT_CAS(var,oldv,newv)               qt_cas((void *volatile *)& (var), (void *)(oldv), (void *)(newv), & (var ## _caslock))
# define QT_CAS_(var,oldv,newv,caslock)      qt_cas((void *volatile *)& (var), (void *)(oldv), (void *)(newv), & (caslock))
static QINLINE void *qt_cas(void *volatile * const ptr, void * const oldv, void * const newv, QTHREAD_FASTLOCK_TYPE *lock)
{
    void * ret;
    QTHREAD_FASTLOCK_LOCK(lock);
    ret = *ptr;
    if (*ptr == oldv) {
        *ptr = newv;
    }
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return ret;
}

static QINLINE uintptr_t qt_cas_read_ui(volatile uintptr_t * const ptr, QTHREAD_FASTLOCK_TYPE *mutex)
{
    uintptr_t ret;
    QTHREAD_FASTLOCK_LOCK(mutex);
    ret = *ptr;
    QTHREAD_FASTLOCK_UNLOCK(mutex);
    return ret;
}

#else /* ifdef QTHREAD_MUTEX_INCREMENT */
# define QTHREAD_CASLOCK(var)           (var)
# define QTHREAD_CASLOCK_EXPLICIT_DECL(name)
# define QTHREAD_CASLOCK_EXPLICIT_INIT(name)
# define QTHREAD_CASLOCK_INIT(var,i)    (var) = i
# define QTHREAD_CASLOCK_DESTROY(var)
# define QTHREAD_CASLOCK_READ(var)      (var)
# define QTHREAD_CASLOCK_READ_UI(var)   (var)
# define QT_CAS(var,oldv,newv)          qt_cas((void *volatile *)& (var), (void *)(oldv), (void *)(newv))
# define QT_CAS_(var,oldv,newv,caslock) qt_cas((void *volatile *)& (var), (void *)(oldv), (void *)(newv))
# ifdef QTHREAD_ATOMIC_CAS_PTR
#  define qt_cas(P,O,N)                 (void *)__sync_val_compare_and_swap((P),(O),(N))
# else
static QINLINE void *qt_cas(void *volatile * const ptr, void * const oldv, void * const newv)
{ /*{{{*/
#  if defined(HAVE_GCC_INLINE_ASSEMBLY)
#   if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    void * result;
    __asm__ __volatile__ ("1:\n\t"
                          "lwarx  %0,0,%3\n\t"
                          "cmpw   %0,%1\n\t"
                          "bne    2f\n\t"
                          "stwcx. %2,0,%3\n\t"
                          "bne-   1b\n"
                          "2:"
                          : "=&b" (result)
                          : "r" (oldv), "r" (newv), "r" (ptr)
                          : "cc", "memory");
    return result;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    void * result;
    __asm__ __volatile__ ("1:\n\t"
                          "ldarx  %0,0,%3\n\t"
                          "cmpw   %0,%1\n\t"
                          "bne    2f\n\t"
                          "stdcx. %2,0,%3\n\t"
                          "bne-   1b\n"
                          "2:"
                          : "=&b" (result)
                          : "r" (oldv), "r" (newv), "r" (ptr)
                          : "cc", "memory");
    return result;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    void *nv = newv;
    __asm__ __volatile__
    ("cas [%1], %2, %0"
     : "=&r" (nv)
     : "r" (ptr), "r" (oldv)
#    if !defined(__SUNPRO_C) && !defined(__SUNPRO_CC)
     , "0" (nv)
#    endif
     : "cc", "memory");
    return nv;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    void *nv = newv;
    __asm__ __volatile__
    ("casx [%1], %2, %0"
     : "=&r" (nv)
     : "r" (ptr), "r" (oldv)
#    if !defined(__SUNPRO_C) && !defined(__SUNPRO_CC)
     , "0" (nv)
#    endif
     : "cc", "memory");
    return nv;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    void * * retval;
    __asm__ __volatile__ ("mov ar.ccv=%0;;" : : "rO" (oldv));
    __asm__ __volatile__ ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
                          : "=r" (retval)
                          : "r" (ptr), "r" (newv)
                          : "memory");
    return retval;

#   elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    void * * retval;

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
                          : "cc","memory");
#    else
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)"
                          : "=a" (retval) /* store from RAX */
                          : "r" (newv), "r" (ptr),
                          "a" (oldv) /* load into RAX */
                          : "cc","memory");
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
#endif /* MUTEX_INCREMENT */

#ifndef QTHREAD_MUTEX_INCREMENT
# define qthread_internal_atomic_read_s(op,lock) (*op)
# define qthread_internal_incr(op,lock,val)      qthread_incr(op, val)
# define qthread_internal_incr_s(op,lock,val)    qthread_incr(op, val)
# define qthread_internal_decr(op,lock)          qthread_incr(op, -1)
# define qthread_internal_incr_mod(op,m,lock)    qthread_internal_incr_mod_(op,m)
# define QTHREAD_OPTIONAL_LOCKARG
#else
# define qthread_internal_incr_mod(op,m,lock)    qthread_internal_incr_mod_(op,m,lock)
# define QTHREAD_OPTIONAL_LOCKARG , QTHREAD_FASTLOCK_TYPE * lock
static QINLINE aligned_t qthread_internal_incr(volatile aligned_t * operand,
                                               QTHREAD_FASTLOCK_TYPE * lock,
                                               int val)
{                                      /*{{{ */
    aligned_t retval;

    QTHREAD_FASTLOCK_LOCK(lock);
    retval = *operand;
    *operand += val;
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return retval;
}                                      /*}}} */

static QINLINE saligned_t qthread_internal_incr_s(volatile saligned_t * operand,
                                                  QTHREAD_FASTLOCK_TYPE * lock, int val)
{                                      /*{{{ */
    saligned_t retval;

    QTHREAD_FASTLOCK_LOCK(lock);
    retval = *operand;
    *operand += val;
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return retval;
}                                      /*}}} */

static QINLINE saligned_t qthread_internal_atomic_read_s(volatile saligned_t * operand,
                                                         QTHREAD_FASTLOCK_TYPE * lock)
{                                      /*{{{ */
    saligned_t retval;

    QTHREAD_FASTLOCK_LOCK(lock);
    retval = *operand;
    QTHREAD_FASTLOCK_UNLOCK(lock);
    return retval;
}                                      /*}}} */

#endif /* ifndef QTHREAD_MUTEX_INCREMENT */

static QINLINE aligned_t qthread_internal_incr_mod_(volatile aligned_t * operand,
                                                    const int max QTHREAD_OPTIONAL_LOCKARG)
{                                      /*{{{ */
    aligned_t retval;

#if defined(HAVE_GCC_INLINE_ASSEMBLY)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) && (QTHREAD_SIZEOF_ALIGNED_T == 4))

    register unsigned int incrd = incrd;        /* these don't need to be initialized */
    register unsigned int compd = compd;        /* they're just tmp variables */

    /* the minus in bne- means "this bne is unlikely to be taken" */
    asm volatile ("1:\n\t"      /* local label */
                  "lwarx  %0,0,%3\n\t"  /* load operand */
                  "addi   %2,%0,1\n\t"  /* increment it into incrd */
                  "cmplw  7,%2,%4\n\t"  /* compare incrd to the max */
                  "mfcr   %1\n\t"       /* move the result into compd */
                  "rlwinm %1,%1,29,1\n\t"       /* isolate the result bit */
                  "mullw  %2,%2,%1\n\t" /* incrd *= compd */
                  "stwcx. %2,0,%3\n\t"  /* *operand = incrd */
                  "bne-   1b\n\t"       /* if it failed, go to label 1 back */
                  "isync"       /* make sure it wasn't all a dream */

                  /* = means this operand is write-only (previous value is discarded)
                   * & means this operand is an earlyclobber (i.e. cannot use the same register as any of the input operands)
                   * b means this is a register but must not be r0 */
                  : "=&b"   (retval), "=&r" (compd), "=&r" (incrd)
                  : "r"     (operand), "r" (max)
                  : "cc", "memory");

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    register uint64_t incrd = incrd;
    register uint64_t compd = compd;

    asm volatile ("1:\n\t"      /* local label */
                  "ldarx  %0,0,%3\n\t"  /* load operand */
                  "addi   %2,%0,1\n\t"  /* increment it into incrd */
                  "cmpl   7,1,%2,%4\n\t"        /* compare incrd to the max */
                  "mfcr   %1\n\t"       /* move the result into compd */
                  "rlwinm %1,%1,29,1\n\t"       /* isolate the result bit */
                  "mulld  %2,%2,%1\n\t" /* incrd *= compd */
                  "stdcx. %2,0,%3\n\t"  /* *operand = incrd */
                  "bne-   1b\n\t"       /* if it failed, to to label 1 back */
                  "isync"       /* make sure it wasn't all just a dream */
                  : "=&b"   (retval), "=&r" (compd), "=&r" (incrd)
                  : "r"     (operand), "r" (max)
                  : "cc", "memory");

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
    ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64) && (QTHREAD_SIZEOF_ALIGNED_T == 4))

    register uint32_t oldval, newval;

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
        retval = oldval = *operand;
        newval = oldval + 1;
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
    register aligned_t oldval, newval;

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
        retval = oldval = *operand;
        newval = oldval + 1;
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
        old = *operand;                /* atomic, because operand is aligned */
        new = old + 1;
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
        old = *operand;                /* atomic, because operand is aligned */
        new = old + 1;
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
        oldval = *operand;
        newval = oldval + 1;
        newval *= (newval < max);
        asm volatile ("lock; cmpxchgl %1, (%2)" : "=&a" (retval)
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
    register char test;

    do {
#  ifdef __PIC__
        /* this saves off %ebx to make PIC code happy :P */
#   define QTHREAD_PIC_PREFIX "pushl %%ebx\n\tmovl %4, %%ebx\n\t"
        /* this restores it */
#   define QTHREAD_PIC_SUFFIX "\n\tpopl %%ebx"
#   define QTHREAD_PIC_REG    "m"
#  else
#   define QTHREAD_PIC_PREFIX
#   define QTHREAD_PIC_SUFFIX
#   define QTHREAD_PIC_REG "b"
#  endif
        oldval.i = *operand;
        newval.i = oldval.i + 1;
        newval.i *= (newval.i < max);
        __asm__ __volatile__ (QTHREAD_PIC_PREFIX "lock; cmpxchg8b (%1)\n\t" "setne %0"   /* test = (ZF==0) */
QTHREAD_PIC_SUFFIX: "=q" (test)
                              : "r"    (operand), /*EAX*/ "a" (oldval.s.l),
                              /*EDX*/ "d" (oldval.s.h),
                              /*EBX*/ QTHREAD_PIC_REG(newval.s.l),
                              /*ECX*/ "c" (newval.s.h)
                              : "memory");
    } while (test);
    retval = oldval.i;

# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    unsigned long oldval, newval;

    do {
        oldval = *operand;
        newval = oldval + 1;
        newval *= (newval < max);
        asm volatile ("lock; cmpxchgq %1, (%2)" : "=a" (retval)
                      : "r"     (newval), "r" (operand), "0" (oldval)
                      : "memory");
    } while (retval != oldval);

# else /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) && (QTHREAD_SIZEOF_ALIGNED_T == 4)) */
#  error "Unimplemented assembly architecture"
# endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) && (QTHREAD_SIZEOF_ALIGNED_T == 4)) */

#elif defined(QTHREAD_MUTEX_INCREMENT)
    QTHREAD_FASTLOCK_LOCK(lock);
    retval = (*operand)++;
    *operand *= (*operand < max);
    QTHREAD_FASTLOCK_UNLOCK(lock);

#else /* if defined(HAVE_GCC_INLINE_ASSEMBLY) */
# error "Neither atomic or mutex increment enabled"
#endif /* if defined(HAVE_GCC_INLINE_ASSEMBLY) */

    return retval;
}                                      /*}}} */

#endif /* ifndef QT_ATOMICS_H */

/* vim:set expandtab */
