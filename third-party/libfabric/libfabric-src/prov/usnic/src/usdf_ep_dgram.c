/*
 * Copyright (c) 2014-2019, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <asm/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_util.h"

#include "usnic_direct.h"
#include "usd.h"
#include "usdf.h"
#include "usdf_endpoint.h"
#include "usdf_dgram.h"
#include "usdf_av.h"
#include "usdf_cq.h"
#include "usdf_cm.h"

static int
usdf_ep_dgram_enable(struct fid_ep *fep)
{
	struct usdf_ep *ep;
	struct usd_filter filt;
	struct usd_qp_impl *uqp;
	int ret;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	ep = ep_ftou(fep);

	if (ep->e.dg.ep_wcq == NULL) {
		ret = -FI_EOPBADSTATE;
		goto fail;
	}
	if (ep->e.dg.ep_rcq == NULL) {
		ret = -FI_EOPBADSTATE;
		goto fail;
	}

	filt.uf_type = USD_FTY_UDP_SOCK;
	filt.uf_filter.uf_udp_sock.u_sock = ep->e.dg.ep_sock;

	if (ep->ep_caps & USDF_EP_CAP_PIO) {
		ret = usd_create_qp(ep->ep_domain->dom_dev,
				USD_QTR_UDP,
				USD_QTY_UD_PIO,
				ep->e.dg.ep_wcq->c.hard.cq_cq,
				ep->e.dg.ep_rcq->c.hard.cq_cq,
				127,	// XXX
				127,	// XXX
				&filt,
				&ep->e.dg.ep_qp);
	} else {
		ret = -FI_EAGAIN;
	}

	if (ret != 0) {
		ret = usd_create_qp(ep->ep_domain->dom_dev,
				USD_QTR_UDP,
				USD_QTY_UD,
				ep->e.dg.ep_wcq->c.hard.cq_cq,
				ep->e.dg.ep_rcq->c.hard.cq_cq,
				ep->ep_wqe,
				ep->ep_rqe,
				&filt,
				&ep->e.dg.ep_qp);
	}
	if (ret != 0) {
		goto fail;
	}
	ep->e.dg.ep_qp->uq_context = ep;

	/*
	 * Allocate a memory region big enough to hold a header for each
	 * RQ entry
	 */
	uqp = to_qpi(ep->e.dg.ep_qp);
	ep->e.dg.ep_hdr_ptr = calloc(uqp->uq_rq.urq_num_entries,
			sizeof(ep->e.dg.ep_hdr_ptr[0]));
	if (ep->e.dg.ep_hdr_ptr == NULL) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	ret = usd_alloc_mr(ep->ep_domain->dom_dev,
		usd_get_recv_credits(ep->e.dg.ep_qp) * USDF_HDR_BUF_ENTRY,
			&ep->e.dg.ep_hdr_buf);
	if (ret != 0) {
		goto fail;
	}

	ep->flags |= USDF_EP_ENABLED;

	return 0;

fail:
	free(ep->e.dg.ep_hdr_ptr);
	ep->e.dg.ep_hdr_ptr = NULL;

	if (ep->e.dg.ep_qp != NULL) {
		usd_destroy_qp(ep->e.dg.ep_qp);
		ep->e.dg.ep_qp = NULL;
	}
	return ret;
}

static int
usdf_ep_dgram_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	int ret;
	struct usdf_ep *ep;
	struct usdf_cq *cq;
	struct usdf_av *av;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	/* Backward compatibility case for Open MPI. We haven't been validating the flags until now.
	 * Older version of Open MPI gives FI_RECV as AV bind flag (bug). */
	if (bfid->fclass == FI_CLASS_AV) {
		av = av_fidtou(bfid);
		if (av->av_domain->dom_info->fabric_attr->api_version <= FI_VERSION(1, 4) && (flags & FI_RECV))
			flags = flags & ~FI_RECV;
	}

	/* Check if the binding flags are valid. */
	ret = ofi_ep_bind_valid(&usdf_ops, bfid, flags);
	if (ret)
		return ret;

	ep = ep_fidtou(fid);

	switch (bfid->fclass) {

	case FI_CLASS_AV:
		if (ep->e.dg.ep_av != NULL) {
			return -FI_EINVAL;
		}

		av = av_fidtou(bfid);
		ep->e.dg.ep_av = av;
		ofi_atomic_inc32(&av->av_refcnt);
		break;

	case FI_CLASS_CQ:
		cq = cq_fidtou(bfid);

		/* actually, could look through CQ list for a hard
		 * CQ with function usd_poll_cq() and use that... XXX
		 */
		if (cq->cq_is_soft) {
			return -FI_EINVAL;
		}
		if (cq->c.hard.cq_cq == NULL) {
			ret = usdf_cq_create_cq(cq, &cq->c.hard.cq_cq, true);
			if (ret != 0) {
				return ret;
			}
		}

		if (flags & FI_SEND) {
			if (ep->e.dg.ep_wcq != NULL) {
				return -FI_EINVAL;
			}

			ep->ep_tx_dflt_signal_comp =
				(flags & FI_SELECTIVE_COMPLETION) ? 0 : 1;

			ep->ep_tx_completion = (ep->ep_tx_dflt_signal_comp ||
					(ep->e.dg.tx_op_flags & FI_COMPLETION));

			ep->e.dg.ep_wcq = cq;
			ofi_atomic_inc32(&cq->cq_refcnt);
		}

		if (flags & FI_RECV) {
			if (ep->e.dg.ep_rcq != NULL) {
				return -FI_EINVAL;
			}

			if (flags & FI_SELECTIVE_COMPLETION)
				return -FI_EOPNOTSUPP;

			ep->ep_rx_dflt_signal_comp =
				(flags & FI_SELECTIVE_COMPLETION) ? 0 : 1;

			ep->ep_rx_completion = (ep->ep_rx_dflt_signal_comp ||
					(ep->e.dg.rx_op_flags & FI_COMPLETION));

			ep->e.dg.ep_rcq = cq;
			ofi_atomic_inc32(&cq->cq_refcnt);
		}
		break;

	case FI_CLASS_EQ:
		if (ep->ep_eq != NULL) {
			return -FI_EINVAL;
		}
		ep->ep_eq = eq_fidtou(bfid);
		ofi_atomic_inc32(&ep->ep_eq->eq_refcnt);
		break;
	default:
		return -FI_EINVAL;
	}

	return 0;
}

static void
usdf_ep_dgram_deref_cq(struct usdf_cq *cq)
{
	struct usdf_cq_hard *hcq;
	void (*rtn)(struct usdf_cq_hard *hcq);

	if (cq == NULL) {
		return;
	}
	ofi_atomic_dec32(&cq->cq_refcnt);

	rtn = usdf_progress_hard_cq;

	if (cq->cq_is_soft) {
		TAILQ_FOREACH(hcq, &cq->c.soft.cq_list, cqh_link) {
			if (hcq->cqh_progress == rtn) {
				ofi_atomic_dec32(&hcq->cqh_refcnt);
				return;
			}
		}
	}
}

static int
usdf_ep_dgram_close(fid_t fid)
{
	struct usdf_ep *ep;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	ep = ep_fidtou(fid);

	if (ofi_atomic_get32(&ep->ep_refcnt) > 0) {
		return -FI_EBUSY;
	}

	free(ep->e.dg.ep_hdr_ptr);

	if (ep->e.dg.ep_qp != NULL) {
		usd_destroy_qp(ep->e.dg.ep_qp);
	}
	ofi_atomic_dec32(&ep->ep_domain->dom_refcnt);
	if (ep->ep_eq != NULL) {
		ofi_atomic_dec32(&ep->ep_eq->eq_refcnt);
	}

	if (ep->e.dg.ep_av)
		ofi_atomic_dec32(&ep->e.dg.ep_av->av_refcnt);

	usdf_ep_dgram_deref_cq(ep->e.dg.ep_wcq);
	usdf_ep_dgram_deref_cq(ep->e.dg.ep_rcq);

	if (ep->e.dg.ep_sock != -1) {
		close(ep->e.dg.ep_sock);
	}

	free(ep);
	return 0;
}

static struct fi_ops_ep usdf_base_dgram_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = usdf_dgram_rx_size_left,
	.tx_size_left = usdf_dgram_tx_size_left,
};

static struct fi_ops_ep usdf_base_dgram_prefix_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = fi_no_cancel,
	.getopt = fi_no_getopt,
	.setopt = fi_no_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = usdf_dgram_prefix_rx_size_left,
	.tx_size_left = usdf_dgram_prefix_tx_size_left,
};

static struct fi_ops_msg usdf_dgram_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = usdf_dgram_recv,
	.recvv = usdf_dgram_recvv,
	.recvmsg = usdf_dgram_recvmsg,
	.send = usdf_dgram_send,
	.sendv = usdf_dgram_sendv,
	.sendmsg = usdf_dgram_sendmsg,
	.inject = usdf_dgram_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

static struct fi_ops_msg usdf_dgram_prefix_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = usdf_dgram_prefix_recv,
	.recvv = usdf_dgram_prefix_recvv,
	.recvmsg = usdf_dgram_prefix_recvmsg,
	.send = usdf_dgram_prefix_send,
	.sendv = usdf_dgram_prefix_sendv,
	.sendmsg = usdf_dgram_prefix_sendmsg,
	.inject = usdf_dgram_prefix_inject,
	.senddata = fi_no_msg_senddata,
	.injectdata = fi_no_msg_injectdata,
};

static struct fi_ops_cm usdf_cm_dgram_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = usdf_cm_dgram_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static struct fi_ops_atomic usdf_dgram_atomic_ops = {
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

/*******************************************************************************
 * Default values for dgram attributes
 ******************************************************************************/
static const struct fi_tx_attr dgram_dflt_tx_attr = {
	.caps = USDF_DGRAM_CAPS,
	.mode = USDF_DGRAM_SUPP_MODE,
	.op_flags = 0,
	.msg_order = USDF_DGRAM_MSG_ORDER,
	.comp_order = USDF_DGRAM_COMP_ORDER,
	.inject_size = USDF_DGRAM_INJECT_SIZE,
	.iov_limit = USDF_DGRAM_IOV_LIMIT,
	.rma_iov_limit = USDF_DGRAM_RMA_IOV_LIMIT
};

static const struct fi_rx_attr dgram_dflt_rx_attr = {
	.caps = USDF_DGRAM_CAPS,
	.mode = USDF_DGRAM_SUPP_MODE,
	.op_flags = 0,
	.msg_order = USDF_DGRAM_MSG_ORDER,
	.comp_order = USDF_DGRAM_COMP_ORDER,
	.total_buffered_recv = 0,
	.iov_limit = USDF_DGRAM_IOV_LIMIT
};

static const struct fi_ep_attr dgram_dflt_ep_attr = {
	.type = FI_EP_DGRAM,
	.protocol = FI_PROTO_UDP,
	.msg_prefix_size = 0,
	.max_order_raw_size = 0,
	.max_order_war_size = 0,
	.max_order_waw_size = 0,
	.mem_tag_format = 0,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1
};

static const struct fi_domain_attr dgram_dflt_domain_attr = {
	.caps = USDF_DOM_CAPS,
	.threading = FI_THREAD_ENDPOINT,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_MANUAL,
	.resource_mgmt = FI_RM_DISABLED,
	.mr_mode = FI_MR_ALLOCATED | FI_MR_LOCAL | FI_MR_BASIC,
	.cntr_cnt = USDF_DGRAM_CNTR_CNT,
	.mr_iov_limit = USDF_DGRAM_MR_IOV_LIMIT,
	.mr_cnt = USDF_DGRAM_MR_CNT,
};

/*******************************************************************************
 * Fill functions for attributes
 ******************************************************************************/
int usdf_dgram_fill_ep_attr(uint32_t version, const struct fi_info *hints, struct
		fi_info *fi, struct usd_device_attrs *dap)
{
	struct fi_ep_attr defaults;

	defaults = dgram_dflt_ep_attr;

	/* The ethernet header does not count against the MTU. */
	defaults.max_msg_size = dap->uda_mtu - sizeof(struct usd_udp_hdr);

	if (FI_VERSION_GE(version, FI_VERSION(1, 3)))
		defaults.max_msg_size += sizeof(struct ether_header);

	if (!hints || !hints->ep_attr)
		goto out;

	/* In prefix mode the max message size is the same as in non-prefix mode
	 * with the advertised header size added on top.
	 */

	if (hints->mode & FI_MSG_PREFIX) {
		defaults.msg_prefix_size = USDF_HDR_BUF_ENTRY;

		if (FI_VERSION_GE(version, FI_VERSION(1, 3)))
			defaults.max_msg_size += defaults.msg_prefix_size;
	}

	if (hints->ep_attr->max_msg_size > defaults.max_msg_size)
		return -FI_ENODATA;

	switch (hints->ep_attr->protocol) {
	case FI_PROTO_UNSPEC:
	case FI_PROTO_UDP:
		break;
	default:
		return -FI_ENODATA;
	}

	if (hints->ep_attr->tx_ctx_cnt > defaults.tx_ctx_cnt)
		return -FI_ENODATA;
	if (hints->ep_attr->rx_ctx_cnt > defaults.rx_ctx_cnt)
		return -FI_ENODATA;

	if (hints->ep_attr->max_order_raw_size > defaults.max_order_raw_size)
		return -FI_ENODATA;
	if (hints->ep_attr->max_order_war_size > defaults.max_order_war_size)
		return -FI_ENODATA;
	if (hints->ep_attr->max_order_waw_size > defaults.max_order_waw_size)
		return -FI_ENODATA;

out:
	*fi->ep_attr = defaults;

	return FI_SUCCESS;
}

int usdf_dgram_fill_dom_attr(uint32_t version, const struct fi_info *hints,
			     struct fi_info *fi, struct usd_device_attrs *dap)
{
	int ret;
	struct fi_domain_attr defaults;

	defaults = dgram_dflt_domain_attr;
	ret = usdf_domain_getname(version, dap, &defaults.name);
	if (ret < 0)
		return -FI_ENODATA;

	if (!hints || !hints->domain_attr)
		goto catch;

	switch (hints->domain_attr->threading) {
	case FI_THREAD_UNSPEC:
	case FI_THREAD_ENDPOINT:
		break;
	case FI_THREAD_FID:
	case FI_THREAD_COMPLETION:
	case FI_THREAD_DOMAIN:
		defaults.threading = hints->domain_attr->threading;
		break;
	default:
		return -FI_ENODATA;
	}

	switch (hints->domain_attr->control_progress) {
	case FI_PROGRESS_UNSPEC:
	case FI_PROGRESS_AUTO:
		break;
	case FI_PROGRESS_MANUAL:
		defaults.control_progress =
			hints->domain_attr->control_progress;
		break;
	default:
		return -FI_ENODATA;
	}

	switch (hints->domain_attr->data_progress) {
	case FI_PROGRESS_UNSPEC:
	case FI_PROGRESS_MANUAL:
		break;
	default:
		return -FI_ENODATA;
	}

	switch (hints->domain_attr->resource_mgmt) {
	case FI_RM_UNSPEC:
	case FI_RM_DISABLED:
		break;
	default:
		return -FI_ENODATA;
	}

	switch (hints->domain_attr->caps) {
	case 0:
	case FI_REMOTE_COMM:
		break;
	default:
		USDF_WARN_SYS(DOMAIN,
			"invalid domain capabilities\n");
		return -FI_ENODATA;
	}

	switch (hints->domain_attr->av_type) {
	case FI_AV_UNSPEC:
	case FI_AV_MAP:
		break;
	default:
		return -FI_ENODATA;
	}

	if (ofi_check_mr_mode(&usdf_ops, version, defaults.mr_mode, hints))
		return -FI_ENODATA;

	if (hints->domain_attr->mr_cnt) {
		if (hints->domain_attr->mr_cnt <= USDF_DGRAM_MR_CNT) {
			defaults.mr_cnt = hints->domain_attr->mr_cnt;
		} else {
			USDF_DBG_SYS(DOMAIN,
				     "mr_count exceeded provider limit\n");
			return -FI_ENODATA;
		}
	}

catch:
	/* catch the version change here. */
	ret = usdf_catch_dom_attr(version, hints, &defaults);
	if (ret)
		return ret;

	*fi->domain_attr = defaults;
	return FI_SUCCESS;
}

int usdf_dgram_fill_tx_attr(uint32_t version, const struct fi_info *hints,
			    struct fi_info *fi,
			    struct usd_device_attrs *dap)
{
	int ret;
	struct fi_tx_attr defaults;
	size_t entries;

	defaults = dgram_dflt_tx_attr;

	defaults.size = dap->uda_max_send_credits / defaults.iov_limit;

	if (!hints || !hints->tx_attr)
		goto out;

	/* make sure we can support the capabilities that are requested */
	if (hints->tx_attr->caps & ~USDF_DGRAM_CAPS)
		return -FI_ENODATA;

	/* clear the mode bits the app doesn't support */
	if (hints->mode || hints->tx_attr->mode)
		defaults.mode &= (hints->mode | hints->tx_attr->mode);

	defaults.op_flags |= hints->tx_attr->op_flags;

	if ((hints->tx_attr->msg_order | USDF_DGRAM_MSG_ORDER) !=
			USDF_DGRAM_MSG_ORDER)
		return -FI_ENODATA;
	if ((hints->tx_attr->comp_order | USDF_DGRAM_COMP_ORDER) !=
			USDF_DGRAM_COMP_ORDER)
		return -FI_ENODATA;

	if (hints->tx_attr->inject_size > defaults.inject_size)
		return -FI_ENODATA;

	if (hints->tx_attr->iov_limit > USDF_DGRAM_MAX_SGE)
		return -FI_ENODATA;

	/* make sure the values for iov_limit and size are within appropriate
	 * bounds. if only one of the two was given, then set the other based
	 * on:
	 * 	max_credits = size * iov_limit;
	 */
	if (hints->tx_attr->iov_limit && hints->tx_attr->size) {
		defaults.size = hints->tx_attr->size;
		defaults.iov_limit = hints->tx_attr->iov_limit;
	} else if (hints->tx_attr->iov_limit) {
		defaults.iov_limit = hints->tx_attr->iov_limit;
		defaults.size =
			dap->uda_max_send_credits / defaults.iov_limit;
	} else if (hints->tx_attr->size) {
		defaults.size = hints->tx_attr->size;
		defaults.iov_limit =
			dap->uda_max_send_credits / defaults.size;
	}

	entries = defaults.size * defaults.iov_limit;
	if (entries > dap->uda_max_send_credits)
		return -FI_ENODATA;

	if (hints->tx_attr->rma_iov_limit > defaults.rma_iov_limit)
		return -FI_ENODATA;

out:
	/* Non-prefix mode requires extra descriptor for header.
	 */
	if (!hints || (hints && !(hints->mode & FI_MSG_PREFIX)))
		defaults.iov_limit -= 1;

	/* catch version changes here. */
	ret = usdf_catch_tx_attr(version, &defaults);
	if (ret)
		return ret;

	*fi->tx_attr = defaults;

	return FI_SUCCESS;
}

int usdf_dgram_fill_rx_attr(uint32_t version, const struct fi_info *hints,
			    struct fi_info *fi, struct usd_device_attrs *dap)
{
	int ret;
	struct fi_rx_attr defaults;
	size_t entries;

	defaults = dgram_dflt_rx_attr;

	defaults.size = dap->uda_max_recv_credits / defaults.iov_limit;

	if (!hints || !hints->rx_attr)
		goto out;

	/* make sure we can support the capabilities that are requested */
	if (hints->rx_attr->caps & ~USDF_DGRAM_CAPS)
		return -FI_ENODATA;

	/* clear the mode bits the app doesn't support */
	if (hints->mode || hints->tx_attr->mode)
		defaults.mode &= (hints->mode | hints->rx_attr->mode);

	defaults.op_flags |= hints->rx_attr->op_flags;

	if ((hints->rx_attr->msg_order | USDF_DGRAM_MSG_ORDER) !=
			USDF_DGRAM_MSG_ORDER)
		return -FI_ENODATA;
	if ((hints->rx_attr->comp_order | USDF_DGRAM_COMP_ORDER) !=
			USDF_DGRAM_COMP_ORDER)
		return -FI_ENODATA;

	if (hints->rx_attr->total_buffered_recv >
			defaults.total_buffered_recv)
		return -FI_ENODATA;

	if (hints->rx_attr->iov_limit > USDF_DGRAM_MAX_SGE)
		return -FI_ENODATA;

	/* make sure the values for iov_limit and size are within appropriate
	 * bounds. if only one of the two was given, then set the other based
	 * on:
	 * 	max_credits = size * iov_limit;
	 */
	if (hints->rx_attr->iov_limit && hints->rx_attr->size) {
		defaults.size = hints->rx_attr->size;
		defaults.iov_limit = hints->rx_attr->iov_limit;
	} else if (hints->rx_attr->iov_limit) {
		defaults.iov_limit = hints->rx_attr->iov_limit;
		defaults.size =
			dap->uda_max_recv_credits / defaults.iov_limit;
	} else if (hints->rx_attr->size) {
		defaults.size = hints->rx_attr->size;
		defaults.iov_limit =
			dap->uda_max_recv_credits / defaults.size;
	}

	entries = defaults.size * defaults.iov_limit;
	if (entries > dap->uda_max_recv_credits)
		return -FI_ENODATA;

out:
	/* Non-prefix mode requires extra descriptor for header.
	 */
	if (!hints || (hints && !(hints->mode & FI_MSG_PREFIX)))
		defaults.iov_limit -= 1;

	/* catch version changes here. */
	ret = usdf_catch_rx_attr(version, &defaults);
	if (ret)
		return ret;

	*fi->rx_attr = defaults;

	return FI_SUCCESS;
}

static int usdf_ep_dgram_control(struct fid *fid, int command, void *arg)
{
	struct fid_ep *ep;
	int ret;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	switch (fid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(fid, struct fid_ep, fid);
		switch (command) {
		case FI_ENABLE:
			ret = usdf_ep_dgram_enable(ep);
			break;
		default:
			ret = -FI_ENOSYS;
		}
		break;
	default:
		ret = -FI_ENOSYS;
	}

	return ret;
}

static struct fi_ops usdf_ep_dgram_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_ep_dgram_close,
	.bind = usdf_ep_dgram_bind,
	.control = usdf_ep_dgram_control,
	.ops_open = fi_no_ops_open
};

int
usdf_ep_dgram_open(struct fid_domain *domain, struct fi_info *info,
	    struct fid_ep **ep_o, void *context)
{
	struct usdf_domain *udp;
	struct usdf_ep *ep;
	int ret;
	struct usdf_pep *parent_pep;
	void *src_addr;
	int is_bound;
	size_t tx_size;
	size_t rx_size;

	USDF_TRACE_SYS(EP_CTRL, "\n");

	parent_pep = NULL;
	src_addr = NULL;

	if ((info->caps & ~USDF_DGRAM_CAPS) != 0) {
		return -FI_EBADF;
	}

	if (info->handle != NULL) {
		if (info->handle->fclass != FI_CLASS_PEP) {
			USDF_WARN_SYS(EP_CTRL,
				"\"handle\" should be a PEP (or NULL)\n");
			return -FI_EINVAL;
		}
		parent_pep = pep_fidtou(info->handle);
	}

	udp = dom_ftou(domain);

	ep = calloc(1, sizeof(*ep));
	if (ep == NULL) {
		return -FI_ENOMEM;
	}

	is_bound = 0;
	if (parent_pep != NULL) {
		ret = usdf_pep_steal_socket(parent_pep, &is_bound, &ep->e.dg.ep_sock);
		if (ret) {
			goto fail;
		}
	} else {
		ep->e.dg.ep_sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (ep->e.dg.ep_sock == -1) {
			ret = -errno;
			goto fail;
		}
	}

	if (!is_bound) {
		if (info->src_addr != NULL)
			src_addr = usdf_format_to_sin(info, info->src_addr);

		if (src_addr != NULL) {
			ret = bind(ep->e.dg.ep_sock, src_addr,
				sizeof(struct sockaddr_in));
			if (ret == -1) {
				ret = -errno;
				goto fail;
			}
		}

		usdf_free_sin_if_needed(info, src_addr);
	}

	ep->ep_fid.fid.fclass = FI_CLASS_EP;
	ep->ep_fid.fid.context = context;
	ep->ep_fid.fid.ops = &usdf_ep_dgram_ops;
	ep->ep_fid.cm = &usdf_cm_dgram_ops;
	ep->ep_fid.atomic = &usdf_dgram_atomic_ops;
	ep->ep_domain = udp;
	ep->ep_caps = info->caps;
	ep->ep_mode = info->mode;

	ep->e.dg.tx_iov_limit = USDF_DGRAM_IOV_LIMIT;
	tx_size = udp->dom_fabric->fab_dev_attrs->uda_max_send_credits /
		ep->e.dg.tx_iov_limit;

	ep->e.dg.rx_iov_limit = USDF_DGRAM_IOV_LIMIT;
	rx_size = udp->dom_fabric->fab_dev_attrs->uda_max_recv_credits /
		ep->e.dg.rx_iov_limit;

	/*
	 * TODO: Add better management of tx_attr/rx_attr to getinfo and dgram
	 * open.
	 */
	if (info->tx_attr) {
		ep->e.dg.tx_op_flags = info->tx_attr->op_flags;
		if (info->tx_attr->iov_limit)
			ep->e.dg.tx_iov_limit = info->tx_attr->iov_limit;
		if (info->tx_attr->size)
			tx_size = info->tx_attr->size;
	}

	if (info->rx_attr) {
		ep->e.dg.rx_op_flags = info->rx_attr->op_flags;
		if (info->rx_attr->iov_limit)
			ep->e.dg.rx_iov_limit = info->rx_attr->iov_limit;
		if (info->rx_attr->size)
			rx_size = info->rx_attr->size;
	}

	if (info->ep_attr)
		ep->max_msg_size = info->ep_attr->max_msg_size;

	if (ep->ep_mode & FI_MSG_PREFIX) {
		ep->ep_wqe = tx_size * ep->e.dg.tx_iov_limit;
		ep->ep_rqe = rx_size * ep->e.dg.rx_iov_limit;
	} else {
		ep->ep_wqe = tx_size * (ep->e.dg.tx_iov_limit + 1);
		ep->ep_rqe = rx_size * (ep->e.dg.rx_iov_limit + 1);
	}

	/* Check that the requested credit size is less than the max credit
	 * counts. If the fi_info struct was acquired from fi_getinfo then this
	 * will always be the case.
	 */
	if (ep->ep_wqe > udp->dom_fabric->fab_dev_attrs->uda_max_send_credits) {
		ret = -FI_EINVAL;
		goto fail;
	}
	if (ep->ep_rqe > udp->dom_fabric->fab_dev_attrs->uda_max_recv_credits) {
		ret = -FI_EINVAL;
		goto fail;
	}

	if (ep->ep_mode & FI_MSG_PREFIX) {
		if (info->ep_attr == NULL) {
			ret = -FI_EBADF;
			goto fail;
		}

		ep->ep_fid.ops = &usdf_base_dgram_prefix_ops;
		info->ep_attr->msg_prefix_size = USDF_HDR_BUF_ENTRY;
		ep->ep_fid.msg = &usdf_dgram_prefix_ops;
	} else {
		ep->ep_fid.ops = &usdf_base_dgram_ops;
		ep->ep_fid.msg = &usdf_dgram_ops;
	}
	ofi_atomic_initialize32(&ep->ep_refcnt, 0);
	ofi_atomic_inc32(&udp->dom_refcnt);

	*ep_o = ep_utof(ep);
	return 0;

fail:
	if (ep != NULL) {
		if (ep->e.dg.ep_sock != -1) {
			close(ep->e.dg.ep_sock);
		}
		free(ep);
	}
	return ret;
}
