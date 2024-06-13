/*
 * Copyright (c) 2016-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#include <assert.h>
#include "ucx.h"

static ssize_t ucx_cq_sreadfrom(struct fid_cq *cq_fid, void *buf, size_t count,
				fi_addr_t *src_addr, const void *cond,
				int timeout)
{
	struct util_cq *cq;
	uint64_t endtime;
	ssize_t ret;

	cq = container_of(cq_fid, struct util_cq, cq_fid);
	endtime = ofi_timeout_time(timeout);

	while (1) {
		ret = fi_cq_readfrom(cq_fid, buf, count, src_addr);
		if (ret != -FI_EAGAIN)
			return ret;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_EAGAIN;

		if (ofi_atomic_get32(&cq->wakeup)) {
			ofi_atomic_set32(&cq->wakeup, 0);
			return -FI_EAGAIN;
		}

		sched_yield();
	}
}

static ssize_t ucx_cq_sread(struct fid_cq *cq_fid, void *buf, size_t count,
			    const void *cond, int timeout)
{
	return ucx_cq_sreadfrom(cq_fid, buf, count, NULL, cond, timeout);
}

static struct fi_ops_cq ucx_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = ucx_cq_sread,
	.sreadfrom = ucx_cq_sreadfrom,
	.signal = ofi_cq_signal,
	.strerror = ofi_cq_strerror,
};

int ucx_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	int status = FI_SUCCESS;
	struct util_cq *u_cq;

	u_cq = calloc(1, sizeof(struct util_cq));
	if (!u_cq)
		return -FI_ENOMEM;

	status = ofi_cq_init(&ucx_prov, domain, attr, u_cq, &ofi_cq_progress,
			     context);
	if (status) {
		free(u_cq);
		return status;
	}

        u_cq->cq_fid.ops = &ucx_cq_ops;
	*cq_fid = &u_cq->cq_fid;
	return FI_SUCCESS;
}
