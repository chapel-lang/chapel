/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx2.h"

#define PSMX2_EP_SET_TAGGED_OPS(suffix, msg_suffix)				\
	do {									\
		if (!send_completion && !recv_completion) {			\
			ep->ep.tagged = &psmx2_tagged_ops_no_event##suffix;	\
			FI_INFO(&psmx2_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				"and event suppression "			\
				msg_suffix					\
				"\n");						\
		} else if (!send_completion) {					\
			ep->ep.tagged = &psmx2_tagged_ops_no_send_event##suffix;\
			FI_INFO(&psmx2_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				"and send event suppression "			\
				msg_suffix					\
				"\n");						\
		} else if (!recv_completion) {					\
			ep->ep.tagged = &psmx2_tagged_ops_no_recv_event##suffix;\
			FI_INFO(&psmx2_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				"and recv event suppression "			\
				msg_suffix					\
				"\n");						\
		} else {							\
			ep->ep.tagged = &psmx2_tagged_ops_no_flag##suffix;	\
			FI_INFO(&psmx2_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				msg_suffix					\
				"\n");						\
		}								\
	} while (0)

static void psmx2_ep_optimize_ops(struct psmx2_fid_ep *ep)
{
	int send_completion;
	int recv_completion;
	uint64_t mask;

	mask = PSMX2_OP_FLAGS &
	       ~(FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE);

	if (ep->ep.tagged) {
		if (ep->tx_flags & mask & ~FI_COMPLETION || ep->rx_flags & mask & ~FI_COMPLETION) {
			ep->ep.tagged = &psmx2_tagged_ops;
			FI_INFO(&psmx2_prov, FI_LOG_EP_DATA,
				"generic tagged ops.\n");
		} else {
			send_completion = !ep->send_selective_completion || ep->tx_flags & FI_COMPLETION;
			recv_completion = !ep->recv_selective_completion || ep->rx_flags & FI_COMPLETION;

			if (ep->av && ep->av->type == FI_AV_MAP) {
				if (ep->caps & FI_DIRECTED_RECV)
					PSMX2_EP_SET_TAGGED_OPS(_directed_av_map, "and directed receive and av map");
				else
					PSMX2_EP_SET_TAGGED_OPS(_undirected_av_map, "and av map");
			} else {
				if (ep->caps & FI_DIRECTED_RECV)
					PSMX2_EP_SET_TAGGED_OPS(_directed, "and directed receive");
				else
					PSMX2_EP_SET_TAGGED_OPS(_undirected, "");
			}
		}
	}
}

DIRECT_FN
STATIC ssize_t psmx2_ep_cancel(fid_t fid, void *context)
{
	struct psmx2_fid_ep *ep;
	psm2_mq_status2_t status;
	struct fi_context *fi_context = context;
	uint64_t flags;
	struct psmx2_cq_event *event;
	int err;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);
	assert(ep->domain);
	assert(fi_context);

	switch (PSMX2_CTXT_TYPE(fi_context)) {
	case PSMX2_TRECV_CONTEXT:
		flags = FI_RECV | FI_TAGGED;
		break;
	case PSMX2_RECV_CONTEXT:
	case PSMX2_MULTI_RECV_CONTEXT:
		flags = FI_RECV | FI_MSG;
		break;
	default:
		return  -FI_EOPNOTSUPP;
	}

	err = psm2_mq_cancel((psm2_mq_req_t *)&PSMX2_CTXT_REQ(fi_context));
	if (err == PSM2_OK) {
		err = psm2_mq_test2((psm2_mq_req_t *)&PSMX2_CTXT_REQ(fi_context), &status);
		if (err == PSM2_OK && ep->recv_cq) {
			event = psmx2_cq_create_event(
					ep->recv_cq,
					status.context,
					NULL,	/* buf */
					flags,
					0,	/* len */
					0,	/* data */
					0,	/* tag */
					0	/* olen */,
					-FI_ECANCELED);
			if (event)
				psmx2_cq_enqueue_event(ep->recv_cq, event);
			else
				return -FI_ENOMEM;
		}
	}

	return psmx2_errno(err);
}

DIRECT_FN
STATIC int psmx2_ep_getopt(fid_t fid, int level, int optname,
			   void *optval, size_t *optlen)
{
	struct psmx2_fid_ep *ep;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		*(size_t *)optval = ep->min_multi_recv;
		*optlen = sizeof(size_t);
		break;

	default:
		return -FI_ENOPROTOOPT;
	}

	return 0;
}

DIRECT_FN
STATIC int psmx2_ep_setopt(fid_t fid, int level, int optname,
			   const void *optval, size_t optlen)
{
	struct psmx2_fid_ep *ep;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		ep->min_multi_recv = *(size_t *)optval;
		break;

	default:
		return -FI_ENOPROTOOPT;
	}

	return 0;
}

static void psmx2_ep_close_internal(struct psmx2_fid_ep *ep)
{
	psmx2_domain_release(ep->domain);
	PSMX2_EP_FINI_OP_CONTEXT(ep);
	free(ep);
}

static int psmx2_ep_close(fid_t fid)
{
	struct psmx2_fid_ep *ep;
	struct psmx2_ep_name ep_name;
	int usage_flags = 0;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);

	if (ep->base_ep) {
		ofi_atomic_dec32(&ep->base_ep->ref);
		return 0;
	}

	if (ofi_atomic_get32(&ep->ref))
		return -FI_EBUSY;

	if (ep->stx)
		ofi_atomic_dec32(&ep->stx->ref);

	if (ep->tx && !ep->stx)
		usage_flags |= PSMX2_TX;

	if (ep->rx) {
		usage_flags |= PSMX2_RX;
		ep_name.epid = ep->rx->psm2_epid;

		ofi_ns_del_local_name(&ep->domain->fabric->name_server,
				      &ep->service, &ep_name);
	}

	psmx2_trx_ctxt_free(ep->rx, usage_flags);
	psmx2_ep_close_internal(ep);
	return 0;
}

static int psmx2_poll_ctxt_match(struct slist_entry *entry, const void *arg)
{
	struct psmx2_poll_ctxt *poll_ctxt;

	poll_ctxt = container_of(entry, struct psmx2_poll_ctxt, list_entry);
	return (poll_ctxt->trx_ctxt == arg);
}

static int psmx2_add_poll_ctxt(struct slist *list, struct psmx2_trx_ctxt *trx_ctxt)
{
	struct psmx2_poll_ctxt *item;

	if (!trx_ctxt)
		return 0;

	if (!slist_empty(list) &&
	    slist_find_first_match(list, psmx2_poll_ctxt_match, trx_ctxt))
		return 0;

	item = calloc(1, sizeof(*item));
	if (!item)
		return -FI_ENOMEM;

	ofi_atomic_inc32(&trx_ctxt->poll_refcnt);
	item->trx_ctxt = trx_ctxt;
	slist_insert_tail(&item->list_entry, list);
	return 0;
}

DIRECT_FN
STATIC int psmx2_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx2_fid_ep *ep;
	struct psmx2_fid_av *av;
	struct psmx2_fid_cq *cq;
	struct psmx2_fid_cntr *cntr;
	struct psmx2_fid_stx *stx;
	int err;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);
	err = ofi_ep_bind_valid(&psmx2_prov, bfid, flags);
	if (err)
		return err;

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		return -FI_ENOSYS;

	case FI_CLASS_CQ:
		cq = container_of(bfid, struct psmx2_fid_cq, cq.fid);
		if (ep->domain != cq->domain)
			return -FI_EINVAL;
		if (flags & FI_SEND) {
			err = psmx2_add_poll_ctxt(&cq->poll_list, ep->tx);
			if (err)
				return err;
			ep->send_cq = cq;
			if (flags & FI_SELECTIVE_COMPLETION)
				ep->send_selective_completion = 1;
		}
		if (flags & FI_RECV) {
			err = psmx2_add_poll_ctxt(&cq->poll_list, ep->rx);
			if (err)
				return err;
			ep->recv_cq = cq;
			if (flags & FI_SELECTIVE_COMPLETION)
				ep->recv_selective_completion = 1;
		}
		psmx2_ep_optimize_ops(ep);
		break;

	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct psmx2_fid_cntr, cntr.fid);
		if (ep->domain != cntr->domain)
			return -FI_EINVAL;
		if (flags & (FI_SEND | FI_WRITE | FI_READ)) {
			err = psmx2_add_poll_ctxt(&cntr->poll_list, ep->tx);
			if (err)
				return err;
		}
		if (flags & (FI_RECV | FI_REMOTE_WRITE | FI_REMOTE_READ)) {
			err = psmx2_add_poll_ctxt(&cntr->poll_list, ep->rx);
			if (err)
				return err;
		}
		if (flags & FI_SEND)
			ep->send_cntr = cntr;
		if (flags & FI_RECV)
			ep->recv_cntr = cntr;
		if (flags & FI_WRITE)
			ep->write_cntr = cntr;
		if (flags & FI_READ)
			ep->read_cntr = cntr;
		if (flags & FI_REMOTE_WRITE)
			ep->remote_write_cntr = cntr;
		if (flags & FI_REMOTE_READ)
			ep->remote_read_cntr = cntr;
		break;

	case FI_CLASS_AV:
		av = container_of(bfid,
				struct psmx2_fid_av, av.fid);
		if (ep->domain != av->domain)
			return -FI_EINVAL;
		ep->av = av;
		psmx2_ep_optimize_ops(ep);
		if (ep->tx)
			psmx2_av_add_trx_ctxt(av, ep->tx);
		if (ep->rx && ep->rx != ep->tx)
			psmx2_av_add_trx_ctxt(av, ep->rx);
		break;

	case FI_CLASS_MR:
		if (!bfid->ops || !bfid->ops->bind)
			return -FI_EINVAL;
		err = bfid->ops->bind(bfid, fid, flags);
		if (err)
			return err;
		break;

	case FI_CLASS_STX_CTX:
		stx = container_of(bfid, struct psmx2_fid_stx, stx.fid);
		if (ep->domain != stx->domain)
			return -FI_EINVAL;
		if (ep->tx || ep->stx)
			return -FI_EINVAL;
		ep->tx = stx->tx;
		ep->stx = stx;
		err = psmx2_domain_enable_ep(ep->domain, ep);
		if (err)
			return err;
#if HAVE_PSM2_MQ_FP_MSG
		if (ep->caps & FI_TRIGGER)
#else
		if (ep->caps & (FI_RMA | FI_TRIGGER))
#endif
			stx->tx->am_progress = 1;
		ofi_atomic_inc32(&stx->ref);
		break;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static inline int psmx2_ep_set_flags(struct psmx2_fid_ep *ep, uint64_t flags)
{
	uint64_t real_flags = flags & ~(FI_TRANSMIT | FI_RECV);

	if ((flags & FI_TRANSMIT) && (flags & FI_RECV))
		return -EINVAL;
	else if (flags & FI_TRANSMIT)
		ep->tx_flags = real_flags;
	else if (flags & FI_RECV)
		ep->rx_flags = real_flags;

	/* otherwise ok to leave the flags intact */

	return 0;
}

static inline int psmx2_ep_get_flags(struct psmx2_fid_ep *ep, uint64_t *flags)
{
	uint64_t flags_in = *flags;

	if ((flags_in & FI_TRANSMIT) && (flags_in & FI_RECV))
		return -EINVAL;
	else if (flags_in & FI_TRANSMIT)
		*flags = ep->tx_flags;
	else if (flags_in & FI_RECV)
		*flags = ep->rx_flags;
	else
		return -EINVAL;

	return 0;
}

DIRECT_FN
STATIC int psmx2_ep_control(fid_t fid, int command, void *arg)
{
	struct fi_alias *alias;
	struct psmx2_fid_ep *ep, *new_ep;
	int err;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);

	switch (command) {
	case FI_ALIAS:
		new_ep = (struct psmx2_fid_ep *) calloc(1, sizeof *ep);
		if (!new_ep)
			return -FI_ENOMEM;
		alias = arg;
		*new_ep = *ep;
		err = psmx2_ep_set_flags(new_ep, alias->flags);
		if (err) {
			free(new_ep);
			return err;
		}
		new_ep->base_ep = ep;
		ofi_atomic_inc32(&ep->ref);
		psmx2_ep_optimize_ops(new_ep);
		*alias->fid = &new_ep->ep.fid;
		break;

	case FI_SETOPSFLAG:
		err = psmx2_ep_set_flags(ep, *(uint64_t *)arg);
		if (err)
			return err;
		psmx2_ep_optimize_ops(ep);
		break;

	case FI_GETOPSFLAG:
		if (!arg)
			return -FI_EINVAL;
		err = psmx2_ep_get_flags(ep, arg);
		if (err)
			return err;
		break;

	case FI_ENABLE:
		ep->enabled = 1;
		return 0;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

DIRECT_FN
STATIC ssize_t psmx2_rx_size_left(struct fid_ep *ep)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	if (ep_priv->enabled)
		return 0x7fffffff;
	else
		return -FI_EOPBADSTATE;
}

DIRECT_FN
STATIC ssize_t psmx2_tx_size_left(struct fid_ep *ep)
{
	struct psmx2_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx2_fid_ep, ep);
	if (ep_priv->enabled)
		return 0x7fffffff;
	else
		return -FI_EOPBADSTATE;
}

static struct fi_ops psmx2_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx2_ep_close,
	.bind = psmx2_ep_bind,
	.control = psmx2_ep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep psmx2_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = psmx2_ep_cancel,
	.getopt = psmx2_ep_getopt,
	.setopt = psmx2_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = psmx2_rx_size_left,
	.tx_size_left = psmx2_tx_size_left,
};

int psmx2_ep_open_internal(struct psmx2_fid_domain *domain_priv,
			   struct fi_info *info,
			   struct psmx2_fid_ep **ep_out, void *context,
			   struct psmx2_trx_ctxt *trx_ctxt,
			   int usage_flags)
{
	struct psmx2_fid_ep *ep_priv;
	uint64_t ep_cap;
	int err = -FI_EINVAL;

	if (info)
		ep_cap = info->caps;
	else
		ep_cap = FI_TAGGED;

	ep_priv = (struct psmx2_fid_ep *) calloc(1, sizeof *ep_priv);
	if (!ep_priv) {
		err = -FI_ENOMEM;
		goto errout;
	}

	ep_priv->ep.fid.fclass = FI_CLASS_EP;
	ep_priv->ep.fid.context = context;
	ep_priv->ep.fid.ops = &psmx2_fi_ops;
	ep_priv->ep.ops = &psmx2_ep_ops;
	ep_priv->ep.cm = &psmx2_cm_ops;
	ep_priv->domain = domain_priv;
	if (usage_flags & PSMX2_RX) {
		ep_priv->rx = trx_ctxt;
		if (trx_ctxt)
			trx_ctxt->ep = ep_priv; /* only used by RMA target */
	}
	if (usage_flags & PSMX2_TX)
		ep_priv->tx = trx_ctxt;
	ofi_atomic_initialize32(&ep_priv->ref, 0);

	PSMX2_CTXT_TYPE(&ep_priv->nocomp_send_context) = PSMX2_NOCOMP_SEND_CONTEXT;
	PSMX2_CTXT_EP(&ep_priv->nocomp_send_context) = ep_priv;
	PSMX2_CTXT_TYPE(&ep_priv->nocomp_tsend_context) = PSMX2_NOCOMP_TSEND_CONTEXT;
	PSMX2_CTXT_EP(&ep_priv->nocomp_tsend_context) = ep_priv;

	if (ep_cap & FI_TAGGED)
		ep_priv->ep.tagged = &psmx2_tagged_ops;
	if (ep_cap & FI_MSG)
		ep_priv->ep.msg = &psmx2_msg_ops;
	if (ep_cap & FI_RMA)
		ep_priv->ep.rma = &psmx2_rma_ops;
	if (ep_cap & FI_ATOMICS)
		ep_priv->ep.atomic = &psmx2_atomic_ops;

	ep_priv->caps = ep_cap;

	err = psmx2_domain_enable_ep(domain_priv, ep_priv);
	if (err)
		goto errout_free_ep;

	psmx2_domain_acquire(domain_priv);

	if (info) {
		if (info->tx_attr)
			ep_priv->tx_flags = info->tx_attr->op_flags;
		if (info->rx_attr)
			ep_priv->rx_flags = info->rx_attr->op_flags;
	}

	psmx2_ep_optimize_ops(ep_priv);

	PSMX2_EP_INIT_OP_CONTEXT(ep_priv);
#if HAVE_PSM2_MQ_FP_MSG
	if ((ep_cap & FI_TRIGGER) && trx_ctxt)
#else
	if ((ep_cap & (FI_RMA | FI_TRIGGER)) && trx_ctxt)
#endif
		trx_ctxt->am_progress = 1;

	*ep_out = ep_priv;
	return 0;

errout_free_ep:
	free(ep_priv);

errout:
	return err;
}

DIRECT_FN
int psmx2_ep_open(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context)
{
	struct psmx2_fid_domain *domain_priv;
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_ep_name ep_name;
	struct psmx2_ep_name *src_addr;
	struct psmx2_trx_ctxt *trx_ctxt = NULL;
	int err = -FI_EINVAL;
	int usage_flags = PSMX2_TX_RX;
	uint8_t *uuid = NULL;

	domain_priv = container_of(domain, struct psmx2_fid_domain,
				   util_domain.domain_fid.fid);
	if (!domain_priv)
		goto errout;

	if (info && info->ep_attr &&
	    info->ep_attr->rx_ctx_cnt == FI_SHARED_CONTEXT)
		return  -FI_ENOSYS;

	if (info && info->ep_attr &&
	    info->ep_attr->tx_ctx_cnt == FI_SHARED_CONTEXT)
		usage_flags &= ~PSMX2_TX;

	if (info && !ofi_send_allowed(info->caps) &&
	    !ofi_rma_initiate_allowed(info->caps))
		usage_flags &= ~PSMX2_TX;

	if (info && !ofi_recv_allowed(info->caps) &&
	    !ofi_rma_target_allowed(info->caps))
		usage_flags &= ~PSMX2_RX;

	src_addr = NULL;
	if (info && info->src_addr) {
		if (info->addr_format == FI_ADDR_STR)
			src_addr = psmx2_string_to_ep_name(info->src_addr);
		else
			src_addr = info->src_addr;
	}

	if (info && info->ep_attr && info->ep_attr->auth_key) {
		if (info->ep_attr->auth_key_size != sizeof(psm2_uuid_t)) {
			FI_WARN(&psmx2_prov, FI_LOG_EP_CTRL,
				"Invalid auth_key_len %"PRIu64
				", should be %"PRIu64".\n",
				info->ep_attr->auth_key_size,
				sizeof(psm2_uuid_t));
			goto errout;
		}
		uuid = info->ep_attr->auth_key;
	}

	if (usage_flags) {
		trx_ctxt = psmx2_trx_ctxt_alloc(domain_priv, src_addr, -1,
						usage_flags, uuid);
		if (!trx_ctxt)
			goto errout;
	} else {
		FI_INFO(&psmx2_prov, FI_LOG_EP_CTRL,
			"Tx only endpoint with STX context.\n");
	}

	err = psmx2_ep_open_internal(domain_priv, info, &ep_priv, context,
				     trx_ctxt, usage_flags);
	if (err)
		goto errout_free_ctxt;

	ep_priv->type = PSMX2_EP_REGULAR;
	ep_priv->service = PSMX2_ANY_SERVICE;
	if (src_addr) {
		ep_priv->service = src_addr->service;
		if (info->addr_format == FI_ADDR_STR)
			free(src_addr);
	}

	if (ep_priv->service == PSMX2_ANY_SERVICE)
		ep_priv->service = ((getpid() & 0x7FFF) << 16) +
				   ((uintptr_t)ep_priv & 0xFFFF);

	if (usage_flags) {
		ep_name.epid = trx_ctxt->psm2_epid;
		ep_name.type = ep_priv->type;

		ofi_ns_add_local_name(&domain_priv->fabric->name_server,
				      &ep_priv->service, &ep_name);
	}

	*ep = &ep_priv->ep;
	return 0;

errout_free_ctxt:
	psmx2_trx_ctxt_free(trx_ctxt, usage_flags);

errout:
	return err;
}

/*
 * Shared tx context
 */

static int psmx2_stx_close(fid_t fid)
{
	struct psmx2_fid_stx *stx;

	stx = container_of(fid, struct psmx2_fid_stx, stx.fid);

	if (ofi_atomic_get32(&stx->ref))
		return -FI_EBUSY;

	psmx2_trx_ctxt_free(stx->tx, PSMX2_TX);
	psmx2_domain_release(stx->domain);
	free(stx);
	return 0;
}

static struct fi_ops psmx2_fi_ops_stx = {
	.size = sizeof(struct fi_ops),
	.close = psmx2_stx_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep psmx2_stx_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

DIRECT_FN
int psmx2_stx_ctx(struct fid_domain *domain, struct fi_tx_attr *attr,
		  struct fid_stx **stx, void *context)
{
	struct psmx2_fid_domain *domain_priv;
	struct psmx2_trx_ctxt *trx_ctxt;
	struct psmx2_fid_stx *stx_priv;
	int err = -FI_EINVAL;

	domain_priv = container_of(domain, struct psmx2_fid_domain,
				   util_domain.domain_fid.fid);
	if (!domain_priv)
		goto errout;

	stx_priv = (struct psmx2_fid_stx *) calloc(1, sizeof *stx_priv);
	if (!stx_priv) {
		err = -FI_ENOMEM;
		goto errout;
	}

	/* no auth_key is provided, use NULL to pick the default uuid */
	trx_ctxt = psmx2_trx_ctxt_alloc(domain_priv, NULL, -1, PSMX2_TX,
					NULL);
	if (!trx_ctxt) {
		err = -FI_ENOMEM;
		goto errout_free_stx;
	}

	psmx2_domain_acquire(domain_priv);
	stx_priv->stx.fid.fclass = FI_CLASS_STX_CTX;
	stx_priv->stx.fid.context = context;
	stx_priv->stx.fid.ops = &psmx2_fi_ops_stx;
	stx_priv->stx.ops = &psmx2_stx_ops;
	stx_priv->domain = domain_priv;
	stx_priv->tx = trx_ctxt;
	ofi_atomic_initialize32(&stx_priv->ref, 0);

	*stx = &stx_priv->stx;
	return 0;

errout_free_stx:
	free(stx_priv);

errout:
	return err;
}

/*
 * Scalable endpoint
 */

static int psmx2_sep_close(fid_t fid)
{
	struct psmx2_fid_sep *sep;
	struct psmx2_ep_name ep_name;
	int i;

	sep = container_of(fid, struct psmx2_fid_sep, ep.fid);

	if (ofi_atomic_get32(&sep->ref))
		return -FI_EBUSY;

	for (i = 0; i < sep->ctxt_cnt; i++) {
		if (sep->ctxts[i].ep && ofi_atomic_get32(&sep->ctxts[i].ep->ref))
			return -FI_EBUSY;
	}

	ep_name.epid = sep->ctxts[0].trx_ctxt->psm2_epid;
	ep_name.sep_id = sep->id;
	ep_name.type = sep->type;

	ofi_ns_del_local_name(&sep->domain->fabric->name_server,
			      &sep->service, &ep_name);

	for (i = 0; i < sep->ctxt_cnt; i++) {
		psmx2_trx_ctxt_free(sep->ctxts[i].trx_ctxt, PSMX2_TX_RX);

		if (sep->ctxts[i].ep)
			psmx2_ep_close_internal(sep->ctxts[i].ep);
	}

	sep->domain->sep_lock_fn(&sep->domain->sep_lock, 1);
	dlist_remove(&sep->entry);
	sep->domain->sep_unlock_fn(&sep->domain->sep_lock, 1);

	psmx2_domain_release(sep->domain);
	free(sep);
	return 0;
}

static int psmx2_sep_control(fid_t fid, int command, void *arg)
{
	struct psmx2_fid_sep *sep;

	sep = container_of(fid, struct psmx2_fid_sep, ep.fid);

	switch (command) {
	case FI_ENABLE:
		sep->enabled = 1;
		return 0;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

DIRECT_FN
STATIC int psmx2_sep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx2_fid_sep *sep;
	int i, err = 0;

	sep = container_of(fid, struct psmx2_fid_sep, ep.fid);

	for (i = 0; i < sep->ctxt_cnt; i++) {
		err = psmx2_ep_bind(&sep->ctxts[i].ep->ep.fid, bfid, flags);
		if (err)
			break;
	}

	return err;
}

DIRECT_FN
STATIC int psmx2_tx_context(struct fid_ep *ep, int index, struct fi_tx_attr *attr,
			    struct fid_ep **tx_ep, void *context)
{
	struct psmx2_fid_sep *sep;

	sep = container_of(ep, struct psmx2_fid_sep, ep);

	assert(index >= 0 && index < sep->ctxt_cnt);

	*tx_ep = &sep->ctxts[index].ep->ep;
	return 0;
}

DIRECT_FN
STATIC int psmx2_rx_context(struct fid_ep *ep, int index, struct fi_rx_attr *attr,
			    struct fid_ep **rx_ep, void *context)
{
	struct psmx2_fid_sep *sep;

	sep = container_of(ep, struct psmx2_fid_sep, ep);

	assert(index >= 0 && index < sep->ctxt_cnt);

	*rx_ep = &sep->ctxts[index].ep->ep;
	return 0;
}

static int psmx2_sep_ctxt_close(fid_t fid)
{
	struct psmx2_fid_ep *ep;

	ep = container_of(fid, struct psmx2_fid_ep, ep.fid);

	if (ep->base_ep)
		ofi_atomic_dec32(&ep->base_ep->ref);

	return 0;
}

static struct fi_ops psmx2_fi_ops_sep_ctxt = {
	.size = sizeof(struct fi_ops),
	.close = psmx2_sep_ctxt_close,
	.bind = psmx2_ep_bind,
	.control = psmx2_ep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops psmx2_fi_ops_sep = {
	.size = sizeof(struct fi_ops),
	.close = psmx2_sep_close,
	.bind = psmx2_sep_bind,
	.control = psmx2_sep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep psmx2_sep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = psmx2_tx_context,
	.rx_ctx = psmx2_rx_context,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

DIRECT_FN
int psmx2_sep_open(struct fid_domain *domain, struct fi_info *info,
		   struct fid_ep **sep, void *context)
{
	struct psmx2_fid_domain *domain_priv;
	struct psmx2_fid_ep *ep_priv;
	struct psmx2_fid_sep *sep_priv;
	struct psmx2_ep_name ep_name;
	struct psmx2_ep_name *src_addr;
	struct psmx2_trx_ctxt *trx_ctxt;
	size_t ctxt_cnt = 1;
	size_t ctxt_size;
	int err = -FI_EINVAL;
	uint8_t *uuid = NULL;
	int i;

	domain_priv = container_of(domain, struct psmx2_fid_domain,
				   util_domain.domain_fid.fid);
	if (!domain_priv)
		goto errout;

	if (info && info->ep_attr) {
		if (info->ep_attr->auth_key_size != sizeof(psm2_uuid_t)) {
			FI_WARN(&psmx2_prov, FI_LOG_EP_CTRL,
				"Invalid auth_key_len %"PRIu64
				", should be %"PRIu64".\n",
				info->ep_attr->auth_key_size,
				sizeof(psm2_uuid_t));
			goto errout;
		}
		uuid = info->ep_attr->auth_key;

		if (info->ep_attr->tx_ctx_cnt > psmx2_hfi_info.max_trx_ctxt) {
			FI_WARN(&psmx2_prov, FI_LOG_EP_CTRL,
				"tx_ctx_cnt %"PRIu64" exceed limit %d.\n",
				info->ep_attr->tx_ctx_cnt,
				psmx2_hfi_info.max_trx_ctxt);
			goto errout;
		}
		if (info->ep_attr->rx_ctx_cnt > psmx2_hfi_info.max_trx_ctxt) {
			FI_WARN(&psmx2_prov, FI_LOG_EP_CTRL,
				"rx_ctx_cnt %"PRIu64" exceed limit %d.\n",
				info->ep_attr->rx_ctx_cnt,
				psmx2_hfi_info.max_trx_ctxt);
			goto errout;
		}
		ctxt_cnt = info->ep_attr->tx_ctx_cnt;
		if (ctxt_cnt < info->ep_attr->rx_ctx_cnt)
			ctxt_cnt = info->ep_attr->rx_ctx_cnt;
		if (ctxt_cnt == 0) {
			FI_INFO(&psmx2_prov, FI_LOG_EP_CTRL,
				"tx_ctx_cnt and rx_ctx_cnt are 0, use 1.\n");
			ctxt_cnt = 1;
		}
	}

	ctxt_size = ctxt_cnt * sizeof(struct psmx2_sep_ctxt);
	sep_priv = (struct psmx2_fid_sep *) calloc(1, sizeof(*sep_priv) + ctxt_size);
	if (!sep_priv) {
		err = -FI_ENOMEM;
		goto errout;
	}

	sep_priv->ep.fid.fclass = FI_CLASS_SEP;
	sep_priv->ep.fid.context = context;
	sep_priv->ep.fid.ops = &psmx2_fi_ops_sep;
	sep_priv->ep.ops = &psmx2_sep_ops;
	sep_priv->ep.cm = &psmx2_cm_ops;
	sep_priv->domain = domain_priv;
	sep_priv->ctxt_cnt = ctxt_cnt;
	ofi_atomic_initialize32(&sep_priv->ref, 0);
 
	src_addr = NULL;
	if (info && info->src_addr) {
		if (info->addr_format == FI_ADDR_STR)
			src_addr = psmx2_string_to_ep_name(info->src_addr);
		else
			src_addr = info->src_addr;
	}

	for (i = 0; i < ctxt_cnt; i++) {
		trx_ctxt = psmx2_trx_ctxt_alloc(domain_priv, src_addr,
						(ctxt_cnt > 1) ? i : -1,
						PSMX2_TX_RX, uuid);
		if (!trx_ctxt) {
			err = -FI_ENOMEM;
			goto errout_free_ctxt;
		}

		sep_priv->ctxts[i].trx_ctxt = trx_ctxt;

		err = psmx2_ep_open_internal(domain_priv, info, &ep_priv, context,
					     trx_ctxt, PSMX2_TX_RX);
		if (err)
			goto errout_free_ctxt;

		/* override the ops so the fid can't be closed individually */
		ep_priv->ep.fid.ops = &psmx2_fi_ops_sep_ctxt;

		sep_priv->ctxts[i].ep = ep_priv;
	}

	sep_priv->type = PSMX2_EP_SCALABLE;
	sep_priv->service = PSMX2_ANY_SERVICE;
	if (src_addr) {
		sep_priv->service = src_addr->service;
		if (info->addr_format == FI_ADDR_STR)
			free(src_addr);
	}

	if (sep_priv->service == PSMX2_ANY_SERVICE)
		sep_priv->service = ((getpid() & 0x7FFF) << 16) +
				   ((uintptr_t)sep_priv & 0xFFFF);

	sep_priv->id = ofi_atomic_inc32(&domain_priv->sep_cnt);
	for (i = 0; i < ctxt_cnt; i++)
		sep_priv->ctxts[i].ep->sep_id = sep_priv->id;

	domain_priv->sep_lock_fn(&domain_priv->sep_lock, 1);
	dlist_insert_before(&sep_priv->entry, &domain_priv->sep_list);
	domain_priv->sep_unlock_fn(&domain_priv->sep_lock, 1);

	ep_name.epid = sep_priv->ctxts[0].trx_ctxt->psm2_epid;
	ep_name.sep_id = sep_priv->id;
	ep_name.type = sep_priv->type;

	ofi_ns_add_local_name(&domain_priv->fabric->name_server,
			      &sep_priv->service, &ep_name);

	psmx2_domain_acquire(domain_priv);
	*sep = &sep_priv->ep;

	/* Make sure the AM handler is installed to answer SEP query */
	psmx2_am_init(sep_priv->ctxts[0].trx_ctxt);

	return 0;

errout_free_ctxt:
	while (i) {
		if (sep_priv->ctxts[i].trx_ctxt)
			psmx2_trx_ctxt_free(sep_priv->ctxts[i].trx_ctxt,
					    PSMX2_TX_RX);

		if (sep_priv->ctxts[i].ep)
			psmx2_ep_close_internal(sep_priv->ctxts[i].ep);

		i--;
	}

	free(sep_priv);

errout:
	return err;
}
