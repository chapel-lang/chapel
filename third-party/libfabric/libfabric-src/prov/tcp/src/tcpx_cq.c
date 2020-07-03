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
	void *wait_contexts[MAX_POLL_EVENTS];
	struct fid_list_entry *fid_entry;
	struct util_wait_fd *wait_fd;
	struct dlist_entry *item;
	struct tcpx_ep *ep;
	struct fid *fid;
	int nfds, i;

	wait_fd = container_of(cq->wait, struct util_wait_fd, util_wait);

	cq->cq_fastlock_acquire(&cq->ep_list_lock);
	dlist_foreach(&cq->ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		ep = container_of(fid_entry->fid, struct tcpx_ep,
				  util_ep.ep_fid.fid);
		tcpx_try_func(&ep->util_ep);
		fastlock_acquire(&ep->lock);
		tcpx_progress_tx(ep);
		if (ep->stage_buf.off != ep->stage_buf.len)
			tcpx_progress_rx(ep);
		fastlock_release(&ep->lock);
	}

	nfds = (wait_fd->util_wait.wait_obj == FI_WAIT_FD) ?
	       ofi_epoll_wait(wait_fd->epoll_fd, wait_contexts,
			      MAX_POLL_EVENTS, 0) :
	       ofi_pollfds_wait(wait_fd->pollfds, wait_contexts,
				MAX_POLL_EVENTS, 0);
	if (nfds <= 0)
		goto unlock;

	for (i = 0; i < nfds; i++) {
		fid = wait_contexts[i];
		if (fid->fclass != FI_CLASS_EP) {
			fd_signal_reset(&wait_fd->signal);
			continue;
		}

		ep = container_of(fid, struct tcpx_ep, util_ep.ep_fid.fid);
		fastlock_acquire(&ep->lock);
		tcpx_progress_rx(ep);
		fastlock_release(&ep->lock);
	}
unlock:
	cq->cq_fastlock_release(&cq->ep_list_lock);
}

static void tcpx_buf_pools_destroy(struct tcpx_buf_pool *buf_pools)
{
	int i;

	for (i = 0; i < TCPX_OP_CODE_MAX; i++)
		ofi_bufpool_destroy(buf_pools[i].pool);
}

static int tcpx_cq_close(struct fid *fid)
{
	int ret;
	struct tcpx_cq *tcpx_cq;

	tcpx_cq = container_of(fid, struct tcpx_cq, util_cq.cq_fid.fid);
	tcpx_buf_pools_destroy(tcpx_cq->buf_pools);
	ret = ofi_cq_cleanup(&tcpx_cq->util_cq);
	if (ret)
		return ret;

	free(tcpx_cq);
	return 0;
}

struct tcpx_xfer_entry *tcpx_xfer_entry_alloc(struct tcpx_cq *tcpx_cq,
					      enum tcpx_xfer_op_codes type)
{
	struct tcpx_xfer_entry *xfer_entry;

	tcpx_cq->util_cq.cq_fastlock_acquire(&tcpx_cq->util_cq.cq_lock);
	if (!ofi_cirque_isfull(tcpx_cq->util_cq.cirq))
		xfer_entry = ofi_buf_alloc(tcpx_cq->buf_pools[type].pool);
	else
		xfer_entry = NULL;
	tcpx_cq->util_cq.cq_fastlock_release(&tcpx_cq->util_cq.cq_lock);

	return xfer_entry;
}

void tcpx_xfer_entry_release(struct tcpx_cq *tcpx_cq,
			     struct tcpx_xfer_entry *xfer_entry)
{
	if (xfer_entry->ep->cur_rx_entry == xfer_entry)
		xfer_entry->ep->cur_rx_entry = NULL;

	xfer_entry->hdr.base_hdr.flags = 0;

	xfer_entry->flags = 0;
	xfer_entry->context = 0;
	xfer_entry->rem_len = 0;

	tcpx_cq->util_cq.cq_fastlock_acquire(&tcpx_cq->util_cq.cq_lock);
	ofi_buf_free(xfer_entry);
	tcpx_cq->util_cq.cq_fastlock_release(&tcpx_cq->util_cq.cq_lock);
}

void tcpx_cq_report_success(struct util_cq *cq,
			    struct tcpx_xfer_entry *xfer_entry)
{
	uint64_t data = 0;
	uint64_t flags = 0;
	void *buf = NULL;
	size_t len = 0;

	flags = xfer_entry->flags;

	if (!(flags & FI_COMPLETION))
		return;

	len = xfer_entry->hdr.base_hdr.size -
	      xfer_entry->hdr.base_hdr.payload_off;

	if (xfer_entry->hdr.base_hdr.flags & OFI_REMOTE_CQ_DATA) {
		flags |= FI_REMOTE_CQ_DATA;
		data = xfer_entry->hdr.cq_data_hdr.cq_data;
	}

	ofi_cq_write(cq, xfer_entry->context,
		     flags, len, buf, data, 0);
	if (cq->wait)
		ofi_cq_signal(&cq->cq_fid);
}

void tcpx_cq_report_error(struct util_cq *cq,
			  struct tcpx_xfer_entry *xfer_entry,
			  int err)
{
	struct fi_cq_err_entry err_entry;
	uint64_t data = 0;

	if (xfer_entry->hdr.base_hdr.flags & OFI_REMOTE_CQ_DATA) {
		xfer_entry->flags |= FI_REMOTE_CQ_DATA;
		data = xfer_entry->hdr.cq_data_hdr.cq_data;
	}

	err_entry.op_context = xfer_entry->context;
	err_entry.flags = xfer_entry->flags;
	err_entry.len = 0;
	err_entry.buf = NULL;
	err_entry.data = data;
	err_entry.tag = 0;
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

static void tcpx_buf_pool_init(struct ofi_bufpool_region *region, void *buf)
{
	struct tcpx_buf_pool *pool = region->pool->attr.context;
	struct tcpx_xfer_entry *xfer_entry = buf;

	xfer_entry->hdr.base_hdr.version = TCPX_HDR_VERSION;
	xfer_entry->hdr.base_hdr.op_data = pool->op_type;

	switch (pool->op_type) {
	case TCPX_OP_MSG_RECV:
	case TCPX_OP_MSG_SEND:
	case TCPX_OP_MSG_RESP:
		xfer_entry->hdr.base_hdr.op = ofi_op_msg;
		break;
	case TCPX_OP_WRITE:
	case TCPX_OP_REMOTE_WRITE:
		xfer_entry->hdr.base_hdr.op = ofi_op_write;
		break;
	case TCPX_OP_READ_REQ:
		xfer_entry->hdr.base_hdr.op = ofi_op_read_req;
		break;
	case TCPX_OP_READ_RSP:
		xfer_entry->hdr.base_hdr.op = ofi_op_read_rsp;
		break;
	case TCPX_OP_REMOTE_READ:
		break;
	default:
		assert(0);
		break;
	}
}

static int tcpx_buf_pools_create(struct tcpx_buf_pool *buf_pools)
{
	int i, ret;
	struct ofi_bufpool_attr attr = {
		.size = sizeof(struct tcpx_xfer_entry),
		.alignment = 16,
		.chunk_cnt = 1024,
		.init_fn = tcpx_buf_pool_init,
		.flags = OFI_BUFPOOL_HUGEPAGES,
	};

	for (i = 0; i < TCPX_OP_CODE_MAX; i++) {
		buf_pools[i].op_type = i;

		attr.context = &buf_pools[i];
		ret = ofi_bufpool_create_attr(&attr, &buf_pools[i].pool);
		if (ret) {
			FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,
				"Unable to create buf pool\n");
			goto err;
		}
	}
	return 0;

err:
	while (i--)
		ofi_bufpool_destroy(buf_pools[i].pool);

	return -ret;
}

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

	ret = tcpx_buf_pools_create(tcpx_cq->buf_pools);
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
	tcpx_buf_pools_destroy(tcpx_cq->buf_pools);
free_cq:
	free(tcpx_cq);
	return ret;
}
