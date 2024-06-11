/*
 * Copyright (c) 2019-2023 Intel Corporation. All rights reserved.
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

ssize_t ucx_do_sendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		       uint64_t flags, const enum ucx_comm_mode mode)
{
	struct ucx_ep* u_ep;
	ucp_send_callback_t cbf;
	ucp_ep_h dst_ep;
	ucs_status_ptr_t status = NULL;
	struct util_cq *cq;
	ucs_status_t cstatus = UCS_OK;
	int no_completion;

	u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	dst_ep = UCX_GET_UCP_EP(u_ep, msg->addr);
	cq = u_ep->ep.tx_cq;

	no_completion = (u_ep->ep.tx_op_flags & FI_SELECTIVE_COMPLETION) &&
			!(flags & FI_COMPLETION);

	cbf = no_completion ? ucx_send_callback_no_compl : ucx_send_callback;

	if (OFI_UNLIKELY(flags & FI_MATCH_COMPLETE)) {
		if (msg->iov_count < 2) {
			status = ucp_tag_send_sync_nb(
					dst_ep,
					msg->msg_iov[0].iov_base,
					msg->msg_iov[0].iov_len,
					ucp_dt_make_contig(1),
					msg->tag, cbf);
		} else {
			status = ucp_tag_send_sync_nb(
					dst_ep,
					msg->msg_iov,
					msg->iov_count,
					ucp_dt_make_iov(),
					msg->tag, cbf);
		}
	} else {
		if (msg->iov_count < 2) {
			status = ucp_tag_send_nb(
					dst_ep,
					msg->msg_iov[0].iov_base,
					msg->msg_iov[0].iov_len,
					ucp_dt_make_contig(1),
					msg->tag, cbf);
		} else {
			status = ucp_tag_send_nb(
					dst_ep,
					msg->msg_iov,
					msg->iov_count,
					ucp_dt_make_iov(),
					msg->tag, cbf);
		}
	}

	if (UCS_PTR_IS_ERR(status)) {
		FI_DBG(&ucx_prov,FI_LOG_CORE,
		       "Send operation returns error: %s",
		       ucs_status_string(*(ucs_status_t*)status));
		return ucx_translate_errcode(*(ucs_status_t*)status);
	}

	if (flags & FI_INJECT) {
		if(UCS_PTR_STATUS(status) != UCS_OK) {
			while ((cstatus = ucp_request_check_status(status))
					== UCS_INPROGRESS)
				ucp_worker_progress(u_ep->worker);

			/*
			 * The callback function should have already taken
			 * care of cntr and cq update.
			 */
			goto fence;
		}

		goto done;
	}

	if (no_completion) {
		if (UCS_PTR_STATUS(status) != UCS_OK)
			goto fence;

		goto done;
	}

	if (msg->context) {
		struct fi_context *ctx = ((struct fi_context*)(msg->context));

		ctx->internal[0] = status;
		ctx->internal[1] = NULL;
	}

	if (UCS_PTR_STATUS(status) != UCS_OK) {
		/*
		 * Not done yet. completion will be handled by the callback
		 * function.
		 */
		struct ucx_request *req = (struct ucx_request *)status;

		req->completion.op_context = msg->context;
		req->completion.flags = FI_SEND |
					(mode == UCX_MSG ? FI_MSG : FI_TAGGED);
		req->completion.len = msg->msg_iov[0].iov_len;
		req->completion.buf = msg->msg_iov[0].iov_base;
		req->completion.tag = msg->tag;
		req->ep = u_ep;
		req->cq = cq;
		goto fence;
	}

done:
	if (!no_completion) {
		ofi_cq_write(cq,  msg->context,
			     FI_SEND | ((mode == UCX_MSG) ? FI_MSG : FI_TAGGED),
			     msg->msg_iov[0].iov_len, msg->msg_iov[0].iov_base,
			     0, msg->tag);
	}

	ofi_ep_cntr_inc(&u_ep->ep, CNTR_TX);

fence:
	if(flags & (FI_FENCE | FI_TRANSMIT_COMPLETE))
		cstatus = ucp_worker_flush(u_ep->worker);

	return ucx_translate_errcode(cstatus);
}

ssize_t ucx_do_recvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		       const uint64_t flags, const enum ucx_comm_mode mode)
{
	ucs_status_ptr_t status = NULL;
	ucp_tag_recv_callback_t cbf;
	struct ucx_ep *u_ep;
	struct ucx_request *req;
	struct util_cq *cq;
	ucp_datatype_t recv_dt;
	size_t recv_cnt;
	void *recv_buf;
	int ret = FI_SUCCESS;
	ssize_t posted_size = 0;
	struct fi_cq_tagged_entry *tc;
	int completion;
	int claim_discard = (flags & FI_CLAIM) && (flags & FI_DISCARD);
	int i;

	u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	cq = u_ep->ep.rx_cq;

	if (claim_discard) {
		recv_dt = ucp_dt_make_contig(1);
		recv_buf = NULL;
		recv_cnt = 0;
		posted_size = recv_cnt;
	} else if (msg->iov_count < 2) {
		recv_dt =  ucp_dt_make_contig(1);
		recv_buf = msg->msg_iov[0].iov_base;
		recv_cnt = msg->msg_iov[0].iov_len;
		posted_size = recv_cnt;
	} else {
		recv_dt = ucp_dt_make_iov();
		recv_buf = (void*) msg->msg_iov;
		recv_cnt = msg->iov_count;
		for (i=0; i < msg->iov_count; ++i )
			posted_size += msg->msg_iov[i].iov_len;
	}

	completion = !(u_ep->ep.rx_op_flags & FI_SELECTIVE_COMPLETION) ||
		     (flags & FI_COMPLETION);

	if (flags & FI_CLAIM) {
		struct ucx_claimed_msg *cmsg = ucx_dequeue_claimed(u_ep, msg);

		if (!cmsg)
			return -FI_EINVAL;

		cbf = completion || claim_discard ?
				ucx_recv_callback : ucx_recv_callback_no_compl;
		status = ucp_tag_msg_recv_nb(u_ep->worker, recv_buf, recv_cnt,
					     recv_dt, cmsg->ucp_msg, cbf);
		free(cmsg);
	} else {
		cbf = completion ?
				ucx_recv_callback : ucx_recv_callback_no_compl;
		status = ucp_tag_recv_nb(u_ep->worker, recv_buf, recv_cnt,
					 recv_dt, msg->tag, ~msg->ignore, cbf);
	}

	if (UCS_PTR_IS_ERR(status)) {
		FI_DBG(&ucx_prov,FI_LOG_CORE,
		       "Recv operation returns error: %s",
		       ucs_status_string(*(ucs_status_t*) status));
		return ucx_translate_errcode(*(ucs_status_t *) status);
	}

	req = (struct ucx_request *) status;
	req->cq = cq;
	req->ep = u_ep;

	if (msg->context) {
		struct fi_context *ctx = ((struct fi_context *)(msg->context));
		ctx->internal[0] = (void*)req;
		ctx->internal[1] = NULL;
	}

	req->completion.op_context = msg->context;
	req->completion.flags = FI_RECV |
				(mode == UCX_MSG ? FI_MSG : FI_TAGGED);
	req->completion.buf = claim_discard ? NULL : msg->msg_iov[0].iov_base;
	req->completion.data = 0;
	req->posted_size = posted_size;

	/*
	 * Nothing has arrived yet. The callback function will handle the
	 * completion.
	 */
	if (req->type == UCX_REQ_UNSPEC) {
		assert(!claim_discard);

		req->type = UCX_REQ_REGULAR;
		req->completion.tag = msg->tag;
		req->completion.len = msg->msg_iov[0].iov_len;
		return FI_SUCCESS;
	}

	/*
	 * Already done (matched an unexpected message). req->type is set
	 * by the callback function.
	 */
	tc = &req->completion;
	if (req->type == UCX_REQ_UNSPEC &&
	    !(claim_discard &&
	      ucx_translate_errcode((int)req->status) == -FI_ETRUNC)) {
		ret = ucx_write_error_completion(cq, tc->op_context,
						 tc->flags, req->status,
						 -ucx_translate_errcode(req->status),
						 (tc->len - req->posted_size),
						 tc->tag);
	} else {
		if (completion)
			ofi_cq_write(cq, tc->op_context, tc->flags, tc->len, tc->buf,
				     0, tc->tag);

		ofi_ep_cntr_inc(&u_ep->ep, CNTR_RX);
	}

	ucx_req_release(req);
	return ret;
}
