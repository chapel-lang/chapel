/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <stdlib.h>
#include <string.h>
#include <ofi_mem.h>
#include <ofi_iov.h>
#include "efa.h"
#include "efa_av.h"
#include "efa_data_path_ops.h"
#include "efa_data_path_direct.h"

/**
 * @brief check whether endpoint was configured with FI_RMA capability
 * @return -FI_EOPNOTSUPP if FI_RMA wasn't requested, 0 if it was.
 */
static inline int efa_rma_check_cap(struct efa_base_ep *base_ep) {
	if ((base_ep->info->caps & FI_RMA) == FI_RMA)
		return 0;
	EFA_WARN_ONCE(FI_LOG_EP_DATA, "Operation requires FI_RMA capability, which was not requested.\n");
	return -FI_EOPNOTSUPP;
}

/*
 * efa_rma_post_read() will post a read request.
 *
 * Input:
 *     base_ep: endpoint
 *     msg: read operation information
 *     flags: currently no flags is taken
 *
 * On success return 0,
 * If read failed, return the error of read operation
 */
static inline ssize_t efa_rma_post_read(struct efa_base_ep *base_ep,
					const struct fi_msg_rma *msg,
					uint64_t flags)
{
	struct efa_domain *domain = base_ep->domain;
	struct efa_mr *efa_mr;
	struct efa_conn *conn;
	size_t iov_count = msg->iov_count;
	struct ibv_sge sge_list[2];  /* efa device support up to 2 iov */
	uintptr_t wr_id;
	int i, err = 0;
	size_t total_len;
	struct efa_context *efa_ctx;
	struct efa_direct_ope *direct_ope;

	efa_tracepoint(read_begin_msg_context, (size_t) msg->context, (size_t) msg->addr);

	total_len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);

	EFA_DBG(FI_LOG_EP_DATA,
		"total len: %zu, addr: %lu, context: %lx, flags: %lx\n",
		total_len, msg->addr, (size_t) msg->context, flags);

	assert(msg->iov_count <= base_ep->domain->info->tx_attr->iov_limit);
	assert(msg->rma_iov_count > 0 &&
	       msg->rma_iov_count <= base_ep->domain->info->tx_attr->rma_iov_limit);
	assert(total_len <= base_ep->domain->device->max_rdma_size);

	ofi_genlock_lock(&base_ep->util_ep.lock);

	/* Prepare work request ID */
	efa_ctx = efa_fill_context(msg->context, msg->addr, flags,
						       FI_RMA | FI_READ);
	if (efa_env.track_mr && efa_ctx) {
		direct_ope = efa_direct_ope_alloc(
			base_ep, efa_ctx, NULL, msg);
		if (!direct_ope) {
			EFA_WARN(FI_LOG_EP_DATA,
				 "Failed to allocate direct TX operation entry for MR tracking\n");
			err = -FI_EAGAIN;
			goto out_err;
		}
		wr_id = (uintptr_t) direct_ope;
	} else {
		wr_id = (uintptr_t) efa_ctx;
	}

	/* Handle 0-byte read with bounce buffer */
	if (total_len == 0) {
		sge_list[0].addr = (uint64_t)domain->zero_byte_bounce_buf;
		sge_list[0].length = 0;
		sge_list[0].lkey = domain->zero_byte_bounce_buf_mr->ibv_mr->lkey;
		iov_count = 1;
	} else {
		/* Prepare SGE list */
		for (i = 0; i < msg->iov_count; ++i) {
			sge_list[i].addr = (uint64_t)msg->msg_iov[i].iov_base;
			sge_list[i].length = msg->msg_iov[i].iov_len;
			if (OFI_UNLIKELY(!msg->desc || !msg->desc[i])) {
				EFA_WARN(FI_LOG_EP_CTRL,
					 "EFA direct requires FI_MR_LOCAL but "
					 "application does not provide a valid desc\n");
				err = -FI_EINVAL;
				goto out_err;
			}
			efa_mr = (struct efa_mr *)msg->desc[i];
			sge_list[i].lkey = efa_mr->ibv_mr->lkey;
		}
	}

	conn = efa_av_addr_to_conn(base_ep->av, msg->addr);
	assert(conn && conn->ep_addr);

	/* Use consolidated RDMA read function */
	/* ep->domain->info->tx_attr->rma_iov_limit is set to 1 */
	err = efa_qp_post_read(base_ep->qp, sge_list, iov_count,
			       msg->rma_iov[0].key, msg->rma_iov[0].addr,
			       wr_id, flags,
			       conn->ah, conn->ep_addr->qpn, conn->ep_addr->qkey);
	if (OFI_UNLIKELY(err))
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;

	efa_tracepoint(post_read, wr_id, (uintptr_t)msg->context);

out_err:
	ofi_genlock_unlock(&base_ep->util_ep.lock);
	return err;
}

static
ssize_t efa_rma_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct efa_base_ep *base_ep;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	return efa_rma_post_read(base_ep, msg, flags | base_ep->util_ep.tx_msg_flags);
}

static
ssize_t efa_rma_readv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		      size_t iov_count, fi_addr_t src_addr, uint64_t addr,
		      uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_base_ep *base_ep;
	size_t len;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	len = ofi_total_iov_len(iov, iov_count);
	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);
	EFA_SETUP_MSG_RMA(msg, iov, desc, iov_count, src_addr, &rma_iov, 1,
			  context, 0);

	return efa_rma_post_read(base_ep, &msg, efa_tx_flags(base_ep));
}

static
ssize_t efa_rma_read(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
		     fi_addr_t src_addr, uint64_t addr, uint64_t key,
		     void *context)
{
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_base_ep *base_ep;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	assert(len <= base_ep->max_rma_size);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);
	EFA_SETUP_MSG_RMA(msg, &iov, &desc, 1, src_addr, &rma_iov, 1, context, 0);

	return efa_rma_post_read(base_ep, &msg, efa_tx_flags(base_ep));
}

/**
 * @brief Post a WRITE request
 *
 * Input:
 *     base_ep: endpoint
 *     msg: read operation information
 *     flags: flags passed
 * @return On success return 0, otherwise return a negative libfabric error code.
 */
static inline ssize_t efa_rma_post_write(struct efa_base_ep *base_ep,
					 const struct fi_msg_rma *msg,
					 uint64_t flags)
{
	struct efa_domain *domain = base_ep->domain;
	struct efa_conn *conn;
	size_t iov_count = msg->iov_count;
	struct ibv_sge sge_list[2];  /* efa device support up to 2 iov */
	uintptr_t wr_id;
	int i, err = 0;
	size_t total_len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
	struct efa_context *efa_ctx;
	struct efa_direct_ope *direct_ope;

	if (flags & FI_INJECT && total_len > base_ep->inject_rma_size) {
		EFA_WARN(FI_LOG_EP_DATA,
			 "Message size of %zu exceeds efa-directs inject_rma_size size of %zu.\n", total_len, base_ep->inject_rma_size);
		return -FI_ENOSYS;
	}

	efa_tracepoint(write_begin_msg_context, (size_t) msg->context, (size_t) msg->addr);
	EFA_DBG(FI_LOG_EP_DATA,
		"total len: %zu, addr: %lu, context: %lx, flags: %lx\n",
		total_len, msg->addr, (size_t) msg->context, flags);

	ofi_genlock_lock(&base_ep->util_ep.lock);

	/* Prepare work request ID */
	efa_ctx = efa_fill_context(msg->context, msg->addr, flags,
						       FI_RMA | FI_WRITE);
	if (efa_env.track_mr && efa_ctx) {
		direct_ope = efa_direct_ope_alloc(
			base_ep, efa_ctx, NULL, msg);
		if (!direct_ope) {
			EFA_WARN(FI_LOG_EP_DATA,
				 "Failed to allocate direct TX operation entry for MR tracking\n");
			err = -FI_EAGAIN;
			goto out_err;
		}
		wr_id = (uintptr_t) direct_ope;
	} else {
		wr_id = (uintptr_t) efa_ctx;
	}

	/* Handle 0-byte write with bounce buffer */
	if (total_len == 0) {
		sge_list[0].addr = (uint64_t)domain->zero_byte_bounce_buf;
		sge_list[0].length = 0;
		sge_list[0].lkey = domain->zero_byte_bounce_buf_mr->ibv_mr->lkey;
		iov_count = 1;
	} else {
		/* Prepare SGE list */
		for (i = 0; i < msg->iov_count; ++i) {
			sge_list[i].addr = (uint64_t)msg->msg_iov[i].iov_base;
			sge_list[i].length = msg->msg_iov[i].iov_len;
			if (OFI_UNLIKELY(!msg->desc || !msg->desc[i])) {
				EFA_WARN(FI_LOG_EP_CTRL,
					 "EFA direct requires FI_MR_LOCAL but "
					 "application does not provide a valid desc\n");
				err = -FI_EINVAL;
				goto out_err;
			}
			sge_list[i].lkey = ((struct efa_mr *)msg->desc[i])->ibv_mr->lkey;
		}
	}

	conn = efa_av_addr_to_conn(base_ep->av, msg->addr);
	assert(conn && conn->ep_addr);

	/* Use consolidated RDMA write function */
	err = efa_qp_post_write(base_ep->qp, sge_list, iov_count,
				msg->rma_iov[0].key, msg->rma_iov[0].addr,
				wr_id, msg->data, flags,
				conn->ah, conn->ep_addr->qpn, conn->ep_addr->qkey);
	if (OFI_UNLIKELY(err))
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;

	efa_tracepoint(post_write, wr_id, (uintptr_t)msg->context);

out_err:
	ofi_genlock_unlock(&base_ep->util_ep.lock);
	return err;
}

ssize_t efa_rma_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
			 uint64_t flags)
{
	struct efa_base_ep *base_ep;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	return efa_rma_post_write(base_ep, msg, flags | base_ep->util_ep.tx_msg_flags);
}

ssize_t efa_rma_writev(struct fid_ep *ep_fid, const struct iovec *iov,
		       void **desc, size_t iov_count, fi_addr_t dest_addr,
		       uint64_t addr, uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_base_ep *base_ep;
	size_t len;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	len = ofi_total_iov_len(iov, iov_count);
	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);
	EFA_SETUP_MSG_RMA(msg, iov, desc, iov_count, dest_addr, &rma_iov, 1,
			  context, 0);

	return efa_rma_post_write(base_ep, &msg, efa_tx_flags(base_ep));
}

ssize_t efa_rma_write(struct fid_ep *ep_fid, const void *buf, size_t len,
		      void *desc, fi_addr_t dest_addr, uint64_t addr,
		      uint64_t key, void *context)
{
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_base_ep *base_ep;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	assert(len <= base_ep->max_rma_size);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);
	EFA_SETUP_MSG_RMA(msg, &iov, &desc, 1, dest_addr, &rma_iov, 1, context, 0);

	return efa_rma_post_write(base_ep, &msg, efa_tx_flags(base_ep));
}

ssize_t efa_rma_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
			  void *desc, uint64_t data, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key, void *context)
{
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;
	struct efa_base_ep *base_ep;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	assert(len <= base_ep->max_rma_size);
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	EFA_SETUP_IOV(iov, buf, len);
	EFA_SETUP_RMA_IOV(rma_iov, addr, len, key);
	EFA_SETUP_MSG_RMA(msg, &iov, &desc, 1, dest_addr, &rma_iov, 1, context, data);

	return efa_rma_post_write(base_ep, &msg, FI_REMOTE_CQ_DATA | efa_tx_flags(base_ep));
}

ssize_t efa_rma_inject_write(struct fid_ep *ep_fid, const void *buf, size_t len,
			      fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct efa_base_ep *base_ep;
	struct efa_domain *domain;
	struct ibv_sge sge;
	struct efa_conn *conn;
	uintptr_t wr_id;
	int err;

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	domain = base_ep->domain;
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	/* Only support 0-byte inject for efa-direct */
	if (len > base_ep->inject_rma_size)
		return -FI_ENOSYS;

	ofi_genlock_lock(&base_ep->util_ep.lock);

	wr_id = (uintptr_t) efa_fill_context(NULL, dest_addr, FI_INJECT, FI_RMA | FI_WRITE);

	sge.addr = (uint64_t)domain->zero_byte_bounce_buf;
	sge.length = 0;
	sge.lkey = domain->zero_byte_bounce_buf_mr->ibv_mr->lkey;

	conn = efa_av_addr_to_conn(base_ep->av, dest_addr);
	assert(conn && conn->ep_addr);

	err = efa_qp_post_write(base_ep->qp, &sge, 1, key, addr,
				wr_id, 0, 0, conn->ah, conn->ep_addr->qpn,
				conn->ep_addr->qkey);
	if (OFI_UNLIKELY(err))
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;

	ofi_genlock_unlock(&base_ep->util_ep.lock);
	return err;
}

static ssize_t efa_rma_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
					 uint64_t data, fi_addr_t dest_addr,
					 uint64_t addr, uint64_t key)
{
	struct efa_base_ep *base_ep;
	struct efa_domain *domain;
	struct efa_conn *conn;
	struct ibv_sge sge;
	uintptr_t wr_id;
	int err;

	base_ep = container_of(ep, struct efa_base_ep, util_ep.ep_fid);
	domain = base_ep->domain;
	err = efa_rma_check_cap(base_ep);
	if (err)
		return err;

	/* Only support 0-byte inject for efa-direct */
	if (len > base_ep->inject_rma_size)
		return -FI_ENOSYS;

	ofi_genlock_lock(&base_ep->util_ep.lock);

	wr_id = (uintptr_t) efa_fill_context(NULL, dest_addr, FI_INJECT | FI_REMOTE_CQ_DATA, FI_RMA | FI_WRITE);

	sge.addr = (uint64_t)domain->zero_byte_bounce_buf;
	sge.length = 0;
	sge.lkey = domain->zero_byte_bounce_buf_mr->ibv_mr->lkey;

	conn = efa_av_addr_to_conn(base_ep->av, dest_addr);
	assert(conn && conn->ep_addr);

	err = efa_qp_post_write(base_ep->qp, &sge, 1, key, addr,
				wr_id, data, IBV_SEND_INLINE, conn->ah, conn->ep_addr->qpn,
				conn->ep_addr->qkey);
	if (OFI_UNLIKELY(err))
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;

	ofi_genlock_unlock(&base_ep->util_ep.lock);
	return err;
}

struct fi_ops_rma efa_dgram_ep_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = fi_no_rma_read,
	.readv = fi_no_rma_readv,
	.readmsg = fi_no_rma_readmsg,
	.write = fi_no_rma_write,
	.writev = fi_no_rma_writev,
	.writemsg = fi_no_rma_writemsg,
	.inject = fi_no_rma_inject,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};

struct fi_ops_rma efa_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = efa_rma_read,
	.readv = efa_rma_readv,
	.readmsg = efa_rma_readmsg,
	.write = efa_rma_write,
	.writev = efa_rma_writev,
	.writemsg = efa_rma_writemsg,
	.inject = efa_rma_inject_write,
	.writedata = efa_rma_writedata,
	.injectdata = efa_rma_inject_writedata,
};
