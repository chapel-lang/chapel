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
#include <pthread.h>
#include <stdio.h>
#include <ofi.h>
#include <ofi_str.h>

#include "ofi_hook.h"
#include "ofi_prov.h"

struct hook_fabric *hook_to_fabric(const struct fid *fid)
{
	switch (fid->fclass) {
	case FI_CLASS_FABRIC:
		return (container_of(fid, struct hook_fabric, fabric.fid));
	case FI_CLASS_DOMAIN:
		return (container_of(fid, struct hook_domain, domain.fid)->
			fabric);
	case FI_CLASS_AV:
		return (container_of(fid, struct hook_av, av.fid)->
			domain->fabric);
	case FI_CLASS_WAIT:
		return (container_of(fid, struct hook_wait, wait.fid)->
			fabric);
	case FI_CLASS_POLL:
		return (container_of(fid, struct hook_poll, poll.fid)->
			domain->fabric);
	case FI_CLASS_EQ:
		return (container_of(fid, struct hook_eq, eq.fid)->
			fabric);
	case FI_CLASS_CQ:
		return (container_of(fid, struct hook_cq, cq.fid)->
			domain->fabric);
	case FI_CLASS_CNTR:
		return (container_of(fid, struct hook_cntr, cntr.fid)->
			domain->fabric);
	case FI_CLASS_SEP:
	case FI_CLASS_EP:
	case FI_CLASS_RX_CTX:
	case FI_CLASS_SRX_CTX:
	case FI_CLASS_TX_CTX:
		return (container_of(fid, struct hook_ep, ep.fid)->
			domain->fabric);
	case FI_CLASS_PEP:
		return (container_of(fid, struct hook_pep, pep.fid)->
			fabric);
	case FI_CLASS_STX_CTX:
		return (container_of(fid, struct hook_stx, stx.fid)->
			domain->fabric);
	case FI_CLASS_MR:
		return (container_of(fid, struct hook_mr, mr.fid)->
			domain->fabric);
	default:
		assert(0);
		return NULL;
	}
}

struct fid *hook_to_hfid(const struct fid *fid)
{
	switch (fid->fclass) {
	case FI_CLASS_FABRIC:
		return &(container_of(fid, struct hook_fabric, fabric.fid)->
			 hfabric->fid);
	case FI_CLASS_DOMAIN:
		return &(container_of(fid, struct hook_domain, domain.fid)->
			 hdomain->fid);
	case FI_CLASS_AV:
		return &(container_of(fid, struct hook_av, av.fid)->
			 hav->fid);
	case FI_CLASS_WAIT:
		return &(container_of(fid, struct hook_wait, wait.fid)->
			 hwait->fid);
	case FI_CLASS_POLL:
		return &(container_of(fid, struct hook_poll, poll.fid)->
			 hpoll->fid);
	case FI_CLASS_EQ:
		return &(container_of(fid, struct hook_eq, eq.fid)->
			 heq->fid);
	case FI_CLASS_CQ:
		return &(container_of(fid, struct hook_cq, cq.fid)->
			 hcq->fid);
	case FI_CLASS_CNTR:
		return &(container_of(fid, struct hook_cntr, cntr.fid)->
			 hcntr->fid);
	case FI_CLASS_SEP:
	case FI_CLASS_EP:
	case FI_CLASS_RX_CTX:
	case FI_CLASS_SRX_CTX:
	case FI_CLASS_TX_CTX:
		return &(container_of(fid, struct hook_ep, ep.fid)->
			 hep->fid);
	case FI_CLASS_PEP:
		return &(container_of(fid, struct hook_pep, pep.fid)->
			 hpep->fid);
	case FI_CLASS_STX_CTX:
		return &(container_of(fid, struct hook_stx, stx.fid)->
			 hstx->fid);
	case FI_CLASS_MR:
		return &(container_of(fid, struct hook_mr, mr.fid)->
			 hmr->fid);
	default:
		return NULL;
	}
}

struct fid_wait *hook_to_hwait(const struct fid_wait *wait)
{
	return container_of(wait, struct hook_wait, wait)->hwait;
}


int hook_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct fid *hfid, *hbfid;

	hfid = hook_to_hfid(fid);
	hbfid = hook_to_hfid(bfid);
	if (!hfid || !hbfid)
		return -FI_EINVAL;

	return hfid->ops->bind(hfid, hbfid, flags);
}

int hook_control(struct fid *fid, int command, void *arg)
{
	struct fid *hfid;

	hfid = hook_to_hfid(fid);
	if (!hfid)
		return -FI_EINVAL;

	return hfid->ops->control(hfid, command, arg);
}

int hook_ops_open(struct fid *fid, const char *name,
			 uint64_t flags, void **ops, void *context)
{
	struct fid *hfid;

	hfid = hook_to_hfid(fid);
	if (!hfid)
		return -FI_EINVAL;

	return hfid->ops->ops_open(hfid, name, flags, ops, context);
}

int hook_close(struct fid *fid)
{
	struct fid *hfid;
	struct hook_prov_ctx *prov_ctx;
	int ret;

	hfid = hook_to_hfid(fid);
	if (!hfid)
		return -FI_EINVAL;

	prov_ctx = hook_to_prov_ctx(fid);
	if (!prov_ctx)
		return -FI_EINVAL;

	hook_fini_fid(prov_ctx, fid);

	ret = hfid->ops->close(hfid);
	if (!ret)
		free(fid);
	return ret;
}


struct fi_ops hook_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

struct fi_ops hook_fabric_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = hook_close,
	.bind = hook_bind,
	.control = hook_control,
	.ops_open = hook_ops_open,
};

struct fi_ops_fabric hook_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = hook_domain,
	.passive_ep = hook_passive_ep,
	.eq_open = hook_eq_open,
	.wait_open = hook_wait_open,
	.trywait = hook_trywait,
};

void hook_fabric_init(struct hook_fabric *fabric, enum ofi_hook_class hclass,
		      struct fid_fabric *hfabric, struct fi_provider *hprov,
		      struct fi_ops *f_ops, struct hook_prov_ctx *prov_ctx)
{
	fabric->hclass = hclass;
	fabric->hfabric = hfabric;
	fabric->hprov = hprov;
	fabric->prov_ctx = prov_ctx;
	fabric->fabric.fid.fclass = FI_CLASS_FABRIC;
	fabric->fabric.fid.context = hfabric->fid.context;
	fabric->fabric.fid.ops = f_ops;
	fabric->fabric.api_version = hfabric->api_version;
	fabric->fabric.ops = &hook_fabric_ops;

	hfabric->fid.context = fabric;
}

struct hook_prov_ctx hook_noop_ctx;

static int hook_noop_fabric(struct fi_fabric_attr *attr,
			    struct fid_fabric **fabric, void *context)
{
	struct fi_provider *hprov = context;
	struct hook_fabric *fab;

	FI_TRACE(hprov, FI_LOG_FABRIC, "Installing noop hook\n");
	fab = calloc(1, sizeof *fab);
	if (!fab)
		return -FI_ENOMEM;

	hook_fabric_init(fab, HOOK_NOOP, attr->fabric, hprov,
			 &hook_fabric_fid_ops, &hook_noop_ctx);
	*fabric = &fab->fabric;
	return 0;
}

struct hook_prov_ctx hook_noop_ctx = {
	.prov = {
		.version = OFI_VERSION_DEF_PROV,
		/* We're a pass-through provider, so the fi_version is always the latest */
		.fi_version = OFI_VERSION_LATEST,
		.name = "ofi_hook_noop",
		.getinfo = NULL,
		.fabric = hook_noop_fabric,
		.cleanup = NULL,
	},
};

HOOK_NOOP_INI
{
	return &hook_noop_ctx.prov;
}
