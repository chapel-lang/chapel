/*
 * Copyright (c) 2018-2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ofi_list.h>
#include <ofi.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_CQ, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_CQ, __VA_ARGS__)

bool cxip_evtq_saturated(struct cxip_evtq *evtq)
{
	if (evtq->eq_saturated)
		return true;

	/* Hardware will automatically update the EQ status writeback area,
	 * which includes a timestamp, once the EQ reaches a certain fill
	 * percentage. The EQ status timestamp is compared against cached
	 * versions of the previous EQ status timestamp to determine if new
	 * writebacks have occurred. Each time a new writeback occurs, the EQ
	 * is treated as saturated.
	 *
	 * Note that the previous EQ status is always updated when the
	 * corresponding OFI completion queue is progressed.
	 */
	if (evtq->eq->status->timestamp_sec >
	    evtq->prev_eq_status.timestamp_sec ||
	    evtq->eq->status->timestamp_ns >
	    evtq->prev_eq_status.timestamp_ns) {
		evtq->eq_saturated = true;
		return true;
	}

	return false;
}

int cxip_evtq_adjust_reserved_fc_event_slots(struct cxip_evtq *evtq, int value)
{
	int ret;

	ret = cxil_evtq_adjust_reserved_fc(evtq->eq, value);
	if (ret >= 0)
		ret = 0;

	return ret;
}

/*
 * cxip_evtq_req_cancel() - Cancel one request.
 *
 * Cancel one Receive request. If match is true, cancel the request with
 * matching op_ctx. Only Receive requests should be in the request list.
 *
 * Caller must hold ep_obj->lock.
 */
int cxip_evtq_req_cancel(struct cxip_evtq *evtq, void *req_ctx,
			 void *op_ctx, bool match)
{
	int ret = -FI_ENOENT;
	struct cxip_req *req;
	struct dlist_entry *tmp;

	dlist_foreach_container_safe(&evtq->req_list, struct cxip_req, req,
				     evtq_entry, tmp) {
		if (req->req_ctx == req_ctx &&
		    req->type == CXIP_REQ_RECV &&
		    !req->recv.canceled &&
		    !req->recv.parent &&
		    (!match || (void *)req->context == op_ctx)) {
			ret = req->recv.rxc->ops.cancel_msg_recv(req);
			break;
		}
	}

	return ret;
}

static void cxip_evtq_req_free_no_lock(struct cxip_req *req)
{
	struct cxip_req *table_req;

	CXIP_DBG("Freeing req: %p (ID: %d)\n", req, req->req_id);

	dlist_remove(&req->evtq_entry);

	if (req->req_id >= 0) {
		table_req = (struct cxip_req *)ofi_idx_remove(
			&req->evtq->req_table, req->req_id);
		if (table_req != req)
			CXIP_WARN("Failed to unmap request: %p\n", req);
	}

	ofi_buf_free(req);
}

/*
 * cxip_evtq_flush_trig_reqs() - Flush triggered TX requests
 */
void cxip_evtq_flush_trig_reqs(struct cxip_evtq *evtq)
{
	struct cxip_req *req;
	struct dlist_entry *tmp;
	struct cxip_txc *txc;

	dlist_foreach_container_safe(&evtq->req_list, struct cxip_req, req,
				     evtq_entry, tmp) {

		if (cxip_is_trig_req(req)) {
			/* If a request is triggered, the context will only be
			 * a TX context (never a RX context).
			 */
			txc = req->req_ctx;

			/* Since an event will not arrive to progress the
			 * request, MDs must be cleaned up now.
			 */
			switch (req->type) {
			case CXIP_REQ_RMA:
				if (req->rma.local_md)
					cxip_unmap(req->rma.local_md);
				if (req->rma.ibuf)
					cxip_txc_ibuf_free(txc,
							   req->rma.ibuf);
				break;

			case CXIP_REQ_AMO:
				if (req->amo.oper1_md)
					cxip_unmap(req->amo.oper1_md);
				if (req->amo.result_md)
					cxip_unmap(req->amo.result_md);
				if (req->amo.ibuf)
					cxip_txc_ibuf_free(txc,
							   req->amo.ibuf);
				break;

			case CXIP_REQ_SEND:
				if (req->send.send_md)
					cxip_unmap(req->send.send_md);
				if (req->send.ibuf)
					cxip_txc_ibuf_free(txc,
							   req->send.ibuf);
				break;

			default:
				CXIP_WARN("Invalid trig req type: %d\n",
					  req->type);
			}

			ofi_atomic_dec32(&txc->otx_reqs);
			cxip_evtq_req_free_no_lock(req);
		}

	}
}

/*
 * cxip_evtq_req_discard() - Discard all matching requests.
 *
 * Mark all requests on the Completion Queue to be discarded. When a marked
 * request completes, it's completion event will be dropped. This is the
 * behavior defined for requests belonging to a closed Endpoint.
 *
 * Caller must hold ep_obj->lock.
 */
void cxip_evtq_req_discard(struct cxip_evtq *evtq, void *req_ctx)
{
	struct cxip_req *req;
	int discards = 0;

	dlist_foreach_container(&evtq->req_list, struct cxip_req, req,
				evtq_entry) {
		if (req->req_ctx == req_ctx) {
			req->discard = true;
			discards++;
		}
	}

	if (discards)
		CXIP_DBG("Marked %d requests\n", discards);
}

/*
 * cxip_evtq_req_find() - Look up a request by ID (from an event).
 */
static struct cxip_req *cxip_evtq_req_find(struct cxip_evtq *evtq, int id)
{
	return ofi_idx_at(&evtq->req_table, id);
}

/*
 * cxip_evtq_req_alloc() - Allocate a request.
 *
 * If remap is set, allocate a 16-bit request ID and map it to the new
 * request.
 *
 * Caller must hold ep_obj->lock of associated EP.
 */
struct cxip_req *cxip_evtq_req_alloc(struct cxip_evtq *evtq, int remap,
				     void *req_ctx)
{
	struct cxip_req *req;

	req = (struct cxip_req *)ofi_buf_alloc(evtq->req_pool);
	if (!req) {
		CXIP_DBG("Failed to allocate request\n");
		goto out;
	}
	memset(req, 0, sizeof(*req));

	if (remap) {
		req->req_id = ofi_idx_insert(&evtq->req_table, req);

		/* Target command buffer IDs are 16 bits wide. */
		if (req->req_id < 0 || req->req_id >= CXIP_BUFFER_ID_MAX) {
			CXIP_WARN("Failed to map request: %d\n",
				  req->req_id);
			if (req->req_id > 0)
				ofi_idx_remove(&evtq->req_table, req->req_id);
			ofi_buf_free(req);
			req = NULL;
			goto out;
		}
	} else {
		req->req_id = -1;
	}

	CXIP_DBG("Allocated req: %p (ID: %d)\n", req, req->req_id);
	req->cq = evtq->cq;
	req->evtq = evtq;
	req->req_ctx = req_ctx;
	req->discard = false;
	dlist_init(&req->evtq_entry);
	dlist_insert_tail(&req->evtq_entry, &evtq->req_list);

out:
	return req;
}

/*
 * cxip_evtq_req_free() - Free a request.
 *
 * Caller must hold ep_obj->lock.
 */
void cxip_evtq_req_free(struct cxip_req *req)
{
	cxip_evtq_req_free_no_lock(req);
}

/*
 * cxip_evtq_event_req() - Locate a request corresponding to the Cassini event.
 */
static struct cxip_req *cxip_evtq_event_req(struct cxip_evtq *evtq,
					    const union c_event *event)
{
	struct cxip_req *req;
	int return_code;

	switch (event->hdr.event_type) {
	case C_EVENT_ACK:
		req = (struct cxip_req *)event->init_short.user_ptr;
		break;
	case C_EVENT_UNLINK:
		switch (cxi_tgt_event_rc(event)) {
		/* User issued unlink events can race with put events. Assume
		 * C_RC_ENTRY_NOT_FOUND is this case.
		 */
		case C_RC_ENTRY_NOT_FOUND:
			return NULL;
		case C_RC_OK:
			break;
		default:
			CXIP_FATAL("Unhandled unlink return code: %d\n",
				   cxi_tgt_event_rc(event));
		}

		/* Fall through. */
	case C_EVENT_LINK:
	case C_EVENT_GET:
	case C_EVENT_PUT:
	case C_EVENT_PUT_OVERFLOW:
	case C_EVENT_RENDEZVOUS:
	case C_EVENT_SEARCH:
		/* RNR C_RC_ENTRY_NOT_FOUND generates an event on the
		 * target, this can be safely ignored as the source
		 * handles the retry logic.
		 */
		if (event->tgt_long.buffer_id == 0) {
			req = NULL;
			break;
		}

		req = cxip_evtq_req_find(evtq, event->tgt_long.buffer_id);
		if (req)
			break;
		/* HW error can return zero buffer_id */
		CXIP_WARN("Invalid buffer_id: %d (%s)\n",
			  event->tgt_long.buffer_id, cxi_event_to_str(event));
		return_code = cxi_tgt_event_rc(event);
		if (return_code != C_RC_OK)
			CXIP_WARN("Hardware return code: %s (%s)\n",
				  cxi_rc_to_str(return_code),
				  cxi_event_to_str(event));
		break;
	case C_EVENT_REPLY:
	case C_EVENT_SEND:
		if (!event->init_short.rendezvous) {
			req = (struct cxip_req *)event->init_short.user_ptr;
		} else {
			struct cxi_rdzv_user_ptr *up =
					(struct cxi_rdzv_user_ptr *)
					 &event->init_short.user_ptr;
			req = cxip_evtq_req_find(evtq, up->buffer_id);
			if (req)
				break;
			/* HW error can return zero buffer_id */
			CXIP_WARN("Invalid buffer_id: %d (%s)\n",
				  event->tgt_long.buffer_id,
				  cxi_event_to_str(event));
			return_code = cxi_tgt_event_rc(event);
			if (return_code != C_RC_OK)
				CXIP_WARN("Hardware return code: %s (%s)\n",
					  cxi_rc_to_str(return_code),
					  cxi_event_to_str(event));
		}
		break;


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

	CXIP_DBG("got event: %s rc: %s (req: %p)\n",
		 cxi_event_to_str(event),
		 cxi_rc_to_str(cxi_event_rc(event)),
		 req);

	return req;
}

/*
 * cxip_evtq_progress() - Progress the CXI hardware EQ specified
 *
 * Caller must hold ep_obj->lock.
 */
void cxip_evtq_progress(struct cxip_evtq *evtq)
{
	const union c_event *event;
	struct cxip_req *req;
	int ret = FI_SUCCESS;

	if (!evtq->eq || !evtq->cq)
		return;

	/* The EQ status needs to be cached on each poll to be able to properly
	 * determine if the OFI completion queue is saturated.
	 */
	evtq->prev_eq_status = *evtq->eq->status;

	while ((event = cxi_eq_peek_event(evtq->eq))) {

		/* State change events can be caused due to unacked events. When
		 * a state change event occurs, always ack EQ.
		 */
		if (event->hdr.event_type == C_EVENT_STATE_CHANGE) {
			cxi_eq_ack_events(evtq->eq);
			evtq->unacked_events = 0;
			cxip_pte_state_change(evtq->cq->domain->iface, event);
		} else {

			req = cxip_evtq_event_req(evtq, event);
			if (req) {
				ret = req->cb(req, event);
				if (ret != FI_SUCCESS)
					break;
			}
		}

		cxi_eq_next_event(evtq->eq);

		evtq->unacked_events++;
		if (evtq->unacked_events >= evtq->ack_batch_size) {
			cxi_eq_ack_events(evtq->eq);
			evtq->unacked_events = 0;
		}
	}

	if (cxi_eq_get_drops(evtq->eq)) {
		CXIP_WARN("EQ %d dropped event, rsvd slots %u, free slots %u\n",
			  evtq->eq->eqn,
			  evtq->eq->status->event_slots_rsrvd,
			  evtq->eq->status->event_slots_free);
		CXIP_FATAL("H/W Event Queue overflow detected.\n");
	}

	if (ret == FI_SUCCESS)
		evtq->eq_saturated = false;
}

void cxip_evtq_fini(struct cxip_evtq *evtq)
{
	if (!evtq->eq)
		return;

	cxil_destroy_evtq(evtq->eq);

	if (evtq->md)
		cxil_unmap(evtq->md);
	else
		madvise(evtq->buf, evtq->len, MADV_DOFORK);

	if (evtq->mmap)
		munmap(evtq->buf, evtq->len);
	else
		free(evtq->buf);

	ofi_idx_reset(&evtq->req_table);
	ofi_bufpool_destroy(evtq->req_pool);
	evtq->eq = NULL;
}

static size_t cxip_evtq_get_queue_size(struct cxip_cq *cq, size_t num_events)
{
	size_t num_slots = num_events + cq->ack_batch_size;

	/* One additional event slot is needed for full queue. */
	num_slots += 1;

	/* One additional event slot is needed for EQ status. */
	num_slots += 1;

	/* Users current expect libfabric CQ size to control sizing of HW EQs.
	 * Honor this by using CQ size to override CXI provider requested EQ
	 * slots.
	 */
	num_slots = MAX(num_slots, cq->attr.size);

	return num_slots * C_EE_CFG_ECB_SIZE;
}

#define MAP_HUGE_2MB    (21 << MAP_HUGE_SHIFT)
int cxip_evtq_init(struct cxip_evtq *evtq, struct cxip_cq *cq,
		   size_t num_events, size_t num_fc_events)
{
	struct cxi_eq_attr eq_attr = {
		.reserved_slots = num_fc_events,
	};
	struct ofi_bufpool_attr bp_attr = {
		.size = sizeof(struct cxip_req),
		.alignment = 8,
		.chunk_cnt = 64,
		.flags = OFI_BUFPOOL_NO_TRACK,
	};
	size_t len;
	size_t eq_len;
	bool eq_passthrough = false;
	int ret;
	int page_size;

	assert(cq->domain->enabled);

	len = cxip_evtq_get_queue_size(cq, num_events + num_fc_events);

	/* Note max_cnt == 0 is unlimited */
	ret = ofi_bufpool_create_attr(&bp_attr, &evtq->req_pool);
	if (ret) {
		CXIP_WARN("Failed to create req pool: %d, %s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}
	memset(&evtq->req_table, 0, sizeof(evtq->req_table));
	dlist_init(&evtq->req_list);

	/* Attempt to use 2 MiB hugepages. */
	if (!cxip_env.disable_eq_hugetlb) {
		eq_len = ofi_get_aligned_size(len, 1U << 21);
		evtq->buf = mmap(NULL, eq_len, PROT_READ | PROT_WRITE,
				 MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB |
				 MAP_HUGE_2MB, -1, 0);
		if (evtq->buf != MAP_FAILED) {
			evtq->mmap = true;

			/* If a single hugepage is used, CXI_EQ_PASSTHROUGH can
			 * be used.
			 */
			if (eq_len <= (1U << 21))
				eq_passthrough = true;
			goto mmap_success;
		}

		CXIP_DBG("Unable to map hugepage for EQ\n");
	}

	page_size = ofi_get_page_size();
	if (page_size < 0)
		return -ofi_syserr();

	evtq->mmap = false;
	eq_len = ofi_get_aligned_size(len, page_size);
	evtq->buf = aligned_alloc(page_size, eq_len);
	if (!evtq->buf) {
		CXIP_WARN("Unable to allocate EQ buffer\n");
		ret = -FI_ENOMEM;
		goto err_free_bp;
	}

mmap_success:
	/* Buffer has been allocated. Only map if needed. */
	evtq->len = eq_len;
	if (eq_passthrough) {
		evtq->md = NULL;
		eq_attr.flags |= CXI_EQ_PASSTHROUGH;

		ret = madvise(evtq->buf, evtq->len, MADV_DONTFORK);
		if (ret) {
			ret = -errno;
			CXIP_WARN("madvise failed: %d\n", ret);
			goto err_free_eq_buf;
		}
	} else {
		ret = cxil_map(cq->domain->lni->lni, evtq->buf, evtq->len,
			       CXIP_EQ_MAP_FLAGS, NULL, &evtq->md);
		if (ret) {
			CXIP_WARN("Unable to map EQ buffer: %d\n", ret);
			goto err_free_eq_buf;
		}
	}

	/* Once the EQ is at CQ fill percentage full, a status event is
	 * generated. When a status event occurs, the CXIP CQ is considered
	 * saturated until the CXI EQ is drained.
	 */
	eq_attr.status_thresh_base = cxip_env.cq_fill_percent;
	eq_attr.status_thresh_delta = 0;
	eq_attr.status_thresh_count = 1;

	eq_attr.queue = evtq->buf;
	eq_attr.queue_len = evtq->len;
	eq_attr.flags |= CXI_EQ_TGT_LONG | CXI_EQ_EC_DISABLE;

	/* CPU number will be ignored if invalid */
	if (cq->attr.flags & FI_AFFINITY && cq->attr.signaling_vector > 0)
		eq_attr.cpu_affinity = cq->attr.signaling_vector;

	/* cq->priv_wait is NULL if not backed by wait object */
	ret = cxil_alloc_evtq(cq->domain->lni->lni, evtq->md, &eq_attr,
			      cq->priv_wait, NULL, &evtq->eq);
	if (ret) {
		CXIP_WARN("Failed to allocated EQ: %d\n", ret);
		goto err_unmap_eq_buf;
	}

	/* Point back to the CQ bound to the TX or RX context */
	evtq->cq = cq;
	evtq->ack_batch_size = cq->ack_batch_size;

	return FI_SUCCESS;

err_unmap_eq_buf:
	if (evtq->md)
		cxil_unmap(evtq->md);
	else
		madvise(evtq->buf, evtq->len, MADV_DOFORK);
err_free_eq_buf:
	if (evtq->mmap)
		munmap(evtq->buf, evtq->len);
	else
		free(evtq->buf);

err_free_bp:
	ofi_idx_reset(&evtq->req_table);
	ofi_bufpool_destroy(evtq->req_pool);

	return ret;
}
