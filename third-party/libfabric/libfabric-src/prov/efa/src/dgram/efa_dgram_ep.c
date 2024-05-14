/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "config.h"
#include "efa_dgram_ep.h"
#include "efa_dgram_cq.h"
#include "efa.h"
#include "efa_av.h"

#include <infiniband/efadv.h>
#define efa_dgram_cq_PROGRESS_ENTRIES 500

static int efa_dgram_ep_getopt(fid_t fid, int level, int optname,
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

static int efa_dgram_ep_setopt(fid_t fid, int level, int optname, const void *optval, size_t optlen)
{
	switch (level) {
	case FI_OPT_ENDPOINT:
		return -FI_ENOPROTOOPT;
	default:
		return -FI_ENOPROTOOPT;
	}
	return 0;
}

static struct fi_ops_ep efa_dgram_ep_base_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = efa_dgram_ep_getopt,
	.setopt = efa_dgram_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static void efa_dgram_ep_destroy(struct efa_dgram_ep *ep)
{
	int ret;

	ret = efa_base_ep_destruct(&ep->base_ep);
	if (ret) {
		EFA_WARN(FI_LOG_EP_CTRL, "Unable to close base endpoint\n");
	}

	free(ep);
}

static int efa_dgram_ep_close(fid_t fid)
{
	struct efa_dgram_ep *ep;

	ep = container_of(fid, struct efa_dgram_ep, base_ep.util_ep.ep_fid.fid);

	ofi_bufpool_destroy(ep->recv_wr_pool);
	ofi_bufpool_destroy(ep->send_wr_pool);
	efa_dgram_ep_destroy(ep);

	return 0;
}

static int efa_dgram_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct efa_dgram_ep *ep;
	struct efa_dgram_cq *cq;
	struct efa_av *av;
	struct util_eq *eq;
	struct util_cntr *cntr;
	int ret;

	ep = container_of(fid, struct efa_dgram_ep, base_ep.util_ep.ep_fid.fid);
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

		cq = container_of(bfid, struct efa_dgram_cq, util_cq.cq_fid);
		if (ep->base_ep.domain != cq->domain)
			return -FI_EINVAL;

		ret = ofi_ep_bind_cq(&ep->base_ep.util_ep, &cq->util_cq, flags);
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
		ret = efa_base_ep_bind_av(&ep->base_ep, av);
		if (ret)
			return ret;
		break;
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct util_cntr, cntr_fid.fid);

		ret = ofi_ep_bind_cntr(&ep->base_ep.util_ep, cntr, flags);
		if (ret)
			return ret;
		break;
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct util_eq, eq_fid.fid);

		ret = ofi_ep_bind_eq(&ep->base_ep.util_ep, eq);
		if (ret)
			return ret;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int efa_dgram_ep_getflags(struct fid_ep *ep_fid, uint64_t *flags)
{
	struct efa_dgram_ep *ep = container_of(ep_fid, struct efa_dgram_ep, base_ep.util_ep.ep_fid);
	struct fi_tx_attr *tx_attr = ep->base_ep.info->tx_attr;
	struct fi_rx_attr *rx_attr = ep->base_ep.info->rx_attr;

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

static int efa_dgram_ep_setflags(struct fid_ep *ep_fid, uint64_t flags)
{
	struct efa_dgram_ep *ep = container_of(ep_fid, struct efa_dgram_ep, base_ep.util_ep.ep_fid);
	struct fi_tx_attr *tx_attr = ep->base_ep.info->tx_attr;
	struct fi_rx_attr *rx_attr = ep->base_ep.info->rx_attr;

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

static int efa_dgram_ep_enable(struct fid_ep *ep_fid)
{
	struct ibv_qp_init_attr_ex attr_ex = { 0 };
	struct ibv_pd *ibv_pd;
	struct efa_dgram_ep *ep;
	int err;

	ep = container_of(ep_fid, struct efa_dgram_ep, base_ep.util_ep.ep_fid);

	if (!ep->scq && !ep->rcq) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a send or receive completion queue\n");
		return -FI_ENOCQ;
	}

	if (!ep->scq && ofi_send_allowed(ep->base_ep.info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a send completion queue when it has transmit capabilities enabled (FI_SEND).\n");
		return -FI_ENOCQ;
	}

	if (!ep->rcq && ofi_recv_allowed(ep->base_ep.info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a receive completion queue when it has receive capabilities enabled. (FI_RECV)\n");
		return -FI_ENOCQ;
	}

	if (ep->scq) {
		attr_ex.cap.max_send_wr = ep->base_ep.info->tx_attr->size;
		attr_ex.cap.max_send_sge = ep->base_ep.info->tx_attr->iov_limit;
		attr_ex.send_cq = ibv_cq_ex_to_cq(ep->scq->ibv_cq_ex);
		ibv_pd = ep->scq->domain->ibv_pd;
	} else {
		attr_ex.send_cq = ibv_cq_ex_to_cq(ep->rcq->ibv_cq_ex);
		ibv_pd = ep->rcq->domain->ibv_pd;
	}

	if (ep->rcq) {
		attr_ex.cap.max_recv_wr = ep->base_ep.info->rx_attr->size;
		attr_ex.cap.max_recv_sge = ep->base_ep.info->rx_attr->iov_limit;
		attr_ex.recv_cq = ibv_cq_ex_to_cq(ep->rcq->ibv_cq_ex);
	} else {
		attr_ex.recv_cq = ibv_cq_ex_to_cq(ep->scq->ibv_cq_ex);
	}

	attr_ex.cap.max_inline_data =
		ep->base_ep.domain->device->efa_attr.inline_buf_size;

	assert(EFA_EP_TYPE_IS_DGRAM(ep->base_ep.domain->info));
	attr_ex.qp_type = IBV_QPT_UD;
	attr_ex.comp_mask = IBV_QP_INIT_ATTR_PD;
	attr_ex.pd = ibv_pd;

	attr_ex.qp_context = ep;
	attr_ex.sq_sig_all = 1;

	err = efa_base_ep_create_qp(&ep->base_ep, &attr_ex);
	if (err)
		return err;

	return efa_base_ep_enable(&ep->base_ep);
}

static int efa_dgram_ep_control(struct fid *fid, int command, void *arg)
{
	struct fid_ep *ep_fid;

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep_fid = container_of(fid, struct fid_ep, fid);
		switch (command) {
		case FI_GETOPSFLAG:
			return efa_dgram_ep_getflags(ep_fid, (uint64_t *)arg);
		case FI_SETOPSFLAG:
			return efa_dgram_ep_setflags(ep_fid, *(uint64_t *)arg);
		case FI_ENABLE:
			return efa_dgram_ep_enable(ep_fid);
		default:
			return -FI_ENOSYS;
		}
		break;
	default:
		return -FI_ENOSYS;
	}
}

static struct fi_ops efa_dgram_ep_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_dgram_ep_close,
	.bind = efa_dgram_ep_bind,
	.control = efa_dgram_ep_control,
	.ops_open = fi_no_ops_open,
};

static void efa_dgram_ep_progress_internal(struct efa_dgram_ep *ep, struct efa_dgram_cq *efa_dgram_cq)
{
	struct util_cq *cq;
	struct fi_cq_tagged_entry cq_entry[efa_dgram_cq_PROGRESS_ENTRIES];
	struct fi_cq_tagged_entry *temp_cq_entry;
	struct fi_cq_err_entry cq_err_entry = {0};
	fi_addr_t src_addr[efa_dgram_cq_PROGRESS_ENTRIES];
	uint64_t flags;
	int i;
	ssize_t ret, err;

	cq = &efa_dgram_cq->util_cq;
	flags = ep->base_ep.util_ep.caps;

	VALGRIND_MAKE_MEM_DEFINED(&cq_entry, sizeof(cq_entry));

	ret = efa_dgram_cq_readfrom(&cq->cq_fid, cq_entry, efa_dgram_cq_PROGRESS_ENTRIES,
			      (flags & FI_SOURCE) ? src_addr : NULL);
	if (ret == -FI_EAGAIN)
		return;

	if (OFI_UNLIKELY(ret < 0)) {
		if (OFI_UNLIKELY(ret != -FI_EAVAIL)) {
			EFA_WARN(FI_LOG_CQ, "no error available errno: %ld\n", ret);
			efa_base_ep_write_eq_error(&ep->base_ep, -ret, FI_EFA_ERR_DGRAM_CQ_READ);
			return;
		}

		err = efa_dgram_cq_readerr(&cq->cq_fid, &cq_err_entry, flags);
		if (OFI_UNLIKELY(err < 0)) {
			EFA_WARN(FI_LOG_CQ, "unable to read error entry errno: %ld\n", err);
			efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, cq_err_entry.prov_errno);
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
				((uint8_t *)temp_cq_entry + efa_dgram_cq->entry_size);
	}
	return;
}

void efa_dgram_ep_progress(struct util_ep *ep)
{
	struct efa_dgram_ep *efa_dgram_ep;
	struct efa_dgram_cq *rcq;
	struct efa_dgram_cq *scq;

	efa_dgram_ep = container_of(ep, struct efa_dgram_ep, base_ep.util_ep);
	rcq = efa_dgram_ep->rcq;
	scq = efa_dgram_ep->scq;

	ofi_genlock_lock(&ep->lock);

	if (rcq)
		efa_dgram_ep_progress_internal(efa_dgram_ep, rcq);

	if (scq && scq != rcq)
		efa_dgram_ep_progress_internal(efa_dgram_ep, scq);

	ofi_genlock_unlock(&ep->lock);
}

static struct fi_ops_atomic efa_dgram_ep_atomic_ops = {
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

struct fi_ops_cm efa_dgram_ep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = efa_base_ep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

int efa_dgram_ep_open(struct fid_domain *domain_fid, struct fi_info *user_info,
		struct fid_ep **ep_fid, void *context)
{
	struct efa_domain *domain;
	const struct fi_info *prov_info;
	struct efa_dgram_ep *ep;
	int ret;

	domain = container_of(domain_fid, struct efa_domain,
			      util_domain.domain_fid);

	if (!user_info || !user_info->ep_attr || !user_info->domain_attr ||
	    strncmp(domain->device->ibv_ctx->device->name, user_info->domain_attr->name,
		    strlen(domain->device->ibv_ctx->device->name))) {
		EFA_INFO(FI_LOG_DOMAIN, "Invalid info->domain_attr->name\n");
		return -FI_EINVAL;
	}

	prov_info = efa_domain_get_prov_info(domain, user_info->ep_attr->type);
	assert(prov_info);

	assert(user_info->ep_attr);
	ret = ofi_check_ep_attr(&efa_util_prov, user_info->fabric_attr->api_version, prov_info, user_info);
	if (ret)
		return ret;

	if (user_info->tx_attr) {
		ret = ofi_check_tx_attr(&efa_prov, prov_info->tx_attr,
					user_info->tx_attr, user_info->mode);
		if (ret)
			return ret;
	}

	if (user_info->rx_attr) {
		ret = ofi_check_rx_attr(&efa_prov, prov_info, user_info->rx_attr, user_info->mode);
		if (ret)
			return ret;
	}

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = efa_base_ep_construct(&ep->base_ep, domain_fid, user_info, efa_dgram_ep_progress, context);
	if (ret)
		goto err_ep_destroy;

	/* struct efa_send_wr and efa_recv_wr allocates memory for 2 IOV
	 * So check with an assert statement that iov_limit is 2 or less
	 */
	assert(user_info->tx_attr->iov_limit <= 2);

	ret = ofi_bufpool_create(&ep->send_wr_pool,
		sizeof(struct efa_send_wr), 16, 0, 1024, 0);
	if (ret)
		goto err_ep_destroy;

	ret = ofi_bufpool_create(&ep->recv_wr_pool,
		sizeof(struct efa_recv_wr), 16, 0, 1024, 0);
	if (ret)
		goto err_send_wr_destroy;

	ep->base_ep.domain = domain;

	*ep_fid = &ep->base_ep.util_ep.ep_fid;
	(*ep_fid)->fid.fclass = FI_CLASS_EP;
	(*ep_fid)->fid.context = context;
	(*ep_fid)->fid.ops = &efa_dgram_ep_ops;
	(*ep_fid)->ops = &efa_dgram_ep_base_ops;
	(*ep_fid)->msg = &efa_dgram_ep_msg_ops;
	(*ep_fid)->cm = &efa_dgram_ep_cm_ops;
	(*ep_fid)->rma = &efa_dgram_ep_rma_ops;
	(*ep_fid)->atomic = &efa_dgram_ep_atomic_ops;

	return 0;

err_send_wr_destroy:
	ofi_bufpool_destroy(ep->send_wr_pool);
err_ep_destroy:
	efa_dgram_ep_destroy(ep);
	return ret;
}
