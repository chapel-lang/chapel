/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_extended.c,v $
 *     $Date: 2010/04/17 03:14:27 $
 * $Revision: 1.47.4.3 $
 * Description: GASNet Extended API Reference Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

/* ------------------------------------------------------------------------------------ */
/*
  Op management
  =============
*/
/*  get a new explicit op */
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t * const thread) {
  gasnete_eopaddr_t head = thread->eop_free;
  if_pt (!gasnete_eopaddr_isnil(head)) {
    gasnete_eop_t *eop = GASNETE_EOPADDR_TO_PTR(thread, head);
    thread->eop_free = eop->addr;
    eop->addr = head;
    gasneti_assert(!gasnete_eopaddr_equal(thread->eop_free,head));
    gasneti_assert(eop->threadidx == thread->threadidx);
    gasneti_assert(eop->type == gasnete_opExplicit);
    gasneti_assert(gasnetc_counter_done(&(eop->req_oust)));
    return eop;
  } else { /*  free list empty - need more eops */
    int bufidx = thread->eop_num_bufs;
    gasnete_eop_t *buf;
    int i;
    gasnete_threadidx_t threadidx = thread->threadidx;
    if (bufidx == 256) gasneti_fatalerror("GASNet Extended API: Ran out of explicit handles (limit=65535)");
    thread->eop_num_bufs++;
    buf = (gasnete_eop_t *)gasneti_calloc(256,sizeof(gasnete_eop_t));
    for (i=0; i < 256; i++) {
      gasnete_eopaddr_t addr;
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
      #if 0 /* this can safely be skipped when values are zero */
        buf[i].type = gasnete_opExplicit; 
        gasnetc_atomic_set(&(buf[i].req_oust), 0);
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
    head.bufferidx = bufidx;
    head.eopidx = 0;
    thread->eop_free = head;

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
        gasneti_assert(eop->type == gasnete_opExplicit);               
        gasneti_assert(gasnetc_counter_done(&(eop->req_oust)));
        gasneti_assert(eop->threadidx == threadidx);                  
        gasneti_assert(addr.bufferidx == bufidx);
        gasneti_assert(!seen[addr.eopidx]);/* see if we hit a cycle */
        seen[addr.eopidx] = 1;
        addr = eop->addr;                                     
      }                                                       
      gasneti_assert(gasnete_eopaddr_isnil(addr)); 
    }
    #endif

    return gasnete_eop_new(thread); /*  should succeed this time */
  }
}

gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t * const thread) {
  gasnete_iop_t *iop;
  if_pt (thread->iop_free) {
    iop = thread->iop_free;
    thread->iop_free = iop->next;
    gasneti_memcheck(iop);
    gasneti_assert(iop->type == gasnete_opImplicit);
    gasneti_assert(iop->threadidx == thread->threadidx);
    gasneti_assert(gasnetc_counter_done(&(iop->get_req_oust)));
    gasneti_assert(gasnetc_counter_done(&(iop->put_req_oust)));
  } else {
    iop = (gasnete_iop_t *)gasneti_malloc(sizeof(gasnete_iop_t));
    iop->type = gasnete_opImplicit;
    iop->threadidx = thread->threadidx;
    gasnetc_counter_reset(&(iop->get_req_oust));
    gasnetc_counter_reset(&(iop->put_req_oust));
  }
  iop->next = NULL;
  gasnete_iop_check(iop);
  return iop;
}

GASNETI_INLINE(gasnete_eop_free)
void gasnete_eop_free(gasnete_eop_t *eop) {
  gasnete_threaddata_t * const thread = gasnete_threadtable[eop->threadidx];
  gasnete_eopaddr_t addr = eop->addr;
  gasneti_assert(thread == gasnete_mythread());
  gasnete_eop_check(eop);
  gasneti_assert(gasnetc_counter_done(&(eop->req_oust)));
  eop->addr = thread->eop_free;
  thread->eop_free = addr;
}

GASNETI_INLINE(gasnete_iop_free)
void gasnete_iop_free(gasnete_iop_t *iop) {
  gasnete_threaddata_t * const thread = gasnete_threadtable[iop->threadidx];
  gasneti_assert(thread == gasnete_mythread());
  gasnete_iop_check(iop);
  gasneti_assert(gasnetc_counter_done(&(iop->get_req_oust)));
  gasneti_assert(gasnetc_counter_done(&(iop->put_req_oust)));
  iop->next = thread->iop_free;
  thread->iop_free = iop;
}

/* query an eop for completeness */
GASNETI_INLINE(gasnete_eop_test)
int gasnete_eop_test(gasnete_eop_t *eop) {
  gasnete_eop_check(eop);
  return gasnetc_counter_done(&eop->req_oust);
}

/* query an iop for completeness - this means both puts and gets */
GASNETI_INLINE(gasnete_iop_test)
int gasnete_iop_test(gasnete_iop_t *iop) {
  gasnete_iop_check(iop);
  if (gasnetc_counter_done(&(iop->get_req_oust)) &&
      gasnetc_counter_done(&(iop->put_req_oust))) {
    gasnetc_counter_reset(&(iop->get_req_oust));
    gasnetc_counter_reset(&(iop->put_req_oust));
    return 1;
  }
  return 0;
}

/*  query an op for completeness 
 *  free it if complete
 *  returns 0 or 1 */
GASNETI_INLINE(gasnete_op_try_free)
int gasnete_op_try_free(gasnet_handle_t handle) {
  gasnete_op_t *op = (gasnete_op_t *)handle;

  gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
  if_pt (op->type == gasnete_opExplicit) {
    gasnete_eop_t *eop = (gasnete_eop_t*)op;

    if (gasnete_eop_test(eop)) {
      gasneti_sync_reads();
      gasnete_eop_free(eop);
      return 1;
    }
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t*)op;

    if (gasnete_iop_test(iop)) {
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
  gasnete_op_t *op = (gasnete_op_t *)(*handle_p);

  gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
  if_pt (op->type == gasnete_opExplicit) {
    gasnete_eop_t *eop = (gasnete_eop_t*)op;

    if (gasnete_eop_test(eop)) {
      gasneti_sync_reads();
      gasnete_eop_free(eop);
      *handle_p = GASNET_INVALID_HANDLE;
      return 1;
    }
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t*)op;

    if (gasnete_iop_test(iop)) {
      gasneti_sync_reads();
      gasnete_iop_free(iop);
      *handle_p = GASNET_INVALID_HANDLE;
      return 1;
    }
  }
  return 0;
}

/* Reply handler to complete an op - might be replaced w/ IB atomics one day */
GASNETI_INLINE(gasnete_markdone_reph_inner)
void gasnete_markdone_reph_inner(gasnet_token_t token, void *counter) {
  gasnetc_counter_dec((gasnetc_counter_t *)counter);
}
SHORT_HANDLER(gasnete_markdone_reph,1,2,
              (token, UNPACK(a0)    ),
              (token, UNPACK2(a0, a1)));
#define GASNETE_DONE(token, counter)                                               \
  GASNETI_SAFE(                                                                    \
    SHORT_REP(1,2,((token), gasneti_handleridx(gasnete_markdone_reph), PACK(counter))) \
  )

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

#define GASNETE_IOP_ISDONE(iop) gasnete_iop_test(iop)

#include "gasnet_extended_common.c"

/* ------------------------------------------------------------------------------------ */
/* GASNET-Internal OP Interface */
gasneti_eop_t *gasneti_eop_create(GASNETE_THREAD_FARG_ALONE) {
  gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
  gasnetc_counter_inc(&op->req_oust);
  gasnete_eop_check(op);
  return (gasneti_eop_t *)op;
}
gasneti_iop_t *gasneti_iop_register(unsigned int noperations, int isget GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  gasnete_iop_check(op);
  if (isget) gasnetc_counter_inc_by(&op->get_req_oust,noperations);
  else       gasnetc_counter_inc_by(&op->put_req_oust,noperations);
  gasnete_iop_check(op);
  return (gasneti_iop_t *)op;
}
void gasneti_eop_markdone(gasneti_eop_t *eop) {
  gasnete_eop_t *op = (gasnete_eop_t *)eop;
  gasnete_eop_check(op);
  gasnetc_counter_dec(&op->req_oust);
  gasnete_eop_check(op);
}
void gasneti_iop_markdone(gasneti_iop_t *iop, unsigned int noperations, int isget) {
  gasnete_iop_t *op = (gasnete_iop_t *)iop;
  gasnetc_counter_t * const pctr = (isget ? &(op->get_req_oust) : &(op->put_req_oust));
  gasnete_iop_check(op);
  gasnetc_counter_dec_by(pctr,noperations);
  gasnete_iop_check(op);
}

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config(void) {
  gasneti_check_config_postattach();

  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));
}

extern void gasnete_init(void) {
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

  { gasnete_threaddata_t *threaddata = NULL;
    #if GASNETI_CLIENT_THREADS
      /* register first thread (optimization) */
      threaddata = gasnete_mythread(); 
    #else
      /* register only thread (required) */
      threaddata = gasnete_new_threaddata();
    #endif

    /* cause the first pool of eops to be allocated (optimization) */
    gasnete_eop_free(gasnete_eop_new(threaddata));
  }
   
  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize VIS subsystem */
  gasnete_vis_init();
}

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_memset_reqh_inner)
void gasnete_memset_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t val, void *nbytes_arg, void *dest, void *counter) {
  size_t nbytes = (uintptr_t)nbytes_arg;
  memset(dest, (int)(uint32_t)val, nbytes);
  gasneti_sync_writes();
  GASNETE_DONE(token, counter);
}
SHORT_HANDLER(gasnete_memset_reqh,4,7,
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));
/* ------------------------------------------------------------------------------------ */

extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,H);
 {
  gasnete_eop_t *eop = gasnete_eop_new(GASNETE_MYTHREAD);

  /* XXX check error returns */
  gasnetc_rdma_get(node, src, dest, nbytes, &eop->req_oust);

  return (gasnet_handle_t)eop;
 }
}

extern gasnet_handle_t gasnete_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
 {
  gasnete_eop_t *eop = gasnete_eop_new(GASNETE_MYTHREAD);
  gasnetc_counter_t mem_oust = GASNETC_COUNTER_INITIALIZER;

  /* XXX check error returns */
  gasnetc_rdma_put(node, src, dest, nbytes, &mem_oust, &eop->req_oust);
  gasnetc_counter_wait(&mem_oust, 0);

  return (gasnet_handle_t)eop;
 }
}

extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
 {
  gasnete_eop_t *eop = gasnete_eop_new(GASNETE_MYTHREAD);

  /* XXX check error returns */
  gasnetc_rdma_put(node, src, dest, nbytes, NULL, &eop->req_oust);

  return (gasnet_handle_t)eop;
 }
}

extern gasnet_handle_t gasnete_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_MEMSET(H);
 {
  gasnete_eop_t *eop = gasnete_eop_new(GASNETE_MYTHREAD);

  gasnetc_counter_inc(&eop->req_oust);
  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK(&eop->req_oust))));

  return (gasnet_handle_t)eop;
 }
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
  
  Note that, other than gasnete_wait_syncnb, these routines do not check for INVALID_HANDLE
*/

/* Note that the handle might actually be an IMPLICIT one! */
extern void gasnete_wait_syncnb(gasnet_handle_t op) {
  if_pt (op != GASNET_INVALID_HANDLE) {
    gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
    if_pt (op->type == gasnete_opExplicit) {
      gasnete_eop_t *eop = (gasnete_eop_t*)op;
      gasnetc_counter_wait(&eop->req_oust, 0);
      gasnete_eop_free(eop);
    } else {
      gasnete_iop_t *iop = (gasnete_iop_t*)op;
      gasnetc_counter_wait(&iop->get_req_oust, 0);
      gasnetc_counter_reset(&iop->get_req_oust);
      gasnetc_counter_wait(&iop->put_req_oust, 0);
      gasnetc_counter_reset(&iop->put_req_oust);
      gasnete_iop_free(iop);
    }
  }
}

extern int  gasnete_try_syncnb(gasnet_handle_t handle) {
  GASNETI_SAFE(gasneti_AMPoll());

  return gasnete_op_try_free(handle) ? GASNET_OK : GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles) {
  int success = 0;
  int empty = 1;

  GASNETI_SAFE(gasneti_AMPoll());

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

  GASNETI_SAFE(gasneti_AMPoll());

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

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,V);
 {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;

  /* XXX check error returns */ 
  gasnetc_rdma_get(node, src, dest, nbytes, &iop->get_req_oust);
 }
}

extern void gasnete_put_nbi (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  
  GASNETI_CHECKPSHM_PUT(ALIGNED,V);
 {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;
  gasnetc_counter_t mem_oust = GASNETC_COUNTER_INITIALIZER;

  /* XXX check error returns */ 
  gasnetc_rdma_put(node, src, dest, nbytes, &mem_oust, &iop->put_req_oust);
  gasnetc_counter_wait(&mem_oust, 0);
 }
}

extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
 {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;

  /* XXX check error returns */ 
  gasnetc_rdma_put(node, src, dest, nbytes, NULL, &iop->put_req_oust);
 }
}

extern void gasnete_memset_nbi   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_MEMSET(V);
 {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;

  gasnetc_counter_inc(&iop->put_req_oust);
  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK(&iop->put_req_oust))));
 }
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;
  gasneti_assert(iop->threadidx == mythread->threadidx);
  gasneti_assert(iop->type == gasnete_opImplicit);
  #if GASNET_DEBUG
    if (iop->next != NULL)
      gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_gets() inside an NBI access region");
  #endif

  if (gasnetc_counter_done(&iop->get_req_oust)) {
    gasnetc_counter_reset(&iop->get_req_oust); /* Avoid overflow */
    return GASNET_OK;
  }
  return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;
  gasneti_assert(iop->threadidx == mythread->threadidx);
  gasneti_assert(iop->type == gasnete_opImplicit);
  #if GASNET_DEBUG
    if (iop->next != NULL)
      gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_puts() inside an NBI access region");
  #endif

  if (gasnetc_counter_done(&iop->put_req_oust)) {
    gasnetc_counter_reset(&iop->put_req_oust); /* Avoid overflow */
    return GASNET_OK;
  }
  return GASNET_ERR_NOT_READY;
}

extern void gasnete_wait_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;
  gasneti_assert(iop->threadidx == mythread->threadidx);
  gasneti_assert(iop->type == gasnete_opImplicit);
  #if GASNET_DEBUG
    if (iop->next != NULL)
      gasneti_fatalerror("VIOLATION: attempted to call gasnete_wait_syncnbi_gets() inside an NBI access region");
  #endif

  gasnetc_counter_wait(&iop->get_req_oust, 0);
  gasnetc_counter_reset(&iop->get_req_oust);
}

extern void gasnete_wait_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;
  gasneti_assert(iop->threadidx == mythread->threadidx);
  gasneti_assert(iop->type == gasnete_opImplicit);
  #if GASNET_DEBUG
    if (iop->next != NULL)
      gasneti_fatalerror("VIOLATION: attempted to call gasnete_wait_syncnbi_puts() inside an NBI access region");
  #endif

  gasnetc_counter_wait(&iop->put_req_oust, 0);
  gasnetc_counter_reset(&iop->put_req_oust);
}

/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
extern void            gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = gasnete_iop_new(mythread);
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
  gasnete_iop_t *iop = mythread->current_iop;
  GASNETI_TRACE_EVENT_VAL(S,END_NBI_ACCESSREGION,gasnetc_counter_val(&iop->get_req_oust) + gasnetc_counter_val(&iop->put_req_oust));
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

extern void gasnete_get_bulk (void *dest, gasnet_node_t node, void *src,
			      size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,V);
 {
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;
  gasnetc_rdma_get(node, src, dest, nbytes, &req_oust);
  gasnetc_counter_wait(&req_oust, 0);
 }
}

extern void gasnete_put_bulk (gasnet_node_t node, void* dest, void *src,
			      size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
 {
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;
  gasnetc_rdma_put(node, src, dest, nbytes, NULL, &req_oust);
  gasnetc_counter_wait(&req_oust, 0);
 }
}   

extern void gasnete_memset (gasnet_node_t node, void *dest, int val,
		            size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_MEMSET(V);
 {
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;

  gasnetc_counter_inc(&req_oust);
  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK(&req_oust))));

  gasnetc_counter_wait(&req_oust, 0);
 }
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
  gasneti_handler_tableentry_with_bits(gasnete_markdone_reph),
  gasneti_handler_tableentry_with_bits(gasnete_memset_reqh),

  { 0, NULL }
};

extern gasnet_handlerentry_t const *gasnete_get_handlertable(void) {
  return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

