/*   $Source: /var/local/cvs/gasnet/gasnet_basic.h,v $
 *     $Date: 2010/05/04 01:45:13 $
 * $Revision: 1.98.2.4 $
 * Description: GASNet basic header utils
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnet.h or gasnet_tools.h
#endif

#ifndef _GASNET_BASIC_H
#define _GASNET_BASIC_H

/* ------------------------------------------------------------------------------------ */
/* must precede everything else to ensure correct operation */
#include "portable_inttypes.h"
#include "portable_platform.h"

/* This is needed for _MIPS_ISA_* on MIPS platforms */
#if PLATFORM_ARCH_MIPS && defined(HAVE_SGIDEFS_H)
# include <sgidefs.h>
#endif

/* try to recognize the compiler in use as one present at configure time.
   note this can set both GASNETI_COMPILER_IS_CC and
   GASNETI_COMPILER_IS_MPI_CC when CC and MPI_CC present the same ID.
 */
#if PLATFORM_COMPILER_ID == GASNETI_PLATFORM_COMPILER_ID && \
    PLATFORM_COMPILER_VERSION == GASNETI_PLATFORM_COMPILER_VERSION
  #define GASNETI_COMPILER_IS_CC 1
#endif
#if PLATFORM_COMPILER_ID == GASNETI_PLATFORM_MPI_CC_ID && \
    PLATFORM_COMPILER_VERSION == GASNETI_PLATFORM_MPI_CC_VERSION
  #define GASNETI_COMPILER_IS_MPI_CC 1
#endif
#if PLATFORM_COMPILER_ID == GASNETI_PLATFORM_CXX_ID && \
    PLATFORM_COMPILER_VERSION == GASNETI_PLATFORM_CXX_VERSION
  #define GASNETI_COMPILER_IS_CXX 1
#endif
#if !defined(GASNETI_COMPILER_IS_CC) && \
    !defined(GASNETI_COMPILER_IS_CXX) && \
    !defined(GASNETI_COMPILER_IS_MPI_CC)
  #define GASNETI_COMPILER_IS_UNKNOWN 1
#endif

#ifndef GASNETI_COMPILER_IS_CC
  /* detect when the compiler in use differs from the $CC tested by configure,
     indicating some of the configure-detected results may be invalid for this compilation
     this is permitted in certain VERY limited contexts, and activates conservative assumptions

     As of 2010-01-27 this has only two remaining uses:
     + GASNETI_THREADKEY_*
       TLS support will conservatively use library calls if compiler has changed
     + GASNETT_CONFIGURE_MISMATCH
       exported for use by client code

     Note that in the common case that $MPI_CC presents the same ID as $CC
     this will not be set simply becuase one compiling with $MPI_CC.
   */
  #define GASNETI_CONFIGURE_MISMATCH 1
#endif

/* include files that may conflict with macros defined later */
#ifdef HAVE_SYS_PARAM_H
  #include <sys/param.h>
#endif

#if PLATFORM_ARCH_32
  #define GASNETI_PTR_CONFIG 32bit
#elif PLATFORM_ARCH_64
  #define GASNETI_PTR_CONFIG 64bit
#else
  #error GASNet currently only supports 32-bit and 64-bit platforms
#endif

/* miscellaneous macro helpers */
#ifdef __cplusplus
  #define GASNETI_BEGIN_EXTERNC extern "C" {
  #define GASNETI_EXTERNC       extern "C" 
  #define GASNETI_END_EXTERNC   }
#else
  #define GASNETI_BEGIN_EXTERNC 
  #define GASNETI_EXTERNC       
  #define GASNETI_END_EXTERNC 
#endif

/* Some symbols need a tentative definition when building libgasnet_tools-*.a.
 * However we want an extern definition in libgasnet-*.a and all clients.
 * This includes C++ clients where tentative definitions are not supported.
 */
#if defined(GASNETT_BUILDING_TOOLS)
  #define GASNETI_TENTATIVE_EXTERN /*empty*/
#else
  #define GASNETI_TENTATIVE_EXTERN extern
#endif

/* pick up restrict keyword (or empty) appropriate for compiler in use
    OR
   client overrides of restict keywords:
     GASNETT_USE_RESTRICT gives the (possibly empty) keyword to use
     GASNETT_USE_RESTRICT_ON_TYPEDEFS boolean
*/
#ifdef GASNETT_USE_RESTRICT
  #define GASNETI_RESTRICT GASNETT_USE_RESTRICT
  #if GASNETT_USE_RESTRICT_ON_TYPEDEFS
    #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS 1
  #else
    #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS 0
  #endif
#elif defined(GASNETT_USE_RESTRICT_ON_TYPEDEFS)
  #error GASNETT_USE_RESTRICT_ON_TYPEDEFS defined without GASNETT_USE_RESTRICT
#elif GASNETI_COMPILER_IS_CC
  #define GASNETI_RESTRICT                      GASNETI_CC_RESTRICT
  #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS GASNETI_CC_RESTRICT_MAY_QUALIFY_TYPEDEFS
#elif GASNETI_COMPILER_IS_MPI_CC
  #define GASNETI_RESTRICT                      GASNETI_MPI_CC_RESTRICT
  #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS GASNETI_MPI_CC_RESTRICT_MAY_QUALIFY_TYPEDEFS
#elif GASNETI_COMPILER_IS_CXX
  #define GASNETI_RESTRICT                      GASNETI_CXX_RESTRICT
  #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS GASNETI_CXX_RESTRICT_MAY_QUALIFY_TYPEDEFS
#else
  /* define away to nothing, which should always be safe */
  #define GASNETI_RESTRICT
  /* define to 1 because 0 triggers use of (void*) in place of the typedef */
  #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS 1
#endif

#ifndef _STRINGIFY
#define _STRINGIFY_HELPER(x) #x
#define _STRINGIFY(x) _STRINGIFY_HELPER(x)
#endif

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif

#ifndef MIN
#define MIN(x,y)  ((x)<(y)?(x):(y))
#endif
#ifndef MAX
#define MAX(x,y)  ((x)>(y)?(x):(y))
#endif

#include <stddef.h> /* get standard types, esp size_t */

/* splitting and reassembling 64-bit quantities */
#define GASNETI_MAKEWORD(hi,lo) ((((uint64_t)(hi)) << 32) | (((uint64_t)(lo)) & 0xFFFFFFFF))
#define GASNETI_HIWORD(arg)     ((uint32_t)(((uint64_t)(arg)) >> 32))
#if PLATFORM_COMPILER_INTEL
  /* This form avoids a #69 truncation warning while generating identical code */
  #define GASNETI_LOWORD(arg)     ((uint32_t)((uint64_t)(arg) & 0xFFFFFFFF))
#else
  #define GASNETI_LOWORD(arg)     ((uint32_t)((uint64_t)(arg)))
#endif

/* Non-asserting alignment macros
 * Use for instance in
 *    char buffer[GASNETI_ALIGNUP_NOASSERT(sizeof(struct foo), GASNETI_CACHE_LINE_BYTES)];
 * where the expression must be a compile-time constant (no assertion).
 * One should really use the asserting versions when possible.
 */
#define GASNETI_ALIGNDOWN_NOASSERT(p,P)   (((uintptr_t)(p))&~((uintptr_t)((P)-1)))
#define GASNETI_ALIGNUP_NOASSERT(p,P)     (GASNETI_ALIGNDOWN_NOASSERT((uintptr_t)(p)+((uintptr_t)((P)-1)),P))

/* alignment macros */
#define GASNETI_POWEROFTWO(P)    (((P)&((P)-1)) == 0)

#define GASNETI_ALIGNDOWN(p,P)    (gasneti_assert(GASNETI_POWEROFTWO(P)), \
                                   GASNETI_ALIGNDOWN_NOASSERT((p),(P)))
#define GASNETI_ALIGNUP(p,P)     (GASNETI_ALIGNDOWN((uintptr_t)(p)+((uintptr_t)((P)-1)),P))

#define GASNETI_PAGE_ALIGNDOWN(p) (GASNETI_ALIGNDOWN(p,GASNET_PAGESIZE))
#define GASNETI_PAGE_ALIGNUP(p)   (GASNETI_ALIGNUP(p,GASNET_PAGESIZE))

/* GASNETI_CACHE_PAD() */
#if 0
  /* This version can yield 0-byte padding, which upsets some compilers */
  #define GASNETI_CACHE_PAD(SZ) (GASNETI_ALIGNUP_NOASSERT((SZ),GASNETI_CACHE_LINE_BYTES)-(SZ))
#else
  #define GASNETI_CACHE_PAD(SZ) (GASNETI_ALIGNUP_NOASSERT((SZ+1),GASNETI_CACHE_LINE_BYTES)-(SZ))
#endif

#ifndef GASNET_PAGESIZE
  #ifdef GASNETI_PAGESIZE
    #define GASNET_PAGESIZE GASNETI_PAGESIZE
  #elif PLATFORM_ARCH_CRAYT3E
    /* on Cray: shmemalign allocates mem aligned across nodes, 
        but there seems to be no fixed page size (man pagesize)
        this is probably because they don't support VM
       actual page size is set separately for each linker section, 
        ranging from 512KB(default) to 8MB
       Here we return 8 to reflect the lack of page alignment constraints
       (for basic sanity, we want page alignment >= MAX(cache line,reqd double alignment))
   */

    #define GASNET_PAGESIZE GASNETI_CACHE_LINE_BYTES
  #else
    #error GASNET_PAGESIZE unknown and not set by conduit
  #endif
  #if GASNET_PAGESIZE <= 0
    #error bad defn of GASNET_PAGESIZE
  #endif
#endif

/* special GCC features */
#if PLATFORM_COMPILER_PGI && defined(__attribute__)
#undef __attribute__ /* bug 1766: undo a stupid, gcc-centric definition from Linux sys/cdefs.h */
#endif

/* work around bug 1620 unless client has explicitly set GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE */
#if PLATFORM_COMPILER_PATHSCALE && !defined(GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE)
  #define GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE 0
#endif

#if PLATFORM_COMPILER_SGI_CXX
  #define GASNETI_PRAGMA(x) /* despite the docs, not supported in MIPSPro C++ */
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION < 742
  #define GASNETI_PRAGMA(x) /* bug1555: broken in older versions (740 fails, 742 works) */
#elif PLATFORM_COMPILER_COMPAQ_C && __DECC_VER < 60590207
  #define GASNETI_PRAGMA(x) /* not supported in older versions (60490014) */
#elif PLATFORM_COMPILER_SUN_C && __SUNPRO_C < 0x570
  #define GASNETI_PRAGMA(x) /* not supported in older versions (550 fails, 570 works) */
#else
  #define GASNETI_PRAGMA(x) _Pragma ( #x )
#endif

/* If we have recognized the compiler, pick up its attribute support */
#if GASNETI_COMPILER_IS_CC && GASNETI_HAVE_CC_ATTRIBUTE
  #define GASNETI_HAVE_GCC_ATTRIBUTE 1
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE GASNETI_HAVE_CC_ATTRIBUTE_ALWAYSINLINE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_NOINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_NOINLINE GASNETI_HAVE_CC_ATTRIBUTE_NOINLINE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_MALLOC
    #define GASNETT_USE_GCC_ATTRIBUTE_MALLOC GASNETI_HAVE_CC_ATTRIBUTE_MALLOC
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT
    #define GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT GASNETI_HAVE_CC_ATTRIBUTE_WARNUNUSEDRESULT
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_USED
    #define GASNETT_USE_GCC_ATTRIBUTE_USED GASNETI_HAVE_CC_ATTRIBUTE_USED
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_UNUSED
    #define GASNETT_USE_GCC_ATTRIBUTE_UNUSED GASNETI_HAVE_CC_ATTRIBUTE_UNUSED
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS
    #define GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS GASNETI_HAVE_CC_ATTRIBUTE_MAYALIAS
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_NORETURN
    #define GASNETT_USE_GCC_ATTRIBUTE_NORETURN GASNETI_HAVE_CC_ATTRIBUTE_NORETURN
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_PURE
    #define GASNETT_USE_GCC_ATTRIBUTE_PURE GASNETI_HAVE_CC_ATTRIBUTE_PURE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_CONST
    #define GASNETT_USE_GCC_ATTRIBUTE_CONST GASNETI_HAVE_CC_ATTRIBUTE_CONST
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_FORMAT
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT GASNETI_HAVE_CC_ATTRIBUTE_FORMAT
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR GASNETI_HAVE_CC_ATTRIBUTE_FORMAT_FUNCPTR
  #endif
#elif GASNETI_COMPILER_IS_MPI_CC && GASNETI_HAVE_MPI_CC_ATTRIBUTE
  #define GASNETI_HAVE_GCC_ATTRIBUTE 1
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE GASNETI_HAVE_MPI_CC_ATTRIBUTE_ALWAYSINLINE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_NOINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_NOINLINE GASNETI_HAVE_MPI_CC_ATTRIBUTE_NOINLINE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_MALLOC
    #define GASNETT_USE_GCC_ATTRIBUTE_MALLOC GASNETI_HAVE_MPI_CC_ATTRIBUTE_MALLOC
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT
    #define GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT GASNETI_HAVE_MPI_CC_ATTRIBUTE_WARNUNUSEDRESULT
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_USED
    #define GASNETT_USE_GCC_ATTRIBUTE_USED GASNETI_HAVE_MPI_CC_ATTRIBUTE_USED
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_UNUSED
    #define GASNETT_USE_GCC_ATTRIBUTE_UNUSED GASNETI_HAVE_MPI_CC_ATTRIBUTE_UNUSED
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS
    #define GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS GASNETI_HAVE_MPI_CC_ATTRIBUTE_MAYALIAS
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_NORETURN
    #define GASNETT_USE_GCC_ATTRIBUTE_NORETURN GASNETI_HAVE_MPI_CC_ATTRIBUTE_NORETURN
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_PURE
    #define GASNETT_USE_GCC_ATTRIBUTE_PURE GASNETI_HAVE_MPI_CC_ATTRIBUTE_PURE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_CONST
    #define GASNETT_USE_GCC_ATTRIBUTE_CONST GASNETI_HAVE_MPI_CC_ATTRIBUTE_CONST
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_FORMAT
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT GASNETI_HAVE_MPI_CC_ATTRIBUTE_FORMAT
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR GASNETI_HAVE_MPI_CC_ATTRIBUTE_FORMAT_FUNCPTR
  #endif
#elif GASNETI_COMPILER_IS_CXX && GASNETI_HAVE_CXX_ATTRIBUTE
  #define GASNETI_HAVE_GCC_ATTRIBUTE 1
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE GASNETI_HAVE_CXX_ATTRIBUTE_ALWAYSINLINE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_NOINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_NOINLINE GASNETI_HAVE_CXX_ATTRIBUTE_NOINLINE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_MALLOC
    #define GASNETT_USE_GCC_ATTRIBUTE_MALLOC GASNETI_HAVE_CXX_ATTRIBUTE_MALLOC
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT
    #define GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT GASNETI_HAVE_CXX_ATTRIBUTE_WARNUNUSEDRESULT
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_USED
    #define GASNETT_USE_GCC_ATTRIBUTE_USED GASNETI_HAVE_CXX_ATTRIBUTE_USED
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_UNUSED
    #define GASNETT_USE_GCC_ATTRIBUTE_UNUSED GASNETI_HAVE_CXX_ATTRIBUTE_UNUSED
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS
    #define GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS GASNETI_HAVE_CXX_ATTRIBUTE_MAYALIAS
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_NORETURN
    #define GASNETT_USE_GCC_ATTRIBUTE_NORETURN GASNETI_HAVE_CXX_ATTRIBUTE_NORETURN
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_PURE
    #define GASNETT_USE_GCC_ATTRIBUTE_PURE GASNETI_HAVE_CXX_ATTRIBUTE_PURE
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_CONST
    #define GASNETT_USE_GCC_ATTRIBUTE_CONST GASNETI_HAVE_CXX_ATTRIBUTE_CONST
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_FORMAT
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT GASNETI_HAVE_CXX_ATTRIBUTE_FORMAT
  #endif
  #ifndef GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR GASNETI_HAVE_CXX_ATTRIBUTE_FORMAT_FUNCPTR
  #endif
#endif

/* GASNETI_WARN_UNUSED_RESULT: warn if function's return value is ignored */
#if GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT
  #define GASNETI_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#else
  #define GASNETI_WARN_UNUSED_RESULT
#endif

/* GASNETI_MALLOC: assert return value is unaliased, and should not be ignored */
#if GASNETT_USE_GCC_ATTRIBUTE_MALLOC 
  #define GASNETI_MALLOC __attribute__((__malloc__)) GASNETI_WARN_UNUSED_RESULT
#else
  #define GASNETI_MALLOC GASNETI_WARN_UNUSED_RESULT
#endif
/* pragma version of GASNETI_MALLOC */
#if PLATFORM_COMPILER_SUN_C
  #define GASNETI_MALLOCP(fnname) GASNETI_PRAGMA(returns_new_memory(fnname))
#elif PLATFORM_COMPILER_HP_C && !PLATFORM_ARCH_IA64
  #define GASNETI_MALLOCP(fnname) GASNETI_PRAGMA(ALLOCS_NEW_MEMORY fnname)
#else
  #define GASNETI_MALLOCP(fnname)
#endif

/* GASNETI_USED: assert that function is used and must not be ommited from object file */
#if GASNETT_USE_GCC_ATTRIBUTE_USED
  #define GASNETI_USED __attribute__((__used__))
#else
  #define GASNETI_USED 
#endif

/* GASNETI_UNUSED: assert that variable is potentially unused to avoid unused variable warnings */
#if GASNETT_USE_GCC_ATTRIBUTE_UNUSED
  #define GASNETI_UNUSED __attribute__((__unused__))
#else
  #define GASNETI_UNUSED 
#endif

/* GASNETI_MAY_ALIAS: annotate type as not subject to ANSI aliasing rules */
#if GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS
  #define GASNETI_MAY_ALIAS __attribute__((__may_alias__))
#else
  #define GASNETI_MAY_ALIAS 
  /* may_alias attribute is sometimes required for correctness */
  #if PLATFORM_COMPILER_GNU && PLATFORM_COMPILER_VERSION_GE(4,4,0) && !GASNETI_BUG1389_WORKAROUND
    #error "GCC's __may_alias__ attribute is required for correctness in gcc >= 4.4, but is disabled or unsupported."
  #endif
#endif

/* GASNETI_NORETURN: assert that function does not return to caller */
#if GASNETT_USE_GCC_ATTRIBUTE_NORETURN
  #define GASNETI_NORETURN __attribute__((__noreturn__))
#else
  #define GASNETI_NORETURN 
#endif
/* pragma version of GASNETI_NORETURN */
#if PLATFORM_COMPILER_SUN_C
  #define GASNETI_NORETURNP(fnname) GASNETI_PRAGMA(does_not_return(fnname))
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION >= 720 && _MIPS_SIM != _ABIO32
  #define GASNETI_NORETURNP(fnname) GASNETI_PRAGMA(mips_frequency_hint NEVER fnname)
#elif PLATFORM_COMPILER_COMPAQ_C /* not Compaq C++ */
  #define GASNETI_NORETURNP(fnname) GASNETI_PRAGMA(assert func_attrs(fnname) noreturn)
#elif PLATFORM_COMPILER_XLC && 0
  /* this *should* work but it causes bizarre compile failures, so disable it */
  #define GASNETI_NORETURNP(fnname) GASNETI_PRAGMA(leaves(fnname))
#else
  #define GASNETI_NORETURNP(fnname)
#endif

/* GASNETI_PURE: assert that function is "pure" */
  /* pure function: one with no effects except the return value, and 
   * return value depends only on the parameters and/or global variables.
   * prohibited from performing volatile accesses, compiler fences, I/O,
   * changing any global variables (including statically scoped ones), or
   * calling any functions that do so
   */
#if GASNETT_USE_GCC_ATTRIBUTE_PURE
  #define GASNETI_PURE __attribute__((__pure__))
#else
  #define GASNETI_PURE 
#endif
/* pragma version of GASNETI_PURE */
#if PLATFORM_COMPILER_XLC && \
   !(PLATFORM_OS_DARWIN && __xlC__ <= 0x0600) /* bug 1542 */
  #define GASNETI_PUREP(fnname) GASNETI_PRAGMA(isolated_call(fnname))
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION >= 710
  #define GASNETI_PUREP(fnname) GASNETI_PRAGMA(no side effects (fnname))
#elif PLATFORM_COMPILER_COMPAQ
  #define GASNETI_PUREP(fnname) \
          GASNETI_PRAGMA(assert func_attrs(fnname) noeffects file_scope_vars(nowrites))
#elif PLATFORM_COMPILER_HP_C && !PLATFORM_ARCH_IA64
  #define GASNETI_PUREP(fnname) GASNETI_PRAGMA(NO_SIDE_EFFECTS fnname)
#else
  #define GASNETI_PUREP(fnname) 
#endif

/* GASNETI_PURE: assert that function is "const" */
  /* const function: a more restricted form of pure function, with all the
   * same restrictions, except additionally the return value must NOT
   * depend on global variables or anything pointed to by the arguments
   */
#if GASNETT_USE_GCC_ATTRIBUTE_CONST
  #define GASNETI_CONST __attribute__((__const__))
#else
  #define GASNETI_CONST GASNETI_PURE
#endif
/* pragma version of GASNETI_CONST */
#if PLATFORM_COMPILER_SUN_C
  #define GASNETI_CONSTP(fnname) GASNETI_PRAGMA(no_side_effect(fnname))
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION >= 730
  #define GASNETI_CONSTP(fnname) GASNETI_PRAGMA(pure (fnname))
#elif PLATFORM_COMPILER_COMPAQ
  #define GASNETI_CONSTP(fnname) \
          GASNETI_PRAGMA(assert func_attrs(fnname) nostate noeffects file_scope_vars(none))
#else
  #define GASNETI_CONSTP(fnname) GASNETI_PUREP(fnname)
#endif

/* GASNETI_ALWAYS_INLINE: force inlining of function if possible */
#if GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE
  /* bug1525: gcc's __always_inline__ attribute appears to be maximally aggressive */
  #define _GASNETI_ALWAYS_INLINE(fnname) __attribute__((__always_inline__))
#elif PLATFORM_COMPILER_CRAY_C
  /* the only way to request inlining a particular fn in Cray C */
  /* possibly should be using inline_always here */
  #define _GASNETI_ALWAYS_INLINE(fnname) GASNETI_PRAGMA(_CRI inline fnname)
#elif PLATFORM_COMPILER_MTA
  #define _GASNETI_ALWAYS_INLINE(fnname) GASNETI_PRAGMA(mta inline)
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION >= 710
  #define _GASNETI_ALWAYS_INLINE(fnname) GASNETI_PRAGMA(inline global fnname)
#elif PLATFORM_COMPILER_COMPAQ_C /* not Compaq C++ */
  #define _GASNETI_ALWAYS_INLINE(fnname) GASNETI_PRAGMA(inline (fnname))
#elif PLATFORM_COMPILER_HP_C && GASNET_NDEBUG /* avoid a warning */ \
   && 0 /* unreliable behavior - Itanium optimizer crashes and 
           PARISC syntax errors unless it appears on a line by itself */
  #define _GASNETI_ALWAYS_INLINE(fnname) GASNETI_PRAGMA(INLINE fnname)
  #undef STATIC_INLINE_WORKS
#else
  #define _GASNETI_ALWAYS_INLINE(fnname)
#endif

/* GASNETI_PLEASE_INLINE: Inline a function if possible, but don't generate an error 
 * for cases where it is impossible (eg recursive functions)
 */
#if GASNET_DEBUG
  #define GASNETI_PLEASE_INLINE(fnname) static
#elif defined(GASNETT_USE_PLEASE_INLINE)
  #define GASNETI_PLEASE_INLINE(fnname) GASNETT_USE_PLEASE_INLINE(fnname)
#elif defined(__cplusplus)
  #define GASNETI_PLEASE_INLINE(fnname) inline
#elif GASNETI_COMPILER_IS_CC && defined(GASNET_CC_INLINE_MODIFIER)
  #define GASNETI_PLEASE_INLINE(fnname) GASNET_CC_INLINE_MODIFIER
#elif GASNETI_COMPILER_IS_MPI_CC && defined(GASNET_MPI_CC_INLINE_MODIFIER)
  #define GASNETI_PLEASE_INLINE(fnname) GASNET_MPI_CC_INLINE_MODIFIER
#else
  #define GASNETI_PLEASE_INLINE(fnname) static
#endif

/* GASNETI_ALWAYS_INLINE aka GASNETI_INLINE: Most forceful inlining demand available.
 * Might generate errors in cases where inlining is semantically impossible 
 * (eg recursive functions, varargs fns)
 */
#if GASNET_DEBUG
  #define GASNETI_ALWAYS_INLINE(fnname) static
#else
  #define GASNETI_ALWAYS_INLINE(fnname) _GASNETI_ALWAYS_INLINE(fnname) GASNETI_PLEASE_INLINE(fnname)
#endif
#define GASNETI_INLINE(fnname) GASNETI_ALWAYS_INLINE(fnname)

/* GASNETI_NEVER_INLINE: Most forceful demand available to disable inlining for function.
 */
#if GASNETT_USE_GCC_ATTRIBUTE_NOINLINE
  #define GASNETI_NEVER_INLINE(fnname,declarator) __attribute__((__noinline__)) declarator
#elif PLATFORM_COMPILER_SUN_C
  #define GASNETI_NEVER_INLINE(fnname,declarator) declarator; GASNETI_PRAGMA(no_inline(fnname)) declarator
#elif PLATFORM_COMPILER_CRAY
  #define GASNETI_NEVER_INLINE(fnname,declarator) GASNETI_PRAGMA(_CRI inline_never fnname) declarator
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION >= 710
  #define GASNETI_NEVER_INLINE(fnname,declarator) GASNETI_PRAGMA(noinline global fnname) declarator
#elif PLATFORM_COMPILER_COMPAQ
  #define GASNETI_NEVER_INLINE(fnname,declarator) GASNETI_PRAGMA(noinline (fnname)) declarator
#elif PLATFORM_COMPILER_HP_C && GASNET_NDEBUG /* avoid a warning */ \
   && PLATFORM_ARCH_IA64 /* unreliable behavior on PARISC unless it appears on a line by itself */
  #define GASNETI_NEVER_INLINE(fnname,declarator) GASNETI_PRAGMA(NOINLINE fnname) declarator
#else
  #define GASNETI_NEVER_INLINE(fnname,declarator) declarator
#endif

/* GASNETI_FORMAT_PRINTF: enable gcc printf format checking of function args */
#if GASNETT_USE_GCC_ATTRIBUTE_FORMAT
  #define GASNETI_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) \
          __attribute__((__format__ (__printf__, fmtarg, firstvararg))) declarator
#elif PLATFORM_COMPILER_COMPAQ_C /* not Compaq C++ */
  #define GASNETI_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator)  \
          declarator; /* declaration required before pragma */ \
          GASNETI_PRAGMA(assert func_attrs(fnname) format (printf,fmtarg,firstvararg)) \
          declarator
#else
  #define GASNETI_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) declarator
#endif

/* GASNETI_FORMAT_PRINTF_FUNCPTR: like GASNETI_FORMAT_PRINTF but applied to a function pointer */
#if GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR
  #define GASNETI_FORMAT_PRINTF_FUNCPTR GASNETI_FORMAT_PRINTF
#else
  #define GASNETI_FORMAT_PRINTF_FUNCPTR(fnpname,fmtarg,firstvararg,declarator) declarator
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNETI_IDENT() takes a unique identifier and a textual string and embeds the textual
   string in the executable file
 */
#define _GASNETI_IDENT(identName, identText)                         \
  extern char volatile identName[];                                  \
  char volatile identName[] = identText;                             \
  extern char *_##identName##_identfn(void) { return (char*)identName; } \
  static int _dummy_##identName = sizeof(_dummy_##identName)
#if PLATFORM_COMPILER_CRAY && !PLATFORM_ARCH_X86_64 /* fouls up concatenation in ident string */
  #if PLATFORM_COMPILER_VERSION_LT(6,0,0)
    #define GASNETI_PRAGMA_SEMI ;
  #else
    /* Cray CC v6.0+ complains if a semicolon follows a _Pragma() */
    #define GASNETI_PRAGMA_SEMI 
  #endif
  #define GASNETI_IDENT(identName, identText) \
    GASNETI_PRAGMA(_CRI ident identText) GASNETI_PRAGMA_SEMI     \
    _GASNETI_IDENT(identName, identText)
#elif PLATFORM_COMPILER_HP_C && PLATFORM_ARCH_IA64 /* bug 1490 */
  #define GASNETI_IDENT(identName, identText) \
    GASNETI_PRAGMA(VERSIONID identText);      \
    _GASNETI_IDENT(identName, identText)
#elif PLATFORM_COMPILER_XLC
    /* #pragma comment(user,"text...") 
         or
       _Pragma ( "comment (user,\"text...\")" );
       are both supposed to work according to compiler docs, but both appear to be broken
     */
  #define GASNETI_IDENT(identName, identText)   \
    _GASNETI_IDENT(identName, identText)
#else
  #define GASNETI_IDENT _GASNETI_IDENT
#endif
/* ------------------------------------------------------------------------------------ */
/* Branch prediction:
   these macros return the value of the expression given, but pass on
   a hint that you expect the value to be true or false.
   Use them to wrap the conditional expression in an if stmt when
   you have strong reason to believe the branch will frequently go
   in one direction and the branch is a bottleneck
 */
#ifndef PREDICT_TRUE
  #if defined(__GNUC__) && defined(HAVE_BUILTIN_EXPECT)
    /* cast to uintptr_t avoids warnings on some compilers about passing 
       non-integer arguments to __builtin_expect(), and we don't use (int)
       because on some systems this is smaller than (void*) and causes 
       other warnings
     */
   #define PREDICT_TRUE(exp)  __builtin_expect( ((uintptr_t)(exp)), 1 )
   #define PREDICT_FALSE(exp) __builtin_expect( ((uintptr_t)(exp)), 0 )
  #elif PLATFORM_COMPILER_XLC && __xlC__ > 0x0600 && \
       defined(_ARCH_PWR5) /* usually helps on Power5, usually hurts on Power3, mixed on other PPCs */
   #if 1 /* execution_frequency pragma only takes effect when it occurs within a block statement */
     #define PREDICT_TRUE(exp)  ((exp) && ({; _Pragma("execution_frequency(very_high)"); 1; }))
     #define PREDICT_FALSE(exp) ((exp) && ({; _Pragma("execution_frequency(very_low)"); 1; }))
   #else /* experimentally determined that pragma is sometimes(?) ignored unless it is
            preceded by a non-trivial statement - unfortunately the dummy statement can also hurt performance */
     static __inline gasneti_xlc_pragma_dummy(void) {} 
     #define PREDICT_TRUE(exp)  ((exp) && ({ gasneti_xlc_pragma_dummy(); _Pragma("execution_frequency(very_high)"); 1; }))
     #define PREDICT_FALSE(exp) ((exp) && ({ gasneti_xlc_pragma_dummy(); _Pragma("execution_frequency(very_low)"); 1; }))
   #endif
  #else
   #define PREDICT_TRUE(exp)  (exp)
   #define PREDICT_FALSE(exp) (exp)
  #endif
#endif

/* if with branch prediction */
#ifndef if_pf
#if PLATFORM_COMPILER_MTA
  /* MTA's pragma mechanism is buggy, so allow it to be selectively disabled */
  #define GASNETT_MTA_PRAGMA_EXPECT_ENABLED(x) _Pragma(x)
  #define GASNETT_MTA_PRAGMA_EXPECT_DISABLED(x) 
  #define GASNETT_MTA_PRAGMA_EXPECT_OVERRIDE GASNETT_MTA_PRAGMA_EXPECT_ENABLED
  #define if_pf(cond) GASNETT_MTA_PRAGMA_EXPECT_OVERRIDE("mta expect false") if (cond)
  #define if_pt(cond) GASNETT_MTA_PRAGMA_EXPECT_OVERRIDE("mta expect true")  if (cond)
#elif PLATFORM_COMPILER_SGI && _SGI_COMPILER_VERSION >= 720 && _MIPS_SIM != _ABIO32
  /* MIPSPro has a predict-false, but unfortunately no predict-true */
  #define if_pf(cond) if (cond) GASNETI_PRAGMA(mips_frequency_hint NEVER)
  #define if_pt(cond) if (PREDICT_TRUE(cond))
#else
  #define if_pf(cond) if (PREDICT_FALSE(cond))
  #define if_pt(cond) if (PREDICT_TRUE(cond))
#endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Non-binding prefetch hints:
   These macros take a single address expression and provide a hint to prefetch the
   corresponding memory to L1 cache for either reading or for writing.
   These are non-binding hints and so the argument need not always be a valid pointer.
   For instance, GASNETI_PREFETCH_{READ,WRITE}_HINT(NULL) is explicitly permitted.
   The macros may expand to nothing, so the argument must not have side effects.
 */
#if HAVE_BUILTIN_PREFETCH
  #define GASNETI_PREFETCH_READ_HINT(P) __builtin_prefetch((void *)(P),0)
  #define GASNETI_PREFETCH_WRITE_HINT(P) __builtin_prefetch((void *)(P),1)
#else
  #define GASNETI_PREFETCH_READ_HINT(P)
  #define GASNETI_PREFETCH_WRITE_HINT(P)
#endif

/* ------------------------------------------------------------------------------------ */
#endif
