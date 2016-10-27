/*
 * Description: psm2 long message protocol
 * Copyright (c) 2014-2015 Intel Corporation. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_core_list.h>
#include <gasnet_extended_internal.h>
#include <gasnet_extended_internal_extra.h>


typedef enum _gasnete_psm_mq_op {
    GASNETE_MQ_RECV_OP = 0,    /* Receives are PUTs */
    GASNETE_MQ_SEND_OP = 1    /* Sends are GETs */
} gasnete_mq_optype_t;

typedef struct _gasnete_mq_op {
    gasnetc_item_t item;
    gasnete_mq_optype_t type;
    uint32_t length;

    psm2_epaddr_t peer;
    void *buf;
    uint64_t mq_op_id;

    psm2_mq_tag_t tag;
} gasnete_mq_op_t;


#define GASNETE_MQOPS_INIT 64
#define GASNETE_MQOPS_INCR 64
#define GASNETE_MQREQS_INIT 512
#define GASNETE_MQREQS_INCR 512

#define GASNETE_PSM_FRAG_SIZE 0x7FFFFFFF

static psm2_mq_req_t *gasnete_get_mq_req(void)
{
    if (gasnetc_psm_state.posted_reqs_length ==
            gasnetc_psm_state.posted_reqs_alloc) {
        int old = gasnetc_psm_state.posted_reqs_alloc;
        int new =  old + GASNETE_MQREQS_INCR;
        int i;

        gasnetc_psm_state.posted_reqs =
            gasneti_realloc(gasnetc_psm_state.posted_reqs,
                    new * sizeof(psm2_mq_req_t));

        gasnetc_psm_state.posted_reqs_alloc = new;
        for (i = old; i < new; i++)
            gasnetc_psm_state.posted_reqs[i] = PSM2_MQ_REQINVALID;
    }

    return &gasnetc_psm_state.posted_reqs[
        gasnetc_psm_state.posted_reqs_length++];
}

static int gasnete_get_transfer(void)
{
    int i;
    if (gasnetc_psm_state.transfers_count ==
            gasnetc_psm_state.transfers_alloc) {
        int old = gasnetc_psm_state.transfers_alloc;
        int new =  old + GASNETE_MQOPS_INCR;

        gasnetc_psm_state.transfers =
            gasneti_realloc(gasnetc_psm_state.transfers,
                    new * sizeof(gasnete_transfer_t));

        gasnetc_psm_state.transfers_alloc = new;
        memset(gasnetc_psm_state.transfers + old, 0, GASNETE_MQOPS_INCR * sizeof(gasnete_transfer_t));
    }
    gasnetc_psm_state.transfers_count++;

    for(i = 0; i < gasnetc_psm_state.transfers_alloc; i++)
        if (gasnetc_psm_state.transfers[i].context == 0)
            return i;
    return -1;
}


GASNETI_COLD
int gasnete_long_msg_init(void)
{
    int i;
    gasnete_mq_op_t *mq_ops;
    uint32_t threshold = 16384;

    threshold = gasneti_getenv_int_withdefault(
            "GASNET_LONG_MSG_THRESHOLD", threshold, 0);
    gasnetc_psm_state.long_msg_threshold = threshold;

    gasnetc_list_init(&gasnetc_psm_state.avail_mq_ops, GASNETE_MQOPS_INIT,
            sizeof(gasnete_mq_op_t));
    gasnetc_list_init(&gasnetc_psm_state.pending_mq_ops, 0, 0);

    gasnetc_psm_state.posted_reqs_length = 0;
    gasnetc_psm_state.posted_reqs = gasneti_malloc(
            sizeof(psm2_mq_req_t) * GASNETE_MQREQS_INIT);
    gasneti_leak(gasnetc_psm_state.posted_reqs);
    gasnetc_psm_state.posted_reqs_alloc = GASNETE_MQREQS_INIT;

    for (i = 0; i < GASNETE_MQREQS_INIT; i++)
        gasnetc_psm_state.posted_reqs[i] = PSM2_MQ_REQINVALID;

    gasnetc_psm_state.transfers_count = 0;
    gasnetc_psm_state.transfers = gasneti_malloc(
            sizeof(gasnete_transfer_t) * GASNETE_MQOPS_INIT);
    gasneti_leak(gasnetc_psm_state.transfers);
    gasnetc_psm_state.transfers_alloc = GASNETE_MQOPS_INIT;

    memset(gasnetc_psm_state.transfers, 0, sizeof(gasnete_transfer_t) * GASNETE_MQOPS_INIT);

    return GASNET_OK;
}

GASNETI_HOT
void gasnete_post_pending_mq_ops(void)
{
    gasnetc_item_t *head;
    gasnetc_item_t *cur;
    gasnetc_item_t *prev;
    psm2_error_t ret;

    /* Separating into separate lists for send/recv might have
       cache and/or contention benefits.  It would also remove the
       op->type branch in this loop.   */
    head = gasnetc_list_drain(&gasnetc_psm_state.pending_mq_ops);

    for (cur = head; cur != NULL; ) {
        psm2_mq_req_t *req = gasnete_get_mq_req();
        gasnete_mq_op_t *op = (gasnete_mq_op_t *)cur;

        if (op->type == GASNETE_MQ_SEND_OP) {
            ret = psm2_mq_isend2(gasnetc_psm_state.mq, op->peer,
                    0, &op->tag, op->buf,
                    op->length, NULL, req);
        } else {
            psm2_mq_tag_t tagsel;

            gasneti_assert(op->type == GASNETE_MQ_RECV_OP);

            tagsel.tag0 = 0xFFFFFFFF;
            tagsel.tag1 = 0xFFFFFFFF;
            tagsel.tag2 = 0xFFFFFFFF;

            ret = psm2_mq_irecv2(gasnetc_psm_state.mq, op->peer,
                    &op->tag, &tagsel, 0, op->buf,
                    op->length, NULL, req);
        }

        if_pf (ret != PSM2_OK) {
            gasneti_fatalerror("PSM MQ send/recv failure: %s\n",
                psm2_error_get_string(ret));
        }

        prev = cur;
        cur = cur->next;
        gasnetc_list_add_head(&gasnetc_psm_state.avail_mq_ops, prev);
    }
}

/* This routine assumes that psm2_poll() has recently been called. */
GASNETI_HOT
void gasnete_finish_mq_reqs(void)
{
    psm2_error_t ret;
    psm2_mq_status2_t stat;
    int i;
    int length;
    int transfer_id;

    for (i = 0; i < gasnetc_psm_state.posted_reqs_length; i++) {
        gasneti_assert(gasnetc_psm_state.posted_reqs[i] !=
                PSM2_MQ_REQINVALID);
        ret = psm2_mq_test2(&gasnetc_psm_state.posted_reqs[i], &stat);
        if (ret != PSM2_OK)
            /* No completion */
            continue;

        if (stat.context != NULL) {
            /* This is an op, mark it done. */
            transfer_id = (int) (uintptr_t) stat.context;
            transfer_id &= 0x7FFFFFFF;
            gasneti_assert(transfer_id < gasnetc_psm_state.transfers_alloc);
            gasnetc_psm_state.transfers[transfer_id].frags_remaining--;
            if (gasnetc_psm_state.transfers[transfer_id].frags_remaining == 0){
                PSM_MARK_DONE(gasnetc_psm_state.transfers[transfer_id].context, gasnetc_psm_state.transfers[transfer_id].optype);
                gasnetc_psm_state.transfers[transfer_id].context = NULL;
                gasnetc_psm_state.transfers_count--;
            }
        }

        /* Fill this slot with an active req. */
        length = gasnetc_psm_state.posted_reqs_length;
        gasneti_assert(length > 0);
        gasnetc_psm_state.posted_reqs[i] =
                gasnetc_psm_state.posted_reqs[length - 1];
        gasnetc_psm_state.posted_reqs_length -= 1;
        i -= 1;
    }
}


int gasnete_handler_long_put(psm2_am_token_t token,
    psm2_amarg_t *args, int nargs, void *addr, uint32_t len)
{
    gasnete_mq_op_t *op;

    gasneti_assert(nargs == 3);

    /* Queue an MQ receive */
    op = (gasnete_mq_op_t *)gasnetc_list_remove_alloc(
            &gasnetc_psm_state.avail_mq_ops,
            GASNETE_MQOPS_INCR, sizeof(gasnete_mq_op_t));

    op->type = GASNETE_MQ_RECV_OP;
    op->length = args[1].u32w0;
    psm2_am_get_source(token, &op->peer);
    op->buf = (void *)args[0].u64w0;
    op->mq_op_id = args[2].u64;
    op->tag.tag0 = op->mq_op_id >> 32;
    op->tag.tag1 = op->mq_op_id & 0xFFFFFFFF;
    op->tag.tag2 = GASNETE_MQ_RECV_OP;
    gasnetc_list_add_tail(&gasnetc_psm_state.pending_mq_ops,
            (gasnetc_item_t *)op);
    return 0;
}

int gasnete_handler_long_get(psm2_am_token_t token,
    psm2_amarg_t *args, int nargs, void *addr, uint32_t len)
{
    gasnete_mq_op_t *op;

    gasneti_assert(nargs == 4);

    /* Queue an MQ send */
    op = (gasnete_mq_op_t *)gasnetc_list_remove_alloc(
            &gasnetc_psm_state.avail_mq_ops,
            GASNETE_MQOPS_INCR, sizeof(gasnete_mq_op_t));

    op->type = GASNETE_MQ_SEND_OP;
    op->length = args[2].u32w0;
    psm2_am_get_source(token, &op->peer);
    op->buf = (void *)args[0].u64w0;
    op->mq_op_id = args[3].u64;
    op->tag.tag0 = op->mq_op_id >> 32;
    op->tag.tag1 = op->mq_op_id & 0xFFFFFFFF;
    op->tag.tag2 = GASNETE_MQ_SEND_OP;

    gasnetc_list_add_tail(&gasnetc_psm_state.pending_mq_ops,
            (gasnetc_item_t *)op);
    return 0;
}

gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t * const thread);

void gasnete_put_long(gasnet_node_t node, void *dest, void *src,
        size_t nbytes, gasnet_handle_t op GASNETE_THREAD_FARG)
{
    psm2_epaddr_t epaddr = gasnetc_psm_state.peer_epaddrs[node];
    psm2_mq_tag_t tag;
    psm2_amarg_t args[3];
    psm2_mq_req_t *req;
    psm2_error_t ret;
    uint64_t mq_op_id;
    uint32_t fraglen;
    int transfer_index;
    uintptr_t srcptr = (uintptr_t)src;
    uintptr_t destptr = (uintptr_t)dest;
    GASNETC_PSM_LOCK();

    transfer_index = gasnete_get_transfer();
    gasneti_assert(transfer_index != -1);
    gasnetc_psm_state.transfers[transfer_index].context = PSM_PACK_EOP_DONE(op);
    gasnetc_psm_state.transfers[transfer_index].frags_remaining = (nbytes + GASNETE_PSM_FRAG_SIZE - 1) / GASNETE_PSM_FRAG_SIZE;
    gasnetc_psm_state.transfers[transfer_index].optype = GASNETE_MQ_RECV_OP;

    while (nbytes > 0) {
        mq_op_id = gasnetc_psm_state.mq_op_id++;
        fraglen = MIN(nbytes, GASNETE_PSM_FRAG_SIZE);
        tag.tag0 = (uintptr_t)mq_op_id >> 32;
        tag.tag1 = (uintptr_t)mq_op_id & 0xFFFFFFFF;
        tag.tag2 = GASNETE_MQ_RECV_OP;

        args[0].u64 = destptr;
        args[1].u32w0 = fraglen;
        args[1].u32w1 = GASNETE_MQ_RECV_OP;
        args[2].u64 = mq_op_id;

        ret = psm2_am_request_short(epaddr,
                       gasnetc_psm_state.am_handlers[AM_HANDLER_LONG_PUT],
                       args, 3, NULL, 0,
                       PSM2_AM_FLAG_NOREPLY, NULL, NULL);
        if (ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                       psm2_error_get_string(ret));
        }

        req = gasnete_get_mq_req();
        gasneti_assert(req != NULL);

        ret = psm2_mq_isend2(gasnetc_psm_state.mq, epaddr, 0, &tag, (void *)srcptr,
                    fraglen, (void *)(uintptr_t)(transfer_index | (1<<31)), req);
        if (ret != PSM2_OK) {
            goto fail;
        }
        srcptr += fraglen;
        destptr += fraglen;
        nbytes -= fraglen;
    }

    GASNETC_PSM_UNLOCK();
    gasnetc_psm_poll_periodic();
    return;

fail:
    GASNETC_PSM_UNLOCK();
    gasneti_fatalerror("psm2_mq_isend2 failure: %s\n",
               psm2_error_get_string(ret));
}


void gasnete_get_long (void *dest, gasnet_node_t node, void *src,
        size_t nbytes, gasnet_handle_t op GASNETE_THREAD_FARG) {
    psm2_epaddr_t epaddr = gasnetc_psm_state.peer_epaddrs[node];

    psm2_mq_tag_t tag, tagsel;
    psm2_amarg_t args[4];
    psm2_mq_req_t *req;
    psm2_error_t ret;
    uint32_t fraglen;
    uint32_t fragnum = 0;
    int transfer_index = -1;
    uint64_t mq_op_id;
    uintptr_t srcptr = (uintptr_t)src;
    uintptr_t destptr = (uintptr_t)dest;

    GASNETC_PSM_LOCK();

    transfer_index = gasnete_get_transfer();
    gasneti_assert(transfer_index != -1);
    gasnetc_psm_state.transfers[transfer_index].context = PSM_PACK_EOP_DONE(op);
    gasnetc_psm_state.transfers[transfer_index].frags_remaining = (nbytes + GASNETE_PSM_FRAG_SIZE - 1) / GASNETE_PSM_FRAG_SIZE ;
    gasnetc_psm_state.transfers[transfer_index].optype = GASNETE_MQ_SEND_OP;

    while (nbytes > 0) {
        mq_op_id = gasnetc_psm_state.mq_op_id++;
        fraglen = MIN(nbytes, GASNETE_PSM_FRAG_SIZE);

        tag.tag0 = (uintptr_t)mq_op_id >> 32;
        tag.tag1 = (uintptr_t)mq_op_id & 0xFFFFFFFF;
        tag.tag2 = GASNETE_MQ_SEND_OP;
        tagsel.tag0 = tagsel.tag1 = tagsel.tag2 = 0xFFFFFFFF;

        args[0].u64 = srcptr;
        args[1].u64 = destptr;
        args[2].u32w0 = fraglen;
        args[2].u32w1 = GASNETE_MQ_SEND_OP;
        args[3].u64 = mq_op_id;

        ret = psm2_am_request_short(epaddr,
                       gasnetc_psm_state.am_handlers[AM_HANDLER_LONG_GET],
                       args, 4, NULL, 0,
                       PSM2_AM_FLAG_NOREPLY, NULL, NULL);
        if (ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                       psm2_error_get_string(ret));
        }

        req = gasnete_get_mq_req();
        gasneti_assert(req != NULL);

        ret = psm2_mq_irecv2(gasnetc_psm_state.mq, epaddr, &tag, &tagsel,
                    0, (void *)destptr, fraglen,
                    (void *)(uintptr_t)(transfer_index | (1<<31)), req);
        if (ret != PSM2_OK) {
            goto fail;
        }
        srcptr += fraglen;
        destptr += fraglen;
        nbytes -= fraglen;
    }

    GASNETC_PSM_UNLOCK();
    gasnetc_psm_poll_periodic();
    return;

fail:
    GASNETC_PSM_UNLOCK();
        gasneti_fatalerror("psm2_mq_irecv2 failure: %s\n",
                psm2_error_get_string(ret));
}
