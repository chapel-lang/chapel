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
    GASNETE_MQ_SEND_LOCAL_OP = 0,   /* Posted as isend on local node */
    GASNETE_MQ_RECV_LOCAL_OP = 1,   /* Posted as irecvs on local node */
    GASNETE_MQ_SEND_OP = 2,    /* Posted as isend on remote node */
    GASNETE_MQ_RECV_OP = 3,    /* Posted as irecvs on remote node */
    GASNETE_MQ_EARLY_ACK_RECVD_OP = 4,    /* ACK received before local completion is marked */
    GASNETE_MQ_MAX_OP
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

typedef struct _gasnete_mq_ack {
    gasnetc_item_t item;
    uint64_t label;
    psm2_epaddr_t peer;
} gasnete_mq_ack_t;

#define GASNETE_MQOPS_INIT 64
#define GASNETE_MQOPS_INCR 64
#define GASNETE_MQREQS_INIT 512
#define GASNETE_MQREQS_INCR 512

#define GASNETE_PSM_FRAG_SIZE 0x7FFFFFFF

#define GASNETE_TRANSFER_ID_INIT -1

static gasnetc_posted_mq_reqs_t *gasnete_get_mq_req(void)
{
    if (gasnetc_psm_state.posted_reqs_length ==
            gasnetc_psm_state.posted_reqs_alloc) {
        int old = gasnetc_psm_state.posted_reqs_alloc;
        int new =  old + GASNETE_MQREQS_INCR;
        int i;
        gasnetc_psm_state.posted_mq_reqs =
            gasneti_realloc(gasnetc_psm_state.posted_mq_reqs,
                    new * sizeof(gasnetc_posted_mq_reqs_t));

        gasnetc_psm_state.posted_reqs_alloc = new;
        for (i = old; i < new; i++)
        {
            gasnetc_psm_state.posted_mq_reqs[i].posted_reqs = PSM2_MQ_REQINVALID;
            gasnetc_psm_state.posted_mq_reqs[i].label = 0;
            gasnetc_psm_state.posted_mq_reqs[i].completion = 0;
            gasnetc_psm_state.posted_mq_reqs[i].optype = GASNETE_MQ_MAX_OP;
            gasnetc_psm_state.posted_mq_reqs[i].peer = 0;
            gasnetc_psm_state.posted_mq_reqs[i].transfer_id = GASNETE_TRANSFER_ID_INIT;
        }
    }

    return &gasnetc_psm_state.posted_mq_reqs[
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


static
uint32_t get_cpu_model(void)
{
    cpuid_t id;
    uint32_t cpu_model = CPUID_MODEL_UNDEFINED;

    /* First check to ensure Genuine Intel */
    get_cpuid(0x0, 0, &id);
    if(id.ebx == CPUID_GENUINE_INTEL_EBX &&
       id.ecx == CPUID_GENUINE_INTEL_ECX &&
       id.edx == CPUID_GENUINE_INTEL_EDX)
    {
        /* Use cpuid with EAX=1 to get processor info */
        get_cpuid(0x1, 0, &id);
        if((id.eax & CPUID_FAMILY_MASK) == CPUID_FAMILY_XEON)
        {
            cpu_model = ((id.eax & CPUID_MODEL_MASK) >> 4) |
                         ((id.eax & CPUID_EXMODEL_MASK) >> 12);
        }
    }

    return cpu_model;
}


GASNETI_COLD
int gasnete_long_msg_init(void)
{
    int i;
    gasnete_mq_op_t *mq_ops;
    uint32_t threshold = 16384;

    threshold = gasneti_getenv_int_withdefault(
            "GASNET_LONG_MSG_THRESHOLD", threshold, 1);
    gasnetc_psm_state.long_msg_threshold = threshold;

    if(CPUID_MODEL_PHI_GEN2 == get_cpu_model()) {
        setenv("PSM2_MQ_RNDV_HFI_WINDOW",
               _STRINGIFY(DEFAULT_PSM2_MQ_RNDV_HFI_WINDOW_SIZE),0);
    }

    gasnetc_list_init(&gasnetc_psm_state.avail_mq_ops, GASNETE_MQOPS_INIT,
            sizeof(gasnete_mq_op_t));
    gasnetc_list_init(&gasnetc_psm_state.pending_mq_ops, 0, 0);
    gasnetc_list_init(&gasnetc_psm_state.pending_ack, 0, 0);

    gasnetc_psm_state.posted_reqs_length = 0;
    gasnetc_psm_state.posted_mq_reqs = gasneti_malloc(
            sizeof(gasnetc_posted_mq_reqs_t) * GASNETE_MQREQS_INIT);
    gasneti_leak(gasnetc_psm_state.posted_mq_reqs);
    gasnetc_psm_state.posted_reqs_alloc = GASNETE_MQREQS_INIT;

    for (i = 0; i < GASNETE_MQREQS_INIT; i++)
    {
        gasnetc_psm_state.posted_mq_reqs[i].posted_reqs = PSM2_MQ_REQINVALID;
        gasnetc_psm_state.posted_mq_reqs[i].label = 0;
        gasnetc_psm_state.posted_mq_reqs[i].completion = 0;
        gasnetc_psm_state.posted_mq_reqs[i].optype = GASNETE_MQ_MAX_OP;
        gasnetc_psm_state.posted_mq_reqs[i].peer = 0;
        gasnetc_psm_state.posted_mq_reqs[i].transfer_id = GASNETE_TRANSFER_ID_INIT;
    }

    gasnetc_psm_state.transfers_count = 0;
    gasnetc_psm_state.transfers = gasneti_malloc(
            sizeof(gasnete_transfer_t) * GASNETE_MQOPS_INIT);
    gasneti_leak(gasnetc_psm_state.transfers);
    gasnetc_psm_state.transfers_alloc = GASNETE_MQOPS_INIT;

    memset(gasnetc_psm_state.transfers, 0, sizeof(gasnete_transfer_t) * GASNETE_MQOPS_INIT);

    return GASNET_OK;
}

GASNETI_HOT
void gasnete_post_pending_ack(void)
{
    gasnetc_item_t *head;
    gasnetc_item_t *cur;
    gasnetc_item_t *prev;
    psm2_error_t ret;

    head = gasnetc_list_drain(&gasnetc_psm_state.pending_ack);

    for (cur = head; cur != NULL; ) {
        gasnete_mq_ack_t *ack = (gasnete_mq_ack_t *)cur;

        for (int i = 0; i < gasnetc_psm_state.posted_reqs_length; i++) {
            if((ack->label == gasnetc_psm_state.posted_mq_reqs[i].label) &&
               (ack->peer  == gasnetc_psm_state.posted_mq_reqs[i].peer) &&
               (GASNETE_MQ_SEND_LOCAL_OP == gasnetc_psm_state.posted_mq_reqs[i].optype)) {

                if(1 == gasnetc_psm_state.posted_mq_reqs[i].completion) {

                    /* This is an op, mark it done. */
                    int transfer_id = gasnetc_psm_state.posted_mq_reqs[i].transfer_id;
                    gasneti_assert(transfer_id < gasnetc_psm_state.transfers_alloc);
                    gasnetc_psm_state.transfers[transfer_id].frags_remaining--;
                    if (gasnetc_psm_state.transfers[transfer_id].frags_remaining == 0){
                        PSM_MARK_DONE(gasnetc_psm_state.transfers[transfer_id].context, gasnetc_psm_state.transfers[transfer_id].optype);
                        gasnetc_psm_state.transfers[transfer_id].context = NULL;
                        gasnetc_psm_state.transfers_count--;
                    }

                    /* Fill this slot with an active req. */
                    int length = gasnetc_psm_state.posted_reqs_length;

                    gasneti_assert(length > 0);
                    gasnetc_psm_state.posted_mq_reqs[i].posted_reqs =
                            gasnetc_psm_state.posted_mq_reqs[length - 1].posted_reqs;
                    gasnetc_psm_state.posted_mq_reqs[i].label =
                            gasnetc_psm_state.posted_mq_reqs[length - 1].label;
                    gasnetc_psm_state.posted_mq_reqs[i].completion =
                            gasnetc_psm_state.posted_mq_reqs[length - 1].completion;
                    gasnetc_psm_state.posted_mq_reqs[i].peer =
                            gasnetc_psm_state.posted_mq_reqs[length - 1].peer;
                    gasnetc_psm_state.posted_mq_reqs[i].transfer_id =
                            gasnetc_psm_state.posted_mq_reqs[length - 1].transfer_id;
                    gasnetc_psm_state.posted_mq_reqs[i].optype =
                            gasnetc_psm_state.posted_mq_reqs[length - 1].optype;

                    gasnetc_psm_state.posted_mq_reqs[length - 1].posted_reqs = PSM2_MQ_REQINVALID;
                    gasnetc_psm_state.posted_mq_reqs[length - 1].label = 0;
                    gasnetc_psm_state.posted_mq_reqs[length - 1].completion = 0;
                    gasnetc_psm_state.posted_mq_reqs[length - 1].optype = GASNETE_MQ_MAX_OP;
                    gasnetc_psm_state.posted_mq_reqs[length - 1].peer = 0;
                    gasnetc_psm_state.posted_mq_reqs[length - 1].transfer_id = GASNETE_TRANSFER_ID_INIT;

                    gasnetc_psm_state.posted_reqs_length -= 1;
                    i -= 1;
                } else {
                    gasnetc_psm_state.posted_mq_reqs[i].optype = GASNETE_MQ_EARLY_ACK_RECVD_OP;
                }
            }
        }
        prev = cur;
        cur = cur->next;
        gasneti_free(prev);
    }
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
        gasnetc_posted_mq_reqs_t *req = gasnete_get_mq_req();
        gasnete_mq_op_t *op = (gasnete_mq_op_t *)cur;

        if (op->type == GASNETE_MQ_SEND_OP) {
            ret = psm2_mq_isend2(gasnetc_psm_state.mq, op->peer,
                    0, &op->tag, op->buf,
                    op->length, NULL, &req->posted_reqs);
        } else {
            psm2_mq_tag_t tagsel;

            gasneti_assert(op->type == GASNETE_MQ_RECV_OP);

            tagsel.tag0 = 0xFFFFFFFF;
            tagsel.tag1 = 0xFFFFFFFF;
            tagsel.tag2 = 0xFFFFFFFF;

            req->label = op->mq_op_id;
            req->optype = GASNETE_MQ_RECV_OP;
            req->completion = 0;
            req->peer = op->peer;
            req->transfer_id = GASNETE_TRANSFER_ID_INIT;

            ret = psm2_mq_irecv2(gasnetc_psm_state.mq, op->peer,
                    &op->tag, &tagsel, 0, op->buf,
                    op->length, NULL, &req->posted_reqs);
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
    int i;
    int length;
    int transfer_id;

    for (i = 0; i < gasnetc_psm_state.posted_reqs_length; i++) {
        if (0 == gasnetc_psm_state.posted_mq_reqs[i].completion) {
            ret = psm2_mq_test2(&gasnetc_psm_state.posted_mq_reqs[i].posted_reqs, NULL);
        } else {
            continue;
        }

        if (ret != PSM2_OK)
            /* No completion */
            continue;

        if (GASNETE_MQ_RECV_OP == gasnetc_psm_state.posted_mq_reqs[i].optype) {
            psm2_amarg_t args;
            args.u64  = gasnetc_psm_state.posted_mq_reqs[i].label;

            ret = psm2_am_request_short(gasnetc_psm_state.posted_mq_reqs[i].peer,
                           gasnetc_psm_state.am_handlers[AM_HANDLER_LONG_PUT_REPLY],
                           &args, 1, NULL, 0,
                           PSM2_AM_FLAG_NOREPLY, NULL, NULL);

            if (ret != PSM2_OK) {
                gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                           psm2_error_get_string(ret));
            }

        } else if (GASNETE_MQ_RECV_LOCAL_OP == gasnetc_psm_state.posted_mq_reqs[i].optype ||
                   GASNETE_MQ_EARLY_ACK_RECVD_OP == gasnetc_psm_state.posted_mq_reqs[i].optype) {

            /* This is an op, mark it done. */
            transfer_id = gasnetc_psm_state.posted_mq_reqs[i].transfer_id;
            gasneti_assert(transfer_id < gasnetc_psm_state.transfers_alloc);
            gasnetc_psm_state.transfers[transfer_id].frags_remaining--;
            if (gasnetc_psm_state.transfers[transfer_id].frags_remaining == 0){
                PSM_MARK_DONE(gasnetc_psm_state.transfers[transfer_id].context, gasnetc_psm_state.transfers[transfer_id].optype);
                gasnetc_psm_state.transfers[transfer_id].context = NULL;
                gasnetc_psm_state.transfers_count--;
            }
        } else if (GASNETE_MQ_SEND_LOCAL_OP == gasnetc_psm_state.posted_mq_reqs[i].optype) {
            gasnetc_psm_state.posted_mq_reqs[i].completion = 1;
            continue;
        }

        /* Fill this slot with an active req. */
        length = gasnetc_psm_state.posted_reqs_length;

        gasneti_assert(length > 0);
        gasnetc_psm_state.posted_mq_reqs[i].posted_reqs =
                gasnetc_psm_state.posted_mq_reqs[length - 1].posted_reqs;
        gasnetc_psm_state.posted_mq_reqs[i].label =
                gasnetc_psm_state.posted_mq_reqs[length - 1].label;
        gasnetc_psm_state.posted_mq_reqs[i].completion =
                gasnetc_psm_state.posted_mq_reqs[length - 1].completion;
        gasnetc_psm_state.posted_mq_reqs[i].peer =
                gasnetc_psm_state.posted_mq_reqs[length - 1].peer;
        gasnetc_psm_state.posted_mq_reqs[i].transfer_id =
                gasnetc_psm_state.posted_mq_reqs[length - 1].transfer_id;
        gasnetc_psm_state.posted_mq_reqs[i].optype =
                gasnetc_psm_state.posted_mq_reqs[length - 1].optype;

        gasnetc_psm_state.posted_mq_reqs[length - 1].posted_reqs = PSM2_MQ_REQINVALID;
        gasnetc_psm_state.posted_mq_reqs[length - 1].label = 0;
        gasnetc_psm_state.posted_mq_reqs[length - 1].completion = 0;
        gasnetc_psm_state.posted_mq_reqs[length - 1].optype = GASNETE_MQ_MAX_OP;
        gasnetc_psm_state.posted_mq_reqs[length - 1].peer = 0;
        gasnetc_psm_state.posted_mq_reqs[length - 1].transfer_id = GASNETE_TRANSFER_ID_INIT;

        gasnetc_psm_state.posted_reqs_length -= 1;
        i -= 1;
    }
}


int gasnete_handler_long_put_reply(psm2_am_token_t token,
    psm2_amarg_t *args, int nargs, void *addr, uint32_t len)
{
    int transfer_id;
    int i;
    int length;
    psm2_epaddr_t from_node;
    uintptr_t *ack_item;
    gasnete_mq_ack_t *recv_ack;

    psm2_am_get_source(token, &from_node);
    gasneti_assert(nargs == 1);

    ack_item = gasneti_malloc(sizeof(gasnete_mq_ack_t));
    recv_ack = (gasnete_mq_ack_t *)ack_item;
    recv_ack->peer = from_node;
    recv_ack->label = args[0].u64;

    gasnetc_list_add_tail(&gasnetc_psm_state.pending_ack, (gasnetc_item_t *)ack_item);

    return 0;
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
        size_t nbytes, gasnet_handle_t op, uint8_t isbulk GASNETE_THREAD_FARG)
{
    psm2_epaddr_t epaddr = gasnetc_psm_state.peer_epaddrs[node];
    psm2_mq_tag_t tag;
    psm2_amarg_t args[3];
    gasnetc_posted_mq_reqs_t *req;
    psm2_error_t ret;
    uint64_t mq_op_id;
    uint32_t fraglen;
    int transfer_index;
    uintptr_t srcptr = (uintptr_t)src;
    uintptr_t destptr = (uintptr_t)dest;
    psm2_mq_req_t non_bulk_reqs;
    GASNETC_PSM_LOCK();

    transfer_index = gasnete_get_transfer();
    gasneti_assert(transfer_index != -1);
    gasnetc_psm_state.transfers[transfer_index].context = PSM_PACK_EOP_DONE(op);
    gasnetc_psm_state.transfers[transfer_index].frags_remaining = (nbytes + GASNETE_PSM_FRAG_SIZE - 1) / GASNETE_PSM_FRAG_SIZE;
    gasnetc_psm_state.transfers[transfer_index].optype = GASNETE_MQ_SEND_LOCAL_OP;

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

        req->label = mq_op_id;
        req->peer = epaddr;
        req->optype = GASNETE_MQ_SEND_LOCAL_OP;
        req->transfer_id = transfer_index;

        if(isbulk) {
            ret = psm2_mq_isend2(gasnetc_psm_state.mq, epaddr, 0, &tag, (void *)srcptr,
                            fraglen, NULL, &req->posted_reqs);
            if (ret != PSM2_OK) {
                goto fail;
            }
        } else {
           ret = psm2_mq_isend2(gasnetc_psm_state.mq, epaddr, 0, &tag, (void *)srcptr,
                        fraglen, NULL, &non_bulk_reqs);
            if (ret != PSM2_OK) {
                goto fail;
            }

            req->completion = 1;
            while(psm2_mq_test2(&non_bulk_reqs, NULL) != PSM2_OK) {
               GASNETC_PSM_UNLOCK();
               GASNETI_WAITHOOK();
               gasnetc_AMPoll();
               GASNETC_PSM_LOCK();
            }
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
    gasnetc_posted_mq_reqs_t *req;
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
    gasnetc_psm_state.transfers[transfer_index].optype = GASNETE_MQ_RECV_LOCAL_OP;

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

        req->label = mq_op_id;
        req->peer = epaddr;
        req->optype = GASNETE_MQ_RECV_LOCAL_OP;
        req->transfer_id = transfer_index;

        ret = psm2_mq_irecv2(gasnetc_psm_state.mq, epaddr, &tag, &tagsel,
                    0, (void *)destptr, fraglen, NULL, &req->posted_reqs);
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
