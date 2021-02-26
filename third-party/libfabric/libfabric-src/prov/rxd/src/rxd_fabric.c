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

#include "rxd.h"

static struct fi_ops_fabric rxd_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = &rxd_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = ofi_wait_fd_open,
	.trywait = ofi_trywait
};

static int rxd_fabric_close(fid_t fid)
{
	int ret;
	struct rxd_fabric *rxd_fabric;

	rxd_fabric = container_of(fid, struct rxd_fabric, util_fabric.fabric_fid.fid);
	ret = fi_close(&rxd_fabric->dg_fabric->fid);
	if (ret)
		return ret;

	ret = ofi_fabric_close(&rxd_fabric->util_fabric);
	if (ret)
		return ret;

	free(rxd_fabric);
	return 0;
}

static struct fi_ops rxd_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxd_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int rxd_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		void *context)
{
	struct rxd_fabric *rxd_fabric;
	struct fi_info *dg_info;
	int ret;

	rxd_fabric = calloc(1, sizeof(*rxd_fabric));
	if (!rxd_fabric)
		return -FI_ENOMEM;

	ret = ofi_fabric_init(&rxd_prov, &rxd_fabric_attr, attr,
			      &rxd_fabric->util_fabric, context);
	if (ret)
		goto err1;

	ret = ofi_get_core_info_fabric(&rxd_prov, attr, &dg_info);
	if (ret) {
		FI_WARN(&rxd_prov, FI_LOG_FABRIC, "Unable to get core info!\n");
		ret = -FI_EINVAL;
		goto err2; 
	}

	ret = fi_fabric(dg_info->fabric_attr, &rxd_fabric->dg_fabric, context);
	if (ret)
		goto err3;

	*fabric = &rxd_fabric->util_fabric.fabric_fid;
	(*fabric)->fid.ops = &rxd_fabric_fi_ops;
	(*fabric)->ops = &rxd_fabric_ops;

	fi_freeinfo(dg_info);
	return 0;
err3:
	fi_freeinfo(dg_info);
err2:
	(void) ofi_fabric_close(&rxd_fabric->util_fabric);
err1:
	free(rxd_fabric);
	return ret;
}
