/*
 * Copyright (c) 2017-2018 Intel Corporation. All rights reserved.
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

#include "rstream.h"


static int rstream_domain_close(fid_t fid)
{
	struct rstream_domain *rstream_domain =
		container_of(fid, struct rstream_domain,
		util_domain.domain_fid.fid);
	int ret;

	ret = fi_close(&rstream_domain->msg_domain->fid);
	if (ret)
		return ret;

	ret = ofi_domain_close(&rstream_domain->util_domain);
	if (ret)
		return ret;

	free(rstream_domain);

	return 0;
}

static struct fi_ops_mr rstream_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = fi_no_mr_reg,
	.regv = fi_no_mr_regv,
	.regattr = fi_no_mr_regattr,
};

static struct fi_ops rstream_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rstream_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_domain rstream_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = fi_no_av_open,
	.cq_open = fi_no_cq_open,
	.endpoint = rstream_ep_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = fi_no_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

int rstream_domain_open(struct fid_fabric *fabric, struct fi_info *info,
			   struct fid_domain **domain, void *context)
{
	struct rstream_domain *rstream_domain;
	struct rstream_fabric *rstream_fabric;
	int ret;
	struct fi_info *cinfo = NULL;

	rstream_domain = calloc(1, sizeof(*rstream_domain));
	if (!rstream_domain)
		return -FI_ENOMEM;

	rstream_fabric = container_of(fabric, struct rstream_fabric,
		util_fabric.fabric_fid);

	ret = ofi_get_core_info(FI_VERSION(1, 8), NULL, NULL, 0,
		&rstream_util_prov, info, rstream_info_to_core, &cinfo);
	if (ret)
		goto err1;

	ret = fi_domain(rstream_fabric->msg_fabric, cinfo,
		&rstream_domain->msg_domain, context);
	if (ret)
		goto err1;

	ret = ofi_domain_init(fabric, info, &rstream_domain->util_domain,
		context);
	if (ret)
		goto err1;

	*domain = &rstream_domain->util_domain.domain_fid;
	(*domain)->fid.ops = &rstream_domain_fi_ops;
	(*domain)->mr = &rstream_domain_mr_ops;
	(*domain)->ops = &rstream_domain_ops;

	return 0;
err1:
	if (cinfo)
		fi_freeinfo(cinfo);
	free(rstream_domain);
	return ret;
}
