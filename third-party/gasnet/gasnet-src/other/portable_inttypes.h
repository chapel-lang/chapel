/*    $Source: bitbucket.org:berkeleylab/gasnet.git/other/portable_inttypes.h $ */
/*  Description: portable_inttypes.h  */
/*  Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu> */

/* inttypes.h is part of the POSIX and C99 specs, but in practice support for it 
   varies wildly across systems. We need a totally portable way to unambiguously
   get the fixed-bit-width integral types, and this file provides that via the 
   following typedefs:
      int8_t, uint8_t     signed/unsigned 8-bit integral types
     int16_t, uint16_t    signed/unsigned 16-bit integral types
     int32_t, uint32_t    signed/unsigned 32-bit integral types
     int64_t, uint64_t    signed/unsigned 64-bit integral types
     intptr_t, uintptr_t  signed/unsigned types big enough to hold any pointer offset
   In general, it uses the system inttypes.h when it's known to be available,
   (as reported by configure via a previously-included config.h file), otherwise
   it uses configure-detected sizes for the types to try and auto construct the
   types. Some specific systems with known issues are handled specially.
*/


#ifndef _PORTABLE_INTTYPES_H
#define _PORTABLE_INTTYPES_H

#ifndef _INTTYPES_DEFINED
#define _INTTYPES_DEFINED
  /* compilers complying to modern standards shouldn't need this header at all */
  #if (__cplusplus >= 201103L || __STDC_VERSION__ >= 199901L) && !defined(COMPLETE_INTTYPES_H)
    #define COMPLETE_INTTYPES_H 1
  #endif
  /* first, check for the easy and preferred case - 
     if configure reports that a standards-compliant 
     system header is available, then use it */
  #if COMPLETE_INTTYPES_H
    /* inttypes.h is preferred, because it also provides format specifiers */
    #ifndef __STDC_FORMAT_MACROS
    #define __STDC_FORMAT_MACROS 1 /* C99 mandates this predef for C++ clients */
    #endif
    #include <inttypes.h>
  #elif COMPLETE_STDINT_H
    #include <stdint.h>
  #elif COMPLETE_SYS_TYPES_H
    #include <sys/types.h>
  /* next, certain known systems are handled specially */
  #elif defined(_WIN32) && defined(_MSC_VER) /* MS Visual C++ */
    typedef signed __int8      int8_t;
    typedef unsigned __int8   uint8_t;
    typedef __int16           int16_t;
    typedef unsigned __int16 uint16_t;
    typedef __int32           int32_t;
    typedef unsigned __int32 uint32_t;
    typedef __int64           int64_t;
    typedef unsigned __int64 uint64_t;

    typedef          int     intptr_t; 
    typedef unsigned int    uintptr_t; 
  #elif defined(_CRAYT3E)
    /* oddball architecture lacking a 16-bit type */
    #define INTTYPES_16BIT_MISSING 1
    typedef signed char        int8_t;
    typedef unsigned char     uint8_t;
    typedef short             int16_t; /* This is 32-bits, should be 16 !!! */
    typedef unsigned short   uint16_t; /* This is 32-bits, should be 16 !!! */
    typedef short             int32_t;
    typedef unsigned short   uint32_t;
    typedef int               int64_t;
    typedef unsigned int     uint64_t;

    typedef          int     intptr_t; 
    typedef unsigned int    uintptr_t; 
  #elif defined(__MTA__)
    #include <sys/types.h>
    typedef u_int8_t          uint8_t;
    typedef u_int16_t        uint16_t;
    typedef u_int32_t        uint32_t;
    typedef u_int64_t        uint64_t;
    typedef int64_t          intptr_t;
    typedef u_int64_t       uintptr_t;
  #elif defined(_SX)
    #include <sys/types.h> /* provides int32_t and uint32_t - use to prevent conflict */
    typedef signed char        int8_t;
    typedef unsigned char     uint8_t;
    typedef short             int16_t;
    typedef unsigned short   uint16_t;
    typedef long              int64_t;
    typedef unsigned long    uint64_t;

    typedef          long    intptr_t; 
    typedef unsigned long   uintptr_t; 
  #elif defined(__CYGWIN__)
   #include <cygwin/version.h>
   #if CYGWIN_VERSION_DLL_MAJOR >= 2005
    /* stdint.h is complete on modern cygwin */
    #include <stdint.h>
   #else
    /* what a mess - 
       inttypes.h and stdint.h are incomplete or missing on 
       older versions of cygwin, with no easy way to check */
    #ifdef HAVE_INTTYPES_H
      #include <inttypes.h>
    #endif
    #ifdef HAVE_STDINT_H
      #include <stdint.h>
    #endif
    #ifdef HAVE_SYS_TYPES_H
      #include <sys/types.h>
    #endif
    #ifndef __uint32_t_defined
      typedef u_int8_t     uint8_t;
      typedef u_int16_t   uint16_t; 
      typedef u_int32_t   uint32_t;
      typedef u_int64_t   uint64_t;
    #endif
    #ifndef __intptr_t_defined
      typedef          int     intptr_t; 
      typedef unsigned int    uintptr_t; 
    #endif
   #endif
  #elif defined(SIZEOF_CHAR) && \
        defined(SIZEOF_SHORT) && \
        defined(SIZEOF_INT) && \
        defined(SIZEOF_LONG) && \
        defined(SIZEOF_LONG_LONG) && \
        defined(SIZEOF_VOID_P)
      /* configure-detected integer sizes are available, 
       * so use those to automatically detect the sizes 
       * system headers may typedef some subset of these, 
       * so we cannot safely use typedefs here
       * so use macros instead
       */
      /* first include the system headers if we know we have them, to try and avoid conflicts */
      #ifdef HAVE_INTTYPES_H
        #include <inttypes.h>
      #endif
      #ifdef HAVE_STDINT_H
        #include <stdint.h>
      #endif
      #ifdef HAVE_SYS_TYPES_H
        #include <sys/types.h>
      #endif

      #if SIZEOF_CHAR == 1
        typedef signed   char  _pit_int8_t;
        typedef unsigned char _pit_uint8_t;
      #else
        #error Cannot find an 8-bit type for your platform
      #endif

      #if SIZEOF_CHAR == 2
        typedef signed   char  _pit_int16_t;
        typedef unsigned char _pit_uint16_t;
      #elif SIZEOF_SHORT == 2
        typedef          short  _pit_int16_t;
        typedef unsigned short _pit_uint16_t;
      #elif SIZEOF_INT == 2
        typedef          int  _pit_int16_t;
        typedef unsigned int _pit_uint16_t;
      #else
        #error Cannot find a 16-bit type for your platform
      #endif

      #if SIZEOF_SHORT == 4
        typedef          short  _pit_int32_t;
        typedef unsigned short _pit_uint32_t;
      #elif SIZEOF_INT == 4
        typedef          int  _pit_int32_t;
        typedef unsigned int _pit_uint32_t;
      #elif SIZEOF_LONG == 4
        typedef          long  _pit_int32_t;
        typedef unsigned long _pit_uint32_t;
      #else
        #error Cannot find a 32-bit type for your platform
      #endif

      #if SIZEOF_INT == 8
        typedef          int  _pit_int64_t;
        typedef unsigned int _pit_uint64_t;
      #elif SIZEOF_LONG == 8
        typedef          long  _pit_int64_t;
        typedef unsigned long _pit_uint64_t;
      #elif SIZEOF_LONG_LONG == 8
        typedef          long long  _pit_int64_t;
        typedef unsigned long long _pit_uint64_t;
      #else
        #error Cannot find a 64-bit type for your platform
      #endif

      #if SIZEOF_VOID_P == SIZEOF_SHORT
        typedef          short  _pit_intptr_t;
        typedef unsigned short _pit_uintptr_t;
      #elif SIZEOF_VOID_P == SIZEOF_INT
        typedef          int  _pit_intptr_t;
        typedef unsigned int _pit_uintptr_t;
      #elif SIZEOF_VOID_P == SIZEOF_LONG
        typedef          long  _pit_intptr_t;
        typedef unsigned long _pit_uintptr_t;
      #elif SIZEOF_VOID_P == SIZEOF_LONG_LONG
        typedef          long long  _pit_intptr_t;
        typedef unsigned long long _pit_uintptr_t;
      #else
        #error Cannot find a integral pointer-sized type for your platform
      #endif  
      
      #define  int8_t    _pit_int8_t
      #define uint8_t   _pit_uint8_t
      #define  int16_t   _pit_int16_t
      #define uint16_t  _pit_uint16_t
      #define  int32_t   _pit_int32_t
      #define uint32_t  _pit_uint32_t
      #define  int64_t   _pit_int64_t
      #define uint64_t  _pit_uint64_t
      #define  intptr_t  _pit_intptr_t
      #define uintptr_t _pit_uintptr_t
  #else
    /* no information available, so try inttypes.h and hope for the best 
       if we die here, the correct fix is to detect the sizes using configure 
       (and include *config.h before this file).
     */
    #include <inttypes.h>
  #endif
#endif /* _INTTYPES_DEFINED */
#endif /* _PORTABLE_INTTYPES_H */

  /* Fixed-width printf/scanf format specifiers to go with the types defined above.
   * C99-compliant inttypes.h should already provide these, so here we just ensure
   * that's the case for the ones we care about and provide a last-ditch effort otherwise.
   * These are processed on every include of this header, to allow context-dependent adjustment.
   *
   * Currently only worry about the 64-bit and PTR versions.
   *
   * Also provide the following best-effort extensions, as replacements for C99 length modifiers t and z:
   *   PRI[diouxX]SZ  - size_t    (signed or unsigned)
   *   PRI[diouxX]PD  - ptrdiff_t (signed or unsigned)
   * these require at least ONE of: 
   *    1. GASNet configure results
   *    2. Advertised C99 support (via __STDC_VERSION__)
   *    3. Defines of __PRISZ_PREFIX and __PRIPD_PREFIX selecting the prefix to use
   *    4. Defines of HAVE_C99_FORMAT_SPECIFIERS to enable/disable use of C99 versions and 
   *       SIZEOF_{INT,LONG,LONG_LONG) defines when disabled.
   */
  #if HAVE_C99_FORMAT_SPECIFIERS || \
      (!defined(HAVE_C99_FORMAT_SPECIFIERS) && __STDC_VERSION__ >= 199901L)
    /* prefer libc-provided length modifiers added in C99 if we can determine they are available */
    #ifndef __PRISZ_PREFIX
    #define __PRISZ_PREFIX "z"
    #endif
    #ifndef __PRIPD_PREFIX
    #define __PRIPD_PREFIX "t"
    #endif
  #endif
  #if !defined(__PRISZ_PREFIX) && SIZEOF_SIZE_T /* use configure info when available */
    #if   SIZEOF_SIZE_T == SIZEOF_INT
       #define __PRISZ_PREFIX 
    #elif SIZEOF_SIZE_T == SIZEOF_LONG
       #define __PRISZ_PREFIX "l"
    #elif SIZEOF_SIZE_T == SIZEOF_LONG_LONG
       #define __PRISZ_PREFIX "ll"
    #endif
  #endif
  #if !defined(__PRIPD_PREFIX) && SIZEOF_PTRDIFF_T /* use configure info when available */
    #if   SIZEOF_PTRDIFF_T == SIZEOF_INT
       #define __PRIPD_PREFIX 
    #elif SIZEOF_PTRDIFF_T == SIZEOF_LONG
       #define __PRIPD_PREFIX "l"
    #elif SIZEOF_PTRDIFF_T == SIZEOF_LONG_LONG
       #define __PRIPD_PREFIX "ll"
    #endif
  #endif
  #if !defined(__PRI64_PREFIX) /* use configure info when available */
    #if SIZEOF_LONG == 8
       #define __PRI64_PREFIX  "l"
    #elif SIZEOF_INT == 8
       #define __PRI64_PREFIX 
    #elif SIZEOF_LONG_LONG == 8
       #define __PRI64_PREFIX  "ll"
    #endif
  #endif
  #if !defined(__PRIPTR_PREFIX) && SIZEOF_VOID_P /* use configure info when available */
    #if   SIZEOF_VOID_P == SIZEOF_LONG
       #define __PRIPTR_PREFIX  "l"
    #elif SIZEOF_VOID_P == SIZEOF_INT
       #define __PRIPTR_PREFIX 
    #elif SIZEOF_VOID_P == SIZEOF_LONG_LONG
       #define __PRIPTR_PREFIX  "ll"
    #endif
  #endif
  /* last resort */
  #if SIZEOF_VOID_P == 4 || PLATFORM_ARCH_32 || __INTPTR_MAX__ == 2147483647 /* assume ILP32 */
    #ifndef __PRI64_PREFIX
    #define __PRI64_PREFIX "ll"
    #endif
    #ifndef __PRIPTR_PREFIX
    #define __PRIPTR_PREFIX 
    #endif
  #else /* assume LP64 */
    #ifndef __PRI64_PREFIX
    #define __PRI64_PREFIX  "l"
    #endif
    #ifndef __PRIPTR_PREFIX
    #define __PRIPTR_PREFIX "l"
    #endif
  #endif

  #ifndef PRIi64
  #define PRIi64 __PRI64_PREFIX "i"
  #endif
  #ifndef PRId64
  #define PRId64 __PRI64_PREFIX "d"
  #endif
  #ifndef PRIu64
  #define PRIu64 __PRI64_PREFIX "u"
  #endif
  #ifndef PRIo64
  #define PRIo64 __PRI64_PREFIX "o"
  #endif
  #ifndef PRIx64
  #define PRIx64 __PRI64_PREFIX "x"
  #endif
  #ifndef PRIX64
  #define PRIX64 __PRI64_PREFIX "X"
  #endif

  #ifndef PRIiPTR
  #define PRIiPTR __PRIPTR_PREFIX "i"
  #endif
  #ifndef PRIdPTR
  #define PRIdPTR __PRIPTR_PREFIX "d"
  #endif
  #ifndef PRIuPTR
  #define PRIuPTR __PRIPTR_PREFIX "u"
  #endif
  #ifndef PRIoPTR
  #define PRIoPTR __PRIPTR_PREFIX "o"
  #endif
  #ifndef PRIxPTR
  #define PRIxPTR __PRIPTR_PREFIX "x"
  #endif
  #ifndef PRIXPTR
  #define PRIXPTR __PRIPTR_PREFIX "X"
  #endif

 #ifdef __PRISZ_PREFIX /* these are best-effort definitions - see above */
  #ifndef PRIiSZ
  #define PRIiSZ __PRISZ_PREFIX "i"
  #endif
  #ifndef PRIdSZ
  #define PRIdSZ __PRISZ_PREFIX "d"
  #endif
  #ifndef PRIuSZ
  #define PRIuSZ __PRISZ_PREFIX "u"
  #endif
  #ifndef PRIoSZ
  #define PRIoSZ __PRISZ_PREFIX "o"
  #endif
  #ifndef PRIxSZ
  #define PRIxSZ __PRISZ_PREFIX "x"
  #endif
  #ifndef PRIXSZ
  #define PRIXSZ __PRISZ_PREFIX "X"
  #endif
 #endif

 #ifdef __PRIPD_PREFIX /* these are best-effort definitions - see above */
  #ifndef PRIiPD
  #define PRIiPD __PRIPD_PREFIX "i"
  #endif
  #ifndef PRIdPD
  #define PRIdPD __PRIPD_PREFIX "d"
  #endif
  #ifndef PRIuPD
  #define PRIuPD __PRIPD_PREFIX "u"
  #endif
  #ifndef PRIoPD
  #define PRIoPD __PRIPD_PREFIX "o"
  #endif
  #ifndef PRIxPD
  #define PRIxPD __PRIPD_PREFIX "x"
  #endif
  #ifndef PRIXPD
  #define PRIXPD __PRIPD_PREFIX "X"
  #endif
 #endif

  #ifndef SCNi64
  #define SCNi64 __PRI64_PREFIX "i"
  #endif
  #ifndef SCNd64
  #define SCNd64 __PRI64_PREFIX "d"
  #endif
  #ifndef SCNu64
  #define SCNu64 __PRI64_PREFIX "u"
  #endif
  #ifndef SCNo64
  #define SCNo64 __PRI64_PREFIX "o"
  #endif
  #ifndef SCNx64
  #define SCNx64 __PRI64_PREFIX "x"
  #endif

  #ifndef SCNiPTR
  #define SCNiPTR __PRIPTR_PREFIX "i"
  #endif
  #ifndef SCNdPTR
  #define SCNdPTR __PRIPTR_PREFIX "d"
  #endif
  #ifndef SCNuPTR
  #define SCNuPTR __PRIPTR_PREFIX "u"
  #endif
  #ifndef SCNoPTR
  #define SCNoPTR __PRIPTR_PREFIX "o"
  #endif
  #ifndef SCNxPTR
  #define SCNxPTR __PRIPTR_PREFIX "x"
  #endif

