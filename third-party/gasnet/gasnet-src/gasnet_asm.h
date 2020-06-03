/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_asm.h $
 * Description: GASNet header for semi-portable inline asm support
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNETEX_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_ASM_H
#define _GASNET_ASM_H

/* Sort out the per-compiler support for asm and atomics */
#ifdef GASNETI_COMPILER_HAS
  #if GASNETI_COMPILER_HAS(GCC_ASM)
      #define GASNETI_HAVE_GCC_ASM 1
  #endif
  #if GASNETI_COMPILER_HAS(SIMPLE_ASM)
      #define GASNETI_HAVE_SIMPLE_ASM 1
  #endif
  #if GASNETI_COMPILER_HAS(SYNC_ATOMICS_32)
      #define GASNETI_HAVE_SYNC_ATOMICS_32 1
  #endif
  #if GASNETI_COMPILER_HAS(SYNC_ATOMICS_64)
      #define GASNETI_HAVE_SYNC_ATOMICS_64 1
  #endif
#else
  #error header inclusion error: missing GASNETI_COMPILER_HAS
#endif

// NOTE:
//
// When adding new support for compilers with usable inline asm, one should
// add to GASNET_CHECK_ASM_SUPPORT() in configure.in as the primary means of
// identifying such support.  The following is used as a secondary mechanism,
// in particular for compilers not probed by configure.
//
// For version-based tests, also check if something should be added to the
// "Sanity Checks" at the emd of this header.
//
#define GASNETI_ASM_AVAILABLE 1
#if GASNETI_HAVE_GCC_ASM
  /* Configure detected support for GCC-style inline asm */
#elif PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_INTEL || PLATFORM_COMPILER_PATHSCALE || \
      PLATFORM_COMPILER_TINY || PLATFORM_COMPILER_OPEN64 || PLATFORM_COMPILER_CLANG || \
      PLATFORM_COMPILER_PGI || \
      (PLATFORM_COMPILER_SUN && PLATFORM_COMPILER_VERSION_GE(5,12,0) && \
       (PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64)) || \
      (PLATFORM_COMPILER_XLC && PLATFORM_ARCH_LITTLE_ENDIAN)
  #define GASNETI_HAVE_GCC_ASM 1
#elif GASNETI_HAVE_SIMPLE_ASM
  /* Configure detected support for asm("mnemonic") */
  /* We only probe compiler families where we trust it (just Sun at this time) */
  #define GASNETI_ASM(mnemonic)  asm(mnemonic)
#elif PLATFORM_COMPILER_SUN_CXX
    // TODO: unknown when/if C++ on Solaris/SPARC begins to support asm()
    #if PLATFORM_OS_LINUX || (PLATFORM_COMPILER_VERSION_GE(5,9,0) && !PLATFORM_ARCH_SPARC)
      #define GASNETI_ASM(mnemonic)  asm(mnemonic)
      #define GASNETI_HAVE_SIMPLE_ASM 1
    #else /* Sun C++ on Solaris (may) lack inline assembly support (man inline) */
      #define GASNETI_ASM(mnemonic)  ERROR_NO_INLINE_ASSEMBLY_AVAIL /* not supported or used */
      #undef GASNETI_ASM_AVAILABLE
    #endif
#elif PLATFORM_COMPILER_SUN_C
    #define GASNETI_ASM(mnemonic)  __asm(mnemonic)
    #define GASNETI_HAVE_SIMPLE_ASM 1 // Equivalent to "simple" with a different spelling
#elif PLATFORM_COMPILER_XLC || PLATFORM_COMPILER_CRAY
  /* platforms where inline assembly not supported or used */
  #define GASNETI_ASM(mnemonic)  ERROR_NO_INLINE_ASSEMBLY_AVAIL 
  #undef GASNETI_ASM_AVAILABLE
  #if PLATFORM_COMPILER_CRAY && PLATFORM_ARCH_X86_64
    #include "intrinsics.h"
  #endif
#else
  #error "Don't know how to use inline assembly for your compiler"
#endif

#if GASNETI_HAVE_GCC_ASM
  #define GASNETI_ASM(mnemonic) __asm__ __volatile__ (mnemonic : : : "memory")
#endif

#ifndef GASNETI_ASM_SPECIAL
  #define GASNETI_ASM_SPECIAL GASNETI_ASM
#endif

//
// Bugs, quirks, dialects, etc.
//

#if PLATFORM_COMPILER_PGI && PLATFORM_ARCH_POWERPC
  #if PLATFORM_COMPILER_VERSION_LT(17,3,0) // All known versions prior to 17.3
    // PGI "tpr 23290"
    // Does not grok the immediate modifier "%I" in an asm template
    #define GASNETI_PGI_ASM_TPR23290 1
  #endif
  #if PLATFORM_COMPILER_PGI_CXX && \
      PLATFORM_COMPILER_VERSION_LT(17,1,0) // All known versions prior to 17.1
    // PGI "tpr 23291"
    // C++ compiler does not grok "cr0", though C compiler does
    #define GASNETI_PGI_ASM_TPR23291 1
  #endif
  #if PLATFORM_COMPILER_PGI_CXX && GASNET_NDEBUG && \
      PLATFORM_COMPILER_VERSION_GE(16,0,0) && PLATFORM_COMPILER_VERSION_LT(17,7,0)
    // PGI "tpr 24514"
    // C++ compiler fails with certain asm constructs at -O2
    #define GASNETI_PGI_ASM_TPR24514 1
  #endif
#elif PLATFORM_COMPILER_PGI /* x86 and x86-64 */
  #if PLATFORM_COMPILER_VERSION_LT(7,2,5)
    #error "GASNet does not support PGI compilers prior to 7.2-5"
  #endif
  #if PLATFORM_ARCH_32 && \
      PLATFORM_COMPILER_VERSION_GE(7,1,5) && PLATFORM_COMPILER_VERSION_LT(8,0,6)
    /* Compiler suffers from "tpr 14969" in which extended asm() output constraints can't
     * be met unless they appear in a specific order.  This is on 32-bit targets only.
     *
     * NOTE: PGI reports TPR 14969 was fixed in 8.0-1.
     * However, we have only been able to test 8.0-6 and later.
     */
    #define GASNETI_PGI_ASM_BUG2294 1
  #endif
  #if PLATFORM_COMPILER_VERSION_GE(7,0,0) && PLATFORM_COMPILER_VERSION_LT(10,8,0)
    /* Compiler suffers from "tpr 17075" in which extended asm() may load only 32 bits of
     * a 64-bit operand at -O1 (but is OK at -O0 and -O2).
     */
    #define GASNETI_PGI_ASM_BUG2843 1
  #endif
  #if PLATFORM_COMPILER_PGI_CXX && PLATFORM_COMPILER_VERSION_GE(17,0,0)
    // C++ compiler generates code that is consistent with having lost the volatile
    // qualifier from the integer member of the atomic type struct.
    #define GASNETI_PGI_ASM_BUG3674 1
  #endif
  #if PLATFORM_COMPILER_PGI_CXX && PLATFORM_COMPILER_VERSION_GT(17,4,0)
    // C++ compiler generates code that promotes 8-bit asm output to
    // 32-bits without clearing the other 24 bits.
    // The work-around is the same as for an older (unrelated) bug 1754.
    // Present in 17.10 and not in 17.4, but uncertain about in between.
    #define GASNETI_PGI_ASM_BUG1754 1
  #endif
#endif

// Compilers with __has_builtin() support (at least clang-derived ones) treat
// the __sync* atomic functions as built-ins, allowing this preprocess-time
// probe.  However, we apply this only to compilers not probed for this support
// by configure.
// Lack of a __builtin prefix on these functions necessitates use of the
// _GASNETI_HAS_BUILTIN macro, not originally intended for use outside
// of gasnet_basic.h.
#if GASNETI_COMPILER_IS_UNKNOWN
  #if _GASNETI_HAS_BUILTIN(__sync_bool_compare_and_swap) && \
      _GASNETI_HAS_BUILTIN(__sync_val_compare_and_swap)  && \
      _GASNETI_HAS_BUILTIN(__sync_fetch_and_add)
    #define GASNETI_HAVE_SYNC_ATOMICS_32 1
    #if PLATFORM_ARCH_64
      #define GASNETI_HAVE_SYNC_ATOMICS_64 1
    #endif
  #endif
#endif

#if PLATFORM_ARCH_ARM && PLATFORM_OS_LINUX
  /* For GASNETI_ARM_ASMCALL() */
  #include "gasnet_arch_arm.h"
#endif

#if PLATFORM_ARCH_MIPS && defined(HAVE_SGIDEFS_H)
  /* For _MIPS_ISA and _MIPS_SIM values on some MIPS platforms */
  #include <sgidefs.h>
#endif

//
// Sanity checks
// For compilers with version-based ASM support here and behavior-based
// logic in configure, we want to know if the version-based is ever
// more permissive than configure-based.
//
#if !GASNETI_COMPILER_IS_UNKNOWN
  #if GASNETI_HAVE_GCC_ASM && !GASNETI_COMPILER_HAS(GCC_ASM)
    #error "Something about your compiler violates GASNet's hard-coded assumptions regarding GCC_ASM support.  Please report this error to gasnet-devel@lbl.gov, including the failing compiler command line and compiler version information."
  #elif GASNETI_HAVE_SIMPLE_ASM && !GASNETI_COMPILER_HAS(SIMPLE_ASM)
    #if PLATFORM_COMPILER_SUN_C
      // Exceptional because configure probe tests a different spelling.
      // C compiler *always* supports `__asm()`, but support for `asm()` is probed.
    #else
      #error "Something about your compiler violates GASNet's hard-coded assumptions regarding SIMPLE_ASM support.  Please report this error to gasnet-devel@lbl.gov, including the failing compiler command line and compiler version information."
    #endif
  #endif
#endif

#endif /* _GASNET_ASM_H */
