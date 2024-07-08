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

#include "coll.h"

static int coll_eq_close(struct fid *fid)
{
	struct ofi_coll_eq *eq;
	int ret;

	eq = container_of(fid, struct ofi_coll_eq, util_eq.eq_fid.fid);

	ret = ofi_eq_cleanup(fid);
	if (ret)
		return ret;

	free(eq);
	return 0;
}

static struct fi_ops coll_eq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = coll_eq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_eq coll_eq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = fi_no_eq_read,
	.readerr = fi_no_eq_readerr,
	.write = fi_no_eq_write,
	.sread = fi_no_eq_sread,
	.strerror = fi_no_eq_strerror,
};

int ofi_coll_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq_fid, void *context)
{
	struct ofi_coll_eq *eq;
	struct fi_peer_eq_context *peer_context = context;
	int ret;

	if (!attr || !(attr->flags & FI_PEER)) {
		FI_WARN(&coll_prov, FI_LOG_CORE, "FI_PEER flag required\n");
                return -EINVAL;
	}

	if (!peer_context || peer_context->size < sizeof(*peer_context)) {
		FI_WARN(&coll_prov, FI_LOG_CORE, "invalid peer EQ context\n");
                return -EINVAL;
	}

	eq = calloc(1, sizeof(*eq));
	if (!eq)
		return -FI_ENOMEM;

	eq->peer_eq = peer_context->eq;

	ret = ofi_eq_init(fabric, attr, &eq->util_eq.eq_fid, context);
	if (ret)
		goto err;

	*eq_fid = &eq->util_eq.eq_fid;
	(*eq_fid)->fid.ops = &coll_eq_fi_ops;
	(*eq_fid)->ops = &coll_eq_ops;
	return 0;

err:
	free(eq);
	return ret;
}
