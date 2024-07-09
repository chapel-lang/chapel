/*
 * Copyright (c) Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "sm2.h"

static struct fi_ops_domain sm2_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = sm2_av_open,
	.cq_open = sm2_cq_open,
	.endpoint = sm2_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = sm2_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = sm2_srx_context,
	.query_atomic = sm2_query_atomic,
	.query_collective = fi_no_query_collective,
};

static int sm2_domain_close(fid_t fid)
{
	int ret;
	struct sm2_domain *domain;

	domain = container_of(fid, struct sm2_domain,
			      util_domain.domain_fid.fid);

	if (domain->ipc_cache)
		ofi_ipc_cache_destroy(domain->ipc_cache);

	ret = ofi_domain_close(&domain->util_domain);
	if (ret)
		return ret;

	free(domain);
	return 0;
}

static struct fi_ops sm2_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sm2_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_mr sm2_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = ofi_mr_reg,
	.regv = ofi_mr_regv,
	.regattr = ofi_mr_regattr,
};

int sm2_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **domain, void *context)
{
	int ret;
	struct sm2_domain *sm2_domain;

	ret = ofi_prov_check_info(&sm2_util_prov, fabric->api_version, info);
	if (ret)
		return ret;

	sm2_domain = calloc(1, sizeof(*sm2_domain));
	if (!sm2_domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(fabric, info, &sm2_domain->util_domain, context,
			      OFI_LOCK_SPINLOCK);
	if (ret) {
		free(sm2_domain);
		return ret;
	}

	ret = ofi_ipc_cache_open(&sm2_domain->ipc_cache,
				 &sm2_domain->util_domain);
	if (ret) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"Unable to open IPC cache\n");
		free(sm2_domain);
		return ret;
	}

	*domain = &sm2_domain->util_domain.domain_fid;
	(*domain)->fid.ops = &sm2_domain_fi_ops;
	(*domain)->ops = &sm2_domain_ops;
	(*domain)->mr = &sm2_mr_ops;

	return 0;
}
