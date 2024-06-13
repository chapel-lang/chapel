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

static ssize_t ucx_tagged_peek(struct fid_ep *ep,
			       const struct fi_msg_tagged *msg,
			       uint64_t flags)
{
	ssize_t retval = FI_SUCCESS;
	struct ucx_ep *u_ep;
	struct util_cq *cq;
	ucp_tag_message_h pmsg = NULL;
	ucp_tag_recv_info_t r_info ;
	int do_remove = ((flags & (FI_CLAIM | FI_DISCARD)) != 0);

	u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	cq = u_ep->ep.rx_cq;
	ucp_worker_progress(u_ep->worker);

	pmsg = ucp_tag_probe_nb(u_ep->worker, msg->tag, ~msg->ignore,
				do_remove, &r_info);
	if (pmsg == NULL) {
		retval = ofi_cq_write_error_peek(cq, msg->tag, msg->context);
	} else {
		retval = ofi_cq_write(cq, msg->context,
				      (FI_RECV | (flags & FI_CLAIM)),
				      r_info.length, NULL, 0,
				      r_info.sender_tag);
		if (flags & FI_CLAIM) {
			ucx_enqueue_claimed(u_ep, &r_info, pmsg);
		} else if (flags & FI_DISCARD) {
			char tmp;
			ucs_status_ptr_t status = NULL;

			status = ucp_tag_msg_recv_nb(u_ep->worker, &tmp, 1,
						     ucp_dt_make_contig(1),
						     pmsg,
						     ucx_recv_callback_no_compl);
			if (!UCS_PTR_IS_ERR(status)) {
				while (ucp_request_check_status(status) ==
				       UCS_INPROGRESS)
					ucp_worker_progress(u_ep->worker);
			}
		}
	}
	return retval;
}

static ssize_t ucx_tagged_recvmsg(struct fid_ep *ep,
				  const struct fi_msg_tagged *msg,
				  uint64_t flags)
{
	if (flags & FI_REMOTE_CQ_DATA)
		return -FI_EBADFLAGS;

#ifdef ENABLE_DEBUG
	if (msg->tag == UCX_EP_MSG_TAG) {
		FI_DBG(&ucx_prov,FI_LOG_CORE, "Invalid tag format.");
		return -FI_EINVAL;
	}
#endif

	if (flags & FI_PEEK)
		return ucx_tagged_peek(ep, msg, flags);

	return ucx_do_recvmsg(ep, msg, flags, UCX_TAGGED);
}

static ssize_t ucx_tagged_sendmsg(struct fid_ep *ep,
				  const struct fi_msg_tagged *msg,
				  uint64_t flags)
{
	if(flags & FI_REMOTE_CQ_DATA)
		return -FI_EBADFLAGS;

#ifdef ENABLE_DEBUG
	if (msg->tag == UCX_EP_MSG_TAG) {
		FI_DBG(&ucx_prov,FI_LOG_CORE, "Invalid tag format.");
		return -FI_EINVAL;
	}
#endif

	return ucx_do_sendmsg(ep, msg, flags, UCX_TAGGED);
}

static ssize_t ucx_tagged_inject(struct fid_ep *ep, const void *buf,
				 size_t len, fi_addr_t dest_addr, uint64_t tag)
{
#ifdef ENABLE_DEBUG
	if (tag == UCX_EP_MSG_TAG) {
		FI_DBG(&ucx_prov,FI_LOG_CORE, "Invalid tag format.");
		return -FI_EINVAL;
	}
#endif

	return ucx_do_inject(ep, buf, len, dest_addr, tag);
}

static ssize_t ucx_tagged_send(struct fid_ep *ep, const void *buf,
			       size_t len, void *desc, fi_addr_t dest_addr,
			       uint64_t tag, void *context)
{
	struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len,
	};
	struct fi_msg_tagged msg = {
		.msg_iov = &iov,
		.desc = desc,
		.iov_count = 1,
		.addr = dest_addr,
		.tag = tag,
		.context = context,
	};

	return ucx_do_sendmsg(ep, &msg, u_ep->ep.tx_op_flags, UCX_TAGGED);
}

static ssize_t ucx_tagged_sendv(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count, fi_addr_t dest_addr,
				uint64_t tag, void *context)
{
	struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	struct fi_msg_tagged msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = dest_addr,
		.tag = tag,
		.context = context,
	};

	return ucx_do_sendmsg(ep, &msg, u_ep->ep.tx_op_flags, UCX_TAGGED);
}

static ssize_t ucx_tagged_recvv(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count, fi_addr_t src_addr,
				uint64_t tag, uint64_t ignore, void *context)
{
	struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	struct fi_msg_tagged msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = src_addr,
		.tag = tag,
		.ignore = ignore,
		.context = context,
	};

	return ucx_do_recvmsg(ep, &msg, u_ep->ep.rx_op_flags, UCX_TAGGED);
}

static ssize_t ucx_tagged_recv(struct fid_ep *ep, void *buf, size_t len,
			       void *desc, fi_addr_t src_addr, uint64_t tag,
			       uint64_t ignore, void *context)
{
	struct ucx_ep *u_ep = container_of(ep, struct ucx_ep, ep.ep_fid);
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = len,
	};
	struct fi_msg_tagged msg = {
		.msg_iov = &iov,
		.desc = desc,
		.iov_count = 1,
		.addr = src_addr,
		.tag = tag,
		.ignore = ignore,
		.context = context,
	};

	return ucx_do_recvmsg(ep, &msg, u_ep->ep.rx_op_flags, UCX_TAGGED);
}

struct fi_ops_tagged ucx_tagged_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = ucx_tagged_recv,
	.recvv = ucx_tagged_recvv,
	.recvmsg = ucx_tagged_recvmsg,
	.send = ucx_tagged_send,
	.senddata = fi_no_tagged_senddata,
	.sendv = ucx_tagged_sendv,
	.inject = ucx_tagged_inject,
	.sendmsg = ucx_tagged_sendmsg,
	.injectdata = fi_no_tagged_injectdata,
};
