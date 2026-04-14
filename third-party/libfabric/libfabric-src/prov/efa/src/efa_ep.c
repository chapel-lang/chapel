/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "config.h"
#include "efa.h"
#include "efa_av.h"
#include "efa_cq.h"

#include <infiniband/efadv.h>

extern struct fi_ops_msg efa_msg_ops;
extern struct fi_ops_rma efa_rma_ops;

static int efa_ep_getopt(fid_t fid, int level, int optname,
			 void *optval, size_t *optlen)
{
	struct efa_base_ep *ep;

	ep = container_of(fid, struct efa_base_ep, util_ep.ep_fid.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_EFA_RNR_RETRY:
		if (*optlen < sizeof(size_t))
			return -FI_ETOOSMALL;
		*(size_t *)optval = ep->rnr_retry;
		*optlen = sizeof(size_t);
		break;
	/* p2p is required for efa direct ep */
	case FI_OPT_FI_HMEM_P2P:
		if (*optlen < sizeof(int))
			return -FI_ETOOSMALL;
		*(int *)optval = FI_HMEM_P2P_REQUIRED;
		*optlen = sizeof(int);
		break;
	case FI_OPT_MAX_MSG_SIZE:
		if (*optlen < sizeof (size_t))
			return -FI_ETOOSMALL;
		*(size_t *) optval = ep->max_msg_size;
		*optlen = sizeof (size_t);
		break;
	case FI_OPT_MAX_RMA_SIZE:
		if (*optlen < sizeof (size_t))
			return -FI_ETOOSMALL;
		*(size_t *) optval = ep->max_rma_size;
		*optlen = sizeof (size_t);
		break;
	case FI_OPT_INJECT_MSG_SIZE:
		if (*optlen < sizeof (size_t))
			return -FI_ETOOSMALL;
		*(size_t *) optval = ep->inject_msg_size;
		*optlen = sizeof (size_t);
		break;
	case FI_OPT_INJECT_RMA_SIZE:
		if (*optlen < sizeof (size_t))
			return -FI_ETOOSMALL;
		*(size_t *) optval = ep->inject_rma_size;
		*optlen = sizeof (size_t);
		break;
	/* Emulated read/write is NOT used for efa direct ep */
	case FI_OPT_EFA_EMULATED_READ: /* fall through */
	case FI_OPT_EFA_EMULATED_WRITE:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = false;
		*optlen = sizeof(bool);
		break;
	default:
		EFA_INFO(FI_LOG_EP_CTRL, "Unknown / unsupported endpoint option\n");
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

static int efa_ep_setopt(fid_t fid, int level, int optname, const void *optval, size_t optlen)
{
	int ret, intval;
	struct efa_base_ep *ep;

	ep = container_of(fid, struct efa_base_ep, util_ep.ep_fid.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_EFA_RNR_RETRY:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		/*
		 * Application is required to call to fi_setopt before EP
		 * enabled. If it's calling to fi_setopt after EP enabled,
		 * fail the call.
		 *
		 * efa_ep->qp will be NULL before EP enabled, use it to check
		 * if the call to fi_setopt is before or after EP enabled for
		 * convience, instead of calling to ibv_query_qp
		 */
		if (ep->efa_qp_enabled) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"The option FI_OPT_EFA_RNR_RETRY is required "
				"to be set before EP enabled\n");
			return -FI_EINVAL;
		}

		if (!efa_domain_support_rnr_retry_modify(ep->domain)) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"RNR capability is not supported\n");
			return -FI_ENOSYS;
		}
		ep->rnr_retry = *(size_t *)optval;
		break;
	case FI_OPT_FI_HMEM_P2P:
		if (optlen != sizeof(int))
			return -FI_EINVAL;

		intval = *(int *)optval;

		if (intval == FI_HMEM_P2P_DISABLED) {
			EFA_WARN(FI_LOG_EP_CTRL, "p2p is required by implementation\n");
			return -FI_EOPNOTSUPP;
		}
		break;
	case FI_OPT_MAX_MSG_SIZE:
		EFA_EP_SETOPT_THRESHOLD(MAX_MSG_SIZE, ep->max_msg_size, (size_t) ep->domain->device->ibv_port_attr.max_msg_sz)
		break;
	case FI_OPT_MAX_RMA_SIZE:
		EFA_EP_SETOPT_THRESHOLD(MAX_RMA_SIZE, ep->max_rma_size, (size_t) ep->domain->device->max_rdma_size)
		break;
	case FI_OPT_INJECT_MSG_SIZE:
		EFA_EP_SETOPT_THRESHOLD(INJECT_MSG_SIZE, ep->inject_msg_size, (size_t) ep->domain->device->efa_attr.inline_buf_size)
		break;
	case FI_OPT_INJECT_RMA_SIZE:
		EFA_EP_SETOPT_THRESHOLD(INJECT_RMA_SIZE, ep->inject_rma_size, (size_t) 0)
		break;
	/* no op as efa direct ep will not use cuda api and shm in data transfer */
	case FI_OPT_CUDA_API_PERMITTED: /* fall through */
	case FI_OPT_SHARED_MEMORY_PERMITTED:
		break;
	/* no op as efa direct ep will always use rdma for rma operations in data transfer */
	case FI_OPT_EFA_USE_DEVICE_RDMA:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		if (!(*(bool *)optval) && (ep->info->caps & FI_RMA)) {
			EFA_WARN(FI_LOG_EP_CTRL, "Device rdma is required for rma operations\n");
			return -FI_EOPNOTSUPP;
		}
		break;
	case FI_OPT_EFA_SENDRECV_IN_ORDER_ALIGNED_128_BYTES:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		if (*(bool *)optval) {
			ret = efa_base_ep_check_qp_in_order_aligned_128_bytes(ep, IBV_WR_SEND);
			if (ret)
				return ret;
		}
		break;
	case FI_OPT_EFA_WRITE_IN_ORDER_ALIGNED_128_BYTES:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		if (*(bool *)optval) {
			ret = efa_base_ep_check_qp_in_order_aligned_128_bytes(ep, IBV_WR_RDMA_WRITE);
			if (ret)
				return ret;
		}
		break;
	/* no op as efa direct ep will not handshake with peers */
	case FI_OPT_EFA_HOMOGENEOUS_PEERS:
		break;
	default:
		EFA_INFO(FI_LOG_EP_CTRL, "Unknown / unsupported endpoint option\n");
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
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

static int efa_ep_close(fid_t fid)
{
	struct efa_base_ep *ep;
	int ret;

	ep = container_of(fid, struct efa_base_ep, util_ep.ep_fid.fid);

	/* We need to free the util_ep first to avoid race conditions
	 * with other threads progressing the cntr. */
	efa_base_ep_close_util_ep(ep);

	efa_base_ep_remove_cntr_ibv_cq_poll_list(ep);

	ret = efa_base_ep_destruct(ep);
	if (ret) {
		EFA_WARN(FI_LOG_EP_CTRL, "Unable to close base endpoint\n");
	}

	free(ep);

	return 0;
}

static int efa_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct efa_base_ep *ep;
	struct efa_cq *cq;
	struct efa_av *av;
	struct efa_domain *efa_domain;
	struct util_eq *eq;
	struct util_cntr *cntr;
	int ret;

	ep = container_of(fid, struct efa_base_ep, util_ep.ep_fid.fid);
	ret = ofi_ep_bind_valid(&efa_prov, bfid, flags);
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		/* Must bind a CQ to either RECV or SEND completions */
		if (!(flags & (FI_RECV | FI_TRANSMIT)))
			return -FI_EBADFLAGS;

		cq = container_of(bfid, struct efa_cq, util_cq.cq_fid);
		efa_domain = container_of(cq->util_cq.domain, struct efa_domain, util_domain);
		if (ep->domain != efa_domain)
			return -FI_EINVAL;

		ret = ofi_ep_bind_cq(&ep->util_ep, &cq->util_cq, flags);
		if (ret)
			return ret;

		break;
	case FI_CLASS_AV:
		av = container_of(bfid, struct efa_av, util_av.av_fid.fid);
		/* Bind util provider endpoint and av */
		ret = ofi_ep_bind_av(&ep->util_ep, &av->util_av);
		if (ret)
			return ret;

		ret = efa_base_ep_bind_av(ep, av);
		if (ret)
			return ret;
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
		EFA_WARN(FI_LOG_EP_CTRL, "invalid fid class\n");
		return -EINVAL;
	}

	return 0;
}

static int efa_ep_getflags(struct fid_ep *ep_fid, uint64_t *flags)
{
	struct efa_base_ep *ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
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
	struct efa_base_ep *ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
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
	struct efa_base_ep *ep;
	int err;

	ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);

	err = efa_base_ep_create_and_enable_qp(ep, false);
	if (err)
		return err;

	err = efa_base_ep_insert_cntr_ibv_cq_poll_list(ep);
	if (err)
		efa_base_ep_destruct_qp(ep);

	return err;
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

/**
 * @brief progress engine for the EFA dgram endpoint
 *
 * This function now a no-op.
 *
 * @param[in] util_ep The endpoint FID to progress
 */
static
void efa_ep_progress_no_op(struct util_ep *util_ep)
{
	return;
}

static struct fi_ops_atomic efa_atomic_ops = {
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

struct fi_ops_cm efa_ep_cm_ops = {
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

int efa_ep_open(struct fid_domain *domain_fid, struct fi_info *user_info,
		struct fid_ep **ep_fid, void *context)
{
	struct efa_base_ep *ep;
	int ret;

	ep = calloc(1, sizeof(*ep));
	if (!ep)
		return -FI_ENOMEM;

	ret = efa_base_ep_construct(ep, domain_fid, user_info, efa_ep_progress_no_op, context);
	if (ret)
		goto err_ep_destroy;

	*ep_fid = &ep->util_ep.ep_fid;
	(*ep_fid)->fid.fclass = FI_CLASS_EP;
	(*ep_fid)->fid.context = context;
	(*ep_fid)->fid.ops = &efa_ep_ops;
	(*ep_fid)->ops = &efa_ep_base_ops;
	(*ep_fid)->msg = &efa_msg_ops;
	(*ep_fid)->cm = &efa_ep_cm_ops;
	(*ep_fid)->rma = &efa_rma_ops;
	(*ep_fid)->atomic = &efa_atomic_ops;

	return 0;

err_ep_destroy:
	efa_base_ep_destruct(ep);
	if (ep)
		free(ep);
	return ret;
}
