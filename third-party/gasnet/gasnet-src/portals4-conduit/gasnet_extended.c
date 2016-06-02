/*   $Source: bitbucket.org:berkeleylab/gasnet.git/portals4-conduit/gasnet_extended.c $
 * Description: GASNet Extended API Reference Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_portals4.h>

static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

/* ------------------------------------------------------------------------------------ */
/*
  Op management
  =============
*/

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
    SET_OPTYPE((gasnete_op_t *)iop, OPTYPE_IMPLICIT);
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
  #if GASNET_DEBUG || !GASNETE_EOP_COUNTED
    SET_OPSTATE(eop, OPSTATE_INFLIGHT);
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
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nb_bulk
     gasnete_put_nb
     gasnete_put_nb_bulk
     gasnete_memset_nb
*/

gasnet_handle_t
gasnete_get_nb_bulk(void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(UNALIGNED,H);
    {
        gasnete_eop_t *op = _gasnete_eop_new(GASNETE_MYTHREAD);
        gasnetc_rdma_get(dest, node, src, nbytes, OP_TYPE_EOP, (gasnet_handle_t) op);
        return (gasnet_handle_t)op;
    }
}


gasnet_handle_t
gasnete_put_nb(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
    {
        gasnete_eop_t *op = _gasnete_eop_new(GASNETE_MYTHREAD);
        gasnetc_rdma_put(node, dest, src, nbytes, OP_TYPE_EOP, (gasnet_handle_t) op);
        gasnetc_rdma_put_wait((gasnet_handle_t) op);
        return (gasnet_handle_t)op;
    }
}


gasnet_handle_t
gasnete_put_nb_bulk(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
    {
        gasnete_eop_t *op = _gasnete_eop_new(GASNETE_MYTHREAD);
        gasnetc_rdma_put(node, dest, src, nbytes, OP_TYPE_EOP, (gasnet_handle_t) op);
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
*/
/* ------------------------------------------------------------------------------------ */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nbi_bulk
     gasnete_put_nbi
     gasnete_put_nbi_bulk
     gasnete_memset_nbi
*/

void
gasnete_get_nbi_bulk(void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(UNALIGNED,V);
    {
        gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
        gasnete_iop_t *op = mythread->current_iop;
        op->initiated_get_cnt++;
        gasnetc_rdma_get(dest, node, src, nbytes, OP_TYPE_IOP, (gasnet_handle_t) op);
    }
}


void
gasnete_put_nbi(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(ALIGNED,V);
    {
        gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
        gasnete_iop_t *op = mythread->current_iop;
        op->initiated_put_cnt++;
        gasnetc_rdma_put(node, dest, src, nbytes, OP_TYPE_IOP, (gasnet_handle_t) op);
        gasnetc_rdma_put_wait((gasnet_handle_t) op);
    }
}


void
gasnete_put_nbi_bulk(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(ALIGNED,V);
    {
        gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
        gasnete_iop_t *op = mythread->current_iop;
        op->initiated_put_cnt++;
        gasnetc_rdma_put(node, dest, src, nbytes, OP_TYPE_IOP, (gasnet_handle_t) op);
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

