/*   $Source: bitbucket.org:berkeleylab/gasnet.git/psm-conduit/gasnet_extended.c $
 * Description: GASNet Extended API Reference Implementation
 * Copyright (c) 2013-2015 Intel Corporation. All rights reserved.
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_core_list.h>
#include <gasnet_extended_internal.h>
#include <gasnet_extended_internal_extra.h>

#include <psm2.h>
#include <psm2_am.h>

static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

void gasnete_put_long(gasnet_node_t node, void *dest, void *src,
        size_t nbytes, gasnet_handle_t op, uint8_t isbulk GASNETE_THREAD_FARG);
void gasnete_get_long (void *dest, gasnet_node_t node, void *src,
        size_t nbytes, gasnet_handle_t op GASNETE_THREAD_FARG);


/* ------------------------------------------------------------------------------------ */
/*
  Tuning Parameters
  =================
  Conduits may choose to override the default tuning parameters below by defining them
  in their gasnet_core_fwd.h
*/

#define GASNETE_GETREQS_INCR 256

/* -------------------------------------------------------------------------- */
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
/* This routine is now used (via psm2_MARK_DONE) by long message code as well. */
/*static*/
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
GASNETI_COLD
static void gasnete_check_config(void) {
  gasneti_check_config_postattach();

  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));

  /* The next two ensure nbytes in AM-based Gets will fit in handler_arg_t (bug 2770) */
  gasneti_assert_always(gasnet_AMMaxMedium() <= (size_t)0xffffffff);
  gasneti_assert_always(gasnet_AMMaxLongReply() <= (size_t)0xffffffff);
}

GASNETI_COLD
extern void gasnete_init(void) {
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

  /* Initialize psm2-specific stuff */
  gasnetc_list_init(&gasnetc_psm_state.getreqs, 0, 0);
  gasnetc_psm_state.getreq_slab = NULL;
  gasnetc_psm_state.getreq_alloc = 0;
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

/*
 * Configuration appears in gasnet_extended_fwd.h
 */
#include "gasnet_extended_amref.c"

/* -------------------------------------------------------------------------- */

/* Internal psm2/AM message handlers */

/* Completion handler used by PUT routines to mark an op as done. */
static void gasnete_complete_markdone_put(void* op)
{
    PSM_MARK_DONE(op, 0);
}

/* Service a put request: write provided data to memory */
int gasnete_handler_put(psm2_am_token_t token,
    psm2_amarg_t* args, int nargs, void* addr, uint32_t len)
{
    void* dest_addr;

    gasneti_assert(nargs == 1);

    dest_addr = (void*)args[0].u64w0;

    GASNETE_FAST_UNALIGNED_MEMCPY(dest_addr, addr, len);

    return 0;
}

/* Service a get request: send data back to the initiator */

/* Instead of passing a destination address and op handle to the target and
   back, pass an integer offset value into an array of get request objects
   (getreqs).  The GET reply handler can use the integer offset to look up
   the destination address and op handle stored by the original get request.

   Passing <= 2 AM arguments allows messages to fit entirely in the psm2 packet
   header, resulting in lower latency.  Thus this is an important optimization.

   A slab, or array of get requests is maintained.  The integer offset value
   passed to the target and back is computed by subtracting the slab's base
   address from the get request's address.

   Since offsets are sent across the wire, the slab can be dynamically grown
   without affecting get requests that are in flight.  The slab's base address
   may change, but using realloc ensures any get request's offset into the slab
   remains the same.  Another benefit is that fewer bits are needed for an
   offset (32 or less) compared to a full address (64).

   Free get requests are tracked in a LIFO linked list.
 */

typedef struct _gasnete_getreq {
    union {
        gasnetc_item_t item;
        void *dest_addr;
    };
    void *op;
} gasnete_getreq_t;

static void gasnete_get_getreq_inner(void)
{
    /* Realloc the slab and add the new elements to the free list. */
    gasnetc_list_t *list = &gasnetc_psm_state.getreqs;
    gasnete_getreq_t *slab = gasnetc_psm_state.getreq_slab;
    int alloc_len = gasnetc_psm_state.getreq_alloc + GASNETE_GETREQS_INCR;
    int i;

    slab = gasneti_realloc(slab, alloc_len * sizeof(gasnete_getreq_t));
    gasneti_leak(slab);

    gasnetc_psm_state.getreq_slab = slab;
    gasnetc_psm_state.getreq_alloc = alloc_len;

    for (i = alloc_len - GASNETE_GETREQS_INCR; i < alloc_len - 1; i++)
        slab[i].item.next = (gasnetc_item_t *)&slab[i + 1];

    slab[alloc_len - 1].item.next = NULL;
    list->head.next = (gasnetc_item_t *)
        &slab[alloc_len - GASNETE_GETREQS_INCR];
    /* List tail is unused. */
}

static gasnete_getreq_t *gasnete_get_getreq(void)
{
    gasnetc_list_t *list = &gasnetc_psm_state.getreqs;
    gasnetc_item_t *item;

    gasneti_spinlock_lock(&list->lock);
    if_pf (list->head.next == NULL)
        gasnete_get_getreq_inner();

    item = list->head.next;
    list->head.next = item->next;
    /* List tail is unused. */
    gasneti_spinlock_unlock(&list->lock);

    return (gasnete_getreq_t *)item;
}

static void gasnete_put_getreq(gasnete_getreq_t *req)
{
    gasnetc_list_add_head(&gasnetc_psm_state.getreqs,
            (gasnetc_item_t *)req);
}

GASNETI_INLINE(gasnete_getreq_to_offset)
uint32_t gasnete_getreq_to_offset(gasnete_getreq_t *req)
{
    return (uintptr_t)req - (uintptr_t)gasnetc_psm_state.getreq_slab;
}

GASNETI_INLINE(gasnete_offset_to_getreq)
gasnete_getreq_t *gasnete_offset_to_getreq(uint32_t offset)
{
    return (gasnete_getreq_t *)
        ((uintptr_t)gasnetc_psm_state.getreq_slab + offset);
}

/* Service a get request: send data back to the initiator */
int gasnete_handler_get_request(psm2_am_token_t token,
    psm2_amarg_t* args, int nargs, void* addr, uint32_t len)
{
    psm2_error_t ret;

    /* args[0] is the target's source address
       args[1].u32w0 is the initiator's packed getreq handle (return it back)
       args[1].u32w1 is the data length (one MTU max) */
    gasneti_assert(nargs == 2);
    gasneti_assert(args[1].u32w1 <= gasnetc_psm_max_reply_len);

    /* Return the second argument as-is back to the initiator. */
    ret = psm2_am_reply_short(token,
            gasnetc_psm_state.am_handlers[AM_HANDLER_GET_REPLY],
            &args[1], 1, (void *)args[0].u64w0, args[1].u32w1,
            PSM2_AM_FLAG_NONE, NULL, NULL);
    if(ret != PSM2_OK) {
        gasneti_fatalerror("psm2_am_reply_short failure: %s\n",
                psm2_error_get_string(ret));
    }

    return 0;
}

/* Complete a get request: receive data from source */
int gasnete_handler_get_reply(psm2_am_token_t token,
    psm2_amarg_t* args, int nargs, void* addr, uint32_t len)
{
    gasnete_getreq_t *req;


    /* The only argument is an offset to a getreq */
    gasneti_assert(nargs == 1);
    req = gasnete_offset_to_getreq(args[0].u32w0);

    GASNETE_FAST_UNALIGNED_MEMCPY(req->dest_addr, addr, len);

    if(req->op != NULL)
    PSM_MARK_DONE(req->op, 1);

    gasnete_put_getreq(req);
    return 0;
}


/* -------------------------------------------------------------------------- */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
  each message sends an ack - we count the number of implicit ops launched and
  compare with the number acknowledged Another possible design would be to
  eliminate some of the acks (at least for puts) by piggybacking them on other
  messages (like get replies) or simply aggregating them the target until the
  source tries to synchronize
*/

static void gasnete_put_nbi_inner (gasnet_node_t node, void *dest, void *src,
                             size_t nbytes, uint8_t isbulk GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;

    size_t mtu_size = gasnetc_psm_max_request_len;
    size_t bytes_remaining = nbytes;
    uintptr_t src_addr = (uintptr_t)src;
    uintptr_t dest_addr = (uintptr_t)dest;
    psm2_epaddr_t epaddr = gasnetc_psm_state.peer_epaddrs[node];
    psm2_handler_t handler = gasnetc_psm_state.am_handlers[AM_HANDLER_PUT];
    psm2_error_t ret;

    gasneti_assert(node < gasneti_nodes);

    if(nbytes >= gasnetc_psm_state.long_msg_threshold) {
            op->initiated_put_cnt++;
        gasnete_put_long(node, dest, src, nbytes,
                (gasnet_handle_t)op, isbulk GASNETE_THREAD_PASS);
        return;
    }

    GASNETC_PSM_LOCK();
    while(bytes_remaining > mtu_size) {
        ret = psm2_am_request_short(epaddr, handler,
                (psm2_amarg_t*)&dest_addr, 1, (void*)src_addr, mtu_size,
                PSM2_AM_FLAG_NOREPLY, NULL, NULL);
        if_pf (ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                psm2_error_get_string(ret));
        }

        src_addr += mtu_size;
        dest_addr += mtu_size;
        bytes_remaining -= mtu_size;
    }

    op->initiated_put_cnt++;
    ret = psm2_am_request_short(epaddr, handler,
            (psm2_amarg_t*)&dest_addr, 1, (void*)src_addr, bytes_remaining,
            PSM2_AM_FLAG_NOREPLY,
            gasnete_complete_markdone_put, PSM_PACK_IOP_DONE(op,put));
    GASNETC_PSM_UNLOCK();
    if_pf (ret != PSM2_OK) {
        gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                psm2_error_get_string(ret));
    }

    gasnetc_psm_poll_periodic();
}

extern void gasnete_put_nbi (gasnet_node_t node, void *dest, void *src,
                             size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(ALIGNED,V);
    gasnete_put_nbi_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src,
                              size_t nbytes GASNETE_THREAD_FARG) {
    GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
    gasnete_put_nbi_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    uintptr_t src_addr = (uintptr_t)src;
    uintptr_t dest_addr = (uintptr_t)dest;
    size_t mtu_size = gasnetc_psm_max_reply_len;
    size_t bytes_remaining = nbytes;
    psm2_amarg_t args[2];
    gasnete_getreq_t* req;
    psm2_error_t ret;

    GASNETI_CHECKPSHM_GET(UNALIGNED,V);
    gasneti_assert(node < gasneti_nodes);

    if(nbytes >= gasnetc_psm_state.long_msg_threshold) {
        op->initiated_get_cnt++;
        gasnete_get_long(dest, node, src, nbytes,
                (gasnet_handle_t)op GASNETE_THREAD_PASS);
        return;
    }

    /* args[0] is the target's source address
       args[1].u32w0 is the initiator's packed getreq handle (return it back)
       args[1].u32w1 is the data length (one MTU max) */
    args[1].u32w1 = mtu_size;

    GASNETC_PSM_LOCK();
    while(bytes_remaining > mtu_size) {
        req = gasnete_get_getreq();
        req->dest_addr = (void *)dest_addr;
        req->op = NULL;

        args[0].u64w0 = src_addr;
        args[1].u32w0 = gasnete_getreq_to_offset(req);

        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[node],
                gasnetc_psm_state.am_handlers[AM_HANDLER_GET_REQUEST],
                args, 2, NULL, 0, PSM2_AM_FLAG_NONE, NULL, NULL);
        if_pf (ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        src_addr += mtu_size;
        dest_addr += mtu_size;
        bytes_remaining -= mtu_size;
    }

    /* Request final MTU worth of payload transfer */
    req = gasnete_get_getreq();
    req->dest_addr = (void *)dest_addr;
    req->op = op;

    args[0].u64w0 = src_addr;
    args[1].u32w0 = gasnete_getreq_to_offset(req);
    args[1].u32w1 = bytes_remaining;

    op->initiated_get_cnt++;
    ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[node],
            gasnetc_psm_state.am_handlers[AM_HANDLER_GET_REQUEST],
            args, 2, NULL, 0, PSM2_AM_FLAG_NONE, NULL, NULL);
    GASNETC_PSM_UNLOCK();
    if_pf (ret != PSM2_OK) {
        gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                psm2_error_get_string(ret));
    }

    gasnetc_psm_poll_periodic();
}


/*
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/
/* -------------------------------------------------------------------------- */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nb_bulk
     gasnete_put_nb
     gasnete_put_nb_bulk
     gasnete_memset_nb
*/

extern gasnet_handle_t gasnete_put_nb_inner(gasnet_node_t node, void *dest,
                               void *src, size_t nbytes, uint8_t isbulk GASNETE_THREAD_FARG)
{
    gasnete_eop_t* op = gasnete_eop_new(GASNETE_MYTHREAD);
    size_t mtu_size = gasnetc_psm_max_request_len;
    size_t bytes_remaining = nbytes;
    uintptr_t src_addr = (uintptr_t)src;
    uintptr_t dest_addr = (uintptr_t)dest;
    psm2_error_t ret;

    gasneti_assert(node < gasneti_nodes);

    if(nbytes >= gasnetc_psm_state.long_msg_threshold) {
        gasnete_put_long(node, dest, src, nbytes,
                (gasnet_handle_t)op, isbulk GASNETE_THREAD_PASS);
        return (gasnet_handle_t)op;
    }

    GASNETC_PSM_LOCK();
    while(bytes_remaining > mtu_size) {
        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[node],
                gasnetc_psm_state.am_handlers[AM_HANDLER_PUT],
                (psm2_amarg_t*)&dest_addr, 1, (void*)src_addr, mtu_size,
                PSM2_AM_FLAG_NOREPLY, NULL, NULL);
        if_pf (ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        src_addr += mtu_size;
        dest_addr += mtu_size;
        bytes_remaining -= mtu_size;
    }

    ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[node],
            gasnetc_psm_state.am_handlers[AM_HANDLER_PUT],
            (psm2_amarg_t*)&dest_addr, 1, (void*)src_addr, bytes_remaining,
            PSM2_AM_FLAG_NONE,
            gasnete_complete_markdone_put, PSM_PACK_EOP_DONE(op));
    GASNETC_PSM_UNLOCK();
    if_pf (ret != PSM2_OK) {
        gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                psm2_error_get_string(ret));
    }

    gasnetc_psm_poll_periodic();
    return (gasnet_handle_t)op;
}

extern gasnet_handle_t gasnete_put_nb(gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(ALIGNED,H);
    return gasnete_put_nb_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
    GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
    return gasnete_put_nb_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}


extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
    gasnete_eop_t* op;
    uintptr_t src_addr = (uintptr_t)src;
    uintptr_t dest_addr = (uintptr_t)dest;
    size_t mtu_size;
    size_t bytes_remaining = nbytes;
    psm2_amarg_t args[2];
    gasnete_getreq_t* req;
    psm2_error_t ret;

    GASNETI_CHECKPSHM_GET(UNALIGNED,H);
    gasneti_assert(node < gasneti_nodes);

    op = gasnete_eop_new(GASNETE_MYTHREAD);
    mtu_size = gasnetc_psm_max_reply_len;

    if(nbytes >= gasnetc_psm_state.long_msg_threshold) {
        gasnete_get_long(dest, node, src, nbytes,
                (gasnet_handle_t)op GASNETE_THREAD_PASS);
        return (gasnet_handle_t)op;
    }

    /* args[0] is the target's source address
       args[1].u32w0 is the initiator's packed getreq handle (return it back)
       args[1].u32w1 is the data length (one MTU max) */
    args[1].u32w1 = mtu_size;

    GASNETC_PSM_LOCK();
    while(bytes_remaining > mtu_size) {
        req = gasnete_get_getreq();
        req->dest_addr = (void *)dest_addr;
        req->op = NULL;

        args[0].u64w0 = src_addr;
        args[1].u32w0 = gasnete_getreq_to_offset(req);

        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[node],
                gasnetc_psm_state.am_handlers[AM_HANDLER_GET_REQUEST],
                args, 2, NULL, 0, PSM2_AM_FLAG_NONE, NULL, NULL);
        if_pf (ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        src_addr += mtu_size;
        dest_addr += mtu_size;
        bytes_remaining -= mtu_size;
    }

    /* Request final MTU worth of payload transfer */
    req = gasnete_get_getreq();
    req->dest_addr = (void *)dest_addr;
    req->op = op;

    args[0].u64w0 = src_addr;
    args[1].u32w0 = gasnete_getreq_to_offset(req);
    args[1].u32w1 = bytes_remaining;

    ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[node],
            gasnetc_psm_state.am_handlers[AM_HANDLER_GET_REQUEST],
            args, 2, NULL, 0, PSM2_AM_FLAG_NONE, NULL, NULL);
    GASNETC_PSM_UNLOCK();
    if_pf (ret != PSM2_OK) {
        gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                psm2_error_get_string(ret));
    }

    gasnetc_psm_poll_periodic();
    return (gasnet_handle_t)op;
}

/* -------------------------------------------------------------------------- */
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
