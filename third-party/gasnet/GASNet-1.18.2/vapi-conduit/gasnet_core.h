/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core.h,v $
 *     $Date: 2012/03/05 20:50:27 $
 * $Revision: 1.63 $
 * Description: GASNet header for vapi conduit core
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_H
#define _GASNET_CORE_H

#include <gasnet_core_help.h>

GASNETI_BEGIN_EXTERNC

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* gasnet_init not inlined or renamed because we use redef-name trick on  
   it to ensure proper version linkage */
extern int gasnet_init(int *argc, char ***argv);

extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset);
#define gasnet_attach gasnetc_attach

extern void gasnetc_exit(int exitcode) GASNETI_NORETURN;
GASNETI_NORETURNP(gasnetc_exit)
#define gasnet_exit gasnetc_exit
/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
*/
/* conduit may or may not need this based on whether interrupts are used for running handlers */
#if GASNETC_USE_INTERRUPTS
  extern void gasnetc_hold_interrupts(void);
  extern void gasnetc_resume_interrupts(void);

  #define gasnet_hold_interrupts    gasnetc_hold_interrupts
  #define gasnet_resume_interrupts  gasnetc_resume_interrupts
#else
  #define gasnet_hold_interrupts()
  #define gasnet_resume_interrupts()
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
typedef struct _gasnet_hsl_t {
  gasneti_mutex_t lock;

  #if GASNETI_STATS_OR_TRACE
    gasneti_tick_t acquiretime;
  #endif

  #if GASNETC_USE_INTERRUPTS
    /* more state may be required for conduits using interrupts */
    #error interrupts not implemented
  #endif
} gasnet_hsl_t GASNETI_THREAD_TYPEDEF;

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_LOCK_STAT_INIT ,0 
#else
  #define GASNETC_LOCK_STAT_INIT  
#endif

#if GASNETC_USE_INTERRUPTS
  #error interrupts not implemented
  #define GASNETC_LOCK_INTERRUPT_INIT 
#else
  #define GASNETC_LOCK_INTERRUPT_INIT  
#endif

#define GASNET_HSL_INITIALIZER { \
  GASNETI_MUTEX_INITIALIZER      \
  GASNETC_LOCK_STAT_INIT         \
  GASNETC_LOCK_INTERRUPT_INIT    \
  }

/* decide whether we have "real" HSL's */
#if GASNETI_THREADS || GASNETC_USE_INTERRUPTS || /* need for safety */ \
    GASNET_DEBUG || GASNETI_STATS_OR_TRACE       /* or debug/tracing */
  #ifdef GASNETC_NULL_HSL 
    #error bad defn of GASNETC_NULL_HSL
  #endif
#else
  #define GASNETC_NULL_HSL 1
#endif

#if GASNETC_NULL_HSL
  /* HSL's unnecessary - compile away to nothing */
  #define gasnet_hsl_init(hsl)
  #define gasnet_hsl_destroy(hsl)
  #define gasnet_hsl_lock(hsl)
  #define gasnet_hsl_unlock(hsl)
  #define gasnet_hsl_trylock(hsl)	GASNET_OK
#else
  extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl);
  extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl);
  extern void gasnetc_hsl_lock   (gasnet_hsl_t *hsl);
  extern void gasnetc_hsl_unlock (gasnet_hsl_t *hsl);
  extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) GASNETI_WARN_UNUSED_RESULT;

  #define gasnet_hsl_init    gasnetc_hsl_init
  #define gasnet_hsl_destroy gasnetc_hsl_destroy
  #define gasnet_hsl_lock    gasnetc_hsl_lock
  #define gasnet_hsl_unlock  gasnetc_hsl_unlock
  #define gasnet_hsl_trylock gasnetc_hsl_trylock
#endif

/* ------------------------------------------------------------------------------------ */
/* Internal threads */

#ifndef GASNETC_DYNAMIC_CONNECT
#define GASNETC_DYNAMIC_CONNECT 1
#endif

/* GASNETC_*_RCV_THREAD enables a progress thread for receiving AMs. */
#if (GASNET_CONDUIT_VAPI && GASNETC_VAPI_RCV_THREAD) || \
    (GASNET_CONDUIT_IBV  && GASNETC_IBV_RCV_THREAD)
  #define GASNETC_IB_RCV_THREAD 1
#else
  #define GASNETC_IB_RCV_THREAD 0
#endif

/* GASNETC_*_CONN_THREAD enables a progress thread for establishing dynamic connections. */
#if GASNETC_DYNAMIC_CONNECT && \
    ((GASNET_CONDUIT_VAPI && GASNETC_VAPI_CONN_THREAD) || \
     (GASNET_CONDUIT_IBV  && GASNETC_IBV_CONN_THREAD))
  #define GASNETC_IB_CONN_THREAD 1
#else
  #define GASNETC_IB_CONN_THREAD 0
#endif

/* ------------------------------------------------------------------------------------ */
/* Measures of concurency
 *
 * GASNETC_ANY_PAR      Non-zero if multiple threads can be executing in GASNet.
 *                      This is inclusive of the AM receive thread.
 * GASNETC_CLI_PAR      Non-zero if multiple _client_ threads can be executing in GASNet.
 *                      This excludes the AM receive thread.
 * These differ from GASNETI_THREADS and GASNETI_CLIENT_THREADS in that they don't count
 * GASNET_PARSYNC, since it has threads which do not enter GASNet concurrently.
 */

#if GASNET_PAR
  #define GASNETC_CLI_PAR 1
#else
  #define GASNETC_CLI_PAR 0
#endif

#define GASNETC_ANY_PAR         (GASNETC_CLI_PAR || GASNETC_IB_RCV_THREAD)

/* ------------------------------------------------------------------------------------ */
/* Type and ops for rdma counters */
/* XXX: only typedef and gasnetc_counter_wait() actually need to be public */
#include <gasnet_atomicops.h> /* must come after hsl defs */

#if GASNETC_ANY_PAR
  /* Concurrent version */
  typedef struct {
      gasneti_atomic_t     completed;
      gasneti_atomic_val_t initiated;
  } gasnetc_counter_t;
  #define GASNETC_COUNTER_INITIALIZER   {gasneti_atomic_init(0), 0}
  #define gasnetc_counter_reset(P)      do { gasneti_atomic_set(&(P)->completed, 0, 0); \
                                             (P)->initiated = 0;                         \
                                        } while (0)
  #define gasnetc_counter_done(P)       ((P)->initiated == gasneti_atomic_read(&(P)->completed, 0))
  #define gasnetc_counter_dec(P)        do { gasneti_assert(!gasnetc_counter_done(P));      \
                                             gasneti_atomic_increment(&(P)->completed, 0); \
                                        } while (0)
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    #define gasnetc_counter_dec_by(P,v)   \
        gasneti_atomic_add(&(P)->completed,(v),0)
  #else /* yuk */
    #define gasnetc_counter_dec_by(P,v)   do {       \
        int _i = (v);                                \
        while (_i) { gasnetc_counter_dec(P); _i--; } \
    } while (0)
  #endif
#else
  /* Sequential version */
  typedef struct {
      gasneti_atomic_val_t completed;
      gasneti_atomic_val_t initiated;
  } gasnetc_counter_t;
  #define GASNETC_COUNTER_INITIALIZER   {0, 0}
  #define gasnetc_counter_reset(P)      do { (P)->completed = (P)->initiated = 0; } while (0)
  #define gasnetc_counter_done(P)       ((P)->initiated == (P)->completed)
  #define gasnetc_counter_dec_by(P,v)   do { gasneti_assert((v) <= \
                                                 ((P)->initiated - (P)->completed)); \
                                             (P)->completed += (v); \
                                        } while (0)
  #define gasnetc_counter_dec(P)        gasnetc_counter_dec_by((P),1)
#endif

/* Same version concurrent/sequential */
#define gasnetc_counter_inc(P)		do { (P)->initiated++; } while (0)
#define gasnetc_counter_inc_by(P,v)	do { (P)->initiated += (v); } while (0)
#define gasnetc_counter_inc_if(P)	do { if(P) gasnetc_counter_inc(P); } while (0)
#define gasnetc_counter_inc_if_pf(P)	do { if_pf(P) gasnetc_counter_inc(P); } while (0)
#define gasnetc_counter_inc_if_pt(P)	do { if_pt(P) gasnetc_counter_inc(P); } while (0)
#define gasnetc_counter_dec_if(P)	do { if(P) gasnetc_counter_dec(P); } while (0)
#define gasnetc_counter_dec_if_pf(P)	do { if_pf(P) gasnetc_counter_dec(P); } while (0)
#define gasnetc_counter_dec_if_pt(P)	do { if_pt(P) gasnetc_counter_dec(P); } while (0)
#if GASNETI_STATS_OR_TRACE
  #define gasnetc_counter_val(P)	((P)->initiated)
#endif

/* Wait until given counter is marked as done.
 * Note that no AMPoll is done in the best case.
 */
extern void gasnetc_counter_wait_aux(gasnetc_counter_t *counter, int handler_context);
GASNETI_INLINE(gasnetc_counter_wait)
void gasnetc_counter_wait(gasnetc_counter_t *counter, int handler_context) { 
  if_pf (!gasnetc_counter_done(counter)) {
    gasnetc_counter_wait_aux(counter, handler_context);
  }
  gasneti_sync_reads();
} 
/* ------------------------------------------------------------------------------------ */
/*
  Active Message Size Limits
  ==========================
*/

#define GASNETC_BUFSZ		4096

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_HDR_TIMESTAMP	8
#else
  #define GASNETC_HDR_TIMESTAMP	0
#endif
#define GASNETC_MEDIUM_HDRSZ	(GASNETC_HDR_TIMESTAMP + 4)
#define GASNETC_LONG_HDRSZ	(GASNETC_HDR_TIMESTAMP + SIZEOF_VOID_P + 4)

#define GASNETC_MAX_ARGS_USER	16
#define GASNETC_MAX_ARGS_EXTRA	1	/* For flow-control info */
#define GASNETC_MAX_ARGS	(GASNETC_MAX_ARGS_USER + GASNETC_MAX_ARGS_EXTRA)

#define GASNETC_MAX_MEDIUM_	\
               (GASNETC_BUFSZ - GASNETI_ALIGNUP_NOASSERT(GASNETC_MEDIUM_HDRSZ + 4*GASNETC_MAX_ARGS, 8))
#if GASNET_PSHM
  #define GASNETC_MAX_MEDIUM	MIN(GASNETC_MAX_MEDIUM_, GASNETI_MAX_MEDIUM_PSHM)
#else
  #define GASNETC_MAX_MEDIUM	GASNETC_MAX_MEDIUM_
#endif
#define GASNETC_MAX_LONG_REQ	(0x7fffffff)
#define GASNETC_MAX_PACKEDLONG	(GASNETC_BUFSZ - GASNETC_LONG_HDRSZ - 4*GASNETC_MAX_ARGS)
#if GASNETC_PIN_SEGMENT
  #define GASNETC_MAX_LONG_REP	GASNETC_MAX_LONG_REQ
#else
  #define GASNETC_MAX_LONG_REP	GASNETC_MAX_PACKEDLONG
#endif

#define gasnet_AMMaxArgs()          ((size_t)GASNETC_MAX_ARGS_USER)
#define gasnet_AMMaxMedium()        ((size_t)GASNETC_MAX_MEDIUM)
#define gasnet_AMMaxLongRequest()   ((size_t)GASNETC_MAX_LONG_REQ)	
#define gasnet_AMMaxLongReply()     ((size_t)GASNETC_MAX_LONG_REP)

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex);
extern int gasnetc_AMPoll(void);

#define gasnet_AMGetMsgSource  gasnetc_AMGetMsgSource

#define GASNET_BLOCKUNTIL(cond) gasneti_polluntil(cond)

/* ------------------------------------------------------------------------------------ */
/*
  RDMA ops
  =====================
 */

/*
 * gasnetc_epid_d is node and qp encoded together
 * passing just a node (the default) means any qp to that node
 */
typedef uint32_t gasnetc_epid_t;

/* RDMA initiation operations */
#if GASNETC_PIN_SEGMENT
  extern int gasnetc_rdma_put(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust);
#else
  extern int gasnetc_rdma_put_fh(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust, gasnetc_counter_t *am_oust);
  #define gasnetc_rdma_put(epid,src_ptr,dst_ptr,nbytes,mem_oust,req_oust) \
	gasnetc_rdma_put_fh(epid,src_ptr,dst_ptr,nbytes,mem_oust,req_oust,NULL)
#endif
extern int gasnetc_rdma_get(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *req_oust);
extern int gasnetc_rdma_putv(gasnetc_epid_t epid, size_t srccount, gasnet_memvec_t const srclist[], void *dst_ptr, gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust);
extern int gasnetc_rdma_getv(gasnetc_epid_t epid, void *src_ptr, size_t dstcount, gasnet_memvec_t const dstlist[], gasnetc_counter_t *req_oust);

/* ------------------------------------------------------------------------------------ */

GASNETI_END_EXTERNC

#endif

#include <gasnet_ammacros.h>
