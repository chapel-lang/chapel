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

struct ucx_claimed_msg {
	struct dlist_entry dentry;
	uint64_t tag;
	ucp_tag_message_h ucp_msg;
};

static inline
struct ucx_claimed_msg *ucx_dequeue_claimed(struct ucx_ep *u_ep,
					    const struct fi_msg_tagged *msg)
{
	struct dlist_entry *dentry = NULL;
	struct ucx_claimed_msg *cmsg;

	dlist_foreach(&u_ep->claimed_list, dentry) {
		cmsg = (struct ucx_claimed_msg *) dentry;
		if ((cmsg->tag & ~msg->ignore) == (msg->tag & ~msg->ignore)) {
			dlist_remove(&cmsg->dentry);
			return cmsg;
		}
	}
	return NULL;
}

static inline
void ucx_enqueue_claimed(struct ucx_ep *u_ep,
			 ucp_tag_recv_info_t *recv_info,
			 ucp_tag_message_h probed_msg)
{
	struct ucx_claimed_msg *cmsg;

	cmsg = malloc(sizeof(struct ucx_claimed_msg));
	dlist_init(&cmsg->dentry);
	cmsg->tag = recv_info->sender_tag;
	cmsg->ucp_msg = probed_msg;
	dlist_insert_tail(&cmsg->dentry, &u_ep->claimed_list);
}

static inline ssize_t ucx_gen_mrecv_completion(struct ucx_mrecv_request *req)
{
	struct ucx_mrecv_ctx *mctx = req->mrecv_ctx;
	struct util_cq *cq = req->cq;
	ssize_t ret = -FI_EAGAIN;

	if (req->status != UCS_OK) {
		ucx_write_error_completion(cq, mctx->context,
					   FI_RECV | FI_MSG, (int)req->status,
					   -ucx_translate_errcode(req->status),
					   (req->last_recvd - mctx->remain),
					   mctx->tag);
		ret = -ucx_translate_errcode(req->status);
	} else {
		uint64_t flags = FI_RECV | FI_MSG;
		int buff_is_full = (req->ep->ep_opts.mrecv_min_size >
					mctx->remain - req->last_recvd);

		if (buff_is_full)
			flags |= FI_MULTI_RECV;
		ofi_cq_write(cq, mctx->context, flags,  req->last_recvd,
			     mctx->head, 0, 0);
		ofi_ep_cntr_inc(&req->ep->ep, CNTR_RX);
		if (buff_is_full)
			ret = FI_SUCCESS;
	}

	if (cq->wait)
		cq->wait->signal(cq->wait);

	return ret;
}

static inline ssize_t ucx_do_inject(struct fid_ep *ep, const void *buf,
				    size_t len, fi_addr_t dest_addr,
				    uint64_t tag)
{
	struct ucx_ep *u_ep;
	ucp_ep_h dst_ep;
	ucs_status_ptr_t status = NULL;
	ucs_status_t ret;

	u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	dst_ep = UCX_GET_UCP_EP(u_ep, dest_addr);

	status = ucp_tag_send_nb(dst_ep, buf, len,
				 ucp_dt_make_contig(1),
				 tag, ucx_send_callback_no_compl);

	if (status == NULL) {
		ofi_ep_cntr_inc(&u_ep->ep, CNTR_TX);
		return FI_SUCCESS;
	}

	if (UCS_PTR_IS_ERR(status)) {
		FI_DBG(&ucx_prov,FI_LOG_CORE,
		       "Send operation returns error: %s",
		       ucs_status_string(*(ucs_status_t*)status));
		return ucx_translate_errcode(*(ucs_status_t*) status);
	}

	while ((ret = ucp_request_check_status(status)) == UCS_INPROGRESS)
		ucp_worker_progress(u_ep->worker);

	return -ucx_translate_errcode(ret);
}

ssize_t ucx_do_sendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		       uint64_t flags, const enum ucx_comm_mode mode);

ssize_t ucx_do_recvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
		       const uint64_t flags, const enum ucx_comm_mode mode);

