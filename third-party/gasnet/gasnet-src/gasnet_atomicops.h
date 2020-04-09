/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_atomicops.h $
 * Description: GASNet header for portable atomic memory operations
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNETEX_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_ATOMICOPS_H
#define _GASNET_ATOMICOPS_H

/* ------------------------------------------------------------------------------------ */
/* Portable atomic operations
   --------------------------

   see README-tools for general usage information

   Signal safety of atomic operations
   ----------------------------------
   On most, but not all, platforms, operations on gasneti_atomic_t are signal safe.  
   On the few platforms where this is not the case GASNETI_ATOMICOPS_NOT_SIGNALSAFE
   will be defined to 1.

   Similarly, GASNETI_ATOMIC32_NOT_SIGNALSAFE and GASNETI_ATOMIC64_NOT_SIGNALSAFE
   are defined to 1 IFF the implementation of the fixed-width atomics is not signal-safe.
   Note that these two are set independently.

   Mutexes
   -------
   If GASNETI_USE_GENERIC_ATOMICOPS is defined, then the gasnet atomics are
   implemented using mutexes.  Therefore, one may wish to consider using other
   algorithms when this symbol is defined.

   Similarly, GASNETI_USE_GENERIC_ATOMIC32 and GASNETI_USE_GENERIC_ATOMIC64
   are defined to 1 IFF the implementation of the fixed-width atomics uses mutexes.
   Note that these two are set independently.

 */

/* ------------------------------------------------------------------------------------ */
/* Flags for memory fences */
#define GASNETI_ATOMIC_NONE			0x00
#define GASNETI_ATOMIC_RMB_PRE			0x01
#define GASNETI_ATOMIC_WMB_PRE			0x02
#define GASNETI_ATOMIC_RMB_POST			0x04
#define GASNETI_ATOMIC_WMB_POST			0x08
#define GASNETI_ATOMIC_RMB_POST_IF_TRUE		0x10
#define GASNETI_ATOMIC_RMB_POST_IF_FALSE	0x20

/* OR into flags to make the weak atomics omit fences in a non-threaded build. */
#define GASNETI_ATOMIC_WEAK_FENCE		0x80000000

#define GASNETI_ATOMIC_MB_PRE		(GASNETI_ATOMIC_WMB_PRE | GASNETI_ATOMIC_RMB_PRE)
#define GASNETI_ATOMIC_MB_POST		(GASNETI_ATOMIC_WMB_POST | GASNETI_ATOMIC_RMB_POST)

#define GASNETI_ATOMIC_REL		GASNETI_ATOMIC_WMB_PRE
#define GASNETI_ATOMIC_ACQ		GASNETI_ATOMIC_RMB_POST
#define GASNETI_ATOMIC_ACQ_IF_TRUE	GASNETI_ATOMIC_RMB_POST_IF_TRUE
#define GASNETI_ATOMIC_ACQ_IF_FALSE	GASNETI_ATOMIC_RMB_POST_IF_FALSE

/* ------------------------------------------------------------------------------------ */
/* Uniform memory fences for GASNet atomics.
 */

#define GASNETI_ATOMIC_MASK_PRE         (GASNETI_ATOMIC_WMB_PRE | GASNETI_ATOMIC_RMB_PRE)
#define GASNETI_ATOMIC_MASK_POST        (GASNETI_ATOMIC_WMB_POST | GASNETI_ATOMIC_RMB_POST)
#define GASNETI_ATOMIC_MASK_BOOL        (GASNETI_ATOMIC_MASK_POST | \
                                         GASNETI_ATOMIC_RMB_POST_IF_TRUE | \
                                         GASNETI_ATOMIC_RMB_POST_IF_FALSE)

#define _gasneti_atomic_cf_before(f)	if (f & GASNETI_ATOMIC_MASK_PRE) gasneti_compiler_fence();
#define _gasneti_atomic_cf_after(f)	if (f & GASNETI_ATOMIC_MASK_POST) gasneti_compiler_fence();
#define _gasneti_atomic_cf_bool(f)	if (f & GASNETI_ATOMIC_MASK_BOOL) gasneti_compiler_fence();

/* Fencing Part 1.  Removal of fences which are redundant on a given platform
 *	_gasneti_atomic_fence_{before,after}(flags)
 *	_gasneti_atomic_fence_bool(flags, value)
 *
 * This level of macros serves to remove at, preprocess-time, any tests
 * which are redundant due to the relationships among fences.  For example,
 * on a platform with a single fence instruction that is mb(), rmb() and
 * wmb() these macros will reduce three conditionals to just one.
 */

#if GASNETI_RMB_IS_MB && GASNETI_WMB_IS_MB
  /* Since mb() == rmb() == wmb() (including case that all are empty), only
   * a single check is needed for all three.
   */
  #define _gasneti_atomic_fence_before(f)       if (f & GASNETI_ATOMIC_MASK_PRE) gasneti_local_mb();
  #define _gasneti_atomic_fence_after(f)        if (f & GASNETI_ATOMIC_MASK_POST) gasneti_local_mb();
#elif GASNETI_MB_IS_SUM
  /* Since mb() == rmb()+wmb(), distinct rmb() and wmb() checks are
   * sufficient to implement a request for mb(), rmb() or wmb().
   * This includes the case where either is just a compiler fence.
   */
  #define _gasneti_atomic_fence_before(f)       if (f & GASNETI_ATOMIC_RMB_PRE) gasneti_local_rmb(); \
                                                if (f & GASNETI_ATOMIC_WMB_PRE) gasneti_local_wmb();
  #define _gasneti_atomic_fence_after(f)        if (f & GASNETI_ATOMIC_RMB_POST) gasneti_local_rmb(); \
                                                if (f & GASNETI_ATOMIC_WMB_POST) gasneti_local_wmb();
#elif GASNETI_RMB_IS_MB
  /* Case with mb() == rmb() and a distinct wmb().
   */
  #define _gasneti_atomic_fence_before(f)	if (f & GASNETI_ATOMIC_RMB_PRE) gasneti_local_rmb(); \
                                                else if (f & GASNETI_ATOMIC_WMB_PRE) gasneti_local_wmb();
  #define _gasneti_atomic_fence_after(f)	if (f & GASNETI_ATOMIC_RMB_POST) gasneti_local_rmb(); \
                                                else if (f & GASNETI_ATOMIC_WMB_POST) gasneti_local_wmb();
#elif GASNETI_WMB_IS_MB
  /* Case with mb() == wmb() and a distinct rmb().
   */
  #define _gasneti_atomic_fence_before(f)	if (f & GASNETI_ATOMIC_WMB_PRE) gasneti_local_wmb(); \
                                                else if (f & GASNETI_ATOMIC_RMB_PRE) gasneti_local_rmb();
  #define _gasneti_atomic_fence_after(f)	if (f & GASNETI_ATOMIC_WMB_POST) gasneti_local_wmb(); \
                                                else if (f & GASNETI_ATOMIC_RMB_POST) gasneti_local_rmb();
#else
  /*  With distinct mb(), rmb() and wmb(), we make the most general 3 checks (like a "switch").
   */
  #define _gasneti_atomic_fence_before(f)	if ((f & GASNETI_ATOMIC_MASK_PRE) == GASNETI_ATOMIC_MB_PRE) gasneti_local_mb(); \
                                                else if (f & GASNETI_ATOMIC_RMB_PRE) gasneti_local_rmb(); \
                                                else if (f & GASNETI_ATOMIC_WMB_PRE) gasneti_local_wmb();
  #define _gasneti_atomic_fence_after(f)	if ((f & GASNETI_ATOMIC_MASK_POST) == GASNETI_ATOMIC_MB_POST) gasneti_local_mb(); \
                                                else if (f & GASNETI_ATOMIC_RMB_POST) gasneti_local_rmb(); \
                                                else if (f & GASNETI_ATOMIC_WMB_POST) gasneti_local_wmb();
#endif

#if 1
  /*
   * Several optimizations are possible when a conditional rmb() is combined
   * with an unconditional POST fence.  Such optimizations would prevent
   * imposing a "double" rmb() in such cases.  However:
   * 1) There are no current callers that mix *MB_POST with a
   *    conditional RMB_POST_IF*, and no likely reason to.
   * 2) Though they all reduce a great deal at compile-time,
   *    such "optimizations" look very large to the inliner
   *    before any dead code can be eliminated.
   * Therefore, they are not currently implemented.
   */
  #define _gasneti_atomic_fence_bool(f, v) \
    if (((f & GASNETI_ATOMIC_RMB_POST_IF_TRUE ) &&  v) || \
        ((f & GASNETI_ATOMIC_RMB_POST_IF_FALSE) && !v)) gasneti_local_rmb();
#endif

/* ------------------------------------------------------------------------------------ */
/* Misc non-public definitions needed in the platform-specific parts */

#define _GASNETI_ATOMIC_CHECKALIGN(_a,_p) \
    gasneti_assert(!(_a) || !(((uintptr_t)(_p))&((_a)-1)))

/* ------------------------------------------------------------------------------------ */
/* All the platform-specific parts */
#include <gasnet_atomic_bits.h>

/* ------------------------------------------------------------------------------------ */
/* Fencing Part 2. Fences for the platform-specific atomics, based on macros in Part 1.
 *	_gasneti_atomic{,32,64}_prologue_{set,read,rmw}(p, flags)
 *	_gasneti_atomic{,32,64}_fence_{before,after}_{set,read,rmw}(p, flags)
 *	_gasneti_atomic{,32,64}_fence_after_bool(p, flags, value)
 *
 * These can/should be predefined by the platform-specific code if there are
 * any fencing side-effects in the unfenced ("_" prefixed) implementaions.
 * Such overrides must be applied to entire groups, as can be seen below.
 *
 * The "atomic_" are used if the platform defines GASNETI_HAVE_PRIVATE_ATOMIC_T.
 * The "atomic32_" are used unless the platform defines GASNETI_USE_GENERIC_ATOMIC32
 * The "atomic64_" are used unless the platform defines GASNETI_USE_GENERIC_ATOMIC64
 */
#ifndef _gasneti_atomic_prologue_set
  #define _gasneti_atomic_prologue_set(p,f)          /*empty*/
  #define _gasneti_atomic_fence_before_set(p,f)      _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic_fence_after_set(p,f)       _gasneti_atomic_fence_after(f)
#endif
#ifndef _gasneti_atomic32_prologue_set
  #define _gasneti_atomic32_prologue_set(p,f)        /*empty*/
  #define _gasneti_atomic32_fence_before_set(p,f)    _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic32_fence_after_set(p,f)     _gasneti_atomic_fence_after(f)
#endif
#ifndef _gasneti_atomic64_prologue_set
  #define _gasneti_atomic64_prologue_set(p,f)        /*empty*/
  #define _gasneti_atomic64_fence_before_set(p,f)    _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic64_fence_after_set(p,f)     _gasneti_atomic_fence_after(f)
#endif
#ifndef _gasneti_atomic_prologue_read
  #define _gasneti_atomic_prologue_read(p,f)         /*empty*/
  #define _gasneti_atomic_fence_before_read(p,f)     _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic_fence_after_read(p,f)      _gasneti_atomic_fence_after(f)
#endif
#ifndef _gasneti_atomic32_prologue_read
  #define _gasneti_atomic32_prologue_read(p,f)       /*empty*/
  #define _gasneti_atomic32_fence_before_read(p,f)   _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic32_fence_after_read(p,f)    _gasneti_atomic_fence_after(f)
#endif
#ifndef _gasneti_atomic64_prologue_read
  #define _gasneti_atomic64_prologue_read(p,f)       /*empty*/
  #define _gasneti_atomic64_fence_before_read(p,f)   _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic64_fence_after_read(p,f)    _gasneti_atomic_fence_after(f)
#endif
#ifndef _gasneti_atomic_prologue_rmw
  #define _gasneti_atomic_prologue_rmw(p,f)          /*empty*/
  #define _gasneti_atomic_fence_before_rmw(p,f)      _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic_fence_after_rmw(p,f)       _gasneti_atomic_fence_after(f)
  #define _gasneti_atomic_fence_after_bool(p,f,v)    _gasneti_atomic_fence_after(f)  \
                                                     _gasneti_atomic_fence_bool(f,v)
#endif
#ifndef _gasneti_atomic32_prologue_rmw
  #define _gasneti_atomic32_prologue_rmw(p,f)        /*empty*/
  #define _gasneti_atomic32_fence_before_rmw(p,f)    _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic32_fence_after_rmw(p,f)     _gasneti_atomic_fence_after(f)
  #define _gasneti_atomic32_fence_after_bool(p,f,v)  _gasneti_atomic_fence_after(f)  \
                                                     _gasneti_atomic_fence_bool(f,v)
#endif
#ifndef _gasneti_atomic64_prologue_rmw
  #define _gasneti_atomic64_prologue_rmw(p,f)        /*empty*/
  #define _gasneti_atomic64_fence_before_rmw(p,f)    _gasneti_atomic_fence_before(f)
  #define _gasneti_atomic64_fence_after_rmw(p,f)     _gasneti_atomic_fence_after(f)
  #define _gasneti_atomic64_fence_after_bool(p,f,v)  _gasneti_atomic_fence_after(f)  \
                                                     _gasneti_atomic_fence_bool(f,v)
#endif

/* ------------------------------------------------------------------------------------ */
/* Fenced atomic templates, using the per-group fencing macros.
 */

#define GASNETI_ATOMIC_CHECKALIGN(stem,p) _GASNETI_ATOMIC_CHECKALIGN(stem##align,p)

#define GASNETI_ATOMIC_FENCED_SET(group,_func,stem,p,v,f)           \
  do {                                                              \
    stem##t * const __p = (p);                                      \
    const int __flags = (f);                                        \
    _gasneti_##group##_prologue_set(__p,__flags)                    \
    GASNETI_ATOMIC_CHECKALIGN(stem,__p);                            \
    _gasneti_##group##_fence_before_set(__p,__flags)                \
    _func(__p,(v));                                                 \
    _gasneti_##group##_fence_after_set(__p,__flags)                 \
  } while (0)
#define GASNETI_ATOMIC_FENCED_INCDEC(group,_func,stem,p,f)          \
  do {                                                              \
    stem##t * const __p = (p);                                      \
    const int __flags = (f);                                        \
    _gasneti_##group##_prologue_rmw(__p,__flags)                    \
    GASNETI_ATOMIC_CHECKALIGN(stem,__p);                            \
    _gasneti_##group##_fence_before_rmw(__p,__flags)                \
    _func(__p);                                                     \
    _gasneti_##group##_fence_after_rmw(__p,__flags)                 \
  } while (0)

#define GASNETI_ATOMIC_FENCED_SET_DEFN_NOT_INLINE(group,func,_func,stem) \
  void func(stem##t *_p, stem##val_t _v, const int _flags) {        \
    GASNETI_ATOMIC_FENCED_SET(group,_func,stem,_p,_v,_flags);       \
  }
#define GASNETI_ATOMIC_FENCED_INCDEC_DEFN_NOT_INLINE(group,func,_func,stem) \
  void func(stem##t *_p, const int _flags) {                        \
    GASNETI_ATOMIC_FENCED_INCDEC(group,_func,stem,_p,_flags);       \
  }
#define GASNETI_ATOMIC_FENCED_READ_DEFN_NOT_INLINE(group,func,_func,stem) \
  stem##val_t func(stem##t *_p, const int _flags) {                 \
    _gasneti_##group##_prologue_read(_p,_flags)                     \
    GASNETI_ATOMIC_CHECKALIGN(stem,_p);                             \
    _gasneti_##group##_fence_before_read(_p,_flags)                 \
    { const stem##val_t _retval = _func(_p);                        \
      _gasneti_##group##_fence_after_read(_p,_flags)                \
      return _retval;                                               \
    }                                                               \
  }
#define GASNETI_ATOMIC_FENCED_DECTEST_DEFN_NOT_INLINE(group,func,_func,stem) \
  int func(stem##t *_p, const int _flags) {                         \
    _gasneti_##group##_prologue_rmw(_p,_flags)                      \
    GASNETI_ATOMIC_CHECKALIGN(stem,_p);                             \
    _gasneti_##group##_fence_before_rmw(_p,_flags)                  \
    { const int _retval = _func(_p);                                \
      _gasneti_##group##_fence_after_bool(_p,_flags, _retval)       \
      return _retval;                                               \
    }                                                               \
  }
#define GASNETI_ATOMIC_FENCED_CAS_DEFN_NOT_INLINE(group,func,_func,stem) \
  int func(stem##t *_p, stem##val_t _oldval,                        \
           stem##val_t _newval, const int _flags) {                 \
    _gasneti_##group##_prologue_rmw(_p,_flags)                      \
    GASNETI_ATOMIC_CHECKALIGN(stem,_p);                             \
    _gasneti_##group##_fence_before_rmw(_p,_flags)                  \
    { const int _retval = _func(_p,_oldval,_newval);                \
      _gasneti_##group##_fence_after_bool(_p,_flags, _retval)       \
      return _retval;                                               \
    }                                                               \
  }
#define GASNETI_ATOMIC_FENCED_SWAP_DEFN_NOT_INLINE(group,func,_func,stem) \
  stem##val_t func(stem##t *_p, stem##val_t _val, const int _flags) { \
    _gasneti_##group##_prologue_rmw(_p,_flags)                      \
    GASNETI_ATOMIC_CHECKALIGN(stem,_p);                             \
    _gasneti_##group##_fence_before_rmw(_p,_flags)                  \
    { const stem##val_t _retval = _func(_p, _val);                  \
      _gasneti_##group##_fence_after_rmw(_p,_flags)                 \
      return _retval;                                               \
    }                                                               \
  }
#define GASNETI_ATOMIC_FENCED_ADDSUB_DEFN_NOT_INLINE(group,func,_func,stem) \
  stem##val_t func(stem##t *_p, stem##val_t _op, const int _flags) { \
    _gasneti_##group##_prologue_rmw(_p,_flags)                      \
    GASNETI_ATOMIC_CHECKALIGN(stem,_p);                             \
    _gasneti_##group##_fence_before_rmw(_p,_flags)                  \
    { const stem##val_t _retval = _func(_p, _op);                   \
      _gasneti_##group##_fence_after_rmw(_p,_flags)                 \
      return _retval;                                               \
    }                                                               \
  }
#define GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN_NOT_INLINE(group,func,_func,stem) \
  stem##val_t func(stem##t *_p, stem##val_t _op, const int _flags) {\
    _gasneti_##group##_prologue_rmw(_p,_flags)                      \
    GASNETI_ATOMIC_CHECKALIGN(stem,_p);                             \
    _gasneti_##group##_fence_before_rmw(_p,_flags)                  \
    { const stem##val_t _retval = _func(_p, _op);                   \
      _gasneti_##group##_fence_after_rmw(_p,_flags)                 \
      return _retval;                                               \
    }                                                               \
  }

#define GASNETI_ATOMIC_FENCED_SET_DEFN(group,func,_func,stem)       \
	GASNETI_INLINE(func)                                        \
	GASNETI_ATOMIC_FENCED_SET_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_INCDEC_DEFN(group,func,_func,stem)    \
	GASNETI_INLINE(func)                                        \
	GASNETI_ATOMIC_FENCED_INCDEC_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_READ_DEFN(group,func,_func,stem)      \
	GASNETI_INLINE(func)                                        \
        GASNETI_ATOMIC_FENCED_READ_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_DECTEST_DEFN(group,func,_func,stem)   \
	GASNETI_INLINE(func)                                        \
        GASNETI_ATOMIC_FENCED_DECTEST_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_CAS_DEFN(group,func,_func,stem)       \
	GASNETI_INLINE(func)                                        \
        GASNETI_ATOMIC_FENCED_CAS_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_SWAP_DEFN(group,func,_func,stem)      \
        GASNETI_INLINE(func)                                        \
        GASNETI_ATOMIC_FENCED_SWAP_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(group,func,_func,stem)    \
	GASNETI_INLINE(func)                                        \
        GASNETI_ATOMIC_FENCED_ADDSUB_DEFN_NOT_INLINE(group,func,_func,stem)
#define GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN(group,func,_func,stem)  \
	GASNETI_INLINE(func)                                        \
        GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN_NOT_INLINE(group,func,_func,stem)

/* ------------------------------------------------------------------------------------ */
/* Typeless unfenced operations on a pointer to a (volatile) scalar */

#define _gasneti_scalar_atomic_init(v)               (v)
#define _gasneti_scalar_atomic_set(p,v)              (*(p) = (v))
#define _gasneti_scalar_atomic_read(p)               (*(p))
#define _gasneti_scalar_atomic_increment(p)          ((*(p))++)
#define _gasneti_scalar_atomic_decrement(p)          ((*(p))--)
#define _gasneti_scalar_atomic_decrement_and_test(p) ((--(*(p))) == 0)
#define _gasneti_scalar_atomic_compare_and_swap(p,oval,nval) \
                                                     (*(p) == (oval) ? (*(p) = (nval), 1) : 0)
#define _gasneti_scalar_atomic_addfetch(p,op)        (*(p) += (op))
#define _gasneti_scalar_atomic_add(p,op)             (*(p) += (op))
#define _gasneti_scalar_atomic_subtract(p,op)        (*(p) -= (op))

/* Swap, as above, but not typless due to need for a temporary */
#define GASNETI_SCALAR_ATOMIC_SWAP_DEFN(func,stem)                      \
  GASNETI_INLINE(func) stem##val_t func(stem##t *_p, stem##val_t _val) {\
    const stem##val_t _retval = *_p; *_p = _val; return _retval;        \
  }

/* ------------------------------------------------------------------------------------ */
/* Fenced generic atomics, if needed, using per-platform defns and the macros above.
 */

#if defined(GASNETI_BUILD_GENERIC_ATOMIC32) || defined(GASNETI_BUILD_GENERIC_ATOMIC64)
  /* Fences for the generics */
  #ifndef GASNETI_GENATOMIC_LOCK
    /* Not locking, so use full fences */
    #define _gasneti_genatomic_prologue_set(p,f)        /*empty*/
    #define _gasneti_genatomic_prologue_read(p,f)       /*empty*/
    #define _gasneti_genatomic_prologue_rmw(p,f)        /*empty*/
    #define _gasneti_genatomic_fence_before_rmw(p,f)    _gasneti_atomic_fence_before(f)
    #define _gasneti_genatomic_fence_after_rmw(p,f)     _gasneti_atomic_fence_after(f)
    #define _gasneti_genatomic_fence_after_bool(p,f,v)  _gasneti_atomic_fence_bool(f,v)
    #define _GASNETI_GENATOMIC_DECL_AND_DEFN(_sz)                                     \
      typedef volatile uint##_sz##_t gasneti_genatomic##_sz##_t;                      \
      typedef uint##_sz##_t gasneti_genatomic##_sz##_val_t;                           \
      typedef int##_sz##_t gasneti_genatomic##_sz##_sval_t;                           \
      GASNETI_ATOMIC_FENCED_SET_DEFN(genatomic,                                       \
                                     gasneti_genatomic##_sz##_set,                    \
                                     _gasneti_scalar_atomic_set,                      \
                                     gasneti_genatomic##_sz##_)                       \
      GASNETI_ATOMIC_FENCED_INCDEC_DEFN(genatomic,                                    \
                                        gasneti_genatomic##_sz##_increment,           \
                                        _gasneti_scalar_atomic_increment,             \
                                        gasneti_genatomic##_sz##_)                    \
      GASNETI_ATOMIC_FENCED_INCDEC_DEFN(genatomic,                                    \
                                        gasneti_genatomic##_sz##_decrement,           \
                                        _gasneti_scalar_atomic_decrement,             \
                                        gasneti_genatomic##_sz##_)                    \
      GASNETI_ATOMIC_FENCED_DECTEST_DEFN(genatomic,                                   \
                                         gasneti_genatomic##_sz##_decrement_and_test, \
                                         _gasneti_scalar_atomic_decrement_and_test,   \
                                         gasneti_genatomic##_sz##_)                   \
      GASNETI_ATOMIC_FENCED_CAS_DEFN(genatomic,                                       \
                                     gasneti_genatomic##_sz##_compare_and_swap,       \
                                     _gasneti_scalar_atomic_compare_and_swap,         \
                                     gasneti_genatomic##_sz##_)                       \
      GASNETI_SCALAR_ATOMIC_SWAP_DEFN(_gasneti_genatomic##_sz##_swap,                 \
                                      gasneti_genatomic##_sz##_)                      \
      GASNETI_ATOMIC_FENCED_SWAP_DEFN(genatomic,                                      \
                                      gasneti_genatomic##_sz##_swap,                  \
                                      _gasneti_genatomic##_sz##_swap,                 \
                                      gasneti_genatomic##_sz##_)                      \
      GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN(genatomic,                                  \
                                          gasneti_genatomic##_sz##_addfetch,          \
                                          _gasneti_scalar_atomic_addfetch,            \
                                          gasneti_genatomic##_sz##_)
  #else /* Mutex-based (HSL or pthread mutex) versions */
    /* The lock acquire includes RMB and release includes WMB */
    #define _gasneti_genatomic_prologue_set(p,f)        GASNETI_GENATOMIC_LOCK_PREP(p);
    #define _gasneti_genatomic_prologue_read(p,f)       /*empty*/
    #define _gasneti_genatomic_prologue_rmw(p,f)        GASNETI_GENATOMIC_LOCK_PREP(p);
    #define _gasneti_genatomic_fence_before_rmw(p,f)	_gasneti_atomic_fence_before((f&~GASNETI_ATOMIC_RMB_PRE)) \
							GASNETI_GENATOMIC_LOCK();
    #define _gasneti_genatomic_fence_after_rmw(p,f)	GASNETI_GENATOMIC_UNLOCK();   \
							_gasneti_atomic_fence_after((f&~GASNETI_ATOMIC_WMB_POST))
    #define _gasneti_genatomic_fence_after_bool(p,f,v)	GASNETI_GENATOMIC_UNLOCK();   \
							_gasneti_atomic_fence_after((f&~GASNETI_ATOMIC_WMB_POST))\
							_gasneti_atomic_fence_bool(f,v)

    /* Because HSL's are not yet available (bug 693: avoid header dependency cycle),
     * we don't define the lock-acquiring operations as inlines.
     * Therefore, we declared them here but define them in gasnet_{internal,tools}.c
     */
    #define _GASNETI_GENATOMIC_DECL_AND_DEFN(_sz)                                            \
      typedef volatile uint##_sz##_t gasneti_genatomic##_sz##_t;                             \
      typedef uint##_sz##_t gasneti_genatomic##_sz##_val_t;                                  \
      typedef int##_sz##_t gasneti_genatomic##_sz##_sval_t;                                  \
      extern void gasneti_genatomic##_sz##_set(gasneti_genatomic##_sz##_t *_p,               \
                                               gasneti_genatomic##_sz##_val_t _v,            \
                                               const int _flags);                            \
      extern void gasneti_genatomic##_sz##_increment(gasneti_genatomic##_sz##_t *_p,         \
                                                     const int _flags);                      \
      extern void gasneti_genatomic##_sz##_decrement(gasneti_genatomic##_sz##_t *_p,         \
                                                     const int _flags);                      \
      extern int gasneti_genatomic##_sz##_decrement_and_test(gasneti_genatomic##_sz##_t *_p, \
                                                             const int _flags);              \
      extern int gasneti_genatomic##_sz##_compare_and_swap(gasneti_genatomic##_sz##_t *_p,   \
                                                           uint##_sz##_t _oldval,            \
                                                           uint##_sz##_t _newval,            \
                                                           const int _flags);                \
      extern uint##_sz##_t gasneti_genatomic##_sz##_swap(gasneti_genatomic##_sz##_t *_p,     \
                                                         uint##_sz##_t _val,                 \
                                                         const int _flags);                  \
      extern uint##_sz##_t gasneti_genatomic##_sz##_addfetch(gasneti_genatomic##_sz##_t *_p, \
                                                             uint##_sz##_t _op,              \
                                                             const int _flags);               
    #define _GASNETI_GENATOMIC_DEFN(_sz)                                                         \
      GASNETI_ATOMIC_FENCED_SET_DEFN_NOT_INLINE(genatomic,                                       \
                                                gasneti_genatomic##_sz##_set,                    \
                                                _gasneti_scalar_atomic_set,                      \
                                                gasneti_genatomic##_sz##_)                       \
      GASNETI_ATOMIC_FENCED_INCDEC_DEFN_NOT_INLINE(genatomic,                                    \
                                                   gasneti_genatomic##_sz##_increment,           \
                                                   _gasneti_scalar_atomic_increment,             \
                                                   gasneti_genatomic##_sz##_)                    \
      GASNETI_ATOMIC_FENCED_INCDEC_DEFN_NOT_INLINE(genatomic,                                    \
                                                   gasneti_genatomic##_sz##_decrement,           \
                                                   _gasneti_scalar_atomic_decrement,             \
                                                   gasneti_genatomic##_sz##_)                    \
      GASNETI_ATOMIC_FENCED_DECTEST_DEFN_NOT_INLINE(genatomic,                                   \
                                                    gasneti_genatomic##_sz##_decrement_and_test, \
                                                    _gasneti_scalar_atomic_decrement_and_test,   \
                                                    gasneti_genatomic##_sz##_)                   \
      GASNETI_ATOMIC_FENCED_CAS_DEFN_NOT_INLINE(genatomic,                                       \
                                                gasneti_genatomic##_sz##_compare_and_swap,       \
                                                _gasneti_scalar_atomic_compare_and_swap,         \
                                                gasneti_genatomic##_sz##_)                       \
      GASNETI_SCALAR_ATOMIC_SWAP_DEFN(_gasneti_genatomic##_sz##_swap,                            \
                                      gasneti_genatomic##_sz##_)                                 \
      GASNETI_ATOMIC_FENCED_SWAP_DEFN_NOT_INLINE(genatomic,                                      \
                                                 gasneti_genatomic##_sz##_swap,                  \
                                                 _gasneti_genatomic##_sz##_swap,                 \
                                                 gasneti_genatomic##_sz##_)                      \
      GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN_NOT_INLINE(genatomic,                                  \
                                                     gasneti_genatomic##_sz##_addfetch,          \
                                                     _gasneti_scalar_atomic_addfetch,            \
                                                     gasneti_genatomic##_sz##_)
  #endif
  #define _gasneti_genatomic_fence_before_set		_gasneti_genatomic_fence_before_rmw
  #define _gasneti_genatomic_fence_after_set		_gasneti_genatomic_fence_after_rmw
  /* READ is almost always performed without the lock (if any) held */
  #define _gasneti_genatomic_fence_before_read(p,f)	_gasneti_atomic_fence_before(f)
  #define _gasneti_genatomic_fence_after_read(p,f)	_gasneti_atomic_fence_after(f)

  /* Build the 32-bit generics if needed */
  #ifdef GASNETI_BUILD_GENERIC_ATOMIC32
    _GASNETI_GENATOMIC_DECL_AND_DEFN(32)
    #define gasneti_genatomic32_init          _gasneti_scalar_atomic_init
    GASNETI_ATOMIC_FENCED_READ_DEFN(genatomic,gasneti_genatomic32_read,
                                    _gasneti_scalar_atomic_read,gasneti_genatomic32_)
    #ifdef _GASNETI_GENATOMIC_DEFN
      #define GASNETI_GENATOMIC32_DEFN        _GASNETI_GENATOMIC_DEFN(32)
    #endif
  #endif

  /* Build the 64-bit generics if needed */
  #ifdef GASNETI_BUILD_GENERIC_ATOMIC64
    _GASNETI_GENATOMIC_DECL_AND_DEFN(64)
    #define gasneti_genatomic64_init          _gasneti_scalar_atomic_init
    #ifdef gasneti_genatomic64_read	/* ILP32 or HYBRID for under-aligned ABIs */
      /* Mutex is needed in read to avoid word tearing.
       * Can't use the normal template w/o also forcing a mutex into the 32-bit generics.
       * Note that we use the "rmw" fencing macros here, since the "read" fencing macros
       * assume no lock is taken and thus would potentially double fence.
       */
      extern uint64_t gasneti_genatomic64_read(gasneti_genatomic64_t *_p, const int _flags);
      #define _GASNETI_GENATOMIC64_DEFN_EXTRA \
	uint64_t gasneti_genatomic64_read(gasneti_genatomic64_t *_p, const int _flags) { \
          _gasneti_genatomic_prologue_rmw(_p,_flags)      /* rmw is NOT a typo here */   \
          GASNETI_ATOMIC_CHECKALIGN(gasneti_genatomic64_,_p);                            \
	  _gasneti_genatomic_fence_before_rmw(_p,_flags)  /* rmw is NOT a typo here */   \
	  { const uint64_t _retval = _gasneti_scalar_atomic_read(_p);                    \
	    _gasneti_genatomic_fence_after_rmw(_p,_flags) /* rmw is NOT a typo here */   \
	    return _retval;                                                              \
	  }                                                                              \
	}
    #else
      /* Read is assumed naturally atomic due to word size, or it doesn't matter in a serial build. */
      GASNETI_ATOMIC_FENCED_READ_DEFN(genatomic,gasneti_genatomic64_read,
                                      _gasneti_scalar_atomic_read,gasneti_genatomic64_)
      #define _GASNETI_GENATOMIC64_DEFN_EXTRA /* Empty */
    #endif
    #ifndef _GASNETI_GENATOMIC_DEFN
      #define _GASNETI_GENATOMIC_DEFN(_sz) /* Empty */
    #endif
    #define GASNETI_GENATOMIC64_DEFN        _GASNETI_GENATOMIC_DEFN(64) \
                                            _GASNETI_GENATOMIC64_DEFN_EXTRA
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Define the fully-fenced fixed-width atomics
 */

typedef uint32_t gasneti_atomic32_val_t;	/* For consistency in fencing macros */
typedef int32_t gasneti_atomic32_sval_t;	/* For consistency in fencing macros */
#if defined(GASNETI_USE_GENERIC_ATOMIC32)
  /* Define 32-bit fixed-width atomics in terms of full-fenced generics */
  #define gasneti_atomic32_t                   gasneti_genatomic32_t
  #define gasneti_atomic32_init                gasneti_genatomic32_init
  #define gasneti_genatomic32_add(p,op,f)      ((uint32_t)gasneti_genatomic32_addfetch((p),(op),(f)))
  #define gasneti_genatomic32_subtract(p,op,f) ((uint32_t)gasneti_genatomic32_addfetch((p),(uint32_t)-(op),(f)))
  #define _gasneti_atomic32_cons(_id)          gasneti_genatomic32_##_id
#elif defined(GASNETI_USING_SLOW_ATOMIC32)
  typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
  #define gasneti_atomic32_init(v) { (v) }
  #define gasneti_slow_atomic32_t     gasneti_atomic32_t
  #define gasneti_slow_atomic32_init  gasneti_atomic32_init
  #define gasneti_slow_atomic32_align gasneti_atomic32_align
  extern uint32_t gasneti_slow_atomic32_read(gasneti_atomic32_t *_p, const int _flags);
  extern void gasneti_slow_atomic32_set(gasneti_atomic32_t *_p, uint32_t _v, const int _flags);
  extern void gasneti_slow_atomic32_increment(gasneti_atomic32_t *_p, const int _flags);
  extern void gasneti_slow_atomic32_decrement(gasneti_atomic32_t *_p, const int _flags);
  extern int gasneti_slow_atomic32_decrement_and_test(gasneti_atomic32_t *_p, const int _flags);
  extern int gasneti_slow_atomic32_compare_and_swap(gasneti_atomic32_t *_p, uint32_t _oldval, uint32_t _newval, const int _flags);
  extern uint32_t gasneti_slow_atomic32_swap(gasneti_atomic32_t *_p, uint32_t _val, const int _flags);
  extern uint32_t gasneti_slow_atomic32_add(gasneti_atomic32_t *_p, uint32_t _op, const int _flags);
  extern uint32_t gasneti_slow_atomic32_subtract(gasneti_atomic32_t *_p, uint32_t _op, const int _flags);
  #define _gasneti_atomic32_cons(_id)    gasneti_slow_atomic32_##_id
#else
  /* Define 32-bit fixed-width atomics in terms of un-fenced native atomics */

  /* First define a fully-fenced addfetch if it appears to be needed */
  #if defined(_gasneti_atomic32_addfetch) || defined(_gasneti_atomic32_fetchadd)
    #ifndef _gasneti_atomic32_addfetch
      #define _gasneti_atomic32_addfetch(p,op) ((op) + _gasneti_atomic32_fetchadd((p),(op)))
    #endif
    GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN(atomic32,                   \
                                        gasneti_atomic32_addfetch,  \
                                        _gasneti_atomic32_addfetch, \
                                        gasneti_atomic32_)
  #elif defined(gasneti_atomic32_fetchadd)
    GASNETI_INLINE(gasneti_atomic32_addfetch)
    uint32_t gasneti_atomic32_addfetch(gasneti_atomic32_t *_p, uint32_t _op, int _f) {
      return (uint32_t)(gasneti_atomic32_fetchadd(_p,_op,_f) + _op);
    }
  #endif

  #ifdef _gasneti_atomic32_set
    #define gasneti_atomic32_set(p,v,f) GASNETI_ATOMIC_FENCED_SET(atomic32,_gasneti_atomic32_set,gasneti_atomic32_,p,v,f)
  #endif
  #ifdef _gasneti_atomic32_read
    GASNETI_ATOMIC_FENCED_READ_DEFN(atomic32,gasneti_atomic32_read,_gasneti_atomic32_read,gasneti_atomic32_)
  #endif
  #ifdef _gasneti_atomic32_increment
    #define gasneti_atomic32_increment(p,f) GASNETI_ATOMIC_FENCED_INCDEC(atomic32,_gasneti_atomic32_increment,gasneti_atomic32_,p,f)
  #elif !defined(gasneti_atomic32_increment)
    #define gasneti_atomic32_increment(p,f) ((void)gasneti_atomic32_addfetch((p),1,(f)))
  #endif
  #ifdef _gasneti_atomic32_decrement
    #define gasneti_atomic32_decrement(p,f) GASNETI_ATOMIC_FENCED_INCDEC(atomic32,_gasneti_atomic32_decrement,gasneti_atomic32_,p,f)
  #elif !defined(gasneti_atomic32_decrement)
    #define gasneti_atomic32_decrement(p,f) ((void)gasneti_atomic32_addfetch((p),(uint32_t)-1,(f)))
  #endif
  #ifndef gasneti_atomic32_decrement_and_test
    #ifndef _gasneti_atomic32_decrement_and_test
      #define _gasneti_atomic32_decrement_and_test(p) (0==_gasneti_atomic32_addfetch((p),(uint32_t)-1))
    #endif
    GASNETI_ATOMIC_FENCED_DECTEST_DEFN(atomic32,gasneti_atomic32_decrement_and_test,_gasneti_atomic32_decrement_and_test,gasneti_atomic32_)
  #endif
  #ifdef _gasneti_atomic32_compare_and_swap
    GASNETI_ATOMIC_FENCED_CAS_DEFN(atomic32,gasneti_atomic32_compare_and_swap,_gasneti_atomic32_compare_and_swap,gasneti_atomic32_)
  #endif
  #ifdef _gasneti_atomic32_swap
    GASNETI_ATOMIC_FENCED_SWAP_DEFN(atomic32,gasneti_atomic32_swap,_gasneti_atomic32_swap,gasneti_atomic32_)
  #endif
  #ifdef _gasneti_atomic32_add
    GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(atomic32,gasneti_atomic32_add,_gasneti_atomic32_add,gasneti_atomic32_)
  #elif !defined(gasneti_atomic32_add)
    #define gasneti_atomic32_add(p,op,f) ((uint32_t)gasneti_atomic32_addfetch((p),(uint32_t)(op),(f)))
  #endif
  #ifdef _gasneti_atomic32_subtract
    GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(atomic32,gasneti_atomic32_subtract,_gasneti_atomic32_subtract,gasneti_atomic32_)
  #elif !defined(gasneti_atomic32_subtract)
    #define gasneti_atomic32_subtract(p,op,f) ((uint32_t)gasneti_atomic32_addfetch((p),(uint32_t)-(op),(f)))
  #endif
#endif
#ifdef _gasneti_atomic32_cons
  #define gasneti_atomic32_read                 _gasneti_atomic32_cons(read)
  #define gasneti_atomic32_set                  _gasneti_atomic32_cons(set)
  #define gasneti_atomic32_increment            _gasneti_atomic32_cons(increment)
  #define gasneti_atomic32_decrement            _gasneti_atomic32_cons(decrement)
  #define gasneti_atomic32_decrement_and_test   _gasneti_atomic32_cons(decrement_and_test)
  #define gasneti_atomic32_compare_and_swap     _gasneti_atomic32_cons(compare_and_swap)
  #define gasneti_atomic32_swap                 _gasneti_atomic32_cons(swap)
  #define gasneti_atomic32_add                  _gasneti_atomic32_cons(add)
  #define gasneti_atomic32_subtract             _gasneti_atomic32_cons(subtract)
#endif

typedef uint64_t gasneti_atomic64_val_t;	/* For consistency in fencing macros */
typedef int64_t gasneti_atomic64_sval_t;	/* For consistency in fencing macros */
#if defined(GASNETI_USE_GENERIC_ATOMIC64)
  /* Define 64-bit fixed-width atomics in terms of full-fenced generics */
  #define gasneti_atomic64_t                   gasneti_genatomic64_t
  #define gasneti_atomic64_init                gasneti_genatomic64_init
  #define gasneti_genatomic64_add(p,op,f)      ((uint64_t)gasneti_genatomic64_addfetch((p),(uint64_t)(op),(f)))
  #define gasneti_genatomic64_subtract(p,op,f) ((uint64_t)gasneti_genatomic64_addfetch((p),(uint64_t)-(op),(f)))
  #define _gasneti_atomic64_cons(_id)          gasneti_genatomic64_##_id
#elif defined(GASNETI_USING_SLOW_ATOMIC64)
  typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
  #define gasneti_atomic64_init(v) { (v) }
  #define gasneti_slow_atomic64_t     gasneti_atomic64_t
  #define gasneti_slow_atomic64_init  gasneti_atomic64_init
  #define gasneti_slow_atomic64_align gasneti_atomic64_align
  extern uint64_t gasneti_slow_atomic64_read(gasneti_atomic64_t *_p, const int _flags);
  extern void gasneti_slow_atomic64_set(gasneti_atomic64_t *_p, uint64_t _v, const int _flags);
  extern void gasneti_slow_atomic64_increment(gasneti_atomic64_t *_p, const int _flags);
  extern void gasneti_slow_atomic64_decrement(gasneti_atomic64_t *_p, const int _flags);
  extern int gasneti_slow_atomic64_decrement_and_test(gasneti_atomic64_t *_p, const int _flags);
  extern int gasneti_slow_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval, const int _flags);
  extern uint64_t gasneti_slow_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _val, const int _flags);
  extern uint64_t gasneti_slow_atomic64_add(gasneti_atomic64_t *_p, uint64_t _op, const int _flags);
  extern uint64_t gasneti_slow_atomic64_subtract(gasneti_atomic64_t *_p, uint64_t _op, const int _flags);
  #define _gasneti_atomic64_cons(_id)    gasneti_slow_atomic64_##_id
#elif defined(GASNETI_HYBRID_ATOMIC64)
  /* Hybrid: need to runtime select between native and generic, based on alignment.
   *
   * Note that this code is currently only used for ILP32 on PPC64 h/w on Darwin.
   * Consequently, it is less general in its assumptions about what thas been
   * implemented in the platform-specific code.  In particular we require the
   * following in the current revision of "hybid" atomics support:
   *     _gasneti_atomic64_{set,read,compare_and_swap,swap,fetchadd}
   */
  
  #define __gasneti_atomic64_set(p,v,f) GASNETI_ATOMIC_FENCED_SET(atomic64,_gasneti_atomic64_set,gasneti_atomic64_,p,v,f)
  #define gasneti_atomic64_set(p,v,f) do {                                   \
      const int _f = (f);                                                    \
      const uint64_t _v = (v);                                               \
      gasneti_atomic64_t * const _p = (p);                                   \
      if_pt (!((uintptr_t)_p & 0x7)) {                                       \
        __gasneti_atomic64_set(_p,_v,_f);                                    \
      } else {                                                               \
	gasneti_genatomic64_set((gasneti_genatomic64_t *)_p,_v,_f);          \
      }                                                                      \
    } while (0)
  GASNETI_ATOMIC_FENCED_READ_DEFN(atomic64,__gasneti_atomic64_read,_gasneti_atomic64_read,gasneti_atomic64_)
  GASNETI_INLINE(gasneti_atomic64_read)
  uint64_t gasneti_atomic64_read(gasneti_atomic64_t *_p, const int _flags) {
    if_pt (!((uintptr_t)_p & 0x7)) {
      return __gasneti_atomic64_read(_p, _flags);
    } else {
      return gasneti_genatomic64_read((gasneti_genatomic64_t *)_p, _flags);
    }
  }
  GASNETI_ATOMIC_FENCED_CAS_DEFN(atomic64,__gasneti_atomic64_compare_and_swap,_gasneti_atomic64_compare_and_swap,gasneti_atomic64_)
  GASNETI_INLINE(gasneti_atomic64_compare_and_swap)
  int gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval,
					uint64_t _newval, const int _flags) {
    if_pt (!((uintptr_t)_p & 0x7)) {
      return __gasneti_atomic64_compare_and_swap(_p,_oldval,_newval,_flags);
    } else {
      return gasneti_genatomic64_compare_and_swap((gasneti_genatomic64_t *)_p,_oldval,_newval,_flags);
    }
  }
  GASNETI_ATOMIC_FENCED_SWAP_DEFN(atomic64,__gasneti_atomic64_swap,_gasneti_atomic64_swap,gasneti_atomic64_)
  GASNETI_INLINE(gasneti_atomic64_swap)
  uint64_t gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _op, const int _flags) {
    if_pt (!((uintptr_t)_p & 0x7)) {
      return __gasneti_atomic64_swap(_p,_op,_flags);
    } else {
      return gasneti_genatomic64_swap((gasneti_genatomic64_t *)_p,_op,_flags);
    }
  }
  GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN(atomic64,__gasneti_atomic64_fetchadd,_gasneti_atomic64_fetchadd,gasneti_atomic64_)
  GASNETI_INLINE(gasneti_atomic64_addfetch)
  uint64_t gasneti_atomic64_addfetch(gasneti_atomic64_t *_p, uint64_t _op, const int _flags) {
    if_pt (!((uintptr_t)_p & 0x7)) {
      return _op + __gasneti_atomic64_fetchadd(_p,_op,_flags);
    } else {
      return gasneti_genatomic64_addfetch((gasneti_genatomic64_t *)_p,_op,_flags);
    }
  }
  GASNETI_INLINE(gasneti_atomic64_decrement_and_test)
  int gasneti_atomic64_decrement_and_test(gasneti_atomic64_t *_p, const int _flags) {
    const int _mask = (GASNETI_ATOMIC_RMB_POST_IF_TRUE|GASNETI_ATOMIC_RMB_POST_IF_FALSE);
    if_pt (!((uintptr_t)_p & 0x7)) {
      const int _result = (1 == __gasneti_atomic64_fetchadd(_p,(uint64_t)-1,_flags));
      _gasneti_atomic64_fence_after_bool(_p,(_flags&_mask),_result);
      return _result;
    } else {
      const int _result = (0 == gasneti_genatomic64_addfetch((gasneti_genatomic64_t *)_p,(uint64_t)-1,_flags));
      _gasneti_atomic_fence_after_bool(_p,(_flags&_mask),_result);
      return _result;
    }
  }
  #define gasneti_atomic64_increment(p,f) ((void)gasneti_atomic64_addfetch((p),1,(f)))
  #define gasneti_atomic64_decrement(p,f) ((void)gasneti_atomic64_addfetch((p),(uint64_t)-1,(f)))
  #define gasneti_atomic64_add(p,op,f) ((uint64_t)gasneti_atomic64_addfetch((p),(uint64_t)(op),(f)))
  #define gasneti_atomic64_subtract(p,op,f) ((uint64_t)gasneti_atomic64_addfetch((p),(uint64_t)-(op),(f)))
#else
  /* Define 64-bit fixed-width atomics in terms of un-fenced native atomics */

  /* First define a fully-fenced addfetch if it appears to be needed */
  #if defined(_gasneti_atomic64_addfetch) || defined(_gasneti_atomic64_fetchadd)
    #ifndef _gasneti_atomic64_addfetch
      #define _gasneti_atomic64_addfetch(p,op) ((op) + _gasneti_atomic64_fetchadd((p),(op)))
    #endif
    GASNETI_ATOMIC_FENCED_ADDFETCH_DEFN(atomic64,                   \
                                        gasneti_atomic64_addfetch,  \
                                        _gasneti_atomic64_addfetch, \
                                        gasneti_atomic64_)
  #elif defined(gasneti_atomic64_fetchadd)
    GASNETI_INLINE(gasneti_atomic64_addfetch)
    uint64_t gasneti_atomic64_addfetch(gasneti_atomic64_t *_p, uint64_t _op, int _f) {
      return (uint64_t)(gasneti_atomic64_fetchadd(_p,_op,_f) + _op);
    }
  #endif

  #ifdef _gasneti_atomic64_set
    #define gasneti_atomic64_set(p,v,f) GASNETI_ATOMIC_FENCED_SET(atomic64,_gasneti_atomic64_set,gasneti_atomic64_,p,v,f)
  #endif
  #ifdef _gasneti_atomic64_read
    GASNETI_ATOMIC_FENCED_READ_DEFN(atomic64,gasneti_atomic64_read,_gasneti_atomic64_read,gasneti_atomic64_)
  #endif
  #ifdef _gasneti_atomic64_increment
    #define gasneti_atomic64_increment(p,f) GASNETI_ATOMIC_FENCED_INCDEC(atomic64,_gasneti_atomic64_increment,gasneti_atomic64_,p,f)
  #elif !defined(gasneti_atomic64_increment)
    #define gasneti_atomic64_increment(p,f) ((void)gasneti_atomic64_addfetch((p),1,(f)))
  #endif
  #ifdef _gasneti_atomic64_decrement
    #define gasneti_atomic64_decrement(p,f) GASNETI_ATOMIC_FENCED_INCDEC(atomic64,_gasneti_atomic64_decrement,gasneti_atomic64_,p,f)
  #elif !defined(gasneti_atomic64_decrement)
    #define gasneti_atomic64_decrement(p,f) ((void)gasneti_atomic64_addfetch((p),(uint64_t)-1,(f)))
  #endif
  #ifndef gasneti_atomic64_decrement_and_test
    #ifndef _gasneti_atomic64_decrement_and_test
      #define _gasneti_atomic64_decrement_and_test(p) (0==_gasneti_atomic64_addfetch((p),(uint64_t)-1))
    #endif
    GASNETI_ATOMIC_FENCED_DECTEST_DEFN(atomic64,gasneti_atomic64_decrement_and_test,_gasneti_atomic64_decrement_and_test,gasneti_atomic64_)
  #endif
  #ifdef _gasneti_atomic64_compare_and_swap
    GASNETI_ATOMIC_FENCED_CAS_DEFN(atomic64,gasneti_atomic64_compare_and_swap,_gasneti_atomic64_compare_and_swap,gasneti_atomic64_)
  #endif
  #ifdef _gasneti_atomic64_swap
    GASNETI_ATOMIC_FENCED_SWAP_DEFN(atomic64,gasneti_atomic64_swap,_gasneti_atomic64_swap,gasneti_atomic64_)
  #endif
  #ifdef _gasneti_atomic64_add
    GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(atomic64,gasneti_atomic64_add,_gasneti_atomic64_add,gasneti_atomic64_)
  #elif !defined(gasneti_atomic64_add)
    #define gasneti_atomic64_add(p,op,f) ((uint64_t)gasneti_atomic64_addfetch((p),(uint64_t)(op),(f)))
  #endif
  #ifdef _gasneti_atomic64_subtract
    GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(atomic64,gasneti_atomic64_subtract,_gasneti_atomic64_subtract,gasneti_atomic64_)
  #elif !defined(gasneti_atomic64_subtract)
    #define gasneti_atomic64_subtract(p,op,f) ((uint64_t)gasneti_atomic64_addfetch((p),(uint64_t)-(op),(f)))
  #endif
#endif
#ifdef _gasneti_atomic64_cons
  #define gasneti_atomic64_read                 _gasneti_atomic64_cons(read)
  #define gasneti_atomic64_set                  _gasneti_atomic64_cons(set)
  #define gasneti_atomic64_increment            _gasneti_atomic64_cons(increment)
  #define gasneti_atomic64_decrement            _gasneti_atomic64_cons(decrement)
  #define gasneti_atomic64_decrement_and_test   _gasneti_atomic64_cons(decrement_and_test)
  #define gasneti_atomic64_compare_and_swap     _gasneti_atomic64_cons(compare_and_swap)
  #define gasneti_atomic64_swap                 _gasneti_atomic64_cons(swap)
  #define gasneti_atomic64_add                  _gasneti_atomic64_cons(add)
  #define gasneti_atomic64_subtract             _gasneti_atomic64_cons(subtract)
#endif

/* ------------------------------------------------------------------------------------ */
/* Define the "normal" atomics, which might be a private type, or either the 32-bit or 64-
 * bit fixed-width types (which in turn may have been derived from the generics, above.)
 */

#if defined(GASNETI_HAVE_PRIVATE_ATOMIC_T)
  /* "Private" atomics, using per-platform defns and the fencing macros.
   * These definitions add fencing around non-fenced implementations, but defer to any
   * platform-specific fully-fenced definitions which may exisit.
   */
  #ifndef gasneti_atomic_set
  #define gasneti_atomic_set(p,v,f) GASNETI_ATOMIC_FENCED_SET(atomic,_gasneti_atomic_set,gasneti_atomic_,p,v,f)
  #endif
  #ifndef gasneti_atomic_increment
    #define gasneti_atomic_increment(p,f) GASNETI_ATOMIC_FENCED_INCDEC(atomic,_gasneti_atomic_increment,gasneti_atomic_,p,f)
  #endif
  #ifndef gasneti_atomic_decrement
    #define gasneti_atomic_decrement(p,f) GASNETI_ATOMIC_FENCED_INCDEC(atomic,_gasneti_atomic_decrement,gasneti_atomic_,p,f)
  #endif
  #ifndef gasneti_atomic_read
    GASNETI_ATOMIC_FENCED_READ_DEFN(atomic,gasneti_atomic_read,_gasneti_atomic_read,gasneti_atomic_)
  #endif
  #ifndef gasneti_atomic_decrement_and_test
    GASNETI_ATOMIC_FENCED_DECTEST_DEFN(atomic,gasneti_atomic_decrement_and_test,_gasneti_atomic_decrement_and_test,gasneti_atomic_)
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_CAS) && !defined(gasneti_atomic_compare_and_swap)
    GASNETI_ATOMIC_FENCED_CAS_DEFN(atomic,gasneti_atomic_compare_and_swap,_gasneti_atomic_compare_and_swap,gasneti_atomic_)
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_CAS) && !defined(gasneti_atomic_swap)
    GASNETI_ATOMIC_FENCED_SWAP_DEFN(atomic,gasneti_atomic_swap,_gasneti_atomic_swap,gasneti_atomic_)
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB) && !defined(gasneti_atomic_add)
    GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(atomic,gasneti_atomic_add,_gasneti_atomic_add,gasneti_atomic_)
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB) && !defined(gasneti_atomic_subtract)
    GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(atomic,gasneti_atomic_subtract,_gasneti_atomic_subtract,gasneti_atomic_)
  #endif
#elif !defined(GASNETI_FORCE_64BIT_ATOMICOPS) && /* Not forcing 64-bits */ \
      (!defined(GASNETI_USE_GENERIC_ATOMIC32) || defined(GASNETI_USE_GENERIC_ATOMIC64)) /* No worse than 64 bit */
  #define GASNETI_USE_32BIT_ATOMICS
#else
  #define GASNETI_USE_64BIT_ATOMICS
#endif

#if defined(GASNETI_USE_32BIT_ATOMICS)
  typedef uint32_t				gasneti_atomic_val_t;
  typedef int32_t				gasneti_atomic_sval_t;
  #define GASNETI_ATOMIC_MAX			((gasneti_atomic_val_t)0xFFFFFFFFU)
  #define GASNETI_ATOMIC_SIGNED_MIN		((gasneti_atomic_sval_t)0x80000000)
  #define GASNETI_ATOMIC_SIGNED_MAX		((gasneti_atomic_sval_t)0x7FFFFFFF)

  #if defined(GASNETI_USE_GENERIC_ATOMIC32)
    #ifndef GASNETI_USE_GENERIC_ATOMICOPS
      #define GASNETI_USE_GENERIC_ATOMICOPS 1
    #endif
  #endif
  #ifdef GASNETI_ATOMIC32_NOT_SIGNALSAFE
    #define GASNETI_ATOMICOPS_NOT_SIGNALSAFE 1
  #endif

  #define GASNETI_HAVE_ATOMIC_CAS               1
  #define GASNETI_HAVE_ATOMIC_ADD_SUB           1

  #if defined(GASNETI_USING_SLOW_ATOMIC32)
    #define _gasneti_atomic_cons(_id)    gasneti_slow_atomic32_##_id
  #else
    #define _gasneti_atomic_cons(_id)    gasneti_atomic32_##_id
  #endif
#elif defined(GASNETI_USE_64BIT_ATOMICS)
  typedef uint64_t			gasneti_atomic_val_t;
  typedef int64_t			gasneti_atomic_sval_t;
  #define GASNETI_ATOMIC_MAX		((gasneti_atomic_val_t)0xFFFFFFFFFFFFFFFFLLU)
  #define GASNETI_ATOMIC_SIGNED_MIN	((gasneti_atomic_sval_t)0x8000000000000000LL)
  #define GASNETI_ATOMIC_SIGNED_MAX	((gasneti_atomic_sval_t)0x7FFFFFFFFFFFFFFFLL)

  #if defined(GASNETI_USE_GENERIC_ATOMIC64)
    #ifndef GASNETI_USE_GENERIC_ATOMICOPS
      #define GASNETI_USE_GENERIC_ATOMICOPS 1
    #endif
  #endif
  #ifdef GASNETI_ATOMIC64_NOT_SIGNALSAFE
    #define GASNETI_ATOMICOPS_NOT_SIGNALSAFE 1
  #endif

  #define GASNETI_HAVE_ATOMIC_CAS               1
  #define GASNETI_HAVE_ATOMIC_ADD_SUB           1

  #if defined(GASNETI_USING_SLOW_ATOMIC64)
    #define _gasneti_atomic_cons(_id)    gasneti_slow_atomic64_##_id
  #else
    #define _gasneti_atomic_cons(_id)    gasneti_atomic64_##_id
  #endif
#elif defined(GASNETI_USING_SLOW_ATOMICOPS) // GASNETI_HAVE_PRIVATE_ATOMIC_T case
  extern gasneti_atomic_val_t gasneti_slow_atomic_read(gasneti_atomic_t *_p, const int _flags);
  extern void gasneti_slow_atomic_set(gasneti_atomic_t *_p, gasneti_atomic_val_t _v, const int _flags);
  extern void gasneti_slow_atomic_increment(gasneti_atomic_t *_p, const int _flags);
  extern void gasneti_slow_atomic_decrement(gasneti_atomic_t *_p, const int _flags);
  extern int gasneti_slow_atomic_decrement_and_test(gasneti_atomic_t *_p, const int _flags);
  #if defined(GASNETI_HAVE_ATOMIC_CAS)
    extern int gasneti_slow_atomic_compare_and_swap(gasneti_atomic_t *_p, gasneti_atomic_val_t _oldval, gasneti_atomic_val_t _newval, const int _flags);
    extern gasneti_atomic_val_t gasneti_slow_atomic_swap(gasneti_atomic_t *_p, gasneti_atomic_val_t _val, const int _flags);
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    extern gasneti_atomic_val_t gasneti_slow_atomic_add(gasneti_atomic_t *_p, gasneti_atomic_val_t _op, const int _flags);
    extern gasneti_atomic_val_t gasneti_slow_atomic_subtract(gasneti_atomic_t *_p, gasneti_atomic_val_t _op, const int _flags);
  #endif

  #define _gasneti_atomic_cons(_id)    gasneti_slow_atomic_##_id
#endif
#ifndef gasneti_atomic_signed
  #define gasneti_atomic_signed(val) ((gasneti_atomic_sval_t)(val))
#endif
#ifdef _gasneti_atomic_cons
  #define gasneti_atomic_t                      _gasneti_atomic_cons(t)
  #define gasneti_atomic_align			_gasneti_atomic_cons(align)
  #define gasneti_atomic_init			_gasneti_atomic_cons(init)
  #define gasneti_atomic_set                    _gasneti_atomic_cons(set)
  #define gasneti_atomic_read                   _gasneti_atomic_cons(read)
  #define gasneti_atomic_increment              _gasneti_atomic_cons(increment)
  #define gasneti_atomic_decrement              _gasneti_atomic_cons(decrement)
  #define gasneti_atomic_decrement_and_test     _gasneti_atomic_cons(decrement_and_test)
  #if defined(GASNETI_HAVE_ATOMIC_CAS)
    #define gasneti_atomic_compare_and_swap     _gasneti_atomic_cons(compare_and_swap)
    #define gasneti_atomic_swap                 _gasneti_atomic_cons(swap)
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    #define gasneti_atomic_add                  _gasneti_atomic_cons(add)
    #define gasneti_atomic_subtract             _gasneti_atomic_cons(subtract)
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNet "non-atomics" - these implement the same operations as the interfaces without
 *   "non" in the names, only in a non-threadsafe manner.
 *
 * On SEQ build the weak atomics will reduce to this implementation, but it is made
 *   available unconditionally to allow use by conduits under appropriate circumstances.
 *
 * Do not need any exclusion mechanism, but we still want to include any fences that
 *   the caller has requested, since any memory in the gasnet segment "protected" by a
 *   fenced atomic may be written by a network adapter.
 */
#define _GASNETI_NONATOMIC_DEFN(_type,_sz)            \
  typedef volatile uint##_sz##_t gasneti_##_type##_t; \
  typedef uint##_sz##_t gasneti_##_type##_val_t;      \
  typedef int##_sz##_t gasneti_##_type##_sval_t;

/* Fencing macros for nonatomics
 *	_gasneti_nonatomic_prologue_{set,read,rmw}(p, flags)
 *	_gasneti_nonatomic_fence_{before,after}_{set,read,rmw}(p, flags)
 *	_gasneti_nonatomic_fence_after_bool(p, flags, value)
 *
 * These are defined defined unconditionally because presently there are no
 * fencing side-effects for the non-atomic code, and thus no reason to override.
 */
#if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
  /* Always apply the fences */
  #define _gasneti_weak_fence_check(f)	0
#else
  /* Apply fences unless "GASNETI_ATOMIC_WEAK_FENCE" is present */
  #define _gasneti_weak_fence_check(f)	(f & GASNETI_ATOMIC_WEAK_FENCE)
#endif
#define _gasneti_nonatomic_fence_before(p,f) \
            if (!_gasneti_weak_fence_check(f)) { _gasneti_atomic_fence_before(f); }
#define _gasneti_nonatomic_fence_after(p,f) \
            if (!_gasneti_weak_fence_check(f)) { _gasneti_atomic_fence_after(f); }
#define _gasneti_nonatomic_fence_after_bool(p,f,v) \
            if (!_gasneti_weak_fence_check(f)) { _gasneti_atomic_fence_after(f)  \
                                                 _gasneti_atomic_fence_bool(f,v) }
#define _gasneti_nonatomic_prologue_set(p,f)  /*empty*/
#define _gasneti_nonatomic_fence_before_set  _gasneti_nonatomic_fence_before
#define _gasneti_nonatomic_fence_after_set   _gasneti_nonatomic_fence_after
#define _gasneti_nonatomic_prologue_read(p,f) /*empty*/
#define _gasneti_nonatomic_fence_before_read _gasneti_nonatomic_fence_before
#define _gasneti_nonatomic_fence_after_read  _gasneti_nonatomic_fence_after
#define _gasneti_nonatomic_prologue_rmw(p,f)  /*empty*/
#define _gasneti_nonatomic_fence_before_rmw  _gasneti_nonatomic_fence_before
#define _gasneti_nonatomic_fence_after_rmw   _gasneti_nonatomic_fence_after

/* Build gasneti_nonatomic_t to match width of "normal" atomic (unless custom) */
#if defined(GASNETI_USE_64BIT_ATOMICS)
  _GASNETI_NONATOMIC_DEFN(nonatomic,64)
  #define GASNETI_NONATOMIC_MAX            ((gasneti_nonatomic_val_t)0xFFFFFFFFFFFFFFFFLLU)
  #define GASNETI_NONATOMIC_SIGNED_MIN     ((gasneti_nonatomic_sval_t)0x8000000000000000LL)
  #define GASNETI_NONATOMIC_SIGNED_MAX     ((gasneti_nonatomic_sval_t)0x7FFFFFFFFFFFFFFFLL)
#else
  _GASNETI_NONATOMIC_DEFN(nonatomic,32)
  #define GASNETI_NONATOMIC_MAX            ((gasneti_nonatomic_val_t)0xFFFFFFFFU)
  #define GASNETI_NONATOMIC_SIGNED_MIN     ((gasneti_nonatomic_sval_t)0x80000000)
  #define GASNETI_NONATOMIC_SIGNED_MAX     ((gasneti_nonatomic_sval_t)0x7FFFFFFF)
#endif

#ifndef gasneti_nonatomic_align
  #define gasneti_nonatomic_align       gasneti_atomic_align
#endif
#ifndef gasneti_nonatomic32_align
  #define gasneti_nonatomic32_align     gasneti_atomic32_align
#endif
#ifndef gasneti_nonatomic64_align
  #define gasneti_nonatomic64_align     gasneti_atomic64_align
#endif

#define gasneti_nonatomic_init            _gasneti_scalar_atomic_init
#define gasneti_nonatomic_signed(v)       ((gasneti_nonatomic_sval_t)(val))
#define gasneti_nonatomic_set(p,v,f)      GASNETI_ATOMIC_FENCED_SET(nonatomic,_gasneti_scalar_atomic_set,gasneti_nonatomic_,p,v,f)
#define gasneti_nonatomic_increment(p,f)  GASNETI_ATOMIC_FENCED_INCDEC(nonatomic,_gasneti_scalar_atomic_increment,gasneti_nonatomic_,p,f)
#define gasneti_nonatomic_decrement(p,f)  GASNETI_ATOMIC_FENCED_INCDEC(nonatomic,_gasneti_scalar_atomic_decrement,gasneti_nonatomic_,p,f)
GASNETI_ATOMIC_FENCED_READ_DEFN(nonatomic,gasneti_nonatomic_read,_gasneti_scalar_atomic_read,gasneti_nonatomic_)
GASNETI_ATOMIC_FENCED_DECTEST_DEFN(nonatomic,gasneti_nonatomic_decrement_and_test,_gasneti_scalar_atomic_decrement_and_test,gasneti_nonatomic_)
GASNETI_ATOMIC_FENCED_CAS_DEFN(nonatomic,gasneti_nonatomic_compare_and_swap,_gasneti_scalar_atomic_compare_and_swap,gasneti_nonatomic_)
GASNETI_SCALAR_ATOMIC_SWAP_DEFN(_gasneti_scalar_atomic_swap, gasneti_nonatomic_)
GASNETI_ATOMIC_FENCED_SWAP_DEFN(nonatomic,gasneti_nonatomic_swap,_gasneti_scalar_atomic_swap,gasneti_nonatomic_)
GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(nonatomic,gasneti_nonatomic_add,_gasneti_scalar_atomic_add,gasneti_nonatomic_)
GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(nonatomic,gasneti_nonatomic_subtract,_gasneti_scalar_atomic_subtract,gasneti_nonatomic_)
#define GASNETI_HAVE_NONATOMIC_CAS 1
#define GASNETI_HAVE_NONATOMIC_ADD_SUB 1

/* Build gasneti_nonatomic32_t */
_GASNETI_NONATOMIC_DEFN(nonatomic32,32)
#define gasneti_nonatomic32_init            _gasneti_scalar_atomic_init
#define gasneti_nonatomic32_set(p,v,f)      GASNETI_ATOMIC_FENCED_SET(nonatomic,_gasneti_scalar_atomic_set,gasneti_nonatomic32_,p,v,f)
#define gasneti_nonatomic32_increment(p,f)  GASNETI_ATOMIC_FENCED_INCDEC(nonatomic,_gasneti_scalar_atomic_increment,gasneti_nonatomic32_,p,f)
#define gasneti_nonatomic32_decrement(p,f)  GASNETI_ATOMIC_FENCED_INCDEC(nonatomic,_gasneti_scalar_atomic_decrement,gasneti_nonatomic32_,p,f)
GASNETI_ATOMIC_FENCED_READ_DEFN(nonatomic,gasneti_nonatomic32_read,_gasneti_scalar_atomic_read,gasneti_nonatomic32_)
GASNETI_ATOMIC_FENCED_DECTEST_DEFN(nonatomic,gasneti_nonatomic32_decrement_and_test,_gasneti_scalar_atomic_decrement_and_test,gasneti_nonatomic32_)
GASNETI_ATOMIC_FENCED_CAS_DEFN(nonatomic,gasneti_nonatomic32_compare_and_swap,_gasneti_scalar_atomic_compare_and_swap,gasneti_nonatomic32_)
GASNETI_SCALAR_ATOMIC_SWAP_DEFN(_gasneti_scalar_atomic32_swap, gasneti_nonatomic32_)
GASNETI_ATOMIC_FENCED_SWAP_DEFN(nonatomic,gasneti_nonatomic32_swap,_gasneti_scalar_atomic32_swap,gasneti_nonatomic32_)
GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(nonatomic,gasneti_nonatomic32_add,_gasneti_scalar_atomic_add,gasneti_nonatomic32_)
GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(nonatomic,gasneti_nonatomic32_subtract,_gasneti_scalar_atomic_subtract,gasneti_nonatomic32_)

/* Build gasneti_nonatomic64_t */
_GASNETI_NONATOMIC_DEFN(nonatomic64,64)
#define gasneti_nonatomic64_init            _gasneti_scalar_atomic_init
#define gasneti_nonatomic64_set(p,v,f)      GASNETI_ATOMIC_FENCED_SET(nonatomic,_gasneti_scalar_atomic_set,gasneti_nonatomic64_,p,v,f)
#define gasneti_nonatomic64_increment(p,f)  GASNETI_ATOMIC_FENCED_INCDEC(nonatomic,_gasneti_scalar_atomic_increment,gasneti_nonatomic64_,p,f)
#define gasneti_nonatomic64_decrement(p,f)  GASNETI_ATOMIC_FENCED_INCDEC(nonatomic,_gasneti_scalar_atomic_decrement,gasneti_nonatomic64_,p,f)
GASNETI_ATOMIC_FENCED_READ_DEFN(nonatomic,gasneti_nonatomic64_read,_gasneti_scalar_atomic_read,gasneti_nonatomic64_)
GASNETI_ATOMIC_FENCED_DECTEST_DEFN(nonatomic,gasneti_nonatomic64_decrement_and_test,_gasneti_scalar_atomic_decrement_and_test,gasneti_nonatomic64_)
GASNETI_ATOMIC_FENCED_CAS_DEFN(nonatomic,gasneti_nonatomic64_compare_and_swap,_gasneti_scalar_atomic_compare_and_swap,gasneti_nonatomic64_)
GASNETI_SCALAR_ATOMIC_SWAP_DEFN(_gasneti_scalar_atomic64_swap, gasneti_nonatomic64_)
GASNETI_ATOMIC_FENCED_SWAP_DEFN(nonatomic,gasneti_nonatomic64_swap,_gasneti_scalar_atomic64_swap,gasneti_nonatomic64_)
GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(nonatomic,gasneti_nonatomic64_add,_gasneti_scalar_atomic_add,gasneti_nonatomic64_)
GASNETI_ATOMIC_FENCED_ADDSUB_DEFN(nonatomic,gasneti_nonatomic64_subtract,_gasneti_scalar_atomic_subtract,gasneti_nonatomic64_)

/* ------------------------------------------------------------------------------------ */
/* GASNet weak atomics - these operations are guaranteed to be atomic if and only if
    the sole updates are from the host processor(s), with no signals involved.
   if !GASNETI_THREADS, they compile away to the non-atomic implementation
    thereby saving the overhead of unnecessary atomic-memory CPU instructions.
   Otherwise, they expand to regular gasneti_atomic_t's
 */
#if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
  #define _GASNETI_WEAKATOMIC_ID(_id)     _CONCAT(GASNETI_ATOMIC,_id)
  #define _gasneti_weakatomic_id(_id)     _CONCAT(gasneti_atomic,_id)
  #ifdef GASNETI_HAVE_ATOMIC_CAS
    #define GASNETI_HAVE_WEAKATOMIC_CAS     1
  #endif
  #ifdef GASNETI_HAVE_ATOMIC_ADD_SUB
    #define GASNETI_HAVE_WEAKATOMIC_ADD_SUB 1
  #endif
#else
  #define _GASNETI_WEAKATOMIC_ID(_id)     _CONCAT(GASNETI_NONATOMIC,_id)
  #define _gasneti_weakatomic_id(_id)     _CONCAT(gasneti_nonatomic,_id)
  #define GASNETI_HAVE_WEAKATOMIC_CAS     1
  #define GASNETI_HAVE_WEAKATOMIC_ADD_SUB 1
#endif

typedef _gasneti_weakatomic_id(_t)             gasneti_weakatomic_t;
typedef _gasneti_weakatomic_id(_val_t)         gasneti_weakatomic_val_t;
typedef _gasneti_weakatomic_id(_sval_t)        gasneti_weakatomic_sval_t;
#define gasneti_weakatomic_init                _gasneti_weakatomic_id(_init)
#define gasneti_weakatomic_signed              _gasneti_weakatomic_id(_signed)
#define gasneti_weakatomic_set                 _gasneti_weakatomic_id(_set)
#define gasneti_weakatomic_read                _gasneti_weakatomic_id(_read)
#define gasneti_weakatomic_increment           _gasneti_weakatomic_id(_increment)
#define gasneti_weakatomic_decrement           _gasneti_weakatomic_id(_decrement)
#define gasneti_weakatomic_decrement_and_test  _gasneti_weakatomic_id(_decrement_and_test)
#ifdef GASNETI_HAVE_WEAKATOMIC_CAS
  #define gasneti_weakatomic_compare_and_swap  _gasneti_weakatomic_id(_compare_and_swap)
  #define gasneti_weakatomic_swap              _gasneti_weakatomic_id(_swap)
#endif
#ifdef GASNETI_HAVE_WEAKATOMIC_ADD_SUB
  #define gasneti_weakatomic_add               _gasneti_weakatomic_id(_add)
  #define gasneti_weakatomic_subtract          _gasneti_weakatomic_id(_subtract)
#endif

typedef _gasneti_weakatomic_id(32_t)           gasneti_weakatomic32_t;
#define gasneti_weakatomic32_init              _gasneti_weakatomic_id(32_init)
#define gasneti_weakatomic32_set               _gasneti_weakatomic_id(32_set)
#define gasneti_weakatomic32_read              _gasneti_weakatomic_id(32_read)
#define gasneti_weakatomic32_increment         _gasneti_weakatomic_id(32_increment)
#define gasneti_weakatomic32_decrement         _gasneti_weakatomic_id(32_decrement)
#define gasneti_weakatomic32_decrement_and_test _gasneti_weakatomic_id(32_decrement_and_test)
#define gasneti_weakatomic32_compare_and_swap  _gasneti_weakatomic_id(32_compare_and_swap)
#define gasneti_weakatomic32_swap              _gasneti_weakatomic_id(32_swap)
#define gasneti_weakatomic32_add               _gasneti_weakatomic_id(32_add)
#define gasneti_weakatomic32_subtract          _gasneti_weakatomic_id(32_subtract)

typedef _gasneti_weakatomic_id(64_t)           gasneti_weakatomic64_t;
#define gasneti_weakatomic64_init              _gasneti_weakatomic_id(64_init)
#define gasneti_weakatomic64_set               _gasneti_weakatomic_id(64_set)
#define gasneti_weakatomic64_read              _gasneti_weakatomic_id(64_read)
#define gasneti_weakatomic64_increment         _gasneti_weakatomic_id(64_increment)
#define gasneti_weakatomic64_decrement         _gasneti_weakatomic_id(64_decrement)
#define gasneti_weakatomic64_decrement_and_test _gasneti_weakatomic_id(64_decrement_and_test)
#define gasneti_weakatomic64_compare_and_swap  _gasneti_weakatomic_id(64_compare_and_swap)
#define gasneti_weakatomic64_swap              _gasneti_weakatomic_id(64_swap)
#define gasneti_weakatomic64_add               _gasneti_weakatomic_id(64_add)
#define gasneti_weakatomic64_subtract          _gasneti_weakatomic_id(64_subtract)

/* ------------------------------------------------------------------------------------ */
/* Configuration strings */

#if defined(GASNETI_FORCE_GENERIC_ATOMICOPS)
  #define GASNETI_ATOMIC_CONFIG   atomics_forced_mutex
#elif defined(GASNETI_FORCE_OS_ATOMICOPS)
  #define GASNETI_ATOMIC_CONFIG   atomics_forced_os
#elif defined(GASNETI_FORCE_COMPILER_ATOMICOPS)
  #define GASNETI_ATOMIC_CONFIG   atomics_forced_compiler
#elif (GASNETI_ATOMIC_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_GENERIC)
  #define GASNETI_ATOMIC_CONFIG   atomics_mutex
#elif (GASNETI_ATOMIC_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_COMPILER)
  #define GASNETI_ATOMIC_CONFIG   atomics_compiler
#elif (GASNETI_ATOMIC_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_OS)
  #define GASNETI_ATOMIC_CONFIG   atomics_os
#else
  #define GASNETI_ATOMIC_CONFIG   atomics_native
#endif

#if defined(GASNETI_FORCE_GENERIC_ATOMICOPS)
  #define GASNETI_ATOMIC32_CONFIG   atomic32_forced_mutex
#elif defined(GASNETI_FORCE_OS_ATOMICOPS)
  #define GASNETI_ATOMIC32_CONFIG   atomic32_forced_os
#elif defined(GASNETI_FORCE_COMPILER_ATOMICOPS)
  #define GASNETI_ATOMIC32_CONFIG   atomic32_forced_compiler
#elif (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_GENERIC)
  #define GASNETI_ATOMIC32_CONFIG   atomic32_mutex
#elif (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_COMPILER)
  #define GASNETI_ATOMIC32_CONFIG   atomic32_compiler
#elif (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_OS)
  #define GASNETI_ATOMIC32_CONFIG   atomic32_os
#else
  #define GASNETI_ATOMIC32_CONFIG   atomic32_native
#endif

#if defined(GASNETI_FORCE_GENERIC_ATOMICOPS)
  #define GASNETI_ATOMIC64_CONFIG   atomic64_forced_mutex
#elif defined(GASNETI_FORCE_OS_ATOMICOPS)
  #define GASNETI_ATOMIC64_CONFIG   atomic64_forced_os
#elif defined(GASNETI_FORCE_COMPILER_ATOMICOPS) && PLATFORM_ARCH_64
  #define GASNETI_ATOMIC64_CONFIG   atomic64_forced_compiler
#elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_GENERIC)
  #define GASNETI_ATOMIC64_CONFIG   atomic64_mutex
#elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_COMPILER)
  #define GASNETI_ATOMIC64_CONFIG   atomic64_compiler
#elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_HYBRID)
  #define GASNETI_ATOMIC64_CONFIG   atomic64_hybrid
#elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_OS)
  #define GASNETI_ATOMIC64_CONFIG   atomic64_os
#else
  #define GASNETI_ATOMIC64_CONFIG   atomic64_native
#endif

/* ------------------------------------------------------------------------------------ */
#endif
