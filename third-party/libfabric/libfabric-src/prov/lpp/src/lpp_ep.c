/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

static const struct fi_ops lpp_ep_fi_ops = {
	.size                  = sizeof(struct fi_ops),
	.close                 = lpp_fi_ep_close,
	.bind                  = lpp_fi_ep_bind,
	.control               = lpp_fi_ep_control,
	.ops_open              = fi_no_ops_open,
};

static const struct fi_ops_ep lpp_ep_ops = {
	.size                  = sizeof(struct fi_ops_ep),
	.cancel                = lpp_fi_cancel,
	.getopt                = fi_no_getopt,
	.setopt                = lpp_fi_setopt,
	.tx_ctx                = fi_no_tx_ctx,
	.rx_ctx                = fi_no_rx_ctx,
	.rx_size_left          = fi_no_rx_size_left,
	.tx_size_left          = fi_no_tx_size_left,
};

static const struct fi_ops_cm lpp_ep_cm_ops = {
	.size                  = sizeof(struct fi_ops_cm),
	.setname               = lpp_fi_setname,
	.getname               = lpp_fi_getname,
	.getpeer               = fi_no_getpeer,
	.connect               = fi_no_connect,
	.listen                = fi_no_listen,
	.accept                = fi_no_accept,
	.reject                = fi_no_reject,
	.shutdown              = fi_no_shutdown,
	.join                  = fi_no_join,
};

static const struct fi_ops_msg lpp_ep_msg_ops = {
	.size                  = sizeof(struct fi_ops_msg),
	.recv                  = lpp_fi_recv,
	.recvv                 = fi_no_msg_recvv,
	.recvmsg               = lpp_fi_recvmsg,
	.send                  = lpp_fi_send,
	.sendv                 = lpp_fi_sendv,
	.sendmsg               = lpp_fi_sendmsg,
	.inject                = lpp_fi_inject,
	.senddata              = fi_no_msg_senddata,
	.injectdata            = fi_no_msg_injectdata
};

static const struct fi_ops_rma lpp_ep_rma = {
	.size                  = sizeof(struct fi_ops_rma),
	.read                  = lpp_fi_read,
	.readv                 = fi_no_rma_readv,
	.readmsg               = lpp_fi_readmsg,
	.write                 = lpp_fi_write,
	.writev                = fi_no_rma_writev,
	.writemsg              = lpp_fi_writemsg,
	.inject                = lpp_fi_inject_write,
	.injectdata            = fi_no_rma_injectdata,
	.writedata             = fi_no_rma_writedata,
};

static const struct fi_ops_tagged lpp_ep_tagged = {
	.size                  = sizeof(struct fi_ops_tagged),
	.recv                  = lpp_fi_trecv,
	.recvv                 = fi_no_tagged_recvv,
	.recvmsg               = lpp_fi_trecvmsg,
	.send                  = lpp_fi_tsend,
	.sendv                 = fi_no_tagged_sendv,
	.sendmsg               = lpp_fi_tsendmsg,
	.inject                = lpp_fi_tinject,
	.senddata              = lpp_fi_tsenddata,
	.injectdata            = lpp_fi_tinjectdata,
};

static const struct fi_ep_attr lpp_default_ep_attr = {
	.type                  = FI_EP_RDM,
	.protocol              = FI_PROTO_LPP,
	.protocol_version      = 1,
	.max_msg_size          = 0,
	.msg_prefix_size       = 0,
	.max_order_raw_size    = 0,
	.max_order_war_size    = 0,
	.max_order_waw_size    = 0,
	.mem_tag_format        = LPP_TAG_FORMAT,
	.tx_ctx_cnt            = 0,
	.rx_ctx_cnt            = 0,
	.auth_key_size         = 0,
	.auth_key              = NULL,
};

struct fi_ep_attr lpp_ep_attrs(const struct klppioc_lf *klpp_devinfo)
{
	struct fi_ep_attr  tmp = lpp_default_ep_attr;

	tmp.max_msg_size       = klpp_devinfo->max_msg_size;
	tmp.max_order_raw_size = klpp_devinfo->max_msg_size;
	tmp.max_order_war_size = klpp_devinfo->max_msg_size;
	tmp.max_order_waw_size = klpp_devinfo->max_msg_size;
	tmp.auth_key           = NULL;

	return tmp;
}

void lpp_ep_setup_attrs(const struct klppioc_lf *klpp_devinfo,
			const struct fi_info *hints,
			struct fi_ep_attr *tmp_ep) {

	tmp_ep->type = lpp_default_ep_attr.type;
	tmp_ep->protocol = lpp_default_ep_attr.protocol;
	tmp_ep->protocol_version = lpp_default_ep_attr.protocol_version;
	tmp_ep->max_msg_size       = klpp_devinfo->max_msg_size;
	tmp_ep->max_order_raw_size = klpp_devinfo->max_msg_size;
	tmp_ep->max_order_war_size = klpp_devinfo->max_msg_size;
	tmp_ep->max_order_waw_size = klpp_devinfo->max_msg_size;
	tmp_ep->msg_prefix_size = lpp_default_ep_attr.msg_prefix_size;
	tmp_ep->mem_tag_format = lpp_default_ep_attr.mem_tag_format;
	tmp_ep->auth_key_size = lpp_default_ep_attr.auth_key_size;
	tmp_ep->auth_key = lpp_default_ep_attr.auth_key;

	tmp_ep->rx_ctx_cnt = (hints && hints->ep_attr)?
		hints->ep_attr->rx_ctx_cnt : lpp_default_ep_attr.rx_ctx_cnt;
	tmp_ep->tx_ctx_cnt = (hints && hints->ep_attr)?
		hints->ep_attr->tx_ctx_cnt : lpp_default_ep_attr.tx_ctx_cnt;
}

int lpp_verify_ep_attrs(const struct klppioc_lf *klpp_devinfo, struct fi_ep_attr *ep_attrs)
{

#define CHECK_ATTR(have, op, cmp)									\
	if ((have) op (cmp)) {										\
		FI_INFO(&lpp_prov, FI_LOG_EP_CTRL, "attr check failed: " #have " " #op " " #cmp "\n");	\
		return -FI_EINVAL;									\
	}

	CHECK_ATTR(ep_attrs->protocol,         !=,  FI_PROTO_LPP);
	CHECK_ATTR(ep_attrs->msg_prefix_size,  >,   0);
	CHECK_ATTR(ep_attrs->auth_key_size,    !=,  0);

#undef CHECK_ATTR

	if(ep_attrs->type != FI_EP_RDM && ep_attrs->type != FI_EP_UNSPEC) {
		FI_INFO(&lpp_prov, FI_LOG_EP_CTRL, "invalid ep type: %d\n",
				ep_attrs->type);
		return -FI_EINVAL;
	}

	if ((ep_attrs->rx_ctx_cnt != 0) && (ep_attrs->rx_ctx_cnt != 1)) {
		FI_INFO(&lpp_prov, FI_LOG_EP_CTRL, "invalid rx_ctx_cnt: %lu\n",
				ep_attrs->tx_ctx_cnt);
		return -FI_EINVAL;
	}

	if ((ep_attrs->tx_ctx_cnt != 0) && (ep_attrs->tx_ctx_cnt != 1) &&
	    (ep_attrs->tx_ctx_cnt != FI_SHARED_CONTEXT)) {
		FI_INFO(&lpp_prov, FI_LOG_EP_CTRL, "invalid tx_ctx_cnt: %lu\n",
				ep_attrs->tx_ctx_cnt);
		return -FI_EINVAL;
	}

	return 0;
}


static int lpp_ep_verify_info(struct klppioc_lf *klpp_devinfo, struct fi_info *info)
{

	if (info == NULL)
		return 0;

	if ((info->ep_attr != NULL) && (lpp_verify_ep_attrs(klpp_devinfo, info->ep_attr) != 0))
		return -FI_ENODATA;

	if ((info->tx_attr != NULL) && (lpp_verify_tx_attrs(klpp_devinfo, info->tx_attr) != 0))
		return -FI_ENODATA;

	if ((info->rx_attr != NULL) && (lpp_verify_rx_attrs(klpp_devinfo, info->rx_attr) != 0))
		return -FI_ENODATA;

	return 0;
}

struct lpp_ep_binding {
	struct dlist_entry	dle;
	struct lpp_ep		*ep;
};

/* Adds endpoint to ep_list, ensuring no duplicates.
 * If using FI_THREAD_SAFE, must hold domain ep_lock. */
static int lpp_ep_list_add(struct dlist_entry *ep_list, struct lpp_ep *lpp_epp)
{
	struct lpp_ep_binding *binding;

	/* It's assumed this list will be short. May need a better algorithm
	 * if that is not true. */
	dlist_foreach_container(ep_list, struct lpp_ep_binding,
	    binding, dle) {
		if (binding->ep == lpp_epp)
			return 0;

	}
	binding = malloc(sizeof(*binding));
	if (binding == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "failed to alloc ep binding\n");
		return -FI_ENOMEM;
	}
	binding->ep = lpp_epp;
	dlist_init(&binding->dle);
	dlist_insert_tail(&binding->dle, ep_list);
	return 0;
}

/* Remove lpp_epp from ep_list, if present.
 * If using FI_THREAD_SAFE, must hold domain ep_lock. */
static void lpp_ep_list_del(struct dlist_entry *ep_list, struct lpp_ep *lpp_epp)
{
	struct lpp_ep_binding *binding;

	dlist_foreach_container(ep_list, struct lpp_ep_binding, binding, dle) {
		if (binding->ep == lpp_epp) {
			dlist_remove(&binding->dle);
			free(binding);
			return;
		}
	}
}

static int lpp_ep_bind_cq(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct lpp_ep *lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	struct lpp_cq *lpp_cqp = container_of(bfid, struct lpp_cq, cq_fid);
	int ret;

	if (lpp_epp->domain != lpp_cqp->domain) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "endpoint and CQ belong to different domains\n");
		return -FI_EINVAL;
	}

	if ((flags & FI_TRANSMIT) && (lpp_epp->cq_transmit != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "transmit CQ already bound\n");
		return -FI_EINVAL;
	}
	if ((flags & FI_RECV) && (lpp_epp->cq_recv != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "recv CQ already bound\n");
		return -FI_EINVAL;
	}

	if (flags & FI_TRANSMIT) {
		lpp_epp->cq_transmit = lpp_cqp;
		lpp_epp->transmit_bind_flags = flags & FI_SELECTIVE_COMPLETION;
		ofi_atomic_inc32(&lpp_cqp->ref_count);
		ret = lpp_ep_list_add(&lpp_cqp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}

	if (flags & FI_RECV) {
		lpp_epp->cq_recv = lpp_cqp;
		lpp_epp->recv_bind_flags = flags & FI_SELECTIVE_COMPLETION;
		ofi_atomic_inc32(&lpp_cqp->ref_count);
		ret = lpp_ep_list_add(&lpp_cqp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}

	return 0;
}

static int lpp_ep_bind_cntr(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct lpp_ep *lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	struct lpp_cntr *lpp_cntrp = container_of(bfid, struct lpp_cntr, cntr_fid);
	int ret;

	if (lpp_epp->domain != lpp_cntrp->domain) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "endpoint and cntr belong to different domains\n");
		return -FI_EINVAL;
	}

	if ((flags & FI_SEND) && (lpp_epp->cntr_send != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "send cntr already bound\n");
		return -FI_EINVAL;
	}
	if ((flags & FI_RECV) && (lpp_epp->cntr_recv != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "recv cntr already bound\n");
		return -FI_EINVAL;
	}
	if ((flags & FI_READ) && (lpp_epp->cntr_read != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "read cntr already bound\n");
		return -FI_EINVAL;
	}
	if ((flags & FI_WRITE) && (lpp_epp->cntr_write != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "write cntr already bound\n");
		return -FI_EINVAL;
	}
	if ((flags & FI_REMOTE_READ) && (lpp_epp->cntr_remote_read != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "remote read cntr already bound\n");
		return -FI_EINVAL;
	}
	if ((flags & FI_REMOTE_WRITE) && (lpp_epp->cntr_remote_write != NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "remote write cntr already bound\n");
		return -FI_EINVAL;
	}

	if (flags & FI_SEND) {
		lpp_epp->cntr_send = lpp_cntrp;
		ofi_atomic_inc32(&lpp_cntrp->ref_count);
		ret = lpp_ep_list_add(&lpp_cntrp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}
	if (flags & FI_RECV) {
		lpp_epp->cntr_recv = lpp_cntrp;
		ofi_atomic_inc32(&lpp_cntrp->ref_count);
		ret = lpp_ep_list_add(&lpp_cntrp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}
	if (flags & FI_READ) {
		lpp_epp->cntr_read = lpp_cntrp;
		ofi_atomic_inc32(&lpp_cntrp->ref_count);
		ret = lpp_ep_list_add(&lpp_cntrp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}
	if (flags & FI_WRITE) {
		lpp_epp->cntr_write = lpp_cntrp;
		ofi_atomic_inc32(&lpp_cntrp->ref_count);
		ret = lpp_ep_list_add(&lpp_cntrp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}
	if (flags & FI_REMOTE_READ) {
		lpp_epp->cntr_remote_read = lpp_cntrp;
		ofi_atomic_inc32(&lpp_cntrp->ref_count);
		ret = lpp_ep_list_add(&lpp_cntrp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}
	if (flags & FI_REMOTE_WRITE) {
		lpp_epp->cntr_remote_write = lpp_cntrp;
		ofi_atomic_inc32(&lpp_cntrp->ref_count);
		ret = lpp_ep_list_add(&lpp_cntrp->ep_list, lpp_epp);
		if (ret)
			return ret;
	}

	return 0;
}

static int lpp_ep_bind_av(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct lpp_ep *lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	struct lpp_av *lpp_avp = container_of(bfid, struct lpp_av, av_fid);

	if (lpp_epp->domain != lpp_avp->domain) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"endpoint and av belong to different domains\n");
		return -FI_EINVAL;
	}

	if (lpp_epp->av != NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"endpoint already has an av bound\n");
		return -FI_EINVAL;
	}
	lpp_epp->av = lpp_avp;
	ofi_atomic_inc32(&lpp_avp->ref_count);

	return 0;
}

static int lpp_ep_bind_stx(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct lpp_ep *lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	struct lpp_stx *lpp_stxp = container_of(bfid, struct lpp_stx, stx_fid);

	if (lpp_epp->domain != lpp_stxp->domain) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"endpoint and stx belong to different domains\n");
		return -FI_EOPBADSTATE;
	}

	if (lpp_epp->stx != NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"endpoint already has a bound stx\n");
		return -FI_EOPBADSTATE;
	}

	if (lpp_stxp->ep != NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"stx already has a bound endpoint\n");
		return -FI_EOPBADSTATE;
	}

	lpp_epp->stx = lpp_stxp;
	lpp_stxp->ep = lpp_epp;
	ofi_atomic_inc32(&lpp_stxp->ref_count);

	return 0;
}

static int lpp_ep_close(struct lpp_ep *lpp_epp)
{
	struct lpp_domain *lpp_domainp = lpp_epp->domain;
	int ret;

	lpp_lock(lpp_domainp, &lpp_domainp->ep_lock);

	if (lpp_epp->is_enabled) {
		lpp_umc_release(lpp_epp, lpp_epp->umc);

		ret = klpp_ep_disable(lpp_epp);
		if (ret != 0) {
			FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
				"disabling ep with klpp returned %d\n", ret);
		}
		lpp_epp->is_enabled = 0;
	}

	ret = klpp_ep_close(lpp_epp);
	if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"closing ep with klpp returned %d\n", ret);
	}

	// Unbind the CNTRs.
	if (lpp_epp->cntr_send != NULL) {
		lpp_ep_list_del(&lpp_epp->cntr_send->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cntr_send->ref_count);
	}

	if (lpp_epp->cntr_recv != NULL) {
		lpp_ep_list_del(&lpp_epp->cntr_recv->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cntr_recv->ref_count);
	}

	if (lpp_epp->cntr_read != NULL) {
		lpp_ep_list_del(&lpp_epp->cntr_read->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cntr_read->ref_count);
	}

	if (lpp_epp->cntr_write != NULL) {
		lpp_ep_list_del(&lpp_epp->cntr_write->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cntr_write->ref_count);
	}

	if (lpp_epp->cntr_remote_read != NULL) {
		lpp_ep_list_del(&lpp_epp->cntr_remote_read->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cntr_remote_read->ref_count);
	}

	if (lpp_epp->cntr_remote_write != NULL) {
		lpp_ep_list_del(&lpp_epp->cntr_remote_write->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cntr_remote_write->ref_count);
	}

	// Unbind the CQs.
	if (lpp_epp->cq_transmit != NULL) {
		lpp_ep_list_del(&lpp_epp->cq_transmit->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cq_transmit->ref_count);
	}

	if (lpp_epp->cq_recv != NULL) {
		lpp_ep_list_del(&lpp_epp->cq_recv->ep_list, lpp_epp);
		ofi_atomic_dec32(&lpp_epp->cq_recv->ref_count);
	}

	// Unbind the AV.
	if (lpp_epp->av != NULL) {
		lpp_epp->av->ep = NULL;
		ofi_atomic_dec32(&lpp_epp->av->ref_count);
	}

	// Unbind the STX queue.
	if (lpp_epp->stx != NULL) {
		lpp_epp->stx->ep = NULL;
		ofi_atomic_dec32(&lpp_epp->stx->ref_count);
		if (lpp_epp->stx_internal) {
			lpp_stx_close(lpp_epp->stx);
		}
	}

	// Unbind and close the RX queue.
	if (lpp_epp->rx != NULL) {
		lpp_epp->rx->ep = NULL;
		ofi_atomic_dec32(&lpp_epp->rx->ref_count);
		lpp_rx_close(lpp_epp->rx);
	}

	// Unmap the IO stat region.
	klpp_munmap(lpp_epp->io_stat, sizeof(*lpp_epp->io_stat) * lpp_domainp->devinfo.host_count);

	ofi_atomic_dec32(&lpp_epp->domain->util_domain.ref);
	free(lpp_epp);

	lpp_unlock(lpp_domainp, &lpp_domainp->ep_lock);

	return 0;
}

int lpp_fi_endpoint(struct fid_domain *domain, struct fi_info *info,
			      struct fid_ep **ep, void *context)
{
	struct util_domain	*util_domain;
	struct lpp_domain		*lpp_domainp;
	struct lpp_ep		*lpp_epp;

	util_domain = container_of(domain, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);

	if (lpp_ep_verify_info(&lpp_domainp->devinfo, info) < 0) {
		return -FI_ENODATA;
	}

	if (lpp_epp = calloc(1, sizeof(struct lpp_ep)), lpp_epp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to alloc endpoint\n");
		return -FI_ENOMEM;
	}

	lpp_epp->domain = lpp_domainp;

	// Tell KLPP so we can get a id.
	if (klpp_ep_create(lpp_epp) != 0) {
		free(lpp_epp);
		return -FI_ENODATA;
	}

	lpp_epp->io_stat = klpp_mmap(lpp_domainp,
	    sizeof(*lpp_epp->io_stat) * lpp_domainp->devinfo.host_count, PROT_READ);
	if (lpp_epp->io_stat == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to mmap io stat region\n");
		free(lpp_epp);
		return -FI_ENOMEM;
	}

	// Populate the endpoint.
	lpp_epp->ep_fid.fid.fclass = FI_CLASS_EP;
	lpp_epp->ep_fid.fid.context = context;
	lpp_epp->ep_fid.fid.ops = (struct fi_ops *)&lpp_ep_fi_ops;

	lpp_epp->ep_fid.ops = (struct fi_ops_ep *)&lpp_ep_ops;
	lpp_epp->ep_fid.cm = (struct fi_ops_cm *)&lpp_ep_cm_ops;
	lpp_epp->ep_fid.msg = (struct fi_ops_msg *)&lpp_ep_msg_ops;
	lpp_epp->ep_fid.rma = (struct fi_ops_rma *)&lpp_ep_rma;
	lpp_epp->ep_fid.tagged = (struct fi_ops_tagged *)&lpp_ep_tagged;
	lpp_epp->ep_fid.atomic = (struct fi_ops_atomic *)&lpp_ep_atomic;

	ofi_atomic_initialize32(&lpp_epp->ref_count, 0);
	lpp_epp->attr = info && info->ep_attr ? *info->ep_attr : lpp_ep_attrs(&lpp_domainp->devinfo);
	lpp_epp->tx_attr = info && info->tx_attr ? *info->tx_attr : lpp_tx_attrs(&lpp_domainp->devinfo);
	lpp_epp->rx_attr = info && info->rx_attr ? *info->rx_attr : lpp_rx_attrs(&lpp_domainp->devinfo);

	ofi_atomic_inc32(&lpp_epp->domain->util_domain.ref);
	*ep = &lpp_epp->ep_fid;
	return 0;
}

int lpp_fi_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	int status = 0;
	struct lpp_ep *lpp_epp;
	struct lpp_domain *lpp_domainp;

	if ((fid == NULL) || (bfid == NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"fid/bfid is NULL (fid %p bfid %p)\n", fid, bfid);
		return -FI_EINVAL;
	}

	if (fid->fclass != FI_CLASS_EP) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "fid class is invalid\n");
		return -FI_EINVAL;
	}

	lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	lpp_domainp = lpp_epp->domain;

	lpp_lock(lpp_domainp, &lpp_domainp->ep_lock);

	if (lpp_epp->is_enabled) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			"endpoint is already enabled\n");
		status = -FI_EINVAL;
		goto unlock;
	}

	// We only support CQ, CNTR, AV, and STX bindings.
	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		status = lpp_ep_bind_cq(fid, bfid, flags);
		break;
	case FI_CLASS_CNTR:
		status = lpp_ep_bind_cntr(fid, bfid, flags);
		break;
	case FI_CLASS_AV:
		status = lpp_ep_bind_av(fid, bfid, flags);
		break;
	case FI_CLASS_STX_CTX:
		status = lpp_ep_bind_stx(fid, bfid, flags);
		break;
	default:
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "invalid bind class %ld\n",
			bfid->fclass);
		status = -FI_EINVAL;
		break;
	}

unlock:
	lpp_unlock(lpp_domainp, &lpp_domainp->ep_lock);
	return status;
}

int lpp_fi_ep_close(struct fid *fid)
{
	int			status = 0;
	struct lpp_ep		*lpp_epp = NULL;
	struct lpp_stx		*lpp_stxp = NULL;

	if (fid == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "fid is invalid\n");
		return -FI_EINVAL;
	}

	switch (fid->fclass) {
	case FI_CLASS_EP:
		lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
		status = lpp_ep_close(lpp_epp);
		break;
	case FI_CLASS_STX_CTX:
		lpp_stxp = container_of(fid, struct lpp_stx, stx_fid);
		status = lpp_stx_close(lpp_stxp);
		break;
	default:
		status = -FI_EINVAL;
		break;
	}

	return status;
}

int lpp_fi_enable(struct fid *fid)
{
	int ret = 0;
	struct lpp_ep *lpp_epp = NULL;
	struct lpp_stx *lpp_stxp = NULL;
	struct lpp_domain *lpp_domainp;

	if (fid == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "fid is invalid\n");
		return -FI_EINVAL;
	}

	lpp_epp = container_of(fid, struct lpp_ep, ep_fid);
	lpp_domainp = lpp_epp->domain;

	lpp_lock(lpp_domainp, &lpp_domainp->ep_lock);

	if (lpp_epp->is_enabled) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "ep is already enabled\n");
		ret = -FI_EINVAL;
		goto unlock;
	}

	/* If this is an RX-capable endpoint and the user didn't create an RX
	 * explicitly, we make one automatically here. */
	if (lpp_epp->rx_attr.caps != 0 && lpp_epp->rx == NULL) {
		lpp_epp->rx = lpp_rx_context(lpp_epp, &lpp_epp->rx_attr);
		if (lpp_epp->rx == NULL) {
			ret = -FI_EINVAL;
			goto unlock;
		}
		ofi_atomic_inc32(&lpp_epp->rx->ref_count);
	}

	/* If we don't already have an STX, we create one.
	 * TODO: if this is not a transmit capable endpoint, we can skip this.
	 * if (lpp_epp->tx_attr.caps == ... && lpp_epp->stx == NULL) { } */
	if (lpp_epp->stx == NULL) {
		lpp_stxp = lpp_stx_alloc(lpp_epp->domain, &lpp_epp->tx_attr,
					 lpp_epp->ep_fid.fid.context);
		if (lpp_stxp == NULL) {
			ret = -FI_EINVAL;
			goto unlock;
		} else {
			ret = lpp_ep_bind_stx(fid, &lpp_stxp->stx_fid.fid, 0);
			if (ret != 0) {
				fi_close(&lpp_stxp->stx_fid.fid);
				ret = -FI_EINVAL;
				goto unlock;
			}
			lpp_epp->stx_internal = 1;
		}
	}

	/* Per fi_endpoint(3): "An endpoint must be bound with CQs capable of
	 * reporting completions for any asynchronous operation initiated on
	 * the endpoint." */
	if (lpp_epp->stx != NULL && lpp_epp->cq_transmit == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "No transmit CQ bound to transmit capable endpoint\n");
		ret = -FI_EINVAL;
		goto unlock;
	}
	if (lpp_epp->rx != NULL && lpp_epp->cq_recv == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
		    "No recv CQ bound to recv capable endpoint\n");
		ret = -FI_EINVAL;
		goto unlock;
	}

	/* We need to fill this part in before acquiring the UMC so that we
	 * identify the user_id/ep_id of the endpoint that is binding to this
	 * UMC. Afterwards, we'll get a UMC id and can fill in the remaining
	 * fields. */
	lpp_epp->addr.fi_addr.node_id = lpp_epp->domain->devinfo.host_index;
	lpp_epp->addr.fi_addr.user_id = lpp_epp->domain->devinfo.user_id;
	lpp_epp->addr.fi_addr.ep_id = lpp_epp->id;

	ret = lpp_umc_acquire(lpp_epp);
	if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to acquire UMC\n");
		goto unlock;
	}

	lpp_epp->addr.fi_addr.umc_id = lpp_epp->umc->id;
	lpp_epp->addr.fi_addr.umc_gen =
	    KLPP_UMC_GEN_TO_ADDR(lpp_epp->umc->generation);

	if (klpp_ep_enable(lpp_epp) < 0) {
		ret = -FI_EINVAL;
		goto unlock;
	}

	if (lpp_epp->stx != NULL)
		lpp_epp->stx->is_enabled = 1;

	if (lpp_epp->rx != NULL)
		lpp_epp->rx->is_enabled = 1;

	lpp_epp->is_enabled = 1;

unlock:
	lpp_unlock(lpp_domainp, &lpp_domainp->ep_lock);
	return ret;
}

int lpp_fi_ep_control(struct fid *fid, int command, void *arg)
{
	if (fid == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "fid is invalid\n");
		return -FI_EINVAL;
	}

	/* We only support an endpoint as the controlling entity. (TX and RX
	 * are not supported.) */
	if (fid->fclass != FI_CLASS_EP) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		return -FI_EINVAL;
	}

	// The only control function we support is fi_enable().
	if (command != FI_ENABLE) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "invalid control command\n");
		return -FI_EINVAL;
	}

	if (arg != NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "invalid 'arg' parameter\n");
		return -FI_EINVAL;
	}

	return lpp_fi_enable(fid);
}

ssize_t lpp_fi_cancel(fid_t fid, void *context)
{
	struct lpp_ep *lpp_epp;
	ssize_t ret;

	if (fid == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "NULL fid specified\n");
		return -FI_EINVAL;
	}

	lpp_epp = container_of(fid, struct lpp_ep, ep_fid);

	if (context == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "NULL context specified\n");
		return -FI_EINVAL;
	}

	lpp_ep_op_lock(lpp_epp);
	ret = lpp_rx_cancel(lpp_epp, context);
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

int lpp_fi_setopt(struct fid *fid, int level, int optname, const void *optval,
		  size_t optlen)
{
	struct lpp_ep *lpp_epp = NULL;
	struct lpp_rx *lpp_rxp = NULL;

	if (fid == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "fid is invalid\n");
		return -FI_EINVAL;
	}

	lpp_epp = container_of(fid, struct lpp_ep, ep_fid);

	// We only support FI_OPT_MIN_MULTI_RECV on the receive context.
	if (lpp_rxp = lpp_epp->rx, lpp_rxp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "no RX ctx bound\n");
		return -FI_EINVAL;
	}

	if (level != FI_OPT_ENDPOINT) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "invalid opt level\n");
		return -ENOPROTOOPT;
	}

	if (optname != FI_OPT_MIN_MULTI_RECV) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "invalid opt name\n");
		return -FI_EINVAL;
	}

	lpp_rxp->min_multi_recv = *(size_t *)optval;
	return 0;
}

// If threading is FI_THREAD_SAFE, the owner of ep_list (cq/cntr) must hold its lock.
void lpp_ep_progress(struct dlist_entry *ep_list)
{
	struct lpp_ep_binding *binding;
	struct lpp_ep *lpp_epp;

	// TODO: There could be multiple endpoints in this list bound to the same UMC.
	dlist_foreach_container(ep_list, struct lpp_ep_binding, binding, dle) {
		lpp_epp = binding->ep;
		lpp_ep_op_lock(lpp_epp);
		lpp_stx_flush_unsent_all(lpp_epp);
		lpp_epp->umc->progfn(lpp_epp);
		lpp_ep_op_unlock(lpp_epp);
	}
}
