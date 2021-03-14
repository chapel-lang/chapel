/*
 * Copyright (c) 2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <ofi_mem.h>
#include <ofi_iov.h>
#include "efa.h"

static
ssize_t efa_rma_post_read(struct efa_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct efa_qp *qp;
	struct efa_mr *efa_mr;
	struct efa_conn *conn;
	struct ibv_sge sge_list[msg->iov_count];
	int i;

	if (OFI_UNLIKELY(msg->iov_count > ep->domain->ctx->max_wr_rdma_sge)) {
		EFA_WARN(FI_LOG_CQ, "invalid iov_count!\n");
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(msg->rma_iov_count > ep->domain->info->tx_attr->rma_iov_limit)) {
		EFA_WARN(FI_LOG_CQ, "invalid rma_iov_count!\n");
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(ofi_total_iov_len(msg->msg_iov, msg->iov_count)
			 > ep->domain->ctx->max_rdma_size)) {
		EFA_WARN(FI_LOG_CQ, "maximum rdma_size exceeded!\n");
		return -FI_EINVAL;
	}

	/* caller must provide desc because EFA require FI_MR_LOCAL */
	assert(msg->desc);

	/* ep->domain->info->tx_attr->rma_iov_limit is set to 1 */
	qp = ep->qp;
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
	conn = ep->av->addr_to_conn(ep->av, msg->addr);
	ibv_wr_set_ud_addr(qp->ibv_qp_ex, conn->ah.ibv_ah, conn->ep_addr.qpn, conn->ep_addr.qkey);
	return ibv_wr_complete(qp->ibv_qp_ex);
}

static
ssize_t efa_rma_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg, uint64_t flags)
{
	struct efa_ep *ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);

	return efa_rma_post_read(ep, msg, flags);
}

static
ssize_t efa_rma_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
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

	return efa_rma_readmsg(ep, &msg, 0);
}

static
ssize_t efa_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
		     fi_addr_t src_addr, uint64_t addr, uint64_t key,
		     void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return efa_rma_readv(ep, &iov, &desc, 1, src_addr, addr, key, context);
}

struct fi_ops_rma efa_ep_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = efa_rma_read,
	.readv = efa_rma_readv,
	.readmsg = efa_rma_readmsg,
	.write = fi_no_rma_write,
	.writev = fi_no_rma_writev,
	.writemsg = fi_no_rma_writemsg,
	.inject = fi_no_rma_inject,
	.writedata = fi_no_rma_writedata,
	.injectdata = fi_no_rma_injectdata,
};

