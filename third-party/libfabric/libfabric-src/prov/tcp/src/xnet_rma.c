/*
 * Copyright (c) 2018-2022 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <rdma/fi_errno.h>
#include "rdma/fi_eq.h"
#include "ofi_iov.h"
#include <ofi_prov.h>
#include "xnet.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <ofi_util.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netdb.h>

static void xnet_rma_read_send_entry_fill(struct xnet_xfer_entry *send_entry,
					  struct xnet_xfer_entry *recv_entry,
					  struct xnet_ep *ep,
					  const struct fi_msg_rma *msg)
{
	struct ofi_rma_iov *rma_iov;
	size_t offset;

	offset = sizeof(send_entry->hdr.base_hdr);
	rma_iov = (struct ofi_rma_iov *) ((uint8_t *) &send_entry->hdr + offset);

	send_entry->hdr.base_hdr.op = xnet_op_read_req;
	send_entry->hdr.base_hdr.rma_iov_cnt = (uint8_t) msg->rma_iov_count;
	memcpy(rma_iov, msg->rma_iov,
	       msg->rma_iov_count * sizeof(msg->rma_iov[0]));

	offset += (msg->rma_iov_count * sizeof(*rma_iov));

	send_entry->hdr.base_hdr.size = offset;
	send_entry->hdr.base_hdr.hdr_size = (uint8_t) offset;

	send_entry->iov[0].iov_base = (void *) &send_entry->hdr;
	send_entry->iov[0].iov_len = offset;
	send_entry->iov_cnt = 1;
	send_entry->context = msg->context;
	send_entry->ctrl_flags = XNET_NEED_RESP;
	send_entry->resp_entry = recv_entry;

	/* Read request generates a completion on error */
	send_entry->cntr = ep->util_ep.cntrs[CNTR_RD];
	send_entry->cq = xnet_ep_tx_cq(ep);
}

static void xnet_rma_read_recv_entry_fill(struct xnet_xfer_entry *recv_entry,
					  struct xnet_ep *ep,
					  const struct fi_msg_rma *msg,
					  uint64_t flags)
{
	memcpy(&recv_entry->iov[0], &msg->msg_iov[0],
	       msg->iov_count * sizeof(struct iovec));

	recv_entry->iov_cnt = msg->iov_count;
	recv_entry->context = msg->context;
	recv_entry->cq_flags = xnet_tx_completion_get_msgflags(ep, flags) |
			       FI_RMA | FI_READ;

	/* Read response completes the RMA read transmit */
	recv_entry->cntr = ep->util_ep.cntrs[CNTR_RD];
	recv_entry->cq = xnet_ep_tx_cq(ep);
	/* Read response is marked as internal until the request completes
	 * successfully.  This way we won't generate 2 completion to the
	 * app in case the read request fails.  Only the request will
	 * generate the completion. */
	recv_entry->ctrl_flags = XNET_INTERNAL_XFER;
}

static ssize_t
xnet_rma_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct xnet_xfer_entry *send_entry;
	struct xnet_xfer_entry *recv_entry;
	struct xnet_ep *ep;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	assert(msg->iov_count <= XNET_IOV_LIMIT);
	assert(msg->rma_iov_count <= XNET_IOV_LIMIT);
	assert(ofi_total_iov_len(msg->msg_iov, msg->iov_count) ==
	       ofi_total_rma_iov_len(msg->rma_iov, msg->rma_iov_count));

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	send_entry = xnet_alloc_tx(ep);
	if (!send_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	recv_entry = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (!recv_entry) {
		xnet_free_xfer(xnet_ep2_progress(ep), send_entry);
		ret = -FI_EAGAIN;
		goto unlock;
	}
	xnet_rma_read_send_entry_fill(send_entry, recv_entry, ep, msg);
	xnet_rma_read_recv_entry_fill(recv_entry, ep, msg, flags);

	slist_insert_tail(&recv_entry->entry, &ep->rma_read_queue);
	xnet_tx_queue_insert(ep, send_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_rma_read(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
	      fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &msg_iov,
		.desc = &desc,
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = src_addr,
		.context = context,
		.data = 0,
	};

	struct xnet_ep *ep;
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	return xnet_rma_readmsg(ep_fid, &msg, ep->util_ep.tx_op_flags);
}

static ssize_t
xnet_rma_readv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
	       size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	       void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = ofi_total_iov_len(iov, count),
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = src_addr,
		.context = context,
		.data = 0,
	};

	struct xnet_ep *ep;
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	return xnet_rma_readmsg(ep_fid, &msg, ep->util_ep.tx_op_flags);
}

static ssize_t
xnet_rma_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *send_entry;
	struct ofi_rma_iov *rma_iov;
	uint64_t data_len;
	size_t offset;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	send_entry = xnet_alloc_tx(ep);
	if (!send_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	assert(msg->iov_count <= XNET_IOV_LIMIT);
	assert(msg->rma_iov_count <= XNET_IOV_LIMIT);

	data_len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);

	assert(ofi_total_rma_iov_len(msg->rma_iov, msg->rma_iov_count) ==
	       data_len);
	assert(!(flags & FI_INJECT) || (data_len <= xnet_max_inject));

	send_entry->hdr.base_hdr.op = xnet_op_write;

	if (flags & FI_REMOTE_CQ_DATA) {
		send_entry->hdr.base_hdr.flags = XNET_REMOTE_CQ_DATA;
		send_entry->hdr.cq_data_hdr.cq_data = msg->data;
		offset = sizeof(send_entry->hdr.cq_data_hdr);
	} else {
		offset = sizeof(send_entry->hdr.base_hdr);
	}

	rma_iov = (struct ofi_rma_iov *)((uint8_t *)&send_entry->hdr + offset);
	memcpy(rma_iov, msg->rma_iov,
	       msg->rma_iov_count * sizeof(msg->rma_iov[0]));
	send_entry->hdr.base_hdr.rma_iov_cnt = (uint8_t) msg->rma_iov_count;

	offset += (send_entry->hdr.base_hdr.rma_iov_cnt * sizeof(*rma_iov));

	send_entry->hdr.base_hdr.hdr_size = (uint8_t) offset;
	send_entry->hdr.base_hdr.size = data_len + offset;
	if (flags & FI_INJECT) {
		ofi_copy_iov_buf(msg->msg_iov, msg->iov_count, 0,
				 (uint8_t *)&send_entry->hdr + offset,
				 data_len,
				 OFI_COPY_IOV_TO_BUF);
		send_entry->iov_cnt = 1;
		offset += data_len;
	} else {
		memcpy(&send_entry->iov[1], &msg->msg_iov[0],
		       msg->iov_count * sizeof(struct iovec));
		send_entry->iov_cnt = msg->iov_count + 1;
	}

	send_entry->iov[0].iov_base = (void *) &send_entry->hdr;
	send_entry->iov[0].iov_len = offset;

	send_entry->cq_flags = xnet_tx_completion_get_msgflags(ep, flags) |
			       FI_RMA | FI_WRITE;
	send_entry->cntr = ep->util_ep.cntrs[CNTR_WR];
	xnet_set_commit_flags(send_entry, flags);
	send_entry->context = msg->context;

	xnet_tx_queue_insert(ep, send_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_rma_write(struct fid_ep *ep_fid, const void *buf, size_t len, void *desc,
	       fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &msg_iov,
		.desc = &desc,
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = dest_addr,
		.context = context,
		.data = 0,
	};

	struct xnet_ep *ep;
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	return xnet_rma_writemsg(ep_fid, &msg, ep->util_ep.tx_op_flags);
}

static ssize_t
xnet_rma_writev(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = ofi_total_iov_len(iov, count),
	};
	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.addr = dest_addr,
		.context = context,
		.data = 0,
	};

	struct xnet_ep *ep;
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	return xnet_rma_writemsg(ep_fid, &msg, ep->util_ep.tx_op_flags);
}


static ssize_t
xnet_rma_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
		   void *desc, uint64_t data, fi_addr_t dest_addr,
		   uint64_t addr, uint64_t key, void *context)
{
	struct iovec msg_iov = {
		.iov_base = (void *)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.key = key,
		.len = len,
	};
	struct fi_msg_rma msg = {
		.desc = &desc,
		.iov_count = 1,
		.rma_iov_count = 1,
		.rma_iov = &rma_iov,
		.msg_iov = &msg_iov,
		.addr = dest_addr,
		.context = context,
		.data = data,
	};

	struct xnet_ep *ep;
	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);
	return xnet_rma_writemsg(ep_fid, &msg, FI_REMOTE_CQ_DATA | ep->util_ep.tx_op_flags);
}

static ssize_t
xnet_rma_inject_common(struct fid_ep *ep_fid, const void *buf, size_t len,
		       uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		       uint64_t key, uint64_t flags)
{
	struct xnet_ep *ep;
	struct xnet_xfer_entry *send_entry;
	struct ofi_rma_iov *rma_iov;
	uint64_t *cq_data;
	size_t offset;
	ssize_t ret = 0;

	ep = container_of(ep_fid, struct xnet_ep, util_ep.ep_fid);

	ofi_genlock_lock(&xnet_ep2_progress(ep)->ep_lock);
	send_entry = xnet_alloc_tx(ep);
	if (!send_entry) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	assert(len <= xnet_max_inject);
	offset = sizeof(send_entry->hdr.base_hdr);

	send_entry->hdr.base_hdr.op = xnet_op_write;

	if (flags & FI_REMOTE_CQ_DATA) {
		send_entry->hdr.base_hdr.flags = XNET_REMOTE_CQ_DATA;
		cq_data = (uint64_t *)((uint8_t *)&send_entry->hdr + offset);
		*cq_data = data;
		offset += sizeof(data);
	}

	rma_iov = (struct ofi_rma_iov *)((uint8_t *)&send_entry->hdr + offset);
	rma_iov[0].addr = addr;
	rma_iov[0].key = key;
	rma_iov[0].len = len;
	send_entry->hdr.base_hdr.rma_iov_cnt = 1;
	offset += sizeof(*rma_iov);

	send_entry->hdr.base_hdr.hdr_size = (uint8_t) offset;
	memcpy((uint8_t *)&send_entry->hdr + offset, (uint8_t *)buf, len);
	offset += len;

	send_entry->iov[0].iov_base = (void *) &send_entry->hdr;
	send_entry->iov[0].iov_len = offset;
	send_entry->iov_cnt = 1;

	send_entry->hdr.base_hdr.size = offset;
	send_entry->cq_flags = FI_INJECT | FI_WRITE;
	send_entry->cntr = ep->util_ep.cntrs[CNTR_WR];
	xnet_tx_queue_insert(ep, send_entry);
unlock:
	ofi_genlock_unlock(&xnet_ep2_progress(ep)->ep_lock);
	return ret;
}

static ssize_t
xnet_rma_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	return xnet_rma_inject_common(ep_fid, buf, len, 0 ,dest_addr,
				      addr, key, FI_INJECT);
}

static ssize_t
xnet_rma_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		    uint64_t key)
{
	return xnet_rma_inject_common(ep_fid, buf, len, data, dest_addr, addr,
				      key, FI_INJECT | FI_REMOTE_CQ_DATA);
}

struct fi_ops_rma xnet_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = xnet_rma_read,
	.readv = xnet_rma_readv,
	.readmsg = xnet_rma_readmsg,
	.write = xnet_rma_write,
	.writev = xnet_rma_writev,
	.writemsg = xnet_rma_writemsg,
	.inject = xnet_rma_inject,
	.writedata = xnet_rma_writedata,
	.injectdata = xnet_rma_injectdata,
};
