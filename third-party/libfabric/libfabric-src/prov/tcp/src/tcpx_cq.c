/*
 * Copyright (c) 2017-2020 Intel Corporation. All rights reserved.
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

#define TCPX_DEF_CQ_SIZE (1024)


void tcpx_cq_progress(struct util_cq *cq)
{
	struct ofi_epollfds_event events[MAX_POLL_EVENTS];
	struct fid_list_entry *fid_entry;
	struct util_wait_fd *wait_fd;
	struct dlist_entry *item;
	struct tcpx_ep *ep;
	struct fid *fid;
	uint32_t inevent, outevent, errevent;
	int nfds, i;

	wait_fd = container_of(cq->wait, struct util_wait_fd, util_wait);

	cq->cq_fastlock_acquire(&cq->ep_list_lock);
	dlist_foreach(&cq->ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		ep = container_of(fid_entry->fid, struct tcpx_ep,
				  util_ep.ep_fid.fid);

		fastlock_acquire(&ep->lock);
		/* We need to progress receives in the case where we're waiting
		 * on the application to post a buffer to consume a receive
		 * that we've already read from the kernel.  If the message is
		 * of length 0, there's no additional data to read, so failing
		 * to progress can result in application hangs.
		 */
		if (ofi_bsock_readable(&ep->bsock) ||
		    (ep->cur_rx.handler && !ep->cur_rx.entry)) {
			assert(ep->state == TCPX_CONNECTED);
			tcpx_progress_rx(ep);
		}

		(void) tcpx_update_epoll(ep);
		fastlock_release(&ep->lock);
	}

	if (wait_fd->util_wait.wait_obj == FI_WAIT_FD) {
		nfds = ofi_epoll_wait(wait_fd->epoll_fd, events,
				      MAX_POLL_EVENTS, 0);
		inevent = POLLIN;
		outevent = POLLOUT;
		errevent = POLLERR;
	} else {
		nfds = ofi_pollfds_wait(wait_fd->pollfds, events,
					MAX_POLL_EVENTS, 0);
		inevent = OFI_EPOLL_IN;
		outevent = OFI_EPOLL_OUT;
		errevent = OFI_EPOLL_ERR;
	}
	if (nfds <= 0)
		goto unlock;

	for (i = 0; i < nfds; i++) {
		fid = events[i].data.ptr;
		if (fid->fclass != FI_CLASS_EP) {
			fd_signal_reset(&wait_fd->signal);
			continue;
		}

		ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);
		fastlock_acquire(&ep->lock);
		if (events[i].events & errevent)
			tcpx_progress_async(ep);
		if (events[i].events & inevent)
			tcpx_progress_rx(ep);
		if (events[i].events & outevent)
			tcpx_progress_tx(ep);
		fastlock_release(&ep->lock);
	}
unlock:
	cq->cq_fastlock_release(&cq->ep_list_lock);
}

static int tcpx_cq_close(struct fid *fid)
{
	int ret;
	struct tcpx_cq *tcpx_cq;

	tcpx_cq = container_of(fid, struct tcpx_cq, util_cq.cq_fid.fid);
	ofi_bufpool_destroy(tcpx_cq->xfer_pool);
	ret = ofi_cq_cleanup(&tcpx_cq->util_cq);
	if (ret)
		return ret;

	free(tcpx_cq);
	return 0;
}

void tcpx_get_cq_info(struct tcpx_xfer_entry *entry, uint64_t *flags,
		      uint64_t *data, uint64_t *tag)
{
	if (entry->hdr.base_hdr.flags & TCPX_REMOTE_CQ_DATA) {
		*data = entry->hdr.cq_data_hdr.cq_data;

		if (entry->hdr.base_hdr.flags & TCPX_TAGGED) {
			*flags |= FI_REMOTE_CQ_DATA | FI_TAGGED;
			*tag = entry->hdr.tag_data_hdr.tag;
		} else {
			*flags |= FI_REMOTE_CQ_DATA;
			*tag = 0;
		}

	} else if (entry->hdr.base_hdr.flags & TCPX_TAGGED) {
		*flags |= FI_TAGGED;
		*data = 0;
		*tag = entry->hdr.tag_hdr.tag;
	} else {
		*data = 0;
		*tag = 0;
	}
}

void tcpx_cq_report_success(struct util_cq *cq,
			    struct tcpx_xfer_entry *xfer_entry)
{
	uint64_t flags, data, tag;
	size_t len;

	if (!(xfer_entry->flags & FI_COMPLETION) ||
	    (xfer_entry->flags & TCPX_INTERNAL_XFER))
		return;

	flags = xfer_entry->flags & ~TCPX_INTERNAL_MASK;
	if (flags & FI_RECV) {
		len = xfer_entry->hdr.base_hdr.size -
		      xfer_entry->hdr.base_hdr.hdr_size;
		tcpx_get_cq_info(xfer_entry, &flags, &data, &tag);
	} else if ((flags & (FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA)) ==
		   (FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA)) {
		len = 0;
		tag = 0;
		data = xfer_entry->hdr.cq_data_hdr.cq_data;
	} else {
		len = 0;
		data = 0;
		tag = 0;
	}

	ofi_cq_write(cq, xfer_entry->context,
		     flags, len, NULL, data, tag);
	if (cq->wait)
		ofi_cq_signal(&cq->cq_fid);
}

void tcpx_cq_report_error(struct util_cq *cq,
			  struct tcpx_xfer_entry *xfer_entry,
			  int err)
{
	struct fi_cq_err_entry err_entry;

	if (xfer_entry->flags & TCPX_INTERNAL_XFER)
		return;

	err_entry.flags = xfer_entry->flags & ~TCPX_INTERNAL_MASK;
	if (err_entry.flags & FI_RECV) {
		tcpx_get_cq_info(xfer_entry, &err_entry.flags, &err_entry.data,
				 &err_entry.tag);
	} else if ((err_entry.flags & (FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA)) ==
		   (FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA)) {
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

	ofi_cq_write_error(cq, &err_entry);
}

static int tcpx_cq_control(struct fid *fid, int command, void *arg)
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

static struct fi_ops tcpx_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = tcpx_cq_close,
	.bind = fi_no_bind,
	.control = tcpx_cq_control,
	.ops_open = fi_no_ops_open,
};

int tcpx_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_fid, void *context)
{
	struct tcpx_cq *tcpx_cq;
	struct fi_cq_attr cq_attr;
	int ret;

	tcpx_cq = calloc(1, sizeof(*tcpx_cq));
	if (!tcpx_cq)
		return -FI_ENOMEM;

	if (!attr->size)
		attr->size = TCPX_DEF_CQ_SIZE;

	ret = ofi_bufpool_create(&tcpx_cq->xfer_pool,
				 sizeof(struct tcpx_xfer_entry), 16, 0,
				 1024, 0);
	if (ret)
		goto free_cq;

	if (attr->wait_obj == FI_WAIT_NONE ||
	    attr->wait_obj == FI_WAIT_UNSPEC) {
		cq_attr = *attr;
		cq_attr.wait_obj = FI_WAIT_POLLFD;
		attr = &cq_attr;
	}

	ret = ofi_cq_init(&tcpx_prov, domain, attr, &tcpx_cq->util_cq,
			  &tcpx_cq_progress, context);
	if (ret)
		goto destroy_pool;

	*cq_fid = &tcpx_cq->util_cq.cq_fid;
	(*cq_fid)->fid.ops = &tcpx_cq_fi_ops;
	return 0;

destroy_pool:
	ofi_bufpool_destroy(tcpx_cq->xfer_pool);
free_cq:
	free(tcpx_cq);
	return ret;
}
