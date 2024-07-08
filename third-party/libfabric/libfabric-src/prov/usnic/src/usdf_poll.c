/*
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ofi.h>
#include <ofi_enosys.h>
#include <ofi_util.h>

#include "usdf.h"
#include "usdf_cq.h"
#include "usdf_poll.h"

static int usdf_poll_poll(struct fid_poll *fps, void **context, int count)
{
	struct usdf_cq *cq;
	struct usdf_poll *ps;
	struct dlist_entry *item;
	struct fid_list_entry *entry;
	int progressed = 0;
	int copied = 0;
	int pending;

	if (!fps || !context) {
		USDF_WARN_SYS(DOMAIN, "pollset and context can't be NULL.\n");
		return -FI_EINVAL;
	}

	ps = poll_ftou(fps);

	ofi_mutex_lock(&ps->lock);

	dlist_foreach(&ps->list, item) {
		entry = container_of(item, struct fid_list_entry, entry);
		assert(entry->fid->fclass == FI_CLASS_CQ);

		cq = cq_fidtou(entry->fid);

		if (cq->cq_is_soft) {
			if (!progressed) {
				usdf_domain_progress(ps->poll_domain);
				progressed = 1;
			}

			pending = !usdf_check_empty_soft_cq(cq);
		} else {
			pending = !usdf_check_empty_hard_cq(cq);
		}

		if (pending) {
			context[copied++] = entry->fid->context;

			if (copied >= count)
				break;
		}
	}

	ofi_mutex_unlock(&ps->lock);

	return copied;
}

static int usdf_poll_add(struct fid_poll *fps, struct fid *event_fid,
		uint64_t flags)
{
	struct usdf_poll *ps;
	struct usdf_cq *cq;
	int ret;

	USDF_TRACE_SYS(DOMAIN, "\n");

	if (!fps || !event_fid) {
		USDF_WARN_SYS(DOMAIN, "pollset and event_fid can't be NULL.\n");
		return -FI_EINVAL;
	}

	ps = poll_ftou(fps);

	switch (event_fid->fclass) {
	case FI_CLASS_CQ:
		break;
	default:
		USDF_WARN_SYS(DOMAIN, "invalid fid class.\n");
		return -FI_EINVAL;
	}

	ret = fid_list_insert(&ps->list, &ps->lock, event_fid);
	if (ret)
		return ret;

	cq = cq_fidtou(event_fid);
	ret = ofi_atomic_inc32(&cq->cq_refcnt);
	assert(ret > 0);
	USDF_DBG_SYS(DOMAIN, "associated with CQ: [%p] with new refcnt: [%d]\n",
			cq, ret);

	return FI_SUCCESS;
}

static int usdf_poll_del(struct fid_poll *fps, struct fid *event_fid,
		uint64_t flags)
{
	struct usdf_poll *ps;
	struct usdf_cq *cq;
	int ret;

	if (!fps || !event_fid) {
		USDF_WARN_SYS(DOMAIN, "pollset and event_fid can't be NULL.\n");
		return -FI_EINVAL;
	}

	USDF_TRACE_SYS(DOMAIN, "\n");

	ps = poll_ftou(fps);

	switch (event_fid->fclass) {
	case FI_CLASS_CQ:
		break;
	default:
		USDF_WARN_SYS(DOMAIN, "invalid fid class.\n");
		return -FI_EINVAL;
	}

	fid_list_remove(&ps->list, &ps->lock, event_fid);

	cq = cq_fidtou(event_fid);
	ret = ofi_atomic_dec32(&cq->cq_refcnt);

	USDF_DBG_SYS(DOMAIN,
			"disassociating from CQ: [%p] with new refcnt: [%d]\n",
			cq, ret);
	assert(ret >= 0);

	if (ret >= 0)
		ret = FI_SUCCESS;
	else
		ret = -FI_EINVAL;
	return ret;
}

static int usdf_poll_close(struct fid *fps)
{
	struct usdf_poll *ps;
	struct dlist_entry *item;
	struct dlist_entry *head;
	struct fid_list_entry *entry;
	struct usdf_cq *cq;
	int val, ret = FI_SUCCESS;

	USDF_TRACE_SYS(DOMAIN, "\n");

	if (!fps) {
		USDF_WARN_SYS(DOMAIN, "pollset can't be NULL.\n");
		return -FI_EINVAL;
	}

	ps = poll_ftou(fps);

	if (ofi_atomic_get32(&ps->poll_refcnt) > 0) {
		USDF_WARN_SYS(DOMAIN,
				"failed to close pollset with non-zero refcnt");
		return -FI_EBUSY;
	}

	head = &ps->list;
	while (!dlist_empty(head)) {
		item = head->next;
		entry = container_of(item, struct fid_list_entry, entry);

		switch (entry->fid->fclass) {
		case FI_CLASS_CQ:
			cq = cq_fidtou(entry->fid);
			val = ofi_atomic_dec32(&cq->cq_refcnt);

			USDF_DBG_SYS(DOMAIN,
					"disassociating from CQ: [%p] with new refcnt: [%d]\n",
					cq, val);
			assert(val >= 0);
			if (val < 0)
				ret = -FI_EINVAL;
			break;
		default:
			USDF_WARN_SYS(DOMAIN, "invalid object\n");
			break;
		}

		dlist_remove(item);
		free(entry);
	}

	ofi_atomic_dec32(&ps->poll_domain->dom_refcnt);
	ofi_mutex_destroy(&ps->lock);
	free(ps);

	return ret;
}

struct fi_ops_poll usdf_poll_ops = {
	.size = sizeof(struct fi_ops_poll),
	.poll = usdf_poll_poll,
	.poll_add = usdf_poll_add,
	.poll_del = usdf_poll_del
};

struct fi_ops usdf_poll_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_poll_close,
	.bind = fi_no_bind,
	.ops_open = fi_no_ops_open
};

int usdf_poll_open(struct fid_domain *fdom, struct fi_poll_attr *attr,
		struct fid_poll **fps)
{
	struct usdf_poll *ps;
	struct usdf_domain *dom;
	int ret;

	USDF_TRACE_SYS(DOMAIN, "\n");

	if (attr && attr->flags != 0) {
		USDF_WARN_SYS(DOMAIN, "flags field of poll attr must be 0.\n");
		ret = -FI_EINVAL;
		goto error;
	}

	dom = dom_ftou(fdom);

	ps = calloc(1, sizeof(*ps));
	if (!ps) {
		USDF_WARN_SYS(DOMAIN,
				"unable to allocate memory for poll obj");
		ret = -FI_ENOMEM;
		goto error;
	}

	dlist_init(&ps->list);
	ofi_atomic_initialize32(&ps->poll_refcnt, 0);
	ofi_mutex_init(&ps->lock);

	ps->poll_fid.fid.ops = &usdf_poll_fi_ops;
	ps->poll_fid.fid.fclass = FI_CLASS_POLL;
	ps->poll_fid.fid.context = 0;

	ps->poll_fid.ops = &usdf_poll_ops;

	ps->poll_domain = dom;

	ret = ofi_atomic_inc32(&ps->poll_domain->dom_refcnt);

	USDF_DBG_SYS(DOMAIN,
			"created pollset from domain: [%p] with new refcnt: [%d]\n",
			ps->poll_domain, ret);

	*fps = &ps->poll_fid;

	return FI_SUCCESS;

error:
	*fps = NULL;
	return ret;
}
