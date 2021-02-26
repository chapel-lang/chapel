/*
 * Copyright (c) 2018 Intel Corporation, Inc.  All rights reserved.
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

#include "mrail.h"

static int mrail_fabric_close(fid_t fid)
{
	struct mrail_fabric *mrail_fabric =
		container_of(fid, struct mrail_fabric, util_fabric.fabric_fid.fid);
	int ret, retv = 0;

	ret = mrail_close_fids((struct fid **)mrail_fabric->fabrics,
			       mrail_fabric->num_fabrics);
	if (ret)
		retv = ret;

	free(mrail_fabric->fabrics);

	ret = ofi_fabric_close(&mrail_fabric->util_fabric);
	if (ret)
		retv = ret;

	free(mrail_fabric);
	return retv;
}

static struct fi_ops mrail_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = mrail_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_fabric mrail_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = mrail_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = ofi_wait_fd_open,
	.trywait = ofi_trywait
};

int mrail_fabric_open(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		      void *context)
{
	struct fi_info *fi;
	struct mrail_fabric *mrail_fabric;
	size_t i;
	int ret;

	mrail_fabric = calloc(1, sizeof(*mrail_fabric));
	if (!mrail_fabric)
		return -FI_ENOMEM;

	mrail_fabric->info = mrail_get_info_cached(attr->name);
	if (!mrail_fabric->info) {
		free(mrail_fabric);
		return -FI_EINVAL;
	}

	ret = ofi_fabric_init(&mrail_prov, &mrail_fabric_attr, attr,
			      &mrail_fabric->util_fabric, context);
	if (ret) {
		free(mrail_fabric);
		return ret;
	}

	for (fi = mrail_fabric->info->next; fi; fi = fi->next)
		mrail_fabric->num_fabrics++;

	mrail_fabric->fabrics = calloc(mrail_fabric->num_fabrics,
				       sizeof(*mrail_fabric->fabrics));
	if (!mrail_fabric->fabrics) {
		ret = -FI_ENOMEM;
		goto err;
	}

	for (i = 0, fi = mrail_fabric->info->next; fi; fi = fi->next, i++) {
		ret = fi_fabric(fi->fabric_attr, &mrail_fabric->fabrics[i], context);
		if (ret)
			goto err;
	}

	*fabric 		= &mrail_fabric->util_fabric.fabric_fid;
	(*fabric)->fid.ops 	= &mrail_fabric_fi_ops;
	(*fabric)->ops 		= &mrail_fabric_ops;

	return 0;
err:
	mrail_fabric_close(&mrail_fabric->util_fabric.fabric_fid.fid);
	return ret;
}
