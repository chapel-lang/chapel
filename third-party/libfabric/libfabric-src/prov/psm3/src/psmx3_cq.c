/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "psmx3.h"

void psmx3_cq_enqueue_event(struct psmx3_fid_cq *cq,
			    struct psmx3_cq_event *event)
{
	cq->domain->cq_lock_fn(&cq->lock, 2);
	slist_insert_tail(&event->list_entry, &cq->event_queue);
	cq->event_count++;
	cq->domain->cq_unlock_fn(&cq->lock, 2);

	if (cq->wait)
		cq->wait->signal(cq->wait);
}

static struct psmx3_cq_event *psmx3_cq_dequeue_event(struct psmx3_fid_cq *cq)
{
	struct slist_entry *entry;

	cq->domain->cq_lock_fn(&cq->lock, 2);
	if (slist_empty(&cq->event_queue)) {
		cq->domain->cq_unlock_fn(&cq->lock, 2);
		return NULL;
	}
	entry = slist_remove_head(&cq->event_queue);
	cq->event_count--;
	cq->domain->cq_unlock_fn(&cq->lock, 2);

	return container_of(entry, struct psmx3_cq_event, list_entry);
}

static struct psmx3_cq_event *psmx3_cq_alloc_event(struct psmx3_fid_cq *cq)
{
	struct psmx3_cq_event *event;

	cq->domain->cq_lock_fn(&cq->lock, 2);
	if (!slist_empty(&cq->free_list)) {
		event = container_of(slist_remove_head(&cq->free_list),
				     struct psmx3_cq_event, list_entry);
		cq->domain->cq_unlock_fn(&cq->lock, 2);
		return event;
	}

	cq->domain->cq_unlock_fn(&cq->lock, 2);
	event = calloc(1, sizeof(*event));
	if (!event)
		PSMX3_WARN(&psmx3_prov, FI_LOG_CQ, "out of memory.\n");

	return event;
}

static void psmx3_cq_free_event(struct psmx3_fid_cq *cq,
				struct psmx3_cq_event *event)
{
	memset(event, 0, sizeof(*event));

	cq->domain->cq_lock_fn(&cq->lock, 2);
	slist_insert_tail(&event->list_entry, &cq->free_list);
	cq->domain->cq_unlock_fn(&cq->lock, 2);
}

struct psmx3_cq_event *psmx3_cq_create_event(struct psmx3_fid_cq *cq,
					     void *op_context, void *buf,
					     uint64_t flags, size_t len,
					     uint64_t data, uint64_t tag,
					     size_t olen, int err)
{
	struct psmx3_cq_event *event;

	event = psmx3_cq_alloc_event(cq);
	if (!event)
		return NULL;

	if ((event->error = !!err)) {
		event->cqe.err.op_context = op_context;
		event->cqe.err.err = -err;
		event->cqe.err.data = data;
		event->cqe.err.tag = tag;
		event->cqe.err.olen = olen;
		event->cqe.err.flags = flags;
		event->cqe.err.prov_errno = PSM2_INTERNAL_ERR;
		goto out;
	}

	switch (cq->format) {
	case FI_CQ_FORMAT_CONTEXT:
		event->cqe.context.op_context = op_context;
		break;

	case FI_CQ_FORMAT_MSG:
		event->cqe.msg.op_context = op_context;
		event->cqe.msg.flags = flags;
		event->cqe.msg.len = len;
		break;

	case FI_CQ_FORMAT_DATA:
		event->cqe.data.op_context = op_context;
		event->cqe.data.buf = buf;
		event->cqe.data.flags = flags;
		event->cqe.data.len = len;
		event->cqe.data.data = data;
		break;

	case FI_CQ_FORMAT_TAGGED:
		event->cqe.tagged.op_context = op_context;
		event->cqe.tagged.buf = buf;
		event->cqe.tagged.flags = flags;
		event->cqe.tagged.len = len;
		event->cqe.tagged.data = data;
		event->cqe.tagged.tag = tag;
		break;

	default:
		PSMX3_WARN(&psmx3_prov, FI_LOG_CQ,
			"unsupported CQ format %d\n", cq->format);
		psmx3_cq_free_event(cq, event);
		return NULL;
	}

out:
	return event;
}

static uint64_t psmx3_comp_flags[PSMX3_MAX_CONTEXT_TYPE] = {
	[PSMX3_NOCOMP_SEND_CONTEXT]	= FI_SEND | FI_MSG,
	[PSMX3_NOCOMP_RECV_CONTEXT]	= FI_RECV | FI_MSG,
	[PSMX3_NOCOMP_TSEND_CONTEXT]	= FI_SEND | FI_TAGGED,
	[PSMX3_NOCOMP_TRECV_CONTEXT]	= FI_RECV | FI_TAGGED,
	[PSMX3_NOCOMP_WRITE_CONTEXT]	= FI_WRITE | FI_RMA,
	[PSMX3_NOCOMP_READ_CONTEXT]	= FI_READ | FI_RMA,
	[PSMX3_SEND_CONTEXT]		= FI_SEND | FI_MSG,
	[PSMX3_RECV_CONTEXT]		= FI_RECV | FI_MSG,
	[PSMX3_MULTI_RECV_CONTEXT]	= FI_RECV | FI_MSG,
	[PSMX3_TSEND_CONTEXT]		= FI_SEND | FI_TAGGED,
	[PSMX3_TRECV_CONTEXT]		= FI_RECV | FI_TAGGED,
	[PSMX3_WRITE_CONTEXT]		= FI_WRITE | FI_RMA,
	[PSMX3_READ_CONTEXT]		= FI_READ | FI_RMA,
	[PSMX3_REMOTE_WRITE_CONTEXT]	= FI_REMOTE_WRITE | FI_RMA,
	[PSMX3_REMOTE_READ_CONTEXT]	= FI_REMOTE_READ | FI_RMA,
	[PSMX3_SENDV_CONTEXT]		= FI_SEND,
	[PSMX3_IOV_SEND_CONTEXT]	= FI_SEND,
	[PSMX3_IOV_RECV_CONTEXT]	= FI_RECV,
};

/*
 * Translate "status" into completion event. A few factors determine where to
 * save the event.
 *
 * If:
 *
 * (1) the CQE is for the CQ being polled; and
 * (2) event buffer is supplied (event_in != NULL); and
 * (3) the CQE is not an error entry,
 *
 * then the event is written to the event buffer directly. Otherwise a CQE is
 * allocated on the corresponding CQ.
 *
 * The function doesn't use PSMX3_STATUS_CONTEXT(status) because the context
 * field could refer to an allocated descriptor that may have already been
 * freed. All the information that are dependent on the field are obtained
 * in advance and passed in as separate parameters ("op_context", "buf",
 * "flags", "data", and "is_recv").
 *
 * The flag "event_saved" is set to indicate to the caller that the event
 * was saved to the user's provided buffer, otherwise the event was an error
 * or the event has been saved to the comp_cq slist.
 */

__attribute__((always_inline))
static inline int psmx3_cq_any_complete(struct psmx3_fid_cq *poll_cq,
					struct psmx3_fid_cq *comp_cq,
					struct psmx3_fid_av *av,
					PSMX3_STATUS_TYPE *status,
					void *op_context,
					void *buf,
					uint64_t flags,
					uint64_t data,
					struct psmx3_cq_event *event_in,
					int *event_saved,
					fi_addr_t *src_addr,
					int is_recv)
{
	struct psmx3_cq_event *event = event_in;

	*event_saved = 1;

	if (OFI_UNLIKELY(PSMX3_STATUS_ERROR(status))) {
		*event_saved = 0;
		event = psmx3_cq_alloc_event(comp_cq);
		if (!event)
			return -FI_ENOMEM;

		event->error = 1;
		event->cqe.err.op_context = op_context;
		event->cqe.err.flags = flags;
		event->cqe.err.err = -psmx3_errno(PSMX3_STATUS_ERROR(status));
		event->cqe.err.prov_errno = PSMX3_STATUS_ERROR(status);
		event->cqe.err.tag = PSMX3_GET_TAG64(PSMX3_STATUS_TAG(status));
		event->cqe.err.olen = PSMX3_STATUS_SNDLEN(status) - PSMX3_STATUS_RCVLEN(status);
		event->cqe.err.data = data;

		psmx3_cq_enqueue_event(comp_cq, event);
		return 0;
	}

	if (OFI_UNLIKELY(poll_cq != comp_cq || !event)) {
		*event_saved = 0;
		event = psmx3_cq_alloc_event(comp_cq);
		if (!event)
			return -FI_ENOMEM;

		event->error = 0;
	}

	if (is_recv) {
		psm2_epaddr_t source = PSMX3_STATUS_PEER(status);
		int source_sep_id = (flags & FI_REMOTE_CQ_DATA) ? 0 : data;

		if (event == event_in) {
			if (src_addr) {
				src_addr[0] = psmx3_av_translate_source(av, source);
				if (src_addr[0] == FI_ADDR_NOTAVAIL) {
					*event_saved = 0;
					event = psmx3_cq_alloc_event(comp_cq);
					if (!event)
						return -FI_ENOMEM;

					event->cqe = event_in->cqe;
					event->cqe.err.err = FI_EADDRNOTAVAIL;
					event->cqe.err.err_data = &comp_cq->error_data;
					event->error = !!event->cqe.err.err;
					if (av->addr_format == FI_ADDR_STR) {
						event->cqe.err.err_data_size = PSMX3_ERR_DATA_SIZE;
						psmx3_get_source_string_name(
							source, source_sep_id,
							(void *)&comp_cq->error_data,
							&event->cqe.err.err_data_size);
					} else {
						psmx3_get_source_name(
							source, source_sep_id,
							(void *)&comp_cq->error_data);
						event->cqe.err.err_data_size = sizeof(struct psmx3_ep_name);
					}
				}
			}
		} else {
			event->source_is_valid = 1;
			event->source_sep_id = source_sep_id;
			event->source = source;
			event->source_av = av;
		}
	}

	switch (comp_cq->format) {
	case FI_CQ_FORMAT_CONTEXT:
		event->cqe.context.op_context = op_context;
		break;

	case FI_CQ_FORMAT_MSG:
		event->cqe.msg.op_context = op_context;
		event->cqe.msg.flags = flags;
		event->cqe.msg.len = PSMX3_STATUS_RCVLEN(status);
		break;

	case FI_CQ_FORMAT_DATA:
		event->cqe.data.op_context = op_context;
		event->cqe.data.buf = buf;
		event->cqe.data.flags = flags;
		event->cqe.data.len = PSMX3_STATUS_RCVLEN(status);
		event->cqe.data.data = data;
		break;

	case FI_CQ_FORMAT_TAGGED:
		event->cqe.tagged.op_context = op_context;
		event->cqe.tagged.buf = buf;
		event->cqe.tagged.flags = flags;
		event->cqe.tagged.len = PSMX3_STATUS_RCVLEN(status);
		event->cqe.tagged.data = data;
		event->cqe.tagged.tag = PSMX3_GET_TAG64(PSMX3_STATUS_TAG(status));
		break;

	default:
		PSMX3_WARN(&psmx3_prov, FI_LOG_CQ,
			"unsupported CQ format %d\n", comp_cq->format);
		if (event != event_in)
			psmx3_cq_free_event(comp_cq, event);
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(event != event_in))
		psmx3_cq_enqueue_event(comp_cq, event);

	return 0;
}

static inline int psmx3_cq_tx_complete(struct psmx3_fid_cq *poll_cq,
				       struct psmx3_fid_cq *comp_cq,
				       struct psmx3_fid_av *av,
				       PSMX3_STATUS_TYPE *status,
				       void *op_context,
				       void *buf,
				       uint64_t flags,
				       uint64_t data,
				       struct psmx3_cq_event *event_in,
				       int *event_saved)
{
	return psmx3_cq_any_complete(poll_cq, comp_cq, av, status,
				     op_context, buf, flags, data,
				     event_in, event_saved, NULL, 0);
}

static inline int psmx3_cq_rx_complete(struct psmx3_fid_cq *poll_cq,
				       struct psmx3_fid_cq *comp_cq,
				       struct psmx3_fid_av *av,
				       PSMX3_STATUS_TYPE *status,
				       void *op_context,
				       void *buf,
				       uint64_t flags,
				       uint64_t data,
				       struct psmx3_cq_event *event_in,
				       fi_addr_t *src_addr,
				       int *event_saved)
{
	return psmx3_cq_any_complete(poll_cq, comp_cq, av, status,
				     op_context, buf, flags, data,
				     event_in, event_saved, src_addr, 1);
}

int
psmx3_mq_status_copy(struct psm2_mq_req_user *req, void *status_array, int entry_index)
{
	struct fi_context *fi_context;
	struct psmx3_fid_ep *ep;
	struct psmx3_fid_mr *mr;
	struct psmx3_am_request *am_req;
	struct psmx3_multi_recv *multi_recv_req;
	struct psmx3_sendv_request *sendv_req;
	struct psmx3_sendv_reply *sendv_rep;
	psm2_mq_req_t psm2_req;
	size_t len_remaining;
	void *op_context;
	void *buf;
	uint64_t flags;
	uint64_t data;
	int err;
	int context_type;
	int event_saved = 0;
	void *entry = NULL;

	struct psmx3_status_data *status_data = status_array;

	if (OFI_LIKELY(status_data->event_buffer && status_data->poll_cq))
		entry = (uint8_t *)status_data->event_buffer +
				(entry_index * status_data->poll_cq->entry_size);

	fi_context = PSMX3_STATUS_CONTEXT(req);

	if (OFI_UNLIKELY(!fi_context))
		return 0;

	context_type = (int)PSMX3_CTXT_TYPE(fi_context);
	flags = psmx3_comp_flags[context_type];
	ep = PSMX3_CTXT_EP(fi_context);

	switch (context_type) {
	case PSMX3_SEND_CONTEXT:
	case PSMX3_TSEND_CONTEXT:
		if (ep->send_cq) {
			op_context = fi_context;
			buf = PSMX3_CTXT_USER(fi_context);
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, buf, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->send_cntr)
			psmx3_cntr_inc(ep->send_cntr, PSMX3_STATUS_ERROR(req));

		/* Bi-directional send/recv performance tweak for KNL */
		if (event_saved && PSMX3_STATUS_SNDLEN(req) > 16384)
			event_saved++;
		break;

	case PSMX3_NOCOMP_SEND_CONTEXT:
	case PSMX3_NOCOMP_TSEND_CONTEXT:
		if (OFI_UNLIKELY(ep->send_cq && PSMX3_STATUS_ERROR(req))) {
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, NULL, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->send_cntr)
			psmx3_cntr_inc(ep->send_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_RECV_CONTEXT:
		if (OFI_UNLIKELY(PSMX3_IS_IOV_HEADER(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))) &&
				  !psmx3_handle_sendv_req(ep, req, 0))) {
			return 0;
		}
		if (ep->recv_cq) {
			op_context = fi_context;
			buf = PSMX3_CTXT_USER(fi_context);
			data = PSMX3_GET_CQDATA(PSMX3_STATUS_TAG(req));
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))))
				flags |= FI_REMOTE_CQ_DATA;
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, ep->recv_cq, ep->av,
					req, op_context, buf, flags, data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->recv_cntr)
			psmx3_cntr_inc(ep->recv_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_TRECV_CONTEXT:
		if (OFI_UNLIKELY(PSMX3_IS_IOV_HEADER(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))) &&
				 !psmx3_handle_sendv_req(ep, req, 0))) {
			return 0;
		}
		if (ep->recv_cq) {
			op_context = fi_context;
			buf = PSMX3_CTXT_USER(fi_context);
			data = PSMX3_GET_CQDATA(PSMX3_STATUS_TAG(req));
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))))
				flags |= FI_REMOTE_CQ_DATA;
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, ep->recv_cq, ep->av,
					req, op_context, buf, flags, data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->recv_cntr)
			psmx3_cntr_inc(ep->recv_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_NOCOMP_RECV_CONTEXT:
		if (OFI_UNLIKELY(PSMX3_IS_IOV_HEADER(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))) &&
				 !psmx3_handle_sendv_req(ep, req, 0))) {
			PSMX3_EP_PUT_OP_CONTEXT(ep, fi_context);
			return 0;
		}
		PSMX3_EP_PUT_OP_CONTEXT(ep, fi_context);
		if (OFI_UNLIKELY(ep->recv_cq && PSMX3_STATUS_ERROR(req))) {
			data = PSMX3_GET_CQDATA(PSMX3_STATUS_TAG(req));
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))))
				flags |= FI_REMOTE_CQ_DATA;
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, ep->recv_cq, ep->av,
					req, NULL, NULL, flags, data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->recv_cntr)
			psmx3_cntr_inc(ep->recv_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_NOCOMP_TRECV_CONTEXT:
		if (OFI_UNLIKELY(PSMX3_IS_IOV_HEADER(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))) &&
				 !psmx3_handle_sendv_req(ep, req, 0))) {
			PSMX3_EP_PUT_OP_CONTEXT(ep, fi_context);
			return 0;
		}
		PSMX3_EP_PUT_OP_CONTEXT(ep, fi_context);
		if (OFI_UNLIKELY(ep->recv_cq && PSMX3_STATUS_ERROR(req))) {
			data = PSMX3_GET_CQDATA(PSMX3_STATUS_TAG(req));
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))))
				flags |= FI_REMOTE_CQ_DATA;
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, ep->recv_cq, ep->av,
					req, NULL, NULL, flags, data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->recv_cntr)
			psmx3_cntr_inc(ep->recv_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_WRITE_CONTEXT:
		am_req = container_of(fi_context, struct psmx3_am_request,
					  fi_context);
		op_context = PSMX3_CTXT_USER(fi_context);
		free(am_req->tmpbuf);
		psmx3_am_request_free(status_data->trx_ctxt, am_req);
		if (ep->send_cq) {
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->write_cntr)
			psmx3_cntr_inc(ep->write_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_NOCOMP_WRITE_CONTEXT:
		am_req = container_of(fi_context, struct psmx3_am_request,
					  fi_context);
		op_context = PSMX3_CTXT_USER(fi_context);
		free(am_req->tmpbuf);
		psmx3_am_request_free(status_data->trx_ctxt, am_req);
		if (OFI_UNLIKELY(ep->send_cq && PSMX3_STATUS_ERROR(req))) {
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->write_cntr)
			psmx3_cntr_inc(ep->write_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_READ_CONTEXT:
		am_req = container_of(fi_context, struct psmx3_am_request,
					  fi_context);
		if (OFI_UNLIKELY(am_req->op == PSMX3_AM_REQ_READV)) {
			am_req->read.len_read += PSMX3_STATUS_RCVLEN(req);
			if (am_req->read.len_read < am_req->read.len) {
				PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,
					"readv: long protocol finishes early\n");
				if (PSMX3_STATUS_ERROR(req))
					am_req->error = psmx3_errno(PSMX3_STATUS_ERROR(req));
				/* Request to be freed in AM handler */
				return 0;
			}
		}
		op_context = PSMX3_CTXT_USER(fi_context);
		free(am_req->tmpbuf);
		psmx3_am_request_free(status_data->trx_ctxt, am_req);
		if (ep->send_cq) {
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->read_cntr)
			psmx3_cntr_inc(ep->read_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_NOCOMP_READ_CONTEXT:
		am_req = container_of(fi_context, struct psmx3_am_request,
					  fi_context);
		if (OFI_UNLIKELY(am_req->op == PSMX3_AM_REQ_READV)) {
			am_req->read.len_read += PSMX3_STATUS_RCVLEN(req);
			if (am_req->read.len_read < am_req->read.len) {
				PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,
					"readv: long protocol finishes early\n");
				if (PSMX3_STATUS_ERROR(req))
					am_req->error = psmx3_errno(PSMX3_STATUS_ERROR(req));
				/* Request to be freed in AM handler */
				return 0;
			}
		}
		op_context = PSMX3_CTXT_USER(fi_context);
		free(am_req->tmpbuf);
		psmx3_am_request_free(status_data->trx_ctxt, am_req);
		if (OFI_UNLIKELY(ep->send_cq && PSMX3_STATUS_ERROR(req))) {
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->read_cntr)
			psmx3_cntr_inc(ep->read_cntr, PSMX3_STATUS_ERROR(req));
		break;

	case PSMX3_MULTI_RECV_CONTEXT:
		if (OFI_UNLIKELY(PSMX3_IS_IOV_HEADER(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))) &&
			!psmx3_handle_sendv_req(ep, req, 1))) {
			return 0;
		}
		multi_recv_req = PSMX3_CTXT_USER(fi_context);
		if (ep->recv_cq) {
			op_context = fi_context;
			buf = multi_recv_req->buf + multi_recv_req->offset;
			data = PSMX3_GET_CQDATA(PSMX3_STATUS_TAG(req));
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(PSMX3_STATUS_TAG(req))))
				flags |= FI_REMOTE_CQ_DATA;
			len_remaining = multi_recv_req->len - multi_recv_req->offset -
					PSMX3_STATUS_RCVLEN(req);
			if (len_remaining < multi_recv_req->min_buf_size ||
			    len_remaining == 0)
				flags |= FI_MULTI_RECV;	/* buffer used up */
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, ep->recv_cq, ep->av,
					req, op_context, buf, flags, data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err))
				return err;
		}
		if (ep->recv_cntr)
			psmx3_cntr_inc(ep->recv_cntr, PSMX3_STATUS_ERROR(req));

		/* repost multi-recv buffer */
		multi_recv_req->offset += PSMX3_STATUS_RCVLEN(req);
		len_remaining = multi_recv_req->len - multi_recv_req->offset;
		if (len_remaining >= multi_recv_req->min_buf_size &&
		    len_remaining > 0) {
			if (len_remaining > PSMX3_MAX_MSG_SIZE)
				len_remaining = PSMX3_MAX_MSG_SIZE;
			err = psm3_mq_irecv2(ep->rx->psm2_mq,
						 multi_recv_req->src_addr, &multi_recv_req->tag,
						 &multi_recv_req->tagsel, multi_recv_req->flag,
						 multi_recv_req->buf + multi_recv_req->offset,
						 len_remaining,
						 (void *)fi_context, &psm2_req);
			if (OFI_UNLIKELY(err != PSM2_OK))
				return psmx3_errno(err);
			PSMX3_CTXT_REQ(fi_context) = psm2_req;
		} else {
			free(multi_recv_req);
		}
		break;

	case PSMX3_REMOTE_WRITE_CONTEXT:
		am_req = container_of(fi_context, struct psmx3_am_request, fi_context);
		if (am_req->op & PSMX3_AM_FORCE_ACK) {
			am_req->error = psmx3_errno(PSMX3_STATUS_ERROR(req));
			psmx3_am_ack_rma(am_req);
		}

		if (am_req->ep->recv_cq && (am_req->cq_flags & FI_REMOTE_CQ_DATA)) {
			flags |= FI_REMOTE_CQ_DATA;
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, am_req->ep->recv_cq, am_req->ep->av,
					req, NULL, NULL, flags, am_req->write.data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err)) {
				psmx3_am_request_free(status_data->trx_ctxt, am_req);
				return err;
			}
		}

		if (am_req->ep->caps & FI_RMA_EVENT) {
			if (am_req->ep->remote_write_cntr)
				psmx3_cntr_inc(am_req->ep->remote_write_cntr, 0);

			mr = PSMX3_CTXT_USER(fi_context);
			if (mr->cntr && mr->cntr != am_req->ep->remote_write_cntr)
				psmx3_cntr_inc(mr->cntr, 0);
		}

		/* NOTE: am_req->tmpbuf is unused here */
		psmx3_am_request_free(status_data->trx_ctxt, am_req);
		break;

	case PSMX3_REMOTE_READ_CONTEXT:
		am_req = container_of(fi_context, struct psmx3_am_request, fi_context);
		if (am_req->ep->caps & FI_RMA_EVENT) {
			if (am_req->ep->remote_read_cntr)
				psmx3_cntr_inc(am_req->ep->remote_read_cntr, 0);
		}

		/* NOTE: am_req->tmpbuf is unused here */
		psmx3_am_request_free(status_data->trx_ctxt, am_req);
		break;

	case PSMX3_SENDV_CONTEXT:
		sendv_req = PSMX3_CTXT_USER(fi_context);
		sendv_req->iov_done++;
		if (sendv_req->iov_protocol == PSMX3_IOV_PROTO_MULTI &&
			sendv_req->iov_done < sendv_req->iov_info.count + 1) {
			sendv_req->tag = PSMX3_STATUS_TAG(req);
			return 0;
		}
		if (ep->send_cq && !sendv_req->no_completion) {
			op_context = sendv_req->user_context;
			flags |= sendv_req->comp_flag;
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err)) {
				free(sendv_req);
				return err;
			}
		}
		if (ep->send_cntr)
			psmx3_cntr_inc(ep->send_cntr, PSMX3_STATUS_ERROR(req));
		free(sendv_req);
		break;

	case PSMX3_IOV_SEND_CONTEXT:
		sendv_req = PSMX3_CTXT_USER(fi_context);
		sendv_req->iov_done++;
		if (sendv_req->iov_done < sendv_req->iov_info.count + 1)
			return 0;
		PSMX3_STATUS_TAG(req) = sendv_req->tag;
		if (ep->send_cq && !sendv_req->no_completion) {
			op_context = sendv_req->user_context;
			flags |= sendv_req->comp_flag;
			err = psmx3_cq_tx_complete(
					status_data->poll_cq, ep->send_cq, ep->av,
					req, op_context, NULL, flags, 0,
					entry, &event_saved);
			if (OFI_UNLIKELY(err)) {
				free(sendv_req);
				return err;
			}
		}
		if (ep->send_cntr)
			psmx3_cntr_inc(ep->send_cntr, PSMX3_STATUS_ERROR(req));
		free(sendv_req);
		break;

	case PSMX3_IOV_RECV_CONTEXT:
		sendv_rep = PSMX3_CTXT_USER(fi_context);
		sendv_rep->iov_done++;
		sendv_rep->msg_length += PSMX3_STATUS_SNDLEN(req);
		sendv_rep->bytes_received += PSMX3_STATUS_RCVLEN(req);
		if (PSMX3_STATUS_ERROR(req) != PSM2_OK)
			sendv_rep->error_code = PSMX3_STATUS_ERROR(req);
		if (sendv_rep->iov_done < sendv_rep->iov_info.count)
			return 0;

		PSMX3_STATUS_TAG(req) = sendv_rep->tag;
		PSMX3_STATUS_RCVLEN(req) = sendv_rep->bytes_received;
		PSMX3_STATUS_SNDLEN(req) = sendv_rep->msg_length;
		PSMX3_STATUS_ERROR(req) = sendv_rep->error_code;

		if (ep->recv_cq && !sendv_rep->no_completion) {
			op_context = sendv_rep->user_context;
			buf = sendv_rep->buf;
			flags |= sendv_rep->comp_flag;
			data = PSMX3_GET_CQDATA(sendv_rep->tag);
			if (PSMX3_HAS_IMM(PSMX3_GET_FLAGS(sendv_rep->tag)))
				flags |= FI_REMOTE_CQ_DATA;
			err = psmx3_cq_rx_complete(
					status_data->poll_cq, ep->recv_cq, ep->av,
					req, op_context, buf, flags, data,
					entry, status_data->src_addr, &event_saved);
			if (OFI_UNLIKELY(err)) {
				free(sendv_rep);
				return err;
			}
		}
		if (ep->recv_cntr)
			psmx3_cntr_inc(ep->recv_cntr, PSMX3_STATUS_ERROR(req));

		if (sendv_rep->multi_recv) {
			/* repost the multi-recv buffer */
			fi_context = sendv_rep->user_context;
			multi_recv_req = PSMX3_CTXT_USER(fi_context);
			multi_recv_req->offset += PSMX3_STATUS_RCVLEN(req);
			len_remaining = multi_recv_req->len - multi_recv_req->offset;
			if (len_remaining >= multi_recv_req->min_buf_size &&
			    len_remaining > 0) {
				if (len_remaining > PSMX3_MAX_MSG_SIZE)
					len_remaining = PSMX3_MAX_MSG_SIZE;
				err = psm3_mq_irecv2(ep->rx->psm2_mq,
							 multi_recv_req->src_addr, &multi_recv_req->tag,
							 &multi_recv_req->tagsel, multi_recv_req->flag,
							 multi_recv_req->buf + multi_recv_req->offset,
							 len_remaining,
							 (void *)fi_context, &psm2_req);
				if (OFI_UNLIKELY(err != PSM2_OK)) {
					free(sendv_rep);
					return psmx3_errno(err);
				}
				PSMX3_CTXT_REQ(fi_context) = psm2_req;
			} else {
				free(multi_recv_req);
			}
		}

		free(sendv_rep);
		break;
	}

	return event_saved;
}

int psmx3_cq_poll_mq(struct psmx3_fid_cq *cq,
		     struct psmx3_trx_ctxt *trx_ctxt,
		     struct psmx3_cq_event *event_in,
		     int count, fi_addr_t *src_addr)
{
	struct psmx3_status_data status_data;

	/* psm3_mq_ipeek_dequeue_multi needs non-zero count to make progress */
	if (!count) {
		event_in = NULL;
		count = 1;
	}

	status_data.poll_cq = cq;
	status_data.event_buffer = event_in;
	status_data.src_addr = src_addr;
	status_data.trx_ctxt = trx_ctxt;

	psm3_mq_ipeek_dequeue_multi(trx_ctxt->psm2_mq, &status_data,
			psmx3_mq_status_copy, &count);
	return count;
}

DIRECT_FN
STATIC ssize_t psmx3_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
				 fi_addr_t *src_addr)
{
	struct psmx3_fid_cq *cq_priv;
	struct psmx3_cq_event *event;
	struct psmx3_poll_ctxt *poll_ctxt;
	struct slist_entry *item, *prev;
	int ret;
	ssize_t read_count;
	fi_addr_t source;
	int i;

	cq_priv = container_of(cq, struct psmx3_fid_cq, cq);

	if (slist_empty(&cq_priv->event_queue) || !buf) {
		slist_foreach(&cq_priv->poll_list, item, prev) {
			poll_ctxt = container_of(item, struct psmx3_poll_ctxt,
						 list_entry);

			if (OFI_UNLIKELY(!poll_ctxt->trx_ctxt->poll_active))
				continue;

			ret = psmx3_cq_poll_mq(cq_priv, poll_ctxt->trx_ctxt,
					       (struct psmx3_cq_event *)buf,
					       count, src_addr);
			if (ret > 0)
				return ret;

			if (poll_ctxt->trx_ctxt->am_progress)
				psmx3_am_progress(poll_ctxt->trx_ctxt);

			(void) prev; /* suppress compiler warning */
		}
	}

	if (OFI_UNLIKELY(cq_priv->pending_error != NULL))
		return -FI_EAVAIL;

	assert(buf || !count);

	read_count = 0;
	for (i = 0; i < count; i++) {
		if (slist_empty(&cq_priv->event_queue))
			break;

		event = psmx3_cq_dequeue_event(cq_priv);
		if (event) {
			if (!event->error) {
				if (src_addr && event->source_is_valid) {
					source = psmx3_av_translate_source(
							event->source_av, event->source);
					if (source == FI_ADDR_NOTAVAIL) {
						if (cq_priv->domain->addr_format == FI_ADDR_STR) {
							event->cqe.err.err_data_size = PSMX3_ERR_DATA_SIZE;
							psmx3_get_source_string_name(
								event->source, event->source_sep_id,
								(void *)&cq_priv->error_data,
								&event->cqe.err.err_data_size);
						} else {
							psmx3_get_source_name(
								event->source,
								event->source_sep_id,
								(void *)&cq_priv->error_data);
							event->cqe.err.err_data_size = sizeof(struct psmx3_ep_name);
						}
						event->cqe.err.err_data = &cq_priv->error_data;
						event->cqe.err.err = FI_EADDRNOTAVAIL;
						event->error = !!event->cqe.err.err;
						cq_priv->pending_error = event;
						if (!read_count)
							read_count = -FI_EAVAIL;
						break;
					}

					*src_addr = source;
				}
				// see assertion above -- buf is non NULL in case if count is non 0
				// coverity[var_deref_model]
				memcpy(buf, (void *)&event->cqe, cq_priv->entry_size);
				psmx3_cq_free_event(cq_priv, event);

				read_count++;
				buf = (uint8_t *)buf + cq_priv->entry_size;
				if (src_addr)
					src_addr++;
				continue;
			} else {
				cq_priv->pending_error = event;
				if (!read_count)
					read_count = -FI_EAVAIL;
				break;
			}
		} else {
			break;
		}
	}

	/*
	 * Return 0 if and only if the input count is 0 and the CQ is not empty.
	 * This is used by the util poll code to check the poll state.
	 */
	if (!read_count &&  (count || slist_empty(&cq_priv->event_queue)))
		read_count = -FI_EAGAIN;

	return read_count;
}

DIRECT_FN
STATIC ssize_t psmx3_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	return psmx3_cq_readfrom(cq, buf, count, NULL);
}

DIRECT_FN
STATIC ssize_t psmx3_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
				uint64_t flags)
{
	struct psmx3_fid_cq *cq_priv;

	cq_priv = container_of(cq, struct psmx3_fid_cq, cq);

	cq_priv->domain->cq_lock_fn(&cq_priv->lock, 2);
	if (cq_priv->pending_error) {
		ofi_cq_err_memcpy(cq_priv->domain->fabric->util_fabric.fabric_fid.api_version,
				  buf, &cq_priv->pending_error->cqe.err);
		free(cq_priv->pending_error);
		cq_priv->pending_error = NULL;
		psmx3_unlock(&cq_priv->lock, 2);
		return 1;
	}
	cq_priv->domain->cq_unlock_fn(&cq_priv->lock, 2);

	return -FI_EAGAIN;
}

DIRECT_FN
STATIC ssize_t psmx3_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
				  fi_addr_t *src_addr, const void *cond,
				  int timeout)
{
	struct psmx3_fid_cq *cq_priv;
	struct psmx3_poll_ctxt *poll_ctxt;
	struct slist_entry *item, *prev;
	struct timespec ts0, ts;
	size_t threshold, event_count;
	int msec_passed = 0;
	int sth_happened = 0;

	cq_priv = container_of(cq, struct psmx3_fid_cq, cq);
	if (cq_priv->wait_cond == FI_CQ_COND_THRESHOLD)
		threshold = (size_t) cond;
	else
		threshold = 1;

	/* NOTE: "cond" is only a hint, not a mandatory condition. */
	event_count = cq_priv->event_count;
	if (event_count < threshold) {
		if (cq_priv->wait) {
			if (ofi_atomic_get32(&cq_priv->signaled)) {
				ofi_atomic_set32(&cq_priv->signaled, 0);
				return -FI_ECANCELED;
			}
			ssize_t wait_result = fi_wait((struct fid_wait *)cq_priv->wait, timeout);
			if (wait_result != FI_SUCCESS) {
				return wait_result;
			}
		} else {
			clock_gettime(CLOCK_REALTIME, &ts0);
			while (!sth_happened) {
				slist_foreach(&cq_priv->poll_list, item, prev) {
					poll_ctxt = container_of(item,
								 struct psmx3_poll_ctxt,
								 list_entry);

					if (OFI_UNLIKELY(!poll_ctxt->trx_ctxt->poll_active))
						continue;

					sth_happened =
						psmx3_cq_poll_mq(cq_priv,
								 poll_ctxt->trx_ctxt,
								 NULL, 0, NULL);
					if (sth_happened)
						break;

					(void) prev; /* suppress compiler warning */
				}

				/* CQ may be updated asynchronously by the AM handlers */
				if (cq_priv->event_count > event_count)
					break;

				if (ofi_atomic_get32(&cq_priv->signaled)) {
					ofi_atomic_set32(&cq_priv->signaled, 0);
					return -FI_ECANCELED;
				}

				if (timeout < 0)
					continue;

				clock_gettime(CLOCK_REALTIME, &ts);
				msec_passed = (ts.tv_sec - ts0.tv_sec) * 1000 +
					       (ts.tv_nsec - ts0.tv_nsec) / 1000000;

				if (msec_passed >= timeout)
					break;
			}
		}
	}

	return psmx3_cq_readfrom(cq, buf, count, src_addr);
}

DIRECT_FN
STATIC ssize_t psmx3_cq_sread(struct fid_cq *cq, void *buf, size_t count,
			      const void *cond, int timeout)
{
	return psmx3_cq_sreadfrom(cq, buf, count, NULL, cond, timeout);
}

DIRECT_FN
STATIC int psmx3_cq_signal(struct fid_cq *cq)
{
	struct psmx3_fid_cq *cq_priv;
	cq_priv = container_of(cq, struct psmx3_fid_cq, cq);

	ofi_atomic_set32(&cq_priv->signaled, 1);
	if (cq_priv->wait)
		cq_priv->wait->signal(cq_priv->wait);

	return 0;
}

DIRECT_FN
STATIC const char *psmx3_cq_strerror(struct fid_cq *cq, int prov_errno, const void *prov_data,
				     char *buf, size_t len)
{
	return psm3_error_get_string(prov_errno);
}

static int psmx3_cq_close(fid_t fid)
{
	struct psmx3_fid_cq *cq;
	struct slist_entry *entry;
	struct psmx3_cq_event *item;
	struct psmx3_poll_ctxt *poll_item;

	cq = container_of(fid, struct psmx3_fid_cq, cq.fid);

	while (!slist_empty(&cq->poll_list)) {
		entry = slist_remove_head(&cq->poll_list);
		poll_item = container_of(entry, struct psmx3_poll_ctxt, list_entry);
		if (!ofi_atomic_dec32(&poll_item->trx_ctxt->poll_refcnt))
			free(poll_item->trx_ctxt);
		free(poll_item);
	}

	while (!slist_empty(&cq->free_list)) {
		entry = slist_remove_head(&cq->free_list);
		item = container_of(entry, struct psmx3_cq_event, list_entry);
		free(item);
	}

	while (!slist_empty(&cq->event_queue)) {
		entry = slist_remove_head(&cq->event_queue);
		item = container_of(entry, struct psmx3_cq_event, list_entry);
		free(item);
	}

	ofi_spin_destroy(&cq->lock);

	if (cq->wait) {
		fi_poll_del(&cq->wait->pollset->poll_fid, &cq->cq.fid, 0);
		if (cq->wait_is_local)
			fi_close(&cq->wait->wait_fid.fid);
	}

	psmx3_domain_release(cq->domain);
	free(cq);

	return 0;
}

static int psmx3_cq_control(struct fid *fid, int command, void *arg)
{
	struct psmx3_fid_cq *cq;
	int ret = 0;

	cq = container_of(fid, struct psmx3_fid_cq, cq.fid);

	switch (command) {
	case FI_GETWAIT:
		if (cq->wait)
			ret = fi_control(&cq->wait->wait_fid.fid, FI_GETWAIT, arg);
		else
			return -FI_EINVAL;
		break;

	default:
		return -FI_ENOSYS;
	}

	return ret;
}

static struct fi_ops psmx3_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_cq_close,
	.bind = fi_no_bind,
	.control = psmx3_cq_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cq psmx3_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = psmx3_cq_read,
	.readfrom = psmx3_cq_readfrom,
	.readerr = psmx3_cq_readerr,
	.sread = psmx3_cq_sread,
	.sreadfrom = psmx3_cq_sreadfrom,
	.signal = psmx3_cq_signal,
	.strerror = psmx3_cq_strerror,
};

DIRECT_FN
int psmx3_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context)
{
	struct psmx3_fid_domain *domain_priv;
	struct psmx3_fid_cq *cq_priv;
	struct fid_wait *wait = NULL;
	struct psmx3_cq_event *event;
	struct fi_wait_attr wait_attr;
	int wait_is_local = 0;
	int entry_size;
	int err;
	int i;

	domain_priv = container_of(domain, struct psmx3_fid_domain,
				   util_domain.domain_fid);
	switch (attr->format) {
	case FI_CQ_FORMAT_UNSPEC:
		attr->format = FI_CQ_FORMAT_TAGGED;
		entry_size = sizeof(struct fi_cq_tagged_entry);
		break;

	case FI_CQ_FORMAT_CONTEXT:
		entry_size = sizeof(struct fi_cq_entry);
		break;

	case FI_CQ_FORMAT_MSG:
		entry_size = sizeof(struct fi_cq_msg_entry);
		break;

	case FI_CQ_FORMAT_DATA:
		entry_size = sizeof(struct fi_cq_data_entry);
		break;

	case FI_CQ_FORMAT_TAGGED:
		entry_size = sizeof(struct fi_cq_tagged_entry);
		break;

	default:
		PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
			"attr->format=%d, supported=%d...%d\n", attr->format,
			FI_CQ_FORMAT_UNSPEC, FI_CQ_FORMAT_TAGGED);
		return -FI_EINVAL;
	}

	if (psmx3_env.yield_mode && attr->wait_obj != FI_WAIT_NONE) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
			"waitset %d not allowed when FI_PSM3_YIELD_MODE enabled\n", attr->wait_obj);
		return -FI_EINVAL;
	}
	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
		break;

	case FI_WAIT_SET:
		if (!attr->wait_set) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
				"FI_WAIT_SET is specified but attr->wait_set is NULL\n");
			return -FI_EINVAL;
		}
		wait = attr->wait_set;
		break;

	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		wait_attr.wait_obj = attr->wait_obj;
		wait_attr.flags = 0;
		err = fi_wait_open(&domain_priv->fabric->util_fabric.fabric_fid,
				   &wait_attr, (struct fid_wait **)&wait);
		if (err)
			return err;
		wait_is_local = 1;
		break;

	default:
		PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
			"attr->wait_obj=%d, supported=%d...%d\n", attr->wait_obj,
			FI_WAIT_NONE, FI_WAIT_MUTEX_COND);
		return -FI_EINVAL;
	}

	if (wait) {
		switch (attr->wait_cond) {
		case FI_CQ_COND_NONE:
		case FI_CQ_COND_THRESHOLD:
			break;

		default:
			PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
				"attr->wait_cond=%d, supported=%d...%d\n",
				attr->wait_cond, FI_CQ_COND_NONE, FI_CQ_COND_THRESHOLD);
			return -FI_EINVAL;
		}
	}

	cq_priv = (struct psmx3_fid_cq *) calloc(1, sizeof *cq_priv);
	if (!cq_priv) {
		if (wait)
			free(wait);
		return -FI_ENOMEM;
	}

	psmx3_domain_acquire(domain_priv);

	cq_priv->domain = domain_priv;
	cq_priv->format = attr->format;
	cq_priv->entry_size = entry_size;
	if (wait) {
		cq_priv->wait = container_of(wait, struct util_wait, wait_fid);
		cq_priv->wait_cond = attr->wait_cond;
	}
	cq_priv->wait_is_local = wait_is_local;
	ofi_atomic_initialize32(&cq_priv->signaled, 0);

	cq_priv->cq.fid.fclass = FI_CLASS_CQ;
	cq_priv->cq.fid.context = context;
	cq_priv->cq.fid.ops = &psmx3_fi_ops;
	cq_priv->cq.ops = &psmx3_cq_ops;

	slist_init(&cq_priv->poll_list);
	slist_init(&cq_priv->event_queue);
	slist_init(&cq_priv->free_list);
	ofi_spin_init(&cq_priv->lock);

#define PSMX3_FREE_LIST_SIZE	64
	for (i=0; i<PSMX3_FREE_LIST_SIZE; i++) {
		event = calloc(1, sizeof(*event));
		if (!event) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CQ, "out of memory.\n");
			exit(-1);
		}
		slist_insert_tail(&event->list_entry, &cq_priv->free_list);
	}

	if (wait)
		fi_poll_add(&cq_priv->wait->pollset->poll_fid, &cq_priv->cq.fid, 0);

	*cq = &cq_priv->cq;
	return 0;
}

