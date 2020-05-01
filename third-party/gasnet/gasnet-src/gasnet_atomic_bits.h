/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_atomic_bits.h $
 * Description: GASNet header for platform-specific parts of atomic operations
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNETEX_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_ATOMIC_BITS_H
#define _GASNET_ATOMIC_BITS_H

// NOTE:
// This header provides only a portion of the atomics implementation.
// It is CRUCIAL that any changes/additions to support for any cpu/os/compiler
// combination also be reflected in gasnet_atomic_fwd.h

/* ------------------------------------------------------------------------------------ */
/* Atomics HOW TO:

   This file implements the platform-specific aspects of GASNet's atomic
   operations, where "platform" is defined as (at least) OS, ABI and compiler.

   The GASNet atomics provide a set of operations on 32-bit and 64-bit types,
   which are implemented using mutexes if a platform cannot implement all of the
   required operations "natively".  If that is the case, then a "private" atomic
   type can be defined which may have either a reduced set of operations, a size
   other than 4 or 8 bytes, or both.

   If a platform can implement the 32- and/or 64-bit operations (enumerated
   below) then it must provide the following items, where "??" is to be
   replaced by "32" or "64".

   + #define GASNETI_HAVE_ATOMIC??_T 1
     This indicates that the given type is implemented natively, suppressing the
     generation of the corresponding mutex-based version.

   + typedef ... gasneti_atomic??_t;
     This is most often implemented as
         struct { volatile uint??_t gasneti_ctr; }
     Use of a struct prevent client code from accidentally treating it as a
     scalar, while the volatile qualifier helps ensure correct code in the
     implementation.

   + #define gasneti_atomic??_init(v) ...
     Static initializer for gasneti_atomic??_t.

   + One must #define EITHER gasneti_atomic??_[OP] or _gasneti_atomic??_[OP],
     for each OP in {set, read, compare_and_swap, swap}.

     The first (no "_" prefix) form takes a "const int flags" argument as
     described in README-tools and must implement the fences it requests.  In
     the case of implementations of the atomic operations which include both a
     compiler fence and full memory barrier, one can simply ignore the argument.
     At present there are no platforms using this non-prefixed option EXCEPT
     for fully-fenced ones which ignore the flags argument.

     The form with the "_" prefix does NOT take a "flags" argument and the
     atomics subsystem automatically implements the fences to produce the
     non-prefixed version.  Because of the way these are used, it is safe to
     define these as macros which evaluate their arguments multiple times
     because the arguments will never have side-effects.

     A #define must be used to allow the rest of the atomics subsystem to know
     which is available.  However, the OP itself is often implemented as an
     inline function and its name is #define'd to itself.

   + The arithmetic operations {increment, decrement, add, subtract} can be
     provided either explicitly or implicitly.  The choice between explicit or
     implicit definition is independent - one may mix explicit and implicit
     definitions of these four operations freely.

     To provide the operations explicitly one #define's them, with or without a
     "_" prefix as above.  If neither form is defined, the atomics subsystem
     will construct them automatically provided one #defines any ONE of the
     following:
           uint??_t gasneti_atomic??_fetchadd(ptr, op, flags)
           uint??_t gasneti_atomic??_addfetch(ptr, op, flags)
           uint??_t _gasneti_atomic??_fetchadd(ptr, op)
           uint??_t _gasneti_atomic??_addfetch(ptr, op)
     A "fetchadd" returns the value before addition, while a "addfetch" returns
     the value after.  Note that the "op" has an unsigned type (uint??_t) rather
     than signed (int??_t) as in the add and subtract operations.  This does not
     impact the results since we perform 2s-complement arithmetic on the 32- and
     64-bit types.

   + The decrement_and_test operation can either be #define'd explicitly (with
     or without a "_" prefix) OR it can be constructed automatically from either
     a "_"-prefixed fetchadd or addfetch.  However, it will NOT be constructed
     automatically from the corresponding non-prefixed (fenced) operations
     because there is currently no suitable mechanism to reconcile the fencing.

   + Fences:
     If you define any of the operations above with the "_" prefix, then the
     non-prefixed version (w/ flags argument) will be constructed automatically,
     and this construction will implement the fences requested by this flags
     argument.  By default this construction assumes that there are no fencing
     side-effects (compiler fence or memory barriers) in the "_"-prefixed
     operations.  When that is NOT the case, one can override this default
     behavior by defining the appropriate fencing macros.  At present this is
     done only for the x86/x86-64 and IA64.

     In the case of the x86/x86-64 all of the read-modify-write operations
     include a full memory barrier but do NOT include a compiler fence.  So
     the following definitions are used to replace the defaults:
        #define _gasneti_atomic??_prologue_rmw(p,f)
        #define _gasneti_atomic??_fence_before_rmw(p,f)   _gasneti_atomic_cf_before(f)
        #define _gasneti_atomic??_fence_after_rmw(p,f)    _gasneti_atomic_cf_after(f)
        #define _gasneti_atomic??_fence_after_bool(p,f,v) _gasneti_atomic_cf_after(f)
     where "cf" stands for compiler fence and _gasneti_atomic_cf_before() and
     ...after are defined in gasneti_atomicops.h along with several other macros
     used to construct the default fences.

   + Native atomics:
     The term "native atomics" is used to refer to the implementation of
     atomic operations via inline assembly.

   + Compiler atomics and OS atomics:
     These terms are used to refer to the implementation of atomic operations by
     a third-party such as a compiler or a system header.

   + Generic atomics:
     The term "generic atomics" is used here and in gasnet_atomicops.h to refer
     to the implementation of atomic operations via mutexes.  These will be
     pthread mutexes in GASNet-tools code, and HSLs in a GASNet client.

   + Hybrid 64-bit atomics:
     With an ABI for which the alignment of 64-bit types is NOT sufficient to
     allow free use of the CPU's native atomic operations, we have a "hybrid"
     mechanism in which a run-time branch selects between native atomics for the
     properly-aligned case and generic atomics for unaligned values.  A platform
     requiring the hybrid implementation need only
        #define GASNETI_HYBRID_ATOMIC64 1
     and provide exactly the operations expected by the corresponding code in
     gasnet_atomicops.h (search for GASNETI_HYBRID_ATOMIC64).

   + Special atomics:
     The term "special atomics" describes the case of native atomics for C
     compilers which require use of "out-of-line" asm support (receiving args
     and returning a value using the ABI-defined function calling convention).
     In this case the implementation defines the necessary operations through
     any combination of macros, inline functions and "special" functions.
     Special functions are defined via GASNETI_ATOMIC{32,64}_[OPNAME]_BODY
     macros, each providing the assembly to be expanded as the body of a
     library function (and triggering generation of the necessary wrappers).
     The macros GASNETI_ATOMIC{32,64}_SPECIALS collect the BODY macros to be
     expanded in gasnet_tools.c.

   + Slow atomics:
     The term "slow atomics" is for when the C compiler which built the gasnet
     library used an implementation which the current compiler cannot be relied
     upon to also generate.  This can occur (1) when the C++ compiler lacks the
     same asm support as the C compiler, or (2) when the compiler used at client
     compile time does not match the one used to build the library.  All that is
     required to use slow atomics is to define GASNETI_USING_SLOW_ATOMIC{32,64}
     where one would otherwise define the atomic functions.  These cause atomic
     operations to resolve function calls to the library.
     Note: The CC probed at configure time cannot use slow atomics.
     Note: The generic and slow atomics are mutually exclusive for a
           given bit-width.

   + Differing implementations of 32-bit and 64-bit atomics:
     There are assumptions in how atomicsops are constructed for compilers other
     than the configure-time CC which limit which 64-bit atomics implementations
     can be mixed with a given 32-bit implementation.  The main limitation is
     that the 64-bit atomics are never "better" than the 32-bit.  The
     assumptions are listed (and enforced) at the end of gasnet_atomic_fwd.h.


   TODO: Fully document the macros used to override the default fences.
         For now, the code for x86/x86_64 is the only decent example.

   TODO: Document definition of a "private" atomic type.
         There are no surviving examples, however the SPARC7 and
         PA-RISC code (removed after GASNet-1.22.0) were good examples.

   SEE ALSO: https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=1607
 */

/* ------------------------------------------------------------------------------------ */
/* Determine which atomic implementations are appropriate to the current compiler */

#include "gasnet_atomic_fwd.h"

/* ------------------------------------------------------------------------------------ */
/* Work-arounds and special cases for various platforms */

/* Cannot always use "register" (bug 3528): */
#ifdef GASNETI_ASM_REGISTER_KEYWORD
  /* Preserve any override */
#elif PLATFORM_ARCH_ARM && defined(GASNETI_HAVE_ARM_CMPXCHG) && \
      PLATFORM_OS_LINUX && (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_CLANG)
   /* This target *must* use the "register" keyword.
    * Leaving GASNETI_ASM_REGISTER_KEYWORD undefined will catch any mis-guided use.
    */
#elif (__cplusplus >= 201103L)
  /* C++11 deprecated the "register" keyword, and C++17 makes it a reserved word. */
  #define GASNETI_ASM_REGISTER_KEYWORD /* empty */
#else
  #define GASNETI_ASM_REGISTER_KEYWORD register
#endif

#if PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_X86 || PLATFORM_ARCH_MIC
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
     * + open64: mimics gcc, but is able to schedule %ebx so no work-around is needed
     * + llvm-gcc: mimics gcc, but is able to schedule %ebx so no work-around is needed
     * + Sun cc: use of specials doesn't encounter the problem
     *
     * Bottom line is that we recommend YOUR_PIC_CFLAGS="-fPIC -DGASNETI_FORCE_PIC",
     * replacing "-fPIC" with your compiler-specific flag(s) as needed.
     */
  #if ((PLATFORM_COMPILER_GNU && !defined(__llvm__)) || \
       PLATFORM_COMPILER_PATHSCALE || PLATFORM_COMPILER_PGI) && \
       ((defined(__PIC__) && !defined(GASNETI_CONFIGURED_PIC)) || defined(GASNETI_FORCE_PIC))
      /* Disable use of %ebx when building PIC, but only on affected compilers. */
      #define GASNETI_USE_X86_EBX 0
  #endif
  /* By default use the configure-probed result */
  #ifndef GASNETI_USE_X86_EBX
    #define GASNETI_USE_X86_EBX GASNETI_HAVE_X86_EBX
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* ABI properties, independent of implementation */

// Exceptional cases
#if PLATFORM_ARCH_X86
  #define gasneti_atomic64_align 4
#endif

// Defaults
#ifndef gasneti_atomic32_align
  #define gasneti_atomic32_align 4
#endif
#ifndef gasneti_atomic64_align
  #define gasneti_atomic64_align 8
#endif

/* ------------------------------------------------------------------------------------ */
/* Helpers for "special" call-based atomics on platforms w/ crippled inline asm support. */

#define GASNETI_SPECIAL_ASM_DECL(name) \
	extern void name(void)
#define GASNETI_SPECIAL_ASM_DEFN(name, body) \
	GASNETI_NEVER_INLINE(name, extern void name(void)) { body; }

/* ------------------------------------------------------------------------------------ */
// Logic to handle compilers other than the configured CC, including:
//  + CXX, if any
//  + MPI_CC, if any and it differs from CC
//  + compilers not probed by configure
// The first two cases (CXX and MPI_CC) differ from the last in the use of
// configure probed information which may be more precise than what can be
// determined from the preprocess environment alone.
//
// NOTE: probably incomplete with respect to a "private" atomic type

#if !GASNETI_COMPILER_IS_CC
  // The current compiler is not the C compiler used to build GASNet libraries.
  // So, we must decide between allowing the compiler to generate either the
  // actual atomicops or library calls.

  // Notes on COMPATIBLE implementations
  //
  // Two implementations are considered "compatible" if they interoperate
  // (linking and producing correct results).  The logic below determines if
  // the current compiler would normally use implementations of the 32- and
  // 64-bit atomic operations that are compatible with those in the GASNet
  // libraries.  If not, then later logic makes the necessary definitions to
  // ensure force use of compatible implementations.  In some cases this
  // means use of "SLOW" atomics - calling GASNet library functions.
  //
  // GENERIC, OS and HYBRID implementations are each compatible only with
  // themselves.  However, each is compatible across all supported compiler
  // families.
  //
  // NATIVE and SYNC implementations are currently all mutually compatible.
  // This is true because we currently only enable __sync atomics which we
  // believe to be compatible with the native inline asm.  In particular we
  // assume compilers do not use a mutex-based implementation which might not
  // be compatible with native atomics or between distinct compiler families.
  // If these assumptions are ever broken, then additional logic will be
  // needed to sort out which pairs are compatible.
  //
  // The compatibility of SPECIAL implementations is asymmetric.  A library
  // built with SPECIAL atomics is compatible with any compiler's use of SYNC
  // or NATIVE atomics implementations for the same reasons those two can be
  // mixed.  However, a SPECIAL implementation uses compiler-specific support
  // routines in the library.  Therefore a compiler's SPECIAL implementation
  // is compatible only with a library built with SPECIAL by the same compiler
  // family (all other cases will lack the appropriate support routines).

  #if (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_SPECIAL)
    #if (GASNETI_ATOMIC32_IMPL == GASNETI_ATOMIC_IMPL_SPECIAL) && \
        (PLATFORM_COMPILER_FAMILYID == GASNETI_PLATFORM_COMPILER_FAMILYID)
      // SPECIAL is self-COMPATIBLE only within a compiler family
    #elif (GASNETI_ATOMIC32_IMPL == GASNETI_ATOMIC_IMPL_NATIVE) || \
          (GASNETI_ATOMIC32_IMPL == GASNETI_ATOMIC_IMPL_SYNC)
      // SPECIAL library is assumed COMPATIBLE with NATIVE and SYNC
      // Exceptions should be added here
    #else
      #define GASNETI_ATOMIC32_WANT_SLOW 1
    #endif
  #elif (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_GENERIC)
    #define GASNETI_ATOMIC32_WANT_GENERIC 1
  #elif (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_OS)
    #define GASNETI_ATOMIC32_WANT_OS 1
  #elif (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_NATIVE) || \
        (GASNETI_ATOMIC32_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_SYNC)
    #if (GASNETI_ATOMIC32_IMPL == GASNETI_ATOMIC_IMPL_NATIVE) || \
        (GASNETI_ATOMIC32_IMPL == GASNETI_ATOMIC_IMPL_SYNC)
      // All NATIVE and SYNC are assumed to be COMPATIBLE (see note above)
      // Exceptions should be added here
    #else
      #define GASNETI_ATOMIC32_WANT_SLOW 1
    #endif
  #else
    // NOTE 32-bit atomics are never "hybrid"
    #error Internal error - unexpected atomics configuration
  #endif

  #if (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_SPECIAL)
    #if (GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_SPECIAL) && \
        (PLATFORM_COMPILER_FAMILYID == GASNETI_PLATFORM_COMPILER_FAMILYID)
      // SPECIAL is self-COMPATIBLE only within a compiler family
    #elif (GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_NATIVE) || \
          (GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_SYNC)
      // SPECIAL library is assumed COMPATIBLE with NATIVE and SYNC
      // Exceptions should be added here
    #else
      #define GASNETI_ATOMIC64_WANT_SLOW 1
    #endif
  #elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_GENERIC)
    #define GASNETI_ATOMIC64_WANT_GENERIC 1
  #elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_OS)
    #define GASNETI_ATOMIC64_WANT_OS 1
  #elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_NATIVE) || \
        (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_SYNC)
    #if (GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_NATIVE) || \
        (GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_SYNC)
      // All NATIVE and SYNC are assumed to be COMPATIBLE (see note above)
      // Exceptions should be added here
    #else
      #define GASNETI_ATOMIC64_WANT_SLOW 1
    #endif
  #elif (GASNETI_ATOMIC64_IMPL_CONFIGURE == GASNETI_ATOMIC_IMPL_HYBRID)
    // Note that hybrid is not "compatible" with anything but itself
    #define GASNETI_ATOMIC64_WANT_SLOW 1
  #else
    #error Internal error - unexpected atomics configuration
  #endif

  // As checked by the logic at the end of gasnet_atomic_fwd.h, the
  // implementation of 32-bit atomics strongly limits which implementations
  // may be used for 64-bit atomics.  As a result, the disposition of 32-bit
  // atomics is the sole determining factor in how the main implementation
  // ladder will be traversed.
  #if GASNETI_ATOMIC32_WANT_GENERIC
    #define GASNETI_USE_GENERIC_ATOMICOPS 1
  #elif GASNETI_ATOMIC32_WANT_OS
    #define GASNETI_USE_OS_ATOMICOPS 1
  #elif GASNETI_ATOMIC32_WANT_SLOW
    #define GASNETI_USING_SLOW_ATOMIC32 1
    #define GASNETI_ATOMIC32_NOINLINE 1 // Suppress OS, SYNC and NATIVE
    #define GASNETI_HAVE_ATOMIC32_T   1 // Suppress GENERIC
  #endif

  // Disposition of 64-bit atomics must suppress cases which could be
  // reachable in the main ladder, but are undesired.
  #if GASNETI_ATOMIC64_WANT_GENERIC
    #define GASNETI_ATOMIC64_NOINLINE 1 // Suppress OS, SYNC, NATIVE, HYBRID
  #elif GASNETI_ATOMIC64_WANT_SLOW
    #define GASNETI_USING_SLOW_ATOMIC64 1
    #define GASNETI_ATOMIC64_NOINLINE 1 // Suppress OS, SYNC, NATIVE, HYBRID
    #define GASNETI_HAVE_ATOMIC64_T 1   // Suppress GENERIC
  #endif

  // Check for cases in which desired 64-bit atomics are unreachable.
  // Any #error here which does not first trip an #error at the end of
  // gasnet_atomic_fwd.h indicates a flaw in the logic above.
  #if (GASNETI_ATOMIC32_NOINLINE && !GASNETI_ATOMIC64_NOINLINE) || \
      (GASNETI_ATOMIC64_WANT_OS && !GASNETI_USE_OS_ATOMICOPS)
    #error Internal error - unexpected atomics configuration
  #endif

  // For local use only
  #undef GASNETI_ATOMIC32_WANT_GENERIC
  #undef GASNETI_ATOMIC64_WANT_GENERIC
  #undef GASNETI_ATOMIC32_WANT_OS
  #undef GASNETI_ATOMIC64_WANT_OS
  #undef GASNETI_ATOMIC32_WANT_SLOW
  #undef GASNETI_ATOMIC64_WANT_SLOW
#endif

/* ------------------------------------------------------------------------------------ */

#if defined(GASNETI_ATOMIC32_NOINLINE)
  /* Logic above has determined current compiler cannot safely use these implementations. */
  /* This case exists only to prevent the following cases from matching. */
#elif defined(GASNETI_USE_GENERIC_ATOMICOPS)
  /* Use a very slow but portable implementation of atomic ops using mutexes */
  /* This case exists only to prevent the following cases from matching. */
#elif defined(GASNETI_USE_COMPILER_ATOMICOPS)
  #if GASNETI_HAVE_SYNC_ATOMICS_32
    /* Generic implementation in terms of GCC's __sync atomics */

    #if 0 // Update if/when using 32-bit __sync atomic where they are not signal-safe
      #define GASNETI_ATOMIC32_NOT_SIGNALSAFE 1
    #endif

    /* GCC documentation promises a full memory barrier */
    #define _gasneti_atomic32_prologue_rmw(p,f)         /*empty*/
    #define _gasneti_atomic32_fence_before_rmw(p,f)     /*empty*/
    #define _gasneti_atomic32_fence_after_rmw(p,f)      /*empty*/
    #define _gasneti_atomic32_fence_after_bool(p,f,v)   /*empty*/

    #define GASNETI_HAVE_ATOMIC32_T 1
    typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
    #define gasneti_atomic32_init(v)       { (v) }
    #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
    #define _gasneti_atomic32_set(p,v)     do { (p)->gasneti_ctr = (v); } while(0)

    /* Default impls of inc, dec, add and sub */
    #define gasneti_atomic32_fetchadd(p,op,f) (__sync_fetch_and_add(&(p)->gasneti_ctr, (uint32_t)(op)))
    #define gasneti_atomic32_decrement_and_test(p,f) (1==gasneti_atomic32_fetchadd((p),-1,(f)))

    #define gasneti_atomic32_compare_and_swap(p,oval,nval,f) \
            (__sync_bool_compare_and_swap(&((p)->gasneti_ctr), (oval), (nval)))

    #if __GCC_ATOMIC_TEST_AND_SET_TRUEVAL // means test_and_set is actually a swap
      // Unlike the other __sync*(), __sync_lock_test_and_set() is documented as
      // having only an ACQ fence.  So, we *may* need to make up the difference.
      #if PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC
        // Nothing to do: SWAP must be lock-prefix instruction (or mutex cycle)
        // TODO: other ARCHs w/ this property?
        #define _gasneti_atomic_fence_before_swap(f) ((void)0)
        #define _gasneti_atomic_fence_after_swap(f)  ((void)0)
      #elif (GASNETI_RMB_IS_MB && 0)
        // TODO: disabled since GASNETI_RMB_IS_MB could reflect *our* choice
        // of RMB and not the implementation of __sync_lock_test_and_set().
        // Example: on PPC we use lwsync but compiler might use isync.
        #define _gasneti_atomic_fence_before_swap(f) _gasneti_atomic_fence_before(f)
        #define _gasneti_atomic_fence_after_swap()   ((void)0)
      #else
        #define _gasneti_atomic_fence_before_swap(f) _gasneti_atomic_fence_before(f)
        #define _gasneti_atomic_fence_after_swap(f)  _gasneti_atomic_fence_after((f) & ~GASNETI_ATOMIC_ACQ)
      #endif
   #endif

    // Two implementations of SWAP are possible:
    // 1) __sync_lock_test_and_set() is often a swap, and modern GCC will
    //    `#define __GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1` on such platforms.
    //    Since it has only an acquire fence, some additional work is required.
    // 2) Loop on (fully-fenced) __sync_val_compare_and_swap().
    GASNETI_INLINE(gasneti_atomic32_swap)
    uint32_t gasneti_atomic32_swap(gasneti_atomic32_t *_p, uint32_t _nval, const int _flags) {
        GASNETI_ASM_REGISTER_KEYWORD volatile uint32_t *_p32 = &(_p->gasneti_ctr);
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _oval;
    #if __GCC_ATOMIC_TEST_AND_SET_TRUEVAL
        _gasneti_atomic_fence_before_swap(_flags);
        _oval = __sync_lock_test_and_set(_p32,_nval);
        _gasneti_atomic_fence_after_swap(_flags);
    #else
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _tmp = *_p32;
        do {
            _oval = _tmp;
        } while (_oval != (_tmp = __sync_val_compare_and_swap(_p32,_oval,_nval)));
    #endif
        return _oval;
    }

    // TODO: use 64-bit sync atomics on ILP32 iff signal safe (otherwise probably equivalent to generics)
    #if GASNETI_ATOMIC64_NOINLINE
      // Using SLOW or GENERIC alternative
    #elif PLATFORM_ARCH_64 && GASNETI_HAVE_SYNC_ATOMICS_64
      #if 0 // Update if/when using 64-bit __sync atomic where they are not signal-safe
        #define GASNETI_ATOMIC64_NOT_SIGNALSAFE 1
      #endif

      #define _gasneti_atomic64_prologue_rmw(p,f)         /*empty*/
      #define _gasneti_atomic64_fence_before_rmw(p,f)     /*empty*/
      #define _gasneti_atomic64_fence_after_rmw(p,f)      /*empty*/
      #define _gasneti_atomic64_fence_after_bool(p,f,v)   /*empty*/

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
      #define gasneti_atomic64_init(v)       { (v) }
      #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic64_set(p,v)     do { (p)->gasneti_ctr = (v); } while(0)

      #define gasneti_atomic64_fetchadd(p,op,f) (__sync_fetch_and_add(&(p)->gasneti_ctr, (uint64_t)(op)))
      #define gasneti_atomic64_decrement_and_test(p,f) (1==gasneti_atomic64_fetchadd((p),-1,(f)))

      #define gasneti_atomic64_compare_and_swap(p,oval,nval,f) \
              (__sync_bool_compare_and_swap(&((p)->gasneti_ctr), (oval), (nval)))

      // See comments w/ gasneti_atomic64_swap
      GASNETI_INLINE(gasneti_atomic64_swap)
      uint64_t gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _nval, const int _flags) {
          GASNETI_ASM_REGISTER_KEYWORD volatile uint64_t *_p64 = &(_p->gasneti_ctr);
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _oval;
      #if __GCC_ATOMIC_TEST_AND_SET_TRUEVAL
          _gasneti_atomic_fence_before_swap(_flags);
          _oval = __sync_lock_test_and_set(_p64,_nval);
          _gasneti_atomic_fence_after_swap(_flags);
      #else
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _tmp = *_p64;
          do {
              _oval = _tmp;
          } while (_oval != (_tmp = __sync_val_compare_and_swap(_p64,_oval,_nval)));
      #endif
          return _oval;
      }
    #endif /* GASNETI_HAVE_SYNC_ATOMICS_64 */

    #undef _gasneti_atomic_fence_before_swap
    #undef _gasneti_atomic_fence_after_swap
  #else 
    #error "GASNETI_USE_COMPILER_ATOMICOPS for unknown or unsupported compiler"
  #endif
#elif defined(GASNETI_USE_OS_ATOMICOPS)
  /* ------------------------------------------------------------------------------------
   * Use OS-provided atomics, which should be CPU-independent and
   * which should work regardless of the compiler's inline assembly support.
   * ------------------------------------------------------------------------------------ */
  #if PLATFORM_OS_CYGWIN
      /* These are *NOT* Cygwin calls, but Windows API calls that may actually
       * be intrinsics in the MS compilers on 64-bit systems.
       * Intrinsics below incur full memory barriers. Win8+ offer finer-grained
       * control over fencing, but we don't currently use those. */
      #include <windows.h>

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define gasneti_atomic32_increment(p,f) InterlockedIncrement((LONG *)&((p)->gasneti_ctr))
      #define gasneti_atomic32_decrement(p,f) InterlockedDecrement((LONG *)&((p)->gasneti_ctr))
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))
      #define gasneti_atomic32_decrement_and_test(p,f) \
                                          (InterlockedDecrement((LONG *)&((p)->gasneti_ctr)) == 0)
      #define gasneti_atomic32_compare_and_swap(p,oval,nval,f) \
	   (InterlockedCompareExchange((LONG *)&((p)->gasneti_ctr),nval,oval) == (LONG)(oval))
      #define gasneti_atomic32_fetchadd(p,op,f) InterlockedExchangeAdd((LONG *)&((p)->gasneti_ctr), op)
      #define gasneti_atomic32_swap(p,op,f) InterlockedExchange((LONG *)&((p)->gasneti_ctr), op)

      #if GASNETI_ATOMIC64_NOINLINE
        // Using SLOW or GENERIC alternative
      #elif PLATFORM_ARCH_64 /* TODO: Identify ILP32 running on 64-bit CPU */
        #define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
        #define gasneti_atomic64_init(v)       { (v) }
        #define gasneti_atomic64_increment(p,f) InterlockedIncrement64((LONGLONG *)&((p)->gasneti_ctr))
        #define gasneti_atomic64_decrement(p,f) InterlockedDecrement64((LONGLONG *)&((p)->gasneti_ctr))
        #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))
        #define gasneti_atomic64_decrement_and_test(p,f) \
                                          (InterlockedDecrement64((LONGLONG *)&((p)->gasneti_ctr)) == 0)
        #define gasneti_atomic64_compare_and_swap(p,oval,nval,f) \
	     (InterlockedCompareExchange64((LONGLONG *)&((p)->gasneti_ctr),nval,oval) == (LONGLONG)(oval))
        #define gasneti_atomic64_fetchadd(p,op,f) InterlockedExchangeAdd64((LONGLONG *)&((p)->gasneti_ctr), op)
        #define gasneti_atomic64_swap(p,op,f) InterlockedExchange64((LONGLONG *)&((p)->gasneti_ctr), op)
      #endif
  /* ------------------------------------------------------------------------------------ */
  #else
    #error GASNETI_USE_OS_ATOMICOPS defined on unsupported OS - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
  #endif
#else
  /* ------------------------------------------------------------------------------------
   * Not using GENERIC (mutex), compiler-provided or OS-provided atomics, so
   * provide our own based on the CPU and compiler support for inline assembly code
   * ------------------------------------------------------------------------------------ */
  #if PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC /* x86 and Athlon64/Opteron and MIC */
    /* We have a full memory barrier in all read-modify-write operations,
     * but NOT a compiler fence. */
    #define _gasneti_atomic32_prologue_rmw(p,f)         /*empty*/
    #define _gasneti_atomic32_fence_before_rmw(p,f)     _gasneti_atomic_cf_before(f)
    #define _gasneti_atomic32_fence_after_rmw(p,f)      _gasneti_atomic_cf_after(f)
    #define _gasneti_atomic32_fence_after_bool(p,f,v)   _gasneti_atomic_cf_after(f)

    #define _gasneti_atomic64_prologue_rmw(p,f)         /*empty*/
    #define _gasneti_atomic64_fence_before_rmw(p,f)     _gasneti_atomic_cf_before(f)
    #define _gasneti_atomic64_fence_after_rmw(p,f)      _gasneti_atomic_cf_after(f)
    #define _gasneti_atomic64_fence_after_bool(p,f,v)   _gasneti_atomic_cf_after(f)

    /* The odd-ball is 64-bit read/set on ILP32, for which we have
     * fully fenced read and set.  */
    #if PLATFORM_ARCH_32
      #define _gasneti_atomic64_prologue_set(p,f)       /*empty*/
      #define _gasneti_atomic64_fence_before_set(p,f)   _gasneti_atomic_cf_before(f)
      #define _gasneti_atomic64_fence_after_set(p,f)    _gasneti_atomic_cf_after(f)
      #define _gasneti_atomic64_prologue_read(p,f)      /*empty*/
      #define _gasneti_atomic64_fence_before_read(p,f)  _gasneti_atomic_cf_before(f)
      #define _gasneti_atomic64_fence_after_read(p,f)   _gasneti_atomic_cf_after(f)
    #endif

    #if PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_INTEL || \
        PLATFORM_COMPILER_PATHSCALE || PLATFORM_COMPILER_PGI || \
        PLATFORM_COMPILER_OPEN64 || \
        PLATFORM_COMPILER_CLANG || \
        (PLATFORM_COMPILER_SUN && GASNETI_HAVE_GCC_ASM)
     #if PLATFORM_COMPILER_SUN_C
       #pragma error_messages(off, E_ASM_UNUSED_PARAM)
     #elif PLATFORM_COMPILER_SUN_CXX
       #pragma error_messages(off, inlasmpnu)
     #endif

     #define GASNETI_HAVE_ATOMIC32_T 1
     typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
     #define gasneti_atomic32_init(v)      { (v) }


      #if !(GASNETI_ATOMIC64_NOINLINE || (PLATFORM_COMPILER_OPEN64 && PLATFORM_ARCH_32))
        #define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
        #define gasneti_atomic64_init(v)      { (v) }
      #endif

      #if PLATFORM_COMPILER_PATHSCALE || PLATFORM_COMPILER_OPEN64
        /* Pathscale optimizer is buggy and fails to clobber memory output location correctly
           unless we include an extraneous full memory clobber 
         */
        #define GASNETI_ATOMIC_MEM_CLOBBER ,"memory"
      #else
        #define GASNETI_ATOMIC_MEM_CLOBBER
      #endif

      GASNETI_INLINE(_gasneti_atomic32_swap)
      uint32_t _gasneti_atomic32_swap(gasneti_atomic32_t *_v, uint32_t _value) {
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _x = _value;
        __asm__ __volatile__(
                GASNETI_X86_LOCK_PREFIX  /* 'lock' is implied, but is the fence? */
		"xchgl %0, %1"
                : "=r" (_x)
                : "m" (_v->gasneti_ctr), "0" (_x)
                : "cc", "memory" /* instead of listing (v->gasneti_ctr) as an output */ );
        return _x;
      }
      #define _gasneti_atomic32_swap _gasneti_atomic32_swap

     #if GASNETI_PGI_ASM_BUG3674
      #define _gasneti_atomic32_read(p)      (*(uint32_t volatile *)&((p)->gasneti_ctr))
      #define _gasneti_atomic32_set(p,v)     (*(uint32_t volatile *)&((p)->gasneti_ctr) = (v))
     #else
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))
     #endif

      #if PLATFORM_COMPILER_SUN
        // Sun compiler warns if any %n fails to appear in the template.
        // Suppression of the warning via #pragma is only partially effective.
        // Fortunately, "appearance" in comments is sufficient.
        #define GASNETI_ASM_USED(n)  "# arg %" #n " is used \n\t"
      #else
        #define GASNETI_ASM_USED(n)
      #endif

      GASNETI_INLINE(_gasneti_atomic32_increment)
      void _gasneti_atomic32_increment(gasneti_atomic32_t *_v) {
        __asm__ __volatile__(
                GASNETI_ASM_USED(1)
                GASNETI_X86_LOCK_PREFIX
		"incl %0"
                : "=m" (_v->gasneti_ctr)
                : "m" (_v->gasneti_ctr)
                : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
      }
      #define _gasneti_atomic32_increment _gasneti_atomic32_increment
      GASNETI_INLINE(_gasneti_atomic32_decrement)
      void _gasneti_atomic32_decrement(gasneti_atomic32_t *_v) {
        __asm__ __volatile__(
                GASNETI_ASM_USED(1)
                GASNETI_X86_LOCK_PREFIX
		"decl %0"
                : "=m" (_v->gasneti_ctr)
                : "m" (_v->gasneti_ctr)
                : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
      }
      #define _gasneti_atomic32_decrement _gasneti_atomic32_decrement
      GASNETI_INLINE(_gasneti_atomic32_decrement_and_test)
      int _gasneti_atomic32_decrement_and_test(gasneti_atomic32_t *_v) {
      #if PLATFORM_COMPILER_SUN
          unsigned char _retval;
      #else
          GASNETI_ASM_REGISTER_KEYWORD unsigned char _retval;
      #endif
          __asm__ __volatile__(
                  GASNETI_ASM_USED(2)
	          GASNETI_X86_LOCK_PREFIX
		  "decl %0		\n\t"
		  "sete %1"
	          : "=m" (_v->gasneti_ctr), "=qm" (_retval)
	          : "m" (_v->gasneti_ctr)
                  : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	#if GASNETI_PGI_ASM_BUG1754
          return _retval & 0xFF;
	#else
          return _retval;
	#endif
      }
      #define _gasneti_atomic32_decrement_and_test _gasneti_atomic32_decrement_and_test

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *_v, uint32_t _oldval, uint32_t _newval) {
      #if PLATFORM_COMPILER_SUN
        unsigned char _retval;
      #else
        GASNETI_ASM_REGISTER_KEYWORD unsigned char _retval;
      #endif
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _readval;
        __asm__ __volatile__ (
                GASNETI_ASM_USED(4)
		GASNETI_X86_LOCK_PREFIX
		"cmpxchgl %3, %1	\n\t"
	#if GASNETI_PGI_ASM_BUG2294 /* Sensitive to output constraint order */
		"sete %2"
		: "=a" (_readval), "=m" (_v->gasneti_ctr), "=qm" (_retval)
	#else /* The version that has always worked everywhere else */
		"sete %0"
		: "=qm" (_retval), "=m" (_v->gasneti_ctr), "=a" (_readval)
	#endif
		: "r" (_newval), "m" (_v->gasneti_ctr), "a" (_oldval)
		: "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	#if GASNETI_PGI_ASM_BUG1754
          return _retval & 0xFF;
	#else
          return _retval;
	#endif
      }
      #define _gasneti_atomic32_compare_and_swap _gasneti_atomic32_compare_and_swap

      GASNETI_INLINE(_gasneti_atomic32_fetchadd)
      uint32_t _gasneti_atomic32_fetchadd(gasneti_atomic32_t *_v, uint32_t _op) {
	/* CAUTION: Both PathScale and Intel compilers have been seen to be
         * rather fragile with respect to this asm template (bug 1563).
         * Change this at your own risk!
         */
	uint32_t _retval = _op;
        __asm__ __volatile__(
                GASNETI_ASM_USED(3)
                GASNETI_X86_LOCK_PREFIX
		"xaddl %0, %1"
                : "=&r" (_retval), "=m" (_v->gasneti_ctr)
                : "0" (_retval), "m" (_v->gasneti_ctr)
                : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	return _retval;
      }
      #define _gasneti_atomic32_fetchadd _gasneti_atomic32_fetchadd

      /* 64-bit differ between x86 and x86-64: */
      #if GASNETI_ATOMIC64_NOINLINE
        // Using SLOW or GENERIC alternative
      #elif PLATFORM_ARCH_64
       #if GASNETI_PGI_ASM_BUG3674
        #define _gasneti_atomic64_read(p)      (*(uint64_t volatile *)&((p)->gasneti_ctr))
        #define _gasneti_atomic64_set(p,v)     (*(uint64_t volatile *)&((p)->gasneti_ctr) = (v))
       #else
        #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))
       #endif

          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  #if GASNETI_PGI_ASM_BUG2843 && GASNET_NDEBUG
            #pragma routine opt 2 /* Bug 2843 - pgcc miscompiles this code at -O1, so force -O2 */
          #endif
            GASNETI_ASM_REGISTER_KEYWORD unsigned char _retval;
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _readval = _oldval;
            __asm__ __volatile__ (
                    GASNETI_ASM_USED(4)
		    GASNETI_X86_LOCK_PREFIX
		    "cmpxchgq %3, %1	\n\t"
		    "sete %0"
		    : "=q" (_retval), "=m" (_p->gasneti_ctr), "=a" (_readval)
		    : "r" (_newval), "m" (_p->gasneti_ctr), "a" (_oldval)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	  #if GASNETI_PGI_ASM_BUG1754
            return _retval & 0xFF;
	  #else
            return _retval;
	  #endif
          }
          #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
          GASNETI_INLINE(_gasneti_atomic64_swap)
          uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_v, uint64_t _value) {
	  #if GASNETI_PGI_ASM_BUG2843 && GASNET_DEBUG
            #pragma routine opt 1 /* pgcc miscompiles this code at -O0, so force -O1 */
          #endif
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _retval;
            __asm__ __volatile__(
                    GASNETI_ASM_USED(2)
                    GASNETI_X86_LOCK_PREFIX  /* 'lock' is implied, but is the fence? */
                    "xchgq %0, %1"
                    : "=r" (_retval), "=m" (_v->gasneti_ctr)
                    : "m" (_v->gasneti_ctr), "0" (_value)
                    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
            return _retval;
          }
          #define _gasneti_atomic64_swap _gasneti_atomic64_swap
          GASNETI_INLINE(_gasneti_atomic64_fetchadd)
          uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *_v, uint64_t _op) {
            /* CAUTION: see atomic32_fetchadd for note about PathScale and Intel compilers */
	  #if GASNETI_PGI_ASM_BUG2843 && GASNET_DEBUG
            #pragma routine opt 1 /* pgcc miscompiles this code at -O0, so force -O1 */
          #endif
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _retval;
            __asm__ __volatile__(
                    GASNETI_ASM_USED(2)
                    GASNETI_X86_LOCK_PREFIX
                    "xaddq %0, %1"
                    : "=r" (_retval), "=m" (_v->gasneti_ctr)
                    : "m" (_v->gasneti_ctr), "0" (_op)
                    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
            return _retval;
          }
          #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd
      #elif PLATFORM_COMPILER_OPEN64
        /* No known working 64-bit atomics for this compiler on ILP32.  See bug 2725. */
      #elif GASNETI_USE_X86_EBX && \
            !(__APPLE_CC__ && defined(__llvm__)) /* bug 3071 */
	/* "Normal" ILP32 case:
	 *
	 * To perform read and set atomically on x86 requires use of the locked
	 * 8-byte c-a-s instruction.  This is the only atomic 64-bit operation
	 * available on this architecture.  Note that we need the lock prefix
	 * even on a uniprocessor to ensure that we are signal safe.
	 *
	 * See the following #elif/#else clauses for slight variants.
	 */
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_newval);
          __asm__ __volatile__ (
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "sete	%b1		"
		    : "=m" (_p->gasneti_ctr), "+&A" (_oldval)
		    : "m" (_p->gasneti_ctr), "b" (_newlo), "c" (_newhi)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
          return (uint8_t)_oldval;
        }
        #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
        GASNETI_INLINE(_gasneti_atomic64_swap)
        uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _v) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval = _p->gasneti_ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_v);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_v);
          _GASNETI_ATOMIC_CHECKALIGN(gasneti_atomic64_align, _p);
          __asm__ __volatile__ (
		    "0:				\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		\n\t"
		    "jnz	0b		\n\t"
		    : "=m" (_p->gasneti_ctr), "+&A" (_oldval)
		    : "m" (_p->gasneti_ctr), "b" (_newlo), "c" (_newhi)
		    : "cc", "memory");
          return _oldval;
	}
	#define _gasneti_atomic64_swap _gasneti_atomic64_swap
	#define _gasneti_atomic64_set(p,v) ((void)_gasneti_atomic64_swap(p,v))
        GASNETI_INLINE(_gasneti_atomic64_read)
        uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *_p) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _retval;
          _GASNETI_ATOMIC_CHECKALIGN(gasneti_atomic64_align, _p);
          __asm__ __volatile__ (
		    /* Set [a:d] = [b:c], thus preserving b and c */
		    "movl	%%ebx, %%eax	\n\t"
		    "movl	%%ecx, %%edx	\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		"
		    : "=m" (_p->gasneti_ctr), "=&A" (_retval)
		    : "m" (_p->gasneti_ctr)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	  return _retval;
	}
	#define _gasneti_atomic64_read _gasneti_atomic64_read
        GASNETI_INLINE(_gasneti_atomic64_cas_val) /* for 64-bit FETCHADD */
        uint64_t _gasneti_atomic64_cas_val(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_newval);
          __asm__ __volatile__ (
                    GASNETI_ASM_USED(2)
		    "lock;			"
		    "cmpxchg8b	%0		"
		    : "=m" (_p->gasneti_ctr), "+&A" (_oldval)
		    : "m" (_p->gasneti_ctr), "b" (_newlo), "c" (_newhi)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
          return _oldval;
        }
	#define _gasneti_atomic64_cas_val _gasneti_atomic64_cas_val
      #elif (__APPLE_CC__ && defined(__llvm__)) /* bug 3071 */
        /* "Normal" ILP32 case except w/o "m" inputs or outputs to CAS and Set.
         * Such operands lead to "Ran out of registers during register allocation!"
         * Instead a "memory" clobber is used.
         * Read is identical to the Normal case.
         */
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_newval);
          __asm__ __volatile__ (
		    "lock;			"
		    "cmpxchg8b	(%1)		\n\t"
		    "sete	%b0		"
		    : "+&A" (_oldval)
		    : "r" (&_p->gasneti_ctr), "b" (_newlo), "c" (_newhi)
		    : "cc", "memory");
          return (uint8_t)_oldval;
        }
        #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
        GASNETI_INLINE(_gasneti_atomic64_swap)
        uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _v) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval = _p->gasneti_ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_v);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_v);
          _GASNETI_ATOMIC_CHECKALIGN(gasneti_atomic64_align, _p);
          __asm__ __volatile__ (
                    GASNETI_ASM_USED(2)
		    "0:				\n\t"
		    "lock;			"
		    "cmpxchg8b	(%1)		\n\t"
		    "jnz	0b		"
		    : "+&A" (_oldval)
		    : "r" (&_p->gasneti_ctr), "b" (_newlo), "c" (_newhi)
		    : "cc", "memory");
          return _oldval;
	}
	#define _gasneti_atomic64_swap _gasneti_atomic64_swap
	#define _gasneti_atomic64_set(p,v) ((void)_gasneti_atomic64_swap(p,v))
        GASNETI_INLINE(_gasneti_atomic64_read)
        uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *_p) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _retval;
          _GASNETI_ATOMIC_CHECKALIGN(gasneti_atomic64_align, _p);
          __asm__ __volatile__ (
		    /* Set [a:d] = [b:c], thus preserving b and c */
		    "movl	%%ebx, %%eax	\n\t"
		    "movl	%%ecx, %%edx	\n\t"
		    "lock;			"
		    "cmpxchg8b	%0		"
		    : "=m" (_p->gasneti_ctr), "=&A" (_retval)
		    : "m" (_p->gasneti_ctr)
		    : "cc" GASNETI_ATOMIC_MEM_CLOBBER);
	  return _retval;
	}
	#define _gasneti_atomic64_read _gasneti_atomic64_read
        GASNETI_INLINE(_gasneti_atomic64_cas_val) /* for 64-bit FETCHADD */
        uint64_t _gasneti_atomic64_cas_val(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_newval);
          __asm__ __volatile__ (
		    "lock;			"
		    "cmpxchg8b	(%1)		"
		    : "+&A" (_oldval)
		    : "r" (&_p->gasneti_ctr), "b" (_newlo), "c" (_newhi)
		    : "cc", "memory");
          return _oldval;
        }
        #define _gasneti_atomic64_cas_val _gasneti_atomic64_cas_val
      #elif !GASNETI_USE_X86_EBX
	/* Much the same as the "normal" ILP32 case, but w/ save and restore of EBX.
	 * This is achieved by passing the "other" 64-bit value in ECX and a second
 	 * register of the compiler's choosing, which is then swapped w/ EBX.
	 *
	 * We also need to take care that the cmpxchg8b intruction won't get a
	 * GOT-relative address argument - since EBX doesn't hold the GOT pointer
	 * at the time it is executed.  This is done by loading the address into
	 * an available register (but not EBX) rather than giving it as an "m".
	 *
	 * Alas, if we try to add an "m" output for the target location, gcc thinks
	 * it needs to allocate another register for it.  Having none left, it gives
	 * up at this point.  So, we need to list "memory" in the clobbers instead.
	 */
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_newval);
          __asm__ __volatile__ (
		    "xchgl	%1, %%ebx	\n\t"
		    "lock;			"
		    "cmpxchg8b	(%3)		\n\t"
		    "sete	%b0		\n\t"
		    "movl	%1, %%ebx	"
		    : "+&A" (_oldval), "+&r" (_newlo)
		    : "c" (_newhi), "DS" (&_p->gasneti_ctr) /* "DS" = EDI or ESI, but not EBX */
		    : "cc", "memory");
          return (uint8_t)_oldval;
        }
        #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
        GASNETI_INLINE(_gasneti_atomic64_swap)
        uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _v) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval = _p->gasneti_ctr;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_v);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_v);
          _GASNETI_ATOMIC_CHECKALIGN(gasneti_atomic64_align, _p);
          __asm__ __volatile__ (
		    "xchgl	%1, %%ebx	\n\t"
		    "0:				\n\t"
		    "lock;			"
		    "cmpxchg8b	(%3)		\n\t"
		    "jnz	0b		\n\t"
		    "movl	%1, %%ebx	"
		    : "+&A" (_oldval), "+&r" (_newlo)
		    : "c" (_newhi), "DS" (&_p->gasneti_ctr) /* "DS" = EDI or ESI, but not EBX */
		    : "cc", "memory");
          return _oldval;
	}
	#define _gasneti_atomic64_swap _gasneti_atomic64_swap
	#define _gasneti_atomic64_set(p,v) ((void)_gasneti_atomic64_swap(p,v))
        GASNETI_INLINE(_gasneti_atomic64_read)
        uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *_p) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _retval;
          _GASNETI_ATOMIC_CHECKALIGN(gasneti_atomic64_align, _p);
          __asm__ __volatile__ (
		    /* Set [a:d] = [b:c], thus preserving b and c */
		    "movl	%%ebx, %%eax	\n\t"
		    "movl	%%ecx, %%edx	\n\t"
		    "lock;			"
		    "cmpxchg8b	(%2)		"
		    : "=m" (_p->gasneti_ctr), "=&A" (_retval)
		    : "r" (&_p->gasneti_ctr)
		    : "cc");
	  return _retval;
	}
	#define _gasneti_atomic64_read _gasneti_atomic64_read
        GASNETI_INLINE(_gasneti_atomic64_cas_val) /* for 64-bit FETCHADD */
        uint64_t _gasneti_atomic64_cas_val(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newlo = GASNETI_LOWORD(_newval);
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _newhi = GASNETI_HIWORD(_newval);
          __asm__ __volatile__ (
		    "xchgl	%1, %%ebx	\n\t"
		    "lock;			"
		    "cmpxchg8b	(%3)		\n\t"
		    "movl	%1, %%ebx	"
		    : "+&A" (_oldval), "+&r" (_newlo)
		    : "c" (_newhi), "DS" (&_p->gasneti_ctr) /* "DS" = EDI or ESI, but not EBX */
		    : "cc", "memory");
          return _oldval;
        }
        #define _gasneti_atomic64_cas_val _gasneti_atomic64_cas_val
      #else
        #error "unreachable case in gasneti_asm_bits.h"
      #endif

      /* Optionally build a 128-bit atomic type using 64-bit types for all args */
      #if GASNETI_HAVE_X86_CMPXCHG16B && !GASNETI_ATOMIC64_NOINLINE
	#define GASNETI_HAVE_ATOMIC128_T 16 /* Encodes aligment */
	typedef struct { volatile uint64_t _lo, _hi; } gasneti_atomic128_t;
	#define gasneti_atomic128_init(hi,lo) {(lo),(hi)}

	GASNETI_INLINE(gasneti_atomic128_compare_and_swap)
	int gasneti_atomic128_compare_and_swap(gasneti_atomic128_t *_p, uint64_t _oldhi, uint64_t _oldlo, uint64_t _newhi, uint64_t _newlo, const int _flags) {
	  GASNETI_ASM_REGISTER_KEYWORD unsigned char _retval;
          _GASNETI_ATOMIC_CHECKALIGN(16, _p); /* cmpxchg16b requires 16-byte alignment */
	  __asm__ __volatile__ (
		"lock; "
		"cmpxchg16b  %1   \n\t"
		"sete        %0   "
		: "=q" (_retval), "=m" (*_p), "+&a" (_oldlo), "+&d" (_oldhi)
		: "b" (_newlo), "c" (_newhi)
		: "cc", "memory");
	  #if GASNETI_PGI_ASM_BUG1754
	    return _retval & 0xFF;
	  #else
	    return _retval;
	  #endif
	}
	#define gasneti_atomic128_compare_and_swap gasneti_atomic128_compare_and_swap

	GASNETI_INLINE(gasneti_atomic128_set)
	void gasneti_atomic128_set(gasneti_atomic128_t *_p, uint64_t _newhi, uint64_t _newlo, const int _flags) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldlo = _p->_lo;
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldhi = _p->_hi;
          _GASNETI_ATOMIC_CHECKALIGN(16, _p); /* cmpxchg16b requires 16-byte alignment */
	  __asm__ __volatile__ (
		"0:               \n\t"
		"lock; "
		"cmpxchg16b  %0   \n\t"
		"jnz         0b   "
		: "=m" (*_p), "+&a" (_oldlo), "+&d" (_oldhi)
		: "b" (_newlo), "c" (_newhi)
		: "cc", "memory");
	}
	#define gasneti_atomic128_set gasneti_atomic128_set

	GASNETI_INLINE(gasneti_atomic128_read)
	void gasneti_atomic128_read(uint64_t *_outhi, uint64_t *_outlo, gasneti_atomic128_t *_p, const int _flags) {
	  GASNETI_ASM_REGISTER_KEYWORD uint64_t _retlo, _rethi;
          _GASNETI_ATOMIC_CHECKALIGN(16, _p); /* cmpxchg16b requires 16-byte alignment */
	  __asm__ __volatile__ (
		"movq        %%rbx, %1 \n\t"
		"movq        %%rcx, %2 \n\t"
		"lock; "
		"cmpxchg16b  %0        "
		: "+m" (*_p), "=&a" (_retlo),  "=&d" (_rethi)
		: /* no inputs */
		: "cc", "memory");
	  *_outlo = _retlo;
	  *_outhi = _rethi;
	}
	#define gasneti_atomic128_read gasneti_atomic128_read
      #endif /* GASNETI_HAVE_X86_CMPXCHG16B */
     #if PLATFORM_COMPILER_SUN_C
       #pragma error_messages(default, E_ASM_UNUSED_PARAM)
     #elif PLATFORM_COMPILER_SUN_CXX
       #pragma error_messages(default, inlasmpnu)
     #endif
    #elif PLATFORM_COMPILER_SUN
      /* First, some macros to hide the x86 vs. x86-64 ABI differences */
      #if PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC
        #define _gasneti_atomic_addr		"(%rdi)"
        #define _gasneti_atomic_load_arg0	""	/* arg0 in rdi */
        #define _gasneti_atomic_load_arg1	"movq %rsi, %rax	\n\t"
	#define _gasneti_atomic_load_arg2	""	/* arg2 in rdx */
      #else
        #define _gasneti_atomic_addr		"(%ecx)"
        #define _gasneti_atomic_load_arg0	"movl 8(%ebp), %ecx	\n\t"
        #define _gasneti_atomic_load_arg1	"movl 12(%ebp), %eax	\n\t"
	#define _gasneti_atomic_load_arg2	"movl 16(%ebp), %edx	\n\t"
      #endif

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))

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

      #define GASNETI_ATOMIC32_SWAP_BODY                                \
          GASNETI_ASM_SPECIAL(                                          \
                       _gasneti_atomic_load_arg0                        \
                       _gasneti_atomic_load_arg1                        \
                       GASNETI_X86_LOCK_PREFIX                          \
                       "xchgl %eax, " _gasneti_atomic_addr  )

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

      #define GASNETI_ATOMIC32_SPECIALS                                        \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_increment,          \
				 GASNETI_ATOMIC32_INCREMENT_BODY)              \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_decrement,          \
				 GASNETI_ATOMIC32_DECREMENT_BODY)              \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_decrement_and_test, \
				 GASNETI_ATOMIC32_DECREMENT_AND_TEST_BODY)     \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_compare_and_swap,   \
				 GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY)       \
        GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_swap,               \
                                 GASNETI_ATOMIC32_SWAP_BODY)                   \
	GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_fetchadd,           \
				 GASNETI_ATOMIC32_FETCHADD_BODY)

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
      #define gasneti_atomic64_init(v)      { (v) }

      /* 64-bit differ between x86 and x86-64: */
      #if PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC /* Athlon64/Opteron */
        #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))

        #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY			\
	  GASNETI_ASM_SPECIAL(                                          \
		       _gasneti_atomic_load_arg0			\
		       _gasneti_atomic_load_arg1			\
		       _gasneti_atomic_load_arg2			\
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchgq %rdx, " _gasneti_atomic_addr		"\n\t" \
		       "sete  %cl					\n\t" \
		       "movzbl  %cl, %eax" )
        #define GASNETI_ATOMIC64_SWAP_BODY                              \
          GASNETI_ASM_SPECIAL(                                          \
                       _gasneti_atomic_load_arg0                        \
                       _gasneti_atomic_load_arg1                        \
                       GASNETI_X86_LOCK_PREFIX                          \
                       "xchgq    %rax, " _gasneti_atomic_addr )
        #define GASNETI_ATOMIC64_FETCHADD_BODY                          \
          GASNETI_ASM_SPECIAL(                                          \
                       _gasneti_atomic_load_arg0                        \
                       _gasneti_atomic_load_arg1                        \
                       GASNETI_X86_LOCK_PREFIX                          \
                       "xaddq    %rax, " _gasneti_atomic_addr )

        #define GASNETI_ATOMIC64_SPECIALS                                        \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap,   \
                                   GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)       \
          GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_swap,               \
                                   GASNETI_ATOMIC64_SWAP_BODY)                   \
          GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_fetchadd,           \
                                   GASNETI_ATOMIC64_FETCHADD_BODY)
      #else /* x86 */
        #define GASNETI_ATOMIC64_READ_BODY     				\
	  GASNETI_ASM_SPECIAL(                                          \
		       "pushl     %edi					\n\t" \
		       "movl      8(%ebp), %edi				\n\t" \
		       "movl      %ebx, %eax				\n\t" \
		       "movl      %ecx, %edx				\n\t" \
		       GASNETI_X86_LOCK_PREFIX				\
		       "cmpxchg8b (%edi)				\n\t" \
		       "popl      %edi" )

        #define GASNETI_ATOMIC64_SET_BODY     				\
	  GASNETI_ASM_SPECIAL(                                          \
		       "pushl     %edi					\n\t" \
		       "movl      8(%ebp), %edi				\n\t" \
		       "pushl     %ebx					\n\t" \
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
		       "pushl     %ebx					\n\t" \
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

        /* Yes, really: */
        #define GASNETI_ATOMIC64_SWAP_BODY GASNETI_ATOMIC64_SET_BODY

        #define GASNETI_ATOMIC64_FETCHADD_BODY                          \
          GASNETI_ASM_SPECIAL(                                          \
                       "pushl     %edi                                  \n\t" \
                       "movl      8(%ebp), %edi                         \n\t" \
                       "pushl     %ebx                                  \n\t" \
                       "movl      0(%edi), %eax                         \n\t" \
                       "pushl     %ebp                                  \n\t" \
                       "movl      4(%edi), %edx                         \n\t" \
                       "pushl     %esi                                  \n\t" \
                       "movl      12(%ebp), %esi                        \n\t" \
                       "movl      16(%ebp), %ebp                        \n\t" \
                       "1:                                              \n\t" \
                       "movl      %eax, %ebx                            \n\t" \
                       "movl      %edx, %ecx                            \n\t" \
                       "addl      %esi, %ebx                            \n\t" \
                       "adcl      %ebp, %ecx                            \n\t" \
                       GASNETI_X86_LOCK_PREFIX                          \
                       "cmpxchg8b (%edi)                                \n\t" \
                       "jnz       1b                                    \n\t" \
                       "popl      %esi                                  \n\t" \
                       "popl      %ebp                                  \n\t" \
                       "popl      %ebx                                  \n\t" \
                       "popl      %edi" )

        #define GASNETI_ATOMIC64_SPECIALS                                      \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_read,             \
				   GASNETI_ATOMIC64_READ_BODY)                 \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_set,              \
				   GASNETI_ATOMIC64_SET_BODY)                  \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
				   GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)     \
          GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_swap,             \
                                   GASNETI_ATOMIC64_SWAP_BODY)                 \
          GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_fetchadd,         \
                                   GASNETI_ATOMIC64_FETCHADD_BODY)
      #endif 
    #elif (PLATFORM_ARCH_X86_64 && PLATFORM_COMPILER_CRAY)
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))
      #define gasneti_atomic32_fetchadd(p,op,f) \
              __sync_fetch_and_add(&((p)->gasneti_ctr), (uint32_t)(op))
      #define gasneti_atomic32_decrement_and_test(p,f) (1==gasneti_atomic32_fetchadd((p),-1,(f)))
      #define gasneti_atomic32_compare_and_swap(p,oval,nval,f) \
              (__sync_val_compare_and_swap(&((p)->gasneti_ctr), (oval), (nval)) == (oval))
      GASNETI_INLINE(gasneti_atomic32_swap)
      uint32_t gasneti_atomic32_swap(gasneti_atomic32_t *_p, uint32_t _nval, const int _flags) {
          GASNETI_ASM_REGISTER_KEYWORD volatile uint32_t *_p32 = &(_p->gasneti_ctr);
          GASNETI_ASM_REGISTER_KEYWORD uint32_t _oval, _tmp = *_p32;
          do {
              _oval = _tmp;
          } while (_oval != (_tmp = __sync_val_compare_and_swap(_p32,_oval,_nval)));
          return _tmp;
      }
      #define gasneti_atomic32_swap gasneti_atomic32_swap

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
      #define gasneti_atomic64_init(v)       { (v) }
      #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))
      #define gasneti_atomic64_fetchadd(p,op,f)\
              __sync_fetch_and_add(&((p)->gasneti_ctr), (uint64_t)(op))
      #define gasneti_atomic64_decrement_and_test(p,f) (1==gasneti_atomic64_fetchadd((p),-1,(f)))
      #define gasneti_atomic64_compare_and_swap(p,oval,nval,f) \
              (__sync_val_compare_and_swap(&((p)->gasneti_ctr), (oval), (nval)) == (oval))
      GASNETI_INLINE(gasneti_atomic64_swap)
      uint64_t gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _nval, const int _flags) {
          GASNETI_ASM_REGISTER_KEYWORD volatile uint64_t *_p64 = &(_p->gasneti_ctr);
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _oval, _tmp = *_p64;
          do {
              _oval = _tmp;
          } while (_oval != (_tmp = __sync_val_compare_and_swap(_p64,_oval,_nval)));
          return _tmp;
      }
      #define gasneti_atomic64_swap gasneti_atomic64_swap
    #else
      #error unrecognized x86 compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
    #endif

    /* Optionally construct 64-bit FETCHADD from a CAS which returns the old value.
       We do it this way because x86 has so few registers that it is VERY hard to build
       this operation purely from inline asm.  The alternative based on the boolean
       CAS is a poor choice because it must either perform cmpxchg8b TWICE per
       iteration (the extra is atomic64_read), or use only non-atomic 64-bit reads.
     */
    #if defined(_gasneti_atomic64_cas_val)
      GASNETI_INLINE(_gasneti_atomic64_fetchadd)
      uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *_p, const uint64_t _op) {
        GASNETI_ASM_REGISTER_KEYWORD uint64_t _oval;
        GASNETI_ASM_REGISTER_KEYWORD uint64_t _tmp = _p->gasneti_ctr;
        do {
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _sum = _op + (_oval = _tmp);
          _tmp = _gasneti_atomic64_cas_val(_p,_oval,_sum);
        } while (_oval != _tmp);
        return _tmp;
      }
      #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_IA64 /* Itanium */
    #if GASNETI_ATOMIC64_NOINLINE
      #error Internal error - unexpected split atomics implementation on IA64
    #endif
    #if PLATFORM_COMPILER_INTEL
      /* Intel compiler's inline assembly broken on Itanium (bug 384) - use intrinsics instead */
      #include <ia64intrin.h>

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_increment(p) __fetchadd4_acq((unsigned int *)&((p)->gasneti_ctr),1)
      #define _gasneti_atomic32_decrement(p) __fetchadd4_acq((unsigned int *)&((p)->gasneti_ctr),-1)
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))
      #define _gasneti_atomic32_decrement_and_test(p) \
                    (__fetchadd4_acq((unsigned int *)&((p)->gasneti_ctr),-1) == 1)
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
                    (_InterlockedCompareExchange_acq((volatile unsigned int *)&((p)->gasneti_ctr),nval,oval) == (oval))
      #define _gasneti_atomic32_swap(p,nval) \
                    _InterlockedExchange((volatile unsigned int *)&((p)->gasneti_ctr),nval)
      GASNETI_INLINE(_gasneti_atomic32_addfetch)
      uint32_t _gasneti_atomic32_addfetch(gasneti_atomic32_t *_p, const uint32_t _op) {
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _sum, _oval;
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _tmp = _p->gasneti_ctr;
        do {
          _sum = _op + (_oval = _tmp);
          _tmp = _InterlockedCompareExchange_acq((volatile unsigned int *)&_p->gasneti_ctr,_oval,_sum);
        } while (_oval != _tmp);
        return _sum;
      }
      #define _gasneti_atomic32_addfetch _gasneti_atomic32_addfetch

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
      #define gasneti_atomic64_init(v)       { (v) }
      #define _gasneti_atomic64_increment(p) __fetchadd8_acq((unsigned __int64 *)&((p)->gasneti_ctr),1)
      #define _gasneti_atomic64_decrement(p) __fetchadd8_acq((unsigned __int64 *)&((p)->gasneti_ctr),-1)
      #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))
      #define _gasneti_atomic64_decrement_and_test(p) \
                    (__fetchadd8_acq((unsigned __int64 *)&((p)->gasneti_ctr),-1) == 1)
      #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
                    (_InterlockedCompareExchange64_acq((volatile unsigned __int64 *)&((p)->gasneti_ctr),nval,oval) == (oval))
      #define _gasneti_atomic64_swap(p,nval) \
                    _InterlockedExchange64((volatile unsigned int *)&((p)->gasneti_ctr),nval)
      GASNETI_INLINE(_gasneti_atomic64_addfetch)
      uint64_t _gasneti_atomic64_addfetch(gasneti_atomic64_t *_p, const uint64_t _op) {
        GASNETI_ASM_REGISTER_KEYWORD uint64_t _sum, _oval;
        GASNETI_ASM_REGISTER_KEYWORD uint64_t _tmp = _p->gasneti_ctr;
        do {
          _sum = _op + (_oval = _tmp);
          _tmp = _InterlockedCompareExchange64_acq((volatile unsigned __int64 *)&_p->gasneti_ctr,_oval,_sum);
        } while (_oval != _tmp);
        return _sum;
      }
      #define _gasneti_atomic64_addfetch _gasneti_atomic64_addfetch

      /* See fence treatment after #endif */
    #elif PLATFORM_COMPILER_GNU
      GASNETI_INLINE(gasneti_atomic32_xchg)
      uint32_t gasneti_atomic32_xchg(uint32_t volatile *_ptr, uint32_t _newval) {
        uint64_t _tmp;
        __asm__ __volatile__ 
            ("xchg4 %0=[%1],%2" : "=r"(_tmp) : "r"(_ptr), "r"(_newval) );
        return (uint32_t) _tmp;
      }
      GASNETI_INLINE(gasneti_atomic32_cmpxchg)
      uint32_t gasneti_atomic32_cmpxchg(uint32_t volatile *_ptr, uint32_t _oldval, uint32_t _newval) {
        uint64_t _tmp = _oldval;
        __asm__ __volatile__ ("mov ar.ccv=%0;;" :: "rO"(_tmp));
        __asm__ __volatile__ 
            ("cmpxchg4.acq %0=[%1],%2,ar.ccv" : "=r"(_tmp) : "r"(_ptr), "r"(_newval) );
        return (uint32_t) _tmp;
      }
      GASNETI_INLINE(gasneti_atomic32_fetchandinc)
      uint32_t gasneti_atomic32_fetchandinc(uint32_t volatile *_ptr) {
        uint64_t _result;
        __asm__ __volatile__ 
            ("fetchadd4.acq %0=[%1],%2" : "=r"(_result) : "r"(_ptr), "i" (1) );
        return (uint32_t) _result;
      }
      GASNETI_INLINE(gasneti_atomic32_fetchanddec)
      uint32_t gasneti_atomic32_fetchanddec(uint32_t volatile *_ptr) {
        uint64_t _result;
        __asm__ __volatile__ 
            ("fetchadd4.acq %0=[%1],%2" : "=r"(_result) : "r"(_ptr), "i" (-1) );
        return (uint32_t) _result;
      }

      GASNETI_INLINE(gasneti_atomic64_xchg)
      uint64_t gasneti_atomic64_xchg(uint64_t volatile *_ptr, uint64_t _newval) {
        uint64_t _tmp;
        __asm__ __volatile__ 
            ("xchg8 %0=[%1],%2" : "=r"(_tmp) : "r"(_ptr), "r"(_newval) );
        return (uint64_t) _tmp;
      }
      GASNETI_INLINE(gasneti_atomic64_cmpxchg)
      uint64_t gasneti_atomic64_cmpxchg(uint64_t volatile *_ptr, uint64_t _oldval, uint64_t _newval) {
        uint64_t _tmp = _oldval;
        __asm__ __volatile__ ("mov ar.ccv=%0;;" :: "rO"(_tmp));
        __asm__ __volatile__ 
            ("cmpxchg8.acq %0=[%1],%2,ar.ccv" : "=r"(_tmp) : "r"(_ptr), "r"(_newval) );
        return (uint64_t) _tmp;
      }
      GASNETI_INLINE(gasneti_atomic64_fetchandinc)
      uint64_t gasneti_atomic64_fetchandinc(uint64_t volatile *_ptr) {
        uint64_t _result;
        __asm__ __volatile__ 
            ("fetchadd8.acq %0=[%1],%2" : "=r"(_result) : "r"(_ptr), "i" (1) );
        return _result;
      }
      GASNETI_INLINE(gasneti_atomic64_fetchanddec)
      uint64_t gasneti_atomic64_fetchanddec(uint64_t volatile *_ptr) {
        uint64_t _result;
        __asm__ __volatile__ 
            ("fetchadd8.acq %0=[%1],%2" : "=r"(_result) : "r"(_ptr), "i" (-1) );
        return _result;
      }

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))
      #define _gasneti_atomic32_increment(p) (gasneti_atomic32_fetchandinc(&((p)->gasneti_ctr)))
      #define _gasneti_atomic32_decrement(p) (gasneti_atomic32_fetchanddec(&((p)->gasneti_ctr)))
      #define _gasneti_atomic32_decrement_and_test(p) (gasneti_atomic32_fetchanddec(&((p)->gasneti_ctr)) == 1)
      #define _gasneti_atomic32_compare_and_swap(p,oval,nval) \
        (gasneti_atomic32_cmpxchg(&((p)->gasneti_ctr),oval,nval) == (oval))
      #define _gasneti_atomic32_swap(p,nval) (gasneti_atomic32_xchg(&((p)->gasneti_ctr),nval))
      GASNETI_INLINE(_gasneti_atomic32_addfetch)
      uint32_t _gasneti_atomic32_addfetch(gasneti_atomic32_t *_p, const uint32_t _op) {
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _sum, _oval;
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _tmp = _p->gasneti_ctr;
        do {
          _sum = _op + (_oval = _tmp);
          _tmp = gasneti_atomic32_cmpxchg(&_p->gasneti_ctr,_oval,_sum);
        } while (_oval != _tmp);
        return _sum;
      }
      #define _gasneti_atomic32_addfetch _gasneti_atomic32_addfetch

      #define GASNETI_HAVE_ATOMIC64_T 1
      typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
      #define gasneti_atomic64_init(v)       { (v) }
      #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))
      #define _gasneti_atomic64_increment(p) (gasneti_atomic64_fetchandinc(&((p)->gasneti_ctr)))
      #define _gasneti_atomic64_decrement(p) (gasneti_atomic64_fetchanddec(&((p)->gasneti_ctr)))
      #define _gasneti_atomic64_decrement_and_test(p) (gasneti_atomic64_fetchanddec(&((p)->gasneti_ctr)) == 1)
      #define _gasneti_atomic64_compare_and_swap(p,oval,nval) \
        (gasneti_atomic64_cmpxchg(&((p)->gasneti_ctr),oval,nval) == (oval))
      #define _gasneti_atomic64_swap(p,nval) (gasneti_atomic64_xchg(&((p)->gasneti_ctr),nval))
      GASNETI_INLINE(_gasneti_atomic64_addfetch)
      uint64_t _gasneti_atomic64_addfetch(gasneti_atomic64_t *_p, const uint64_t _op) {
        GASNETI_ASM_REGISTER_KEYWORD uint64_t _sum, _oval;
        GASNETI_ASM_REGISTER_KEYWORD uint64_t _tmp = _p->gasneti_ctr;
        do {
          _sum = _op + (_oval = _tmp);
          _tmp = gasneti_atomic64_cmpxchg(&_p->gasneti_ctr,_oval,_sum);
        } while (_oval != _tmp);
        return _sum;
      }
      #define _gasneti_atomic64_addfetch _gasneti_atomic64_addfetch

      /* See fence treatment after #endif */
    #else
      #error unrecognized Itanium compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
    #endif

    /* Since supported compilers are generating r-m-w with .acq variants, we can customize
     * the atomic fencing implementation by noting that "mf;; foo.acq" is a full memory
     * barrier both before and after. */
    #define _gasneti_atomic32_prologue_rmw(p,f) /*empty*/
    #define _gasneti_atomic32_fence_before_rmw(p, flags) \
	if (flags & (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST)) gasneti_local_mb();
    #define _gasneti_atomic32_fence_after_rmw(p, flags) \
	/* Nothing */
    #define _gasneti_atomic32_fence_after_bool(p, flags, val) \
	if (!(flags & (GASNETI_ATOMIC_MB_PRE | GASNETI_ATOMIC_MB_POST))) \
	  { if (((flags & GASNETI_ATOMIC_RMB_POST_IF_TRUE ) &&  val) || \
                ((flags & GASNETI_ATOMIC_RMB_POST_IF_FALSE) && !val)) gasneti_local_rmb(); }
    #define _gasneti_atomic64_prologue_rmw(p,f) /*empty*/
    #define _gasneti_atomic64_fence_before_rmw  _gasneti_atomic32_fence_before_rmw
    #define _gasneti_atomic64_fence_after_rmw   _gasneti_atomic32_fence_after_rmw
    #define _gasneti_atomic64_fence_after_bool  _gasneti_atomic32_fence_after_bool

  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_SPARC
    #if defined(__sparcv9) || defined(__sparcv9cpu) || \
        defined(__sparc_v9__) || defined(GASNETI_ARCH_ULTRASPARC) /* SPARC v9 ISA */
      #if PLATFORM_COMPILER_GNU
        #define GASNETI_HAVE_ATOMIC32_T 1
        typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
        #define gasneti_atomic32_init(v)       { (v) }
        #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
        #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))

        GASNETI_INLINE(_gasneti_atomic32_swap)
        uint32_t _gasneti_atomic32_swap(gasneti_atomic32_t *_v, uint32_t _newval) {
          GASNETI_ASM_REGISTER_KEYWORD uint32_t volatile * _addr = (uint32_t volatile *)(&_v->gasneti_ctr);
          GASNETI_ASM_REGISTER_KEYWORD uint32_t _val = _newval;
          __asm__ __volatile__ ( 
            "swap %1, %0"   
            : "+r" (_val), "=m" (*_addr) );
          return _val;
        }
        #define _gasneti_atomic32_swap _gasneti_atomic32_swap

        /* Default impls of inc, dec, dec-and-test, add and sub */
        GASNETI_INLINE(_gasneti_atomic32_fetchadd)
        uint32_t _gasneti_atomic32_fetchadd(gasneti_atomic32_t *_v, uint32_t _op) {
          /* SPARC v9 architecture manual, p.333 
           * This function requires the cas instruction in Sparc V9, and therefore gcc -mcpu=ultrasparc
	   * The manual says (sec A.9) no memory fences in CAS (in conflict w/ JMM web page).
           */
          GASNETI_ASM_REGISTER_KEYWORD uint32_t volatile * _addr = (uint32_t volatile *)(&_v->gasneti_ctr);
          GASNETI_ASM_REGISTER_KEYWORD uint32_t _oldval;
          GASNETI_ASM_REGISTER_KEYWORD uint32_t _newval;
          __asm__ __volatile__ ( 
            "ld       [%4],%0    \n\t" /* oldval = *addr; */
            "0:			 \t" 
            "add      %0,%3,%1   \n\t" /* newval = oldval + op; */
            "cas      [%4],%0,%1 \n\t" /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
            "cmp      %0, %1     \n\t" /* check if newval == oldval (swap succeeded) */
            "bne,a,pn %%icc, 0b  \n\t" /* otherwise, retry (,pn == predict not taken; ,a == annul) */
            "  mov    %1, %0     "     /* oldval = newval; (branch delay slot, annulled if not taken) */
            : "=&r"(_oldval), "=&r"(_newval), "=m"(_v->gasneti_ctr)
            : "rI"(_op), "r"(_addr), "m"(_v->gasneti_ctr) );
          return _oldval;
        }
        #define _gasneti_atomic32_fetchadd _gasneti_atomic32_fetchadd

        GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
        int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *_v, uint32_t _oldval, uint32_t _newval) {
          GASNETI_ASM_REGISTER_KEYWORD volatile uint32_t * _addr = (volatile uint32_t *)&(_v->gasneti_ctr);
          __asm__ __volatile__ ( 
              "cas      [%3],%2,%0"  /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
              : "+r"(_newval), "=m"(_v->gasneti_ctr)
              : "r"(_oldval), "r"(_addr), "m"(_v->gasneti_ctr) );
          return (int)(_newval == _oldval);
        }
        #define _gasneti_atomic32_compare_and_swap _gasneti_atomic32_compare_and_swap

        #if GASNETI_ATOMIC64_NOINLINE
          // Using SLOW or GENERIC alternative
        #elif PLATFORM_ARCH_64
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
          #define gasneti_atomic64_init(v)       { (v) }
          #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
          #define _gasneti_atomic64_set(p,v)     do { (p)->gasneti_ctr = (v); } while(0)
          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_v, uint64_t _oldval, uint64_t _newval) {
            GASNETI_ASM_REGISTER_KEYWORD volatile uint64_t * _addr = (volatile uint64_t *)&(_v->gasneti_ctr);
            __asm__ __volatile__ ( 
		"casx	[%3],%2,%0"  /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
              : "+r"(_newval), "=m"(_v->gasneti_ctr)
              : "r"(_oldval), "r"(_addr), "m"(_v->gasneti_ctr) );
            return (int)(_newval == _oldval);
          }
          #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap

          GASNETI_INLINE(_gasneti_atomic64_fetchadd)
          uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *_v, uint64_t _op) {
            GASNETI_ASM_REGISTER_KEYWORD int64_t volatile * _addr = (int64_t volatile *)(&_v->gasneti_ctr);
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval;
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _newval;
            __asm__ __volatile__ (
              "ldx      [%4],%0    \n\t" /* oldval = *addr; */
              "0:\t"
              "add      %0,%3,%1   \n\t" /* newval = oldval + op; */
              "casx     [%4],%0,%1 \n\t" /* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */
              "cmp      %0, %1     \n\t" /* check if newval == oldval (swap succeeded) */
              "bne,a,pn %%xcc, 0b  \n\t" /* otherwise, retry (,pn == predict not taken; ,a == annul) */
              "  mov    %1, %0     "     /* oldval = newval; (branch delay slot, annulled if not taken) */
              : "=&r"(_oldval), "=&r"(_newval), "=m"(_v->gasneti_ctr)
              : "rI"(_op), "r"(_addr), "m"(_v->gasneti_ctr) );
            return _oldval;
          }
          #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd

          GASNETI_INLINE(_gasneti_atomic64_swap)
          uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_v, int64_t _newval) {
            GASNETI_ASM_REGISTER_KEYWORD uint64_t volatile * _addr = (uint64_t volatile *)(&_v->gasneti_ctr);
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval;
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _tmp;
            __asm__ __volatile__ ( 
              "mov      %3, %1     \n\t" /* tmp = newval; */
              "0:\t" 
              "ldx      [%4],%0    \n\t" /* oldval = *addr; */
              "casx     [%4],%0,%1 \n\t" /* if (*addr == oldval) SWAP(*addr,tmp); else tmp = *addr; */
              "cmp      %0, %1     \n\t" /* check if tmp == oldval (swap succeeded) */
              "bne,a,pn %%xcc, 0b  \n\t" /* otherwise, retry (,pn == predict not taken; ,a == annul) */
              "  mov    %3, %1     "     /* tmp = newval; (branch delay slot, annulled if not taken) */
              : "=&r"(_oldval), "=&r"(_tmp), "=m"(_v->gasneti_ctr)
              : "r"(_newval), "r"(_addr), "m"(_v->gasneti_ctr) );
            return _oldval;
          }
          #define _gasneti_atomic64_swap _gasneti_atomic64_swap
        #elif GASNETI_HAVE_SPARC32_64BIT_ASM /* compiler supports "U" and "h" constraints */
          /* ILP32 on a 64-bit CPU. */
          /* Note that the ldd/std instructions *are* atomic, even though they use 2 registers.
           * We wouldn't need asm here if we could be sure the compiler always used ldd/std.
           */
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
          #define gasneti_atomic64_init(v)       { (v) }
          GASNETI_INLINE(_gasneti_atomic64_set)
          void _gasneti_atomic64_set(gasneti_atomic64_t *_p, uint64_t _v) {
          #if PLATFORM_OS_SOLARIS // Bug 3797
            __asm__ __volatile__ ( "std	%1, %0" : "=m"(_p->gasneti_ctr) : "U!f"(_v) );
          #else
            __asm__ __volatile__ ( "std	%1, %0" : "=m"(_p->gasneti_ctr) : "U"(_v) );
          #endif
	  }
          #define _gasneti_atomic64_set _gasneti_atomic64_set
          GASNETI_INLINE(_gasneti_atomic64_read)
          uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *_p) {
	    GASNETI_ASM_REGISTER_KEYWORD uint64_t _retval;
          #if PLATFORM_OS_SOLARIS // Bug 3797
            __asm__ __volatile__ ( "ldd	%1, %0" : "=U!f"(_retval) : "m"(_p->gasneti_ctr) );
          #else
            __asm__ __volatile__ ( "ldd	%1, %0" : "=U"(_retval) : "m"(_p->gasneti_ctr) );
          #endif
	    return _retval;
	  }
          #define _gasneti_atomic64_read _gasneti_atomic64_read
          GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
          int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_v, uint64_t _oldval, uint64_t _newval) {
            /* TODO: Even though {new,old}val are inputs we clear their upper 32-bits.
             * That *probably* should never matter, but we could use another tmp.
             */
            GASNETI_ASM_REGISTER_KEYWORD volatile uint64_t * _addr = (volatile uint64_t *)&(_v->gasneti_ctr);
	    GASNETI_ASM_REGISTER_KEYWORD int _retval, _tmp;
            __asm__ __volatile__ ( 
		"sllx	%H5,32,%0	\n\t"	/* retval = HI(new) << 32 */
		"sllx	%H6,32,%1	\n\t"	/* tmp = HI(old) << 32 */
		"srl	%L5,0,%L5	\n\t"	/* zap any sign-extension of newval */
		"srl	%L6,0,%L6	\n\t"	/* zap any sign-extension of oldval */
		"or	%0,%L5,%0	\n\t"	/* retval |= LO(new) */
		"or	%1,%L6,%1	\n\t"	/* tmp |= LO(old) */
		"casx	[%3],%1,%0	\n\t"	/* atomic CAS, with read value -> retval */
		"xor	%1,%0,%1	\n\t"	/* tmp = 0 IFF retval == tmp */
		"clr	%0		\n\t"   /* retval = 0 */
		"movrz	%1,1,%0"		/* retval = 1 IFF tmp == 0 */
		: "=&h"(_retval), "=&h"(_tmp), "=m"(_v->gasneti_ctr) /* 'h' = 64bit 'o' or 'g' reg */
		: "r"(_addr), "m"(_v->gasneti_ctr), "r"(_newval), "r"(_oldval) );
            return _retval;
          }
          #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
          GASNETI_INLINE(_gasneti_atomic64_swap)
          uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_v, uint64_t _newval) {
            /* TODO: as in CAS, above, consider preserving upper 32-bits of newval */
            GASNETI_ASM_REGISTER_KEYWORD uint64_t volatile * _addr = (uint64_t volatile *)(&_v->gasneti_ctr);
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval; /* register pair for ILP32 ABI */
            GASNETI_ASM_REGISTER_KEYWORD int _tmp1, _tmp2, _tmp3;
            __asm__ __volatile__ (
              "sllx     %H5,32,%1  \n\t" /* tmp1 = HI(newval) << 32 */
              "srl      %L5,0,%L5  \n\t" /* zap any sign-extension of newval */
              "or       %1,%L5,%1  \n\t" /* tmp1 |= LO(newval) */
              "mov      %1,%3      \n\t" /* tmp3 = tmp1 */
              "0:\t"
              "ldx      [%6],%2    \n\t" /* tmp2 = *addr; */
              "casx     [%6],%2,%1 \n\t" /* if (*addr == tmp2) SWAP(*addr,tmp1); else tmp1 = *addr; */
              "cmp      %2,%1      \n\t" /* check if tmp1 == tmp2 (swap succeeded) */
              "bne,a,pn %%xcc, 0b  \n\t" /* otherwise, retry (,pn == predict not taken; ,a == annul) */
              "  mov    %3,%1      \n\t" /* tmp1 = tmp3; (branch delay slot, annulled if not taken) */
              "srlx     %1,32,%H0  \n\t" /* HI(oldval) = tmp1 >> 32 */
              "srl      %1,0,%L0   "     /* LO(oldval) = (uint32_t)tmp1 */
              : "=&r"(_oldval), "=&h"(_tmp1), "=&h"(_tmp2), "=&h"(_tmp3), "=m"(_v->gasneti_ctr)
              : "r"(_newval), "r"(_addr), "m"(_v->gasneti_ctr) );
            return _oldval;
          }
          #define _gasneti_atomic64_swap _gasneti_atomic64_swap
          GASNETI_INLINE(_gasneti_atomic64_fetchadd)
          uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *_v, uint64_t _op) {
            /* TODO: as in CAS, above, consider preserving upper 32-bits of op */
            GASNETI_ASM_REGISTER_KEYWORD uint64_t volatile * _addr = (uint64_t volatile *)(&_v->gasneti_ctr);
            GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval; /* register pair for ILP32 ABI */
            GASNETI_ASM_REGISTER_KEYWORD int _tmp1, _tmp2, _tmp3;
            __asm__ __volatile__ (
              "sllx     %H5,32,%1  \n\t" /* tmp1 = HI(op) << 32 */
              "srl      %L5,0,%L5  \n\t" /* zap any sign-extension of op */
              "or       %1,%L5,%1  \n\t" /* tmp1 |= LO(op) */
              "ldx      [%6],%2    \n\t" /* tmp2 = *addr; */
              "0:\t"
              "add      %1,%2,%3   \n\t" /* tmp3 = tmp1 + tmp2 */
              "casx     [%6],%2,%3 \n\t" /* if (*addr == tmp2) SWAP(*addr,tmp3); else tmp3 = *addr; */
              "cmp      %2,%3      \n\t" /* check if tmp2 == tmp3 (swap succeeded) */
              "bne,a,pn %%xcc, 0b  \n\t" /* otherwise, retry (,pn == predict not taken; ,a == annul) */
              "  mov    %3,%2      \n\t" /* tmp2 = tmp3; (branch delay slot, annulled if not taken) */
              "srlx     %3,32,%H0  \n\t" /* HI(oldval) = tmp3 >> 32 */
              "srl      %3,0,%L0   "     /* LO(oldval) = (uint32_t)tmp3 */
              : "=&r"(_oldval), "=&h"(_tmp1), "=&h"(_tmp2), "=&h"(_tmp3), "=m"(_v->gasneti_ctr)
              : "r"(_op), "r"(_addr), "m"(_v->gasneti_ctr) );
            return _oldval;
          }
          #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd
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
	typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
        #define gasneti_atomic32_init(v)       { (v) }
        #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
        #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))

        /* Default impls of inc, dec, dec-and-test, add and sub */

        #define GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY					\
	    GASNETI_ASM_SPECIAL(                        \
		/* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */	\
		     "cas	[%i0], %i1, %i2		\n\t"				\
		/* retval = (oldval == newval) ? 1 : 0				*/	\
		     "xor	%i2, %i1, %g1		\n\t" /* g1 = 0 IFF old==new */ \
		     "cmp	%g0, %g1		\n\t" /* Set/clear carry bit */	\
		     "subx	%g0, -1, %i0 " )	      /* Subtract w/ carry */

        #define GASNETI_ATOMIC32_SWAP_BODY \
            GASNETI_ASM_SPECIAL( "swap      [%i0], %i1 \n\t" \
                                 "mov       %i1, %i0" )

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
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_swap,             \
                                   GASNETI_ATOMIC32_SWAP_BODY)                 \
	  GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic32_fetchadd,         \
				   GASNETI_ATOMIC32_FETCHADD_BODY)

        #if GASNETI_ATOMIC64_NOINLINE
          // Using SLOW or GENERIC alternative
        #elif PLATFORM_ARCH_64
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
          #define gasneti_atomic64_init(v)       { (v) }
          #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
          #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))
          #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY				\
	    GASNETI_ASM_SPECIAL(							\
		/* if (*addr == oldval) SWAP(*addr,newval); else newval = *addr; */	\
		     "casx	[%i0], %i1, %i2		\n\t"				\
		/* retval = (oldval == newval) ? 1 : 0				*/	\
		     "xor	%i2, %i1, %g1		\n\t" /* g1 = 0 IFF old==new */ \
		     "clr	%i0			\n\t" /* retval = 0 */		\
		     GASNETI_SPARC_MOVRZ_g1_1_i0 )	      /* retval = 1 IFF g1 == 0 */
          #define GASNETI_ATOMIC64_SWAP_BODY                                            \
            GASNETI_ASM_SPECIAL(                                                        \
                     "mov      %i1, %g1                 \n\t"                           \
                     ".LGN001:                          \n\t"                           \
                     "ldx      [%i0], %g4               \n\t"                           \
                     "casx     [%i0], %g4, %g1          \n\t"                           \
                     "cmp      %g4, %g1                 \n\t"                           \
                     "bne,a,pn %xcc, .LGN001            \n\t"                           \
                     "  mov    %i1, %g1                 \n\t"                           \
                     "mov      %g4, %i0" )
          #define GASNETI_ATOMIC64_FETCHADD_BODY                                        \
            GASNETI_ASM_SPECIAL(                                                        \
                     "ldx      [%i0], %g1               \n\t"                           \
                     ".LGN002:                          \n\t"                           \
                     "add      %g1, %i1, %g4            \n\t"                           \
                     "casx     [%i0], %g1, %g4          \n\t"                           \
                     "cmp      %g1, %g4                 \n\t"                           \
                     "bne,a,pn %xcc, .LGN002            \n\t"                           \
                     "  mov    %g4, %g1                 \n\t"                           \
                     "mov      %g4, %i0" )
          #define GASNETI_ATOMIC64_SPECIALS                                      \
            GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
                                     GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)     \
            GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_fetchadd,         \
                                     GASNETI_ATOMIC64_FETCHADD_BODY)             \
            GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_swap,             \
                                     GASNETI_ATOMIC64_SWAP_BODY)
        #else
          /* ILP32 on a 64-bit CPU. */
          #define GASNETI_HAVE_ATOMIC64_T 1
          typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
          #define gasneti_atomic64_init(v)       { (v) }

          /* see gcc asm, above, for explanations */	\
          #define GASNETI_ATOMIC64_SET_BODY                                          	\
	    GASNETI_ASM_SPECIAL(							\
		     "mov	%i1, %o2		\n\t"				\
		     "mov	%i2, %o3		\n\t"				\
		     "std	%o2, [%i0]" )
          #define GASNETI_ATOMIC64_READ_BODY                                          	\
	    GASNETI_ASM_SPECIAL(							\
		     "ldd	[%i0], %i0 " );
          #define GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY                                \
	    GASNETI_ASM_SPECIAL(							\
		     "sllx	%i3, 32, %o1		\n\t"				\
		     "sllx	%i1, 32, %g1		\n\t"				\
		     "srl	%i4, 0, %i4		\n\t"				\
		     "srl	%i2, 0, %i2		\n\t"				\
		     "or	%o1, %i4, %o1		\n\t"				\
		     "or	%g1, %i2, %g1		\n\t"				\
		     "casx	[%i0], %g1, %o1		\n\t"				\
		     "xor	%g1, %o1, %g1		\n\t"				\
		     "clr	%i0			\n\t" /* retval = 0 */		\
		     GASNETI_SPARC_MOVRZ_g1_1_i0 )	      /* retval = 1 IFF g1 == 0 */
          #define GASNETI_ATOMIC64_SWAP_BODY                                            \
            GASNETI_ASM_SPECIAL(                                                        \
                     "sllx     %i1,32,%o5               \n\t"                           \
                     "srl      %i2,0,%i2                \n\t"                           \
                     "or       %o5,%i2,%o5              \n\t"                           \
                     "mov      %o5,%g1                  \n\t"                           \
                     ".LGN001:                          \n\t"                           \
                     "ldx      [%i0],%o7                \n\t"                           \
                     "casx     [%i0],%o7,%o5            \n\t"                           \
                     "cmp      %o7,%o5                  \n\t"                           \
                     "bne,a,pn %xcc, .LGN001            \n\t"                           \
                     "  mov    %g1,%o5                  \n\t"                           \
                     "srlx     %o5,32,%i0               \n\t"                           \
                     "srl      %o5,0,%i1" )
          #define GASNETI_ATOMIC64_FETCHADD_BODY                                        \
            GASNETI_ASM_SPECIAL(                                                        \
                     "sllx     %i1,32,%o5               \n\t"                           \
                     "srl      %i2,0,%i2                \n\t"                           \
                     "or       %o5,%i2,%o5              \n\t"                           \
                     "ldx      [%i0],%o7                \n\t"                           \
                     ".LGN002:                          \n\t"                           \
                     "add      %o5,%o7,%g1              \n\t"                           \
                     "casx     [%i0],%o7,%g1            \n\t"                           \
                     "cmp      %o7,%g1                  \n\t"                           \
                     "bne,a,pn %xcc, .LGN002            \n\t"                           \
                     "  mov    %g1,%o7                  \n\t"                           \
                     "srlx     %g1,32,%i0               \n\t"                           \
                     "srl      %g1,0,%i1" )
          #define GASNETI_ATOMIC64_SPECIALS                                      \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_set,              \
				     GASNETI_ATOMIC64_SET_BODY)                  \
	    GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_read,             \
				     GASNETI_ATOMIC64_READ_BODY)                 \
            GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_compare_and_swap, \
                                     GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY)     \
            GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_swap,             \
                                     GASNETI_ATOMIC64_SWAP_BODY)                 \
            GASNETI_SPECIAL_ASM_DEFN(_gasneti_special_atomic64_fetchadd,         \
                                     GASNETI_ATOMIC64_FETCHADD_BODY)
        #endif

	/* Using default fences, as our asm includes none */
      #else
        #error unrecognized Sparc v9 compiler - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)
      #endif
    #else /* SPARC pre-v9 lacks RMW instructions - we no longer support atomic swap */
      #error "GASNet no longer supports SPARC prior to the v8+/v9 ABIs"
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_POWERPC
    #if defined(GASNETI_ARCH_PPC64)
      /* We are running on a 64-bit capable CPU/OS, but can't use native atomics
         on improperly aligned data.  Since our "contract" with the developer says
         atomic64_t works on 64-bit types without any extra alignment, we may need
         to use mutex-based atomics when not aligned.  See bug 1595 for more info.
       */
      #if (PLATFORM_OS_DARWIN && PLATFORM_ARCH_32)
        /* + Apple's 32-bit ABI only guarantees 4-byte minimum aligment for 64-bit integers and doubles.
         */
        #define GASNETI_HYBRID_ATOMIC64	1
      #endif

      /* Should we use native 64-bit atomics on ILP32? */
      #if PLATFORM_ARCH_32 && (PLATFORM_OS_DARWIN || PLATFORM_OS_LINUX)
        #define GASNETI_PPC64_ILP32_NATIVE_ATOMICS 1
      #endif
    #endif

    #if GASNETI_HAVE_GCC_ASM
      #if GASNETI_PGI_ASM_TPR23291
        #define GASNETI_ASM_CR0 /*empty*/
      #else
        #define GASNETI_ASM_CR0 "cr0"
      #endif

      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))

      GASNETI_INLINE(_gasneti_atomic32_addfetch)
      uint32_t _gasneti_atomic32_addfetch(gasneti_atomic32_t *_v, uint32_t _op) {
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _result;
      #if GASNETI_PGI_ASM_TPR24514 // Work-around bug 3570
        gasneti_atomic32_t * volatile _w = _v; _v = _w;
      #endif
      #if GASNETI_PGI_ASM_TPR23290
        __asm__ __volatile__ (
          "0:\t"
          "lwarx    %0,0,%2 \n\t"
          "add      %0,%0,%3 \n\t"
          "stwcx.   %0,0,%2 \n\t"
          "bne-     0b \n\t"
          : "=&b"(_result), "=m" (_v->gasneti_ctr)  /* constraint b = 'b'ase register (not r0) */
          : "r" (_v), "r"(_op) , "m"(_v->gasneti_ctr)
          : GASNETI_ASM_CR0);
      #else
        __asm__ __volatile__ ( 
          "0:\t"
          "lwarx    %0,0,%2 \n\t" 
          "add%I3   %0,%0,%3 \n\t"
          "stwcx.   %0,0,%2 \n\t"
          "bne-     0b \n\t" 
          : "=&b"(_result), "=m" (_v->gasneti_ctr)  /* constraint b = 'b'ase register (not r0) */
          : "r" (_v), "Ir"(_op) , "m"(_v->gasneti_ctr)
          : GASNETI_ASM_CR0);
      #endif
        return _result;
      }
      #define _gasneti_atomic32_addfetch _gasneti_atomic32_addfetch

      /* Default impls of inc, dec, dec-and-test, add and sub */

      GASNETI_INLINE(_gasneti_atomic32_swap)
      uint32_t _gasneti_atomic32_swap(gasneti_atomic32_t *_v, uint32_t _newval) {
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _oldval;
        __asm__ __volatile__ ( 
          "0:\t"
          "lwarx    %0,0,%2 \n\t" 
          "stwcx.   %3,0,%2 \n\t"
          "bne-     0b \n\t" 
          : "=&r"(_oldval), "=m" (_v->gasneti_ctr)
          : "r" (_v), "r"(_newval) , "m"(_v->gasneti_ctr)
          : GASNETI_ASM_CR0);
        return _oldval;
      }
      #define _gasneti_atomic32_swap _gasneti_atomic32_swap

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *_p, uint32_t _oldval, uint32_t _newval) {
        GASNETI_ASM_REGISTER_KEYWORD uint32_t _result;
        // The following use of _tmp addresses Bug 3742 by clearing the upper
        // 32 bits of the register on LP64, while being a no-op on ILP32:
        GASNETI_ASM_REGISTER_KEYWORD uintptr_t _tmp = _oldval;
        __asm__ __volatile__ (
          "0:\t"
	  "lwarx    %0,0,%2 \n\t"         /* load to result */
	  "xor.     %0,%0,%3 \n\t"        /* xor result w/ tmp */
	  "bne      1f \n\t"              /* branch on mismatch */
	  "stwcx.   %4,0,%2 \n\t"         /* store newval */
	  "bne-     0b \n\t" 
	  "1:	"
	  : "=&r"(_result), "=m"(_p->gasneti_ctr)
	  : "r" (_p), "r"(_tmp), "r"(_newval), "m"(_p->gasneti_ctr)
	  : GASNETI_ASM_CR0);
  
        return (_result == 0);
      } 
      #define _gasneti_atomic32_compare_and_swap _gasneti_atomic32_compare_and_swap

      #if GASNETI_ATOMIC64_NOINLINE
        // Using SLOW or GENERIC alternative
      #elif PLATFORM_ARCH_64
	#define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
        #define gasneti_atomic64_init(_v)       { (_v) }
        #define _gasneti_atomic64_set(_p,_v)	do { (_p)->gasneti_ctr = (_v); } while(0)
        #define _gasneti_atomic64_read(_p)	((_p)->gasneti_ctr)
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _result;
          __asm__ __volatile__ (
		"0:\t"
		"ldarx    %0,0,%2 \n\t"         /* load to result */
		"xor.     %0,%0,%3 \n\t"        /* compare result w/ oldval */
		"bne      1f \n\t"              /* branch on mismatch */
		"stdcx.   %4,0,%2 \n\t"         /* store newval */
		"bne-     0b \n\t"              /* retry on conflict */
		"1:	"
		: "=&b"(_result), "=m"(_p->gasneti_ctr)
		: "r" (_p), "r"(_oldval), "r"(_newval), "m"(_p->gasneti_ctr)
		: GASNETI_ASM_CR0);
          return (_result == 0);
        } 
        #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
        GASNETI_INLINE(_gasneti_atomic64_swap)
        uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _newval) {
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _oldval;
          __asm__ __volatile__ (
                "0:\t"
                "ldarx    %0,0,%2 \n\t"         /* load to oldval */
                "stdcx.   %3,0,%2 \n\t"         /* store newval */
                "bne-     0b"                   /* retry on conflict */
                : "=&b"(_oldval), "=m"(_p->gasneti_ctr)
                : "r" (_p), "r"(_newval), "m"(_p->gasneti_ctr)
                : GASNETI_ASM_CR0);
          return _oldval;
        }
        #define _gasneti_atomic64_swap _gasneti_atomic64_swap
        GASNETI_INLINE(_gasneti_atomic64_addfetch)
        uint64_t _gasneti_atomic64_addfetch(gasneti_atomic64_t *_p, uint64_t _op) {
          GASNETI_ASM_REGISTER_KEYWORD uint64_t _result;
        #if GASNETI_PGI_ASM_TPR24514 // Work-around bug 3570
          gasneti_atomic64_t * volatile _q = _p; _p = _q;
        #endif
        #if GASNETI_PGI_ASM_TPR23290
          __asm__ __volatile__ (
                "0:\t"
                "ldarx    %0,0,%2 \n\t"
                "add      %0,%0,%3 \n\t"
                "stdcx.   %0,0,%2 \n\t"
                "bne-     0b \n\t"
                : "=&b"(_result), "=m" (_p->gasneti_ctr)  /* constraint b = 'b'ase register (not r0) */
                : "r" (_p), "r"(_op) , "m"(_p->gasneti_ctr)
                : GASNETI_ASM_CR0);
        #else
          __asm__ __volatile__ (
                "0:\t"
                "ldarx    %0,0,%2 \n\t"
                "add%I3   %0,%0,%3 \n\t"
                "stdcx.   %0,0,%2 \n\t"
                "bne-     0b \n\t"
                : "=&b"(_result), "=m" (_p->gasneti_ctr)  /* constraint b = 'b'ase register (not r0) */
                : "r" (_p), "Ir"(_op) , "m"(_p->gasneti_ctr)
                : GASNETI_ASM_CR0);
        #endif
          return _result;
        }
        #define _gasneti_atomic64_addfetch _gasneti_atomic64_addfetch
      #elif defined(GASNETI_PPC64_ILP32_NATIVE_ATOMICS) /* ILP32 on 64-bit CPU */
	#define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
        #define gasneti_atomic64_init(_v)       { (_v) }
        GASNETI_INLINE(_gasneti_atomic64_set)
        void _gasneti_atomic64_set(gasneti_atomic64_t *_p, uint64_t _val) {
          uint32_t _tmp;
	  /* We are using the ll/sc reservation as a "canary" that will ensure we
	     don't write to memory a value that was clobbered by an interruption
	     (context switch, signal handler, etc.). */
          __asm__ __volatile__ (
		"clrldi	%L2,%L2,32	\n\t"	/* Zap undefined top half of val */
		"0:\t"
		"ldarx	%1,0,%3		\n\t"	/* establish reservation */
		"sldi	%1,%2,32	\n\t"	/* construct 64-bit...   */
		"or	%1,%1,%L2	\n\t"	/* ... value in tmp register */
		"stdcx.	%1,0,%3		\n\t"	/* store val */
		"bne-	0b		"	/* retry on loss of reservation */
		: "=m"(_p->gasneti_ctr), "=&b"(_tmp)
		: "r"(_val), "r"(_p), "m"(_p->gasneti_ctr)
		: GASNETI_ASM_CR0 );
        }
        #define _gasneti_atomic64_set _gasneti_atomic64_set
        GASNETI_INLINE(_gasneti_atomic64_read)
        uint64_t _gasneti_atomic64_read(gasneti_atomic64_t *_p) {
          uint64_t _retval;	/* gcc allocates a pair of regs for this */
          uint32_t _tmp;
	  /* We are using an extra register with a non-zero upper half as a "canary"
	     to detect when an interruption (context switch, signal handler, etc.) has
	     clobbered the upper halves of the register set.  We pick a value that is
	     zero in the lower half to be insensitive to whether the "clobber" does
	     sign extension or zero extension. */ 
          __asm__ __volatile__ (
		"0:\t"
		"li	%1,0x7fff	\n\t"	/* Canary value in tmp ... */
		"sldi	%1,%1,32	\n\t"   /*  ... = 0x00007FFF.00000000 */
		"ld	%0,%2		\n\t"	/* 64-bit load into "hi" reg of pair */
		"clrldi	%L0,%0,32	\n\t"	/* "lo" reg of pair gets 32 low bits */
		"srdi	%0,%0,32	\n\t"	/* "hi" reg of pair gets 32 high bits */
		"srdi	%1,%1,32	\n\t"	/* Check that upper half of canary... */
		"cmpdi	%1,0x7fff	\n\t"	/*  ... is still 0x00007FFF */
		"bne-	0b		"	/* retry on canary changed */
		: "=&r"(_retval), "=&r"(_tmp)
		: "m"(_p->gasneti_ctr)
		: GASNETI_ASM_CR0 );
          return _retval;
        }
        #define _gasneti_atomic64_read _gasneti_atomic64_read
        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
          GASNETI_ASM_REGISTER_KEYWORD int _result;
	  GASNETI_ASM_REGISTER_KEYWORD uint32_t _tmp;
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
		"0:\t"
		"ldarx    %1,0,%3	\n\t"	/* load memory to tmp */
		"sldi     %0,%5,32	\n\t"	/* shift hi32 half of oldval to result */
		"or       %0,%0,%L5	\n\t"	/*   and or in lo32 of oldval to result */
		"cmpd     0,%0,%1	\n\t"	/* compare memory (tmp) w/ oldval (result) */
		"li	  %0,0		\n\t"	/* assume failure */
		"bne      1f		\n\t"	/* branch to stdcx. on mismatch */
		"li	  %0,1		\n\t"	/* success */
		"sldi     %1,%6,32      \n\t"	/* shift hi32 half of newval to tmp  */
		"or       %1,%1,%L6     \n\t"	/*   and or in lo32 of newval to tmp */
		"1:\t"
		"stdcx.   %1,0,%3	\n\t"	/* try to store tmp (may be newval or read value) */
		"bne-     0b		"	/* retry on loss of reservation */
		: "=&r"(_result), "=&r"(_tmp), "=m"(_p->gasneti_ctr)
		: "r" (_p), "m"(_p->gasneti_ctr), "r"(_oldval), "r"(_newval)
		: GASNETI_ASM_CR0);
          return _result;
        } 
        #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap
        GASNETI_INLINE(_gasneti_atomic64_swap)
        uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _val) {
          uint64_t _retval;	/* gcc allocates a pair of regs for this */
          uint32_t _tmp;
	  /* We are using the ll/sc reservation as a "canary" that will ensure we
	     don't write to memory a value that was clobbered by an interruption
	     (context switch, signal handler, etc.). */
          __asm__ __volatile__ (
		"clrldi	%L3,%L3,32	\n\t"	/* Zap undefined top half of val */
		"0:\t"
		"ldarx	%2,0,%4		\n\t"	/* load retval, establishing reservation */
		"sldi	%1,%3,32	\n\t"	/* construct 64-bit...   */
		"or	%1,%1,%L3	\n\t"	/* ... value in tmp register */
		"clrldi	%L2,%2,32	\n\t"	/* "lo" reg of pair gets 32 low bits */
		"srdi	%2,%2,32	\n\t"	/* "hi" reg of pair gets 32 high bits */
		"stdcx.	%1,0,%4		\n\t"	/* store tmp */
		"bne-	0b		"	/* retry on loss of reservation */
		: "=m"(_p->gasneti_ctr), "=&b"(_tmp), "=&b"(_retval)
		: "r"(_val), "r"(_p), "m"(_p->gasneti_ctr)
		: GASNETI_ASM_CR0 );
          return _retval;
        }
        #define _gasneti_atomic64_swap _gasneti_atomic64_swap
        GASNETI_INLINE(_gasneti_atomic64_fetchadd)
        uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *_p, uint64_t _val) {
          uint64_t _retval;	/* gcc allocates a pair of regs for this */
          uint32_t _tmp;
	  /* We are using the ll/sc reservation as a "canary" that will ensure we
	     don't write to memory a value that was clobbered by an interruption
	     (context switch, signal handler, etc.). */
          __asm__ __volatile__ (
		"clrldi	%L3,%L3,32	\n\t"	/* Zap undefined top half of val */
		"0:\t"
		"ldarx	%2,0,%4		\n\t"	/* load retval, establishing reservation */
		"sldi	%1,%3,32	\n\t"	/* construct 64-bit...   */
		"or	%1,%1,%L3	\n\t"	/* ... value in tmp register */
		"add    %1,%1,%2	\n\t"	/* 64-bit sum in tmp register */
		"clrldi	%L2,%2,32	\n\t"	/* "lo" reg of pair gets 32 low bits */
		"srdi	%2,%2,32	\n\t"	/* "hi" reg of pair gets 32 high bits */
		"stdcx.	%1,0,%4		\n\t"	/* store tmp */
		"bne-	0b		"	/* retry on loss of reservation */
		: "=m"(_p->gasneti_ctr), "=&b"(_tmp), "=&b"(_retval)
		: "r"(_val), "r"(_p), "m"(_p->gasneti_ctr)
		: GASNETI_ASM_CR0 );
          return _retval;
        }
        #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd
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

    #if defined(GASNETI_HAVE_MIPS_REG_AT)
      #define GASNETI_MIPS_AT "$at"
    #elif defined(GASNETI_HAVE_MIPS_REG_1)
      #define GASNETI_MIPS_AT "$1"
    #endif

    #if PLATFORM_OS_LINUX && PLATFORM_COMPILER_GNU
      #define GASNETI_HAVE_ATOMIC32_T 1
      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))

      /* We can't assume GNU as, so no push/pop. */
      #if 1 /* Only gcc supported currently */
        /* Default is at,reorder,macro */
        #define GASNETI_MIPS_START_NOAT        ".set   noat\n\t"
        #define GASNETI_MIPS_END_NOAT          ".set   at\n\t"
        #define GASNETI_MIPS_START_NOREORDER   ".set   noreorder\n\t.set   nomacro\n\t"
        #define GASNETI_MIPS_END_NOREORDER     ".set   reorder\n\t.set   macro\n\t"
        #define GASNETI_MIPS_RETRY(ARGS)       GASNETI_MIPS_BEQZ ARGS "\n\t"
      #else
        #error
      #endif

      /* Macro that expands to either 32- or 64-bit SWAP */
      /* NOTE: evaluates _p multiple times */
      #define __gasneti_atomic_swap_inner(_abi, _ll, _sc, _retval, _p, _newval, _tmp) \
        __asm__ __volatile__(                                                         \
                ".set   " _abi "        \n\t"                                         \
                "1:                     \n\t"                                         \
                "move   %1,%3           \n\t"                                         \
                _ll "   %0,0(%4)        \n\t"                                         \
                _sc "   %1,0(%4)        \n\t"                                         \
                GASNETI_MIPS_RETRY("%1,1b")                                           \
                ".set   mips0           \n\t"                                         \
                : "=&r" (_retval), "=&r" (_tmp), "=m" ((_p)->gasneti_ctr)             \
                : "r" (_newval), "r" (_p), "m" ((_p)->gasneti_ctr) )

      GASNETI_INLINE(_gasneti_atomic32_swap)
      uint32_t _gasneti_atomic32_swap(gasneti_atomic32_t *_p, uint32_t _newval) {
        uint32_t _retval, _tmp;
        __gasneti_atomic_swap_inner("mips2", "ll", "sc", _retval, _p, _newval, _tmp);
	return _retval;
      }
      #define _gasneti_atomic32_swap _gasneti_atomic32_swap

      GASNETI_INLINE(_gasneti_atomic32_fetchadd)
      uint32_t _gasneti_atomic32_fetchadd(gasneti_atomic32_t *_p, uint32_t _op) {
       #ifdef GASNETI_MIPS_AT
	uint32_t _retval;
	__asm__ __volatile__(
		GASNETI_MIPS_START_NOAT
		".set	mips2		\n\t"
		"1:			\n\t"
		"ll	%0,0(%3)	\n\t"
		"addu	" GASNETI_MIPS_AT ",%0,%2	\n\t"
		"sc	" GASNETI_MIPS_AT ",0(%3)	\n\t"
		GASNETI_MIPS_RETRY(GASNETI_MIPS_AT ",1b")
		".set	mips0		\n\t"
		GASNETI_MIPS_END_NOAT
		: "=&r" (_retval), "=m" (_p->gasneti_ctr)
		: "Ir" (_op), "r" (_p), "m" (_p->gasneti_ctr) );
       #else
        /* Don't know how to access $1/$at.  So use another temporary */
        uint32_t _tmp, _retval;
	__asm__ __volatile__(
		".set	mips2		\n\t"
		"1:			\n\t"
		"ll	%0,0(%4)	\n\t"
		"addu	%1,%0,%3	\n\t"
		"sc	%1,0(%4)	\n\t"
		GASNETI_MIPS_RETRY("%1,1b")
		".set	mips0		\n\t"
		: "=&r" (_retval), "=&r" (_tmp), "=m" (_p->gasneti_ctr)
		: "Ir" (_op), "r" (_p), "m" (_p->gasneti_ctr) );
       #endif
	return _retval;
      }
      #define _gasneti_atomic32_fetchadd _gasneti_atomic32_fetchadd

      /* Macro that expands to either 32- or 64-bit CAS */
      /* NOTE: evaluates _p multiple times */
      #define __gasneti_atomic_compare_and_swap_inner(_abi, _ll, _sc, _retval, _p, _oldval, _newval) \
         __asm__ __volatile__ (                                                                \
                "1:                      \n\t"                                                 \
                ".set   " _abi "         \n\t" /* [ set ABI to allow ll/sc ]                */ \
		_ll "   %0,0(%4)         \n\t" /* _retval = *p (starts ll/sc reservation)   */ \
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
                : "=&r" (_retval), "=m" ((_p)->gasneti_ctr)                                    \
                : "Jr" (_oldval), "Jr" (_newval), "r" ((_p)), "m" ((_p)->gasneti_ctr) )

      GASNETI_INLINE(_gasneti_atomic32_compare_and_swap)
      int _gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *_p, uint32_t _oldval, uint32_t _newval) {
        int _retval;
        #if defined(_MIPS_SIM) && (_MIPS_SIM >= 2) /* N32 or 64-bit ABI */
        if (!__builtin_constant_p(_oldval)) {
          /* Ensure oldval is properly sign-extended for comparison to read value */
          __asm__ __volatile__("sll %0,%0,0" : "+r" (_oldval));
        }
        #endif
        __gasneti_atomic_compare_and_swap_inner("mips2", "ll", "sc", _retval, _p, _oldval, _newval);
        return _retval;
      }
      #define _gasneti_atomic32_compare_and_swap _gasneti_atomic32_compare_and_swap

      #if GASNETI_ATOMIC64_NOINLINE
        // Using SLOW or GENERIC alternative
      #elif defined(_MIPS_SIM) && (_MIPS_SIM >= 2) /* N32 or 64-bit ABI */
        #define GASNETI_HAVE_ATOMIC64_T 1
        typedef struct { volatile uint64_t gasneti_ctr; } gasneti_atomic64_t;
        #define gasneti_atomic64_init(v)       { (v) }
        #define _gasneti_atomic64_read(p)      ((p)->gasneti_ctr)
        #define _gasneti_atomic64_set(p,v)     ((p)->gasneti_ctr = (v))

        GASNETI_INLINE(_gasneti_atomic64_swap)
        uint64_t _gasneti_atomic64_swap(gasneti_atomic64_t *_p, uint64_t _newval) {
          uint64_t _retval, _tmp;
          __gasneti_atomic_swap_inner("mips3", "lld", "scd", _retval, _p, _newval, _tmp);
	  return _retval;
        }
        #define _gasneti_atomic64_swap _gasneti_atomic64_swap

        GASNETI_INLINE(_gasneti_atomic64_compare_and_swap)
        int _gasneti_atomic64_compare_and_swap(gasneti_atomic64_t *_p, uint64_t _oldval, uint64_t _newval) {
          int _retval;
          __gasneti_atomic_compare_and_swap_inner("mips3", "lld", "scd", _retval, _p, _oldval, _newval);
          return _retval;
        }
        #define _gasneti_atomic64_compare_and_swap _gasneti_atomic64_compare_and_swap

        GASNETI_INLINE(_gasneti_atomic64_fetchadd)
        uint64_t _gasneti_atomic64_fetchadd(gasneti_atomic64_t *_p, uint64_t _op) {
        #ifdef GASNETI_MIPS_AT
          uint64_t _retval;
          __asm__ __volatile__(
                  GASNETI_MIPS_START_NOAT
                  ".set      mips3           \n\t"
                  "1:                        \n\t"
                  "lld       %0,0(%3)        \n\t"
                  "daddu   " GASNETI_MIPS_AT ",%0,%2        \n\t"
                  "scd     " GASNETI_MIPS_AT ",0(%3)        \n\t"
                  GASNETI_MIPS_RETRY(GASNETI_MIPS_AT ",1b")
                  ".set      mips0           \n\t"
                  GASNETI_MIPS_END_NOAT
                  : "=&r" (_retval), "=m" (_p->gasneti_ctr)
                  : "Ir" (_op), "r" (_p), "m" (_p->gasneti_ctr) );
        #else
          /* Don't know how to access $1/$at.  So use another temporary */
          uint64_t _tmp, _retval;
          __asm__ __volatile__(
                  ".set      mips3           \n\t"
                  "1:                        \n\t"
                  "lld       %0,0(%4)        \n\t"
                  "daddu     %1,%0,%3        \n\t"
                  "scd       %1,0(%4)        \n\t"
                  GASNETI_MIPS_RETRY("%1,1b")
                  ".set      mips0           \n\t"
                  : "=&r" (_retval), "=&r" (_tmp), "=m" (_p->gasneti_ctr)
                  : "Ir" (_op), "r" (_p), "m" (_p->gasneti_ctr) );
        #endif
          return _retval;
        }
        #define _gasneti_atomic64_fetchadd _gasneti_atomic64_fetchadd
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
       *   #define _gasneti_atomic{32,64}_prologue_rmw(p,f)
       #   #define _gasneti_atomic{32,64}_fence_before_rmw(p,f)     _gasneti_atomic_cf_before(f)
       #   #define _gasneti_atomic{32,64}_fence_after_rmw(p,f)      _gasneti_atomic_cf_after(f)
       *   #define _gasneti_atomic{32,64}_fence_after_bool(p,f,v)   _gasneti_atomic_cf_after(f)
       * #endif
       */
      /* No memory fences in our asm, so using default fences */
    #else
      #error "unrecognized MIPS compiler and/or OS - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)"
    #endif
  /* ------------------------------------------------------------------------------------ */
  #elif PLATFORM_ARCH_ARM && defined(GASNETI_HAVE_ARM_CMPXCHG)
    #if PLATFORM_OS_LINUX && (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_CLANG)
      #define GASNETI_HAVE_ATOMIC32_T 1

      typedef struct { volatile uint32_t gasneti_ctr; } gasneti_atomic32_t;
      #define gasneti_atomic32_init(v)       { (v) }
      #define _gasneti_atomic32_read(p)      ((p)->gasneti_ctr)
      #define _gasneti_atomic32_set(p,v)     ((p)->gasneti_ctr = (v))

      // A note regarding use of the "register" keyword:
      //
      // For the GASNETI_HAVE_ARM_CMPXCHG code, we must use a GCC feature known
      // as Explicit Register Variables which in turn leaves us no alternative
      // but to use the "register" keyword even when C++11 and newer have
      // deprecated or reserved it.  Fortunately, g++ and clang++ both allow
      // this particular usage without warnings.

      #define gasneti_atomic32_addfetch_const(p, op) ({                         \
	register unsigned long __sum __asm("r1");                               \
	register unsigned long __ptr __asm("r2") = (unsigned long)(p);          \
	__asm__ __volatile__ (                                                  \
		"0:	ldr	r0, [r2]	@ r0 = *p		\n"     \
		"	add	r1, r0, %2	@ r1 = r0 + op		\n"     \
		GASNETI_ARM_ASMCALL(r3, 0x3f)                                   \
		"	bcc     0b		@ retry on Carry Clear"         \
		: "=&r" (__sum)                                                 \
		: "r" (__ptr), "IL" (op)                                        \
		: "r0", "r3", "ip", "lr", "cc", "memory" );                     \
	(__sum);                                                                \
      })

      GASNETI_INLINE(gasneti_atomic32_increment)
      void gasneti_atomic32_increment(gasneti_atomic32_t *_p, const int _flags) {
        (void)gasneti_atomic32_addfetch_const(_p, 1);
      }
      #define gasneti_atomic32_increment gasneti_atomic32_increment

      GASNETI_INLINE(gasneti_atomic32_decrement)
      void gasneti_atomic32_decrement(gasneti_atomic32_t *_p, const int _flags) {
        (void)gasneti_atomic32_addfetch_const(_p, -1);
      }
      #define gasneti_atomic32_decrement gasneti_atomic32_decrement

      GASNETI_INLINE(gasneti_atomic32_decrement_and_test)
      int gasneti_atomic32_decrement_and_test(gasneti_atomic32_t *_p, const int _flags) {
        return !gasneti_atomic32_addfetch_const(_p, -1);
      }
      #define gasneti_atomic32_decrement_and_test gasneti_atomic32_decrement_and_test

      /* Need to schedule r4, since '=&r' doesn't appear to prevent
       * selection of the same register for __op and __sum.
       * XXX: otherwise could use for inc, dec, dec-and-test
       */
      GASNETI_INLINE(gasneti_atomic32_addfetch)
      uint32_t gasneti_atomic32_addfetch(gasneti_atomic32_t *_p, uint32_t _op, const int _flags) {
	register unsigned long __sum __asm("r1");
	register unsigned long __ptr __asm("r2") = (unsigned long)(_p);
	register unsigned long __op __asm("r4") = _op;

	__asm__ __volatile__ (
		"0:	ldr	r0, [r2]	@ r0 = *p		\n"
		"	add	r1, r0, %2	@ r1 = r0 + op		\n"
		GASNETI_ARM_ASMCALL(r3, 0x3f)
		"	bcc     0b		@ retry on Carry Clear  "
		: "=&r" (__sum)
		: "r" (__ptr), "r" (__op)
		: "r0", "r3", "ip", "lr", "cc", "memory" );
	
    	return __sum;
      }
      #define gasneti_atomic32_addfetch gasneti_atomic32_addfetch

      /* Default impls of add and sub */

      GASNETI_INLINE(gasneti_atomic32_compare_and_swap)
      int gasneti_atomic32_compare_and_swap(gasneti_atomic32_t *_v, int _oldval, int _newval, const int _flags) {
	register unsigned int _result __asm("r0");
	register unsigned int __newval __asm("r1") = _newval;
	register unsigned int __v __asm("r2") = (unsigned long)_v;
	register unsigned int __oldval __asm("r4") = _oldval;

	/* Transient failure is possible if interrupted.
	 * Since we can't distinguish the cause of the failure,
	 * we must retry as long as the failure looks "improper"
	 * which is defined as (!swapped && (v->gasneti_ctr == oldval))
	 */
	__asm__ __volatile__ (
		"0:	mov	r0, r4          @ r0 = oldval              \n"
		GASNETI_ARM_ASMCALL(r3, 0x3f)
	#ifdef __thumb2__
		"	ite	cc		@ THUMB2: If(cc)-Then-Else \n"
	#endif
		"	ldrcc	ip, [r2]	@ if (!swapped) ip=v->gasneti_ctr  \n"
		"	eorcs	ip, r4, #1	@ else ip=oldval^1         \n"
		"	teq	r4, ip		@ if (ip == oldval)        \n"
		"	beq	0b		@    then retry            "
		: "=&r" (_result)
		: "r" (__oldval), "r" (__v), "r" (__newval)
		: "r3", "ip", "lr", "cc", "memory" );

	return !_result;
      } 
      #define gasneti_atomic32_compare_and_swap gasneti_atomic32_compare_and_swap

      GASNETI_INLINE(gasneti_atomic32_swap)
      uint32_t gasneti_atomic32_swap(gasneti_atomic32_t *_p, uint32_t _op, const int _flags) {
        register unsigned long _newval __asm("r1") = _op;
        register unsigned long __p __asm("r2") = (unsigned long)(_p);
        register unsigned long _oldval __asm("r4");
        __asm__ __volatile__ (
                "0:     ldr     r0, [r2]        @ r0 = *p       \n"
                "       mov     r4, r0          @ oldval = r0   \n"
                GASNETI_ARM_ASMCALL(r3, 0x3f)
                "       bcc     0b              @ retry on Carry Clear  "
                : "=&r" (_oldval)
                : "r" (__p), "r" (_newval)
                : "r0", "r3", "ip", "lr", "cc", "memory" );
        return _oldval;
      }
      #define gasneti_atomic32_swap gasneti_atomic32_swap

      /* Linux kernel sources say c-a-s "includes memory barriers as needed"
       * and our code includes a "memory" clobber (compiler fence). */
      #define _gasneti_atomic32_prologue_rmw(p,f)         /*empty*/
      #define _gasneti_atomic32_fence_before_rmw(p,f)     /*empty*/
      #define _gasneti_atomic32_fence_after_rmw(p,f)      /*empty*/
      #define _gasneti_atomic32_fence_after_bool(p,f,v)   /*empty*/
    #else
      #error "unrecognized ARM compiler and/or OS - need to implement GASNet atomics (or #define GASNETI_USE_GENERIC_ATOMICOPS)"
    #endif
  /* ------------------------------------------------------------------------------------ */
  #else
    /* Unknown ARCH will "fall through" to use of the generics */
  #endif
#endif
#undef GASNETI_ASM_REGISTER_KEYWORD
// Not for use beyond this point:
#undef GASNETI_ATOMIC32_NOINLINE
#undef GASNETI_ATOMIC64_NOINLINE

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
  #define GASNETI_ATOMIC32_NOT_SIGNALSAFE	1
  #define GASNETI_BUILD_GENERIC_ATOMIC32	1	/* Build the 32-bit generics */
  #define gasneti_nonatomic32_align		gasneti_genatomic32_align
#endif
#if defined(GASNETI_USE_GENERIC_ATOMIC64) || defined(GASNETI_HYBRID_ATOMIC64)
  #define GASNETI_ATOMIC64_NOT_SIGNALSAFE	1
  #define GASNETI_BUILD_GENERIC_ATOMIC64	1	/* Build the 64-bit generics */
  #define gasneti_nonatomic64_align		gasneti_genatomic64_align
#endif

/* ------------------------------------------------------------------------------------ */
/* Define configuration-dependent choice of locks for generic atomics (if any) */

#if defined(GASNETI_BUILD_GENERIC_ATOMIC32) || defined(GASNETI_BUILD_GENERIC_ATOMIC64)
  #if defined(_INCLUDED_GASNETEX_H) && GASNETI_USE_TRUE_MUTEXES
    /* Case I: Real HSLs in a gasnet client */
    #define GASNETI_GENATOMIC_LOCK_PREP(ptr) \
		gex_HSL_t * const _genatomic_lock = gasneti_hsl_atomic_hash_lookup((uintptr_t)ptr)
    #define GASNETI_GENATOMIC_LOCK()   gex_HSL_Lock(_genatomic_lock)
    #define GASNETI_GENATOMIC_UNLOCK() gex_HSL_Unlock(_genatomic_lock)
    #define _gasneti_genatomic_cons(_id) gasneti_hsl_atomic##_id
  #elif defined(_INCLUDED_GASNETEX_H)
    /* Case II: Empty HSLs in a GASNET_SEQ or GASNET_PARSYNC client w/o conduit-internal threads */
  #elif GASNETI_USE_TRUE_MUTEXES /* thread-safe tools-only client OR forced true mutexes */
    /* Case III: a version for pthreads which is independent of GASNet HSL's */
    #define GASNETI_GENATOMIC_LOCK_PREP(ptr) \
		gasnett_mutex_t * const _genatomic_lock = gasneti_pthread_atomic_hash_lookup((uintptr_t)ptr)
    #define GASNETI_GENATOMIC_LOCK()   gasnett_mutex_lock(_genatomic_lock)
    #define GASNETI_GENATOMIC_UNLOCK() gasnett_mutex_unlock(_genatomic_lock)
    #define _gasneti_genatomic_cons(_id) gasneti_pthread_atomic##_id
  #else
    /* Case IV: Serial gasnet tools client. */
    /* attempt to generate a compile error if pthreads actually are in use */
    #define PTHREAD_MUTEX_INITIALIZER ERROR_include_pthread_h_before_gasnet_tools_h
    extern int pthread_mutex_lock; 
  #endif

  /* Name shift to avoid link conflicts between hsl and pthread versions */
  #ifdef _gasneti_genatomic_cons
    #define gasneti_genatomic32_set                _gasneti_genatomic_cons(32_set)
    #define gasneti_genatomic32_increment          _gasneti_genatomic_cons(32_increment)
    #define gasneti_genatomic32_decrement          _gasneti_genatomic_cons(32_decrement)
    #define gasneti_genatomic32_decrement_and_test _gasneti_genatomic_cons(32_decrement_and_test)
    #define gasneti_genatomic32_compare_and_swap   _gasneti_genatomic_cons(32_compare_and_swap)
    #define gasneti_genatomic32_swap               _gasneti_genatomic_cons(32_swap)
    #define gasneti_genatomic32_addfetch           _gasneti_genatomic_cons(32_addfetch)
    #define gasneti_genatomic64_set                _gasneti_genatomic_cons(64_set)
    #define gasneti_genatomic64_increment          _gasneti_genatomic_cons(64_increment)
    #define gasneti_genatomic64_decrement          _gasneti_genatomic_cons(64_decrement)
    #define gasneti_genatomic64_decrement_and_test _gasneti_genatomic_cons(64_decrement_and_test)
    #define gasneti_genatomic64_compare_and_swap   _gasneti_genatomic_cons(64_compare_and_swap)
    #define gasneti_genatomic64_swap               _gasneti_genatomic_cons(64_swap)
    #define gasneti_genatomic64_addfetch           _gasneti_genatomic_cons(64_addfetch)
    #if PLATFORM_ARCH_32 || defined(GASNETI_HYBRID_ATOMIC64) || defined(GASNETI_UNALIGNED_ATOMIC64)
      /* Need mutex on 64-bit read() to avoid word tearing */
      /* NOTE: defining gasneti_genatomic_read triggers matching behavior in gasnet_atomicops.h */
      #define gasneti_genatomic64_read             _gasneti_genatomic_cons(64_read)
    #endif
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
          type##t _lock;                                                  \
          char _pad[GASNETI_CACHE_PAD(sizeof(type##t))];                  \
        } stem##tbl_t;                                                    \
        extern stem##tbl_t *stem##tbl;                                    \
        extern uintptr_t stem##tbl_mask;                                  \
        extern void stem##tbl_init(void);                                 \
        GASNETI_INLINE(stem##hash_lookup) GASNETI_CONST                   \
        type##t * stem##hash_lookup(uintptr_t _val) {                     \
          /* Step 0. Initialization check */                              \
          if_pf (!stem##tbl_mask) stem##tbl_init();                       \
          else gasneti_local_rmb();                                       \
          /* Step 1.  Mask out the bits within a single cache line */     \
          _val &= ~(((uintptr_t)1 << GASNETI_CACHE_LINE_SHIFT) - 1);      \
          /* Step 2. Fold with xor so all bits influence the lowest 8 */  \
          GASNETI_ATOMIC_LOCK_HASH_64(_val);                              \
          _val ^= (_val >> 16);                                           \
          _val ^= (_val >> 8);                                            \
          /* Step 3. Return the index */                                  \
          return &((stem##tbl[_val & stem##tbl_mask])._lock);             \
        }                                                                 \
        GASNETI_CONSTP(stem##hash_lookup)
    #define GASNETI_ATOMIC_LOCK_TBL_DEFNS(stem,type)                      \
        uintptr_t stem##tbl_mask = 0;                                     \
        stem##tbl_t *stem##tbl = NULL;                                    \
        GASNETI_NEVER_INLINE(stem##tbl_init,                              \
                             extern void stem##tbl_init(void)) {          \
          static type##t stem##tbl_lock = _gasneti_atomic_lock_initializer; \
          _gasneti_atomic_lock_lock(&stem##tbl_lock);                     \
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
              _gasneti_atomic_lock_init(&(stem##tbl[i]._lock));           \
            }                                                             \
            gasneti_local_wmb(); /* enforce locks init before mask is written */ \
            stem##tbl_mask = (stem##tbl_size - 1);                        \
          }                                                               \
          _gasneti_atomic_lock_unlock(&stem##tbl_lock);                   \
        }
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Wrappers for "special" atomics, if any */

#ifdef GASNETI_ATOMIC32_READ_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_read);
  #define _gasneti_atomic32_read (*(uint32_t (*)(gasneti_atomic32_t *))(&_gasneti_special_atomic32_read))
#endif
#ifdef GASNETI_ATOMIC32_SET_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_set);
  #define _gasneti_atomic32_set (*(void (*)(gasneti_atomic32_t *, uint32_t))(&_gasneti_special_atomic32_set))
#endif
#ifdef GASNETI_ATOMIC32_INCREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_increment);
  #define _gasneti_atomic32_increment (*(void (*)(gasneti_atomic32_t *))(&_gasneti_special_atomic32_increment))
#endif
#ifdef GASNETI_ATOMIC32_DECREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_decrement);
  #define _gasneti_atomic32_decrement (*(void (*)(gasneti_atomic32_t *))(&_gasneti_special_atomic32_decrement))
#endif
#ifdef GASNETI_ATOMIC32_DECREMENT_AND_TEST_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_decrement_and_test);
  #define _gasneti_atomic32_decrement_and_test (*(int (*)(gasneti_atomic32_t *))(&_gasneti_special_atomic32_decrement_and_test))
#endif
#ifdef GASNETI_ATOMIC32_COMPARE_AND_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_compare_and_swap);
  #define _gasneti_atomic32_compare_and_swap (*(int (*)(gasneti_atomic32_t *, uint32_t, uint32_t))(&_gasneti_special_atomic32_compare_and_swap))
#endif
#ifdef GASNETI_ATOMIC32_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic32_swap);
  #define _gasneti_atomic32_swap (*(uint32_t (*)(gasneti_atomic32_t *, uint32_t))(&_gasneti_special_atomic32_swap))
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
  #define _gasneti_atomic32_addfetch (*(uint32_t (*)(gasneti_atomic32_t *, uint32_t))(&_gasneti_special_atomic32_addfetch))
#endif

#ifdef GASNETI_ATOMIC64_READ_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_read);
  #define _gasneti_atomic64_read (*(uint64_t (*)(gasneti_atomic64_t *))(&_gasneti_special_atomic64_read))
#endif
#ifdef GASNETI_ATOMIC64_SET_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_set);
  #define _gasneti_atomic64_set (*(void (*)(gasneti_atomic64_t *, uint64_t))(&_gasneti_special_atomic64_set))
#endif
#ifdef GASNETI_ATOMIC64_INCREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_increment);
  #define _gasneti_atomic64_increment (*(void (*)(gasneti_atomic64_t *))(&_gasneti_special_atomic64_increment))
#endif
#ifdef GASNETI_ATOMIC64_DECREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_decrement);
  #define _gasneti_atomic64_decrement (*(void (*)(gasneti_atomic64_t *))(&_gasneti_special_atomic64_decrement))
#endif
#ifdef GASNETI_ATOMIC64_DECREMENT_AND_TEST_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_decrement_and_test);
  #define _gasneti_atomic64_decrement_and_test (*(int (*)(gasneti_atomic64_t *))(&_gasneti_special_atomic64_decrement_and_test))
#endif
#ifdef GASNETI_ATOMIC64_COMPARE_AND_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_compare_and_swap);
  #define _gasneti_atomic64_compare_and_swap (*(int (*)(gasneti_atomic64_t *, uint64_t, uint64_t))(&_gasneti_special_atomic64_compare_and_swap))
#endif
#ifdef GASNETI_ATOMIC64_SWAP_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic64_swap);
  #define _gasneti_atomic64_swap (*(uint64_t (*)(gasneti_atomic64_t *, uint64_t))(&_gasneti_special_atomic64_swap))
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
  #define _gasneti_atomic64_addfetch (*(uint64_t (*)(gasneti_atomic64_t *, uint64_t))(&_gasneti_special_atomic64_addfetch))
#endif

#ifdef GASNETI_ATOMIC_READ_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_read);
  #define _gasneti_atomic_read (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *))(&_gasneti_special_atomic_read))
#endif
#ifdef GASNETI_ATOMIC_SET_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_set);
  #define _gasneti_atomic_set (*(void (*)(gasneti_atomic_t *, gasneti_atomic_val_t))(&_gasneti_special_atomic_increment))
#endif
#ifdef GASNETI_ATOMIC_INCREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_increment);
  #define _gasneti_atomic_increment (*(void (*)(gasneti_atomic_t *))(&_gasneti_special_atomic_increment))
#endif
#ifdef GASNETI_ATOMIC_DECREMENT_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_decrement);
  #define _gasneti_atomic_decrement (*(void (*)(gasneti_atomic_t *))(&_gasneti_special_atomic_decrement))
#endif
#ifdef GASNETI_ATOMIC_DECREMENT_AND_TEST_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_decrement_and_test);
  #define _gasneti_atomic_decrement_and_test (*(int (*)(gasneti_atomic_t *))(&_gasneti_special_atomic_decrement_and_test))
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
  #define _gasneti_atomic_fetchadd (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *, gasneti_atomic_val_t))(&_gasneti_special_atomic_fetchadd))
#endif
#ifdef GASNETI_ATOMIC_ADDFETCH_BODY
  GASNETI_SPECIAL_ASM_DECL(_gasneti_special_atomic_addfetch);
  #define _gasneti_atomic_addfetch (*(gasneti_atomic_val_t (*)(gasneti_atomic_t *, gasneti_atomic_val_t))(&_gasneti_special_atomic_addfetch))
#endif

/* ------------------------------------------------------------------------------------ */
#endif
