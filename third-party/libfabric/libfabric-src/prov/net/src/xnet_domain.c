/*
 * Copyright (c) 2017-2022 Intel Corporation. All rights reserved.
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

#include "ofi_atomic.h"
#include "xnet.h"


static int
xnet_mr_reg(struct fid *fid, const void *buf, size_t len,
	    uint64_t access, uint64_t offset, uint64_t requested_key,
	    uint64_t flags, struct fid_mr **mr, void *context)
{
	struct xnet_domain *domain;
	int ret;

	domain = container_of(fid, struct xnet_domain,
			      util_domain.domain_fid.fid);
	ofi_genlock_lock(&domain->progress.lock);
	ret = ofi_mr_reg(fid, buf, len, access, offset, requested_key, flags,
			 mr, context);
	ofi_genlock_unlock(&domain->progress.lock);
	return ret;
}

static int
xnet_mr_regv(struct fid *fid, const struct iovec *iov,
	     size_t count, uint64_t access,
	     uint64_t offset, uint64_t requested_key,
	     uint64_t flags, struct fid_mr **mr, void *context)
{
	struct xnet_domain *domain;
	int ret;

	domain = container_of(fid, struct xnet_domain,
			      util_domain.domain_fid.fid);
	ofi_genlock_lock(&domain->progress.lock);
	ret = ofi_mr_regv(fid, iov, count, access, offset, requested_key, flags,
			 mr, context);
	ofi_genlock_unlock(&domain->progress.lock);
	return ret;
}

static int
xnet_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		uint64_t flags, struct fid_mr **mr)
{
	struct xnet_domain *domain;
	int ret;

	domain = container_of(fid, struct xnet_domain,
			      util_domain.domain_fid.fid);
	ofi_genlock_lock(&domain->progress.lock);
	ret = ofi_mr_regattr(fid, attr, flags, mr);
	ofi_genlock_unlock(&domain->progress.lock);
	return ret;
}

static int xnet_open_ep(struct fid_domain *domain, struct fi_info *info,
			struct fid_ep **ep_fid, void *context)
{
	if (info->ep_attr->type == FI_EP_MSG)
		return xnet_endpoint(domain, info, ep_fid, context);

	if (info->ep_attr->type == FI_EP_RDM)
		return xnet_rdm_ep(domain, info, ep_fid, context);

	return -FI_EINVAL;
}

static int xnet_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
			struct fid_av **fid_av, void *context)
{
	return rxm_util_av_open(domain_fid, attr, fid_av, context,
				sizeof(struct xnet_conn), NULL);
}

static int
xnet_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		  enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	int ret;

	ret = ofi_atomic_valid(&xnet_prov, datatype, op, flags);
	if (ret || !attr)
		return ret;

	return -FI_EOPNOTSUPP;
}

static struct fi_ops_domain xnet_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = xnet_av_open,
	.cq_open = xnet_cq_open,
	.endpoint = xnet_open_ep,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = xnet_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = xnet_srx_context,
	.query_atomic = xnet_query_atomic,
	.query_collective = fi_no_query_collective,
};

static void xnet_del_wait_eq_list(struct xnet_domain *domain)
{
	struct xnet_fabric *fabric;
	struct xnet_eq *eq;
	struct dlist_entry *item;
	int ret;

	fabric = container_of(domain->util_domain.fabric,
			      struct xnet_fabric,
			      util_fabric.fabric_fid);

	ofi_mutex_lock(&fabric->util_fabric.lock);
	dlist_foreach(&fabric->wait_eq_list, item) {
		eq = container_of(item, struct xnet_eq, wait_eq_entry);
		ret = xnet_eq_del_progress(eq, &domain->progress);
		if (ret) {
			FI_WARN(&xnet_prov, FI_LOG_DOMAIN,
				"Failed to del progress from eq");
		}
	}
	ofi_mutex_unlock(&fabric->util_fabric.lock);
}

static int xnet_domain_close(fid_t fid)
{
	struct xnet_fabric *fabric;
	struct xnet_domain *domain;
	int ret;

	domain = container_of(fid, struct xnet_domain,
			      util_domain.domain_fid.fid);
	fabric = container_of(domain->util_domain.fabric,
			      struct xnet_fabric,
			      util_fabric.fabric_fid);

	if (!fabric->progress.auto_progress)
		xnet_del_wait_eq_list(domain);

	ret = ofi_domain_close(&domain->util_domain);
	if (ret)
		return ret;

	xnet_close_progress(&domain->progress);
	free(domain);
	return FI_SUCCESS;
}

static struct fi_ops xnet_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = xnet_domain_close,
	.bind = ofi_domain_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
	.tostr = fi_no_tostr,
	.ops_set = fi_no_ops_set,
};

static struct fi_ops_mr xnet_domain_fi_ops_mr = {
	.size = sizeof(struct fi_ops_mr),
	.reg = xnet_mr_reg,
	.regv = xnet_mr_regv,
	.regattr = xnet_mr_regattr,
};

static int xnet_add_wait_eq_list(struct xnet_domain *domain)
{
	struct xnet_fabric *fabric;
	struct xnet_eq *eq;
	struct dlist_entry *error_item;
	struct dlist_entry *item;
	int ret;

	fabric = container_of(domain->util_domain.fabric,
			      struct xnet_fabric,
			      util_fabric.fabric_fid);

	ofi_mutex_lock(&fabric->util_fabric.lock);
	dlist_foreach(&fabric->wait_eq_list, item) {
		eq = container_of(item, struct xnet_eq, wait_eq_entry);
		ret = xnet_eq_add_progress(eq, &domain->progress,
					   &domain->util_domain.domain_fid);
		if (ret) {
			error_item = item;
			goto clean;
		}
	}
	ofi_mutex_unlock(&fabric->util_fabric.lock);
	return FI_SUCCESS;

clean:
	/* Traverse the list backwards from where the error occurred */
	dlist_foreach_reverse(error_item, item) {
		eq = container_of(item, struct xnet_eq, wait_eq_entry);
		xnet_eq_del_progress(eq, &domain->progress);
	}
	ofi_mutex_unlock(&fabric->util_fabric.lock);
	return ret;
}

int xnet_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		     struct fid_domain **domain_fid, void *context)
{
	struct xnet_fabric *fabric;
	struct xnet_domain *domain;
	int ret;

	fabric = container_of(fabric_fid, struct xnet_fabric,
			      util_fabric.fabric_fid);
	ret = ofi_prov_check_info(&xnet_util_prov, fabric_fid->api_version, info);
	if (ret)
		return ret;

	domain = calloc(1, sizeof(*domain));
	if (!domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(fabric_fid, info, &domain->util_domain, context,
			      OFI_LOCK_NONE);
	if (ret)
		goto free;

	ret = xnet_init_progress(&domain->progress, info);
	if (ret)
		goto close;

	if (fabric->progress.auto_progress) {
		ret = xnet_start_progress(&domain->progress);
	} else {
		ret = xnet_add_wait_eq_list(domain);
	}
	if (ret)
		goto close_prog;

	domain->util_domain.domain_fid.fid.ops = &xnet_domain_fi_ops;
	domain->util_domain.domain_fid.ops = &xnet_domain_ops;
	domain->util_domain.domain_fid.mr = &xnet_domain_fi_ops_mr;
	*domain_fid = &domain->util_domain.domain_fid;

	return FI_SUCCESS;

close_prog:
	xnet_close_progress(&domain->progress);
close:
	(void) ofi_domain_close(&domain->util_domain);
free:
	free(domain);
	return ret;
}
