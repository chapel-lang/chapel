/*
 * SPDX-License-Identifier: BSD-2 Clause or GPL-2.0-only
 *
 * Copyright (c) 2018-2024 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>
#include <fasthash.h>

#include "cxip.h"

/*
 * cxip_recv_req_src_addr() - Translate request source address to FI address.
 */
fi_addr_t cxip_recv_req_src_addr(struct cxip_req *req)
{
	struct cxip_rxc *rxc = req->recv.rxc;

	/* If the FI_SOURCE capability is enabled, convert the initiator's
	 * address to an FI address to be reported in a CQ event. If
	 * application AVs are symmetric, the match_id in the EQ event is
	 * logical and translation is not needed. Otherwise, translate the
	 * physical address in the EQ event to logical FI address.
	 */
	if (rxc->attr.caps & FI_SOURCE) {
		struct cxip_addr addr = {};

		if (rxc->ep_obj->av->symmetric)
			return CXI_MATCH_ID_EP(rxc->pid_bits,
					       req->recv.initiator);

		addr.nic = CXI_MATCH_ID_EP(rxc->pid_bits, req->recv.initiator);
		addr.pid = CXI_MATCH_ID_PID(rxc->pid_bits, req->recv.initiator);
		addr.vni = req->recv.vni;

		return cxip_av_lookup_fi_addr(rxc->ep_obj->av, &addr);
	}

	return FI_ADDR_NOTAVAIL;
}

/*
 * cxip_recv_req_alloc() - Allocate a request handle for a receive,
 * mapping the associated buffer if required.
 *
 * Caller must hold ep->ep_obj->lock.
 */
int cxip_recv_req_alloc(struct cxip_rxc *rxc, void *buf, size_t len,
			struct cxip_md *md, struct cxip_req **cxip_req,
			int (*recv_cb)(struct cxip_req *req,
				       const union c_event *event))
{
	struct cxip_domain *dom = rxc->domain;
	struct cxip_req *req;
	struct cxip_md *recv_md = NULL;
	int ret;

	/* Software EP only mode receives are not posted to hardware
	 * and are not constrained by hardware buffer ID limits.
	 *
	 * Note: cxip_evtq_req_alloc() zeros the req.
	 */
	req = cxip_evtq_req_alloc(&rxc->rx_evtq, !rxc->sw_ep_only, rxc);
	if (!req) {
		RXC_INFO(rxc, "Recv request unavailable: -FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	if (len) {
		/* If hybrid descriptor not passed, map for dma */
		if (!md) {
			ret = cxip_map(dom, (void *)buf, len, 0, &recv_md);
			if (ret) {
				RXC_WARN(rxc,
					 "Map of recv buffer failed: %d, %s\n",
					 ret, fi_strerror(-ret));
				goto err_free_request;
			}
			req->recv.hybrid_md = false;
		} else {
			req->recv.hybrid_md = true;
			recv_md = md;
		}
	}

	/* Initialize common receive request attributes. */
	req->type = CXIP_REQ_RECV;
	req->cb = recv_cb;
	req->recv.rxc = rxc;
	req->recv.recv_buf = buf;
	req->recv.recv_md = recv_md;
	req->recv.ulen = len;
	dlist_init(&req->recv.children);
	dlist_init(&req->recv.rxc_entry);

	ofi_atomic_inc32(&rxc->orx_reqs);
	*cxip_req = req;

	return FI_SUCCESS;

err_free_request:
	cxip_evtq_req_free(req);

	return ret;
}

void cxip_recv_req_free(struct cxip_req *req)
{
	struct cxip_rxc *rxc = req->recv.rxc;

	assert(req->type == CXIP_REQ_RECV);
	assert(dlist_empty(&req->recv.children));
	assert(dlist_empty(&req->recv.rxc_entry));

	ofi_atomic_dec32(&rxc->orx_reqs);

	if (req->recv.recv_md && !req->recv.hybrid_md)
		cxip_unmap(req->recv.recv_md);

	cxip_evtq_req_free(req);
}

/*
 * recv_req_event_success() - Generate successful receive event completions.
 */
static inline int recv_req_event_success(struct cxip_rxc *rxc,
					 struct cxip_req *req)
{
	int ret;
	fi_addr_t src_addr;
	struct cxip_addr *addr;

	/* If this is a  FI_MULTI_RECV mandatory completion not associated
	 * with a receive completion then source information is not required.
	 */
	if (req->recv.multi_recv && !(req->flags & FI_RECV)) {
		req->flags |= FI_MULTI_RECV;
		return cxip_cq_req_complete(req);
	}

	if (req->recv.rxc->attr.caps & FI_SOURCE) {
		src_addr = cxip_recv_req_src_addr(req);
		if (src_addr != FI_ADDR_NOTAVAIL ||
		    !(rxc->attr.caps & FI_SOURCE_ERR))
			return cxip_cq_req_complete_addr(req, src_addr);

		addr = calloc(1, sizeof(*addr));
		if (!addr)
			return -FI_ENOMEM;

		addr->nic = CXI_MATCH_ID_EP(rxc->pid_bits, req->recv.initiator);
		addr->pid = CXI_MATCH_ID_PID(rxc->pid_bits,
					     req->recv.initiator);

		src_addr = cxip_av_lookup_auth_key_fi_addr(rxc->ep_obj->av,
							   req->recv.vni);

		ret = cxip_cq_req_error(req, 0, FI_EADDRNOTAVAIL, req->recv.rc,
					addr, sizeof(*addr), src_addr);

		free(addr);
	} else {
		ret = cxip_cq_req_complete(req);
	}

	return ret;
}

/*
 * recv_recv_req_report() - Report the completion of a receive operation.
 */
void cxip_recv_req_report(struct cxip_req *req)
{
	int ret;
	int err;
	int success_event = (req->flags & FI_COMPLETION) ||
			    (req->flags & FI_MULTI_RECV &&
			     !(req->flags & FI_COMPLETION));
	struct cxip_rxc *rxc = req->recv.rxc;
	ssize_t truncated = req->recv.rlen - req->data_len;

	/* data_len (i.e. mlength) should NEVER be greater than rlength. */
	assert(truncated >= 0);

	req->flags &= (FI_MSG | FI_TAGGED | FI_RECV | FI_REMOTE_CQ_DATA);

	if (req->recv.parent) {
		struct cxip_req *parent = req->recv.parent;
		bool unlinked = false;

		parent->recv.mrecv_bytes += req->data_len;
		RXC_DBG(rxc,
			"Putting %lu mrecv bytes (req: %p consumed: %lu auto_unlinked: %u unlink_bytes: %lu addr: %#lx ulen=%u min_free=%lu hw_offloaded=%u)\n",
			req->data_len, parent, parent->recv.mrecv_bytes,
			parent->recv.auto_unlinked, parent->recv.mrecv_unlink_bytes,
			req->buf, parent->recv.ulen, rxc->min_multi_recv,
			parent->recv.hw_offloaded);

		/* Handle mrecv edge case. If all unexpected headers were
		 * onloaded, the entire mrecv buffer may be matched against the
		 * sw_ux_list list before being offloaded to HW. Detect this
		 * case.
		 */
		if (parent->recv.hw_offloaded) {
			if (parent->recv.auto_unlinked &&
			    parent->recv.mrecv_bytes == parent->recv.mrecv_unlink_bytes)
				unlinked = true;
		} else {
			if ((parent->recv.ulen - parent->recv.mrecv_bytes) < rxc->min_multi_recv)
				unlinked = true;
		}

		if (unlinked) {
			RXC_DBG(rxc, "Freeing parent: %p\n", req->recv.parent);
			cxip_recv_req_free(req->recv.parent);

			req->flags |= FI_MULTI_RECV;
		}
	}

	if (req->recv.rc == C_RC_OK && (!truncated || rxc->trunc_ok)) {
		RXC_DBG(rxc, "Request success: %p\n", req);

		/* Completion requested or mandatory FI_MULTI_RECV
		 * buffer un-link completion
		 */
		if (success_event) {
			if (truncated)
				req->flags |= FI_CXI_TRUNC;

			ret = recv_req_event_success(rxc, req);
			if (ret != FI_SUCCESS)
				RXC_WARN(rxc,
					 "Failed to report completion: %d\n",
					 ret);
		}

		if (req->recv.cntr) {
			ret = cxip_cntr_mod(req->recv.cntr, 1, false, false);
			if (ret)
				RXC_WARN(rxc, "cxip_cntr_mod returned: %d\n",
					 ret);
		}
	} else {
		if (req->recv.unlinked) {
			err = FI_ECANCELED;
			if (req->recv.multi_recv)
				req->flags |= FI_MULTI_RECV;
			RXC_DBG(rxc, "Request canceled: %p (err: %d)\n", req,
				err);
		} else if (truncated) {
			err = FI_ETRUNC;
			RXC_DBG(rxc, "Request truncated: %p (err: %d)\n", req,
				err);
		} else if (req->recv.flags & FI_PEEK) {
			req->data_len = 0;
			err = FI_ENOMSG;
			RXC_DBG(rxc, "Peek request not found: %p (err: %d)\n",
				req, err);
		} else {
			err = proverr2errno(req->recv.rc);

			RXC_WARN(rxc, "Request error: %p (err: %d, %s)\n", req,
				 err, cxi_rc_to_str(req->recv.rc));
		}

		ret = cxip_cq_req_error(req, truncated, err, req->recv.rc,
					NULL, 0, FI_ADDR_UNSPEC);
		if (ret != FI_SUCCESS)
			RXC_WARN(rxc, "Failed to report error: %d\n", ret);

		if (req->recv.cntr) {
			ret = cxip_cntr_mod(req->recv.cntr, 1, false, true);
			if (ret)
				RXC_WARN(rxc, "cxip_cntr_mod returned: %d\n",
					 ret);
		}
	}
}

/*
 * mrecv_req_dup() - Create a new request using an event targeting a
 * multi-recv buffer.
 *
 * @mrecv_req: A previously posted multi-recv buffer request.
 */
struct cxip_req *cxip_mrecv_req_dup(struct cxip_req *mrecv_req)
{
	struct cxip_rxc *rxc = mrecv_req->recv.rxc;
	struct cxip_req *req;

	req = cxip_evtq_req_alloc(&rxc->rx_evtq, 0, rxc);
	if (!req)
		return NULL;

	/* Duplicate the parent request. */
	req->cb = mrecv_req->cb;
	req->context = mrecv_req->context;
	req->flags = mrecv_req->flags;
	req->type = mrecv_req->type;
	req->recv = mrecv_req->recv;

	/* Update fields specific to this Send */
	req->recv.parent = mrecv_req;

	/* Start pointer and data_len must be set elsewhere! */

	return req;
}

/*
 * recv_req_peek_complete - FI_PEEK operation completed
 */
void cxip_recv_req_peek_complete(struct cxip_req *req,
				 struct cxip_ux_send *ux_send)
{
	/* If no unexpected message match we need to return original
	 * tag in the completion.
	 */
	if (req->recv.rc != C_RC_OK)
		req->tag = req->recv.tag;
	else if (req->recv.flags & FI_CLAIM)
		((struct fi_context *)req->context)->internal[0] = ux_send;

	/* Avoid truncation processing, peek does not receive data */
	req->data_len = req->recv.rlen;

	cxip_recv_req_report(req);

	cxip_recv_req_free(req);
}

/*
 * cxip_complete_put() - Common C_EVENT_PUT success event processing
 *
 * Data is delivered directly to user buffer.
 */
int cxip_complete_put(struct cxip_req *req, const union c_event *event)
{
	if (req->recv.multi_recv) {
		if (event->tgt_long.auto_unlinked) {
			uintptr_t mrecv_head;

			/* Special C_EVENT_PUT case when FI_MULTI_RECV was
			 * requested, but FI_COMPLETION was not specified.
			 * Must generate an FI_MULTI_RECV completion associated
			 * with only the un-link of the buffer.
			 */
			if (!(req->flags & FI_COMPLETION)) {
				req->recv.auto_unlinked = true;
				req->recv.rxc->ops.recv_req_tgt_event(req,
								      event);
				req->flags = FI_MULTI_RECV;
				req->recv.rlen = 0;
				req->data_len = 0;
				req->tag = 0;
				req->buf = (uint64_t)NULL;

				cxip_recv_req_report(req);
				cxip_recv_req_free(req);

				return FI_SUCCESS;
			}

			/* For C_EVENT_PUT, need to calculate how much of the
			 * multi-recv buffer was consumed while factoring in
			 * any truncation.
			 */
			mrecv_head = CXI_IOVA_TO_VA(req->recv.recv_md->md,
						    event->tgt_long.start);

			req->recv.auto_unlinked = true;
			req->recv.mrecv_unlink_bytes = mrecv_head -
						(uintptr_t)req->recv.recv_buf +
						event->tgt_long.mlength;
		}

		req = cxip_mrecv_req_dup(req);
		if (!req)
			return -FI_EAGAIN;

		req->recv.rxc->ops.recv_req_tgt_event(req, event);
		req->buf = (uint64_t)(CXI_IOVA_TO_VA(req->recv.recv_md->md,
						     event->tgt_long.start));
		req->data_len = event->tgt_long.mlength;

		cxip_recv_req_report(req);
		cxip_evtq_req_free(req);
	} else {
		req->data_len = event->tgt_long.mlength;
		req->recv.rxc->ops.recv_req_tgt_event(req, event);
		cxip_recv_req_report(req);
		cxip_recv_req_free(req);
	}

	return FI_SUCCESS;
}

/* Caller must hold ep_obj->lock */
int cxip_recv_pending_ptlte_disable(struct cxip_rxc *rxc,
				    bool check_fc)
{
	int ret;

	assert(rxc->state == RXC_ENABLED ||
	       rxc->state == RXC_ONLOAD_FLOW_CONTROL ||
	       rxc->state == RXC_ONLOAD_FLOW_CONTROL_REENABLE ||
	       rxc->state == RXC_FLOW_CONTROL ||
	       rxc->state == RXC_PENDING_PTLTE_SOFTWARE_MANAGED ||
	       rxc->state == RXC_PENDING_PTLTE_DISABLE);

	/* Having flow control triggered while in flow control is a sign of LE
	 * exhaustion. Software endpoint mode is required to scale past hardware
	 * LE limit.
	 */
	if (check_fc && rxc->state == RXC_FLOW_CONTROL)
		RXC_FATAL(rxc, FC_SW_LE_MSG_FATAL);

	if (rxc->state != RXC_ENABLED)
		return FI_SUCCESS;

	RXC_DBG(rxc, "Manual request PTLTE_DISABLED\n");

	ret = cxip_pte_set_state(rxc->rx_pte, rxc->rx_cmdq,
				 C_PTLTE_DISABLED, 0);
	if (ret == FI_SUCCESS)
		rxc->state = RXC_PENDING_PTLTE_DISABLE;

	return ret;
}

void cxip_rxc_record_req_stat(struct cxip_rxc *rxc, enum c_ptl_list list,
			      size_t rlength, struct cxip_req *req)
{
	enum fi_hmem_iface iface = rlength ? req->recv.recv_md->info.iface :
					     FI_HMEM_SYSTEM;

	cxip_msg_counters_msg_record(&rxc->cntrs, list, iface, rlength);
}

/*
 * cxip_recv_cancel() - Cancel outstanding receive request.
 */
int cxip_recv_cancel(struct cxip_req *req)
{
	int ret = FI_SUCCESS;
	struct cxip_rxc *rxc = req->recv.rxc;

	/* In hybrid mode requests could be on priority list
	 * or software receive list.
	 */
	if (req->recv.software_list) {
		dlist_remove_init(&req->recv.rxc_entry);
		req->recv.canceled = true;
		req->recv.unlinked = true;
		cxip_recv_req_report(req);
		cxip_recv_req_free(req);
	} else {
		ret = cxip_pte_unlink(rxc->rx_pte, C_PTL_LIST_PRIORITY,
				req->req_id, rxc->rx_cmdq);
		if (ret == FI_SUCCESS)
			req->recv.canceled = true;
	}
	return ret;
}

/*
 * tag_match() - Compare match bits
 */
bool tag_match(uint64_t init_mb, uint64_t mb, uint64_t ib)
{
	return !((init_mb ^ mb) & ~ib);
}

/*
 * init_match() - Compare UX Send initiator and Receive initiator in SW.
 */
bool init_match(struct cxip_rxc *rxc, uint32_t init, uint32_t match_id)
{
	if (match_id == CXI_MATCH_ID_ANY)
		return true;

	if (rxc->ep_obj->av->symmetric) {
		init = CXI_MATCH_ID_EP(rxc->pid_bits, init);
		match_id = CXI_MATCH_ID_EP(rxc->pid_bits, match_id);
	}

	return init == match_id;
}

/*
 * cxip_flush_appends() - Flush all user appends for a RXC.
 *
 * Before cxip_ux_onload() can be called, all user appends in the command queue
 * must be flushed. If not, this can cause cxip_ux_onload() to read incorrect
 * remote offsets from cxil_pte_status(). The flush is implemented by issuing
 * a search command which will match zero ULEs. When the search event is
 * processed, all pending user appends will have been processed. Since the RXC
 * is not enabled, new appends cannot occur during this time.
 *
 * Caller must hold ep_obj->lock.
 */
int cxip_flush_appends(struct cxip_rxc_hpc *rxc,
		       int (*flush_cb)(struct cxip_req *req,
				       const union c_event *event))
{
	struct cxip_req *req;
	union c_cmdu cmd = {};
	int ret;

	/* Populate request */
	req = cxip_evtq_req_alloc(&rxc->base.rx_evtq, 1, rxc);
	if (!req) {
		RXC_DBG(rxc, "Failed to allocate request\n");
		ret = -FI_EAGAIN;
		goto err;
	}
	ofi_atomic_inc32(&rxc->base.orx_reqs);

	rxc->base.rx_evtq.ack_batch_size = 1;

	req->cb = flush_cb;
	req->type = CXIP_REQ_SEARCH;

	/* Search command which should match nothing. */
	cmd.command.opcode = C_CMD_TGT_SEARCH;
	cmd.target.ptl_list = C_PTL_LIST_UNEXPECTED;
	cmd.target.ptlte_index = rxc->base.rx_pte->pte->ptn;
	cmd.target.buffer_id = req->req_id;
	cmd.target.match_bits = -1UL;
	cmd.target.length = 0;

	ret = cxi_cq_emit_target(rxc->base.rx_cmdq->dev_cmdq, &cmd);
	if (ret) {
		RXC_WARN(rxc, "Failed to write Search command: %d\n", ret);
		ret = -FI_EAGAIN;
		goto err_dec_free_cq_req;
	}

	cxi_cq_ring(rxc->base.rx_cmdq->dev_cmdq);

	return FI_SUCCESS;

err_dec_free_cq_req:
	ofi_atomic_dec32(&rxc->base.orx_reqs);
	cxip_evtq_req_free(req);
err:
	return ret;
}

/*
 * cxip_recv_req_dropped() - Mark the Received request dropped.
 *
 * If HW does not have sufficient LEs to perform an append, the command is
 * dropped. Queue the request for replay. When all outstanding append commands
 * complete, replay all Receives.
 *
 * Caller must hold ep_obj->lock
 */
int cxip_recv_req_dropped(struct cxip_req *req)
{
	struct cxip_rxc_hpc *rxc = req->recv.rxc_hpc;

	assert(rxc->base.protocol == FI_PROTO_CXI);
	assert(dlist_empty(&req->recv.rxc_entry));

	dlist_insert_tail(&req->recv.rxc_entry, &rxc->replay_queue);

	RXC_DBG(rxc, "Receive dropped: %p\n", req);

	return FI_SUCCESS;
}

/*
 * _txc_fi_addr() - Return the FI address of the TXC.
 */
static fi_addr_t _txc_fi_addr(struct cxip_txc *txc)
{
	if (txc->ep_obj->fi_addr == FI_ADDR_NOTAVAIL) {
		txc->ep_obj->fi_addr =
				cxip_av_lookup_fi_addr(txc->ep_obj->av,
						       &txc->ep_obj->src_addr);
		TXC_DBG(txc, "Found EP FI Addr: %lu\n", txc->ep_obj->fi_addr);
	}

	return txc->ep_obj->fi_addr;
}

/*
 * cxip_msg_match_id() - Return the TXC's initiator address used to transmit a
 * message.
 *
 * By default, the physical address of the TXC is returned. This address is
 * sent along with message data and is used for source address matching at the
 * target. When the target receives a message, the physical ID is translated to
 * a logical FI address. Translation adds overhead to the receive path.
 *
 * As an optimization, if rendezvous offload is not being used and the process
 * is part of a job with symmetric AVs, a logical FI address is returned. This
 * way, there is no source address translation overhead involved in the
 * receive.
 */
uint32_t cxip_msg_match_id(struct cxip_txc *txc)
{
	/* PID is not used for logical matching, but is used for rendezvous. */
	if (txc->ep_obj->av->symmetric)
		return CXI_MATCH_ID(txc->pid_bits, txc->ep_obj->src_addr.pid,
				    _txc_fi_addr(txc));

	return CXI_MATCH_ID(txc->pid_bits, txc->ep_obj->src_addr.pid,
			    txc->ep_obj->src_addr.nic);
}

/*
 * report_send_completion() - Report the completion of a send operation.
 */
void cxip_report_send_completion(struct cxip_req *req, bool sw_cntr)
{
	int ret;
	int ret_err;
	int success_event = (req->flags & FI_COMPLETION);
	struct cxip_txc *txc = req->send.txc;

	req->flags &= (FI_MSG | FI_TAGGED | FI_SEND | FI_CXI_TRUNC);

	if (req->send.rc == C_RC_OK) {
		TXC_DBG(txc, "Request success: %p\n", req);

		if (success_event) {
			ret = cxip_cq_req_complete(req);
			if (ret != FI_SUCCESS)
				TXC_WARN(txc,
					 "Failed to report completion: %d\n",
					 ret);
		}

		if (sw_cntr && req->send.cntr) {
			ret = cxip_cntr_mod(req->send.cntr, 1, false, false);
			if (ret)
				TXC_WARN(txc, "cxip_cntr_mod returned: %d\n",
					 ret);
		}
	} else {
		if (req->send.canceled) {
			ret_err = FI_ECANCELED;
			TXC_DBG(txc, "Request canceled: %p (err: %d)\n",
				req, ret_err);
		} else {
			ret_err = proverr2errno(req->send.rc);
		}

		TXC_WARN(txc, "Request dest_addr: %ld caddr.nic: %#X caddr.pid: %u error: %p (err: %d, %s)\n",
			 req->send.dest_addr, req->send.caddr.nic,
			 req->send.caddr.pid, req, ret_err,
			 cxi_rc_to_str(req->send.rc));

		ret = cxip_cq_req_error(req, 0, ret_err,
					req->send.rc, NULL, 0,
					FI_ADDR_UNSPEC);
		if (ret != FI_SUCCESS)
			TXC_WARN(txc, "Failed to report error: %d\n", ret);

		if (sw_cntr && req->send.cntr) {
			ret = cxip_cntr_mod(req->send.cntr, 1, false, true);
			if (ret)
				TXC_WARN(txc, "cxip_cntr_mod returned: %d\n",
					 ret);
		}
	}
}

bool cxip_send_eager_idc(struct cxip_req *req)
{
	return (req->send.len <= CXIP_INJECT_SIZE) &&
		!cxip_env.disable_non_inject_msg_idc;
}

void cxip_send_buf_fini(struct cxip_req *req)
{
	if (req->send.send_md && !req->send.hybrid_md)
		cxip_unmap(req->send.send_md);
	if (req->send.ibuf)
		cxip_txc_ibuf_free(req->send.txc, req->send.ibuf);
}

int cxip_send_buf_init(struct cxip_req *req)
{
	struct cxip_txc *txc = req->send.txc;
	int ret;

	/* Nothing to do for zero byte sends. */
	if (!req->send.len)
		return FI_SUCCESS;

	/* Triggered operation always requires memory registration. */
	if (req->triggered)
		return cxip_map(txc->domain, req->send.buf, req->send.len, 0,
			       &req->send.send_md);

	/* FI_INJECT operations always require an internal bounce buffer. This
	 * is needed to replay FI_INJECT operations which may experience flow
	 * control.
	 */
	if (req->send.flags & FI_INJECT) {

		req->send.ibuf = cxip_txc_ibuf_alloc(txc);
		if (!req->send.ibuf)
			return -FI_EAGAIN;

		if (txc->hmem) {
			ret = cxip_txc_copy_from_hmem(txc, NULL, req->send.ibuf,
						      req->send.buf,
						      req->send.len);
			if (ret) {
				TXC_WARN(txc,
					 "cxip_txc_copy_from_hmem failed: %d:%s\n",
					 ret, fi_strerror(-ret));
				goto err_buf_fini;
			}

			return FI_SUCCESS;
		}

		memcpy(req->send.ibuf, req->send.buf, req->send.len);
		return FI_SUCCESS;
	}

	/* If message is going to be sent as an IDC, a bounce buffer is needed
	 * if FI_HMEM is being used. This is due to the buffer type being
	 * unknown.
	 */
	if (cxip_send_eager_idc(req)) {
		if (txc->hmem) {

			req->send.ibuf = cxip_txc_ibuf_alloc(txc);
			if (!req->send.ibuf) {
				ret = -FI_EAGAIN;
				goto err_buf_fini;
			}

			ret = cxip_txc_copy_from_hmem(txc, NULL, req->send.ibuf,
						      req->send.buf,
						      req->send.len);
			if (ret) {
				TXC_WARN(txc,
					 "cxip_txc_copy_from_hmem failed: %d:%s\n",
					 ret, fi_strerror(-ret));
				goto err_buf_fini;
			}
		}

		return FI_SUCCESS;
	}

	/* Everything else requires memory registeration. */
	return cxip_map(txc->domain, req->send.buf, req->send.len, 0,
			&req->send.send_md);

err_buf_fini:
	cxip_send_buf_fini(req);

	return ret;
}

/*
 * Libfabric APIs
 */
static ssize_t cxip_trecv(struct fid_ep *fid_ep, void *buf, size_t len,
			  void *desc, fi_addr_t src_addr, uint64_t tag,
			  uint64_t ignore, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	uint64_t flags = ep->rx_attr.op_flags & ~FI_MULTI_RECV;

	return rxc->ops.recv_common(rxc, buf, len, desc, src_addr, tag, ignore,
				    context, flags, true, NULL);
}

static ssize_t cxip_trecvv(struct fid_ep *fid_ep, const struct iovec *iov,
			   void **desc, size_t count, fi_addr_t src_addr,
			   uint64_t tag, uint64_t ignore, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	uint64_t flags = ep->rx_attr.op_flags & ~FI_MULTI_RECV;
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	size_t len;
	void *buf;
	void *mr_desc;

	if (count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (iov && count == 1) {
		len = iov[0].iov_len;
		buf = iov[0].iov_base;
		mr_desc = desc ? desc[0] : NULL;
	} else {
		RXC_WARN(rxc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	return rxc->ops.recv_common(rxc, buf, len, mr_desc, src_addr, tag,
				    ignore, context, flags, true, NULL);
}

static ssize_t cxip_trecvmsg(struct fid_ep *fid_ep,
			     const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	size_t len;
	void *buf;
	void *mr_desc;

	if (flags & ~(CXIP_RX_OP_FLAGS | CXIP_RX_IGNORE_OP_FLAGS |
		      FI_PEEK | FI_CLAIM))
		return -FI_EBADFLAGS;

	if (!msg) {
		RXC_WARN(rxc, "NULL msg not supported\n");
		return -FI_EINVAL;
	}

	flags &= ~FI_MULTI_RECV;

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!rxc->selective_completion)
		flags |= FI_COMPLETION;

	if (!(flags & FI_PEEK)) {
		if (msg->iov_count == 0) {
			len = 0;
			buf = NULL;
			mr_desc = NULL;
		} else if (msg->msg_iov && msg->iov_count == 1) {
			len = msg->msg_iov[0].iov_len;
			buf = msg->msg_iov[0].iov_base;
			mr_desc = msg->desc ? msg->desc[0] : NULL;
		} else {
			RXC_WARN(rxc, "Invalid IOV\n");
			return -FI_EINVAL;
		}

		return rxc->ops.recv_common(rxc, buf, len, mr_desc, msg->addr,
					    msg->tag, msg->ignore, msg->context,
					    flags, true, NULL);
	}

	/* FI_PEEK does not post a recv or return message payload */
	return rxc->ops.recv_common(rxc, NULL, 0UL, NULL, msg->addr, msg->tag,
				    msg->ignore, msg->context, flags, true,
				    NULL);
}

static ssize_t cxip_tsend(struct fid_ep *fid_ep, const void *buf, size_t len,
			  void *desc, fi_addr_t dest_addr, uint64_t tag,
			  void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, desc, 0,
				    dest_addr, tag, context,
				    ep->tx_attr.op_flags, true, false, 0,
				    NULL, NULL);
}

static ssize_t cxip_tsendv(struct fid_ep *fid_ep, const struct iovec *iov,
			   void **desc, size_t count, fi_addr_t dest_addr,
			   uint64_t tag, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	size_t len;
	const void *buf;
	void *mr_desc;

	if (count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (iov && count == 1) {
		len = iov[0].iov_len;
		buf = iov[0].iov_base;
		mr_desc = desc ? desc[0] : NULL;
	} else {
		TXC_WARN(txc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, mr_desc,
				    0, dest_addr, tag, context,
				    ep->tx_attr.op_flags, true, false, 0,
				    NULL, NULL);
}

static ssize_t cxip_tsendmsg(struct fid_ep *fid_ep,
			     const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	size_t len;
	const void *buf;
	void *mr_desc;

	if (!msg) {
		TXC_WARN(txc, "NULL msg not supported\n");
		return -FI_EINVAL;
	}

	if (msg->iov_count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (msg->msg_iov && msg->iov_count == 1) {
		len = msg->msg_iov[0].iov_len;
		buf = msg->msg_iov[0].iov_base;
		mr_desc = msg->desc ? msg->desc[0] : NULL;
	} else {
		TXC_WARN(txc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	if (flags & ~CXIP_TX_OP_FLAGS)
		return -FI_EBADFLAGS;

	if (flags & FI_FENCE && !(txc->attr.caps & FI_FENCE))
		return -FI_EINVAL;

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!txc->selective_completion)
		flags |= FI_COMPLETION;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, mr_desc,
				    msg->data, msg->addr, msg->tag,
				    msg->context, flags, true, false, 0,
				    NULL, NULL);
}

static ssize_t cxip_tinject(struct fid_ep *fid_ep, const void *buf, size_t len,
			    fi_addr_t dest_addr, uint64_t tag)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, NULL, 0,
				    dest_addr, tag, NULL, FI_INJECT, true,
				    false, 0, NULL, NULL);
}

static ssize_t cxip_tsenddata(struct fid_ep *fid_ep, const void *buf,
			      size_t len, void *desc, uint64_t data,
			      fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, desc,
				    data, dest_addr, tag, context,
				    ep->tx_attr.op_flags | FI_REMOTE_CQ_DATA,
				    true, false, 0, NULL, NULL);
}

static ssize_t cxip_tinjectdata(struct fid_ep *fid_ep, const void *buf,
				size_t len, uint64_t data, fi_addr_t dest_addr,
				uint64_t tag)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, NULL,
				    data, dest_addr, tag, NULL,
				    FI_INJECT | FI_REMOTE_CQ_DATA, true, false,
				    0, NULL, NULL);
}

struct fi_ops_tagged cxip_ep_tagged_no_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
	.send = fi_no_tagged_send,
	.sendv = fi_no_tagged_sendv,
	.sendmsg = fi_no_tagged_sendmsg,
	.inject = fi_no_tagged_inject,
	.senddata = fi_no_tagged_senddata,
	.injectdata = fi_no_tagged_injectdata,
};

struct fi_ops_tagged cxip_ep_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = cxip_trecv,
	.recvv = cxip_trecvv,
	.recvmsg = cxip_trecvmsg,
	.send = cxip_tsend,
	.sendv = cxip_tsendv,
	.sendmsg = cxip_tsendmsg,
	.inject = cxip_tinject,
	.senddata = cxip_tsenddata,
	.injectdata = cxip_tinjectdata,
};

struct fi_ops_tagged cxip_ep_tagged_no_tx_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = cxip_trecv,
	.recvv = cxip_trecvv,
	.recvmsg = cxip_trecvmsg,
	.send = fi_no_tagged_send,
	.sendv = fi_no_tagged_sendv,
	.sendmsg = fi_no_tagged_sendmsg,
	.inject = fi_no_tagged_inject,
	.senddata = fi_no_tagged_senddata,
	.injectdata = fi_no_tagged_injectdata,
};

struct fi_ops_tagged cxip_ep_tagged_no_rx_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
	.send = cxip_tsend,
	.sendv = cxip_tsendv,
	.sendmsg = cxip_tsendmsg,
	.inject = cxip_tinject,
	.senddata = cxip_tsenddata,
	.injectdata = cxip_tinjectdata,
};

static ssize_t cxip_recv(struct fid_ep *fid_ep, void *buf, size_t len,
			 void *desc, fi_addr_t src_addr, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;

	return rxc->ops.recv_common(rxc, buf, len, desc, src_addr, 0, 0,
				    context, ep->rx_attr.op_flags, false, NULL);
}

static ssize_t cxip_recvv(struct fid_ep *fid_ep, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t src_addr,
			  void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	size_t len;
	void *buf;
	void *mr_desc;

	if (count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (iov && count == 1) {
		len = iov[0].iov_len;
		buf = iov[0].iov_base;
		mr_desc = desc ? desc[0] : NULL;
	} else {
		RXC_WARN(rxc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	return rxc->ops.recv_common(rxc, buf, len, mr_desc, src_addr, 0, 0,
				    context, ep->rx_attr.op_flags, false, NULL);
}

static ssize_t cxip_recvmsg(struct fid_ep *fid_ep, const struct fi_msg *msg,
			    uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_rxc *rxc = ep->ep_obj->rxc;
	size_t len;
	void *buf;
	void *mr_desc;

	if (flags & ~(CXIP_RX_OP_FLAGS | CXIP_RX_IGNORE_OP_FLAGS))
		return -FI_EBADFLAGS;

	if (!msg) {
		RXC_WARN(rxc, "NULL msg not supported\n");
		return -FI_EINVAL;
	}

	if (msg->iov_count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (msg->msg_iov && msg->iov_count == 1) {
		len = msg->msg_iov[0].iov_len;
		buf = msg->msg_iov[0].iov_base;
		mr_desc = msg->desc ? msg->desc[0] : NULL;
	} else {
		RXC_WARN(rxc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!rxc->selective_completion)
		flags |= FI_COMPLETION;

	return rxc->ops.recv_common(rxc, buf, len, mr_desc, msg->addr, 0, 0,
				    msg->context, flags, false, NULL);
}

static ssize_t cxip_send(struct fid_ep *fid_ep, const void *buf, size_t len,
			 void *desc, fi_addr_t dest_addr, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, desc, 0,
				    dest_addr, 0, context, ep->tx_attr.op_flags,
				    false, false, 0, NULL, NULL);
}

static ssize_t cxip_sendv(struct fid_ep *fid_ep, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t dest_addr,
			  void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	size_t len;
	const void *buf;
	void *mr_desc;

	if (count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (iov && count == 1) {
		len = iov[0].iov_len;
		buf = iov[0].iov_base;
		mr_desc = desc ? desc[0] : NULL;
	} else {
		TXC_WARN(txc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, mr_desc,
				    0, dest_addr, 0, context,
				    ep->tx_attr.op_flags, false, false, 0,
				    NULL, NULL);
}

static ssize_t cxip_sendmsg(struct fid_ep *fid_ep, const struct fi_msg *msg,
			    uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	size_t len;
	const void *buf;
	void *mr_desc;

	if (!msg) {
		TXC_WARN(txc, "NULL msg not supported\n");
		return -FI_EINVAL;
	}

	if (msg->iov_count == 0) {
		len = 0;
		buf = NULL;
		mr_desc = NULL;
	} else if (msg->msg_iov && msg->iov_count == 1) {
		len = msg->msg_iov[0].iov_len;
		buf = msg->msg_iov[0].iov_base;
		mr_desc = msg->desc ? msg->desc[0] : NULL;
	} else {
		TXC_WARN(txc, "Invalid IOV\n");
		return -FI_EINVAL;
	}

	if (flags & ~CXIP_TX_OP_FLAGS)
		return -FI_EBADFLAGS;

	if (flags & FI_FENCE && !(txc->attr.caps & FI_FENCE))
		return -FI_EINVAL;

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!txc->selective_completion)
		flags |= FI_COMPLETION;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, mr_desc,
				    msg->data, msg->addr, 0, msg->context,
				    flags, false, false, 0, NULL, NULL);
}

static ssize_t cxip_inject(struct fid_ep *fid_ep, const void *buf, size_t len,
			   fi_addr_t dest_addr)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, NULL, 0,
				    dest_addr, 0, NULL, FI_INJECT, false, false,
				    0, NULL, NULL);
}

static ssize_t cxip_senddata(struct fid_ep *fid_ep, const void *buf, size_t len,
			     void *desc, uint64_t data, fi_addr_t dest_addr,
			     void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, desc,
				    data, dest_addr, 0, context,
				    ep->tx_attr.op_flags | FI_REMOTE_CQ_DATA,
				    false, false, 0, NULL, NULL);
}

static ssize_t cxip_injectdata(struct fid_ep *fid_ep, const void *buf,
			       size_t len, uint64_t data, fi_addr_t dest_addr)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	return txc->ops.send_common(txc, ep->tx_attr.tclass, buf, len, NULL,
				    data, dest_addr, 0, NULL,
				    FI_INJECT | FI_REMOTE_CQ_DATA, false, false,
				    0, NULL, NULL);
}

struct fi_ops_msg cxip_ep_msg_no_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_msg_recv,
	.recvv = fi_no_msg_recvv,
	.recvmsg = fi_no_msg_recvmsg,
	.send = fi_no_msg_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = fi_no_msg_sendmsg,
	.inject = fi_no_msg_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

struct fi_ops_msg cxip_ep_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = cxip_recv,
	.recvv = cxip_recvv,
	.recvmsg = cxip_recvmsg,
	.send = cxip_send,
	.sendv = cxip_sendv,
	.sendmsg = cxip_sendmsg,
	.inject = cxip_inject,
	.senddata = cxip_senddata,
	.injectdata = cxip_injectdata,
};

struct fi_ops_msg cxip_ep_msg_no_tx_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = cxip_recv,
	.recvv = cxip_recvv,
	.recvmsg = cxip_recvmsg,
	.send = fi_no_msg_send,
	.sendv = fi_no_msg_sendv,
	.sendmsg = fi_no_msg_sendmsg,
	.inject = fi_no_msg_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

struct fi_ops_msg cxip_ep_msg_no_rx_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_msg_recv,
	.recvv = fi_no_msg_recvv,
	.recvmsg = fi_no_msg_recvmsg,
	.send = cxip_send,
	.sendv = cxip_sendv,
	.sendmsg = cxip_sendmsg,
	.inject = cxip_inject,
	.senddata = cxip_senddata,
	.injectdata = cxip_injectdata,
};
