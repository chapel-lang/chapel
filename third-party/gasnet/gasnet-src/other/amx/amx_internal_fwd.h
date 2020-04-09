/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/amx_internal_fwd.h $
 * Description: AMX internal header file, forward defines
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _AMX_INTERNAL_FWD_H
#define _AMX_INTERNAL_FWD_H

#undef _PORTABLE_PLATFORM_H
#include <amx_portable_platform.h>

#ifdef HAVE_GASNET_TOOLS 
  #ifndef GASNETT_LITE_MODE
  #define GASNETT_LITE_MODE /* use lite mode, to preserve AMX's threading neutrality */
  #endif
  #include <gasnet_tools.h> /* must precede internal assert defs */
#endif
#if !defined(HAVE_C99_FORMAT_SPECIFIERS) && !(defined(__PRISZ_PREFIX) && defined(__PRIPD_PREFIX))
#define HAVE_C99_FORMAT_SPECIFIERS 1
#endif
#include <portable_inttypes.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>

#ifndef AMX_STRINGIFY
#define _AMX_STRINGIFY_HELPER(x) #x
#define AMX_STRINGIFY(x) _AMX_STRINGIFY_HELPER(x)
#endif

#ifndef AMX_CONCAT
#define _AMX_CONCAT_HELPER(a,b) a ## b
#define AMX_CONCAT(a,b) _AMX_CONCAT_HELPER(a,b)
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef MIN
#define MIN(x,y)  ((x)<(y)?(x):(y))
#endif
#ifndef MAX
#define MAX(x,y)  ((x)>(y)?(x):(y))
#endif

#ifdef GASNETT_CURRENT_FUNCTION
  #define AMX_CURRENT_FUNCTION GASNETT_CURRENT_FUNCTION
#elif defined(__GNUC__) /* try to get the function name from GCC */
  #define AMX_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif __cplusplus >= 201103L || __STDC_VERSION__ >= 199901
  #define AMX_CURRENT_FUNCTION __func__
#else
  #define AMX_CURRENT_FUNCTION ""
#endif

/* alignment macros */
#define AMX_POWEROFTWO(P)    (((P)&((P)-1)) == 0)

#define AMX_ALIGNDOWN(p,P)    (AMX_assert(AMX_POWEROFTWO(P)), \
                                   ((uintptr_t)(p))&~((uintptr_t)((P)-1)))
#define AMX_ALIGNUP(p,P)     (AMX_ALIGNDOWN((uintptr_t)(p)+((uintptr_t)((P)-1)),P))

/* these macros return the boolean value of the expression given, but pass on
   a hint that you expect the value to be true or false.
   Use them to wrap the conditional expression in an if stmt when
   you have strong reason to believe the branch will frequently go
   in one direction and the branch is a bottleneck
 */
#ifdef GASNETT_PREDICT_TRUE
  #define AMX_PREDICT_TRUE(exp)  GASNETT_PREDICT_TRUE(exp)
  #define AMX_PREDICT_FALSE(exp) GASNETT_PREDICT_FALSE(exp)
#elif defined(__GNUC__) && __GNUC__ >= 3 && 0
  #define AMX_PREDICT_TRUE(exp)  (!__builtin_expect( (!(uintptr_t)(exp)), 0 ))
  #define AMX_PREDICT_FALSE(exp) ( __builtin_expect( ( (uintptr_t)(exp)), 0 ))
#else
  #define AMX_PREDICT_TRUE(exp)  (exp)
  #define AMX_PREDICT_FALSE(exp) (exp)
#endif

#ifndef if_pf
  /* if with branch prediction */
  #define if_pf(cond) if (AMX_PREDICT_FALSE(cond))
  #define if_pt(cond) if (AMX_PREDICT_TRUE(cond))
#endif

//------------------------------------------------------------------------------------
// AMX_IDENT() takes a unique identifier and a textual string and embeds the textual string in the executable file
#define AMX_PRAGMA(x) _Pragma ( #x )

#ifdef GASNETT_IDENT
  #define AMX_IDENT(identName, identText) GASNETT_IDENT(identName, identText)
#else
  #define _AMX_IDENT(identName, identText)    \
    extern char volatile identName[];         \
    char volatile identName[] = identText;    \
    extern char *_##identName##_identfn(void) { return (char*)identName; } \
    static int _dummy_##identName = sizeof(_dummy_##identName)
  #if PLATFORM_COMPILER_CRAY && !PLATFORM_ARCH_X86_64 /* fouls up concatenation in ident string */
    #define AMX_IDENT(identName, identText) \
      AMX_PRAGMA(_CRI ident identText);     \
      _AMX_IDENT(identName, identText)
  #else
    #define AMX_IDENT _AMX_IDENT
  #endif
#endif

//------------------------------------------------------------------------------------
#if PLATFORM_OS_SUPERUX || PLATFORM_OS_NETBSD || \
    PLATFORM_OS_BLRTS || PLATFORM_OS_OPENBSD
  // these implement sched_yield() in libpthread only, which we may not want
  #define AMX_sched_yield() sleep(0)
#elif 0 && defined(HAVE_GASNET_TOOLS) // not currently part of GASNETT_LITE_MODE
  #define AMX_sched_yield() gasnett_sched_yield()
#else
  #include <sched.h>
  #define AMX_sched_yield() sched_yield() 
#endif

#if PLATFORM_OS_BLRTS
  /* lack working select */
  #define AMX_usleep(timeoutusec) sleep(timeoutusec/1000000)
#else
  #define AMX_usleep(timeoutusec) do {                                    \
    struct timeval _tv;                                                   \
    int64_t _timeoutusec = (timeoutusec);                                 \
    _tv.tv_sec  = _timeoutusec / 1000000;                                 \
    _tv.tv_usec = _timeoutusec % 1000000;                                 \
    if (select(1, NULL, NULL, NULL, &_tv) < 0) /* sleep a little while */ \
       AMX_Err("failed to select(): %s(%i)", strerror(errno), errno);     \
  } while (0)
#endif

//------------------------------------------------------------------------------------
// Meta-macros for fixed-count text generation
//
// The following family of macros (named with a integer suffix in N=0..16)
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

#define AMX_META_ASC0(fnb,fni)  fnb(-1,0,1)
#define AMX_META_ASC1(fnb,fni)  AMX_META_ASC0(fnb,fni)  fni(0,1,2)
#define AMX_META_ASC2(fnb,fni)  AMX_META_ASC1(fnb,fni)  fni(1,2,3)
#define AMX_META_ASC3(fnb,fni)  AMX_META_ASC2(fnb,fni)  fni(2,3,4)
#define AMX_META_ASC4(fnb,fni)  AMX_META_ASC3(fnb,fni)  fni(3,4,5)
#define AMX_META_ASC5(fnb,fni)  AMX_META_ASC4(fnb,fni)  fni(4,5,6)
#define AMX_META_ASC6(fnb,fni)  AMX_META_ASC5(fnb,fni)  fni(5,6,7)
#define AMX_META_ASC7(fnb,fni)  AMX_META_ASC6(fnb,fni)  fni(6,7,8)
#define AMX_META_ASC8(fnb,fni)  AMX_META_ASC7(fnb,fni)  fni(7,8,9)
#define AMX_META_ASC9(fnb,fni)  AMX_META_ASC8(fnb,fni)  fni(8,9,10)
#define AMX_META_ASC10(fnb,fni) AMX_META_ASC9(fnb,fni)  fni(9,10,11)
#define AMX_META_ASC11(fnb,fni) AMX_META_ASC10(fnb,fni) fni(10,11,12)
#define AMX_META_ASC12(fnb,fni) AMX_META_ASC11(fnb,fni) fni(11,12,13)
#define AMX_META_ASC13(fnb,fni) AMX_META_ASC12(fnb,fni) fni(12,13,14)
#define AMX_META_ASC14(fnb,fni) AMX_META_ASC13(fnb,fni) fni(13,14,15)
#define AMX_META_ASC15(fnb,fni) AMX_META_ASC14(fnb,fni) fni(14,15,16)
#define AMX_META_ASC16(fnb,fni) AMX_META_ASC15(fnb,fni) fni(15,16,17)

#define AMX_META_DES0(fnb,fni)  fnb(-1,0,1)
#define AMX_META_DES1(fnb,fni)  fni(0,1,2)    AMX_META_DES0(fnb,fni)
#define AMX_META_DES2(fnb,fni)  fni(1,2,3)    AMX_META_DES1(fnb,fni)
#define AMX_META_DES3(fnb,fni)  fni(2,3,4)    AMX_META_DES2(fnb,fni)
#define AMX_META_DES4(fnb,fni)  fni(3,4,5)    AMX_META_DES3(fnb,fni)
#define AMX_META_DES5(fnb,fni)  fni(4,5,6)    AMX_META_DES4(fnb,fni)
#define AMX_META_DES6(fnb,fni)  fni(5,6,7)    AMX_META_DES5(fnb,fni)
#define AMX_META_DES7(fnb,fni)  fni(6,7,8)    AMX_META_DES6(fnb,fni)
#define AMX_META_DES8(fnb,fni)  fni(7,8,9)    AMX_META_DES7(fnb,fni)
#define AMX_META_DES9(fnb,fni)  fni(8,9,10)   AMX_META_DES8(fnb,fni)
#define AMX_META_DES10(fnb,fni) fni(9,10,11)  AMX_META_DES9(fnb,fni)
#define AMX_META_DES11(fnb,fni) fni(10,11,12) AMX_META_DES10(fnb,fni)
#define AMX_META_DES12(fnb,fni) fni(11,12,13) AMX_META_DES11(fnb,fni)
#define AMX_META_DES13(fnb,fni) fni(12,13,14) AMX_META_DES12(fnb,fni)
#define AMX_META_DES14(fnb,fni) fni(13,14,15) AMX_META_DES13(fnb,fni)
#define AMX_META_DES15(fnb,fni) fni(14,15,16) AMX_META_DES14(fnb,fni)
#define AMX_META_DES16(fnb,fni) fni(15,16,17) AMX_META_DES15(fnb,fni)

#endif

