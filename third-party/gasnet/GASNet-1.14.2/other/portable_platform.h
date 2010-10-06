/*   $Source: /var/local/cvs/gasnet/other/portable_platform.h,v $
 *     $Date: 2010/04/17 00:17:07 $
 * $Revision: 1.25.4.3 $
 * Description: Portable platform detection header
 * Copyright 2006, Dan Bonachea <bonachea@cs.berkeley.edu>
 */
#ifndef _PORTABLE_PLATFORM_H
#define _PORTABLE_PLATFORM_H 1

/* the version number for the public interface to this header */
#define PLATFORM_HEADER_VERSION 1

/* most of this file was written based on information in vendor documents, system headers,
   and inspecting verbose compiler output. 
   Another useful source of information: http://predef.sourceforge.net/
*/

/* ------------------------------------------------------------------------------------ */
/* helpers */

#ifndef _STRINGIFY
#define _STRINGIFY_HELPER(x) #x
#define _STRINGIFY(x) _STRINGIFY_HELPER(x)
#endif

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif

/* ------------------------------------------------------------------------------------ */
/* Compiler detection */
/* 
  PLATFORM_COMPILER_<family>:
     defined to 1 if compiler is a given family, undef otherwise
  PLATFORM_COMPILER_<family>_C
  PLATFORM_COMPILER_<family>_CXX
     defined to 1 if compiler is a given family, and is the C or C++ compiler, respectively
  PLATFORM_COMPILER_FAMILYNAME:
     unquoted token which provides the compiler family name
  PLATFORM_COMPILER_FAMILYID:
     defined to a positive integral value which is unique to a given compiler family
     or zero if the compiler is unrecognized
  PLATFORM_COMPILER_ID:
     same as PLATFORM_COMPILER_FAMILYID, except C and C++ compilers are differentiated
  PLATFORM_COMPILER_VERSION:
     defined to an integral expression which is guaranteed to be monotonically non-decreasing 
     with increasing compiler versions. Will be zero for unrecognized compilers.
  PLATFORM_COMPILER_VERSION_STR:
     A string representation of the compiler version, which may contain additional info
  PLATFORM_COMPILER_VERSION_[GT,GE,EQ,LE,LT](maj,min,pat):
     evaluate to non-zero iff the compiler version in use is respectively 
     greater-than, greater-or-equal, equal, less-or-equal, less-than 
     the provided version components
  PLATFORM_COMPILER_IDSTR:
     a string which uniquely identifies recognized compilers
*/

#if defined(__INTEL_COMPILER)
  #define PLATFORM_COMPILER_INTEL  1
  #define PLATFORM_COMPILER_FAMILYNAME INTEL
  #define PLATFORM_COMPILER_FAMILYID 2
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_INTEL_CXX  1
  #else
    #define PLATFORM_COMPILER_INTEL_C  1
  #endif
  #define _PLATFORM_COMPILER_INTEL_MIN_BUILDDATE 19700000 /* year 1970: predates most intel products :) */
  #ifdef __INTEL_COMPILER_BUILD_DATE
    #define _PLATFORM_INTEL_COMPILER_BUILD_DATE __INTEL_COMPILER_BUILD_DATE
  #else
    #define _PLATFORM_INTEL_COMPILER_BUILD_DATE _PLATFORM_COMPILER_INTEL_MIN_BUILDDATE
  #endif
  /* patch number is a decimal build date: YYYYMMDD */
  #define PLATFORM_COMPILER_VERSION_INT(maj,min,pat)         \
        (((((maj) * 10) | (min)) << 20) |                    \
           ((pat) < _PLATFORM_COMPILER_INTEL_MIN_BUILDDATE ? \
            _PLATFORM_COMPILER_INTEL_MIN_BUILDDATE : ((pat)-_PLATFORM_COMPILER_INTEL_MIN_BUILDDATE)))
  #define PLATFORM_COMPILER_VERSION \
          PLATFORM_COMPILER_VERSION_INT(__INTEL_COMPILER/10, __INTEL_COMPILER/100, _PLATFORM_INTEL_COMPILER_BUILD_DATE)
  #define PLATFORM_COMPILER_VERSION_STR \
          _STRINGIFY(__INTEL_COMPILER)"."_STRINGIFY(_PLATFORM_INTEL_COMPILER_BUILD_DATE)

#elif defined(__PATHSCALE__)
  #define PLATFORM_COMPILER_PATHSCALE  1
  #define PLATFORM_COMPILER_FAMILYNAME PATHSCALE
  #define PLATFORM_COMPILER_FAMILYID 3
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_PATHSCALE_CXX  1
  #else
    #define PLATFORM_COMPILER_PATHSCALE_C  1
  #endif
  #define PLATFORM_COMPILER_VERSION \
          PLATFORM_COMPILER_VERSION_INT(__PATHCC__,__PATHCC_MINOR__,__PATHCC_PATCHLEVEL__+0)
  #define PLATFORM_COMPILER_VERSION_STR __PATHSCALE__

#elif defined(__PGI)
  #define PLATFORM_COMPILER_PGI  1
  #define PLATFORM_COMPILER_FAMILYNAME PGI
  #define PLATFORM_COMPILER_FAMILYID 4
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_PGI_CXX  1
  #else
    #define PLATFORM_COMPILER_PGI_C  1
  #endif
  #if __PGIC__ == 99 
    /* bug 2230: PGI versioning was broken for some platforms in 7.0
                 no way to know exact version, but provide something slightly more accurate */
    #define PLATFORM_COMPILER_VERSION 0x070000
    #define PLATFORM_COMPILER_VERSION_STR "7.?-?"
  #elif defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__)
    #define PLATFORM_COMPILER_VERSION \
            PLATFORM_COMPILER_VERSION_INT(__PGIC__,__PGIC_MINOR__,__PGIC_PATCHLEVEL__)
    #define PLATFORM_COMPILER_VERSION_STR \
            _STRINGIFY(__PGIC__)"."_STRINGIFY(__PGIC_MINOR__)"-"_STRINGIFY(__PGIC_PATCHLEVEL__)
  #else
    /* PGI before 6.1-4 lacks any version ID preprocessor macros - so use this filthy hack */
    #ifdef PLATFORM_PGI_IS_ANCIENT
      /* Include below might fail for ancient versions lacking this header, but testing shows it
         works back to at least 5.1-3 (Nov 2003), and based on docs probably back to 3.2 (Sep 2000) */
        #define PLATFORM_COMPILER_VERSION 0       
    #elif defined(__x86_64__) /* bug 1753 - 64-bit omp.h upgrade happenned in <6.0-8,6.1-1] */
      #include "omp.h"
      #if defined(_PGOMP_H)
        /* 6.1.1 or newer */
        #define PLATFORM_COMPILER_VERSION 0x060101
        #define PLATFORM_COMPILER_VERSION_STR ">=6.1-1"
      #else
        /* 6.0.8 or older */
        #define PLATFORM_COMPILER_VERSION 0
        #define PLATFORM_COMPILER_VERSION_STR "<=6.0-8"
      #endif
    #else /* 32-bit omp.h upgrade happenned in <5.2-4,6.0-8] */
      #include "omp.h"
      #if defined(_PGOMP_H)
        /* 6.0-8 or newer */
        #define PLATFORM_COMPILER_VERSION 0x060008
        #define PLATFORM_COMPILER_VERSION_STR ">=6.0-8"
      #else
        /* 5.2-4 or older */
        #define PLATFORM_COMPILER_VERSION 0
        #define PLATFORM_COMPILER_VERSION_STR "<=5.2-4"
      #endif
    #endif
  #endif

#elif defined(__xlC__) 
  #define PLATFORM_COMPILER_XLC  1
  #define PLATFORM_COMPILER_FAMILYNAME XLC
  #define PLATFORM_COMPILER_FAMILYID 5
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_XLC_CXX  1
  #else
    #define PLATFORM_COMPILER_XLC_C  1
  #endif
  #define PLATFORM_COMPILER_VERSION __xlC__
  #define PLATFORM_COMPILER_VERSION_INT(maj,min,pat) \
        ( ((maj) << 8) | ((min) << 4) | (pat) )

#elif defined(__DECC) || defined(__DECCXX)
  #define PLATFORM_COMPILER_COMPAQ  1
  #define PLATFORM_COMPILER_FAMILYNAME COMPAQ
  #define PLATFORM_COMPILER_FAMILYID 6
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_COMPAQ_CXX  1
  #else
    #define PLATFORM_COMPILER_COMPAQ_C  1
  #endif
  #if defined(__DECC_VER)
    #define PLATFORM_COMPILER_VERSION __DECC_VER
  #elif defined(__DECCXX_VER)
    #define PLATFORM_COMPILER_VERSION __DECCXX_VER
  #endif

  #define PLATFORM_COMPILER_VERSION_INT(maj,min,pat) \
        ( ((maj) * 10000000) + ((min) * 100000) + (90000) + (pat) )
  /* 90000 = official ver, 80000 = customer special ver, 60000 = field test ver */

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
  #define PLATFORM_COMPILER_SUN  1
  #define PLATFORM_COMPILER_FAMILYNAME SUN
  #define PLATFORM_COMPILER_FAMILYID 7
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_SUN_CXX  1
  #else
    #define PLATFORM_COMPILER_SUN_C  1
  #endif
  #if defined(__SUNPRO_C) && __SUNPRO_C > 0
    #define PLATFORM_COMPILER_VERSION __SUNPRO_C
  #elif defined(__SUNPRO_CC) && __SUNPRO_CC > 0
    #define PLATFORM_COMPILER_VERSION __SUNPRO_CC
  #endif
  #define PLATFORM_COMPILER_VERSION_INT(maj,min,pat) \
        ( ((maj) << 8) | ((min) << 4) | (pat) )

#elif defined(__HP_cc) || defined(__HP_aCC)
  #define PLATFORM_COMPILER_HP  1
  #define PLATFORM_COMPILER_FAMILYNAME HP
  #define PLATFORM_COMPILER_FAMILYID 8
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_HP_CXX  1
  #else
    #define PLATFORM_COMPILER_HP_C  1
  #endif
  #if defined(__HP_cc) && __HP_cc > 0
    #define PLATFORM_COMPILER_VERSION __HP_cc
  #elif defined(__HP_aCC) && __HP_aCC > 0
    #define PLATFORM_COMPILER_VERSION __HP_aCC
  #endif
  #define PLATFORM_COMPILER_VERSION_INT(maj,min,pat) \
        ( ((maj) << 16) | ((min) << 8) | (pat) )

#elif defined(_SGI_COMPILER_VERSION) || \
   (defined(_COMPILER_VERSION) && defined(__sgi) && !defined(__GNUC__)) /* 7.3.0 and earlier lack _SGI_COMPILER_VERSION */
  #define PLATFORM_COMPILER_SGI  1
  #define PLATFORM_COMPILER_FAMILYNAME SGI
  #define PLATFORM_COMPILER_FAMILYID 9
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_SGI_CXX  1
  #else
    #define PLATFORM_COMPILER_SGI_C  1
  #endif
  #if defined(_SGI_COMPILER_VERSION) && _SGI_COMPILER_VERSION > 0
    #define PLATFORM_COMPILER_VERSION _SGI_COMPILER_VERSION
  #elif defined(_COMPILER_VERSION) && _COMPILER_VERSION > 0
    #define PLATFORM_COMPILER_VERSION _COMPILER_VERSION
  #endif
  #define PLATFORM_COMPILER_VERSION_INT(maj,min,pat) \
        ( ((maj) << 8) | ((min) << 4) | (pat) )

#elif defined(_CRAYC) 
  #define PLATFORM_COMPILER_CRAY  1
  #define PLATFORM_COMPILER_FAMILYNAME CRAY
  #define PLATFORM_COMPILER_FAMILYID 10
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_CRAY_CXX  1
  #else
    #define PLATFORM_COMPILER_CRAY_C  1
  #endif
  #if defined(_RELEASE) && defined(_RELEASE_MINOR) /* X1 and XT */
    #define PLATFORM_COMPILER_VERSION \
            PLATFORM_COMPILER_VERSION_INT(_RELEASE,_RELEASE_MINOR,0)
  #elif defined(_RELEASE) /* T3E */
    #define PLATFORM_COMPILER_VERSION \
            PLATFORM_COMPILER_VERSION_INT(_RELEASE,0,0)
  #endif
  #ifdef _RELEASE_STRING /* X1 and XT */
    #define PLATFORM_COMPILER_VERSION_STR _RELEASE_STRING
  #endif

#elif defined(__KCC)
  #define PLATFORM_COMPILER_KAI  1
  #define PLATFORM_COMPILER_FAMILYNAME KAI
  #define PLATFORM_COMPILER_FAMILYID 11
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_KAI_CXX  1
  #else
    #define PLATFORM_COMPILER_KAI_C  1
  #endif

#elif defined(__MTA__)
  #define PLATFORM_COMPILER_MTA  1
  #define PLATFORM_COMPILER_FAMILYNAME MTA
  #define PLATFORM_COMPILER_FAMILYID 12
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_MTA_CXX  1
  #else
    #define PLATFORM_COMPILER_MTA_C  1
  #endif

#elif defined(_SX)
  #define PLATFORM_COMPILER_NECSX  1
  #define PLATFORM_COMPILER_FAMILYNAME NECSX
  #define PLATFORM_COMPILER_FAMILYID 13
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_NECSX_CXX  1
  #else
    #define PLATFORM_COMPILER_NECSX_C  1
  #endif

#elif defined(_MSC_VER)
  #define PLATFORM_COMPILER_MICROSOFT  1
  #define PLATFORM_COMPILER_FAMILYNAME MICROSOFT
  #define PLATFORM_COMPILER_FAMILYID 14
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_MICROSOFT_CXX  1
  #else
    #define PLATFORM_COMPILER_MICROSOFT_C  1
  #endif
  #define PLATFORM_COMPILER_VERSION _MSC_VER

#elif defined(__TINYC__)
  #define PLATFORM_COMPILER_TINY  1
  #define PLATFORM_COMPILER_FAMILYNAME TINY
  #define PLATFORM_COMPILER_FAMILYID 15
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_TINY_CXX  1
  #else
    #define PLATFORM_COMPILER_TINY_C  1
  #endif

#elif defined(__LCC__)
  #define PLATFORM_COMPILER_LCC 1
  #define PLATFORM_COMPILER_FAMILYNAME LCC
  #define PLATFORM_COMPILER_FAMILYID 16
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_LCC_CXX  1
  #else
    #define PLATFORM_COMPILER_LCC_C  1
  #endif

#elif defined(__OPENCC__)
  #define PLATFORM_COMPILER_OPEN64  1
  #define PLATFORM_COMPILER_FAMILYNAME OPEN64
  #define PLATFORM_COMPILER_FAMILYID 17
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_OPEN64_CXX  1
  #else
    #define PLATFORM_COMPILER_OPEN64_C  1
  #endif
  #define PLATFORM_COMPILER_VERSION \
          PLATFORM_COMPILER_VERSION_INT(__OPENCC__,__OPENCC_MINOR__,__OPENCC_PATCHLEVEL__+0)
  #define PLATFORM_COMPILER_VERSION_STR __OPEN64__

#elif defined(__PCC__)
  #define PLATFORM_COMPILER_PCC  1
  #define PLATFORM_COMPILER_FAMILYNAME PCC
  #define PLATFORM_COMPILER_FAMILYID 18
  #ifdef __cplusplus
    #define PLATFORM_COMPILER_PCC_CXX  1
  #else
    #define PLATFORM_COMPILER_PCC_C  1
  #endif
  #define PLATFORM_COMPILER_VERSION \
          PLATFORM_COMPILER_VERSION_INT(__PCC__,__PCC_MINOR__,__PCC_MINORMINOR__)
  #define PLATFORM_COMPILER_VERSION_STR \
      _STRINGIFY(__PCC__)"."_STRINGIFY(__PCC_MINOR__)"."_STRINGIFY(__PCC_MINORMINOR__)

#else /* unknown compiler */
  #define PLATFORM_COMPILER_UNKNOWN  1
#endif

/* this stanza comes last, because many vendor compilers lie and claim 
   to be GNU C for compatibility reasons and/or because they share a frontend */ 
#if defined(__GNUC__)
  #undef PLATFORM_COMPILER_UNKNOWN
  #ifndef PLATFORM_COMPILER_FAMILYID
    #define PLATFORM_COMPILER_GNU  1
    #define PLATFORM_COMPILER_FAMILYNAME GNU
    #define PLATFORM_COMPILER_FAMILYID 1
    #ifdef __cplusplus
      #define PLATFORM_COMPILER_GNU_CXX  1
    #else
      #define PLATFORM_COMPILER_GNU_C  1
    #endif
   #if defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
    #define PLATFORM_COMPILER_VERSION \
            PLATFORM_COMPILER_VERSION_INT(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
   #elif defined(__GNUC_MINOR__) /* older versions of egcs lack __GNUC_PATCHLEVEL__ */
    #define PLATFORM_COMPILER_VERSION \
            PLATFORM_COMPILER_VERSION_INT(__GNUC__,__GNUC_MINOR__,0)
   #else
    #define PLATFORM_COMPILER_VERSION \
            PLATFORM_COMPILER_VERSION_INT(__GNUC__,0,0)
   #endif
    #define PLATFORM_COMPILER_VERSION_STR __PLATFORM_COMPILER_GNU_VERSION_STR
  #else
    #define _PLATFORM_COMPILER_GNU_VERSION_STR __PLATFORM_COMPILER_GNU_VERSION_STR
  #endif
  /* gather any advertised GNU version number info, even for non-gcc compilers */
  #if defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
    #define __PLATFORM_COMPILER_GNU_VERSION_STR \
      _STRINGIFY(__GNUC__)"."_STRINGIFY(__GNUC_MINOR__)"."_STRINGIFY(__GNUC_PATCHLEVEL__)
  #elif defined(__GNUC_MINOR__)
    #define __PLATFORM_COMPILER_GNU_VERSION_STR \
      _STRINGIFY(__GNUC__)"."_STRINGIFY(__GNUC_MINOR__)".?"
  #else
    #define __PLATFORM_COMPILER_GNU_VERSION_STR \
      _STRINGIFY(__GNUC__)".?.?"
  #endif
#elif defined(PLATFORM_COMPILER_UNKNOWN) /* unknown compiler */
  #define PLATFORM_COMPILER_FAMILYNAME UNKNOWN
  #define PLATFORM_COMPILER_FAMILYID 0 
#endif

/* defaulting */

#ifndef PLATFORM_COMPILER_VERSION
#define PLATFORM_COMPILER_VERSION 0 /* don't know */
#endif

#ifndef PLATFORM_COMPILER_VERSION_STR
#define PLATFORM_COMPILER_VERSION_STR _STRINGIFY(PLATFORM_COMPILER_VERSION)
#endif

#ifndef PLATFORM_COMPILER_VERSION_INT
#define PLATFORM_COMPILER_VERSION_INT(maj,min,pat) \
        (((maj) << 16) | ((min) << 8) | (pat))
#endif

/* version check macros */

#define PLATFORM_COMPILER_VERSION_GT(maj,min,pat) \
        PLATFORM_COMPILER_VERSION >  PLATFORM_COMPILER_VERSION_INT(maj,min,pat)
#define PLATFORM_COMPILER_VERSION_GE(maj,min,pat) \
        PLATFORM_COMPILER_VERSION >= PLATFORM_COMPILER_VERSION_INT(maj,min,pat)
#define PLATFORM_COMPILER_VERSION_EQ(maj,min,pat) \
        PLATFORM_COMPILER_VERSION == PLATFORM_COMPILER_VERSION_INT(maj,min,pat)
#define PLATFORM_COMPILER_VERSION_LE(maj,min,pat) \
        PLATFORM_COMPILER_VERSION <= PLATFORM_COMPILER_VERSION_INT(maj,min,pat)
#define PLATFORM_COMPILER_VERSION_LT(maj,min,pat) \
        PLATFORM_COMPILER_VERSION <  PLATFORM_COMPILER_VERSION_INT(maj,min,pat)

/* misc feature detection */

#ifdef __cplusplus
  #define PLATFORM_COMPILER_ID (10000+PLATFORM_COMPILER_FAMILYID)
#else
  #define PLATFORM_COMPILER_ID PLATFORM_COMPILER_FAMILYID
#endif

#ifdef __STDC__
  #define _PLATFORM_COMPILER_STD_STDC "__STDC__"
#else
  #define _PLATFORM_COMPILER_STD_STDC "-"
#endif
#ifdef __STDC_VERSION__
  #define _PLATFORM_COMPILER_STD_STDC_VERSION ",__STDC_VERSION__="_STRINGIFY(__STDC_VERSION__)
#else
  #define _PLATFORM_COMPILER_STD_STDC_VERSION
#endif
#ifdef __STDC_EXT__
  #define _PLATFORM_COMPILER_STD_STDC_EXT ",__STDC_EXT__="_STRINGIFY(__STDC_EXT__)
#else
  #define _PLATFORM_COMPILER_STD_STDC_EXT 
#endif
#ifdef __cplusplus
  #define _PLATFORM_COMPILER_STD_CPLUSPLUS ",__cplusplus="_STRINGIFY(__cplusplus)
#else
  #define _PLATFORM_COMPILER_STD_CPLUSPLUS 
#endif

#ifndef _PLATFORM_COMPILER_MISC_VERSION_STR
  #ifdef __VERSION__
    #define _PLATFORM_COMPILER_MISC_VERSION_STR "|misc:"__VERSION__
  #else
    #define _PLATFORM_COMPILER_MISC_VERSION_STR
  #endif
#endif
#ifdef _PLATFORM_COMPILER_GNU_VERSION_STR
    #define _PLATFORM_COMPILER_GNU_VERSION_STR_HELP "|GNU:"_PLATFORM_COMPILER_GNU_VERSION_STR
#else
    #define _PLATFORM_COMPILER_GNU_VERSION_STR_HELP
#endif

#define PLATFORM_COMPILER_IDSTR                                      \
        "|COMPILER_FAMILY:" _STRINGIFY(PLATFORM_COMPILER_FAMILYNAME) \
        "|COMPILER_VERSION:" PLATFORM_COMPILER_VERSION_STR           \
        "|COMPILER_FAMILYID:" _STRINGIFY(PLATFORM_COMPILER_FAMILYID) \
         _PLATFORM_COMPILER_GNU_VERSION_STR_HELP                     \
        "|STD:" _PLATFORM_COMPILER_STD_STDC                          \
                _PLATFORM_COMPILER_STD_STDC_VERSION                  \
                _PLATFORM_COMPILER_STD_STDC_EXT                      \
                _PLATFORM_COMPILER_STD_CPLUSPLUS                     \
         _PLATFORM_COMPILER_MISC_VERSION_STR                         \
        "|"

/* ------------------------------------------------------------------------------------ */
/* OS detection */
/* 
   PLATFORM_OS_<family>:
     defined to a positive value if OS belongs to a given family, undef otherwise
   PLATFORM_OS_FAMILYNAME:
     unquoted token which provides the compiler family name
*/

#if defined(__LIBCATAMOUNT__) || defined(__QK_USER__)
  #define PLATFORM_OS_CATAMOUNT 1
  #define PLATFORM_OS_FAMILYNAME CATAMOUNT

#elif defined(__CRAYXT_COMPUTE_LINUX_TARGET)
  #define PLATFORM_OS_CNL 1
  #define PLATFORM_OS_FAMILYNAME CNL

#elif defined(__blrts) || defined(__blrts__) || defined(__gnu_blrts__)
  #define PLATFORM_OS_BLRTS 1
  #define PLATFORM_OS_FAMILYNAME BLRTS

#elif defined(GASNETI_ARCH_BGP) || defined(__bgp__)
  #define PLATFORM_OS_BGP 1
  #define PLATFORM_OS_FAMILYNAME BGP

#elif defined(__K42)
  #define PLATFORM_OS_K42 1
  #define PLATFORM_OS_FAMILYNAME K42

#elif defined(__uClinux__)
  #define PLATFORM_OS_UCLINUX 1
  #define PLATFORM_OS_FAMILYNAME UCLINUX

#elif defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
  #define PLATFORM_OS_LINUX 1
  #define PLATFORM_OS_FAMILYNAME LINUX

#elif defined(__CYGWIN__)
  #define PLATFORM_OS_CYGWIN 1
  #define PLATFORM_OS_FAMILYNAME CYGWIN

#elif defined(_WIN32)
  #define PLATFORM_OS_MSWINDOWS 1
  #define PLATFORM_OS_FAMILYNAME MSWINDOWS

#elif defined(_AIX)
  #define PLATFORM_OS_AIX 1
  #define PLATFORM_OS_FAMILYNAME AIX

#elif defined(__osf__) || defined(__digital__)
  #define PLATFORM_OS_TRU64 1
  #define PLATFORM_OS_FAMILYNAME TRU64

#elif defined(__FreeBSD) || defined(__FreeBSD__)
  #define PLATFORM_OS_FREEBSD 1
  #define PLATFORM_OS_FAMILYNAME FREEBSD

#elif defined(__NetBSD) || defined(__NetBSD__)
  #define PLATFORM_OS_NETBSD 1
  #define PLATFORM_OS_FAMILYNAME NETBSD

#elif defined(__OpenBSD__)
  #define PLATFORM_OS_OPENBSD 1
  #define PLATFORM_OS_FAMILYNAME OPENBSD

#elif defined(__sun) || defined(__sun__)
  #define PLATFORM_OS_SOLARIS 1
  #define PLATFORM_OS_FAMILYNAME SOLARIS

#elif (defined(__APPLE__) && defined(__MACH__)) || \
      defined(__osx86__) /* PGI on OSX */
  #define PLATFORM_OS_DARWIN 1
  #define PLATFORM_OS_FAMILYNAME DARWIN

#elif defined(__sgi) || defined(__sgi__)
  #define PLATFORM_OS_IRIX 1
  #define PLATFORM_OS_FAMILYNAME IRIX

#elif defined(__hpux) || defined(__hpux__)
  #define PLATFORM_OS_HPUX 1
  #define PLATFORM_OS_FAMILYNAME HPUX

#elif defined(_CRAY) || defined(_UNICOSMP)
  #define PLATFORM_OS_UNICOS 1
  #define PLATFORM_OS_FAMILYNAME UNICOS

#elif defined(__MTA__)
  #define PLATFORM_OS_MTA 1
  #define PLATFORM_OS_FAMILYNAME MTA

#elif defined(_SX)
  #define PLATFORM_OS_SUPERUX 1
  #define PLATFORM_OS_FAMILYNAME SUPERUX

#else
  #define PLATFORM_OS_UNKNOWN 1
  #define PLATFORM_OS_FAMILYNAME UNKNOWN
#endif

/* ------------------------------------------------------------------------------------ */
/* Architecture detection */
/* 
   PLATFORM_ARCH_<family>:
     defined to positive value if CPU belongs to a given family, undef otherwise
   PLATFORM_ARCH_FAMILYNAME:
     unquoted token which provides the CPU family name

   PLATFORM_ARCH_BIG_ENDIAN 
   PLATFORM_ARCH_LITTLE_ENDIAN:
     defined to positive value if CPU is known to be big/little endian, undef otherwise

   PLATFORM_ARCH_32              - 32-bit pointers
   PLATFORM_ARCH_64              - 64-bit pointers
   PLATFORM_ARCH_BIG_ENDIAN      - big-endian word order
   PLATFORM_ARCH_LITTLE_ENDIAN   - little-endian word order
     defined to positive value if CPU is known to have the indicated property, undef otherwise
 */

#if defined(__ppc64) || defined(__ppc64__) || \
    defined(__PPC64) || defined(__PPC64__) || \
    defined(__powerpc64) || defined(__powerpc64__) 
  #define PLATFORM_ARCH_POWERPC 1
  #define PLATFORM_ARCH_FAMILYNAME POWERPC
  #define _PLATFORM_ARCH_64 1
  #define PLATFORM_ARCH_BIG_ENDIAN 1

#elif defined(_POWER) || \
      defined(__PPC)  || defined(__PPC__) || \
      defined(__powerpc) || defined(__powerpc__) || \
      defined(__ppc) || defined(__ppc__) || \
      defined(__POWERPC__)
  #define PLATFORM_ARCH_POWERPC 1
  #define PLATFORM_ARCH_FAMILYNAME POWERPC
  #define _PLATFORM_ARCH_32 1
  #define PLATFORM_ARCH_BIG_ENDIAN 1

#elif defined(_ARCH_PPC) || defined(_ARCH_PPC64)
  #define PLATFORM_ARCH_POWERPC 1
  #define PLATFORM_ARCH_FAMILYNAME POWERPC
  #define PLATFORM_ARCH_BIG_ENDIAN 1

#elif defined(__x86_64) || defined(__x86_64__) || \
    defined(__athlon) || defined(__athlon__) || \
    defined(__amd64)  || defined(__amd64__)
  #define PLATFORM_ARCH_X86_64 1
  #define PLATFORM_ARCH_FAMILYNAME X86_64
  #define _PLATFORM_ARCH_64 1
  #define PLATFORM_ARCH_LITTLE_ENDIAN 1

#elif defined(__ia64__) || defined(__ia64)
  #define PLATFORM_ARCH_IA64 1
  #define PLATFORM_ARCH_FAMILYNAME IA64
  #define _PLATFORM_ARCH_64 1
  #if defined(PLATFORM_OS_LINUX) || defined(PLATFORM_OS_FREEBSD)
    #define PLATFORM_ARCH_LITTLE_ENDIAN 1
  #elif defined(PLATFORM_OS_HPUX)
    #define PLATFORM_ARCH_BIG_ENDIAN 1
  #else
    /* Unknown.  Hope one of the other mechanisms can sort it out. */
  #endif

#elif defined(__i386__) || defined(__i386) || \
      defined(__i486__) || defined(__i486) || \
      defined(__i586__) || defined(__i586) || \
      defined(__i686__) || defined(__i686) || \
      defined(__pentiumpro) || defined(__pentiumpro__) || \
      defined(_M_IX86)
  #define PLATFORM_ARCH_X86 1
  #define PLATFORM_ARCH_FAMILYNAME X86
  #define _PLATFORM_ARCH_32 1
  #define PLATFORM_ARCH_LITTLE_ENDIAN 1

#elif defined(__alpha) || defined(__alpha__)
  #define PLATFORM_ARCH_ALPHA 1
  #define PLATFORM_ARCH_FAMILYNAME ALPHA
  #define _PLATFORM_ARCH_64 1
  #define PLATFORM_ARCH_LITTLE_ENDIAN 1

#elif defined(_mips) || defined(__mips) || defined(__mips__) || \
    defined(__host_mips) || defined(__host_mips__) || \
    defined(_MIPS_ARCH) || defined(__R4000)
  #define PLATFORM_ARCH_MIPS 1
  #define PLATFORM_ARCH_FAMILYNAME MIPS
  #ifdef _MIPSEL /* MIPS cores support both little and big endian modes */
    /* SiCortex */
    #define PLATFORM_ARCH_LITTLE_ENDIAN 1
  #else
    /* IRIX */
    #define PLATFORM_ARCH_BIG_ENDIAN 1
  #endif

#elif defined(__sparc) || defined(__sparc__) || \
    defined(__sparclet__) || defined(__sparclite__) || \
    defined(__sparcv8) || defined(__sparcv9)
  #define PLATFORM_ARCH_SPARC 1
  #define PLATFORM_ARCH_FAMILYNAME SPARC
  #define PLATFORM_ARCH_BIG_ENDIAN 1

#elif defined(__hppa) || defined(__hppa__) || \
    defined(__parisc) || defined(__parisc__) || \
    defined(_PA_RISC1_1) || defined(_PA_RISC2_0)
  #define PLATFORM_ARCH_PARISC 1
  #define PLATFORM_ARCH_FAMILYNAME PARISC
  #define PLATFORM_ARCH_BIG_ENDIAN 1

#elif defined(__crayx1)
  #define PLATFORM_ARCH_CRAYX1 1
  #define PLATFORM_ARCH_FAMILYNAME CRAYX1
  #define PLATFORM_ARCH_BIG_ENDIAN 1
  #define _PLATFORM_ARCH_64 1

#elif defined(_CRAYT3E)
  #define PLATFORM_ARCH_CRAYT3E 1
  #define PLATFORM_ARCH_FAMILYNAME CRAYT3E
  #define PLATFORM_ARCH_BIG_ENDIAN 1
  #define _PLATFORM_ARCH_64 1

#elif defined(__MTA__)
  #define PLATFORM_ARCH_MTA 1
  #define PLATFORM_ARCH_FAMILYNAME MTA

#elif defined(_SX)
  #define PLATFORM_ARCH_NECSX 1
  #define PLATFORM_ARCH_FAMILYNAME NECSX

#elif defined(__MICROBLAZE__)
  #define PLATFORM_ARCH_MICROBLAZE 1
  #define PLATFORM_ARCH_FAMILYNAME MICROBLAZE
  #define PLATFORM_ARCH_BIG_ENDIAN 1
  #define _PLATFORM_ARCH_32 1

#elif defined(__arm__)
  #define PLATFORM_ARCH_ARM 1
  #define PLATFORM_ARCH_FAMILYNAME ARM
  #define _PLATFORM_ARCH_32 1
  #if defined(__ARMEB__)
    #define PLATFORM_ARCH_BIG_ENDIAN 1
  #elif defined(__ARMEL__)
    #define PLATFORM_ARCH_LITTLE_ENDIAN 1
  #endif

#else /* unknown CPU */
  #define PLATFORM_ARCH_UNKNOWN 1
  #define PLATFORM_ARCH_FAMILYNAME UNKNOWN
#endif

/* generic chip properties */

#if !defined(PLATFORM_ARCH_BIG_ENDIAN) && !defined(PLATFORM_ARCH_LITTLE_ENDIAN)
  #if defined(_BIG_ENDIAN) || defined(__BIG_ENDIAN__) || defined(WORDS_BIGENDIAN)
    #define PLATFORM_ARCH_BIG_ENDIAN 1
  #endif
#endif

#if defined(PLATFORM_ARCH_BIG_ENDIAN) && defined(PLATFORM_ARCH_LITTLE_ENDIAN)
  #error conflicting endianness information
#endif

/* PLATFORM_ARCH_{32,64}: 
    first trust SIZEOF_VOID_P, which is most likely to be accurate
    next, detect common 32/64 preprocessor defines
    finally default to any arch-specific value provided
 */
#if defined(PLATFORM_ARCH_64) || defined(PLATFORM_ARCH_32)
  #error internal error in bit width configuration
#endif

#if SIZEOF_VOID_P == 8
  #define PLATFORM_ARCH_64 1
#elif SIZEOF_VOID_P == 4
  #define PLATFORM_ARCH_32 1
#elif defined(_LP64) || defined(__LP64__) || \
      defined(__arch64__) || defined(__64BIT__)
  #define PLATFORM_ARCH_64 1
#elif defined(_ILP32) || defined(__ILP32__) || \
      defined(__arch32__) || defined(__32BIT__)
  #define PLATFORM_ARCH_32 1
#elif _PLATFORM_ARCH_64
  #define PLATFORM_ARCH_64 1
#elif _PLATFORM_ARCH_32
  #define PLATFORM_ARCH_32 1
#endif

#if defined(PLATFORM_ARCH_64) && defined(PLATFORM_ARCH_32)
  #error conflicting bit width information
#elif !defined(PLATFORM_ARCH_64) && !defined(PLATFORM_ARCH_32)
  #error missing bit width information
#endif

/* ------------------------------------------------------------------------------------ */
/* handy test code that can be parsed after preprocess or executed to show platform results */
#ifdef PLATFORM_SHOW
#include <stdio.h>
const char *
COMPILER_FAMILYNAME = _STRINGIFY(PLATFORM_COMPILER_FAMILYNAME)
, *
COMPILER_FAMILYID = _STRINGIFY(PLATFORM_COMPILER_FAMILYID)
, *
COMPILER_VERSION_STR = PLATFORM_COMPILER_VERSION_STR
, *
COMPILER_IDSTR = PLATFORM_COMPILER_IDSTR
, *
OS_FAMILYNAME = _STRINGIFY(PLATFORM_OS_FAMILYNAME)
, *
ARCH_FAMILYNAME = _STRINGIFY(PLATFORM_ARCH_FAMILYNAME)
;
int main(void) {
  #define PLATFORM_DISP(x) printf("PLATFORM_"#x"=%s\n",x)
  #define PLATFORM_DISPI(x) printf("PLATFORM_"#x"=%i\n",PLATFORM_##x)
  #define PLATFORM_DISPX(x) printf("PLATFORM_"#x"=0x%x\n",PLATFORM_##x)
  PLATFORM_DISP(COMPILER_FAMILYNAME);
  PLATFORM_DISP(COMPILER_FAMILYID);
  PLATFORM_DISPI(COMPILER_ID);
  PLATFORM_DISPX(COMPILER_VERSION);
  PLATFORM_DISP(COMPILER_VERSION_STR);
  PLATFORM_DISP(COMPILER_IDSTR);
  PLATFORM_DISP(OS_FAMILYNAME);
  PLATFORM_DISP(ARCH_FAMILYNAME);
  return 0;
}
#endif
/* ------------------------------------------------------------------------------------ */
#endif
