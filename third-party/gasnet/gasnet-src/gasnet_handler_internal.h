/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_handler_internal.h $
 * Description: GASNet Helpers for using bit-width-independent AM handlers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


// NOTICE:
// While gasnet_handler.h was a "pseudo-public" header in GASNet-1, we are
// no longer providing the analog in GASNet-EX.  Any clients in need of the
// GASNet-1 gasnet_handler.h will find it in the other/contrib directory.

#ifndef _GASNET_HANDLER_INTERNAL_H
#define _GASNET_HANDLER_INTERNAL_H

/* argument-list macros */
#define ARGS0
#define ARGS1 , gex_AM_Arg_t a0
#define ARGS2 , gex_AM_Arg_t a0, gex_AM_Arg_t a1
#define ARGS3 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, gex_AM_Arg_t a2
#define ARGS4 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, gex_AM_Arg_t a2, gex_AM_Arg_t a3

#define ARGS5 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
                gex_AM_Arg_t a4
#define ARGS6 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
                gex_AM_Arg_t a4, gex_AM_Arg_t a5
#define ARGS7 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
                gex_AM_Arg_t a4, gex_AM_Arg_t a5, gex_AM_Arg_t a6
#define ARGS8 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
                gex_AM_Arg_t a4, gex_AM_Arg_t a5, gex_AM_Arg_t a6, gex_AM_Arg_t a7

#define ARGS9  , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8
#define ARGS10 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9
#define ARGS11 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9,  gex_AM_Arg_t a10
#define ARGS12 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9,  gex_AM_Arg_t a10, gex_AM_Arg_t a11


#define ARGS13 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9,  gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
                 gex_AM_Arg_t a12
#define ARGS14 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9,  gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
                 gex_AM_Arg_t a12, gex_AM_Arg_t a13
#define ARGS15 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9,  gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
                 gex_AM_Arg_t a12, gex_AM_Arg_t a13, gex_AM_Arg_t a14
#define ARGS16 , gex_AM_Arg_t a0,  gex_AM_Arg_t a1,  gex_AM_Arg_t a2,  gex_AM_Arg_t a3,  \
                 gex_AM_Arg_t a4,  gex_AM_Arg_t a5,  gex_AM_Arg_t a6,  gex_AM_Arg_t a7,  \
                 gex_AM_Arg_t a8,  gex_AM_Arg_t a9,  gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
                 gex_AM_Arg_t a12, gex_AM_Arg_t a13, gex_AM_Arg_t a14, gex_AM_Arg_t a15

#define ARGNAMES0
#define ARGNAMES1 , a0
#define ARGNAMES2 , a0, a1
#define ARGNAMES3 , a0, a1, a2
#define ARGNAMES4 , a0, a1, a2, a3

#define ARGNAMES5 , a0, a1, a2, a3, a4
#define ARGNAMES6 , a0, a1, a2, a3, a4, a5
#define ARGNAMES7 , a0, a1, a2, a3, a4, a5, a6
#define ARGNAMES8 , a0, a1, a2, a3, a4, a5, a6, a7

#define ARGNAMES9  , a0, a1, a2, a3, a4, a5, a6, a7, a8
#define ARGNAMES10 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9
#define ARGNAMES11 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10
#define ARGNAMES12 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11

#define ARGNAMES13 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12
#define ARGNAMES14 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13
#define ARGNAMES15 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14
#define ARGNAMES16 , a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15

/* 
  =====================================
    Pointer-width abstraction support
  =====================================
  AM arguments are always 32-bits wide, but PTR64 platforms need the ability to 
   send their 64-bit pointers as arguments. We do this by packing pointers into 
   2 arguments. The macros below support this packing/unpacking so you can write the
   calls and handlers once and generate the packing/unpacking code automatically
   as required for the platform.
*/

/* AM send call macros (use instead of direct calls to e.g. gex_AM_RequestShort2)
  Usage: cnt32 = the number of args needed for AM call on PTR32 platform
         cnt64 = the number of args needed for AM call on PTR64 platform
         args = parenthesized argument list for AM request/reply call,
           with any pointer args wrapped in a PACK() macro
 */
#if PLATFORM_ARCH_32
  #define SHORT_REQ(cnt32, cnt64, args) gex_AM_RequestShort ## cnt32 args
  #define SHORT_REP(cnt32, cnt64, args) gex_AM_ReplyShort ## cnt32 args
  #define MEDIUM_REQ(cnt32, cnt64, args) gex_AM_RequestMedium ## cnt32 args
  #define MEDIUM_REP(cnt32, cnt64, args) gex_AM_ReplyMedium ## cnt32 args
  #define LONG_REQ(cnt32, cnt64, args) gex_AM_RequestLong ## cnt32 args
  #define LONG_REP(cnt32, cnt64, args) gex_AM_ReplyLong ## cnt32 args
#elif PLATFORM_ARCH_64
  #define SHORT_REQ(cnt32, cnt64, args) gex_AM_RequestShort ## cnt64 args
  #define SHORT_REP(cnt32, cnt64, args) gex_AM_ReplyShort ## cnt64 args
  #define MEDIUM_REQ(cnt32, cnt64, args) gex_AM_RequestMedium ## cnt64 args
  #define MEDIUM_REP(cnt32, cnt64, args) gex_AM_ReplyMedium ## cnt64 args
  #define LONG_REQ(cnt32, cnt64, args) gex_AM_RequestLong ## cnt64 args
  #define LONG_REP(cnt32, cnt64, args) gex_AM_ReplyLong ## cnt64 args
#endif

/* pointer packing/unpacking helper macros */
#if PLATFORM_ARCH_32
  #define PACK(ptr) ((gex_AM_Arg_t)(ptr))
  #define UNPACK(a0) ((void *)a0)
#elif PLATFORM_ARCH_64
  #define PACK(ptr) ((gex_AM_Arg_t)GASNETI_HIWORD(ptr)), ((gex_AM_Arg_t)GASNETI_LOWORD(ptr))
  #define UNPACK2(a0,a1) ((void *)GASNETI_MAKEWORD(a0,a1))
#endif

#ifndef GASNETI_HANDLER_SCOPE
#define GASNETI_HANDLER_SCOPE extern
#endif

#if PLATFORM_ARCH_32
  #define SHORT_HANDLER_DECL(name, cnt32, cnt64) \
    GASNETI_HANDLER_SCOPE void name ## _32(gex_Token_t token ARGS ## cnt32)
  #define SHORT_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)            \
    GASNETI_HANDLER_SCOPE void name ## _32(gex_Token_t token ARGS ## cnt32) {    \
      name ## _inner innerargs32 ;                                               \
    } static int _dummy_##name = sizeof(_dummy_##name)
  #define MEDIUM_HANDLER_DECL(name, cnt32, cnt64)                                          \
    GASNETI_HANDLER_SCOPE void name ## _32(gex_Token_t token, void *addr, size_t nbytes    \
                           ARGS ## cnt32)
  #define MEDIUM_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)                     \
    GASNETI_HANDLER_SCOPE void name ## _32(gex_Token_t token, void *addr, size_t nbytes    \
                           ARGS ## cnt32) {                                                \
      name ## _inner innerargs32 ;                                                         \
    } static int _dummy_##name = sizeof(_dummy_##name)
#elif PLATFORM_ARCH_64
  #define SHORT_HANDLER_DECL(name, cnt32, cnt64) \
    GASNETI_HANDLER_SCOPE void name ## _64(gex_Token_t token ARGS ## cnt64)
  #define SHORT_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)            \
    GASNETI_HANDLER_SCOPE void name ## _64(gex_Token_t token ARGS ## cnt64) {    \
      name ## _inner innerargs64 ;                                               \
    } static int _dummy_##name = sizeof(_dummy_##name)
  #define MEDIUM_HANDLER_DECL(name, cnt32, cnt64)                                          \
    GASNETI_HANDLER_SCOPE void name ## _64(gex_Token_t token, void *addr, size_t nbytes    \
                           ARGS ## cnt64)
  #define MEDIUM_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)                     \
    GASNETI_HANDLER_SCOPE void name ## _64(gex_Token_t token, void *addr, size_t nbytes    \
                           ARGS ## cnt64) {                                                \
      name ## _inner innerargs64 ;                                                         \
    } static int _dummy_##name = sizeof(_dummy_##name)
#endif

/* convenience declarators for bit-width-independent handlers */
#define SHORT_HANDLER_NOBITS_DECL(name, cnt) \
  GASNETI_HANDLER_SCOPE void name(gex_Token_t token ARGS ## cnt)
#define MEDIUM_HANDLER_NOBITS_DECL(name, cnt) \
  GASNETI_HANDLER_SCOPE void name(gex_Token_t token, void *addr, size_t nbytes ARGS ## cnt)

/* long and medium handlers have the same signature */
#define LONG_HANDLER              MEDIUM_HANDLER
#define LONG_HANDLER_DECL         MEDIUM_HANDLER_DECL
#define LONG_HANDLER_NOBITS_DECL  MEDIUM_HANDLER_NOBITS_DECL

/*  handler table construction */
#define gasneti_handleridx(fnname) _hidx_ ## fnname
#define _gasneti_handler_tableentry_flags(required_flag1, required_flag2, optional_flags) \
    (GEX_FLAG_AM_##required_flag1 | GEX_FLAG_AM_##required_flag2 | optional_flags)

#define gasneti_handler_tableentry_no_bits(fnname, nargs, required_flag1, required_flag2, optional_flags) \
  { gasneti_handleridx(fnname), (gex_AM_Fn_t)(fnname), \
    _gasneti_handler_tableentry_flags(required_flag1, required_flag2, optional_flags), \
    (nargs), NULL, _STRINGIFY(fnname) }

#if PLATFORM_ARCH_32
  #define gasneti_handler_tableentry_with_bits(fnname, nargs32, nargs64, required_flag1, required_flag2, optional_flags) \
  { gasneti_handleridx(fnname), (gex_AM_Fn_t)(fnname ## _32), \
    _gasneti_handler_tableentry_flags(required_flag1, required_flag2, optional_flags), \
    (nargs32), NULL, _STRINGIFY(fnname) }
#elif PLATFORM_ARCH_64
  #define gasneti_handler_tableentry_with_bits(fnname, nargs32, nargs64, required_flag1, required_flag2, optional_flags) \
  { gasneti_handleridx(fnname), (gex_AM_Fn_t)(fnname ## _64), \
    _gasneti_handler_tableentry_flags(required_flag1, required_flag2, optional_flags), \
    (nargs64), NULL, _STRINGIFY(fnname) }
#endif

#define GASNETI_HANDLER_EOT {0,NULL,0,0,NULL,NULL}

#define GASNETI_HANDLER_NARGS_UNK 255

#endif
