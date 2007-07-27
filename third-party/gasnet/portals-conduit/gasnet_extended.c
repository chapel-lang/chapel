/*   $Source$
 *     $Date$
 * $Revision$
 * Description: GASNet Extended API Reference Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
#include <gasnet_portals.h>

GASNETI_IDENT(gasnete_IdentString_Version, "$GASNetExtendedLibraryVersion: " GASNET_EXTENDED_VERSION_STR " $");
GASNETI_IDENT(gasnete_IdentString_ExtendedName, "$GASNetExtendedLibraryName: " GASNET_EXTENDED_NAME_STR " $");

gasnete_threaddata_t *gasnete_threadtable[GASNETI_MAX_THREADS] = { 0 };
static int gasnete_numthreads = 0;
static gasnet_hsl_t threadtable_lock = GASNET_HSL_INITIALIZER;
#if GASNETI_CLIENT_THREADS
  /* pthread thread-specific ptr to our threaddata (or NULL for a thread never-seen before) */
  static gasneti_threadkey_t gasnete_threaddata = GASNETI_THREADKEY_INITIALIZER;
#endif
const gasnete_opaddr_t gasnete_opaddr_nil = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }


#if 0
GASNETI_INLINE(gasnete_opaddr_to_ptr)
gasnete_op_t *gasnete_opaddr_to_ptr(gasnete_threadidx_t threadid, gasnete_opaddr_t opaddr)
{
  gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
  return ((threadid & OPTYPE_IMPLICIT)  == OPTYPE_IMPLICIT
	  ? (gasnete_op_t*)(GASNETE_IOPADDR_TO_PTR(th,opaddr))
	  : (gasnete_op_t*)(GASNETE_EOPADDR_TO_PTR(th,opaddr))
	  );
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Tuning Parameters
  =================
  Conduits may choose to override the default tuning parameters below by defining them
  in their gasnet_core_fwd.h
*/

/* the size threshold where gets/puts stop using medium messages and start using longs */
#ifndef GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD   gasnet_AMMaxMedium()
#endif

/* true if we should try to use Long replies in gets (only possible if dest falls in segment) */
#ifndef GASNETE_USE_LONG_GETS
#define GASNETE_USE_LONG_GETS 1
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Thread Management
  =================
*/
static gasnete_threaddata_t * gasnete_new_threaddata() {
  gasnete_threaddata_t *threaddata = NULL;
  int idx;
  gasnet_hsl_lock(&threadtable_lock);
  idx = gasnete_numthreads;
  gasnete_numthreads++;
  gasnet_hsl_unlock(&threadtable_lock);
  gasneti_assert(GASNETI_MAX_THREADS <= 256);
  #if GASNETI_CLIENT_THREADS
    if (idx >= GASNETI_MAX_THREADS) 
      gasneti_fatalerror("GASNet Extended API: Too many local client threads (limit=%i)",GASNETI_MAX_THREADS);
  #else
    gasneti_assert(idx == 0);
  #endif
  gasneti_assert(gasnete_threadtable[idx] == NULL);

  threaddata = (gasnete_threaddata_t *)gasneti_calloc(1,sizeof(gasnete_threaddata_t));
  GASNETI_TRACE_PRINTF(C,("gasnete_new_threaddata: idx=%i, numthreads=%i, threaddata=0x%lx",idx,gasnete_numthreads,(uintptr_t)threaddata));

  threaddata->threadidx = idx;
  threaddata->eop_free = gasnete_opaddr_nil;
#if GASNETI_STATS_OR_TRACE
  threaddata->eop_inuse = 0;
  threaddata->eop_hwm = 0;
#endif

  threaddata->iop_free = NULL;
  threaddata->current_iop = NULL;

  gasneti_weakatomic_set(&(threaddata->local_completion_count), 0, 0);
  gasnete_threadtable[idx] = threaddata;

  threaddata->current_iop = gasnete_iop_new(threaddata);

  return threaddata;
}
/* PURE function (returns same value for a given thread every time) 
*/
#if GASNETI_CLIENT_THREADS
  extern gasnete_threaddata_t *gasnete_mythread() {
    gasnete_threaddata_t *threaddata = gasneti_threadkey_get(gasnete_threaddata);
    GASNETI_TRACE_EVENT(C, DYNAMIC_THREADLOOKUP);
    if_pt (threaddata) {
      gasneti_memcheck(threaddata);
      return threaddata;
    }

    /* first time we've seen this thread - need to set it up */
    threaddata = gasnete_new_threaddata();
    gasneti_threadkey_set(gasnete_threaddata, threaddata);
    return threaddata;
  }
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config() {
  gasneti_check_config_postattach();

  gasneti_assert_always(GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD <= gasnet_AMMaxMedium());
  gasneti_assert_always(gasnete_opaddr_isnil(gasnete_opaddr_nil));
}


/* ------------------------------------------------------------------------------------ */
/*
  Op management
  =============
*/
/*  get a new op and mark it in flight */
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t * const thread) {
  gasnete_opaddr_t head = thread->eop_free;
  if_pt (!gasnete_opaddr_isnil(head)) {
    gasnete_eop_t *eop = GASNETE_EOPADDR_TO_PTR(thread, head);
#if GASNETI_STATS_OR_TRACE
    thread->eop_inuse++;
    if (thread->eop_inuse > thread->eop_hwm) thread->eop_hwm = thread->eop_inuse;
    GASNETI_TRACE_PRINTF(C,("EOP_NEW: inuse = %d, hwm = %d eop = 0x%lx",thread->eop_inuse,thread->eop_hwm,(uintptr_t)eop));
#endif
    GASNETI_TRACE_EVENT(C,EOP_ALLOC);
    thread->eop_free = eop->addr;   /* next eop in freelist */
    eop->addr = head;               /* my opaddr_t          */
    gasneti_assert(!gasnete_opaddr_equal(thread->eop_free,head));
    gasneti_assert(GASNETE_OP_THREADID(eop) == thread->threadidx);
    gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);
    gasneti_assert(OPTYPE(eop) == OPSTATE_FREE);
    SET_OPSTATE((gasnete_op_t*)eop, OPSTATE_INFLIGHT);
    return eop;
  } else { /*  free list empty - need more eops */
    int bufidx = thread->eop_num_bufs;
    gasnete_eop_t *buf;
    int i;
    gasnete_threadidx_t threadidx = thread->threadidx;

    if (bufidx == 256) gasneti_fatalerror("GASNet Extended API: Ran out of explicit handles (limit=65535)");
    thread->eop_num_bufs++;
    buf = (gasnete_eop_t *)gasneti_calloc(256,sizeof(gasnete_eop_t));
    GASNETI_TRACE_EVENT(C, EOP_BUCKETS);
    for (i=0; i < 256; i++) {
      gasnete_opaddr_t addr;
      addr.bufferidx = bufidx;
      #if GASNETE_SCATTER_EOPS_ACROSS_CACHELINES
        #ifdef GASNETE_EOP_MOD
          addr.opidx = (i+32) % 255;
        #else
          { int k = i+32;
            addr.opidx = k > 255 ? k - 255 : k;
          }
        #endif
      #else
        /* Remember... addr points to next elem when on free list */
        addr.opidx = i+1;
      #endif
      buf[i].threadidx = threadidx;
      buf[i].addr = addr;
      #if 0 /* these can safely be skipped when the values are zero */
      SET_OPSTATE((gasnete_op_t*)&(buf[i]),OPSTATE_FREE); 
      SET_OPTYPE(gasnete_op_t*)&(buf[i]),OPTYPE_EXPLICIT); 
      #endif
    }
     /*  add a list terminator */
    #if GASNETE_SCATTER_EOPS_ACROSS_CACHELINES
      #ifdef GASNETE_EOP_MOD
        buf[223].addr.opidx = 255; /* modular arithmetic messes up this one */
      #endif
      buf[255].addr = gasnete_opaddr_nil;
    #else
      buf[255].addr = gasnete_opaddr_nil;
    #endif
    thread->eop_bufs[bufidx] = buf;
    head.bufferidx = bufidx;
    head.opidx = 0;
    thread->eop_free = head;

    #if GASNET_DEBUG
    { /* verify new free list got built correctly */
      int i;
      int seen[256];
      gasnete_opaddr_t addr = thread->eop_free;

      #if 0
      if (gasneti_mynode == 0)
        for (i=0;i<256;i++) {                                   
          fprintf(stderr,"%i:  %i: next=%i\n",gasneti_mynode,i,buf[i].addr.opidx);
          fflush(stderr);
        }
        sleep(5);
      #endif

      gasneti_memcheck(thread->eop_bufs[bufidx]);
      memset(seen, 0, 256*sizeof(int));
      for (i=0;i<(bufidx==255?255:256);i++) {                                   
        gasnete_eop_t *eop;                                   
        gasneti_assert(!gasnete_opaddr_isnil(addr));                 
        eop = GASNETE_EOPADDR_TO_PTR(thread,addr);            
        gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);               
        gasneti_assert(OPSTATE(eop) == OPSTATE_FREE);                 
        gasneti_assert(GASNETE_OP_THREADID(eop) == threadidx);                  
        gasneti_assert(addr.bufferidx == bufidx);
        gasneti_assert(!seen[addr.opidx]);/* see if we hit a cycle */
        seen[addr.opidx] = 1;
        addr = eop->addr;                                     
      }                                                       
      gasneti_assert(gasnete_opaddr_isnil(addr)); 
    }
    #endif

    return gasnete_eop_new(thread); /*  should succeed this time */
  }
}

gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t * const thread) {
  if_pt (thread->iop_free) {
    gasnete_iop_t *iop = thread->iop_free;

    GASNETI_TRACE_PRINTF(C,("gasnete_iop: iop = 0x%lx",(uintptr_t)iop));

    thread->iop_free = iop->next;    
    iop->next = NULL;                
    gasneti_assert(GASNETE_OP_THREADID(iop) == thread->threadidx);
    gasneti_assert(iop == GASNETE_IOPADDR_TO_PTR(thread,iop->addr));
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    gasneti_assert(OPSTATE(iop) == OPSTATE_FREE);
    SET_OPSTATE((gasnete_op_t*)iop, OPSTATE_INFLIGHT);
    iop->initiated_get_cnt = 0;
    iop->initiated_put_cnt = 0;
    gasneti_weakatomic_set(&(iop->completed_get_cnt), 0, 0);
    gasneti_weakatomic_set(&(iop->completed_put_cnt), 0, 0);
    gasnete_iop_check(iop);
    return iop;
  } else { /*  free list empty - need more iops */
    int bufidx = thread->iop_num_bufs;
    gasnete_iop_t *buf;
    int i;
    gasnete_threadidx_t threadidx = thread->threadidx;

    GASNETI_TRACE_PRINTF(C,("gasnete_iop_new: Allocating 256 more IOPs"));

    if (bufidx == 256) gasneti_fatalerror("GASNet Extended API: Ran out of implicit handles (limit=65535)");
    thread->iop_num_bufs++;
    buf = (gasnete_iop_t *)gasneti_calloc(256,sizeof(gasnete_iop_t));
    thread->iop_bufs[bufidx] = buf;
    for (i=0; i < 256; i++) {
      gasnete_opaddr_t addr;
      addr.bufferidx = bufidx;
      /* Differing from eops, iops addr always point to themselves */
      addr.opidx = i;
      buf[i].threadidx = threadidx;
      buf[i].addr = addr;
      buf[i].next = thread->iop_free;
      thread->iop_free = &buf[i];
      SET_OPSTATE((gasnete_op_t*)&(buf[i]),OPSTATE_FREE); 
      SET_OPTYPE((gasnete_op_t*)&(buf[i]),OPTYPE_IMPLICIT); 
    }

    #if GASNET_DEBUG
    { /* verify new free list got built correctly */
      gasnete_iop_t *p = thread->iop_free;
      int count = 0;
      gasneti_memcheck(thread->iop_bufs[bufidx]);
      while (p) {
	gasnete_threadidx_t p_th = GASNETE_OP_THREADID(p);
	gasnete_iop_t *me;
	gasneti_assert(threadidx == GASNETE_THREADID(p_th));
	me = GASNETE_IOPADDR_TO_PTR(thread,p->addr);
	gasneti_assert(p == me);
        gasneti_assert(OPTYPE(p) == OPTYPE_IMPLICIT);               
        gasneti_assert(OPSTATE(p) == OPSTATE_FREE);                 
        gasneti_assert(p->addr.bufferidx == bufidx);
	p = p->next;
	count++;
      }
      gasneti_assert(count == 256);
    }
    #endif

    return gasnete_iop_new(thread); /*  should succeed this time */
  }
}

/*  query an op for completeness - for iop this means both puts and gets */
int gasnete_op_isdone(gasnete_op_t *op) {
    gasneti_assert(GASNETE_OP_THREADID(op) == gasnete_mythread()->threadidx);
  if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
    gasnete_eop_check((gasnete_eop_t *)op);
    return OPSTATE(op) == OPSTATE_COMPLETE;
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t*)op;
    gasnete_iop_check(iop);
    return (gasneti_weakatomic_read(&(iop->completed_get_cnt), 0) == iop->initiated_get_cnt) &&
           (gasneti_weakatomic_read(&(iop->completed_put_cnt), 0) == iop->initiated_put_cnt);
  }
}

/*  mark an op done - isget ignored for explicit ops */
void gasnete_op_markdone(gasnete_op_t *op, int isget) {
  if (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t *)op;
    gasneti_assert(OPSTATE(eop) == OPSTATE_INFLIGHT);
    gasnete_eop_check(eop);
    SET_OPSTATE((gasnete_op_t*)eop, OPSTATE_COMPLETE);
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
    if (isget) gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
    else gasneti_weakatomic_increment(&(iop->completed_put_cnt), 0);
  }
}

/*  free an op */
void gasnete_op_free(gasnete_op_t *op) {
  gasnete_threaddata_t * const thread = gasnete_threadtable[GASNETE_OP_THREADID(op)];
  gasneti_assert(thread == gasnete_mythread());
  if (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t *)op;
    gasnete_opaddr_t addr = eop->addr;
    gasneti_assert(OPSTATE(eop) == OPSTATE_COMPLETE);
    gasnete_eop_check(eop);
    SET_OPSTATE((gasnete_op_t*)eop, OPSTATE_FREE);
    eop->addr = thread->eop_free;
    thread->eop_free = addr;
#if GASNETI_STATS_OR_TRACE
    thread->eop_inuse--;
    GASNETI_TRACE_PRINTF(C,("EOP_FREE: inuse = %d, eop = 0x%lx",thread->eop_inuse,(uintptr_t)op));
    GASNETI_TRACE_EVENT(C,EOP_FREE);
#endif
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
    iop->next = thread->iop_free;
    thread->iop_free = iop;
    SET_OPSTATE((gasnete_op_t*)iop, OPSTATE_FREE);
    GASNETI_TRACE_PRINTF(C,("IOP_FREE: iop = 0x%lx",(uintptr_t)op));
  }
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
  gasnete_op_markdone((gasnete_op_t *)eop, 0);
}
void gasneti_iop_markdone(gasneti_iop_t *iop, unsigned int noperations, int isget) {
  gasnete_iop_t *op = (gasnete_iop_t *)iop;
  gasneti_weakatomic_t * const pctr = (isget ? &(op->completed_get_cnt) : &(op->completed_put_cnt));
  gasnete_iop_check(op);
  if (noperations == 1) gasneti_weakatomic_increment(pctr, 0);
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

extern void gasnete_init() {
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

#if 0
  GASNETI_TRACE_PRINTF(C,("Sizeof(int) = %i",(int)sizeof(int)));
  GASNETI_TRACE_PRINTF(C,("Sizeof(long) = %i",(int)sizeof(long)));
  GASNETI_TRACE_PRINTF(C,("Sizeof(long long) = %i",(int)sizeof(long long)));
  GASNETI_TRACE_PRINTF(C,("Sizeof(void*) = %i",(int)sizeof(void*)));
  GASNETI_TRACE_PRINTF(C,("Sizeof(ptl_match_bits_t) = %i",(int)sizeof(ptl_match_bits_t)));
  GASNETI_TRACE_PRINTF(C,("Sizeof(gasnete_opaddr_t) = %i",(int)sizeof(gasnete_opaddr_t)));
#endif

  { gasnete_threaddata_t *threaddata = NULL;
    gasnete_eop_t *eop = NULL;
    gasnete_iop_t *iop = NULL;
    #if GASNETI_CLIENT_THREADS
      /* register first thread (optimization) */
      threaddata = gasnete_mythread(); 
    #else
      /* register only thread (required) */
      threaddata = gasnete_new_threaddata();
    #endif

    /* cause the first pool of eops and iops to be allocated (optimization) */
    eop = gasnete_eop_new(threaddata);
    gasnete_op_markdone((gasnete_op_t *)eop, 0);
    gasnete_op_free((gasnete_op_t *)eop);

    /* MLW: add for iops as well */
    iop = gasnete_iop_new(threaddata);
    gasnete_op_free((gasnete_op_t *)iop);
  }

  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize VIS subsystem */
  gasnete_vis_init();
}

/* This is called by gasnetc_exit for the purposes of cleanup-up
 * resources used exclusively by the extended API implementation.
 */
void gasnete_exit(int exitcode)
{
  GASNETI_TRACE_PRINTF(C,("gasnete_exit"));
}

/* 
 * used to print final conduit-extended specific stats
 */
void gasnete_trace_finish(void)
{
  int i;
  for (i=0; i < gasnete_numthreads; i++) {
    GASNETI_STATS_PRINTF(C,("Thread %i, EOP HWM = %i\n",i,gasnete_threadtable[i]->eop_hwm));
  }
}





/* ------------------------------------------------------------------------------------ */
/*
 * Design/Approach for gets/puts in Extended Reference API in terms of Core
 * ========================================================================
 *
 * The extended API implements gasnet_put and gasnet_put_nbi differently, 
 * all in terms of 'nbytes', the number of bytes to be transferred as 
 * payload.
 *
 * The core usually implements AMSmall and AMMedium as host-side copies and
 * AMLongs are implemented according to the implementation.  Some conduits 
 * may optimize AMLongRequest/AMLongRequestAsync/AMLongReply with DMA
 * operations.
 *
 * gasnet_put(_bulk) is translated to a gasnete_put_nb(_bulk) + sync
 * gasnet_get(_bulk) is translated to a gasnete_get_nb(_bulk) + sync
 *
 * gasnete_put_nb(_bulk) translates to
 *    if nbytes < GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMMedium(payload)
 *    else if nbytes < AMMaxLongRequest
 *      AMLongRequest(payload)
 *    else
 *      gasnete_put_nbi(_bulk)(payload)
 *
 * gasnete_get_nb(_bulk) translates to
 *    if nbytes < GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMSmall request + AMMedium(payload) reply
 *    else
 *      gasnete_get_nbi(_bulk)()
 *
 * gasnete_put_nbi(_bulk) translates to
 *    if nbytes < GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMMedium(payload)
 *    else if nbytes < AMMaxLongRequest
 *      AMLongRequest(payload)
 *    else
 *      chunks of AMMaxLongRequest with AMLongRequest()
 *      AMLongRequestAsync is used instead of AMLongRequest for put_bulk
 *
 * gasnete_get_nbi(_bulk) translates to
 *    if nbytes < GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMSmall request + AMMedium(payload) reply
 *    else
 *      chunks of AMMaxMedium with AMSmall request + AMMedium() reply
 *
 * The current implementation uses AMLongs for large puts because the 
 * destination is guaranteed to fall within the registered GASNet segment.
 * The spec allows gets to be received anywhere into the virtual memory space,
 * so we can only use AMLong when the destination happens to fall within the 
 * segment - GASNETE_USE_LONG_GETS indicates whether or not we should try to do this.
 * (conduits which can support AMLongs to areas outside the segment
 * could improve on this through the use of this conduit-specific information).
 * 
 */

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_get_reqh_inner)
void gasnete_get_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t nbytes, void *dest, void *src, void *op) {
  gasneti_assert(nbytes <= gasnet_AMMaxMedium());
  GASNETI_SAFE(
    MEDIUM_REP(2,4,(token, gasneti_handleridx(gasnete_get_reph),
                  src, nbytes, 
                  PACK(dest), PACK(op))));
}
SHORT_HANDLER(gasnete_get_reqh,4,7, 
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_get_reph_inner)
void gasnete_get_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *dest, void *op) {
  GASNETE_FAST_UNALIGNED_MEMCPY(dest, addr, nbytes);
  gasneti_sync_writes();
  gasnete_op_markdone((gasnete_op_t *)op, 1);
}
MEDIUM_HANDLER(gasnete_get_reph,2,4,
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1)    ),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_getlong_reqh_inner)
void gasnete_getlong_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t nbytes, void *dest, void *src, void *op) {

  GASNETI_SAFE(
    LONG_REP(1,2,(token, gasneti_handleridx(gasnete_getlong_reph),
                  src, nbytes, dest,
                  PACK(op))));
}
SHORT_HANDLER(gasnete_getlong_reqh,4,7, 
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_getlong_reph_inner)
void gasnete_getlong_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes, 
  void *op) {
  gasneti_sync_writes();
  gasnete_op_markdone((gasnete_op_t *)op, 1);
}
LONG_HANDLER(gasnete_getlong_reph,1,2,
              (token,addr,nbytes, UNPACK(a0)     ),
              (token,addr,nbytes, UNPACK2(a0, a1)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_put_reqh_inner)
void gasnete_put_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *dest, void *op) {
  GASNETE_FAST_UNALIGNED_MEMCPY(dest, addr, nbytes);
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_markdone_reph),
                  PACK(op))));
}
MEDIUM_HANDLER(gasnete_put_reqh,2,4, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1)     ),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_putlong_reqh_inner)
void gasnete_putlong_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *op) {
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_markdone_reph),
                  PACK(op))));
}
LONG_HANDLER(gasnete_putlong_reqh,1,2, 
              (token,addr,nbytes, UNPACK(a0)     ),
              (token,addr,nbytes, UNPACK2(a0, a1)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_memset_reqh_inner)
void gasnete_memset_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t val, gasnet_handlerarg_t nbytes, void *dest, void *op) {
  memset(dest, (int)(uint32_t)val, nbytes);
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_markdone_reph),
                  PACK(op))));
}
SHORT_HANDLER(gasnete_memset_reqh,4,6,
              (token, a0, a1, UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, a1, UNPACK2(a2, a3), UNPACK2(a4, a5)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_markdone_reph_inner)
void gasnete_markdone_reph_inner(gasnet_token_t token, 
  void *op) {
  gasnete_op_markdone((gasnete_op_t *)op, 0); /*  assumes this is a put or explicit */
}
SHORT_HANDLER(gasnete_markdone_reph,1,2,
              (token, UNPACK(a0)    ),
              (token, UNPACK2(a0, a1)));
/* ------------------------------------------------------------------------------------ */

#define LOCBUF_RAR 0
#define LOCBUF_BB  1
#define LOCBUF_TMP 2
#if GASNETI_STATS_OR_TRACE
#define SET_LOCBUF(loc,val) do { loc = val; } while(0)
#else
#define SET_LOCBUF(loc,val) 
#endif
static const char* locbuf_name[] = {"RAR","BB","TMP"};

extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  if (nbytes <= GASNETC_PTL_MAX_TRANS_SZ) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
    ptl_size_t local_offset = 0;
    ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,src);
    ptl_handle_md_t md_h;
    ptl_process_id_t target_id = gasnetc_procid_map[node];
    ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
    ptl_match_bits_t match_bits = 0UL;
    uint8_t lbits = GASNETC_PTL_RAR_BITS | GASNETC_PTL_MSG_GET;
    int local_buf;

    gasneti_assert(remote_offset >= 0 && remote_offset < gasneti_seginfo[node].size);

    /* stall here if too many puts/gets in progress */
    if (gasnete_putget_limit > 0) {
      int inflight = gasneti_weakatomic_read(&gasnete_putget_inflight, 0);
      if (inflight > gasnete_putget_limit) {
	GASNETI_TRACE_PRINTF(C,("get_nb: throttling, inflight=%i",inflight));
	GASNETI_TRACE_EVENT(C, PUTGET_THROTTLE);
	gasneti_pollwhile( (gasneti_weakatomic_read(&gasnete_putget_inflight,0) > gasnete_putget_limit) );
      }
      gasneti_weakatomic_increment(&gasnete_putget_inflight,0);
    }

    /* encode gasnet handle into match bits, upper bits ignored */
    gasnete_set_mbits_lowbits(&match_bits, lbits, (gasnete_op_t*)op);
    /* Determine destination MD for Ptl Get */
    if (gasnetc_in_local_rar(dest,nbytes)) {
      md_h = gasnetc_RARAM_md_h;
      local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,dest);
      GASNETI_TRACE_EVENT(C, GET_NB_RAR);
      SET_LOCBUF(local_buf,LOCBUF_RAR);
    } else if ( (nbytes <= (GASNETC_CHUNKSIZE - (sizeof(void*))))  &&
		gasnetc_chunk_alloc(&gasnetc_ReqSB, nbytes, &local_offset) ) {
      /* Encode dest addr in BB chunk for later copy */
      void* bb;
      md_h = gasnetc_ReqSB.md_h;
      /* get the addr of the start of the chunk */
      bb = ((uint8_t*)gasnetc_ReqSB.start + local_offset);
      /* store the dest address at this location */
      *(uintptr_t*)bb = (uintptr_t)dest;
      /* Let portals use the rest of the chunk */
      local_offset += sizeof(void*);
      GASNETI_TRACE_EVENT(C, GET_NB_BB);
      SET_LOCBUF(local_buf,LOCBUF_BB);

    } else {
      /* alloc a temp md for the destination region */
      md_h = gasnetc_alloc_tmpmd(dest, nbytes, gasnetc_EQ_h);
      local_offset = 0;
      GASNETI_TRACE_EVENT(C, GET_NB_TMPMD);
      SET_LOCBUF(local_buf,LOCBUF_TMP);
    }

    GASNETI_TRACE_PRINTF(C,("get_nb: match_bits = 0x%lx, locbuf = %s, local_off=%lld, remote_off=%lld, bytes=%i",(uint64_t)match_bits,locbuf_name[local_buf],(long long)local_offset,(long long)remote_offset,(int)nbytes));
    /* Issue Ptl Get operation */
    GASNETC_PTLSAFE(PtlGetRegion(md_h, local_offset, nbytes, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset));

    if (gasnete_putget_poll > 0) {
      if (gasneti_weakatomic_read(&gasnete_putget_poll_cnt,0) > gasnete_putget_poll-1) {
	/* this will reset gasnete_putget_poll_cnt to zero */
	gasnetc_portals_poll();
      } else {
	/* bump the put/get counter */
	gasneti_weakatomic_increment(&gasnete_putget_poll_cnt, 0);
      }
    }

    return (gasnet_handle_t)op;
  } else {
    /*  need many messages - use an access region to coalesce them into a single handle */
    /*  (note this relies on the fact that our implementation of access regions allows recursion) */
    gasnete_begin_nbi_accessregion(1 /* enable recursion */ GASNETE_THREAD_PASS);
    gasnete_get_nbi_bulk(dest, node, src, nbytes GASNETE_THREAD_PASS);
    return gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
  }
}

GASNETI_INLINE(gasnete_put_nb_inner)
gasnet_handle_t gasnete_put_nb_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  if (nbytes <= GASNETC_PTL_MAX_TRANS_SZ) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_eop_t *op = gasnete_eop_new(mythread);
    ptl_size_t local_offset = 0;
    ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,dest);
    ptl_handle_md_t md_h;
    ptl_process_id_t target_id = gasnetc_procid_map[node];
    ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
    ptl_match_bits_t match_bits = 0ULL;
    uint8_t lbits = GASNETC_PTL_RAR_BITS | GASNETC_PTL_MSG_PUT;
    int wait_for_local_completion = 0;
    ptl_hdr_data_t hdr_data = 0;
    int local_buf;

    gasneti_assert(remote_offset >= 0 && remote_offset < gasneti_seginfo[node].size);

    gasneti_assert(gasneti_weakatomic_read(&(mythread->local_completion_count), 0) == 0);

    /* stall here if too many puts/gets in progress */
    if (gasnete_putget_limit > 0) {
      int inflight = gasneti_weakatomic_read(&gasnete_putget_inflight, 0);
      if (inflight > gasnete_putget_limit) {
	GASNETI_TRACE_PRINTF(C,("put_nb: throttling, inflight=%i",inflight));
	GASNETI_TRACE_EVENT(C, PUTGET_THROTTLE);
	gasneti_pollwhile( (gasneti_weakatomic_read(&gasnete_putget_inflight,0) > gasnete_putget_limit) );
      }
      gasneti_weakatomic_increment(&gasnete_putget_inflight,0);
    }

    /* Determine destination MD for Ptl Put */
    if (gasnetc_in_local_rar(src,nbytes)) {
      md_h = gasnetc_RARAM_md_h;
      local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,src);
      if (! isbulk) wait_for_local_completion = 1;
      GASNETI_TRACE_EVENT(C, PUT_NB_RAR);
      SET_LOCBUF(local_buf,LOCBUF_RAR);
    } else if ( (nbytes <= GASNETC_CHUNKSIZE)  &&
		gasnetc_chunk_alloc(&gasnetc_ReqSB,nbytes, &local_offset) ) {
      void* bb;
      md_h = gasnetc_ReqSB.md_h;
      /* get the addr of the start of the chunk */
      bb = ((uint8_t*)gasnetc_ReqSB.start + local_offset);
      /* copy the src data to the bounce buffer */
      memcpy(bb,src,nbytes);
      GASNETI_TRACE_EVENT(C, PUT_NB_BB);
      SET_LOCBUF(local_buf,LOCBUF_BB);
    } else {
      /* alloc a temp md for the source region */
      md_h = gasnetc_alloc_tmpmd(src, nbytes, gasnetc_EQ_h);
      local_offset = 0;
      if (! isbulk) wait_for_local_completion = 1;
      GASNETI_TRACE_EVENT(C, PUT_NB_TMPMD);
      SET_LOCBUF(local_buf,LOCBUF_TMP);
    }
    if (wait_for_local_completion) {
      /* increment local completion flag and indicate to event handler to decrement */
      gasneti_weakatomic_increment(&(mythread->local_completion_count), 0);
      lbits |= GASNETC_PTL_MSG_DOLC;
    }

    /* encode gasnet handle into match bits, upper bits ignored */
    gasnete_set_mbits_lowbits(&match_bits, lbits, (gasnete_op_t*)op);
    GASNETI_TRACE_PRINTF(C,("put_nb: match_bits = 0x%lx, locbuf = %s, local_off=%lld, remote_off=%lld, bytes=%i",(uint64_t)match_bits,locbuf_name[local_buf],(long long)local_offset,(long long)remote_offset,(int)nbytes));

    /* Issue Ptl Get operation */
    GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, nbytes, PTL_ACK_REQ, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset, hdr_data));

    /* bump the put/get counter */
    if (gasnete_putget_poll) gasneti_weakatomic_increment(&gasnete_putget_poll_cnt, 0);

    /* poll here for local completion in non-bulk or non-bb case */
    if (wait_for_local_completion) {
      gasneti_pollwhile( (gasneti_weakatomic_read(&(mythread->local_completion_count), 0) > 0) );
    }

    if (gasnete_putget_poll && (gasneti_weakatomic_read(&gasnete_putget_poll_cnt,0) > gasnete_putget_poll)) {
      /* this will reset gasnete_putget_poll_cnt to zero */
      gasnetc_portals_poll();
    }

    return (gasnet_handle_t)op;

  } else { 
    /*  need many messages - use an access region to coalesce them into a single handle */
    /*  (note this relies on the fact that our implementation of access regions allows recursion) */
    gasnete_begin_nbi_accessregion(1 /* enable recursion */ GASNETE_THREAD_PASS);
      if (isbulk) gasnete_put_nbi_bulk(node, dest, src, nbytes GASNETE_THREAD_PASS);
      else        gasnete_put_nbi    (node, dest, src, nbytes GASNETE_THREAD_PASS);
    return gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
  }
}

extern gasnet_handle_t gasnete_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  return gasnete_put_nb_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  return gasnete_put_nb_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}

extern gasnet_handle_t gasnete_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

  GASNETI_SAFE(
    SHORT_REQ(4,6,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, (gasnet_handlerarg_t)nbytes,
                 PACK(dest), PACK(op))));

  return (gasnet_handle_t)op;
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnb(gasnet_handle_t handle) {
  GASNETI_SAFE(gasneti_AMPoll());

  if (gasnete_op_isdone(handle)) {
    gasneti_sync_reads();
    gasnete_op_free(handle);
    return GASNET_OK;
  }
  else return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles) {
  int success = 0;
  int empty = 1;
  GASNETI_SAFE(gasneti_AMPoll());

  gasneti_assert(phandle);

  { int i;
    for (i = 0; i < numhandles; i++) {
      gasnete_op_t *op = phandle[i];
      if (op != GASNET_INVALID_HANDLE) {
        empty = 0;
        if (gasnete_op_isdone(op)) {
	  gasneti_sync_reads();
          gasnete_op_free(op);
          phandle[i] = GASNET_INVALID_HANDLE;
          success = 1;
        }  
      }
    }
  }

  if (success || empty) return GASNET_OK;
  else return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_all (gasnet_handle_t *phandle, size_t numhandles) {
  int success = 1;
  GASNETI_SAFE(gasneti_AMPoll());

  gasneti_assert(phandle);

  { int i;
    for (i = 0; i < numhandles; i++) {
      gasnete_op_t *op = phandle[i];
      if (op != GASNET_INVALID_HANDLE) {
        if (gasnete_op_isdone(op)) {
	  gasneti_sync_reads();
          gasnete_op_free(op);
          phandle[i] = GASNET_INVALID_HANDLE;
        } else success = 0;
      }
    }
  }

  if (success) return GASNET_OK;
  else return GASNET_ERR_NOT_READY;
}

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
  each message sends an ack - we count the number of implicit ops launched and compare
    with the number acknowledged
  Another possible design would be to eliminate some of the acks (at least for puts) 
    by piggybacking them on other messages (like get replies) or simply aggregating them
    the target until the source tries to synchronize
*/

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  ptl_process_id_t target_id = gasnetc_procid_map[node];
  ptl_handle_md_t md_h;
  ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
  ptl_size_t local_offset;
  ptl_size_t remote_offset;
  ptl_match_bits_t match_bits = 0ULL;
  uint8_t lbits = GASNETC_PTL_RAR_BITS | GASNETC_PTL_MSG_GET;
  int local_buf;

  gasneti_assert(gasneti_weakatomic_read(&(mythread->local_completion_count), 0) == 0);
  gasnete_set_mbits_lowbits(&match_bits, lbits, (gasnete_op_t*)op);

  /* Max transfer size is large, this loop will almost always execute exactly once */
  while (nbytes > 0) {
    size_t toget = MIN(nbytes,GASNETC_PTL_MAX_TRANS_SZ);

    /* stall here if too many puts/gets in progress */
    if (gasnete_putget_limit > 0) {
      int inflight = gasneti_weakatomic_read(&gasnete_putget_inflight, 0);
      if (inflight > gasnete_putget_limit) {
	GASNETI_TRACE_PRINTF(C,("get_nbi: throttling, inflight=%i",inflight));
	GASNETI_TRACE_EVENT(C, PUTGET_THROTTLE);
	gasneti_pollwhile( (gasneti_weakatomic_read(&gasnete_putget_inflight,0) > gasnete_putget_limit) );
      }
      gasneti_weakatomic_increment(&gasnete_putget_inflight,0);
    }

    local_offset = 0;
    remote_offset = GASNETC_PTL_OFFSET(node,src);
    gasneti_assert(remote_offset >= 0 && remote_offset < gasneti_seginfo[node].size);

    /* encode gasnet handle into match bits, upper bits ignored */
    /* Determine destination MD for Ptl Get */
    if (gasnetc_in_local_rar(dest,toget)) {
      md_h = gasnetc_RARAM_md_h;
      local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,dest);
      GASNETI_TRACE_EVENT(C, GET_NBI_RAR);
      SET_LOCBUF(local_buf,LOCBUF_RAR);
    } else if ( (toget <= (GASNETC_CHUNKSIZE - (sizeof(void*))))  &&
		gasnetc_chunk_alloc(&gasnetc_ReqSB,toget, &local_offset) ) {
      /* Encode dest addr in BB chunk for later copy */
      void* bb;
      md_h = gasnetc_ReqSB.md_h;
      /* get the addr of the start of the chunk */
      bb = ((uint8_t*)gasnetc_ReqSB.start + local_offset);
      /* store the dest address at this location */
      *(uintptr_t*)bb = (uintptr_t)dest;
      /* Let portals use the rest of the chunk */
      local_offset += sizeof(void*);
      GASNETI_TRACE_EVENT(C, GET_NBI_BB);
      SET_LOCBUF(local_buf,LOCBUF_BB);
    } else {
      /* alloc a temp md for the destination region */
      md_h = gasnetc_alloc_tmpmd(dest, toget, gasnetc_EQ_h);
      local_offset = 0;
      GASNETI_TRACE_EVENT(C, GET_NBI_TMPMD);
      SET_LOCBUF(local_buf,LOCBUF_TMP);
    }

    /* Issue Ptl Get operation */
    GASNETI_TRACE_PRINTF(C,("get_nbi: match_bits = 0x%lx, locbuf = %s, local_off=%lld, remote_off=%lld, nbytes=%i",(uint64_t)match_bits,locbuf_name[local_buf],(long long)local_offset,(long long)remote_offset,(int)nbytes));
    op->initiated_get_cnt++;
    GASNETC_PTLSAFE(PtlGetRegion(md_h, local_offset, toget, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset));
    nbytes -= toget;
    dest = ((uint8_t*)dest + toget);
    src = ((uint8_t*)src + toget);

    /* Make sure we poll occasionally */
    if (gasnete_putget_poll) {
      if (gasneti_weakatomic_read(&gasnete_putget_poll_cnt,0) > gasnete_putget_poll-1) {
	/* this will reset gasnete_putget_poll_cnt to zero */
	gasnetc_portals_poll();
      } else {
	/* bump the put/get counter */
	gasneti_weakatomic_increment(&gasnete_putget_poll_cnt, 0);
      }
    }
    
  }
  return;
}

GASNETI_INLINE(gasnete_put_nbi_inner)
void gasnete_put_nbi_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  ptl_handle_md_t md_h;
  ptl_process_id_t target_id = gasnetc_procid_map[node];
  ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
  ptl_match_bits_t match_bits = 0ULL;
  int wait_for_local_completion = 0;
  ptl_hdr_data_t hdr_data = 0;
  int local_buf;

  gasneti_assert(gasneti_weakatomic_read(&(mythread->local_completion_count), 0) == 0);

  /* Max transfer size is large, this loop will almost always execute exactly once */
  while (nbytes > 0) {
    size_t toput = MIN(nbytes,GASNETC_PTL_MAX_TRANS_SZ);
    ptl_size_t local_offset = 0;
    ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,dest);
    uint8_t lbits = GASNETC_PTL_RAR_BITS | GASNETC_PTL_MSG_PUT;

    gasneti_assert(remote_offset >= 0 && remote_offset < gasneti_seginfo[node].size);

    /* stall here if too many puts/gets in progress */
    if (gasnete_putget_limit > 0) {
      int inflight = gasneti_weakatomic_read(&gasnete_putget_inflight, 0);
      if (inflight > gasnete_putget_limit) {
	GASNETI_TRACE_PRINTF(C,("put_nbi: throttling, inflight=%i",inflight));
	GASNETI_TRACE_EVENT(C, PUTGET_THROTTLE);
	gasneti_pollwhile( (gasneti_weakatomic_read(&gasnete_putget_inflight,0) > gasnete_putget_limit) );
      }
      gasneti_weakatomic_increment(&gasnete_putget_inflight,0);
    }

    /* Determine destination MD for Ptl Get */
    if (gasnetc_in_local_rar(src,toput)) {
      md_h = gasnetc_RARAM_md_h;
      local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,src);
      SET_LOCBUF(local_buf,LOCBUF_RAR);
      if (! isbulk) {
	wait_for_local_completion = 1;
	lbits |= GASNETC_PTL_MSG_DOLC;
	gasneti_weakatomic_increment(&(mythread->local_completion_count), 0);
      }
      GASNETI_TRACE_EVENT(C, PUT_NBI_RAR);
    } else if ( (toput <= GASNETC_CHUNKSIZE)  &&
		gasnetc_chunk_alloc(&gasnetc_ReqSB,toput, &local_offset) ) {
      /* Encode dest addr in BB chunk for later copy */
      void* bb;
      md_h = gasnetc_ReqSB.md_h;
      /* get the addr of the start of the chunk */
      bb = ((uint8_t*)gasnetc_ReqSB.start + local_offset);
      /* copy the src data to the bounce buffer */
      memcpy(bb,src,toput);
      GASNETI_TRACE_EVENT(C, PUT_NBI_BB);
      SET_LOCBUF(local_buf,LOCBUF_BB);
    } else {
      /* alloc a temp md for the source region */
      md_h = gasnetc_alloc_tmpmd(src, toput, gasnetc_EQ_h);
      local_offset = 0;
      if (! isbulk) {
	wait_for_local_completion = 1;
	lbits |= GASNETC_PTL_MSG_DOLC;
	gasneti_weakatomic_increment(&(mythread->local_completion_count), 0);
      }
      GASNETI_TRACE_EVENT(C, PUT_NBI_TMPMD);
      SET_LOCBUF(local_buf,LOCBUF_TMP);
    }

    /* encode gasnet handle into match bits, upper bits ignored */
    gasnete_set_mbits_lowbits(&match_bits, lbits, (gasnete_op_t*)op);
    GASNETI_TRACE_PRINTF(C,("put_nbi: match_bits = 0x%lx, locbuf = %s, local_off=%lld, remote_off=%lld, bytes=%i",(uint64_t)match_bits,locbuf_name[local_buf],(long long)local_offset,(long long)remote_offset,(int)nbytes));

    /* Issue Ptl Put operation */
    op->initiated_put_cnt++;
    GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, toput, PTL_ACK_REQ, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset, hdr_data));

    /* bump the put/get counter */
    if (gasnete_putget_poll) gasneti_weakatomic_increment(&gasnete_putget_poll_cnt, 0);

    nbytes -= toput;
    src = ((uint8_t*)src + toput);
    dest = ((uint8_t*)dest + toput);
  }

  /* poll here for local completion in non-bulk or non-bb case */
  if (wait_for_local_completion) {
    gasneti_pollwhile( (gasneti_weakatomic_read(&(mythread->local_completion_count), 0) > 0) );
  }

  if (gasnete_putget_poll && (gasneti_weakatomic_read(&gasnete_putget_poll_cnt,0) > gasnete_putget_poll)) {
    /* this will reset gasnete_putget_poll_cnt to zero */
    gasnetc_portals_poll();
  }
}

extern void gasnete_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_put_nbi_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_put_nbi_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}

extern void gasnete_memset_nbi   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  op->initiated_put_cnt++;

  GASNETI_SAFE(
    SHORT_REQ(4,6,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, (gasnet_handlerarg_t)nbytes,
                 PACK(dest), PACK(op))));
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
    gasneti_assert(GASNETE_OP_THREADID(iop) == mythread->threadidx);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    #if GASNET_DEBUG
      if (iop->next != NULL)
        gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_gets() inside an NBI access region");
    #endif

    if (gasneti_weakatomic_read(&(iop->completed_get_cnt), 0) == iop->initiated_get_cnt) {
      if_pf (iop->initiated_get_cnt > 65000) { /* make sure we don't overflow the counters */
        gasneti_weakatomic_set(&(iop->completed_get_cnt), 0, 0);
        iop->initiated_get_cnt = 0;
      }
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
    gasneti_assert(GASNETE_OP_THREADID(iop) == mythread->threadidx);
    gasneti_assert(iop->next == NULL);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    #if GASNET_DEBUG
      if (iop->next != NULL)
        gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_puts() inside an NBI access region");
    #endif


    if (gasneti_weakatomic_read(&(iop->completed_put_cnt), 0) == iop->initiated_put_cnt) {
      if_pf (iop->initiated_put_cnt > 65000) { /* make sure we don't overflow the counters */
        gasneti_weakatomic_set(&(iop->completed_put_cnt), 0, 0);
        iop->initiated_put_cnt = 0;
      }
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
  Non-Blocking Value Get (explicit-handle)
  ========================================
*/
typedef struct _gasnet_valget_op_t {
  gasnet_handle_t handle;
  gasnet_register_value_t val;

  struct _gasnet_valget_op_t* next; /* for free-list only */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
} gasnet_valget_op_t;

extern gasnet_valget_handle_t gasnete_get_nb_val(gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnet_valget_handle_t retval;
  gasneti_assert(nbytes > 0 && nbytes <= sizeof(gasnet_register_value_t));
  gasneti_boundscheck(node, src, nbytes);
  if (mythread->valget_free) {
    retval = mythread->valget_free;
    mythread->valget_free = retval->next;
    gasneti_memcheck(retval);
  } else {
    retval = (gasnet_valget_op_t*)gasneti_malloc(sizeof(gasnet_valget_op_t));
    retval->threadidx = mythread->threadidx;
  }

  retval->val = 0;
  if (gasnete_islocal(node)) {
    GASNETE_FAST_ALIGNED_MEMCPY(GASNETE_STARTOFBITS(&(retval->val),nbytes), src, nbytes);
    retval->handle = GASNET_INVALID_HANDLE;
  } else {
    retval->handle = gasnete_get_nb_bulk(GASNETE_STARTOFBITS(&(retval->val),nbytes), node, src, nbytes GASNETE_THREAD_PASS);
  }
  return retval;
}

extern gasnet_register_value_t gasnete_wait_syncnb_valget(gasnet_valget_handle_t handle) {
  gasnet_register_value_t val;
  gasnete_threaddata_t * const thread = gasnete_threadtable[GASNETE_OP_THREADID(handle)];
  gasneti_assert(thread == gasnete_mythread());
  handle->next = thread->valget_free; /* free before the wait to save time after the wait, */
  thread->valget_free = handle;       /*  safe because this thread is under our control */

  gasnete_wait_syncnb(handle->handle);
  val = handle->val;
  return val;
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
  gasneti_handler_tableentry_with_bits(gasnete_get_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_get_reph),
  gasneti_handler_tableentry_with_bits(gasnete_getlong_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_getlong_reph),
  gasneti_handler_tableentry_with_bits(gasnete_put_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_putlong_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_memset_reqh),
  gasneti_handler_tableentry_with_bits(gasnete_markdone_reph),

  { 0, NULL }
};

extern gasnet_handlerentry_t const *gasnete_get_handlertable() {
  return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

