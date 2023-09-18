/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <stdlib.h>
#include <string.h>

#include "tcpx.h"


static struct fi_ops_domain tcpx_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = ofi_ip_av_create,
	.cq_open = tcpx_cq_open,
	.endpoint = tcpx_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = tcpx_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = tcpx_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

static int tcpx_set_ops(struct fid *fid, const char *name,
			uint64_t flags, void *ops, void *context)
{
	struct tcpx_domain *domain;

	domain = container_of(fid, struct tcpx_domain,
			      util_domain.domain_fid.fid);
	if (flags)
		return -FI_EBADFLAGS;

	if (!strcasecmp(name, OFI_OPS_DYNAMIC_RBUF)) {
		domain->dynamic_rbuf = ops;
		if (domain->dynamic_rbuf->size != sizeof(*domain->dynamic_rbuf)) {
			domain->dynamic_rbuf = NULL;
			return -FI_ENOSYS;
		}

		return 0;
	}

	return -FI_ENOSYS;
}

static int tcpx_domain_close(fid_t fid)
{
	struct tcpx_domain *domain;
	int ret;

	domain = container_of(fid, struct tcpx_domain,
			      util_domain.domain_fid.fid);

	ret = ofi_domain_close(&domain->util_domain);
	if (ret)
		return ret;

	free(domain);
	return FI_SUCCESS;
}

static struct fi_ops tcpx_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_domain_close,
	.bind = ofi_domain_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
	.tostr = NULL,
	.ops_set = tcpx_set_ops,
};

static struct fi_ops_mr tcpx_domain_fi_ops_mr = {
	.size = sizeof(struct fi_ops_mr),
	.reg = ofi_mr_reg,
	.regv = ofi_mr_regv,
	.regattr = ofi_mr_regattr,
};

int tcpx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		     struct fid_domain **domain_fid, void *context)
{
	struct tcpx_domain *domain;
	int ret;

	ret = ofi_prov_check_info(&tcpx_util_prov, fabric->api_version, info);
	if (ret)
		return ret;

	domain = calloc(1, sizeof(*domain));
	if (!domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(fabric, info, &domain->util_domain, context,
			      OFI_LOCK_MUTEX);
	if (ret)
		goto err;

	domain->util_domain.domain_fid.fid.ops = &tcpx_domain_fi_ops;
	domain->util_domain.domain_fid.ops = &tcpx_domain_ops;
	domain->util_domain.domain_fid.mr = &tcpx_domain_fi_ops_mr;
	*domain_fid = &domain->util_domain.domain_fid;

	return FI_SUCCESS;
err:
	free(domain);
	return ret;
}
