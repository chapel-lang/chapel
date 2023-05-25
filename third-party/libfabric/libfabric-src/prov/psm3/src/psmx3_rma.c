/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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
#include "psmx3_trigger.h"

static inline void psmx3_iov_copy(struct iovec *iov, size_t count,
				  size_t offset, const void *src,
				  size_t len)
{
	int i;
	size_t copy_len;

	for (i=0; i<count && len; i++) {
		if (offset >= iov[i].iov_len) {
			offset -= iov[i].iov_len;
			continue;
		}

		copy_len = iov[i].iov_len - offset;
		if (copy_len > len)
			copy_len = len;

		memcpy((uint8_t *)iov[i].iov_base + offset, src, copy_len);

		src = (const uint8_t *)src + copy_len;
		len -= copy_len;

		if (offset)
			offset = 0;
	}
}

/* RMA protocol:
 *
 * Write REQ:
 *	args[0].u32w0	cmd, flag
 *	args[0].u32w1	len
 *	args[1].u64	req
 *	args[2].u64	addr
 *	args[3].u64	key
 *	args[4].u64	data (optional)
 *
 * Write REP:
 *	args[0].u32w0	cmd, flag
 *	args[0].u32w1	error
 *	args[1].u64	req
 *
 * Read REQ:
 *	args[0].u32w0	cmd, flag
 *	args[0].u32w1	len
 *	args[1].u64	req
 *	args[2].u64	addr
 *	args[3].u64	key
 *	args[4].u64	offset / unused for long protocol
 *
 * Read REP:
 *	args[0].u32w0	cmd, flag
 *	args[0].u32w1	error
 *	args[1].u64	req
 *	args[2].u64	offset
 */

int psmx3_am_rma_handler(psm2_am_token_t token, psm2_amarg_t *args,
		int nargs, void *src, uint32_t len,
		void *hctx)
{
	psm2_amarg_t rep_args[8];
	uint8_t *rma_addr;
	ssize_t rma_len;
	uint64_t key;
	int err = 0;
	int op_error = 0;
	int cmd, eom, has_data;
	struct psmx3_am_request *req;
	struct psmx3_cq_event *event;
	uint64_t offset;
	struct psmx3_fid_mr *mr;
	psm2_epaddr_t epaddr;
	struct psmx3_trx_ctxt *rx;

	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;

	psm3_am_get_source(token, &epaddr);
	cmd = PSMX3_AM_GET_OP(args[0].u32w0);
	eom = args[0].u32w0 & PSMX3_AM_EOM;
	has_data = args[0].u32w0 & PSMX3_AM_DATA;

	switch (cmd) {
	case PSMX3_AM_REQ_WRITE:
		rx = (struct psmx3_trx_ctxt *)hctx;
		rma_len = args[0].u32w1;
		rma_addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		mr = psmx3_mr_get(rx->domain, key);
		op_error = mr ?
			psmx3_mr_validate(mr, (uint64_t)rma_addr, len, FI_REMOTE_WRITE) :
			-FI_EINVAL;
		if (!op_error) {
			rma_addr += mr->offset;
			memcpy(rma_addr, src, len);
			if (eom) {
				if (rx->ep->recv_cq && has_data) {
					/* TODO: report the addr/len of the whole write */
					event = psmx3_cq_create_event(
							rx->ep->recv_cq,
							0, /* context */
							rma_addr,
							FI_REMOTE_WRITE | FI_RMA | FI_REMOTE_CQ_DATA,
							rma_len,
							args[4].u64,
							0, /* tag */
							0, /* olen */
							0);

					if (event)
						psmx3_cq_enqueue_event(rx->ep->recv_cq, event);
					else
						err = -FI_ENOMEM;
				}

				if (rx->ep->caps & FI_RMA_EVENT) {
					if (rx->ep->remote_write_cntr)
						psmx3_cntr_inc(rx->ep->remote_write_cntr, 0);

					if (mr->cntr && mr->cntr != rx->ep->remote_write_cntr)
						psmx3_cntr_inc(mr->cntr, 0);
				}
			}
		}
		if (eom || op_error) {
			rep_args[0].u32w0 = PSMX3_AM_REP_WRITE | eom;
			rep_args[0].u32w1 = op_error;
			rep_args[1].u64 = args[1].u64;
			err = psm3_am_reply_short(token, PSMX3_AM_RMA_HANDLER,
						  rep_args, 2, NULL, 0, 0,
						  NULL, NULL );
		}
		break;

	case PSMX3_AM_REQ_WRITE_LONG:
		rx = (struct psmx3_trx_ctxt *)hctx;
		rma_len = args[0].u32w1;
		rma_addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		mr = psmx3_mr_get(rx->domain, key);
		op_error = mr ?
			psmx3_mr_validate(mr, (uint64_t)rma_addr, rma_len, FI_REMOTE_WRITE) :
			-FI_EINVAL;
		if (op_error) {
			rep_args[0].u32w0 = PSMX3_AM_REP_WRITE | eom;
			rep_args[0].u32w1 = op_error;
			rep_args[1].u64 = args[1].u64;
			err = psm3_am_reply_short(token, PSMX3_AM_RMA_HANDLER,
						  rep_args, 2, NULL, 0, 0,
						  NULL, NULL );
			break;
		}

		rma_addr += mr->offset;

		req = psmx3_am_request_alloc(rx);
		if (!req) {
			err = -FI_ENOMEM;
		} else {
			req->ep = rx->ep;
			req->op = args[0].u32w0;
			req->write.addr = (uint64_t)rma_addr;
			req->write.len = rma_len;
			req->write.key = key;
			req->write.context = (void *)args[1].u64;
			req->write.peer_addr = (void *)epaddr;
			req->write.data = has_data ? args[4].u64 : 0;
			req->cq_flags = FI_REMOTE_WRITE | FI_RMA |
					(has_data ? FI_REMOTE_CQ_DATA : 0),
			PSMX3_CTXT_TYPE(&req->fi_context) = PSMX3_REMOTE_WRITE_CONTEXT;
			PSMX3_CTXT_USER(&req->fi_context) = mr;
			PSMX3_SET_TAG(psm2_tag, (uint64_t)req->write.context, 0,
					PSMX3_RMA_TYPE_WRITE);
			PSMX3_SET_MASK(psm2_tagsel, PSMX3_MATCH_ALL, PSMX3_RMA_TYPE_MASK);
			op_error = psm3_mq_fp_msg(rx->psm2_ep, rx->psm2_mq,
						 (psm2_epaddr_t)epaddr,
						 &psm2_tag, &psm2_tagsel, 0,
						 (void *)rma_addr, rma_len,
						 (void *)&req->fi_context, PSM2_MQ_IRECV_FP, &psm2_req);
			if (op_error) {
				rep_args[0].u32w0 = PSMX3_AM_REP_WRITE | eom;
				rep_args[0].u32w1 = op_error;
				rep_args[1].u64 = args[1].u64;
				err = psm3_am_reply_short(token, PSMX3_AM_RMA_HANDLER,
							  rep_args, 2, NULL, 0, 0,
							  NULL, NULL );
				psmx3_am_request_free(rx, req);
				break;
			}
		}
		break;

	case PSMX3_AM_REQ_READ:
		rx = (struct psmx3_trx_ctxt *)hctx;
		rma_len = args[0].u32w1;
		rma_addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		offset = args[4].u64;
		mr = psmx3_mr_get(rx->domain, key);
		op_error = mr ?
			psmx3_mr_validate(mr, (uint64_t)rma_addr, rma_len, FI_REMOTE_READ) :
			-FI_EINVAL;
		if (!op_error) {
			rma_addr += mr->offset;
		} else {
			rma_addr = NULL;
			rma_len = 0;
		}

		rep_args[0].u32w0 = PSMX3_AM_REP_READ | eom;
		rep_args[0].u32w1 = op_error;
		rep_args[1].u64 = args[1].u64;
		rep_args[2].u64 = offset;
		err = psm3_am_reply_short(token, PSMX3_AM_RMA_HANDLER,
				rep_args, 3, rma_addr, rma_len, 0,
				NULL, NULL );

		if (eom && !op_error) {
			if (rx->ep->caps & FI_RMA_EVENT) {
				if (rx->ep->remote_read_cntr)
					psmx3_cntr_inc(rx->ep->remote_read_cntr, 0);
			}
		}
		break;

	case PSMX3_AM_REQ_READ_LONG:
		rx = (struct psmx3_trx_ctxt *)hctx;
		rma_len = args[0].u32w1;
		rma_addr = (uint8_t *)(uintptr_t)args[2].u64;
		key = args[3].u64;
		mr = psmx3_mr_get(rx->domain, key);
		op_error = mr ?
			psmx3_mr_validate(mr, (uint64_t)rma_addr, rma_len, FI_REMOTE_READ) :
			-FI_EINVAL;
		if (op_error) {
			rep_args[0].u32w0 = PSMX3_AM_REP_READ | eom;
			rep_args[0].u32w1 = op_error;
			rep_args[1].u64 = args[1].u64;
			rep_args[2].u64 = 0;
			err = psm3_am_reply_short(token, PSMX3_AM_RMA_HANDLER,
					rep_args, 3, NULL, 0, 0,
					NULL, NULL );
			break;
		}

		rma_addr += mr->offset;

		req = psmx3_am_request_alloc(rx);
		if (!req) {
			err = -FI_ENOMEM;
		} else {
			req->ep = rx->ep;
			req->op = args[0].u32w0;
			req->read.addr = (uint64_t)rma_addr;
			req->read.len = rma_len;
			req->read.key = key;
			req->read.context = (void *)args[1].u64;
			req->read.peer_addr = (void *)epaddr;
			PSMX3_CTXT_TYPE(&req->fi_context) = PSMX3_REMOTE_READ_CONTEXT;
			PSMX3_CTXT_USER(&req->fi_context) = mr;
			PSMX3_SET_TAG(psm2_tag, (uint64_t)req->read.context, 0,
			PSMX3_RMA_TYPE_READ);
			op_error = psm3_mq_fp_msg(rx->psm2_ep, rx->psm2_mq,
				  (psm2_epaddr_t)req->read.peer_addr,
				 &psm2_tag, 0, 0,
				 (void *)req->read.addr, req->read.len,
				 (void *)&req->fi_context, PSM2_MQ_ISEND_FP, &psm2_req);
			if (op_error) {
				rep_args[0].u32w0 = PSMX3_AM_REP_READ | eom;
				rep_args[0].u32w1 = op_error;
				rep_args[1].u64 = args[1].u64;
				rep_args[2].u64 = 0;
				err = psm3_am_reply_short(token, PSMX3_AM_RMA_HANDLER,
						rep_args, 3, NULL, 0, 0,
						NULL, NULL );
				psmx3_am_request_free(rx, req);
				break;
			}
		}
		break;

	case PSMX3_AM_REP_WRITE:
		req = (struct psmx3_am_request *)(uintptr_t)args[1].u64;
		assert(req->op == PSMX3_AM_REQ_WRITE);
		op_error = (int)args[0].u32w1;
		if (!req->error)
			req->error = op_error;
		if (eom) {
			if (req->ep->send_cq && (!req->no_event || req->error)) {
				event = psmx3_cq_create_event(
						req->ep->send_cq,
						req->write.context,
						req->write.buf,
						req->cq_flags,
						req->write.len,
						0, /* data */
						0, /* tag */
						0, /* olen */
						req->error);
				if (event)
					psmx3_cq_enqueue_event(req->ep->send_cq, event);
				else
					err = -FI_ENOMEM;
			}

			if (req->ep->write_cntr)
				psmx3_cntr_inc(req->ep->write_cntr, req->error);

			free(req->tmpbuf);
			psmx3_am_request_free(req->ep->tx, req);
		}
		break;

	case PSMX3_AM_REP_READ:
		req = (struct psmx3_am_request *)(uintptr_t)args[1].u64;
		assert(req->op == PSMX3_AM_REQ_READ || req->op == PSMX3_AM_REQ_READV);
		op_error = (int)args[0].u32w1;
		offset = args[2].u64;
		if (!req->error)
			req->error = op_error;
		if (!op_error) {
			if (req->op == PSMX3_AM_REQ_READ)
				memcpy(req->read.buf + offset, src, len);
			else
				psmx3_iov_copy(req->iov, req->read.iov_count, offset, src, len);

			req->read.len_read += len;
		}
		if (eom || req->read.len == req->read.len_read) {
			if (!eom)
				FI_INFO(&psmx3_prov, FI_LOG_EP_DATA,
					"readv: short protocol finishes after long protocol.\n");
			if (req->ep->send_cq && (!req->no_event || req->error)) {
				event = psmx3_cq_create_event(
						req->ep->send_cq,
						req->read.context,
						req->read.buf,
						req->cq_flags,
						req->read.len_read,
						0, /* data */
						0, /* tag */
						req->read.len - req->read.len_read,
						req->error);
				if (event)
					psmx3_cq_enqueue_event(req->ep->send_cq, event);
				else
					err = -FI_ENOMEM;
			}

			if (req->ep->read_cntr)
				psmx3_cntr_inc(req->ep->read_cntr, req->error);
 
			free(req->tmpbuf);
			psmx3_am_request_free(req->ep->tx, req);
		}
		break;

	default:
		err = -FI_EINVAL;
	}
	return err;
}

static ssize_t psmx3_rma_self(int am_cmd,
			      struct psmx3_fid_ep *ep,
			      void *buf, size_t len, void *desc,
			      uint64_t addr, uint64_t key,
			      void *context, uint64_t flags, uint64_t data)
{
	struct psmx3_fid_mr *mr;
	struct psmx3_cq_event *event;
	struct psmx3_fid_cntr *cntr = NULL;
	struct psmx3_fid_cntr *mr_cntr = NULL;
	struct psmx3_fid_cq *cq = NULL;
	int no_event;
	int err = 0;
	int op_error = 0;
	int access;
	uint8_t *dst, *src;
	uint64_t cq_flags;
	struct iovec *iov = buf;
	size_t iov_count = len;
	int i;

	switch (am_cmd) {
	case PSMX3_AM_REQ_WRITE:
		access = FI_REMOTE_WRITE;
		cq_flags = FI_WRITE | FI_RMA;
		break;
	case PSMX3_AM_REQ_WRITEV:
		access = FI_REMOTE_WRITE;
		cq_flags = FI_WRITE | FI_RMA;
		len = 0;
		for (i=0; i<iov_count; i++)
			len += iov[i].iov_len;
		break;
	case PSMX3_AM_REQ_READ:
		access = FI_REMOTE_READ;
		cq_flags = FI_READ | FI_RMA;
		break;
	case PSMX3_AM_REQ_READV:
		access = FI_REMOTE_READ;
		cq_flags = FI_READ | FI_RMA;
		len = 0;
		for (i=0; i<iov_count; i++)
			len += iov[i].iov_len;
		break;
	default:
		return -FI_EINVAL;
	}

	mr = psmx3_mr_get(ep->domain, key);
	op_error = mr ? psmx3_mr_validate(mr, addr, len, access) : -FI_EINVAL;

	if (!op_error) {
		addr += mr->offset;
		switch (am_cmd) {
		case PSMX3_AM_REQ_WRITE:
			cntr = ep->remote_write_cntr;
			if (flags & FI_REMOTE_CQ_DATA)
				cq = ep->recv_cq;
			if (mr->cntr != cntr)
				mr_cntr = mr->cntr;
			memcpy((void *)addr, buf, len);
			break;

		case PSMX3_AM_REQ_WRITEV:
			cntr = ep->remote_write_cntr;
			if (flags & FI_REMOTE_CQ_DATA)
				cq = ep->recv_cq;
			if (mr->cntr != cntr)
				mr_cntr = mr->cntr;
			dst = (void *)addr;
			for (i=0; i<iov_count; i++)
				if (iov[i].iov_len) {
					memcpy(dst, iov[i].iov_base, iov[i].iov_len);
					dst += iov[i].iov_len;
				}
			break;

		case PSMX3_AM_REQ_READ:
			cntr = ep->remote_read_cntr;
			memcpy(buf, (void *)addr, len);
			break;

		case PSMX3_AM_REQ_READV:
			cntr = ep->remote_read_cntr;
			src = (void *)addr;
			for (i=0; i<iov_count; i++)
				if (iov[i].iov_len) {
					memcpy(iov[i].iov_base, src, iov[i].iov_len);
					src += iov[i].iov_len;
				}
			break;
		}

		if (cq) {
			event = psmx3_cq_create_event(
					cq,
					0, /* context */
					(void *)addr,
					FI_REMOTE_WRITE | FI_RMA | FI_REMOTE_CQ_DATA,
					len,
					data,
					0, /* tag */
					0, /* olen */
					0 /* err */);

			if (event)
				psmx3_cq_enqueue_event(cq, event);
			else
				err = -FI_ENOMEM;
		}

		if (ep->caps & FI_RMA_EVENT) {
			if (cntr)
				psmx3_cntr_inc(cntr, 0);

			if (mr_cntr)
				psmx3_cntr_inc(mr_cntr, 0);
		}
	}

	no_event = (flags & PSMX3_NO_COMPLETION) ||
		   (ep->send_selective_completion && !(flags & FI_COMPLETION));

	if (ep->send_cq && (!no_event || op_error)) {
		event = psmx3_cq_create_event(
				ep->send_cq,
				context,
				(void *)buf,
				cq_flags,
				len,
				0, /* data */
				0, /* tag */
				0, /* olen */
				op_error);
		if (event)
			psmx3_cq_enqueue_event(ep->send_cq, event);
		else
			err = -FI_ENOMEM;
	}

	switch (am_cmd) {
	case PSMX3_AM_REQ_WRITE:
	case PSMX3_AM_REQ_WRITEV:
		if (ep->write_cntr)
			psmx3_cntr_inc(ep->write_cntr, op_error);
		break;

	case PSMX3_AM_REQ_READ:
	case PSMX3_AM_REQ_READV:
		if (ep->read_cntr)
			psmx3_cntr_inc(ep->read_cntr, op_error);
		break;
	}

	return err;
}

void psmx3_am_ack_rma(struct psmx3_am_request *req)
{
	psm2_amarg_t args[8];
	int err;

	if ((req->op & PSMX3_AM_OP_MASK) != PSMX3_AM_REQ_WRITE_LONG)
		return;

	args[0].u32w0 = PSMX3_AM_REP_WRITE | PSMX3_AM_EOM;
	args[0].u32w1 = req->error;
	args[1].u64 = (uint64_t)(uintptr_t)req->write.context;

	err = psm3_am_request_short(req->write.peer_addr,
				    PSMX3_AM_RMA_HANDLER, args, 2, NULL, 0,
				    PSM2_AM_FLAG_NOREPLY, NULL, NULL);
	if (err)
		FI_INFO(&psmx3_prov, FI_LOG_EP_DATA,
			"failed to send am_ack: err %d.\n", err);
}

ssize_t psmx3_read_generic(struct fid_ep *ep, void *buf, size_t len,
			   void *desc, fi_addr_t src_addr,
			   uint64_t addr, uint64_t key, void *context,
			   uint64_t flags)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	struct psmx3_am_request *req;
	psm2_amarg_t args[8];
	int chunk_size;
	size_t offset = 0;
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	size_t req_refcnt = 0;
	int err;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_read(ep, buf, len, desc, src_addr,
						addr, key, context, flags);

	assert(buf);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->tx, src_addr, av->type);
	psm3_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (!psm3_epid_cmp(psm2_epid, ep_priv->tx->psm2_epid))
		return psmx3_rma_self(PSMX3_AM_REQ_READ, ep_priv,
				      buf, len, desc, addr, key,
				      context, flags, 0);

	req = psmx3_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	req->op = PSMX3_AM_REQ_READ;
	req->read.buf = buf;
	req->read.len = len;
	req->read.addr = addr;	/* needed? */
	req->read.key = key; 	/* needed? */
	req->read.context = context;
	req->ep = ep_priv;
	req->cq_flags = FI_READ | FI_RMA;
	PSMX3_CTXT_TYPE(&req->fi_context) = PSMX3_READ_CONTEXT;
	PSMX3_CTXT_USER(&req->fi_context) = context;
	PSMX3_CTXT_EP(&req->fi_context) = ep_priv;

	if (ep_priv->send_selective_completion && !(flags & FI_COMPLETION)) {
		PSMX3_CTXT_TYPE(&req->fi_context) = PSMX3_NOCOMP_READ_CONTEXT;
		req->no_event = 1;
	}

	chunk_size = ep_priv->tx->psm2_am_param.max_reply_short;

	args[0].u32w0 = 0;

	if (psmx3_env.tagged_rma && len > chunk_size) {
		PSMX3_SET_TAG(psm2_tag, (uint64_t)req, 0, PSMX3_RMA_TYPE_READ);
		PSMX3_SET_MASK(psm2_tagsel, PSMX3_MATCH_ALL, PSMX3_RMA_TYPE_MASK);
		err = psm3_mq_irecv2(ep_priv->tx->psm2_mq, psm2_epaddr,
				     &psm2_tag, &psm2_tagsel, 0, buf, len,
				     (void *)&req->fi_context, &psm2_req);
		if (err) {
			psmx3_am_request_free(ep_priv->tx, req);
			return psmx3_errno(err);
		}

		PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_READ_LONG);
		args[0].u32w1 = len;
		args[1].u64 = (uint64_t)req;
		args[2].u64 = addr;
		args[3].u64 = key;
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, 4, NULL, 0, 0, NULL, NULL);
		if (err) {
			/* req in use, don't free */
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
		return 0;
	}

	PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_READ);
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[3].u64 = key;
	while (len > chunk_size) {
		args[0].u32w1 = chunk_size;
		args[2].u64 = addr;
		args[4].u64 = offset;
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, 5, NULL, 0, 0, NULL, NULL);
		if (err) {
			if (!req_refcnt)
				psmx3_am_request_free(ep_priv->tx, req);
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
		addr += chunk_size;
		len -= chunk_size;
		offset += chunk_size;
		req_refcnt++;
	}

	PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_EOM);
	args[0].u32w1 = len;
	args[2].u64 = addr;
	args[4].u64 = offset;
	err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
				    args, 5, NULL, 0, 0, NULL, NULL);
	if (err) {
		if (!req_refcnt)
			psmx3_am_request_free(ep_priv->tx, req);
		return psmx3_errno(err);
	}
	psmx3_am_poll(ep_priv->tx);
	return 0;
}

ssize_t psmx3_readv_generic(struct fid_ep *ep, const struct iovec *iov,
			    void *desc, size_t count, fi_addr_t src_addr,
			    uint64_t addr, uint64_t key, void *context,
			    uint64_t flags)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	struct psmx3_am_request *req;
	psm2_amarg_t args[8];
	int chunk_size;
	size_t offset = 0;
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag, psm2_tagsel;
	size_t total_len, long_len = 0, short_len;
	void *long_buf = NULL;
	int i;
	size_t req_refcnt = 0;
	int err;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_readv(ep, iov, desc, count, src_addr,
						 addr, key, context, flags);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->tx, src_addr, av->type);
	psm3_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (!psm3_epid_cmp(psm2_epid, ep_priv->tx->psm2_epid))
		return psmx3_rma_self(PSMX3_AM_REQ_READV, ep_priv,
				      (void *)iov, count, desc, addr,
				      key, context, flags, 0);

	total_len = 0;
	for (i=0; i<count; i++)
		total_len += iov[i].iov_len;

	req = psmx3_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	req->tmpbuf = malloc(count * sizeof(struct iovec));
	if (!req->tmpbuf) {
		psmx3_am_request_free(ep_priv->tx, req);
		return -FI_ENOMEM;
	}

	req->iov = req->tmpbuf;
	memcpy(req->iov, iov, count * sizeof(struct iovec));

	req->op = PSMX3_AM_REQ_READV;
	req->read.iov_count = count;
	req->read.len = total_len;
	req->read.addr = addr;	/* needed? */
	req->read.key = key; 	/* needed? */
	req->read.context = context;
	req->ep = ep_priv;
	req->cq_flags = FI_READ | FI_RMA;
	PSMX3_CTXT_TYPE(&req->fi_context) = PSMX3_READ_CONTEXT;
	PSMX3_CTXT_USER(&req->fi_context) = context;
	PSMX3_CTXT_EP(&req->fi_context) = ep_priv;

	if (ep_priv->send_selective_completion && !(flags & FI_COMPLETION)) {
		PSMX3_CTXT_TYPE(&req->fi_context) = PSMX3_NOCOMP_READ_CONTEXT;
		req->no_event = 1;
	}

	chunk_size = ep_priv->tx->psm2_am_param.max_reply_short;

	if (psmx3_env.tagged_rma) {
		for (i=count-1; i>=0; i--) {
			if (iov[i].iov_len > chunk_size) {
				long_buf = iov[i].iov_base;
				long_len = iov[i].iov_len;
				break;
			} else if (iov[i].iov_len) {
				break;
			}
		}
	}

	short_len = total_len - long_len;

	/* Use short protocol for all but the last segment (long_len) */
	args[0].u32w0 = 0;
	PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_READ);
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[3].u64 = key;
	while (short_len > chunk_size) {
		args[0].u32w1 = chunk_size;
		args[2].u64 = addr;
		args[4].u64 = offset;
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, 5, NULL, 0, 0, NULL, NULL);
		if (err) {
			if (!req_refcnt) {
				free(req->tmpbuf);
				psmx3_am_request_free(ep_priv->tx, req);
			}
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
		addr += chunk_size;
		short_len -= chunk_size;
		offset += chunk_size;
		req_refcnt++;
	}

	if (short_len) {
		if (!long_len)
			PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_EOM);
		args[0].u32w1 = short_len;
		args[2].u64 = addr;
		args[4].u64 = offset;
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, 5, NULL, 0, 0, NULL, NULL);
		if (err) {
			if (!req_refcnt) {
				free(req->tmpbuf);
				psmx3_am_request_free(ep_priv->tx, req);
			}
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
		req_refcnt++;
	}

	/* Use the long protocol for the last segment */
	if (long_len) {
		PSMX3_SET_TAG(psm2_tag, (uint64_t)req, 0, PSMX3_RMA_TYPE_READ);
		PSMX3_SET_MASK(psm2_tagsel, PSMX3_MATCH_ALL, PSMX3_RMA_TYPE_MASK);
		err = psm3_mq_irecv2(ep_priv->tx->psm2_mq, psm2_epaddr,
				     &psm2_tag, &psm2_tagsel, 0,
				     long_buf, long_len,
				     (void *)&req->fi_context, &psm2_req);
		if (err) {
			if (!req_refcnt) {
				free(req->tmpbuf);
				psmx3_am_request_free(ep_priv->tx, req);
			}
			return psmx3_errno(err);
		}

		PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_READ_LONG);
		args[0].u32w1 = long_len;
		args[1].u64 = (uint64_t)req;
		args[2].u64 = addr + short_len;
		args[3].u64 = key;
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, 4, NULL, 0, 0, NULL, NULL);
		if (err) {
			/* req in use, don't free */
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
	}

	return 0;
}

DIRECT_FN
STATIC ssize_t psmx3_read(struct fid_ep *ep, void *buf, size_t len,
			  void *desc, fi_addr_t src_addr,
			  uint64_t addr, uint64_t key, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_read_generic(ep, buf, len, desc, src_addr, addr,
				  key, context, ep_priv->tx_flags);
}

DIRECT_FN
STATIC ssize_t psmx3_readmsg(struct fid_ep *ep,
			     const struct fi_msg_rma *msg,
			     uint64_t flags)
{
	assert(msg);
	assert(msg->iov_count);
	assert(msg->msg_iov);
	assert(msg->rma_iov);
	assert(msg->rma_iov_count == 1);

	if (msg->iov_count > 1)
		return psmx3_readv_generic(ep, msg->msg_iov,
					   msg->desc ? msg->desc[0] : NULL,
					   msg->iov_count, msg->addr,
					   msg->rma_iov[0].addr,
					   msg->rma_iov[0].key,
					   msg->context, flags);

	return psmx3_read_generic(ep, msg->msg_iov[0].iov_base,
				  msg->msg_iov[0].iov_len,
				  msg->desc ? msg->desc[0] : NULL,
				  msg->addr, msg->rma_iov[0].addr,
				  msg->rma_iov[0].key, msg->context,
				  flags);
}

DIRECT_FN
STATIC ssize_t psmx3_readv(struct fid_ep *ep, const struct iovec *iov,
			   void **desc, size_t count, fi_addr_t src_addr,
			   uint64_t addr, uint64_t key, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	assert(iov);
	assert(count);

	if (count > 1)
		return psmx3_readv_generic(ep, iov, desc ? desc[0] : NULL,
					   count, src_addr, addr, key,
					   context, ep_priv->tx_flags);

	return psmx3_read(ep, iov->iov_base, iov->iov_len,
			  desc ? desc[0] : NULL, src_addr, addr, key, context);
}

ssize_t psmx3_write_generic(struct fid_ep *ep, const void *buf, size_t len,
			    void *desc, fi_addr_t dest_addr,
			    uint64_t addr, uint64_t key, void *context,
			    uint64_t flags, uint64_t data)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	struct psmx3_am_request *req;
	psm2_amarg_t args[8];
	int nargs;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size;
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	void *psm2_context;
	int no_event;
	size_t req_refcnt = 0;
	int err;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_write(ep, buf, len, desc, dest_addr,
						 addr, key, context, flags,
						 data);

	assert(buf);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm3_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (!psm3_epid_cmp(psm2_epid, ep_priv->tx->psm2_epid))
		return psmx3_rma_self(PSMX3_AM_REQ_WRITE, ep_priv,
				      (void *)buf, len, desc, addr,
				      key, context, flags, data);

	no_event = (flags & PSMX3_NO_COMPLETION) ||
		   (ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	req = psmx3_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	if (flags & FI_INJECT) {
		if (len > psmx3_env.inject_size) {
			psmx3_am_request_free(ep_priv->tx, req);
			return -FI_EMSGSIZE;
		}

		req->tmpbuf = malloc(len);
		if (!req->tmpbuf) {
			psmx3_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}

		memcpy(req->tmpbuf, (void *)buf, len);
		buf = req->tmpbuf;
	} else {
		PSMX3_CTXT_TYPE(&req->fi_context) = no_event ?
						    PSMX3_NOCOMP_WRITE_CONTEXT :
						    PSMX3_WRITE_CONTEXT;
	}

	req->no_event = no_event;
	req->op = PSMX3_AM_REQ_WRITE;
	req->write.buf = (void *)buf;
	req->write.len = len;
	req->write.addr = addr;	/* needed? */
	req->write.key = key; 	/* needed? */
	req->write.context = context;
	req->ep = ep_priv;
	req->cq_flags = FI_WRITE | FI_RMA;
	PSMX3_CTXT_USER(&req->fi_context) = context;
	PSMX3_CTXT_EP(&req->fi_context) = ep_priv;

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;

	args[0].u32w0 = 0;

	if (psmx3_env.tagged_rma && len > chunk_size) {
		PSMX3_SET_TAG(psm2_tag, (uint64_t)req, 0, PSMX3_RMA_TYPE_WRITE);
		PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_WRITE_LONG);
		args[0].u32w1 = len;
		args[1].u64 = (uint64_t)req;
		args[2].u64 = addr;
		args[3].u64 = key;
		nargs = 4;
		if (flags & FI_REMOTE_CQ_DATA) {
			PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_DATA);
			args[4].u64 = data;
			nargs++;
		}

		if (flags & FI_DELIVERY_COMPLETE) {
			args[0].u32w0 |= PSMX3_AM_FORCE_ACK;
			psm2_context = NULL;
		} else {
			psm2_context = (void *)&req->fi_context;
		}

		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, nargs, NULL, 0, am_flags,
					    NULL, NULL);
		if (err) {
			free(req->tmpbuf);
			psmx3_am_request_free(ep_priv->tx, req);
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);

		err = psm3_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr, 0,
				     &psm2_tag, buf, len, psm2_context, &psm2_req);
		if (err) {
			/* req in use, don't free */
			return psmx3_errno(err);
		}
		return 0;
	}

	PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_WRITE);
	nargs = 4;
	while (len > chunk_size) {
		args[0].u32w1 = chunk_size;
		args[1].u64 = (uint64_t)(uintptr_t)req;
		args[2].u64 = addr;
		args[3].u64 = key;
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, nargs, (void *)buf,
					    chunk_size, am_flags, NULL, NULL);
		if (err) {
			if (!req_refcnt) {
				free(req->tmpbuf);
				psmx3_am_request_free(ep_priv->tx, req);
			}
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
		buf = (const uint8_t *)buf + chunk_size;
		addr += chunk_size;
		len -= chunk_size;
		req_refcnt++;
	}

	args[0].u32w1 = len;
	args[1].u64 = (uint64_t)(uintptr_t)req;
	args[2].u64 = addr;
	args[3].u64 = key;
	if (flags & FI_REMOTE_CQ_DATA) {
		PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_DATA | PSMX3_AM_EOM);
		args[4].u64 = data;
		nargs++;
	} else {
		PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_EOM);
	}
	err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
				    args, nargs, (void *)buf, len, am_flags,
				    NULL, NULL);
	if (err) {
		if (!req_refcnt) {
			free(req->tmpbuf);
			psmx3_am_request_free(ep_priv->tx, req);
		}
		return psmx3_errno(err);
	}
	psmx3_am_poll(ep_priv->tx);
	return 0;
}

ssize_t psmx3_writev_generic(struct fid_ep *ep, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t dest_addr,
			     uint64_t addr, uint64_t key, void *context,
			     uint64_t flags, uint64_t data)
{
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_av *av;
	struct psmx3_am_request *req;
	psm2_amarg_t args[8];
	int nargs;
	int am_flags = PSM2_AM_FLAG_ASYNC;
	int chunk_size;
	psm2_epaddr_t psm2_epaddr;
	psm2_epid_t psm2_epid;
	psm2_mq_req_t psm2_req;
	psm2_mq_tag_t psm2_tag;
	void *psm2_context;
	int no_event;
	size_t total_len, len, len_sent;
	uint8_t *buf, *p;
	int i;
	size_t req_refcnt = 0;
	int err;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	if (flags & FI_TRIGGER)
		return psmx3_trigger_queue_writev(ep, iov, desc, count,
						  dest_addr, addr, key,
						  context, flags, data);

	av = ep_priv->av;
	assert(av);

	psm2_epaddr = psmx3_av_translate_addr(av, ep_priv->tx, dest_addr, av->type);
	psm3_epaddr_to_epid(psm2_epaddr, &psm2_epid);

	if (!psm3_epid_cmp(psm2_epid, ep_priv->tx->psm2_epid))
		return psmx3_rma_self(PSMX3_AM_REQ_WRITEV, ep_priv,
				      (void *)iov, count, desc, addr,
				      key, context, flags, data);

	no_event = (flags & PSMX3_NO_COMPLETION) ||
		   (ep_priv->send_selective_completion && !(flags & FI_COMPLETION));

	total_len = 0;
	for (i=0; i<count; i++)
		total_len += iov[i].iov_len;

	chunk_size = ep_priv->tx->psm2_am_param.max_request_short;

	req = psmx3_am_request_alloc(ep_priv->tx);
	if (!req)
		return -FI_ENOMEM;

	/* Case 1: fit into a AM message, then pack and send */
	if (total_len <= chunk_size) {
		req->tmpbuf = malloc(total_len);
		if (!req->tmpbuf) {
			psmx3_am_request_free(ep_priv->tx, req);
			return -FI_ENOMEM;
		}

		p = req->tmpbuf;
		for (i=0; i<count; i++) {
			if (iov[i].iov_len) {
				memcpy(p, iov[i].iov_base, iov[i].iov_len);
				p += iov[i].iov_len;
			}
		}
		buf = req->tmpbuf;
		len = total_len;

		req->no_event = no_event;
		req->op = PSMX3_AM_REQ_WRITE;
		req->write.buf = (void *)buf;
		req->write.len = len;
		req->write.addr = addr;	/* needed? */
		req->write.key = key; 	/* needed? */
		req->write.context = context;
		req->ep = ep_priv;
		req->cq_flags = FI_WRITE | FI_RMA;
		PSMX3_CTXT_USER(&req->fi_context) = context;
		PSMX3_CTXT_EP(&req->fi_context) = ep_priv;

		args[0].u32w0 = 0;
		PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_WRITE);
		args[0].u32w1 = len;
		args[1].u64 = (uint64_t)(uintptr_t)req;
		args[2].u64 = addr;
		args[3].u64 = key;
		nargs = 4;
		if (flags & FI_REMOTE_CQ_DATA) {
			PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_DATA | PSMX3_AM_EOM);
			args[4].u64 = data;
			nargs++;
		} else {
			PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_EOM);
		}
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, nargs, (void *)buf, len,
					    am_flags, NULL, NULL);
		if (err) {
			free(req->tmpbuf);
			psmx3_am_request_free(ep_priv->tx, req);
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);
		return 0;
	}

	if (flags & FI_INJECT) {
		psmx3_am_request_free(ep_priv->tx, req);
		return -FI_EMSGSIZE;
	}

	PSMX3_CTXT_TYPE(&req->fi_context) = no_event ?
					    PSMX3_NOCOMP_WRITE_CONTEXT :
					    PSMX3_WRITE_CONTEXT;

	req->no_event = no_event;
	req->op = PSMX3_AM_REQ_WRITE;
	req->write.buf = (void *)iov[0].iov_base;
	req->write.len = total_len;
	req->write.addr = addr;	/* needed? */
	req->write.key = key; 	/* needed? */
	req->write.context = context;
	req->ep = ep_priv;
	req->cq_flags = FI_WRITE | FI_RMA;
	PSMX3_CTXT_USER(&req->fi_context) = context;
	PSMX3_CTXT_EP(&req->fi_context) = ep_priv;

	/* Case 2: send iov in sequence */
	args[0].u32w0 = 0;

	len_sent = 0;
	for (i=0; i<count; i++) {
		if (!iov[i].iov_len)
			continue;

		/* Case 2.1: use long protocol for the last segment if it is large */
		if (psmx3_env.tagged_rma && iov[i].iov_len > chunk_size &&
		    len_sent + iov[i].iov_len == total_len) {
			PSMX3_SET_TAG(psm2_tag, (uint64_t)req, 0, PSMX3_RMA_TYPE_WRITE);
			PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_WRITE_LONG);
			args[0].u32w1 = iov[i].iov_len;
			args[1].u64 = (uint64_t)req;
			args[2].u64 = addr;
			args[3].u64 = key;
			nargs = 4;
			if (flags & FI_REMOTE_CQ_DATA) {
				PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_DATA);
				args[4].u64 = data;
				nargs++;
			}

			if (flags & FI_DELIVERY_COMPLETE) {
				args[0].u32w0 |= PSMX3_AM_FORCE_ACK;
				psm2_context = NULL;
			} else {
				psm2_context = (void *)&req->fi_context;
			}

			err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
						    args, nargs, NULL, 0, am_flags,
						    NULL, NULL);
			if (err) {
				if (!req_refcnt)
					psmx3_am_request_free(ep_priv->tx, req);
				return psmx3_errno(err);
			}
			psmx3_am_poll(ep_priv->tx);

			err = psm3_mq_isend2(ep_priv->tx->psm2_mq, psm2_epaddr,
					     0, &psm2_tag, iov[i].iov_base,
					     iov[i].iov_len, psm2_context,
					     &psm2_req);
			if (err) {
				/* req in use, don't free */
				return psmx3_errno(err);
			}
			return 0;
		}

		/* Case 2.2: use short protocol all other segments */
		PSMX3_AM_SET_OP(args[0].u32w0, PSMX3_AM_REQ_WRITE);
		nargs = 4;
		buf = iov[i].iov_base;
		len = iov[i].iov_len;
		while (len > chunk_size) {
			args[0].u32w1 = chunk_size;
			args[1].u64 = (uint64_t)(uintptr_t)req;
			args[2].u64 = addr;
			args[3].u64 = key;
			err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
						    args, nargs, (void *)buf,
						    chunk_size, am_flags,
						    NULL, NULL);
			if (err) {
				if (!req_refcnt)
					psmx3_am_request_free(ep_priv->tx, req);
				return psmx3_errno(err);
			}
			psmx3_am_poll(ep_priv->tx);
			buf += chunk_size;
			addr += chunk_size;
			len -= chunk_size;
			len_sent += chunk_size;
			req_refcnt++;
		}

		args[0].u32w1 = len;
		args[1].u64 = (uint64_t)(uintptr_t)req;
		args[2].u64 = addr;
		args[3].u64 = key;
		if (len_sent + len == total_len) {
			if (flags & FI_REMOTE_CQ_DATA) {
				PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_DATA | PSMX3_AM_EOM);
				args[4].u64 = data;
				nargs++;
			} else {
				PSMX3_AM_SET_FLAG(args[0].u32w0, PSMX3_AM_EOM);
			}
		}
		err = psm3_am_request_short(psm2_epaddr, PSMX3_AM_RMA_HANDLER,
					    args, nargs, (void *)buf, len,
					    am_flags, NULL, NULL);
		if (err) {
			if (!req_refcnt)
				psmx3_am_request_free(ep_priv->tx, req);
			return psmx3_errno(err);
		}
		psmx3_am_poll(ep_priv->tx);

		addr += len;
		len_sent += len;
		req_refcnt++;
	}

	return 0;
}

DIRECT_FN
STATIC ssize_t psmx3_write(struct fid_ep *ep, const void *buf, size_t len,
			   void *desc, fi_addr_t dest_addr, uint64_t addr,
			   uint64_t key, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_write_generic(ep, buf, len, desc, dest_addr, addr,
				   key, context, ep_priv->tx_flags, 0);
}

DIRECT_FN
STATIC ssize_t psmx3_writemsg(struct fid_ep *ep,
			      const struct fi_msg_rma *msg,
			      uint64_t flags)
{
	assert(msg);
	assert(msg->msg_iov);
	assert(msg->iov_count);
	assert(msg->rma_iov);
	assert(msg->rma_iov_count == 1);

	if (msg->iov_count > 1)
		return psmx3_writev_generic(ep, msg->msg_iov, msg->desc,
					    msg->iov_count, msg->addr,
					    msg->rma_iov[0].addr,
					    msg->rma_iov[0].key,
					    msg->context, flags, msg->data);

	return psmx3_write_generic(ep, msg->msg_iov[0].iov_base,
				   msg->msg_iov[0].iov_len,
				   msg->desc ? msg->desc[0] : NULL, msg->addr,
				   msg->rma_iov[0].addr, msg->rma_iov[0].key,
				   msg->context, flags, msg->data);
}

DIRECT_FN
STATIC ssize_t psmx3_writev(struct fid_ep *ep, const struct iovec *iov,
			    void **desc, size_t count, fi_addr_t dest_addr,
			    uint64_t addr, uint64_t key, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	assert(iov);
	assert(count);

	if (count > 1)
		return psmx3_writev_generic(ep, iov, desc, count, dest_addr,
					    addr, key, context, ep_priv->tx_flags, 0);

	return psmx3_write_generic(ep, iov->iov_base, iov->iov_len,
				   desc ? desc[0] : NULL, dest_addr, addr, key,
				   context, ep_priv->tx_flags, 0);
}

DIRECT_FN
STATIC ssize_t psmx3_inject_write(struct fid_ep *ep, const void *buf, size_t len,
			          fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_write_generic(ep, buf, len, NULL, dest_addr, addr, key, NULL,
				   ep_priv->tx_flags | FI_INJECT | PSMX3_NO_COMPLETION,
				   0);
}

DIRECT_FN
STATIC ssize_t psmx3_writedata(struct fid_ep *ep, const void *buf, size_t len,
			       void *desc, uint64_t data, fi_addr_t dest_addr,
			       uint64_t addr, uint64_t key, void *context)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_write_generic(ep, buf, len, desc, dest_addr, addr, key,
				   context, ep_priv->tx_flags | FI_REMOTE_CQ_DATA,
				   data);
}

DIRECT_FN
STATIC ssize_t psmx3_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
				      uint64_t data, fi_addr_t dest_addr, uint64_t addr,
				      uint64_t key)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);

	return psmx3_write_generic(ep, buf, len, NULL, dest_addr, addr, key, NULL,
				   ep_priv->tx_flags | FI_INJECT | PSMX3_NO_COMPLETION,
				   data);
}

struct fi_ops_rma psmx3_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = psmx3_read,
	.readv = psmx3_readv,
	.readmsg = psmx3_readmsg,
	.write = psmx3_write,
	.writev = psmx3_writev,
	.writemsg = psmx3_writemsg,
	.inject = psmx3_inject_write,
	.writedata = psmx3_writedata,
	.injectdata = psmx3_inject_writedata,
};

