/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2010/05/18 02:54:15 $
 * $Revision: 1.20.2.1 $
 * Description: GASNet header for PORTALS conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.4
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         PORTALS
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_PORTALS 1

#ifndef GASNET_MAXNODES
  #define GASNET_MAXNODES (0xFFFFu)
#endif

#if GASNET_MAXNODES <= 0xFFFFu
  #define _GASNET_NODE_T
  typedef uint16_t        gasnet_node_t;
#else
  #define _GASNET_NODE_T
  typedef uint32_t        gasnet_node_t;
#endif

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS
  #define GASNET_ALIGNED_SEGMENTS   0 /* user disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   0
#endif

  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
#if 0
#define GASNETI_CONDUIT_THREADS 1
#endif

  /* define to 1 if your conduit may interrupt an application thread 
     (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
   */
#if 0
#define GASNETC_USE_INTERRUPTS 1
#endif

  /* define these to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
/* #define GASNETC_GET_HANDLER 1 */
/* #define GASNETC_TOKEN_CREATE 1 */

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#if PLATFORM_OS_CATAMOUNT
  #define GASNETC_FIREHOSE_STATS(CNT,VAL,TIME) /*empty*/
#else
  #define GASNETC_FIREHOSE_STATS(CNT,VAL,TIME)  \
        CNT(C, FH_OP_ALLOC_BUF, count)          \
        CNT(C, FH_OP_ALLOC, count)              \
        CNT(C, FH_OP_FREE, count)               \
        CNT(C, GET_FH, count)                   \
        CNT(C, PUT_FH, count)                   \
        CNT(C, LONG_FH, count)                  \
        TIME(C, FIREHOSE_MOVE, processing time) \
        VAL(C, FIREHOSE_PIN, pages)             \
        VAL(C, FIREHOSE_UNPIN, pages)
#endif
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)     \
        CNT(C, CHUNK_ALLOC, count)              \
        CNT(C, CHUNK_FREE, count)               \
        CNT(C, TMPMD_ALLOC, count)              \
        CNT(C, TMPMD_FREE, count)               \
        CNT(C, MSG_THROTTLE, count)             \
        CNT(C, CREDIT_THROTTLE, count)		\
        CNT(C, TMPMD_THROTTLE, count)		\
        TIME(C, CREDIT_STALL, stalled time)	\
        CNT(C, SYSQ_DROPPED, count)		\
        CNT(C, END_EPOCH, count)		\
	CNT(C, GET_RAR, count)                  \
	CNT(C, GET_BB, count)                   \
	CNT(C, GET_TMPMD, count)                \
	CNT(C, PUT_RAR, count)                  \
	CNT(C, PUT_BB, count)                   \
	CNT(C, PUT_TMPMD, count)                \
	CNT(C, LONG_PACKED, count)              \
	CNT(C, LONG_RAR, count)                 \
	CNT(C, LONG_TMPMD, count)               \
	GASNETC_FIREHOSE_STATS(CNT,VAL,TIME)	\
	CNT(C, FRESH_REQRB, count)              \
        TIME(C, FRESH_STALL, stalled time)      \
        TIME(C, REFRESH_STALL, stalled time)    \
        VAL(C, EVENT_REAP, numreaped)

/* This is the default, but we define it here for use in gasnet_core.h */
#define GASNETI_MEDBUF_ALIGNMENT 8

#endif
