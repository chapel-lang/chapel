/*
 * Copyright (c) 2018-2019 Intel Corporation. All rights reserved.
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
#include <sys/uio.h>
#include "ofi_hook.h"
#include "ofi_util.h"

static int hook_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			   uint64_t flags, struct fid_mr **mr)
{
	struct hook_domain *dom = container_of(fid, struct hook_domain, domain.fid);
	struct hook_mr *mymr;
	int ret;

	mymr = calloc(1, sizeof *mymr);
	if (!mymr)
		return -FI_ENOMEM;

	mymr->domain = dom;
	mymr->mr.fid.fclass = FI_CLASS_MR;
	mymr->mr.fid.context = attr->context;
	mymr->mr.fid.ops = &hook_fid_ops;

	ret = fi_mr_regattr(dom->hdomain, attr, flags, &mymr->hmr);
	if (ret) {
		free(mymr);
	} else {
		mymr->mr.mem_desc = mymr->hmr->mem_desc;
		mymr->mr.key = mymr->hmr->key;
		*mr = &mymr->mr;
	}

	return ret;
}

static int hook_mr_regv(struct fid *fid, const struct iovec *iov,
			size_t count, uint64_t access,
			uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.auth_key_size = 0;
	attr.auth_key = NULL;
	attr.iface = FI_HMEM_SYSTEM;

	return hook_mr_regattr(fid, &attr, flags, mr);
}

static int hook_mr_reg(struct fid *fid, const void *buf, size_t len,
		       uint64_t access, uint64_t offset, uint64_t requested_key,
		       uint64_t flags, struct fid_mr **mr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	return hook_mr_regv(fid, &iov, 1, access, offset, requested_key,
			    flags, mr, context);
}

static struct fi_ops_mr hook_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = hook_mr_reg,
	.regv = hook_mr_regv,
	.regattr = hook_mr_regattr,
};

static ssize_t hook_credit_handler(struct fid_ep *ep_fid, uint64_t credits)
{
	/*
	 * called from the base provider, ep_fid is the base ep, and
	 * it's fid context is the hook ep.
	 */
	struct hook_ep *ep = (struct hook_ep *)ep_fid->fid.context;

	return (*ep->domain->base_credit_handler)(&ep->ep, credits);
}

static void hook_set_send_handler(struct fid_domain *domain_fid,
		ssize_t (*credit_handler)(struct fid_ep *ep, uint64_t credits))
{
	struct hook_domain *domain = container_of(domain_fid,
						  struct hook_domain, domain);

	domain->base_credit_handler = credit_handler;
	domain->base_ops_flow_ctrl->set_send_handler(domain->hdomain,
						     hook_credit_handler);
}

static int hook_enable_ep_flow_ctrl(struct fid_ep *ep_fid, uint64_t threshold)
{
	struct hook_ep *ep = container_of(ep_fid, struct hook_ep, ep);

	return ep->domain->base_ops_flow_ctrl->enable(ep->hep, threshold);
}

static void hook_add_credits(struct fid_ep *ep_fid, uint64_t credits)
{
	struct hook_ep *ep = container_of(ep_fid, struct hook_ep, ep);

	ep->domain->base_ops_flow_ctrl->add_credits(ep->hep, credits);
}

static bool hook_flow_ctrl_available(struct fid_ep *ep_fid)
{
	struct hook_ep *ep = container_of(ep_fid, struct hook_ep, ep);

	return ep->domain->base_ops_flow_ctrl->available(ep->hep);
}

static struct ofi_ops_flow_ctrl hook_ops_flow_ctrl = {
	.size = sizeof(struct ofi_ops_flow_ctrl),
	.add_credits = hook_add_credits,
	.enable = hook_enable_ep_flow_ctrl,
	.set_send_handler = hook_set_send_handler,
	.available = hook_flow_ctrl_available,
};

static int hook_domain_ops_open(struct fid *fid, const char *name,
				uint64_t flags, void **ops, void *context)
{
	int err;
	struct hook_domain *domain = container_of(fid, struct hook_domain,
						  domain);

	err = fi_open_ops(hook_to_hfid(fid), name, flags, ops, context);
	if (err)
		return err;

	if (!strcasecmp(name, OFI_OPS_FLOW_CTRL)) {
		domain->base_ops_flow_ctrl = *ops;
		*ops = &hook_ops_flow_ctrl;
	}

	return 0;
}

struct fi_ops hook_domain_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_domain_ops_open,
};


int hook_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		  enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);

	return fi_query_atomic(dom->hdomain, datatype, op, attr, flags);
}

int hook_query_collective(struct fid_domain *domain, enum fi_collective_op coll,
			  struct fi_collective_attr *attr, uint64_t flags)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);

	return fi_query_collective(dom->hdomain, coll, attr, flags);
}

struct fi_ops_domain hook_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = hook_av_open,
	.cq_open = hook_cq_open,
	.endpoint = hook_endpoint,
	.scalable_ep = hook_scalable_ep,
	.cntr_open = hook_cntr_open,
	.poll_open = hook_poll_open,
	.stx_ctx = hook_stx_ctx,
	.srx_ctx = hook_srx_ctx,
	.query_atomic = hook_query_atomic,
	.query_collective = hook_query_collective,
};


int hook_domain_init(struct fid_fabric *fabric, struct fi_info *info,
		     struct fid_domain **domain, void *context,
		     struct hook_domain *dom)
{
	struct hook_fabric *fab = container_of(fabric, struct hook_fabric, fabric);
	int ret;

	dom->fabric = fab;
	dom->domain.fid.fclass = FI_CLASS_DOMAIN;
	dom->domain.fid.context = context;
	dom->domain.fid.ops = &hook_domain_fid_ops;
	dom->domain.ops = &hook_domain_ops;
	dom->domain.mr = &hook_mr_ops;

	ret = fi_domain(fab->hfabric, info, &dom->hdomain, &dom->domain.fid);
	if (ret)
		return ret;

	*domain = &dom->domain;

	return 0;
}

int hook_domain(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **domain, void *context)
{
	struct hook_domain *dom;
	int ret;

	dom = calloc(1, sizeof *dom);
	if (!dom)
		return -FI_ENOMEM;

	ret = hook_domain_init(fabric, info, domain, context, dom);
	if (ret)
		goto err1;

	ret = hook_ini_fid(dom->fabric->prov_ctx, &dom->domain.fid);
	if (ret)
		goto err2;

	return 0;
err2:
	fi_close(&dom->domain.fid);
err1:
	free(dom);
	return ret;
}
