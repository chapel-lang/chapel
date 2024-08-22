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

#include "cxip.h"

#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_EP_CTRL, __VA_ARGS__)

#define APPEND_LE_FATAL "Recieve LE resources exhuasted. Requires use " \
	" of FI_PROTO_CXI endpoint protocol\n"

static int cxip_rnr_send_cb(struct cxip_req *req, const union c_event *event);

static void cxip_rnr_recv_pte_cb(struct cxip_pte *pte,
				 const union c_event *event)
{
	struct cxip_rxc *rxc = (struct cxip_rxc *)pte->ctx;
	uint32_t state;

	assert(rxc->protocol == FI_PROTO_CXI_RNR);

	switch (event->hdr.event_type) {
	case C_EVENT_STATE_CHANGE:
		if (cxi_event_rc(event) != C_RC_OK ||
		    event->tgt_long.ptlte_index != rxc->rx_pte->pte->ptn)
			CXIP_FATAL("Failed receive PtlTE state change, %s\n",
				   cxi_rc_to_str(cxi_event_rc(event)));

		state = event->tgt_long.initiator.state_change.ptlte_state;

		switch (state) {
		case C_PTLTE_ENABLED:
			assert(rxc->state == RXC_DISABLED);
			rxc->state = RXC_ENABLED;
			RXC_DBG(rxc, "Receive PtlTE enabled\n");
			break;
		case C_PTLTE_DISABLED:
			/* Set to disabled before issuing command */
			assert(rxc->state == RXC_DISABLED);
			rxc->state = RXC_DISABLED;
			RXC_DBG(rxc, "Receive PtlTE disabled\n");
			break;
		default:
			CXIP_FATAL("Unexpected receive PtlTE state %d\n",
				   state);
		}
		break;

	case C_EVENT_COMMAND_FAILURE:
		CXIP_FATAL("Command failure: cq=%u target=%u fail_loc=%u "
			   "cmd_type=%u cmd_size=%u opcode=%u\n",
			   event->cmd_fail.cq_id, event->cmd_fail.is_target,
			   event->cmd_fail.fail_loc,
			   event->cmd_fail.fail_command.cmd_type,
			   event->cmd_fail.fail_command.cmd_size,
			   event->cmd_fail.fail_command.opcode);
		break;
	default:
		CXIP_FATAL("Invalid event type: %s\n", cxi_event_to_str(event));
	}
}

static int cxip_rnr_recv_selective_comp_cb(struct cxip_req *req,
					   const union c_event *event)
{
	int event_rc;
	int ret_err;

	/* When errors happen, send events can occur before the put/get
	 * event. These events should just be dropped.
	 */
	if (event->hdr.event_type == C_EVENT_SEND) {
		CXIP_WARN("Unexpected %s event: rc=%s\n",
			  cxi_event_to_str(event),
			  cxi_rc_to_str(cxi_event_rc(event)));
		return FI_SUCCESS;
	}

	if (req->recv.tagged)
		req->flags = FI_RECV | FI_TAGGED;
	else
		req->flags = FI_RECV | FI_MSG;

	event_rc = cxi_init_event_rc(event);
	ret_err = proverr2errno(event_rc);

	return cxip_cq_req_error(req, 0, ret_err,
				 cxi_event_rc(event), NULL, 0,
				 FI_ADDR_UNSPEC);
}

/*
 * cxip_rnr_recv_req() - Submit Receive request to hardware.
 */
static ssize_t cxip_rnr_recv_req(struct cxip_req *req, struct cxip_cntr *cntr,
				 bool restart_seq)
{
	struct cxip_rxc *rxc = req->recv.rxc;
	uint32_t le_flags;
	union cxip_match_bits mb = {};
	union cxip_match_bits ib = {
		.rnr_cq_data = 1,
	};
	int ret;
	struct cxip_md *recv_md = req->recv.recv_md;
	uint64_t recv_iova = 0;

	if (req->recv.tagged) {
		mb.tagged = 1;
		mb.rnr_tag = req->recv.tag;
		ib.rnr_tag = req->recv.ignore;
	}

	if (req->recv.match_id == CXI_MATCH_ID_ANY)
		ib.rnr_vni = ~0;
	else
		mb.rnr_vni = req->recv.vni;

	/* Always set manage_local in Receive LEs. This makes Cassini ignore
	 * initiator remote_offset in all Puts. With this, remote_offset in Put
	 * events can be used by the initiator for protocol data. The behavior
	 * of use_once is not impacted by manage_local.
	 */
	le_flags = C_LE_EVENT_LINK_DISABLE | C_LE_EVENT_UNLINK_DISABLE |
		   C_LE_MANAGE_LOCAL | C_LE_UNRESTRICTED_BODY_RO |
		   C_LE_UNRESTRICTED_END_RO | C_LE_OP_PUT;

	if (req->recv.success_disable)
		le_flags |= C_LE_EVENT_SUCCESS_DISABLE;

	/* Note: FI_CXI_CNTR_EVENTS_BYTES == FI_CNTR_EVENTS_BYTES */
	if (cntr) {
		le_flags |= C_LE_EVENT_CT_COMM;

		if (cntr->attr.events == FI_CXI_CNTR_EVENTS_BYTES)
			le_flags |= C_LE_EVENT_CT_BYTES;
	}

	if (!req->recv.multi_recv)
		le_flags |= C_LE_USE_ONCE;
	if (restart_seq)
		le_flags |= C_LE_RESTART_SEQ;

	if (recv_md)
		recv_iova = CXI_VA_TO_IOVA(recv_md->md,
					   (uint64_t)req->recv.recv_buf +
					   req->recv.start_offset);

	req->recv.hw_offloaded = true;

	/* Issue Append command */
	ret = cxip_pte_append(rxc->rx_pte, recv_iova,
			      req->recv.ulen - req->recv.start_offset,
			      recv_md ? recv_md->md->lac : 0,
			      C_PTL_LIST_PRIORITY, req->req_id,
			      mb.raw, ib.raw, req->recv.match_id,
			      req->recv.multi_recv ?
			      rxc->min_multi_recv : 0,
			      le_flags, cntr, rxc->rx_cmdq,
			      !(req->recv.flags & FI_MORE));
	if (ret != FI_SUCCESS) {
		RXC_WARN(rxc, "Failed to write Append command: %d\n", ret);
		return ret;
	}

	return FI_SUCCESS;
}

/*
 * cxip_rnr_recv_cb() - Process user receive buffer events.
 *
 * For the CS protocol a receive buffer is described by an LE linked to
 * the Priority List. Local unexpected message buffering and rendezvous
 * messaging are not enabled.
 */
static int cxip_rnr_recv_cb(struct cxip_req *req, const union c_event *event)
{
	struct cxip_rxc_rnr *rxc = req->recv.rxc_rnr;

	switch (event->hdr.event_type) {
	case C_EVENT_LINK:
		/* Success events are disabled */
		assert(cxi_tgt_event_rc(event) != C_RC_OK);

		/* Failure to link a receive buffer is a fatal operation and
		 * indicates that FI_PROTO_CXI and portals flow-control is
		 * required.
		 */
		RXC_FATAL(rxc, APPEND_LE_FATAL);
		break;

	case C_EVENT_UNLINK:
		/* If request is for FI_MULTI_RECV and success events are being
		 * taken (completions required) then cxip_recv_req_report()
		 * will handle making sure the unlink is not reported prior to
		 * all messages being reported.
		 */
		req->recv.unlinked = true;
		cxip_recv_req_report(req);
		cxip_recv_req_free(req);

		return FI_SUCCESS;

	case C_EVENT_PUT:
		cxip_rxc_record_req_stat(&rxc->base, C_PTL_LIST_PRIORITY,
					 event->tgt_long.rlength, req);
		return cxip_complete_put(req, event);
	default:
		RXC_FATAL(rxc, CXIP_UNEXPECTED_EVENT,
			  cxi_event_to_str(event),
			  cxi_rc_to_str(cxi_event_rc(event)));
	}
}

static void cxip_rxc_rnr_progress(struct cxip_rxc *rxc)
{
	cxip_evtq_progress(&rxc->rx_evtq);
}

static void cxip_rxc_rnr_recv_req_tgt_event(struct cxip_req *req,
					    const union c_event *event)
{
	union cxip_match_bits mb = {
		.raw = event->tgt_long.match_bits
	};
	uint32_t init = event->tgt_long.initiator.initiator.process;

	assert(req->recv.rxc->protocol == FI_PROTO_CXI_RNR);
	assert(event->hdr.event_type == C_EVENT_PUT);

	req->tag = mb.rnr_tag;
	req->recv.initiator = init;

	if (mb.rnr_cq_data)
		req->flags |= FI_REMOTE_CQ_DATA;

	req->recv.src_offset = event->tgt_long.remote_offset;

	/* Only need one event to set remaining fields. */
	if (req->recv.tgt_event)
		return;

	req->recv.tgt_event = true;

	/* VNI is needed to support FI_AV_AUTH_KEY. */
	req->recv.vni = event->tgt_long.vni;

	/* rlen is used to detect truncation. */
	req->recv.rlen = event->tgt_long.rlength;

	/* RC is used when generating completion events. */
	req->recv.rc = cxi_tgt_event_rc(event);

	/* Header data is provided in all completion events. */
	req->data = event->tgt_long.header_data;

	/* data_len must be set uniquely for each protocol! */
}

static int cxip_rxc_rnr_cancel_msg_recv(struct cxip_req *req)
{
	/* Perform default */
	return cxip_recv_cancel(req);
}

/* Handle any control messaging callbacks specific to protocol */
static int cxip_rxc_rnr_ctrl_msg_cb(struct cxip_ctrl_req *req,
				    const union c_event *event)
{
	/* Placeholder */
	return -FI_ENOSYS;
}

static void cxip_rxc_rnr_init_struct(struct cxip_rxc *rxc_base,
				     struct cxip_ep_obj *ep_obj)
{
	struct cxip_rxc_rnr *rxc = container_of(rxc_base, struct cxip_rxc_rnr,
						base);

	assert(rxc->base.protocol == FI_PROTO_CXI_RNR);

	/* Supports treating truncation as success */
	rxc->base.trunc_ok = cxip_env.trunc_ok;

	/* Overrides */
	rxc->base.recv_ptl_idx = CXIP_PTL_IDX_RNR_RXQ;
}

static void cxip_rxc_rnr_fini_struct(struct cxip_rxc *rxc)
{
	/* Placeholder */
}

static int cxip_rxc_rnr_msg_init(struct cxip_rxc *rxc_base)
{
	struct cxip_rxc_rnr *rxc = container_of(rxc_base, struct cxip_rxc_rnr,
						base);
	struct cxi_pt_alloc_opts pt_opts = {
		.use_long_event = 1,
		.is_matching = 1,
		.lossless = cxip_env.msg_lossless,
	};
	struct cxip_req *req;
	int ret;

	assert(rxc->base.protocol == FI_PROTO_CXI_RNR);

	if (rxc->base.domain->hybrid_mr_desc) {
		ret = cxip_recv_req_alloc(&rxc->base, NULL, 0, NULL, &req,
					  cxip_rnr_recv_selective_comp_cb);
		if (ret) {
			CXIP_WARN("FI_MSG hybrid req alloc failed\n");
			return -FI_ENOMEM;
		}
		req->context = (uint64_t)rxc->base.context;
		req->flags = FI_MSG | FI_RECV;
		req->recv.success_disable = true;
		rxc->req_selective_comp_msg = req;

		/* Will not be used */
		dlist_init(&req->recv.children);
		dlist_init(&req->recv.rxc_entry);

		/* Selective does not count toward outstanding RX operations */
		ofi_atomic_dec32(&rxc->base.orx_reqs);

		ret = cxip_recv_req_alloc(&rxc->base, NULL, 0, NULL, &req,
					  cxip_rnr_recv_selective_comp_cb);
		if (ret) {
			CXIP_WARN("FI_MSG hybrid req alloc failed\n");
			ret = -FI_ENOMEM;
			goto free_req_msg;
		}
		req->context = (uint64_t)rxc->base.context;
		req->recv.tagged = true;
		req->flags = FI_TAGGED | FI_RECV;
		req->recv.success_disable = true;
		rxc->req_selective_comp_tag = req;

		/* Will not be used */
		dlist_init(&req->recv.children);
		dlist_init(&req->recv.rxc_entry);

		/* Selective does not count toward outstanding RX operations */
		ofi_atomic_dec32(&rxc->base.orx_reqs);
		rxc->hybrid_mr_desc = true;
	}

	/* If applications AVs are symmetric, use logical FI addresses for
	 * matching. Otherwise, physical addresses will be used.
	 */
	if (rxc->base.ep_obj->av->symmetric) {
		CXIP_DBG("Using logical PTE matching\n");
		pt_opts.use_logical = 1;
	}

	ret = cxip_pte_alloc(rxc->base.ep_obj->ptable, rxc->base.rx_evtq.eq,
			     rxc->base.recv_ptl_idx, false, &pt_opts,
			     cxip_rnr_recv_pte_cb, &rxc->base,
			     &rxc->base.rx_pte);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate RX PTE: %d\n", ret);
		goto free_req_tag;
	}

	/* Start accepting Puts. */
	ret = cxip_pte_set_state(rxc->base.rx_pte, rxc->base.rx_cmdq,
				 C_PTLTE_ENABLED, 0);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("cxip_pte_set_state returned: %d\n", ret);
		goto free_pte;
	}

	/* Wait for PTE state change */
	do {
		sched_yield();
		cxip_evtq_progress(&rxc->base.rx_evtq);
	} while (rxc->base.rx_pte->state != C_PTLTE_ENABLED);

	return FI_SUCCESS;

free_pte:
	cxip_pte_free(rxc->base.rx_pte);
free_req_tag:
	if (rxc->req_selective_comp_tag) {
		ofi_atomic_inc32(&rxc->base.orx_reqs);
		cxip_recv_req_free(rxc->req_selective_comp_tag);
	}
free_req_msg:
	if (rxc->req_selective_comp_msg) {
		ofi_atomic_inc32(&rxc->base.orx_reqs);
		cxip_recv_req_free(rxc->req_selective_comp_msg);
	}

	return ret;
}

static int cxip_rxc_rnr_msg_fini(struct cxip_rxc *rxc_base)
{
	struct cxip_rxc_rnr *rxc = container_of(rxc_base, struct cxip_rxc_rnr,
					       base);

	assert(rxc->base.protocol == FI_PROTO_CXI_RNR);

	/* Must add selective completion requests RX reference counts
	 * back before freeing.
	 */
	if (rxc->req_selective_comp_msg) {
		ofi_atomic_inc32(&rxc->base.orx_reqs);
		cxip_recv_req_free(rxc->req_selective_comp_msg);
	}
	if (rxc->req_selective_comp_tag) {
		ofi_atomic_inc32(&rxc->base.orx_reqs);
		cxip_recv_req_free(rxc->req_selective_comp_tag);
	}

	return FI_SUCCESS;
}

static void cxip_rxc_rnr_cleanup(struct cxip_rxc *rxc_base)
{
	cxip_rxc_recv_req_cleanup(rxc_base);
}

/*
 * cxip_recv_common() - Common message receive function. Used for tagged and
 * untagged sends of all sizes.
 */
static ssize_t
cxip_recv_common(struct cxip_rxc *rxc, void *buf, size_t len, void *desc,
		 fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
		 void *context, uint64_t flags, bool tagged,
		 struct cxip_cntr *comp_cntr)
{
	struct cxip_rxc_rnr *rxc_rnr = container_of(rxc, struct cxip_rxc_rnr,
						    base);
	struct cxip_req *req = NULL;
	struct cxip_req *recv_req;
	struct cxip_mr *mr = rxc_rnr->hybrid_mr_desc ? desc : NULL;
	struct cxip_cntr *cntr;
	int ret;
	uint32_t match_id;
	uint16_t vni;

	assert(rxc_rnr->base.protocol == FI_PROTO_CXI_RNR);

#if ENABLE_DEBUG
	if (len && !buf) {
		RXC_WARN(rxc, "Length %ld but local buffer NULL\n", len);
		return -FI_EINVAL;
	}

	if (rxc->state == RXC_DISABLED)
		return -FI_EOPBADSTATE;

	if (tagged) {
		if (tag & ~CXIP_CS_TAG_MASK || ignore & ~CXIP_CS_TAG_MASK) {
			RXC_WARN(rxc,
				 "Invalid tag: %#018lx ignore: %#018lx (%#018lx)\n",
				 tag, ignore, CXIP_CS_TAG_MASK);
			return -FI_EINVAL;
		}
	}
#endif

	if (!rxc->selective_completion)
		flags |= FI_COMPLETION;

	ret = cxip_set_recv_match_id(rxc, src_addr, rxc->ep_obj->av_auth_key &&
				     (flags & FI_AUTH_KEY), &match_id, &vni);
	if (ret) {
		RXC_WARN(rxc, "Error setting match_id: %d %s\n",
			 ret, fi_strerror(-ret));
		return ret;
	}

	ofi_genlock_lock(&rxc->ep_obj->lock);

	if (mr && !(flags & (FI_COMPLETION | FI_MULTI_RECV |
			     FI_PEEK | FI_CLAIM))) {
		recv_req = tagged ? rxc_rnr->req_selective_comp_tag :
				    rxc_rnr->req_selective_comp_msg;
		assert(recv_req != NULL);

		recv_req->recv.recv_md = mr->md;
		recv_req->recv.hybrid_md = true;
		recv_req->recv.recv_buf = buf;
		recv_req->recv.ulen = len;
	} else {
		ret = cxip_recv_req_alloc(rxc, buf, len, mr ? mr->md : NULL,
					  &req, cxip_rnr_recv_cb);
		if (ret)
			goto err;

		recv_req = req;
		recv_req->context = (uint64_t)context;
		recv_req->flags = ((tagged ? FI_TAGGED : FI_MSG) | FI_RECV |
				   (flags & FI_COMPLETION));
		recv_req->recv.tagged = tagged;

		/* Can still disable success events if multi-recv and
		 * completions are not requested since final mandatory unlink
		 * will cleanup resources. However, if buffer will be auto-
		 * unlinked take internal completions to handle the
		 * accounting to ensure all data has landed.
		 */
		if (flags & FI_MULTI_RECV && !(flags & FI_COMPLETION) &&
		    !rxc->min_multi_recv)
			recv_req->recv.success_disable = true;
		else
			recv_req->recv.success_disable = false;
	}
	cntr = comp_cntr ? comp_cntr : rxc->recv_cntr;
	recv_req->recv.match_id = match_id;
	recv_req->recv.vni = vni;
	recv_req->recv.tag = tag;
	recv_req->recv.ignore = ignore;
	recv_req->recv.flags = flags;
	recv_req->recv.multi_recv = (flags & FI_MULTI_RECV ? true : false);

	if (!(recv_req->recv.flags & (FI_PEEK | FI_CLAIM))) {
		ret = cxip_rnr_recv_req(recv_req, cntr, false);
		if (ret) {
			RXC_WARN(rxc, "Receive append failed: %d %s\n",
				 ret, fi_strerror(-ret));
			goto free_req;
		}
		ofi_genlock_unlock(&rxc->ep_obj->lock);

		RXC_DBG(rxc,
			"req: %p buf: %p len: %lu src_addr: %ld tag(%c):"
			" 0x%lx ignore: 0x%lx context: %p\n",
			recv_req, buf, len, src_addr, tagged ? '*' : '-', tag,
			ignore, context);

		return FI_SUCCESS;
	}

	/* No buffered unexpected messages, so FI_PEEK always fails */
	if (recv_req->recv.flags & FI_PEEK) {
		recv_req->recv.rc = C_RC_NO_MATCH;
		cxip_recv_req_peek_complete(recv_req, NULL);
		ofi_genlock_unlock(&rxc->ep_obj->lock);

		return FI_SUCCESS;
	}

	/* FI_CLAIM specified by itself cannot be valid */
	RXC_WARN(rxc, "FI_CLAIM not valid\n");
	ret = -FI_EINVAL;

free_req:
	if (req)
		cxip_recv_req_free(req);
err:
	ofi_genlock_unlock(&rxc->ep_obj->lock);

	return ret;
}

static inline bool cxip_rnr_req_uses_idc(struct cxip_txc_rnr *txc,
					 ssize_t len, bool triggered)

{
	/* TODO: Consider supporting HMEM and IDC by mapping memory */
	return  !txc->base.hmem && len && len <= CXIP_INJECT_SIZE &&
		!triggered && !cxip_env.disable_non_inject_msg_idc;
}

static inline bool cxip_rnr_tx_success_disable(struct cxip_txc_rnr *txc,
					       struct cxip_mr *mr,
					       bool idc, uint64_t flags)
{
	/* Success events can be avoided if we do not require local
	 * memory registration, RNR retries will not be done, and
	 * a user completion is not requested,
	 */
	return (mr || idc) && !txc->max_retry_wait_us &&
		!(flags & FI_COMPLETION);
}

static int cxip_rnr_send_selective_comp_cb(struct cxip_req *req,
					   const union c_event *event)
{
	int event_rc;
	int ret_err;

	/* When errors happen, send events can occur before the put/get
	 * event. These events should just be dropped.
	 */
	if (event->hdr.event_type == C_EVENT_SEND) {
		CXIP_WARN("Unexpected %s event: rc=%s\n",
			  cxi_event_to_str(event),
			  cxi_rc_to_str(cxi_event_rc(event)));
		return FI_SUCCESS;
	}

	if (req->send.tagged)
		req->flags = FI_SEND | FI_TAGGED;
	else
		req->flags = FI_SEND | FI_MSG;

	event_rc = cxi_init_event_rc(event);
	ret_err = proverr2errno(event_rc);

	return cxip_cq_req_error(req, 0, ret_err,
				 cxi_event_rc(event), NULL, 0,
				 FI_ADDR_UNSPEC);
}

static inline ssize_t cxip_rnr_send_dma(struct cxip_req *req,
					union cxip_match_bits *mb,
					union c_fab_addr *dfa, uint8_t idx_ext)
{
	struct cxip_txc *txc = req->send.txc;
	struct c_full_dma_cmd cmd = {};

	cmd.command.cmd_type = C_CMD_TYPE_DMA;
	cmd.command.opcode = C_CMD_PUT;
	cmd.index_ext = idx_ext;
	cmd.event_send_disable = 1;
	cmd.dfa = *dfa;
	cmd.eq = cxip_evtq_eqn(&txc->tx_evtq);
	cmd.user_ptr = (uint64_t)req;
	cmd.initiator = cxip_msg_match_id(txc);
	cmd.match_bits = mb->raw;
	cmd.header_data = req->send.data;
	cmd.event_success_disable = req->send.success_disable;

	/* Triggered ops could result in 0 length DMA */
	if (req->send.send_md) {
		cmd.lac = req->send.send_md->md->lac;
		cmd.local_addr = CXI_VA_TO_IOVA(req->send.send_md->md,
						req->send.buf);
		cmd.request_len = req->send.len;
	}

	if (req->send.cntr) {
		if (req->send.cntr->attr.events == FI_CXI_CNTR_EVENTS_BYTES)
			cmd.event_ct_bytes = 1;

		cmd.event_ct_ack = 1;
		cmd.ct = req->send.cntr->ct->ctn;
	}

	return cxip_txc_emit_dma(txc, req->send.caddr.vni,
				 cxip_ofi_to_cxi_tc(req->send.tclass),
				 CXI_TC_TYPE_DEFAULT,
				 req->triggered ?  req->trig_cntr : NULL,
				 req->trig_thresh, &cmd, req->send.flags);
}

static inline ssize_t cxip_rnr_send_idc(struct cxip_req *req,
					union cxip_match_bits *mb,
					union c_fab_addr *dfa, uint8_t idx_ext)
{
	struct cxip_txc *txc = req->send.txc;
	struct c_cstate_cmd cstate_cmd = {};
	struct c_idc_msg_hdr idc_cmd;

	assert(req->send.len > 0);
	assert(!txc->hmem);

	cstate_cmd.event_send_disable = 1;
	cstate_cmd.index_ext = idx_ext;
	cstate_cmd.eq = cxip_evtq_eqn(&txc->tx_evtq);
	cstate_cmd.initiator = cxip_msg_match_id(txc);
	cstate_cmd.event_success_disable = req->send.success_disable;

	if (req->send.cntr) {
		if (req->send.cntr->attr.events == FI_CXI_CNTR_EVENTS_BYTES)
			cstate_cmd.event_ct_bytes = 1;

		cstate_cmd.event_ct_ack = 1;
		cstate_cmd.ct = req->send.cntr->ct->ctn;
	}

	/* Note: IDC command completely filled in */
	idc_cmd.unused_0 = 0;
	idc_cmd.dfa = *dfa;
	idc_cmd.match_bits = mb->raw;
	idc_cmd.header_data = req->send.data;
	idc_cmd.user_ptr = (uint64_t)req;

	return cxip_txc_emit_idc_msg(txc, req->send.caddr.vni,
				     cxip_ofi_to_cxi_tc(req->send.tclass),
				     CXI_TC_TYPE_DEFAULT, &cstate_cmd, &idc_cmd,
				     req->send.buf, req->send.len,
				     req->send.flags);
}

/* Caller must hold ep_obj->lock */
static ssize_t cxip_rnr_msg_send(struct cxip_req *req)
{
	struct cxip_txc *txc = req->send.txc;
	union cxip_match_bits mb = {
		.rnr_vni = req->send.caddr.vni,
		.rnr_tag = req->send.tag,
		.rnr_cq_data = !!(req->send.flags & FI_REMOTE_CQ_DATA),
	};
	union c_fab_addr dfa;
	uint8_t idx_ext;
	ssize_t ret;
	bool idc = !req->send.send_md || !req->send.len;

	/* Calculate DFA */
	cxi_build_dfa(req->send.caddr.nic, req->send.caddr.pid, txc->pid_bits,
		      txc->recv_ptl_idx, &dfa, &idx_ext);

	if (req->send.send_md || !req->send.len)
		ret = cxip_rnr_send_dma(req, &mb, &dfa, idx_ext);
	else
		ret = cxip_rnr_send_idc(req, &mb, &dfa, idx_ext);
	if (ret) {
		TXC_WARN(txc, "Failed to write %s command: %ld\n",
			 idc ? "IDC" : "DMA", ret);
		return ret;
	}

	TXC_DBG(txc, "Send %s command submitted for req %p\n",
		idc ? "IDC" : "DMA", req);

	return FI_SUCCESS;
}

/* Queue RNR retry. There are CXIP_NUM_RNR_WAIT_QUEUE, each
 * has a consistent time wait for that queue (smaller to larger).
 * Therefore, appends to tail will keep each queue in retry time
 * order.
 *
 * Caller must hold ep_obj->lock
 */
static int cxip_rnr_queue_retry(struct cxip_txc_rnr *txc, struct cxip_req *req)
{
	uint64_t cur_time;
	uint64_t retry_time;
	int index;

	cur_time = ofi_gettime_us();

	index = req->send.retries < CXIP_NUM_RNR_WAIT_QUEUE ?
			req->send.retries : CXIP_NUM_RNR_WAIT_QUEUE - 1;

	/* 1us, 11us, 81us 271us, 641us (max) */
	retry_time = cur_time + 1 + (index * index * index) * 10;
#if 0
	TXC_WARN(txc, "retry_time %ld req->send.max_rnr_time %ld\n",
		 retry_time, req->send.max_rnr_time);
#endif
	if (retry_time > req->send.max_rnr_time)
		return -FI_ETIMEDOUT;

	/* Insert and update next timeout */
	req->send.retry_rnr_time = retry_time;

	dlist_insert_tail(&req->send.rnr_entry, &txc->time_wait_queue[index]);
	if (retry_time < txc->next_retry_wait_us)
		txc->next_retry_wait_us = retry_time;

	req->send.retries++;
	ofi_atomic_inc32(&txc->time_wait_reqs);

	TXC_DBG(txc, "Entry added to txc->time_wait_queue[%d]\n", index);
#if 0
	TXC_WARN(txc,
		 "txc->next_retry_wait_us %ld, req->send.retry_rnr_time %ld\n",
		 txc->next_retry_wait_us, req->send.retry_rnr_time);
#endif

	return FI_SUCCESS;
}

static int cxip_process_rnr_time_wait(struct cxip_txc_rnr *txc)
{
	struct cxip_req *req;
	struct dlist_entry *tmp;
	uint64_t cur_time;
	uint64_t next_time;
	int index;
	int ret;

#if 0
	TXC_WARN(txc, "Process RNR timewait, wait_reqs %d "
		 "txc->next_retry_wait_us %ld\n",
		 ofi_atomic_get32(&txc->time_wait_reqs),
		 txc->next_retry_wait_us);
#endif
	if (!ofi_atomic_get32(&txc->time_wait_reqs))
		return FI_SUCCESS;

	cur_time = ofi_gettime_us();
	if (cur_time < txc->next_retry_wait_us)
		return FI_SUCCESS;

	ret = FI_SUCCESS;
	for (index = 0; index < CXIP_NUM_RNR_WAIT_QUEUE; index++) {
		dlist_foreach_container_safe(&txc->time_wait_queue[index],
					     struct cxip_req, req,
					     send.rnr_entry, tmp) {
#if 0
			TXC_WARN(txc, "req %p, req->send.retry_rnr_time "
				 "%ld cur_time %ld\n", req,
				 req->send.retry_rnr_time, cur_time);
#endif
			if (req->send.retry_rnr_time <= cur_time) {

				/* Do not retry if TX canceled */
				if (req->send.canceled) {
					dlist_remove_init(&req->send.rnr_entry);
					ofi_atomic_dec32(&txc->time_wait_reqs);
					cxip_send_buf_fini(req);
					cxip_report_send_completion(req, true);
					ofi_atomic_dec32(&txc->base.otx_reqs);
					cxip_evtq_req_free(req);

					continue;
				}

				/* Must TX return credit, will take it back if
				 * we could not send.
				 */
				ofi_atomic_dec32(&txc->base.otx_reqs);
				ret = cxip_rnr_msg_send(req);
				if (ret != FI_SUCCESS) {
					ofi_atomic_inc32(&txc->base.otx_reqs);
					goto reset_min_time_wait;
				}

				txc->total_retries++;
				dlist_remove_init(&req->send.rnr_entry);
				ofi_atomic_dec32(&txc->time_wait_reqs);
			} else {
				break;
			}
		}
	}

reset_min_time_wait:
	next_time = UINT64_MAX;

	for (index = 0; index < CXIP_NUM_RNR_WAIT_QUEUE; index++) {
		req = dlist_first_entry_or_null(&txc->time_wait_queue[index],
						struct cxip_req,
						send.rnr_entry);
		if (req && req->send.retry_rnr_time < next_time)
			next_time = req->send.retry_rnr_time;
	}
#if 0
	TXC_WARN(txc, "Set txc->next_retry_wait_us to %ld\n", next_time);
#endif
	txc->next_retry_wait_us = next_time;

	return ret;
}

static void cxip_txc_rnr_progress(struct cxip_txc *txc_base)
{
	struct cxip_txc_rnr *txc = container_of(txc_base, struct cxip_txc_rnr,
						base);

	assert(txc->base.protocol == FI_PROTO_CXI_RNR);

	cxip_evtq_progress(&txc->base.tx_evtq);
	cxip_process_rnr_time_wait(txc);
}

static int cxip_txc_rnr_cancel_msg_send(struct cxip_req *req)
{
	if (req->type != CXIP_REQ_SEND)
		return -FI_ENOENT;

	req->send.canceled = true;

	return FI_SUCCESS;
}

static void cxip_txc_rnr_init_struct(struct cxip_txc *txc_base,
				     struct cxip_ep_obj *ep_obj)
{
	struct cxip_txc_rnr *txc = container_of(txc_base, struct cxip_txc_rnr,
						base);
	int i;

	assert(txc->base.protocol == FI_PROTO_CXI_RNR);

	/* Supports treating truncation as success */
	txc->base.trunc_ok = cxip_env.trunc_ok;

	txc->base.recv_ptl_idx = CXIP_PTL_IDX_RNR_RXQ;
	ofi_atomic_initialize32(&txc->time_wait_reqs, 0);
	txc->max_retry_wait_us = cxip_env.rnr_max_timeout_us;
	txc->next_retry_wait_us = UINT64_MAX;

	for (i = 0; i < CXIP_NUM_RNR_WAIT_QUEUE; i++)
		dlist_init(&txc->time_wait_queue[i]);
}

static void cxip_txc_rnr_fini_struct(struct cxip_txc *txc)
{
	/* Placeholder */
}

static int cxip_txc_rnr_msg_init(struct cxip_txc *txc_base)
{
	struct cxip_txc_rnr *txc = container_of(txc_base, struct cxip_txc_rnr,
						base);
	struct cxip_req *req;

	assert(txc->base.protocol == FI_PROTO_CXI_RNR);

	if (txc->base.domain->hybrid_mr_desc) {
		req = cxip_evtq_req_alloc(&txc->base.tx_evtq, 0, &txc->base);
		if (!req) {
			TXC_WARN(txc, "FI_MSG hybrid req alloc failed\n");
			return -FI_ENOMEM;
		}
		req->type = CXIP_REQ_SEND;
		req->cb = cxip_rnr_send_selective_comp_cb;
		req->context = (uint64_t)txc->base.context;
		req->flags = FI_MSG | FI_SEND;
		req->addr = FI_ADDR_UNSPEC;
		req->send.success_disable = true;
		req->send.txc_rnr = txc;
		txc->req_selective_comp_msg = req;

		req = cxip_evtq_req_alloc(&txc->base.tx_evtq, 0, &txc->base);
		if (!req) {
			TXC_WARN(txc, "FI_TAGGED hybrid req alloc failed\n");
			cxip_evtq_req_free(txc->req_selective_comp_msg);
			txc->req_selective_comp_msg = NULL;
			return -FI_ENOMEM;
		}
		req->type = CXIP_REQ_SEND;
		req->cb = cxip_rnr_send_selective_comp_cb;
		req->context = (uint64_t)txc->base.context;
		req->flags = FI_TAGGED | FI_SEND;
		req->addr = FI_ADDR_UNSPEC;
		req->send.success_disable = true;
		req->send.txc_rnr = txc;
		txc->req_selective_comp_tag = req;

		txc->hybrid_mr_desc = true;
	}

	return FI_SUCCESS;
}

static int cxip_txc_rnr_msg_fini(struct cxip_txc *txc_base)
{
	struct cxip_txc_rnr *txc = container_of(txc_base, struct cxip_txc_rnr,
						base);

	assert(txc->base.protocol == FI_PROTO_CXI_RNR);

	if (txc->req_selective_comp_msg)
		cxip_evtq_req_free(txc->req_selective_comp_msg);
	if (txc->req_selective_comp_tag)
		cxip_evtq_req_free(txc->req_selective_comp_tag);

	TXC_INFO(txc, "Total received RNR nacks %ld, TX retries %ld\n",
		 txc->total_rnr_nacks, txc->total_retries);

	return FI_SUCCESS;
}

static void cxip_txc_rnr_cleanup(struct cxip_txc *txc_base)
{
	/* Placeholder */
}

static void cxip_rnr_send_req_dequeue(struct cxip_req *req)
{
	/* TODO: Place holder for anything additional */

	dlist_remove(&req->send.txc_entry);
}

static int cxip_rnr_send_cb(struct cxip_req *req, const union c_event *event)
{
	struct cxip_txc_rnr *txc = req->send.txc_rnr;
	int rc = cxi_event_rc(event);
	int ret;

#if 0
	TXC_WARN(txc, "Event %s RC %s received\n",
		 cxi_event_to_str(event),
		 cxi_rc_to_str(rc));
#endif

	/* Handle at TX FI_MSG/FI_TAGGED message events */
	if (event->hdr.event_type != C_EVENT_ACK) {
		TXC_WARN(req->send.txc, CXIP_UNEXPECTED_EVENT,
			 cxi_event_to_str(event),
			 cxi_rc_to_str(rc));
		return FI_SUCCESS;
	}

	req->send.rc = rc;

	/* Handle RNR acks */
	if (rc == C_RC_ENTRY_NOT_FOUND &&
	    txc->base.enabled && !req->send.canceled) {

		txc->total_rnr_nacks++;
		ret  = cxip_rnr_queue_retry(txc, req);

		if (ret == FI_SUCCESS)
			return ret;

		TXC_WARN(&txc->base, "req %p RNR max timeout buf: %p len: %lu, "
			 "dest_addr: 0x%lX nic: %#x pid: %d tag(%c) 0x%lx "
			 "retries %u TX outstanding %u\n", req, req->send.buf,
			 req->send.len, req->send.dest_addr,
			 req->send.caddr.nic, req->send.caddr.pid,
			 req->send.tagged ? '*' : '-',
			 req->send.tag, req->send.retries,
			 ofi_atomic_get32(&txc->base.otx_reqs));
	}

	cxip_rnr_send_req_dequeue(req);
	cxip_send_buf_fini(req);

	/* If status is good, then the request completed before it could
	 * be canceled. If canceled, indicate software update of the
	 * error count is required.
	 */
	if (rc == C_RC_OK) {
		req->send.canceled = false;

		/* Report truncation if requested */
		if (txc->base.trunc_ok) {
			req->data_len = event->init_short.mlength;
			if (req->send.len > req->data_len)
				req->flags |= FI_CXI_TRUNC;
		}
	}

	cxip_report_send_completion(req, req->send.canceled);

	ofi_atomic_dec32(&txc->base.otx_reqs);
	cxip_evtq_req_free(req);

	return FI_SUCCESS;
}

/*
 * cxip_send_common() - Common message send function. Used for tagged and
 * untagged sends of all sizes. This includes triggered operations.
 */
static ssize_t
cxip_send_common(struct cxip_txc *txc, uint32_t tclass, const void *buf,
		 size_t len, void *desc, uint64_t data, fi_addr_t dest_addr,
		 uint64_t tag, void *context, uint64_t flags, bool tagged,
		 bool triggered, uint64_t trig_thresh,
		 struct cxip_cntr *trig_cntr, struct cxip_cntr *comp_cntr)
{
	struct cxip_txc_rnr *txc_rnr = container_of(txc, struct cxip_txc_rnr,
						    base);
	struct cxip_mr *mr = txc->domain->hybrid_mr_desc ? desc : NULL;
	struct cxip_req *req = NULL;
	struct cxip_req *send_req;
	struct cxip_addr caddr;
	int ret;
	bool idc;

	assert(txc->protocol == FI_PROTO_CXI_RNR);

#if ENABLE_DEBUG
	if (len && !buf) {
		TXC_WARN(txc, "Length %ld but source buffer NULL\n", len);
		return -FI_EINVAL;
	}
	if (flags & FI_INJECT) {
		TXC_WARN(txc, "FI_INJECT not supported\n");
		return -FI_EINVAL;
	}
#endif
	/* TODO: This check should not be required in other than debug builds,
	 * to do that we would need to return -FI_EFAULT, so leaving here for
	 * now.
	 */
	if (len > CXIP_EP_MAX_MSG_SZ)
		return -FI_EMSGSIZE;

	/* TODO: Move to tagged sends */
	if (tagged && tag & ~CXIP_CS_TAG_MASK) {
		TXC_WARN(txc, "Invalid tag: %#018lx (%#018lx)\n",
			 tag, CXIP_CS_TAG_MASK);
		return -FI_EINVAL;
	}

	ofi_genlock_lock(&txc->ep_obj->lock);
	/* If RNR list is not empty, check if the first retry entry time
	 * wait has expired, and if so force progress to initiate any
	 * read retry/retries.
	 */
	if (txc_rnr->next_retry_wait_us != UINT64_MAX &&
	    ofi_atomic_get32(&txc_rnr->time_wait_reqs)) {
		if (ofi_gettime_us() >= txc_rnr->next_retry_wait_us)
			cxip_txc_rnr_progress(txc);
	}

	idc = cxip_rnr_req_uses_idc(txc_rnr, len, triggered);

	if (cxip_rnr_tx_success_disable(txc_rnr, mr, idc, flags)) {
		/* This request cannot be retried, we use the common request
		 * to pass parameters to the send function. This is done
		 * with exclusive access to the request.
		 */
		send_req = tagged ? txc_rnr->req_selective_comp_tag :
				    txc_rnr->req_selective_comp_msg;
		send_req->send.send_md = NULL;
		send_req->send.hybrid_md = false;
	} else {
		req = cxip_evtq_req_alloc(&txc->tx_evtq, false, txc);
		if (!req) {
			TXC_DBG(txc,
				"Failed to allocate request, ret -FI_EAGAIN\n");
			ret = -FI_EAGAIN;
			goto unlock;
		}
		send_req = req;
		send_req->cb = cxip_rnr_send_cb;
		send_req->type = CXIP_REQ_SEND;
		send_req->send.txc = txc;
		send_req->context = (uint64_t)context;
		send_req->flags = FI_SEND |
				  (flags & (FI_COMPLETION | FI_MATCH_COMPLETE));
		send_req->send.success_disable = false;
	}

	/* Restrict outstanding success event requests to queue size */
	if (ofi_atomic_get32(&txc->otx_reqs) > txc->attr.size) {
		ret = -FI_EAGAIN;
		goto free_req;
	}

	send_req->triggered = triggered;
	send_req->trig_thresh = trig_thresh;
	send_req->trig_cntr = trig_cntr;

	/* Save Send parameters to replay */
	send_req->send.tclass = tclass;
	send_req->send.cntr = triggered ? comp_cntr : txc->send_cntr;
	send_req->send.buf = buf;
	send_req->send.len = len;
	send_req->send.data = data;
	send_req->send.flags = flags;
	/* Set completion parameters */
	if (tagged) {
		send_req->send.tagged = tagged;
		send_req->send.tag = tag;
		send_req->flags |= FI_TAGGED;
	} else {
		send_req->flags |= FI_MSG;
	}

	if (send_req->send.len && !idc) {
		if (!mr) {
			ret = cxip_map(txc->domain, send_req->send.buf,
				       send_req->send.len, 0,
				       &send_req->send.send_md);
			if (ret) {
				TXC_WARN(txc,
					 "Local buffer map failed: %d %s\n",
					 ret, fi_strerror(-ret));
				goto free_req;
			}
		} else {
			send_req->send.send_md = mr->md;
			send_req->send.hybrid_md = true;
		}
	}

	/* Look up target CXI address */
	ret = cxip_av_lookup_addr(txc->ep_obj->av, dest_addr, &caddr);
	if (ret != FI_SUCCESS) {
		TXC_WARN(txc, "Failed to look up FI addr: %d %s\n",
			 ret, fi_strerror(-ret));
		goto free_map;
	}

	if (!txc->ep_obj->av_auth_key)
		caddr.vni = txc->ep_obj->auth_key.vni;

	send_req->send.caddr = caddr;
	send_req->send.dest_addr = dest_addr;

	if (cxip_evtq_saturated(&txc->tx_evtq)) {
		TXC_DBG(txc, "TX HW EQ saturated\n");
		ret = -FI_EAGAIN;
		goto free_map;
	}

	dlist_insert_tail(&send_req->send.txc_entry, &txc->msg_queue);
	send_req->send.max_rnr_time = ofi_gettime_us() +
				      txc_rnr->max_retry_wait_us;

	ret = cxip_rnr_msg_send(send_req);
	if (ret != FI_SUCCESS)
		goto req_dequeue;

	TXC_DBG(txc,
		"req: %p buf: %p len: %lu dest_addr: 0x%lX nic: %d "
		"pid: %d tag(%c): 0x%lx context %#lx\n",
		send_req, send_req->send.buf, send_req->send.len, dest_addr,
		caddr.nic, caddr.pid, send_req->send.tagged ? '*' : '-',
		send_req->send.tag, send_req->context);
	ofi_genlock_unlock(&txc->ep_obj->lock);

	return FI_SUCCESS;

req_dequeue:
	cxip_rnr_send_req_dequeue(send_req);
free_map:
	if (send_req->send.send_md && !send_req->send.hybrid_md)
		cxip_unmap(send_req->send.send_md);
free_req:
	if (req)
		cxip_evtq_req_free(req);
unlock:
	ofi_genlock_unlock(&txc->ep_obj->lock);

	return ret;
}

struct cxip_rxc_ops rnr_rxc_ops = {
	.recv_common = cxip_recv_common,
	.progress = cxip_rxc_rnr_progress,
	.recv_req_tgt_event = cxip_rxc_rnr_recv_req_tgt_event,
	.cancel_msg_recv = cxip_rxc_rnr_cancel_msg_recv,
	.ctrl_msg_cb = cxip_rxc_rnr_ctrl_msg_cb,
	.init_struct = cxip_rxc_rnr_init_struct,
	.fini_struct = cxip_rxc_rnr_fini_struct,
	.cleanup = cxip_rxc_rnr_cleanup,
	.msg_init = cxip_rxc_rnr_msg_init,
	.msg_fini = cxip_rxc_rnr_msg_fini,
};

struct cxip_txc_ops rnr_txc_ops = {
	.send_common = cxip_send_common,
	.progress = cxip_txc_rnr_progress,
	.cancel_msg_send = cxip_txc_rnr_cancel_msg_send,
	.init_struct = cxip_txc_rnr_init_struct,
	.fini_struct = cxip_txc_rnr_fini_struct,
	.cleanup = cxip_txc_rnr_cleanup,
	.msg_init = cxip_txc_rnr_msg_init,
	.msg_fini = cxip_txc_rnr_msg_fini,
};
