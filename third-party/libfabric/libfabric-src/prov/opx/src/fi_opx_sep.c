/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2023 Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"

#include <ofi.h>
#include <ofi_enosys.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

/* forward declaration */
int fi_opx_endpoint_rx_tx (struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context);

static int fi_opx_close_sep(fid_t fid)
{
	int ret;
	struct fi_opx_sep *opx_sep = container_of(fid, struct fi_opx_sep, ep_fid);

	ret = fi_opx_fid_check(fid, FI_CLASS_SEP, "scalable endpoint");
	if (ret)
		return ret;

	fi_opx_finalize_cm_ops(&opx_sep->ep_fid.fid);

	ret = fi_opx_ref_dec(&opx_sep->av->ref_cnt, "address vector");
	if (ret)
		return ret;

    // TODO:  Are these cleaned up
    ret = fi_opx_ref_dec(&opx_sep->ref_cnt, "rx");
    ret = fi_opx_ref_dec(&opx_sep->ref_cnt, "tx");

	ret = fi_opx_ref_finalize(&opx_sep->ref_cnt, "scalable endpoint");
	if (ret)
		return ret;

	ret = fi_opx_ref_dec(&opx_sep->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	free(opx_sep->info->ep_attr);
	opx_sep->info->ep_attr = NULL;
	free(opx_sep->info);
	opx_sep->info = NULL;
	void * memptr = opx_sep->memptr;
	free(memptr);
	//opx_sep (the object passed in as fid) is now unusable

	return 0;
}

static int fi_opx_control_sep(fid_t fid, int command, void *arg)
{
	struct fid_ep *ep __attribute__ ((unused));
	ep = container_of(fid, struct fid_ep, fid);
	return 0;
}

static int fi_opx_tx_ctx(struct fid_ep *sep, int index,
			struct fi_tx_attr *attr, struct fid_ep **tx_ep,
			void *context)
{
	int ret;
	struct fi_info info = {0};
	struct fi_tx_attr tx_attr = {0};
	struct fi_ep_attr ep_attr = {0};
	struct fi_domain_attr dom_attr = {0};
	struct fi_fabric_attr fab_attr = {0};
	struct fi_opx_sep *opx_sep;
	struct fi_opx_ep  *opx_tx_ep;


	if (!sep || !attr || !tx_ep) {
		errno = FI_EINVAL;
		return -errno;
	}
	opx_sep = container_of(sep, struct fi_opx_sep, ep_fid);

	/* "By default, a transmit context inherits the properties of its associated endpoint.
 	 * However, applications may request context specific attributes through the attr
 	 * parameter." */
	uint64_t caps = attr->caps;

	if (caps == 0) {
		caps = (FI_OPX_BASE_CAPS | FI_OPX_TXONLY_CAPS);
	}

	if ((caps & FI_MSG || caps & FI_TAGGED) && (caps & FI_RECV)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"FI_MSG|FI_TAGGED with FI_RECV capability specified for a TX context\n");
		caps &= ~FI_RECV;
	}
	caps &= ~FI_RECV;

	if ((caps & FI_RMA || caps & FI_ATOMIC) && (caps & FI_REMOTE_READ || caps & FI_REMOTE_WRITE)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"FI_RMA|FI_ATOMIC with FI_REMOTE_READ|FI_REMOTE_WRITE capability specified for a TX context\n");
		caps &= ~FI_REMOTE_READ;
		caps &= ~FI_REMOTE_WRITE;
	}

	if (caps & FI_MSG || caps & FI_TAGGED) {
		caps |= FI_SEND;
	}

	if (caps & FI_RMA || caps & FI_ATOMIC) {
		caps |= FI_READ;
		caps |= FI_WRITE;
	}

	if (ofi_recv_allowed(caps) || ofi_rma_target_allowed(caps)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"RX capabilities specified for TX context\n");
		errno = FI_EINVAL;
		return -errno;
	}

	if (!ofi_send_allowed(caps) && !ofi_rma_initiate_allowed(caps)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"TX capabilities not specified for TX context\n");
		errno = FI_EINVAL;
		return -errno;
	}

	if (opx_sep->domain->tx_count >= fi_opx_domain_get_tx_max((struct fid_domain *)opx_sep->domain)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"TX ctx count exceeded (max %u, created %u)\n",
				fi_opx_domain_get_tx_max((struct fid_domain *)opx_sep->domain), opx_sep->domain->tx_count);
		errno = FI_EINVAL;
		return -errno;
	}

	info.caps = caps;
	info.mode = attr->mode;

	info.tx_attr = &tx_attr;
	memcpy(info.tx_attr, attr, sizeof(*info.tx_attr));

	info.ep_attr = &ep_attr;
	memcpy(info.ep_attr, opx_sep->info->ep_attr, sizeof(*info.ep_attr));

	info.domain_attr = &dom_attr;
	memcpy(info.domain_attr, opx_sep->info->domain_attr, sizeof(*info.domain_attr));

	info.fabric_attr = &fab_attr;
	memcpy(info.fabric_attr, opx_sep->info->fabric_attr, sizeof(*info.fabric_attr));

	ret = fi_opx_endpoint_rx_tx((struct fid_domain *)opx_sep->domain,
		&info, tx_ep, context);
	if (ret) {
		goto err;
	}

	opx_tx_ep = container_of(*tx_ep, struct fi_opx_ep, ep_fid);
	opx_tx_ep->ep_fid.fid.fclass = FI_CLASS_TX_CTX;
    opx_tx_ep->tx_info = fi_dupinfo(&info);

	opx_tx_ep->av = opx_sep->av;
	fi_opx_ref_inc(&opx_tx_ep->av->ref_cnt, "address vector");

	opx_tx_ep->sep = container_of(sep, struct fi_opx_sep, ep_fid);

	++ opx_sep->domain->tx_count;

	fi_opx_ref_inc(&opx_sep->ref_cnt, "scalable endpoint");

	attr->caps = caps;

	return 0;

err:
	return -errno;
}

static int fi_opx_rx_ctx(struct fid_ep *sep, int index,
			struct fi_rx_attr *attr, struct fid_ep **rx_ep,
			void *context)
{
	struct fi_info info = {0};
	struct fi_opx_sep *opx_sep;
	struct fi_opx_ep  *opx_rx_ep;

	if (!sep || !attr || !rx_ep || (index < 0) || (index >= FI_OPX_ADDR_SEP_RX_MAX)) {
		errno = FI_EINVAL;
		return -errno;
	}

	opx_sep = container_of(sep, struct fi_opx_sep, ep_fid);
	/* "By default, a receive context inherits the properties of its
 	 * associated endpoint. However, applications may request context
 	 * specific attributes through the attr parameter." */
	uint64_t caps = attr->caps;

	if (caps == 0) {
		caps = (FI_OPX_BASE_CAPS | FI_OPX_RXONLY_CAPS);
	}

	if ((caps & FI_MSG || caps & FI_TAGGED) && (caps & FI_SEND)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"FI_MSG|FI_TAGGED with FI_SEND capability specified for a RX context\n");
		caps &= ~FI_SEND;
	}

	if ((caps & FI_RMA || caps & FI_ATOMIC) && (caps & FI_READ || caps & FI_WRITE)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"FI_RMA|FI_ATOMIC with FI_READ|FI_WRITE capability specified for a RX context\n");
		caps &= ~FI_READ;
		caps &= ~FI_WRITE;
	}

	if (caps & FI_MSG || caps & FI_TAGGED) {
		caps |= FI_RECV;
	}

	if (caps & FI_RMA || caps & FI_ATOMIC) {
		caps |= FI_REMOTE_READ;
		caps |= FI_REMOTE_WRITE;
	}

	if (ofi_send_allowed(caps) || ofi_rma_initiate_allowed(caps)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"TX capabilities specified for RX context\n");
		errno = FI_EINVAL;
		return -errno;
	}

	if (!ofi_recv_allowed(caps) && !ofi_rma_target_allowed(caps)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"RX capabilities not specified for RX context\n");
		errno = FI_EINVAL;
		return -errno;
	}

	if (opx_sep->domain->rx_count >= fi_opx_domain_get_rx_max((struct fid_domain *)opx_sep->domain)) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"RX ctx count exceeded (max %u, created %u)\n",
				fi_opx_domain_get_rx_max((struct fid_domain *)opx_sep->domain), opx_sep->domain->rx_count);
		errno = FI_EINVAL;
		return -errno;
	}

	info.caps = caps;
	info.mode = attr->mode;

	info.rx_attr = calloc(1, sizeof(*info.rx_attr));
	if (!info.rx_attr) {
		errno = FI_ENOMEM;
		goto err;
	}

	info.rx_attr->caps     = caps;
	info.rx_attr->mode     = attr->mode;
	info.rx_attr->op_flags = attr->op_flags;
	info.rx_attr->msg_order = attr->msg_order;
	info.rx_attr->total_buffered_recv = attr->total_buffered_recv;
	info.rx_attr->iov_limit = attr->iov_limit;

	info.ep_attr = calloc(1, sizeof(*info.ep_attr));
	if (!info.ep_attr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(info.ep_attr, opx_sep->info->ep_attr,
			sizeof(*info.ep_attr));

	info.domain_attr = calloc(1, sizeof(*info.domain_attr));
	if (!info.domain_attr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(info.domain_attr, opx_sep->info->domain_attr,
			sizeof(*info.domain_attr));

	info.fabric_attr = calloc(1, sizeof(*info.fabric_attr));
	if (!info.fabric_attr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(info.fabric_attr, opx_sep->info->fabric_attr,
			sizeof(*info.fabric_attr));


	fi_opx_endpoint_rx_tx(&opx_sep->domain->domain_fid, &info,
		rx_ep, context);

	opx_rx_ep = container_of(*rx_ep, struct fi_opx_ep, ep_fid);
	opx_rx_ep->ep_fid.fid.fclass = FI_CLASS_RX_CTX;
    opx_rx_ep->rx_info = fi_dupinfo(&info);


	opx_sep->ep[index] = opx_rx_ep;


	/* check for non-contiguous rx ids */
	if (index > 0) {
		unsigned i;
		for (i=0; i<index; ++i) {
			if (opx_sep->ep[i]->rx->self.hfi1_rx + 1 != opx_sep->ep[i+1]->rx->self.hfi1_rx) {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__); abort();
			}
		}
	}


	opx_rx_ep->sep = container_of(sep, struct fi_opx_sep, ep_fid);

	opx_rx_ep->av = opx_sep->av;
	fi_opx_ref_inc(&opx_rx_ep->av->ref_cnt, "address vector");

	++ opx_sep->domain->rx_count;

	fi_opx_ref_inc(&opx_sep->ref_cnt, "scalable endpoint");

	free(info.fabric_attr);
	free(info.domain_attr);
	free(info.ep_attr);
	free(info.tx_attr);
	free(info.rx_attr);

	return 0;

err:
	if (info.fabric_attr)
		free(info.fabric_attr);
	if (info.domain_attr)
		free(info.domain_attr);
	if (info.ep_attr)
		free(info.ep_attr);
	if (info.tx_attr)
		free(info.tx_attr);

	info.fabric_attr = NULL;
	info.domain_attr = NULL;
	info.ep_attr = NULL;
	info.tx_attr = NULL;
	
	return -errno;
}

static int fi_opx_bind_sep(struct fid *fid, struct fid *bfid,
		uint64_t flags)
{
	int ret = 0;
	struct fi_opx_sep *opx_sep = container_of(fid, struct fi_opx_sep, ep_fid);
	struct fi_opx_av *opx_av;

	if (!fid || !bfid) {
		errno = FI_EINVAL;
		return -errno;
	}

	switch (bfid->fclass) {
	case FI_CLASS_AV:
		opx_av = container_of(bfid, struct fi_opx_av, av_fid);
		fi_opx_ref_inc(&opx_av->ref_cnt, "address vector");
		opx_sep->av = opx_av;
		break;
	default:
		errno = FI_ENOSYS;
		return -errno;
	}

	return ret;
}

static struct fi_ops fi_opx_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_opx_close_sep,
	.bind		= fi_opx_bind_sep,
	.control	= fi_opx_control_sep,
	.ops_open	= fi_no_ops_open
};

static struct fi_ops_ep fi_opx_sep_ops = {
	.size		= sizeof(struct fi_ops_ep),
	.cancel		= fi_no_cancel,
	.getopt		= fi_no_getopt,
	.setopt		= fi_no_setopt,
	.tx_ctx		= fi_opx_tx_ctx,
	.rx_ctx		= fi_opx_rx_ctx,
	.rx_size_left   = fi_no_rx_size_left,
	.tx_size_left   = fi_no_tx_size_left
};

int fi_opx_scalable_ep (struct fid_domain *domain,
	struct fi_info *info,
	struct fid_ep **sep,
	void *context)
{
	struct fi_opx_sep *opx_sep = NULL;

	if (!info || !domain) {
		errno = FI_EINVAL;
		goto err;
	}

	void * memptr = NULL;
	memptr = malloc(sizeof(struct fi_opx_sep)+L2_CACHE_LINE_SIZE);
	if (!memptr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memset(memptr, 0, sizeof(struct fi_opx_sep)+L2_CACHE_LINE_SIZE);
	opx_sep = (struct fi_opx_sep *)(((uintptr_t)memptr+L2_CACHE_LINE_SIZE) & ~(L2_CACHE_LINE_SIZE-1));
	opx_sep->memptr = memptr;
	memptr = NULL;

	opx_sep->domain = (struct fi_opx_domain *) domain;

	opx_sep->ep_fid.fid.fclass	= FI_CLASS_SEP;
	opx_sep->ep_fid.fid.context	= context;
	opx_sep->ep_fid.fid.ops		= &fi_opx_fi_ops;
	opx_sep->ep_fid.ops		= &fi_opx_sep_ops;

	opx_sep->info = calloc(1, sizeof (struct fi_info));
	if (!opx_sep->info) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(opx_sep->info, info, sizeof (struct fi_info));
	opx_sep->info->next = NULL;
	opx_sep->info->ep_attr = calloc(1, sizeof(struct fi_ep_attr));
	if (!opx_sep->info->ep_attr) {
		errno = FI_ENOMEM;
		goto err;
	}
	memcpy(opx_sep->info->ep_attr, info->ep_attr, sizeof(struct fi_ep_attr));

	/*
	 * fi_endpoint.3
	 *
	 * "tx_ctx_cnt - Transmit Context Count
	 * 	Number of transmit contexts to associate with the endpoint. If
	 * 	not specified (0), 1 context will be assigned if the endpoint
	 * 	supports outbound transfers."
	 */
	if (0 == opx_sep->info->ep_attr->tx_ctx_cnt) {
		opx_sep->info->ep_attr->tx_ctx_cnt = 1;
	}

	/*
	 * fi_endpoint.3
	 *
	 * "rx_ctx_cnt - Receive Context Count
	 * 	Number of receive contexts to associate with the endpoint. If
	 * 	not specified, 1 context will be assigned if the endpoint
	 * 	supports inbound transfers."
	 */
	if (0 == opx_sep->info->ep_attr->rx_ctx_cnt) {
		opx_sep->info->ep_attr->rx_ctx_cnt = 1;
	}

	unsigned i;
	for (i=0; i<FI_OPX_ADDR_SEP_RX_MAX; ++i) {
		opx_sep->ep[i] = NULL;
	}

	int ret = fi_opx_init_cm_ops(&opx_sep->ep_fid.fid, info);
	if (ret)
		goto err;

	fi_opx_ref_init(&opx_sep->ref_cnt, "scalable endpoint");
	fi_opx_ref_inc(&opx_sep->domain->ref_cnt, "domain");

	*sep = &opx_sep->ep_fid;

	return 0;
err:
	if (opx_sep) {
		fi_opx_finalize_cm_ops(&opx_sep->ep_fid.fid);
		if (opx_sep->info) {
			if (opx_sep->info->ep_attr) {
				free(opx_sep->info->ep_attr);
				opx_sep->info->ep_attr = NULL;
			}
			free(opx_sep->info);
			opx_sep->info = NULL;
		}
		memptr = opx_sep->memptr;
		free(memptr);
		opx_sep = NULL; 
	}
	return -errno;
}
