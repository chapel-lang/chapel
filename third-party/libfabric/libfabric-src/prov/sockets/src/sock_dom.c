/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
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

#include <stdlib.h>
#include <string.h>

#include <ofi_util.h>

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_DOMAIN, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_DOMAIN, __VA_ARGS__)

extern struct fi_ops_mr sock_dom_mr_ops;


static int sock_dom_close(struct fid *fid)
{
	struct sock_domain *dom;
	dom = container_of(fid, struct sock_domain, dom_fid.fid);
	if (ofi_atomic_get32(&dom->ref))
		return -FI_EBUSY;

	sock_conn_stop_listener_thread(&dom->conn_listener);
	sock_ep_cm_stop_thread(&dom->cm_head);

	sock_pe_finalize(dom->pe);
	ofi_mutex_destroy(&dom->lock);
	ofi_mr_map_close(&dom->mr_map);
	sock_dom_remove_from_list(dom);
	free(dom);
	return 0;
}

static int sock_dom_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct sock_domain *dom;
	struct sock_eq *eq;

	dom = container_of(fid, struct sock_domain, dom_fid.fid);
	eq = container_of(bfid, struct sock_eq, eq.fid);

	if (dom->eq)
		return -FI_EINVAL;

	dom->eq = eq;
	if (flags & FI_REG_MR)
		dom->mr_eq = eq;

	return 0;
}

static int sock_dom_ctrl(struct fid *fid, int command, void *arg)
{
	struct sock_domain *dom;

	dom = container_of(fid, struct sock_domain, dom_fid.fid);
	switch (command) {
	case FI_QUEUE_WORK:
		return (int) sock_queue_work(dom, arg);
	default:
		return -FI_ENOSYS;
	}
}

static int sock_endpoint(struct fid_domain *domain, struct fi_info *info,
			 struct fid_ep **ep, void *context)
{
	switch (info->ep_attr->type) {
	case FI_EP_RDM:
		return sock_rdm_ep(domain, info, ep, context);
	case FI_EP_DGRAM:
		return sock_dgram_ep(domain, info, ep, context);
	case FI_EP_MSG:
		return sock_msg_ep(domain, info, ep, context);
	default:
		return -FI_ENOPROTOOPT;
	}
}

static int sock_scalable_ep(struct fid_domain *domain, struct fi_info *info,
		     struct fid_ep **sep, void *context)
{
	switch (info->ep_attr->type) {
	case FI_EP_RDM:
		return sock_rdm_sep(domain, info, sep, context);
	case FI_EP_DGRAM:
		return sock_dgram_sep(domain, info, sep, context);
	case FI_EP_MSG:
		return sock_msg_sep(domain, info, sep, context);
	default:
		return -FI_ENOPROTOOPT;
	}
}

static struct fi_ops sock_dom_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_dom_close,
	.bind = sock_dom_bind,
	.control = sock_dom_ctrl,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_domain sock_dom_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = sock_av_open,
	.cq_open = sock_cq_open,
	.endpoint = sock_endpoint,
	.scalable_ep = sock_scalable_ep,
	.cntr_open = sock_cntr_open,
	.poll_open = sock_poll_open,
	.stx_ctx = sock_stx_ctx,
	.srx_ctx = sock_srx_ctx,
	.query_atomic = sock_query_atomic,
	.query_collective = fi_no_query_collective,
};

int sock_domain(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **dom, void *context)
{
	struct sock_domain *sock_domain;
	struct sock_fabric *fab;
	int ret;

	assert(info && info->domain_attr);
	fab = container_of(fabric, struct sock_fabric, fab_fid);

	sock_domain = calloc(1, sizeof(*sock_domain));
	if (!sock_domain)
		return -FI_ENOMEM;

	ofi_mutex_init(&sock_domain->lock);
	ofi_atomic_initialize32(&sock_domain->ref, 0);

	sock_domain->info = *info;
	sock_domain->info.domain_attr = NULL;

	sock_domain->dom_fid.fid.fclass = FI_CLASS_DOMAIN;
	sock_domain->dom_fid.fid.context = context;
	sock_domain->dom_fid.fid.ops = &sock_dom_fi_ops;
	sock_domain->dom_fid.ops = &sock_dom_ops;
	sock_domain->dom_fid.mr = &sock_dom_mr_ops;

	if (info->domain_attr->data_progress == FI_PROGRESS_UNSPEC)
		sock_domain->progress_mode = FI_PROGRESS_AUTO;
	else
		sock_domain->progress_mode = info->domain_attr->data_progress;

	sock_domain->pe = sock_pe_init(sock_domain);
	if (!sock_domain->pe) {
		SOCK_LOG_ERROR("Failed to init PE\n");
		goto err1;
	}

	sock_domain->fab = fab;
	*dom = &sock_domain->dom_fid;

	sock_domain->attr = *(info->domain_attr);

	ret = ofi_mr_map_init(&sock_prov, sock_domain->attr.mr_mode,
			      &sock_domain->mr_map);
	if (ret)
		goto err2;

	ret = sock_conn_start_listener_thread(&sock_domain->conn_listener);
	if (ret)
		goto err2;

	ret = sock_ep_cm_start_thread(&sock_domain->cm_head);
	if (ret)
		goto err3;

	sock_dom_add_to_list(sock_domain);
	return 0;

err3:
	sock_conn_stop_listener_thread(&sock_domain->conn_listener);
err2:
	sock_pe_finalize(sock_domain->pe);
err1:
	ofi_mutex_destroy(&sock_domain->lock);
	free(sock_domain);
	return -FI_EINVAL;
}
