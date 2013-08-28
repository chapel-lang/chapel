/*   $Source: /var/local/cvs/gasnet/gemini-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2013/03/13 23:50:00 $
 * $Revision: 1.16 $
 * Description: GASNet header for <conduitname> conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      0.3
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#if defined GASNET_CONDUIT_GEMINI
  #define GASNET_CORE_NAME       GEMINI
#elif defined GASNET_CONDUIT_ARIES
  #define GASNET_CORE_NAME       ARIES
#else
  #error "Exactly one of GASNET_CONDUIT_GEMINI or GASNET_CONDUIT_ARIES must be defined"
#endif
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)

/* Aries supports only 24 bits of inst_id and we steal one bit for ACKs */
#define GASNET_MAXNODES 0x800000
#define GASNETC_LOG2_MAXNODES 23

  /* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
#define GASNET_PSHM 1
#endif

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   ###
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
			    /* LCS don't need, I think */
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
#if 0
#define GASNETC_GET_HANDLER 1
typedef ### gasnetc_handler_t;
#endif
#if 0
#define GASNETC_TOKEN_CREATE 1
#endif

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)       \
        TIME(C, GET_AM_CREDIT_STALL, stalled time) \
        TIME(C, ALLOC_PD_STALL, stalled time) \
        TIME(C, ALLOC_BB_STALL, stalled time) \
        TIME(C, MEM_REG_STALL, stalled time) \
        VAL(C, POST_FMA_RETRY, retries) \
        VAL(C, POST_RDMA_RETRY, retries) \
        VAL(C, SMSG_SEND_RETRY, retries) \
        VAL(C, MEM_REG_RETRY, retries) \
        /* blank */

#endif
