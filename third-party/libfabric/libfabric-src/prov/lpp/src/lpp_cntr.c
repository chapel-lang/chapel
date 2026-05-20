/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

static const struct fi_ops lpp_fi_cntr_ops = {
	.size      = sizeof(struct fi_ops),
	.close     = lpp_fi_cntr_close,
	.bind      = fi_no_bind,
	.control   = fi_no_control,
	.ops_open  = fi_no_ops_open,
};

static const struct fi_ops_cntr lpp_cntr_ops = {
	.size      = sizeof(struct fi_ops_cntr),
	.read      = lpp_fi_cntr_read,
	.readerr   = lpp_fi_cntr_readerr,
	.add       = lpp_fi_cntr_add,
	.adderr    = lpp_fi_cntr_adderr,
	.set       = lpp_fi_cntr_set,
	.seterr    = lpp_fi_cntr_seterr,
	.wait      = lpp_fi_cntr_wait,
};

static const struct fi_cntr_attr lpp_default_cntr_attrs = {
	.events    = FI_CNTR_EVENTS_COMP,
	.wait_obj  = FI_WAIT_UNSPEC,
	.wait_set  = NULL,
	.flags     = 0,
};

int lpp_fi_cntr_open(struct fid_domain *domain,
			       struct fi_cntr_attr *attr,
			       struct fid_cntr **cntr, void *context)
{
	int			i = 0;
	struct lpp_cntr		*lpp_cntrp = NULL;
	struct util_domain	*util_domain;
	struct lpp_domain	*lpp_domainp;

	util_domain = container_of(domain, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);

	// Verify that the attributes are correct.
	if (attr != NULL) {
		if (attr->events != FI_CNTR_EVENTS_COMP) {
			FI_WARN(&lpp_prov, FI_LOG_CNTR, "attrs->events is invalid (%d)\n", attr->events);
			return -FI_EINVAL;
		}

		if ((attr->wait_obj != FI_WAIT_NONE) && (attr->wait_obj != FI_WAIT_UNSPEC)) {
			FI_WARN(&lpp_prov, FI_LOG_CNTR, "attr->wait_obj is invalid (%d)\n", attr->wait_obj);
			return -FI_EINVAL;
		}
	}

	// Find a domain counter to use.
	lpp_lock(lpp_domainp, &lpp_domainp->cntr_lock);
	for (i = 0, lpp_cntrp = lpp_domainp->cntr_array; i < lpp_domainp->cntr_total; i++, lpp_cntrp++) {
		if (lpp_cntrp->valid == 0) {
			break;
		}
	}

	// If we found one, mark it as in use.
	if (i < lpp_domainp->cntr_total)
		lpp_cntrp->valid = 1;

	lpp_unlock(lpp_domainp, &lpp_domainp->cntr_lock);

	// If we didn't find one, return an error.
	if (i >= lpp_domainp->cntr_total) {
		FI_WARN(&lpp_prov, FI_LOG_CNTR, "maximum number of cntrs reached\n");
		return -FI_ENOMEM;
	}

	// Hook into the kernel.  This gives us the id. If this fails, free the CNTR.
	lpp_cntrp->domain = lpp_domainp;
	if (klpp_cntr_create(lpp_cntrp) < 0) {
		lpp_cntrp->valid = 0;
		return -FI_ENOMEM;
	}

	// Fill in the counter.
	lpp_cntrp->cntr_fid.fid.fclass = FI_CLASS_CNTR;
	lpp_cntrp->cntr_fid.fid.context = context;
	lpp_cntrp->cntr_fid.fid.ops = (struct fi_ops *)&lpp_fi_cntr_ops;
	lpp_cntrp->cntr_fid.ops = (struct fi_ops_cntr *)&lpp_cntr_ops;

	lpp_cntrp->attrs = (attr != NULL) ? *attr : lpp_default_cntr_attrs;
	lpp_cntrp->valid = 1;
	lpp_cntrp->index = i;

	ofi_atomic_initialize32(&lpp_cntrp->ref_count, 0);
	ofi_atomic_initialize64(&lpp_cntrp->last_counter, 0);
	ofi_atomic_initialize64(&lpp_cntrp->last_errcounter, 0);
	dlist_init(&lpp_cntrp->ep_list);

	ofi_mutex_init(&lpp_cntrp->lock);

	*lpp_cntrp->counter = 0;
	*lpp_cntrp->errcounter = 0;

	ofi_atomic_inc32(&lpp_cntrp->domain->util_domain.ref);
	*cntr = &lpp_cntrp->cntr_fid;
	return 0;
}

int lpp_fi_cntr_close(struct fid *cntr)
{
	struct lpp_cntr *lpp_cntrp = NULL;

	lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	if (!dlist_empty(&lpp_cntrp->ep_list)) {
		FI_WARN(&lpp_prov, FI_LOG_CQ,
			"cannot close cntr still bound to endpoints\n");
		return -FI_EBUSY;
	}

	// Tell KLPP that we are done with this counter.
	if (klpp_cntr_close(lpp_cntrp) < 0) {
		return -FI_EBUSY;
	}

	// Give the counters back to the domain.
	lpp_cntrp->valid = 0;

	// Free the counter.
	ofi_atomic_dec32(&lpp_cntrp->domain->util_domain.ref);
	return 0;
}

uint64_t lpp_fi_cntr_read(struct fid_cntr *cntr)
{
	struct lpp_cntr *lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);
	uint64_t val;
	uint64_t last_val;

	lpp_lock(lpp_cntrp->domain, &lpp_cntrp->lock);
	val = atomic_load(lpp_cntrp->counter);
	last_val = ofi_atomic_get64(&lpp_cntrp->last_counter);
	ofi_atomic_set64(&lpp_cntrp->last_counter, val);

	// If nothing has happened since we last checked, go try to make some progress.
	if (val == last_val) {
		lpp_ep_progress(&lpp_cntrp->ep_list);
	}

	lpp_unlock(lpp_cntrp->domain, &lpp_cntrp->lock);

	return val;
}

uint64_t lpp_fi_cntr_readerr(struct fid_cntr *cntr)
{
	struct lpp_cntr *lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	lpp_lock(lpp_cntrp->domain, &lpp_cntrp->lock);
	ofi_atomic_set64(&lpp_cntrp->last_errcounter, atomic_load(lpp_cntrp->errcounter));
	lpp_unlock(lpp_cntrp->domain, &lpp_cntrp->lock);

	return ofi_atomic_get64(&lpp_cntrp->last_errcounter);
}

int lpp_fi_cntr_wait(struct fid_cntr *cntr, uint64_t threshold,
			       int timeout)
{
	uint64_t cntr_errvalue;
	struct lpp_cntr *lpp_cntrp;
	uint64_t start_ms;
	int ret;

	lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	cntr_errvalue = ofi_atomic_get64(&lpp_cntrp->last_errcounter);
	if (lpp_fi_cntr_readerr(&lpp_cntrp->cntr_fid) > cntr_errvalue)
		return -FI_EAVAIL;

	start_ms = lpp_get_time_ms();
	while (true) {
		if (lpp_fi_cntr_read(&lpp_cntrp->cntr_fid) >= threshold) {
			ret = 0;
			break;
		} else if (timeout >= 0 &&
			   lpp_get_time_ms() - start_ms > timeout) {
			ret = -FI_ETIMEDOUT;
			break;
		} else if (lpp_fi_cntr_readerr(&lpp_cntrp->cntr_fid) >
			   cntr_errvalue) {
			ret = -FI_EAVAIL;
			break;
		}
	}

	return ret;
}

int lpp_fi_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	struct lpp_cntr	*lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	atomic_fetch_add(lpp_cntrp->counter, value);

	return 0;
}

int lpp_fi_cntr_adderr(struct fid_cntr *cntr, uint64_t value)
{
	struct lpp_cntr	*lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	atomic_fetch_add(lpp_cntrp->errcounter, value);

	return 0;
}

int lpp_fi_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct lpp_cntr	*lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	atomic_store(lpp_cntrp->counter, value);

	return 0;
}

int lpp_fi_cntr_seterr(struct fid_cntr *cntr, uint64_t value)
{
	struct lpp_cntr	*lpp_cntrp = container_of(cntr, struct lpp_cntr, cntr_fid);

	atomic_store(lpp_cntrp->errcounter, value);

	return 0;
}
