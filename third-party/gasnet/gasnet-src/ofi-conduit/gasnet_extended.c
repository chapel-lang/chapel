/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_extended.c $
 * Description: GASNet Extended API over libfabric (OFI)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_internal.h> // for refbarrier.c
#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>

#include <gasnet_ofi.h>

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

#if GASNETC_OFI_REFERENCE_EXTENDED
  if (! gasneti_mynode) {
    gasneti_console_message("WARNING",
                            "This build of GASNet-EX ofi-conduit has been configured to "
                            "use the unsupported legacy implementation of RMA operations.");
  }
#endif

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

#if !GASNETC_OFI_REFERENCE_EXTENDED

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
  op->ofi.type = OFI_TYPE_EGET;
  if (gasnetc_rdma_get(dest, tm, rank, src, nbytes,
                       &op->ofi, flags GASNETI_THREAD_PASS)) {
    gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
    GASNETE_EOP_MARKDONE(op);
    gasnete_eop_free(op GASNETI_THREAD_PASS);
    return GEX_EVENT_NO_OP;
  }
  return (gex_Event_t)op;
}

// TODO-EX: Improved LC support.
//  + NOW will sometimes need to block for RC
//  + explicit handle might use FI_INJECT for synchronous LC of small nbytes
extern
gex_Event_t gasnete_put_nb(
                    gex_TM_t tm,
                    gex_Rank_t rank, void *dest,
                    void *src,
                    size_t nbytes, gex_Event_t *lc_opt,
                    gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);
  op->ofi.type = OFI_TYPE_EPUT;

#if GASNET_DEBUG
  if (lc_opt == GEX_EVENT_GROUP) {
    gasneti_fatalerror("Invalid lc_opt argument to gex_RMA_PutNB");
  }
#endif

  if (lc_opt == GEX_EVENT_NOW) {
    // Try to submit for synchronous LC.
    // If we can't, then we must block for RC.
    gex_Event_t ev = gasnetc_rdma_put_non_bulk(tm, rank, dest, src, nbytes, &op->ofi, flags GASNETI_THREAD_PASS);
    if (ev) {
      GASNETE_EOP_MARKDONE(op);
      gasnete_eop_free(op GASNETI_THREAD_PASS);
      if (ev == GEX_EVENT_NO_OP) return ev;
      op = NULL; // aka GASNET_EVENT_INVALID
      gasnete_wait(ev GASNETI_THREAD_PASS);
    }
  } else {
    int alc = (lc_opt != GEX_EVENT_DEFER);
    if (alc) {
      gasneti_assert(gasneti_leaf_is_pointer(lc_opt));
      // TODO: should attempt inject-based synchronous LC, unless GEX_FLAG_LC_COPY_NO
      GASNETE_EOP_LC_START(op);
      *lc_opt = gasneti_op_event(op, gasnete_eop_event_alc);
    }
    if (gasnetc_rdma_put(tm, rank, dest, src, nbytes,
                         &op->ofi, alc, flags GASNETI_THREAD_PASS)) {
      gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
      if (alc) GASNETE_EOP_LC_FINISH(op);
      GASNETE_EOP_MARKDONE(op);
      gasnete_eop_free(op GASNETI_THREAD_PASS);
      return GEX_EVENT_NO_OP;
    }
  }

  return (gex_Event_t)op;
}

#endif // !GASNETC_OFI_REFERENCE_EXTENDED

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */

#if !GASNETC_OFI_REFERENCE_EXTENDED

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nbi
     gasnete_put_nbi
*/

extern
int gasnete_get_nbi(
                    gex_TM_t tm,
                    void *dest,
                    gex_Rank_t rank, void *src,
                    size_t nbytes,
                    gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  gasneti_assert(op->get_ofi.type == OFI_TYPE_IGET);
  op->initiated_get_cnt++;
  if (gasnetc_rdma_get(dest, tm, rank, src, nbytes,
                       &op->get_ofi, flags GASNETI_THREAD_PASS)) {
    gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
    GASNETE_IOP_CNT_FINISH(op, get, 1, GASNETI_ATOMIC_NONE); // NONE only because no xfer
    return 1;
  }
  return GASNET_OK;
}

// TODO-EX: Improved LC support.
//  + NOW will sometimes need to block for RC
//  + GROUP might use FI_INJECT for synchronous LC of small nbytes
extern
int gasnete_put_nbi(
                    gex_TM_t tm,
                    gex_Rank_t rank, void *dest,
                    void *src,
                    size_t nbytes, gex_Event_t *lc_opt,
                    gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  gasneti_assert(op->put_ofi.type == OFI_TYPE_IPUT);
  op->initiated_put_cnt++;

#if GASNET_DEBUG
  if (gasneti_leaf_is_pointer(lc_opt)) {
    gasneti_fatalerror("Invalid lc_opt argument to gex_RMA_PutNBI");
  }
#endif

  if (lc_opt == GEX_EVENT_NOW) {
    // Try to submit for synchronous LC.
    // If we can't, then we must block for RC.
    gex_Event_t ev = gasnetc_rdma_put_non_bulk(tm, rank, dest, src, nbytes, &op->put_ofi, flags GASNETI_THREAD_PASS);
    if (ev) {
      GASNETE_IOP_CNT_FINISH(op, put, 1, GASNETI_ATOMIC_NONE);
      if (ev == GEX_EVENT_NO_OP) return 1;
      gasnete_wait(ev GASNETI_THREAD_PASS);
    }
  } else {
    int alc = (lc_opt != GEX_EVENT_DEFER);
    if (alc) {
      gasneti_assert(lc_opt == GEX_EVENT_GROUP);
      // TODO: should attempt inject-based synchronous LC, unless GEX_FLAG_LC_COPY_NO
      GASNETE_IOP_LC_START(op);
    }
    if (gasnetc_rdma_put(tm, rank, dest, src, nbytes,
                         &op->put_ofi, alc, flags GASNETI_THREAD_PASS)) {
      gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
      if (alc) GASNETE_IOP_LC_FINISH(op);
      GASNETE_IOP_CNT_FINISH(op, put, 1, GASNETI_ATOMIC_NONE);
      return 1;
    }
  }

  return GASNET_OK;
}

#endif // !GASNETC_OFI_REFERENCE_EXTENDED

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/

// Must override the default barrier selection until we've resolved
//   Bug 4427 - ofi-conduit failures with RDMADISSEM barrier
#define GASNETE_BARRIER_DEFAULT "AMDISSEM"

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

