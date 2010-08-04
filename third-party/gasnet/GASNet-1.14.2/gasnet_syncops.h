/*   $Source: /var/local/cvs/gasnet/gasnet_syncops.h,v $
 *     $Date: 2010/04/16 23:24:42 $
 * $Revision: 1.53.2.1 $
 * Description: GASNet header for synchronization operations used in GASNet implementation
 * Copyright 2006, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_INTERNAL_H)
  #error This file is not meant to be included directly- internal code should include gasnet_internal.h
#endif

#ifndef _GASNET_SYNCOPS_H
#define _GASNET_SYNCOPS_H

GASNETI_BEGIN_EXTERNC

/* ------------------------------------------------------------------------------------ */

/* 
 * The gasnet mutex and spinlock code are in gasnet_help.h.
 */

/* ------------------------------------------------------------------------------------ */
/*
 * gasneti_semaphore_t
 *
 * This is a simple busy-waiting semaphore used, for instance, to control access to
 * some resource of known multiplicity.
 *
 * Unless GASNETI_SEMAPHORES_NOT_SIGNALSAFE is defined, the operations "up" and "up_n"
 * are safe from signal context and the "trydown", "trydown_n" and "trydown_partial"
 * are non-blocking from signal context.  However, calls to the trydown* functions from
 * signal context might never succeed if the thead interrupted by the signal was to
 * execute a matching "up".
 *
 * When debugging a non-zero 'limit' (given in the static or dynamic initializer) will
 * be checked to detect usage errors.  A 'limit' of zero disables this checking.
 *
 *	GASNETI_SEMAPHORE_MAX
 *				Maximum value supported by the implementation.
 *	GASNETI_SEMAPHORE_INITIALIZER(value, limit)
 *				Static initializer macro.
 *	gasneti_semaphore_init(&sema, value, limit)
 *				Dynamic initializer
 *	gasneti_semaphore_destroy(&sema)
 *				Destructor for dynamicaly initialized semaphores.
 *	gasneti_semaphore_read(&sema)
 *				Returns current value - mainly useful for debugging.
 *	gasneti_semaphore_up(&sema)
 *				Increments value atomically.
 *	gasneti_semaphore_up_n(&sema, n)
 *				Adds 'n' to value atomically.
 *	gasneti_semaphore_trydown(&sema)
 *				Atomically reduce value by 1 if new value would be non-negative.
 *				Returns 1 if value was reduced, or 0 otherwise.
 *	gasneti_semaphore_trydown_n(&sema, n)
 *				Atomically reduce value by 'n' if new value would be non-negative.
 *				Returns 'n' if value was reduced, or 0 otherwise.
 *	gasneti_semaphore_trydown_partial(&sema, n)
 *				Atomically reduce value by up to 'n', such that value is non-negative.
 *				Returns value in range 0 to 'n', indicating amount value was reduced.
 */

#if defined(GASNETI_FORCE_GENERIC_SEMAPHORES) || /* for debugging */ \
    defined(GASNETI_USE_GENERIC_ATOMICOPS)    || /* avoids double locking */ \
    !GASNETI_THREADS                          || /* avoids complexity of atomic algorithms */ \
    !(defined(GASNETI_HAVE_ATOMIC_CAS) || defined(GASNETI_HAVE_ATOMIC_ADD_SUB)) /* lack needed ops */
  #define GASNETI_USE_GENERIC_SEMAPHORES 1
#endif

#if defined(GASNETI_USE_GENERIC_SEMAPHORES) 
  /* Generic mutex-based implementation */
  typedef struct {
    gasneti_mutex_t		lock;
    gasneti_atomic_val_t	count;	 /* keeps size consistent beween SEQ and PAR builds */
  } _gasneti_semaphore_t;
  #define _GASNETI_SEMAPHORE_INITIALIZER(N) {GASNETI_MUTEX_INITIALIZER, (N)}
  #define GASNETI_SEMAPHORE_MAX GASNETI_ATOMIC_MAX
  GASNETI_INLINE(_gasneti_semaphore_init)
  void _gasneti_semaphore_init(_gasneti_semaphore_t *s, int n) {
    gasneti_mutex_init(&(s->lock));
    s->count = n;
  }
  GASNETI_INLINE(_gasneti_semaphore_destroy)
  void _gasneti_semaphore_destroy(_gasneti_semaphore_t *s) {
    gasneti_mutex_destroy(&(s->lock));
  }
  GASNETI_INLINE(_gasneti_semaphore_read)
  gasneti_atomic_val_t _gasneti_semaphore_read(_gasneti_semaphore_t *s) {
    return s->count;	/* no lock required */
  }
  GASNETI_INLINE(_gasneti_semaphore_up)
  void _gasneti_semaphore_up(_gasneti_semaphore_t *s) {
    gasneti_mutex_lock(&(s->lock));
    s->count += 1;
    gasneti_mutex_unlock(&(s->lock));
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown)
  int _gasneti_semaphore_trydown(_gasneti_semaphore_t *s) {
    int retval;
    gasneti_mutex_lock(&(s->lock));
    retval = s->count;
    if_pt (retval != 0)
      s->count -= 1;
    gasneti_mutex_unlock(&(s->lock));
    return retval;
  }
  GASNETI_INLINE(_gasneti_semaphore_up_n)
  void _gasneti_semaphore_up_n(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    gasneti_mutex_lock(&(s->lock));
    s->count += n;
    gasneti_mutex_unlock(&(s->lock));
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown_partial)
  gasneti_atomic_val_t _gasneti_semaphore_trydown_partial(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    gasneti_atomic_val_t retval, old;
    gasneti_mutex_lock(&(s->lock));
    old = s->count;
    retval = MIN(old, n);
    s->count -= retval;
    gasneti_mutex_unlock(&(s->lock));
    return retval;
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown_n)
  gasneti_atomic_val_t _gasneti_semaphore_trydown_n(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    gasneti_mutex_lock(&(s->lock));
    if_pt (s->count >= n)
      s->count -= n;
    else
      n = 0;
    gasneti_mutex_unlock(&(s->lock));
    return n;
  }
#elif defined(GASNETI_HAVE_ATOMIC_CAS)
  /* Semi-generic implementation for CAS-capable systems */
  typedef gasneti_weakatomic_t _gasneti_semaphore_t;
  #define _GASNETI_SEMAPHORE_INITIALIZER gasneti_weakatomic_init
  #define GASNETI_SEMAPHORE_MAX GASNETI_ATOMIC_MAX
  GASNETI_INLINE(_gasneti_semaphore_init)
  void _gasneti_semaphore_init(_gasneti_semaphore_t *s, int n) {
    gasneti_weakatomic_set(s, n, GASNETI_ATOMIC_REL);
  }
  GASNETI_INLINE(_gasneti_semaphore_destroy)
  void _gasneti_semaphore_destroy(_gasneti_semaphore_t *s) {
    /* Nothing */
  }
  GASNETI_INLINE(_gasneti_semaphore_read)
  gasneti_atomic_val_t _gasneti_semaphore_read(_gasneti_semaphore_t *s) {
    return gasneti_weakatomic_read(s, 0);
  }
  GASNETI_INLINE(_gasneti_semaphore_up)
  void _gasneti_semaphore_up(_gasneti_semaphore_t *s) {
    gasneti_weakatomic_increment(s, GASNETI_ATOMIC_REL);
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown)
  int _gasneti_semaphore_trydown(_gasneti_semaphore_t *s) {
    int retval = 0;
    do {
      const gasneti_atomic_val_t old = gasneti_weakatomic_read(s, 0);
      if_pf (old == 0) {
	return 0;	/* Note: "break" here generates infinite loop w/ pathcc 2.4 (bug 1620) */
      }
      retval = gasneti_weakatomic_compare_and_swap(s, old, old - 1, GASNETI_ATOMIC_ACQ_IF_TRUE);
    } while (PREDICT_FALSE(!retval));
    return retval;
  }
  GASNETI_INLINE(_gasneti_semaphore_up_n)
  void _gasneti_semaphore_up_n(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    #if GASNETI_HAVE_ATOMIC_ADD_SUB
      (void)gasneti_weakatomic_add(s, n, GASNETI_ATOMIC_REL);
    #else
      int swap;
      do {
	const gasneti_atomic_val_t old = gasneti_weakatomic_read(s, 0);
	swap = gasneti_weakatomic_compare_and_swap(s, old, old + n, GASNETI_ATOMIC_REL);
      } while (PREDICT_FALSE(!swap));
    #endif
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown_partial)
  gasneti_atomic_val_t _gasneti_semaphore_trydown_partial(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    gasneti_atomic_val_t retval = 0;
    int swap;
    do {
      const gasneti_atomic_val_t old = gasneti_weakatomic_read(s, 0);
      if_pf (old == 0)
        break;
      retval = MIN(old, n);
      swap = gasneti_weakatomic_compare_and_swap(s, old, old - retval, GASNETI_ATOMIC_ACQ_IF_TRUE);
    } while (PREDICT_FALSE(!swap));
    return retval;
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown_n)
  gasneti_atomic_val_t _gasneti_semaphore_trydown_n(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    int swap;
    do {
      const gasneti_atomic_val_t old = gasneti_weakatomic_read(s, 0);
      if_pf (old < n) {
        n = 0;
        break;
      }
      swap = gasneti_weakatomic_compare_and_swap(s, old, old - n, GASNETI_ATOMIC_ACQ_IF_TRUE);
    } while (PREDICT_FALSE(!swap));
    return n;
  }
#elif defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
  /* Semi-generic implementation for and-add-fetch capable systems */
  typedef gasneti_weakatomic_t _gasneti_semaphore_t;
  #define _GASNETI_SEMAPHORE_INITIALIZER gasneti_weakatomic_init
  #define GASNETI_SEMAPHORE_MAX GASNETI_ATOMIC_SIGNED_MAX
  GASNETI_INLINE(_gasneti_semaphore_init)
  void _gasneti_semaphore_init(_gasneti_semaphore_t *s, int n) {
    gasneti_weakatomic_set(s, n, GASNETI_ATOMIC_REL);
  }
  GASNETI_INLINE(_gasneti_semaphore_destroy)
  void _gasneti_semaphore_destroy(_gasneti_semaphore_t *s) {
    /* Nothing */
  }
  GASNETI_INLINE(_gasneti_semaphore_read)
  gasneti_atomic_val_t _gasneti_semaphore_read(_gasneti_semaphore_t *s) {
    const gasneti_atomic_sval_t tmp = gasneti_atomic_signed(gasneti_weakatomic_read(s, 0));
    return (tmp >= 0) ? tmp : 0;
  }
  GASNETI_INLINE(_gasneti_semaphore_up)
  void _gasneti_semaphore_up(_gasneti_semaphore_t *s) {
    gasneti_weakatomic_increment(s, GASNETI_ATOMIC_REL);
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown)
  int _gasneti_semaphore_trydown(_gasneti_semaphore_t *s) {
    int retval = 0;
    if_pt (gasneti_atomic_signed(gasneti_weakatomic_read(s, 0)) > 0) {
      if_pt (gasneti_atomic_signed(gasneti_weakatomic_subtract(s, 1, 0)) >= 0) {
        gasneti_local_rmb(); /* Acquire */
        retval = 1;
      } else {
        gasneti_weakatomic_increment(s, 0);
      }
    }
    return retval;
  }
  GASNETI_INLINE(_gasneti_semaphore_up_n)
  void _gasneti_semaphore_up_n(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    (void)gasneti_weakatomic_add(s, n, GASNETI_ATOMIC_REL);
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown_partial)
  gasneti_atomic_val_t _gasneti_semaphore_trydown_partial(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    int retval = 0;
    if_pt (gasneti_atomic_signed(gasneti_weakatomic_read(s, 0)) > 0) {
      gasneti_atomic_sval_t tmp = gasneti_atomic_signed(gasneti_weakatomic_subtract(s, n, 0));
      if_pt (tmp >= 0) {
        gasneti_local_rmb(); /* Acquire */
        retval = n;
      } else if (tmp >= -((gasneti_atomic_sval_t)n)) {
	retval = n + tmp;
        (void)gasneti_weakatomic_add(s, -tmp, 0);
      } else {
        /* retval = 0 already */
      }
    }
    return retval;
  }
  GASNETI_INLINE(_gasneti_semaphore_trydown_n)
  gasneti_atomic_val_t _gasneti_semaphore_trydown_n(_gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
    int retval = 0;
    if_pt (gasneti_atomic_signed(gasneti_weakatomic_read(s, 0)) >= n) {
      if_pt (gasneti_atomic_signed(gasneti_weakatomic_subtract(s, n, 0)) >= 0) {
        gasneti_local_rmb(); /* Acquire */
        retval = n;
      } else {
        (void)gasneti_weakatomic_add(s, n, 0);
      }
    }
    return retval;
  }
#endif

#if defined(GASNETI_USE_GENERIC_SEMAPHORES) || defined(GASNETI_ATOMICOPS_NOT_SIGNALSAFE)
  #define GASNETI_SEMAPHORES_NOT_SIGNALSAFE 1
#endif

typedef struct {
  #if GASNET_DEBUG
    _gasneti_semaphore_t	S;
    gasneti_atomic_val_t	limit;
    char			_pad[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_val_t)+sizeof(_gasneti_semaphore_t))];
  #else
    _gasneti_semaphore_t	S;
    char			_pad[GASNETI_CACHE_PAD(sizeof(_gasneti_semaphore_t))];
  #endif
} gasneti_semaphore_t;

#if GASNET_DEBUG
  #define GASNETI_SEMAPHORE_INITIALIZER(N,L) {_GASNETI_SEMAPHORE_INITIALIZER(N), (L),}
  #define GASNETI_SEMA_CHECK(_s)	do {                    \
      gasneti_atomic_val_t _tmp = _gasneti_semaphore_read(&(_s)->S); \
      gasneti_assert(_tmp <= GASNETI_SEMAPHORE_MAX);            \
      gasneti_assert((_tmp <= (_s)->limit) || !(_s)->limit);    \
    } while (0)
#else
  #define GASNETI_SEMAPHORE_INITIALIZER(N,L) {_GASNETI_SEMAPHORE_INITIALIZER(N),}
  #define GASNETI_SEMA_CHECK(_s)	do {} while(0)
#endif

/* gasneti_semaphore_init */
GASNETI_INLINE(gasneti_semaphore_init)
void gasneti_semaphore_init(gasneti_semaphore_t *s, int n, gasneti_atomic_val_t limit) {
  gasneti_assert(limit <= GASNETI_SEMAPHORE_MAX);
  _gasneti_semaphore_init(&(s->S), n);
  #if GASNET_DEBUG
    s->limit = limit;
  #endif
  GASNETI_SEMA_CHECK(s);
}

/* gasneti_semaphore_destroy */
GASNETI_INLINE(gasneti_semaphore_destroy)
void gasneti_semaphore_destroy(gasneti_semaphore_t *s) {
  GASNETI_SEMA_CHECK(s);
  _gasneti_semaphore_destroy(&(s->S));
}

/* gasneti_semaphore_read
 *
 * Returns current value of the semaphore
 */
GASNETI_INLINE(gasneti_semaphore_read)
gasneti_atomic_val_t gasneti_semaphore_read(gasneti_semaphore_t *s) {
  GASNETI_SEMA_CHECK(s);
  return _gasneti_semaphore_read(&(s->S));
}

/* gasneti_semaphore_up
 *
 * Atomically increments the value of the semaphore.
 * Since this just a busy-waiting semaphore, no waking operations are required.
 */
GASNETI_INLINE(gasneti_semaphore_up)
void gasneti_semaphore_up(gasneti_semaphore_t *s) {
  GASNETI_SEMA_CHECK(s);
  _gasneti_semaphore_up(&(s->S));
  GASNETI_SEMA_CHECK(s);
}

/* gasneti_semaphore_trydown
 *
 * If the value of the semaphore is non-zero, decrements it and returns non-zero.
 * If the value is zero, returns zero.  Otherwise returns 1;
 */
GASNETI_INLINE(gasneti_semaphore_trydown) GASNETI_WARN_UNUSED_RESULT
int gasneti_semaphore_trydown(gasneti_semaphore_t *s) {
  int retval;

  GASNETI_SEMA_CHECK(s);
  retval = _gasneti_semaphore_trydown(&(s->S));
  GASNETI_SEMA_CHECK(s);

  return retval;
}

/* gasneti_semaphore_up_n
 *
 * Increases the value of the semaphore by the indicated count.
 * Since this just a busy-waiting semaphore, no waking operations are required.
 */
GASNETI_INLINE(gasneti_semaphore_up_n)
void gasneti_semaphore_up_n(gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
  GASNETI_SEMA_CHECK(s);
  _gasneti_semaphore_up_n(&(s->S), n);
  GASNETI_SEMA_CHECK(s);
}

/* gasneti_semaphore_trydown_n
 *
 * Decrements the semaphore by 'n' or fails.
 * If the "old" value is zero, returns zero.
 */
GASNETI_INLINE(gasneti_semaphore_trydown_n) GASNETI_WARN_UNUSED_RESULT
gasneti_atomic_val_t gasneti_semaphore_trydown_n(gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
  gasneti_atomic_val_t retval;

  GASNETI_SEMA_CHECK(s);
  retval = _gasneti_semaphore_trydown_n(&(s->S), n);
  GASNETI_SEMA_CHECK(s);

  return retval;
}

/* gasneti_semaphore_trydown_partial
 *
 * Decrements the semaphore by as much as 'n' and returns the number of "counts" thus
 * obtained.  The decrement is the smaller of 'n' and the "old" value of the semaphore,
 * and this value is returned.
 * If the "old" value is zero, returns zero.
 */
GASNETI_INLINE(gasneti_semaphore_trydown_partial) GASNETI_WARN_UNUSED_RESULT
gasneti_atomic_val_t gasneti_semaphore_trydown_partial(gasneti_semaphore_t *s, gasneti_atomic_val_t n) {
  gasneti_atomic_val_t retval;

  GASNETI_SEMA_CHECK(s);
  retval = _gasneti_semaphore_trydown_partial(&(s->S), n);
  GASNETI_SEMA_CHECK(s);

  return retval;
}

/* ------------------------------------------------------------------------------------ */
/* Optional atomic operations for pointer-sized data.
 * FOR USE IN THIS FILE ONLY, UNTIL THIS IS MORE STABLE/COMPLETE.
 *
 * If GASNETI_HAVE_ATOMIC_PTR_CAS is defined:
 *	gasneti_atomic_ptr_t
 *	gasneti_atomic_ptr_init(val)
 *	gasneti_atomic_ptr_set(ptr, val)
 *	gasneti_atomic_ptr_read(ptr)
 *	gasneti_atomic_ptr_cas(ptr, oldval, newval, flags)
 *
 * If GASNETI_HAVE_ATOMIC_DBLPTR_CAS is defined:
 *	gasneti_atomic_dblptr_t
 *	gasneti_atomic_dblptr_init(hi, lo)
 *	gasneti_atomic_dblptr_set(ptr, hi, lo) // NON atomic
 *	gasneti_atomic_dblptr_set_hi(ptr, hi)
 *	gasneti_atomic_dblptr_set_lo(ptr, lo)
 *	gasneti_atomic_dblptr_read_lo(ptr)
 *	gasneti_atomic_dblptr_read_hi(ptr)
 *	gasneti_atomic_dblptr_cas2(ptr, oldhi, oldlo, newhi, newlo, flags)
 *	gasneti_atomic_dblptr_cas_lo(ptr, oldhi, oldlo, flags)
 *	gasneti_atomic_dblptr_cas_hi(ptr, newhi, newlo, flags)
 *
 * All values are uintptr_t, not (void*).
 *
 * NOTE: The set/read operations all currently lack a "flags" argument.
 * It can be added later if ever really needed.
 *
 * NOTE: These are for internal use only.  So, there are no "slow atomic" versions
 * for use with C++ compilers that lack the asm support present in the corresponding
 * C compiler.  However, it is possible to implement "special atomic" versions for
 * compilers with limited asm support (PGI < 6.1 and SunPro)
 */
/* Default implementations: */
#if defined(GASNETI_HAVE_ATOMIC_PTR_CAS)
  /* Use platform-specific version */
#elif PLATFORM_ARCH_32 && !defined(GASNETI_USE_GENERIC_ATOMIC32)
  #define GASNETI_HAVE_ATOMIC_PTR_CAS 1
  typedef gasneti_atomic32_t                    gasneti_atomic_ptr_t;
  #define gasneti_atomic_ptr_init(_v)		gasneti_atomic32_init((uintptr_t)(_v))
  #define gasneti_atomic_ptr_set(_p,_v)		gasneti_atomic32_set(_p,(uintptr_t)(_v),0)
  #define gasneti_atomic_ptr_read(_p)		((uintptr_t)gasneti_atomic32_read(_p,0))
  #define gasneti_atomic_ptr_cas(_p,_o,_n,_f)	gasneti_atomic32_compare_and_swap(_p,(uintptr_t)(_o),(uintptr_t)(_n),_f)
#elif PLATFORM_ARCH_64 && !defined(GASNETI_USE_GENERIC_ATOMIC64)
  #define GASNETI_HAVE_ATOMIC_PTR_CAS 1
  typedef gasneti_atomic64_t                    gasneti_atomic_ptr_t;
  #define gasneti_atomic_ptr_init(_v)		gasneti_atomic64_init((uintptr_t)(_v))
  #define gasneti_atomic_ptr_set(_p,_v)		gasneti_atomic64_set(_p,(uintptr_t)(_v),0)
  #define gasneti_atomic_ptr_read(_p)		((uintptr_t)gasneti_atomic64_read(_p,0))
  #define gasneti_atomic_ptr_cas(_p,_o,_n,_f)	gasneti_atomic64_compare_and_swap(_p,(uintptr_t)(_o),(uintptr_t)(_n),_f)
#endif

#if defined(GASNETI_HAVE_ATOMIC_DBLPTR_CAS)
  /* Use platform-specific version */
#elif PLATFORM_ARCH_32 && defined(GASNETI_HAVE_ATOMIC_PTR_CAS) && !defined(GASNETI_USE_GENERIC_ATOMIC64)
  #if PLATFORM_ARCH_BIG_ENDIAN
    typedef union {
      struct { gasneti_atomic_ptr_t hi_ptr, lo_ptr; } ptrs;	/* must be first for initializer */
      gasneti_atomic64_t a64;
    } gasneti_atomic_dblptr_t;
  #else
    typedef union {
      struct { gasneti_atomic_ptr_t lo_ptr, hi_ptr; } ptrs;	/* must be first for initializer */
      gasneti_atomic64_t a64;
    } gasneti_atomic_dblptr_t;
  #endif

  GASNETI_INLINE(gasneti_atomic_dblptr_cas2)
  int gasneti_atomic_dblptr_cas2(gasneti_atomic_dblptr_t *v, uintptr_t oldhi, uintptr_t oldlo, uintptr_t newhi, uintptr_t newlo, int flags) {
     uint64_t oldval = GASNETI_MAKEWORD(oldhi, oldlo);
     uint64_t newval = GASNETI_MAKEWORD(newhi, newlo);
     return gasneti_atomic64_compare_and_swap(&(v->a64), oldval, newval, flags);
  }
  #define GASNETI_GEN_ATOMIC_DBLPTR_CAS 1 /* Causes default version to be constructed below */
#elif PLATFORM_ARCH_64 && defined(GASNETI_HAVE_ATOMIC_PTR_CAS) && \
	defined(GASNETI_HAVE_ATOMIC128_T) && (GASNETI_HAVE_ATOMIC128_T <= 8 /*alignment*/)
  #if PLATFORM_ARCH_BIG_ENDIAN
    typedef union {
      struct { gasneti_atomic_ptr_t hi_ptr, lo_ptr; } ptrs;	/* must be first for initializer */
      gasneti_atomic128_t a128;
    } gasneti_atomic_dblptr_t;
  #else
    typedef union {
      struct { gasneti_atomic_ptr_t lo_ptr, hi_ptr; } ptrs;	/* must be first for initializer */
      gasneti_atomic128_t a128;
    } gasneti_atomic_dblptr_t;
  #endif

  GASNETI_INLINE(gasneti_atomic_dblptr_cas2)
  int gasneti_atomic_dblptr_cas2(gasneti_atomic_dblptr_t *v, uintptr_t oldhi, uintptr_t oldlo, uintptr_t newhi, uintptr_t newlo, int flags) {
     return gasneti_atomic128_compare_and_swap(&(v->a128), oldhi, oldlo, newhi, newlo, flags);
  }
  #define GASNETI_GEN_ATOMIC_DBLPTR_CAS 1 /* Causes default version to be constructed below */
#endif
#ifdef GASNETI_GEN_ATOMIC_DBLPTR_CAS
  #define GASNETI_HAVE_ATOMIC_DBLPTR_CAS 1
  /* Defaults in terms of the hi and lo halves */
  #if PLATFORM_ARCH_BIG_ENDIAN
    #ifndef gasneti_atomic_dblptr_init
      #define gasneti_atomic_dblptr_init(hi,lo)     { { gasneti_atomic_ptr_init((uintptr_t)(hi)), \
                                                        gasneti_atomic_ptr_init((uintptr_t)(lo)) } }
    #endif
    #ifndef gasneti_atomic_dblptr_set
      #define gasneti_atomic_dblptr_set(p,hi,lo)    do {                                        \
                                                      gasneti_atomic_ptr_set(&(p)->ptrs.hi_ptr, \
                                                                           (uintptr_t)(hi));    \
                                                      gasneti_atomic_ptr_set(&(p)->ptrs.lo_ptr, \
                                                                           (uintptr_t)(lo));    \
                                                    } while (0)
    #endif
  #else
    #ifndef gasneti_atomic_dblptr_init
      #define gasneti_atomic_dblptr_init(hi,lo)     { { gasneti_atomic_ptr_init((uintptr_t)(lo)), \
                                                        gasneti_atomic_ptr_init((uintptr_t)(hi)) } }
    #endif
    #ifndef gasneti_atomic_dblptr_set
      #define gasneti_atomic_dblptr_set(p,hi,lo)    do {                                        \
                                                      gasneti_atomic_ptr_set(&(p)->ptrs.lo_ptr, \
                                                                           (uintptr_t)(lo));    \
                                                      gasneti_atomic_ptr_set(&(p)->ptrs.hi_ptr, \
                                                                           (uintptr_t)(hi));    \
                                                    } while (0)
    #endif
  #endif
  #ifndef gasneti_atomic_dblptr_set_lo
    #define gasneti_atomic_dblptr_set_lo(p,lo)    do {                                        \
                                                    gasneti_atomic_ptr_set(&(p)->ptrs.lo_ptr, \
                                                                         (uintptr_t)(lo));    \
                                                  } while (0)
  #endif
  #ifndef gasneti_atomic_dblptr_set_hi
    #define gasneti_atomic_dblptr_set_hi(p,hi)    do {                                        \
                                                    gasneti_atomic_ptr_set(&(p)->ptrs.hi_ptr, \
                                                                         (uintptr_t)(hi));    \
                                                  } while (0)
  #endif
  #ifndef gasneti_atomic_dblptr_read_lo
    #define gasneti_atomic_dblptr_read_lo(p)      gasneti_atomic_ptr_read(&(p)->ptrs.lo_ptr)
  #endif
  #ifndef gasneti_atomic_dblptr_read_hi
    #define gasneti_atomic_dblptr_read_hi(p)      gasneti_atomic_ptr_read(&(p)->ptrs.hi_ptr)
  #endif
  #ifndef gasneti_atomic_dblptr_cas_lo
    #define gasneti_atomic_dblptr_cas_lo(v,oldlo,newlo,flags) \
                         gasneti_atomic_ptr_cas(&(v)->ptrs.lo_ptr,oldlo,newlo,flags)
  #endif
  #ifndef gasneti_atomic_dblptr_cas_hi
    #define gasneti_atomic_dblptr_cas_hi(v,oldhi,newhi,flags) \
                         gasneti_atomic_ptr_cas(&(v)->ptrs.hi_ptr,oldhi,newhi,flags)
  #endif
#endif


/* ------------------------------------------------------------------------------------ */

/* Gasnet internal LIFO (stack) container.
 *
 * This data type provides a last in first out linked list implementation which
 * is suitable for use as a multiple-producer, multiple-consumer free list.
 * On architectires where possible, the synchronization is performed with
 * lock-free/wait-free algorithms.  Elsewhere, gasneti_mutex's are used.
 *
 * This container type is independent of the type to be stored.  The only requirement
 * is that the first sizeof(void *) bytes of the object are used for the list linkage.
 *
 *
 * GASNETI_LIFO_INITIALIZER
 * 		Static initializer for empty LIFO
 * void gasneti_lifo_init(gasneti_lifo_head_t *lifo);
 *		Initializer for dynamically allocated LIFO
 * void gasneti_lifo_destroy(gasneti_lifo_head_t *lifo);
 *		Destructor for dynamically allocated LIFO
 * void *gasneti_lifo_pop(gasneti_lifo_head_t *lifo);
 *		Pop "top" element from the LIFO or NULL if it is empty
 * void gasneti_lifo_push(gasneti_lifo_head_t *lifo, void *elem);
 *		Push one element on the LIFO
 * void gasneti_lifo_push_many(gasneti_lifo_head_t *lifo, void *head, void *tail);
 *		Push a chain of linked elements on the LIFO
 * void gasneti_lifo_link(void *p, void *q);
 *		Build a chain (q follows p) for use with _lifo_push_many()
 * void *gasneti_lifo_next(void *elem);
 *		Get next element in a chain built with _lifo_link
 *
 * Unless GASNETI_LIFOS_NOT_SIGNALSAFE is defined, the operations "pop", "push", and
 * "push_many" are signal safe.  The operations "link" and "next" are always signal
 * safe since they don't involve access to any shared data structures.
 *
 * NOTE: Because a pop operation must derefence a potentially out-of-date "next"
 * pointer, we prohibit any use of the LIFO in which a former element becomes unmapped
 * unless one can ensure that no partially-completed POP could be referencing the
 * object.
 */


/* Optional arch-specific code */
#if !GASNETI_THREADS
  /* No threads, so we use the mutex code that compiles away. */
#elif defined(GASNETI_USE_GENERIC_ATOMICOPS)
  /* If using mutexes, then just use the mutex code */
#elif PLATFORM_ARCH_POWERPC && GASNETI_HAVE_ATOMIC_PTR_CAS
  /* Among the platforms we currently support, PPC is unique in having an LL/SC
   * construct which allows a load between the LL and the SC.
   */
  #if PLATFORM_COMPILER_GNU
    /* Note use of "Lga.0.%=" for labels works around the AIX assembler, which doesn't like "1:" */
    typedef struct {
      /* Ensure list head pointer is the only item on its cache line.
       * This prevents a live-lock which would result if a list element fell
       * on the same cache line.
       */
      char			_pad0[GASNETI_CACHE_LINE_BYTES];
      gasneti_atomic_ptr_t	head;
      char			_pad1[GASNETI_CACHE_LINE_BYTES];
    } gasneti_lifo_head_t;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t *p, void **head, void **tail) {
      /* RELEASE semantics */
      uintptr_t oldhead;
      do {
	oldhead = gasneti_atomic_ptr_read(&p->head);
	*tail = (void *)oldhead;
      } while (!gasneti_atomic_ptr_cas(&p->head, oldhead, (uintptr_t)head, GASNETI_ATOMIC_REL));
    }

    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t *p) {
      /* ACQUIRE semantics: 'isync' between read of head and head->next */
      register uintptr_t addr = (uintptr_t)(&p->head);
      register uintptr_t head, next;
      if_pf (gasneti_atomic_ptr_read(&p->head) == 0) {
	/* One expects the empty list case to be the most prone to contention because
	 * many threads may be continuously polling for it become non-empty.  The l[wd]arx
	 * involves obtaining the cache line in an Exclusive state, while this normal
	 * load does not.  Thus this redundant check is IBM's recommended practice.
	 */
	return NULL;
      }
      #if PLATFORM_ARCH_32
        __asm__ __volatile__ ("Lga.1.%=:	   \n\t"
			      "lwarx	%1,0,%0    \n\t" /* head = p->head */
			      "cmpwi	0,%1,0     \n\t" /* head == NULL? */
			      "beq-	Lga.2.%=   \n\t" /* end on NULL */
			      GASNETI_PPC_RMB_ASM "\n\t" /* rmb */
			      "lwz	%2,0(%1)   \n\t" /* next = head->next */
			      "stwcx.	%2,0,%0    \n\t" /* p->head = next */
			      "bne-	Lga.1.%=   \n"   /* retry on conflict */
			      "Lga.2.%=: "
				: "=b" (addr), "=b" (head), "=r" (next)
				: "0" (addr)
				: "memory", "cc");
      #elif PLATFORM_ARCH_64
        __asm__ __volatile__ ("Lga.1.%=:	   \n\t"
			      "ldarx	%1,0,%0    \n\t" /* head = p->head */
			      "cmpdi	0,%1,0     \n\t" /* head == NULL? */
			      "beq-	Lga.2.%=   \n\t" /* end on NULL */
			      GASNETI_PPC_RMB_ASM "\n\t" /* rmb */
			      "ld	%2,0(%1)   \n\t" /* next = head->next */
			      "stdcx.	%2,0,%0    \n\t" /* p->head = next */
			      "bne-	Lga.1.%=   \n"   /* retry on conflict */
			      "Lga.2.%=: "
				: "=b" (addr), "=b" (head), "=r" (next)
				: "0" (addr)
				: "memory", "cc");
      #else
        #error "PPC w/ unknown word size"
      #endif
      return (void *)head;
    }
    GASNETI_INLINE(_gasneti_lifo_init)
    void _gasneti_lifo_init(gasneti_lifo_head_t *p) {
      gasneti_atomic_ptr_set(&p->head, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER	{{0,}, gasneti_atomic_ptr_init(0),}
    #define GASNETI_HAVE_ARCH_LIFO	1
  #elif PLATFORM_COMPILER_XLC
    typedef struct {
      /* Ensure list head pointer is the only item on its cache line.
       * This prevents a live-lock which would result if a list element fell
       * on the same cache line.
       * XXX: Can't use GASNETI_CACHE_LINE_BYTES w/o some extra indirection.
       */
      char			_pad0[128];
      gasneti_atomic_ptr_t	head;
      char			_pad1[128 - sizeof(void **)];
    } gasneti_lifo_head_t;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t *p, void **head, void **tail) {
      /* RELEASE semantics */
      uintptr_t oldhead;
      do {
	oldhead = gasneti_atomic_ptr_read(&p->head);
	*tail = (void *)oldhead;
      } while (!gasneti_atomic_ptr_cas(&p->head, oldhead, (uintptr_t)head, GASNETI_ATOMIC_REL));
    }

    static void *_gasneti_lifo_pop(gasneti_lifo_head_t *p);
    /* ARGS: r3 = p  LOCAL: r0 = next, r4 = head */
    #if PLATFORM_ARCH_32
      #pragma mc_func _gasneti_lifo_pop {\
	"80830080"	/* lwz		r4,128(r3)	*/ \
	"38630080"	/* addi		r3,r3,128	*/ \
	"2c040000"	/* cmpwi	r4,0		*/ \
	"38800000"	/* li		r4,0		*/ \
	"41820020"	/* beq-		2f		*/ \
	"7c801828"	/* 1: lwarx	r4,0,r3		*/ \
	"2c040000"	/* cmpwi	r4,0		*/ \
	"41820014"	/* beq-		2f		*/ \
	GASNETI_PPC_RMB_ASM				\
	"80040000"	/* lwz		r0,0(r4)	*/ \
	"7c00192d"	/* stwcx.	r0,0,r3		*/ \
	"40a2ffe8"	/* bne-		1b		*/ \
	"7c832378"	/* 2: mr	r3,r4		*/ \
      }
    #elif PLATFORM_ARCH_64
      #pragma mc_func _gasneti_lifo_pop {\
	"e8830080"	/* ld		r4,128(r3)	*/ \
	"38630080"	/* addi		r3,r3,128	*/ \
	"2c240000"	/* cmpdi	r4,0		*/ \
	"38800000"	/* li		r4,0		*/ \
	"41820020"	/* beq-		2f		*/ \
	"7c8018a8"	/* 1: ldarx	r4,0,r3		*/ \
	"2c240000"	/* cmpdi	r4,0		*/ \
	"41820014"	/* beq-		2f		*/ \
	GASNETI_PPC_RMB_ASM				\
	"e8040000"	/* ld		r0,0(r4)	*/ \
	"7c0019ad"	/* stdcx.	r0,0,r3		*/ \
	"40a2ffe8"	/* bne-		1b		*/ \
	"7c832378"	/* mr		r3,r4		*/ \
      }
    #else
      #error "PPC w/ unknown word size"
    #endif
    #pragma reg_killed_by _gasneti_lifo_pop cr0, gr0, gr4

    GASNETI_INLINE(_gasneti_lifo_init)
    void _gasneti_lifo_init(gasneti_lifo_head_t *p) {
      gasneti_atomic_ptr_set(&p->head, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER	{{0,}, gasneti_atomic_ptr_init(0),}
    #define GASNETI_HAVE_ARCH_LIFO	1
  #endif
#elif defined(GASNETI_HAVE_ATOMIC_DBLPTR_CAS)
    /* Algorithm if we have a compare-and-swap for a type as wide as two pointers.
     * The lower half holds the head pointer, which the upper half hold a "tag"
     * which is advanced by one on each operation to avoid the "classic ABA problem".
     */
    typedef struct {
      char		_pad0[GASNETI_CACHE_LINE_BYTES];
      gasneti_atomic_dblptr_t 	head_and_tag;
      char		_pad1[GASNETI_CACHE_LINE_BYTES];
    } gasneti_lifo_head_t;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t *p, void **newhead, void **tail) {
      uintptr_t tag, oldhead;
      do {
	oldhead = gasneti_atomic_dblptr_read_lo(&p->head_and_tag);
	tag = gasneti_atomic_dblptr_read_hi(&p->head_and_tag);
	*tail = (void *)oldhead;
      } while (!gasneti_atomic_dblptr_cas2(&p->head_and_tag, tag, oldhead, tag+1, (uintptr_t)newhead, GASNETI_ATOMIC_REL));
    }
    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t *p) {
      uintptr_t tag, oldhead, newhead;
      do {
	oldhead = gasneti_atomic_dblptr_read_lo(&p->head_and_tag);
	tag = gasneti_atomic_dblptr_read_hi(&p->head_and_tag);
	if_pf (!oldhead) break;
	newhead = (uintptr_t)(*(void **)oldhead);
      } while (!gasneti_atomic_dblptr_cas2(&p->head_and_tag, tag, oldhead, tag+1, newhead, GASNETI_ATOMIC_ACQ_IF_TRUE));
      return (void *)oldhead;
    }
    GASNETI_INLINE(_gasneti_lifo_init)
    void _gasneti_lifo_init(gasneti_lifo_head_t *p) {
      gasneti_atomic_dblptr_set(&p->head_and_tag, 0, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER	{{0,}, gasneti_atomic_dblptr_init(0,0),}
    #define GASNETI_HAVE_ARCH_LIFO	1
#elif PLATFORM_ARCH_64 && defined(GASNETI_HAVE_ATOMIC128_T)
    /* Same algorithm as dblptr_cas, above, but with alignment worries added in */
    typedef struct {
      char                _pad0[GASNETI_CACHE_LINE_BYTES + GASNETI_HAVE_ATOMIC128_T];
      gasneti_atomic128_t head_and_tag; /* Actually might be lower addr, in _pad0 */
      char                _pad1[GASNETI_CACHE_LINE_BYTES];
    } gasneti_lifo_head_t;
    #define _GASNETI_LIFO_ALIGN(p) ((uintptr_t)p & ~(GASNETI_HAVE_ATOMIC128_T - 1))

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t *p, void **newhead, void **tail) {
      uintptr_t tag, oldhead;
      gasneti_atomic64_t *head = (gasneti_atomic64_t *)_GASNETI_LIFO_ALIGN(p);
      gasneti_atomic128_t *head_and_tag = (gasneti_atomic128_t *)head;
      do {
        oldhead = gasneti_atomic64_read(head, 0);
        tag = gasneti_atomic64_read(head+1, 0);
        *tail = (void *)oldhead;
      } while
  #if PLATFORM_ARCH_BIG_ENDIAN
              (!gasneti_atomic128_compare_and_swap(head_and_tag, oldhead, tag,
			                           (uintptr_t)newhead, tag+1, GASNETI_ATOMIC_REL));
  #else
              (!gasneti_atomic128_compare_and_swap(head_and_tag, tag, oldhead,
			                           tag+1, (uintptr_t)newhead, GASNETI_ATOMIC_REL));
  #endif
    }
    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t *p) {
      uintptr_t tag, oldhead, newhead;
      gasneti_atomic64_t *head = (gasneti_atomic64_t *)_GASNETI_LIFO_ALIGN(p);
      gasneti_atomic128_t *head_and_tag = (gasneti_atomic128_t *)head;
      do {
        oldhead = gasneti_atomic64_read(head, 0);
        tag = gasneti_atomic64_read(head+1, 0);
        if_pf (!oldhead) break;
        newhead = (uintptr_t)(*(void **)oldhead);
      } while
  #if PLATFORM_ARCH_BIG_ENDIAN
              (!gasneti_atomic128_compare_and_swap(head_and_tag, oldhead, tag,
                                                   newhead, tag+1, GASNETI_ATOMIC_ACQ_IF_TRUE));
  #else
              (!gasneti_atomic128_compare_and_swap(head_and_tag, tag, oldhead,
                                                   tag+1, newhead, GASNETI_ATOMIC_ACQ_IF_TRUE));
  #endif
      return (void *)oldhead;
    }
    GASNETI_INLINE(_gasneti_lifo_init)
    void _gasneti_lifo_init(gasneti_lifo_head_t *p) {
      gasneti_atomic128_t *head_and_tag = (gasneti_atomic128_t *)_GASNETI_LIFO_ALIGN(p);
      gasneti_atomic128_set(head_and_tag, 0, 0, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER    {{0,}, }
    #define GASNETI_HAVE_ARCH_LIFO      1
#elif PLATFORM_ARCH_IA64 && PLATFORM_ARCH_64 && GASNETI_HAVE_IA64_CMP8XCHG16
    /* Use the SCDS (Single-compare, double-swap) cmp8xchg16 instruction added to
     * the Montecito processors.  The algorithm is essentially the same as w/ CAS,
     * but the TAG is advanced/checked on both Push and Pop operations.  Note that
     * we also need the "ld16" (128-bit atomic read) to ensure "tag" and "head" match
     * (since we only compare on tag).
     *
     * We use compiler-specific code for:
     *   _gasneti_lifo_store16(): cmp8xchg16 w/ tag++ and return 0 on success
     *   _gasneti_lifo_load16(_addr, _tag, _addr): 16-byte atomic read macro
     *   _gasneti_lifo_st8_rel(): st8.rel instruction
     * and implement push/pop in terms of those using compiler-independent code.
     */
    #if PLATFORM_COMPILER_HP
      #include <machine/sys/inline.h>
	
      GASNETI_INLINE(_gasneti_lifo_store16)
      int _gasneti_lifo_store16(void volatile *ptr, uint64_t oldtag, void *newval) {
	_Asm_mov_to_ar(_AREG_CSD, (int64_t)newval);
	_Asm_mov_to_ar(_AREG_CCV, (int64_t)oldtag);
	return oldtag != _Asm_cmp8xchg16(_SEM_ACQ, ptr, (oldtag+1), _LDHINT_NONE, _UNGUARDED,
                                         (_Asm_fence)(_UP_MEM_FENCE | _DOWN_MEM_FENCE));
      }
      #define _gasneti_lifo_load16(_addr, _tag, _head) do { \
        (_tag) = _Asm_ld16(_LDHINT_NONE, (void *)(_addr), _UNGUARDED); \
        (_head) = _Asm_mov_from_ar(_AREG_CSD); \
      } while (0)
      #define _gasneti_lifo_st8_rel(_addr, _val) \
	_Asm_st_volatile(_SZ_D, _LDHINT_NONE, (void *)(_addr), (int64_t)(_val))

      #define GASNETI_HAVE_ARCH_LIFO	1
    #elif PLATFORM_COMPILER_INTEL
      #include <ia64intrin.h>
      GASNETI_INLINE(_gasneti_lifo_store16)
      int _gasneti_lifo_store16(void volatile *ptr, uint64_t oldtag, void *newval) {
        return oldtag != _InterlockedCompare64Exchange128_acq(ptr, (uint64_t)newval, oldtag+1, oldtag);
      }
      #define _gasneti_lifo_load16(_addr, _tag, _head) \
	((_tag) = __load128((_addr), &(_head)))
      #define _gasneti_lifo_st8_rel(_addr, _val) \
	__st8_rel((_addr), (_val))

      #define GASNETI_HAVE_ARCH_LIFO	1
    #elif PLATFORM_COMPILER_GNU
      GASNETI_INLINE(_gasneti_lifo_store16)
      int _gasneti_lifo_store16(void volatile *ptr, uint64_t oldtag, void *newval) {
	register uint64_t tmp = oldtag + 1;
        __asm__ __volatile__ (
          "mov			ar.ccv=%1		\n\t"
          "mov			ar.csd=%2;;		\n\t"
          "cmp8xchg16.acq	%0=[%3],%0,ar.csd,ar.ccv\n"
          : "+r"(tmp) : "r"(oldtag), "r"(newval), "r"(ptr) : "memory" );
        return tmp != oldtag;
      }
      #define _gasneti_lifo_load16(_addr, _tag, _head)        \
        __asm__ __volatile__ (                                \
          "ld16		%0,ar.csd=[%2];;\n\t"                 \
          "mov		%1=ar.csd	\n"                   \
          : "=r"(_tag), "=r"(_head) : "r"(_addr) : "memory" )
      #define _gasneti_lifo_st8_rel(_addr, _val) \
	__asm__ __volatile__ ( "st8.rel [%0]=%1" : : "r"(_addr), "r"(_val) : "memory")

      #define GASNETI_HAVE_ARCH_LIFO	1
    #else
      /* Unknown/unsupported compiler - mutexes will be used */
    #endif

    /* Here are the compiler-independent parts */
    #ifdef GASNETI_HAVE_ARCH_LIFO /* Only true if compiler-specific parts defined above */
      typedef struct {
        void 			*array[3]; /* for 16-byte aligment use either 0+1 or 1+2 */
        char			_pad[GASNETI_CACHE_PAD(3*sizeof(void *))];
      } gasneti_lifo_head_t;

      GASNETI_INLINE(_gasneti_lifo_push)
      void _gasneti_lifo_push(gasneti_lifo_head_t *p, void **head, void **tail) {
        uint64_t tag, old_head;
        void *q = (void *)GASNETI_ALIGNUP(p, 16);
        do {
          _gasneti_lifo_load16(q, tag, old_head);
          _gasneti_lifo_st8_rel(tail, old_head);
        } while (_gasneti_lifo_store16(q, tag, head));
      }
      GASNETI_INLINE(_gasneti_lifo_pop)
      void *_gasneti_lifo_pop(gasneti_lifo_head_t *p) {
        uint64_t tag, old_head;
        void *q = (void *)GASNETI_ALIGNUP(p, 16);
        do {
          _gasneti_lifo_load16(q, tag, old_head);
	  if (!old_head) break;
        } while (_gasneti_lifo_store16(q, tag, *(void **)old_head));
        return (void *)old_head;
      }
      GASNETI_INLINE(_gasneti_lifo_init)
      void _gasneti_lifo_init(gasneti_lifo_head_t *p) {
        void **q = (void **)GASNETI_ALIGNUP(p, 16);
        q[0] = q[1] = NULL;
      }
      GASNETI_INLINE(_gasneti_lifo_destroy)
      void _gasneti_lifo_destroy(gasneti_lifo_head_t *p) {
        /* NOTHING */
      }
      #define GASNETI_LIFO_INITIALIZER	{ { NULL, NULL, NULL} }
    #endif /* Compiler-independent portion of 64-bit ia64 support */
#else
  /* The LL/SC algorithm used on the PPC will not work on the Alpha or MIPS, which don't
   * allow for the load we perform between the ll and the sc.  More complex algorithms are
   * probably possible.  I'll continue to look into this.  -PHH 2006.04.19
   *
   * We do support x86-64 CPUs which implement their optional CAS2 (cmpxchg16b) instruction.
   *
   * We do support IA64 CPUs which implement their optional SCDS (cmp8xchg16) instruction.
   *
   * One possible solution for all remaining platforms is "software ll/sc".  Using just pointer
   * CAS, one can implement an ideal LL/SC which allows for arbitrary loads and stores between
   * the LL and the SC.  This would require a compare-and-swap-pointer atomic operation.
   * In the contention-free case such an algorithm needs a thread-local data lookup, an rmb()
   * and two CAS operations, which may or may not make it competative with mutexes.  Because
   * such an algorithm is "wait free", it is expected to perform better under contention than
   * mutexes.
   */
#endif

/* Generic mutex-based default implementation */
#ifndef GASNETI_HAVE_ARCH_LIFO
    typedef struct {
      gasneti_mutex_t		lock;
      void			**head;
      char			_pad[GASNETI_CACHE_PAD(sizeof(gasneti_mutex_t)+sizeof(void **))];
    } gasneti_lifo_head_t;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t *p, void **head, void **tail) {
      gasneti_mutex_lock(&(p->lock));
      *tail = p->head;
      p->head = head;
      gasneti_mutex_unlock(&(p->lock));
    }
    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t *p) {
      void **elem;
      gasneti_mutex_lock(&(p->lock));
      elem = p->head;
      if_pt (elem != NULL) {
        p->head = *elem;
      }
      gasneti_mutex_unlock(&(p->lock));
      return (void *)elem;
    }
    GASNETI_INLINE(_gasneti_lifo_init)
    void _gasneti_lifo_init(gasneti_lifo_head_t *p) {
      gasneti_mutex_init(&(p->lock));
      p->head = NULL;
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t *p) {
      gasneti_mutex_destroy(&(p->lock));
    }
    #define GASNETI_LIFO_INITIALIZER	{ GASNETI_MUTEX_INITIALIZER, NULL }
    #define GASNETI_HAVE_ARCH_LIFO	0
    #define GASNETI_LIFOS_NOT_SIGNALSAFE 1
#endif
    

/* Initializer for dynamically allocated lifo heads */
GASNETI_INLINE(gasneti_lifo_init)
void gasneti_lifo_init(gasneti_lifo_head_t *lifo) {
  gasneti_assert(lifo != NULL);
  _gasneti_lifo_init(lifo);
}

/* Destructor for dynamically allocated lifo heads */
GASNETI_INLINE(gasneti_lifo_destroy)
void gasneti_lifo_destroy(gasneti_lifo_head_t *lifo) {
  gasneti_assert(lifo != NULL);
  _gasneti_lifo_destroy(lifo);
}

/* Get one element from the LIFO or NULL if it is empty */
GASNETI_INLINE(gasneti_lifo_pop) GASNETI_MALLOC
void *gasneti_lifo_pop(gasneti_lifo_head_t *lifo) {
  gasneti_assert(lifo != NULL);
  return _gasneti_lifo_pop(lifo);
}

/* Push element on the LIFO */
GASNETI_INLINE(gasneti_lifo_push)
void gasneti_lifo_push(gasneti_lifo_head_t *lifo, void *elem) {
  gasneti_assert(lifo != NULL);
  gasneti_assert(elem != NULL);
  _gasneti_lifo_push(lifo, elem, elem);
}

/* Push a chain of linked elements on the LIFO */
GASNETI_INLINE(gasneti_lifo_push_many)
void gasneti_lifo_push_many(gasneti_lifo_head_t *lifo, void *head, void *tail) {
  gasneti_assert(lifo != NULL);
  gasneti_assert(head != NULL);
  gasneti_assert(tail != NULL);
  _gasneti_lifo_push(lifo, head, tail);
}

/* Build a chain (q follows p) for use with _lifo_push_many() */
GASNETI_INLINE(gasneti_lifo_link)
void gasneti_lifo_link(void *p, void *q) {
  gasneti_assert(p != NULL);
  gasneti_assert(q != NULL);
  *((void **)p) = q;
}

/* Get next element in a chain built with _lifo_link */
GASNETI_INLINE(gasneti_lifo_next)
void *gasneti_lifo_next(void *elem) {
  gasneti_assert(elem != NULL);
  return *((void **)elem);
}

/* ------------------------------------------------------------------------------------ */
GASNETI_END_EXTERNC

#endif
