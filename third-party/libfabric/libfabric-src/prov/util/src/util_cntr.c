/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#include <ofi_enosys.h>
#include <ofi_util.h>

static int ofi_check_cntr_attr(const struct fi_provider *prov,
			       const struct fi_cntr_attr *attr)
{
	if (!attr)
		return FI_SUCCESS;

        if (attr->flags) {
		FI_WARN(prov, FI_LOG_CNTR, "unsupported flags\n");
		return -FI_EINVAL;
	}

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_YIELD:
		break;
	case FI_WAIT_SET:
		if (!attr->wait_set) {
			FI_WARN(prov, FI_LOG_CNTR, "invalid wait set\n");
			return -FI_EINVAL;
		}
		/* fall through */
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_POLLFD:
		break;
	default:
		FI_WARN(prov, FI_LOG_CNTR, "unsupported wait object\n");
		return -FI_EINVAL;
	}

	return 0;
}

static uint64_t ofi_cntr_read(struct fid_cntr *cntr_fid)
{
	struct util_cntr *cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);

	assert(cntr->cntr_fid.fid.fclass == FI_CLASS_CNTR);
	cntr->progress(cntr);

	return ofi_atomic_get64(&cntr->cnt);
}

static uint64_t ofi_cntr_readerr(struct fid_cntr *cntr_fid)
{
	struct util_cntr *cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);

	assert(cntr->cntr_fid.fid.fclass == FI_CLASS_CNTR);
	cntr->progress(cntr);

	return ofi_atomic_get64(&cntr->err);
}

static int ofi_cntr_add(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);

	assert(cntr->cntr_fid.fid.fclass == FI_CLASS_CNTR);

	ofi_atomic_add64(&cntr->cnt, value);
	if (cntr->wait)
		cntr->wait->signal(cntr->wait);

	return FI_SUCCESS;
}

static int ofi_cntr_adderr(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);

	assert(cntr->cntr_fid.fid.fclass == FI_CLASS_CNTR);

	ofi_atomic_add64(&cntr->err, value);
	if (cntr->wait)
		cntr->wait->signal(cntr->wait);

	return FI_SUCCESS;
}

static int ofi_cntr_set(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);

	assert(cntr->cntr_fid.fid.fclass == FI_CLASS_CNTR);

	ofi_atomic_set64(&cntr->cnt, value);
	if (cntr->wait)
		cntr->wait->signal(cntr->wait);

	return FI_SUCCESS;
}

static int ofi_cntr_seterr(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	assert(cntr->cntr_fid.fid.fclass == FI_CLASS_CNTR);

	ofi_atomic_set64(&cntr->err, value);
	if (cntr->wait)
		cntr->wait->signal(cntr->wait);

	return FI_SUCCESS;
}

#define OFI_TIMEOUT_QUANTUM_MS 50

static int ofi_cntr_wait(struct fid_cntr *cntr_fid, uint64_t threshold, int timeout)
{
	struct util_cntr *cntr;
	uint64_t endtime, errcnt;
	int ret, timeout_quantum;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	assert(cntr->wait);
	errcnt = ofi_atomic_get64(&cntr->err);
	endtime = ofi_timeout_time(timeout);

	do {
		cntr->progress(cntr);
		if (threshold <= ofi_atomic_get64(&cntr->cnt))
			return FI_SUCCESS;

		if (errcnt != ofi_atomic_get64(&cntr->err))
			return -FI_EAVAIL;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_ETIMEDOUT;

		/*
		 * Temporary work-around to avoid a thread hanging in underlying
		 * epoll_wait called from fi_wait. This can happen if one thread
		 * updates the counter, another thread reads it (thereby resetting
		 * cntr signal fd) and the current thread is about to wait. The
		 * current thread would never wake up and doesn't know the counter
		 * has been updated. Fix it by checking counter state every now
		 * and then instead of waiting for a longer period. This does
		 * have the overhead of threads waking up unnecessarily.
		 */
		timeout_quantum = (timeout < 0 ? OFI_TIMEOUT_QUANTUM_MS :
				   MIN(OFI_TIMEOUT_QUANTUM_MS, timeout));

		ret = fi_wait(&cntr->wait->wait_fid, timeout_quantum);
	} while (!ret || (ret == -FI_ETIMEDOUT &&
			  (timeout < 0 || timeout_quantum < timeout)));

	return ret;
}

static struct fi_ops_cntr util_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = ofi_cntr_read,
	.readerr = ofi_cntr_readerr,
	.add = ofi_cntr_add,
	.adderr = ofi_cntr_adderr,
	.set = ofi_cntr_set,
	.seterr = ofi_cntr_seterr,
	.wait = ofi_cntr_wait
};

static struct fi_ops_cntr util_cntr_no_wait_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = ofi_cntr_read,
	.readerr = ofi_cntr_readerr,
	.add = ofi_cntr_add,
	.adderr = ofi_cntr_adderr,
	.set = ofi_cntr_set,
	.seterr = ofi_cntr_seterr,
	.wait = fi_no_cntr_wait,
};

int ofi_cntr_cleanup(struct util_cntr *cntr)
{
	if (ofi_atomic_get32(&cntr->ref))
		return -FI_EBUSY;

	if (cntr->wait) {
		fi_poll_del(&cntr->wait->pollset->poll_fid,
			    &cntr->cntr_fid.fid, 0);
		if (cntr->internal_wait)
			fi_close(&cntr->wait->wait_fid.fid);
	}

	ofi_atomic_dec32(&cntr->domain->ref);
	fastlock_destroy(&cntr->ep_list_lock);
	return 0;
}

static int util_cntr_close(struct fid *fid)
{
	struct util_cntr *cntr;
	int ret;

	cntr = container_of(fid, struct util_cntr, cntr_fid.fid);
	ret = ofi_cntr_cleanup(cntr);
	if (ret)
		return ret;
	free(cntr);
	return 0;
}

void ofi_cntr_progress(struct util_cntr *cntr)
{
	struct util_ep *ep;
	struct fid_list_entry *fid_entry;
	struct dlist_entry *item;

	fastlock_acquire(&cntr->ep_list_lock);
	dlist_foreach(&cntr->ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		ep = container_of(fid_entry->fid, struct util_ep, ep_fid.fid);
		ep->progress(ep);
	}
	fastlock_release(&cntr->ep_list_lock);
}

static struct fi_ops util_cntr_fi_ops = {
	.size = sizeof(util_cntr_fi_ops),
	.close = util_cntr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int ofi_cntr_init(const struct fi_provider *prov, struct fid_domain *domain,
		  struct fi_cntr_attr *attr, struct util_cntr *cntr,
		  ofi_cntr_progress_func progress, void *context)
{
	int ret;
	struct fi_wait_attr wait_attr;
	struct fid_wait *wait;

	assert(progress);
	ret = ofi_check_cntr_attr(prov, attr);
	if (ret)
		return ret;

	cntr->progress = progress;
	cntr->domain = container_of(domain, struct util_domain, domain_fid);
	ofi_atomic_initialize32(&cntr->ref, 0);
	ofi_atomic_initialize64(&cntr->cnt, 0);
	ofi_atomic_initialize64(&cntr->err, 0);
	dlist_init(&cntr->ep_list);

	cntr->cntr_fid.fid.fclass = FI_CLASS_CNTR;
	cntr->cntr_fid.fid.context = context;
	cntr->cntr_fid.fid.ops = &util_cntr_fi_ops;
	cntr->cntr_fid.ops = &util_cntr_ops;

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
		wait = NULL;
		cntr->cntr_fid.ops = &util_cntr_no_wait_ops;
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_POLLFD:
	case FI_WAIT_MUTEX_COND:
	case FI_WAIT_YIELD:
		memset(&wait_attr, 0, sizeof wait_attr);
		wait_attr.wait_obj = attr->wait_obj;
		cntr->internal_wait = 1;
		ret = fi_wait_open(&cntr->domain->fabric->fabric_fid,
				   &wait_attr, &wait);
		if (ret)
			return ret;
		break;
	case FI_WAIT_SET:
		wait = attr->wait_set;
		break;
	default:
		assert(0);
		return -FI_EINVAL;
	}

	fastlock_init(&cntr->ep_list_lock);
	ofi_atomic_inc32(&cntr->domain->ref);

	/* CNTR must be fully operational before adding to wait set */
	if (wait) {
		cntr->wait = container_of(wait, struct util_wait, wait_fid);
		ret = fi_poll_add(&cntr->wait->pollset->poll_fid,
				  &cntr->cntr_fid.fid, 0);
		if (ret) {
			ofi_cntr_cleanup(cntr);
			return ret;
		}
	}

	return 0;
}

ofi_ep_cntr_inc_func ofi_ep_tx_cntr_inc_funcs[] = {
	[ofi_op_msg] = ofi_ep_tx_cntr_inc,
	[ofi_op_tagged] = ofi_ep_tx_cntr_inc,
	[ofi_op_read_req] = ofi_ep_rd_cntr_inc,
	[ofi_op_read_rsp] = ofi_ep_rem_rd_cntr_inc,
	[ofi_op_write] = ofi_ep_wr_cntr_inc,
	[ofi_op_write_async] = ofi_ep_wr_cntr_inc,
	[ofi_op_atomic] = ofi_ep_wr_cntr_inc,
	[ofi_op_atomic_fetch] = ofi_ep_rd_cntr_inc,
	[ofi_op_atomic_compare] = ofi_ep_rd_cntr_inc,
	[ofi_op_read_async] = ofi_ep_rd_cntr_inc,
};

ofi_ep_cntr_inc_func ofi_ep_rx_cntr_inc_funcs[] = {
	[ofi_op_msg] = ofi_ep_rx_cntr_inc,
	[ofi_op_tagged] = ofi_ep_rx_cntr_inc,
	[ofi_op_read_req] = ofi_ep_rem_rd_cntr_inc,
	[ofi_op_read_rsp] = ofi_ep_rd_cntr_inc,
	[ofi_op_write] = ofi_ep_rem_wr_cntr_inc,
	[ofi_op_write_async] = ofi_ep_rem_wr_cntr_inc,
	[ofi_op_atomic] = ofi_ep_rem_wr_cntr_inc,
	[ofi_op_atomic_fetch] = ofi_ep_rem_rd_cntr_inc,
	[ofi_op_atomic_compare] = ofi_ep_rem_rd_cntr_inc,
	[ofi_op_read_async] = ofi_ep_rem_rd_cntr_inc,
};
