/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <stdlib.h>
#include <string.h>
#include <ofi_mem.h>
#include <ofi_iov.h>
#include "efa_dgram_ep.h"
#include "efa.h"
#include "efa_av.h"


/*
 * efa_dgram_rma_post_read() will post a read request.
 *
 * Input:
 *     ep: endpoint
 *     msg: read operation information
 *     flags: currently no flags is taken
 *     self_comm: indicate whether the read is toward
 *                the end point itself. If self_comm is true,
 *                caller must set msg->addr to FI_ADDR_NOTAVAIL.
 *
 * On success return 0,
 * If read iov and rma_iov count out of device limit, return -FI_EINVAL
 * If read failed, return the error of read operation
 */
ssize_t efa_dgram_rma_post_read(struct efa_dgram_ep *ep, const struct fi_msg_rma *msg,
			  uint64_t flags, bool self_comm)
{
	struct efa_qp *qp;
	struct efa_mr *efa_mr;
	struct efa_conn *conn;
#ifndef _WIN32
	struct ibv_sge sge_list[msg->iov_count];
#else
	/* MSVC compiler does not support array declarations with runtime size, so hardcode
	 * the expected iov_limit/max_sq_sge from the lower-level efa provider.
	 */
	struct ibv_sge sge_list[EFA_DEV_ATTR_MAX_WR_SGE];
#endif
	int i;

	if (OFI_UNLIKELY(msg->iov_count > ep->base_ep.domain->device->ibv_attr.max_sge_rd)) {
		EFA_WARN(FI_LOG_CQ, "invalid iov_count!\n");
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(msg->rma_iov_count > ep->base_ep.domain->info->tx_attr->rma_iov_limit)) {
		EFA_WARN(FI_LOG_CQ, "invalid rma_iov_count!\n");
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(ofi_total_iov_len(msg->msg_iov, msg->iov_count)
			 > ep->base_ep.domain->device->max_rdma_size)) {
		EFA_WARN(FI_LOG_CQ, "maximum rdma_size exceeded!\n");
		return -FI_EINVAL;
	}

	/* caller must provide desc because EFA require FI_MR_LOCAL */
	assert(msg->desc);

	/* ep->domain->info->tx_attr->rma_iov_limit is set to 1 */
	qp = ep->base_ep.qp;
	ibv_wr_start(qp->ibv_qp_ex);
	qp->ibv_qp_ex->wr_id = (uintptr_t)msg->context;
	ibv_wr_rdma_read(qp->ibv_qp_ex, msg->rma_iov[0].key, msg->rma_iov[0].addr);

	for (i = 0; i < msg->iov_count; ++i) {
		sge_list[i].addr = (uint64_t)msg->msg_iov[i].iov_base;
		sge_list[i].length = msg->msg_iov[i].iov_len;
		assert(msg->desc[i]);
		efa_mr = (struct efa_mr *)msg->desc[i];
		sge_list[i].lkey = efa_mr->ibv_mr->lkey;
	}

	ibv_wr_set_sge_list(qp->ibv_qp_ex, msg->iov_count, sge_list);
	if (self_comm) {
		assert(msg->addr == FI_ADDR_NOTAVAIL);
		ibv_wr_set_ud_addr(qp->ibv_qp_ex, ep->base_ep.self_ah,
				   qp->qp_num, qp->qkey);
	} else {
		conn = efa_av_addr_to_conn(ep->base_ep.av, msg->addr);
		assert(conn && conn->ep_addr);
		ibv_wr_set_ud_addr(qp->ibv_qp_ex, conn->ah->ibv_ah,
				   conn->ep_addr->qpn, conn->ep_addr->qkey);
	}

	return ibv_wr_complete(qp->ibv_qp_ex);
}

static
ssize_t efa_dgram_rma_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct efa_dgram_ep *ep = container_of(ep_fid, struct efa_dgram_ep, base_ep.util_ep.ep_fid);

	return efa_dgram_rma_post_read(ep, msg, flags, false);
}

static
ssize_t efa_dgram_rma_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		      size_t iov_count, fi_addr_t src_addr, uint64_t addr,
		      uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;

	rma_iov.addr = addr;
	rma_iov.len = ofi_total_iov_len(iov, iov_count);
	rma_iov.key = key;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.desc = desc;
	msg.iov_count = iov_count;
	msg.addr = src_addr;
	msg.context = context;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;

	return efa_dgram_rma_readmsg(ep, &msg, 0);
}

static
ssize_t efa_dgram_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
		     fi_addr_t src_addr, uint64_t addr, uint64_t key,
		     void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return efa_dgram_rma_readv(ep, &iov, &desc, 1, src_addr, addr, key, context);
}

struct fi_ops_rma efa_dgram_ep_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = efa_dgram_rma_read,
	.readv = efa_dgram_rma_readv,
	.readmsg = efa_dgram_rma_readmsg,
	.write = fi_no_rma_write,
	.writev = fi_no_rma_writev,
	.writemsg = fi_no_rma_writemsg,
	.inject = fi_no_rma_inject,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};

