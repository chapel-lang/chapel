/*   $Source: /var/local/cvs/gasnet/gasnet_atomic_bits.h,v $
 *     $Date: 2010/05/04 01:45:13 $
 * $Revision: 1.311.2.5 $
 * Description: GASNet header for platform-specific parts of atomic operations
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNET_H) && !defined(_IN_CONFIGURE)
  #error This file is not meant to be included directly- clients should include gasnet.h or gasnet_tools.h
#endif

#ifndef _GASNET_ATOMIC_BITS_H
#define _GASNET_ATOMIC_BITS_H

/* ------------------------------------------------------------------------------------ */
/* Identify special cases lacking native support */

#if defined(GASNETI_FORCE_GENERIC_ATOMICOPS) || /* for debugging */                \
    PLATFORM_ARCH_CRAYT3E    || /* T3E seems to have no atomic ops */              \
    PLATFORM_ARCH_NECSX      || /* NEC SX-6 atomics not available to user code? */ \
    PLATFORM_COMPILER_LCC    || /* not implemented - lacks inline asm */           \
    (PLATFORM_ARCH_ARM && !defined(GASNETI_HAVE_ARM_CMPXCHG)) ||                   \
    (PLATFORM_ARCH_MIPS && defined(_MIPS_ISA) && (_MIPS_ISA < 2)) ||               \
    PLATFORM_ARCH_MICROBLAZE   /* no atomic instructions */
  #define GASNETI_USE_GENERIC_ATOMICOPS
#elif defined(GASNETI_FORCE_OS_ATOMICOPS) || /* for debugging */ \
    PLATFORM_ARCH_MTA   ||                                       \
    PLATFORM_COMPILER_SGI
  #define GASNETI_USE_OS_ATOMICOPS
#endif

/* ------------------------------------------------------------------------------------ */
/* Work-arounds and special cases for various platforms */

#if PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_X86
  #ifdef GASNETI_UNI_BUILD
    #define GASNETI_X86_LOCK_PREFIX ""
  #else
    #define GASNETI_X86_LOCK_PREFIX "lock\n\t"
  #endif

    /* Partial x86 solution(s) to bug 1718 (-fPIC support when configure-time check was non-PIC).
     * AUTOMATIC WORK AROUND:
     * + gcc: on most platforms (including Linux, Darwin and Solaris) defines __PIC__ when building
     *      position independent code (e.g. -fPIC or -fpic; not passed -mdynamic-no-pic on Darwin).
     * + pathcc: same as gcc
     * MANUAL WORK AROUND:
     * + pgcc: no distinguishing macro when passed -fPIC, so no automatic work-around available
     * JUST WORKS:
     * + icc: mimics gcc, but is able to schedule %ebx so no work-around is needed
     * + Sun cc: use of specials doesn't encounter the problem
     * + tcc: N/A since no PIC support
     * + lcc: N/A since no native atomic support
     *
     * Bottom line is that we recommend YOUR_PIC_CFLAGS="-fPIC -DGASNETI_FORCE_PIC",
     * replacing "-fPIC" with your compiler-specific flag(s) as needed.
     */
  #if (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_PATHSCALE || \
       PLATFORM_COMPILER_PGI || PLATFORM_COMPILER_OPEN64 ) && \
	(defined(__PIC__) || defined(GASNETI_FORCE_PIC))
      /* Disable use of %ebx when building PIC, but only on affected compilers. */
      #define GASNETI_USE_X86_EBX 0
  #endif
  /* By default use the configure-probed result */
  #ifndef GASNETI_USE_X86_EBX
    #define GASNETI_USE_X86_EBX GASNETI_HAVE_X86_EBX
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Helpers for "special" call-based atomics on platforms w/ crippled inline asm support. */

#define GASNETI_SPECIAL_ASM_DECL(name) \
	GASNETI_EXTERNC void name(void)
#define GASNETI_SPECIAL_ASM_DEFN(name, body) \
	GASNETI_NEVER_INLINE(name, extern void name(void)) { body; }

/* ------------------------------------------------------------------------------------ */

#if defined(GASNETI_USE_GENERIC_ATOMICOPS)
  /* Use a very slow but portable implementation of atomic ops using mutexes */
  /* This case exists only to prevent the following cases from matching. */
#elif defined(GASNETI_USE_OS_ATOMICOPS)
  /* ------------------------------------------------------------------------------------
   * Use OS-provided atomics, which should be CPU-independent and
   * which should work regardless of the compiler's inline assembly support.
   * ------------------------------------------------------------------------------------ */
  #if PLATFORM_OS_AIX
      #define GASNETI_HAVE_ATOMIC32_T 1
      #include <sys/atomic_op.h>
      typedef struct { volatile unsigned int ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }

      /* Default impls of inc, dec, dec-and-test, add and sub */
      #define _gasneti_atomic32_fetchadd(p,op) fetch_and_add((atomic_p)&((p)->ctr), op)

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, int oldval, int newval) {
        return compare_and_swap( (atomic_p)p, &oldval, newval );
      } 

      /* ABI doesn't ensure 8-byte alignment */
      #define GASNETI_UNALIGNED_ATOMIC64 4

      /* No syncs in these calls, so use default fences */
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_OS_IRIX
      #define GASNETI_HAVE_ATOMIC32_T 1
      #include <mutex.h>
      #include <ulocks.h>
      typedef __uint32_t gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      (*(volatile __uint32_t *)(p))
      #define _gasneti_atomic32_set(p,v)     (*(volatile __uint32_t *)(p) = (v))
      #define _gasneti_atomic32_init(v)      (v)

      /* Default impls of inc, dec, dec-and-test, add and sub */
      #define _gasneti_atomic32_addfetch(p,op) (add_then_test32((p),(uint32_t)(op))) 

      #if PLATFORM_COMPILER_SGI
        GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
        int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, int oldval, int newval) {
            return __compare_and_swap( p, oldval, newval ); /* bug1534: compiler built-in */
        }
      #else /* flaky OS-provided CAS */
        usptr_t * volatile _gasneti_usmem_ptr;
        gasneti_atomic32_t _gasneti_usmem_ptr_init;
        GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
        int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, int oldval, int newval) {
          if_pf (!_gasneti_usmem_ptr) { /* need exactly one call to usinit on first invocation */
            if (test_and_set32(&_gasneti_usmem_ptr_init,1) == 0) {
              _gasneti_usmem_ptr = usinit("/dev/zero");
            } else while (!_gasneti_usmem_ptr);
          }
          return uscas32( p, oldval, newval, _gasneti_usmem_ptr ); /* from libc */
        }
      #endif

      #if PLATFORM_ARCH_64 || (defined(_MIPS_ISA) && (_MIPS_ISA >= 3) /* 64-bit capable CPU */)
        #if PLATFORM_COMPILER_SGI
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
          #define _gasneti_atomic64_read(p)      ((p)->ctr)
          #define _gasneti_atomic64_set(p,v)     do { (p)->ctr = (v); } while(0)
          #define _gasneti_atomic64_init(v)      { (v) }

          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *v, uint64_t oldval, uint64_t newval) {
            return __compare_and_swap(&v->ctr, oldval, newval); /* Polymorphic */
          }
        #else
	  /* TODO: No "other" compiler support yet */
        #endif
      #endif /* 64-bit available */
      
      /* Using default fences - the docs claim acquire or release "barriers" for the various 
         intrinsics, but those are only compiler fences and not architectural sync instructions */
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_OS_MTA
      /* use MTA intrinsics */
      #define GASNETI_HAVE_PRIVATE_ATOMIC_T 1	/* No CAS */
      typedef uint64_t                      gasneti_atomic_val_t;
      typedef int64_t                       gasneti_atomic_sval_t;
      #define GASNETI_ATOMIC_MAX            ((gasneti_atomic_val_t)0xFFFFFFFFFFFFFFFFLLU)
      #define GASNETI_ATOMIC_SIGNED_MIN     ((gasneti_atomic_sval_t)0x8000000000000000LL)
      #define GASNETI_ATOMIC_SIGNED_MAX     ((gasneti_atomic_sval_t)0x7FFFFFFFFFFFFFFFLL)
      #define gasneti_atomic_align          8
      typedef uint64_t gasneti_atomic_t;
      #define _gasneti_atomic_read(p)      ((uint64_t)*(volatile uint64_t*)(p))
      #define _gasneti_atomic_set(p,v)     ((*(volatile uint64_t*)(p)) = (v))
      #define _gasneti_atomic_init(v)      (v)

      /* Default impls of inc, dec, dec-and-test, add and sub */
      #define _gasneti_atomic_fetchadd int_fetch_add

      /* Using default fences, but this machine is Sequential Consistent anyway */
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_OS_SOLARIS /* BROKEN (and incomplete + out-of-date) */
      /* $%*(! Solaris has atomic functions in the kernel but refuses to expose them
         to the user... after all, what application would be interested in performance? */
      #include <sys/atomic.h>
      typedef struct { volatile uint32_t ctr; } gasneti_atomic_t;
      #define _gasneti_atomic_increment(p) (atomic_add_32((uint32_t *)&((p)->ctr),1))
      #define _gasneti_atomic_read(p)      ((p)->ctr)
      #define _gasneti_atomic_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic_init(v)      { (v) }
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_OS_CYGWIN
      /* These are *NOT* Cywgin calls, but Windows API calls that may actually
       * be intrinsics in the MS compilers on 64-bit systems. */
      #include <windows.h>

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_increment(p) InterlockedIncrement((LONG *)&((p)->ctr))
      #define _gasneti_atomic32_decrement(p) InterlockedDecrement((LONG *)&((p)->ctr))
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_decrement_and_test(p) \
                                          (InterlockedDecrement((LONG *)&((p)->ctr)) == 0)
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
	   (InterlockedCompareExchange((LONG *)&((p)->ctr),nval,oval) == (LONG)(oval))
      #define _gasneti_atomic32_fetchadd(p, op) InterlockedExchangeAdd((LONG *)&((p)->ctr), op)

      #if PLATFORM_ARCH_64 /* TODO: Identify ILP32 running on 64-bit CPU */
        #define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
        #define _gasneti_atomic64_increment(p) InterlockedIncrement64((LONGLONG *)&((p)->ctr))
        #define _gasneti_atomic64_decrement(p) InterlockedDecrement64((LONGLONG *)&((p)->ctr))
        #define _gasneti_atomic64_read(p)      ((p)->ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))
        #define _gasneti_atomic64_init(v)      { (v) }
        #define _gasneti_atomic64_decrement_and_test(p) \
                                          (InterlockedDecrement64((LONGLONG *)&((p)->ctr)) == 0)
        #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
	     (InterlockedCompareExchange64((LONGLONG *)&((p)->ctr),nval,oval) == (LONGLONG)(oval))
        #define _gasneti_atomic64_fetchadd(p, op) InterlockedExchangeAdd64((LONGLONG *)&((p)->ctr), op)
      #endif

      /* MSDN docs ensure memory fence in these calls, even on ia64 */
      #define GASNETI_ATOMIC_FENCE_RMW (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_OS_LINUX
      /* ------------------------------------------------------------------------------------
       * Linux provides an asm/atomic.h that is sometimes just useless
       * and other times supplies all but compare-and-swap (even when
       * it is implemented).  So, this code is probably only useful when
       * we encounter a new Linux platform.
       * ------------------------------------------------------------------------------------ */
      /* Disable using this code if this is a gasnet-smp build and the 
         linux/config.h settings disagree (due to system config problem or 
         cross-compiling on a uniprocessor frontend for smp nodes) */
      #include <linux/config.h>
      #if !(defined(CONFIG_SMP) || defined(GASNETI_UNI_BUILD))
        #error Building against a uniprocessor kernel.  Configure with --disable-smp-safe (for uniprocessor compute nodes), or build on an SMP host.
      #endif
      #if PLATFORM_ARCH_ALPHA
        /* work-around for a puzzling header bug in alpha Linux */
        #define extern static
      #endif
      #ifdef __cplusplus
        /* work around a really stupid C++ header bug observed in HP Linux */
        #define new new_
      #endif
      #include <asm/bitops.h>
      #include <asm/system.h>
      #include <asm/atomic.h>
      #if PLATFORM_ARCH_ALPHA
        #undef extern
      #endif
      #ifdef __cplusplus
        #undef new
      #endif

      #define GASNETI_HAVE_PRIVATE_ATOMIC_T 1
      #if 0 /* XXX: Broken since we don't know the range on a given platform. */
        typedef uint32_t                      gasneti_atomic_val_t;
        typedef int32_t                       gasneti_atomic_sval_t;
        #define GASNETI_ATOMIC_MAX            ((gasneti_atomic_val_t)0xFFFFFFFFU)
        #define GASNETI_ATOMIC_SIGNED_MIN     ((gasneti_atomic_sval_t)0x80000000)
        #define GASNETI_ATOMIC_SIGNED_MAX     ((gasneti_atomic_sval_t)0x7FFFFFFF)
      #endif
      typedef atomic_t gasneti_atomic_t;
      #define gasneti_atomic_align         4

      #define _gasneti_atomic_increment(p) atomic_inc(p)
      #define _gasneti_atomic_decrement(p) atomic_dec(p)
      #define _gasneti_atomic_read(p)      atomic_read(p)
      #define _gasneti_atomic_set(p,v)     atomic_set(p,v)
      #define _gasneti_atomic_init(v)      ATOMIC_INIT(v)
      #define _gasneti_atomic_decrement_and_test(p) \
                                          atomic_dec_and_test(p)
      #ifdef cmpxchg
        /* we must violate the Linux atomic_t abstraction below and pass
           cmpxchg a pointer to the struct field, otherwise cmpxchg will
           stupidly attempt to cast its result to a struct type and fail
         */
        #define _gasneti_atomic_compare_and_swap(p,oval,nval) \
             (cmpxchg(&((p)->counter),oval,nval) == (oval))
        #define GASNETI_HAVE_ATOMIC_CAS 1
      #endif

      /* Using default fences as we can't hope to know what to expect on new platforms */
  /* ------------------------------------------------------------------------------------ */
  #else
    #error GASNETI_USE_OS_ATOMICS defined on unsupported OS - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
  #endif
#else
  /* ------------------------------------------------------------------------------------
   * Not using GENERIC (mutex) or OS-provided atomics, so provide our own based on the
   * CPU and compiler support for inline assembly code
   * ------------------------------------------------------------------------------------ */
  #if PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 /* x86 and Athlon64/Opteron */
    #if PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_INTEL || \
        PLATFORM_COMPILER_PATHSCALE || GASNETI_PGI_ASM_THREADSAFE || \
        PLATFORM_COMPILER_TINY || PLATFORM_COMPILER_OPEN64
     #define GASNETI_HAVE_ATOMIC32_T 1
     typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
     #define _gasneti_atomic32_init(v)      { (v) }

     #define GASNETI_HAVE_ATOMIC64_T 1
     typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
     #define _gasneti_atomic64_init(v)      { (v) }

      #if PLATFORM_COMPILER_PATHSCALE || PLATFORM_COMPILER_OPEN64
        /* Pathscale optimizer is buggy and fails to clobber memory output location correctly
           unless we include an extraneous full memory clobber 
         */
        #define GASNETI_ATOMIC_MEM_CLOBBER ,"memory"
      #else
        #define GASNETI_ATOMIC_MEM_CLOBBER
      #endif

      #if GASNETI_PGI_ASM_BUG2149
        /* PGI can generate bad code in the presence of "register".
	 * See gasnet_asm.h and bug 2149 for info. */
	#define GASNETI_ASM_REGISTER_KEYWORD /* empty */
      #else
	#define GASNETI_ASM_REGISTER_KEYWORD register
      #endif

      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))

      GASNETI_INLINE(_gasneti_atomic32_increment)
      void _gasneti_atomic32_increment(gasneti_atomic32_t *v) {
        __asm__ __volatile__(
                GASNETI_X86_LOCK_PREFIX
		"incl %0"
                : "=m" (v->ctr)
                : "m" (v->ctr)
                : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
      }
      #define _gasneti_atomic32_increment _gasneti_atomic32_increment
      GASNETI_INLINE(_gasneti_atomic32_decrement)
      void _gasneti_atomic32_decrement(gasneti_atomic32_t *v) {
        __asm__ __volatile__(
                GASNETI_X86_LOCK_PREFIX
		"decl %0"
                : "=m" (v->ctr)
                : "m" (v->ctr) 
                : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
      }
      #define _gasneti_atomic32_decrement _gasneti_atomic32_decrement
      GASNETI_INLINE(_gasneti_atomic32_decrement_and_test)
      int _gasneti_atomic32_decrement_and_test(gasneti_atomic32_t *v) {
          GASNETI_ASM_REGISTER_KEYWORD unsigned char retval;
          __asm__ __volatile__(
	          GASNETI_X86_LOCK_PREFIX
		  "decl %0		\n\t"
		  "sete %1"
	          : "=m" (v->ctr), "=qm" (retval)
	          : "m" (v->ctr) 
                  : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	#if GASNETI_PGI_ASM_BUG1754
          return retval & 0xFF;
	#else
          return retval;
	#endif
      }
      #define _gasneti_atomic32_decrement_and_test _gasneti_atomic32_decrement_and_test

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *v, uint32_t oldval, uint32_t newval) {
        GASNETI_ASM_REGISTER_KEYWORD unsigned char retval;
        GASNETI_ASM_REGISTER_KEYWORD uint32_t readval;
        __asm__ __volatile__ (
		GASNETI_X86_LOCK_PREFIX
		"cmpxchgl %3, %1	\n\t"
	#if GASNETI_PGI_ASM_BUG2294 /* Sensitive to output constraint order */
		"sete %2"
		: "=a" (readval), "=m" (v->ctr), "=qm" (retval)
	#else /* The version that has always worked everywhere else */
		"sete %0"
		: "=qm" (retval), "=m" (v->ctr), "=a" (readval)
	#endif
		: "r" (newval), "m" (v->ctr), "a" (oldval)
		: "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	#if GASNETI_PGI_ASM_BUG1754
          return retval & 0xFF;
	#else
          return retval;
	#endif
      }

      GASNETI_INLINE(gasneti_atomic32_fetchadd)
      uint32_t gasneti_atomic32_fetchadd(gasneti_atomic32_t *v, int32_t op) {
	/* CAUTION: Both PathScale and Intel compilers have been seen to be
         * rather fragile with respect to this asm template (bug 1563).
         * Change this at your own risk!
         */
	uint32_t retval = op;
        __asm__ __volatile__(
                GASNETI_X86_LOCK_PREFIX
		"xaddl %0, %1"
                : "=&r" (retval), "=m" (v->ctr)
                : "0" (retval), "m" (v->ctr)
                : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	return retval;
      }
      #define _gasneti_atomic32_fetchadd gasneti_atomic32_fetchadd

      /* 64-bit differ between x86 and x86-64: */
      #if PLATFORM_ARCH_X86_64 /* Athlon64/Opteron */
        #define _gasneti_atomic64_read(p)      ((p)->ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))

        #if PLATFORM_COMPILER_PATHSCALE && PLATFORM_COMPILER_VERSION_LT(2,3,0)
	  /* A "dirty hack" for bug 1620 because pathcc < 2.3 botches the 64-bit asm */
          #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY\
	    GASNETI_ASM_SPECIAL(                        \
		         "movq     %rsi, %rax		\n\t" \
		         GASNETI_X86_LOCK_PREFIX	\
		         "cmpxchgq %rdx, (%rdi)		\n\t" \
		         "sete     %cl			\n\t" \
		         "movzbl   %cl, %eax" )

          #define GASNETI_ATOMIC_SPECIALS                                        \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
				     GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)
        #else
          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
            GASNETI_ASM_REGISTER_KEYWORD unsigned char retval;
            GASNETI_ASM_REGISTER_KEYWORD uint64_t readval = oldval;
            __asm__ __volatile__ (
		    GASNETI_X86_LOCK_PREFIX
		    "cmpxchgq %3, %1	\n\t"
		    "sete %0"
		    : "=q" (retval), "=m" (p->ctr), "=a" (readval)
		    : "r" (newval), "m" (p->ctr), "a" (oldval)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	  #if GASNETI_PGI_ASM_BUG1754
            return retval & 0xFF;
	  #else
            return retval;
	  #endif
          }
        #endif
      #elif PLATFORM_COMPILER_OPEN64
        /* No known working 64-bit atomics for this compiler on ILP32.  See bug 2725 */
        #undef GASNETI_HAVE_ATOMIC64_T
        #undef _gasneti_atomic64_init
        /* left-over typedef of gasneti_atomic64_t will get hidden by a #define */
      #elif GASNETI_USE_X86_EBX && \
            !PLATFORM_COMPILER_TINY && !PLATFORM_COMPILER_PGI && \
            !(PLATFORM_COMPILER_GNU && PLATFORM_COMPILER_VERSION_LT(3,0,0)) /* bug 1790 */
	/* "Normal" ILP32 case:
	 *
	 * To perform read and set atomically on x86 requires use of the locked
	 * 8-byte c-a-s instruction.  This is the only atomic 64-bit operation
	 * available on this architecture.  Note that we need the lock prefix
	 * even on a uniprocessor to ensure that we are signal safe.
	 * Since we don't have separate GASNETI_ATOMIC_FENCE_* settings for the
	 * 64-bit types, we define the fully-fenced (non _-prefixed) versions
	 * of read and set here, to avoid having them double fenced.
	 *
	 * Note that we have no way to tell the compiler exactly where to place a second
	 * uint64_t.  However, with the eax and edx already allocated, the only possibilities
	 * left in class 'q' are (ecx,ebx) and (ebx,ecx).  The use of an 'xchgl' instruction
	 * ensures we always end up with the second 64-bit value in (ebx,ecx).
	 * [ So far gcc has been observed to always use (ecx,ebx) and icc has been
	 *   observed to always use (ebx,ecx).  Nothing documents either behavior, so we
	 *   must use the 'xchgl' unconditionally. ]
	 *
	 * See the following #elif/#else clauses for slight variants.
	 */
        #define gasneti_atomic64_align 4 /* only need 4-byte alignment, not the default 8 */
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t retval = newval;
          __asm__ __volatile__ (
		    "xchgl	%2, %%ebx	\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "sete	%b2		\n\t"
		    "andl	$255, %k2"
		    : "=m" (p->ctr), "+&A" (oldval), "+&q" (retval)
		    : "m" (p->ctr)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
          return retval;
        }
        GASNETI_INLINE(gasneti_atomic64_set)
        void gasneti_atomic64_set(gasneti_atomic64_t *p, uint64_t v, int flags) {
	  uint64_t oldval = p->ctr;
          __asm__ __volatile__ (
		    "xchgl	%2,%%ebx	\n\t"
		    "0:				\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "jnz	0b		"
		    : "=m" (p->ctr), "+&A" (oldval), "+&q" (v)
		    : "m" (p->ctr)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	}
	#define gasneti_atomic64_set gasneti_atomic64_set
        GASNETI_INLINE(gasneti_atomic64_read)
        uint64_t gasneti_atomic64_read(gasneti_atomic64_t *p, int flags) {
	  uint64_t retval = p->ctr;
	  uint64_t tmp;
          __asm__ __volatile__ (
		    "0:				\n\t"
		    "movl	%%eax, %%ebx	\n\t"
		    "movl	%%edx, %%ecx	\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "jnz	0b		"
		    : "=m" (p->ctr), "+&A" (retval), "=&q" (tmp) /* tmp allocates ebx and ecx */
		    : "m" (p->ctr)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	  return retval;
	}
	#define gasneti_atomic64_read gasneti_atomic64_read
      #elif !GASNETI_USE_X86_EBX
	/* Much the same as the "normal" ILP32 case, but w/ save and restore of EBX.
	 * This is achieved by passing the "other" 64-bit value in ECX and a second
 	 * register of the compiler's choosing, which is then swapped w/ EBX.
	 *
	 * We also need to take care that the cmpxchg8b intruction won't get a
	 * GOT-relative address argument - since EBX doesn't hold the GOT pointer
	 * at the time it is executed.  This is done by loading the address into
	 * a register rather than giving it as an "m".
	 *
	 * Alas, if we try to add an "m" output for the target location, gcc thinks
	 * it needs to allocate another register for it.  Having none left, it gives
	 * up at this point.  So, we need to list "memory" in the clobbers instead.
	 */
        #define gasneti_atomic64_align 4 /* only need 4-byte alignment, not the default 8 */
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newlo = (uint32_t)newval;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newhi = (uint32_t)(newval >> 32);
          __asm__ __volatile__ (
		    "xchgl	%2, %%ebx	\n\t"
		    "lock;			"
		    "cmpxchg8b	(%3)		\n\t"
		    "sete	%b1		\n\t"
		    "andl	$255, %k1	\n\t"
		    "movl	%2, %%ebx	"
		    : "+&A" (oldval), "+&c" (newhi), "+&r" (newlo)
		    : "r" (&p->ctr)
		    : "cc", "memory");
          return newhi;
        }
        GASNETI_INLINE(gasneti_atomic64_set)
        void gasneti_atomic64_set(gasneti_atomic64_t *p, uint64_t v, int flags) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t oldval = p->ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newlo = (uint32_t)v;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newhi = (uint32_t)(v >> 32);
          __asm__ __volatile__ (
		    "xchgl	%1, %%ebx	\n\t"
		    "0:				\n\t"
		    "lock;			"
		    "cmpxchg8b	(%3)		\n\t"
		    "jnz	0b		\n\t"
		    "movl	%1, %%ebx	"
		    : "+&A" (oldval), "+&r" (newlo)
		    : "c" (newhi), "r" (&p->ctr)
		    : "cc", "memory");
	}
	#define gasneti_atomic64_set gasneti_atomic64_set
        GASNETI_INLINE(gasneti_atomic64_read)
        uint64_t gasneti_atomic64_read(gasneti_atomic64_t *p, int flags) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t retval = p->ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t save_ebx, ecx;
          __asm__ __volatile__ (
		    "movl	%%ebx, %2	\n\t"
		    "0:				\n\t"
		    "movl	%%eax, %%ebx	\n\t"
		    "movl	%%edx, %%ecx	\n\t"
		    "lock;			"
		    "cmpxchg8b	(%3)		\n\t"
		    "jnz	0b		\n\t"
		    "movl	%2, %%ebx	"
		    : "+&A" (retval), "=&c" (ecx), "=&r" (save_ebx)
		    : "r" (&p->ctr)
		    : "cc");
	  return retval;
	}
	#define gasneti_atomic64_read gasneti_atomic64_read
      #else /* Tiny CC, PGI and (GCC < 3.0.0) */
	/* Everything here works like the "normal" ILP32 case, except that we break everything
	 * down in to nice bite-sized (4-bytes actually) chunks and explictly assign
	 * them to registers A through D.
	 * This is needed for TCC and PGI that lack (or have buggy) support for the "A" constraint.
	 * This is used for older GCC that complain about too many reloads for the "normal" case.
	 * Also to, appease older GCC, we use "+m" where "=m" is sufficient/correct (see bug 1790).
	 */
        #define gasneti_atomic64_align 4 /* only need 4-byte alignment, not the default 8 */
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t oldlo = GASNETI_LOWORD(oldval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t oldhi = GASNETI_HIWORD(oldval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newlo = GASNETI_LOWORD(newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newhi = GASNETI_HIWORD(newval);
          __asm__ __volatile__ (
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "sete	%b1		\n\t"
		    "andl	$255, %1"
                    : "+m" (p->ctr), "+&a" (oldlo), "+&d" (oldhi)
                    : "b" (newlo), "c" (newhi)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
          return oldlo;
        }
        GASNETI_INLINE(gasneti_atomic64_set)
        void gasneti_atomic64_set(gasneti_atomic64_t *p, uint64_t v, int flags) {
	  uint64_t oldval = p->ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t oldlo = GASNETI_LOWORD(oldval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t oldhi = GASNETI_HIWORD(oldval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newlo = GASNETI_LOWORD(v);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t newhi = GASNETI_HIWORD(v);
          __asm__ __volatile__ (
		    "0:				\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "jnz	0b		"
		    : "+m" (p->ctr), "+&a" (oldlo),  "+&d" (oldhi)
		    : "b" (newlo), "c" (newhi)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	}
	#define gasneti_atomic64_set gasneti_atomic64_set
        GASNETI_INLINE(gasneti_atomic64_read)
        uint64_t gasneti_atomic64_read(gasneti_atomic64_t *p, int flags) {
	  uint64_t retval = p->ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t retlo = GASNETI_LOWORD(retval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t rethi = GASNETI_HIWORD(retval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t tmplo, tmphi;
          __asm__ __volatile__ (
		    "0:				\n\t"
		    "movl	%%eax, %%ebx	\n\t"
		    "movl	%%edx, %%ecx	\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "jnz	0b		"
		    : "+m" (p->ctr), "+&a" (retlo),  "+&d" (rethi), "=&b" (tmplo), "=&c" (tmphi)
		    : /* no inputs */
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	  return GASNETI_MAKEWORD(rethi, retlo);
	}
	#define gasneti_atomic64_read gasneti_atomic64_read
      #endif

      /* x86 and x86_64 include full memory fence in locked RMW insns */
      #define GASNETI_ATOMIC_FENCE_RMW (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)

      /* Optionally build a 128-bit atomic type using 64-bit types for all args */
      #if GASNETI_HAVE_X86_CMPXCHG16B
	#define GASNETI_HAVE_ATOMIC128_T 16 /* Encodes aligment */
	typedef struct { volatile uint64_t lo, hi; } gasneti_atomic128_t;

	GASNETI_INLINE(gasneti_atomic128_compare_and_swap)
	int gasneti_atomic128_compare_and_swap(gasneti_atomic128_t *p, uint64_t oldhi, uint64_t oldlo, uint64_t newhi, uint64_t newlo, int flags) {
	  GASNETI_ASM_REGISTER_KEYWORD unsigned char retval;
          gasneti_assert(!(((uintptr_t)p) & 0xF)); /* cmpxchg16b requires 16-byte alignment */
	  __asm__ __volatile__ (
		"lock; "
		"cmpxchg16b  %1   \n\t"
		"sete        %0   "
		: "=q" (retval), "=m" (*p), "+&a" (oldlo), "+&d" (oldhi)
		: "b" (newlo), "c" (newhi)
		: "cc", "memory");
	  #if GASNETI_PGI_ASM_BUG1754
	    return retval & 0xFF;
	  #else
	    return retval;
	  #endif
	}
	#define gasneti_atomic128_compare_and_swap gasneti_atomic128_compare_and_swap

	GASNETI_INLINE(gasneti_atomic128_set)
	void gasneti_atomic128_set(gasneti_atomic128_t *p, uint64_t newhi, uint64_t newlo, int flags) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t oldlo = p->lo;
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t oldhi = p->hi;
          gasneti_assert(!(((uintptr_t)p) & 0xF)); /* cmpxchg16b requires 16-byte alignment */
	  __asm__ __volatile__ (
		"0:               \n\t"
		"lock; "
		"cmpxchg16b  %0   \n\t"
		"jnz         0b   "
		: "=m" (*p), "+&a" (oldlo), "+&d" (oldhi)
		: "b" (newlo), "c" (newhi)
		: "cc", "memory");
	}
	#define gasneti_atomic128_set gasneti_atomic128_set

	GASNETI_INLINE(gasneti_atomic128_read)
	void gasneti_atomic128_read(uint64_t *outhi, uint64_t *outlo, gasneti_atomic128_t *p, int flags) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t retlo = p->lo;
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t rethi = p->hi;
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t tmphi, tmplo;
          gasneti_assert(!(((uintptr_t)p) & 0xF)); /* cmpxchg16b requires 16-byte alignment */
	  __asm__ __volatile__ (
		"0:                 \n\t"
		"movq        %1, %3 \n\t"
		"movq        %2, %4 \n\t"
		"lock; "
		"cmpxchg16b  %0     \n\t"
		"jnz         0b          "
		: "+m" (*p), "+&a" (retlo),  "+&d" (rethi), "=&b" (tmplo), "=&c" (tmphi)
		: /* no inputs */
		: "cc", "memory");
	  *outlo = retlo;
	  *outhi = rethi;
	}
	#define gasneti_atomic128_read gasneti_atomic128_read
      #endif /* GASNETI_HAVE_X86_CMPXCHG16B */
    #elif PLATFORM_COMPILER_SUN || PLATFORM_COMPILER_PGI_C
      /* First, some macros to hide the x86 vs. x86-64 ABI differences */
      #if PLATFORM_ARCH_X86_64
        #define _gasneti_atomic_addr		"(%rdi)"
        #define _gasneti_atomic_load_arg0	""	/* arg0 in rdi */
        #define _gasneti_atomic_load_arg1	"movq %rsi, %rax	\n\t"
	#define _gasneti_atomic_load_arg2	""	/* arg2 in rdx */
      #else
        #define _gasneti_atomic_addr		"(%ecx)"
        #define _gasneti_atomic_load_arg0	"movl 8(%ebp), %ecx	\n\t"
        #define _gasneti_atomic_load_arg1	"movl 12(%ebp), %eax	\n\t"
	#define _gasneti_atomic_load_arg2	"movl 16(%ebp), %edx	\n\t"
        #define gasneti_atomic64_align 4 /* only need 4-byte alignment, not the default 8 */
      #endif

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))

      #define GASNETI_ATOMIC32_INCREMENT_BODY				\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "incl " _gasneti_atomic_addr )

      #define GASNETI_ATOMIC32_DECREMENT_BODY				\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "decl " _gasneti_atomic_addr )

      #define GASNETI_ATOMIC32_DECREMENT_AND_TEST_BODY			\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "decl " _gasneti_atomic_addr			"\n\t" \
		       "sete %cl					\n\t" \
		       "movzbl %cl, %eax" )

      #define GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY			\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       _gasneti_atomic_load_arg1			\
		       _gasneti_atomic_load_arg2			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchgl %edx, " _gasneti_atomic_addr		"\n\t" \
		       "sete  %cl					\n\t" \
		       "movzbl  %cl, %eax" )

    #if 1
      /* Fetch-add version is faster for calls that ignore the result and
       * for subtraction of constants.  In both cases because the "extra"
       * work is done in C code that the optimizer can discard.
       */
      #define GASNETI_ATOMIC32_FETCHADD_BODY				\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       _gasneti_atomic_load_arg1			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "xadd %eax, " _gasneti_atomic_addr	)
    #else
      #define GASNETI_ATOMIC32_ADD_BODY					\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       _gasneti_atomic_load_arg1			\
		       "movl %eax, %edx					\n\t" \
		       GASNETI_X86_LOCK_PREFIX				\
		       "xadd %eax, " _gasneti_atomic_addr		"\n\t" \
		       "addl %edx, %eax"	)

      #define GASNETI_ATOMIC32_SUBTRACT_BODY				\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       _gasneti_atomic_load_arg1			\
		       "movl %eax, %edx					\n\t" \
		       "negl %eax					\n\t" \
		       GASNETI_X86_LOCK_PREFIX				\
		       "xadd %eax, " _gasneti_atomic_addr		"\n\t" \
		       "subl %edx, %eax"	)

      #define GASNETI_HAVE_ATOMIC_ADD_SUB 1
    #endif

      #define GASNETI_ATOMIC32_SPECIALS                                        \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_increment,          \
				 GASNETI_ATOMIC32_INCREMENT_BODY)              \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_decrement,          \
				 GASNETI_ATOMIC32_DECREMENT_BODY)              \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_decrement_and_test, \
				 GASNETI_ATOMIC32_DECREMENT_AND_TEST_BODY)     \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_compare_and_swap,   \
				 GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY)       \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_fetchadd,           \
				 GASNETI_ATOMIC32_FETCHADD_BODY)

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_init(v)      { (v) }

      /* 64-bit differ between x86 and x86-64: */
      #if PLATFORM_ARCH_X86_64 /* Athlon64/Opteron */
        #define _gasneti_atomic64_read(p)      ((p)->ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))

        #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY			\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       _gasneti_atomic_load_arg1			\
		       _gasneti_atomic_load_arg2			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchgq %rdx, " _gasneti_atomic_addr		"\n\t" \
		       "sete  %cl					\n\t" \
		       "movzbl  %cl, %eax" )

        #define GASNETI_ATOMIC64_SPECIALS                                        \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap,   \
				   GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)
      #else /* x86 */
        #define GASNETI_ATOMIC64_READ_BODY     				\
	  GASNETI_ASM_SPECIAL(                                          \
		       "pushl     %edi					\n\t" \
		       "movl      8(%ebp), %edi				\n\t" \
		       "pushl     %ebx					\n" \
		       "movl      0(%edi), %eax				\n\t" \
		       "movl      4(%edi), %edx				\n\t" \
		       "1:						\n\t" \
		       "movl      %eax, %ebx				\n\t" \
		       "movl      %edx, %ecx				\n\t" \
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchg8b (%edi)				\n\t" \
		       "jnz       1b					\n\t" \
		       "popl      %ebx					\n\t" \
		       "popl      %edi" )

        #define GASNETI_ATOMIC64_SET_BODY     				\
	  GASNETI_ASM_SPECIAL(                                          \
		       "pushl     %edi					\n\t" \
		       "movl      8(%ebp), %edi				\n\t" \
		       "pushl     %ebx					\n" \
		       "movl      0(%edi), %eax				\n\t" \
		       "movl      4(%edi), %edx				\n\t" \
		       "movl      12(%ebp), %ebx			\n\t" \
		       "movl      16(%ebp), %ecx			\n\t" \
		       "1:						\n\t" \
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchg8b (%edi)				\n\t" \
		       "jnz       1b					\n\t" \
		       "popl      %ebx					\n\t" \
		       "popl      %edi" )

        #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY			\
	  GASNETI_ASM_SPECIAL(                                          \
		       "pushl     %edi					\n\t" \
		       "movl      8(%ebp), %edi				\n\t" \
		       "pushl     %ebx					\n" \
		       "movl      12(%ebp), %eax			\n\t" \
		       "movl      16(%ebp), %edx			\n\t" \
		       "movl      20(%ebp), %ebx			\n\t" \
		       "movl      24(%ebp), %ecx			\n\t" \
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchg8b (%edi)				\n\t" \
		       "popl      %ebx					\n\t" \
		       "sete      %cl					\n\t" \
		       "popl      %edi					\n\t" \
		       "movzbl    %cl, %eax" )

        #define GASNETI_ATOMIC64_SPECIALS                                      \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_read,             \
				   GASNETI_ATOMIC64_READ_BODY)                 \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_set,              \
				   GASNETI_ATOMIC64_SET_BODY)                  \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
				   GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)
      #endif 

      #define GASNETI_ATOMIC_SPECIALS   GASNETI_ATOMIC32_SPECIALS \
                                        GASNETI_ATOMIC64_SPECIALS

      /* x86 and x86_64 include full memory fence in locked RMW insns */
      #define GASNETI_ATOMIC_FENCE_RMW (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)
    #elif PLATFORM_COMPILER_PGI_CXX
      /* pgCC w/o threadsafe GNU-style asm():
       * Here we must use the slow atomics since we don't know if the library has been
       * built w/ native or "special" atomics support.
       * See bug 1752 for discussion of how this might be avoided.
       */
      /* XXX: Only works because both possible library versions use these representations. */
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_init(v)      { (v) }

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_init(v)      { (v) }

      #define GASNETI_HAVE_ATOMIC_CAS 1	/* Explicit */
      #define GASNETI_HAVE_ATOMIC_ADD_SUB 1	/* Derived */
      #define GASNETI_USING_SLOW_ATOMICS 1
    #elif (PLATFORM_ARCH_X86_64 && PLATFORM_COMPILER_CRAY)
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_fetchadd(p,op) \
              __sync_fetch_and_add(&((p)->ctr), (op))
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
              (__sync_val_compare_and_swap(&((p)->ctr), (oval), (nval)) == (oval))

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_read(p)      ((p)->ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic64_init(v)      { (v) }
      #define _gasneti_atomic64_fetchadd(p,op)\
              __sync_fetch_and_add(&((p)->ctr), (op))
      #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
              (__sync_val_compare_and_swap(&((p)->ctr), (oval), (nval)) == (oval))

      /* x86 and x86_64 include full memory fence in locked RMW insns */
      #define GASNETI_ATOMIC_FENCE_RMW (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)
    #else
      #error unrecognized x86 compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_IA64 /* Itanium */
    #if PLATFORM_COMPILER_INTEL
      /* Intel compiler's inline assembly broken on Itanium (bug 384) - use intrinsics instead */
      #include <ia64intrin.h>

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_increment(p) __fetchadd4_acq((unsigned int *)&((p)->ctr),1)
      #define _gasneti_atomic32_decrement(p) __fetchadd4_acq((unsigned int *)&((p)->ctr),-1)
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_decrement_and_test(p) \
                    (__fetchadd4_acq((unsigned int *)&((p)->ctr),-1) == 1)
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
                    (_InterlockedCompareExchange_acq((volatile unsigned int *)&((p)->ctr),nval,oval) == (oval))

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_increment(p) __fetchadd8_acq((unsigned __int64 *)&((p)->ctr),1)
      #define _gasneti_atomic64_decrement(p) __fetchadd8_acq((unsigned __int64 *)&((p)->ctr),-1)
      #define _gasneti_atomic64_read(p)      ((p)->ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic64_init(v)      { (v) }
      #define _gasneti_atomic64_decrement_and_test(p) \
                    (__fetchadd8_acq((unsigned __int64 *)&((p)->ctr),-1) == 1)
      #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
                    (_InterlockedCompareExchange64_acq((volatile unsigned __int64 *)&((p)->ctr),nval,oval) == (oval))

      /* See fence treatment after #endif */
    #elif PLATFORM_COMPILER_GNU
      GASNETI_INLINE(gasneti_atomic32_cmpxchg)
      uint32_t gasneti_atomic32_cmpxchg(uint32_t volatile *ptr, uint32_t oldval, uint32_t newval) {
        uint64_t tmp = oldval;
        __asm__ __volatile__ ("mov ar.ccv=%0;;" :: "rO"(tmp));
        __asm__ __volatile__ 
        #if PLATFORM_ARCH_64
            ("cmpxchg4.acq %0=[%1],%2,ar.ccv" : "=r"(tmp) : "r"(ptr), "r"(newval) );
        #else
            ("addp4 %1=0,%1;;\n\t"
             "cmpxchg4.acq %0=[%1],%2,ar.ccv" : "=r"(tmp), "+r"(ptr) :  "r"(newval) );
        #endif
        return (uint32_t) tmp;
      }
      GASNETI_INLINE(gasneti_atomic32_fetchandinc)
      uint32_t gasneti_atomic32_fetchandinc(uint32_t volatile *ptr) {
        uint64_t result;
        __asm__ __volatile__ 
        #if PLATFORM_ARCH_64
            ("fetchadd4.acq %0=[%1],%2" : "=r"(result) : "r"(ptr), "i" (1) );
        #else
            ("addp4 %1=0,%1;;\n\t"
             "fetchadd4.acq %0=[%1],%2" : "=r"(result), "+r"(ptr) :  "i" (1) );
        #endif
        return (uint32_t) result;
      }
      GASNETI_INLINE(gasneti_atomic32_fetchanddec)
      uint32_t gasneti_atomic32_fetchanddec(uint32_t volatile *ptr) {
        uint64_t result;
        __asm__ __volatile__ 
        #if PLATFORM_ARCH_64
            ("fetchadd4.acq %0=[%1],%2" : "=r"(result) : "r"(ptr), "i" (-1) );
        #else
            ("addp4 %1=0,%1;;\n\t"
             "fetchadd4.acq %0=[%1],%2" : "=r"(result), "+r"(ptr) :  "i" (-1) );
        #endif
        return (uint32_t) result;
      }

      GASNETI_INLINE(gasneti_atomic64_cmpxchg)
      uint64_t gasneti_atomic64_cmpxchg(uint64_t volatile *ptr, uint64_t oldval, uint64_t newval) {
        uint64_t tmp = oldval;
        __asm__ __volatile__ ("mov ar.ccv=%0;;" :: "rO"(tmp));
        __asm__ __volatile__ 
        #if PLATFORM_ARCH_64
            ("cmpxchg8.acq %0=[%1],%2,ar.ccv" : "=r"(tmp) : "r"(ptr), "r"(newval) );
        #else
            ("addp4 %1=0,%1;;\n\t"
             "cmpxchg8.acq %0=[%1],%2,ar.ccv" : "=r"(tmp), "+r"(ptr) :  "r"(newval) );
        #endif
        return (uint64_t) tmp;
      }
      GASNETI_INLINE(gasneti_atomic64_fetchandinc)
      uint64_t gasneti_atomic64_fetchandinc(uint64_t volatile *ptr) {
        uint64_t result;
        __asm__ __volatile__ 
        #if PLATFORM_ARCH_64
            ("fetchadd8.acq %0=[%1],%2" : "=r"(result) : "r"(ptr), "i" (1) );
        #else
            ("addp4 %1=0,%1;;\n\t"
             "fetchadd8.acq %0=[%1],%2" : "=r"(result), "+r"(ptr) :  "i" (1) );
        #endif
        return result;
      }
      GASNETI_INLINE(gasneti_atomic64_fetchanddec)
      uint64_t gasneti_atomic64_fetchanddec(uint64_t volatile *ptr) {
        uint64_t result;
        __asm__ __volatile__ 
        #if PLATFORM_ARCH_64
            ("fetchadd8.acq %0=[%1],%2" : "=r"(result) : "r"(ptr), "i" (-1) );
        #else
            ("addp4 %1=0,%1;;\n\t"
             "fetchadd8.acq %0=[%1],%2" : "=r"(result), "+r"(ptr) :  "i" (-1) );
        #endif
        return result;
      }

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_increment(p) (gasneti_atomic32_fetchandinc(&((p)->ctr)))
      #define _gasneti_atomic32_decrement(p) (gasneti_atomic32_fetchanddec(&((p)->ctr)))
      #define _gasneti_atomic32_decrement_and_test(p) (gasneti_atomic32_fetchanddec(&((p)->ctr)) == 1)
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
        (gasneti_atomic32_cmpxchg(&((p)->ctr),oval,nval) == (oval))

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_read(p)      ((p)->ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic64_init(v)      { (v) }
      #define _gasneti_atomic64_increment(p) (gasneti_atomic64_fetchandinc(&((p)->ctr)))
      #define _gasneti_atomic64_decrement(p) (gasneti_atomic64_fetchanddec(&((p)->ctr)))
      #define _gasneti_atomic64_decrement_and_test(p) (gasneti_atomic64_fetchanddec(&((p)->ctr)) == 1)
      #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
        (gasneti_atomic64_cmpxchg(&((p)->ctr),oval,nval) == (oval))

      /* The default c-a-s based add and subtract are already the best we can do. */

      /* See fence treatment after #endif */
    #elif PLATFORM_COMPILER_HP /* HP C/C++ Itanium intrinsics */
      #include <machine/sys/inline.h>

      /* legal values for imm are -16, -8, -4, -1, 1, 4, 8, and 16 returns *old* value */
      #define gasneti_atomic32_addandfetch(ptr, imm) \
         _Asm_fetchadd(_FASZ_W, _SEM_ACQ, ptr, imm,  \
                       _LDHINT_NONE, (_Asm_fence)(_UP_MEM_FENCE | _DOWN_MEM_FENCE))
      GASNETI_INLINE(gasneti_atomic32_cmpxchg)
      uint32_t gasneti_atomic32_cmpxchg(uint32_t volatile *ptr, uint32_t oldval, uint32_t newval) {
        register uint64_t result;
        _Asm_mov_to_ar(_AREG_CCV, (int64_t)oldval);
        result = _Asm_cmpxchg(_SZ_W, _SEM_ACQ, ptr, newval, 
                           _LDHINT_NONE, (_Asm_fence)(_UP_MEM_FENCE | _DOWN_MEM_FENCE));
        return (uint32_t) result;
      }

      /* legal values for imm are -16, -8, -4, -1, 1, 4, 8, and 16 returns *old* value */
      #define gasneti_atomic64_addandfetch(ptr, imm) \
         _Asm_fetchadd(_FASZ_D, _SEM_ACQ, ptr, imm,  \
                       _LDHINT_NONE, (_Asm_fence)(_UP_MEM_FENCE | _DOWN_MEM_FENCE))
      GASNETI_INLINE(gasneti_atomic64_cmpxchg)
      uint64_t gasneti_atomic64_cmpxchg(uint64_t volatile *ptr, uint64_t oldval, uint64_t newval) {
        register uint64_t result;
        _Asm_mov_to_ar(_AREG_CCV, oldval);
        result = _Asm_cmpxchg(_SZ_D, _SEM_ACQ, ptr, newval, 
                              _LDHINT_NONE, (_Asm_fence)(_UP_MEM_FENCE | _DOWN_MEM_FENCE));
        return result;
      }

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_increment(p) (gasneti_atomic32_addandfetch(&((p)->ctr),1))
      #define _gasneti_atomic32_decrement(p) (gasneti_atomic32_addandfetch(&((p)->ctr),-1))
      #define _gasneti_atomic32_decrement_and_test(p) (gasneti_atomic32_addandfetch(&((p)->ctr),-1) == 1)
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
        (gasneti_atomic32_cmpxchg(&((p)->ctr),oval,nval) == (oval))

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_read(p)      ((p)->ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic64_init(v)      { (v) }
      #define _gasneti_atomic64_increment(p) (gasneti_atomic64_addandfetch(&((p)->ctr),1))
      #define _gasneti_atomic64_decrement(p) (gasneti_atomic64_addandfetch(&((p)->ctr),-1))
      #define _gasneti_atomic64_decrement_and_test(p) (gasneti_atomic64_addandfetch(&((p)->ctr),-1) == 1)
      #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
        (gasneti_atomic64_cmpxchg(&((p)->ctr),oval,nval) == (oval))

      /* The default c-a-s based add and subtract are already the best we can do. */

      /* See fence treatment after #endif */
    #else
      #error unrecognized Itanium compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
    #endif

    /* Since all 3 compilers are generating r-m-w with .acq variants, we can customize
     * the atomic fencing implementation by noting that "mf;; foo.acq" is a full memory
     * barrier both before and after. */
    #define _gasneti_atomic_fence_before_rmw(p, flags) \
	if (flags & (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)) gasneti_local_mb();
    #define _gasneti_atomic_fence_after_rmw(p, flags) \
	/* Nothing */
    #define _gasneti_atomic_fence_after_bool(p, flags, val) \
	if (!(flags & (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST))) \
	  { _gasneti_atomic_rmb_bool(flags, val) } 
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_ALPHA /* DEC Alpha */
    #if PLATFORM_COMPILER_GNU
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }

      GASNETI_INLINE(_gasneti_atomic32_fetchadd)
      uint32_t _gasneti_atomic32_fetchadd(gasneti_atomic32_t *v, int32_t op) {
        register int32_t temp;
        register uint32_t result;
        __asm__ __volatile__ (
		"1:	ldl_l	%1, %2		\n"	/* result = *addr */
		"	addl	%1, %3, %0	\n"	/* temp = result + op */
		"	stl_c	%0, %2		\n"	/* *addr = temp; temp = store_OK */
		"	beq	%0, 1b"			/* Retry on ll/ss failure */
		: "=&r" (temp), "=&r" (result), "=m" (*v) /* outputs */
		: "IOr" (op)       /* inputs */
		: "cc"); /* kills */
        return result;
      }
      #define _gasneti_atomic32_fetchadd _gasneti_atomic32_fetchadd

      GASNETI_INLINE(gasneti_atomic32_addx)
      void gasneti_atomic32_addx(gasneti_atomic32_t *v, int32_t op) {
        register int32_t temp;
        __asm__ __volatile__ (
		"1:	ldl_l	%0, %1		\n"	/* temp = *addr */
		"	addl	%0, %2, %0	\n"	/* temp += op */
		"	stl_c	%0, %1		\n"	/* *addr = temp; temp = store_OK */
		"	beq	%0, 1b"			/* Retry on ll/ss failure */
		: "=&r" (temp), "=m" (*v) /* outputs */
		: "IOr" (op)              /* inputs */
		: "cc");        /* kills */
      }
      #define _gasneti_atomic32_increment(p) (gasneti_atomic32_addx(p,1))
      #define _gasneti_atomic32_decrement(p) (gasneti_atomic32_addx(p,-1))

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, uint32_t oldval, uint32_t newval) {
        const int64_t s_oldval = (int64_t)(int32_t)oldval; /* Force sign extension */
        unsigned long ret;
        __asm__ __volatile__ (
		"1:	ldl_l	%0,%1\n"	/* Load-linked of current value (w/ sign extension) */
		"	cmpeq	%0,%2,%0\n"	/* compare to oldval */
		"	beq	%0,2f\n"	/* done/fail on mismatch (success/fail in ret) */
		"	mov	%3,%0\n"	/* copy newval to ret */
		"	stl_c	%0,%1\n"	/* Store-conditional of newval (success/fail in ret) */
		"	beq	%0,1b\n"	/* Retry on stl_c failure */
		"2:	"
       		: "=&r"(ret), "=m"(*p)
		: "r" (s_oldval), "r"(newval)
		: "cc");
        return ret;
      }

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
      #define _gasneti_atomic64_read(p)      ((p)->ctr)
      #define _gasneti_atomic64_set(p,v)     do { (p)->ctr = (v); } while(0)
      #define _gasneti_atomic64_init(v)      { (v) }
      GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
      int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
        uint64_t ret;
        __asm__ __volatile__ (
		"1:	ldq_l	%0,%1\n"	/* Load-linked of current value */
		"	cmpeq	%0,%2,%0\n"	/* compare to oldval */
		"	beq	%0,2f\n"	/* done/fail on mismatch (success/fail in ret) */
		"	mov	%3,%0\n"	/* copy newval to ret */
		"	stq_c	%0,%1\n"	/* Store-conditional of newval (success/fail in ret) */
		"	beq	%0,1b\n"	/* Retry on stl_c failure */
		"2:	"
       		: "=&r"(ret), "=m"(*p)
		: "r"(oldval), "r"(newval)
		: "cc");
        return (int)ret;
      }

      /* No fences in our asm, so using default fences */
    #elif PLATFORM_COMPILER_COMPAQ && PLATFORM_OS_TRU64
       /* Compaq C / OSF atomics are compiler built-ins */
       #include <sys/machine/builtins.h>
	
       #define GASNETI_HAVE_ATOMIC32_T 1
       typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
       #define _gasneti_atomic32_increment(p) (__ATOMIC_INCREMENT_LONG(&((p)->ctr)))
       #define _gasneti_atomic32_decrement(p) (__ATOMIC_DECREMENT_LONG(&((p)->ctr)))
       #define _gasneti_atomic32_read(p)      ((p)->ctr)
       #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
       #define _gasneti_atomic32_init(v)      { (v) }
       #define _gasneti_atomic32_decrement_and_test(p) \
                                          (__ATOMIC_DECREMENT_LONG(&((p)->ctr)) == 1)

       /* The __CMP_STORE_LONG built-in is insufficient alone because it returns
	  a failure indication if the LL/SC is interrupted by another write to the
          same cache line (it does not retry).
       */
       GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
       int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, uint32_t oldval, uint32_t newval) {
         const int64_t s_oldval = (int64_t)(int32_t)oldval; /* Force sign extension */
         return asm("1:	ldl_l	%v0,(%a0);"	/* Load-linked of current value to %v0 */
		    "	cmpeq	%v0,%a1,%v0;"	/* compare %v0 to oldval w/ result to %v0 */
		    "	beq	%v0,2f;"	/* done/fail on mismatch (success/fail in %v0) */
		    "	mov	%a2,%v0;"	/* copy newval to %v0 */
		    "	stl_c	%v0,(%a0);"	/* Store-conditional of newval (success/fail in %v0) */
		    "	beq	%v0,1b;"	/* Retry on stl_c failure */
		    "2:	", p, s_oldval, newval);  /* Returns value from %v0 */
       }

       #define _gasneti_atomic32_fetchadd(p, op) __ATOMIC_ADD_LONG(&((p)->ctr), op)

       #define GASNETI_HAVE_ATOMIC64_T 1
       typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
       #define _gasneti_atomic64_read(p)      ((p)->ctr)
       #define _gasneti_atomic64_set(p,v)     do { (p)->ctr = (v); } while(0)
       #define _gasneti_atomic64_init(v)      { (v) }
       GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
       int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
         return asm("1:	ldq_l	%v0,(%a0);"	/* Load-linked of current value to %v0 */
		   "	cmpeq	%v0,%a1,%v0;"	/* compare %v0 to oldval w/ result to %v0 */
		   "	beq	%v0,2f;"	/* done/fail on mismatch (success/fail in %v0) */
		   "	mov	%a2,%v0;"	/* copy newval to %v0 */
		   "	stq_c	%v0,(%a0);"	/* Store-conditional of newval (success/fail in %v0) */
		   "	beq	%v0,1b;"	/* Retry on std_c failure */
		   "2:	", p, oldval, newval);  /* Returns value from %v0 */
       }

       /* Both the instrisics and our asm lack built-in fences.  So, using default fences */
    #else
      #error unrecognized Alpha compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_SPARC
    #if defined(__sparcv9) || defined(__sparcv9cpu) || defined(GASNETI_ARCH_ULTRASPARC) /* SPARC v9 ISA */
      #if PLATFORM_COMPILER_GNU
        #define GASNETI_HAVE_ATOMIC32_T 1
        typedef struct { volatile int32_t ctr; } gasneti_atomic32_t;
        #define _gasneti_atomic32_read(p)      ((p)->ctr)
        #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
        #define _gasneti_atomic32_init(v)      { (v) }

        /* Default impls of inc, dec, dec-and-test, add and sub */
        GASNETI_INLINE(_gasneti_atomic32_fetchadd)
        uint32_t _gasneti_atomic32_fetchadd(gasneti_atomic32_t *v, int32_t op) {
          /* SPARC v9 architecture manual, p.333 
           * This function requires the cas instruction in Sparc V9, and therefore gcc -mcpu=ultrasparc
	   * The manual says (sec A.9) no memory fences in CAS (in conflict w/ JMM web page).
           */
          register int32_t volatile * addr = (int32_t volatile *)(&v->ctr);
          register uint32_t oldval;
          register uint32_t newval;
          __asm__ __volatile__ ( 
            "ld       [%4],%0    \n\t" /* oldval = *addr; */
            "0:			 \t" 
            "add      %0,%3,%1   \n\t" /* newval = oldval + op; */
            "cas      [%4],%0,%1 \n\t" /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
            "cmp      %0, %1     \n\t" /* check if newval == oldval (swap succeeded) */
            "bne,a,pn %%icc, 0b  \n\t" /* otherwise, retry (,pn == predict not taken; ,a == annul) */
            "  mov    %1, %0     "     /* oldval = newval; (branch delay slot, annulled if not taken) */
            : "=&r"(oldval), "=&r"(newval), "=m"(v->ctr)
            : "rI"(op), "r"(addr), "m"(v->ctr) );
          return oldval;
        }
        #define _gasneti_atomic32_fetchadd _gasneti_atomic32_fetchadd

        GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
        int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *v, uint32_t oldval, uint32_t newval) {
          register volatile uint32_t * addr = (volatile uint32_t *)&(v->ctr);
          __asm__ __volatile__ ( 
              "cas      [%3],%2,%0"  /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
              : "+r"(newval), "=m"(v->ctr)
              : "r"(oldval), "r"(addr), "m"(v->ctr) );
          return (int)(newval == oldval);
        }

        #if PLATFORM_ARCH_64
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
          #define _gasneti_atomic64_init(v)      { (v) }
          #define _gasneti_atomic64_read(p)      ((p)->ctr)
          #define _gasneti_atomic64_set(p,v)     do { (p)->ctr = (v); } while(0)
          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *v, uint64_t oldval, uint64_t newval) {
            register volatile uint64_t * addr = (volatile uint64_t *)&(v->ctr);
            __asm__ __volatile__ ( 
		"casx	[%3],%2,%0"  /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
              : "+r"(newval), "=m"(v->ctr)
              : "r"(oldval), "r"(addr), "m"(v->ctr) );
            return (int)(newval == oldval);
          }
        #elif GASNETI_HAVE_SPARC32_64BIT_ASM /* compiler supports "U" and "h" constraints */
          /* ILP32 on a 64-bit CPU. */
          /* Note that the ldd/std instructions *are* atomic, even though they use 2 registers.
           * We wouldn't need asm here if we could be sure the compiler always used ldd/std.
           */
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
          #define _gasneti_atomic64_init(v)      { (v) }
          GASNETI_INLINE(_gasneti_atomic64_set)
          void _gasneti_atomic64_set(gasneti_atomic64_t *p, uint64_t v) {
            __asm__ __volatile__ ( "std	%1, %0" : "=m"(p->ctr) : "U"(v) );
	  }
          GASNETI_INLINE(_gasneti_atomic64_read)
          uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *p) {
	    register uint64_t retval;
            __asm__ __volatile__ ( "ldd	%1, %0" : "=U"(retval) : "m"(p->ctr) );
	    return retval;
	  }
          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *v, uint64_t oldval, uint64_t newval) {
            register volatile uint64_t * addr = (volatile uint64_t *)&(v->ctr);
	    register int retval, tmp;
            __asm__ __volatile__ ( 
		"sllx	%H5,32,%0	\n\t"	/* retval = HI(new) << 32 */
		"sllx	%H6,32,%1	\n\t"	/* tmp = HI(old) << 32 */
		"or	%0,%L5,%0	\n\t"	/* retval |= LO(new) */
		"or	%1,%L6,%1	\n\t"	/* tmp |= LO(old) */
		"casx	[%3],%1,%0	\n\t"	/* atomic CAS, with read value -> retval */
		"xor	%1,%0,%1	\n\t"	/* tmp = 0 IFF retval == tmp */
		"clr	%0		\n\t"   /* retval = 0 */
		"movrz	%1,1,%0"		/* retval = 1 IFF tmp == 0 */
		: "=&h"(retval), "=&h"(tmp), "=m"(v->ctr)		/* 'h' = 64bit 'o' or 'g' reg */
		: "r"(addr), "m"(v->ctr), "r"(newval), "r"(oldval) );
            return retval;
          }
        #else
          /* use generics, since fixed-register asm never did work right */
	#endif

	/* Using default fences, as our asm includes none */
      #elif PLATFORM_COMPILER_SUN
        #if 0 /* Sun compiler gets an assertion failure upon seeing movrz */
          #define GASNETI_SPARC_MOVRZ_g1_1_i0		"movrz %g1, 1, %i0"
        #else
          #define GASNETI_SPARC_MOVRZ_g1_1_i0		".long 0xb1786401"
        #endif

        #define GASNETI_HAVE_ATOMIC32_T 1
	typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
	#define _gasneti_atomic32_init(v)      { (v) }
        #define _gasneti_atomic32_read(p)      ((p)->ctr)
        #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))

        /* Default impls of inc, dec, dec-and-test, add and sub */

        #define GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY					\
	    GASNETI_ASM_SPECIAL(                        \
		/* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */	\
		     "cas	[%i0], %i1, %i2		\n\t"				\
		/* retval = (oldval == newval) ? 1 : 0				*/	\
		     "xor	%i2, %i1, %g1		\n\t" /* g1 = 0 IFF old==new */ \
		     "cmp	%g0, %g1		\n\t" /* Set/clear carry bit */	\
		     "subx	%g0, -1, %i0 " )	      /* Subtract w/ carry */

        #define GASNETI_ATOMIC32_FETCHADD_BODY /* see gcc asm, above, for more detail */	\
	    GASNETI_ASM_SPECIAL(                        \
		/* oldval = *addr;						*/	\
		     "ld	[%i0], %g1		\n"				\
		/* while (!cas(addr, oldval, oldval + op)) { oldval = *addr; }	*/	\
		     "0:				\n\t"				\
		     "add	%g1, %i1, %i5		\n\t"				\
		     "cas	[%i0], %g1, %i5		\n\t"				\
		     "cmp	%g1, %i5		\n\t"				\
		     "bne,a,pn	%icc, 0b		\n\t"				\
		     "  mov	%i5, %g1		\n\t" /* annulled delay slot */	\
		/* Retval = oldval						*/	\
		     "mov	%i5, %i0" )

        #define GASNETI_ATOMIC32_SPECIALS                                      \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_compare_and_swap, \
				   GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY)     \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_fetchadd,         \
				   GASNETI_ATOMIC32_FETCHADD_BODY)

        #define GASNETI_HAVE_ATOMIC64_T 1
	typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
	#define _gasneti_atomic64_init(v)      { (v) }

        #if PLATFORM_ARCH_64
          #define _gasneti_atomic64_read(p)      ((p)->ctr)
          #define _gasneti_atomic64_set(p,v)     ((p)->ctr = (v))
          #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY				\
	    GASNETI_ASM_SPECIAL(							\
		/* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */	\
		     "casx	[%i0], %i1, %i2		\n\t"				\
		/* retval = (oldval == newval) ? 1 : 0				*/	\
		     "xor	%i2, %i1, %g1		\n\t" /* g1 = 0 IFF old==new */ \
		     "clr	%i0			\n\t" /* retval = 0 */		\
		     GASNETI_SPARC_MOVRZ_g1_1_i0 )	      /* retval = 1 IFF g1 == 0 */
          #define GASNETI_ATOMIC64_SPECIALS                                      \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
				     GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)
        #else
          /* ILP32 on a 64-bit CPU. */
          #define GASNETI_ATOMIC64_SET_BODY /* see gcc asm, above, for explanation */	\
	    GASNETI_ASM_SPECIAL(							\
		     "mov	%i1, %o2		\n\t"				\
		     "mov	%i2, %o3		\n\t"				\
		     "std	%o2, [%i0]" )
          #define GASNETI_ATOMIC64_READ_BODY /* see gcc asm, above, for explanation */	\
	    GASNETI_ASM_SPECIAL(							\
		     "ldd	[%i0], %i0 " );
          #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY /* see gcc asm, above, for explanation */ \
	    GASNETI_ASM_SPECIAL(							\
		     "sllx	%i3, 32, %o1		\n\t"				\
		     "sllx	%i1, 32, %g1		\n\t"				\
		     "or	%o1, %i4, %o1		\n\t"				\
		     "or	%g1, %i2, %g1		\n\t"				\
		     "casx	[%i0], %g1, %o1		\n\t"				\
		     "xor	%g1, %o1, %g1		\n\t"				\
		     "clr	%i0			\n\t" /* retval = 0 */		\
		     GASNETI_SPARC_MOVRZ_g1_1_i0 )	      /* retval = 1 IFF g1 == 0 */
          #define GASNETI_ATOMIC64_SPECIALS                                      \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_set,              \
				     GASNETI_ATOMIC64_SET_BODY)                  \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_read,             \
				     GASNETI_ATOMIC64_READ_BODY)                 \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
				     GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)
        #endif

        #define GASNETI_ATOMIC_SPECIALS   GASNETI_ATOMIC32_SPECIALS \
                                          GASNETI_ATOMIC64_SPECIALS

	/* Using default fences, as our asm includes none */
      #else
        #error unrecognized Sparc v9 compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
      #endif
    #else /* SPARC pre-v9 lacks RMW instructions - 
             all we get is atomic swap, but that's actually just barely enough  */
      #define GASNETI_ATOMICOPS_NOT_SIGNALSAFE 1 /* not signal-safe because of "checkout" semantics */
      #define GASNETI_HAVE_PRIVATE_ATOMIC_T 1
      #define gasneti_atomic_align 4
      #if PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_SUN
        #if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
          /* Only 31 bits: */
          #define GASNETI_ATOMIC_MAX		((uint32_t)0x7FFFFFFFU)
          #define GASNETI_ATOMIC_SIGNED_MIN	((int32_t)0xC0000000)
          #define GASNETI_ATOMIC_SIGNED_MAX	((int32_t)0x3FFFFFFF)
          #define gasneti_atomic_signed(val)	(((int32_t)((val)<<1))>>1)
        #else
          /* Full 32 bits: */
          #define GASNETI_ATOMIC_MAX		((uint32_t)0xFFFFFFFFU)
          #define GASNETI_ATOMIC_SIGNED_MIN	((int32_t)0x80000000)
          #define GASNETI_ATOMIC_SIGNED_MAX	((int32_t)0x7FFFFFFF)
        #endif
        typedef uint32_t                        gasneti_atomic_val_t;
        typedef int32_t                         gasneti_atomic_sval_t;

        #define GASNETI_ATOMIC_PRESENT    ((uint32_t)0x80000000)
        #define GASNETI_ATOMIC_INIT_MAGIC ((uint64_t)0x8BDEF66BAD1E3F3AULL)

        typedef struct { volatile uint64_t initflag; volatile uint32_t ctr; } gasneti_atomic_t;
        #define _gasneti_atomic_init(v)      { GASNETI_ATOMIC_INIT_MAGIC, (GASNETI_ATOMIC_PRESENT|(v)) }

        /* would like to use gasneti_waituntil here, but it requires libgasnet for waitmode */
        #define gasneti_atomic_spinuntil(cond) do {       \
                while (!(cond)) gasneti_compiler_fence(); \
                gasneti_local_rmb();                      \
                } while (0)

        #if PLATFORM_COMPILER_GNU
          GASNETI_INLINE(gasneti_loadandclear_32)
          uint32_t gasneti_loadandclear_32(uint32_t volatile *v) {
            register uint32_t volatile * addr = (uint32_t volatile *)v;
            register uint32_t val = 0;
            __asm__ __volatile__ ( 
              "swap %1, %0 \n"   
              : "+r" (val), "=m" (*addr) );
            return val;
          }
          GASNETI_INLINE(gasneti_checkout_32)
          uint32_t gasneti_checkout_32(gasneti_atomic_t *p) {
	    uint32_t retval;
            gasneti_assert(p->initflag == GASNETI_ATOMIC_INIT_MAGIC);
            gasneti_local_wmb();
            gasneti_atomic_spinuntil(p->ctr && (retval = gasneti_loadandclear_32(&(p->ctr))));
	    return retval;
	  }
	#else
          /* The following is straight-forward asm for
	   *  "while(!(p->ctr && (retval = gasneti_loadandclear_32(&(p->ctr))))) {}" 
	   * If one adds the WAITHOOK() to the gcc version, then this should change too.
	   */
	  #define GASNETI_ATOMIC_SPECIALS                                          \
		GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic_checkout,         \
		GASNETI_ASM("	add	%i0,8,%g1	\n\t"	/* g1 = &p->ctr */ \
			    "1:	ld	[%g1],%i0	\n\t"	/* i0 = *g1     */ \
			    "	cmp	%i0,0		\n\t"	/* if (!i0)     */ \
			    "	be	1b		\n\t"	/*     goto 1   */ \
			    "	 mov	0,%i0		\n\t"	/* i0 = 0       */ \
			    "	swap	[%g1],%i0	\n\t"	/* swap(*g1,i0) */ \
			    "	cmp	%i0,0		\n\t"	/* if (!i0)     */ \
			    "	be	1b		\n\t"	/*     goto 1   */ \
			    "	 nop			")	/* returns i0   */ \
		)
          GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_checkout);
          GASNETI_INLINE(gasneti_checkout_32)
          uint32_t gasneti_checkout_32(gasneti_atomic_t *p) {
	    uint32_t retval;
            gasneti_assert(p->initflag == GASNETI_ATOMIC_INIT_MAGIC);
            gasneti_local_wmb();
	    retval = (*(uint32_t (*)(gasneti_atomic_t *p))(&_gasneti_special_atomic_checkout))(p);
	    return retval;
	  }
	#endif

        GASNETI_INLINE(gasneti_atomic_fetchandadd_32)
        uint32_t gasneti_atomic_fetchandadd_32(gasneti_atomic_t *p, int32_t op) {
          const uint32_t tmp = gasneti_checkout_32(p);
          gasneti_assert(tmp & GASNETI_ATOMIC_PRESENT);
          p->ctr = (GASNETI_ATOMIC_PRESENT | (tmp + op));
          return (tmp & ~GASNETI_ATOMIC_PRESENT);
        }
        #if 0
          /* this version fails if set is used in a race with fetchandadd */
          GASNETI_INLINE(_gasneti_atomic_set)
          void _gasneti_atomic_set(gasneti_atomic_t *p, uint32_t val) {
            gasneti_local_wmb();
            p->ctr = (GASNETI_ATOMIC_PRESENT | val);
          }
        #else
          GASNETI_INLINE(_gasneti_atomic_set)
          void _gasneti_atomic_set(gasneti_atomic_t *p, uint32_t val) {
            uint32_t tmp;
            if_pf (p->initflag != GASNETI_ATOMIC_INIT_MAGIC) {
              gasneti_local_wmb();
              p->ctr = (GASNETI_ATOMIC_PRESENT | val);
              gasneti_local_wmb();
              p->initflag = GASNETI_ATOMIC_INIT_MAGIC;
            } else {
	      (void)gasneti_checkout_32(p);
              p->ctr = (GASNETI_ATOMIC_PRESENT | val);
            }
          }
        #endif
        GASNETI_INLINE(_gasneti_atomic_read)
        uint32_t _gasneti_atomic_read(gasneti_atomic_t *p) {
          uint32_t tmp;
          gasneti_assert(p->initflag == GASNETI_ATOMIC_INIT_MAGIC);
          gasneti_atomic_spinuntil((tmp = p->ctr));
          gasneti_assert(tmp & GASNETI_ATOMIC_PRESENT);
          return (tmp & ~GASNETI_ATOMIC_PRESENT);
        }

        /* Default impls of inc, dec, dec-and-test, add and sub */
        #define _gasneti_atomic_fetchadd gasneti_atomic_fetchandadd_32

        GASNETI_INLINE(_gasneti_atomic_compare_and_swap)
        int _gasneti_atomic_compare_and_swap(gasneti_atomic_t *p, uint32_t oldval, uint32_t newval) {
          uint32_t tmp = gasneti_checkout_32(p);
          const int retval = (tmp == (GASNETI_ATOMIC_PRESENT | oldval));
          gasneti_assert(tmp & GASNETI_ATOMIC_PRESENT);
          if_pt (retval) {
            tmp = (GASNETI_ATOMIC_PRESENT | newval);
          }
          p->ctr = tmp;
          return retval;
        }
        #define GASNETI_HAVE_ATOMIC_CAS 1

        /* Our code has the following fences: (noting that RMB is empty) */
	#define GASNETI_ATOMIC_FENCE_SET	GASNETI_ATOMIC_MB_PRE
	#define GASNETI_ATOMIC_FENCE_RMW	GASNETI_ATOMIC_MB_PRE
      #else
        #error unrecognized Sparc pre-v9 compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
      #endif
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_PARISC /* PA-RISC */
    /* all we get is atomic load-and-clear, but that's actually just barely enough  */
    #define GASNETI_ATOMICOPS_NOT_SIGNALSAFE 1 /* not signal-safe because of "checkout" semantics */
    #define GASNETI_HAVE_PRIVATE_ATOMIC_T 1
    #if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
      /* Only 31 bits, w/ 8-byte alignment: */
      #define GASNETI_ATOMIC_MAX		((uint32_t)0x7FFFFFFFU)
      #define GASNETI_ATOMIC_SIGNED_MIN		((int32_t)0xC0000000)
      #define GASNETI_ATOMIC_SIGNED_MAX		((int32_t)0x3FFFFFFF)
      #define gasneti_atomic_signed(val)	(((int32_t)((val)<<1))>>1)
      #define gasneti_atomic_align 8
    #else
      /* Full 32 bits, w/ 4-byte alignment: */
      #define GASNETI_ATOMIC_MAX		((uint32_t)0xFFFFFFFFU)
      #define GASNETI_ATOMIC_SIGNED_MIN		((int32_t)0x80000000)
      #define GASNETI_ATOMIC_SIGNED_MAX		((int32_t)0x7FFFFFFF)
      #define gasneti_atomic_align 4
    #endif
    #ifndef _PA_RISC2_0
      /* The load-and-clear requires 16-byte alignment.  Therefore the type (and its
       * initializer) replicate the value field 4 times.  The actual ops will only use
       * the one of them that turns out to be 16-byte aligned.
       */
      typedef struct { volatile uint64_t initflag; volatile uint32_t _ctr[4]; char _pad; } gasneti_atomic_t;
      #define _gasneti_atomic_init(v)      {    \
            GASNETI_ATOMIC_INIT_MAGIC,       \
            { (GASNETI_ATOMIC_PRESENT|(v)),  \
              (GASNETI_ATOMIC_PRESENT|(v)),  \
              (GASNETI_ATOMIC_PRESENT|(v)),  \
              (GASNETI_ATOMIC_PRESENT|(v)) } \
            }
      #define GASNETI_ATOMIC_CTR(p)     ((volatile uint32_t *)GASNETI_ALIGNUP(&(p->_ctr),16))
    #else
      /* The load-and-clear requires only 4-byte alignment for PA-RISC 2.0 */
      typedef struct { volatile uint64_t initflag; volatile uint32_t _ctr; char _pad; } gasneti_atomic_t;
      #define _gasneti_atomic_init(v)   { GASNETI_ATOMIC_INIT_MAGIC, (GASNETI_ATOMIC_PRESENT|(v)) } 
      #define GASNETI_ATOMIC_CTR(p)     (&(p->_ctr))
    #endif
    #define GASNETI_ATOMIC_PRESENT    ((uint32_t)0x80000000)
    #define GASNETI_ATOMIC_INIT_MAGIC ((uint64_t)0x8BDEF66BAD1E3F3AULL)
    typedef uint32_t gasneti_atomic_val_t;
    typedef int32_t gasneti_atomic_sval_t;

    #if PLATFORM_COMPILER_HP_CXX
      #define GASNETI_HAVE_ATOMIC_CAS 1		/* Explicit */
      #define GASNETI_HAVE_ATOMIC_ADD_SUB 1	/* Derived */
      #define GASNETI_USING_SLOW_ATOMICS 1
    #else
      GASNETI_INLINE(gasneti_loadandclear_32)
      uint32_t gasneti_loadandclear_32(uint32_t volatile *v) {
        register uint32_t volatile * addr = (uint32_t volatile *)v;
        register uint32_t val = 0;
        #ifndef _PA_RISC2_0
          gasneti_assert(!(((uintptr_t)addr) & 0xF)); /* ldcws requires 16-byte alignment */
        #else
          gasneti_assert(!(((uintptr_t)addr) & 0x3)); /* ldcws requires 4-byte alignment */
        #endif
        *(volatile char *)(v+1) = 0; /* fetch this cache line as a dirty word - speeds up ldcw */
        #if PLATFORM_COMPILER_GNU
          __asm__ __volatile__ ( 
          #if !GASNET_HAVE_PARISC_LDCW_CO
            "ldcws 0(%2), %0 \n"  
            /* should be using "ldcws,co" here for better performance, 
               but GNU assembler rejects it (works with system assembler) 
             */
          #else
            "ldcw,co 0(%2), %0 \n"  
            /* this alternate, undocumented pseudo-op instruction appears to do the right thing */
          #endif
            : "=r"(val), "=m" (*addr) : "r" (addr), "m" (*addr));
        #elif PLATFORM_COMPILER_HP_C
          _asm("LDCWS,CO",0,0,addr,val);
        #else
          #error unrecognized PA-RISC compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
        #endif
        return val;
      }
      /* would like to use gasneti_waituntil here, but it requires libgasnet for waitmode */
      #define gasneti_atomic_spinuntil(cond) do {       \
              while (!(cond)) gasneti_compiler_fence(); \
              gasneti_local_rmb();                      \
              } while (0)
      GASNETI_INLINE(gasneti_checkout_32)
      uint32_t gasneti_checkout_32(gasneti_atomic_t *p,  volatile uint32_t * pctr) {
	uint32_t retval;
        gasneti_assert(p->initflag == GASNETI_ATOMIC_INIT_MAGIC);
        gasneti_local_wmb();
        gasneti_atomic_spinuntil(*pctr && (retval = gasneti_loadandclear_32(pctr)));
	return retval;
      }
      GASNETI_INLINE(gasneti_atomic_fetchandadd_32)
      uint32_t gasneti_atomic_fetchandadd_32(gasneti_atomic_t *p, int32_t op) {
        volatile uint32_t * const pctr = GASNETI_ATOMIC_CTR(p);
        const uint32_t tmp = gasneti_checkout_32(p, pctr);
        gasneti_assert(tmp & GASNETI_ATOMIC_PRESENT);
        *pctr = (GASNETI_ATOMIC_PRESENT | (tmp + op));
        return (tmp & ~GASNETI_ATOMIC_PRESENT);
      }
      #if 0
        /* this version fails if set is used in a race with fetchandadd */
        GASNETI_INLINE(_gasneti_atomic_set)
        void _gasneti_atomic_set(gasneti_atomic_t *p, uint32_t val) {
          volatile uint32_t * const pctr = GASNETI_ATOMIC_CTR(p);
          gasneti_local_wmb();
          *pctr = (GASNETI_ATOMIC_PRESENT | val);
        }
      #else
        GASNETI_INLINE(_gasneti_atomic_set)
        void _gasneti_atomic_set(gasneti_atomic_t *p, uint32_t val) {
          uint32_t tmp;
          volatile uint32_t * const pctr = GASNETI_ATOMIC_CTR(p);
          if_pf (p->initflag != GASNETI_ATOMIC_INIT_MAGIC) {
            gasneti_local_wmb();
            *pctr = (GASNETI_ATOMIC_PRESENT | val);
            gasneti_local_wmb();
            p->initflag = GASNETI_ATOMIC_INIT_MAGIC;
          } else {
	    (void)gasneti_checkout_32(p, pctr);
            *pctr = (GASNETI_ATOMIC_PRESENT | val);
          }
        }
      #endif
      GASNETI_INLINE(_gasneti_atomic_read)
      uint32_t _gasneti_atomic_read(gasneti_atomic_t *p) {
        uint32_t tmp;
        volatile uint32_t * const pctr = GASNETI_ATOMIC_CTR(p);
        gasneti_assert(p->initflag == GASNETI_ATOMIC_INIT_MAGIC);
        gasneti_atomic_spinuntil((tmp = *pctr));
        gasneti_assert(tmp & GASNETI_ATOMIC_PRESENT);
        return (tmp & ~GASNETI_ATOMIC_PRESENT);
      }

      /* Default impls of inc, dec, dec-and-test, add and sub */
      #define _gasneti_atomic_fetchadd gasneti_atomic_fetchandadd_32

      GASNETI_INLINE(_gasneti_atomic_compare_and_swap)
      int _gasneti_atomic_compare_and_swap(gasneti_atomic_t *p, uint32_t oldval, uint32_t newval) {
        volatile uint32_t * const pctr = GASNETI_ATOMIC_CTR(p);
        uint32_t tmp = gasneti_checkout_32(p, pctr);
        const int retval = (tmp == (GASNETI_ATOMIC_PRESENT | oldval));
        gasneti_assert(tmp & GASNETI_ATOMIC_PRESENT);
        if_pt (retval) {
          tmp = (GASNETI_ATOMIC_PRESENT | newval);
        }
        *pctr = tmp;
        return retval;
      }
      #define GASNETI_HAVE_ATOMIC_CAS 1

      /* Our code has the following fences: (noting that RMB is empty) */
      #define GASNETI_ATOMIC_FENCE_SET	GASNETI_ATOMIC_MB_PRE
      #define GASNETI_ATOMIC_FENCE_RMW	GASNETI_ATOMIC_MB_PRE
    #endif /* ! slow atomics */
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_CRAYX1 /* This works on X1, but NOT the T3E */
    #include <intrinsics.h>
    /* man pages for atomic ops claim gsync is required for using atomic ops,
       but it's unclear when exactly it is required for our purposes - 
       technically we shouldn't need any sync for a bare unfenced AMO, but 
       experimentally determined using testtools that without a gsync in the vicinity
       of the AMO call, the compiler/architecture will break the semantics of the AMO
       with respect to the atomic location, even with only a single thread!
       Note gsync call MUST be _gsync(0x1) - the manpage docs are gratuitiously wrong, 
       everything else gives a bus error
     */
    #define gasneti_atomic_presync()  ((void)0)
    #define gasneti_atomic_postsync()  _gsync(0x1)

    #define GASNETI_HAVE_ATOMIC64_T 1
    typedef volatile int64_t gasneti_atomic64_t;

    #define _gasneti_atomic64_increment(p)	\
      (gasneti_atomic_presync(),_amo_aadd((p),(long)1),gasneti_atomic_postsync())
    #define _gasneti_atomic64_decrement(p)	\
      (gasneti_atomic_presync(),_amo_aadd((p),(long)-1),gasneti_atomic_postsync())
    #define _gasneti_atomic64_read(p)      (*(p))
    #define _gasneti_atomic64_set(p,v)     (*(p) = (v))
    #define _gasneti_atomic64_init(v)      (v)

    GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
    int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
      long result;
      gasneti_atomic_presync();
      result = _amo_acswap(p, (long)oldval, (long)newval);
      gasneti_atomic_postsync();
      return (result == oldval); 
    }

    GASNETI_INLINE(_gasneti_atomic64_fetchadd)
    uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *p, int64_t op) {
       long oldval;
       gasneti_atomic_presync();
       oldval = _amo_afadd((p),(long)(op));
       gasneti_atomic_postsync();
       return oldval;
    }
    #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd

    /* Currently operating on the assumption that gsync() is a full MB: */
    #define GASNETI_ATOMIC_FENCE_RMW	GASNETI_ATOMIC_MB_POST
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_NECSX
    #define GASNETI_HAVE_PRIVATE_ATOMIC_T 1	/* No CAS */
    typedef uint32_t                      gasneti_atomic_val_t;
    typedef int32_t                       gasneti_atomic_sval_t;
    #define GASNETI_ATOMIC_MAX            ((gasneti_atomic_val_t)0xFFFFFFFFU)
    #define GASNETI_ATOMIC_SIGNED_MIN     ((gasneti_atomic_sval_t)0x80000000)
    #define GASNETI_ATOMIC_SIGNED_MAX     ((gasneti_atomic_sval_t)0x7FFFFFFF)
    typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
    #define _gasneti_atomic_init(v)      { (v) }
   #if 0
    /* these are disabled for now because they don't link */
    #include <sys/mplock.h>
    #define _gasneti_atomic_read(p)      (atomic_read4((p)->ctr))
    #define _gasneti_atomic_set(p,v)     (atomic_set4((p)->ctr,(v)))

    /* Default impls of inc, dec, dec-and-test, add and sub */
    #define _gasneti_atomic_addfetch(p,op) atomic_add4(&((p)->ctr),op)

    /* Using default fences (TODO: VERIFY THAT WE NEED THEM) */
   #else
    #define _gasneti_atomic_read(p)      (muget(&((p)->ctr)))
    #define _gasneti_atomic_set(p,v)     (muset(&((p)->ctr),(v)))

    /* Default impls of inc, dec, dec-and-test, add and sub */
    #define _gasneti_atomic_addfetch(p,op) muadd(&((p)->ctr),op)

    /* Using default fences (TODO: VERIFY THAT WE NEED THEM) */
   #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_POWERPC
    #if defined(GASNETI_ARCH_PPC64)
      /* We are running on a 64-bit capable CPU/OS, but can't use native atomics
         on improperly aligned data.  Since our "contract" with the developer says
         atomic64_t works on 64-bit types without any extra alignment, we may need
         to use mutex-based atomics when not aligned.  See bug 1595 for more info.
       */
      #if (PLATFORM_OS_DARWIN || PLATFORM_OS_AIX)
        /* + Apple's ABI only guarantees 4-byte minimum aligment for 64-bit integers and doubles.
         * + AIX's ABI only guarantees 4-byte minimum aligment for doubles.
         */
        #define GASNETI_HYBRID_ATOMIC64	1
      #endif

      /* Should we use native 64-bit atomics on ILP32? */
      #if PLATFORM_ARCH_32 && \
	  (PLATFORM_OS_DARWIN || PLATFORM_OS_AIX || PLATFORM_OS_LINUX)
        #define GASNETI_PPC64_ILP32_NATIVE_ATOMICS 1
      #endif
    #endif

    #if PLATFORM_COMPILER_XLC
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }

      /* XLC machine code functions are very rigid, thus we produce all
       * three read-modify-write ops as distinct functions in order to
       * get anything near to optimal code.
       */
      static void _gasneti_atomic32_increment(gasneti_atomic32_t *v);
      #pragma mc_func _gasneti_atomic32_increment {\
	/* ARGS: r3 = v  LOCAL: r4 = tmp */ \
	"7c801828"	/* 0: lwarx	r4,0,r3		*/ \
	"38840001"	/*    addi	r4,r4,0x1	*/ \
	"7c80192d"	/*    stwcx.	r4,0,r3		*/ \
	"40a2fff4"	/*    bne-	0b		*/ \
      }
      #pragma reg_killed_by _gasneti_atomic32_increment cr0, gr4

      static void _gasneti_atomic32_decrement(gasneti_atomic32_t *v);
      #pragma mc_func _gasneti_atomic32_decrement {\
	/* ARGS: r3 = v  LOCAL: r4 = tmp */ \
	"7c801828"	/* 0: lwarx	r4,0,r3		*/ \
	"3884ffff"	/*    subi	r4,r4,0x1	*/ \
	"7c80192d"	/*    stwcx.	r4,0,r3		*/ \
	"40a2fff4"	/*    bne-	0b		*/ \
      }
      #pragma reg_killed_by _gasneti_atomic32_decrement cr0, gr4

      static uint32_t gasneti_atomic32_decandfetch(gasneti_atomic32_t *v);
      #pragma mc_func gasneti_atomic32_decandfetch {\
	/* ARGS: r3 = v  LOCAL: r4 = tmp */ \
	"7c801828"	/* 0: lwarx	r4,0,r3		*/ \
	"3884ffff"	/*    subi	r4,r4,0x1	*/ \
	"7c80192d"	/*    stwcx.	r4,0,r3		*/ \
	"40a2fff4"	/*    bne-	0b		*/ \
	"7c832378"	/*    mr	r3,r4		*/ \
	/* RETURN in r3 = result after dec */ \
      }
      #pragma reg_killed_by gasneti_atomic32_decandfetch cr0, gr4
      #define _gasneti_atomic32_decrement_and_test(p) (gasneti_atomic32_decandfetch(p) == 0)

      static int gasneti_atomic32_swap_not(gasneti_atomic32_t *v, uint32_t oldval, uint32_t newval);
      #pragma mc_func gasneti_atomic32_swap_not {\
	/* ARGS: r3 = p, r4=oldval, r5=newval   LOCAL: r0 = tmp */ \
	"7c001828"	/* 0: lwarx	r0,0,r3		*/ \
	"7c002279"	/*    xor.	r0,r0,r4	*/ \
	"40820010"	/*    bne	1f		*/ \
	"7ca0192d"	/*    stwcx.	r5,0,r3		*/ \
	"40a2fff0"	/*    bne-	0b		*/ \
	"7c030378"	/* 1: mr	r3,r0		*/ \
	/* RETURN in r3 = 0 iff swap took place */ \
      }
      #pragma reg_killed_by gasneti_atomic32_swap_not cr0, gr0
      #define _gasneti_atomic32_compare_and_swap(p, oldval, newval) \
	(gasneti_atomic32_swap_not((p),(oldval),(newval)) == 0)

      static uint32_t _gasneti_atomic32_addfetch(gasneti_atomic32_t *v, int32_t op);
      #pragma mc_func _gasneti_atomic32_addfetch {\
	/* ARGS: r3 = v  LOCAL: r4 = op, r5 = tmp */ \
	"7ca01828"	/* 0: lwarx	r5,0,r3		*/ \
	"7ca52214"	/*    add	r5,r5,r4	*/ \
	"7ca0192d"	/*    stwcx.	r5,0,r3		*/ \
	"40a2fff4"	/*    bne-	0b		*/ \
	"7ca32b78"	/*    mr	r3,r5		*/ \
	/* RETURN in r3 = result after addition */ \
      }
      #pragma reg_killed_by _gasneti_atomic32_addfetch cr0, gr4, gr5
      #define _gasneti_atomic32_addfetch _gasneti_atomic32_addfetch

      #if PLATFORM_ARCH_64
	#define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
        #define _gasneti_atomic64_init(_v)	{ (_v) }
        #define _gasneti_atomic64_set(_p,_v)	do { (_p)->ctr = (_v); } while(0)
        #define _gasneti_atomic64_read(_p)	((_p)->ctr)

        static uint64_t gasneti_atomic64_swap_not(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval);
        #pragma mc_func gasneti_atomic64_swap_not {\
	  /* ARGS: r3 = p, r4=oldval, r5=newval   LOCAL: r0 = tmp */ \
	  "7c0018a8"	/* 0: ldarx   r0,0,r3	*/ \
	  "7c002279"	/*    xor.    r0,r0,r4	*/ \
	  "4082000c"	/*    bne-    1f	*/ \
	  "7ca019ad"	/*    stdcx.  r5,0,r3	*/ \
	  "40a2fff0"	/*    bne-    0b	*/ \
	  "7c030378"	/* 1: mr      r3,r0	*/ \
	  /* RETURN in r3 = 0 iff swap took place */ \
        }
        #pragma reg_killed_by gasneti_atomic64_swap_not cr0, gr0
        #define _gasneti_atomic64_compare_and_swap(p, oldval, newval) \
					(gasneti_atomic64_swap_not(p, oldval, newval) == 0)
      #elif defined(GASNETI_PPC64_ILP32_NATIVE_ATOMICS) /* ILP32 on 64-bit CPU */
	#define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
        #define _gasneti_atomic64_init(_v)	{ (_v) }

        static uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *p);
        static void _gasneti_atomic64_set(gasneti_atomic64_t *p, uint64_t val);
        static int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval);
        #pragma mc_func _gasneti_atomic64_read { \
          /* ARGS: r3 = p  RESULT: r3 = hi32, r4 = lo32 */ \
	  /* LOCAL: r0 = canary, r5 = tmp */ \
	  "38007fff"  /* 0: li      r0,0x7fff */ \
	  "780007c6"  /*    sldi    r0,r0,32  */ \
	  "e8a30000"  /*    ld      r5,0(r3)  */ \
	  "78a40020"  /*    clrldi  r4,r5,32  */ \
	  "78a50022"  /*    srdi    r5,r5,32  */ \
	  "78000022"  /*    srdi    r0,r0,32  */ \
	  "2c207fff"  /*    cmpdi   r0,0x7fff */ \
	  "40a2ffe4"  /*    bne-    0b        */ \
	  "7ca32b78"  /*    mr      r3,r5     */ \
        }
        #pragma reg_killed_by _gasneti_atomic64_read cr0, gr0, gr5
	#if PLATFORM_OS_LINUX || PLATFORM_OS_BLRTS /* ABI differs from Darwin and AIX */
          #pragma mc_func _gasneti_atomic64_set { \
            /* ARGS: r3 = p, r5 = hi32, r6 = lo32  LOCAL: r0 = tmp */ \
	    "78c60020"  /*    clrldi  r6,r6,32 */ \
	    "7c0018a8"  /* 0: ldarx   r0,r0,r3 */ \
	    "78a007c6"  /*    sldi    r0,r5,32 */ \
	    "7c003378"  /*    or      r0,r0,r6 */ \
	    "7c0019ad"  /*    stdcx.  r0,r0,r3 */ \
	    "40a2fff0"  /*    bne-    0b       */ \
          }
          #pragma reg_killed_by _gasneti_atomic64_set cr0, gr0, gr6
          #pragma mc_func _gasneti_atomic64_compare_and_swap {\
	    /* ARGS: r3 = p, r5=oldhi32, r6=oldlo32, r7=newhi32, r8=newlo32 */ \
	    /* LOCAL: r0 = tmp1, r4 = tmp2 */ \
	    "78c60020"  /*    clrldi  r6,r6,32 */ \
	    "79080020"  /*    clrldi  r8,r8,32 */ \
	    "7c0018a8"  /* 0: ldarx   r0,r0,r3 */ \
	    "78a407c6"  /*    sldi    r4,r5,32 */ \
	    "7c843378"  /*    or      r4,r4,r6 */ \
	    "7c240000"  /*    cmpd    r4,r0    */ \
	    "38800000"  /*    li      r4,0     */ \
	    "40820010"  /*    bne-    1f       */ \
	    "38800001"  /*    li      r4,1     */ \
	    "78e007c6"  /*    sldi    r0,r7,32 */ \
	    "7c004378"  /*    or      r0,r0,r8 */ \
	    "7c0019ad"  /* 1: stdcx.  r0,r0,r3 */ \
	    "40a2ffd8"  /*    bne-    0b       */ \
	    "7c832378"  /*    mr      r3,r4    */ \
	    /* RETURN in r3 = 1 iff swap took place */ \
          }
          #pragma reg_killed_by _gasneti_atomic64_compare_and_swap cr0, gr0, gr4, gr6, gr7
        #else
          #pragma mc_func _gasneti_atomic64_set { \
            /* ARGS: r3 = p, r4 = hi32, r5 = lo32  LOCAL: r0 = tmp */ \
	    "78a50020"  /*    clrldi  r5,r5,32 */ \
	    "7c0018a8"  /* 0: ldarx   r0,0,r3  */ \
	    "788007c6"  /*    sldi    r0,r4,32 */ \
	    "7c002b78"  /*    or      r0,r0,r5 */ \
	    "7c0019ad"  /*    stdcx.  r0,0,r3  */ \
	    "40a2fff0"  /*    bne-    0b       */ \
          }
          #pragma reg_killed_by _gasneti_atomic64_set cr0, gr0, gr5
          #pragma mc_func _gasneti_atomic64_compare_and_swap {\
	    /* ARGS: r3 = p, r4=oldhi32, r5=oldlo32, r6=newhi32, r7=newlo32 */ \
	    /* LOCAL: r0 = tmp1, r8 = tmp2 */ \
	    "78a50020"  /*    clrldi  r5,r5,32 */ \
	    "78e70020"  /*    clrldi  r7,r7,32 */ \
	    "7c0018a8"  /* 0: ldarx   r0,0,r3  */ \
	    "788807c6"  /*    srdi    r8,r4,32 */ \
	    "7d082b78"  /*    or      r8,r8,r5 */ \
	    "7c280000"  /*    cmpd    r8,r0    */ \
	    "39000000"  /*    li      r8,0     */ \
	    "40820010"  /*    bne-    1f       */ \
	    "39000001"  /*    li      r8,1     */ \
	    "78c007c6"  /*    srdi    r0,r6,32 */ \
	    "7c003b78"  /*    or      r0,r0,r7 */ \
	    "7c0019ad"  /* 1: stdcx.  r0,0,r3  */ \
	    "40a2ffd8"  /*    bne-    0b       */ \
	    "7d034378"  /*    mr      r3,r8    */ \
	    /* RETURN in r3 = 1 iff swap took place */ \
          }
          #pragma reg_killed_by _gasneti_atomic64_compare_and_swap cr0, gr0, gr5, gr7, gr8
	#endif
      #else
	/* 32-bit CPU - generics are the only option */
      #endif

      /* Using default fences as we have none in our asms */
    #elif PLATFORM_COMPILER_GNU
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }

      GASNETI_INLINE(gasneti_atomic32_addandfetch)
      uint32_t gasneti_atomic32_addandfetch(gasneti_atomic32_t *v, int32_t op) {
        register uint32_t result;
        __asm__ __volatile__ ( 
          "Lga.0.%=:\t"                 /* AIX assembler doesn't grok "0:"-type local labels */
          "lwarx    %0,0,%2 \n\t" 
          "add%I3   %0,%0,%3 \n\t"
          "stwcx.   %0,0,%2 \n\t"
          "bne-     Lga.0.%= \n\t" 
          : "=&b"(result), "=m" (v->ctr)	/* constraint b = "b"ase register (not r0) */
          : "r" (v), "Ir"(op) , "m"(v->ctr)
          : "cr0");
        return result;
      }
      #define _gasneti_atomic32_addfetch gasneti_atomic32_addandfetch

      /* Default impls of inc, dec, dec-and-test, add and sub */

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, uint32_t oldval, uint32_t newval) {
        register uint32_t result;
        __asm__ __volatile__ (
          "Lga.0.%=:\t"                   /* AIX assembler doesn't grok "0:"-type local labels */
	  "lwarx    %0,0,%2 \n\t"         /* load to result */
	  "xor.     %0,%0,%3 \n\t"        /* xor result w/ oldval */
	  "bne      Lga.1.%= \n\t"        /* branch on mismatch */
	  "stwcx.   %4,0,%2 \n\t"         /* store newval */
	  "bne-     Lga.0.%= \n\t" 
	  "Lga.1.%=:	"
	  : "=&r"(result), "=m"(p->ctr)
	  : "r" (p), "r"(oldval), "r"(newval), "m"(p->ctr)
	  : "cr0");
  
        return (result == 0);
      } 

      #if PLATFORM_ARCH_64
	#define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
        #define _gasneti_atomic64_init(_v)	{ (_v) }
        #define _gasneti_atomic64_set(_p,_v)	do { (_p)->ctr = (_v); } while(0)
        #define _gasneti_atomic64_read(_p)	((_p)->ctr)
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
          register uint64_t result;
          __asm__ __volatile__ (
		"Lga.0.%=:\t"                   /* AIX assembler doesn't grok "0:"-type local labels */
		"ldarx    %0,0,%2 \n\t"         /* load to result */
		"xor.     %0,%0,%3 \n\t"        /* compare result w/ oldval */
		"bne      Lga.1.%= \n\t"        /* branch on mismatch */
		"stdcx.   %4,0,%2 \n\t"         /* store newval */
		"bne-     Lga.0.%= \n\t"        /* retry on conflict */
		"Lga.1.%=:	"
		: "=&b"(result), "=m"(p->ctr)
		: "r" (p), "r"(oldval), "r"(newval), "m"(p->ctr)
		: "cr0");
          return (result == 0);
        } 
      #elif defined(GASNETI_PPC64_ILP32_NATIVE_ATOMICS) /* ILP32 on 64-bit CPU */
	#define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
        #define _gasneti_atomic64_init(_v)	{ (_v) }
        GASNETI_INLINE(_gasneti_atomic64_set)
        void _gasneti_atomic64_set(gasneti_atomic64_t *p, uint64_t val) {
          uint32_t tmp;
	  /* We are using the ll/sc reservation as a "canary" that will ensure we
	     don't write to memory a value that was clobbered by an interruption
	     (context switch, signal handler, etc.). */
          __asm__ __volatile__ (
		"clrldi	%L2,%L2,32	\n\t"	/* Zap undefined top half of val */
		"Lga.0.%=:		\t"	/* AIX assembler doesn't grok "0:"-type local labels */
		"ldarx	%1,0,%3		\n\t"	/* establish reservation */
		"sldi	%1,%2,32	\n\t"	/* construct 64-bit...   */
		"or	%1,%1,%L2	\n\t"	/* ... value in tmp register */
		"stdcx.	%1,0,%3		\n\t"	/* store val */
		"bne-	Lga.0.%=	"	/* retry on loss of reservation */
		: "=m"(p->ctr), "=&b"(tmp)
		: "r"(val), "r"(p), "m"(p->ctr)
		: "cr0" );
        }
        GASNETI_INLINE(_gasneti_atomic64_read)
        uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *p) {
          uint64_t retval;	/* gcc allocates a pair of regs for this */
          uint32_t tmp;
	  /* We are using an extra register with a non-zero upper half as a "canary"
	     to detect when an interruption (context switch, signal handler, etc.) has
	     clobbered the upper halves of the register set.  We pick a value that is
	     zero in the lower half to be insensitive to whether the "clobber" does
	     sign extension or zero extension. */ 
          __asm__ __volatile__ (
		"Lga.0.%=:\t"                   /* AIX assembler doesn't grok "0:"-type local labels */
		"li	%1,0x7fff	\n\t"	/* Canary value in tmp ... */
		"sldi	%1,%1,32	\n\t"   /*  ... = 0x00007FFF.00000000 */
		"ld	%0,%2		\n\t"	/* 64-bit load into "hi" reg of pair */
		"clrldi	%L0,%0,32	\n\t"	/* "lo" reg of pair gets 32 low bits */
		"srdi	%0,%0,32	\n\t"	/* "hi" reg of pair gets 32 high bits */
		"srdi	%1,%1,32	\n\t"	/* Check thar upper half of canary... */
		"cmpdi	%1,0x7fff	\n\t"	/*  ... is still 0x00007FFF */
		"bne-	Lga.0.%=	"	/* retry on canary changed */
		: "=r"(retval), "=r"(tmp)
		: "m"(p->ctr)
		: "cr0" );
          return retval;
        }
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
          register int result;
	  register uint32_t tmp;
	  /* We are using the ll/sc reservation as a "canary" that will ensure we
	     don't trust registers clobbered by an interruption (context switch,
	     signal handler, etc.).  To make this work correctly we need to perform
	     the "swap" even on a failed "compare" (in case the clobber is the only
	     reason the compare failed).  If that is the case, then we swap in the
	     original value, knowing that the normal ll/sc rules will not let us
	     overwrite the value if it changed since we read it. */
          __asm__ __volatile__ (
		"clrldi   %L5,%L5,32	\n\t"	/* Zap undefined top half of oldval */
		"clrldi   %L6,%L6,32	\n\t"	/* Zap undefined top half of newval */
		"Lga.0.%=:		\t"	/* AIX assembler doesn't grok "0:"-type local labels */
		"ldarx    %1,0,%3	\n\t"	/* load memory to tmp */
		"sldi     %0,%5,32	\n\t"	/* shift hi32 half of oldval to result */
		"or       %0,%0,%L5	\n\t"	/*   and or in lo32 of oldval to result */
		"cmpd     0,%0,%1	\n\t"	/* compare memory (tmp) w/ oldval (result) */
		"li	  %0,0		\n\t"	/* assume failure */
		"bne      Lga.1.%=	\n\t"	/* branch to stdcx. on mismatch */
		"li	  %0,1		\n\t"	/* success */
		"sldi     %1,%6,32      \n\t"	/* shift hi32 half of newval to tmp  */
		"or       %1,%1,%L6     \n\t"	/*   and or in lo32 of newval to tmp */
		"Lga.1.%=:		\t"
		"stdcx.   %1,0,%3	\n\t"	/* try to store tmp (may be newval or read value) */
		"bne-     Lga.0.%=	"	/* retry on loss of reservation */
		: "=&r"(result), "=&r"(tmp), "=m"(p->ctr)
		: "r" (p), "m"(p->ctr), "r"(oldval), "r"(newval)
		: "cr0");
          return result;
        } 
      #else
	/* 32-bit CPU - generics are the only option */
      #endif

      /* Using default fences as we have none in our asms */
    #else
      #error Unrecognized PowerPC - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_MIPS
    #if GASNETI_ARCH_SGI_IP27
      /* According to the Linux kernel source, there is an erratum for the R10k cpu
       * (multi-processor only) in which ll/sc or lld/scd may not execute atomically!
       * The work-around is to predict-taken the back-branch after the sc or scd.
       * We must *not* use "beqzl" unconditionally, since the MIPS32 manual warns
       * that the "branch likely" instructions will be removed in a future revision.
       */
      #define GASNETI_MIPS_BEQZ "beqzl "	/* 'l' = likely */
    #else
      #define GASNETI_MIPS_BEQZ "beqz "
    #endif

    #if GASNETI_ARCH_ICE9A
      /* According to SiCortex the ICE9A processor has an errata that
       * can be worked-around by doubling of the LL or LLD instruction.
       */
      #define GASNETI_MIPS_LL(_insn) _insn _insn
    #else
      #define GASNETI_MIPS_LL(_insn) _insn
    #endif
  
    #if PLATFORM_COMPILER_PATHSCALE
      /* Don't define GASNETI_MIPS_AT, as pathcc uses $at as a GP register */
      #undef GASNETI_MIPS_AT
    #elif defined(GASNETI_HAVE_MIPS_REG_AT)
      #define GASNETI_MIPS_AT "$at"
    #elif defined(GASNETI_HAVE_MIPS_REG_1)
      #define GASNETI_MIPS_AT "$1"
    #endif

    #if PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_PATHSCALE
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_init(v)      { (v) }
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))

      /* We can't assume GNU as, so no push/pop. */
      #if PLATFORM_COMPILER_GNU
        /* Default is at,reorder,macro */
        #define GASNETI_MIPS_START_NOAT        ".set   noat\n\t"
        #define GASNETI_MIPS_END_NOAT          ".set   at\n\t"
        #define GASNETI_MIPS_START_NOREORDER   ".set   noreorder\n\t.set   nomacro\n\t"
        #define GASNETI_MIPS_END_NOREORDER     ".set   reorder\n\t.set   macro\n\t"
        #define GASNETI_MIPS_RETRY(ARGS)       GASNETI_MIPS_BEQZ ARGS "\n\t"
      #elif PLATFORM_COMPILER_PATHSCALE
        /* Default is noat,noreorder,nomacro */
        #define GASNETI_MIPS_START_NOAT
        #define GASNETI_MIPS_END_NOAT
        #define GASNETI_MIPS_START_NOREORDER
        #define GASNETI_MIPS_END_NOREORDER
        #define GASNETI_MIPS_RETRY(ARGS)       GASNETI_MIPS_BEQZ ARGS "\n\tnop\n\t"
      #else
        #error
      #endif

      GASNETI_INLINE(gasneti_atomic32_fetchadd)
      uint32_t gasneti_atomic32_fetchadd(gasneti_atomic32_t *p, int32_t op) {
       #ifdef GASNETI_MIPS_AT
	uint32_t retval;
	__asm__ __volatile__(
		GASNETI_MIPS_START_NOAT
		".set	mips2		\n\t"
		"1:			\n\t"
GASNETI_MIPS_LL("ll	%0,0(%3)	\n\t")
		"addu	" GASNETI_MIPS_AT ",%0,%2	\n\t"
		"sc	" GASNETI_MIPS_AT ",0(%3)	\n\t"
		GASNETI_MIPS_RETRY(GASNETI_MIPS_AT ",1b")
		".set	mips0		\n\t"
		GASNETI_MIPS_END_NOAT
		: "=&r" (retval), "=m" (p->ctr)
		: "Ir" (op), "r" (p), "m" (p->ctr) );
       #else
        /* Don't know how to access $1/$at.  So use another temporary */
        uint32_t tmp, retval;
	__asm__ __volatile__(
		".set	mips2		\n\t"
		"1:			\n\t"
GASNETI_MIPS_LL("ll	%0,0(%4)	\n\t")
		"addu	%1,%0,%3	\n\t"
		"sc	%1,0(%4)	\n\t"
		GASNETI_MIPS_RETRY("%1,1b")
		".set	mips0		\n\t"
		: "=&r" (retval), "=&r" (tmp), "=m" (p->ctr)
		: "Ir" (op), "r" (p), "m" (p->ctr) );
       #endif
	return retval;
      }
      #define _gasneti_atomic32_fetchadd gasneti_atomic32_fetchadd

      /* Macro that expands to either 32- or 64-bit CAS */
      /* NOTE: evaluates _p multiple times */
      #define __gasneti_atomic_compare_and_swap_inner(_abi, _ll, _sc, _retval, _p, _oldval, _newval) \
         __asm__ __volatile__ (                                                                \
                "1:                      \n\t"                                                 \
                ".set   " _abi "         \n\t" /* [ set ABI to allow ll/sc ]                */ \
GASNETI_MIPS_LL(_ll "   %0,0(%4)         \n\t")/* _retval = *p (starts ll/sc reservation)   */ \
                ".set   mips0            \n\t" /* [ set ABI back to default ]               */ \
                GASNETI_MIPS_START_NOREORDER   /* [ tell assembler we fill our delay slot ] */ \
                "bne    %0,%z2,2f        \n\t" /* Break loop on mismatch                    */ \
                " move  %0,$0            \n\t" /* Zero _retval (in delay slot)              */ \
                GASNETI_MIPS_END_NOREORDER     /* [ tell assembler to fill delay slots ]    */ \
                "move   %0,%z3           \n\t" /* _retval = _newval                         */ \
                ".set   " _abi "         \n\t" /* [ set ABI to allow ll/sc ]                */ \
                _sc "   %0,0(%4)         \n\t" /* Try *p = _retval (sets _retval 0 or 1)    */ \
                ".set   mips0            \n\t" /* [ set ABI back to default ]               */ \
                GASNETI_MIPS_RETRY("%0,1b")    /* Retry on contention                       */ \
                "2:                        "                                                   \
                : "=&r" (_retval), "=m" ((_p)->ctr)                                            \
                : "Jr" (_oldval), "Jr" (_newval), "r" ((_p)), "m" ((_p)->ctr) )
      #define __gasneti_atomic32_compare_and_swap_inner(_retval, _p, _oldval, _newval) \
              __gasneti_atomic_compare_and_swap_inner("mips2", "ll", "sc", \
                                                      (_retval), (_p), (_oldval), (_newval))
      #define __gasneti_atomic64_compare_and_swap_inner(_retval, _p, _oldval, _newval) \
              __gasneti_atomic_compare_and_swap_inner("mips3", "lld", "scd", \
                                                      (_retval), (_p), (_oldval), (_newval))

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *p, uint32_t oldval, uint32_t newval) {
        int retval;
        #if PLATFORM_ARCH_64 || (defined(_MIPS_ISA) && (_MIPS_ISA >= 3) /* 64-bit capable CPU */)
        if (!__builtin_constant_p(oldval)) {
          /* Ensure oldval is properly sign-extended for comparison to read value */
          __asm__ __volatile__("sll %0,%0,0" : "+r" (oldval));
        }
        #endif
        __gasneti_atomic32_compare_and_swap_inner(retval, p, oldval, newval);
        return retval;
      }

      #if PLATFORM_ARCH_64 || (defined(_MIPS_ISA) && (_MIPS_ISA >= 3) /* 64-bit capable CPU */)
        #define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t ctr; } gasneti_atomic64_t;
        #define _gasneti_atomic64_read(p)      ((p)->ctr)
        #define _gasneti_atomic64_set(p,v)     do { (p)->ctr = (v); } while(0)
        #define _gasneti_atomic64_init(v)      { (v) }

        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval) {
          int retval;
          __gasneti_atomic64_compare_and_swap_inner(retval, p, oldval, newval);
          return retval;
        }
      #endif

      /* SGI docs say ll/sc include a memory fence.
       *    SGI Part Number 02-00036-005, page 5-5 and 5-7
       * Tests conducted on the an SGI Origin 2000 are consistent with this.
       *
       * Since the 2.6.23 kernel, Linux is supporting a configuration option
       * (CONFIG_WEAK_REORDERING_BEYOND_LLSC) for MIPS processors, though there
       * are no template configs that set this option as of the 2.6.28 kernel.
       * When set, this option causes a "SYNC" to be inserted in the kernel's
       * mutex-type assembly routines as follows:
       *   - AFTER lock acquisition
       *   - BEFORE lock release
       * The behavior of a SiCortex machine appears consistent with this
       * "weak reordering beyond ll/sc" behavior.
       *
       * We'll stick w/ the safe option for now: use the default fences.
       * However, we could potentially improve peformance on SGI machines
       * as follows:
       * #if PLATFORM_OS_IRIX
       *   #define GASNETI_ATOMIC_FENCE_RMW (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)
       * #endif
       */
      /* No memory fences in our asm, so using default fences */
    #else
      #error "unrecognized MIPS compiler and/or OS - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)"
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_ARM && defined(GASNETI_HAVE_ARM_CMPXCHG)
    #if PLATFORM_COMPILER_GNU && PLATFORM_OS_LINUX
      #define GASNETI_HAVE_ATOMIC32_T 1

      typedef struct { volatile unsigned int ctr; } gasneti_atomic32_t;
      #define _gasneti_atomic32_read(p)      ((p)->ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->ctr = (v))
      #define _gasneti_atomic32_init(v)      { (v) }

      #define gasneti_atomic32_addfetch_const(p, op) ({                         \
	register unsigned long __sum asm("r1");                                 \
	register unsigned long __ptr asm("r2") = (unsigned long)(p);            \
	__asm__ __volatile__ (                                                  \
		"0:	ldr	r0, [r2]	@ r0 = *p		\n"     \
		"	mov	r3, #0xffff0fff @ r3 = base addr        \n"     \
		"	adr	lr, 1f		@ lr = return to '1:'	\n"     \
		"	add	r1, r0, %2	@ r1 = r0 + op		\n"     \
		"	sub	pc, r3, #0x3f   @ call 0xfff0fc0        \n"     \
		"1:	bcc     0b		@ retry on Carry Clear"         \
		: "=&r" (__sum)                                                 \
		: "r" (__ptr), "IL" (op)                                        \
		: "r0", "r3", "ip", "lr", "cc", "memory" );                     \
	(__sum);                                                                \
      })

      GASNETI_INLINE(gasneti_atomic32_inc)
      void gasneti_atomic32_inc(gasneti_atomic32_t *p) {
        (void)gasneti_atomic32_addfetch_const(p, 1);
      }
      #define _gasneti_atomic32_increment gasneti_atomic32_inc

      GASNETI_INLINE(gasneti_atomic32_dec)
      void gasneti_atomic32_dec(gasneti_atomic32_t *p) {
        (void)gasneti_atomic32_addfetch_const(p, -1);
      }
      #define _gasneti_atomic32_decrement gasneti_atomic32_dec

      GASNETI_INLINE(gasneti_atomic32_dec_and_test)
      int gasneti_atomic32_dec_and_test(gasneti_atomic32_t *p) {
        return !gasneti_atomic32_addfetch_const(p, -1);
      }
      #define _gasneti_atomic32_decrement_and_test gasneti_atomic32_dec_and_test

      /* Need to schedule r4, since '=&r' doesn't appear to prevent
       * selection of the same register for __op and __sum.
       * XXX: otherwise could use for inc, dec, dec-and-test
       */
      GASNETI_INLINE(gasneti_atomic32_addfetch)
      uint32_t gasneti_atomic32_addfetch(gasneti_atomic32_t *p, int32_t op) {
	register unsigned long __sum asm("r1");
	register unsigned long __ptr asm("r2") = (unsigned long)(p);
	register unsigned long __op asm("r4") = op;

	__asm__ __volatile__ (
		"0:	ldr	r0, [r2]	@ r0 = *p		\n"
		"	mov	r3, #0xffff0fff @ r3 = base addr        \n"
		"	adr	lr, 1f		@ lr = return to '1:'	\n"
		"	add	r1, r0, %2	@ r1 = r0 + op		\n"
		"	sub	pc, r3, #0x3f   @ call 0xfff0fc0        \n"
		"1:	bcc     0b		@ retry on Carry Clear  "
		: "=&r" (__sum)
		: "r" (__ptr), "r" (__op)
		: "r0", "r3", "ip", "lr", "cc", "memory" );
	
    	return __sum;
      }
      #define _gasneti_atomic32_addfetch gasneti_atomic32_addfetch

      /* Default impls of add and sub */

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *v, int oldval, int newval) {
	register unsigned int result asm("r0");
	register unsigned int _newval asm("r1") = newval;
	register unsigned int _v asm("r2") = (unsigned long)v;
	register unsigned int _oldval asm("r4") = oldval;

	/* Transient failure is possible if interrupted.
	 * Since we can't distinguish the cause of the failure,
	 * we must retry as long as the failure looks "improper"
	 * which is defined as (!swapped && (v->ctr == oldval))
	 */
	__asm__ __volatile__ (
		"0:	mov	r0, r4          @ r0 = oldval              \n"
		"	mov	r3, #0xffff0fff @ r3 = base addr           \n"
		"	mov	lr, pc		@ lr = return addr         \n"
		"	sub	pc, r3, #0x3f   @ call 0xffff0fc0          \n"
		"	ldrcc	ip, [r2]	@ if (!swapped) ip=v->ctr  \n"
		"	eorcs	ip, r4, #1	@ else ip=oldval^1         \n"
		"	teq	r4, ip		@ if (ip == oldval)        \n"
		"	beq	0b		@    then retry            "
		: "=&r" (result)
		: "r" (_oldval), "r" (_v), "r" (_newval)
		: "r3", "ip", "lr", "cc", "memory" );

	return !result;
      } 

      /* Linux kernel sources say c-a-s "includes memory barriers as needed" */
      #define GASNETI_ATOMIC_FENCE_RMW (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)
    #else
      #error "unrecognized ARM compiler and/or OS - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)"
    #endif
  /* ------------------------------------------------------------------------------------ */
  #else
    #error Unrecognized platform - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Configure non-default features of generic atomics IFF required for the current platform. */

/*
 * Example for a CPU w/o native atomics and only 2-byte aligment for uint32_t and uint64_t:
#if PLATFORM_JUST_AN_EXAMPLE
  #define gasneti_genatomic32_align 2
  #define gasneti_genatomic64_align 2
#endif
 */

/* ------------------------------------------------------------------------------------ */
/* Request build of generic atomics IFF required for the current platform */

#ifndef GASNETI_HAVE_ATOMIC32_T
  #define GASNETI_USE_GENERIC_ATOMIC32		1	/* Use generics for gasneti_atomic32_t */
#endif
#ifndef GASNETI_HAVE_ATOMIC64_T
  #define GASNETI_USE_GENERIC_ATOMIC64		1	/* Use generics for gasneti_atomic64_t */
#endif
/* Not for use outside this file: */
#undef GASNETI_HAVE_ATOMIC32_T
#undef GASNETI_HAVE_ATOMIC64_T

#if defined(GASNETI_USE_GENERIC_ATOMIC32)
  #define GASNETI_BUILD_GENERIC_ATOMIC32	1	/* Build the 32-bit generics */
  #define gasneti_weakatomic32_align		gasneti_genatomic32_align
#endif
#if defined(GASNETI_USE_GENERIC_ATOMIC64) || defined(GASNETI_HYBRID_ATOMIC64)
  #define GASNETI_BUILD_GENERIC_ATOMIC64	1	/* Build the 64-bit generics */
  #define gasneti_weakatomic64_align		gasneti_genatomic64_align
#endif

/* ------------------------------------------------------------------------------------ */
/* Define configuration-dependent choice of locks for generic atomics (if any) */

#if defined(GASNETI_BUILD_GENERIC_ATOMIC32) || defined(GASNETI_BUILD_GENERIC_ATOMIC64)
  #if defined(_INCLUDED_GASNET_H) && GASNETI_USE_TRUE_MUTEXES
    /* Case I: Real HSLs in a gasnet client */
    #define GASNETI_GENATOMIC_LOCK_PREP(ptr) \
		gasnet_hsl_t * const lock = gasneti_hsl_atomic_hash_lookup((uintptr_t)ptr)
    #define GASNETI_GENATOMIC_LOCK()   gasnet_hsl_lock(lock)
    #define GASNETI_GENATOMIC_UNLOCK() gasnet_hsl_unlock(lock)

    /* Name shift to avoid link conflicts between hsl and pthread versions */
    #define gasneti_genatomic32_set                gasneti_hsl_atomic32_set
    #define gasneti_genatomic32_increment          gasneti_hsl_atomic32_increment
    #define gasneti_genatomic32_decrement          gasneti_hsl_atomic32_decrement
    #define gasneti_genatomic32_decrement_and_test gasneti_hsl_atomic32_decrement_and_test
    #define gasneti_genatomic32_compare_and_swap   gasneti_hsl_atomic32_compare_and_swap
    #define gasneti_genatomic32_addfetch           gasneti_hsl_atomic32_addfetch
    #define gasneti_genatomic64_set                gasneti_hsl_atomic64_set
    #define gasneti_genatomic64_increment          gasneti_hsl_atomic64_increment
    #define gasneti_genatomic64_decrement          gasneti_hsl_atomic64_decrement
    #define gasneti_genatomic64_decrement_and_test gasneti_hsl_atomic64_decrement_and_test
    #define gasneti_genatomic64_compare_and_swap   gasneti_hsl_atomic64_compare_and_swap
    #define gasneti_genatomic64_addfetch           gasneti_hsl_atomic64_addfetch
    #if PLATFORM_ARCH_32 || defined(GASNETI_HYBRID_ATOMIC64) || defined(GASNETI_UNALIGNED_ATOMIC64)
      /* Need mutex on 64-bit read() to avoid word tearing */
      /* NOTE: defining gasneti_genatomic_read triggers matching behavior in gasnet_atomicops.h */
      #define gasneti_genatomic64_read             gasneti_hsl_atomic64_read
    #endif
  #elif defined(_INCLUDED_GASNET_H)
    /* Case II: Empty HSLs in a GASNET_SEQ or GASNET_PARSYNC client w/o conduit-internal threads */
  #elif GASNETI_USE_TRUE_MUTEXES /* thread-safe tools-only client OR forced true mutexes */
    /* Case III: a version for pthreads which is independent of GASNet HSL's */
    #define GASNETI_GENATOMIC_LOCK_PREP(ptr) \
		gasnett_mutex_t * const lock = gasneti_pthread_atomic_hash_lookup((uintptr_t)ptr)
    #define GASNETI_GENATOMIC_LOCK()   gasnett_mutex_lock(lock)
    #define GASNETI_GENATOMIC_UNLOCK() gasnett_mutex_unlock(lock)

    /* Name shift to avoid link conflicts between hsl and pthread versions */
    #define gasneti_genatomic32_set                gasneti_pthread_atomic32_set
    #define gasneti_genatomic32_increment          gasneti_pthread_atomic32_increment
    #define gasneti_genatomic32_decrement          gasneti_pthread_atomic32_decrement
    #define gasneti_genatomic32_decrement_and_test gasneti_pthread_atomic32_decrement_and_test
    #define gasneti_genatomic32_compare_and_swap   gasneti_pthread_atomic32_compare_and_swap
    #define gasneti_genatomic32_addfetch           gasneti_pthread_atomic32_addfetch
    #define gasneti_genatomic64_set                gasneti_pthread_atomic64_set
    #define gasneti_genatomic64_increment          gasneti_pthread_atomic64_increment
    #define gasneti_genatomic64_decrement          gasneti_pthread_atomic64_decrement
    #define gasneti_genatomic64_decrement_and_test gasneti_pthread_atomic64_decrement_and_test
    #define gasneti_genatomic64_compare_and_swap   gasneti_pthread_atomic64_compare_and_swap
    #define gasneti_genatomic64_addfetch           gasneti_pthread_atomic64_addfetch
    #if PLATFORM_ARCH_32 || defined(GASNETI_HYBRID_ATOMIC64) || defined(GASNETI_UNALIGNED_ATOMIC64)
      /* Need mutex on 64-bit read() to avoid word tearing */
      /* NOTE: defining gasneti_genatomic_read triggers matching behavior in gasnet_atomicops.h */
      #define gasneti_genatomic64_read             gasneti_pthread_atomic64_read
    #endif
  #else
    /* Case IV: Serial gasnet tools client. */
    /* attempt to generate a compile error if pthreads actually are in use */
    #define PTHREAD_MUTEX_INITIALIZER ERROR_include_pthread_h_before_gasnet_tools_h
    extern int pthread_mutex_lock; 
  #endif

  #ifndef gasneti_genatomic32_align
    #define gasneti_genatomic32_align 4
  #endif
  #if defined(gasneti_genatomic64_align)
    /* Keep the current value */
  #elif defined(GASNETI_UNALIGNED_ATOMIC64)
    #define gasneti_genatomic64_align GASNETI_UNALIGNED_ATOMIC64
  #elif PLATFORM_ARCH_32 || defined(GASNETI_HYBRID_ATOMIC64)
    #define gasneti_genatomic64_align 4
  #else
    #define gasneti_genatomic64_align 8
  #endif

  #ifdef GASNETI_GENATOMIC_LOCK
    /* To avoid a header dependence cycle (bug 693), we must declare and define all
     * bits involving the lock type later.  Here, we just build the templates.
     */
    #if PLATFORM_ARCH_64
      #define GASNETI_ATOMIC_LOCK_HASH_64(val)    val ^= (val >> 32)
    #else
      #define GASNETI_ATOMIC_LOCK_HASH_64(val)
    #endif
    #define GASNETI_ATOMIC_LOCK_TBL_DECLS(stem,type)                      \
        typedef struct {                                                  \
          type##t lock;                                                   \
          char _pad[GASNETI_CACHE_PAD(sizeof(type##t))];                  \
        } stem##tbl_t;                                                    \
        extern stem##tbl_t *stem##tbl;                                    \
        extern uintptr_t stem##tbl_mask;                                  \
        extern void stem##tbl_init(void);                                 \
        GASNETI_INLINE(stem##hash_lookup) GASNETI_CONST                   \
        type##t * stem##hash_lookup(uintptr_t val) {                      \
          /* Step 0. Initialization check */                              \
          if_pf (!stem##tbl_mask) stem##tbl_init();                       \
          else gasneti_local_rmb();                                       \
          /* Step 1.  Mask out the bits within a single cache line */     \
          val &= ~(((uintptr_t)1 << GASNETI_CACHE_LINE_SHIFT) - 1);       \
          /* Step 2. Fold with xor so all bits influence the lowest 8 */  \
          GASNETI_ATOMIC_LOCK_HASH_64(val);                               \
          val ^= (val >> 16);                                             \
          val ^= (val >> 8);                                              \
          /* Step 3. Return the index */                                  \
          return &((stem##tbl[val & stem##tbl_mask]).lock);               \
        }                                                                 \
        GASNETI_CONSTP(stem##hash_lookup)
    #define GASNETI_ATOMIC_LOCK_TBL_DEFNS(stem,type)                      \
        uintptr_t stem##tbl_mask = 0;                                     \
        stem##tbl_t *stem##tbl = NULL;                                    \
        GASNETI_NEVER_INLINE(stem##tbl_init,                              \
                             extern void stem##tbl_init(void)) {          \
          static type##t stem##tbl_lock = _gasneti_atomic_lock_initializer; \
          type##lock(&stem##tbl_lock);                                    \
          if (stem##tbl_mask == 0) {                                      \
            int i;                                                        \
            int stem##tbl_size = gasnett_getenv_int_withdefault(          \
                                         "GASNET_ATOMIC_TABLESZ", 256,0); \
            gasneti_assert_always(GASNETI_POWEROFTWO(stem##tbl_size));    \
            /* Over allocate to leave at least a cache line before and after */ \
            stem##tbl = _gasneti_atomic_lock_malloc((2 + stem##tbl_size)  \
                                                    * sizeof(stem##tbl_t)); \
            ++stem##tbl;                                                  \
            for (i = 0; i < stem##tbl_size; ++i) {                        \
              _gasneti_atomic_lock_init(&(stem##tbl[i].lock));            \
            }                                                             \
            gasneti_local_wmb(); /* enforce locks init before mask is written */ \
            stem##tbl_mask = (stem##tbl_size - 1);                        \
          }                                                               \
          type##unlock(&stem##tbl_lock);                                  \
        }
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Wrappers for "special" atomics, if any */

#ifdef GASNETI_ATOMIC32_READ_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_read);
  #define _gasneti_atomic32_read (*(uint32_t (*)(gasneti_atomic32_t *p))(&_gasneti_special_atomic32_read))
#endif
#ifdef GASNETI_ATOMIC32_SET_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_set);
  #define _gasneti_atomic32_set (*(void (*)(gasneti_atomic32_t *p, uint32_t))(&_gasneti_special_atomic32_set))
#endif
#ifdef GASNETI_ATOMIC32_INCREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_increment);
  #define _gasneti_atomic32_increment (*(void (*)(gasneti_atomic32_t *p))(&_gasneti_special_atomic32_increment))
#endif
#ifdef GASNETI_ATOMIC32_DECREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_decrement);
  #define _gasneti_atomic32_decrement (*(void (*)(gasneti_atomic32_t *p))(&_gasneti_special_atomic32_decrement))
#endif
#ifdef GASNETI_ATOMIC32_DECREMENT_AND_TEST_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_decrement_and_test);
  #define _gasneti_atomic32_decrement_and_test (*(int (*)(gasneti_atomic32_t *p))(&_gasneti_special_atomic32_decrement_and_test))
#endif
#ifdef GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_compare_and_swap);
  #define _gasneti_atomic32_compare_and_swap (*(int (*)(gasneti_atomic32_t *, uint32_t, uint32_t))(&_gasneti_special_atomic32_compare_and_swap))
#endif
#ifdef GASNETI_ATOMIC32_ADD_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_add);
  #define _gasneti_atomic32_add (*(uint32_t (*)(gasneti_atomic32_t *, uint32_t))(&_gasneti_special_atomic32_add))
#endif
#ifdef GASNETI_ATOMIC32_SUBTRACT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_subtract);
  #define _gasneti_atomic32_subtract (*(uint32_t (*)(gasneti_atomic32_t *, uint32_t))(&_gasneti_special_atomic32_subtract))
#endif
#ifdef GASNETI_ATOMIC32_FETCHADD_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_fetchadd);
  #define _gasneti_atomic32_fetchadd (*(uint32_t (*)(gasneti_atomic32_t *, uint32_t))(&_gasneti_special_atomic32_fetchadd))
#endif
#ifdef GASNETI_ATOMIC32_ADDFETCH_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_addfetch);
  #define _gasneti_atomic32_addfetch (*(uint32_t (*)(gasneti_atomic_t *, uint32_t))(&_gasneti_special_atomic_addfetch))
#endif

#ifdef GASNETI_ATOMIC64_READ_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_read);
  #define _gasneti_atomic64_read (*(uint64_t (*)(gasneti_atomic64_t *p))(&_gasneti_special_atomic64_read))
#endif
#ifdef GASNETI_ATOMIC64_SET_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_set);
  #define _gasneti_atomic64_set (*(void (*)(gasneti_atomic64_t *p, uint64_t))(&_gasneti_special_atomic64_set))
#endif
#ifdef GASNETI_ATOMIC64_INCREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_increment);
  #define _gasneti_atomic64_increment (*(void (*)(gasneti_atomic64_t *p))(&_gasneti_special_atomic64_increment))
#endif
#ifdef GASNETI_ATOMIC64_DECREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_decrement);
  #define _gasneti_atomic64_decrement (*(void (*)(gasneti_atomic64_t *p))(&_gasneti_special_atomic64_decrement))
#endif
#ifdef GASNETI_ATOMIC64_DECREMENT_AND_TEST_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_decrement_and_test);
  #define _gasneti_atomic64_decrement_and_test (*(int (*)(gasneti_atomic64_t *p))(&_gasneti_special_atomic64_decrement_and_test))
#endif
#ifdef GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_compare_and_swap);
  #define _gasneti_atomic64_compare_and_swap (*(int (*)(gasneti_atomic64_t *, uint64_t, uint64_t))(&_gasneti_special_atomic64_compare_and_swap))
#endif
#ifdef GASNETI_ATOMIC64_ADD_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_add);
  #define _gasneti_atomic64_add (*(uint64_t (*)(gasneti_atomic64_t *, uint64_t))(&_gasneti_special_atomic64_add))
#endif
#ifdef GASNETI_ATOMIC64_SUBTRACT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_subtract);
  #define _gasneti_atomic64_subtract (*(uint64_t (*)(gasneti_atomic64_t *, uint64_t))(&_gasneti_special_atomic64_subtract))
#endif
#ifdef GASNETI_ATOMIC64_FETCHADD_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_fetchadd);
  #define _gasneti_atomic64_fetchadd (*(uint64_t (*)(gasneti_atomic64_t *, uint64_t))(&_gasneti_special_atomic64_fetchadd))
#endif
#ifdef GASNETI_ATOMIC64_ADDFETCH_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_addfetch);
  #define _gasneti_atomic64_addfetch (*(uint64_t (*)(gasneti_atomic_t *, uint64_t))(&_gasneti_special_atomic_addfetch))
#endif

#ifdef GASNETI_ATOMIC_READ_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_read);
  #define _gasneti_atomic_read (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *p))(&_gasneti_special_atomic_read))
#endif
#ifdef GASNETI_ATOMIC_SET_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_set);
  #define _gasneti_atomic_set (*(void (*)(gasneti_atomic_t *p, gasneti_atomic_val_t))(&_gasneti_special_atomic_increment))
#endif
#ifdef GASNETI_ATOMIC_INCREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_increment);
  #define _gasneti_atomic_increment (*(void (*)(gasneti_atomic_t *p))(&_gasneti_special_atomic_increment))
#endif
#ifdef GASNETI_ATOMIC_DECREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_decrement);
  #define _gasneti_atomic_decrement (*(void (*)(gasneti_atomic_t *p))(&_gasneti_special_atomic_decrement))
#endif
#ifdef GASNETI_ATOMIC_DECREMENT_AND_TEST_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_decrement_and_test);
  #define _gasneti_atomic_decrement_and_test (*(int (*)(gasneti_atomic_t *p))(&_gasneti_special_atomic_decrement_and_test))
#endif
#ifdef GASNETI_ATOMIC_COMPARE_AND_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_compare_and_swap);
  #define _gasneti_atomic_compare_and_swap (*(int (*)(gasneti_atomic_t *, gasneti_atomic_val_t, gasneti_atomic_val_t))(&_gasneti_special_atomic_compare_and_swap))
#endif
#ifdef GASNETI_ATOMIC_ADD_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_add);
  #define _gasneti_atomic_add (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *, gasneti_atomic_val_t))(&_gasneti_special_atomic_add))
#endif
#ifdef GASNETI_ATOMIC_SUBTRACT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_subtract);
  #define _gasneti_atomic_subtract (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *, gasneti_atomic_val_t))(&_gasneti_special_atomic_subtract))
#endif
#ifdef GASNETI_ATOMIC_FETCHADD_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_fetchadd);
  #define _gasneti_atomic_fetchadd (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *, gasneti_atomic_sval_t))(&_gasneti_special_atomic_fetchadd))
#endif
#ifdef GASNETI_ATOMIC_ADDFETCH_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_addfetch);
  #define _gasneti_atomic_addfetch (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *, gasneti_atomic_sval_t))(&_gasneti_special_atomic_addfetch))
#endif

/* ------------------------------------------------------------------------------------ */

#ifndef gasneti_atomic32_align
  #define gasneti_atomic32_align 4
#endif
#ifndef gasneti_atomic64_align
  #define gasneti_atomic64_align 8
#endif

/* ------------------------------------------------------------------------------------ */
#endif
