/*
 * Description: GASNet MXM conduit implementation
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_mxm_req.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <alloca.h>

#define GASNET_DEBUG_AM 0
#define GASNET_USE_MXM_SELF_PTL 0

#define GASNET_USE_ZCOPY 1


/* -------------------------------------------------------------------------- */

#if GASNET_SEGMENT_FAST
#define GASNET_ADDR_IN_RANGE(seg_addr, seg_size, addr, len)        \
            ( ( ((uintptr_t)seg_addr) <= ((uintptr_t)addr) ) &&    \
              ( ((uintptr_t)seg_addr) + (uintptr_t)seg_size >=     \
                    (((uintptr_t)addr) + (uintptr_t)len) ) )
#endif

/* -------------------------------------------------------------------------- */

void gasnetc_free_send_req(void * p_sreq)
{
    gasnet_mxm_send_req_t * sreq = (gasnet_mxm_send_req_t *) p_sreq;

    gasneti_assert(sreq && "bad sreq to free");
#if GASNET_DEBUG
    /* clear object's data before freeing it - might catch use after free */
    memset(sreq, 0, sizeof(gasnet_mxm_send_req_t));
#endif
    gasneti_free(sreq);
}

/* -------------------------------------------------------------------------- */

gasnet_mxm_send_req_t * gasnetc_alloc_send_req(void)
{
    gasnet_mxm_send_req_t * sreq = (gasnet_mxm_send_req_t *)
#if GASNET_DEBUG
                                   gasneti_calloc(1, sizeof(gasnet_mxm_send_req_t));
#else
                                   gasneti_malloc(sizeof(gasnet_mxm_send_req_t));
#endif
    gasneti_assert(sreq && "Out of memory");
    return sreq;
}

/* -------------------------------------------------------------------------- */

extern gasnet_mxm_recv_req_t * gasnetc_alloc_recv_req(void)
{
    gasnet_mxm_recv_req_t * r;
    size_t size = GASNETI_PAGE_ALIGNUP(gasneti_AMMaxMedium());
    void * buf; 

    r = gasneti_malloc(sizeof(*r));
    gasneti_assert(r && "Out of memory");

    buf = gasneti_mmap(size);
    gasneti_assert(buf && "Out of memory");

    r->mxm_rreq.base.data.buffer.ptr = buf;
    r->mxm_rreq.base.data.buffer.length = size;

    return r;
}

/* -------------------------------------------------------------------------- */

void gasnetc_free_recv_req(gasnet_mxm_recv_req_t *p_rreq)
{
    gasneti_munmap(p_rreq->mxm_rreq.base.data.buffer.ptr, p_rreq->mxm_rreq.base.data.buffer.length);
    gasneti_free(p_rreq);
}

/* -------------------------------------------------------------------------- */

#if (1)
#define GASNETC_LONG_ASYNC_REQ(category, is_req, is_sync) \
                    (((category) == gasnetc_Long) && (is_req) && !(is_sync))
#else
#define GASNETC_LONG_ASYNC_REQ(category, is_req, is_sync) 0
#endif

/* -------------------------------------------------------------------------- */

static inline void gasnetc_ReqRepWait(mxm_send_req_t * mxm_sreq,
                                      uint8_t is_request, uint8_t msg_num)
{
    if (is_request) {
        /*
         * Ensure progress, but only on requester side (even on async request).
         * We don't want to have recursion on responder side - otherwise
         * send might be called from context of receive and so on.
         */
#if GASNET_DEBUG_AM
        MXM_LOG("[pid %d] polling and testing(%s, msg_num = 0x%02x)...\n",
                getpid(), is_request ? "request" : "response", msg_num);
#endif

        while (!mxm_req_test(&mxm_sreq->base))
            gasnetc_AMPoll();

#if GASNET_DEBUG_AM
        MXM_LOG("[pid %d] done polling and testing(%s, msg_num = 0x%02x)...\n",
                getpid(), is_request ? "request" : "response", msg_num);
#endif
    } else {
        mxm_wait_t wait;
        wait.progress_cb = NULL;
        wait.progress_arg = NULL;
        wait.req = &mxm_sreq->base;
        wait.state = MXM_REQ_COMPLETED;
        /*
         * We're on the responder side - just wait for completion.
         */
#if GASNET_DEBUG_AM
        MXM_LOG("[pid %d] calling mxm_req_wait(%s, msg_num = 0x%02x)...\n",
                getpid(), is_request ? "request" : "response", msg_num);
#endif

        mxm_wait(&wait);

#if GASNET_DEBUG_AM
        MXM_LOG("[pid %d] mxm_req_wait(%s, msg_num = 0x%02x) returned\n",
                getpid(), is_request ? "request" : "response", msg_num);
#endif
    }
}

/* -------------------------------------------------------------------------- */

static inline void gasnetc_PostSend(mxm_send_req_t * mxm_sreq,
                                    uint8_t is_request,
                                    uint8_t block,
                                    uint8_t msg_num)
{
    mxm_error_t mxm_res;

#if GASNET_DEBUG_AM
    MXM_LOG("[pid %d] calling mxm_req_send()...\n", getpid());
#endif

    mxm_res = mxm_req_send(mxm_sreq);

#if GASNET_DEBUG_AM
    MXM_LOG("[pid %d] mxm_req_send() returned %d\n", getpid(), (int)mxm_res);
#endif

    if_pf (mxm_res != MXM_OK)
    gasneti_fatalerror("Error posting send request - %s\n",
                       mxm_error_string(mxm_res));

    if_pt (block)
    gasnetc_ReqRepWait(mxm_sreq, is_request, msg_num);
}

/* -------------------------------------------------------------------------- */

extern void gasnetc_barrier_fence(void)
{
    gasnet_mxm_send_req_t * sreqs;
    mxm_send_req_t * mxm_sreq;
    mxm_error_t mxm_res;
    int dest;

    sreqs = (gasnet_mxm_send_req_t *)
            alloca(sizeof(gasnet_mxm_send_req_t) * gasneti_nodes);

    for (dest = 0; dest < gasneti_nodes; dest++) {
        if (!gasnet_mxm_module.need_fence[dest])
            continue;
        if_pf (dest == gasneti_mynode)
        continue;
#if GASNET_PSHM
        if_pf (gasneti_pshm_in_supernode(dest))
        continue;
#endif
        mxm_sreq = &sreqs[dest].mxm_sreq;
#if MXM_API < MXM_VERSION(2,0)
        mxm_sreq->opcode = MXM_REQ_OP_FENCE;
        mxm_sreq->base.flags = MXM_REQ_FLAG_SEND_SYNC;
#else
        mxm_sreq->opcode = MXM_REQ_OP_PUT_SYNC;
        mxm_sreq->flags  = MXM_REQ_SEND_FLAG_FENCE;
        mxm_sreq->op.mem.remote_vaddr = 0;
        mxm_sreq->op.mem.remote_mkey  = &mxm_empty_mem_key;
#endif

        mxm_sreq->base.state = MXM_REQ_NEW;
        mxm_sreq->base.conn = gasnet_mxm_module.connections[dest];
        mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;

        mxm_sreq->base.data.buffer.ptr = NULL;
        mxm_sreq->base.data.buffer.length = 0;

#if MXM_API < MXM_VERSION(1,5)
        mxm_sreq->base.data.buffer.mkey = MXM_MKEY_NONE;
#else
        mxm_sreq->base.data.buffer.memh = NULL;
#endif
        mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;

        mxm_sreq->base.completed_cb = NULL;
        mxm_sreq->base.context = NULL;

        mxm_res = mxm_req_send(mxm_sreq);
        if_pt (MXM_OK != mxm_res)
        gasneti_fatalerror("Error posting send request - %s\n",
                           mxm_error_string(mxm_res));
    }

    for (dest = 0; dest < gasneti_nodes; dest++) {
        if (!gasnet_mxm_module.need_fence[dest])
            continue;
        if_pf (dest == gasneti_mynode)
        continue;
#if GASNET_PSHM
        if_pf (gasneti_pshm_in_supernode(dest))
        continue;
#endif
        mxm_sreq = &sreqs[dest].mxm_sreq;

        /* we are waiting for real completion, not just for SENT state */
        while (!mxm_req_test(&mxm_sreq->base))
            gasnetc_AMPoll();

        gasnet_mxm_module.need_fence[dest] = 0;
    }
}

/* -------------------------------------------------------------------------- */

#if GASNET_SEGMENT_FAST
extern int gasnetc_max_regs;
static gasnetc_memreg_t * gasnetc_find_reg(void *src_addr, int nbytes, int rank)
{
#if GASNET_USE_ZCOPY
    int i;
    int rank_reg_offset = gasnetc_max_regs * rank;

    if ((!GASNET_ADDR_IN_RANGE(gasneti_seginfo[rank].addr,
                                   gasneti_seginfo[rank].size,
                                   src_addr, nbytes)) )
        return NULL;

    /* Small optimization - try the first entry before going into loop.
     * Usually the first registered segment will contain the data. */
    if (GASNET_ADDR_IN_RANGE(gasnet_mxm_module.reg[rank_reg_offset].addr,
                             gasnet_mxm_module.reg[rank_reg_offset].len,
                             src_addr, nbytes))
        return &gasnet_mxm_module.reg[rank_reg_offset];

    for (i = 1; i < gasnetc_max_regs; i++) {
        if (!gasnet_mxm_module.reg[i + rank_reg_offset].addr)
            return NULL;
        if (GASNET_ADDR_IN_RANGE(gasnet_mxm_module.reg[i + rank_reg_offset].addr,
                                 gasnet_mxm_module.reg[i + rank_reg_offset].len,
                                 src_addr, nbytes))
            return &gasnet_mxm_module.reg[i + rank_reg_offset];
    }
#endif /* ZCOPY */
    return NULL;
}
#endif /* SEGMENT_FAST */

/* -------------------------------------------------------------------------- */

#if MXM_API < MXM_VERSION(1,5)

inline uint32_t gasnetc_find_lkey(void *src_addr, int nbytes)
{
#if GASNET_SEGMENT_FAST
    gasnetc_memreg_t * reg;

    if (nbytes < gasnet_mxm_module.zcopy_thresh)
        return NULL;

    reg = gasnetc_find_reg(src_addr, nbytes, gasnet_mynode());
    return (reg) ? reg->lkey : MXM_MKEY_NONE;
#else
    return MXM_MKEY_NONE;
#endif
}

inline uint32_t gasnetc_find_rkey(void *src_addr, int nbytes, int rank)
{
    return MXM_MKEY_NONE;
}

#elif MXM_API < MXM_VERSION(2,0)

inline mxm_mem_h gasnetc_find_memh(void *addr, int nbytes)
{
#if GASNET_SEGMENT_FAST
    gasnetc_memreg_t * reg;

    if (nbytes < gasnet_mxm_module.zcopy_thresh)
        return NULL;

    reg = gasnetc_find_reg(addr, nbytes, gasnet_mynode());
    return (reg) ? reg->memh : NULL;
#else
    return NULL;
#endif
}

inline mxm_mem_h gasnetc_find_remote_memh(void *addr, int nbytes, int rank)
{
    return NULL; /* No zcopy on receive side, so don't bother */
}

#else

inline mxm_mem_key_t *gasnetc_find_remote_mkey(void *addr, int nbytes, int rank)
{
#if (GASNET_SEGMENT_FAST)
    gasnetc_memreg_t * reg = gasnetc_find_reg(addr, nbytes, rank);
    return  reg ? &reg->m_key : &mxm_empty_mem_key ;
#else
    return  &mxm_empty_mem_key;
#endif
}


#endif

/* -------------------------------------------------------------------------- */

int gasnetc_AM_Generic(gasnetc_category_t category,
                       int dest,
                       gasnet_handler_t handler_id,
                       void *src_addr,
                       int nbytes,
                       void *dst_addr,
                       uint8_t is_request,
                       uint8_t is_sync_request,
                       uint8_t msg_num,
                       int numargs, va_list argptr)
{
    gasnet_mxm_send_req_t sreq;
    gasnet_mxm_send_req_t * p_sreq;
    mxm_send_req_t * mxm_sreq;
    int i, sge_idx = 0;

    /*
     * Encoding the following metadata in the immediate field:
     *  - AM handler ID (1 byte)
     *  - number of args (1 byte)
     *  - message category (2 bits)
     *  - whether the message is a request (1 bit)
     *  - whether the message is a synchronous request (1 bit)
     *  - message number (1 byte)
     */
    mxm_imm_t imm_data = (mxm_imm_t) GASNETC_MSG_METADATA(
                             handler_id, numargs, category,
                             is_request, is_sync_request,
                             SYSTEM_MSG_NA, msg_num);
    /* using source node ID as tag */
    mxm_tag_t tag = (mxm_tag_t) gasnet_mynode();

    if_pf (GASNETC_LONG_ASYNC_REQ(category, is_request, is_sync_request))
        p_sreq = gasnetc_alloc_send_req();
    else
        p_sreq = &sreq;

    mxm_sreq = &p_sreq->mxm_sreq;

#if GASNET_DEBUG_AM
    MXM_LOG("[msg 0x%02x%02x] [pid %d] %s %s%s%s%s, %d --> %d, nbytes = %d, numargs = %d\n",
            (is_request) ? (uint8_t)gasneti_mynode : (uint8_t)dest,
            msg_num, getpid(), am_category_str[category],
            (is_request && is_sync_request) ? "sync request" : "",
            (is_request && !is_sync_request) ? "ASYNC request" : "",
            (!is_request && is_sync_request) ? "response to sync request" : "",
            (!is_request && !is_sync_request) ? "response to ASYNC request" : "",
            (uint32_t)gasnet_mynode(), (uint32_t)dest, nbytes, numargs);
#endif

    gasneti_assert((category & ~3) == 0);
    gasneti_assert(numargs <= GASNETC_MAX_ARGS);

    /*
     * From a sender's point of view, the request and reply functions block
     * until the message is sent. A message is defined to be sent once it is
     * safe for the caller to reuse the memory containing the message (except
     * for gasnet_RequestLongAsyncM() case).
     *
     * In implementations which copy or buffer messages for transmission,
     * the definition still holds: message sent means the layer has copied
     * the message and promises to deliver the copy with its "best effort",
     * and the original message storage may be reused.
     *
     * By best effort, the message layer promises it will take care of all
     * the details necessary to transmit the message. These details include
     * any retransmission attempts and buffering issues on unreliable networks.
     *
     * However, in either case, sent does not imply received. Once control
     * returns from a request or reply function, clients cannot assume that
     * the message has been received and handled at the destination.
     * The message layer only guarantees that if a request or reply is sent,
     * and, if the receiver occasionally polls for arriving messages, then
     * the message will eventually be received and handled.
     *
     * From a receiver's point of view, a message is defined to be received
     * only once its handler function is invoked.
     */

    mxm_sreq->base.state = MXM_REQ_NEW;
    mxm_sreq->base.conn = gasnet_mxm_module.connections[dest];
    mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;

#if MXM_API < MXM_VERSION(2,0)
    mxm_sreq->base.flags = 0;

    /*
     * FLAG_BLOCKING tells MXM to free user's buffer ASAP.
     * This does NOT mean that the call will be blocked,
     * but it will affect MXM decision w.r.t. how to send
     * the message. E.g. MXM will not use ZCOPY.
     */
    mxm_sreq->base.flags |= MXM_REQ_FLAG_BLOCKING;
#else
    mxm_sreq->flags = MXM_REQ_SEND_FLAG_BLOCKING;
#endif

    mxm_sreq->base.completed_cb = NULL;
    mxm_sreq->base.context = NULL;

    mxm_sreq->opcode = MXM_REQ_OP_SEND;
    mxm_sreq->op.send.imm_data = imm_data;
    mxm_sreq->op.send.tag = tag;

    /*
     * Create a contiguous buffer of parameters for the handler function
     */
    if (numargs) {
        for (i = 0; i < numargs; i++) {
            p_sreq->args_buf[i] =
                (gasnet_handlerarg_t) va_arg(argptr, gasnet_handlerarg_t);
        }
    }

    switch (category) {

    case gasnetc_System:
    case gasnetc_Short: {

        if_pt (numargs) {
            mxm_sreq->base.data.buffer.ptr = p_sreq->args_buf;
            mxm_sreq->base.data.buffer.length = numargs * sizeof(gasnet_handlerarg_t);
        }
        else {
            mxm_sreq->base.data.buffer.ptr = NULL;
            mxm_sreq->base.data.buffer.length = 0;
        }

#if MXM_API < MXM_VERSION(1,5)
        mxm_sreq->base.data.buffer.mkey = MXM_MKEY_NONE;
#else
        mxm_sreq->base.data.buffer.memh = NULL;
#endif

        mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;

        gasnetc_PostSend(mxm_sreq, is_request, 1, msg_num);
    }
    break;

    case gasnetc_Medium: {

        if_pt (numargs) {
            p_sreq->sendiov[sge_idx].ptr = p_sreq->args_buf;
            p_sreq->sendiov[sge_idx].length = GASNETI_ALIGNUP(
                                                  numargs * sizeof(gasnet_handlerarg_t),
                                                  GASNETI_MEDBUF_ALIGNMENT);
#if MXM_API < MXM_VERSION(1,5)
            p_sreq->sendiov[sge_idx].mkey = MXM_MKEY_NONE;
#else
            p_sreq->sendiov[sge_idx].memh = NULL;
#endif
            sge_idx++;
        }

        if_pt (nbytes && src_addr) {
            p_sreq->sendiov[sge_idx].ptr = src_addr;
            p_sreq->sendiov[sge_idx].length = nbytes;
#if MXM_API < MXM_VERSION(1,5)
            p_sreq->sendiov[sge_idx].mkey = gasnetc_find_lkey(src_addr, nbytes);
#elif MXM_API == MXM_VERSION(1,5)
            p_sreq->sendiov[sge_idx].memh = gasnetc_find_memh(src_addr, nbytes);
#endif
            sge_idx++;
        }

        mxm_sreq->base.data_type = MXM_REQ_DATA_IOV;
        mxm_sreq->base.data.iov.vector = p_sreq->sendiov;
        mxm_sreq->base.data.iov.count = sge_idx;

        gasnetc_PostSend(mxm_sreq, is_request, 1, msg_num);
    }
    break;

    case gasnetc_Long: {

        gasnet_mxm_send_req_t put_sreq;
        gasnet_mxm_send_req_t * p_put_sreq = &put_sreq;
        mxm_send_req_t * put_mxm_sreq = NULL;

        if_pt (nbytes && src_addr && dst_addr) {

            if_pf (GASNETC_LONG_ASYNC_REQ(category, is_request, is_sync_request))
                p_put_sreq = gasnetc_alloc_send_req();

            put_mxm_sreq = &p_put_sreq->mxm_sreq;

            put_mxm_sreq->base.state = MXM_REQ_NEW;
            put_mxm_sreq->base.conn = gasnet_mxm_module.connections[dest];
            put_mxm_sreq->base.mq = gasnet_mxm_module.mxm_mq;
#if MXM_API < MXM_VERSION(2,0)
            put_mxm_sreq->base.flags = MXM_REQ_FLAG_BLOCKING;
#else
            put_mxm_sreq->flags = MXM_REQ_SEND_FLAG_BLOCKING;
#endif

            put_mxm_sreq->base.data_type = MXM_REQ_DATA_BUFFER;
            put_mxm_sreq->opcode = MXM_REQ_OP_PUT;

            put_mxm_sreq->base.data.buffer.ptr = src_addr;
            put_mxm_sreq->base.data.buffer.length = nbytes;
            put_mxm_sreq->op.mem.remote_vaddr = (mxm_vaddr_t)dst_addr;
#if MXM_API < MXM_VERSION(1,5)
            put_mxm_sreq->base.data.buffer.mkey = gasnetc_find_lkey(src_addr, nbytes);
            put_mxm_sreq->op.mem.remote_mkey = gasnetc_find_rkey(dst_addr, nbytes, dest);
#elif MXM_API == MXM_VERSION(1,5)
            put_mxm_sreq->base.data.buffer.memh = gasnetc_find_memh(src_addr, nbytes);
            put_mxm_sreq->op.mem.remote_memh = gasnetc_find_remote_memh(dst_addr, nbytes, dest);
#else
            put_mxm_sreq->op.mem.remote_mkey = gasnetc_find_remote_mkey(dst_addr, nbytes, dest);
#endif

            /*
             * If this is a blocking send, then MXM doesn't have to call
             * any callback to let gasnet know that sreq can be freed.
             * Otherwise, provide the callback and context.
             */
            if_pf (GASNETC_LONG_ASYNC_REQ(category, is_request, is_sync_request)) {
                put_mxm_sreq->base.completed_cb = gasnetc_free_send_req;
                put_mxm_sreq->base.context = p_put_sreq;
            }
            else {
                put_mxm_sreq->base.completed_cb = NULL;
                put_mxm_sreq->base.context = NULL;
            }

            /*
             * Post PUT.
             * NOTE: This operation will ALWAYS be async - we don't care
             * about PUT completion. If synchronization is needed, it
             * should be done through the subsequent SEND operation.
             */
            gasnetc_PostSend(put_mxm_sreq, is_request, 0, msg_num);
        }

        /* Now do usual SEND operation.
         * Send scatter-gather list of up to 2 entries:
         *   1. Parameters buffer (if parameters provided)
         *   2. Destination address and data length of the PUT operation
         */
        if_pt (numargs) {
            p_sreq->sendiov[sge_idx].ptr = p_sreq->args_buf;
            p_sreq->sendiov[sge_idx].length = numargs * sizeof(gasnet_handlerarg_t);
#if MXM_API < MXM_VERSION(1,5)
            p_sreq->sendiov[sge_idx].mkey = MXM_MKEY_NONE;
#else
            p_sreq->sendiov[sge_idx].memh = NULL;
#endif
            sge_idx++;
        }

        p_sreq->long_info[0] = (uint64_t) dst_addr;
        p_sreq->long_info[1] = (uint64_t) nbytes;
        p_sreq->sendiov[sge_idx].ptr = p_sreq->long_info;
        p_sreq->sendiov[sge_idx].length = sizeof(p_sreq->long_info);
#if MXM_API < MXM_VERSION(1,5)
        p_sreq->sendiov[sge_idx].mkey = MXM_MKEY_NONE;
#else
        p_sreq->sendiov[sge_idx].memh = NULL;
#endif
        sge_idx++;

        mxm_sreq->base.data_type = MXM_REQ_DATA_IOV;
        mxm_sreq->base.data.iov.vector = p_sreq->sendiov;
        mxm_sreq->base.data.iov.count = sge_idx;

        if_pf (GASNETC_LONG_ASYNC_REQ(category, is_request, is_sync_request)) {
            mxm_sreq->base.completed_cb = gasnetc_free_send_req;
            mxm_sreq->base.context = p_sreq;
        }

        gasnetc_PostSend(mxm_sreq, is_request,
                         !GASNETC_LONG_ASYNC_REQ(category, is_request, is_sync_request),
                         msg_num);

        /*
         * If it was a sync Long message, then once we returned here,
         * we know that the operation is over - SEND completed.
         * So the only thing that is left to do for blocking Long
         * message is clean the already completed PUT request.
         *
         * **NOTE**: one would think that since the operations are
         * done in-order, then there is no need to wait for completion
         * of previous operation (PUT) when the next operation (SEND)
         * is over. However, I see cases when the *completions* for
         * these operations are not processed in order, so if the code
         * would assume that PUT is completed and exit w/o waiting for
         * PUT, MXM will sometimes try to process the completion after
         * this function is already exited and destroyed put_mxm_sreq,
         * which is a local variable (not good...).
         */
        if_pt (put_mxm_sreq &&
               !(GASNETC_LONG_ASYNC_REQ(category, is_request, is_sync_request))) {
            gasnetc_ReqRepWait(put_mxm_sreq, is_request, msg_num);
        }
    }
    break;

    default:
        gasneti_fatalerror("invalid message category");
    }

    GASNETI_RETURN(GASNET_OK);
}

/* -------------------------------------------------------------------------- */

#if !GASNET_USE_MXM_SELF_PTL
void gasnetc_ProcessRecvSelf(gasnetc_category_t category,
                             gasnet_handler_t handler_id,
                             void *src_addr,
                             int nbytes,
                             void *dst_addr,
                             uint8_t is_request,
                             uint8_t is_sync_request,
                             uint8_t msg_num,
                             int numargs, gasnet_handlerarg_t *args)
{
    gasnetc_am_token_t token;
    gasnetc_am_token_t * p_token = &token;
    uint8_t sys_msg_type;

    gasneti_handler_fn_t handler_fn = gasnetc_handler[handler_id];;

    token.src_node        = gasneti_mynode;
    token.is_request      = is_request;
    token.is_sync_request = is_sync_request;
    token.msg_num         = msg_num;
    numargs               = numargs;
    sys_msg_type          = SYSTEM_MSG_NA;

    switch (category) {
    case gasnetc_Short: {
        GASNETI_RUN_HANDLER_SHORT(token.is_request,
                                  handler_id, handler_fn,
                                  p_token,
                                  args, numargs);
    }
    break;

    case gasnetc_Medium: {
        void *med_buf = NULL;
        void *al_buf = NULL;
        if (src_addr && nbytes) {
#if GASNETI_USE_ALLOCA
            med_buf = alloca(nbytes + GASNETI_MEDBUF_ALIGNMENT);
#else
            med_buf = gasneti_malloc(nbytes + GASNETI_MEDBUF_ALIGNMENT);
#endif
            al_buf = (void*)GASNETI_ALIGNUP(med_buf, GASNETI_MEDBUF_ALIGNMENT);

            GASNETE_FAST_UNALIGNED_MEMCPY(al_buf, src_addr, nbytes);
        }
        GASNETI_RUN_HANDLER_MEDIUM(token.is_request,
                                   handler_id, handler_fn,
                                   p_token,
                                   args, numargs,
                                   al_buf, nbytes);
#if !GASNETI_USE_ALLOCA
        if (med_buf)
            gasneti_free(med_buf);
#endif
    }
    break;

    case gasnetc_Long: {
        if_pt(src_addr && dst_addr && nbytes)
            memcpy(dst_addr, src_addr, nbytes);

        GASNETI_RUN_HANDLER_LONG(token.is_request,
                                 handler_id, handler_fn,
                                 p_token,
                                 args, numargs,
                                 dst_addr, nbytes);
    }
    break;

    case gasnetc_System:
        /* falling through - system messages should go through MXM */
    default:
        gasneti_fatalerror("invalid message category");
    }

}
#endif

/* -------------------------------------------------------------------------- */

#if !GASNET_USE_MXM_SELF_PTL
int gasnetc_AM_Generic_Self(gasnetc_category_t category,
                            gasnet_handler_t handler_id,
                            void *src_addr,
                            int nbytes,
                            void *dst_addr,
                            uint8_t is_request,
                            uint8_t is_sync_request,
                            uint8_t msg_num,
                            int numargs, va_list argptr)
{
    gasnet_handlerarg_t args_buf[GASNETC_MAX_ARGS];

#if GASNET_DEBUG_AM
    MXM_LOG("[msg 0x%02x%02x] [pid %d] %s %s%s%s%s, %d --> %d, nbytes = %d, numargs = %d\n",
            (uint8_t)gasneti_mynode,
            msg_num, getpid(), am_category_str[category],
            (is_request && is_sync_request) ? "sync request" : "",
            (is_request && !is_sync_request) ? "ASYNC request" : "",
            (!is_request && is_sync_request) ? "response to sync request" : "",
            (!is_request && !is_sync_request) ? "response to ASYNC request" : "",
            (uint32_t)gasnet_mynode(), (uint32_t)gasnet_mynode(), nbytes, numargs);
#endif

    gasneti_assert((category & ~3) == 0);
    gasneti_assert(numargs <= GASNETC_MAX_ARGS);

    /*
     * Create a contiguous buffer of parameters for the handler function
     */
    if (numargs) {
        int i;
        for (i = 0; i < numargs; i++) {
            args_buf[i] =
                (gasnet_handlerarg_t) va_arg(argptr, gasnet_handlerarg_t);
        }
    }

    gasnetc_ProcessRecvSelf(category, handler_id,
                            nbytes ? src_addr : NULL,
                            nbytes, dst_addr,
                            is_request, is_sync_request, msg_num,
                            numargs,
                            numargs ? args_buf : NULL);

    GASNETI_RETURN(GASNET_OK);
}
#endif

/* -------------------------------------------------------------------------- */

int gasnetc_RequestGeneric(gasnetc_category_t category,
                           int dest,
                           gasnet_handler_t handler,
                           void *src_addr,
                           int nbytes,
                           void *dst_addr,
                           uint8_t is_sync,
                           int numargs, va_list argptr)
{
    static uint8_t msg_num =  0;
    msg_num = (msg_num + 1) % 255;

    /*
     * Right now msg_num is used for debug perposes only,
     * so that we'll be able to track requests and correlating
     * response.
     * It may be used later to support better tracking
     * of long async messages.
     */

#if !GASNET_PSHM
#if !GASNET_USE_MXM_SELF_PTL
    /*
     * PSHM already checked if the source and destination
     * are in the same supernode - do it now for cases where
     * PSHM is disabled.
     * Note: system messages will go the usual way.
     */
    if_pf(gasneti_mynode == dest && category != gasnetc_System)
        return gasnetc_AM_Generic_Self(category, handler,
                                       src_addr, nbytes,
                                       dst_addr,
                                       1, /* 1 for request, 0 for reply */
                                       is_sync, /* is it a syncronous request */
                                       msg_num,
                                       numargs, argptr);
    else
#endif
#endif
        return gasnetc_AM_Generic(category, dest, handler,
                                  src_addr, nbytes,
                                  dst_addr,
                                  1,       /* 1 for request, 0 for reply */
                                  is_sync, /* is it a syncronous request */
                                  msg_num,
                                  numargs, argptr);
}

/* -------------------------------------------------------------------------- */

int gasnetc_ReplyGeneric(gasnetc_category_t category,
                         gasnet_token_t gasnet_token,
                         gasnet_handler_t handler,
                         void *src_addr,
                         int nbytes,
                         void *dst_addr,
                         int numargs, va_list argptr)
{

    gasnetc_am_token_t * token = (gasnetc_am_token_t *)gasnet_token;

#if !GASNET_PSHM
#if !GASNET_USE_MXM_SELF_PTL
    /*
     * PSHM already checked if the source and destination
     * are in the same supernode - do it now for cases where
     * PSHM is disabled.
     * Note: system messages will go the usual way.
     */
    if_pf(gasneti_mynode == token->src_node && category != gasnetc_System)
        return gasnetc_AM_Generic_Self(category, handler,
                                       src_addr, nbytes,
                                       dst_addr,
                                       0, /* 1 for request, 0 for reply */
                                       token->is_sync_request, /* return same flag as received */
                                       token->msg_num,
                                       numargs, argptr);
    else
#endif
#endif
        return gasnetc_AM_Generic(category, token->src_node, handler,
                                  src_addr, nbytes,
                                  dst_addr,
                                  0, /* 1 for request, 0 for reply */
                                  token->is_sync_request, /* return same flag as received */
                                  token->msg_num,
                                  numargs, argptr);
}

/* -------------------------------------------------------------------------- */

extern void gasnetc_HandleSystemExitMessage(gasnetc_am_token_t * token,
        gasnet_handlerarg_t * args, uint8_t numargs);

static void gasnetc_HandleSystemMessage(gasnetc_am_token_t * token,
                                        gasnet_handlerarg_t * args, uint8_t numargs)
{
    /*
     * The only system messages we have are exit sequence coordination.
     * Just call the function that handles this type of messages.
     */
    gasnetc_HandleSystemExitMessage(token, args, numargs);
}

/* -------------------------------------------------------------------------- */

void gasnetc_ProcessRecv(gasnet_mxm_recv_req_t *r)
{
    gasnetc_am_token_t token;
    gasnetc_am_token_t * p_token = &token;
    gasnetc_category_t category;
    uint8_t sys_msg_type;

    gasnet_handler_t handler_id = 0;
    gasneti_handler_fn_t handler_fn = NULL;

    uint8_t  numargs = 0;
    size_t   args_len = 0;
    gasnet_handlerarg_t *args = NULL;

    gasneti_assert(r->mxm_rreq.base.state == MXM_REQ_COMPLETED);

    token.src_node        = (gasnet_node_t)r->mxm_rreq.completion.sender_tag;
    token.is_request      = GASNETC_MSG_ISREQUEST(r->mxm_rreq.completion.sender_imm);
    token.is_sync_request = GASNETC_MSG_ISSYNC(r->mxm_rreq.completion.sender_imm);
    token.msg_num         = GASNETC_MSG_NUMBER(r->mxm_rreq.completion.sender_imm);
    numargs               = GASNETC_MSG_NUMARGS(r->mxm_rreq.completion.sender_imm);
    category              = GASNETC_MSG_CATEGORY(r->mxm_rreq.completion.sender_imm);
    sys_msg_type          = GASNETC_MSG_TYPE_SYS(r->mxm_rreq.completion.sender_imm);
    handler_id            = GASNETC_MSG_HANDLERID(r->mxm_rreq.completion.sender_imm);

#if GASNET_DEBUG_AM
    MXM_LOG("[msg 0x%02x%02x] [pid %d] RECV on node %d: source ID = %d, category = %s, handler_id = %d, numargs = %d, is_request = %d, is_sync_request = %d, sender len = %d, actual len = %d\n",
            (token.is_request) ?
            (uint8_t)(r->mxm_rreq.completion.sender_tag) :
            (uint8_t)gasneti_mynode,
            token.msg_num, getpid(), gasnet_mynode(),
            r->mxm_rreq.completion.sender_tag,
            am_category_str[category],
            handler_id,
            numargs,
            token.is_request,
            token.is_sync_request,
            (int)r->mxm_rreq.completion.sender_len,
            (int)r->mxm_rreq.completion.actual_len);
#endif
#if 0
    printf("[msg 0x%02x%02x] [pid %d] RECV on node %d: source ID = %d, category = %s, handler_id = %d, numargs = %d, is_request = %d, is_sync_request = %d, sender len = %d, actual len = %d\n",
            (token.is_request) ?
            (uint8_t)(r->mxm_rreq.completion.sender_tag) :
            (uint8_t)gasneti_mynode,
            token.msg_num, getpid(), gasnet_mynode(),
            r->mxm_rreq.completion.sender_tag,
            am_category_str[category],
            handler_id,
            numargs,
            token.is_request,
            token.is_sync_request,
            (int)r->mxm_rreq.completion.sender_len,
            (int)r->mxm_rreq.completion.actual_len);
#endif
    handler_fn = gasnetc_handler[handler_id];

    if (numargs)
        args = (gasnet_handlerarg_t *)r->mxm_rreq.base.data.buffer.ptr;

    switch (category) {
    case gasnetc_Short: {
        GASNETI_RUN_HANDLER_SHORT(token.is_request,
                                  handler_id, handler_fn,
                                  p_token,
                                  args, numargs);
    }
    break;

    case gasnetc_Medium: {
        size_t med_data_len;
        void * med_data;

        /*
         * The received buffer contains arguments and medium data.
         * GASNet requires medium data to be aligned to 8 bytes.
         */
        args_len = GASNETI_ALIGNUP(
                       numargs * sizeof(gasnet_handlerarg_t), GASNETI_MEDBUF_ALIGNMENT);
        med_data_len =
            r->mxm_rreq.completion.actual_len - args_len;
        med_data = med_data_len ? (void *)(
                       (uintptr_t)r->mxm_rreq.base.data.buffer.ptr +
                       (uintptr_t)args_len) : NULL;

        GASNETI_RUN_HANDLER_MEDIUM(token.is_request,
                                   handler_id, handler_fn,
                                   p_token,
                                   args, numargs,
                                   med_data, med_data_len);
    }
    break;

    case gasnetc_Long: {
        void * long_data = NULL;
        void * addr;
        size_t len;

        args_len = numargs * sizeof(gasnet_handlerarg_t);

        long_data = (void *)(
                        (uintptr_t)r->mxm_rreq.base.data.buffer.ptr +
                        (uintptr_t)args_len);

        addr = (void *)((uint64_t *)long_data)[0];
        len = (size_t)((uint64_t *)long_data)[1];

        GASNETI_RUN_HANDLER_LONG(token.is_request,
                                 handler_id, handler_fn,
                                 p_token,
                                 args, numargs,
                                 addr, len);
    }
    break;

    case gasnetc_System:
        gasnetc_HandleSystemMessage(p_token, args, numargs);
        break;

    default:
        gasneti_fatalerror("invalid message category");
    }

}

/* -------------------------------------------------------------------------- */
