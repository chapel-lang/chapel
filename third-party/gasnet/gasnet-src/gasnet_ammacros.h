/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_ammacros.h $
 * Description: GASNet ammacros header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_AMMACROS_H
#define _GASNET_AMMACROS_H

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request/Reply Functions
  ======================================
*/

/* This header uses macros to coalesce all the possible
   AM request/reply functions into the following short list of variable-argument
   functions. This is not the only implementation option, but seems to work 
   rather cleanly.
*/

// TODO-EX: introduce 'const' - has cascading effects!!!

// Long
extern int gasnetc_AMRequestLongM(
                gex_TM_t _tm,                 // Names a local context ("return address")
                gex_Rank_t _rank,             // Together with 'tm', names a remote context
                gex_AM_Index_t _handler,      // Index into handler table of remote context
                /*const*/ void *_source_addr, // Payload address (or OFFSET)
                size_t _nbytes,               // Payload length
                void *_dest_addr,             // Payload destination address (or OFFSET)
                gex_Event_t *_lc_opt,         // Local completion control (see above)
                gex_Flags_t _flags            // Flags to control this operation
                GASNETI_THREAD_FARG,          // Hidden thread-specific info argument
                int _numargs, ...);           // Argument list (0..AMMaxArgs) as varargs
extern int gasnetc_AMReplyLongM(
                gex_Token_t _token,           // Names local and remote contexts
                gex_AM_Index_t _handler,
                /*const*/ void *_source_addr,
                size_t _nbytes,
                void *_dest_addr,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags,
                int _numargs, ...);
// Medium
extern int gasnetc_AMRequestMediumM(
                gex_TM_t _tm,
                gex_Rank_t _rank,
                gex_AM_Index_t _handler,
                /*const*/ void *_source_addr,
                size_t _nbytes,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags
                GASNETI_THREAD_FARG,
                int _numargs, ...);
extern int gasnetc_AMReplyMediumM(
                gex_Token_t _token,
                gex_AM_Index_t _handler,
                /*const*/ void *_source_addr,
                size_t _nbytes,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags,
                int _numargs, ...);
// Short
extern int gasnetc_AMRequestShortM(
                gex_TM_t _tm,
                gex_Rank_t _rank,
                gex_AM_Index_t _handler,
                gex_Flags_t _flags
                GASNETI_THREAD_FARG,
                int _numargs, ...);
extern int gasnetc_AMReplyShortM(
                gex_Token_t _token,
                gex_AM_Index_t _handler,
                gex_Flags_t _flags,
                int _numargs, ...);

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Macros
  =====================
*/
/*  yes, this is ugly, but it works... */
/* ------------------------------------------------------------------------------------ */
#define gex_AM_RequestShort0(tm, rank, handler, flags) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 0)
#define gex_AM_RequestShort1(tm, rank, handler, flags, a0) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 1, (gex_AM_Arg_t)(a0))
#define gex_AM_RequestShort2(tm, rank, handler, flags, a0, a1) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 2, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_RequestShort3(tm, rank, handler, flags, a0, a1, a2) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 3, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_RequestShort4(tm, rank, handler, flags, a0, a1, a2, a3) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 4, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_RequestShort5(tm, rank, handler, flags, a0, a1, a2, a3, a4) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 5, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_RequestShort6(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 6, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_RequestShort7(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 7, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_RequestShort8(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 8, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_RequestShort9(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 9, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_RequestShort10(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 10, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_RequestShort11(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 11, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_RequestShort12(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 12, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_RequestShort13(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 13, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_RequestShort14(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 14, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_RequestShort15(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 15, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_RequestShort16(tm, rank, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AMRequestShortM(tm, rank, handler, flags GASNETI_THREAD_GET, 16, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */
#define gex_AM_RequestMedium0(tm, rank, handler, source_addr, nbytes, lc_opt, flags) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 0)
#define gex_AM_RequestMedium1(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 1, (gex_AM_Arg_t)(a0))
#define gex_AM_RequestMedium2(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 2, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_RequestMedium3(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 3, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_RequestMedium4(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 4, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_RequestMedium5(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 5, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_RequestMedium6(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 6, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_RequestMedium7(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 7, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_RequestMedium8(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 8, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_RequestMedium9(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 9, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_RequestMedium10(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 10, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_RequestMedium11(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 11, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_RequestMedium12(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 12, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_RequestMedium13(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 13, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_RequestMedium14(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 14, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_RequestMedium15(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 15, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_RequestMedium16(tm, rank, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AMRequestMediumM(tm, rank, handler, source_addr, nbytes, lc_opt, flags GASNETI_THREAD_GET, 16, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */
#define gex_AM_RequestLong0(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 0)
#define gex_AM_RequestLong1(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 1, (gex_AM_Arg_t)(a0))
#define gex_AM_RequestLong2(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 2, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_RequestLong3(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 3, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_RequestLong4(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 4, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_RequestLong5(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 5, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_RequestLong6(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 6, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_RequestLong7(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 7, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_RequestLong8(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 8, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_RequestLong9(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 9, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_RequestLong10(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 10, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_RequestLong11(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 11, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_RequestLong12(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 12, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_RequestLong13(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 13, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_RequestLong14(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 14, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_RequestLong15(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 15, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_RequestLong16(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AMRequestLongM(tm, rank, handler, source_addr, nbytes, dest_addr, lc_opt, flags GASNETI_THREAD_GET, 16, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */
#define gex_AM_ReplyShort0(token, handler, flags) \
       gasnetc_AMReplyShortM(token, handler, flags, 0)
#define gex_AM_ReplyShort1(token, handler, flags, a0) \
       gasnetc_AMReplyShortM(token, handler, flags, 1, (gex_AM_Arg_t)(a0))
#define gex_AM_ReplyShort2(token, handler, flags, a0, a1) \
       gasnetc_AMReplyShortM(token, handler, flags, 2, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_ReplyShort3(token, handler, flags, a0, a1, a2) \
       gasnetc_AMReplyShortM(token, handler, flags, 3, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_ReplyShort4(token, handler, flags, a0, a1, a2, a3) \
       gasnetc_AMReplyShortM(token, handler, flags, 4, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_ReplyShort5(token, handler, flags, a0, a1, a2, a3, a4) \
       gasnetc_AMReplyShortM(token, handler, flags, 5, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_ReplyShort6(token, handler, flags, a0, a1, a2, a3, a4, a5) \
       gasnetc_AMReplyShortM(token, handler, flags, 6, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_ReplyShort7(token, handler, flags, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AMReplyShortM(token, handler, flags, 7, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_ReplyShort8(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AMReplyShortM(token, handler, flags, 8, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_ReplyShort9(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AMReplyShortM(token, handler, flags, 9, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_ReplyShort10(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AMReplyShortM(token, handler, flags, 10, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_ReplyShort11(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AMReplyShortM(token, handler, flags, 11, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_ReplyShort12(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AMReplyShortM(token, handler, flags, 12, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_ReplyShort13(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AMReplyShortM(token, handler, flags, 13, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_ReplyShort14(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AMReplyShortM(token, handler, flags, 14, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_ReplyShort15(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AMReplyShortM(token, handler, flags, 15, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_ReplyShort16(token, handler, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AMReplyShortM(token, handler, flags, 16, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */
#define gex_AM_ReplyMedium0(token, handler, source_addr, nbytes, lc_opt, flags) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 0)
#define gex_AM_ReplyMedium1(token, handler, source_addr, nbytes, lc_opt, flags, a0) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 1, (gex_AM_Arg_t)(a0))
#define gex_AM_ReplyMedium2(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 2, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_ReplyMedium3(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 3, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_ReplyMedium4(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 4, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_ReplyMedium5(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 5, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_ReplyMedium6(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 6, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_ReplyMedium7(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 7, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_ReplyMedium8(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 8, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_ReplyMedium9(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags,  9, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_ReplyMedium10(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 10, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_ReplyMedium11(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 11, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_ReplyMedium12(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 12, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_ReplyMedium13(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 13, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_ReplyMedium14(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 14, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_ReplyMedium15(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 15, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_ReplyMedium16(token, handler, source_addr, nbytes, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AMReplyMediumM(token, handler, source_addr, nbytes, lc_opt, flags, 16, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */
#define gex_AM_ReplyLong0(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 0)
#define gex_AM_ReplyLong1(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 1, (gex_AM_Arg_t)(a0))
#define gex_AM_ReplyLong2(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 2, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_ReplyLong3(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 3, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_ReplyLong4(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 4, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_ReplyLong5(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 5, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_ReplyLong6(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 6, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_ReplyLong7(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 7, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_ReplyLong8(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 8, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_ReplyLong9(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags,  9, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_ReplyLong10(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 10, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_ReplyLong11(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 11, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_ReplyLong12(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 12, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_ReplyLong13(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 13, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_ReplyLong14(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 14, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_ReplyLong15(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 15, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_ReplyLong16(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AMReplyLongM(token, handler, source_addr, nbytes, dest_addr, lc_opt, flags, 16, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */

/*
  Active Message Prepare/Commit Functions
  =======================================
*/

#if GASNET_DEBUG
  #undef GASNETC_AM_COMMIT_REQ_MEDIUM_NARGS
  #define GASNETC_AM_COMMIT_REQ_MEDIUM_NARGS 1
  #undef GASNETC_AM_COMMIT_REP_MEDIUM_NARGS
  #define GASNETC_AM_COMMIT_REP_MEDIUM_NARGS 1
  #undef GASNETC_AM_COMMIT_REQ_LONG_NARGS
  #define GASNETC_AM_COMMIT_REQ_LONG_NARGS 1
  #undef GASNETC_AM_COMMIT_REP_LONG_NARGS
  #define GASNETC_AM_COMMIT_REP_LONG_NARGS 1
#endif

// Medium split-phase
extern gex_AM_SrcDesc_t gasnetc_AM_PrepareRequestMedium(
                gex_TM_t _tm,
                gex_Rank_t _rank,
                const void *_client_buf,
                size_t _min_length,
                size_t _max_length,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags
                GASNETI_THREAD_FARG,
                unsigned int _numargs);
extern void gasnetc_AM_CommitRequestMediumM(
                gex_AM_Index_t _handler,
                size_t _nbytes
                GASNETI_THREAD_FARG,
              #if GASNETC_AM_COMMIT_REQ_MEDIUM_NARGS
                unsigned int _numargs,
              #endif
                gex_AM_SrcDesc_t _sd, ...);
extern gex_AM_SrcDesc_t gasnetc_AM_PrepareReplyMedium(
                gex_Token_t _token,
                const void *_client_buf,
                size_t _min_length,
                size_t _max_length,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags,
                unsigned int _numargs);
extern void gasnetc_AM_CommitReplyMediumM(
                gex_AM_Index_t _handler,
                size_t _nbytes,
              #if GASNETC_AM_COMMIT_REP_MEDIUM_NARGS
                unsigned int _numargs,
              #endif
                gex_AM_SrcDesc_t _sd, ...);

// Long split-phase
extern gex_AM_SrcDesc_t gasnetc_AM_PrepareRequestLong(
                gex_TM_t _tm,
                gex_Rank_t _rank,
                const void *_client_buf,
                size_t _min_length,
                size_t _max_length,
                void *_dest_addr,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags
                GASNETI_THREAD_FARG,
                unsigned int _numargs);
extern void gasnetc_AM_CommitRequestLongM(
                gex_AM_Index_t _handler,
                size_t _nbytes,
                void *_dest_addr
                GASNETI_THREAD_FARG,
              #if GASNETC_AM_COMMIT_REQ_LONG_NARGS
                unsigned int _numargs,
              #endif
                gex_AM_SrcDesc_t _sd, ...);
extern gex_AM_SrcDesc_t gasnetc_AM_PrepareReplyLong(
                gex_Token_t _token,
                const void *_client_buf,
                size_t _min_length,
                size_t _max_length,
                void *_dest_addr,
                gex_Event_t *_lc_opt,
                gex_Flags_t _flags,
                unsigned int _numargs);
extern void gasnetc_AM_CommitReplyLongM(
                gex_AM_Index_t _handler,
                size_t _nbytes,
                void *_dest_addr,
              #if GASNETC_AM_COMMIT_REP_LONG_NARGS
                unsigned int _numargs,
              #endif
                gex_AM_SrcDesc_t _sd, ...);

#define gex_AM_PrepareRequestMedium(tm, rank, cbuf, minlen, maxlen, lc_opt, flags, nargs) \
    gasnetc_AM_PrepareRequestMedium(tm, rank, cbuf, minlen, maxlen, lc_opt, flags GASNETI_THREAD_GET, nargs)

#define gex_AM_PrepareReplyMedium(token, cbuf, minlen, maxlen, lc_opt, flags, nargs) \
    gasnetc_AM_PrepareReplyMedium(token, cbuf, minlen, maxlen, lc_opt, flags, nargs)

#define gex_AM_PrepareRequestLong(tm, rank, cbuf, minlen, maxlen, dest_addr, lc_opt, flags, nargs) \
    gasnetc_AM_PrepareRequestLong(tm, rank, cbuf, minlen, maxlen, dest_addr, lc_opt, flags GASNETI_THREAD_GET, nargs)

#define gex_AM_PrepareReplyLong(token, cbuf, minlen, maxlen, dest_addr, lc_opt, flags, nargs) \
    gasnetc_AM_PrepareReplyLong(token, cbuf, minlen, maxlen, dest_addr, lc_opt, flags, nargs)

#if GASNETC_AM_COMMIT_REQ_MEDIUM_NARGS
  #define GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(x) , x
#else
  #define GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(x)
#endif
#if GASNETC_AM_COMMIT_REP_MEDIUM_NARGS
  #define GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(x) , x
#else
  #define GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(x)
#endif
#if GASNETC_AM_COMMIT_REQ_LONG_NARGS
  #define GASNETI_AM_COMMIT_REQ_LONG_NARGS(x) , x
#else
  #define GASNETI_AM_COMMIT_REQ_LONG_NARGS(x)
#endif
#if GASNETC_AM_COMMIT_REP_LONG_NARGS
  #define GASNETI_AM_COMMIT_REP_LONG_NARGS(x) , x
#else
  #define GASNETI_AM_COMMIT_REP_LONG_NARGS(x)
#endif

#define gex_AM_CommitRequestMedium0(sd, handler, nbytes) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(0), sd)
#define gex_AM_CommitRequestMedium1(sd, handler, nbytes, a0) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(1), sd, (gex_AM_Arg_t)(a0))
#define gex_AM_CommitRequestMedium2(sd, handler, nbytes, a0, a1) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(2), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_CommitRequestMedium3(sd, handler, nbytes, a0, a1, a2) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(3), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_CommitRequestMedium4(sd, handler, nbytes, a0, a1, a2, a3) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(4), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_CommitRequestMedium5(sd, handler, nbytes, a0, a1, a2, a3, a4) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(5), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_CommitRequestMedium6(sd, handler, nbytes, a0, a1, a2, a3, a4, a5) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(6), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_CommitRequestMedium7(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(7), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_CommitRequestMedium8(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(8), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_CommitRequestMedium9(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(9), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_CommitRequestMedium10(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(10), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_CommitRequestMedium11(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(11), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_CommitRequestMedium12(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(12), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_CommitRequestMedium13(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(13), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_CommitRequestMedium14(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(14), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_CommitRequestMedium15(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(15), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_CommitRequestMedium16(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AM_CommitRequestMediumM(handler, nbytes GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_MEDIUM_NARGS(16), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))

#define gex_AM_CommitReplyMedium0(sd, handler, nbytes) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(0), sd)
#define gex_AM_CommitReplyMedium1(sd, handler, nbytes, a0) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(1), sd, (gex_AM_Arg_t)(a0))
#define gex_AM_CommitReplyMedium2(sd, handler, nbytes, a0, a1) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(2), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_CommitReplyMedium3(sd, handler, nbytes, a0, a1, a2) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(3), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_CommitReplyMedium4(sd, handler, nbytes, a0, a1, a2, a3) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(4), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_CommitReplyMedium5(sd, handler, nbytes, a0, a1, a2, a3, a4) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(5), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_CommitReplyMedium6(sd, handler, nbytes, a0, a1, a2, a3, a4, a5) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(6), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_CommitReplyMedium7(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(7), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_CommitReplyMedium8(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(8), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_CommitReplyMedium9(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(9), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_CommitReplyMedium10(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(10), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_CommitReplyMedium11(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(11), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_CommitReplyMedium12(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(12), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_CommitReplyMedium13(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(13), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_CommitReplyMedium14(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(14), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_CommitReplyMedium15(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(15), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_CommitReplyMedium16(sd, handler, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AM_CommitReplyMediumM(handler, nbytes GASNETI_AM_COMMIT_REP_MEDIUM_NARGS(16), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))
/* ------------------------------------------------------------------------------------ */
#define gex_AM_CommitRequestLong0(sd, handler, nbytes, dest_addr) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(0), sd)
#define gex_AM_CommitRequestLong1(sd, handler, nbytes, dest_addr, a0) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(1), sd, (gex_AM_Arg_t)(a0))
#define gex_AM_CommitRequestLong2(sd, handler, nbytes, dest_addr, a0, a1) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(2), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_CommitRequestLong3(sd, handler, nbytes, dest_addr, a0, a1, a2) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(3), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_CommitRequestLong4(sd, handler, nbytes, dest_addr, a0, a1, a2, a3) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(4), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_CommitRequestLong5(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(5), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_CommitRequestLong6(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(6), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_CommitRequestLong7(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(7), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_CommitRequestLong8(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(8), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_CommitRequestLong9(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(9), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_CommitRequestLong10(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(10), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_CommitRequestLong11(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(11), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_CommitRequestLong12(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(12), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_CommitRequestLong13(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(13), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_CommitRequestLong14(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(14), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_CommitRequestLong15(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(15), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_CommitRequestLong16(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AM_CommitRequestLongM(handler, nbytes, dest_addr GASNETI_THREAD_GET GASNETI_AM_COMMIT_REQ_LONG_NARGS(16), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))

#define gex_AM_CommitReplyLong0(sd, handler, nbytes, dest_addr) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(0), sd)
#define gex_AM_CommitReplyLong1(sd, handler, nbytes, dest_addr, a0) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(1), sd, (gex_AM_Arg_t)(a0))
#define gex_AM_CommitReplyLong2(sd, handler, nbytes, dest_addr, a0, a1) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(2), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1))
#define gex_AM_CommitReplyLong3(sd, handler, nbytes, dest_addr, a0, a1, a2) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(3), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2))
#define gex_AM_CommitReplyLong4(sd, handler, nbytes, dest_addr, a0, a1, a2, a3) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(4), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3))

#define gex_AM_CommitReplyLong5(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(5), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4))
#define gex_AM_CommitReplyLong6(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(6), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5))
#define gex_AM_CommitReplyLong7(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(7), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6))
#define gex_AM_CommitReplyLong8(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7) \
       gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(8), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7))

#define gex_AM_CommitReplyLong9(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(9), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8))
#define gex_AM_CommitReplyLong10(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(10), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9))
#define gex_AM_CommitReplyLong11(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(11), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10))
#define gex_AM_CommitReplyLong12(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(12), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11))

#define gex_AM_CommitReplyLong13(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(13), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12))
#define gex_AM_CommitReplyLong14(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(14), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13))
#define gex_AM_CommitReplyLong15(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(15), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14))
#define gex_AM_CommitReplyLong16(sd, handler, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gasnetc_AM_CommitReplyLongM(handler, nbytes, dest_addr GASNETI_AM_COMMIT_REP_LONG_NARGS(16), sd, (gex_AM_Arg_t)(a0), (gex_AM_Arg_t)(a1), (gex_AM_Arg_t)(a2), (gex_AM_Arg_t)(a3), (gex_AM_Arg_t)(a4), (gex_AM_Arg_t)(a5), (gex_AM_Arg_t)(a6), (gex_AM_Arg_t)(a7), (gex_AM_Arg_t)(a8), (gex_AM_Arg_t)(a9), (gex_AM_Arg_t)(a10), (gex_AM_Arg_t)(a11), (gex_AM_Arg_t)(a12), (gex_AM_Arg_t)(a13), (gex_AM_Arg_t)(a14), (gex_AM_Arg_t)(a15))

/* ------------------------------------------------------------------------------------ */
#endif
