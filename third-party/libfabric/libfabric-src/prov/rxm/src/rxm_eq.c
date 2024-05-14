/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved.
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
#include <inttypes.h>

#include <ofi.h>

#include "rxm.h"

static int rxm_eq_close(struct fid *fid)
{
	struct rxm_eq *rxm_eq;
	int ret, retv = 0;

	rxm_eq = container_of(fid, struct rxm_eq, util_eq.eq_fid.fid);

	if (rxm_eq->offload_coll_eq) {
		ret = fi_close(&rxm_eq->offload_coll_eq->fid);
		if (ret)
			return ret;
		rxm_eq->offload_coll_eq = NULL;
	}

	if (rxm_eq->util_coll_eq) {
		ret = fi_close(&rxm_eq->util_coll_eq->fid);
		if (ret)
			return ret;
		rxm_eq->util_coll_eq = NULL;
	}

	ret = ofi_eq_cleanup(&rxm_eq->util_eq.eq_fid.fid);
	if (ret)
		retv = ret;

	free(rxm_eq);
	return retv;
}

static struct fi_ops rxm_eq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxm_eq_close,
	.bind = fi_no_bind,
	.control = ofi_eq_control,
	.ops_open = fi_no_ops_open,
};

int rxm_eq_open(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		struct fid_eq **eq_fid, void *context)
{
	struct rxm_fabric *rxm_fabric;
	struct rxm_eq *rxm_eq;
	struct fi_peer_eq_context peer_context = {
		.size = sizeof(struct fi_peer_eq_context),
	};
	struct fi_eq_attr peer_attr = {
		.flags = FI_PEER,
	};
	int ret;

	rxm_fabric = container_of(fabric_fid, struct rxm_fabric,
				  util_fabric.fabric_fid);

	rxm_eq = calloc(1, sizeof(*rxm_eq));
	if (!rxm_eq)
		return -FI_ENOMEM;

	ret = ofi_eq_init(fabric_fid, attr, &rxm_eq->util_eq.eq_fid, context);
	if (ret)
		goto err1;

	peer_context.eq = &rxm_eq->util_eq.eq_fid;

	if (rxm_fabric->util_coll_fabric) {
		ret = fi_eq_open(rxm_fabric->util_coll_fabric, &peer_attr,
				 &rxm_eq->util_coll_eq, &peer_context);
		if (ret)
			goto err2;
	}

	if (rxm_fabric->offload_coll_fabric) {
		ret = fi_eq_open(rxm_fabric->offload_coll_fabric, &peer_attr,
				 &rxm_eq->offload_coll_eq, &peer_context);
		if (ret)
			goto err3;
	}

	rxm_eq->util_eq.eq_fid.fid.ops = &rxm_eq_fi_ops;
	*eq_fid = &rxm_eq->util_eq.eq_fid;
	return 0;

err3:
	fi_close(&rxm_eq->util_coll_eq->fid);
err2:
	ofi_eq_cleanup(&rxm_eq->util_eq.eq_fid.fid);
err1:
	free(rxm_eq);
	return ret;
}
