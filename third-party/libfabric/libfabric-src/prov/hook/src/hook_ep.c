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
#include <ofi_enosys.h>
#include "hook_prov.h"
#include "ofi_hook.h"


static int hook_open_tx_ctx(struct fid_ep *sep, int index,
			    struct fi_tx_attr *attr, struct fid_ep **tx_ep,
			    void *context);
static int hook_open_rx_ctx(struct fid_ep *sep, int index,
			    struct fi_rx_attr *attr, struct fid_ep **rx_ep,
			    void *context);


static ssize_t hook_cancel(fid_t fid, void *context)
{
	return fi_cancel(hook_to_hfid(fid), context);
}

static int hook_getopt(fid_t fid, int level, int optname,
		       void *optval, size_t *optlen)
{
	return fi_getopt(hook_to_hfid(fid), level, optname, optval, optlen);
}

static int hook_setopt(fid_t fid, int level, int optname,
		       const void *optval, size_t optlen)
{
	return fi_setopt(hook_to_hfid(fid), level, optname, optval, optlen);
}

static int hook_tx_ctx(struct fid_ep *sep, int index,
		       struct fi_tx_attr *attr, struct fid_ep **tx_ep,
		       void *context)
{
	return hook_open_tx_ctx(sep, index, attr, tx_ep, context);
}

static int hook_rx_ctx(struct fid_ep *sep, int index,
		       struct fi_rx_attr *attr, struct fid_ep **rx_ep,
		       void *context)
{
	return hook_open_rx_ctx(sep, index, attr, rx_ep, context);
}

static struct fi_ops_ep hook_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = hook_cancel,
	.getopt = hook_getopt,
	.setopt = hook_setopt,
	.tx_ctx = hook_tx_ctx,
	.rx_ctx = hook_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};


static void hook_setup_ep(enum ofi_hook_class hclass, struct fid_ep *ep,
			  int fclass, void *context)
{
	ep->fid.fclass = fclass;
	ep->fid.context = context;
	ep->fid.ops = &hook_fid_ops;
	ep->ops = &hook_ep_ops;
	ep->cm = &hook_cm_ops;
	ep->msg = &hook_msg_ops;
	ep->rma = &hook_rma_ops;
	ep->tagged = &hook_tagged_ops;
	ep->atomic = &hook_atomic_ops;
}

int hook_scalable_ep(struct fid_domain *domain, struct fi_info *info,
		     struct fid_ep **sep, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_ep *mysep;
	int ret;

	mysep = calloc(1, sizeof *mysep);
	if (!mysep)
		return -FI_ENOMEM;

	mysep->domain = dom;
	hook_setup_ep(dom->fabric->hclass, &mysep->ep, FI_CLASS_SEP, context);
	ret = fi_scalable_ep(dom->hdomain, info, &mysep->hep, &mysep->ep.fid);
	if (ret)
		free(mysep);
	else
		*sep = &mysep->ep;

	return ret;
}

int hook_stx_ctx(struct fid_domain *domain,
		 struct fi_tx_attr *attr, struct fid_stx **stx,
		 void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_stx *mystx;
	int ret;

	mystx = calloc(1, sizeof *mystx);
	if (!mystx)
		return -FI_ENOMEM;

	mystx->domain = dom;
	mystx->stx.fid.fclass = FI_CLASS_STX_CTX;
	mystx->stx.fid.context = context;
	mystx->stx.fid.ops = &hook_fid_ops;
	mystx->stx.ops = &hook_ep_ops;

	ret = fi_stx_context(dom->hdomain, attr, &mystx->hstx, &mystx->stx.fid);
	if (ret)
		free(mystx);
	else
		*stx = &mystx->stx;

	return ret;
}

int hook_srx_ctx(struct fid_domain *domain, struct fi_rx_attr *attr,
		 struct fid_ep **rx_ep, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_ep *srx;
	int ret;

	srx = calloc(1, sizeof *srx);
	if (!srx)
		return -FI_ENOMEM;

	srx->domain = dom;
	hook_setup_ep(dom->fabric->hclass, &srx->ep, FI_CLASS_SRX_CTX, context);
	ret = fi_srx_context(dom->hdomain, attr, &srx->hep, &srx->ep.fid);
	if (ret)
		free(srx);
	else
		*rx_ep = &srx->ep;

	return ret;
}

static int hook_open_tx_ctx(struct fid_ep *sep, int index,
			    struct fi_tx_attr *attr, struct fid_ep **tx_ep,
			    void *context)
{
	struct hook_ep *mysep = container_of(sep, struct hook_ep, ep);
	struct hook_ep *mytx;
	int ret;

	mytx = calloc(1, sizeof *mytx);
	if (!mytx)
		return -FI_ENOMEM;

	mytx->domain = mysep->domain;
	hook_setup_ep(mysep->domain->fabric->hclass, &mytx->ep,
		      FI_CLASS_TX_CTX, context);
	ret = fi_tx_context(mysep->hep, index, attr, &mytx->hep, &mytx->ep.fid);
	if (ret)
		free(mytx);
	else
		*tx_ep = &mytx->ep;

	return ret;
}

static int hook_open_rx_ctx(struct fid_ep *sep, int index,
			    struct fi_rx_attr *attr, struct fid_ep **rx_ep,
			    void *context)
{
	struct hook_ep *mysep = container_of(sep, struct hook_ep, ep);
	struct hook_ep *myrx;
	int ret;

	myrx = calloc(1, sizeof *myrx);
	if (!myrx)
		return -FI_ENOMEM;

	myrx->domain = mysep->domain;
	hook_setup_ep(mysep->domain->fabric->hclass, &myrx->ep,
		      FI_CLASS_RX_CTX, context);
	ret = fi_rx_context(mysep->hep, index, attr, &myrx->hep, &myrx->ep.fid);
	if (ret)
		free(myrx);
	else
		*rx_ep = &myrx->ep;

	return ret;
}

int hook_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_pep **pep, void *context)
{
	struct hook_fabric *fab = container_of(fabric, struct hook_fabric, fabric);
	struct hook_pep *mypep;
	int ret;

	mypep = calloc(1, sizeof *mypep);
	if (!mypep)
		return -FI_ENOMEM;

	mypep->fabric = fab;
	mypep->pep.fid.fclass = FI_CLASS_PEP;
	mypep->pep.fid.context = context;
	mypep->pep.fid.ops = &hook_fid_ops;
	mypep->pep.ops = &hook_ep_ops;
	mypep->pep.cm = &hook_cm_ops;

	ret = fi_passive_ep(fab->hfabric, info, &mypep->hpep, &mypep->pep.fid);
	if (ret)
		free(mypep);
	else
		*pep = &mypep->pep;

	return ret;
}

int hook_endpoint_init(struct fid_domain *domain, struct fi_info *info,
		       struct fid_ep **ep, void *context, struct hook_ep *myep)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct fid *saved_fid;
	int ret;

	saved_fid = info->handle;
	if (saved_fid) {
		info->handle = hook_to_hfid(info->handle);
		if (!info->handle)
			info->handle = saved_fid;
	}
	myep->domain = dom;

	hook_setup_ep(dom->fabric->hclass, &myep->ep, FI_CLASS_EP, context);

	ret = fi_endpoint(dom->hdomain, info, &myep->hep, &myep->ep.fid);
	info->handle = saved_fid;

	if (ret)
		return ret;

	*ep = &myep->ep;
	return 0;
}

int hook_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_ep *myep;
	int ret;

	myep = calloc(1, sizeof *myep);
	if (!myep)
		return -FI_ENOMEM;

	ret = hook_endpoint_init(domain, info, ep, context, myep);
	if (ret)
		goto err1;

	ret = hook_ini_fid(dom->fabric->prov_ctx, &myep->ep.fid);
	if (ret)
		goto err2;

	return 0;
err2:
	fi_close(&myep->hep->fid);
err1:
	free(myep);
	return ret;
}
