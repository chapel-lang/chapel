/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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
#include "hook_prov.h"


static uint64_t hook_cntr_read(struct fid_cntr *cntr)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_read(mycntr->hcntr);
}

static uint64_t hook_cntr_readerr(struct fid_cntr *cntr)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_readerr(mycntr->hcntr);
}

static int hook_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_add(mycntr->hcntr, value);
}

static int hook_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_set(mycntr->hcntr, value);
}

static int hook_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_wait(mycntr->hcntr, threshold, timeout);
}

static int hook_cntr_adderr(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_adderr(mycntr->hcntr, value);
}

static int hook_cntr_seterr(struct fid_cntr *cntr, uint64_t value)
{
	struct hook_cntr *mycntr = container_of(cntr, struct hook_cntr, cntr);

	return fi_cntr_seterr(mycntr->hcntr, value);
}

struct fi_ops_cntr hook_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = hook_cntr_read,
	.readerr = hook_cntr_readerr,
	.add = hook_cntr_add,
	.set = hook_cntr_set,
	.wait = hook_cntr_wait,
	.adderr = hook_cntr_adderr,
	.seterr = hook_cntr_seterr,
};

int hook_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_cntr *mycntr;
	struct fi_cntr_attr hattr;
	int ret;

	mycntr = calloc(1, sizeof *mycntr);
	if (!mycntr)
		return -FI_ENOMEM;

	mycntr->domain = dom;
	mycntr->cntr.fid.fclass = FI_CLASS_CNTR;
	mycntr->cntr.fid.context = context;
	mycntr->cntr.fid.ops = &hook_fid_ops;
	mycntr->cntr.ops = &hook_cntr_ops;

	hattr = *attr;
	if (attr->wait_obj == FI_WAIT_SET)
		hattr.wait_set = hook_to_hwait(attr->wait_set);

	ret = fi_cntr_open(dom->hdomain, &hattr, &mycntr->hcntr,
			   &mycntr->cntr.fid);
	if (ret)
		goto err1;

	*cntr = &mycntr->cntr;

	ret = hook_ini_fid(dom->fabric->prov_ctx, &mycntr->cntr.fid);
	if (ret)
		goto err2;

	return ret;
err2:
	fi_close(&mycntr->hcntr->fid);
err1:
	free(mycntr);
	return ret;
}
