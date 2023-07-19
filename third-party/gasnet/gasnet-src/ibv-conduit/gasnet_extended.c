/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_extended.c $
 * Description: GASNet Extended API over IB Verbs Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_internal.h> // for refbarrier.c
#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_ibv.h>

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

extern void gasnetc_new_threaddata_callback(void **core_threadinfo);
#define GASNETC_NEW_THREADDATA_CALLBACK(td) gasnetc_new_threaddata_callback(&((td)->gasnetc_threaddata))

#include "gasnet_extended_common.c"

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config(void) {
  gasneti_check_config_postattach();
  gasnete_check_config_amref();

  gasneti_assert(sizeof(gasnete_eop_t) >= sizeof(void*));
}

extern void gasnete_init(void) {
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

  { gasneti_threaddata_t *threaddata = NULL;
  #if GASNETI_MAX_THREADS > 1
    /* register first thread (optimization) */
    threaddata = _gasneti_mythread_slow();
  #else
    /* register only thread (required) */
    threaddata = gasnete_new_threaddata();
  #endif
  #if !GASNETI_DISABLE_REFERENCE_EOP
    /* cause the first pool of eops to be allocated (optimization) */
    GASNET_POST_THREADINFO(threaddata);
    gasnete_eop_t *eop = gasnete_eop_new(threaddata);
    GASNETE_EOP_MARKDONE(eop);
    gasnete_eop_free(eop GASNETI_THREAD_PASS);
  #endif
  }

  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize team/collectives */
  gasnete_coll_init_subsystem();

  /* Initialize VIS subsystem */
  gasnete_vis_init();
}

/* ------------------------------------------------------------------------------------ */
/*
  Get/Put:
  ========
*/

/* Use some or all of the reference implementation of get/put in terms of AMs
 * Configuration appears in gasnet_extended_fwd.h
 */
#include "gasnet_extended_amref.c"

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit event)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nb
     gasnete_put_nb
*/

extern
gex_Event_t gasnete_get_nb(
                     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

  /* XXX check error returns */
  gasnetc_rdma_get(tm, rank, src, dest, nbytes, flags,
                   &op->initiated_cnt, gasnetc_cb_eop_get
                   GASNETI_THREAD_PASS);
  return (gex_Event_t)op;
}

extern
gex_Event_t gasnete_put_nb(
                     gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

  /* XXX check error returns */
  #define GASNETC_RDMA_PUT(local_cnt, local_cb)              \
    gasnetc_rdma_put(tm, rank, src, dest, nbytes, flags,     \
                     local_cnt, local_cb,                    \
                     &op->initiated_cnt, gasnetc_cb_eop_put  \
                     GASNETI_THREAD_PASS)

  if (lc_opt == GEX_EVENT_NOW) {
    gasnetc_counter_t counter = GASNETC_COUNTER_INITIALIZER;
    GASNETC_RDMA_PUT(&counter.initiated, gasnetc_cb_counter);
    gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
  } else if (lc_opt == GEX_EVENT_DEFER) {
    GASNETC_RDMA_PUT(NULL, NULL);
  } else {
    gasneti_assume_leaf_is_pointer(lc_opt);
    GASNETE_EOP_LC_START(op);
    gasnetc_atomic_val_t start_cnt = op->initiated_alc;
    GASNETC_RDMA_PUT(&op->initiated_alc, gasnetc_cb_eop_alc);
    if (start_cnt == op->initiated_alc) {
      // Synchronous LC - reset the eop's LC state
      GASNETE_EOP_LC_FINISH(op);
      *lc_opt = GEX_EVENT_INVALID;
    } else {
      *lc_opt = gasneti_op_event(op, gasnete_eop_event_alc);
    }
  }

  return (gex_Event_t)op;
}
#undef GASNETC_RDMA_PUT

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nbi
     gasnete_put_nbi
*/

extern
int gasnete_get_nbi (gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;

  /* XXX check error returns */ 
  gasnetc_rdma_get(tm, rank, src, dest, nbytes, flags,
                   &op->initiated_get_cnt,
                   op->next ? gasnetc_cb_nar_get : gasnetc_cb_iop_get
                   GASNETI_THREAD_PASS);
  return 0;
}

extern
int gasnete_put_nbi (gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
  gasnetc_atomic_val_t *local_cnt;
  gasnetc_cb_t         local_cb;

  if (lc_opt == GEX_EVENT_GROUP) {
    local_cnt = &op->initiated_alc_cnt;
    local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
  } else if (lc_opt == GEX_EVENT_NOW) {
    local_cnt = &counter.initiated;
    local_cb = gasnetc_cb_counter;
  } else if (lc_opt == GEX_EVENT_DEFER) {
    local_cnt = NULL;
    local_cb = NULL;
  } else {
    gasneti_fatalerror("Invalid lc_opt argument to Put_nbi");
  }

  /* XXX check error returns */ 
  gasnetc_rdma_put(tm, rank, src, dest, nbytes, flags,
                   local_cnt, local_cb,
                   &op->initiated_put_cnt,
                   op->next ? gasnetc_cb_nar_put : gasnetc_cb_iop_put
                   GASNETI_THREAD_PASS);
  if (lc_opt == GEX_EVENT_NOW) gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/

extern int gasnete_get  (gex_TM_t tm,
                         void* dest,
                         gex_Rank_t rank, void *src,
                         size_t nbytes, gex_Flags_t flags
                         GASNETI_THREAD_FARG)
{
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;

  /* XXX check error returns */ 
  gasnetc_rdma_get(tm, rank, src, dest, nbytes, flags,
                   &req_oust.initiated, gasnetc_cb_counter_rel
                   GASNETI_THREAD_PASS);
  gasnetc_counter_wait(&req_oust, 0 GASNETI_THREAD_PASS);
  return 0;
}

extern int gasnete_put  (gex_TM_t tm,
                         gex_Rank_t rank, void* dest,
                         /*const*/ void *src,
                         size_t nbytes, gex_Flags_t flags
                         GASNETI_THREAD_FARG)
{
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;

  /* XXX check error returns */ 
  gasnetc_rdma_put(tm, rank, src, dest, nbytes, flags,
                   NULL, NULL,
                   &req_oust.initiated, gasnetc_cb_counter
                   GASNETI_THREAD_PASS);
  gasnetc_counter_wait(&req_oust, 0 GASNETI_THREAD_PASS);
  return 0;
}   

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/

// GASNET_BARRIER="IBDISSEM" is now an alias for "RDMADISSEM"
#define GASNETE_BARRIER_READENV() do {                                  \
  if (GASNETE_ISBARRIER("IBDISSEM"))                                    \
    gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_RDMADISSEM;\
} while (0)
#define GASNETE_CONDUIT_RDMABARRIER(barrier, retval_ptr) \
  (!strcmp(barrier,"IBDISSEM"))

/* use reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C

/* ------------------------------------------------------------------------------------ */
/*
  Vector, Indexed & Strided:
  =========================
*/

/* use reference implementation of scatter/gather and strided */
#include "gasnet_refvis.h"

/* ------------------------------------------------------------------------------------ */
/*
  Collectives:
  ============
*/

/* use reference implementation of collectives */
#include "gasnet_refcoll.h"

/* ------------------------------------------------------------------------------------ */
/*
  Remote Atomics:
  ==============
*/

/* use reference implementation of remote atomics */
#include "gasnet_refratomic.h"

/* ------------------------------------------------------------------------------------ */
/*
  Handlers:
  =========
*/
static gex_AM_Entry_t const gasnete_handlers[] = {
  #ifdef GASNETE_REFBARRIER_HANDLERS
    GASNETE_REFBARRIER_HANDLERS(),
  #endif
  #ifdef GASNETE_REFVIS_HANDLERS
    GASNETE_REFVIS_HANDLERS()
  #endif
  #ifdef GASNETE_REFCOLL_HANDLERS
    GASNETE_REFCOLL_HANDLERS()
  #endif
  #ifdef GASNETE_AMREF_HANDLERS
    GASNETE_AMREF_HANDLERS()
  #endif
  #ifdef GASNETE_AMRATOMIC_HANDLERS
    GASNETE_AMRATOMIC_HANDLERS()
  #endif

  /* ptr-width independent handlers */

  /* ptr-width dependent handlers */

  GASNETI_HANDLER_EOT
};

extern gex_AM_Entry_t const *gasnete_get_handlertable(void) {
  return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

