/*
 * Copyright (c) 2017-2018 Intel Corporation. All rights reserved.
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

#include "rstream.h"


static int rstream_ep_close(fid_t fid)
{
	int ret;
	struct rstream_ep *rstream_ep =
		container_of(fid, struct rstream_ep, util_ep.ep_fid.fid);

	ret = fi_close(&rstream_ep->local_mr.mr->fid);
	if (ret)
		return ret;
	free(rstream_ep->local_mr.base_addr);

	ret = fi_close(&rstream_ep->ep_fd->fid);
	if (ret)
		return ret;

	ret = fi_close(&rstream_ep->cq->fid);
	if (ret)
		return ret;

	ofi_endpoint_close(&rstream_ep->util_ep);

	rstream_tx_ctx_fs_free(rstream_ep->tx_ctxs);

	fastlock_destroy(&rstream_ep->send_lock);
	fastlock_destroy(&rstream_ep->recv_lock);
	fastlock_destroy(&rstream_ep->cq_lock);
	free(rstream_ep->rx_ctxs);
	free(rstream_ep);
	return 0;
}

static int rstream_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	int ret;
	struct rstream_ep *rstream_ep =
		container_of(ep_fid, struct rstream_ep, util_ep.ep_fid.fid);
	struct rstream_eq *rstream_eq = NULL;

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		rstream_eq = container_of(bfid, struct rstream_eq,
			util_eq.eq_fid.fid);
		ret = fi_ep_bind(rstream_ep->ep_fd, &rstream_eq->eq_fd->fid,
			flags);
		rbtInsert(rstream_eq->ep_map, &rstream_ep->ep_fd->fid,
			rstream_ep);
		break;
	default:
		FI_WARN(&rstream_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static int rstream_reg_mrs(struct fid_domain *domain,
	struct rstream_lmr_data *lmr)
{
	int ret;
	uint64_t rx_meta_data_offset = 0;
	uint32_t full_mr_size = lmr->tx.size + lmr->rx.size;

	if (RSTREAM_USING_IWARP)
		rx_meta_data_offset = RSTREAM_IWARP_DATA_SIZE * lmr->rx.size;

	full_mr_size = full_mr_size + rx_meta_data_offset;
	lmr->base_addr = malloc(full_mr_size);

	ret = fi_mr_reg(domain, lmr->base_addr, full_mr_size,
		FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE,
		0, 0, 0, &lmr->mr, NULL);
	if (ret)
		return ret;

	lmr->ldesc = fi_mr_desc(lmr->mr);
	lmr->rkey = fi_mr_key(lmr->mr);
	lmr->tx.data_start = (char *)lmr->base_addr;
	lmr->tx.avail_size = lmr->tx.size;
	lmr->rx.data_start = (char *)lmr->tx.data_start +
		lmr->tx.size + rx_meta_data_offset;

	return ret;
}

static int rstream_cq_init(struct fid_domain *domain, struct rstream_ep *rep)
{
	int ret;
	struct fi_cq_attr attr;

	memset(&attr, 0, sizeof(attr));
	attr.format = FI_CQ_FORMAT_DATA;
	attr.wait_obj = FI_WAIT_FD;
	attr.size = rep->qp_win.max_rx_credits + rep->qp_win.max_tx_credits;

	ret = fi_cq_open(domain, &attr, &rep->cq, NULL);
	if (ret)
		return ret;

	ret = fi_ep_bind(rep->ep_fd, &rep->cq->fid, FI_TRANSMIT | FI_RECV);
	if (ret)
		return ret;

	rep->qp_win.tx_credits =
		rep->qp_win.max_tx_credits - RSTREAM_MAX_CTRL;

	return ret;
}

static int rstream_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct rstream_ep *rstream_ep;
	int ret = 0;
	rstream_ep = container_of(fid, struct rstream_ep, util_ep.ep_fid.fid);

	switch (command) {
	case FI_ENABLE:
		ret = rstream_reg_mrs(rstream_ep->msg_domain,
			&rstream_ep->local_mr);
		if (ret)
			goto err1;
		ret = rstream_cq_init(rstream_ep->msg_domain, rstream_ep);
		if (ret)
			goto err1;
		ret = fi_enable(rstream_ep->ep_fd);
		break;
	case FI_GETWAIT:
		ret = fi_control(&rstream_ep->cq->fid, FI_GETWAIT, arg);
		if (ret)
			return ret;
		break;
	default:
		return -FI_ENOSYS;
	}

	return ret;

err1:
	if (rstream_ep->local_mr.base_addr)
		free(rstream_ep->local_mr.base_addr);
	if (rstream_ep->local_mr.mr)
		fi_close(&rstream_ep->local_mr.mr->fid);

	return ret;
}

static struct fi_ops rstream_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rstream_ep_close,
	.bind = rstream_ep_bind,
	.control = rstream_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int rstream_ep_setopt(fid_t fid, int level, int optname,
	const void *optval, size_t optlen)
{
	struct rstream_ep *rstream_ep =
		container_of(fid, struct rstream_ep, util_ep.ep_fid.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	if (optname == FI_OPT_SEND_BUF_SIZE) {
		if(sizeof(rstream_ep->local_mr.tx.size) != optlen)
			return -FI_EINVAL;
		rstream_ep->local_mr.tx.size = *((uint32_t *)optval);
	} else if (optname == FI_OPT_RECV_BUF_SIZE) {
		if(sizeof(rstream_ep->local_mr.rx.size) != optlen)
			return -FI_EINVAL;
		rstream_ep->local_mr.rx.size = *((uint32_t *)optval);
	} else if (optname == FI_OPT_TX_SIZE) {
		if(sizeof(rstream_ep->qp_win.max_tx_credits) != optlen)
			return -FI_EINVAL;
		rstream_ep->qp_win.max_tx_credits = *((uint16_t *)optval);
	} else if (optname == FI_OPT_RX_SIZE) {
		if(sizeof(rstream_ep->qp_win.max_rx_credits) != optlen)
			return -FI_EINVAL;
		rstream_ep->qp_win.max_rx_credits = *((uint16_t *)optval);
	} else {
		return -FI_ENOPROTOOPT;
	}

	return 0;
}

static struct fi_ops_ep rstream_ops_ep = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = rstream_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

int rstream_ep_open(struct fid_domain *domain, struct fi_info *info,
		   struct fid_ep **ep_fid, void *context)
{
	struct rstream_ep *rstream_ep;
	struct rstream_domain *rstream_domain;
	struct rstream_pep *rstream_pep = NULL;
	int ret;

	rstream_domain = container_of(domain, struct rstream_domain,
		util_domain.domain_fid);

	rstream_ep = calloc(1, sizeof(*rstream_ep));
	if (!rstream_ep)
		return -FI_ENOMEM;

	/* manual progress */
	ret = ofi_endpoint_init(domain, &rstream_util_prov, info,
		&rstream_ep->util_ep, context, NULL);
	if (ret)
		goto err1;

	rstream_info_to_core(FI_VERSION(1, 8), NULL, info);

	if (info->handle && info->handle->fclass == FI_CLASS_PEP) {
		rstream_pep = container_of(info->handle,
			struct rstream_pep, util_pep.pep_fid);
		info->handle = &rstream_pep->pep_fd->fid;
	}

	ret = fi_endpoint(rstream_domain->msg_domain, info,
		&rstream_ep->ep_fd, NULL);
	if (ret)
		goto err1;

	if (rstream_pep)
		free(rstream_pep);

	rstream_ep->msg_domain = rstream_domain->msg_domain;
	rstream_ep->local_mr.tx.size = RSTREAM_DEFAULT_MR_SEG_SIZE;
	rstream_ep->local_mr.rx.size = RSTREAM_DEFAULT_MR_SEG_SIZE;

	rstream_ep->qp_win.max_tx_credits = rstream_info.tx_attr->size;
	rstream_ep->qp_win.ctrl_credits = RSTREAM_MAX_CTRL;
	rstream_ep->qp_win.max_rx_credits = rstream_info.rx_attr->size;

	rstream_ep->tx_ctxs =
		rstream_tx_ctx_fs_create(rstream_ep->qp_win.max_tx_credits,
			NULL, NULL);

	assert(rstream_ep->tx_ctxs);
	rstream_ep->rx_ctxs = (struct fi_context *)
		calloc(rstream_ep->qp_win.max_rx_credits,
		sizeof(*rstream_ep->rx_ctxs));
	assert(rstream_ep->rx_ctxs);

	*ep_fid = &rstream_ep->util_ep.ep_fid;
	(*ep_fid)->fid.ops = &rstream_ep_fi_ops;
	(*ep_fid)->ops = &rstream_ops_ep;
	(*ep_fid)->cm = &rstream_ops_cm;
	(*ep_fid)->msg = &rstream_ops_msg;
	fastlock_init(&rstream_ep->send_lock);
	fastlock_init(&rstream_ep->recv_lock);
	fastlock_init(&rstream_ep->cq_lock);
	return 0;

err1:
	free(rstream_ep);
	return ret;
}

static int rstream_pep_bind(struct fid *pep_fid, struct fid *bfid,
	uint64_t flags)
{
	struct rstream_pep *rstream_pep = container_of(pep_fid,
		struct rstream_pep, util_pep.pep_fid);
	struct rstream_eq *rstream_eq = NULL;
	int ret;

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		rstream_eq = container_of(bfid, struct rstream_eq,
			util_eq.eq_fid.fid);
		ret = fi_pep_bind(rstream_pep->pep_fd, &rstream_eq->eq_fd->fid,
			flags);
		break;
	default:
		FI_WARN(&rstream_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

static int rstream_pep_ctrl(struct fid *fid, int command, void *arg)
{
	struct rstream_pep *rstream_pep;
	int ret = 0;

	switch (fid->fclass) {
	case FI_CLASS_PEP:
		if (command != FI_BACKLOG)
			return -FI_EINVAL;
		rstream_pep = container_of(fid, struct rstream_pep,
			util_pep.pep_fid.fid);
		ret = fi_control(&rstream_pep->pep_fd->fid, command, arg);
		break;
	default:
		return -FI_ENOSYS;
	}

	return ret;
}

static int rstream_pep_close(fid_t fid)
{
	struct rstream_pep *rstream_pep =
		container_of(fid, struct rstream_pep, util_pep.pep_fid.fid);
	int ret;

	ret = fi_close(&rstream_pep->pep_fd->fid);
	if (ret)
		return ret;

	ofi_pep_close(&rstream_pep->util_pep);
	free(rstream_pep);

	return ret;
}

static struct fi_ops rstream_pep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rstream_pep_close,
	.bind = rstream_pep_bind,
	.control = rstream_pep_ctrl,
	.ops_open = fi_no_ops_open,
};

int rstream_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
	struct fid_pep **pep, void *context)
{
	int ret;
	struct rstream_fabric *rstream_fabric = container_of(fabric,
		struct rstream_fabric, util_fabric.fabric_fid);
	struct rstream_pep *rstream_pep;

	rstream_pep = calloc(1, sizeof(*rstream_pep));
	if (!rstream_pep)
		return -FI_ENOMEM;

	rstream_info_to_core(FI_VERSION(1, 8), NULL, info);

	ret = fi_passive_ep(rstream_fabric->msg_fabric, info,
		&rstream_pep->pep_fd, NULL);
	if (ret)
		goto err1;

	*pep = &rstream_pep->util_pep.pep_fid;
	(*pep)->fid.fclass = FI_CLASS_PEP;
	(*pep)->fid.ops = &rstream_pep_fi_ops;
	(*pep)->ops = &rstream_ops_ep;
	(*pep)->cm = &rstream_ops_pep_cm;

	return 0;

err1:
	free(rstream_pep);
	return ret;
}

void rstream_process_cm_event(struct rstream_ep *ep, void *cm_data)
{
	assert(ep && cm_data);

	int i;
	struct rstream_cm_data *rcv_data = (struct rstream_cm_data *)cm_data;

	assert(rcv_data->version == RSTREAM_RSOCKETV2);

	ep->qp_win.target_rx_credits = ntohs(rcv_data->max_rx_credits);
	ep->qp_win.max_target_rx_credits = ep->qp_win.target_rx_credits;
	ep->remote_data.rkey = ntohll(rcv_data->rkey);
	ep->remote_data.mr.data_start = (void *)ntohll(rcv_data->base_addr);
	ep->remote_data.mr.size = ntohl(rcv_data->rmr_size);
	ep->remote_data.mr.avail_size = ep->remote_data.mr.size;

	for(i = 0; i < ep->qp_win.max_rx_credits; i++) {
		rstream_post_cq_data_recv(ep, NULL);
	}
}
