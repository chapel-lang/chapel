/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2009/10/28 00:09:23 $
 * $Revision: 1.46 $
 * Description: GASNet header for vapi conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

/* At least one VAPI MPI does '#define VAPI 1'.
 * This will clobber our GASNET_CORE_NAME.
 * Grumble, grumble.
 */
#ifdef VAPI
  #undef VAPI
#endif

#define GASNET_CORE_VERSION      1.11
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#if defined(GASNET_CONDUIT_VAPI)
  #define GASNET_CORE_NAME         VAPI
#elif defined(GASNET_CONDUIT_IBV)
  #define GASNET_CORE_NAME         IBV
#else
  #error "Exactly one of GASNET_CONDUIT_VAPI or GASNET_CONDUIT_IBV must be defined"
#endif
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)

/* 16K is the limit on the LID space, but we must allow more than 1 proc per node */
#define GASNET_MAXNODES	65535

/* Explicitly set some types because we depend on their sizes when encoding them */
#define _GASNET_NODE_T
typedef uint16_t gasnet_node_t;
#define _GASNET_HANDLER_T
typedef uint8_t gasnet_handler_t;

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   1
#endif

  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
/*
 * The VAPI conduit may have a network progress thread, even for GASNET_SEQ
 * XXX: no progress thread for IBV yet
 */
#if GASNET_CONDUIT_VAPI && GASNETC_VAPI_RCV_THREAD
  #define GASNETI_CONDUIT_THREADS 1
#endif
  
  /* define to 1 if your conduit may interrupt an application thread 
     (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
   */
/* #define GASNETC_USE_INTERRUPTS 1 */
  
  /* define these to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
#define GASNETC_GET_HANDLER 1 /* Need wider type to encode System category AMs */
typedef uint16_t gasnetc_handler_t;
/* #define GASNETC_TOKEN_CREATE 1 */

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)       \
        CNT(C, AMREQUEST_SYS, cnt)                \
        CNT(C, AMREPLY_SYS, cnt)                  \
        CNT(C, AMREQUEST_SYS_HANDLER, cnt)        \
        CNT(C, AMREPLY_SYS_HANDLER, cnt)          \
        CNT(C, SND_AM_SNDRCV, cnt)                \
        CNT(C, SND_AM_RDMA, cnt)                  \
        CNT(C, RCV_AM_SNDRCV, cnt)                \
        CNT(C, RCV_AM_RDMA, cnt)                  \
        VAL(C, RDMA_PUT_IN_MOVE, bytes)           \
        VAL(C, RDMA_PUT_INLINE, bytes)            \
        VAL(C, RDMA_PUT_BOUNCE, bytes)            \
        VAL(C, RDMA_PUT_ZEROCP, bytes)            \
        VAL(C, RDMA_GET_BOUNCE, bytes)            \
        VAL(C, RDMA_GET_ZEROCP, bytes)            \
        CNT(C, ALLOC_AM_SPARE, cnt)	          \
        CNT(C, GET_AMREQ_CREDIT, cnt)             \
	TIME(C, GET_AMREQ_CREDIT_STALL, stalled time) \
	TIME(C, GET_AMREQ_BUFFER_STALL, stalled time) \
	TIME(C, AM_ROUNDTRIP_TIME, time) \
	TIME(C, RCV_THREAD_WAKE, time awake)      \
	CNT(C, GET_BBUF, cnt)                     \
	TIME(C, GET_BBUF_STALL, stalled time)     \
	VAL(C, ALLOC_SREQ, sreqs)                 \
	VAL(C, POST_SR, segments)                 \
	CNT(C, POST_INLINE_SR, cnt)               \
	TIME(C, POST_SR_STALL_CQ, stalled time)   \
	TIME(C, POST_SR_STALL_SQ, stalled time)   \
	VAL(C, SND_POST_LIST, requests)           \
	VAL(C, POST_SR_LIST, requests)            \
	VAL(C, SND_REAP, reaped)                  \
	VAL(C, RCV_REAP, reaped)                  \
	TIME(C, FIREHOSE_MOVE, processing time)   \
	VAL(C, FIREHOSE_PIN, pages)               \
	VAL(C, FIREHOSE_UNPIN, pages)

#define GASNETC_FATALSIGNAL_CALLBACK(sig) gasnetc_fatalsignal_callback(sig)
extern void gasnetc_fatalsignal_callback(int sig);

#if PLATFORM_OS_DARWIN && !GASNET_SEQ
  #define GASNETC_PTHREAD_CREATE_OVERRIDE(create_fn, thread, attr, start_routine, arg) \
	gasnetc_pthread_create(create_fn, thread, attr, start_routine, arg)
#endif

#if PLATFORM_COMPILER_PGI && GASNET_CONDUIT_VAPI
  /* VAPI headers rely on the non-portable u_int*_t names
     PGI lacks these, so translate them to the versions guaranteed by the C99 spec and portable_inttypes
   */
 #ifndef u_int8_t
  #define u_int8_t  uint8_t
 #endif
 #ifndef u_int16_t
  #define u_int16_t uint16_t
 #endif
 #ifndef u_int32_t
  #define u_int32_t uint32_t
 #endif
 #if 0 && !defined(u_int64_t)
  #define u_int64_t uint64_t
 #endif
#endif

#endif
