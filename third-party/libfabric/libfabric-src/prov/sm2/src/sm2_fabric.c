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

static int sm2_wait_open(struct fid_fabric *fabric_fid,
			 struct fi_wait_attr *attr, struct fid_wait **waitset)
{
	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_YIELD:
		return ofi_wait_yield_open(fabric_fid, attr, waitset);
	case FI_WAIT_FD:
		return ofi_wait_fd_open(fabric_fid, attr, waitset);
	default:
		return -FI_ENOSYS;
	}
}

static struct fi_ops_fabric sm2_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = sm2_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = sm2_wait_open,
	.trywait = ofi_trywait,
};

static int sm2_fabric_close(fid_t fid)
{
	int ret;
	struct util_fabric *fabric;
	fabric = container_of(fid, struct util_fabric, fabric_fid.fid);
	ret = ofi_fabric_close(fabric);
	if (ret)
		return ret;
	free(fabric);
	return 0;
}

static struct fi_ops sm2_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sm2_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int sm2_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
	       void *context)
{
	int ret;
	struct util_fabric *util_fabric;

	util_fabric = calloc(1, sizeof(*util_fabric));
	if (!util_fabric)
		return -FI_ENOMEM;

	ret = ofi_fabric_init(&sm2_prov, sm2_info.fabric_attr, attr,
			      util_fabric, context);
	if (ret) {
		free(util_fabric);
		return ret;
	}

	*fabric = &util_fabric->fabric_fid;
	(*fabric)->fid.ops = &sm2_fabric_fi_ops;
	(*fabric)->ops = &sm2_fabric_ops;
	return 0;
}
