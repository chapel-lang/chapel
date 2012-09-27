/*   $Source: /var/local/cvs/gasnet/gasnet_handler.h,v $
 *     $Date: 2009/03/29 04:03:01 $
 * $Revision: 1.10 $
 * Description: GASNet Helpers for using bit-width-independent AM handlers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_HANDLER_H
#define _GASNET_HANDLER_H

/* argument-list macros */
#define ARGS0
#define ARGS1 , gasnet_handlerarg_t a0
#define ARGS2 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1
#define ARGS3 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1, gasnet_handlerarg_t a2
#define ARGS4 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1, gasnet_handlerarg_t a2, gasnet_handlerarg_t a3

#define ARGS5 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1, gasnet_handlerarg_t a2, gasnet_handlerarg_t a3, \
                gasnet_handlerarg_t a4
#define ARGS6 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1, gasnet_handlerarg_t a2, gasnet_handlerarg_t a3, \
                gasnet_handlerarg_t a4, gasnet_handlerarg_t a5
#define ARGS7 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1, gasnet_handlerarg_t a2, gasnet_handlerarg_t a3, \
                gasnet_handlerarg_t a4, gasnet_handlerarg_t a5, gasnet_handlerarg_t a6
#define ARGS8 , gasnet_handlerarg_t a0, gasnet_handlerarg_t a1, gasnet_handlerarg_t a2, gasnet_handlerarg_t a3, \
                gasnet_handlerarg_t a4, gasnet_handlerarg_t a5, gasnet_handlerarg_t a6, gasnet_handlerarg_t a7

#define ARGS9  , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8
#define ARGS10 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9
#define ARGS11 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9,  gasnet_handlerarg_t a10
#define ARGS12 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9,  gasnet_handlerarg_t a10, gasnet_handlerarg_t a11


#define ARGS13 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9,  gasnet_handlerarg_t a10, gasnet_handlerarg_t a11, \
                 gasnet_handlerarg_t a12
#define ARGS14 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9,  gasnet_handlerarg_t a10, gasnet_handlerarg_t a11, \
                 gasnet_handlerarg_t a12, gasnet_handlerarg_t a13
#define ARGS15 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9,  gasnet_handlerarg_t a10, gasnet_handlerarg_t a11, \
                 gasnet_handlerarg_t a12, gasnet_handlerarg_t a13, gasnet_handlerarg_t a14
#define ARGS16 , gasnet_handlerarg_t a0,  gasnet_handlerarg_t a1,  gasnet_handlerarg_t a2,  gasnet_handlerarg_t a3,  \
                 gasnet_handlerarg_t a4,  gasnet_handlerarg_t a5,  gasnet_handlerarg_t a6,  gasnet_handlerarg_t a7,  \
                 gasnet_handlerarg_t a8,  gasnet_handlerarg_t a9,  gasnet_handlerarg_t a10, gasnet_handlerarg_t a11, \
                 gasnet_handlerarg_t a12, gasnet_handlerarg_t a13, gasnet_handlerarg_t a14, gasnet_handlerarg_t a15

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

/* AM send call macros (use instead of direct calls to e.g. gasnet_AMRequestShort2)
  Usage: cnt32 = the number of args needed for AM call on PTR32 platform
         cnt64 = the number of args needed for AM call on PTR64 platform
         args = parenthesized argument list for AM request/reply call, 
           with any pointer args wrapped in a PACK() macro
 */
#if PLATFORM_ARCH_32
  #define SHORT_REQ(cnt32, cnt64, args) gasnet_AMRequestShort ## cnt32 args
  #define SHORT_REP(cnt32, cnt64, args) gasnet_AMReplyShort ## cnt32 args
  #define MEDIUM_REQ(cnt32, cnt64, args) gasnet_AMRequestMedium ## cnt32 args
  #define MEDIUM_REP(cnt32, cnt64, args) gasnet_AMReplyMedium ## cnt32 args
  #define LONG_REQ(cnt32, cnt64, args) gasnet_AMRequestLong ## cnt32 args
  #define LONG_REP(cnt32, cnt64, args) gasnet_AMReplyLong ## cnt32 args
  #define LONGASYNC_REQ(cnt32, cnt64, args) gasnet_AMRequestLongAsync ## cnt32 args
#elif PLATFORM_ARCH_64
  #define SHORT_REQ(cnt32, cnt64, args) gasnet_AMRequestShort ## cnt64 args
  #define SHORT_REP(cnt32, cnt64, args) gasnet_AMReplyShort ## cnt64 args
  #define MEDIUM_REQ(cnt32, cnt64, args) gasnet_AMRequestMedium ## cnt64 args
  #define MEDIUM_REP(cnt32, cnt64, args) gasnet_AMReplyMedium ## cnt64 args
  #define LONG_REQ(cnt32, cnt64, args) gasnet_AMRequestLong ## cnt64 args
  #define LONG_REP(cnt32, cnt64, args) gasnet_AMReplyLong ## cnt64 args
  #define LONGASYNC_REQ(cnt32, cnt64, args) gasnet_AMRequestLongAsync ## cnt64 args
#endif

/* pointer packing/unpacking helper macros */
#if PLATFORM_ARCH_32
  #define PACK(ptr) ((gasnet_handlerarg_t)ptr)
  #define UNPACK(a0) ((void *)a0)
#elif PLATFORM_ARCH_64
  #define PACK(ptr) ((gasnet_handlerarg_t)GASNETI_HIWORD(ptr)), ((gasnet_handlerarg_t)GASNETI_LOWORD(ptr))
  #define UNPACK2(a0,a1) ((void *)GASNETI_MAKEWORD(a0,a1))
#endif

#ifndef GASNETI_HANDLER_SCOPE
#define GASNETI_HANDLER_SCOPE extern
#endif

#if PLATFORM_ARCH_32
  #define SHORT_HANDLER_DECL(name, cnt32, cnt64) \
    GASNETI_HANDLER_SCOPE void name ## _32(gasnet_token_t token ARGS ## cnt32)
  #define SHORT_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)            \
    GASNETI_HANDLER_SCOPE void name ## _32(gasnet_token_t token ARGS ## cnt32) { \
      name ## _inner innerargs32 ;                                               \
    } static int _dummy_##name = sizeof(_dummy_##name)
  #define MEDIUM_HANDLER_DECL(name, cnt32, cnt64)                                          \
    GASNETI_HANDLER_SCOPE void name ## _32(gasnet_token_t token, void *addr, size_t nbytes \
                           ARGS ## cnt32)
  #define MEDIUM_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)                     \
    GASNETI_HANDLER_SCOPE void name ## _32(gasnet_token_t token, void *addr, size_t nbytes \
                           ARGS ## cnt32) {                                                \
      name ## _inner innerargs32 ;                                                         \
    } static int _dummy_##name = sizeof(_dummy_##name)
#elif PLATFORM_ARCH_64
  #define SHORT_HANDLER_DECL(name, cnt32, cnt64) \
    GASNETI_HANDLER_SCOPE void name ## _64(gasnet_token_t token ARGS ## cnt64)
  #define SHORT_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)            \
    GASNETI_HANDLER_SCOPE void name ## _64(gasnet_token_t token ARGS ## cnt64) { \
      name ## _inner innerargs64 ;                                               \
    } static int _dummy_##name = sizeof(_dummy_##name)
  #define MEDIUM_HANDLER_DECL(name, cnt32, cnt64)                                          \
    GASNETI_HANDLER_SCOPE void name ## _64(gasnet_token_t token, void *addr, size_t nbytes \
                           ARGS ## cnt64)
  #define MEDIUM_HANDLER(name, cnt32, cnt64, innerargs32, innerargs64)                     \
    GASNETI_HANDLER_SCOPE void name ## _64(gasnet_token_t token, void *addr, size_t nbytes \
                           ARGS ## cnt64) {                                                \
      name ## _inner innerargs64 ;                                                         \
    } static int _dummy_##name = sizeof(_dummy_##name)
#endif

/* convenience declarators for bit-width-independent handlers */
#define SHORT_HANDLER_NOBITS_DECL(name, cnt) \
  GASNETI_HANDLER_SCOPE void name(gasnet_token_t token ARGS ## cnt)
#define MEDIUM_HANDLER_NOBITS_DECL(name, cnt) \
  GASNETI_HANDLER_SCOPE void name(gasnet_token_t token, void *addr, size_t nbytes ARGS ## cnt)

/* long and medium handlers have the same signature */
#define LONG_HANDLER              MEDIUM_HANDLER
#define LONG_HANDLER_DECL         MEDIUM_HANDLER_DECL
#define LONG_HANDLER_NOBITS_DECL  MEDIUM_HANDLER_NOBITS_DECL

#if GASNET_USE_STRICT_PROTOTYPES
typedef void *gasneti_handler_fn_t;
#else
typedef void (*gasneti_handler_fn_t)();  /* prototype for generic handler function */
#endif

/*  handler table construction */
#define gasneti_handleridx(fnname) _hidx_ ## fnname
#define gasneti_handler_tableentry_no_bits(fnname) \
    { gasneti_handleridx(fnname), (gasneti_handler_fn_t)fnname }

#if PLATFORM_ARCH_32
  #define gasneti_handler_tableentry_with_bits(fnname) \
    { gasneti_handleridx(fnname), (gasneti_handler_fn_t)fnname ## _32 }
#elif PLATFORM_ARCH_64
  #define gasneti_handler_tableentry_with_bits(fnname) \
    { gasneti_handleridx(fnname), (gasneti_handler_fn_t)fnname ## _64 }
#endif


#endif
