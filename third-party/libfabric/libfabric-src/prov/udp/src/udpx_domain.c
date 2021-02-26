/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
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

#include <stdlib.h>
#include <string.h>

#include "udpx.h"


static struct fi_ops_domain udpx_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = ofi_ip_av_create,
	.cq_open = udpx_cq_open,
	.endpoint = udpx_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = fi_no_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

static int udpx_domain_close(fid_t fid)
{
	int ret;
	struct util_domain *domain;
	domain = container_of(fid, struct util_domain, domain_fid.fid);
	ret = ofi_domain_close(domain);
	if (ret)
		return ret;
	free(domain);
	return 0;
}

static struct fi_ops udpx_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = udpx_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_mr udpx_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = ofi_mr_reg,
	.regv = ofi_mr_regv,
	.regattr = ofi_mr_regattr,
};

int udpx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **domain, void *context)
{
	struct util_domain *util_domain;
	int ret;

	ret = ofi_prov_check_info(&udpx_util_prov, fabric->api_version, info);
	if (ret)
		return ret;

	util_domain = calloc(1, sizeof(*util_domain));
	if (!util_domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(fabric, info, util_domain, context);
	if (ret) {
		free(util_domain);
		return ret;
	}

	*domain = &util_domain->domain_fid;
	(*domain)->fid.ops = &udpx_domain_fi_ops;
	(*domain)->ops = &udpx_domain_ops;
	(*domain)->mr = &udpx_mr_ops;
	return 0;
}
