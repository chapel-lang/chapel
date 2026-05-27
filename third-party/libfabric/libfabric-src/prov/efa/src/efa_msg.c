/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "config.h"


#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_iov.h"

#include "efa.h"
#include "efa_av.h"
#include "efa_data_path_ops.h"

#include "efa_tp.h"
#include "efa_data_path_direct.h"

#ifndef EFA_MSG_DUMP
static inline void dump_msg(const struct fi_msg *msg, const char *context) {}
#else
#define DUMP_IOV(i, iov, desc) \
	EFA_DBG(FI_LOG_EP_DATA, \
		"\t{ iov[%d] = { base = %p, buff = \"%s\", len = %zu }, desc = %p },\n", \
		i, iov.iov_base, (char *)iov.iov_base, iov.iov_len, (desc ? desc[i] : NULL))

static inline void dump_msg(const struct fi_msg *msg, const char *context)
{
	int i;

	EFA_DBG(FI_LOG_EP_DATA, "%s: { data = %u, addr = %" PRIu64 ", iov_count = %zu, [\n",
		context, (unsigned)msg->data, msg->addr, msg->iov_count);
	for (i = 0; i < msg->iov_count; ++i)
		DUMP_IOV(i, msg->msg_iov[i], msg->desc);
	EFA_DBG(FI_LOG_EP_DATA, " ] }\n");
}
#endif /* EFA_MSG_DUMP */

/**
 * @brief post receive buffer to EFA device via ibv_post_recv
 *
 * @param[in]	base_ep	endpoint
 * @param[in]	msg	libfabric message
 * @param[in]	flags	libfabric flags, currently only FI_MORE is supported.
 * @reutrn	On Success, return 0
 * 		On failure, return negative libfabric error code
 */
static inline ssize_t efa_post_recv(struct efa_base_ep *base_ep, const struct fi_msg *msg, uint64_t flags)
{
	struct efa_mr *efa_mr;
	struct efa_qp *qp = base_ep->qp;
	struct ibv_recv_wr *bad_wr;
	struct ibv_recv_wr *wr;
	struct efa_context *efa_ctx;
	struct efa_direct_ope *direct_ope;
	uintptr_t addr;
	ssize_t err, post_recv_err;
	size_t i, wr_index;

	efa_tracepoint(recv_begin_msg_context, (size_t) msg->context, (size_t) msg->addr);

	EFA_DBG(FI_LOG_EP_DATA,
		"total len: %zu, addr: %lu, context: %lx, flags: %lx\n",
		ofi_total_iov_len(msg->msg_iov, msg->iov_count),
		msg->addr, (size_t) msg->context, flags);

	ofi_genlock_lock(&base_ep->util_ep.lock);
	wr_index = base_ep->recv_wr_index;
	if (wr_index >= base_ep->info->rx_attr->size) {
		EFA_INFO(FI_LOG_EP_DATA,
			 "recv_wr_index exceeds the rx limit, "
			 "recv_wr_index = %zu, rx size = %zu\n",
			 wr_index, base_ep->info->rx_attr->size);
		err = -FI_EAGAIN;
		goto out_err;
	}

	memset(&base_ep->efa_recv_wr_vec[wr_index], 0, sizeof(base_ep->efa_recv_wr_vec[wr_index]));
	dump_msg(msg, "recv");

	assert(msg->iov_count <= base_ep->info->rx_attr->iov_limit);

	if (qp->ibv_qp->qp_type == IBV_QPT_UD &&
	    OFI_UNLIKELY(msg->msg_iov[0].iov_len <
			 base_ep->info->ep_attr->msg_prefix_size)) {
		EFA_WARN(FI_LOG_EP_DATA,
			 "prefix not present on first iov, "
			 "iov_len[%zu]\n",
			 msg->msg_iov[0].iov_len);
		err = -EINVAL;
		goto out_err;
	}

	wr = &base_ep->efa_recv_wr_vec[wr_index].wr;
	wr->num_sge = msg->iov_count;
	wr->sg_list = base_ep->efa_recv_wr_vec[wr_index].sge;
	efa_ctx = efa_fill_context(msg->context, msg->addr, flags,
				   FI_RECV | FI_MSG);
	if (efa_env.track_mr && efa_ctx) {
		direct_ope = efa_direct_ope_alloc(base_ep, efa_ctx, msg, NULL);
		if (!direct_ope) {
			EFA_WARN(FI_LOG_EP_DATA,
				 "Failed to allocate direct RX operation entry for MR tracking\n");
			err = -FI_EAGAIN;
			goto out_err;
		}
		wr->wr_id = (uintptr_t) direct_ope;
	} else {
		wr->wr_id = (uintptr_t) efa_ctx;
	}

	for (i = 0; i < msg->iov_count; i++) {
		addr = (uintptr_t)msg->msg_iov[i].iov_base;

		/* Set RX buffer desc from SGE */
		wr->sg_list[i].length = msg->msg_iov[i].iov_len;
		if (OFI_UNLIKELY(!msg->desc || !msg->desc[i])) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "EFA direct requires FI_MR_LOCAL but "
				 "application does not provide a valid desc\n");
			err = -FI_EINVAL;
			goto out_err;
		}
		efa_mr = (struct efa_mr *)msg->desc[i];
		wr->sg_list[i].lkey = efa_mr->ibv_mr->lkey;
		wr->sg_list[i].addr = addr;
	}

	base_ep->efa_recv_wr_vec[wr_index].wr.next = NULL;
	if (wr_index > 0)
		base_ep->efa_recv_wr_vec[wr_index - 1].wr.next = &base_ep->efa_recv_wr_vec[wr_index].wr;

	base_ep->recv_wr_index++;

	if (flags & FI_MORE) {
		err = 0;
		goto out;
	}

	efa_tracepoint(post_recv, wr->wr_id, (uintptr_t)msg->context);

	err = efa_qp_post_recv(qp, &base_ep->efa_recv_wr_vec[0].wr, &bad_wr);
	if (OFI_UNLIKELY(err)) {
		/* On failure, ibv_post_recv() return positive errno.
		 * Meanwhile, this function return a negative errno.
		 * So, we do the conversion here.
		 */
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;
	}

	base_ep->recv_wr_index = 0;

out:
	ofi_genlock_unlock(&base_ep->util_ep.lock);

	return err;

out_err:
	if (base_ep->recv_wr_index > 0) {
		post_recv_err = efa_qp_post_recv(qp, &base_ep->efa_recv_wr_vec[0].wr, &bad_wr);
		if (OFI_UNLIKELY(post_recv_err)) {
			EFA_WARN(FI_LOG_EP_DATA,
				 "Encountered error %ld when ibv_post_recv on error handling path\n",
				 post_recv_err);
		}
	}

	base_ep->recv_wr_index = 0;

	ofi_genlock_unlock(&base_ep->util_ep.lock);

	return err;
}

static ssize_t efa_ep_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);

	return efa_post_recv(base_ep, msg, flags | base_ep->util_ep.rx_msg_flags);
}

static ssize_t efa_ep_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			   void *desc, fi_addr_t src_addr, void *context)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct iovec iov;
	struct fi_msg msg;

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_MSG(msg, &iov, &desc, 1, src_addr, context, 0);

	return efa_post_recv(base_ep, &msg, efa_rx_flags(base_ep));
}

static ssize_t efa_ep_recvv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
			    size_t count, fi_addr_t src_addr, void *context)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct fi_msg msg;

	EFA_SETUP_MSG(msg, iov, desc, count, src_addr, context, 0);

	return efa_post_recv(base_ep, &msg, efa_rx_flags(base_ep));
}

static inline ssize_t efa_post_send(struct efa_base_ep *base_ep, const struct fi_msg *msg, uint64_t flags)
{
	struct efa_qp *qp = base_ep->qp;
	struct efa_conn *conn;
	struct ibv_sge sg_list[2];  /* efa device support up to 2 iov */
	struct ibv_data_buf inline_data_list[2];
	struct efa_context *efa_ctx;
	struct efa_direct_ope *direct_ope;
	size_t len, i;
	size_t iov_count = msg->iov_count;
	bool use_inline;
	int ret = 0;
	uintptr_t wr_id;

	efa_tracepoint(send_begin_msg_context, (size_t) msg->context, (size_t) msg->addr);

	len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);

	EFA_DBG(FI_LOG_EP_DATA,
		"total len: %zu, addr: %lu, context: %lx, flags: %lx\n",
		len, msg->addr, (size_t) msg->context, flags);

	dump_msg(msg, "send");

	conn = efa_av_addr_to_conn(base_ep->av, msg->addr);
	assert(conn && conn->ep_addr);

	assert(msg->iov_count <= base_ep->info->tx_attr->iov_limit);

	if (qp->ibv_qp->qp_type == IBV_QPT_UD) {
		assert(msg->msg_iov[0].iov_len >= base_ep->info->ep_attr->msg_prefix_size);
		len -= base_ep->info->ep_attr->msg_prefix_size;
	}

	assert(len <= base_ep->info->ep_attr->max_msg_size);

	ofi_genlock_lock(&base_ep->util_ep.lock);

	/* Prepare work request ID */
	efa_ctx = efa_fill_context(msg->context, msg->addr, flags,
				   FI_SEND | FI_MSG);
	if (efa_env.track_mr && efa_ctx) {
		direct_ope = efa_direct_ope_alloc(base_ep, efa_ctx, msg, NULL);
		if (!direct_ope) {
			EFA_WARN(FI_LOG_EP_DATA,
				 "Failed to allocate direct TX operation entry for MR tracking\n");
			ret = -FI_EAGAIN;
			goto out_err;
		}
		wr_id = (uintptr_t) direct_ope;
	} else {
		wr_id = (uintptr_t) efa_ctx;
	}

	/* Determine if we should use inline data */
	use_inline = (len <= base_ep->domain->device->efa_attr.inline_buf_size &&
		      (!msg->desc || !efa_mr_is_hmem(msg->desc[0])));

	/* Handle 0-byte send with inline path and 0 SGEs */
	if (len == 0) {
		iov_count = 0;
		use_inline = true;
		goto post;
	}

	if (use_inline) {
		/* Prepare inline data list */
		for (i = 0; i < msg->iov_count; i++) {
			inline_data_list[i].addr = msg->msg_iov[i].iov_base;
			inline_data_list[i].length = msg->msg_iov[i].iov_len;

			/* Whole prefix must be on the first sgl for dgram */
			if (!i && qp->ibv_qp->qp_type == IBV_QPT_UD) {
				inline_data_list[i].addr = (char*)inline_data_list[i].addr + base_ep->info->ep_attr->msg_prefix_size;
				inline_data_list[i].length -= base_ep->info->ep_attr->msg_prefix_size;
			}
		}
	} else {
		/* Prepare SGE list */
		for (i = 0; i < msg->iov_count; i++) {
			/* Set TX buffer desc from SGE */
			if (OFI_UNLIKELY(!msg->desc || !msg->desc[i])) {
				EFA_WARN(FI_LOG_EP_CTRL,
					 "EFA direct requires FI_MR_LOCAL but "
					 "application does not provide a valid desc\n");
				ret = -FI_EINVAL;
				goto out_err;
			}
			sg_list[i].lkey = ((struct efa_mr *)msg->desc[i])->ibv_mr->lkey;
			sg_list[i].addr = (uintptr_t)msg->msg_iov[i].iov_base;
			sg_list[i].length = msg->msg_iov[i].iov_len;

			/* Whole prefix must be on the first sgl for dgram */
			if (!i && qp->ibv_qp->qp_type == IBV_QPT_UD) {
				sg_list[i].addr += base_ep->info->ep_attr->msg_prefix_size;
				sg_list[i].length -= base_ep->info->ep_attr->msg_prefix_size;
			}
		}
	}

post:
	/* Use consolidated send function */
	ret = efa_qp_post_send(qp, sg_list, inline_data_list, iov_count,
			       use_inline, wr_id, msg->data, flags,
			       conn->ah, conn->ep_addr->qpn, conn->ep_addr->qkey);
	if (OFI_UNLIKELY(ret))
		ret = (ret == ENOMEM) ? -FI_EAGAIN : -ret;

	efa_tracepoint(post_send, wr_id, (uintptr_t)msg->context);

out_err:
	ofi_genlock_unlock(&base_ep->util_ep.lock);
	return ret;
}

static ssize_t efa_ep_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg, uint64_t flags)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);

	return efa_post_send(base_ep, msg, flags | base_ep->util_ep.tx_msg_flags);
}

static ssize_t efa_ep_send(struct fid_ep *ep_fid, const void *buf, size_t len,
			   void *desc, fi_addr_t dest_addr, void *context)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct fi_msg msg;
	struct iovec iov;

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_MSG(msg, &iov, &desc, 1, dest_addr, context, 0);

	return efa_post_send(base_ep, &msg, efa_tx_flags(base_ep));
}

static ssize_t efa_ep_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			       void *desc, uint64_t data, fi_addr_t dest_addr, void *context)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct fi_msg msg;
	struct iovec iov;

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_MSG(msg, &iov, &desc, 1, dest_addr, context, data);

	return efa_post_send(base_ep, &msg, efa_tx_flags(base_ep) | FI_REMOTE_CQ_DATA);
}

static ssize_t efa_ep_sendv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
			    size_t count, fi_addr_t dest_addr, void *context)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct fi_msg msg;

	EFA_SETUP_MSG(msg, iov, desc, count, dest_addr, context, 0);

	return efa_post_send(base_ep, &msg, efa_tx_flags(base_ep));
}

static ssize_t efa_ep_msg_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
				 fi_addr_t dest_addr)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct fi_msg msg;
	struct iovec iov;

	assert(len <= base_ep->domain->device->efa_attr.inline_buf_size);

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_MSG(msg, &iov, NULL, 1, dest_addr, NULL, 0);

	return efa_post_send(base_ep, &msg, FI_INJECT);
}

static ssize_t efa_ep_msg_injectdata(struct fid_ep *ep_fid, const void *buf,
				     size_t len, uint64_t data,
				     fi_addr_t dest_addr)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct fi_msg msg;
	struct iovec iov;

	assert(len <= base_ep->domain->device->efa_attr.inline_buf_size);

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_MSG(msg, &iov, NULL, 1, dest_addr, NULL, data);

	return efa_post_send(base_ep, &msg, FI_REMOTE_CQ_DATA | FI_INJECT);
}

struct fi_ops_msg efa_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = efa_ep_recv,
	.recvv = efa_ep_recvv,
	.recvmsg = efa_ep_recvmsg,
	.send = efa_ep_send,
	.sendv = efa_ep_sendv,
	.sendmsg = efa_ep_sendmsg,
	.senddata = efa_ep_senddata,
	.inject = efa_ep_msg_inject,
	.injectdata = efa_ep_msg_injectdata,
};

struct fi_ops_msg efa_dgram_ep_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = efa_ep_recv,
	.recvv = efa_ep_recvv,
	.recvmsg = efa_ep_recvmsg,
	.send = efa_ep_send,
	.sendv = efa_ep_sendv,
	.sendmsg = efa_ep_sendmsg,
	.senddata = efa_ep_senddata,
	.inject = fi_no_msg_inject,
	.injectdata = fi_no_msg_injectdata,
};
