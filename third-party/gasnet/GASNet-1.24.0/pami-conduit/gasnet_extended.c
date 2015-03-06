/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_extended.c $
 * Description: GASNet Extended API PAMI-conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2012, Lawrence Berkeley National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

static pami_send_hint_t gasnete_null_send_hint;
static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
static pami_send_hint_t gasnete_rdma_send_hint;
static uintptr_t gasnete_mysegbase;
static uintptr_t gasnete_mysegsize;
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Op management
  =============
*/

/*  local completion flag - valid only for ops which block for local-completion */
#define OPFLAG_LC  OPFLAG_CONDUIT0
GASNETI_INLINE(gasnete_op_read_lc)
int gasnete_op_read_lc(gasnete_op_t *op) {
  return (op->flags & OPFLAG_LC);
}
GASNETI_INLINE(gasnete_op_set_lc)
void gasnete_op_set_lc(gasnete_op_t *op) {
  op->flags |= OPFLAG_LC;
}
GASNETI_INLINE(gasnete_op_clr_lc)
void gasnete_op_clr_lc(gasnete_op_t *op) {
  op->flags &= ~OPFLAG_LC;
}

/*  allocate more eops */
GASNETI_NEVER_INLINE(gasnete_eop_alloc,
static void gasnete_eop_alloc(gasnete_threaddata_t * const thread)) {
    gasnete_eopaddr_t addr;
    int bufidx = thread->eop_num_bufs;
    gasnete_eop_t *buf;
    int i;
    gasnete_threadidx_t threadidx = thread->threadidx;
    if (bufidx == 256) gasneti_fatalerror("GASNet Extended API: Ran out of explicit handles (limit=65535)");
    thread->eop_num_bufs++;
    buf = (gasnete_eop_t *)gasneti_calloc(256,sizeof(gasnete_eop_t));
    gasneti_leak(buf);
    for (i=0; i < 256; i++) {
      addr.bufferidx = bufidx;
      #if GASNETE_SCATTER_EOPS_ACROSS_CACHELINES
        #ifdef GASNETE_EOP_MOD
          addr.eopidx = (i+32) % 255;
        #else
          { int k = i+32;
            addr.eopidx = k > 255 ? k - 255 : k;
          }
        #endif
      #else
        addr.eopidx = i+1;
      #endif
      buf[i].threadidx = threadidx;
      buf[i].addr = addr;
      #if 0 /* these can safely be skipped when the values are zero */
        SET_OPSTATE(&(buf[i]),OPSTATE_FREE); 
        SET_OPTYPE(&(buf[i]),OPTYPE_EXPLICIT); 
       #if GASNETE_EOP_COUNTED
        buff[i].initiated_cnt = 0;
       #endif
      #endif
      #if GASNETE_EOP_COUNTED
        gasneti_weakatomic_set(&buf[i].completed_cnt, 0 , 0);
      #endif
    }
     /*  add a list terminator */
    #if GASNETE_SCATTER_EOPS_ACROSS_CACHELINES
      #ifdef GASNETE_EOP_MOD
        buf[223].addr.eopidx = 255; /* modular arithmetic messes up this one */
      #endif
      buf[255].addr = EOPADDR_NIL;
    #else
      buf[255].addr = EOPADDR_NIL;
    #endif
    thread->eop_bufs[bufidx] = buf;
    addr.bufferidx = bufidx;
    addr.eopidx = 0;
    thread->eop_free = addr;

    #if GASNET_DEBUG
    { /* verify new free list got built correctly */
      int i;
      int seen[256];
      gasnete_eopaddr_t addr = thread->eop_free;

      #if 0
      if (gasneti_mynode == 0)
        for (i=0;i<256;i++) {                                   
          fprintf(stderr,"%i:  %i: next=%i\n",gasneti_mynode,i,buf[i].addr.eopidx);
          fflush(stderr);
        }
        sleep(5);
      #endif

      gasneti_memcheck(thread->eop_bufs[bufidx]);
      memset(seen, 0, 256*sizeof(int));
      for (i=0;i<(bufidx==255?255:256);i++) {                                   
        gasnete_eop_t *eop;                                   
        gasneti_assert(!gasnete_eopaddr_isnil(addr));                 
        eop = GASNETE_EOPADDR_TO_PTR(thread,addr);            
        gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);               
        gasneti_assert(OPSTATE(eop) == OPSTATE_FREE);                 
        gasneti_assert(eop->threadidx == threadidx);                  
        gasneti_assert(addr.bufferidx == bufidx);
        gasneti_assert(!seen[addr.eopidx]);/* see if we hit a cycle */
        seen[addr.eopidx] = 1;
        addr = eop->addr;                                     
      }                                                       
      gasneti_assert(gasnete_eopaddr_isnil(addr)); 
    }
    #endif
}

/*  allocate a new iop */
GASNETI_NEVER_INLINE(gasnete_iop_alloc,
static gasnete_iop_t *gasnete_iop_alloc(gasnete_threaddata_t * const thread)) {
    gasnete_iop_t *iop = (gasnete_iop_t *)gasneti_malloc(sizeof(gasnete_iop_t));
    gasneti_leak(iop);
    #if GASNET_DEBUG
      memset(iop, 0, sizeof(gasnete_iop_t)); /* set pad to known value */
    #endif
#if 0
    gasnete_op_clr_lc((gasnete_op_t *)iop));
    SET_OPTYPE((gasnete_op_t *)iop, OPTYPE_IMPLICIT);
#else
    iop->flags = OPTYPE_IMPLICIT;
#endif
    iop->threadidx = thread->threadidx;
    iop->initiated_get_cnt = 0;
    iop->initiated_put_cnt = 0;
    gasneti_weakatomic_set(&(iop->completed_get_cnt), 0, 0);
    gasneti_weakatomic_set(&(iop->completed_put_cnt), 0, 0);
    return iop;
}

/*  get a new op */
static
gasnete_eop_t *_gasnete_eop_new(gasnete_threaddata_t * const thread) {
  gasnete_eopaddr_t head = thread->eop_free;
  if_pf (gasnete_eopaddr_isnil(head)) {
    gasnete_eop_alloc(thread);
    head = thread->eop_free;
  }
  {
    gasnete_eop_t *eop = GASNETE_EOPADDR_TO_PTR(thread, head);
    thread->eop_free = eop->addr;
    eop->addr = head;
    gasneti_assert(!gasnete_eopaddr_equal(thread->eop_free,head));
    gasneti_assert(eop->threadidx == thread->threadidx);
    gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);
    gasneti_assert(OPSTATE(eop) == OPSTATE_FREE);
#if 0
    gasnete_op_clr_lc((gasnete_op_t *)eop));
    SET_OPSTATE(eop, OPSTATE_INFLIGHT);
#else
    eop->flags = OPSTATE_INFLIGHT;
#endif
    return eop;
  }
}

/*  get a new op AND mark it in flight */
GASNETI_INLINE(gasnete_eop_new)
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t * const thread) {
  gasnete_eop_t *eop = _gasnete_eop_new(thread);
#if GASNETE_EOP_COUNTED
  eop->initiated_cnt++;
#endif
  return eop;
}

/*  get a new iop */
static
gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t * const thread) {
  gasnete_iop_t *iop = thread->iop_free;
  if_pt (iop) {
    thread->iop_free = iop->next;
    gasneti_memcheck(iop);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    gasneti_assert(! gasnete_op_read_lc((gasnete_op_t *)iop));
    gasneti_assert(iop->threadidx == thread->threadidx);
    /* If using trace or stats, want meaningful counts when tracing NBI access regions */
    #if GASNETI_STATS_OR_TRACE
      iop->initiated_get_cnt = 0;
      iop->initiated_put_cnt = 0;
      gasneti_weakatomic_set(&(iop->completed_get_cnt), 0, 0);
      gasneti_weakatomic_set(&(iop->completed_put_cnt), 0, 0);
    #endif
  } else {
    iop = gasnete_iop_alloc(thread);
  }
  iop->next = NULL;
  gasnete_iop_check(iop);
  return iop;
}

/*  query an eop for completeness */
static
int gasnete_eop_isdone(gasnete_eop_t *eop) {
  gasneti_assert(eop->threadidx == gasnete_mythread()->threadidx);
  gasnete_eop_check(eop);
  return GASNETE_EOP_DONE(eop);
}

/*  query an iop for completeness - this means both puts and gets */
static
int gasnete_iop_isdone(gasnete_iop_t *iop) {
  gasneti_assert(iop->threadidx == gasnete_mythread()->threadidx);
  gasnete_iop_check(iop);
  return (GASNETE_IOP_CNTDONE(iop,get) && GASNETE_IOP_CNTDONE(iop,put));
}

/*  mark an op done - isget ignored for explicit ops */
static
void gasnete_op_markdone(gasnete_op_t *op, int isget) {
  if (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t *)op;
    gasnete_eop_check(eop);
    GASNETE_EOP_MARKDONE(eop);
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
    if (isget) gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
    else gasneti_weakatomic_increment(&(iop->completed_put_cnt), 0);
  }
}

/* callbacks implementing subsets of gasnete_op_markdone */
static void gasnete_cb_eop_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_eop_t *eop = (gasnete_eop_t *)cookie;
  gasneti_assert(OPSTATE(eop) == OPSTATE_INFLIGHT);
  /* gasnete_eop_check(eop);  XXX: conflicts w/ on-stack EOP used for blocking ops */
  SET_OPSTATE(eop, OPSTATE_COMPLETE);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_iput_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  gasneti_weakatomic_increment(&(iop->completed_put_cnt), 0);
  gasneti_assert(status == PAMI_SUCCESS);
}
static void gasnete_cb_iget_done(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_iop_t *iop = (gasnete_iop_t *)cookie;
  gasnete_iop_check(iop);
  gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
  gasneti_assert(status == PAMI_SUCCESS);
}

/* callback for local completion of a non-bulk put */
static void gasnete_cb_op_lc(pami_context_t context, void *cookie, pami_result_t status) {
  gasnete_op_t *op = (gasnete_op_t *)cookie;
  if (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t *)op;
    /* While rare, the REMOTE completion event might be processed before the LOCAL one.
     * So, OPSTATE_COMPLETE is a valid state here. */
    gasneti_assert((OPSTATE(eop) == OPSTATE_INFLIGHT) ||
                   (OPSTATE(eop) == OPSTATE_COMPLETE));
    gasnete_eop_check(eop);
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
  }
  gasneti_assert(! gasnete_op_read_lc(op));
  gasnete_op_set_lc(op);
  gasneti_assert(status == PAMI_SUCCESS);
}

/*  free an eop */
static
void gasnete_eop_free(gasnete_eop_t *eop) {
  gasnete_threaddata_t * const thread = gasnete_threadtable[eop->threadidx];
  gasnete_eopaddr_t addr = eop->addr;
  gasneti_assert(thread == gasnete_mythread());
  gasnete_eop_check(eop);
  gasneti_assert(GASNETE_EOP_DONE(eop));
#if GASNET_DEBUG
  SET_OPSTATE(eop, OPSTATE_FREE);
#endif
  eop->addr = thread->eop_free;
  thread->eop_free = addr;
}

/*  free an iop */
static
void gasnete_iop_free(gasnete_iop_t *iop) {
  gasnete_threaddata_t * const thread = gasnete_threadtable[iop->threadidx];
  gasneti_assert(thread == gasnete_mythread());
  gasnete_iop_check(iop);
  gasneti_assert(GASNETE_IOP_CNTDONE(iop,get));
  gasneti_assert(GASNETE_IOP_CNTDONE(iop,put));
  gasneti_assert(iop->next == NULL);
  iop->next = thread->iop_free;
  thread->iop_free = iop;
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

  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));
}

extern void gasnete_init(void) {
  GASNETI_UNUSED_UNLESS_DEBUG
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

  { gasnete_threaddata_t *threaddata = NULL;
    gasnete_eop_t *eop = NULL;
    #if GASNETI_MAX_THREADS > 1
      /* register first thread (optimization) */
      threaddata = gasnete_mythread(); 
    #else
      /* register only thread (required) */
      threaddata = gasnete_new_threaddata();
    #endif

    /* cause the first pool of eops to be allocated (optimization) */
    eop = gasnete_eop_new(threaddata);
    GASNETE_EOP_MARKDONE(eop);
    gasnete_eop_free(eop);
  }

  /* Initialize barrier resources */
  gasnete_barrier_init();

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
#endif
}

/* ------------------------------------------------------------------------------------ */
/* GASNET-Internal OP Interface */
gasneti_eop_t *gasneti_eop_create(GASNETE_THREAD_FARG_ALONE) {
  gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
  return (gasneti_eop_t *)op;
}
gasneti_iop_t *gasneti_iop_register(unsigned int noperations, int isget GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  gasnete_iop_check(op);
  if (isget) op->initiated_get_cnt += noperations;
  else       op->initiated_put_cnt += noperations;
  gasnete_iop_check(op);
  return (gasneti_iop_t *)op;
}
void gasneti_eop_markdone(gasneti_eop_t *eop) {
  gasnete_eop_t *op = (gasnete_eop_t *)eop;
  gasnete_eop_check(op);
  GASNETE_EOP_MARKDONE(op);
}
void gasneti_iop_markdone(gasneti_iop_t *iop, unsigned int noperations, int isget) {
  gasnete_iop_t *op = (gasnete_iop_t *)iop;
  gasneti_weakatomic_t * const pctr = (isget ? &(op->completed_get_cnt) : &(op->completed_put_cnt));
  gasnete_iop_check(op);
  if (gasneti_constant_p(noperations) && (noperations == 1))
      gasneti_weakatomic_increment(pctr, 0);
  else {
    #if defined(GASNETI_HAVE_WEAKATOMIC_ADD_SUB)
      gasneti_weakatomic_add(pctr, noperations, 0);
    #else /* yuk */
      while (noperations) {
        gasneti_weakatomic_increment(pctr, 0);
        noperations--;
      }
    #endif
  }
  gasnete_iop_check(op);
}

/* ------------------------------------------------------------------------------------ */
/*
  Get/Put/Memset:
  ===============
*/

/* Use reference implementation of memset in terms of AMs.
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
 * both the handle and the spin-flag from the same pointer.  Rather than
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
void gasnete_put_common(gasnet_node_t node, void *dest, void *src, size_t nbytes,
                        gasnete_op_t *op, int need_lc, int is_eop) {
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  uintptr_t loc_offset = (uintptr_t)src - gasnete_mysegbase;
  uintptr_t rem_offset = (uintptr_t)dest - (uintptr_t)gasneti_seginfo[node].addr;

  if ((loc_offset < gasnete_mysegsize) && GASNETT_PREDICT_TRUE(rem_offset < gasneti_seginfo[node].size)) {
    pami_rput_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(node);
    cmd.rma.hints = gasnete_rdma_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = op;
    cmd.rma.done_fn = need_lc ? gasnete_cb_op_lc : NULL;
    cmd.rdma.local.mr = &gasnetc_mymemreg;
    cmd.rdma.local.offset = loc_offset;
    cmd.rdma.remote.mr = &gasnetc_memreg[node];
    cmd.rdma.remote.offset = rem_offset;
    cmd.put.rdone_fn = is_eop ? gasnete_cb_eop_done : gasnete_cb_iput_done;

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

    if (need_lc) {
      gasneti_polluntil(GASNETT_PREDICT_TRUE(gasnete_op_read_lc((gasnete_op_t *)op)));
    }
  } else
#endif
  {
    pami_put_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(node);
    cmd.rma.hints = gasnete_null_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = op;
    cmd.rma.done_fn = need_lc ? gasnete_cb_op_lc : NULL;
    cmd.addr.local = src;
    cmd.addr.remote = dest;
    cmd.put.rdone_fn = is_eop ? gasnete_cb_eop_done : gasnete_cb_iput_done;

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

    if (need_lc) {
      gasneti_polluntil(GASNETT_PREDICT_TRUE(gasnete_op_read_lc((gasnete_op_t *)op)));
    }
  }
}

/* TODO: use Rget w/ firehose or bounce buffers when only src is in-segment */
GASNETI_INLINE(gasnete_get_common)
void gasnete_get_common(void *dest, gasnet_node_t node, void *src, size_t nbytes,
                        gasnete_op_t *op, int is_eop) {
#if (GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE) && !GASNETI_ARCH_BGQ /* work-around a BG/Q bug */
  uintptr_t loc_offset = (uintptr_t)dest - gasnete_mysegbase;
  uintptr_t rem_offset = (uintptr_t)src - (uintptr_t)gasneti_seginfo[node].addr;

  if ((loc_offset < gasnete_mysegsize) && GASNETT_PREDICT_TRUE(rem_offset < gasneti_seginfo[node].size)) {
    pami_rget_simple_t cmd;

    cmd.rma.dest = gasnetc_endpoint(node);
    cmd.rma.hints = gasnete_rdma_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = op;
    cmd.rma.done_fn = is_eop ? gasnete_cb_eop_done : gasnete_cb_iget_done;
    cmd.rdma.local.mr = &gasnetc_mymemreg;
    cmd.rdma.local.offset = loc_offset;
    cmd.rdma.remote.mr = &gasnetc_memreg[node];
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

    cmd.rma.dest = gasnetc_endpoint(node);
    cmd.rma.hints = gasnete_null_send_hint;
    cmd.rma.bytes = nbytes;
    cmd.rma.cookie = op;
    cmd.rma.done_fn = is_eop ? gasnete_cb_eop_done : gasnete_cb_iget_done;
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
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nb_bulk
     gasnete_put_nb
     gasnete_put_nb_bulk
     gasnete_memset_nb
*/

extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,H);
  {
    gasnete_eop_t * op = gasnete_eop_new(GASNETE_MYTHREAD);
    gasnete_get_common(dest, node, src, nbytes, (gasnete_op_t *)op, 1);
    return (gasnet_handle_t)op;
  }
}

extern gasnet_handle_t gasnete_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(ALIGNED,H);
  {
    gasnete_eop_t * op = gasnete_eop_new(GASNETE_MYTHREAD);
    gasnete_put_common(node, dest, src, nbytes, (gasnete_op_t *)op, 1, 1);
    gasneti_assert(gasnete_op_read_lc((gasnete_op_t *)op));
    return (gasnet_handle_t)op;
  }
}

extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
  {
    gasnete_eop_t * op = gasnete_eop_new(GASNETE_MYTHREAD);
    gasnete_put_common(node, dest, src, nbytes, (gasnete_op_t *)op, 0, 1);
    return (gasnet_handle_t)op;
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

/*  query an op for completeness 
 *  free it if complete
 *  returns 0 or 1 */
GASNETI_INLINE(gasnete_op_try_free)
int gasnete_op_try_free(gasnet_handle_t handle) {
  gasnete_op_t *op = (gasnete_op_t *)handle;

  gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
  if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t*)op;

    if (gasnete_eop_isdone(eop)) {
      gasneti_sync_reads();
      gasnete_eop_free(eop);
      return 1;
    }
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t*)op;

    if (gasnete_iop_isdone(iop)) {
      gasneti_sync_reads();
      gasnete_iop_free(iop);
      return 1;
    }
  }
  return 0;
}

/*  query an op for completeness 
 *  free it and clear the handle if complete
 *  returns 0 or 1 */
GASNETI_INLINE(gasnete_op_try_free_clear)
int gasnete_op_try_free_clear(gasnet_handle_t *handle_p) {
  if (gasnete_op_try_free(*handle_p)) {
    *handle_p = GASNET_INVALID_HANDLE;
    return 1;
  }
  return 0;
}

extern int  gasnete_try_syncnb(gasnet_handle_t handle) {
#if 0
  /* polling now takes place in callers which needed and NOT in those which don't */
  GASNETI_SAFE(gasneti_AMPoll());
#endif

  return gasnete_op_try_free(handle) ? GASNET_OK : GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles) {
  int success = 0;
  int empty = 1;
#if 0
  /* polling for syncnb now happens in header file to avoid duplication */
  GASNETI_SAFE(gasneti_AMPoll());
#endif

  gasneti_assert(phandle);

  { int i;
    for (i = 0; i < numhandles; i++) {
      if (phandle[i] != GASNET_INVALID_HANDLE) {
        empty = 0;
        success |= gasnete_op_try_free_clear(&phandle[i]);
      }
    }
  }

  return (success || empty) ? GASNET_OK : GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_all (gasnet_handle_t *phandle, size_t numhandles) {
  int success = 1;
#if 0
  /* polling for syncnb now happens in header file to avoid duplication */
  GASNETI_SAFE(gasneti_AMPoll());
#endif

  gasneti_assert(phandle);

  { int i;
      for (i = 0; i < numhandles; i++) {
      if (phandle[i] != GASNET_INVALID_HANDLE) {
        success &= gasnete_op_try_free_clear(&phandle[i]);
      }
    }
  }

  return success ? GASNET_OK : GASNET_ERR_NOT_READY;
}

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
  each completion increments a counter - we compare this to the  number of implicit ops launched
  for memset only, the completion is an explicit AM-level ack
*/

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nb_bulk
     gasnete_put_nb
     gasnete_put_nb_bulk
     gasnete_memset_nb
*/

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,V);
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    op->initiated_get_cnt++;
    gasnete_get_common(dest, node, src, nbytes, (gasnete_op_t *)op, 0);
  }
}

extern void gasnete_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(ALIGNED,V);
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    op->initiated_put_cnt++;
    gasnete_put_common(node, dest, src, nbytes, (gasnete_op_t *)op, 1, 0);
    /* reset LC flag for next time: */
    gasneti_assert(gasnete_op_read_lc((gasnete_op_t *)op));
  #if 0
    gasnete_op_clr_lc((gasnete_op_t *)op));
  #else
    op->flags = OPTYPE_IMPLICIT; /* Should be cheaper than r-m-w */
  #endif
  }
}

extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    op->initiated_put_cnt++;
    gasnete_put_common(node, dest, src, nbytes, (gasnete_op_t *)op, 0, 0);
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
  #if 0
    /* polling for syncnbi now happens in header file to avoid duplication */
    GASNETI_SAFE(gasneti_AMPoll());
  #endif
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    gasneti_assert(! gasnete_op_read_lc((gasnete_op_t *)iop));
    #if GASNET_DEBUG
      if (iop->next != NULL)
        gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_gets() inside an NBI access region");
    #endif

    if (GASNETE_IOP_CNTDONE(iop,get)) {
      gasneti_sync_reads();
      return GASNET_OK;
    } else return GASNET_ERR_NOT_READY;
  }
}

extern int  gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
  #if 0
    /* polling for syncnbi now happens in header file to avoid duplication */
    GASNETI_SAFE(gasneti_AMPoll());
  #endif
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(iop->next == NULL);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    gasneti_assert(! gasnete_op_read_lc((gasnete_op_t *)iop));
    #if GASNET_DEBUG
      if (iop->next != NULL)
        gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_puts() inside an NBI access region");
    #endif


    if (GASNETE_IOP_CNTDONE(iop,put)) {
      gasneti_sync_reads();
      return GASNET_OK;
    } else return GASNET_ERR_NOT_READY;
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
/*  This implementation allows recursive access regions, although the spec does not require that */
/*  operations are associated with the most immediately enclosing access region */
extern void            gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = gasnete_iop_new(mythread); /*  push an iop  */
  GASNETI_TRACE_PRINTF(S,("BEGIN_NBI_ACCESSREGION"));
  #if GASNET_DEBUG
    if (!allowrecursion && mythread->current_iop->next != NULL)
      gasneti_fatalerror("VIOLATION: tried to initiate a recursive NBI access region");
  #endif
  iop->next = mythread->current_iop;
  mythread->current_iop = iop;
}

extern gasnet_handle_t gasnete_end_nbi_accessregion(GASNETE_THREAD_FARG_ALONE) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop; /*  pop an iop */
  GASNETI_TRACE_EVENT_VAL(S,END_NBI_ACCESSREGION,iop->initiated_get_cnt + iop->initiated_put_cnt);
  #if GASNET_DEBUG
    if (iop->next == NULL)
      gasneti_fatalerror("VIOLATION: call to gasnete_end_nbi_accessregion() outside access region");
  #endif
  mythread->current_iop = iop->next;
  iop->next = NULL;
  return (gasnet_handle_t)iop;
}

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/

#if GASNETI_DIRECT_GET_BULK
extern void gasnete_get_bulk (void *dest, gasnet_node_t node, void *src,
			      size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,V);
  {
    volatile gasnete_eop_t op = { OPSTATE_INFLIGHT, };
    gasnete_get_common(dest, node, src, nbytes, (gasnete_op_t *)&op, 1);
    gasneti_polluntil(op.flags == OPSTATE_COMPLETE);
  }
}
#endif

#if GASNETI_DIRECT_PUT_BULK
extern void gasnete_put_bulk (gasnet_node_t node, void* dest, void *src,
			      size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
  {
    volatile gasnete_eop_t op = { OPSTATE_INFLIGHT, };
    gasnete_put_common(node, dest, src, nbytes, (gasnete_op_t *)&op, 0, 1);
    gasneti_polluntil(op.flags == OPSTATE_COMPLETE);
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

#define GASNETE_BARRIER_INIT(TEAM, BARRIER_TYPE) do {            \
    if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_PAMIALLREDUCE) {  \
      gasnete_parbarrier_init(TEAM);                             \
    } else                                                       \
    if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_PAMIDISSEM) {     \
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
  gasnet_node_t *peer_list;
  volatile int phase;
#if GASNETI_PSHM_BARRIER_HIER
  gasnete_pshmbarrier_data_t *pshm_data; /* non-NULL if using hierarchical code */
  gasnet_node_t pshm_rep;                /* the "representative" member of my supernode */
  int pshm_shift;                        /* "shift" if this node is not the representative */
#endif
  volatile int prev_value, prev_flags;   /* for gasnet_barrier_result() */
  struct gasnete_pdbarrier_state {
    int value, flags;
    uint64_t arrived;   /* Which messages have arrived (bit map) need 33 bits */
    volatile int next;  /* Which message index are we waiting for */
    int busy;           /* Prevent recursion */
  } state[2]; /* per-phase */
  pami_xfer_t barrier_op;
} gasnete_pdbarrier_t;

typedef struct {
  uint32_t teamid;
  uint32_t value;
#if ((GASNET_BARRIERFLAG_MISMATCH|GASNET_BARRIERFLAG_ANONYMOUS|0xff) == 0xff)
  uint8_t  flags;
#elif ((GASNET_BARRIERFLAG_MISMATCH|GASNET_BARRIERFLAG_ANONYMOUS|0xffff) == 0xffff)
  uint16_t flags;
#else
  uint32_t flags;
#endif
  uint8_t  phase : 1;
  uint8_t  index : 6; /* 0 .. 32 */
  uint8_t  end;
} gasnete_pdbarrier_msg_t;
#define GASNETE_PDBARRIER_MSG_T offsetof(gasnete_pdbarrier_msg_t, end)

static pami_send_hint_t gasnete_pdbarrier_send_hint;

static void gasnete_cb_pdbarr_done(pami_context_t context, void *cookie, pami_result_t status) {
  volatile int *next_p = (volatile int *)cookie;
  *next_p = -1;
  gasneti_sync_writes();
  gasneti_assert(status == PAMI_SUCCESS);
}

/* Called only w/ context lock held */
GASNETI_ALWAYS_INLINE(gasnete_pdbarr_send)
void gasnete_pdbarr_send(
        gasnet_node_t peer,
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

  /* Avoid recursion if run from below gasnete_pdbarr_send() */
  if (state->busy) return;
  else state->busy = 1;

  /* Send any messages */
  { int index = state->next;
    uint64_t distance = 1 << index;

    /* Note: state->{arrived,value,flags} may change within each send */
    while (state->arrived & distance) {
      const gasnet_node_t peer = barr->peer_list[index];

      if (peer == gasneti_nodes) {
        gasneti_sync_writes();
        index = -1; /* DONE! */
        break;
      }

      gasnete_pdbarr_send(peer, teamid, state->value, state->flags, msg_phase, ++index);

      distance <<= 1;
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

static void gasnete_pdbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_pdbarrier_t *barr = team->barrier_data;
  const int is_unnamed = ((flags & GASNETE_BARRIERFLAG_UNNAMED) && barr->barrier_op.cb_done);
  int do_send = 1;
  int phase;
  
  gasneti_sync_reads();
  phase = barr->phase ^ 1;
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barr->pshm_data && !is_unnamed) {
    PSHM_BDATA_DECL(pshm_bdata, barr->pshm_data);
    (void)gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags);
    do_send = !barr->pshm_shift;
    id = pshm_bdata->shared->value;
    flags = pshm_bdata->shared->flags;
  }
#endif

  if (do_send) {
    GASNETC_PAMI_LOCK(gasnetc_context);
    if (is_unnamed) {
      /* launch a non-blocking barrier on the geometry */
      struct gasnete_pdbarrier_state * const state = &barr->state[phase];
      pami_result_t rc;
      gasneti_assert(state->flags == GASNET_BARRIERFLAG_ANONYMOUS);
      barr->barrier_op.cookie = (void *)&state->next;
      rc = PAMI_Collective(gasnetc_context, &barr->barrier_op);
      GASNETC_PAMI_CHECK(rc, "initiating collective for barrier");
    } else {
      /* Merge w/ any earlier arrivals and send the notify: */
      gasnete_pdbarr_advance(barr, team->team_id, id, flags, phase, 0);
    }
    GASNETC_PAMI_UNLOCK(gasnetc_context);
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
  if (barr->pshm_data && !((flags & GASNETE_BARRIERFLAG_UNNAMED) && barr->barrier_op.cb_done)) {
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
  if (barr->pshm_data && !((flags & GASNETE_BARRIERFLAG_UNNAMED) && barr->barrier_op.cb_done)) {
    const int passive_shift = barr->pshm_shift;
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
  if (barr->pshm_data && !((flags & GASNETE_BARRIERFLAG_UNNAMED) && barr->barrier_op.cb_done)) {
    const int pshm_shift = barr->pshm_shift;
    if (!gasnete_pshmbarrier_try_inner(barr->pshm_data, pshm_shift))
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
#if GASNETI_PSHM_BARRIER_HIER
  gasnet_node_t *supernode_reps = NULL;
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &supernode_reps));
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
  if (pshm_bdata) {
    barr->pshm_data = pshm_bdata;
    barr->pshm_shift = pshm_bdata->private.rank ? 2 : 0;
    if (supernode_reps) {
      barr->pshm_rep = supernode_reps[myrank];
    } else {
      barr->pshm_rep = gasneti_pshm_firsts[myrank];
    }
  }
#endif

  /* Build the peer list */
  { int step, steps;
    uint64_t distance;
#if GASNETI_PSHM_BARRIER_HIER
    gasnet_node_t *nodes = supernode_reps ? supernode_reps : gasneti_pshm_firsts;
#endif

    for (steps=0, distance=1; distance < total_ranks; ++steps, distance*=2) { /* empty */ }

    barr->peer_list = gasneti_calloc(steps+1, sizeof(gasnet_node_t));
    gasneti_leak(barr->peer_list);

    for (step=0, distance=1; step < steps; ++step, distance*=2) {
      gasnet_node_t peer = (myrank < distance)
                              ? (myrank + (total_ranks - distance))
                              : (myrank - distance);
#if GASNETI_PSHM_BARRIER_HIER
      if (pshm_bdata) {
        barr->peer_list[step] = nodes[peer];
      } else
#endif
      barr->peer_list[step] = GASNETE_COLL_REL2ACT(team, peer);
    }
    barr->peer_list[steps] = gasneti_nodes; /* sentinel */
  }

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
#include "gasnet_extended_refvis.h"

/* ------------------------------------------------------------------------------------ */
/*
  Collectives:
  ============
*/

/* use reference implementation of collectives */
#include "gasnet_extended_refcoll.h"

/* ------------------------------------------------------------------------------------ */
/*
  Handlers:
  =========
*/
static gasnet_handlerentry_t const gasnete_handlers[] = {
  #ifdef GASNETE_REFBARRIER_HANDLERS
    GASNETE_REFBARRIER_HANDLERS(),
  #endif
  #ifdef GASNETE_REFVIS_HANDLERS
    GASNETE_REFVIS_HANDLERS()
  #endif
  #ifdef GASNETE_REFCOLL_HANDLERS
    GASNETE_REFCOLL_HANDLERS()
  #endif

  /* ptr-width independent handlers */

  /* ptr-width dependent handlers */
#if GASNETE_BUILD_AMREF_GET_HANDLERS
  gasneti_handler_tableentry_with_bits(gasnete_amref_get_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_amref_get_reph),
  gasneti_handler_tableentry_with_bits(gasnete_amref_getlong_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_amref_getlong_reph),
#endif
#if GASNETE_BUILD_AMREF_PUT_HANDLERS
  gasneti_handler_tableentry_with_bits(gasnete_amref_put_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_amref_putlong_reqh),
#endif
#if GASNETE_BUILD_AMREF_MEMSET_HANDLERS
  gasneti_handler_tableentry_with_bits(gasnete_amref_memset_reqh),
#endif
#if GASNETE_BUILD_AMREF_PUT_HANDLERS || GASNETE_BUILD_AMREF_MEMSET_HANDLERS
  gasneti_handler_tableentry_with_bits(gasnete_amref_markdone_reph),
#endif

  { 0, NULL }
};

extern gasnet_handlerentry_t const *gasnete_get_handlertable(void) {
  return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

