/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_basic.h $
 * Description: GASNet basic header utils
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNETEX_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_BASIC_H
#define _GASNET_BASIC_H

/* ------------------------------------------------------------------------------------ */
/* must precede everything else to ensure correct operation */
#include "portable_inttypes.h"
#undef _PORTABLE_PLATFORM_H
#include "gasnet_portable_platform.h"

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
#if GASNETT_COMPILER_FORCE_MISMATCH // for testing purposes
  #undef GASNETI_COMPILER_IS_CC
  #undef GASNETI_COMPILER_IS_CXX
  #undef GASNETI_COMPILER_IS_MPI_CC
#endif
#if !defined(GASNETI_COMPILER_IS_CC) && \
    !defined(GASNETI_COMPILER_IS_CXX) && \
    !defined(GASNETI_COMPILER_IS_MPI_CC)
  /* detect when the compiler in use differs from the compilers tested by configure,
     indicating some of the configure-detected results may be invalid for this compilation
     this is permitted in certain VERY limited contexts, and activates conservative assumptions
  */
  #define GASNETI_COMPILER_IS_UNKNOWN 1
#endif

// preprocessor conditional: configure detected GASNETI_HAVE_<id>_<feature> for the current compiler
#define GASNETI_COMPILER_HAS(feature) ( \
     (GASNETI_COMPILER_IS_CC     && GASNETI_HAVE_CC_ ## feature)  || \
     (GASNETI_COMPILER_IS_CXX    && GASNETI_HAVE_CXX_ ## feature) || \
     (GASNETI_COMPILER_IS_MPI_CC && GASNETI_HAVE_MPI_CC_ ## feature) )

// GASNETI_COMPILER_HAS_BUILTIN: specialized for testing builtins
#ifdef __has_builtin
  #define _GASNETI_HAS_BUILTIN(x) __has_builtin(x)
#else
  #define _GASNETI_HAS_BUILTIN(x) 0
#endif
#define GASNETI_COMPILER_HAS_BUILTIN(MACRO_NAME,token_name) \
       (GASNETI_COMPILER_HAS(BUILTIN_ ## MACRO_NAME) || \
        (GASNETI_COMPILER_IS_UNKNOWN && _GASNETI_HAS_BUILTIN(__builtin_ ## token_name)))

// GASNETI_COMPILER_HAS_ATTRIBUTE: specialized for testing attributes
#ifdef __has_attribute
  #define _GASNETI_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
  #define _GASNETI_HAS_ATTRIBUTE(x) 0
#endif
#define GASNETI_COMPILER_HAS_ATTRIBUTE(MACRO_NAME,attrib_token) \
       (GASNETI_COMPILER_HAS(ATTRIBUTE_ ## MACRO_NAME) || \
        (GASNETI_COMPILER_IS_UNKNOWN && _GASNETI_HAS_ATTRIBUTE(attrib_token)))

// GASNETI_COMPILER_HAS_CXX11_ATTRIBUTE: specialized for testing C++11 attributes
#ifdef __has_cpp_attribute
  #define _GASNETI_HAS_CXX11_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
  #define _GASNETI_HAS_CXX11_ATTRIBUTE(x) 0
#endif
// only trust configure results for C++ attrib if current C++ langlvl is >= configure C++ langlvl
#define GASNETI_COMPILER_HAS_CXX11_ATTRIBUTE(MACRO_NAME,attrib_token) \
       ((GASNETI_COMPILER_HAS(CXX11_ATTRIBUTE_ ## MACRO_NAME) && \
        PLATFORM_COMPILER_CXX_LANGLVL >= GASNETI_PLATFORM_CXX_CXX_LANGLVL) \
      || (GASNETI_COMPILER_IS_UNKNOWN && _GASNETI_HAS_CXX11_ATTRIBUTE(attrib_token)))

// __has_*() Blacklists

// Some compilers have a broken implementation of one or more of the following Gnu/clang extension macros:
//     __has_builtin()  __has_attribute()  __has_cpp_attribute()
// where "broken" means providing a definition that incorrectly returns non-zero answers in some cases.
// We blacklist the use of the relevant macro on those particular compilers to ensure we never use it there.
// Some compilers bitch about redefining these built-in macros, so instead we force our own
// wrapper to conservatively always returns zero. 

#if PLATFORM_COMPILER_PATHSCALE /* broken builtin_assume on Linux */ \
   || ( PLATFORM_COMPILER_PGI_CXX && PLATFORM_OS_DARWIN ) /* Bug 3736 */
  #undef  _GASNETI_HAS_BUILTIN
  #define _GASNETI_HAS_BUILTIN(x)  0
#endif

#if PLATFORM_COMPILER_SUN /* bug 3666: Sun CC __has_attribute returns wrong answers and cannot be trusted */
  #undef  _GASNETI_HAS_ATTRIBUTE
  #define _GASNETI_HAS_ATTRIBUTE(x)  0
#endif

#if (PLATFORM_COMPILER_GNU_CXX && PLATFORM_COMPILER_CXX_LANGLVL < 201100) \
      /* g++ __has_cpp_attribute returns false positives with langlvl < C++11 */ \
  || (PLATFORM_COMPILER_SUN_CXX && PLATFORM_COMPILER_VERSION_LT(5,15,0)) /* bug 3730 */
  #undef  _GASNETI_HAS_CXX11_ATTRIBUTE
  #define _GASNETI_HAS_CXX11_ATTRIBUTE(x) 0
#endif

#if PLATFORM_COMPILER_CLANG && PLATFORM_COMPILER_VERSION_LT(3,6,0)
  // bug3801: old clangs report __has_attribute(__fallthrough__)=1, but incorrectly implement the attribute
  // in a way that leads to empty statement warnings when used as per GNU instructions
  #define GASNETT_USE_GCC_ATTRIBUTE_FALLTHROUGH 0
#endif

// token expansion: expands to configure-detected token GASNETI_<id>_<feature> for the current compiler
//                  (which MUST NOT be #undef, although it can be #defined to blank)
//                  or 'otherwise' in the case of a compiler mismatch
#if GASNETI_COMPILER_IS_CC
  #define GASNETI_COMPILER_FEATURE(feature, otherwise) GASNETI_CC_ ## feature
#elif GASNETI_COMPILER_IS_MPI_CC
  #define GASNETI_COMPILER_FEATURE(feature, otherwise) GASNETI_MPI_CC_ ## feature
#elif GASNETI_COMPILER_IS_CXX
  #define GASNETI_COMPILER_FEATURE(feature, otherwise) GASNETI_CXX_ ## feature
#else
  #define GASNETI_COMPILER_FEATURE(feature, otherwise) otherwise
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
  #define GASNETI_EXTERNC       extern
  #define GASNETI_END_EXTERNC 
#endif

/* Tentative definition declaration specifier macros
 *
 * GASNETI_TENTATIVE_CLIENT - a symbol that is tentative in the client, 
 *                            and is optionally defined in the library
 * GASNETI_TENTATIVE_LIBRARY - a symbol that is tentative in the library,
 *                            and is optionally defined in the client
 *
 * Where tentative these expand to a common-block annotation, if appropriate.
 * Where not tentative, these expand to extern.
 * C++ clients where tentative definitions are not supported always see these as extern.
 */
#if defined(__cplusplus)
  #define GASNETI_TENTATIVE_CLIENT   extern
  #define GASNETI_TENTATIVE_LIBRARY  extern
#elif GASNETI_BUILDING_TOOLS || GASNETI_BUILDING_CONDUIT
  #if !GASNETI_COMPILER_IS_CC
  #error Attempted to build libgasnet with an unrecognized different compiler - please re-run configure.
  #endif
  #define GASNETI_TENTATIVE_CLIENT   extern
  #define GASNETI_TENTATIVE_LIBRARY  GASNETI_COMMON
#elif GASNETI_COMPILER_HAS_ATTRIBUTE(COMMON,__common__)
  #define GASNETI_TENTATIVE_CLIENT   __attribute__((__common__))
  #define GASNETI_TENTATIVE_LIBRARY  extern
#else // client C code with mismatched compiler and no attribute support
  #define GASNETI_TENTATIVE_CLIENT   /* empty (be conservative) */
  #define GASNETI_TENTATIVE_LIBRARY  extern
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
#else
  // mismatch behavior: define away to nothing, which should always be safe
  //   define to 1 because 0 triggers use of (void*) in place of the typedef
  #define GASNETI_RESTRICT                      GASNETI_COMPILER_FEATURE(RESTRICT,)
  #define GASNETI_RESTRICT_MAY_QUALIFY_TYPEDEFS GASNETI_COMPILER_FEATURE(RESTRICT_MAY_QUALIFY_TYPEDEFS,1)
#endif

#if (!defined(GASNETT_USE_BUILTIN_CONSTANT_P) && GASNETI_COMPILER_HAS_BUILTIN(CONSTANT_P,constant_p)) || \
              GASNETT_USE_BUILTIN_CONSTANT_P
  #define     GASNETT_USE_BUILTIN_CONSTANT_P 1
  #define gasneti_constant_p(_expr) __builtin_constant_p(_expr)
#else
  #define gasneti_constant_p(_expr) (0)
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

//------------------------------------------------------------------------------------
// Meta-macros for fixed-count text generation
//
// The following family of macros (named with a integer suffix in N=0..GASNETI_META_MAX)
// perform N expansions of the provided macros.
// They take two 3-argument macros as arguments:
//  fnb(-1,0,1) is expanded exactly once as the base case
//  fni(i_minus_1, i, i_plus_1) is expanded exactly N times as the inductive case,
//     with i=[1..N] and i_minus_1=(i-1) and i_plus_1=(i+1) at each step
// The ASC[N] variants expand the macros in ascending numerical order,
// whereas the DES[N] variants expand the macros in descending order
// Note that due to C preprocessor rules fnb and fni cannot themselves
// contain an expansion of the same meta-macro (even with different arguments), 
// but can they can use the "other" meta-macro.

// supported recursion depth
#define GASNETI_META_MAX 16

// convenience macro, useful for base case
#define GASNETI_META_EMPTY(a,b,c)

#define GASNETI_META_ASC0(fnb,fni)  fnb(-1,0,1)
#define GASNETI_META_ASC1(fnb,fni)  GASNETI_META_ASC0(fnb,fni)  fni(0,1,2)
#define GASNETI_META_ASC2(fnb,fni)  GASNETI_META_ASC1(fnb,fni)  fni(1,2,3)
#define GASNETI_META_ASC3(fnb,fni)  GASNETI_META_ASC2(fnb,fni)  fni(2,3,4)
#define GASNETI_META_ASC4(fnb,fni)  GASNETI_META_ASC3(fnb,fni)  fni(3,4,5)
#define GASNETI_META_ASC5(fnb,fni)  GASNETI_META_ASC4(fnb,fni)  fni(4,5,6)
#define GASNETI_META_ASC6(fnb,fni)  GASNETI_META_ASC5(fnb,fni)  fni(5,6,7)
#define GASNETI_META_ASC7(fnb,fni)  GASNETI_META_ASC6(fnb,fni)  fni(6,7,8)
#define GASNETI_META_ASC8(fnb,fni)  GASNETI_META_ASC7(fnb,fni)  fni(7,8,9)
#define GASNETI_META_ASC9(fnb,fni)  GASNETI_META_ASC8(fnb,fni)  fni(8,9,10)
#define GASNETI_META_ASC10(fnb,fni) GASNETI_META_ASC9(fnb,fni)  fni(9,10,11)
#define GASNETI_META_ASC11(fnb,fni) GASNETI_META_ASC10(fnb,fni) fni(10,11,12)
#define GASNETI_META_ASC12(fnb,fni) GASNETI_META_ASC11(fnb,fni) fni(11,12,13)
#define GASNETI_META_ASC13(fnb,fni) GASNETI_META_ASC12(fnb,fni) fni(12,13,14)
#define GASNETI_META_ASC14(fnb,fni) GASNETI_META_ASC13(fnb,fni) fni(13,14,15)
#define GASNETI_META_ASC15(fnb,fni) GASNETI_META_ASC14(fnb,fni) fni(14,15,16)
#define GASNETI_META_ASC16(fnb,fni) GASNETI_META_ASC15(fnb,fni) fni(15,16,17)

#define GASNETI_META_DES0(fnb,fni)  fnb(-1,0,1)
#define GASNETI_META_DES1(fnb,fni)  fni(0,1,2)    GASNETI_META_DES0(fnb,fni)
#define GASNETI_META_DES2(fnb,fni)  fni(1,2,3)    GASNETI_META_DES1(fnb,fni)
#define GASNETI_META_DES3(fnb,fni)  fni(2,3,4)    GASNETI_META_DES2(fnb,fni)
#define GASNETI_META_DES4(fnb,fni)  fni(3,4,5)    GASNETI_META_DES3(fnb,fni)
#define GASNETI_META_DES5(fnb,fni)  fni(4,5,6)    GASNETI_META_DES4(fnb,fni)
#define GASNETI_META_DES6(fnb,fni)  fni(5,6,7)    GASNETI_META_DES5(fnb,fni)
#define GASNETI_META_DES7(fnb,fni)  fni(6,7,8)    GASNETI_META_DES6(fnb,fni)
#define GASNETI_META_DES8(fnb,fni)  fni(7,8,9)    GASNETI_META_DES7(fnb,fni)
#define GASNETI_META_DES9(fnb,fni)  fni(8,9,10)   GASNETI_META_DES8(fnb,fni)
#define GASNETI_META_DES10(fnb,fni) fni(9,10,11)  GASNETI_META_DES9(fnb,fni)
#define GASNETI_META_DES11(fnb,fni) fni(10,11,12) GASNETI_META_DES10(fnb,fni)
#define GASNETI_META_DES12(fnb,fni) fni(11,12,13) GASNETI_META_DES11(fnb,fni)
#define GASNETI_META_DES13(fnb,fni) fni(12,13,14) GASNETI_META_DES12(fnb,fni)
#define GASNETI_META_DES14(fnb,fni) fni(13,14,15) GASNETI_META_DES13(fnb,fni)
#define GASNETI_META_DES15(fnb,fni) fni(14,15,16) GASNETI_META_DES14(fnb,fni)
#define GASNETI_META_DES16(fnb,fni) fni(15,16,17) GASNETI_META_DES15(fnb,fni)

// Extended variant that also threads three arbitrary arguments though the expansion chain
#define GASNETI_META3_EMPTY(a,b,c,a1,a2,a3)

#define GASNETI_META3_ASC0(fnb,fni,a1,a2,a3)  fnb(-1,0,1,a1,a2,a3)
#define GASNETI_META3_ASC1(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC0(fnb,fni,a1,a2,a3)  fni(0,1,2,a1,a2,a3)
#define GASNETI_META3_ASC2(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC1(fnb,fni,a1,a2,a3)  fni(1,2,3,a1,a2,a3)
#define GASNETI_META3_ASC3(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC2(fnb,fni,a1,a2,a3)  fni(2,3,4,a1,a2,a3)
#define GASNETI_META3_ASC4(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC3(fnb,fni,a1,a2,a3)  fni(3,4,5,a1,a2,a3)
#define GASNETI_META3_ASC5(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC4(fnb,fni,a1,a2,a3)  fni(4,5,6,a1,a2,a3)
#define GASNETI_META3_ASC6(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC5(fnb,fni,a1,a2,a3)  fni(5,6,7,a1,a2,a3)
#define GASNETI_META3_ASC7(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC6(fnb,fni,a1,a2,a3)  fni(6,7,8,a1,a2,a3)
#define GASNETI_META3_ASC8(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC7(fnb,fni,a1,a2,a3)  fni(7,8,9,a1,a2,a3)
#define GASNETI_META3_ASC9(fnb,fni,a1,a2,a3)  GASNETI_META3_ASC8(fnb,fni,a1,a2,a3)  fni(8,9,10,a1,a2,a3)
#define GASNETI_META3_ASC10(fnb,fni,a1,a2,a3) GASNETI_META3_ASC9(fnb,fni,a1,a2,a3)  fni(9,10,11,a1,a2,a3)
#define GASNETI_META3_ASC11(fnb,fni,a1,a2,a3) GASNETI_META3_ASC10(fnb,fni,a1,a2,a3) fni(10,11,12,a1,a2,a3)
#define GASNETI_META3_ASC12(fnb,fni,a1,a2,a3) GASNETI_META3_ASC11(fnb,fni,a1,a2,a3) fni(11,12,13,a1,a2,a3)
#define GASNETI_META3_ASC13(fnb,fni,a1,a2,a3) GASNETI_META3_ASC12(fnb,fni,a1,a2,a3) fni(12,13,14,a1,a2,a3)
#define GASNETI_META3_ASC14(fnb,fni,a1,a2,a3) GASNETI_META3_ASC13(fnb,fni,a1,a2,a3) fni(13,14,15,a1,a2,a3)
#define GASNETI_META3_ASC15(fnb,fni,a1,a2,a3) GASNETI_META3_ASC14(fnb,fni,a1,a2,a3) fni(14,15,16,a1,a2,a3)
#define GASNETI_META3_ASC16(fnb,fni,a1,a2,a3) GASNETI_META3_ASC15(fnb,fni,a1,a2,a3) fni(15,16,17,a1,a2,a3)

#include <stddef.h> /* get standard types, esp size_t */

// gasneti_offsetof is our version of C's offsetof() that allows field arguments 
// containing non-constant expressions. If the member_field_expr contains only
// constant expressions then the result is a constant, otherwise it is non-constant.
// Some compilers (eg XLC) complain about using stddef.h offsetof() for this purpose
#if PLATFORM_COMPILER_XLC || PLATFORM_COMPILER_PATHSCALE
  #define gasneti_offsetof(type, member_field_expr) \
          ((size_t)(uintptr_t)&(((type *)NULL)->member_field_expr))
#elif PLATFORM_COMPILER_CRAY
  // seen to botch all the remotely normal expressions involving constant addresses
  extern volatile int gasnet_frozen;
  #define GASNETI_OFFSET_BASE (&gasnet_frozen)
  #define gasneti_offsetof(type, member_field_expr) \
          ((size_t)((uintptr_t)&(((type *)GASNETI_OFFSET_BASE)->member_field_expr) - \
                    (uintptr_t)GASNETI_OFFSET_BASE))
#else
  #define gasneti_offsetof offsetof
#endif

// gasneti_container_of()
// Given address of a structure member, recreate a pointer to the structure.
// Use of gasneti_offsetof() allows 'field' to be a non-constant expression.
//   ptr = member pointer
//   type = container's type
//   field = member's field name
#define gasneti_container_of(ptr,type,field) \
          ((type*) ((uintptr_t)(ptr) - gasneti_offsetof(type,field)))

/* splitting and reassembling 64-bit quantities */
#define GASNETI_MAKEWORD(hi,lo) ((((uint64_t)(hi)) << 32) | (((uint64_t)(lo)) & 0xFFFFFFFF))
#define GASNETI_HIWORD(arg)     ((uint32_t)(((uint64_t)(arg)) >> 32))
#if PLATFORM_COMPILER_INTEL
  /* This form avoids a #69 truncation warning while generating identical code */
  #define GASNETI_LOWORD(arg)     ((uint32_t)((uint64_t)(arg) & 0xFFFFFFFF))
#else
  #define GASNETI_LOWORD(arg)     ((uint32_t)((uint64_t)(arg)))
#endif

/* assembling "signatures" from 2, 4 or 8 (7-bit ASCII) characters */
#if PLATFORM_ARCH_LITTLE_ENDIAN
  #define GASNETI_SIGNATURE2(c0,c1) ((uint16_t)((c0)|((c1)<<8)))
  #define GASNETI_SIGNATURE4(c0,c1,c2,c3) ((uint32_t)((c0)|((c1)<<8)|((c2)<<16)|((uint32_t)(c3)<<24)))
  #define GASNETI_SIGNATURE8(c0,c1,c2,c3,c4,c5,c6,c7) \
          GASNETI_MAKEWORD(GASNETI_SIGNATURE4(c4,c5,c6,c7),GASNETI_SIGNATURE4(c0,c1,c2,c3))
#else
  #define GASNETI_SIGNATURE2(c0,c1) ((uint16_t)(((c0)<<8)|(c1)))
  #define GASNETI_SIGNATURE4(c0,c1,c2,c3) ((uint32_t)(((uint32_t)(c0)<<24)|((c1)<<16)|((c2)<<8)|(c3)))
  #define GASNETI_SIGNATURE8(c0,c1,c2,c3,c4,c5,c6,c7) \
          GASNETI_MAKEWORD(GASNETI_SIGNATURE4(c0,c1,c2,c3),GASNETI_SIGNATURE4(c4,c5,c6,c7))
#endif

/* magic numbers for identifying/protecting types
 * WARNING: GASNETI_{CHECK,IMPORT}_MAGIC() may evaluate the pointer argument more than once!
 */
#define GASNETI_MAKE_MAGIC(c0,c1,c2,c3) GASNETI_SIGNATURE8('g','e','x',':',c0,c1,c2,c3)
#define GASNETI_MAKE_BAD_MAGIC(c0,c1,c2,c3) GASNETI_SIGNATURE8('B','A','D',':',c0,c1,c2,c3)
typedef union { uint64_t _u; char _c[8]; } gasneti_magic_t;
#if GASNET_DEBUG
  #define GASNETI_INIT_MAGIC(p,m)  ((void)((p)->_magic._u = (m)))
  #define GASNETI_CHECK_MAGIC(p,m) do { if (p) gasneti_assert_uint((p)->_magic._u ,==, (m)); } while (0)
  #define GASNETI_IMPORT_MAGIC(p,type) do { \
      if ((p) && ((p)->_magic._u == GASNETI_##type##_BAD_MAGIC)) {              \
        gasneti_fatalerror("Likely use-after-free error for " #type " object"); \
      }                                                                         \
      GASNETI_CHECK_MAGIC(p,GASNETI_##type##_MAGIC);                            \
    } while (0)
#else
  #define GASNETI_INIT_MAGIC(p,m)  ((void)0)
  #define GASNETI_CHECK_MAGIC(p,m) ((void)0)
  #define GASNETI_IMPORT_MAGIC(p,t) ((void)0)
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
  #else
    #error GASNET_PAGESIZE unknown and not set by conduit
  #endif
  #if GASNET_PAGESIZE <= 0
    #error bad defn of GASNET_PAGESIZE
  #endif
#endif

/* special GCC features */

/* work around bug 1620 unless client has explicitly set GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE */
#if PLATFORM_COMPILER_PATHSCALE && !defined(GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE)
  #define GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE 0
#endif

#if PLATFORM_COMPILER_SUN_C && __SUNPRO_C < 0x570
  #define GASNETI_PRAGMA(x) /* not supported in older versions (550 fails, 570 works) */
#else
  #define GASNETI_PRAGMA(x) _Pragma ( #x )
#endif

#if GASNETI_COMPILER_HAS(PRAGMA_GCC_DIAGNOSTIC)
  #define GASNETI_USE_PRAGMA_GCC_DIAGNOSTIC 1
  #if defined(__cplusplus)
    #define _GASNETI_NOWARN_IGNORES_C_ONLY
  #else
    #define _GASNETI_NOWARN_IGNORES_C_ONLY \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wstrict-prototypes") \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wmissing-prototypes")
  #endif
  #define GASNETI_BEGIN_NOWARN                                          \
          GASNETI_PRAGMA(GCC diagnostic push)                           \
          _GASNETI_NOWARN_IGNORES_C_ONLY                                \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wunused-function")    \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wunused-variable")    \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wunused-value")       \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wunused-parameter")   \
          GASNETI_PRAGMA(GCC diagnostic ignored "-Wunused") 
  #define GASNETI_END_NOWARN  \
          GASNETI_PRAGMA(GCC diagnostic pop)
#else
  #define GASNETI_BEGIN_NOWARN
  #define GASNETI_END_NOWARN 
#endif

/* If we have recognized the compiler, pick up its attribute support */
#if GASNETI_COMPILER_HAS(ATTRIBUTE) || defined(__has_attribute)
  #define GASNETI_HAVE_GCC_ATTRIBUTE 1
  /* __has_attribute(x) macro provided by some compilers gives the ability
   * to probe attributes at compile time. The following do not use this 
   * detection mechanism because the probes are context dependent:
   *    ATTRIBUTE_FORMAT_FUNCPTR
   *    ATTRIBUTE_FORMAT_FUNCPTR_ARG
   */
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE \
       GASNETI_COMPILER_HAS_ATTRIBUTE(ALWAYSINLINE,__always_inline__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_NOINLINE
    #define GASNETT_USE_GCC_ATTRIBUTE_NOINLINE \
       GASNETI_COMPILER_HAS_ATTRIBUTE(NOINLINE,__noinline__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_MALLOC
    #define GASNETT_USE_GCC_ATTRIBUTE_MALLOC \
       GASNETI_COMPILER_HAS_ATTRIBUTE(MALLOC,__malloc__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT
    #define GASNETT_USE_GCC_ATTRIBUTE_WARNUNUSEDRESULT \
       GASNETI_COMPILER_HAS_ATTRIBUTE(WARNUNUSEDRESULT,__warn_unused_result__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_USED
    #define GASNETT_USE_GCC_ATTRIBUTE_USED \
       GASNETI_COMPILER_HAS_ATTRIBUTE(USED,__used__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS
    #define GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS \
       GASNETI_COMPILER_HAS_ATTRIBUTE(MAYALIAS,__may_alias__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_NORETURN
    #define GASNETT_USE_GCC_ATTRIBUTE_NORETURN \
       GASNETI_COMPILER_HAS_ATTRIBUTE(NORETURN,__noreturn__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_PURE
    #define GASNETT_USE_GCC_ATTRIBUTE_PURE \
       GASNETI_COMPILER_HAS_ATTRIBUTE(PURE,__pure__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_CONST
    #define GASNETT_USE_GCC_ATTRIBUTE_CONST \
       GASNETI_COMPILER_HAS_ATTRIBUTE(CONST,__const__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_HOT
    #define GASNETT_USE_GCC_ATTRIBUTE_HOT \
       GASNETI_COMPILER_HAS_ATTRIBUTE(HOT,__hot__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_COLD
    #define GASNETT_USE_GCC_ATTRIBUTE_COLD \
       GASNETI_COMPILER_HAS_ATTRIBUTE(COLD,__cold__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_DEPRECATED
    #define GASNETT_USE_GCC_ATTRIBUTE_DEPRECATED \
       GASNETI_COMPILER_HAS_ATTRIBUTE(DEPRECATED,__deprecated__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_FALLTHROUGH
    #define GASNETT_USE_GCC_ATTRIBUTE_FALLTHROUGH \
       GASNETI_COMPILER_HAS_ATTRIBUTE(FALLTHROUGH,__fallthrough__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_FORMAT
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT \
       GASNETI_COMPILER_HAS_ATTRIBUTE(FORMAT,__format__)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR \
       GASNETI_COMPILER_HAS(ATTRIBUTE_FORMAT_FUNCPTR)
  #endif
  #ifndef   GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR_ARG
    #define GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR_ARG \
       GASNETI_COMPILER_HAS(ATTRIBUTE_FORMAT_FUNCPTR_ARG)
  #endif
#endif

/* C++11 attributes */
// The Clang C compiler defines __has_cpp_attribute(), expanding to 0.
// However, it errors on __has_cpp_attribute(namespage::token) and thus
// we check for __cplusplus even though that could be seen as redundant.
#if defined(__cplusplus) && \
    (GASNETI_COMPILER_HAS(CXX11_ATTRIBUTE) || defined(__has_cpp_attribute))
  #ifndef   GASNETT_USE_CXX11_ATTRIBUTE_FALLTHROUGH
    #define GASNETT_USE_CXX11_ATTRIBUTE_FALLTHROUGH \
       GASNETI_COMPILER_HAS_CXX11_ATTRIBUTE(FALLTHROUGH,fallthrough)
  #endif
  #ifndef   GASNETT_USE_CXX11_ATTRIBUTE_CLANG__FALLTHROUGH
    #define GASNETT_USE_CXX11_ATTRIBUTE_CLANG__FALLTHROUGH \
       GASNETI_COMPILER_HAS_CXX11_ATTRIBUTE(CLANG__FALLTHROUGH,clang::fallthrough)
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
#else
  #define GASNETI_MALLOCP(fnname)
#endif

/* GASNETI_USED: assert that function is used and must not be ommited from object file */
#if GASNETT_USE_GCC_ATTRIBUTE_USED
  #define GASNETI_USED __attribute__((__used__))
#else
  #define GASNETI_USED 
#endif

/* GASNETI_MAY_ALIAS override(s) */
#if !defined(GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS) && !GASNETI_BUG1389_WORKAROUND && GASNETI_COMPILER_IS_UNKNOWN
  /* Apply conservative default based on compiler version if user did not
   * define GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS nor --enable-conservative-local-copy
   */
  #if PLATFORM_COMPILER_GNU && PLATFORM_COMPILER_VERSION_GE(4,4,0)
    #define GASNETT_USE_GCC_ATTRIBUTE_MAYALIAS 1
  #endif
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
#else
  #define GASNETI_PUREP(fnname) 
#endif

/* GASNETI_CONST: assert that function is "const" */
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
#else
  #define GASNETI_CONSTP(fnname) GASNETI_PUREP(fnname)
#endif

// bug 3673: Cannot use Cray pragma _CRI inline_always here
#if GASNETT_USE_GCC_ATTRIBUTE_ALWAYSINLINE
  /* bug1525: gcc's __always_inline__ attribute appears to be maximally aggressive */
  #define _GASNETI_ALWAYS_INLINE(fnname) __attribute__((__always_inline__))
#else
  #define _GASNETI_ALWAYS_INLINE(fnname)
#endif

#if GASNET_DEBUG
  #define _GASNETI_INLINE_MODIFIER static
#elif defined(__cplusplus)
  #define _GASNETI_INLINE_MODIFIER inline
#elif __STDC_VERSION__ >= 199901L
  #define _GASNETI_INLINE_MODIFIER GASNETI_COMPILER_FEATURE(INLINE_MODIFIER,static inline)
#else
  #define _GASNETI_INLINE_MODIFIER GASNETI_COMPILER_FEATURE(INLINE_MODIFIER,static)
#endif

/* GASNETI_INLINE: Most forceful inlining demand available.
 * Might generate errors in cases where inlining is semantically impossible 
 * (eg recursive functions, varargs fns)
 */
#if GASNET_DEBUG
  #define GASNETI_INLINE(fnname) static
#else
  #define GASNETI_INLINE(fnname) _GASNETI_ALWAYS_INLINE(fnname) _GASNETI_INLINE_MODIFIER
#endif

/* GASNETI_NEVER_INLINE: Most forceful demand available to disable inlining for function.
 */
// bug 3673: Cannot use Cray pragma _CRI inline_never here
#if GASNETT_USE_GCC_ATTRIBUTE_NOINLINE
 #ifdef __noinline__ /* e.g. nvcc's host_defines */
  #define GASNETI_NEVER_INLINE(fnname,declarator) __attribute__((noinline)) declarator
 #else
  #define GASNETI_NEVER_INLINE(fnname,declarator) __attribute__((__noinline__)) declarator
 #endif
#elif PLATFORM_COMPILER_SUN_C
  #define GASNETI_NEVER_INLINE(fnname,declarator) declarator; GASNETI_PRAGMA(no_inline(fnname)) declarator
#else
  #define GASNETI_NEVER_INLINE(fnname,declarator) declarator
#endif

/* GASNETI_HOT (COLD): assert function is frequently (infrequently) called */
#if GASNETT_USE_GCC_ATTRIBUTE_HOT
  #define GASNETI_HOT __attribute__((__hot__))
#else
  #define GASNETI_HOT
#endif
#if GASNETT_USE_GCC_ATTRIBUTE_COLD
  #define GASNETI_COLD __attribute__((__cold__))
#else
  #define GASNETI_COLD
#endif

/* GASNETI_DEPRECATED: mark a function as deprecated (subject to future removal) */
#if GASNETT_USE_GCC_ATTRIBUTE_DEPRECATED
  #define GASNETI_DEPRECATED __attribute__((__deprecated__))
#else
  #define GASNETI_DEPRECATED
#endif

/* GASNETI_FALLTHROUGH: annotate a switch case as intentionally lacking "break".
   Legal only between a statement and subsequent "case" (where "break" normally appears) or label.
   Not legal (or necessary) between back-to-back cases w/o intervening statements. */
#if GASNETT_USE_GCC_ATTRIBUTE_FALLTHROUGH
  // Syntax requires the attribute to be attached to a null statement (the semicolon).
  #define GASNETI_FALLTHROUGH __attribute__((__fallthrough__)) ;
#elif GASNETT_USE_CXX11_ATTRIBUTE_FALLTHROUGH
  #define GASNETI_FALLTHROUGH [[fallthrough]] ;
#elif GASNETT_USE_CXX11_ATTRIBUTE_CLANG__FALLTHROUGH
  #define GASNETI_FALLTHROUGH [[clang::fallthrough]] ;
#else
  #define GASNETI_FALLTHROUGH
#endif

/* GASNETI_FORMAT_PRINTF: enable gcc printf format checking of function args */
#if GASNETT_USE_GCC_ATTRIBUTE_FORMAT
  #define GASNETI_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) \
          __attribute__((__format__ (__printf__, fmtarg, firstvararg))) declarator
#else
  #define GASNETI_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) declarator
#endif

/* GASNETI_FORMAT_PRINTF_FUNCPTR: like GASNETI_FORMAT_PRINTF but applied to a function pointer */
#if GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR
  #define GASNETI_FORMAT_PRINTF_FUNCPTR GASNETI_FORMAT_PRINTF
#else
  #define GASNETI_FORMAT_PRINTF_FUNCPTR(fnpname,fmtarg,firstvararg,declarator) declarator
#endif

/* GASNETI_FORMAT_PRINTF_FUNCPTR_ARG: GASNETI_FORMAT_PRINTF_FUNCPTR applied to a function argument  */
#if GASNETT_USE_GCC_ATTRIBUTE_FORMAT_FUNCPTR_ARG
  #define GASNETI_FORMAT_PRINTF_FUNCPTR_ARG GASNETI_FORMAT_PRINTF
#else
  #define GASNETI_FORMAT_PRINTF_FUNCPTR_ARG(fnpname,fmtarg,firstvararg,declarator) declarator
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNETI_IDENT() takes a unique identifier and a textual string and embeds the textual
   string in the executable file
 */
#define _GASNETI_IDENT(identName, identText)                         \
  extern char volatile identName[];                                  \
  char volatile identName[] = identText;                             \
  GASNETI_COLD                                                       \
  extern char *_##identName##_identfn(void) { return (char*)identName; } \
  extern char *_##identName##_identfn(void) /* swallow semicolon */
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
#ifndef GASNETT_PREDICT_TRUE
  #if (!defined(GASNETT_USE_BUILTIN_EXPECT) && GASNETI_COMPILER_HAS_BUILTIN(EXPECT,expect)) || \
                GASNETT_USE_BUILTIN_EXPECT
    #define     GASNETT_USE_BUILTIN_EXPECT 1
    /* cast to uintptr_t avoids warnings on some compilers about passing 
       non-integer arguments to __builtin_expect(), and we don't use (int)
       because on some systems this is smaller than (void*) and causes 
       other warnings
       bug 3664: PREDICT_TRUE negates exp to hint equality to zero
     */
    #define GASNETT_PREDICT_TRUE(exp)  (!__builtin_expect( (!(uintptr_t)(exp)), 0 ))
    #define GASNETT_PREDICT_FALSE(exp) ( __builtin_expect( ( (uintptr_t)(exp)), 0 ))
  #else
    #define GASNETT_PREDICT_TRUE(exp)  (exp)
    #define GASNETT_PREDICT_FALSE(exp) (exp)
  #endif
#endif

/* if with branch prediction */
#ifndef if_pf
  #define if_pf(cond) if (GASNETT_PREDICT_FALSE(cond))
  #define if_pt(cond) if (GASNETT_PREDICT_TRUE(cond))
#endif

/* ------------------------------------------------------------------------------------ */
/* Non-binding prefetch hints:
   These macros take a single address expression and provide a hint to prefetch the
   corresponding memory to L1 cache for either reading or for writing.
   These are non-binding hints and so the argument need not always be a valid pointer.
   For instance, GASNETI_PREFETCH_{READ,WRITE}_HINT(NULL) is explicitly permitted.
   The macros may expand to nothing, so the argument must not have side effects.
 */
#if (!defined(GASNETT_USE_BUILTIN_PREFETCH) && GASNETI_COMPILER_HAS_BUILTIN(PREFETCH,prefetch)) || \
              GASNETT_USE_BUILTIN_PREFETCH
  #define     GASNETT_USE_BUILTIN_PREFETCH 1
  #define GASNETI_PREFETCH_READ_HINT(P) __builtin_prefetch((void *)(P),0)
  #define GASNETI_PREFETCH_WRITE_HINT(P) __builtin_prefetch((void *)(P),1)
#else
  #define GASNETI_PREFETCH_READ_HINT(P)  ((void)0)
  #define GASNETI_PREFETCH_WRITE_HINT(P) ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
// Misc builtins 

#if !defined(GASNETT_USE_ASSUME) && GASNETI_COMPILER_HAS(ASSUME)
    #define GASNETT_USE_ASSUME 1
#endif
#if !defined(GASNETT_USE_BUILTIN_ASSUME) && GASNETI_COMPILER_HAS_BUILTIN(ASSUME,assume)
    #define GASNETT_USE_BUILTIN_ASSUME 1
#endif
#if !defined(GASNETT_USE_BUILTIN_UNREACHABLE) && GASNETI_COMPILER_HAS_BUILTIN(UNREACHABLE,unreachable)
    #define GASNETT_USE_BUILTIN_UNREACHABLE 1
#endif
/* ------------------------------------------------------------------------------------ */
#endif
