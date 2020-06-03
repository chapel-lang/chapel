/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_extended.c $
 * Description: GASNet Extended API Reference Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_internal.h> // for refbarrier.c
#include <gasnet_core_internal.h>
#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_ucx_req.h>

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

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

  gasneti_static_assert(sizeof(gasnete_eop_t) >= sizeof(void*));
}

extern void gasnete_init(void) {
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert_uint(gasneti_nodes ,>=, 1);
  gasneti_assert_uint(gasneti_mynode ,<, gasneti_nodes);

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

static
void gasnetc_ucx_rma_cb(void *request, ucs_status_t status)
{
  gasnetc_ucx_request_t *req = (gasnetc_ucx_request_t*) request;

  if (status != UCS_OK) {
    gasneti_fatalerror("UCX RDMA operation failed: %s",
                       ucs_status_string(status));
  }
  if (req->completion.cbfunc) {
    req->completion.cbfunc(req->completion.cbdata);
  }
  req->completion.cbfunc = NULL;
  req->completion.cbdata = NULL;
  ucp_request_free(request);
  return;
}

extern
gex_Event_t gasnete_get_nb(
                     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);

  gasnete_eop_t *eop = gasnete_eop_new(GASNETI_MYTHREAD);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  
  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  gasnetc_ucx_putget_inner(0, jobrank, dest, nbytes, src,
                           &eop->initiated_cnt, gasnetc_cb_eop_get,
                           NULL, NULL);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  return (gex_Event_t)eop;
}

extern
gex_Event_t gasnete_put_nb(
                     gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
  gasnetc_counter_t counter = GASNETC_COUNTER_INITIALIZER;

  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  gasnete_eop_t *eop = gasnete_eop_new(GASNETI_MYTHREAD);

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  if (lc_opt == GEX_EVENT_NOW) {
    gasnetc_ucx_putget_inner(1, jobrank, src, nbytes, dest,
                             &counter.initiated, gasnetc_cb_counter,
                             &eop->initiated_cnt, gasnetc_cb_eop_put);
  } else if (lc_opt == GEX_EVENT_DEFER) {
    gasnetc_ucx_putget_inner(1, jobrank, src, nbytes, dest,
                             NULL, NULL,
                             &eop->initiated_cnt, gasnetc_cb_eop_put);
  } else {
    GASNETE_EOP_LC_START(eop);
    gasnetc_atomic_val_t start_cnt = eop->initiated_alc;
    gasnetc_ucx_putget_inner(1, jobrank, src, nbytes, dest,
                             &eop->initiated_alc, gasnetc_cb_eop_alc,
                             &eop->initiated_cnt, gasnetc_cb_eop_put);
    if (start_cnt == eop->initiated_alc) {
      // Synchronous LC - reset the eop's LC state
      GASNETE_EOP_LC_FINISH(eop);
      *lc_opt = GEX_EVENT_INVALID;
    } else {
      *lc_opt = gasneti_op_event(eop, gasnete_eop_event_alc);
    }
  }
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);

  if (lc_opt == GEX_EVENT_NOW) {
    gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
  }
  return (gex_Event_t)eop;
}

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
int gasnete_get_nbi (gex_TM_t tm, void *dest, gex_Rank_t rank, void *src,
                     size_t nbytes, gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  int ret;

  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;

  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  ret = gasnetc_ucx_putget_inner(
        0, jobrank, dest, nbytes, src, &op->initiated_get_cnt,
        op->next ? gasnetc_cb_nar_get : gasnetc_cb_iop_get,
        NULL, NULL);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  return ret;
}

extern
int gasnete_put_nbi (gex_TM_t tm, gex_Rank_t rank, void *dest,
                     void *src, size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);

  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
  gasnetc_cbfunc_t local_cb;
  gasnetc_atomic_val_t *local_cnt;

  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);

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
  
  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  gasnetc_ucx_putget_inner(1, jobrank, src, nbytes, dest,
                           local_cnt, local_cb, &op->initiated_put_cnt,
                           op->next ? gasnetc_cb_nar_put : gasnetc_cb_iop_put);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);

  if (lc_opt == GEX_EVENT_NOW) {
    gasnetc_counter_wait(&counter, 1 GASNETI_THREAD_PASS);
  }
  return 0;
}

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/

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

