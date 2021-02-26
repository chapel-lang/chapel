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
#include <string.h>

#include "tcpx.h"

static ssize_t tcpx_eq_read(struct fid_eq *eq_fid, uint32_t *event,
			    void *buf, size_t len, uint64_t flags)
{
	struct util_eq *eq;

	eq = container_of(eq_fid, struct util_eq, eq_fid);

	tcpx_conn_mgr_run(eq);

	return ofi_eq_read(eq_fid, event, buf, len, flags);
}

static int tcpx_eq_close(struct fid *fid)
{
	struct tcpx_eq *eq;
	int ret;

	ret = ofi_eq_cleanup(fid);
	if (ret)
		return ret;

	eq = container_of(fid, struct tcpx_eq, util_eq.eq_fid.fid);

	fastlock_destroy(&eq->close_lock);
	free(eq);
	return 0;
}

static struct fi_ops_eq tcpx_eq_ops = {
	.size = sizeof(struct fi_ops_eq),
	.read = tcpx_eq_read,
	.readerr = ofi_eq_readerr,
	.sread = ofi_eq_sread,
	.write = ofi_eq_write,
	.strerror = ofi_eq_strerror,
};

static struct fi_ops tcpx_eq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_eq_close,
	.bind = fi_no_bind,
	.control = ofi_eq_control,
	.ops_open = fi_no_ops_open,
};

int tcpx_eq_create(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		   struct fid_eq **eq_fid, void *context)
{
	struct tcpx_eq *eq;
	struct fi_wait_attr wait_attr;
	struct fid_wait *wait;
	int ret;

	eq = calloc(1, sizeof(*eq));
	if (!eq)
		return -FI_ENOMEM;

	ret = ofi_eq_init(fabric_fid, attr, &eq->util_eq.eq_fid, context);
	if (ret) {
		FI_WARN(&tcpx_prov, FI_LOG_EQ,
			"EQ creation failed\n");
		goto err1;
	}

	ret = fastlock_init(&eq->close_lock);
	if (ret)
		goto err2;

	eq->util_eq.eq_fid.ops	= &tcpx_eq_ops;
	eq->util_eq.eq_fid.fid.ops = &tcpx_eq_fi_ops;

	if (!eq->util_eq.wait) {
		memset(&wait_attr, 0, sizeof wait_attr);
		wait_attr.wait_obj = FI_WAIT_POLLFD;
		ret = fi_wait_open(fabric_fid, &wait_attr, &wait);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EQ,
				"opening wait failed\n");
			goto err3;
		}
		eq->util_eq.internal_wait = 1;
		eq->util_eq.wait = container_of(wait, struct util_wait,
					wait_fid);
	}
	*eq_fid = &eq->util_eq.eq_fid;
	return 0;
err3:
	fastlock_destroy(&eq->close_lock);
err2:
	ofi_eq_cleanup(&eq->util_eq.eq_fid.fid);
err1:
	free(eq);
	return ret;
}
