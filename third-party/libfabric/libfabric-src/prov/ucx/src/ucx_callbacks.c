/*
 * Copyright (c) 2016-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#include "ucx.h"
#include "ucx_core.h"

void ucx_send_callback_no_compl(void *request, ucs_status_t status)
{
	struct ucx_request *ucx_req = request;

	if (ucx_req->ep)
		ofi_ep_cntr_inc(&ucx_req->ep->ep, CNTR_TX);

	ucx_req_release(request);
}

void ucx_send_callback(void *request, ucs_status_t status)
{
	struct util_cq *cq;
	struct ucx_request *ucx_req = request;
	struct fi_cq_tagged_entry *tc = &ucx_req->completion;

	cq = ucx_req->cq;
	if (status != UCS_OK){
		ucx_write_error_completion(cq, tc->op_context, tc->flags,
					   (int) status,
					   -ucx_translate_errcode(status),
					   0, tc->tag);
	} else {
		ofi_cq_write(cq, tc->op_context, tc->flags, tc->len,
			     tc->buf, 0, tc->tag);
		ofi_ep_cntr_inc(&ucx_req->ep->ep, CNTR_TX);
	}
	ucx_req_release(request);
}

void ucx_recv_callback_no_compl(void *request, ucs_status_t status,
				ucp_tag_recv_info_t *info)
{
	struct ucx_request *ucx_req = request;

	if (ucx_req->ep)
		ofi_ep_cntr_inc(&ucx_req->ep->ep, CNTR_RX);

	ucx_req_release(request);
}

void ucx_recv_callback(void *request, ucs_status_t status,
		       ucp_tag_recv_info_t *info)
{
	struct util_cq *cq;
	struct ucx_request *ucx_req;
	struct fi_cq_tagged_entry *tc;

	ucx_req = (struct ucx_request*)request;
	ucx_req->completion.tag = info->sender_tag;
	ucx_req->completion.len = info->length;
	ucx_req->status = status;

	if (ucx_req->type == UCX_REQ_UNSPEC) {
		ucx_req->type = (status != UCS_OK) ?
					UCX_REQ_UNEXPECTED_ERR :
					UCX_REQ_UNEXPECTED;
		return;
	}

	cq = ucx_req->cq;
	tc = &ucx_req->completion;
	if (status != UCS_OK) {
		size_t olen = (info->length > ucx_req->posted_size) ?
					info->length - ucx_req->posted_size : 0;
		ucx_write_error_completion(cq, tc->op_context, tc->flags,
					   (int) status,
					   -ucx_translate_errcode(status),
					   olen, tc->tag);
	} else {
		ofi_cq_write(cq, tc->op_context, tc->flags, tc->len, tc->buf,
			     0, tc->tag);
		ofi_ep_cntr_inc(&ucx_req->ep->ep, CNTR_RX);
	}

	if (cq->wait)
		cq->wait->signal(cq->wait);

	ucx_req_release(request);
}

void ucx_multi_recv_callback(void *request, ucs_status_t ustatus,
			     ucp_tag_recv_info_t *info)
{
	struct ucx_mrecv_request *ucx_req;
	struct ucx_mrecv_ctx *mctx;
	struct ucx_ep* ep;
	ssize_t status = -FI_EAGAIN;

	ucx_req = (struct ucx_mrecv_request*) request;
	ucx_req->last_recvd = info->length;
	ucx_req->status = ustatus;
	if (ucx_req->type == UCX_REQ_UNSPEC) {
		ucx_req->type = UCX_REQ_MULTIRECV_UNEXP;
		return;
	}

	mctx = ucx_req->mrecv_ctx;
	ep = ucx_req->ep;

	if (!(ep->ep.rx_op_flags & FI_SELECTIVE_COMPLETION) ||
	    (mctx->flags & FI_COMPLETION) ||
	    (ucx_req->status != UCS_OK) ||
	    ((mctx->remain - ucx_req->last_recvd) < ep->ep_opts.mrecv_min_size))
		status = ucx_gen_mrecv_completion(ucx_req);

	mctx->head = (void*)((char*)(mctx->head) + info->length);
	mctx->remain -= info->length;
	ucx_req_release((struct ucx_request*) ucx_req);

	if (status == FI_SUCCESS)
		dlist_insert_head(&mctx->list, &ep->mctx_freelist);
	else
		dlist_insert_tail(&mctx->list, &ep->mctx_repost);
}

