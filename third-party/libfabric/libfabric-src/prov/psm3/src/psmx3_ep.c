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

#include "psmx3.h"

#define PSMX3_EP_SET_TAGGED_OPS(suffix, msg_suffix)				\
	do {									\
		if (!send_completion && !recv_completion) {			\
			ep->ep.tagged = &psmx3_tagged_ops_no_event##suffix;	\
			PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				"and event suppression "			\
				msg_suffix					\
				"\n");						\
		} else if (!send_completion) {					\
			ep->ep.tagged = &psmx3_tagged_ops_no_send_event##suffix;\
			PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				"and send event suppression "			\
				msg_suffix					\
				"\n");						\
		} else if (!recv_completion) {					\
			ep->ep.tagged = &psmx3_tagged_ops_no_recv_event##suffix;\
			PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				"and recv event suppression "			\
				msg_suffix					\
				"\n");						\
		} else {							\
			ep->ep.tagged = &psmx3_tagged_ops_no_flag##suffix;	\
			PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,			\
				"tagged ops optimized for op_flags=0 "		\
				msg_suffix					\
				"\n");						\
		}								\
	} while (0)

static void psmx3_ep_optimize_ops(struct psmx3_fid_ep *ep)
{
	int send_completion;
	int recv_completion;
	uint64_t mask;

	mask = PSMX3_OP_FLAGS &
	       ~(FI_INJECT_COMPLETE | FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE);

	if (ep->ep.tagged) {
		if (ep->tx_flags & mask & ~FI_COMPLETION || ep->rx_flags & mask & ~FI_COMPLETION) {
			ep->ep.tagged = &psmx3_tagged_ops;
			PSMX3_INFO(&psmx3_prov, FI_LOG_EP_DATA,
				"generic tagged ops.\n");
		} else {
			send_completion = !ep->send_selective_completion || ep->tx_flags & FI_COMPLETION;
			recv_completion = !ep->recv_selective_completion || ep->rx_flags & FI_COMPLETION;

			if (ep->av && ep->av->type == FI_AV_MAP) {
				if (ep->caps & FI_DIRECTED_RECV)
					PSMX3_EP_SET_TAGGED_OPS(_directed_av_map, "and directed receive and av map");
				else
					PSMX3_EP_SET_TAGGED_OPS(_undirected_av_map, "and av map");
			} else {
				if (ep->caps & FI_DIRECTED_RECV)
					PSMX3_EP_SET_TAGGED_OPS(_directed, "and directed receive");
				else
					PSMX3_EP_SET_TAGGED_OPS(_undirected, "");
			}
		}
	}
}

DIRECT_FN
STATIC ssize_t psmx3_ep_cancel(fid_t fid, void *context)
{
	struct psmx3_fid_ep *ep;
	psm2_mq_status2_t status;
	struct fi_context *fi_context = context;
	uint64_t flags;
	struct psmx3_cq_event *event;
	int err;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);
	assert(ep->domain);
	assert(fi_context);

	switch (PSMX3_CTXT_TYPE(fi_context)) {
	case PSMX3_TRECV_CONTEXT:
		flags = FI_RECV | FI_TAGGED;
		break;
	case PSMX3_RECV_CONTEXT:
	case PSMX3_MULTI_RECV_CONTEXT:
		flags = FI_RECV | FI_MSG;
		break;
	default:
		return  -FI_EOPNOTSUPP;
	}

	err = psm3_mq_cancel((psm2_mq_req_t *)&PSMX3_CTXT_REQ(fi_context));
	if (err == PSM2_OK) {
		err = psm3_mq_test2((psm2_mq_req_t *)&PSMX3_CTXT_REQ(fi_context), &status);
		if (err == PSM2_OK && ep->recv_cq) {
			event = psmx3_cq_create_event(
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
				psmx3_cq_enqueue_event(ep->recv_cq, event);
			else
				return -FI_ENOMEM;
		}
	}

	return psmx3_errno(err);
}

DIRECT_FN
STATIC int psmx3_ep_getopt(fid_t fid, int level, int optname,
			   void *optval, size_t *optlen)
{
	struct psmx3_fid_ep *ep;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);

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
STATIC int psmx3_ep_setopt(fid_t fid, int level, int optname,
			   const void *optval, size_t optlen)
{
	struct psmx3_fid_ep *ep;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);

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

static void psmx3_ep_close_internal(struct psmx3_fid_ep *ep)
{
	psmx3_domain_release(ep->domain);
	PSMX3_EP_FINI_OP_CONTEXT(ep);
	free(ep);
}

static int psmx3_ep_close(fid_t fid)
{
	struct psmx3_fid_ep *ep;
	struct psmx3_ep_name ep_name;
	int usage_flags = 0;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);

	if (ep->base_ep) {
		ofi_atomic_dec32(&ep->base_ep->ref);
		return 0;
	}

	if (ofi_atomic_get32(&ep->ref))
		return -FI_EBUSY;

	if (ep->stx)
		ofi_atomic_dec32(&ep->stx->ref);

	if (ep->tx && !ep->stx)
		usage_flags |= PSMX3_TX;

	if (ep->rx) {
		usage_flags |= PSMX3_RX;
		ep_name.epid = ep->rx->psm2_epid;

		ofi_ns_del_local_name(&ep->domain->fabric->name_server,
				      &ep->service, &ep_name);
	}

	psmx3_trx_ctxt_free(ep->rx, usage_flags);
	psmx3_ep_close_internal(ep);
	return 0;
}

static int psmx3_poll_ctxt_match(struct slist_entry *entry, const void *arg)
{
	struct psmx3_poll_ctxt *poll_ctxt;

	poll_ctxt = container_of(entry, struct psmx3_poll_ctxt, list_entry);
	return (poll_ctxt->trx_ctxt == arg);
}

static int psmx3_add_poll_ctxt(struct slist *list, struct psmx3_trx_ctxt *trx_ctxt)
{
	struct psmx3_poll_ctxt *item;

	if (!trx_ctxt)
		return 0;

	if (!slist_empty(list) &&
	    slist_find_first_match(list, psmx3_poll_ctxt_match, trx_ctxt))
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
STATIC int psmx3_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx3_fid_ep *ep;
	struct psmx3_fid_av *av;
	struct psmx3_fid_cq *cq;
	struct psmx3_fid_cntr *cntr;
	struct psmx3_fid_stx *stx;
	int err;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);
	err = ofi_ep_bind_valid(&psmx3_prov, bfid, flags);
	if (err)
		return err;

	switch (bfid->fclass) {
	case FI_CLASS_EQ:
		return -FI_ENOSYS;

	case FI_CLASS_CQ:
		cq = container_of(bfid, struct psmx3_fid_cq, cq.fid);
		if (ep->domain != cq->domain)
			return -FI_EINVAL;
		if (flags & FI_SEND) {
			err = psmx3_add_poll_ctxt(&cq->poll_list, ep->tx);
			if (err)
				return err;
			ep->send_cq = cq;
			if (flags & FI_SELECTIVE_COMPLETION)
				ep->send_selective_completion = 1;
		}
		if (flags & FI_RECV) {
			err = psmx3_add_poll_ctxt(&cq->poll_list, ep->rx);
			if (err)
				return err;
			ep->recv_cq = cq;
			if (flags & FI_SELECTIVE_COMPLETION)
				ep->recv_selective_completion = 1;
		}
		psmx3_ep_optimize_ops(ep);
		break;

	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct psmx3_fid_cntr, cntr.fid);
		if (ep->domain != cntr->domain)
			return -FI_EINVAL;
		if (flags & (FI_SEND | FI_WRITE | FI_READ)) {
			err = psmx3_add_poll_ctxt(&cntr->poll_list, ep->tx);
			if (err)
				return err;
		}
		if (flags & (FI_RECV | FI_REMOTE_WRITE | FI_REMOTE_READ)) {
			err = psmx3_add_poll_ctxt(&cntr->poll_list, ep->rx);
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
				struct psmx3_fid_av, av.fid);
		if (ep->domain != av->domain)
			return -FI_EINVAL;
		ep->av = av;
		psmx3_ep_optimize_ops(ep);
		if (ep->tx)
			psmx3_av_add_trx_ctxt(av, ep->tx);
		if (ep->rx && ep->rx != ep->tx)
			psmx3_av_add_trx_ctxt(av, ep->rx);
		break;

	case FI_CLASS_MR:
		if (!bfid->ops || !bfid->ops->bind)
			return -FI_EINVAL;
		err = bfid->ops->bind(bfid, fid, flags);
		if (err)
			return err;
		break;

	case FI_CLASS_STX_CTX:
		stx = container_of(bfid, struct psmx3_fid_stx, stx.fid);
		if (ep->domain != stx->domain)
			return -FI_EINVAL;
		if (ep->tx || ep->stx)
			return -FI_EINVAL;
		ep->tx = stx->tx;
		ep->stx = stx;
		err = psmx3_domain_enable_ep(ep->domain, ep);
		if (err)
			return err;
		if (ep->caps & FI_TRIGGER)
			stx->tx->am_progress = 1;
		ofi_atomic_inc32(&stx->ref);
		break;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static inline int psmx3_ep_set_flags(struct psmx3_fid_ep *ep, uint64_t flags)
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

static inline int psmx3_ep_get_flags(struct psmx3_fid_ep *ep, uint64_t *flags)
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
STATIC int psmx3_ep_control(fid_t fid, int command, void *arg)
{
	struct fi_alias *alias;
	struct psmx3_fid_ep *ep, *new_ep;
	int err;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);

	switch (command) {
	case FI_ALIAS:
		new_ep = (struct psmx3_fid_ep *) calloc(1, sizeof *ep);
		if (!new_ep)
			return -FI_ENOMEM;
		alias = arg;
		*new_ep = *ep;
		err = psmx3_ep_set_flags(new_ep, alias->flags);
		if (err) {
			free(new_ep);
			return err;
		}
		new_ep->base_ep = ep;
		ofi_atomic_inc32(&ep->ref);
		psmx3_ep_optimize_ops(new_ep);
		*alias->fid = &new_ep->ep.fid;
		break;

	case FI_SETOPSFLAG:
		err = psmx3_ep_set_flags(ep, *(uint64_t *)arg);
		if (err)
			return err;
		psmx3_ep_optimize_ops(ep);
		break;

	case FI_GETOPSFLAG:
		if (!arg)
			return -FI_EINVAL;
		err = psmx3_ep_get_flags(ep, arg);
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
STATIC ssize_t psmx3_rx_size_left(struct fid_ep *ep)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);
	if (ep_priv->enabled)
		return 0x7fffffff;
	else
		return -FI_EOPBADSTATE;
}

DIRECT_FN
STATIC ssize_t psmx3_tx_size_left(struct fid_ep *ep)
{
	struct psmx3_fid_ep *ep_priv;

	ep_priv = container_of(ep, struct psmx3_fid_ep, ep);
	if (ep_priv->enabled)
		return 0x7fffffff;
	else
		return -FI_EOPBADSTATE;
}

static struct fi_ops psmx3_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_ep_close,
	.bind = psmx3_ep_bind,
	.control = psmx3_ep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep psmx3_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = psmx3_ep_cancel,
	.getopt = psmx3_ep_getopt,
	.setopt = psmx3_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = psmx3_rx_size_left,
	.tx_size_left = psmx3_tx_size_left,
};

int psmx3_ep_open_internal(struct psmx3_fid_domain *domain_priv,
			   struct fi_info *info,
			   struct psmx3_fid_ep **ep_out, void *context,
			   struct psmx3_trx_ctxt *trx_ctxt,
			   int usage_flags)
{
	struct psmx3_fid_ep *ep_priv;
	uint64_t ep_cap;
	int err = -FI_EINVAL;

	if (info)
		ep_cap = info->caps;
	else
		ep_cap = FI_TAGGED;

	ep_priv = (struct psmx3_fid_ep *) calloc(1, sizeof *ep_priv);
	if (!ep_priv) {
		err = -FI_ENOMEM;
		goto errout;
	}

	ep_priv->ep.fid.fclass = FI_CLASS_EP;
	ep_priv->ep.fid.context = context;
	ep_priv->ep.fid.ops = &psmx3_fi_ops;
	ep_priv->ep.ops = &psmx3_ep_ops;
	ep_priv->ep.cm = &psmx3_cm_ops;
	ep_priv->domain = domain_priv;
	if (usage_flags & PSMX3_RX) {
		ep_priv->rx = trx_ctxt;
		if (trx_ctxt)
			trx_ctxt->ep = ep_priv; /* only used by RMA target */
	}
	if (usage_flags & PSMX3_TX)
		ep_priv->tx = trx_ctxt;
	ofi_atomic_initialize32(&ep_priv->ref, 0);

	PSMX3_CTXT_TYPE(&ep_priv->nocomp_send_context) = PSMX3_NOCOMP_SEND_CONTEXT;
	PSMX3_CTXT_EP(&ep_priv->nocomp_send_context) = ep_priv;
	PSMX3_CTXT_TYPE(&ep_priv->nocomp_tsend_context) = PSMX3_NOCOMP_TSEND_CONTEXT;
	PSMX3_CTXT_EP(&ep_priv->nocomp_tsend_context) = ep_priv;

	if (ep_cap & FI_TAGGED)
		ep_priv->ep.tagged = &psmx3_tagged_ops;
	if (ep_cap & FI_MSG)
		ep_priv->ep.msg = &psmx3_msg_ops;
	if (ep_cap & FI_RMA)
		ep_priv->ep.rma = &psmx3_rma_ops;
	if (ep_cap & FI_ATOMICS)
		ep_priv->ep.atomic = &psmx3_atomic_ops;

	ep_priv->caps = ep_cap;

	err = psmx3_domain_enable_ep(domain_priv, ep_priv);
	if (err)
		goto errout_free_ep;

	psmx3_domain_acquire(domain_priv);

	if (info) {
		if (info->tx_attr)
			ep_priv->tx_flags = info->tx_attr->op_flags;
		if (info->rx_attr)
			ep_priv->rx_flags = info->rx_attr->op_flags;
	}

	psmx3_ep_optimize_ops(ep_priv);

	PSMX3_EP_INIT_OP_CONTEXT(ep_priv);
	if ((ep_cap & FI_TRIGGER) && trx_ctxt)
		trx_ctxt->am_progress = 1;

	*ep_out = ep_priv;
	return 0;

errout_free_ep:
	free(ep_priv);

errout:
	return err;
}

DIRECT_FN
int psmx3_ep_open(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context)
{
	struct psmx3_fid_domain *domain_priv;
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_ep_name ep_name;
	struct psmx3_ep_name *src_addr;
	struct psmx3_trx_ctxt *trx_ctxt = NULL;
	int err = -FI_EINVAL;
	int usage_flags = PSMX3_TX_RX;
	uint8_t *uuid = NULL;

	domain_priv = container_of(domain, struct psmx3_fid_domain,
				   util_domain.domain_fid.fid);
	if (!domain_priv)
		goto errout;

	if (info && info->ep_attr &&
	    info->ep_attr->rx_ctx_cnt == FI_SHARED_CONTEXT)
		return  -FI_ENOSYS;

	if (info && info->ep_attr &&
	    info->ep_attr->tx_ctx_cnt == FI_SHARED_CONTEXT)
		usage_flags &= ~PSMX3_TX;

	if (info && !ofi_send_allowed(info->caps) &&
	    !ofi_rma_initiate_allowed(info->caps))
		usage_flags &= ~PSMX3_TX;

	if (info && !ofi_recv_allowed(info->caps) &&
	    !ofi_rma_target_allowed(info->caps))
		usage_flags &= ~PSMX3_RX;

	src_addr = NULL;
	if (info && info->src_addr) {
		if (info->addr_format == FI_ADDR_STR)
			src_addr = psmx3_string_to_ep_name(info->src_addr);
		else
			src_addr = info->src_addr;
	}
	if (!psmx3_override_uuid() && info && info->domain_attr && info->domain_attr->auth_key) {
		if (info->domain_attr->auth_key_size != sizeof(psm2_uuid_t)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_EP_CTRL,
				"Invalid domain auth_key_len %"PRIu64
				", should be %"PRIu64".\n",
				info->domain_attr->auth_key_size,
				sizeof(psm2_uuid_t));
			goto errout;
		}
		uuid = info->domain_attr->auth_key;
	}

	if (!psmx3_override_uuid() && info && info->ep_attr && info->ep_attr->auth_key) {
		if (info->ep_attr->auth_key_size != sizeof(psm2_uuid_t)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_EP_CTRL,
				"Invalid ep auth_key_len %"PRIu64
				", should be %"PRIu64".\n",
				info->ep_attr->auth_key_size,
				sizeof(psm2_uuid_t));
			goto errout;
		}
		uuid = info->ep_attr->auth_key;
	}

	 /* If override is true, the FI_PSM3_UUID was set to override other uuid */
	if (psmx3_override_uuid()) {
		uuid = domain_priv->fabric->uuid;
	}

	if (usage_flags) {
		trx_ctxt = psmx3_trx_ctxt_alloc(domain_priv, src_addr, -1,
						usage_flags, uuid);
		if (!trx_ctxt)
			goto errout;
	} else {
		PSMX3_INFO(&psmx3_prov, FI_LOG_EP_CTRL,
			"Tx only endpoint with STX context.\n");
	}

	err = psmx3_ep_open_internal(domain_priv, info, &ep_priv, context,
				     trx_ctxt, usage_flags);
	if (err)
		goto errout_free_ctxt;

	ep_priv->type = PSMX3_EP_REGULAR;
	ep_priv->service = PSMX3_ANY_SERVICE;
	if (src_addr) {
		ep_priv->service = src_addr->service;
		if (info->addr_format == FI_ADDR_STR)
			free(src_addr);
	}

	if (ep_priv->service == PSMX3_ANY_SERVICE)
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
	psmx3_trx_ctxt_free(trx_ctxt, usage_flags);

errout:
	return err;
}

/*
 * Shared tx context
 */

static int psmx3_stx_close(fid_t fid)
{
	struct psmx3_fid_stx *stx;

	stx = container_of(fid, struct psmx3_fid_stx, stx.fid);

	if (ofi_atomic_get32(&stx->ref))
		return -FI_EBUSY;

	psmx3_trx_ctxt_free(stx->tx, PSMX3_TX);
	psmx3_domain_release(stx->domain);
	free(stx);
	return 0;
}

static struct fi_ops psmx3_fi_ops_stx = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_stx_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep psmx3_stx_ops = {
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
int psmx3_stx_ctx(struct fid_domain *domain, struct fi_tx_attr *attr,
		  struct fid_stx **stx, void *context)
{
	struct psmx3_fid_domain *domain_priv;
	struct psmx3_trx_ctxt *trx_ctxt;
	struct psmx3_fid_stx *stx_priv;
	int err = -FI_EINVAL;

	domain_priv = container_of(domain, struct psmx3_fid_domain,
				   util_domain.domain_fid.fid);
	if (!domain_priv)
		goto errout;

	stx_priv = (struct psmx3_fid_stx *) calloc(1, sizeof *stx_priv);
	if (!stx_priv) {
		err = -FI_ENOMEM;
		goto errout;
	}

	/* no auth_key is provided, use NULL to pick the default uuid */
	trx_ctxt = psmx3_trx_ctxt_alloc(domain_priv, NULL, -1, PSMX3_TX,
					NULL);
	if (!trx_ctxt) {
		err = -FI_ENOMEM;
		goto errout_free_stx;
	}

	psmx3_domain_acquire(domain_priv);
	stx_priv->stx.fid.fclass = FI_CLASS_STX_CTX;
	stx_priv->stx.fid.context = context;
	stx_priv->stx.fid.ops = &psmx3_fi_ops_stx;
	stx_priv->stx.ops = &psmx3_stx_ops;
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

static int psmx3_sep_close(fid_t fid)
{
	struct psmx3_fid_sep *sep;
	struct psmx3_ep_name ep_name;
	int i;

	sep = container_of(fid, struct psmx3_fid_sep, ep.fid);

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
		psmx3_trx_ctxt_free(sep->ctxts[i].trx_ctxt, PSMX3_TX_RX);

		if (sep->ctxts[i].ep)
			psmx3_ep_close_internal(sep->ctxts[i].ep);
	}

	sep->domain->sep_lock_fn(&sep->domain->sep_lock, 1);
	dlist_remove(&sep->entry);
	sep->domain->sep_unlock_fn(&sep->domain->sep_lock, 1);

	psmx3_domain_release(sep->domain);
	free(sep);
	return 0;
}

static int psmx3_sep_control(fid_t fid, int command, void *arg)
{
	struct psmx3_fid_sep *sep;

	sep = container_of(fid, struct psmx3_fid_sep, ep.fid);

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
STATIC int psmx3_sep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx3_fid_sep *sep;
	int i, err = 0;

	sep = container_of(fid, struct psmx3_fid_sep, ep.fid);

	for (i = 0; i < sep->ctxt_cnt; i++) {
		err = psmx3_ep_bind(&sep->ctxts[i].ep->ep.fid, bfid, flags);
		if (err)
			break;
	}

	return err;
}

DIRECT_FN
STATIC int psmx3_tx_context(struct fid_ep *ep, int index, struct fi_tx_attr *attr,
			    struct fid_ep **tx_ep, void *context)
{
	struct psmx3_fid_sep *sep;

	sep = container_of(ep, struct psmx3_fid_sep, ep);

	assert(index >= 0 && index < sep->ctxt_cnt);

	*tx_ep = &sep->ctxts[index].ep->ep;
	return 0;
}

DIRECT_FN
STATIC int psmx3_rx_context(struct fid_ep *ep, int index, struct fi_rx_attr *attr,
			    struct fid_ep **rx_ep, void *context)
{
	struct psmx3_fid_sep *sep;

	sep = container_of(ep, struct psmx3_fid_sep, ep);

	assert(index >= 0 && index < sep->ctxt_cnt);

	*rx_ep = &sep->ctxts[index].ep->ep;
	return 0;
}

static int psmx3_sep_ctxt_close(fid_t fid)
{
	struct psmx3_fid_ep *ep;

	ep = container_of(fid, struct psmx3_fid_ep, ep.fid);

	if (ep->base_ep)
		ofi_atomic_dec32(&ep->base_ep->ref);

	return 0;
}

static struct fi_ops psmx3_fi_ops_sep_ctxt = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_sep_ctxt_close,
	.bind = psmx3_ep_bind,
	.control = psmx3_ep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops psmx3_fi_ops_sep = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_sep_close,
	.bind = psmx3_sep_bind,
	.control = psmx3_sep_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_ep psmx3_sep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = psmx3_tx_context,
	.rx_ctx = psmx3_rx_context,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

DIRECT_FN
int psmx3_sep_open(struct fid_domain *domain, struct fi_info *info,
		   struct fid_ep **sep, void *context)
{
	struct psmx3_fid_domain *domain_priv;
	struct psmx3_fid_ep *ep_priv;
	struct psmx3_fid_sep *sep_priv;
	struct psmx3_ep_name ep_name;
	struct psmx3_ep_name *src_addr;
	struct psmx3_trx_ctxt *trx_ctxt;
	size_t ctxt_cnt = 1;
	size_t ctxt_size;
	int err = -FI_EINVAL;
	uint8_t *uuid = NULL;
	int i;

	domain_priv = container_of(domain, struct psmx3_fid_domain,
				   util_domain.domain_fid.fid);
	if (!domain_priv)
		goto errout;

	if (!psmx3_override_uuid() && info && info->domain_attr && info->domain_attr->auth_key) {
		if (info->domain_attr->auth_key_size != sizeof(psm2_uuid_t)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_EP_CTRL,
				"Invalid domain auth_key_len %"PRIu64
				", should be %"PRIu64".\n",
				info->domain_attr->auth_key_size,
				sizeof(psm2_uuid_t));
			goto errout;
		}
		uuid = info->domain_attr->auth_key;
	}

	if (info && info->ep_attr) {
		if (!psmx3_override_uuid() && info->ep_attr->auth_key) {
			if (info->ep_attr->auth_key_size != sizeof(psm2_uuid_t)) {
				PSMX3_WARN(&psmx3_prov, FI_LOG_EP_CTRL,
					"Invalid ep auth_key_len %"PRIu64
					", should be %"PRIu64".\n",
					info->ep_attr->auth_key_size,
					sizeof(psm2_uuid_t));
				goto errout;
			}
			uuid = info->ep_attr->auth_key;
		}

		if (info->ep_attr->tx_ctx_cnt > psmx3_domain_info.max_trx_ctxt) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_EP_CTRL,
				"tx_ctx_cnt %"PRIu64" exceed limit %d.\n",
				info->ep_attr->tx_ctx_cnt,
				psmx3_domain_info.max_trx_ctxt);
			goto errout;
		}
		if (info->ep_attr->rx_ctx_cnt > psmx3_domain_info.max_trx_ctxt) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_EP_CTRL,
				"rx_ctx_cnt %"PRIu64" exceed limit %d.\n",
				info->ep_attr->rx_ctx_cnt,
				psmx3_domain_info.max_trx_ctxt);
			goto errout;
		}
		ctxt_cnt = info->ep_attr->tx_ctx_cnt;
		if (ctxt_cnt < info->ep_attr->rx_ctx_cnt)
			ctxt_cnt = info->ep_attr->rx_ctx_cnt;
		if (ctxt_cnt == 0) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_EP_CTRL,
				"tx_ctx_cnt and rx_ctx_cnt are 0, use 1.\n");
			ctxt_cnt = 1;
		}
	}

	/* If override is true, the FI_PSM3_UUID was set to override other uuid */
	if (psmx3_override_uuid()) {
		uuid = domain_priv->fabric->uuid;
	}

	ctxt_size = ctxt_cnt * sizeof(struct psmx3_sep_ctxt);
	sep_priv = (struct psmx3_fid_sep *) calloc(1, sizeof(*sep_priv) + ctxt_size);
	if (!sep_priv) {
		err = -FI_ENOMEM;
		goto errout;
	}

	sep_priv->ep.fid.fclass = FI_CLASS_SEP;
	sep_priv->ep.fid.context = context;
	sep_priv->ep.fid.ops = &psmx3_fi_ops_sep;
	sep_priv->ep.ops = &psmx3_sep_ops;
	sep_priv->ep.cm = &psmx3_cm_ops;
	sep_priv->domain = domain_priv;
	sep_priv->ctxt_cnt = ctxt_cnt;
	ofi_atomic_initialize32(&sep_priv->ref, 0);
 
	src_addr = NULL;
	if (info && info->src_addr) {
		if (info->addr_format == FI_ADDR_STR)
			src_addr = psmx3_string_to_ep_name(info->src_addr);
		else
			src_addr = info->src_addr;
	}

	for (i = 0; i < ctxt_cnt; i++) {
		trx_ctxt = psmx3_trx_ctxt_alloc(domain_priv, src_addr,
						(ctxt_cnt > 1) ? i : -1,
						PSMX3_TX_RX, uuid);
		if (!trx_ctxt) {
			err = -FI_ENOMEM;
			goto errout_free_ctxt;
		}

		sep_priv->ctxts[i].trx_ctxt = trx_ctxt;

		err = psmx3_ep_open_internal(domain_priv, info, &ep_priv, context,
					     trx_ctxt, PSMX3_TX_RX);
		if (err)
			goto errout_free_ctxt;

		/* override the ops so the fid can't be closed individually */
		ep_priv->ep.fid.ops = &psmx3_fi_ops_sep_ctxt;

		sep_priv->ctxts[i].ep = ep_priv;
	}

	sep_priv->type = PSMX3_EP_SCALABLE;
	sep_priv->service = PSMX3_ANY_SERVICE;
	if (src_addr) {
		sep_priv->service = src_addr->service;
		if (info->addr_format == FI_ADDR_STR)
			free(src_addr);
	}

	if (sep_priv->service == PSMX3_ANY_SERVICE)
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

	psmx3_domain_acquire(domain_priv);
	*sep = &sep_priv->ep;

	/* Make sure the AM handler is installed to answer SEP query */
	psmx3_am_init(sep_priv->ctxts[0].trx_ctxt);

	return 0;

errout_free_ctxt:
	while (i) {
		if (sep_priv->ctxts[i].trx_ctxt)
			psmx3_trx_ctxt_free(sep_priv->ctxts[i].trx_ctxt,
					    PSMX3_TX_RX);

		if (sep_priv->ctxts[i].ep)
			psmx3_ep_close_internal(sep_priv->ctxts[i].ep);

		i--;
	}

	free(sep_priv);

errout:
	return err;
}
