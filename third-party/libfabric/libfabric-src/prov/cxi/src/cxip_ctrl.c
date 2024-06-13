/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2017 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2018-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <ofi_util.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

/*
 * cxip_ctrl_msg_cb() - Process control message target events.
 */
int cxip_ctrl_msg_cb(struct cxip_ctrl_req *req, const union c_event *event)
{
	uint32_t pid_bits = req->ep_obj->domain->iface->dev->info.pid_bits;
	uint32_t nic_addr;
	uint32_t pid;
	union cxip_match_bits mb = {
		.raw = event->tgt_long.match_bits,
	};
	uint32_t init = event->tgt_long.initiator.initiator.process;
	int ret;

	/* Check to see if event processing is implemented or overridden
	 * int the protocol. A return of -FI_ENOSYS indicated the event
	 * was not consumed.
	 */
	ret = req->ep_obj->rxc->ops.ctrl_msg_cb(req, event);
	if (ret != -FI_ENOSYS)
		goto done;

	switch (event->hdr.event_type) {
	case C_EVENT_MATCH:
		break;
	case C_EVENT_PUT:
		assert(cxi_event_rc(event) == C_RC_OK);

		nic_addr = CXI_MATCH_ID_EP(pid_bits, init);
		pid = CXI_MATCH_ID_PID(pid_bits, init);

		/* Messages not handled by the protocol */
		if (mb.ctrl_msg_type == CXIP_CTRL_MSG_ZB_DATA) {
			ret = cxip_zbcoll_recv_cb(req->ep_obj, nic_addr, pid,
						  mb.raw);
			assert(ret == FI_SUCCESS);
		} else {
			CXIP_FATAL("Unexpected msg type: %d\n",
				   mb.ctrl_msg_type);
		}
		break;
	default:
		CXIP_FATAL(CXIP_UNEXPECTED_EVENT,
			   cxi_event_to_str(event),
			   cxi_rc_to_str(cxi_event_rc(event)));
	}

done:
	CXIP_DBG("got event: %s rc: %s (req: %p)\n",
		 cxi_event_to_str(event),
		 cxi_rc_to_str(cxi_event_rc(event)),
		 req);

	return FI_SUCCESS;
}

/*
 * cxip_ctrl_msg_send() - Send a control message.
 *
 * Caller should hold req->ep_obj->lock.
 */
int cxip_ctrl_msg_send(struct cxip_ctrl_req *req)
{
	struct cxip_cmdq *txq;
	union c_fab_addr dfa;
	uint8_t idx_ext;
	uint32_t pid_bits;
	struct c_cstate_cmd c_state = {};
	struct c_idc_msg_hdr idc_msg = {};
	uint32_t match_id;
	int ret;

	if (!req->ep_obj->ctrl.tx_credits) {
		CXIP_WARN("Control TX credits exhausted\n");
		return -FI_EAGAIN;
	}

	req->ep_obj->ctrl.tx_credits--;

	txq = req->ep_obj->ctrl.txq;
	pid_bits = req->ep_obj->domain->iface->dev->info.pid_bits;
	cxi_build_dfa(req->send.nic_addr, req->send.pid, pid_bits,
		      CXIP_PTL_IDX_CTRL, &dfa, &idx_ext);
	match_id = CXI_MATCH_ID(pid_bits, req->ep_obj->src_addr.pid,
				req->ep_obj->src_addr.nic);

	c_state.event_send_disable = 1;
	c_state.index_ext = idx_ext;
	c_state.eq = req->ep_obj->ctrl.tx_evtq->eqn;
	c_state.initiator = match_id;

	idc_msg.dfa = dfa;
	idc_msg.match_bits = req->send.mb.raw;
	idc_msg.user_ptr = (uint64_t)req;

	if (req->ep_obj->av_auth_key) {
		ret = cxip_domain_emit_idc_msg(req->ep_obj->domain,
					       req->send.vni,
					       CXI_TC_BEST_EFFORT, &c_state,
					       &idc_msg, NULL, 0, 0);
		if (ret) {
			CXIP_DBG("Failed to write domain IDC: %d\n", ret);
			goto err_return_credit;
		}
	} else {
		ret = cxip_cmdq_cp_set(txq, req->send.vni, CXI_TC_BEST_EFFORT,
				       CXI_TC_TYPE_DEFAULT);
		if (ret) {
			CXIP_DBG("Failed to set cp: %d\n", ret);
			goto err_return_credit;
		}

		ret = cxip_cmdq_emit_idc_msg(txq, &c_state, &idc_msg, NULL, 0,
					     0);
		if (ret) {
			CXIP_DBG("Failed to write IDC: %d\n", ret);
			goto err_return_credit;
		}

		cxi_cq_ring(txq->dev_cmdq);
	}

	CXIP_DBG("Queued control message: %p\n", req);

	return FI_SUCCESS;

err_return_credit:
	req->ep_obj->ctrl.tx_credits++;

	return ret;
}

/*
 * cxip_ctrl_msg_init() - Initialize control messaging resources.
 *
 * Caller must hold ep_obj->lock.
 */
int cxip_ctrl_msg_init(struct cxip_ep_obj *ep_obj)
{
	const union c_event *event;
	int ret;
	uint32_t le_flags;
	union cxip_match_bits mb = {
		.ctrl_le_type = CXIP_CTRL_LE_TYPE_CTRL_MSG,
	};
	union cxip_match_bits ib = {
		.raw = ~0,
	};

	ret = cxip_domain_ctrl_id_alloc(ep_obj->domain, &ep_obj->ctrl.msg_req);
	if (ret) {
		CXIP_WARN("Failed to allocate MR buffer ID: %d\n", ret);
		return -FI_ENOSPC;
	}
	ep_obj->ctrl.msg_req.ep_obj = ep_obj;
	ep_obj->ctrl.msg_req.cb = cxip_ctrl_msg_cb;

	le_flags = C_LE_UNRESTRICTED_BODY_RO | C_LE_UNRESTRICTED_END_RO |
		   C_LE_OP_PUT;

	ib.ctrl_le_type = 0;

	ret = cxip_pte_append(ep_obj->ctrl.pte, 0, 0, 0,
			      C_PTL_LIST_PRIORITY, ep_obj->ctrl.msg_req.req_id,
			      mb.raw, ib.raw, CXI_MATCH_ID_ANY, 0, le_flags,
			      NULL, ep_obj->ctrl.tgq, true);
	if (ret) {
		CXIP_DBG("Failed to write Append command: %d\n", ret);
		goto err_free_id;
	}

	/* Wait for link EQ event */
	while (!(event = cxi_eq_get_event(ep_obj->ctrl.tgt_evtq)))
		sched_yield();

	if (event->hdr.event_type != C_EVENT_LINK ||
	    event->tgt_long.buffer_id != ep_obj->ctrl.msg_req.req_id) {
		/* This is a device malfunction */
		CXIP_WARN("Invalid Link EQE %u %u %u %u\n",
			  event->hdr.event_type,
			  event->tgt_long.return_code,
			  event->tgt_long.buffer_id,
			  ep_obj->ctrl.msg_req.req_id);
		ret = -FI_EIO;
		goto err_free_id;
	}

	if (cxi_event_rc(event) != C_RC_OK) {
		CXIP_WARN("Append failed: %s\n",
			  cxi_rc_to_str(cxi_event_rc(event)));
		ret = -FI_ENOSPC;
		goto err_free_id;
	}

	cxi_eq_ack_events(ep_obj->ctrl.tgt_evtq);

	CXIP_DBG("Control messaging initialized: %p\n", ep_obj);

	return FI_SUCCESS;

err_free_id:
	cxip_domain_ctrl_id_free(ep_obj->domain, &ep_obj->ctrl.msg_req);

	return ret;
}

/*
 * cxip_ctrl_msg_fini() - Finalize control messaging resources.
 *
 * Caller must hold ep_obj->lock.
 */
void cxip_ctrl_msg_fini(struct cxip_ep_obj *ep_obj)
{
	cxip_domain_ctrl_id_free(ep_obj->domain, &ep_obj->ctrl.msg_req);

	CXIP_DBG("Control messaging finalized: %p\n", ep_obj);
}

/*
 * cxip_ep_ctrl_event_req() - Look up a control request using Cassini event.
 */
static struct cxip_ctrl_req *cxip_ep_ctrl_event_req(struct cxip_ep_obj *ep_obj,
						    const union c_event *event)
{
	struct cxip_ctrl_req *req;
	int event_rc;

	switch (event->hdr.event_type) {
	case C_EVENT_ACK:
		req = (struct cxip_ctrl_req *)event->init_short.user_ptr;
		break;
	case C_EVENT_LINK:
	case C_EVENT_UNLINK:
	case C_EVENT_MATCH:
		req = cxip_domain_ctrl_id_at(ep_obj->domain,
					     event->tgt_long.buffer_id);
		if (!req)
			CXIP_WARN("Invalid buffer_id: %d (%s)\n",
				  event->tgt_long.buffer_id,
				  cxi_event_to_str(event));
		break;
	case C_EVENT_PUT:
	case C_EVENT_GET:
	case C_EVENT_ATOMIC:
	case C_EVENT_FETCH_ATOMIC:
		event_rc = cxi_event_rc(event);

		if (event_rc != C_RC_ENTRY_NOT_FOUND &&
		    event_rc != C_RC_MST_CANCELLED) {
			req = cxip_domain_ctrl_id_at(ep_obj->domain,
						     event->tgt_long.buffer_id);
			if (!req)
				CXIP_WARN("Invalid buffer_id: %d (%s)\n",
					  event->tgt_long.buffer_id,
					  cxi_event_to_str(event));
			break;
		}

		req = NULL;

		/* Silently drop any invalidated LE events. Since the control
		 * PtlTE is used for non-optimized MRs, it is possible to
		 * trigger a target error event if an invalid MR key was
		 * specified. For such operations, it is safe to just log the
		 * bad access attempt and drop the EQ event, the error will be
		 * reported to the initiator.
		 */
		if (event_rc != C_RC_MST_CANCELLED)
			CXIP_WARN("Unexpected %s event rc: %s\n",
				  cxi_event_to_str(event),
				  cxi_rc_to_str(event_rc));

		break;
	case C_EVENT_STATE_CHANGE:
		cxip_pte_state_change(ep_obj->domain->iface, event);

		req = NULL;
		break;
	case C_EVENT_COMMAND_FAILURE:
		CXIP_FATAL("Command failure: cq=%u target=%u fail_loc=%u cmd_type=%u cmd_size=%u opcode=%u\n",
			   event->cmd_fail.cq_id, event->cmd_fail.is_target,
			   event->cmd_fail.fail_loc,
			   event->cmd_fail.fail_command.cmd_type,
			   event->cmd_fail.fail_command.cmd_size,
			   event->cmd_fail.fail_command.opcode);
	default:
		CXIP_FATAL("Invalid event type: %d\n", event->hdr.event_type);
	}

	CXIP_DBG("got control event: %s rc: %s (req: %p)\n",
		 cxi_event_to_str(event),
		 cxi_rc_to_str(cxi_event_rc(event)),
		 req);

	return req;
}

/* Caller must hold ep_obj->lock. */
static void cxip_ep_return_ctrl_tx_credits(struct cxip_ep_obj *ep_obj,
					   unsigned int credits)
{
	ep_obj->ctrl.tx_credits += credits;
}

void cxip_ep_ctrl_eq_progress(struct cxip_ep_obj *ep_obj,
			      struct cxi_eq *ctrl_evtq, bool tx_evtq,
			      bool ep_obj_locked)
{
	const union c_event *event;
	struct cxip_ctrl_req *req;
	int ret;

	/* The Control EQ is shared by a SEP. Avoid locking. */
	if (!cxi_eq_peek_event(ctrl_evtq))
		return;

	if (!ep_obj_locked)
		ofi_genlock_lock(&ep_obj->lock);

	while ((event = cxi_eq_peek_event(ctrl_evtq))) {
		req = cxip_ep_ctrl_event_req(ep_obj, event);
		if (req) {
			ret = req->cb(req, event);
			if (ret != FI_SUCCESS)
				break;
		}

		/* Consume and ack event. */
		cxi_eq_next_event(ctrl_evtq);

		cxi_eq_ack_events(ctrl_evtq);

		if (tx_evtq)
			cxip_ep_return_ctrl_tx_credits(ep_obj, 1);

	}

	if (cxi_eq_get_drops(ctrl_evtq))
		CXIP_FATAL("Control EQ drops detected\n");

	if (!ep_obj_locked)
		ofi_genlock_unlock(&ep_obj->lock);
}

void cxip_ep_tx_ctrl_progress(struct cxip_ep_obj *ep_obj)
{
	cxip_ep_ctrl_eq_progress(ep_obj, ep_obj->ctrl.tx_evtq, true, false);
}

void cxip_ep_tx_ctrl_progress_locked(struct cxip_ep_obj *ep_obj)
{
	cxip_ep_ctrl_eq_progress(ep_obj, ep_obj->ctrl.tx_evtq, true, true);
}

/*
 * cxip_ep_ctrl_progress() - Progress operations using the control EQ.
 */
void cxip_ep_ctrl_progress(struct cxip_ep_obj *ep_obj)
{
	cxip_ep_ctrl_eq_progress(ep_obj, ep_obj->ctrl.tgt_evtq, false, false);
	cxip_ep_tx_ctrl_progress(ep_obj);
}

/*
 * cxip_ep_ctrl_progress_locked() - Progress operations using the control EQ.
 */
void cxip_ep_ctrl_progress_locked(struct cxip_ep_obj *ep_obj)
{
	cxip_ep_ctrl_eq_progress(ep_obj, ep_obj->ctrl.tgt_evtq, false, true);
	cxip_ep_tx_ctrl_progress_locked(ep_obj);
}

/*
 * cxip_ep_tgt_ctrl_progress() - Progress TGT operations using the control EQ.
 */
void cxip_ep_tgt_ctrl_progress(struct cxip_ep_obj *ep_obj)
{
	cxip_ep_ctrl_eq_progress(ep_obj, ep_obj->ctrl.tgt_evtq, false, false);
}

/*
 * cxip_ep_tgt_ctrl_progress_locked() - Progress operations using the control
 * EQ.
 */
void cxip_ep_tgt_ctrl_progress_locked(struct cxip_ep_obj *ep_obj)
{
	cxip_ep_ctrl_eq_progress(ep_obj, ep_obj->ctrl.tgt_evtq, false, true);
}

/*
 * cxip_ep_ctrl_trywait() - Return 0 if no events need to be progressed.
 */
int cxip_ep_ctrl_trywait(void *arg)
{
	struct cxip_ep_obj *ep_obj = (struct cxip_ep_obj *)arg;

	if (!ep_obj->ctrl.wait) {
		CXIP_WARN("No CXI ep_obj wait object\n");
		return -FI_EINVAL;
	}

	if (cxi_eq_peek_event(ep_obj->ctrl.tgt_evtq) ||
	    cxi_eq_peek_event(ep_obj->ctrl.tx_evtq))
		return -FI_EAGAIN;

	ofi_genlock_lock(&ep_obj->lock);
	cxil_clear_wait_obj(ep_obj->ctrl.wait);

	if (cxi_eq_peek_event(ep_obj->ctrl.tgt_evtq) ||
	    cxi_eq_peek_event(ep_obj->ctrl.tx_evtq)) {
		ofi_genlock_unlock(&ep_obj->lock);

		return -FI_EAGAIN;
	}
	ofi_genlock_unlock(&ep_obj->lock);

	return FI_SUCCESS;
}

static void cxip_eq_ctrl_eq_free(void *eq_buf, struct cxi_md *eq_md,
				 struct cxi_eq *eq)
{
	int ret;

	ret = cxil_destroy_evtq(eq);
	if (ret)
		CXIP_WARN("Failed to free CXI EQ: ret=%d", ret);

	ret = cxil_unmap(eq_md);
	if (ret)
		CXIP_WARN("Failed to unmap EQ buffer: ret=%d", ret);

	free(eq_buf);
}

static int cxip_ep_ctrl_eq_alloc(struct cxip_ep_obj *ep_obj, size_t len,
				 void **eq_buf, struct cxi_md **eq_md,
				 struct cxi_eq **eq)
{
	struct cxi_eq_attr eq_attr = {
		.flags = CXI_EQ_TGT_LONG,
	};
	int ret;
	int unmap_ret __attribute__((unused));
	int page_size;

	page_size = ofi_get_page_size();
	if (page_size < 0)
		return -ofi_syserr();

	len = ofi_get_aligned_size(len, page_size);
	*eq_buf = aligned_alloc(page_size, len);
	if (!eq_buf) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ret = cxil_map(ep_obj->domain->lni->lni, *eq_buf, len,
		       CXIP_EQ_MAP_FLAGS, NULL, eq_md);
	if (ret)
		goto err_free_eq_buf;

	eq_attr.queue = *eq_buf;
	eq_attr.queue_len = len;

	/* ep_obj->ctrl.wait will be NULL if not required */
	ret = cxil_alloc_evtq(ep_obj->domain->lni->lni, *eq_md, &eq_attr,
			      ep_obj->ctrl.wait, NULL, eq);
	if (ret)
		goto err_free_eq_md;

	return FI_SUCCESS;

err_free_eq_md:
	unmap_ret = cxil_unmap(*eq_md);
	assert(unmap_ret == 0);

err_free_eq_buf:
	free(*eq_buf);
err:
	return ret;
}

/*
 * cxip_ep_wait_required() - return true if base EP wait object is required.
 */
static bool cxip_ctrl_wait_required(struct cxip_ep_obj *ep_obj)
{
	if (ep_obj->rxc->recv_cq && ep_obj->rxc->recv_cq->priv_wait)
		return true;

	if (ep_obj->txc->send_cq && ep_obj->txc->send_cq->priv_wait)
		return true;

	return false;
}

/*
 * cxip_ep_ctrl_del_wait() - Delete control FD object
 */
void cxip_ep_ctrl_del_wait(struct cxip_ep_obj *ep_obj)
{
	int wait_fd;

	wait_fd = cxil_get_wait_obj_fd(ep_obj->ctrl.wait);

	if (ep_obj->txc->send_cq) {
		ofi_wait_del_fd(ep_obj->txc->send_cq->util_cq.wait, wait_fd);
		CXIP_DBG("Deleted control HW EQ FD: %d from CQ: %p\n",
			 wait_fd, ep_obj->txc->send_cq);
	}

	if (ep_obj->rxc->recv_cq &&
	    ep_obj->rxc->recv_cq != ep_obj->txc->send_cq) {
		ofi_wait_del_fd(ep_obj->rxc->recv_cq->util_cq.wait, wait_fd);
		CXIP_DBG("Deleted control HW EQ FD: %d from CQ %p\n",
			 wait_fd, ep_obj->rxc->recv_cq);
	}
}

/*
 * cxip_ep_ctrl_add_wait() - Add control FD to CQ object
 */
int cxip_ep_ctrl_add_wait(struct cxip_ep_obj *ep_obj)
{
	struct cxip_cq *cq;
	int wait_fd;
	int ret;

	ret = cxil_alloc_wait_obj(ep_obj->domain->lni->lni,
				  &ep_obj->ctrl.wait);
	if (ret) {
		CXIP_WARN("Control wait object allocation failed: %d\n", ret);
		return -FI_ENOMEM;
	}

	wait_fd = cxil_get_wait_obj_fd(ep_obj->ctrl.wait);
	ret = fi_fd_nonblock(wait_fd);
	if (ret) {
		CXIP_WARN("Unable to set control wait non-blocking: %d, %s\n",
			  ret, fi_strerror(-ret));
		goto err;
	}

	cq = ep_obj->txc->send_cq;
	if (cq) {
		ret = ofi_wait_add_fd(cq->util_cq.wait, wait_fd,
				      POLLIN, cxip_ep_ctrl_trywait, ep_obj,
				      &cq->util_cq.cq_fid.fid);
		if (ret) {
			CXIP_WARN("TX CQ add FD failed: %d, %s\n",
				  ret, fi_strerror(-ret));
			goto err;
		}
	}

	if (ep_obj->rxc->recv_cq && ep_obj->rxc->recv_cq != cq) {
		cq = ep_obj->rxc->recv_cq;

		ret = ofi_wait_add_fd(cq->util_cq.wait, wait_fd,
				      POLLIN, cxip_ep_ctrl_trywait, ep_obj,
				      &cq->util_cq.cq_fid.fid);
		if (ret) {
			CXIP_WARN("RX CQ add FD failed: %d, %s\n",
				  ret, fi_strerror(-ret));
			goto err_add_fd;
		}
	}

	CXIP_DBG("Added control EQ private wait object, intr FD: %d\n",
		 wait_fd);

	return FI_SUCCESS;

err_add_fd:
	if (ep_obj->txc->send_cq)
		ofi_wait_del_fd(ep_obj->txc->send_cq->util_cq.wait, wait_fd);
err:
	cxil_destroy_wait_obj(ep_obj->ctrl.wait);
	ep_obj->ctrl.wait = NULL;

	return ret;
}

/*
 * cxip_ep_ctrl_init() - Initialize endpoint control resources.
 *
 * Caller must hold ep_obj->lock.
 */
int cxip_ep_ctrl_init(struct cxip_ep_obj *ep_obj)
{
	struct cxi_pt_alloc_opts pt_opts = {
		.use_long_event = 1,
		.is_matching = 1,
	};
	const union c_event *event;
	int ret;
	size_t rx_eq_size = MIN(cxip_env.ctrl_rx_eq_max_size,
				ofi_universe_size * 64 +
				ep_obj->domain->mr_match_events * 256 * 64);

	/* When MR event counting has been requested turn on
	 * delivery of match events.
	 */
	if (ep_obj->domain->mr_match_events)
		pt_opts.en_event_match = 1;

	/* If CQ(s) are using a wait object, then control event
	 * queues need to unblock CQ poll as well. CQ will add the
	 * associated FD to the CQ FD list.
	 */
	if (cxip_ctrl_wait_required(ep_obj)) {
		ret = cxil_alloc_wait_obj(ep_obj->domain->lni->lni,
					  &ep_obj->ctrl.wait);
		if (ret) {
			CXIP_WARN("EP ctrl wait object alloc failed: %d\n",
				  ret);
			return ret;
		}
	}

	ret = cxip_ep_ctrl_eq_alloc(ep_obj, 4 * s_page_size,
				    &ep_obj->ctrl.tx_evtq_buf,
				    &ep_obj->ctrl.tx_evtq_buf_md,
				    &ep_obj->ctrl.tx_evtq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate TX EQ resources, ret: %d\n", ret);
		goto err;
	}

	ret = cxip_ep_ctrl_eq_alloc(ep_obj, rx_eq_size,
				    &ep_obj->ctrl.tgt_evtq_buf,
				    &ep_obj->ctrl.tgt_evtq_buf_md,
				    &ep_obj->ctrl.tgt_evtq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate TGT EQ resources, ret: %d\n",
			  ret);
		goto free_tx_evtq;
	}

	ret = cxip_ep_cmdq(ep_obj, true, ep_obj->domain->tclass,
			   ep_obj->ctrl.tx_evtq, &ep_obj->ctrl.txq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate control TXQ, ret: %d\n", ret);
		ret = -FI_EDOMAIN;
		goto free_tgt_evtq;
	}

	ret = cxip_ep_cmdq(ep_obj, false, ep_obj->domain->tclass,
			   ep_obj->ctrl.tgt_evtq, &ep_obj->ctrl.tgq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate control TGQ, ret: %d\n", ret);
		ret = -FI_EDOMAIN;
		goto free_txq;
	}

	ret = cxip_pte_alloc_nomap(ep_obj->ptable, ep_obj->ctrl.tgt_evtq,
				   &pt_opts, NULL, NULL, &ep_obj->ctrl.pte);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate control PTE: %d\n", ret);
		goto free_tgq;
	}

	/* CXIP_PTL_IDX_WRITE_MR_STD is shared with CXIP_PTL_IDX_CTRL. */
	ret = cxip_pte_map(ep_obj->ctrl.pte, CXIP_PTL_IDX_WRITE_MR_STD, false);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map write PTE: %d\n", ret);
		goto free_pte;
	}

	ret = cxip_pte_map(ep_obj->ctrl.pte, CXIP_PTL_IDX_READ_MR_STD, false);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map read PTE: %d\n", ret);
		goto free_pte;
	}

	ret = cxip_pte_set_state(ep_obj->ctrl.pte, ep_obj->ctrl.tgq,
				 C_PTLTE_ENABLED, 0);
	if (ret) {
		/* This is a bug, we have exclusive access to this CMDQ. */
		CXIP_WARN("Failed to enqueue command: %d\n", ret);
		goto free_pte;
	}

	/* Wait for Enable event */
	while (!(event = cxi_eq_get_event(ep_obj->ctrl.tgt_evtq)))
		sched_yield();

	switch (event->hdr.event_type) {
	case C_EVENT_STATE_CHANGE:
		if (event->tgt_long.return_code != C_RC_OK ||
		    event->tgt_long.initiator.state_change.ptlte_state !=
		    C_PTLTE_ENABLED ||
		    event->tgt_long.ptlte_index != ep_obj->ctrl.pte->pte->ptn)
			CXIP_FATAL("Invalid PtlTE enable event\n");
		break;
	case C_EVENT_COMMAND_FAILURE:
		CXIP_FATAL("Command failure: cq=%u target=%u fail_loc=%u cmd_type=%u cmd_size=%u opcode=%u\n",
			   event->cmd_fail.cq_id, event->cmd_fail.is_target,
			   event->cmd_fail.fail_loc,
			   event->cmd_fail.fail_command.cmd_type,
			   event->cmd_fail.fail_command.cmd_size,
			   event->cmd_fail.fail_command.opcode);
	default:
		CXIP_FATAL("Invalid event type: %d\n", event->hdr.event_type);
	}

	cxi_eq_ack_events(ep_obj->ctrl.tgt_evtq);

	ret = cxip_ctrl_msg_init(ep_obj);
	if (ret != FI_SUCCESS)
		goto free_pte;

	/* Reserve 4 event queue slots to prevent EQ overrun.
	 * 1. One slot for EQ status writeback
	 * 2. One slot for default reserved_fc value
	 * 3. One slot for EQ overrun detection.
	 * 4. TODO: Determine why an additional slot needs to be reserved.
	 */
	ep_obj->ctrl.tx_credits =
		ep_obj->ctrl.tx_evtq->byte_size / C_EE_CFG_ECB_SIZE - 4;

	CXIP_DBG("EP control initialized: %p\n", ep_obj);

	return FI_SUCCESS;

free_pte:
	cxip_pte_free(ep_obj->ctrl.pte);
free_tgq:
	cxip_ep_cmdq_put(ep_obj, false);
free_txq:
	cxip_ep_cmdq_put(ep_obj, true);
free_tgt_evtq:
	cxip_eq_ctrl_eq_free(ep_obj->ctrl.tgt_evtq_buf,
			     ep_obj->ctrl.tgt_evtq_buf_md,
			     ep_obj->ctrl.tgt_evtq);
free_tx_evtq:
	cxip_eq_ctrl_eq_free(ep_obj->ctrl.tx_evtq_buf,
			     ep_obj->ctrl.tx_evtq_buf_md, ep_obj->ctrl.tx_evtq);
err:
	if (ep_obj->ctrl.wait) {
		cxil_destroy_wait_obj(ep_obj->ctrl.wait);
		ep_obj->ctrl.wait = NULL;
	}

	return ret;
}

/*
 * cxip_ep_ctrl_fini() - Finalize endpoint control resources.
 *
 * Caller must hold ep_obj->lock.
 */
void cxip_ep_ctrl_fini(struct cxip_ep_obj *ep_obj)
{
	cxip_ctrl_mr_cache_flush(ep_obj);
	cxip_ctrl_msg_fini(ep_obj);
	cxip_pte_free(ep_obj->ctrl.pte);
	cxip_ep_cmdq_put(ep_obj, false);
	cxip_ep_cmdq_put(ep_obj, true);

	cxip_eq_ctrl_eq_free(ep_obj->ctrl.tgt_evtq_buf,
			     ep_obj->ctrl.tgt_evtq_buf_md,
			     ep_obj->ctrl.tgt_evtq);
	cxip_eq_ctrl_eq_free(ep_obj->ctrl.tx_evtq_buf,
			     ep_obj->ctrl.tx_evtq_buf_md, ep_obj->ctrl.tx_evtq);

	if (ep_obj->ctrl.wait) {
		cxil_destroy_wait_obj(ep_obj->ctrl.wait);
		ep_obj->ctrl.wait = NULL;

		CXIP_DBG("Deleted control EQ wait object\n");
	}

	CXIP_DBG("EP control finalized: %p\n", ep_obj);
}
