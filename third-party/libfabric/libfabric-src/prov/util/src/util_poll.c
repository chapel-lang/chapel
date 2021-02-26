/*
 * Copyright (c) 2014-2016 Intel Corporation, Inc.  All rights reserved.
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


static int util_poll_add(struct fid_poll *poll_fid, struct fid *event_fid,
			 uint64_t flags)
{
	struct util_poll *pollset;

	pollset = container_of(poll_fid, struct util_poll, poll_fid);
	switch (event_fid->fclass) {
	case FI_CLASS_CQ:
	case FI_CLASS_CNTR:
		break;
	case FI_CLASS_EQ:
		/* We support EQs for internal poll sets */
		if (!pollset->domain)
			break;
		/* fall through */
	default:
		FI_WARN(pollset->prov, FI_LOG_DOMAIN,
			"invalid fid class\n");
		return -FI_EINVAL;
	}

	return fid_list_insert(&pollset->fid_list, &pollset->lock, event_fid);
}

static int util_poll_del(struct fid_poll *poll_fid, struct fid *event_fid,
			 uint64_t flags)
{
	struct util_poll *pollset;

	pollset = container_of(poll_fid, struct util_poll, poll_fid);
	fid_list_remove(&pollset->fid_list, &pollset->lock, event_fid);
	return 0;
}

static int util_poll_run(struct fid_poll *poll_fid, void **context, int count)
{
	struct util_poll *pollset;
	struct util_eq *eq;
	struct util_cq *cq;
	struct util_cntr *cntr;
	struct fid_list_entry *fid_entry;
	struct dlist_entry *item;
	int i = 0, err = 0;
	ssize_t ret;
	uint64_t val;

	pollset = container_of(poll_fid, struct util_poll, poll_fid.fid);

	fastlock_acquire(&pollset->lock);
	dlist_foreach(&pollset->fid_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		switch (fid_entry->fid->fclass) {
		case FI_CLASS_CQ:
			cq = container_of(fid_entry->fid, struct util_cq,
					  cq_fid.fid);
			ret = fi_cq_read(&cq->cq_fid, NULL, 0);
			if (ret == 0 || ret == -FI_EAVAIL)
				ret = 1;
			break;
		case FI_CLASS_CNTR:
			cntr = container_of(fid_entry->fid, struct util_cntr,
					    cntr_fid.fid);
			val = fi_cntr_read(&cntr->cntr_fid);
			ret = (val != cntr->checkpoint_cnt);
			if (ret) {
				cntr->checkpoint_cnt = val;
			} else {
				val = fi_cntr_readerr(&cntr->cntr_fid);
				ret = (val != cntr->checkpoint_err);
				if (ret)
					cntr->checkpoint_err = val;
			}
			break;
		case FI_CLASS_EQ:
			eq = container_of(fid_entry->fid, struct util_eq,
					  eq_fid.fid);
			ret = fi_eq_read(&eq->eq_fid, NULL, NULL, 0, FI_PEEK);
			if (ret == 0 || ret == -FI_EAVAIL)
				ret = 1;
			break;
		default:
			ret = -FI_EINVAL;
			break;
		}

		if (ret > 0 && i < count)
			context[i++] = fid_entry->fid->context;
		else if (ret < 0 && ret != -FI_EAGAIN)
			err = (int) ret;
	}
	fastlock_release(&pollset->lock);
	return i ? i : err;
}

static int util_poll_close(struct fid *fid)
{
	struct util_poll *pollset;

	pollset = container_of(fid, struct util_poll, poll_fid.fid);
	if (ofi_atomic_get32(&pollset->ref))
		return -FI_EBUSY;

	if (pollset->domain)
		ofi_atomic_dec32(&pollset->domain->ref);

	fastlock_destroy(&pollset->lock);

	free(pollset);
	return 0;
}

static struct fi_ops_poll util_poll_ops = {
	.size = sizeof(struct fi_ops_poll),
	.poll = util_poll_run,
	.poll_add = util_poll_add,
	.poll_del = util_poll_del,
};

static struct fi_ops util_poll_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = util_poll_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static int util_verify_poll_attr(const struct fi_provider *prov,
				 struct fi_poll_attr *attr)
{
	if (attr->flags) {
		FI_WARN(prov, FI_LOG_DOMAIN, "invalid flags\n");
		return -FI_EINVAL;
	}

	return 0;
}

int fi_poll_create_(const struct fi_provider *prov, struct fid_domain *domain,
		    struct fi_poll_attr *attr, struct fid_poll **poll_fid)
{
	struct util_poll *pollset;
	int ret;

	ret = util_verify_poll_attr(prov, attr);
	if (ret)
		return ret;

	pollset = calloc(1, sizeof(*pollset));
	if (!pollset)
		return -FI_ENOMEM;

	pollset->prov = prov;
	ofi_atomic_initialize32(&pollset->ref, 0);
	dlist_init(&pollset->fid_list);
	fastlock_init(&pollset->lock);

	pollset->poll_fid.fid.fclass = FI_CLASS_POLL;
	pollset->poll_fid.fid.ops = &util_poll_fi_ops;
	pollset->poll_fid.ops = &util_poll_ops;

	/* domain is NULL if pollset is used internally by a waitset */
	if (domain) {
		pollset->domain = container_of(domain, struct util_domain,
					       domain_fid);
		ofi_atomic_inc32(&pollset->domain->ref);
	}

	*poll_fid = &pollset->poll_fid;
	return 0;
}

int fi_poll_create(struct fid_domain *domain_fid, struct fi_poll_attr *attr,
		   struct fid_poll **poll_fid)
{
	struct util_domain *domain;

	domain = container_of(domain_fid, struct util_domain, domain_fid);
	return fi_poll_create_(domain->prov, domain_fid, attr, poll_fid);
}
