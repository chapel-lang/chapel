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

static const struct fi_ops lpp_fi_cq_ops = {
	.size     = sizeof(struct fi_ops),
	.close    = lpp_fi_cq_close,
	.bind     = fi_no_bind,
	.control  = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static const struct fi_ops_cq lpp_cq_ops = {
	.size      = sizeof(struct fi_ops_cq),
	.read      = lpp_fi_cq_read,
	.readfrom  = lpp_fi_cq_readfrom,
	.readerr   = lpp_fi_cq_readerr,
	.sread     = lpp_fi_cq_sread,
	.sreadfrom = lpp_fi_cq_sreadfrom,
	.signal    = fi_no_cq_signal,
	.strerror  = fi_no_cq_strerror,
};

static const struct fi_cq_attr lpp_default_cq_attrs = {
	.size              = LPP_CQ_DEFAULT_SIZE,
	.flags             = 0,
	.format            = FI_CQ_FORMAT_TAGGED,
	.wait_obj          = FI_WAIT_NONE,
	.wait_set          = NULL,
	.wait_cond         = FI_CQ_COND_NONE,
	.signaling_vector  = 0,
};

static int lpp_cq_verify_attr(struct fi_cq_attr *attr)
{

#define CHECK_ATTR(actual, op, desired)													\
	if (actual op desired) {													\
		FI_INFO(&lpp_prov, FI_LOG_CQ, #actual " (%ld) " #op " " #desired " (%ld)\n", (uint64_t)actual, (uint64_t)desired);	\
		return -FI_EINVAL;													\
	}

	CHECK_ATTR(attr->size,              >,  LPP_CQ_MAX_SIZE);
	CHECK_ATTR(attr->flags,             !=, 0);

	CHECK_ATTR(attr->format,            <, FI_CQ_FORMAT_CONTEXT);
	CHECK_ATTR(attr->format,            >, FI_CQ_FORMAT_TAGGED);

	CHECK_ATTR(attr->wait_obj,          >, FI_WAIT_UNSPEC);
	CHECK_ATTR(attr->signaling_vector,  !=, 0);
	CHECK_ATTR(attr->wait_cond,         !=, FI_CQ_COND_NONE);
	CHECK_ATTR(attr->wait_set,          !=, FI_WAIT_NONE);

#undef CHECK_ATTR

	return 0;
}

int lpp_fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
			     struct fid_cq **cq, void *context)
{
	int			status = 0;
	struct fi_cq_attr	cq_attrs = lpp_default_cq_attrs;
	struct lpp_cq		*lpp_cqp = NULL;
	struct util_domain	*util_domain;
	struct lpp_domain	*lpp_domainp;

	util_domain = container_of(domain, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);

	// Verify that the attributes make sense for us.
	if (attr != NULL)
		cq_attrs = *attr;

	if (cq_attrs.format == FI_CQ_FORMAT_UNSPEC)
		cq_attrs.format = FI_CQ_FORMAT_TAGGED;

	if (cq_attrs.size == 0) {
		cq_attrs.size = LPP_CQ_DEFAULT_SIZE;
	} else {
		// We must round up to the next power of 2 for satisfy the ring buffer.
		cq_attrs.size = roundup_power_of_two(cq_attrs.size);
	}

	if (status = lpp_cq_verify_attr(&cq_attrs), status != 0) {
		return status;
	}

	if (lpp_cqp = calloc(1, sizeof(struct lpp_cq)), lpp_cqp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "failed to allocate CQ\n");
		return -FI_ENOMEM;
	}

	lpp_cqp->domain = lpp_domainp;
	lpp_cqp->attr = cq_attrs;

	// Mmap the queues.
	lpp_cqp->num_entries = cq_attrs.size;
	lpp_cqp->cq_size = KLPP_CQ_BYTES(lpp_cqp->num_entries);
	if (lpp_cqp->kernel_shared_base = klpp_mmap(lpp_cqp->domain, lpp_cqp->cq_size, PROT_READ | PROT_WRITE), lpp_cqp->kernel_shared_base == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "failed to mmap shared CQ area\n");
		status = -FI_ENOMEM;
		goto err;
	}

	// Get the pointers into the shared space.
	lpp_cqp->header = KLPP_CQ_GET_HEADER(lpp_cqp->kernel_shared_base, lpp_cqp->num_entries);
	lpp_cqp->cq = KLPP_CQ_GET_RINGBUF(lpp_cqp->kernel_shared_base, lpp_cqp->num_entries);
	lpp_cqp->cqerr = KLPP_CQ_GET_RINGBUF_ERR(lpp_cqp->kernel_shared_base, lpp_cqp->num_entries);

	// Hook into the kernel.  This gives us the id.
	if (klpp_cq_create(lpp_cqp) < 0) {
		status = -FI_ENOMEM;
		goto err;
	}

	// Populate the CQ.
	lpp_cqp->cq_fid.fid.fclass = FI_CLASS_CQ;
	lpp_cqp->cq_fid.fid.context = context;
	lpp_cqp->cq_fid.fid.ops = (struct fi_ops *)&lpp_fi_cq_ops;
	lpp_cqp->cq_fid.ops = (struct fi_ops_cq *)&lpp_cq_ops;
	ofi_atomic_initialize32(&lpp_cqp->ref_count, 0);
	dlist_init(&lpp_cqp->ep_list);

	switch(cq_attrs.format) {
	case FI_CQ_FORMAT_CONTEXT:
		lpp_cqp->entry_size = sizeof(struct fi_cq_entry);
		break;
	case FI_CQ_FORMAT_MSG:
		lpp_cqp->entry_size = sizeof(struct fi_cq_msg_entry);
		break;
	case FI_CQ_FORMAT_DATA:
		lpp_cqp->entry_size = sizeof(struct fi_cq_data_entry);
		break;
	case FI_CQ_FORMAT_TAGGED:
		lpp_cqp->entry_size = sizeof(struct fi_cq_tagged_entry);
		break;
	default:
		FI_WARN(&lpp_prov, FI_LOG_CQ, "invalid CQ format\n");
		status = -FI_EINVAL;
		goto err;
	}

	ofi_mutex_init(&lpp_cqp->lock);
	ofi_atomic_inc32(&lpp_cqp->domain->util_domain.ref);
	*cq = &lpp_cqp->cq_fid;
	return 0;

err:
	if (lpp_cqp->kernel_shared_base)
		klpp_munmap(lpp_cqp->kernel_shared_base, lpp_cqp->cq_size);

	if (lpp_cqp)
		free(lpp_cqp);

	return status;
}

int lpp_fi_cq_close(struct fid *cq)
{
	struct lpp_cq *lpp_cqp = container_of(cq, struct lpp_cq, cq_fid);

	if (!dlist_empty(&lpp_cqp->ep_list)) {
		FI_WARN(&lpp_prov, FI_LOG_CQ,
		    "cannot close CQ still bound to endpoints\n");
		return -FI_EBUSY;
	}

	// Unmap the queues.  Close the KLPP side and free the memory.
	klpp_munmap(lpp_cqp->kernel_shared_base, lpp_cqp->cq_size);
	klpp_cq_close(lpp_cqp);

	ofi_atomic_dec32(&lpp_cqp->domain->util_domain.ref);
	free(lpp_cqp);
	return 0;
}

static ssize_t lpp_cq_read_common(struct fid_cq *cq, void *buf, size_t count,
				  fi_addr_t *src_addr)
{
	struct lpp_cq *lpp_cqp = container_of(cq, struct lpp_cq, cq_fid);
	struct klpp_cq_tagged_entry *tentry;
	uint8_t *entry = buf;
	int status = 0;
	size_t i;

	if (buf == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "buf parameter is NULL\n");
		return -FI_EINVAL;
	}

	if (count == 0) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "count parameter is 0\n");
		return 0;
	}

	lpp_lock(lpp_cqp->domain, &lpp_cqp->lock);

	lpp_ep_progress(&lpp_cqp->ep_list);

	// If there are any error CQs, then return -FI_EAVAIL per fi_cq(3).
	if (!klpp_ringbuf_empty_cqerr(lpp_cqp->cqerr, lpp_cqp->num_entries)) {
		status = -FI_EAVAIL;
		goto unlock;
	}

	if (klpp_ringbuf_empty_cq(lpp_cqp->cq, lpp_cqp->num_entries)) {
		// If there are no completions, then we could be in an overrun conditon.
		if (lpp_cqp->header->overrun != 0) {
			status = -FI_EOVERRUN;
		} else {
			status = -FI_EAGAIN;
			lpp_cqp->empty_cnt++;
		}
	} else {
		for (i = 0; i < count && (klpp_ringbuf_peek_cq(lpp_cqp->cq, lpp_cqp->num_entries, &tentry) == 0); i++) {
			if (src_addr)
				src_addr[i] = tentry->lpp.src_addr.raw;

			memcpy(entry, &tentry->generic, lpp_cqp->entry_size);
			entry += lpp_cqp->entry_size;
			klpp_ringbuf_dequeue_discard_cq(lpp_cqp->cq, lpp_cqp->num_entries);
		}
		status = i;
	}

unlock:
	lpp_unlock(lpp_cqp->domain, &lpp_cqp->lock);
	return status;
}

ssize_t lpp_fi_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	return lpp_cq_read_common(cq, buf, count, NULL);
}

ssize_t lpp_fi_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
				     fi_addr_t *src_addr)
{
	return lpp_cq_read_common(cq, buf, count, src_addr);
}

static ssize_t lpp_cq_sread_common(struct fid_cq *cq, void *buf, size_t count,
				   fi_addr_t *src_addr, int timeout)
{
	uint64_t start_ms;
	ssize_t ret;

	start_ms = lpp_get_time_ms();
	while (true) {
		ret = lpp_cq_read_common(cq, buf, count, src_addr);
		if ((ret < 0 && ret != -FI_EAGAIN) || ret > 0) {
			break;
		} else if (timeout >= 0 &&
			   lpp_get_time_ms() - start_ms > (uint64_t)timeout) {
			ret = -FI_EAGAIN;
			break;
		}
	}

	return ret;
}

ssize_t lpp_fi_cq_sread(struct fid_cq *cq, void *buf, size_t count,
				  const void *cond, int timeout)
{
	return lpp_cq_sread_common(cq, buf, count, NULL, timeout);
}

ssize_t lpp_fi_cq_sreadfrom(struct fid_cq *cq, void *buf,
				      size_t count, fi_addr_t *src_addr,
				      const void *cond, int timeout)
{
	return lpp_cq_sread_common(cq, buf, count, src_addr, timeout);
}

ssize_t lpp_fi_cq_readerr(struct fid_cq *cq,
				    struct fi_cq_err_entry *buf, uint64_t flags)
{
	int			status = 0;
	struct lpp_cq		*lpp_cqp = container_of(cq, struct lpp_cq, cq_fid);

	if (buf == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "buf parameter is NULL\n");
		return -FI_EINVAL;
	}

	lpp_lock(lpp_cqp->domain, &lpp_cqp->lock);

	if (klpp_ringbuf_empty_cqerr(lpp_cqp->cqerr, lpp_cqp->num_entries)) {
		// If there are no completions, then we could be in an overrun conditon.
		status = (lpp_cqp->header->overrun != 0) ? -FI_EOVERRUN : -FI_EAGAIN;
	} else {
		klpp_ringbuf_dequeue_cqerr(lpp_cqp->cqerr, lpp_cqp->num_entries,
				(struct klpp_cq_err_entry *)buf);
		buf->err = lpp_translate_errno(buf->prov_errno);
		status = 1;
	}

	lpp_unlock(lpp_cqp->domain, &lpp_cqp->lock);

	return status;
}

void lpp_cq_enqueue_entry(struct lpp_cq *lpp_cqp, struct klpp_cq_tagged_entry *entry)
{
	if (lpp_cqp->header->overrun) {
		FI_WARN(&lpp_prov, FI_LOG_CQ,
			"CQ in overrun state, dropping completion (flags %lx)\n",
			entry->generic.flags);
	} else if (klpp_ringbuf_enqueue_cq(lpp_cqp->cq, lpp_cqp->num_entries, entry) != 0) {
		FI_WARN(&lpp_prov, FI_LOG_CQ,
			"Unable to enqueue CQ entry, setting overrun (flags %lx)\n",
			entry->generic.flags);
		lpp_cqp->header->overrun = 1;
	}
}

void lpp_cq_enqueue_err_entry(struct lpp_cq *lpp_cqp, struct klpp_cq_err_entry *entry)
{
	if (lpp_cqp->header->overrun) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "CQ in overrun state, dropping completion\n");
	} else if (klpp_ringbuf_enqueue_cqerr(lpp_cqp->cqerr, lpp_cqp->num_entries, entry) != 0) {
		FI_WARN(&lpp_prov, FI_LOG_CQ, "Unable to enqueue CQ entry, setting overrun\n");
		lpp_cqp->header->overrun = 1;
	}
}

// If threading == FI_THREAD_SAFE, this operation is protected by the ep "op lock".
int lpp_cq_reserve_slot(struct lpp_cq *lpp_cqp, uint64_t desc_flags)
{
	/* Note: We reserve only 1 slot for a MULTI_RECV descriptor, even
	 * though it may generate more than 1 completion (it is impossible to
	 * know a priori how many completions it could generate). The pending
	 * field will be decremented when the MULTI_RECV descriptor is consumed.
	 */
	if (!lpp_cq_overcommit && (desc_flags & (FI_COMPLETION | FI_MULTI_RECV)) != 0) {
		/* The order of the num_free check and atomic_fetch matters.
		 * The kernel first decrements pending and then enqueues the CQ
		 * entry, and can't do this atomically. So there is a small
		 * window where an IO is neither counted in pending nor has a
		 * CQ entry yet. Because we check the num_free first and
		 * pending second (opposite order of the kernel), the only
		 * inconsistency we might see here is that there are fewer
		 * available CQ slots than there actually are (because the
		 * kernel has decremented pending and is about to enqueue an
		 * entry, thereby reducing num_free). We prefer to allow for
		 * the small chance of CQ overrun (which is always possible,
		 * since a MULTI_RECV descriptor can produce an arbitrary
		 * number of completions) rather than to prevent an IO from
		 * starting when there would actually be space available. */
		if (klpp_ringbuf_num_free_cq(lpp_cqp->cq, lpp_cqp->num_entries) <=
		    atomic_fetch_add((_Atomic int64_t *)&lpp_cqp->header->pending, 1)) {
			atomic_fetch_sub((_Atomic int64_t *)&lpp_cqp->header->pending, 1);
			return -1;
		} else {
			return 1;
		}
	}
	return 0;
}

void lpp_cq_unreserve_slot(struct lpp_cq *lpp_cqp)
{
	atomic_fetch_sub((_Atomic int64_t *)&lpp_cqp->header->pending, 1);
}
