/*
 * Description: GASNet Extended API Implementation
 * Copyright (c)  2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_mxm_req.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) {
    gasnete_iop_check(iop);
}

/* -------------------------------------------------------------------------- */

#if MXM_API < MXM_VERSION(1,5)
extern uint32_t gasnetc_find_lkey(void *addr, int nbytes);
extern uint32_t gasnetc_find_rkey(void *addr, int nbytes, int rank);
#elif MXM_API == MXM_VERSION(1,5)
extern mxm_mem_h gasnetc_find_memh(void *addr, int nbytes);
extern mxm_mem_h gasnetc_find_remote_memh(void *addr, int nbytes, int rank);
#else
#error MXM version is not supported
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

static int gasnete_mxm_max_outstanding_msgs;
#define GASNETE_MXM_MAX_OUTSTANDING_MSGS gasnete_mxm_max_outstanding_msgs
#define GASNETE_ACTIVE_MXM_GET_NUMBER (GASNETE_MYTHREAD->current_iop->initiated_get_cnt - gasneti_weakatomic_read(&(GASNETE_MYTHREAD->current_iop->completed_get_cnt),0))
#define GASNETE_ACTIVE_MXM_PUT_NUMBER (GASNETE_MYTHREAD->current_iop->initiated_put_cnt - gasneti_weakatomic_read(&(GASNETE_MYTHREAD->current_iop->completed_put_cnt),0))
#define GASNETE_ACTIVE_MXM_MSG_NUMBER (GASNETE_ACTIVE_MXM_GET_NUMBER + GASNETE_ACTIVE_MXM_PUT_NUMBER)
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
        return eop;
    }
    else { /*  free list empty - need more eops */
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
            {   int k = i+32;
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
        {   /* verify new free list got built correctly */
            int i;
            int seen[256];
            gasnete_eopaddr_t addr = thread->eop_free;

#if 0
            if (gasneti_mynode == 0)
                for (i=0; i<256; i++) {
                    fprintf(stderr,"%i:  %i: next=%i\n",gasneti_mynode,i,buf[i].addr.eopidx);
                    fflush(stderr);
                }
            sleep(5);
#endif

            gasneti_memcheck(thread->eop_bufs[bufidx]);
            memset(seen, 0, 256*sizeof(int));
            for (i=0; i<(bufidx==255?255:256); i++) {
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
    }
    else {
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
GASNETI_INLINE(gasnete_eop_isdone)
int gasnete_eop_isdone(gasnete_op_t *op) {
    gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
    gasnete_eop_check((gasnete_eop_t *)op);
    return OPSTATE(op) == OPSTATE_COMPLETE;
}

GASNETI_INLINE(gasnete_iop_isdone)
int gasnete_iop_isdone(gasnete_op_t *op) {
    gasnete_iop_t *iop = (gasnete_iop_t*)op;
    gasnete_iop_check(iop);
    return (gasneti_weakatomic_read(&(iop->completed_get_cnt), 0) == iop->initiated_get_cnt) &&
           (gasneti_weakatomic_read(&(iop->completed_put_cnt), 0) == iop->initiated_put_cnt);
}
int gasnete_op_isdone(gasnete_op_t *op) {
    gasnet_handle_t handle = (gasnet_handle_t)op;
    if (MXM_HANDLE_TYPE == handle->type) {
        return mxm_req_test(&((gasnet_mxm_send_req_t *)handle->handle)->mxm_sreq.base);
    }
    else {
        op = handle->handle;
        gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
        if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
            return gasnete_eop_isdone(op);
        }
        else {
            return gasnete_iop_isdone(op);
        }
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
    gasnet_handle_t handle = (gasnet_handle_t)op;
    if (MXM_HANDLE_TYPE == handle->type) {
        gasnetc_free_send_req(handle->handle);
    }
    else {
        gasnete_threaddata_t * const thread =
            gasnete_threadtable[((gasnete_op_t *)handle->handle)->threadidx];
        op = handle->handle;
        gasneti_assert(thread == gasnete_mythread());
        if (OPTYPE(op) == OPTYPE_EXPLICIT) {
            gasnete_eop_t *eop = (gasnete_eop_t *)op;
            gasnete_eopaddr_t addr = eop->addr;
            gasneti_assert(OPSTATE(eop) == OPSTATE_COMPLETE);
            gasnete_eop_check(eop);
            SET_OPSTATE(eop, OPSTATE_FREE);
            eop->addr = thread->eop_free;
            thread->eop_free = addr;
        } else {
            gasnete_iop_t *iop = (gasnete_iop_t *)op;
            gasnete_iop_check(iop);
            gasneti_assert(iop->next == NULL);
            iop->next = thread->iop_free;
            thread->iop_free = iop;
        }
    }
    gasneti_free(handle);
}

gasnet_handle_t gasneti_eop_to_handle(gasneti_eop_t *eop) {
    gasnet_handle_t handle = (gasnet_handle_t)gasneti_malloc(sizeof(struct gasnet_handle_t));
    handle->type = OP_HANDLE_TYPE;
    handle->handle = (gasnet_handle_t)eop;
    return handle;
}
/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

#define GASNETE_IOP_ISDONE(iop) gasnete_iop_isdone((gasnete_op_t *)(iop))
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

    /* The next two ensure nbytes in AM-based Gets will fit in handler_arg_t (bug 2770) */
    gasneti_assert_always(gasnet_AMMaxMedium() <= (size_t)0xffffffff);
    gasneti_assert_always(gasnet_AMMaxLongReply() <= (size_t)0xffffffff);
}

extern void gasnete_init(void) {
    static int firstcall = 1;
    GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
    gasneti_assert(firstcall); /*  make sure we haven't been called before */
    firstcall = 0;

    gasnete_check_config(); /*  check for sanity */

    gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

    {   gasnete_threaddata_t *threaddata = NULL;
        gasnete_eop_t *eop = NULL;
        gasnet_handle_t handle = (gasnet_handle_t)gasneti_malloc(sizeof(struct gasnet_handle_t));
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
        handle->handle = (void *)eop;
        handle->type = OP_HANDLE_TYPE;
        gasnete_op_free((gasnete_op_t *)handle);
    }

    gasnete_mxm_max_outstanding_msgs = gasneti_getenv_int_withdefault("GASNET_MXM_MAX_OUTSTANDING_MSGS", 500, 1);
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

typedef struct mxm_nbi_callback_struct {
    gasnet_mxm_send_req_t *gasnet_mxm_sreq_send;
    gasnet_mxm_send_req_t *gasnet_mxm_sreq_fence;
    gasnete_iop_t *op;
} mxm_nbi_callback_struct_t;

static void mxm_nbi_put_callback(void *mxm_callback_data)
{
    mxm_nbi_callback_struct_t *cb = (mxm_nbi_callback_struct_t *)mxm_callback_data;
    gasnetc_free_send_req(cb->gasnet_mxm_sreq_send);
    gasnetc_free_send_req(cb->gasnet_mxm_sreq_fence);
    gasnete_op_markdone((gasnete_op_t *)(cb->op), 0);
#if GASNET_DEBUG
    /* clear object's data before freeing it - might catch use after free */
    memset(cb, 0, sizeof(mxm_nbi_callback_struct_t));
#endif
    gasneti_free(cb);
}

static void mxm_nbi_get_callback(void *mxm_callback_data) {
    gasnetc_free_send_req(((mxm_nbi_callback_struct_t *)mxm_callback_data)->gasnet_mxm_sreq_send);
    gasnete_op_markdone((gasnete_op_t *)(((mxm_nbi_callback_struct_t *)mxm_callback_data)->op), 1);
    gasneti_free(mxm_callback_data);
}

GASNETI_INLINE(_single_mxm_fence)
gasnet_mxm_send_req_t * gasnete_fill_fence_request(gasnet_node_t node, void *callback_fn, void *callback_data)
{
    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    mxm_sreq->opcode = MXM_REQ_OP_FENCE;

    mxm_sreq->base.state = MXM_REQ_NEW;
    mxm_sreq->base.conn = gasnet_mxm_module.connections[node];
    mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;
    mxm_sreq->base.flags = MXM_REQ_FLAG_SEND_SYNC;

    mxm_sreq->base.data.buffer.ptr = NULL;
    mxm_sreq->base.data.buffer.length = 0;
#if MXM_API < MXM_VERSION(1,5)
    mxm_sreq->base.data.buffer.mkey = MXM_MKEY_NONE;
#elif MXM_API == MXM_VERSION(1,5)
    mxm_sreq->base.data.buffer.memh = NULL;
#else
#error MXM version is not supported
#endif
    mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;

    mxm_sreq->base.completed_cb = (void (*)(void *))callback_fn;
    mxm_sreq->base.context = callback_data;

    return gasnet_mxm_sreq;
}

static void _mxm_fence_nbi(gasnet_node_t node, void *callback_fn, void *callback_data)
{
    mxm_error_t mxm_res;
    gasnet_mxm_send_req_t* gasnet_mxm_sreq =
        gasnete_fill_fence_request(node, callback_fn, callback_data);

    ((mxm_nbi_callback_struct_t *)callback_data)->gasnet_mxm_sreq_fence = gasnet_mxm_sreq;
    mxm_res = mxm_req_send(&gasnet_mxm_sreq->mxm_sreq);
    if_pt (MXM_OK != mxm_res)
    gasneti_fatalerror("Error posting send request - %s\n",
                       mxm_error_string(mxm_res));
}

static gasnet_mxm_send_req_t* _mxm_fence_nb(gasnet_node_t node, void *callback_fn, void *callback_data)
{
    mxm_error_t mxm_res;
    gasnet_mxm_send_req_t* gasnet_mxm_sreq =
        gasnete_fill_fence_request(node, callback_fn, callback_data);

    mxm_res = mxm_req_send(&gasnet_mxm_sreq->mxm_sreq);
    if_pt (MXM_OK != mxm_res)
    gasneti_fatalerror("Error posting send request - %s\n",
                       mxm_error_string(mxm_res));
    return gasnet_mxm_sreq;
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

/* -------------------------------------------------------------------------- */

GASNETI_INLINE(gasnete_fill_get_request)
void gasnete_fill_get_request(mxm_send_req_t * mxm_sreq, void *dest,
                              gasnet_node_t node, void *src, size_t nbytes)
{
    mxm_sreq->base.state = MXM_REQ_NEW;
    mxm_sreq->base.conn = gasnet_mxm_module.connections[node];
    mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;
    mxm_sreq->base.flags = 0;

    mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;
    mxm_sreq->opcode = MXM_REQ_OP_GET;

    mxm_sreq->base.data.buffer.ptr = dest;
    mxm_sreq->base.data.buffer.length = nbytes;
    mxm_sreq->op.mem.remote_vaddr = (mxm_vaddr_t)src;

#if MXM_API < MXM_VERSION(1,5)
    mxm_sreq->base.data.buffer.mkey = gasnetc_find_lkey(dest, nbytes);
    mxm_sreq->op.mem.remote_mkey = gasnetc_find_rkey(src, nbytes, (int)node);
#elif MXM_API == MXM_VERSION(1,5)
    mxm_sreq->base.data.buffer.memh = gasnetc_find_memh(dest, nbytes);
    mxm_sreq->op.mem.remote_memh = gasnetc_find_remote_memh(src, nbytes, (int)node);
#else
#error MXM version is not supported
#endif

    mxm_sreq->base.completed_cb = NULL;
    mxm_sreq->base.context = NULL;
}

/* -------------------------------------------------------------------------- */

GASNETI_INLINE(gasnete_fill_put_request)
void gasnete_fill_put_request(mxm_send_req_t * mxm_sreq, void *dest,
                              gasnet_node_t node, void *src, size_t nbytes)
{
    mxm_sreq->base.state = MXM_REQ_NEW;
    mxm_sreq->base.conn = gasnet_mxm_module.connections[node];
    mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;
    mxm_sreq->base.flags = 0;

    mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;
    mxm_sreq->opcode = MXM_REQ_OP_PUT;

    mxm_sreq->base.data.buffer.ptr = src;
    mxm_sreq->base.data.buffer.length = nbytes;
    mxm_sreq->op.mem.remote_vaddr = (mxm_vaddr_t)dest;

#if MXM_API < MXM_VERSION(1,5)
    mxm_sreq->base.data.buffer.mkey = gasnetc_find_lkey(src, nbytes);
    mxm_sreq->op.mem.remote_mkey = gasnetc_find_rkey(dest, nbytes, (int)node);
#elif MXM_API == MXM_VERSION(1,5)
    mxm_sreq->base.data.buffer.memh = gasnetc_find_memh(src, nbytes);
    mxm_sreq->op.mem.remote_memh = gasnetc_find_remote_memh(dest, nbytes, (int)node);
#else
#error MXM version is not supported
#endif

    mxm_sreq->base.completed_cb = NULL;
    mxm_sreq->base.context = NULL;
}

/* -------------------------------------------------------------------------- */
/*
 * gasnet_get_nb
 * gasnet_get_nb_bulk
 *
 */

GASNETI_INLINE(gasnete_get_nb_inner)
gasnet_handle_t gasnete_get_nb_inner (void *dest, gasnet_node_t node,
                                      void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    gasnet_handle_t handle = (gasnet_handle_t)
                             gasneti_malloc(sizeof(struct gasnet_handle_t));
    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    mxm_error_t mxm_res;

    gasnete_fill_get_request(mxm_sreq, dest, node, src, nbytes);

    mxm_res = mxm_req_send(mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));
    handle->handle = (void *)gasnet_mxm_sreq;
    handle->type = MXM_HANDLE_TYPE;

    gasnetc_AMPoll();
    return handle;
}

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_GET_NB
#if (GASNETI_DIRECT_GET_NB)

extern gasnet_handle_t gasnete_get_nb (void *dest, gasnet_node_t node,
                                       void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(ALIGNED,H);
    return gasnete_get_nb_inner (dest, node, src, nbytes GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_GET_NB) */
#endif /* ifdef GASNETI_DIRECT_GET_NB */

/* -------------------------------------------------------------------------- */

extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node,
        void *src, size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(UNALIGNED,H);
    return gasnete_get_nb_inner (dest, node, src, nbytes GASNETE_THREAD_PASS);
}

/* -------------------------------------------------------------------------- */

GASNETI_INLINE(gasnete_put_nb_inner)
gasnet_handle_t gasnete_put_nb_inner(
    gasnet_node_t node, void *dest, void *src,
    size_t nbytes, int isbulk GASNETE_THREAD_FARG)
{
    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    gasnet_mxm_send_req_t *mxm_fence_sreq;
    mxm_error_t mxm_res;
    gasnet_handle_t handle = (gasnet_handle_t)gasneti_malloc(sizeof(struct gasnet_handle_t));

    gasnete_fill_put_request(mxm_sreq, dest, node, src, nbytes);
    mxm_sreq->base.flags = MXM_REQ_FLAG_BLOCKING;

    mxm_res = mxm_req_send(mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));

    mxm_fence_sreq = _mxm_fence_nb(node, gasnetc_free_send_req,
                                   (void *)gasnet_mxm_sreq);

    handle->handle = (void *)mxm_fence_sreq;
    handle->type = MXM_HANDLE_TYPE;

    gasnetc_AMPoll();

    return handle;
}

/* -------------------------------------------------------------------------- */
extern gasnet_handle_t gasnete_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
    GASNETI_CHECKPSHM_PUT(ALIGNED,H);
    return gasnete_put_nb_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

/* -------------------------------------------------------------------------- */
extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
    GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
    return gasnete_put_nb_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}
/* -------------------------------------------------------------------------- */

extern gasnet_handle_t gasnete_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
    GASNETI_CHECKPSHM_MEMSET(H);
    {
        gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
        gasnet_handle_t handle = (gasnet_handle_t)gasneti_malloc(sizeof(struct gasnet_handle_t));
        GASNETI_SAFE(
            SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                           (gasnet_handlerarg_t)val, PACK(nbytes),
                           PACK(dest), PACK(op))));
        handle->handle = (gasnet_handle_t)op;
        handle->type = OP_HANDLE_TYPE;
        return handle;
    }
}

/* -------------------------------------------------------------------------- */

GASNETI_INLINE(gasnete_put_inner)
void gasnete_put_inner(gasnet_node_t node, void* dest, void *src,
                       size_t nbytes GASNETE_THREAD_FARG)
{
    mxm_send_req_t mxm_sreq;
    mxm_error_t mxm_res;

    gasnete_fill_put_request(&mxm_sreq, dest, node, src, nbytes);
    mxm_sreq.base.flags = MXM_REQ_FLAG_BLOCKING;
    if (gasnet_mxm_module.strict_api)
        mxm_sreq.base.flags |= MXM_REQ_FLAG_SEND_SYNC;

    mxm_res = mxm_req_send(&mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));

    while (!mxm_req_test(&mxm_sreq.base))
        gasnetc_AMPoll();

    if (!gasnet_mxm_module.strict_api)
        gasnet_mxm_module.need_fence[node] = 1;
}

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_PUT
#if (GASNETI_DIRECT_PUT)

extern void gasnete_put(gasnet_node_t node, void* dest, void *src,
                        size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(ALIGNED,V);
    gasnete_put_inner(node, dest, src, nbytes GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_PUT) */
#endif /* ifdef GASNETI_DIRECT_PUT */

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_PUT_BULK
#if (GASNETI_DIRECT_PUT_BULK)

extern void gasnete_put_bulk(gasnet_node_t node, void* dest, void *src,
                             size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
    gasnete_put_inner(node, dest, src, nbytes GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_PUT_BULK) */
#endif /* ifdef GASNETI_DIRECT_PUT_BULK */

/* -------------------------------------------------------------------------- */

GASNETI_INLINE(gasnete_get_inner)
void gasnete_get_inner(void *dest, gasnet_node_t node, void *src,
                       size_t nbytes GASNETE_THREAD_FARG)
{
    mxm_send_req_t mxm_sreq;
    mxm_error_t mxm_res;

    gasnete_fill_get_request(&mxm_sreq, dest, node, src, nbytes);
    mxm_res = mxm_req_send(&mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));

    while (!mxm_req_test(&mxm_sreq.base))
        gasnetc_AMPoll();
}

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_GET
#if (GASNETI_DIRECT_GET)

extern void gasnete_get(void *dest, gasnet_node_t node, void *src,
                        size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(ALIGNED,V);
    gasnete_get_inner(dest, node, src, nbytes GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_GET) */
#endif /* ifdef GASNETI_DIRECT_GET */

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_GET_BULK
#if (GASNETI_DIRECT_GET_BULK)

extern void gasnete_get_bulk(void *dest, gasnet_node_t node, void *src,
                             size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(UNALIGNED,V);
    gasnete_get_inner(dest, node, src, nbytes GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_GET_BULK) */
#endif /* ifdef GASNETI_DIRECT_GET_BULK */

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnb(gasnet_handle_t handle) {
    GASNETI_SAFE(gasneti_AMPoll());

    if (gasnete_op_isdone((gasnete_op_t *)handle)) {
        gasneti_sync_reads();
        gasnete_op_free((gasnete_op_t *)handle);
        return GASNET_OK;
    }
    else return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles) {
    int success = 0;
    int empty = 1;
    GASNETI_SAFE(gasneti_AMPoll());

    gasneti_assert(phandle);

    {   int i;
        for (i = 0; i < numhandles; i++) {
            gasnet_handle_t handle = phandle[i];
            if (handle != GASNET_INVALID_HANDLE) {
                empty = 0;
                if (gasnete_op_isdone((gasnete_op_t *)handle)) {
                    gasneti_sync_reads();
                    gasnete_op_free((gasnete_op_t *)handle);
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

    {   int i;
        for (i = 0; i < numhandles; i++) {
            gasnet_handle_t handle = phandle[i];
            if (handle != GASNET_INVALID_HANDLE) {
                if (gasnete_op_isdone((gasnete_op_t *)handle)) {
                    gasneti_sync_reads();
                    gasnete_op_free((gasnete_op_t *)handle);
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

/*
 * gasnet_get_nbi
 * gasnet_get_nbi_bulk
 *
 */

GASNETI_INLINE(gasnete_get_nbi_inner)
void gasnete_get_nbi_inner (void *dest, gasnet_node_t node, void *src,
                            size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    mxm_nbi_callback_struct_t *mxm_nbi_cb_data =
        (mxm_nbi_callback_struct_t *)gasneti_malloc(sizeof(mxm_nbi_callback_struct_t));

    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    mxm_error_t mxm_res;

    op->initiated_get_cnt++;
    mxm_nbi_cb_data->gasnet_mxm_sreq_send = gasnet_mxm_sreq;
    mxm_nbi_cb_data->op = op;
    /*No fence is needed for get operation*/
    mxm_nbi_cb_data->gasnet_mxm_sreq_fence = NULL;

    gasnete_fill_get_request(mxm_sreq, dest, node, src, nbytes);
    mxm_sreq->base.completed_cb = mxm_nbi_get_callback;
    mxm_sreq->base.context = mxm_nbi_cb_data;

    if_pf (GASNETE_ACTIVE_MXM_MSG_NUMBER >= GASNETE_MXM_MAX_OUTSTANDING_MSGS) {
        do {
            gasnetc_AMPoll();
        } while (GASNETE_ACTIVE_MXM_MSG_NUMBER >= GASNETE_MXM_MAX_OUTSTANDING_MSGS);
    }
    mxm_res = mxm_req_send(mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));
    gasnetc_AMPoll();
}

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_GET_NBI
#if (GASNETI_DIRECT_GET_NBI)

extern void gasnete_get_nbi (void *dest, gasnet_node_t node, void *src,
                             size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(ALIGNED,V);
    gasnete_get_nbi_inner(dest, node, src, nbytes GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_GET_NBI) */
#endif /* ifdef GASNETI_DIRECT_GET_NBI */

/* -------------------------------------------------------------------------- */

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src,
                                  size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_GET(UNALIGNED,V);
    gasnete_get_nbi_inner(dest, node, src, nbytes GASNETE_THREAD_PASS);
}

/* -------------------------------------------------------------------------- */
GASNETI_INLINE(gasnete_put_nbi_inner)
void gasnete_put_nbi_inner(gasnet_node_t node, void *dest, void *src,
                           size_t nbytes, int isbulk GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t * const op = mythread->current_iop;
    mxm_nbi_callback_struct_t *mxm_nbi_cb_data =
        (mxm_nbi_callback_struct_t *)gasneti_malloc(sizeof(mxm_nbi_callback_struct_t));

    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    mxm_error_t mxm_res;

    op->initiated_put_cnt++;
    mxm_nbi_cb_data->gasnet_mxm_sreq_send = gasnet_mxm_sreq;
    mxm_nbi_cb_data->op = op;

    gasnete_fill_put_request(mxm_sreq, dest, node, src, nbytes);
    mxm_sreq->base.flags = MXM_REQ_FLAG_BLOCKING;

    if_pf (GASNETE_ACTIVE_MXM_MSG_NUMBER >= GASNETE_MXM_MAX_OUTSTANDING_MSGS) {
        do {
            gasnetc_AMPoll();
        } while (GASNETE_ACTIVE_MXM_MSG_NUMBER >= GASNETE_MXM_MAX_OUTSTANDING_MSGS);
    }

    mxm_res = mxm_req_send(mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));

    _mxm_fence_nbi(node, mxm_nbi_put_callback, mxm_nbi_cb_data);

    gasnetc_AMPoll();
}

/* -------------------------------------------------------------------------- */

#ifdef GASNETI_DIRECT_PUT_NBI
#if (GASNETI_DIRECT_PUT_NBI)

extern void gasnete_put_nbi (gasnet_node_t node, void *dest, void *src,
                             size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(ALIGNED,V);
    gasnete_put_nbi_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}

#endif /* if  (GASNETI_DIRECT_PUT_NBI) */
#endif /* ifdef GASNETI_DIRECT_PUT_NBI */

/* -------------------------------------------------------------------------- */

extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src,
                                  size_t nbytes GASNETE_THREAD_FARG)
{
    GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
    gasnete_put_nbi_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}

/* -------------------------------------------------------------------------- */

extern void gasnete_memset_nbi   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *op = mythread->current_iop;
    GASNETI_CHECKPSHM_MEMSET(V);

    op->initiated_put_cnt++;

    GASNETI_SAFE(
        SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
                       (gasnet_handlerarg_t)val, PACK(nbytes),
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
    gasnet_handle_t handle = (gasnet_handle_t)gasneti_malloc(sizeof(struct gasnet_handle_t));
    GASNETI_TRACE_EVENT_VAL(S,END_NBI_ACCESSREGION,iop->initiated_get_cnt + iop->initiated_put_cnt);
#if GASNET_DEBUG
    if (iop->next == NULL)
        gasneti_fatalerror("VIOLATION: call to gasnete_end_nbi_accessregion() outside access region");
#endif
    mythread->current_iop = iop->next;
    iop->next = NULL;
    handle->handle = (void *)iop;
    handle->type = OP_HANDLE_TYPE;
    return handle;
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

extern gasnet_handlerentry_t const *gasnete_get_handlertable(void) {
    return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

