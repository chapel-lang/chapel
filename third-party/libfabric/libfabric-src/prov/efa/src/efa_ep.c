/*
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "config.h"
#include "efa.h"

#include <sys/time.h>
#include <infiniband/efadv.h>
#define EFA_CQ_PROGRESS_ENTRIES 500

static int efa_generate_qkey()
{
	struct timeval tv;
	struct timezone tz;
	uint32_t val;
	int err;

	err = gettimeofday(&tv, &tz);
	if (err) {
		EFA_WARN(FI_LOG_EP_CTRL, "Cannot gettimeofday, err=%d.\n", err);
		return 0;
	}

	/* tv_usec is in range [0,1,000,000), shift it by 12 to [0,4,096,000,000 */
	val = (tv.tv_usec << 12) + tv.tv_sec;

	val = ofi_xorshift_random(val);

	/* 0x80000000 and up is privileged Q Key range. */
	val &= 0x7fffffff;

	return val;
}

static int efa_ep_destroy_qp(struct efa_qp *qp)
{
	struct efa_domain *domain;
	int err;

	if (!qp)
		return 0;

	domain = qp->ep->domain;
	domain->qp_table[qp->qp_num & domain->qp_table_sz_m1] = NULL;
	err = -ibv_destroy_qp(qp->ibv_qp);
	if (err)
		EFA_INFO(FI_LOG_CORE, "destroy qp[%u] failed!\n", qp->qp_num);

	free(qp);
	return err;
}

static int efa_ep_modify_qp_state(struct efa_qp *qp, enum ibv_qp_state qp_state,
				  int attr_mask)
{
	struct ibv_qp_attr attr = {};

	attr.qp_state = qp_state;

	if (attr_mask & IBV_QP_PORT)
		attr.port_num = 1;

	if (attr_mask & IBV_QP_QKEY)
		attr.qkey = qp->qkey;

	return -ibv_modify_qp(qp->ibv_qp, &attr, attr_mask);

}

static int efa_ep_modify_qp_rst2rts(struct efa_qp *qp)
{
	int err;

	err = efa_ep_modify_qp_state(qp, IBV_QPS_INIT,
				     IBV_QP_STATE | IBV_QP_PKEY_INDEX |
				     IBV_QP_PORT | IBV_QP_QKEY);
	if (err)
		return err;

	err = efa_ep_modify_qp_state(qp, IBV_QPS_RTR, IBV_QP_STATE);
	if (err)
		return err;

	return efa_ep_modify_qp_state(qp, IBV_QPS_RTS,
				      IBV_QP_STATE | IBV_QP_SQ_PSN);
}

static int efa_ep_create_qp_ex(struct efa_ep *ep,
			       struct ibv_pd *ibv_pd,
			       struct ibv_qp_init_attr_ex *init_attr_ex)
{
	struct efa_domain *domain;
	struct efa_qp *qp;
	struct efadv_qp_init_attr efa_attr = {};
	int err;

	domain = ep->domain;
	qp = calloc(1, sizeof(*qp));
	if (!qp)
		return -FI_ENOMEM;

	if (init_attr_ex->qp_type == IBV_QPT_UD) {
		qp->ibv_qp = ibv_create_qp_ex(ibv_pd->context, init_attr_ex);
	} else {
		assert(init_attr_ex->qp_type == IBV_QPT_DRIVER);
		efa_attr.driver_qp_type = EFADV_QP_DRIVER_TYPE_SRD;
		qp->ibv_qp = efadv_create_qp_ex(ibv_pd->context, init_attr_ex, &efa_attr,
						sizeof(struct efadv_qp_init_attr));
	}

	if (!qp->ibv_qp) {
		EFA_WARN(FI_LOG_EP_CTRL, "ibv_create_qp failed\n");
		err = -EINVAL;
		goto err_free_qp;
	}

	qp->ibv_qp_ex = ibv_qp_to_qp_ex(qp->ibv_qp);
	qp->qkey = efa_generate_qkey();
	err = efa_ep_modify_qp_rst2rts(qp);
	if (err)
		goto err_destroy_qp;

	qp->qp_num = qp->ibv_qp->qp_num;
	ep->qp = qp;
	qp->ep = ep;
	domain->qp_table[ep->qp->qp_num & domain->qp_table_sz_m1] = ep->qp;
	EFA_INFO(FI_LOG_EP_CTRL, "%s(): create QP %d qkey: %d\n", __func__, qp->qp_num, qp->qkey);

	return 0;

err_destroy_qp:
	ibv_destroy_qp(qp->ibv_qp);
err_free_qp:
	free(qp);

	return err;
}

static int efa_ep_getopt(fid_t fid, int level, int optname,
			 void *optval, size_t *optlen)
{
	switch (level) {
	case FI_OPT_ENDPOINT:
		return -FI_ENOPROTOOPT;
	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static int efa_ep_setopt(fid_t fid, int level, int optname, const void *optval, size_t optlen)
{
	switch (level) {
	case FI_OPT_ENDPOINT:
		return -FI_ENOPROTOOPT;
	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static struct fi_ops_ep efa_ep_base_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = efa_ep_getopt,
	.setopt = efa_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static struct efa_ep *efa_ep_alloc(struct fi_info *info)
{
	struct efa_ep *ep;

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return NULL;

	ep->info = fi_dupinfo(info);
	if (!ep->info)
		goto err;

	return ep;

err:
	free(ep);
	return NULL;
}

static void efa_ep_destroy(struct efa_ep *ep)
{
	efa_ep_destroy_qp(ep->qp);
	fi_freeinfo(ep->info);
	free(ep->src_addr);
	if (ofi_endpoint_close(&ep->util_ep))
		FI_WARN(&efa_prov, FI_LOG_EP_CTRL, "Unable to close util EP\n");
	free(ep);
}

static int efa_ep_close(fid_t fid)
{
	struct efa_ep *ep;

	ep = container_of(fid, struct efa_ep, util_ep.ep_fid.fid);

	ofi_bufpool_destroy(ep->recv_wr_pool);
	ofi_bufpool_destroy(ep->send_wr_pool);
	efa_ep_destroy(ep);

	return 0;
}

static int efa_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct efa_ep *ep;
	struct efa_cq *cq;
	struct efa_av *av;
	struct util_eq *eq;
	struct util_cntr *cntr;
	int ret;

	ep = container_of(fid, struct efa_ep, util_ep.ep_fid.fid);
	ret = ofi_ep_bind_valid(&efa_prov, bfid, flags);
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		if (flags & FI_SELECTIVE_COMPLETION) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "Endpoint cannot be bound with selective completion.\n");
			return -FI_EBADFLAGS;
		}

		/* Must bind a CQ to either RECV or SEND completions */
		if (!(flags & (FI_RECV | FI_TRANSMIT)))
			return -FI_EBADFLAGS;

		cq = container_of(bfid, struct efa_cq, util_cq.cq_fid);
		if (ep->domain != cq->domain)
			return -FI_EINVAL;

		ret = ofi_ep_bind_cq(&ep->util_ep, &cq->util_cq, flags);
		if (ret)
			return ret;

		if (flags & FI_RECV) {
			if (ep->rcq)
				return -EINVAL;
			ep->rcq = cq;
		}
		if (flags & FI_TRANSMIT) {
			if (ep->scq)
				return -EINVAL;
			ep->scq = cq;
		}
		break;
	case FI_CLASS_AV:
		av = container_of(bfid, struct efa_av, util_av.av_fid.fid);
		if (ep->domain != av->domain) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "Address vector doesn't belong to same domain as EP.\n");
			return -FI_EINVAL;
		}
		if (ep->av) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "Address vector already bound to EP.\n");
			return -FI_EINVAL;
		}
		ep->av = av;

		ep->av->ep = ep;
		break;
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct util_cntr, cntr_fid.fid);

		ret = ofi_ep_bind_cntr(&ep->util_ep, cntr, flags);
		if (ret)
			return ret;
		break;
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct util_eq, eq_fid.fid);

		ret = ofi_ep_bind_eq(&ep->util_ep, eq);
		if (ret)
			return ret;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int efa_ep_getflags(struct fid_ep *ep_fid, uint64_t *flags)
{
	struct efa_ep *ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	struct fi_tx_attr *tx_attr = ep->info->tx_attr;
	struct fi_rx_attr *rx_attr = ep->info->rx_attr;

	if ((*flags & FI_TRANSMIT) && (*flags & FI_RECV)) {
		EFA_WARN(FI_LOG_EP_CTRL, "Both Tx/Rx flags cannot be specified\n");
		return -FI_EINVAL;
	} else if (tx_attr && (*flags & FI_TRANSMIT)) {
		*flags = tx_attr->op_flags;
	} else if (rx_attr && (*flags & FI_RECV)) {
		*flags = rx_attr->op_flags;
	} else {
		EFA_WARN(FI_LOG_EP_CTRL, "Tx/Rx flags not specified\n");
		return -FI_EINVAL;
	}
	return 0;
}

static int efa_ep_setflags(struct fid_ep *ep_fid, uint64_t flags)
{
	struct efa_ep *ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	struct fi_tx_attr *tx_attr = ep->info->tx_attr;
	struct fi_rx_attr *rx_attr = ep->info->rx_attr;

	if ((flags & FI_TRANSMIT) && (flags & FI_RECV)) {
		EFA_WARN(FI_LOG_EP_CTRL, "Both Tx/Rx flags cannot be specified.\n");
		return -FI_EINVAL;
	} else if (tx_attr && (flags & FI_TRANSMIT)) {
		tx_attr->op_flags = flags;
		tx_attr->op_flags &= ~FI_TRANSMIT;
	} else if (rx_attr && (flags & FI_RECV)) {
		rx_attr->op_flags = flags;
		rx_attr->op_flags &= ~FI_RECV;
	} else {
		EFA_WARN(FI_LOG_EP_CTRL, "Tx/Rx flags not specified\n");
		return -FI_EINVAL;
	}

	return 0;
}

static int efa_ep_enable(struct fid_ep *ep_fid)
{
	struct ibv_qp_init_attr_ex attr_ex = { 0 };
	const struct fi_info *efa_info;
	struct ibv_pd *ibv_pd;
	struct efa_ep *ep;

	ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);

	if (!ep->scq && !ep->rcq) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Endpoint is not bound to a send or receive completion queue\n");
		return -FI_ENOCQ;
	}

	if (!ep->scq && ofi_send_allowed(ep->info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Endpoint is not bound to a send completion queue when it has transmit capabilities enabled (FI_SEND).\n");
		return -FI_ENOCQ;
	}

	if (!ep->rcq && ofi_recv_allowed(ep->info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Endpoint is not bound to a receive completion queue when it has receive capabilities enabled. (FI_RECV)\n");
		return -FI_ENOCQ;
	}

	efa_info = efa_get_efa_info(ep->info->domain_attr->name);
	if (!efa_info) {
		EFA_INFO(FI_LOG_EP_CTRL, "Unable to find matching efa_info\n");
		return -FI_EINVAL;
	}

	if (ep->scq) {
		attr_ex.cap.max_send_wr = ep->info->tx_attr->size;
		attr_ex.cap.max_send_sge = ep->info->tx_attr->iov_limit;
		attr_ex.send_cq = ep->scq->ibv_cq;
		ibv_pd = ep->scq->domain->ibv_pd;
	} else {
		attr_ex.send_cq = ep->rcq->ibv_cq;
		ibv_pd = ep->rcq->domain->ibv_pd;
	}

	if (ep->rcq) {
		attr_ex.cap.max_recv_wr = ep->info->rx_attr->size;
		attr_ex.cap.max_recv_sge = ep->info->rx_attr->iov_limit;
		attr_ex.recv_cq = ep->rcq->ibv_cq;
	} else {
		attr_ex.recv_cq = ep->scq->ibv_cq;
	}

	attr_ex.cap.max_inline_data = ep->domain->ctx->inline_buf_size;

	if (ep->domain->type == EFA_DOMAIN_RDM) {
		attr_ex.qp_type = IBV_QPT_DRIVER;
		attr_ex.comp_mask = IBV_QP_INIT_ATTR_PD | IBV_QP_INIT_ATTR_SEND_OPS_FLAGS;
		attr_ex.send_ops_flags = IBV_QP_EX_WITH_SEND;
		if (efa_ep_support_rdma_read(&ep->util_ep.ep_fid))
			attr_ex.send_ops_flags |= IBV_QP_EX_WITH_RDMA_READ;
		attr_ex.pd = ibv_pd;
	} else {
		attr_ex.qp_type = IBV_QPT_UD;
		attr_ex.comp_mask = IBV_QP_INIT_ATTR_PD;
		attr_ex.pd = ibv_pd;
	}

	attr_ex.qp_context = ep;
	attr_ex.sq_sig_all = 1;

	return efa_ep_create_qp_ex(ep, ibv_pd, &attr_ex);
}

static int efa_ep_control(struct fid *fid, int command, void *arg)
{
	struct fid_ep *ep_fid;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep_fid = container_of(fid, struct fid_ep, fid);
		switch (command) {
		case FI_GETOPSFLAG:
			return efa_ep_getflags(ep_fid, (uint64_t *)arg);
		case FI_SETOPSFLAG:
			return efa_ep_setflags(ep_fid, *(uint64_t *)arg);
		case FI_ENABLE:
			return efa_ep_enable(ep_fid);
		default:
			return -FI_ENOSYS;
		}
		break;
	default:
		return -FI_ENOSYS;
	}
}

static struct fi_ops efa_ep_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_ep_close,
	.bind = efa_ep_bind,
	.control = efa_ep_control,
	.ops_open = fi_no_ops_open,
};

static void efa_ep_progress_internal(struct efa_ep *ep, struct efa_cq *efa_cq)
{
	struct util_cq *cq;
	struct fi_cq_tagged_entry cq_entry[EFA_CQ_PROGRESS_ENTRIES];
	struct fi_cq_tagged_entry *temp_cq_entry;
	struct fi_cq_err_entry cq_err_entry;
	fi_addr_t src_addr[EFA_CQ_PROGRESS_ENTRIES];
	uint64_t flags;
	int i;
	ssize_t ret, err;

	cq = &efa_cq->util_cq;
	flags = ep->util_ep.caps;

	VALGRIND_MAKE_MEM_DEFINED(&cq_entry, sizeof(cq_entry));

	ret = efa_cq_readfrom(&cq->cq_fid, cq_entry, EFA_CQ_PROGRESS_ENTRIES,
			      (flags & FI_SOURCE) ? src_addr : NULL);
	if (ret == -FI_EAGAIN)
		return;

	if (OFI_UNLIKELY(ret < 0)) {
		if (OFI_UNLIKELY(ret != -FI_EAVAIL)) {
			EFA_WARN(FI_LOG_CQ, "no error available errno: %ld\n", ret);
			efa_eq_write_error(&ep->util_ep, FI_EOTHER, ret);
			return;
		}

		err = efa_cq_readerr(&cq->cq_fid, &cq_err_entry, flags);
		if (OFI_UNLIKELY(err < 0)) {
			EFA_WARN(FI_LOG_CQ, "unable to read error entry errno: %ld\n", err);
			efa_eq_write_error(&ep->util_ep, FI_EOTHER, err);
			return;
		}

		ofi_cq_write_error(cq, &cq_err_entry);
		return;
	}

	temp_cq_entry = (struct fi_cq_tagged_entry *)cq_entry;
	for (i = 0; i < ret; i++) {
		(flags & FI_SOURCE) ?
			ofi_cq_write_src(cq, temp_cq_entry->op_context,
					 temp_cq_entry->flags,
					 temp_cq_entry->len,
					 temp_cq_entry->buf,
					 temp_cq_entry->data,
					 temp_cq_entry->tag,
					 src_addr[i]) :
			ofi_cq_write(cq, temp_cq_entry->op_context,
				     temp_cq_entry->flags,
				     temp_cq_entry->len,
				     temp_cq_entry->buf,
				     temp_cq_entry->data,
				     temp_cq_entry->tag);

		temp_cq_entry = (struct fi_cq_tagged_entry *)
				((uint8_t *)temp_cq_entry + efa_cq->entry_size);
	}
	return;
}

void efa_ep_progress(struct util_ep *ep)
{
	struct efa_ep *efa_ep;
	struct efa_cq *rcq;
	struct efa_cq *scq;

	efa_ep = container_of(ep, struct efa_ep, util_ep);
	rcq = efa_ep->rcq;
	scq = efa_ep->scq;

	fastlock_acquire(&ep->lock);

	if (rcq)
		efa_ep_progress_internal(efa_ep, rcq);

	if (scq && scq != rcq)
		efa_ep_progress_internal(efa_ep, scq);

	fastlock_release(&ep->lock);
}

static struct fi_ops_atomic efa_ep_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = fi_no_atomic_write,
	.writev = fi_no_atomic_writev,
	.writemsg = fi_no_atomic_writemsg,
	.inject = fi_no_atomic_inject,
	.readwrite = fi_no_atomic_readwrite,
	.readwritev = fi_no_atomic_readwritev,
	.readwritemsg = fi_no_atomic_readwritemsg,
	.compwrite = fi_no_atomic_compwrite,
	.compwritev = fi_no_atomic_compwritev,
	.compwritemsg = fi_no_atomic_compwritemsg,
	.writevalid = fi_no_atomic_writevalid,
	.readwritevalid = fi_no_atomic_readwritevalid,
	.compwritevalid = fi_no_atomic_compwritevalid,
};

int efa_ep_open(struct fid_domain *domain_fid, struct fi_info *info,
		struct fid_ep **ep_fid, void *context)
{
	struct efa_domain *domain;
	const struct fi_info *fi;
	struct efa_ep *ep;
	int ret;

	domain = container_of(domain_fid, struct efa_domain,
			      util_domain.domain_fid);

	if (!info || !info->ep_attr || !info->domain_attr ||
	    strncmp(domain->ctx->ibv_ctx->device->name, info->domain_attr->name,
		    strlen(domain->ctx->ibv_ctx->device->name))) {
		EFA_INFO(FI_LOG_DOMAIN, "Invalid info->domain_attr->name\n");
		return -FI_EINVAL;
	}

	fi = efa_get_efa_info(info->domain_attr->name);
	if (!fi) {
		EFA_INFO(FI_LOG_DOMAIN, "Unable to find matching efa_info\n");
		return -FI_EINVAL;
	}

	if (info->ep_attr) {
		ret = ofi_check_ep_attr(&efa_util_prov, info->fabric_attr->api_version, fi, info);
		if (ret)
			return ret;
	}

	if (info->tx_attr) {
		ret = ofi_check_tx_attr(&efa_prov, fi->tx_attr,
					info->tx_attr, info->mode);
		if (ret)
			return ret;
	}

	if (info->rx_attr) {
		ret = ofi_check_rx_attr(&efa_prov, fi, info->rx_attr, info->mode);
		if (ret)
			return ret;
	}

	ep = efa_ep_alloc(info);
	if (!ep)
		return -FI_ENOMEM;

	ret = ofi_endpoint_init(domain_fid, &efa_util_prov, info, &ep->util_ep,
				context, efa_ep_progress);
	if (ret)
		goto err_ep_destroy;

	ret = ofi_bufpool_create(&ep->send_wr_pool,
		sizeof(struct efa_send_wr) +
		info->tx_attr->iov_limit * sizeof(struct ibv_sge),
		16, 0, 1024, 0);
	if (ret)
		goto err_ep_destroy;

	ret = ofi_bufpool_create(&ep->recv_wr_pool,
		sizeof(struct efa_recv_wr) +
		info->rx_attr->iov_limit * sizeof(struct ibv_sge),
		16, 0, 1024, 0);
	if (ret)
		goto err_send_wr_destroy;

	ep->domain = domain;
	ep->xmit_more_wr_tail = &ep->xmit_more_wr_head;
	ep->recv_more_wr_tail = &ep->recv_more_wr_head;

	if (info->src_addr) {
		ep->src_addr = (void *)calloc(1, EFA_EP_ADDR_LEN);
		if (!ep->src_addr) {
			ret = -FI_ENOMEM;
			goto err_recv_wr_destroy;
		}
		memcpy(ep->src_addr, info->src_addr, info->src_addrlen);
	}

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.fclass = FI_CLASS_EP;
	(*ep_fid)->fid.context = context;
	(*ep_fid)->fid.ops = &efa_ep_ops;
	(*ep_fid)->ops = &efa_ep_base_ops;
	(*ep_fid)->msg = &efa_ep_msg_ops;
	(*ep_fid)->cm = &efa_ep_cm_ops;
	(*ep_fid)->rma = &efa_ep_rma_ops;
	(*ep_fid)->atomic = &efa_ep_atomic_ops;

	return 0;

err_recv_wr_destroy:
	ofi_bufpool_destroy(ep->recv_wr_pool);
err_send_wr_destroy:
	ofi_bufpool_destroy(ep->send_wr_pool);
err_ep_destroy:
	efa_ep_destroy(ep);
	return ret;
}
