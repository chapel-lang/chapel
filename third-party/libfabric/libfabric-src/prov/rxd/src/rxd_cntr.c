/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
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
#include "rxd.h"

#define RXD_FLAG(flag, mask) (((flag) & (mask)) == (mask))

static int rxd_cntr_wait(struct fid_cntr *cntr_fid, uint64_t threshold, int timeout)
{
	struct fid_list_entry *fid_entry;
	struct util_cntr *cntr;
	struct rxd_ep *ep;
	uint64_t endtime, errcnt;
	int ret, ep_retry;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	assert(cntr->wait);
	errcnt = ofi_atomic_get64(&cntr->err);
	endtime = ofi_timeout_time(timeout);

	do {
		cntr->progress(cntr);
		if (threshold <= (uint64_t) ofi_atomic_get64(&cntr->cnt))
			return FI_SUCCESS;

		if (errcnt != (uint64_t) ofi_atomic_get64(&cntr->err))
			return -FI_EAVAIL;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_ETIMEDOUT;

		ep_retry = -1;
		ofi_genlock_lock(&cntr->ep_list_lock);
		dlist_foreach_container(&cntr->ep_list, struct fid_list_entry,
					fid_entry, entry) {
			ep = container_of(fid_entry->fid, struct rxd_ep,
					  util_ep.ep_fid.fid);
			if (ep->next_retry == -1)
				continue;
			ep_retry = ep_retry == -1 ? ep->next_retry :
					MIN(ep_retry, ep->next_retry);
		}
		ofi_genlock_unlock(&cntr->ep_list_lock);

		ret = fi_wait(&cntr->wait->wait_fid, ep_retry == -1 ?
			      timeout : rxd_get_timeout(ep_retry));
		if (ep_retry != -1 && ret == -FI_ETIMEDOUT)
			ret = 0;
	} while (!ret);

	return ret;
}

int rxd_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context)
{
	int ret;
	struct util_cntr *cntr;

	cntr = calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	ret = ofi_cntr_init(&rxd_prov, domain, attr, cntr,
			    &ofi_cntr_progress, context);
	if (ret)
		goto free;

	*cntr_fid = &cntr->cntr_fid;
	cntr->cntr_fid.ops->wait = rxd_cntr_wait;
	return FI_SUCCESS;

free:
	free(cntr);
	return ret;
}

void rxd_cntr_report_error(struct rxd_ep *ep, struct fi_cq_err_entry *err)
{
        struct util_cntr *cntr;

	cntr = RXD_FLAG(err->flags, (FI_WRITE)) ? ep->util_ep.cntrs[CNTR_WR] :
	       RXD_FLAG(err->flags, (FI_ATOMIC)) ? ep->util_ep.cntrs[CNTR_WR] :
	       RXD_FLAG(err->flags, (FI_READ)) ? ep->util_ep.cntrs[CNTR_RD] :
	       RXD_FLAG(err->flags, (FI_SEND)) ? ep->util_ep.cntrs[CNTR_TX] :
	       RXD_FLAG(err->flags, (FI_RECV)) ? ep->util_ep.cntrs[CNTR_RX] :
	       NULL;

	if (cntr)
		cntr->cntr_fid.ops->adderr(&cntr->cntr_fid, 1);
}
