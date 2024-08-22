/*
 * Copyright (c) 2017-2022 Intel Corporation. All rights reserved.
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

#include "xnet.h"

#define XNET_DEF_CQ_SIZE (1024)


static ssize_t
xnet_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count,
		 fi_addr_t *src_addr)
{
	struct xnet_cq *cq;
	ssize_t ret;

	cq = container_of(cq_fid, struct xnet_cq, util_cq.cq_fid);
	ofi_genlock_lock(xnet_cq2_progress(cq)->active_lock);
	ret = ofi_cq_readfrom(cq_fid, buf, count, src_addr);
	ofi_genlock_unlock(xnet_cq2_progress(cq)->active_lock);
	return ret;
}

static ssize_t
xnet_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *buf,
		uint64_t flags)
{
	struct xnet_cq *cq;
	ssize_t ret;

	cq = container_of(cq_fid, struct xnet_cq, util_cq.cq_fid);
	ofi_genlock_lock(xnet_cq2_progress(cq)->active_lock);
	ret = ofi_cq_readerr(cq_fid, buf, flags);
	ofi_genlock_unlock(xnet_cq2_progress(cq)->active_lock);
	return ret;
}

static struct fi_ops_cq xnet_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = xnet_cq_readfrom,
	.readerr = xnet_cq_readerr,
	.sread = ofi_cq_sread,
	.sreadfrom = ofi_cq_sreadfrom,
	.signal = ofi_cq_signal,
	.strerror = ofi_cq_strerror,
};

static void xnet_cq_progress(struct util_cq *util_cq)
{
	struct xnet_cq *cq;
	cq = container_of(util_cq, struct xnet_cq, util_cq);
	xnet_run_progress(xnet_cq2_progress(cq), false);
}

static int xnet_cq_close(struct fid *fid)
{
	int ret;
	struct xnet_cq *cq;

	cq = container_of(fid, struct xnet_cq, util_cq.cq_fid.fid);
	ret = ofi_cq_cleanup(&cq->util_cq);
	if (ret)
		return ret;

	free(cq);
	return 0;
}

static void xnet_get_cq_info(struct xnet_xfer_entry *entry, uint64_t *flags,
			     uint64_t *data, uint64_t *tag)
{
	if (entry->hdr.base_hdr.flags & XNET_REMOTE_CQ_DATA) {
		*data = entry->hdr.cq_data_hdr.cq_data;

		if (entry->hdr.base_hdr.op == xnet_op_tag ||
		    entry->hdr.base_hdr.op == xnet_op_tag_rts) {
			*flags |= FI_REMOTE_CQ_DATA | FI_TAGGED;
			*tag = entry->hdr.tag_data_hdr.tag;
		} else {
			*flags |= FI_REMOTE_CQ_DATA;
			*tag = 0;
		}

	} else if (entry->hdr.base_hdr.op == xnet_op_tag ||
		   entry->hdr.base_hdr.op == xnet_op_tag_rts) {
		*flags |= FI_TAGGED;
		*data = 0;
		*tag = entry->hdr.tag_hdr.tag;
	} else {
		*data = 0;
		*tag = 0;
	}
}

void xnet_report_success(struct xnet_xfer_entry *xfer_entry)
{
	struct util_cq *cq;
	uint64_t flags, data, tag;
	size_t len;

	if (xfer_entry->ctrl_flags & (XNET_INTERNAL_XFER | XNET_SAVED_XFER))
		return;

	if (xfer_entry->cntr)
		ofi_cntr_inc(xfer_entry->cntr);

	if (!(xfer_entry->cq_flags & FI_COMPLETION))
		return;

	assert(xfer_entry->cq);
	cq = &xfer_entry->cq->util_cq;
	if (xfer_entry->ctrl_flags & XNET_COPY_RECV) {
		xfer_entry->ctrl_flags &= ~XNET_COPY_RECV;
		/* TODO: io_uring support, see comment in xnet_recv_saved() */
		xnet_complete_saved(xfer_entry, &xfer_entry->msg_data);
		return;
	}

	flags = xfer_entry->cq_flags & ~FI_COMPLETION;
	if (flags & FI_RECV) {
		len = xnet_msg_len(&xfer_entry->hdr);
		if (xfer_entry->ctrl_flags & XNET_MULTI_RECV &&
		    xfer_entry->mrecv) {
			xfer_entry->mrecv->ref_cnt--;
			if (!xfer_entry->mrecv->ref_cnt) {
				flags |= FI_MULTI_RECV;
				free(xfer_entry->mrecv);
			}
		}
		xnet_get_cq_info(xfer_entry, &flags, &data, &tag);
	} else if (flags & FI_REMOTE_CQ_DATA) {
		assert(flags & FI_REMOTE_WRITE);
		len = xnet_msg_len(&xfer_entry->hdr);
		tag = 0;
		data = xfer_entry->hdr.cq_data_hdr.cq_data;
	} else {
		len = 0;
		data = 0;
		tag = 0;
	}

	if (cq->src) {
		ofi_cq_write_src(cq, xfer_entry->context, flags, len,
				 xfer_entry->user_buf, data, tag,
				 xfer_entry->src_addr);
	} else {
		ofi_cq_write(cq, xfer_entry->context, flags, len,
			     xfer_entry->user_buf, data, tag);
	}
	if (cq->wait)
		cq->wait->signal(cq->wait);
}

void xnet_report_error(struct xnet_xfer_entry *xfer_entry, int err)
{
	struct fi_cq_err_entry err_entry;

	if (xfer_entry->ctrl_flags &
	    (XNET_INTERNAL_XFER | XNET_SAVED_XFER | XNET_INJECT_OP)) {
		if (xfer_entry->ctrl_flags &
		    (XNET_INTERNAL_XFER | XNET_SAVED_XFER)) {
			FI_WARN(&xnet_prov, FI_LOG_CQ, "internal/saved transfer "
				"failed (%s)\n", fi_strerror(err));
		} else {
			FI_WARN(&xnet_prov, FI_LOG_CQ, "inject transfer "
				"failed (%s)\n", fi_strerror(err));
		}
		return;
	}

	err_entry.flags = xfer_entry->cq_flags & ~FI_COMPLETION;
	if (err_entry.flags & FI_RECV) {
		if (xfer_entry->ctrl_flags & XNET_MULTI_RECV &&
		    xfer_entry->mrecv) {
			xfer_entry->mrecv->ref_cnt--;
			if (!xfer_entry->mrecv->ref_cnt) {
				err_entry.flags |= FI_MULTI_RECV;
				free(xfer_entry->mrecv);
			}
		}
		xnet_get_cq_info(xfer_entry, &err_entry.flags, &err_entry.data,
				 &err_entry.tag);
	} else if (err_entry.flags & FI_REMOTE_CQ_DATA) {
		assert(err_entry.flags & FI_REMOTE_WRITE);
		err_entry.tag = 0;
		err_entry.data = xfer_entry->hdr.cq_data_hdr.cq_data;
	} else {
		err_entry.data = 0;
		err_entry.tag = 0;
	}

	err_entry.op_context = xfer_entry->context;
	err_entry.len = 0;
	err_entry.buf = NULL;
	err_entry.olen = 0;
	err_entry.err = err;
	err_entry.prov_errno = ofi_sockerr();
	err_entry.err_data = NULL;
	err_entry.err_data_size = 0;

	ofi_cq_write_error(&xfer_entry->cq->util_cq, &err_entry);
}

static int xnet_cq_control(struct fid *fid, int command, void *arg)
{
	struct util_cq *cq;
	int ret;

	cq = container_of(fid, struct util_cq, cq_fid.fid);

	switch(command) {
	case FI_GETWAIT:
	case FI_GETWAITOBJ:
		if (!cq->wait)
			return -FI_ENODATA;

		ret = fi_control(&cq->wait->wait_fid.fid, command, arg);
		break;
	default:
		return -FI_ENOSYS;
	}

	return ret;
}

static struct fi_ops xnet_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = xnet_cq_close,
	.bind = fi_no_bind,
	.control = xnet_cq_control,
	.ops_open = fi_no_ops_open,
};

static int xnet_cq_wait_try_func(void *arg)
{
	OFI_UNUSED(arg);
	return FI_SUCCESS;
}

int xnet_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_fid, void *context)
{
	struct xnet_cq *cq;
	struct fi_cq_attr cq_attr;
	int ret;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	if (!attr->size)
		attr->size = XNET_DEF_CQ_SIZE;

	if (attr->wait_obj == FI_WAIT_UNSPEC) {
		cq_attr = *attr;
		cq_attr.wait_obj = FI_WAIT_FD;
		attr = &cq_attr;
	}

	ret = ofi_cq_init(&xnet_prov, domain, attr, &cq->util_cq,
			  &xnet_cq_progress, context);
	if (ret)
		goto free_cq;

	if (cq->util_cq.wait && ofi_have_epoll) {
		ret = ofi_wait_add_fd(cq->util_cq.wait,
			       ofi_dynpoll_get_fd(&xnet_cq2_progress(cq)->epoll_fd),
			       POLLIN, xnet_cq_wait_try_func, cq,
			       &cq->util_cq.cq_fid);
		if (ret)
			goto cleanup;
	}

	*cq_fid = &cq->util_cq.cq_fid;
	(*cq_fid)->fid.ops = &xnet_cq_fi_ops;
	(*cq_fid)->ops = &xnet_cq_ops;
	return 0;

cleanup:
	ofi_cq_cleanup(&cq->util_cq);
free_cq:
	free(cq);
	return ret;
}


static void xnet_cntr_progress(struct util_cntr *cntr)
{
	xnet_progress(xnet_cntr2_progress(cntr), false);
}

void xnet_cntr_incerr(struct xnet_xfer_entry *xfer_entry)
{
	if (!xfer_entry->cntr ||
	    xfer_entry->ctrl_flags & (XNET_INTERNAL_XFER | XNET_SAVED_XFER))
		return;

	fi_cntr_adderr(&xfer_entry->cntr->cntr_fid, 1);
}

static uint64_t xnet_cntr_read(struct fid_cntr *cntr_fid)
{
	struct util_cntr *cntr;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	xnet_progress(xnet_cntr2_progress(cntr), false);
	return ofi_atomic_get64(&cntr->cnt);
}

static uint64_t xnet_cntr_readerr(struct fid_cntr *cntr_fid)
{
	struct util_cntr *cntr;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	xnet_progress(xnet_cntr2_progress(cntr), false);
	return ofi_atomic_get64(&cntr->err);
}

static int xnet_cntr_add(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	ofi_atomic_add64(&cntr->cnt, value);
	/* No need to signal, see comment above xnet_cntr_ops */
	return FI_SUCCESS;
}

static int xnet_cntr_adderr(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	ofi_atomic_add64(&cntr->err, value);
	/* No need to signal, see comment above xnet_cntr_ops */
	return FI_SUCCESS;
}

static int xnet_cntr_set(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	ofi_atomic_set64(&cntr->cnt, value);
	/* No need to signal, see comment above xnet_cntr_ops */
	return FI_SUCCESS;
}

static int xnet_cntr_seterr(struct fid_cntr *cntr_fid, uint64_t value)
{
	struct util_cntr *cntr;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	ofi_atomic_set64(&cntr->err, value);
	/* No need to signal, see comment above xnet_cntr_ops */
	return FI_SUCCESS;
}

static int
xnet_cntr_wait(struct fid_cntr *cntr_fid, uint64_t threshold, int timeout)
{
	struct util_cntr *cntr;
	uint64_t endtime, errcnt;
	int ret;

	cntr = container_of(cntr_fid, struct util_cntr, cntr_fid);
	errcnt = xnet_cntr_readerr(cntr_fid);
	endtime = ofi_timeout_time(timeout);

	do {
		if (threshold <= (uint64_t) ofi_atomic_get64(&cntr->cnt))
			return FI_SUCCESS;

		if (errcnt != (uint64_t) ofi_atomic_get64(&cntr->err))
			return -FI_EAVAIL;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_ETIMEDOUT;

		ret = xnet_progress_wait(xnet_cntr2_progress(cntr), timeout);
		if (ret < 0)
			break;

		xnet_progress(xnet_cntr2_progress(cntr), true);
	} while (true);

	return ret;
}

/* We use these calls if the app uses a single thread around any access
 * to the counter (e.g. FI_THREAD_DOMAIN).  There should be no other
 * threads updating the counter or waiting for a value in parallel with
 * these calls.
 */
static struct fi_ops_cntr xnet_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = xnet_cntr_read,
	.readerr = xnet_cntr_readerr,
	.add = xnet_cntr_add,
	.adderr = xnet_cntr_adderr,
	.set = xnet_cntr_set,
	.seterr = xnet_cntr_seterr,
	.wait = xnet_cntr_wait
};


static int xnet_cntr_wait_try_func(void *arg)
{
	OFI_UNUSED(arg);
	return FI_SUCCESS;
}

int xnet_cntr_open(struct fid_domain *fid_domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr_fid, void *context)
{
	struct xnet_progress *progress;
	struct xnet_domain *domain;
	struct util_cntr *cntr;
	struct fi_cntr_attr cntr_attr;
	int ret;

	cntr = calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	domain = container_of(fid_domain, struct xnet_domain,
			      util_domain.domain_fid);
	if (attr->wait_obj == FI_WAIT_UNSPEC) {
		cntr_attr = *attr;
		if (domain->progress.auto_progress ||
		    domain->util_domain.threading != FI_THREAD_DOMAIN) {
			cntr_attr.wait_obj = FI_WAIT_FD;
		} else {
			/* We can wait on the progress allfds */
			cntr_attr.wait_obj = FI_WAIT_NONE;
		}
		attr = &cntr_attr;
	}

	ret = ofi_cntr_init(&xnet_prov, fid_domain, attr, cntr,
			    &xnet_cntr_progress, context);
	if (ret)
		goto free;

	if (attr->wait_obj == FI_WAIT_NONE) {
		cntr->cntr_fid.ops = &xnet_cntr_ops;
	} else {
		progress = xnet_cntr2_progress(cntr);
		if (attr->wait_obj == FI_WAIT_FD && ofi_have_epoll) {
			ret = ofi_wait_add_fd(cntr->wait,
					ofi_dynpoll_get_fd(&progress->epoll_fd),
					POLLIN, xnet_cntr_wait_try_func, NULL,
					&cntr->cntr_fid);
		} else {
			ret = xnet_start_progress(progress);
		}
		if (ret)
			goto cleanup;
	}

	*cntr_fid = &cntr->cntr_fid;
	return FI_SUCCESS;

cleanup:
	ofi_cntr_cleanup(cntr);
free:
	free(cntr);
	return ret;
}
