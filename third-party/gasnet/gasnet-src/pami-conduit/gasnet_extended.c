/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_extended.c $
 * Description: GASNet Extended API PAMI-conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2012, Lawrence Berkeley National Laboratory
 * Terms of use are as specified in license.txt
 */

// for team->pami, used in conduit-specific barrier:
#define GASNETI_NEED_GASNET_COLL_H 1

#include <gasnet_internal.h>
#include <gasnet_coll_internal.h> // for refbarrier.c
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>

static pami_send_hint_t gasnete_null_send_hint;

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
static pami_send_hint_t gasnete_rdma_send_hint;
static uintptr_t gasnete_mysegbase;
static uintptr_t gasnete_mysegsize;
static uintptr_t gasnete_myauxbase;
static uintptr_t gasnete_myauxsize;
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Conduit-specific op management
  ==============================
*/

/* callbacks implementing subsets of gasnete_op_markdone */
static void gasnete_cb_eop_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_eop_t *eop = (gasnete_eop_t *)cookie;
  gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);
  gasneti_assert(! EVENT_DONE(eop, 0));
  /* gasnete_eop_check(eop);  XXX: conflicts w/ on-stack EOP used for blocking ops */
  SET_EVENT_DONE(eop, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_rput_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, put, 1, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_rget_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, get, 1, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_ralc_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, alc, 1, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_iput_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, put, 1, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_iget_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, get, 1, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_ialc_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, alc, 1, 0);
  gasneti_assert(status == PAMI_SUCCESS);
}

/* callbacks for local completion of non-bulk puts */
static void gasnete_cb_eop_lc(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_eop_t *eop = (gasnete_eop_t *)cookie;
  gasnete_eop_check(eop);
  GASNETE_LC_NOW_FINISH(eop);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_iop_lc(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  GASNETE_LC_NOW_FINISH(iop);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_ptr_lc(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_eop_t *eop = (gasnete_eop_t *)cookie;
  gasnete_eop_check(eop);
  GASNETE_EOP_LC_FINISH(eop);
  gasneti_assert(status == PAMI_SUCCESS);
}

/* callback for synchronous remote completion */
static void gasnete_cb_int_done(pami_context_t context, void *cookie, pami_result_t status) {
  int *p = (int *)cookie;
  gasneti_assert(0 == *p);
  *p = 1;
  gasneti_assert(status == PAMI_SUCCESS);
}

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

  /* Initialize conduit-specific resources */

  memset(&gasnete_null_send_hint, 0, sizeof(gasnete_null_send_hint));
#if GASNET_PSHM
  gasnete_null_send_hint.use_shmem = PAMI_HINT_DISABLE;
#endif

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  memset(&gasnete_rdma_send_hint, 0, sizeof(gasnete_rdma_send_hint));
  gasnete_rdma_send_hint.buffer_registered = PAMI_HINT_ENABLE; /* Kind of obvious */
 #if GASNET_PSHM
  gasnete_rdma_send_hint.use_shmem = PAMI_HINT_DISABLE;
 #endif
  gasnete_rdma_send_hint.use_rdma = PAMI_HINT_ENABLE;

  if (gasnetc_memreg != NULL) {
    gasnete_mysegbase = (uintptr_t)gasneti_seginfo[gasneti_mynode].addr;
    gasnete_mysegsize = gasneti_seginfo[gasneti_mynode].size;
  } else {
    gasnete_mysegbase = 0;
    gasnete_mysegsize = 0;
  }

  if (gasnetc_auxreg != NULL) {
    gasnete_myauxbase = (uintptr_t)gasneti_seginfo_aux[gasneti_mynode].addr;
    gasnete_myauxsize = gasneti_seginfo_aux[gasneti_mynode].size;
  } else {
    gasnete_myauxbase = 0;
    gasnete_myauxsize = 0;
  }
#endif
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
 * Design/Approach for gets/puts in Extended API in terms of PAMI
 * ========================================================================
 *
 * gasnet_put(_bulk) is translated to PAMI_Put() or PAMI_Rput()
 *   and blocks on an on-stack eop (avoiding alloc/free overheads)
 *
 * gasnet_get(_bulk) is translated to PAMI_Get() or PAMI_Rget()
 *   and blocks on an on-stack eop (avoiding alloc/free overheads)
 *
 * gasnete_put_nb(_bulk) translates to PAMI_Put() or PAMI_Rput()
 *   non-bulk spin-polls for local-completion flag in the eop
 *
 * gasnete_get_nb(_bulk) translates to PAMI_Get() or PAMI_Rget()
 *
 * gasnete_put_nbi(_bulk) translates to PAMI_Put() or PAMI_Rput()
 *   non-bulk spin-polls for local-completion flag in the eop
 *
 * gasnete_get_nbi(_bulk) translates to PAMI_Get() or PAMI_Rget()
 *
 * For the local-completion, the design is slightly complicated by the fact
 * that while PAMI has distinct Local and Remote completion callbacks, they
 * are passed the same "cookie".  This means one can't just block for local
 * completion by spinning on a stack variable without some method to get
 * both the event and the spin-flag from the same pointer.  Rather than
 * try to do that, an unused bit in the 'flags' field common to all ops has
 * been allocated to be an "LC" flag, on which we can spin.
 */

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (helpers)
  ==========================================================
*/

/* TODO: use Rput w/ firehose or bounce buffers when only dest is in-segment */
GASNETI_INLINE(gasnete_put_common)
void gasnete_put_common(gex_Rank_t jobrank, void *dest, void *src, size_t nbytes,
                        pami_event_function ldone_fn, pami_event_function rdone_fn,
                        void *cookie) {
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  // Find local registration, if any
  pami_memregion_t *loc_mr = NULL;
  uintptr_t loc_offset;
  if ((loc_offset = (uintptr_t)src - gasnete_mysegbase)
                                   < gasnete_mysegsize) {
    loc_mr = &gasnetc_mymemreg;
  } else if ((loc_offset = (uintptr_t)src - gasnete_myauxbase)
                                          < gasnete_myauxsize) {
    loc_mr = &gasnetc_myauxreg;
  }

  // Find remote registration, if any (but only if we found a local one)
  pami_memregion_t *rem_mr = NULL;
  uintptr_t rem_offset;
  if (loc_mr) {
    if_pt ((rem_offset = (uintptr_t)dest - (uintptr_t)gasneti_seginfo[jobrank].addr)
                                                    < gasneti_seginfo[jobrank].size) {
      rem_mr = &gasnetc_memreg[jobrank];
    } else if ((rem_offset = (uintptr_t)dest - (uintptr_t)gasneti_seginfo_aux[jobrank].addr)
                                                        < gasneti_seginfo_aux[jobrank].size) {
      rem_mr = &gasnetc_auxreg[jobrank];
    }
  }

  if (rem_mr) {
    pami_rput_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(jobrank);
    cmd.rma.hints = gasnete_rdma_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = cookie;
    cmd.rma.done_fn = ldone_fn;
    cmd.rdma.local.mr = loc_mr;
    cmd.rdma.local.offset = loc_offset;
    cmd.rdma.remote.mr = rem_mr;
    cmd.rdma.remote.offset = rem_offset;
    cmd.put.rdone_fn = rdone_fn;

    GASNETC_PAMI_LOCK(gasnetc_context);
    {
      pami_result_t rc;

      rc = PAMI_Rput(gasnetc_context, &cmd);
      GASNETC_PAMI_CHECK(rc, "calling PAMI_Rput");

      /* Always advance at least once */
      rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing PAMI_Rput");
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  } else
#endif
  {
    pami_put_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(jobrank);
    cmd.rma.hints = gasnete_null_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = cookie;
    cmd.rma.done_fn = ldone_fn;
    cmd.addr.local = src;
    cmd.addr.remote = dest;
    cmd.put.rdone_fn = rdone_fn;

    GASNETC_PAMI_LOCK(gasnetc_context);
    {
      pami_result_t rc;

      rc = PAMI_Put(gasnetc_context, &cmd);
      GASNETC_PAMI_CHECK(rc, "calling PAMI_Put");

      /* Always advance at least once */
      rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing PAMI_Put");
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  }
}

/* TODO: use Rget w/ firehose or bounce buffers when only src is in-segment */
// TODO-EX: enable Rget for auxseg (except no point due to Rget bug on BG/Q)
GASNETI_INLINE(gasnete_get_common)
void gasnete_get_common(void *dest, gex_Rank_t jobrank, void *src, size_t nbytes,
                        pami_event_function done_fn, void *cookie) {
#if (GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE) && !GASNETI_ARCH_BGQ /* work-around a BG/Q bug */
  uintptr_t loc_offset = (uintptr_t)dest - gasnete_mysegbase;
  uintptr_t rem_offset = (uintptr_t)src - (uintptr_t)gasneti_seginfo[jobrank].addr;

  if ((loc_offset < gasnete_mysegsize) && GASNETT_PREDICT_TRUE(rem_offset < gasneti_seginfo[jobrank].size)) {
    pami_rget_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(jobrank);
    cmd.rma.hints = gasnete_rdma_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = cookie;
    cmd.rma.done_fn = done_fn;
    cmd.rdma.local.mr = &gasnetc_mymemreg;
    cmd.rdma.local.offset = loc_offset;
    cmd.rdma.remote.mr = &gasnetc_memreg[jobrank];
    cmd.rdma.remote.offset = rem_offset;

    GASNETC_PAMI_LOCK(gasnetc_context);
    { pami_result_t rc;

      rc = PAMI_Rget(gasnetc_context, &cmd);
      GASNETC_PAMI_CHECK(rc, "calling PAMI_Rget");

      /* Always advance at least once */
      rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing PAMI_Rget");
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  } else
#endif
  {
    pami_get_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(jobrank);
    cmd.rma.hints = gasnete_null_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = cookie;
    cmd.rma.done_fn = done_fn;
    cmd.addr.local = dest;
    cmd.addr.remote = src;

    GASNETC_PAMI_LOCK(gasnetc_context);
    { pami_result_t rc;

      rc = PAMI_Get(gasnetc_context, &cmd);
      GASNETC_PAMI_CHECK(rc, "calling PAMI_Get");

      /* Always advance at least once */
      rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing PAMI_Get");
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit event)
  ==========================================================
*/

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
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  {
    gasnete_eop_t * op = gasnete_eop_new(GASNETI_MYTHREAD);
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    gasnete_get_common(dest, jobrank, src, nbytes, gasnete_cb_eop_done, op);
    return (gex_Event_t)op;
  }
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
  {
    gasnete_eop_t * op = gasnete_eop_new(GASNETI_MYTHREAD);
    pami_event_function ldone_fn = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      ldone_fn = gasnete_cb_ptr_lc;
      *lc_opt = gasneti_op_event(op, gasnete_eop_event_alc);
      GASNETE_EOP_LC_START(op);
    } else if (lc_opt == GEX_EVENT_NOW) {
      ldone_fn = gasnete_cb_eop_lc;
      GASNETE_LC_NOW_START(op);
    } else if (lc_opt == GEX_EVENT_DEFER) {
      // Nothing to do
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to Put_nb");
    }

    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    gasnete_put_common(jobrank, dest, src, nbytes, ldone_fn, gasnete_cb_eop_done, op);
    if (lc_opt == GEX_EVENT_NOW) {
      gasneti_polluntil(GASNETT_PREDICT_TRUE(GASNETE_LC_NOW_DONE(op)));
    }

    return (gex_Event_t)op;
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
  each completion increments a counter - we compare this to the  number of implicit ops launched
  for memset only, the completion is an explicit AM-level ack
*/

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nbi
     gasnete_put_nbi
*/

extern
int gasnete_get_nbi( gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  {
    gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    op->initiated_get_cnt++;
    pami_event_function rdone_fn = op->next ? gasnete_cb_rget_done : gasnete_cb_iget_done;
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    gasnete_get_common(dest, jobrank, src, nbytes, rdone_fn, op);
    return 0;
  }
}

extern
int gasnete_put_nbi( gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
  {
    gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    pami_event_function ldone_fn = NULL;

    op->initiated_put_cnt++;

    if (lc_opt == GEX_EVENT_GROUP) {
      ldone_fn = op->next ? gasnete_cb_ralc_done : gasnete_cb_ialc_done;
      op->initiated_alc_cnt += 1;
    } else if (lc_opt == GEX_EVENT_NOW) {
      ldone_fn = gasnete_cb_iop_lc;
      GASNETE_LC_NOW_START(op);
    } else if (lc_opt == GEX_EVENT_DEFER) {
      // Nothing to do
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to Put_nb");
    }

    pami_event_function rdone_fn = op->next ? gasnete_cb_rput_done : gasnete_cb_iput_done;
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    gasnete_put_common(jobrank, dest, src, nbytes, ldone_fn, rdone_fn, op);
    if (lc_opt == GEX_EVENT_NOW) {
      gasneti_polluntil(GASNETT_PREDICT_TRUE(GASNETE_LC_NOW_DONE(op)));
    }

    return 0;
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/

#if GASNETI_DIRECT_BLOCKING_GET
extern
int gasnete_get(     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  {
    volatile int done = 0;
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    gasnete_get_common(dest, jobrank, src, nbytes, gasnete_cb_int_done, (void*)&done);
    gasneti_polluntil( done );
    return 0;
  }
}
#endif

#if GASNETI_DIRECT_BLOCKING_PUT
extern
int gasnete_put(     gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT_NOLC(tm,rank,dest,src,nbytes);
  {
    volatile int done = 0;
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    gasnete_put_common(jobrank, dest, src, nbytes, NULL, gasnete_cb_int_done, (void*)&done);
    gasneti_polluntil( done );
    return 0;
  }
}   
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
  "par" = PAMI All Reduce
  "pd" = PAMI Dissemination
*/

#if 1
  /* Benchmarks upto 50% better than PAMIALLREDUCE on both BG/Q and PERCS */
  #define GASNETE_BARRIER_DEFAULT "PAMIDISSEM"
#else
  /* Both BG/Q and PERCS show uniformly "good" (not always best, but never worst)
     performance over a wide range of test conditions (nodes and ppn), when using
     the "I0:Binomial:" algorithms which are the current defaults for PAMI_ALLREDUCE.
   */
  #define GASNETE_BARRIER_DEFAULT "PAMIALLREDUCE" 
#endif

/* Forward decls for init functions: */
static void gasnete_parbarrier_init(gasnete_coll_team_t team);
static void gasnete_pdbarrier_init(gasnete_coll_team_t team);

#define GASNETE_BARRIER_READENV() do {                                      \
  if (GASNETE_ISBARRIER("PAMIALLREDUCE"))                                   \
    gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_PAMIALLREDUCE; \
  else if (GASNETE_ISBARRIER("PAMIDISSEM"))                                 \
    gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_PAMIDISSEM;    \
} while (0)

#define GASNETE_BARRIER_INIT(TEAM, TYPE, NODES, SUPERNODES) do { \
    if ((TYPE) == GASNETE_COLL_BARRIER_PAMIALLREDUCE) {          \
      gasnete_parbarrier_init(TEAM);                             \
    } else                                                       \
    if ((TYPE) == GASNETE_COLL_BARRIER_PAMIDISSEM) {             \
      gasnete_pdbarrier_init(TEAM);                              \
    }                                                            \
  } while (0)

/* use reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C

/* PAMI All Reduce ("par") Barrier:
 * Barrier via PAMI-level all-reduce of two 64-bit unsigned integers.
 */

typedef struct {
  /* PAMI portions */
  pami_xfer_t barrier_op;
  pami_xfer_t reduce_op;
  uint64_t sndbuf[2];
  uint64_t rcvbuf[2];
  volatile unsigned int done;    /* counter incremented by PAMI callback */
  /* GASNet portions */
  unsigned int count;            /* how many times we've notify()ed */
} gasnete_parbarrier_t;

static void gasnete_parbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_parbarrier_t *barr = team->barrier_data;
  pami_xfer_t *op = &barr->reduce_op;
  
  gasneti_sync_reads();
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

  ++barr->count;

  if (flags & GASNETE_BARRIERFLAG_UNNAMED) {
    /* Setup for sucessful match in gasnete_parbarrier_finish() */
    barr->rcvbuf[0] = 0;
    barr->rcvbuf[1] = 0xFFFFFFFF;
    op = &barr->barrier_op;
  } else
  if (flags & GASNET_BARRIERFLAG_MISMATCH) {
    /* Larger than any possible "id" AND fails low-word test */
    barr->sndbuf[0] = GASNETI_MAKEWORD(2,0);
    barr->sndbuf[1] = GASNETI_MAKEWORD(2,0);
  } else if (flags & GASNET_BARRIERFLAG_ANONYMOUS) {
    /* Smaller than any possible "id" AND passes low-word test */
    barr->sndbuf[0] = 0;
    barr->sndbuf[1] = 0xFFFFFFFF;
  } else {
    barr->sndbuf[0] = GASNETI_MAKEWORD(1, (uint32_t)id);
    barr->sndbuf[1] = GASNETI_MAKEWORD(1,~(uint32_t)id);
  }

  GASNETC_PAMI_LOCK(gasnetc_context);
  {
    pami_result_t rc = PAMI_Collective(gasnetc_context, op);
    GASNETC_PAMI_CHECK(rc, "initiating collective for barrier");
  }
  GASNETC_PAMI_UNLOCK(gasnetc_context);
  
  gasneti_sync_writes();
}

GASNETI_INLINE(gasnete_parbarrier_finish)
int gasnete_parbarrier_finish(gasnete_coll_team_t team, int id, int flags) {
  gasnete_parbarrier_t *barr = team->barrier_data;

  int retval = (GASNETI_LOWORD(barr->rcvbuf[0]) == ~GASNETI_LOWORD(barr->rcvbuf[1]))
               ? GASNET_OK : GASNET_ERR_BARRIER_MISMATCH;

  if_pf (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && 
         (1  == GASNETI_HIWORD(barr->rcvbuf[0])) &&
         (id != GASNETI_LOWORD(barr->rcvbuf[0]))) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  GASNETE_SPLITSTATE_LEAVE(team);
  gasneti_sync_writes();
  return retval;
}

static int gasnete_parbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_parbarrier_t *barr = team->barrier_data;

  gasneti_sync_reads();
  GASNETE_SPLITSTATE_WAIT(team);

  gasneti_polluntil(barr->done == barr->count);

  return gasnete_parbarrier_finish(team, id, flags);
}

static int gasnete_parbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_parbarrier_t *barr = team->barrier_data;

  gasneti_sync_reads();
  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

  return (barr->done == barr->count) ? gasnete_parbarrier_finish(team, id, flags) : GASNET_ERR_NOT_READY;
}

static int gasnete_parbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads();
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_parbarrier_t * const barr = team->barrier_data;
    *id = GASNETI_LOWORD(barr->rcvbuf[0]);
    return !GASNETI_HIWORD(barr->rcvbuf[0]);
  }
}

static void gasnete_parbarrier_init(gasnete_coll_team_t team) {
  gasnete_parbarrier_t *barr;
  pami_geometry_t geom = PAMI_GEOMETRY_NULL;

  if (team == GASNET_TEAM_ALL)  {
    geom = gasnetc_world_geom; /* team init not completed yet, sigh */
  } else {
  #if GASNET_PAMI_NATIVE_COLL
    geom = team->pami.geom;
  #endif
  }

  if (geom == PAMI_GEOMETRY_NULL) {
    /* Not using native collectives for this team (or not at all) */
    return;
  }

  barr = gasneti_malloc(sizeof(gasnete_parbarrier_t));

  barr->count = barr->done = 0;

  memset(&barr->barrier_op, 0, sizeof(pami_xfer_t));
  gasnetc_dflt_coll_alg(geom, PAMI_XFER_BARRIER, &barr->barrier_op.algorithm);
  barr->barrier_op.cookie = (void *)&barr->done;
  barr->barrier_op.cb_done = &gasnetc_cb_inc_release;
  barr->barrier_op.options.multicontext = PAMI_HINT_DISABLE;

  memset(&barr->reduce_op, 0, sizeof(pami_xfer_t));
  gasnetc_dflt_coll_alg(geom, PAMI_XFER_ALLREDUCE, &barr->reduce_op.algorithm);
  barr->reduce_op.cookie = (void *)&barr->done;
  barr->reduce_op.cb_done = &gasnetc_cb_inc_release;
  barr->reduce_op.options.multicontext = PAMI_HINT_DISABLE;

#if GASNETI_ARCH_BGQ
  /* Yes, this is correct, and reproducably faster! */
  barr->reduce_op.cmd.xfer_allreduce.stype = PAMI_TYPE_DOUBLE;
  barr->reduce_op.cmd.xfer_allreduce.rtype = PAMI_TYPE_DOUBLE;
#elif SIZEOF_LONG == 8
  barr->reduce_op.cmd.xfer_allreduce.stype = PAMI_TYPE_UNSIGNED_LONG;
  barr->reduce_op.cmd.xfer_allreduce.rtype = PAMI_TYPE_UNSIGNED_LONG;
#elif SIZEOF_LONG_LONG == 8
  barr->reduce_op.cmd.xfer_allreduce.stype = PAMI_TYPE_UNSIGNED_LONG_LONG;
  barr->reduce_op.cmd.xfer_allreduce.rtype = PAMI_TYPE_UNSIGNED_LONG_LONG;
#else
  #error "No 8-bytes type?"
#endif
  barr->reduce_op.cmd.xfer_allreduce.sndbuf = (void*)&barr->sndbuf;
  barr->reduce_op.cmd.xfer_allreduce.rcvbuf = (void*)&barr->rcvbuf;
  barr->reduce_op.cmd.xfer_allreduce.stypecount = 2;
  barr->reduce_op.cmd.xfer_allreduce.rtypecount = 2;
  barr->reduce_op.cmd.xfer_allreduce.op = PAMI_DATA_MAX;
  barr->reduce_op.cmd.xfer_allreduce.data_cookie = NULL;
  barr->reduce_op.cmd.xfer_allreduce.commutative = 1;

  team->barrier_notify = &gasnete_parbarrier_notify;
  team->barrier_wait =   &gasnete_parbarrier_wait;
  team->barrier_try =    &gasnete_parbarrier_try;
  team->barrier_result = &gasnete_parbarrier_result;
  team->barrier_pf =     NULL; /* AMPoll is sufficient */

  team->barrier_data = barr;
}


/* PAMI Dissemination ("pd") Barrier:
 * Barrier via PAMI-level implementation of Dissemination
 * Differs from AMDISSEM mainly in ability to do sends from handlers.
 * Also uses the "reduction" formulation of name matching.
 */

typedef struct {
  gex_Rank_t *peer_list;
  gex_Rank_t peer_count;
  volatile int phase;
#if GASNETI_PSHM_BARRIER_HIER
  gasnete_pshmbarrier_data_t *pshm_data; /* non-NULL if using hierarchical code */
  int pshm_shift;                        /* "shift" if this node is not the representative */
#endif
  volatile int prev_value, prev_flags;   /* for gasnet_barrier_result() */
  struct gasnete_pdbarrier_state {
    int value, flags;
    uint64_t arrived;   /* Which messages have arrived (bit map) need 33 bits */
    volatile int next;  /* Which message index are we waiting for */
    int busy;           /* Prevent recursion */
#if GASNETI_PSHM_BARRIER_HIER
    int notify_done;    /* Need to kick pshm until non-zero */
#endif
  } state[2]; /* per-phase */
  pami_xfer_t barrier_op;
} gasnete_pdbarrier_t;

typedef struct {
  uint32_t teamid;
  uint32_t value;
#if ((GASNETE_BARRIERFLAGS_CLIENT_ALL|0xff) == 0xff)
  uint8_t  flags;
#elif ((GASNETE_BARRIERFLAGS_CLIENT_ALL|0xffff) == 0xffff)
  uint16_t flags;
#else
  uint32_t flags;
#endif
  uint8_t  phase : 1;
  uint8_t  index : 6; /* 0 .. 32 */
  uint8_t  end;
} gasnete_pdbarrier_msg_t;
#define GASNETE_PDBARRIER_MSG_T offsetof(gasnete_pdbarrier_msg_t, end)

#if 0 // DISABLED due to (not yet explained) errorneous behavior - see bug 3764
#define GASNETE_PDBARRIER_UNNAMED(_flags,_barr) \
  (((_flags) & GASNETE_BARRIERFLAG_UNNAMED) && (_barr)->barrier_op.cb_done)
#else
#define GASNETE_PDBARRIER_UNNAMED(_flags,_barr) 0
#endif

static pami_send_hint_t gasnete_pdbarrier_send_hint;

static void gasnete_cb_pdbarr_done(pami_context_t context, void *cookie, pami_result_t status) {
  volatile int *next_p = (volatile int *)cookie;
  *next_p = -1;
  gasneti_sync_writes();
  gasneti_assert(status == PAMI_SUCCESS);
}

/* Called only w/ context lock held */
GASNETI_INLINE(gasnete_pdbarr_send)
void gasnete_pdbarr_send(
        gex_Rank_t peer,
        uint32_t teamid,
        int value, int flags,
        int phase, int index)
{
  const gasnete_pdbarrier_msg_t msg = { teamid, value, flags, phase, index };
  pami_send_immediate_t cmd;
  pami_result_t rc;

  cmd.header.iov_base = (char *)&msg;
  cmd.header.iov_len = GASNETE_PDBARRIER_MSG_T;
  cmd.data.iov_base = NULL;
  cmd.data.iov_len = 0;
  cmd.dest = gasnetc_endpoint(peer);
  cmd.dispatch = GASNETC_DISP_DISSEM_BARR;
  cmd.hints = gasnete_pdbarrier_send_hint;

  rc = PAMI_Send_immediate(gasnetc_context, &cmd);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Send_immediate for barrier");
}

/* Called only w/ context lock held */
static void gasnete_pdbarr_advance(
        gasnete_pdbarrier_t *barr,
        uint32_t teamid,
        int msg_value, int msg_flags,
        int msg_phase, int msg_index)
{
  struct gasnete_pdbarrier_state *state = &barr->state[msg_phase];

  /* Merge w/ any previous arrivals (order doesn't matter here).
   * Keep in mind that the context lock is held.
   */
  register int value = state->value;
  register int flags = state->flags;
#if GASNETI_PSHM_BARRIER_HIER
again:
#endif
  if ((flags | msg_flags) & GASNET_BARRIERFLAG_MISMATCH) {
    flags = GASNET_BARRIERFLAG_MISMATCH;
  } else if (flags & GASNET_BARRIERFLAG_ANONYMOUS) {
    flags = msg_flags;
    value = msg_value;
  } else if (!(msg_flags & GASNET_BARRIERFLAG_ANONYMOUS) && (msg_value != value)) {
    flags = GASNET_BARRIERFLAG_MISMATCH;
  }
  state->value = value;
  state->flags = flags;
  state->arrived |= (1 << msg_index);

#if GASNETI_PSHM_BARRIER_HIER
  /* may need to advance local notify */
  if (barr->pshm_data && !state->notify_done) {
    PSHM_BDATA_DECL(pshm_bdata, barr->pshm_data);
    if (gasnete_pshmbarrier_kick(pshm_bdata)) {
      /* "simulate" arrival of the notify */
      state->notify_done = 1;
      msg_value = pshm_bdata->shared->value,
      msg_flags = pshm_bdata->shared->flags,
      msg_index = 0;
      goto again;
    } else {
      return;
    }
  }
#endif

  /* Avoid recursion if run from below gasnete_pdbarr_send() */
  if (state->busy) return;
  else state->busy = 1;

  /* Send any messages */
  { int index = state->next;
    uint64_t distance = 1 << index;

    /* Note: state->{arrived,value,flags} may change within each send */
    while (state->arrived & distance) {
      if (index == barr->peer_count) {
        gasneti_sync_writes();
        index = -1; /* DONE! */
        break;
      } else {
        const gex_Rank_t peer = barr->peer_list[index];
        gasnete_pdbarr_send(peer, teamid, state->value, state->flags, msg_phase, ++index);
        distance <<= 1;
      }
    }

    state->next = index;
  }

  state->busy = 0;
}

static void gasnete_pdbarr_dispatch(
        pami_context_t context, void *cookie,
        const void *head_addr, size_t head_size,
        const void *pipe_addr, size_t pipe_size,
        pami_endpoint_t origin, pami_recv_t *recv)
{
  const gasnete_pdbarrier_msg_t *msg = (gasnete_pdbarrier_msg_t *)head_addr;
  const gasnete_coll_team_t team = gasnete_coll_team_lookup(msg->teamid);
  gasnete_pdbarrier_t *barr = team->barrier_data;
  gasnete_pdbarr_advance(barr, msg->teamid, msg->value, msg->flags, msg->phase, msg->index);
}

#if GASNETI_PSHM_BARRIER_HIER
static int gasnete_pdbarrier_kick_pshm(gasnete_coll_team_t team) {
  gasnete_pdbarrier_t *barr = team->barrier_data;
  int done = barr->state[barr->phase].notify_done;

  if (!done && !GASNETC_PAMI_TRYLOCK(gasnetc_context)) {
    const int phase = barr->phase;
    done = barr->state[phase].notify_done;
    if (!done) {
      PSHM_BDATA_DECL(pshm_bdata, barr->pshm_data);
      if (gasnete_pshmbarrier_kick(pshm_bdata)) {
        done = barr->state[phase].notify_done = 1;
        if (!barr->pshm_shift) {
          gasnete_pdbarr_advance(barr, team->team_id,
                                 pshm_bdata->shared->value,
                                 pshm_bdata->shared->flags,
                                 phase, 0);
        }
      }
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  }

  return done;
}
#endif

static void gasnete_pdbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_pdbarrier_t *barr = team->barrier_data;
  int phase;
  
  gasneti_sync_reads();
  phase = barr->phase ^ 1;
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

  if (GASNETE_PDBARRIER_UNNAMED(flags, barr)) {
    /* UNNAMED barrier - use a non-blocking PAMI barrier on the geometry */
    struct gasnete_pdbarrier_state * const state = &barr->state[phase];
    gasneti_assert(state->flags == GASNET_BARRIERFLAG_ANONYMOUS);
    barr->barrier_op.cookie = (void *)&state->next;
    GASNETC_PAMI_LOCK(gasnetc_context);
    { pami_result_t rc = PAMI_Collective(gasnetc_context, &barr->barrier_op);
      GASNETC_PAMI_CHECK(rc, "initiating collective for barrier");
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  } else {
    int do_send = 1;

#if GASNETI_PSHM_BARRIER_HIER
    if (barr->pshm_data) {
      struct gasnete_pdbarrier_state * const state = &barr->state[phase];
      PSHM_BDATA_DECL(pshm_bdata, barr->pshm_data);
      do_send = 0;
      state->notify_done = gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags);
      if (state->notify_done) {
        id = pshm_bdata->shared->value;
        flags = pshm_bdata->shared->flags;
        do_send = !barr->pshm_shift;
      }
    }
#endif

    if (do_send) {
      GASNETC_PAMI_LOCK(gasnetc_context);
      /* Merge w/ any earlier arrivals and send the notify: */
      gasnete_pdbarr_advance(barr, team->team_id, id, flags, phase, 0);
      GASNETC_PAMI_UNLOCK(gasnetc_context);
    }
  }
  
  barr->phase = phase;
  gasneti_sync_writes();
}

GASNETI_INLINE(gasnete_pdbarrier_finish)
int gasnete_pdbarrier_finish(
      gasnete_coll_team_t team, 
      gasnete_pdbarrier_t *barr,
      struct gasnete_pdbarrier_state *state,
      int id, int flags)
{
  const int final_value = state->value;
  const int final_flags = state->flags;
  int retval = (final_flags & GASNET_BARRIERFLAG_MISMATCH)
               ? GASNET_ERR_BARRIER_MISMATCH : GASNET_OK;

  if_pf(!((flags|final_flags) & GASNET_BARRIERFLAG_ANONYMOUS) && (id != final_value)) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  }
  
  /* Preserve state for gasnet_barrier_result(): */
  barr->prev_value = final_value;
  barr->prev_flags = final_flags;

  /* Reset: */
  state->flags   = GASNET_BARRIERFLAG_ANONYMOUS;
  state->arrived = 0;
  state->next    = 0;
  GASNETE_SPLITSTATE_LEAVE(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barr->pshm_data && !GASNETE_PDBARRIER_UNNAMED(flags, barr)) {
    /* Signal any passive peers w/ the final result */
    const PSHM_BDATA_DECL(pshm_bdata, barr->pshm_data);
    pshm_bdata->shared->value = final_value;
    pshm_bdata->shared->flags = final_flags;
    PSHM_BSTATE_SIGNAL(pshm_bdata, retval, pshm_bdata->private.two_to_phase << 2); /* includes a WMB */
    gasneti_assert(!barr->pshm_shift);
  } else
#endif
  gasneti_sync_writes();

  return retval;
}

static int gasnete_pdbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  struct gasnete_pdbarrier_state *state;
  gasnete_pdbarrier_t *barr = team->barrier_data;
  int retval = GASNET_OK;

  gasneti_sync_reads();
  GASNETE_SPLITSTATE_WAIT(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barr->pshm_data && !GASNETE_PDBARRIER_UNNAMED(flags, barr)) {
    const int passive_shift = barr->pshm_shift;
    gasneti_polluntil(gasnete_pdbarrier_kick_pshm(team));
    retval = gasnete_pshmbarrier_wait_inner(barr->pshm_data, id, flags, passive_shift);
    if (passive_shift) {
      /* Once the active peer signals done, we can return */
      const PSHM_BDATA_DECL(pshm_bdata, barr->pshm_data);
      barr->prev_value = pshm_bdata->shared->value;
      barr->prev_flags = pshm_bdata->shared->flags;
      GASNETE_SPLITSTATE_LEAVE(team);
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      return retval;
    }
  }
#endif

  state = &barr->state[barr->phase];
  gasneti_polluntil(state->next < 0);

  return gasnete_pdbarrier_finish(team, barr, state, id, flags);
}

static int gasnete_pdbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  struct gasnete_pdbarrier_state *state;
  gasnete_pdbarrier_t *barr = team->barrier_data;
  int retval = GASNET_ERR_NOT_READY;

  gasneti_sync_reads();
  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

#if GASNETI_PSHM_BARRIER_HIER
  if (barr->pshm_data && !GASNETE_PDBARRIER_UNNAMED(flags, barr)) {
    const int pshm_shift = barr->pshm_shift;
    if (!gasnete_pdbarrier_kick_pshm(team) ||
        !gasnete_pshmbarrier_try_inner(barr->pshm_data, pshm_shift))
      return GASNET_ERR_NOT_READY;
    if (pshm_shift)
      return gasnete_pdbarrier_wait(team, id, flags);
  }
#endif

  state = &barr->state[barr->phase];

  if (state->next < 0) {
    gasneti_sync_reads();
    retval = gasnete_pdbarrier_finish(team, barr, state, id, flags);
  }
  return retval;
}

static int gasnete_pdbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads();
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_pdbarrier_t * const barr = team->barrier_data;
    *id = barr->prev_value;
    return (GASNET_BARRIERFLAG_ANONYMOUS & barr->prev_flags);
  }
}

static void gasnete_pdbarrier_init(gasnete_coll_team_t team) {
  static int is_init = 0;
  int total_ranks = team->total_ranks;
  int myrank = team->myrank;
  gasnete_coll_peer_list_t *peers = &team->peers;

#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &peers));
#endif

  /* Allocate memory */
  gasnete_pdbarrier_t *barr = gasneti_malloc(sizeof(gasnete_pdbarrier_t));

  /* Try to setup resources for GASNET_BARRIERFLAG_UNNAMED */
  { pami_geometry_t geom = PAMI_GEOMETRY_NULL;
    if (team == GASNET_TEAM_ALL)  {
      geom = gasnetc_world_geom; /* team init not completed yet, sigh */
    } else {
    #if GASNET_PAMI_NATIVE_COLL
      geom = team->pami.geom;
    #endif
    }
    memset(&barr->barrier_op, 0, sizeof(pami_xfer_t));
    if (geom != PAMI_GEOMETRY_NULL) {
      /* Using native collectives for this team */
      gasnetc_dflt_coll_alg(geom, PAMI_XFER_BARRIER, &barr->barrier_op.algorithm);
      barr->barrier_op.cb_done = &gasnete_cb_pdbarr_done;
      barr->barrier_op.options.multicontext = PAMI_HINT_DISABLE;
      /* cookie is set at each call */
    }
  }

#if GASNETI_PSHM_BARRIER_HIER
  barr->pshm_data = pshm_bdata;
  if (pshm_bdata) {
    barr->pshm_shift = pshm_bdata->private.rank ? 2 : 0;
  }
#endif

  /* The peer list */
  barr->peer_list = peers->fwd;
  barr->peer_count = peers->num;

  /* TODO: Anything we should hint here? */
  memset(&gasnete_pdbarrier_send_hint, 0, sizeof(pami_send_hint_t));

  /* Register (only once!) the dispatch that does the "real work": */
  if (!is_init) {
    pami_dispatch_hint_t hints;
    pami_dispatch_callback_function fn;
    pami_result_t rc;

    memset(&hints, 0, sizeof(hints));
    memset(&fn, 0, sizeof(fn));
  
    hints.multicontext = PAMI_HINT_DISABLE;
    hints.recv_contiguous = PAMI_HINT_ENABLE;
    hints.recv_copy = PAMI_HINT_ENABLE;
  
    /* TODO: do we need to support "too big"? */
    hints.long_header = (gasnetc_recv_imm_max >= GASNETE_PDBARRIER_MSG_T)
                        ? PAMI_HINT_DISABLE : PAMI_HINT_ENABLE;
    hints.recv_immediate = (gasnetc_recv_imm_max >= GASNETE_PDBARRIER_MSG_T)
                         ? PAMI_HINT_ENABLE : PAMI_HINT_DEFAULT;
    fn.p2p = &gasnete_pdbarr_dispatch;
    rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_DISSEM_BARR, fn, NULL, hints);
    GASNETC_PAMI_CHECK(rc, "registering GASNETC_DISP_DISSEM_BARR");
    is_init = 1;
  }

  barr->phase = 1;

  barr->state[0].value   = 0; /* not strictly required */
  barr->state[0].flags   = GASNET_BARRIERFLAG_ANONYMOUS;
  barr->state[0].arrived = 0;
  barr->state[0].next    = 0;
  barr->state[0].busy    = 0;

  barr->state[1].value   = 0; /* not strictly required */
  barr->state[1].flags   = GASNET_BARRIERFLAG_ANONYMOUS;
  barr->state[1].arrived = 0;
  barr->state[1].next    = 0;
  barr->state[1].busy    = 0;

  team->barrier_notify = &gasnete_pdbarrier_notify;
  team->barrier_wait =   &gasnete_pdbarrier_wait;
  team->barrier_try =    &gasnete_pdbarrier_try;
  team->barrier_result = &gasnete_pdbarrier_result;
  team->barrier_pf =     NULL; /* AMPoll is sufficient */

  team->barrier_data = barr;

  gasneti_leak(barr);
}

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

