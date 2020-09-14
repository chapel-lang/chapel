/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include "ofi_util.h"
#include "rxr.h"
#include "rxr_cntr.h"

static int efa_cntr_wait(struct fid_cntr *cntr_fid, uint64_t threshold, int timeout)
{
	struct util_cntr *cntr;
	uint64_t start, errcnt;
	int ret;
	int numtry = 5;
	int tryid = 0;
	int waitim = 1;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	assert(cntr->wait);
	errcnt = ofi_atomic_get64(&cntr->err);
	start = (timeout >= 0) ? ofi_gettime_ms() : 0;

	for (tryid = 0; tryid < numtry; ++tryid) {
		cntr->progress(cntr);
		if (threshold <= ofi_atomic_get64(&cntr->cnt))
			return FI_SUCCESS;

		if (errcnt != ofi_atomic_get64(&cntr->err))
			return -FI_EAVAIL;

		if (timeout >= 0) {
			timeout -= (int)(ofi_gettime_ms() - start);
			if (timeout <= 0)
				return -FI_ETIMEDOUT;
		}

		ret = fi_wait(&cntr->wait->wait_fid, waitim);
		if (ret == -FI_ETIMEDOUT)
			ret = 0;

		waitim *= 2;
	}

	return ret;
}

int efa_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context)
{
	int ret;
	struct util_cntr *cntr;

	cntr = calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	ret = ofi_cntr_init(&rxr_prov, domain, attr, cntr,
			    &ofi_cntr_progress, context);
	if (ret)
		goto free;

	*cntr_fid = &cntr->cntr_fid;
	cntr->cntr_fid.ops->wait = efa_cntr_wait;
	return FI_SUCCESS;

free:
	free(cntr);
	return ret;
}

void efa_cntr_report_tx_completion(struct util_ep *ep, uint64_t flags)
{
	struct util_cntr *cntr;

	flags &= (FI_SEND | FI_WRITE | FI_READ);
	assert(flags == FI_SEND || flags == FI_WRITE || flags == FI_READ);

	if (flags == FI_SEND)
		cntr = ep->tx_cntr;
	else if (flags == FI_WRITE)
		cntr = ep->wr_cntr;
	else if (flags == FI_READ)
		cntr = ep->rd_cntr;
	else
		cntr = NULL;

	if (cntr)
		cntr->cntr_fid.ops->add(&cntr->cntr_fid, 1);
}

void efa_cntr_report_rx_completion(struct util_ep *ep, uint64_t flags)
{
	struct util_cntr *cntr;

	flags &= (FI_RECV | FI_REMOTE_WRITE | FI_REMOTE_READ);
	assert(flags == FI_RECV || flags == FI_REMOTE_WRITE || flags == FI_REMOTE_READ);

	if (flags == FI_RECV)
		cntr = ep->rx_cntr;
	else if (flags == FI_REMOTE_READ)
		cntr = ep->rem_rd_cntr;
	else if (flags == FI_REMOTE_WRITE)
		cntr = ep->rem_wr_cntr;
	else
		cntr = NULL;

	if (cntr)
		cntr->cntr_fid.ops->add(&cntr->cntr_fid, 1);
}

void efa_cntr_report_error(struct util_ep *ep, uint64_t flags)
{
	flags = flags & (FI_SEND | FI_READ | FI_WRITE | FI_ATOMIC |
			 FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE);

	struct util_cntr *cntr;

	if (flags == FI_WRITE || flags == FI_ATOMIC)
		cntr = ep->wr_cntr;
	else if (flags == FI_READ)
		cntr = ep->rd_cntr;
	else if (flags == FI_SEND)
		cntr = ep->tx_cntr;
	else if (flags == FI_RECV)
		cntr = ep->rx_cntr;
	else if (flags == FI_REMOTE_READ)
		cntr = ep->rem_rd_cntr;
	else if (flags == FI_REMOTE_WRITE)
		cntr = ep->rem_wr_cntr;
	else
		cntr = NULL;

	if (cntr)
		cntr->cntr_fid.ops->adderr(&cntr->cntr_fid, 1);
}

