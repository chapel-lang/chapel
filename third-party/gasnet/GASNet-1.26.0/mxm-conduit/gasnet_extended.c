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
extern mxm_mem_key_t *gasnetc_find_remote_mkey(void *addr, int nbytes, int rank);
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Tuning Parameters
  =================
*/

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
}

/*  allocate a new iop */
GASNETI_NEVER_INLINE(gasnete_iop_alloc,
static gasnete_iop_t * gasnete_iop_alloc(gasnete_threaddata_t * const thread)) {
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

    {   gasnete_threaddata_t *threaddata = NULL;
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

    gasnete_mxm_max_outstanding_msgs = gasneti_getenv_int_withdefault("GASNET_MXM_MAX_OUTSTANDING_MSGS", 500, 1);
    /* Initialize barrier resources */
    gasnete_barrier_init();

    /* Initialize VIS subsystem */
    gasnete_vis_init();
}

/* ------------------------------------------------------------------------------------ */
/* Make a gasnet_mxm_send_req_t act as a gasnete_op_t */

#define OPFLAG_MXM OPFLAG_CONDUIT0

GASNETI_INLINE(gasnete_sreq_to_handle) /* two call sites */
gasnet_handle_t gasnete_sreq_to_handle(gasnet_mxm_send_req_t *mop GASNETE_THREAD_FARG) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    mop->flags = OPFLAG_MXM;
    mop->threadidx = mythread->threadidx;
    return (gasnet_handle_t)mop;
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

typedef struct mxm_nbi_callback_struct {
    gasnet_mxm_send_req_t *gasnet_mxm_sreq_send;
    gasnete_iop_t *op;
} mxm_nbi_callback_struct_t;

GASNETI_INLINE(gasnete_rls_send_req)
void gasnete_rls_send_req(gasnet_mxm_send_req_t *sreq)
{
    if (gasneti_atomic_decrement_and_test(&sreq->ref_count, 0)) {
        gasnetc_free_send_req(sreq);
    }
}

static void mxm_nbi_put_callback(void *mxm_callback_data)
{
    mxm_nbi_callback_struct_t *cb = (mxm_nbi_callback_struct_t *)mxm_callback_data;

    gasnete_rls_send_req(cb->gasnet_mxm_sreq_send);
    gasnete_op_markdone((gasnete_op_t *)(cb->op), 0);
#if GASNET_DEBUG
    /* clear object's data before freeing it - might catch use after free */
    memset(cb, 0, sizeof(mxm_nbi_callback_struct_t));
#endif
    gasneti_free(cb);
}

static void mxm_nbi_get_callback(void *mxm_callback_data) {
    mxm_nbi_callback_struct_t *cb = (mxm_nbi_callback_struct_t *)mxm_callback_data;
    gasnetc_free_send_req(cb->gasnet_mxm_sreq_send);
    gasnete_op_markdone((gasnete_op_t *)(cb->op), 1);
#if GASNET_DEBUG
    /* clear object's data before freeing it - might catch use after free */
    memset(cb, 0, sizeof(mxm_nbi_callback_struct_t));
#endif
    gasneti_free(cb);
}

/* -------------------------------------------------------------------------- */

GASNETI_INLINE(gasnete_fill_get_request)
void gasnete_fill_get_request(mxm_send_req_t * mxm_sreq, void *dest,
                              gasnet_node_t node, void *src, size_t nbytes)
{
    mxm_sreq->base.state = MXM_REQ_NEW;
    mxm_sreq->base.conn = gasnet_mxm_module.connections[node];
    mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;
#if MXM_API < MXM_VERSION(2,0)
    mxm_sreq->base.flags = 0;
#else
    mxm_sreq->flags = 0;
#endif

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
    mxm_sreq->op.mem.remote_mkey = gasnetc_find_remote_mkey(src, nbytes, (int)node);
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

#if MXM_API < MXM_VERSION(2,0)
    mxm_sreq->opcode = MXM_REQ_OP_PUT;
    mxm_sreq->base.flags = MXM_REQ_FLAG_BLOCKING|MXM_REQ_FLAG_SEND_SYNC;
#else
    mxm_sreq->opcode = MXM_REQ_OP_PUT_SYNC;
    mxm_sreq->flags = MXM_REQ_SEND_FLAG_BLOCKING;
#endif
    mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;

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
    mxm_sreq->op.mem.remote_mkey = gasnetc_find_remote_mkey(dest, nbytes, (int)node);
#endif

    mxm_sreq->base.completed_cb = NULL;
    mxm_sreq->base.context = NULL;
}

/* wait till source buffer is safe for reuse */
GASNETI_INLINE(gasneti_wait)
void gasneti_wait(gasnet_mxm_send_req_t *h)
{ 
    mxm_wait_t wait;

    wait.req = &h->mxm_sreq.base;
    wait.state = (mxm_req_state_t)(MXM_REQ_SENT | MXM_REQ_COMPLETED);
    wait.progress_cb = NULL;
    wait.progress_arg = NULL;
    mxm_wait(&wait);
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
    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    mxm_error_t mxm_res;

    gasnete_fill_get_request(mxm_sreq, dest, node, src, nbytes);

    mxm_res = mxm_req_send(mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));

    gasnetc_AMPoll();
    return gasnete_sreq_to_handle(gasnet_mxm_sreq GASNETE_THREAD_PASS);
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
    size_t nbytes, const int isbulk GASNETE_THREAD_FARG)
{
    gasnet_mxm_send_req_t *gasnet_mxm_sreq = gasnetc_alloc_send_req();
    mxm_send_req_t *mxm_sreq = &gasnet_mxm_sreq->mxm_sreq;
    mxm_error_t mxm_res;

    gasnete_fill_put_request(mxm_sreq, dest, node, src, nbytes);

    mxm_res = mxm_req_send(mxm_sreq);
    if (mxm_res != MXM_OK)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));

    gasnetc_AMPoll();

    if (! isbulk) {
        gasneti_wait(gasnet_mxm_sreq);
    }

    return gasnete_sreq_to_handle(gasnet_mxm_sreq GASNETE_THREAD_PASS);
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

GASNETI_INLINE(gasnete_put_inner)
void gasnete_put_inner(gasnet_node_t node, void* dest, void *src,
                       size_t nbytes GASNETE_THREAD_FARG)
{
    mxm_send_req_t mxm_sreq;
    mxm_error_t mxm_res;

    gasnete_fill_put_request(&mxm_sreq, dest, node, src, nbytes);

    if (!gasnet_mxm_module.strict_api) {
#if MXM_API < MXM_VERSION(2,0)
        mxm_sreq.base.flags = MXM_REQ_FLAG_BLOCKING;
#else
        mxm_sreq.opcode = MXM_REQ_OP_PUT;
#endif
    }

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

/*  query an op for completeness 
 *  free it if complete
 *  returns 0 or 1 */
GASNETI_INLINE(gasnete_op_try_free)
int gasnete_op_try_free(gasnet_handle_t handle) {
    gasneti_assert(handle->threadidx == gasnete_mythread()->threadidx);

    if_pt (handle->flags == OPFLAG_MXM) {
            gasnet_mxm_send_req_t *send_req = (gasnet_mxm_send_req_t *)handle;
            if (mxm_req_test(&send_req->mxm_sreq.base)) {
                gasneti_sync_reads();
                gasnetc_free_send_req(send_req);
                return 1;
            }
    }
    else if_pt (OPTYPE(handle) == OPTYPE_EXPLICIT) {
            gasnete_eop_t *eop = (gasnete_eop_t*)handle;

            if (gasnete_eop_isdone(eop)) {
                gasneti_sync_reads();
                gasnete_eop_free(eop);
                return 1;
            }
    }
    else {
            gasnete_iop_t *iop = (gasnete_iop_t*)handle;

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

    {   int i;
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

    {   int i;
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
                           size_t nbytes, const int isbulk GASNETE_THREAD_FARG)
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

    mxm_sreq->base.completed_cb = (void (*)(void *))mxm_nbi_put_callback;
    mxm_sreq->base.context = mxm_nbi_cb_data;

    gasneti_atomic_set(&gasnet_mxm_sreq->ref_count, (isbulk ? 1 : 2), 0);

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

    if (! isbulk) {
       gasneti_wait(gasnet_mxm_sreq);
       gasnete_rls_send_req(gasnet_mxm_sreq);
    }
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

