/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_extended.c,v $
 *     $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.7.2.5 $
 * Description: GASNet Extended API Implementation for DCMF
 * Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>
 *                 Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

#include <gasnet_core_internal.h>

#include <gasnet_coll.h>
#include <gasnet_coll_internal.h>
#include <gasnet_coll_autotune_internal.h>

#include <gasnet_extended_coll_dcmf.h>

static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

static void increment_value(void *arg, DCMF_Error_t *e) {
  volatile int* in = (volatile int*) arg;
  gasneti_assert(in);
  (*in)++;
}

static void empty_cb(void *arg, DCMF_Error_t *e) {
  return;
}

/* No loopback check should be required for gasnete_{get,put,memset}*:
 *  For the external Extended API the wrappers in gasnet_extended.h check for loopback
 *  For uses in vis, collectives, etc. the burden lies with the caller to check.
 */
#ifndef GASNETE_CHECK_LOOPBACK
#define GASNETE_CHECK_LOOPBACK 0
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


/*enable puts to turn into DCMF_Puts*/
#ifndef GASNETE_DIRECT_PUT_GET
#define GASNETE_DIRECT_PUT_GET 1
#endif


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

  gasneti_assert_always(GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD <= gasnet_AMMaxMedium());
  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));

#if !GASNETE_DIRECT_PUT_GET
  /* The next two ensure nbytes in AM-based Gets will fit in handler_arg_t (bug 2770) */
  gasneti_assert_always(gasnet_AMMaxMedium() <= (size_t)0xffffffff);
  gasneti_assert_always(gasnet_AMMaxLongReply() <= (size_t)0xffffffff);
#endif
}

#if GASNETE_DIRECT_PUT_GET
static DCMF_Memregion_t gasnete_dcmf_my_mem_region;
static DCMF_Memregion_t *gasnete_dcmf_all_mem_regions;
static DCMF_Protocol_t gasnete_dcmf_put_registration;
static DCMF_Protocol_t gasnete_dcmf_get_registration;
#endif

extern void gasnete_init(void) {
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

  { gasnete_threaddata_t *threaddata = NULL;
    gasnete_eop_t *eop = NULL;
    #if GASNETI_CLIENT_THREADS
      /* register first thread (optimization) */
      threaddata = gasnete_mythread(); 
    #else
      /* register only thread (required) */
      threaddata = gasnete_new_threaddata();
    #endif

    /* cause the first pool of eops to be allocated (optimization) */
    eop = gasnete_eop_new(threaddata);
    gasnete_op_markdone((gasnete_op_t *)eop, 0);
    gasnete_op_free((gasnete_op_t *)eop);
  }
#if GASNETE_DIRECT_PUT_GET
 {
   DCMF_Put_Configuration_t put_config;
   DCMF_Get_Configuration_t get_config;
   size_t bytes_out;
   size_t bytes_in;
   
   DCMF_Hardware_t hw;
   size_t memsize;
   GASNETC_DCMF_LOCK();
   DCMF_SAFE(DCMF_Hardware(&hw));
   bytes_in = (hw.memSize/hw.tSize)*1024*1024;
   GASNETC_DCMF_UNLOCK();
   
   bzero(&put_config, sizeof(DCMF_Put_Configuration_t));
   bzero(&get_config, sizeof(DCMF_Get_Configuration_t));
   
   put_config.protocol=DCMF_DEFAULT_PUT_PROTOCOL;
   get_config.protocol=DCMF_DEFAULT_GET_PROTOCOL;
   
   /*intialize the memregions and try to pin entire VM space*/
   GASNETC_DCMF_LOCK();
   GASNETI_TRACE_PRINTF(C, ("Trying to pin %d bytes\n", bytes_in));
   DCMF_SAFE(DCMF_Memregion_create(&gasnete_dcmf_my_mem_region, &bytes_out, bytes_in, 0, 0));
   GASNETI_TRACE_PRINTF(C, ("Bytes Pinned: %d\n", bytes_out));
   GASNETC_DCMF_CHECK_PTR(&gasnete_dcmf_put_registration);
   DCMF_SAFE(DCMF_Put_register(&gasnete_dcmf_put_registration, &put_config));
   GASNETC_DCMF_CHECK_PTR(&gasnete_dcmf_get_registration);
   DCMF_SAFE(DCMF_Get_register(&gasnete_dcmf_get_registration, &get_config));
   GASNETC_DCMF_UNLOCK();
   
   /*make srue everyone knows about everybody elses memory*/
   gasnete_dcmf_all_mem_regions = gasneti_malloc_aligned(16, sizeof(DCMF_Memregion_t)*gasneti_nodes);

   gasnetc_dcmf_bootstrapExchange(gasnete_dcmf_my_mem_region, sizeof(DCMF_Memregion_t), gasnete_dcmf_all_mem_regions);
   
 }                                
#endif
  
  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize VIS subsystem */
  gasnete_vis_init();

  /* Initialize the collectives subsystem which is necessary for named
     barrier operations */
  if (!gasnete_coll_dcmf_inited)
    gasnete_coll_init_dcmf();
  
  /* Initialize the dcmf-conduit team components which are necessary
     for dcmf-conduit named barriers.  This is usually done at team
     initialization of GASNET_TEAM_ALL in gasnet_coll_init().  But
     because some gasnet client programs, such as testbarrier.c, may
     use named barriers without calling gasnet_coll_init(), we need to
     make sure that the dcmf-conduit named barrier is ready after the
     GASNet client calls gasnet_attach(). */
  if (GASNET_TEAM_ALL->dcmf_tp == NULL)
    {
      /* rel2act_map is necessary to initialize DCMF_Geometry */
      if (GASNET_TEAM_ALL->rel2act_map == NULL) 
        { 
          int i;
          GASNET_TEAM_ALL->rel2act_map = 
            (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t)*GASNET_TEAM_ALL->total_ranks);
          for(i=0; i<gasneti_nodes; i++)
            GASNET_TEAM_ALL->rel2act_map[i] = i;
        }
      gasnete_coll_team_init_dcmf(GASNET_TEAM_ALL);
    }
}

/* ------------------------------------------------------------------------------------ */
/*
  Op management
  =============
*/
/*  get a new op and mark it in flight */
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t * const thread) {
  gasnete_eopaddr_t head = thread->eop_free;
  if_pt (!gasnete_eopaddr_isnil(head)) {
    gasnete_eop_t *eop = GASNETE_EOPADDR_TO_PTR(thread, head);
    thread->eop_free = eop->addr;
    eop->addr = head;
    gasneti_assert(!gasnete_eopaddr_equal(thread->eop_free,head));
    gasneti_assert(eop->threadidx == thread->threadidx);
    gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);
    gasneti_assert(OPTYPE(eop) == OPSTATE_FREE);
    SET_OPSTATE(eop, OPSTATE_INFLIGHT);
    /*eop->dcmf_req = gasnetc_get_dcmf_req();*/
    return eop;
  } else { /*  free list empty - need more eops */
    int bufidx = thread->eop_num_bufs;
    gasnete_eop_t *buf;
    int i;
    gasnete_threadidx_t threadidx = thread->threadidx;
    if (bufidx == 256) gasneti_fatalerror("GASNet Extended API: Ran out of explicit handles (limit=65535)");
    thread->eop_num_bufs++;
    buf = (gasnete_eop_t *)gasneti_calloc(256,sizeof(gasnete_eop_t));
    /* buf = (gasnete_eop_t *)gasneti_malloc_aligned(16,256*sizeof(gasnete_eop_t));
       bzero(buf, sizeof(gasnete_eop_t)*256); */
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
      #if 0 /* these can safely be skipped when the values are zero */
        SET_OPSTATE(&(buf[i]),OPSTATE_FREE); 
        SET_OPTYPE(&(buf[i]),OPTYPE_EXPLICIT); 
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

    return gasnete_eop_new(thread); /*  should succeed this time */
  }
}

gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t * const thread) {
  gasnete_iop_t *iop;
  if_pt (thread->iop_free) {
    iop = thread->iop_free;
    thread->iop_free = iop->next;
    gasneti_memcheck(iop);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    gasneti_assert(iop->threadidx == thread->threadidx);
  } else {
    iop = (gasnete_iop_t *)gasneti_malloc(sizeof(gasnete_iop_t));
    #if GASNET_DEBUG
      memset(iop, 0, sizeof(gasnete_iop_t)); /* set pad to known value */
    #endif
    SET_OPTYPE((gasnete_op_t *)iop, OPTYPE_IMPLICIT);
    iop->threadidx = thread->threadidx;
  }
  iop->next = NULL;

  iop->initiated_get_cnt = 0;
  iop->initiated_put_cnt = 0;
  gasneti_weakatomic_set(&(iop->completed_get_cnt), 0, 0);
  gasneti_weakatomic_set(&(iop->completed_put_cnt), 0, 0);
  gasnete_iop_check(iop);
  return iop;
}

/*  query an op for completeness - for iop this means both puts and gets */
int gasnete_op_isdone(gasnete_op_t *op) {
  gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
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
    SET_OPSTATE(eop, OPSTATE_COMPLETE);
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
    if (isget) gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
    else gasneti_weakatomic_increment(&(iop->completed_put_cnt), 0);
  }
}

/*  free an op */
void gasnete_op_free(gasnete_op_t *op) {
  gasnete_threaddata_t * const thread = gasnete_threadtable[op->threadidx];
  gasneti_assert(thread == gasnete_mythread());
  if (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t *)op;
    gasnete_eopaddr_t addr = eop->addr;
    gasneti_assert(OPSTATE(eop) == OPSTATE_COMPLETE);
    gasnete_eop_check(eop);
    SET_OPSTATE(eop, OPSTATE_FREE);
    eop->addr = thread->eop_free;
    thread->eop_free = addr;
    /*    gasnetc_free_dcmf_req(eop->dcmf_req);*/
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
    gasneti_assert(iop->next == NULL);
    iop->next = thread->iop_free;
    thread->iop_free = iop;

    /*go through and add all the dcmf requests back onto the free list*/
    /*     while(iop->dcmf_req_head) { */
    /*       gasnetc_dcmf_req_t *req = iop->dcmf_req_head; */
    /*       iop->dcmf_req_head = iop->dcmf_req_head->next; */
    /*       gasnetc_free_dcmf_req(req); */
    /*     } */
    /*     gasneti_assert(iop->dcmf_req_head ==NULL); */
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
  gasnet_handlerarg_t val, void *nbytes_arg, void *dest, void *op) {
  size_t nbytes = (uintptr_t)nbytes_arg;
  memset(dest, (int)(uint32_t)val, nbytes);
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_markdone_reph),
                  PACK(op))));
}
SHORT_HANDLER(gasnete_memset_reqh,4,7,
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));
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


#if GASNETE_DIRECT_PUT_GET

static void gasnete_mark_dcmf_get_done(void *arg, DCMF_Error_t *e) {
  gasnete_op_markdone((gasnete_op_t *)arg, 1);
}

extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_eop_t *op;
  DCMF_Callback_t done_cb;

#if GASNETE_CHECK_LOOPBACK
  if(node == gasneti_mynode) {
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    return GASNET_INVALID_HANDLE;
  }
#endif
  GASNETI_CHECKPSHM_GET(UNALIGNED,H);
  
  op = gasnete_eop_new(GASNETE_MYTHREAD);
  
  done_cb.function = gasnete_mark_dcmf_get_done;
  done_cb.clientdata = (void*) op;

  GASNETC_DCMF_LOCK();
  GASNETC_DCMF_CHECK_PTR(&op->dcmf_req);
  DCMF_SAFE(DCMF_Get(&gasnete_dcmf_get_registration,
                     &op->dcmf_req, done_cb,
                     DCMF_RELAXED_CONSISTENCY, node,
                     nbytes, gasnete_dcmf_all_mem_regions+node,
                     &gasnete_dcmf_my_mem_region, 
                     (size_t) src, (size_t) dest));
  GASNETC_DCMF_UNLOCK();
  return (gasnet_handle_t)op;
}

static void gasnete_mark_dcmf_put_done(void *arg, DCMF_Error_t *e) {
  gasnete_op_markdone((gasnete_op_t *)arg, 0);
}

GASNETI_INLINE(gasnete_put_nb_inner)
gasnet_handle_t gasnete_put_nb_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  gasnete_eop_t *op;
  volatile int local_put_done = 0; 
  DCMF_Callback_t local_done_cb, remote_done_cb;
  
#if GASNETE_CHECK_LOOPBACK
  if(node == gasneti_mynode) {
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    return GASNET_INVALID_HANDLE;
  }
#endif
  
  op = gasnete_eop_new(GASNETE_MYTHREAD);


  if(isbulk) {
    local_done_cb.function = empty_cb;
  } else {
    local_done_cb.function = increment_value;
    local_done_cb.clientdata = (void*) &local_put_done;
  }
  
  remote_done_cb.function = gasnete_mark_dcmf_put_done;
  remote_done_cb.clientdata = (void*) op;
  
  GASNETC_DCMF_LOCK();
  GASNETC_DCMF_CHECK_PTR(&op->dcmf_req);
  DCMF_SAFE(DCMF_Put(&gasnete_dcmf_put_registration, &op->dcmf_req,
                     local_done_cb,
                     DCMF_RELAXED_CONSISTENCY, node, nbytes,
                     &gasnete_dcmf_my_mem_region, gasnete_dcmf_all_mem_regions+node,
                     (size_t)src, (size_t)dest, remote_done_cb));
  GASNETC_DCMF_UNLOCK();
  if(!isbulk) {
    gasneti_polluntil(local_put_done!=0);
    /*while(local_put_done == 0) {
      gasneti_AMPoll();
      }*/
  }
  
  return (gasnet_handle_t)op;
}
#else

extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    GASNETI_SAFE(
      SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_get_reqh), 
                   (gasnet_handlerarg_t)nbytes, PACK(dest), PACK(src), PACK(op))));

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
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    GASNETI_SAFE(
      MEDIUM_REQ(2,4,(node, gasneti_handleridx(gasnete_put_reqh),
                    src, nbytes,
                    PACK(dest), PACK(op))));

    return (gasnet_handle_t)op;
  } else if (nbytes <= gasnet_AMMaxLongRequest()) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    if (isbulk) {
      GASNETI_SAFE(
        LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                    src, nbytes, dest,
                    PACK(op))));
    } else {
      GASNETI_SAFE(
        LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                    src, nbytes, dest,
                    PACK(op))));
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
#endif

extern gasnet_handle_t gasnete_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(ALIGNED,H);
  return gasnete_put_nb_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
  return gasnete_put_nb_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}

extern gasnet_handle_t gasnete_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_MEMSET(H);
 {
  gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK(op))));

  return (gasnet_handle_t)op;
 }
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

#if GASNETE_DIRECT_PUT_GET
static gasneti_lifo_head_t gasnete_iop_dcmf_req_free_list = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(gasnete_get_iop_dcmf_req)
gasnete_iop_dcmf_req_t *gasnete_get_iop_dcmf_req(gasnete_iop_t * const op) {
  gasnete_iop_dcmf_req_t *ret;
  gasnete_iop_check(op);

  ret = gasneti_lifo_pop(&gasnete_iop_dcmf_req_free_list);
  if(!ret) {

   /*assume taht we'll need a few more of these so just go ahead and allocate 256 and push them on to the free list*/
    int i=0;
    gasnete_iop_dcmf_req_t *tail, *head;
    /*XXX: fix alignment to make sure that the requests are separated by cachelines*/
    ret =  (gasnete_iop_dcmf_req_t*) gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, sizeof(gasnete_iop_dcmf_req_t)*256);
    head = ret+1; /*link the rest on the free list*/
    tail = head; /*start at ret[1]*/
    for(i=1; i<255; i++) {
      gasneti_lifo_link(tail, tail+1); /*link to the next one*/
      tail = tail+1; /*set to the next one*/
    }
    
    gasneti_lifo_push_many(&gasnete_iop_dcmf_req_free_list, (void*) head, (void*) tail);
  }
  

  ret->ptr = op;
  return ret;
}

GASNETI_INLINE(gasnete_free_iop_dcmf_req)
void gasnete_free_iop_dcmf_req (gasnete_iop_dcmf_req_t* req) {
  req->ptr = NULL;

  gasneti_lifo_push(&gasnete_iop_dcmf_req_free_list, (void*) req);


}
#endif

#if GASNETE_DIRECT_PUT_GET 
static void gasnete_mark_iop_put_done(void *arg, DCMF_Error_t *error) {
  gasnete_iop_dcmf_req_t *in = (gasnete_iop_dcmf_req_t*) arg;
  
  gasneti_assert(arg!=0);
  gasneti_assert(in->ptr!=0);
  gasnete_op_markdone((gasnete_op_t*) in->ptr, 0);
  gasnete_free_iop_dcmf_req(in);
  return;
}

GASNETI_INLINE(gasnete_put_nbi_inner)
void gasnete_put_nbi_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  volatile int local_put_done = 0; 
  DCMF_Callback_t local_done_cb, remote_done_cb;
  gasnete_iop_dcmf_req_t *req = NULL;
  
#if GASNETE_CHECK_LOOPBACK
  if(node == gasneti_mynode) {
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    return;
  }
#endif
  
  if(isbulk) {
    local_done_cb.function = empty_cb;
  } else {
    local_done_cb.function = increment_value;
    local_done_cb.clientdata = (void*) &local_put_done;
  }
  gasnete_iop_check(op);
  req = gasnete_get_iop_dcmf_req(op);
  gasneti_assert(req);
  remote_done_cb.function = gasnete_mark_iop_put_done;
  remote_done_cb.clientdata = (void*) req;

  
  /*each put needs its own request so allocate one and push it onto 
    a lifo list of these ops that will get freed when the iop is freed*/
  
  GASNETC_DCMF_LOCK();
  op->initiated_put_cnt++;
  GASNETC_DCMF_CHECK_PTR(&req->dcmf_req);
  DCMF_SAFE(DCMF_Put(&gasnete_dcmf_put_registration, &req->dcmf_req,
                     local_done_cb,
                     DCMF_RELAXED_CONSISTENCY, node, nbytes,
                     &gasnete_dcmf_my_mem_region, gasnete_dcmf_all_mem_regions+node,
                     (size_t)src, (size_t)dest, remote_done_cb));
  
  GASNETC_DCMF_UNLOCK();
  
  if(!isbulk) {
    gasneti_polluntil(local_put_done!=0);
    /*while(local_put_done == 0) {
      gasneti_AMPoll();
      }*/
  }

  return;
}
#else

GASNETI_INLINE(gasnete_put_nbi_inner)
void gasnete_put_nbi_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;

  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    op->initiated_put_cnt++;

    GASNETI_SAFE(
      MEDIUM_REQ(2,4,(node, gasneti_handleridx(gasnete_put_reqh),
                    src, nbytes,
                    PACK(dest), PACK(op))));
    return;
  } else if (nbytes <= gasnet_AMMaxLongRequest()) {
    op->initiated_put_cnt++;

    if (isbulk) {
      GASNETI_SAFE(
        LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                      src, nbytes, dest,
                      PACK(op))));
    } else {
      GASNETI_SAFE(
        LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                      src, nbytes, dest,
                      PACK(op))));
    }

    return;
  } else {
    int chunksz = gasnet_AMMaxLongRequest();
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    for (;;) {
      op->initiated_put_cnt++;
      if (nbytes > chunksz) {
        if (isbulk) {
          GASNETI_SAFE(
            LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                          psrc, chunksz, pdest,
                          PACK(op))));
        } else {
          GASNETI_SAFE(
            LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                          psrc, chunksz, pdest,
                          PACK(op))));
        }
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        if (isbulk) {
          GASNETI_SAFE(
            LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                          psrc, nbytes, pdest,
                          PACK(op))));
        } else {
          GASNETI_SAFE(
            LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_putlong_reqh),
                          psrc, nbytes, pdest,
                          PACK(op))));
        }
        break;
      }
    }
    return;
  }
}
#endif


#if GASNETE_DIRECT_PUT_GET
static void gasnete_mark_iop_get_done(void *arg, DCMF_Error_t *error) {
  gasnete_iop_dcmf_req_t *in = (gasnete_iop_dcmf_req_t*) arg;
  
  gasneti_assert(arg);
  gasneti_assert(in->ptr);
  
  gasnete_op_markdone((gasnete_op_t*) in->ptr, 1);
  gasnete_free_iop_dcmf_req(in);
  return ;
}

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  DCMF_Callback_t done_cb;
  gasnete_iop_dcmf_req_t *req = NULL;

#if GASNETE_CHECK_LOOPBACK
  if(node == gasneti_mynode) {
    GASNETE_FAST_UNALIGNED_MEMCPY(dest, src, nbytes);
    return;
  }
#endif
  GASNETI_CHECKPSHM_GET(UNALIGNED,V);
  
  req = gasnete_get_iop_dcmf_req(op);
  gasneti_assert(req);
  done_cb.function = gasnete_mark_iop_get_done;
  done_cb.clientdata = (void*) req;

  
  GASNETC_DCMF_LOCK();
  op->initiated_get_cnt++;
  GASNETC_DCMF_CHECK_PTR(&req->dcmf_req);
  DCMF_SAFE(DCMF_Get(&gasnete_dcmf_get_registration,
                     &req->dcmf_req, done_cb,
                     DCMF_RELAXED_CONSISTENCY, node,
                     nbytes, gasnete_dcmf_all_mem_regions+node,
                     &gasnete_dcmf_my_mem_region, 
                     (size_t) src, (size_t) dest));
  GASNETC_DCMF_UNLOCK();
  
  return;
}

#else
extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    op->initiated_get_cnt++;
  
    GASNETI_SAFE(
      SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_get_reqh), 
                   (gasnet_handlerarg_t)nbytes, PACK(dest), PACK(src), PACK(op))));
    return;
  } else {
    int chunksz;
    gasnet_handler_t reqhandler;
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    #if GASNETE_USE_LONG_GETS
      gasneti_memcheck(gasneti_seginfo);
      if (gasneti_in_segment(gasneti_mynode, dest, nbytes)) {
        chunksz = gasnet_AMMaxLongReply();
        reqhandler = gasneti_handleridx(gasnete_getlong_reqh);
      }
      else 
    #endif
      { reqhandler = gasneti_handleridx(gasnete_get_reqh);
        chunksz = gasnet_AMMaxMedium();
      }
    for (;;) {
      op->initiated_get_cnt++;
      if (nbytes > chunksz) {
        GASNETI_SAFE(
          SHORT_REQ(4,7,(node, reqhandler, 
                       (gasnet_handlerarg_t)chunksz, PACK(pdest), PACK(psrc), PACK(op))));
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        GASNETI_SAFE(
          SHORT_REQ(4,7,(node, reqhandler, 
                       (gasnet_handlerarg_t)nbytes, PACK(pdest), PACK(psrc), PACK(op))));
        break;
      }
    }
    return;
  }
}
#endif
extern void gasnete_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(ALIGNED,V);
  gasnete_put_nbi_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
  gasnete_put_nbi_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}

extern void gasnete_memset_nbi   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_MEMSET(V);
 {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  op->initiated_put_cnt++;

  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK(op))));
 }
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
  #if 1
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
    gasneti_assert(iop->threadidx == mythread->threadidx);
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
extern void  gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG) {
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

static void gasnete_dcmfbarrier_init(gasnete_coll_team_t team);
static void gasnete_dcmfbarrier_notify(gasnete_coll_team_t team, int id, int flags);
static int gasnete_dcmfbarrier_wait(gasnete_coll_team_t team, int id, int flags);
static int gasnete_dcmfbarrier_try(gasnete_coll_team_t team, int id, int flags);
int gasnete_dcmfbarrier_fast = 0;

#define GASNETE_BARRIER_DEFAULT "DCMF_BARRIER"
#define GASNETE_BARRIER_READENV() do { \
  if(GASNETE_ISBARRIER("DCMF_BARRIER")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_DCMF; \
} while (0)

#define GASNETE_BARRIER_INIT(TEAM, BARRIER_TYPE) do {                         \
    if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_DCMF && (TEAM)==GASNET_TEAM_ALL) {                \
      (TEAM)->barrier_notify = &gasnete_dcmfbarrier_notify; \
      (TEAM)->barrier_wait =   &gasnete_dcmfbarrier_wait;   \
      (TEAM)->barrier_try =    &gasnete_dcmfbarrier_try;    \
      (TEAM)->barrier_pf =     NULL;                        \
       gasnete_dcmfbarrier_init(TEAM);                      \
    }                                                       \
  } while (0)


/* use reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C



/********************************
 * DCMF Barrier Implementation 
 * Anonymous Barriers:
 *     + use DCMF_GlobalBarrier for anonymous barriers
 * Named Barriers:
 *     + Use DCMF_Allreduce with barrier id as argument 
 *     + Use MIN as reduction operator. When reduced value and my id don't 
 *       declare a barrier mismatch
 *     + Use -1 to denote a barrier mismatch
 **********************************/

static int current_barrier_flags;
static int current_barrier_id;
static volatile int barrier_done; /* should be a per-team variable */
static long long named_barrier_source[2];
static long long named_barrier_result[2];
static DCMF_Request_t barrier_req;
static DCMF_Protocol_t anon_barrier_registration;
static DCMF_Protocol_t named_barrier_registration;

static int gasnete_allow_hw_barrier;


static void gasnete_dcmfbarrier_init(gasnete_coll_team_t team) {

  team->barrier_splitstate = OUTSIDE_BARRIER;
  
  /* by default assume that if the user provides the anonymous flag on
     one node it is done so on all the nodes and thus we can use the built-in
     *fast* hardware barrier. IBM advertises O(1us) for 72k nodes so we def want to use it
     with this disabled we revert to the spec-compliant Gasnet barrier

  */
  
  gasnete_allow_hw_barrier = gasneti_getenv_yesno_withdefault("GASNET_DCMF_FAST_BARRIER", 1);
  /*initialize anonymous barrier*/
  if(gasnete_allow_hw_barrier) {
    DCMF_GlobalBarrier_Configuration_t config;
    
    /*for now just sticked w/ a single barrier protocol that we pick*/
    const char *barrier_protocol = gasneti_getenv_withdefault("GASNET_DCMF_ANONBARRIER_PROTOCOL", "DEFAULT");
    bzero(&config, sizeof(DCMF_GlobalBarrier_Configuration_t));
    gasnete_dcmfbarrier_fast = 1;
    if(!strcmp(barrier_protocol, "DEFAULT")) {
      config.protocol = DCMF_DEFAULT_GLOBALBARRIER_PROTOCOL;
    } else if(!strcmp(barrier_protocol, "GLOBAL_INTERRUPT")) {
      config.protocol = DCMF_GI_GLOBALBARRIER_PROTOCOL;
    } else {
      gasneti_fatalerror("unknown dcmf barrier protocol: %s", barrier_protocol);
    }
    
    /*tested both DCMF_GI_GLOBALBARRIER_PROTOCOL and DEFAULT_PROTOCOL*/
    /*default performacne @ 256 nodes was 1.3us 
      GI perforamance @ 256 nodes was us 1.308 
      Thus we will use DEFAULT for portability sake
    */

    GASNETC_DCMF_LOCK();
    GASNETC_DCMF_CHECK_PTR(&anon_barrier_registration);
    DCMF_SAFE(DCMF_GlobalBarrier_register(&anon_barrier_registration, &config));
    GASNETC_DCMF_UNLOCK();
  } else {
    /* if DCMF hardware barrier is disabled then just return and use the default AM barrier */
    team->barrier_notify = NULL;
    team->barrier_wait = NULL;
    team->barrier_try = NULL;
    return;
  }

  /*initialize named barrier*/

  /* Bug 2781. Because DCMF_GlobalAllreduce(), which only uses the
     BG/P collectives network, would block and not return from the
     call in dual and vn mode if some nodes haven't entered the
     matching DCMF_GlobalAllreduce().  We change the GASNet named
     barrier to use DCMF_Allreduce(), which can run on either the
     torus network or the collectives network, depending on the
     protocol selected.  Experiments show that only the TREE allreduce
     protocols would block but the TORUS allreduce protocols work
     fine. The named barrier initialization is done in the dcmf
     conduit-specific team initialization module. See
     gasnet_extended_coll_dcmf.c and gasnet_coll_allreduce_dcmf.c for
     more details. */
  named_barrier_source[0] = -1; named_barrier_source[1]=0;
  named_barrier_result[0] = -1; named_barrier_result[1]=1;

  barrier_done= 0;
}

#define BUILD_BARRIER_TAG(FLAGS, ID) GASNETI_MAKEWORD(FLAGS, ID)
#define EXTRACT_BARRIER_FLAGS(TAG) GASNETI_HIWORD(TAG)
#define EXTRACT_BARRIER_ID(TAG) GASNETI_LOWORD(TAG)
#define MISMATCH_FLAG 0x00
#define ANON_FLAG 0x01
#define NAMED_FLAG 0x02

static void gasnete_dcmfbarrier_notify(gasnete_coll_team_t team, int id, int flags) 
{
  int barrier_id;
  DCMF_Callback_t cb_done;
  
  gasneti_sync_reads();
  if(team->barrier_splitstate == INSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");
  } 

  /*gasneti_assert(id >= 0);*/
  GASNETI_TRACE_PRINTF(B, ("running barrier notify (%d,%d)", id, flags));
  named_barrier_result[0] = named_barrier_source[0] = 0;
  named_barrier_result[0] = named_barrier_source[1] = 0;
  
  if (flags == GASNET_BARRIERFLAG_MISMATCH) 
    {
      /*signal mismatch*/
      named_barrier_source[0] = BUILD_BARRIER_TAG(MISMATCH_FLAG, id);
      GASNETI_TRACE_PRINTF(B, ("mismatch barrier tag: %llx", named_barrier_source[0]));
    } 
  else if (flags == GASNET_BARRIERFLAG_ANONYMOUS) 
    {
      /*for an anonymous barrier propagate the id as 0 so there isn't any confusion amongst
	the nodes*/
      named_barrier_source[0] = BUILD_BARRIER_TAG(ANON_FLAG, 0);
      GASNETI_TRACE_PRINTF(B, ("anon barrier tag: %llx", named_barrier_source[0]));
    } 
  else 
    {
      named_barrier_source[0] = BUILD_BARRIER_TAG(NAMED_FLAG, id);
      GASNETI_TRACE_PRINTF(B, ("named barrier tag: %llx", named_barrier_source[0]));
    }
  
  named_barrier_source[1] = -named_barrier_source[0];
  
  barrier_done = 0;
  cb_done.function = increment_value;
  cb_done.clientdata = (void*)&barrier_done;
  
  /* Run a global barrier for both anonymous barrier and named
     barrier.  This is a workaround for bug 2781 because
     DCMF_GlobalAllreuce() and DCMF_Allreduce() would block when using
     the collective (tree) network. The allreduce operation of named
     barrier will be done in finish_barrier() after all nodes have
     arrived. DCMF_GlobalBarrier() is non-blocking and the added
     overhead is very small compared to the required allreduce
     operation because it is a hardware barrier. */
  GASNETC_DCMF_LOCK();
  GASNETC_DCMF_CHECK_PTR(&barrier_req);
  DCMF_SAFE(DCMF_GlobalBarrier(&anon_barrier_registration, &barrier_req, cb_done));
  GASNETC_DCMF_UNLOCK();

  current_barrier_flags = flags;
  current_barrier_id = id;
  team->barrier_splitstate = INSIDE_BARRIER; 

  gasneti_sync_writes();
  GASNETI_TRACE_PRINTF(B, ("finishing barrier notify (%d,%d)", id, flags));
}

static inline int finish_barrier(gasnete_coll_team_t team, int id, int flags) 
{
  int ret;
  
  /*we need to run the named barrier algorithm so lets check the results*/
  /*if someone signalled a mismatch then it will be propagated to all 
    the other nodes since mismatch is the lowest value and thus the min
    will pick it up*/
  
  DCMF_Callback_t cb_done;
  volatile int barrier_rerun_done;
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  dcmf_tp = team->dcmf_tp;  
  gasneti_assert(dcmf_tp != NULL);

  cb_done.function = increment_value;
  cb_done.clientdata = (void*)&barrier_rerun_done;

  /* Bug 2781 workaround. We need to rerun a global hardware barrier
     here to make sure everyone has arrived in finish_barrier(). This
     is to protect DCMF_Allreduce from hanging in case some nodes
     cannot enter it if they are waiting for AM replies. The barrier
     in barrier_notify() is to make sure everyone has arrived in
     barrier_notify(). */
  barrier_rerun_done = 0;
  GASNETC_DCMF_LOCK();
  GASNETC_DCMF_CHECK_PTR(&barrier_req);
  DCMF_SAFE(DCMF_GlobalBarrier(&anon_barrier_registration, &barrier_req, cb_done));
  GASNETC_DCMF_UNLOCK();
  gasneti_polluntil(barrier_rerun_done != 0);

  barrier_rerun_done = 0;
  GASNETI_TRACE_PRINTF(B, ("starting dcmf allreduce with <%llx, %llx>", 
			   named_barrier_source[0], named_barrier_source[1]));
  
  GASNETC_DCMF_LOCK();
  /*since we pass in two arguments, one which is the negation of the
    other, the first field will contain a min the second a negative of
    the max of the lfags passed in: on a normal barrier where
    everything matches these two values should be equal to each ohter
    more details on how this is handled in finish barrier*/

  if (team->total_ranks == 1)
    {
      /* In my experiments with BG/P system software version
         V1R4M1_460, DCMF_Allreduce() would generate an internal
         assertion failure when using the torus network with only 1
         node. Hence, we handle this special but simple case here as a
         workaround. */
      named_barrier_result[0] = named_barrier_source[0];
      named_barrier_result[1] = named_barrier_source[1];
      barrier_rerun_done = 1;
    }
  else 
    {
      DCMF_SAFE(DCMF_Allreduce(dcmf_tp->named_barrier_proto,
                               &dcmf_tp->named_barrier_req, 
                               cb_done, 
                               DCMF_MATCH_CONSISTENCY,
                               &dcmf_tp->geometry,
                               (char*)&named_barrier_source, 
                               (char*)&named_barrier_result, 
                               2, 
                               DCMF_SIGNED_LONG_LONG, 
                               DCMF_MIN));
    }
  GASNETC_DCMF_UNLOCK();
  
  gasneti_polluntil(barrier_rerun_done != 0);
  
  GASNETI_TRACE_PRINTF(B, ("finish barrier wait named barrier res:(0x%llx,0x%llx) (%d,%d)", 
                           named_barrier_source[1], named_barrier_result[1], id, flags));

  if(EXTRACT_BARRIER_FLAGS(named_barrier_result[0])==MISMATCH_FLAG) 
    {
      /*someone has signalled a mismatch so return mismatch on everyone*/
      
      GASNETI_TRACE_PRINTF(B, ("mismatch caught: 0x%llx",named_barrier_result[0]));
      ret = GASNET_ERR_BARRIER_MISMATCH;
    } 
  else if(named_barrier_result[0] == -named_barrier_result[1]) 
    {
      /*everyone passed same id and flags so the barrier result is good... should be the 
	normal path (could be the anonymous tag but everyone was consistent)*/
      GASNETI_TRACE_PRINTF(B, ("barriers match: 0x%llx",named_barrier_result[0]));
      ret = GASNET_OK;
    }
  else if(EXTRACT_BARRIER_FLAGS(named_barrier_result[0]) == ANON_FLAG) 
    {
      barrier_rerun_done = 0;
      /*min does not equal the max that means there is a potential mismatch*/
      /*first check that someone isn't trying to just pass in anonymous barriers 
	and named on others*/
      /*replace all threads passing anonymous with the named barrier and the max of the IDs
	and rerun the barrier (this should be an uncommon case)*/
      GASNETI_TRACE_PRINTF(B, ("caught anon rerunning: 0x%llx",named_barrier_result[0]));
      if(current_barrier_flags == GASNET_BARRIERFLAG_ANONYMOUS) {
	named_barrier_source[0] = -named_barrier_result[1];
	named_barrier_source[1] = named_barrier_result[1];
      } else {
	/*this thread didn't pass anonymous so passin what we sent in before*/
      }
      
      GASNETC_DCMF_LOCK();
      if (team->total_ranks == 1)
        {
          named_barrier_result[0] = named_barrier_source[0];
          named_barrier_result[1] = named_barrier_source[1];
          barrier_rerun_done = 1;
        }
      else 
        {
          DCMF_SAFE(DCMF_Allreduce(dcmf_tp->named_barrier_proto,
                                   &dcmf_tp->named_barrier_req, 
                                   cb_done, 
                                   DCMF_MATCH_CONSISTENCY,
                                   &dcmf_tp->geometry,
                                   (char*)&named_barrier_source, 
                                   (char*)&named_barrier_result, 
                                   2, 
                                   DCMF_SIGNED_LONG_LONG, 
                                   DCMF_MIN));
        }
      GASNETC_DCMF_UNLOCK();
      
      /* need to use gasneti_AMPoll() instead of DCMF_MESSAGER_POLL()
         so that the gasnet AM queues can make progress while
         waiting. */
      gasneti_polluntil(barrier_rerun_done != 0);
      
      /*if the reran barrier has min = max then we have a sucessful barrier*/
      /*otherwise there's a mismatch*/
      if(named_barrier_result[0] == -named_barrier_result[1]) 
        ret = GASNET_OK;
      else
        ret = GASNET_ERR_BARRIER_MISMATCH;
      
    } 
  else 
    {
      /*min does not equal max and no one tried to pass in anonymous so there
        is a mismatch*/
      GASNETI_TRACE_PRINTF(B, ("min!=max: 0x%llx 0x%llx",named_barrier_result[0], -named_barrier_result[1]));
      ret = GASNET_ERR_BARRIER_MISMATCH;
    }
  
  team->barrier_splitstate = OUTSIDE_BARRIER;
  gasneti_sync_writes();
  
  /*at this point the barrier is complete so check the flags 
    that we get and make sure they are the same as the ones
    we pass in*/
  if ((flags != current_barrier_flags)  || (id != current_barrier_id))
    ret = GASNET_ERR_BARRIER_MISMATCH; 
  
  return ret;
}

static int gasnete_dcmfbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  int ret;
  
  gasneti_sync_reads();
  if(team->barrier_splitstate == OUTSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");
  }
  


  GASNETI_TRACE_PRINTF(B, ("start barrier wait (%d,%d)", id, flags));
  /*wait for whatever barrier we executed to be done*/

  /*ampoll calls DCMF Messager advance and will make progress on outstanding AMs*/
  
  gasneti_polluntil(barrier_done!=0);
  /*while(barrier_done == 0) GASNETI_SAFE(gasneti_AMPoll());*/
  
  ret = finish_barrier(team, id, flags);
  GASNETI_TRACE_PRINTF(B, ("returning %d", ret));
  return ret;

}

static int gasnete_dcmfbarrier_try(gasnete_coll_team_t team, int id, int flags) { 
  gasneti_sync_reads();
  if(team->barrier_splitstate == OUTSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");
  }
  
  
  if(barrier_done!=1) {
    /*barrier is not done yet ... try again later*/
    GASNETI_SAFE(gasneti_AMPoll());

    /*this last call to messager advance could have finished the barrier so see if it has
      and then finish up the barrier*/
    if(barrier_done==1) {
      return finish_barrier(team, id, flags);
    } else {
      return GASNET_ERR_NOT_READY;
    }
  } else {
    return finish_barrier(team, id, flags);
  }
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

extern gasnet_handlerentry_t const *gasnete_get_handlertable(void) {
  return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

