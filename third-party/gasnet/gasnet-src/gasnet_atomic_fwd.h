/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_atomic_fwd.h $
 * Description: GASNet header for platform-specific selection of atomic implementation
 * Copyright 2018, Regents of the University of California
 * Terms of use are as specified in license.txt
 *
 * This file is used by gasnet_atomicops.h and configure.
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNETEX_H) && !defined(GASNETI_IN_CONFIGURE)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_ATOMIC_FWD_H
#define _GASNET_ATOMIC_FWD_H

/* ------------------------------------------------------------------------------------ */
// The purpose of this file is to define each of GASNETI_ATOMIC{,32,64}_IMPL to one of
// the following.  Any changes should be kept consistent with GASNET_GET_ATOMIC_IMPL()
// in configure.in.
#define GASNETI_ATOMIC_IMPL_GENERIC 0
#define GASNETI_ATOMIC_IMPL_NATIVE  1
#define GASNETI_ATOMIC_IMPL_OS      2
#define GASNETI_ATOMIC_IMPL_SYNC    3
#define GASNETI_ATOMIC_IMPL_HYBRID  4
#define GASNETI_ATOMIC_IMPL_SPECIAL 5

// *ONLY* when we are using a probed compiler can we use/trust configure probed values
// Otherwise they remain undefined
#if GASNETI_IN_CONFIGURE || !GASNETI_COMPILER_IS_UNKNOWN
  #define GASNETI_CONFIG_ARCH_PPC64             GASNETI_ARCH_PPC64
  #define GASNETI_CONFIG_ARCH_ULTRASPARC        GASNETI_ARCH_ULTRASPARC
  #define GASNETI_CONFIG_HAVE_ARM_CMPXCHG       GASNETI_HAVE_ARM_CMPXCHG
  #define GASNETI_CONFIG_HAVE_SPARC32_64BIT_ASM GASNETI_HAVE_SPARC32_64BIT_ASM
#endif

#if GASNETI_IN_CONFIGURE
  // When in configure provide what gasnet_asm.h would otherwise
  #define GASNETI_HAVE_GCC_ASM           GASNETI_HAVE_CC_GCC_ASM
  #define GASNETI_HAVE_SIMPLE_ASM        GASNETI_HAVE_CC_SIMPLE_ASM
  #define GASNETI_HAVE_SYNC_ATOMICS_32   GASNETI_HAVE_CC_SYNC_ATOMICS_32
  #define GASNETI_HAVE_SYNC_ATOMICS_64   GASNETI_HAVE_CC_SYNC_ATOMICS_64
#endif

/* ------------------------------------------------------------------------------------ */
/* Identify special cases lacking native support */

#if PLATFORM_ARCH_MIPS && defined(HAVE_SGIDEFS_H)
  /* For _MIPS_ISA and _MIPS_SIM values on some MIPS platforms */
  #include <sgidefs.h>
#endif

#if defined(GASNETI_FORCE_GENERIC_ATOMICOPS) || /* for debugging */                \
    (PLATFORM_ARCH_ARM && !defined(GASNETI_CONFIG_HAVE_ARM_CMPXCHG)) ||            \
    (PLATFORM_ARCH_MIPS && defined(_MIPS_ISA) && (_MIPS_ISA < 2)) ||               \
    PLATFORM_COMPILER_TINY ||  /* not worth special case atomics implementation */ \
    PLATFORM_ARCH_MICROBLAZE   /* no atomic instructions */
  #define GASNETI_USE_GENERIC_ATOMICOPS 1
#elif defined(GASNETI_FORCE_OS_ATOMICOPS) /* for debugging */
  #define GASNETI_USE_OS_ATOMICOPS 1
#elif defined(GASNETI_FORCE_COMPILER_ATOMICOPS) || /* for debugging */ \
    (PLATFORM_COMPILER_XLC && GASNETI_HAVE_SYNC_ATOMICS_32) || \
    (PLATFORM_ARCH_SPARC && PLATFORM_COMPILER_CLANG /* TODO: not using GCC ASM due to Bug 3805 */) || \
    PLATFORM_ARCH_AARCH64 || \
    PLATFORM_ARCH_S390 || \
    PLATFORM_ARCH_TILE
  /* TODO: can (should?) do TILE and AARCH64 natively */
  /* TODO: probe for an XLC version with non-broken gcc inline asm support? */
  #define GASNETI_USE_COMPILER_ATOMICOPS 1
#endif

/* ------------------------------------------------------------------------------------ */

// NOTE: This logic *MUST* be kept in sync with gasnet_atomic_bits.h
//
// However, care must be taken with compiler properties which are determined
// using configure probes.  Preprocessor identifiers for these properties
// cannot be used directly here and should be replaced by a GASNET_CONFIG_*
// identifier (one defined above, or new ones added).

#if defined(GASNETI_USE_GENERIC_ATOMICOPS)
  // Nothing to define
#elif defined(GASNETI_USE_COMPILER_ATOMICOPS)
  #if GASNETI_HAVE_SYNC_ATOMICS_32
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_SYNC
    #if PLATFORM_ARCH_64 && GASNETI_HAVE_SYNC_ATOMICS_64
      #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_SYNC
    #endif
  #endif
#elif defined(GASNETI_USE_OS_ATOMICOPS)
  #if PLATFORM_OS_CYGWIN
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_OS
    #if PLATFORM_ARCH_64
      #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_OS
    #endif
  #endif
#elif PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC /* x86 and Athlon64/Opteron and MIC */
  #if (PLATFORM_COMPILER_SUN && !GASNETI_HAVE_GCC_ASM)
    // Native via NON-inline ASM
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_SPECIAL
    #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_SPECIAL
  #elif PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_INTEL || \
      PLATFORM_COMPILER_PATHSCALE || PLATFORM_COMPILER_PGI || \
      PLATFORM_COMPILER_CLANG || PLATFORM_COMPILER_SUN
    // Native via inline ASM
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
  #elif PLATFORM_COMPILER_OPEN64
    // Native via inline ASM, but lacking 64-bit atomics on ILP32
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #if PLATFORM_ARCH_64
      #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #endif
  #elif (PLATFORM_ARCH_X86_64 && PLATFORM_COMPILER_CRAY)
    // Native via compiler intrinsics
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
  #endif
#elif PLATFORM_ARCH_IA64 /* Itanium */
  #if PLATFORM_COMPILER_INTEL || PLATFORM_COMPILER_GNU
    // Native via icc intrinsics or gcc inline asm
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
  #endif
#elif PLATFORM_ARCH_SPARC
  #if defined(__sparcv9) || defined(__sparcv9cpu) || \
      defined(__sparc_v9__) || defined(GASNETI_CONFIG_ARCH_ULTRASPARC) /* SPARC v9 ISA */
    #if PLATFORM_COMPILER_GNU
      // Native via inline ASM
      #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
      #if PLATFORM_ARCH_64 || GASNETI_CONFIG_HAVE_SPARC32_64BIT_ASM
        #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
      #endif
    #elif PLATFORM_COMPILER_SUN
      // Native via NON-inline ASM
      #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_SPECIAL
      #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_SPECIAL
    #endif
  #endif
#elif PLATFORM_ARCH_POWERPC
  #if GASNETI_HAVE_GCC_ASM
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #if PLATFORM_ARCH_64
      #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #elif defined(GASNETI_CONFIG_ARCH_PPC64) // ILP32 on PPC64
      #if PLATFORM_OS_DARWIN
        #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_HYBRID
      #elif PLATFORM_OS_LINUX
        #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
      #endif
    #endif
  #endif
#elif PLATFORM_ARCH_MIPS
  #if PLATFORM_OS_LINUX && PLATFORM_COMPILER_GNU
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #if defined(_MIPS_SIM) && (_MIPS_SIM >= 2) /* N32 or 64-bit ABI */
      #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_NATIVE
    #endif
  #endif
#elif PLATFORM_ARCH_ARM && defined(GASNETI_CONFIG_HAVE_ARM_CMPXCHG)
  #if PLATFORM_OS_LINUX && (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_CLANG)
    #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_NATIVE
  #endif
  /* ------------------------------------------------------------------------------------ */
#else
  /* Unknown ARCH will "fall through" to use of the generics */
#endif

// Apply defaults
#ifndef GASNETI_ATOMIC32_IMPL
  #define GASNETI_ATOMIC32_IMPL GASNETI_ATOMIC_IMPL_GENERIC
#endif
#ifndef GASNETI_ATOMIC64_IMPL
  #define GASNETI_ATOMIC64_IMPL GASNETI_ATOMIC_IMPL_GENERIC
#endif
#ifndef GASNETI_ATOMIC_IMPL
  #if GASNETI_FORCE_64BIT_ATOMICOPS
    #define GASNETI_ATOMIC_IMPL GASNETI_ATOMIC64_IMPL
  #else
    #define GASNETI_ATOMIC_IMPL GASNETI_ATOMIC32_IMPL
  #endif
#endif

#undef GASNETI_CONFIG_ARCH_PPC64
#undef GASNETI_CONFIG_ARCH_ULTRASPARC
#undef GASNETI_CONFIG_HAVE_ARM_CMPXCHG
#undef GASNETI_CONFIG_HAVE_SPARC32_64BIT_ASM

// Sanity checks on relationship between 32- and 64-bit implementations:
//  1) 64-bit atomics are either same-as-32-bit, GENERIC or HYBRID
//  2) 64-bit NATIVE implies 32-bit NATIVE
//  3) 64-bit SYNC   implies 32-bit SYNC
//  4) 64-bit OS     implies 32-bit OS
//  5) 64-bit HYBRID implies 32-bit NATIVE
//  6) 32-bit HYBRID does not exist
#if /* Rule #1: 64-bit is one of (32-bit, GENERIC, HYBRID) */    \
    ((GASNETI_ATOMIC64_IMPL != GASNETI_ATOMIC32_IMPL)       &&   \
     (GASNETI_ATOMIC64_IMPL != GASNETI_ATOMIC_IMPL_GENERIC) &&   \
     (GASNETI_ATOMIC64_IMPL != GASNETI_ATOMIC_IMPL_HYBRID)) ||   \
    /* Rule #2: 64-bit NATIVE implies 32-bit NATIVE */           \
    ((GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_NATIVE) &&    \
     (GASNETI_ATOMIC32_IMPL != GASNETI_ATOMIC_IMPL_NATIVE)) ||   \
    /* Rule #3: 64-bit SYNC implies 32-bit SYNC */               \
    ((GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_SYNC) &&      \
     (GASNETI_ATOMIC32_IMPL != GASNETI_ATOMIC_IMPL_SYNC)) ||     \
    /* Rule #4: 64-bit OS implies 32-bit OS */                   \
    ((GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_OS) &&        \
     (GASNETI_ATOMIC32_IMPL != GASNETI_ATOMIC_IMPL_OS)) ||       \
    /* Rule #5: 64-bit HYBRID implies 32-bit NATIVE */           \
    ((GASNETI_ATOMIC64_IMPL == GASNETI_ATOMIC_IMPL_HYBRID) &&    \
     (GASNETI_ATOMIC32_IMPL != GASNETI_ATOMIC_IMPL_NATIVE)) ||   \
    /* Rule #6: 32-bit HYBRID does not exist */                  \
     (GASNETI_ATOMIC32_IMPL == GASNETI_ATOMIC_IMPL_HYBRID)
  #error Internal error - unexpected atomics configuration
#endif

#endif // _GASNET_ATOMIC_FWD_H
