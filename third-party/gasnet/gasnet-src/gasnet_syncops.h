/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_syncops.h $
 * Description: GASNet header for synchronization operations used in GASNet implementation
 * Copyright 2006, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_INTERNAL_H)
  #error This file is not meant to be included directly- internal code should include gasnet_internal.h
#endif

#ifndef _GASNET_SYNCOPS_H
#define _GASNET_SYNCOPS_H

/* ------------------------------------------------------------------------------------ */

/* 
 * The gasnet mutex code is in gasnet_toolhelp.h
 * The gasnet spinlock code is in gasnet_help.h
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
    } while (GASNETT_PREDICT_FALSE(!retval));
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
      } while (GASNETT_PREDICT_FALSE(!swap));
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
    } while (GASNETT_PREDICT_FALSE(!swap));
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
    } while (GASNETT_PREDICT_FALSE(!swap));
    return n;
  }
#elif defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
  /* Semi-generic implementation for add-and-fetch capable systems */
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
} gasneti_semaphore_t_PAR;

#if GASNET_DEBUG
  #define GASNETI_SEMAPHORE_INITIALIZER_PAR(N,L) {_GASNETI_SEMAPHORE_INITIALIZER(N), (L),}
  #define GASNETI_SEMA_CHECK(_s)	do {                    \
      gasneti_atomic_val_t _tmp = _gasneti_semaphore_read(&(_s)->S); \
      gasneti_assert_uint(_tmp ,<=, GASNETI_SEMAPHORE_MAX);     \
      gasneti_assert((_tmp <= (_s)->limit) || !(_s)->limit);    \
    } while (0)
#else
  #define GASNETI_SEMAPHORE_INITIALIZER_PAR(N,L) {_GASNETI_SEMAPHORE_INITIALIZER(N),}
  #define GASNETI_SEMA_CHECK(_s)	do {} while(0)
#endif

/* gasneti_semaphore_init */
GASNETI_INLINE(gasneti_semaphore_init_PAR)
void gasneti_semaphore_init_PAR(gasneti_semaphore_t_PAR *s, int n, gasneti_atomic_val_t limit) {
  gasneti_assert_uint(limit ,<=, GASNETI_SEMAPHORE_MAX);
  _gasneti_semaphore_init(&(s->S), n);
  #if GASNET_DEBUG
    s->limit = limit;
  #endif
  GASNETI_SEMA_CHECK(s);
}

/* gasneti_semaphore_destroy */
GASNETI_INLINE(gasneti_semaphore_destroy_PAR)
void gasneti_semaphore_destroy_PAR(gasneti_semaphore_t_PAR *s) {
  GASNETI_SEMA_CHECK(s);
  _gasneti_semaphore_destroy(&(s->S));
}

/* gasneti_semaphore_read
 *
 * Returns current value of the semaphore
 */
GASNETI_INLINE(gasneti_semaphore_read_PAR)
gasneti_atomic_val_t gasneti_semaphore_read_PAR(gasneti_semaphore_t_PAR *s) {
  GASNETI_SEMA_CHECK(s);
  return _gasneti_semaphore_read(&(s->S));
}

/* gasneti_semaphore_up
 *
 * Atomically increments the value of the semaphore.
 * Since this just a busy-waiting semaphore, no waking operations are required.
 */
GASNETI_INLINE(gasneti_semaphore_up_PAR)
void gasneti_semaphore_up_PAR(gasneti_semaphore_t_PAR *s) {
  GASNETI_SEMA_CHECK(s);
  _gasneti_semaphore_up(&(s->S));
  GASNETI_SEMA_CHECK(s);
}

/* gasneti_semaphore_trydown
 *
 * If the value of the semaphore is non-zero, decrements it and returns non-zero.
 * If the value is zero, returns zero.  Otherwise returns 1;
 */
GASNETI_INLINE(gasneti_semaphore_trydown_PAR) GASNETI_WARN_UNUSED_RESULT
int gasneti_semaphore_trydown_PAR(gasneti_semaphore_t_PAR *s) {
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
GASNETI_INLINE(gasneti_semaphore_up_n_PAR)
void gasneti_semaphore_up_n_PAR(gasneti_semaphore_t_PAR *s, gasneti_atomic_val_t n) {
  GASNETI_SEMA_CHECK(s);
  _gasneti_semaphore_up_n(&(s->S), n);
  GASNETI_SEMA_CHECK(s);
}

/* gasneti_semaphore_trydown_n
 *
 * Decrements the semaphore by 'n' or fails.
 * If the "old" value is zero, returns zero.
 */
GASNETI_INLINE(gasneti_semaphore_trydown_n_PAR) GASNETI_WARN_UNUSED_RESULT
gasneti_atomic_val_t gasneti_semaphore_trydown_n_PAR(gasneti_semaphore_t_PAR *s, gasneti_atomic_val_t n) {
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
GASNETI_INLINE(gasneti_semaphore_trydown_partial_PAR) GASNETI_WARN_UNUSED_RESULT
gasneti_atomic_val_t gasneti_semaphore_trydown_partial_PAR(gasneti_semaphore_t_PAR *s, gasneti_atomic_val_t n) {
  gasneti_atomic_val_t retval;

  GASNETI_SEMA_CHECK(s);
  retval = _gasneti_semaphore_trydown_partial(&(s->S), n);
  GASNETI_SEMA_CHECK(s);

  return retval;
}

/* _SEQ implementation of semaphores */
#define GASNETI_SEMAPHORE_MAX_SEQ GASNETI_ATOMIC_MAX
#if GASNET_DEBUG
  typedef struct {
    gasneti_atomic_val_t count;
    gasneti_atomic_val_t limit;
  } gasneti_semaphore_t_SEQ;
  #define GASNETI_SEMAPHORE_INITIALIZER_SEQ(count,limit) { (count), (limit) }
  #define GASNETI_SEMA_CHECK_SEQ(_s)        do {             \
    gasneti_assert_uint((_s)->count ,<=, GASNETI_SEMAPHORE_MAX_SEQ); \
    gasneti_assert(((_s)->count <= (_s)->limit) || !(_s)->limit); \
  } while (0)
#else
  typedef struct {
    gasneti_atomic_val_t count;
  } gasneti_semaphore_t_SEQ;
  #define GASNETI_SEMAPHORE_INITIALIZER_SEQ(count,limit) { (count) }
  #define GASNETI_SEMA_CHECK_SEQ(_s) do { } while (0)
#endif
GASNETI_INLINE(gasneti_semaphore_init_SEQ)
void gasneti_semaphore_init_SEQ(gasneti_semaphore_t_SEQ *s,
                                gasneti_atomic_val_t value,
                                gasneti_atomic_val_t limit) {
  s->count = value;
#if GASNET_DEBUG
  s->limit = limit;
#endif
  GASNETI_SEMA_CHECK_SEQ(s);
}
GASNETI_INLINE(gasneti_semaphore_destroy_SEQ)
void gasneti_semaphore_destroy_SEQ(gasneti_semaphore_t_SEQ *s) {
  GASNETI_SEMA_CHECK_SEQ(s);
  /* Nothing */
}
GASNETI_INLINE(gasneti_semaphore_read_SEQ)
gasneti_atomic_val_t gasneti_semaphore_read_SEQ(gasneti_semaphore_t_SEQ *s) {
  GASNETI_SEMA_CHECK_SEQ(s);
  return s->count;
}
GASNETI_INLINE(gasneti_semaphore_up_SEQ)
void gasneti_semaphore_up_SEQ(gasneti_semaphore_t_SEQ *s) {
  GASNETI_SEMA_CHECK_SEQ(s);
  s->count += 1;
  GASNETI_SEMA_CHECK_SEQ(s);
}
GASNETI_INLINE(gasneti_semaphore_trydown_SEQ) GASNETI_WARN_UNUSED_RESULT
int gasneti_semaphore_trydown_SEQ(gasneti_semaphore_t_SEQ *s) {
  int retval;
  GASNETI_SEMA_CHECK_SEQ(s);
  retval = s->count;
  if_pt (retval != 0)
    s->count -= 1;
  GASNETI_SEMA_CHECK_SEQ(s);
  return retval;
}
GASNETI_INLINE(gasneti_semaphore_up_n_SEQ)
void gasneti_semaphore_up_n_SEQ(gasneti_semaphore_t_SEQ *s, gasneti_atomic_val_t n) {
  GASNETI_SEMA_CHECK_SEQ(s);
  s->count += n;
  GASNETI_SEMA_CHECK_SEQ(s);
}
GASNETI_INLINE(gasneti_semaphore_trydown_n_SEQ) GASNETI_WARN_UNUSED_RESULT
gasneti_atomic_val_t gasneti_semaphore_trydown_n_SEQ(gasneti_semaphore_t_SEQ *s, gasneti_atomic_val_t n) {
  GASNETI_SEMA_CHECK_SEQ(s);
  if_pt (s->count >= n) {
    s->count -= n;
  } else {
    n = 0;
  }
  GASNETI_SEMA_CHECK_SEQ(s);
  return n;
}
GASNETI_INLINE(gasneti_semaphore_trydown_partial_SEQ) GASNETI_WARN_UNUSED_RESULT
gasneti_atomic_val_t gasneti_semaphore_trydown_partial_SEQ(gasneti_semaphore_t_SEQ *s, gasneti_atomic_val_t n) {
  GASNETI_SEMA_CHECK_SEQ(s);
  n = MIN(s->count, n);
  s->count -= n;
  GASNETI_SEMA_CHECK_SEQ(s);
  return n;
}

/* Finally define gasneti_semaphore in terms of either the _PAR or _SEQ variant */
#define GASNETI_CONS_SEMA(_suff,_id)       _CONCAT(GASNETI_SEMAPHORE_,_CONCAT(_id,_suff))
#define gasneti_cons_sema(_suff,_id)       _CONCAT(gasneti_semaphore_,_CONCAT(_id,_suff))
#define GASNETI_SEMAPHORE_INITIALIZER      GASNETI_CONS_SEMA(_GASNETI_PARSEQ,INITIALIZER)
#define gasneti_semaphore_t                gasneti_cons_sema(_GASNETI_PARSEQ,t)
#define gasneti_semaphore_init             gasneti_cons_sema(_GASNETI_PARSEQ,init)
#define gasneti_semaphore_destroy          gasneti_cons_sema(_GASNETI_PARSEQ,destroy)
#define gasneti_semaphore_read             gasneti_cons_sema(_GASNETI_PARSEQ,read)
#define gasneti_semaphore_up               gasneti_cons_sema(_GASNETI_PARSEQ,up)
#define gasneti_semaphore_trydown          gasneti_cons_sema(_GASNETI_PARSEQ,trydown)
#define gasneti_semaphore_up_n             gasneti_cons_sema(_GASNETI_PARSEQ,up_n)
#define gasneti_semaphore_trydown_n        gasneti_cons_sema(_GASNETI_PARSEQ,trydown_n)
#define gasneti_semaphore_trydown_partial  gasneti_cons_sema(_GASNETI_PARSEQ,trydown_partial)

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
 *
 * NOTE: Because of the specifics of the representation, it is not permitted to copy
 * a gasneti_lifo_head_t by value.
 */


/* Optional arch-specific code */
#if !GASNETI_THREADS
  /* No threads, so we use the mutex code that compiles away. */
#elif defined(GASNETI_USE_GENERIC_ATOMICOPS)
  /* If using mutexes, then just use the mutex code */
#elif PLATFORM_ARCH_POWERPC && GASNETI_HAVE_ATOMIC_PTR_CAS && \
      GASNETI_HAVE_GCC_ASM
  /* Among the platforms we currently support, PPC is unique in having an LL/SC
   * construct which allows a load between the LL and the SC.
   */
  #if GASNETI_HAVE_GCC_ASM
    typedef struct {
      /* Ensure list head pointer is the only item on its cache line.
       * This prevents a live-lock which would result if a list element fell
       * on the same cache line.
       */
      char			_pad0[GASNETI_CACHE_LINE_BYTES];
      gasneti_atomic_ptr_t	head;
      char			_pad1[GASNETI_CACHE_LINE_BYTES];
    } gasneti_lifo_head_t_PAR;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t_PAR *p, void **head, void **tail) {
      /* RELEASE semantics */
      uintptr_t oldhead;
      do {
	oldhead = gasneti_atomic_ptr_read(&p->head);
	*tail = (void *)oldhead;
      } while (!gasneti_atomic_ptr_cas(&p->head, oldhead, (uintptr_t)head, GASNETI_ATOMIC_REL));
    }

    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t_PAR *p) {
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
        __asm__ __volatile__ ("1: \n\t"
			      "lwarx	%1,0,%0    \n\t" /* head = p->head */
			      "cmpwi	0,%1,0     \n\t" /* head == NULL? */
			      "beq-	2f         \n\t" /* end on NULL */
			      GASNETI_PPC_RMB_ASM "\n\t" /* rmb */
			      "lwz	%2,0(%1)   \n\t" /* next = head->next */
			      "stwcx.	%2,0,%0    \n\t" /* p->head = next */
			      "bne-	1b         \n"   /* retry on conflict */
			      "2: "
				: "=b" (addr), "=b" (head), "=r" (next)
				: "0" (addr)
				: "memory", "cc");
      #elif PLATFORM_ARCH_64
        __asm__ __volatile__ ("1: \n\t"
			      "ldarx	%1,0,%0    \n\t" /* head = p->head */
			      "cmpdi	0,%1,0     \n\t" /* head == NULL? */
			      "beq-	2f         \n\t" /* end on NULL */
			      GASNETI_PPC_RMB_ASM "\n\t" /* rmb */
			      "ld	%2,0(%1)   \n\t" /* next = head->next */
			      "stdcx.	%2,0,%0    \n\t" /* p->head = next */
			      "bne-	1b         \n"   /* retry on conflict */
			      "2: "
				: "=b" (addr), "=b" (head), "=r" (next)
				: "0" (addr)
				: "memory", "cc");
      #else
        #error "PPC w/ unknown word size"
      #endif
      return (void *)head;
    }
    GASNETI_INLINE(_gasneti_lifo_init)
    void _gasneti_lifo_init(gasneti_lifo_head_t_PAR *p) {
      gasneti_atomic_ptr_set(&p->head, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t_PAR *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER_PAR {{0,}, gasneti_atomic_ptr_init(0), {0,}}
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
    } gasneti_lifo_head_t_PAR;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t_PAR *p, void **newhead, void **tail) {
      uintptr_t tag, oldhead;
      do {
	oldhead = gasneti_atomic_dblptr_read_lo(&p->head_and_tag);
	tag = gasneti_atomic_dblptr_read_hi(&p->head_and_tag);
	*tail = (void *)oldhead;
      } while (!gasneti_atomic_dblptr_cas2(&p->head_and_tag, tag, oldhead, tag+1, (uintptr_t)newhead, GASNETI_ATOMIC_REL));
    }
    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t_PAR *p) {
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
    void _gasneti_lifo_init(gasneti_lifo_head_t_PAR *p) {
      gasneti_atomic_dblptr_set(&p->head_and_tag, 0, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t_PAR *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER_PAR {{0,}, gasneti_atomic_dblptr_init(0,0), {0,}}
    #define GASNETI_HAVE_ARCH_LIFO	1
#elif PLATFORM_ARCH_64 && defined(GASNETI_HAVE_ATOMIC128_T)
    /* Same algorithm as dblptr_cas, above, but with alignment worries added in */
    typedef struct {
      char                _pad0[GASNETI_CACHE_LINE_BYTES + GASNETI_HAVE_ATOMIC128_T];
      gasneti_atomic128_t head_and_tag; /* Actually might be lower addr, in _pad0 */
      char                _pad1[GASNETI_CACHE_LINE_BYTES];
    } gasneti_lifo_head_t_PAR;
    #define _GASNETI_LIFO_ALIGN(p) ((uintptr_t)(&(p)->head_and_tag) & ~(GASNETI_HAVE_ATOMIC128_T - 1))

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t_PAR *p, void **newhead, void **tail) {
    #if PLATFORM_COMPILER_PGI && PLATFORM_COMPILER_VERSION_GE(16,4,0) && PLATFORM_COMPILER_VERSION_LT(17,1,0)
      volatile uintptr_t tag; /* See GASNet bug #3324 */
    #else
      uintptr_t tag;
    #endif
      uintptr_t oldhead;
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
    void *_gasneti_lifo_pop(gasneti_lifo_head_t_PAR *p) {
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
    void _gasneti_lifo_init(gasneti_lifo_head_t_PAR *p) {
      gasneti_atomic128_t *head_and_tag = (gasneti_atomic128_t *)_GASNETI_LIFO_ALIGN(p);
      gasneti_atomic128_set(head_and_tag, 0, 0, 0);
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t_PAR *p) {
      /* NOTHING */
    }
    #define GASNETI_LIFO_INITIALIZER_PAR {{0,}, gasneti_atomic128_init(0,0), {0,}}
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
    #if PLATFORM_COMPILER_INTEL
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
      } gasneti_lifo_head_t_PAR;

      GASNETI_INLINE(_gasneti_lifo_push)
      void _gasneti_lifo_push(gasneti_lifo_head_t_PAR *p, void **head, void **tail) {
        uint64_t tag, old_head;
        void *q = (void *)GASNETI_ALIGNUP(p, 16);
        do {
          _gasneti_lifo_load16(q, tag, old_head);
          _gasneti_lifo_st8_rel(tail, old_head);
        } while (_gasneti_lifo_store16(q, tag, head));
      }
      GASNETI_INLINE(_gasneti_lifo_pop)
      void *_gasneti_lifo_pop(gasneti_lifo_head_t_PAR *p) {
        uint64_t tag, old_head;
        void *q = (void *)GASNETI_ALIGNUP(p, 16);
        do {
          _gasneti_lifo_load16(q, tag, old_head);
	  if (!old_head) break;
        } while (_gasneti_lifo_store16(q, tag, *(void **)old_head));
        return (void *)old_head;
      }
      GASNETI_INLINE(_gasneti_lifo_init)
      void _gasneti_lifo_init(gasneti_lifo_head_t_PAR *p) {
        void **q = (void **)GASNETI_ALIGNUP(p, 16);
        q[0] = q[1] = NULL;
      }
      GASNETI_INLINE(_gasneti_lifo_destroy)
      void _gasneti_lifo_destroy(gasneti_lifo_head_t_PAR *p) {
        /* NOTHING */
      }
      #define GASNETI_LIFO_INITIALIZE_PAR { { NULL, NULL, NULL} }
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
    } gasneti_lifo_head_t_PAR;

    GASNETI_INLINE(_gasneti_lifo_push)
    void _gasneti_lifo_push(gasneti_lifo_head_t_PAR *p, void **head, void **tail) {
      gasneti_mutex_lock(&(p->lock));
      *tail = p->head;
      p->head = head;
      gasneti_mutex_unlock(&(p->lock));
    }
    GASNETI_INLINE(_gasneti_lifo_pop)
    void *_gasneti_lifo_pop(gasneti_lifo_head_t_PAR *p) {
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
    void _gasneti_lifo_init(gasneti_lifo_head_t_PAR *p) {
      gasneti_mutex_init(&(p->lock));
      p->head = NULL;
    }
    GASNETI_INLINE(_gasneti_lifo_destroy)
    void _gasneti_lifo_destroy(gasneti_lifo_head_t_PAR *p) {
      gasneti_mutex_destroy(&(p->lock));
    }
    #define GASNETI_LIFO_INITIALIZER_PAR { GASNETI_MUTEX_INITIALIZER, NULL, {0,} }
    #define GASNETI_HAVE_ARCH_LIFO	0
    #define GASNETI_LIFOS_NOT_SIGNALSAFE 1
#endif
    
/* _SEQ variant of gasneti_lifo_head_t */
typedef struct { void **head; } gasneti_lifo_head_t_SEQ;
#define GASNETI_LIFO_INITIALIZER_SEQ  { NULL }

/* Initializer for dynamically allocated lifo heads */
GASNETI_INLINE(gasneti_lifo_init_PAR)
void gasneti_lifo_init_PAR(gasneti_lifo_head_t_PAR *lifo) {
  gasneti_assert(lifo != NULL);
  _gasneti_lifo_init(lifo);
}
GASNETI_INLINE(gasneti_lifo_init_SEQ)
void gasneti_lifo_init_SEQ(gasneti_lifo_head_t_SEQ *lifo) {
  gasneti_assert(lifo != NULL);
  lifo->head = NULL;
}

/* Destructor for dynamically allocated lifo heads */
GASNETI_INLINE(gasneti_lifo_destroy_PAR)
void gasneti_lifo_destroy_PAR(gasneti_lifo_head_t_PAR *lifo) {
  gasneti_assert(lifo != NULL);
  _gasneti_lifo_destroy(lifo);
}
GASNETI_INLINE(gasneti_lifo_destroy_SEQ)
void gasneti_lifo_destroy_SEQ(gasneti_lifo_head_t_SEQ *lifo) {
  gasneti_assert(lifo != NULL);
  /* Nothing */
}

/* Get one element from the LIFO or NULL if it is empty */
GASNETI_INLINE(gasneti_lifo_pop_PAR) GASNETI_MALLOC
void *gasneti_lifo_pop_PAR(gasneti_lifo_head_t_PAR *lifo) {
  gasneti_assert(lifo != NULL);
  return _gasneti_lifo_pop(lifo);
}
GASNETI_INLINE(gasneti_lifo_pop_SEQ) GASNETI_MALLOC
void *gasneti_lifo_pop_SEQ(gasneti_lifo_head_t_SEQ *lifo) {
  void **elem;
  gasneti_assert(lifo != NULL);
  elem = lifo->head;
  if_pt (elem != NULL) {
    lifo->head = *elem;
  }
  return (void *)elem;
}

/* Push element on the LIFO */
GASNETI_INLINE(gasneti_lifo_push_PAR)
void gasneti_lifo_push_PAR(gasneti_lifo_head_t_PAR *lifo, void *elem) {
  gasneti_assert(lifo != NULL);
  gasneti_assert(elem != NULL);
  _gasneti_lifo_push(lifo, elem, elem);
}
GASNETI_INLINE(gasneti_lifo_push_SEQ)
void gasneti_lifo_push_SEQ(gasneti_lifo_head_t_SEQ *lifo, void *elem) {
  gasneti_assert(lifo != NULL);
  gasneti_assert(elem != NULL);
  *(void**)elem = lifo->head;
  lifo->head = (void**)elem;
}

/* Push a chain of linked elements on the LIFO */
GASNETI_INLINE(gasneti_lifo_push_many_PAR)
void gasneti_lifo_push_many_PAR(gasneti_lifo_head_t_PAR *lifo, void *head, void *tail) {
  gasneti_assert(lifo != NULL);
  gasneti_assert(head != NULL);
  gasneti_assert(tail != NULL);
  _gasneti_lifo_push(lifo, head, tail);
}
GASNETI_INLINE(gasneti_lifo_push_many_SEQ)
void gasneti_lifo_push_many_SEQ(gasneti_lifo_head_t_SEQ *lifo, void *head, void *tail) {
  gasneti_assert(lifo != NULL);
  gasneti_assert(head != NULL);
  gasneti_assert(tail != NULL);
  *(void**)tail = lifo->head;
  lifo->head = (void**)head;
}

/* Build a chain (q follows p) for use with _lifo_push_many() (no PAR-vs-SEQ differences) */
GASNETI_INLINE(gasneti_lifo_link)
void gasneti_lifo_link(void *p, void *q) {
  gasneti_assert(p != NULL);
  gasneti_assert(q != NULL);
  *((void **)p) = q;
}

/* Get next element in a chain built with _lifo_link (no PAR-vs-SEQ differences) */
GASNETI_INLINE(gasneti_lifo_next)
void *gasneti_lifo_next(void *elem) {
  gasneti_assert(elem != NULL);
  return *((void **)elem);
}

/* Finally define gasneti_lifo_head in terms of either the _PAR or _SEQ variant */
#define GASNETI_CONS_LIFO(_suff,_id) _CONCAT(GASNETI_LIFO_,_CONCAT(_id,_suff))
#define gasneti_cons_lifo(_suff,_id) _CONCAT(gasneti_lifo_,_CONCAT(_id,_suff))
#define GASNETI_LIFO_INITIALIZER     GASNETI_CONS_LIFO(_GASNETI_PARSEQ,INITIALIZER)
#define gasneti_lifo_head_t          gasneti_cons_lifo(_GASNETI_PARSEQ,head_t)
#define gasneti_lifo_init            gasneti_cons_lifo(_GASNETI_PARSEQ,init)
#define gasneti_lifo_destroy         gasneti_cons_lifo(_GASNETI_PARSEQ,destroy)
#define gasneti_lifo_pop             gasneti_cons_lifo(_GASNETI_PARSEQ,pop)
#define gasneti_lifo_push            gasneti_cons_lifo(_GASNETI_PARSEQ,push)
#define gasneti_lifo_push_many       gasneti_cons_lifo(_GASNETI_PARSEQ,push_many)

/* ------------------------------------------------------------------------------------ */

#endif
