/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation. All rights reserved.
 * Copyright (c) 2019-2023 Hewlett Packard Enterprise Development LP
 */

/* CXI TX Context Management */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

/* 8 Rendezvous, 2 RMA and 2 Atomic + 4 extra */
#define CXIP_INTERNAL_TX_REQS	16

extern struct cxip_txc_ops hpc_txc_ops;
extern struct cxip_txc_ops rnr_txc_ops;

struct cxip_md *cxip_txc_ibuf_md(void *ibuf)
{
	return ofi_buf_hdr(ibuf)->region->context;
}

/*
 * cxip_txc_ibuf_alloc() - Allocate an inject buffer.
 *
 * Caller must hold txc->ep_obj.lock
 */
void *cxip_txc_ibuf_alloc(struct cxip_txc *txc)
{
	void *ibuf;

	ibuf = (struct cxip_req *)ofi_buf_alloc(txc->ibuf_pool);
	if (ibuf)
		CXIP_DBG("Allocated inject buffer: %p\n", ibuf);
	else
		CXIP_WARN("Failed to allocate inject buffer\n");

	return ibuf;
}

/*
 * cxip_txc_ibuf_free() - Free an inject buffer.
 *
 * Caller must hold txc->ep_obj.lock
 */
void cxip_txc_ibuf_free(struct cxip_txc *txc, void *ibuf)
{
	ofi_buf_free(ibuf);
	CXIP_DBG("Freed inject buffer: %p\n", ibuf);
}

int cxip_ibuf_chunk_init(struct ofi_bufpool_region *region)
{
	struct cxip_txc *txc = region->pool->attr.context;
	struct cxip_md *md;
	int ret;

	ret = cxip_map(txc->domain, region->mem_region,
		       region->pool->region_size, OFI_MR_NOCACHE, &md);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map inject buffer chunk\n");
		return ret;
	}

	region->context = md;

	return FI_SUCCESS;
}

void cxip_ibuf_chunk_fini(struct ofi_bufpool_region *region)
{
	cxip_unmap(region->context);
}

int cxip_txc_ibuf_create(struct cxip_txc *txc)
{
	struct ofi_bufpool_attr bp_attrs = {};
	int ret;

	bp_attrs.size = CXIP_INJECT_SIZE;
	bp_attrs.alignment = 8;
	bp_attrs.max_cnt = UINT16_MAX;
	bp_attrs.chunk_cnt = 64;
	bp_attrs.alloc_fn = cxip_ibuf_chunk_init;
	bp_attrs.free_fn = cxip_ibuf_chunk_fini;
	bp_attrs.context = txc;

	/* Avoid creating VA holes outside the buffer pool
	 * if CXI_FORK_SAFE/CXI_FORK_SAFE_HP is enabled.
	 */
	if (cxip_env.fork_safe_requested)
		bp_attrs.flags = OFI_BUFPOOL_NONSHARED;

	ret = ofi_bufpool_create_attr(&bp_attrs, &txc->ibuf_pool);
	if (ret)
		ret = -FI_ENOMEM;

	return ret;
}

/*
 * cxip_tx_id_alloc() - Allocate a TX ID.
 *
 * TX IDs are assigned to Put operations that need to be tracked by the target.
 * One example of this is a Send with completion that guarantees match
 * completion at the target. This only applies to eager, unexpected Sends.
 *
 * Caller must hold txc->ep_obj.lock
 */
int cxip_tx_id_alloc(struct cxip_txc_hpc *txc, void *ctx)
{
	int id;

	id = ofi_idx_insert(&txc->tx_ids, ctx);
	if (id < 0 || id >= CXIP_TX_IDS) {
		CXIP_DBG("Failed to allocate TX ID: %d\n", id);
		if (id > 0)
			ofi_idx_remove(&txc->tx_ids, id);

		return -FI_ENOSPC;
	}

	CXIP_DBG("Allocated ID: %d\n", id);

	return id;
}

/*
 * cxip_tx_id_free() - Free a TX ID.
 *
 * Caller must hold txc->ep_obj.lock
 */
int cxip_tx_id_free(struct cxip_txc_hpc *txc, int id)
{
	if (id < 0 || id >= CXIP_TX_IDS)
		return -FI_EINVAL;

	ofi_idx_remove(&txc->tx_ids, id);
	CXIP_DBG("Freed ID: %d\n", id);

	return FI_SUCCESS;
}

/* Caller must hold txc->ep_obj.lock */
void *cxip_tx_id_lookup(struct cxip_txc_hpc *txc, int id)
{
	return ofi_idx_lookup(&txc->tx_ids, id);
}

/*
 * cxip_rdzv_id_alloc() - Allocate a rendezvous ID.
 *
 * A Rendezvous ID are assigned to rendezvous Send operation. The ID is used by
 * the target to differentiate rendezvous Send operations initiated by a source.
 *
 * Caller must hold txc->ep_obj->lock.
 */
int cxip_rdzv_id_alloc(struct cxip_txc_hpc *txc, struct cxip_req *req)
{
	struct indexer *rdzv_ids;
	int max_rdzv_id;
	int id_offset;
	int id;

	/* FI_TAGGED sends by definition do not support FI_MULTI_RECV;
	 * they can utilize the pool of rendezvous ID [256 to 32K-1].
	 * FI_MSG which supports FI_MULTI_RECV is restricted to a rendezvous
	 * ID range of [0 to 255].
	 */
	if (req->send.tagged) {
		rdzv_ids = &txc->rdzv_ids;
		max_rdzv_id = CXIP_RDZV_IDS;
		id_offset = CXIP_RDZV_IDS_MULTI_RECV;
	} else {
		rdzv_ids = &txc->msg_rdzv_ids;
		max_rdzv_id = CXIP_RDZV_IDS_MULTI_RECV;
		id_offset = 0;
	}

	id = ofi_idx_insert(rdzv_ids, req);
	if (id < 0 || id + id_offset >= max_rdzv_id) {
		CXIP_DBG("Failed to allocate rdzv ID: %d\n", id);
		if (id > 0)
			ofi_idx_remove(rdzv_ids, id);

		return -FI_ENOSPC;
	}

	id += id_offset;
	CXIP_DBG("Allocated ID: %d\n", id);

	return id;
}

/*
 * cxip_rdzv_id_free() - Free a rendezvous ID.
 *
 * Caller must hold txc->ep_obj->lock.
 */
int cxip_rdzv_id_free(struct cxip_txc_hpc *txc, int id)
{
	if (id < 0 || id >= CXIP_RDZV_IDS)
		return -FI_EINVAL;

	CXIP_DBG("Freed RDZV ID: %d\n", id);

	/* ID value indicates which pool it comes from */
	if (id >= CXIP_RDZV_IDS_MULTI_RECV) {
		id -= CXIP_RDZV_IDS_MULTI_RECV;
		ofi_idx_remove(&txc->rdzv_ids, id);
	} else {
		ofi_idx_remove(&txc->msg_rdzv_ids, id);
	}

	return FI_SUCCESS;
}

/* Caller must hold txc->ep_obj->lock. */
void *cxip_rdzv_id_lookup(struct cxip_txc_hpc *txc, int id)
{

	if (id >= CXIP_RDZV_IDS_MULTI_RECV) {
		id -= CXIP_RDZV_IDS_MULTI_RECV;
		return ofi_idx_lookup(&txc->rdzv_ids, id);
	}
	return ofi_idx_lookup(&txc->msg_rdzv_ids, id);
}

/*
 * txc_msg_init() - Initialize an RX context for messaging.
 *
 * Allocates and initializes hardware resources used for transmitting messages.
 *
 * Caller must hold ep_obj->lock
 */
static int txc_msg_init(struct cxip_txc *txc)
{
	int ret = FI_SUCCESS;

	/* Any common initialization should be added here */

	/* Derived TXC message initialization */
	ret = txc->ops.msg_init(txc);

	return ret;
}

/*
 * txc_msg_fini() - Finalize TX context messaging.
 *
 * Free hardware resources allocated when the TX context was initialized for
 * messaging.
 *
 * Caller must hold txc->ep_obj->lock.
 */
static int txc_msg_fini(struct cxip_txc *txc)
{
	int ret;

	/* Any common cleanup should be added here */

	/* Derived TXC message cleanup */
	ret = txc->ops.msg_fini(txc);

	return ret;
}

static size_t cxip_txc_get_num_events(struct cxip_txc *txc)
{
	size_t num_events;

	/* Need enough events to accommodate initiator credits which is
	 * based on TX attr size.
	 */
	num_events = txc->attr.size;

	/* Worse case is an initiator credit needs two events (e.g. rendezvous
	 * send).
	 */
	num_events *= 2;

	/* For messaging, target initiator rendezvous gets has its own set of
	 * credits. These are always single event operations.
	 */
	num_events += cxip_env.sw_rx_tx_init_max;

	/* Account for internal operations. */
	num_events += CXIP_INTERNAL_TX_REQS;

	return num_events;
}

/*
 * cxip_txc_enable() - Enable a TX context for use.
 *
 * Called via fi_enable(). The context could be used in a standard endpoint or
 * a scalable endpoint.
 */
int cxip_txc_enable(struct cxip_txc *txc)
{
	int ret = FI_SUCCESS;
	size_t num_events;

	if (txc->enabled)
		return FI_SUCCESS;

	if (!txc->send_cq) {
		CXIP_WARN("Undefined send CQ\n");
		return -FI_ENOCQ;
	}

	ret = cxip_txc_ibuf_create(txc);
	if (ret) {
		CXIP_WARN("Failed to create inject bufpool %d\n", ret);
		return ret;
	}

	num_events = cxip_txc_get_num_events(txc);

	ret = cxip_evtq_init(&txc->tx_evtq, txc->send_cq, num_events, 0);
	if (ret) {
		CXIP_WARN("Failed to initialize TX event queue: %d, %s\n",
			  ret, fi_strerror(-ret));
		goto destroy_ibuf;
	}

	ret = cxip_ep_cmdq(txc->ep_obj, true, txc->tclass, txc->tx_evtq.eq,
			   &txc->tx_cmdq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Unable to allocate TX CMDQ, ret: %d\n", ret);
		ret = -FI_EDOMAIN;
		/* CQ disable will be done at CQ close */
		goto destroy_evtq;
	}

	if (ofi_send_allowed(txc->attr.caps)) {
		ret = txc_msg_init(txc);
		if (ret != FI_SUCCESS) {
			CXIP_WARN("Unable to init TX CTX, ret: %d\n", ret);
			goto put_tx_cmdq;
		}
	}

	txc->enabled = true;

	return FI_SUCCESS;

put_tx_cmdq:
	cxip_ep_cmdq_put(txc->ep_obj, true);
destroy_evtq:
	cxip_evtq_fini(&txc->tx_evtq);
destroy_ibuf:
	ofi_bufpool_destroy(txc->ibuf_pool);

	return ret;
}

/*
 * txc_cleanup() - Attempt to free outstanding requests.
 *
 * Outstanding commands may be dropped when the TX Command Queue is freed.
 * This leads to missing events. Attempt to gather all events before freeing
 * the TX CQ. If events go missing, resources will be leaked until the
 * Completion Queue is freed.
 */
static void txc_cleanup(struct cxip_txc *txc)
{
	uint64_t start;

	if (!ofi_atomic_get32(&txc->otx_reqs))
		goto proto_cleanup;

	cxip_evtq_req_discard(&txc->tx_evtq, txc);

	start = ofi_gettime_ms();
	while (ofi_atomic_get32(&txc->otx_reqs)) {
		sched_yield();

		cxip_evtq_progress(&txc->tx_evtq);
		cxip_ep_ctrl_progress_locked(txc->ep_obj);

		if (ofi_gettime_ms() - start > CXIP_REQ_CLEANUP_TO) {
			CXIP_WARN("Timeout waiting for outstanding requests.\n");
			break;
		}
	}

	assert(ofi_atomic_get32(&txc->otx_reqs) == 0);

proto_cleanup:
	txc->ops.cleanup(txc);

	ofi_bufpool_destroy(txc->ibuf_pool);
}

/*
 * cxip_txc_disable() - Disable a TX context for a base endpoint object.
 *
 * Free hardware resources allocated when the context was enabled. Called via
 * fi_close().
 */
void cxip_txc_disable(struct cxip_txc *txc)
{
	int ret;

	if (!txc->enabled)
		return;

	txc->enabled = false;
	txc_cleanup(txc);

	if (ofi_send_allowed(txc->attr.caps)) {
		ret = txc_msg_fini(txc);
		if (ret)
			CXIP_WARN("Unable to destroy TX CTX, ret: %d\n",
				       ret);
	}

	cxip_ep_cmdq_put(txc->ep_obj, true);
	cxip_evtq_fini(&txc->tx_evtq);
}

/* Caller must hold ep_obj->lock. */
void cxip_txc_flush_msg_trig_reqs(struct cxip_txc *txc)
{
	struct cxip_req *req;
	struct dlist_entry *tmp;

	/* Drain the message queue. */
	dlist_foreach_container_safe(&txc->msg_queue, struct cxip_req, req,
				     send.txc_entry, tmp) {
		if (cxip_is_trig_req(req)) {
			ofi_atomic_dec32(&txc->otx_reqs);
			dlist_remove(&req->send.txc_entry);
			cxip_unmap(req->send.send_md);
			cxip_evtq_req_free(req);
		}
	}
}

static bool cxip_txc_can_emit_op(struct cxip_txc *txc,
				 bool event_success_disabled)
{
	if (cxip_evtq_saturated(&txc->tx_evtq)) {
		TXC_WARN(txc, "TX HW EQ saturated\n");
		return false;
	}

	/* If taking a successful completion, limit outstanding operations */
	if (!event_success_disabled &&
	    (ofi_atomic_get32(&txc->otx_reqs) >= txc->attr.size)) {
		TXC_WARN(txc, "TXC attr size saturated\n");
		return false;
	}

	return true;
}

int cxip_txc_emit_idc_put(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_put_cmd *put, const void *buf,
			  size_t len, uint64_t flags)
{
	int ret;

	if (!cxip_txc_can_emit_op(txc, c_state->event_success_disable))
		return -FI_EAGAIN;

	if (txc->ep_obj->av_auth_key) {
		ret = cxip_domain_emit_idc_put(txc->domain, vni, tc, c_state,
					       put, buf, len, flags);
		if (ret)
			TXC_WARN(txc, "Failed to emit domain idc put: %d\n",
				 ret);
		else if (!c_state->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(txc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		TXC_WARN(txc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emit_idc_put(txc->tx_cmdq, c_state, put, buf, len,
				     flags);
	if (ret) {
		TXC_WARN(txc, "Failed to emit idc_put command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	/* Kick the command queue. */
	cxip_txq_ring(txc->tx_cmdq, !!(flags & FI_MORE),
		      ofi_atomic_get32(&txc->otx_reqs));

	if (!c_state->event_success_disable)
		ofi_atomic_inc32(&txc->otx_reqs);

	return FI_SUCCESS;
}

int cxip_txc_emit_dma(struct cxip_txc *txc, uint16_t vni,
		      enum cxi_traffic_class tc,
		      enum cxi_traffic_class_type tc_type,
		      struct cxip_cntr *trig_cntr, size_t trig_thresh,
		      struct c_full_dma_cmd *dma, uint64_t flags)
{
	int ret;

	if (!cxip_txc_can_emit_op(txc, dma->event_success_disable))
		return -FI_EAGAIN;

	if (trig_cntr) {
		ret = cxip_domain_dwq_emit_dma(txc->domain, vni,
					       tc, tc_type, trig_cntr,
					       trig_thresh, dma, flags);
		if (ret)
			TXC_WARN(txc,
				 "Failed to emit trigger dma command: %d:%s\n",
				 ret, fi_strerror(-ret));
		else if (!dma->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	if (txc->ep_obj->av_auth_key) {
		ret = cxip_domain_emit_dma(txc->domain, vni, tc, dma, flags);
		if (ret)
			TXC_WARN(txc, "Failed to emit domain dma command: %d\n",
				 ret);
		else if (!dma->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(txc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		TXC_WARN(txc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emit_dma(txc->tx_cmdq, dma, flags);
	if (ret) {
		TXC_WARN(txc, "Failed to emit dma command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	/* Kick the command queue. */
	cxip_txq_ring(txc->tx_cmdq, !!(flags & FI_MORE),
		      ofi_atomic_get32(&txc->otx_reqs));

	if (!dma->event_success_disable)
		ofi_atomic_inc32(&txc->otx_reqs);

	return FI_SUCCESS;
}

int cxip_txc_emit_idc_amo(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_amo_cmd *amo, uint64_t flags,
			  bool fetching, bool flush)
{
	int ret;

	if (!cxip_txc_can_emit_op(txc, c_state->event_success_disable))
		return -FI_EAGAIN;

	if (txc->ep_obj->av_auth_key) {
		ret = cxip_domain_emit_idc_amo(txc->domain, vni, tc, c_state,
					       amo, flags, fetching, flush);
		if (ret)
			TXC_WARN(txc, "Failed to emit domain idc amo: %d\n",
				 ret);
		else if (!c_state->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(txc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		TXC_WARN(txc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emic_idc_amo(txc->tx_cmdq, c_state, amo, flags,
				     fetching, flush);
	if (ret) {
		TXC_WARN(txc, "Failed to emit idc_put command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	/* Kick the command queue. */
	cxip_txq_ring(txc->tx_cmdq, !!(flags & FI_MORE),
		      ofi_atomic_get32(&txc->otx_reqs));

	if (!c_state->event_success_disable)
		ofi_atomic_inc32(&txc->otx_reqs);

	return FI_SUCCESS;
}

int cxip_txc_emit_dma_amo(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  struct cxip_cntr *trig_cntr, size_t trig_thresh,
			  struct c_dma_amo_cmd *amo, uint64_t flags,
			  bool fetching, bool flush)
{
	int ret;

	if (!cxip_txc_can_emit_op(txc, amo->event_success_disable))
		return -FI_EAGAIN;

	if (trig_cntr) {
		ret = cxip_domain_dwq_emit_amo(txc->domain, vni, tc,
					       CXI_TC_TYPE_DEFAULT, trig_cntr,
					       trig_thresh, amo, flags,
					       fetching, flush);
		if (ret)
			TXC_WARN(txc,
				 "Failed to emit trigger amo command: %d:%s\n",
				 ret, fi_strerror(-ret));
		else if (!amo->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	if (txc->ep_obj->av_auth_key) {
		ret = cxip_domain_emit_dma_amo(txc->domain, vni, tc, amo, flags,
					       fetching, flush);
		if (ret)
			TXC_WARN(txc, "Failed to emit domain amo: %d\n",
				 ret);
		else if (!amo->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(txc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		TXC_WARN(txc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emit_dma_amo(txc->tx_cmdq, amo, flags, fetching, flush);
	if (ret) {
		TXC_WARN(txc, "Failed to emit DMA amo command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	/* Kick the command queue. */
	cxip_txq_ring(txc->tx_cmdq, !!(flags & FI_MORE),
		      ofi_atomic_get32(&txc->otx_reqs));

	if (!amo->event_success_disable)
		ofi_atomic_inc32(&txc->otx_reqs);

	return FI_SUCCESS;
}

int cxip_txc_emit_idc_msg(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_msg_hdr *msg, const void *buf,
			  size_t len, uint64_t flags)
{
	int ret;

	if (!cxip_txc_can_emit_op(txc, c_state->event_success_disable))
		return -FI_EAGAIN;

	if (txc->ep_obj->av_auth_key) {
		ret = cxip_domain_emit_idc_msg(txc->domain, vni, tc, c_state,
					       msg, buf, len, flags);
		if (ret)
			TXC_WARN(txc, "Failed to emit domain idc msg: %d\n",
				 ret);
		else if (!c_state->event_success_disable)
			ofi_atomic_inc32(&txc->otx_reqs);

		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(txc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		TXC_WARN(txc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emit_idc_msg(txc->tx_cmdq, c_state, msg, buf, len,
				     flags);
	if (ret) {
		TXC_WARN(txc, "Failed to emit idc_msg command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	/* Kick the command queue. */
	cxip_txq_ring(txc->tx_cmdq, !!(flags & FI_MORE),
		      ofi_atomic_get32(&txc->otx_reqs));

	if (!c_state->event_success_disable)
		ofi_atomic_inc32(&txc->otx_reqs);

	return FI_SUCCESS;
}

struct cxip_txc *cxip_txc_calloc(struct cxip_ep_obj *ep_obj, void *context)
{
	struct cxip_txc *txc = NULL;

	switch (ep_obj->protocol) {
	case FI_PROTO_CXI:
		txc = calloc(1, sizeof(struct cxip_txc_hpc));
		if (txc)
			txc->ops = hpc_txc_ops;
		break;
	case FI_PROTO_CXI_RNR:
		txc = calloc(1, sizeof(struct cxip_txc_rnr));
		if (txc)
			txc->ops = rnr_txc_ops;
		break;
	default:
		CXIP_WARN("Unsupported EP protocol requested %d\n",
			  ep_obj->protocol);
		return NULL;
	}

	if (!txc) {
		CXIP_WARN("Memory allocation failure\n");
		return NULL;
	}

	/* Common structure initialization */
	txc->protocol = ep_obj->protocol;
	txc->context = context;
	txc->ep_obj = ep_obj;
	txc->domain = ep_obj->domain;
	txc->tclass = ep_obj->tx_attr.tclass;
	txc->hrp_war_req = ep_obj->asic_ver < CASSINI_2_0;
	txc->attr = ep_obj->tx_attr;
	txc->hmem = !!(txc->attr.caps & FI_HMEM);
	txc->pid_bits = txc->domain->iface->dev->info.pid_bits;

	dlist_init(&txc->msg_queue);
	dlist_init(&txc->dom_entry);
	ofi_atomic_initialize32(&txc->otx_reqs, 0);

	/* Derived initialization/overrides */
	txc->ops.init_struct(txc, ep_obj);

	return txc;
}

void cxip_txc_free(struct cxip_txc *txc)
{
	if (!txc)
		return;

	/* Derived structure free */
	txc->ops.fini_struct(txc);

	/* Any base stuff */

	free(txc);
}
