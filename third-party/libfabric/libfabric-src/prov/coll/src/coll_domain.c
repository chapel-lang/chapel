/*
 * Copyright (c) 2022 Intel Corporation, Inc.  All rights reserved.
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

#include "coll.h"

static struct fi_ops_domain coll_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = coll_av_open,
	.cq_open = ofi_coll_cq_open,
	.endpoint = coll_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = fi_no_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = coll_query_collective,
	.endpoint2 = fi_no_endpoint2,
};

static int coll_domain_close(fid_t fid)
{
	struct coll_domain *domain;
	int ret;

	domain = container_of(fid, struct coll_domain, util_domain.domain_fid.fid);

	ret = ofi_domain_close(&domain->util_domain);
	if (ret)
		return ret;

	free(domain);
	return 0;
}

static struct fi_ops coll_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = coll_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_mr coll_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = fi_no_mr_reg,
	.regv = fi_no_mr_regv,
	.regattr = fi_no_mr_regattr,
};

int coll_domain_open2(struct fid_fabric *fabric, struct fi_info *info,
		      struct fid_domain **domain_fid, uint64_t flags,
		      void *context)
{
	struct coll_domain *domain;
	struct fi_peer_domain_context *peer_context = context;
	int ret;

	if (!(flags & FI_PEER)) {
		FI_WARN(&coll_prov, FI_LOG_CORE,
			"FI_PEER flag required\n");
		return -EINVAL;
	}

	if (!peer_context || peer_context->size < sizeof(*peer_context)) {
		FI_WARN(&coll_prov, FI_LOG_CORE,
			"Invalid peer domain context\n");
		return -EINVAL;
	}

	domain = calloc(1, sizeof(*domain));
	if (!domain)
		return -FI_ENOMEM;

	domain->peer_domain = peer_context->domain;

	ret = ofi_domain_init(fabric, info, &domain->util_domain, context,
			      OFI_LOCK_MUTEX);
	if (ret)
		goto err;

	*domain_fid = &domain->util_domain.domain_fid;
	(*domain_fid)->fid.ops = &coll_domain_fi_ops;
	(*domain_fid)->ops = &coll_domain_ops;
	(*domain_fid)->mr = &coll_domain_mr_ops;
	return 0;

err:
	free(domain);
	return ret;
}
