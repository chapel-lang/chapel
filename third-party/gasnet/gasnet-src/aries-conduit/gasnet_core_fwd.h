/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/gasnet_core_fwd.h $
 * Description: GASNet header for aries conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      2.2
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         ARIES
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_ARIES     1

/* Aries supports only 24 bits of inst_id and we leverage that */
#define GASNET_MAXNODES 0x1000000
#define GASNETC_LOG2_MAXNODES 24

  /* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
#define GASNET_PSHM 1
#endif

// PSHM and loopback support need to know largest Medium if larger than MAX(LUB{Request,Reply}Medium)
#define GASNETC_MAX_MEDIUM_NBRHD GASNETC_MAX_MEDIUM(0)

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

  /* define these to 1 if your conduit needs to augment the implementation
     of gasneti_reghandler() (in gasnet_internal.c)
   */
#if 0
#define GASNETC_AMREGISTER 1
#endif

  /* define this to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
#if 0
#define GASNETC_GET_HANDLER 1
#endif

  /* uncomment for each {Request,Reply} X {Medium,Long} pair for which your
     conduit implements the corresponding gasnetc_AM_{Prepare,Commit}*().
     If unset, a conduit-independent implementation in terms of the internal
     functions gasnetc_AM{Request,Reply}{Medium,Long}V() will be used, and
     your conduit must provide the V-suffixed functions for any of these that
     are not defined.
   */
#define GASNETC_HAVE_NP_REQ_MEDIUM 1
#define GASNETC_HAVE_NP_REP_MEDIUM 1
/* #define GASNETC_HAVE_NP_REQ_LONG 1 */
/* #define GASNETC_HAVE_NP_REP_LONG 1 */

  /* uncomment for each GASNETC_HAVE_NP_* enabled above if the Commit function
     has the numargs argument even in an NDEBUG build (it is always passed in
     DEBUG builds).
   */
//#define GASNETC_AM_COMMIT_REQ_MEDIUM_NARGS 1
//#define GASNETC_AM_COMMIT_REP_MEDIUM_NARGS 1
//#define GASNETC_AM_COMMIT_REQ_LONG_NARGS 1
//#define GASNETC_AM_COMMIT_REP_LONG_NARGS 1

  /* uncomment if your conduit's gasnetc_AMRequest{Short,Medium,Long}V()
     include a call to gasneti_AMPoll (or equivalent) for progress.
     The preferred implementation is to Poll only in the M-suffixed calls
     and not the V-suffixed calls (and GASNETC_REQUESTV_POLLS undefined).
     Used if (and only if) any of the GASNETC_HAVE_NP_* values above are unset.
   */
/* #define GASNETC_REQUESTV_POLLS 1 */

#if defined(GASNET_PAR) && GASNETC_GNI_MULTI_DOMAIN
/* Need to hook pthread create to ensure collective creation of domains */
typedef int (gasnetc_pthread_create_fn_t)(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
extern int gasnetc_pthread_create(gasnetc_pthread_create_fn_t *create_fn, pthread_t *thread, const pthread_attr_t *attr, void * (*fn)(void *), void * arg) ;
#define GASNETC_PTHREAD_CREATE_OVERRIDE(create_fn, thread, attr, start_routine, arg) \
   gasnetc_pthread_create(create_fn, thread, attr, start_routine, arg)
#endif

#if !GASNETC_GNI_MULTI_DOMAIN
/* support top-level poll throttling when not using multi-domain */
#define GASNETC_USING_SUSPEND_RESUME 1
#endif

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)       \
        TIME(C, GET_AM_REM_BUFFER_STALL, stalled time) \
        TIME(C, GET_AM_LOC_BUFFER_STALL, stalled time) \
        TIME(C, GET_AM_LONG_CREDIT_STALL, stalled time) \
        TIME(C, ALLOC_PD_STALL, stalled time) \
        TIME(C, ALLOC_BB_STALL, stalled time) \
        TIME(C, ALLOC_AMRV_STALL, stalled time) \
        TIME(C, MEM_REG_STALL, stalled time) \
        VAL(C, POST_FMA_RETRY, retries) \
        VAL(C, POST_RDMA_RETRY, retries) \
        VAL(C, AM_SEND_RETRY, retries) \
        VAL(C, CTRL_SEND_RETRY, retries) \
        VAL(C, MEM_REG_RETRY, retries) \
        VAL(C, AMPOLL_EVENTS, GNI events) \
        CNT(C, AMPOLL_INS, late notifies) \
        /* blank */

extern void gasnetc_fatalsignal_callback(int sig);
#define GASNETC_FATALSIGNAL_CALLBACK(sig) gasnetc_fatalsignal_callback(sig)

extern void gasnetc_trace_finish(void);
#define GASNETC_TRACE_FINISH() gasnetc_trace_finish()

#endif
