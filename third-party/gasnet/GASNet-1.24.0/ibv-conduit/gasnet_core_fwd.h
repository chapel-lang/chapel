/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core_fwd.h $
 * Description: GASNet header for ibv conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#ifdef GASNET_CONDUIT_VAPI
  #error "VAPI-conduit is no longer supported"
#endif

#define GASNET_CORE_VERSION      1.15
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         IBV
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_IBV       1

/* 16K is the limit on the LID space, but we must allow more than 1 proc per node */
/* 64K corresponds to 16 bits used in the AM Header and 16-bit gasnet_node_t */
#define GASNET_MAXNODES	65535

/* Explicitly set some types because we depend on their sizes when encoding them */
#define _GASNET_NODE_T
typedef uint16_t gasnet_node_t;
#define _GASNET_HANDLER_T
typedef uint8_t gasnet_handler_t;

  /* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
  #define GASNET_PSHM 1
#endif

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   1
#endif

  /* define to 1 if conduit allows internal GASNet fns to issue put/get for remote
     addrs out of segment - not true when PSHM is used */
#if 0
#define GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET 1
#endif

  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
#ifndef GASNETC_DYNAMIC_CONNECT
  #define GASNETC_DYNAMIC_CONNECT 1
#endif
#if GASNETC_IBV_RCV_THREAD || (GASNETC_DYNAMIC_CONNECT && GASNETC_IBV_CONN_THREAD)
  #define GASNETI_CONDUIT_THREADS 1
#endif

  /* define to 1 if your conduit may interrupt an application thread 
     (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
   */
/* #define GASNETC_USE_INTERRUPTS 1 */
  
  /* define these to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
/* #define GASNETC_GET_HANDLER 1 */
/* #define GASNETC_TOKEN_CREATE 1 */

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)       \
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
	CNT(C, GET_BBUF, cnt)                     \
	TIME(C, GET_BBUF_STALL, stalled time)     \
	VAL(C, ALLOC_SREQ, sreqs)                 \
	VAL(C, POST_SR, segments)                 \
	CNT(C, POST_INLINE_SR, cnt)               \
	TIME(C, POST_SR_STALL_CQ, stalled time)   \
	TIME(C, POST_SR_STALL_SQ, stalled time)   \
	VAL(C, POST_SR_LIST, requests)            \
	VAL(C, SND_REAP, reaped)                  \
	VAL(C, RCV_REAP, reaped)                  \
	CNT(C, CONN_STATIC, peers)                \
	CNT(C, CONN_DYNAMIC, peers)               \
	TIME(C, CONN_TIME_ACTV, active connect time) \
	TIME(C, CONN_TIME_PASV, passive connect time) \
	TIME(C, CONN_TIME_A2P, active-became-passive connect time) \
	TIME(C, CONN_REQ2REP, REQ-to-REP delay)   \
	TIME(C, CONN_RTU2ACK, RTU-to-ACK delay)   \
	VAL(C, CONN_REQ, resends)                 \
	VAL(C, CONN_RTU, resends)                 \
	CNT(C, CONN_REP, sent)                    \
	CNT(C, CONN_NOREP, not sent)              \
	CNT(C, CONN_ACK, sent)                    \
	CNT(C, CONN_NOACK, not sent)              \
	CNT(C, CONN_AAA, remained Active)         \
	CNT(C, CONN_AAP, became Passive)          \
	CNT(C, CONN_IMPLIED_ACK, cnt)             \
	TIME(C, CONN_STALL_CQ, stalled time)      \
	TIME(C, CONN_STALL_DESC, stalled time)    \
	TIME(C, FIREHOSE_MOVE, processing time)   \
	VAL(C, FIREHOSE_PIN, pages)               \
	VAL(C, FIREHOSE_UNPIN, pages)

#define GASNETC_FATALSIGNAL_CALLBACK(sig) gasnetc_fatalsignal_callback(sig)
	extern void gasnetc_fatalsignal_callback(int sig);

#if PLATFORM_OS_DARWIN && !GASNET_SEQ
  #define GASNETC_PTHREAD_CREATE_OVERRIDE(create_fn, thread, attr, start_routine, arg) \
	gasnetc_pthread_create(create_fn, thread, attr, start_routine, arg)
#endif

#endif
