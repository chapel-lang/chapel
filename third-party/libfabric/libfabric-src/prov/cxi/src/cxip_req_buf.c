/*
 * (C) Copyright 2021-2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */
#include "config.h"
#include "cxip.h"

static bool cxip_req_buf_is_head(struct cxip_ptelist_buf *buf)
{
	struct cxip_ptelist_buf *head_buf =
			container_of(buf->pool->active_bufs.next,
				     struct cxip_ptelist_buf, buf_entry);

	return head_buf == buf;
}

static bool cxip_req_buf_is_consumed(struct cxip_ptelist_buf *buf)
{
	return buf->unlink_length && buf->unlink_length == buf->cur_offset &&
		dlist_empty(&buf->request.pending_ux_list);
}

static bool cxip_req_buf_is_next_put(struct cxip_ptelist_buf *buf,
				     const union c_event *event)
{
	return (CXI_VA_TO_IOVA(buf->md->md, buf->data) + buf->cur_offset) ==
		event->tgt_long.start;
}

static void cxip_req_buf_get_header_info(struct cxip_ptelist_buf *buf,
					 struct cxip_ux_send *ux,
					 size_t *header_length,
					 uint64_t *remote_offset)
{
	struct c_port_fab_hdr *fab_hdr =
		(void *)CXI_IOVA_TO_VA(buf->md->md, ux->put_ev.tgt_long.start);
	struct c_port_unrestricted_hdr *unres_hdr =
		(void *)((char *)fab_hdr + sizeof(*fab_hdr));

	if (fab_hdr->ver != 4)
		RXC_FATAL(buf->rxc, "Unsupported fabric header version: %u\n",
			  fab_hdr->ver);

	switch (unres_hdr->ver_pkt_type) {
	case C_V4_PKT_UNRESTRICTED:
		*header_length = sizeof(*fab_hdr) +
			sizeof(struct c_port_unrestricted_hdr);
		*remote_offset =
			c_port_unrestricted_hdr_get_remote_offset(unres_hdr);
		break;
	case C_V4_PKT_SMALLMSG:
		*header_length = sizeof(*fab_hdr) +
			sizeof(struct c_port_small_msg_hdr);
		*remote_offset = 0;
		break;
	default:
		RXC_FATAL(buf->rxc, "Unsupported packet type: %u\n",
			  unres_hdr->ver_pkt_type);
	}
}

void cxip_req_buf_ux_free(struct cxip_ux_send *ux)
{
	_cxip_req_buf_ux_free(ux, true);
}

static struct cxip_ux_send *cxip_req_buf_ux_alloc(struct cxip_ptelist_buf *buf,
						  const union c_event *event)
{
	struct cxip_ux_send *ux;

	ux = calloc(1, sizeof(*ux));
	if (!ux)
		return NULL;

	ux->put_ev = *event;
	ux->req = buf->req;
	dlist_init(&ux->rxc_entry);
	cxip_ptelist_buf_get(buf);

	RXC_DBG(buf->rxc, "Referenced REQ buf=%p ux=%p\n", buf, ux);

	return ux;
}

/* Caller must hold ep_obj->lock */
static int cxip_req_buf_process_ux(struct cxip_ptelist_buf *buf,
				   struct cxip_ux_send *ux)
{
	struct cxip_rxc_hpc *rxc = buf->rxc;
	size_t header_length;
	uint64_t remote_offset;
	int ret;
	size_t unlink_length;
	bool unlinked = ux->put_ev.tgt_long.auto_unlinked;

	/* Pre-processing of unlink events. */
	if (unlinked)
		unlink_length = ux->put_ev.tgt_long.start -
			CXI_VA_TO_IOVA(buf->md->md, buf->data) +
			ux->put_ev.tgt_long.mlength;

	buf->cur_offset += ux->put_ev.tgt_long.mlength;

	/* Fixed the put event to point to where the payload resides in the
	 * request buffer. In addition, extract the remote offset needed for
	 * rendezvous.
	 */
	cxip_req_buf_get_header_info(buf, ux, &header_length, &remote_offset);
	assert((ssize_t)ux->put_ev.tgt_long.mlength -
	       (ssize_t)header_length >= 0);

	ux->put_ev.tgt_long.start += header_length;
	ux->put_ev.tgt_long.mlength -= header_length;
	ux->put_ev.tgt_long.remote_offset = remote_offset +
		ux->put_ev.tgt_long.mlength;

	rxc->sw_ux_list_len++;

	ret = cxip_recv_ux_sw_matcher(ux);
	switch (ret) {
	/* Unexpected message needs to be processed again. Put event fields
	 * need to be reset.
	 */
	case -FI_EAGAIN:
		ux->put_ev.tgt_long.mlength += header_length;
		ux->put_ev.tgt_long.start -= header_length;
		buf->cur_offset -= ux->put_ev.tgt_long.mlength;

		rxc->sw_ux_list_len--;
		return -FI_EAGAIN;

	/* Unexpected message failed to match a user posted request. Need to
	 * queue the unexpected message for future processing.
	 */
	case -FI_ENOMSG:
		/* Check to see if a PtlTE transition to software managed EP
		 * is in progress, and if so add to the pending UX list which
		 * will be appended to software UX message list following
		 * completion of the on-loading.
		 */
		if (rxc->base.state != RXC_ENABLED_SOFTWARE &&
		    rxc->base.state != RXC_FLOW_CONTROL) {
			rxc->sw_ux_list_len--;
			dlist_insert_tail(&ux->rxc_entry,
					  &rxc->sw_pending_ux_list);
			rxc->sw_pending_ux_list_len++;

			RXC_DBG(buf->rxc,
				"rbuf=%p ux=%p sw_pending_ux_list_len=%u\n",
				buf, ux, buf->rxc->sw_pending_ux_list_len);
		} else {
			dlist_insert_tail(&ux->rxc_entry, &rxc->sw_ux_list);

			RXC_DBG(buf->rxc, "rbuf=%p ux=%p sw_ux_list_len=%u\n",
				buf, ux, buf->rxc->sw_ux_list_len);
		}
		break;

	/* Unexpected message successfully matched a user posted request. */
	case FI_SUCCESS:
		break;

	default:
		RXC_FATAL(rxc, "Unexpected cxip_recv_ux_sw_matcher() rc: %d\n",
			  ret);
	}

	/* Once unexpected send has been accepted, complete processing of the
	 * unlink.
	 */
	if (unlinked) {
		buf->unlink_length = unlink_length;
		ofi_atomic_dec32(&buf->pool->bufs_linked);

		RXC_DBG(rxc, "rbuf=%p rxc_rbuf_linked=%u\n", buf,
			ofi_atomic_get32(&buf->pool->bufs_linked));

		/* Replenish to keep minimum linked */
		ret = cxip_ptelist_buf_replenish(rxc->req_list_bufpool, false);
		if (ret)
			RXC_WARN(rxc, "Request replenish failed: %d\n", ret);
	}

	RXC_DBG(rxc, "rbuf=%p processed ux_send=%p\n", buf, ux);

	return FI_SUCCESS;
}

static void cxip_req_buf_progress_pending_ux(struct cxip_ptelist_buf *buf)
{
	struct cxip_ux_send *ux;
	struct dlist_entry *tmp;
	int ret;

again:
	dlist_foreach_container_safe(&buf->request.pending_ux_list,
				     struct cxip_ux_send, ux, rxc_entry, tmp) {
		if (cxip_req_buf_is_next_put(buf, &ux->put_ev)) {
			dlist_remove(&ux->rxc_entry);

			/* The corresponding event from the completion queue has
			 * already been consumed. Thus, -FI_EAGAIN cannot be
			 * returned.
			 */
			do {
				ret = cxip_req_buf_process_ux(buf, ux);
			} while (ret == -FI_EAGAIN);

			/* Previously processed unexpected messages may now be
			 * valid. Need to reprocess the entire list.
			 */
			goto again;
		}
	}
}

static int cxip_req_buf_process_put_event(struct cxip_ptelist_buf *buf,
					  const union c_event *event)
{
	struct cxip_ux_send *ux;
	int ret = FI_SUCCESS;
	struct cxip_rxc_hpc *rxc = buf->rxc;
	struct cxip_ptelist_bufpool *pool = buf->pool;

	assert(event->tgt_long.mlength >= CXIP_REQ_BUF_HEADER_MIN_SIZE);

	ux = cxip_req_buf_ux_alloc(buf, event);
	if (!ux) {
		RXC_WARN(rxc, "Memory allocation error\n");
		return -FI_EAGAIN;
	}

	/* Target events can be out-of-order with respect to how they were
	 * matched on the PtlTE request list. To maintain the hardware matched
	 * order, software unexpected entries are only processed in the order in
	 * which they land in the request buffer.
	 */
	if (cxip_req_buf_is_head(buf) && cxip_req_buf_is_next_put(buf, event)) {
		ret = cxip_req_buf_process_ux(buf, ux);
		if (ret == -FI_EAGAIN) {
			_cxip_req_buf_ux_free(ux, false);
			return ret;
		}

		/* Since events arrive out-of-order, it is possible that a
		 * non-head request buffer receive an event. Scrub all request
		 * buffers processing their pending unexpected lists until a
		 * request buffer is not consumed.
		 */
		while ((buf = dlist_first_entry_or_null(&pool->active_bufs,
							struct cxip_ptelist_buf,
							buf_entry))) {
			cxip_req_buf_progress_pending_ux(buf);

			if (cxip_req_buf_is_consumed(buf)) {
				RXC_DBG(rxc, "buf=%p consumed\n", buf);
				cxip_ptelist_buf_consumed(buf);
			} else {
				break;
			}
		}
	} else {
		/* Out-of-order target event. Queue unexpected message on
		 * pending list until these addition events occur.
		 */
		dlist_insert_tail(&ux->rxc_entry,
				  &buf->request.pending_ux_list);

		RXC_DBG(rxc, "rbuf=%p pend ux_send=%p\n", buf, ux);
	}

	return ret;
}

static int cxip_req_buf_cb(struct cxip_req *req, const union c_event *event)
{
	struct cxip_ptelist_buf *buf = req->req_ctx;

	switch (event->hdr.event_type) {
	case C_EVENT_LINK:
		/* Success events not requested */
		cxip_ptelist_buf_link_err(buf, cxi_event_rc(event));
		return FI_SUCCESS;

	case C_EVENT_UNLINK:
		assert(!event->tgt_long.auto_unlinked);
		cxip_ptelist_buf_unlink(buf);
		return FI_SUCCESS;

	case C_EVENT_PUT:
		return cxip_req_buf_process_put_event(buf, event);

	default:
		RXC_FATAL(buf->rxc, CXIP_UNEXPECTED_EVENT,
			  cxi_event_to_str(event),
			  cxi_rc_to_str(cxi_event_rc(event)));
	}
}

int cxip_req_bufpool_init(struct cxip_rxc_hpc *rxc)
{
	struct cxip_ptelist_bufpool_attr attr = {
		.list_type = C_PTL_LIST_REQUEST,
		.ptelist_cb = cxip_req_buf_cb,
		.buf_size = cxip_env.req_buf_size,
		.min_space_avail = CXIP_REQ_BUF_HEADER_MAX_SIZE +
				   rxc->max_eager_size,
		.min_posted = cxip_env.req_buf_min_posted,
		/* Allow growing the number request bufs posted */
		.max_posted = cxip_env.req_buf_min_posted << 3,
		.max_cached = cxip_env.req_buf_max_cached,
	};

	return cxip_ptelist_bufpool_init(rxc, &rxc->req_list_bufpool, &attr);
}

void cxip_req_bufpool_fini(struct cxip_rxc_hpc *rxc)
{
	return cxip_ptelist_bufpool_fini(rxc->req_list_bufpool);
}
