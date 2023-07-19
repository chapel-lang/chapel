/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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
#include "tcpx.h"

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

static void tcpx_rma_read_send_entry_fill(struct tcpx_xfer_entry *send_entry,
					  struct tcpx_xfer_entry *recv_entry,
					  struct tcpx_ep *ep,
					  const struct fi_msg_rma *msg)
{
	struct ofi_rma_iov *rma_iov;
	size_t offset;

	offset = sizeof(send_entry->hdr.base_hdr);
	rma_iov = (struct ofi_rma_iov *) ((uint8_t *) &send_entry->hdr + offset);

	send_entry->hdr.base_hdr.op = ofi_op_read_req;
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
	send_entry->ctrl_flags = TCPX_NEED_RESP;
	send_entry->resp_entry = recv_entry;
}

static void tcpx_rma_read_recv_entry_fill(struct tcpx_xfer_entry *recv_entry,
					  struct tcpx_ep *ep,
					  const struct fi_msg_rma *msg,
					  uint64_t flags)
{
	memcpy(&recv_entry->iov[0], &msg->msg_iov[0],
	       msg->iov_count * sizeof(struct iovec));

	recv_entry->iov_cnt = msg->iov_count;
	recv_entry->ep = ep;
	recv_entry->context = msg->context;
	recv_entry->cq_flags = tcpx_tx_completion_flag(ep, flags) |
			       FI_RMA | FI_READ;
	recv_entry->ctrl_flags = TCPX_INTERNAL_XFER;
}

static ssize_t
tcpx_rma_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct tcpx_ep *ep;
	struct tcpx_cq *cq;
	struct tcpx_xfer_entry *send_entry;
	struct tcpx_xfer_entry *recv_entry;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq,
			       util_cq);

	assert(msg->iov_count <= TCPX_IOV_LIMIT);
	assert(msg->rma_iov_count <= TCPX_IOV_LIMIT);
	assert(ofi_total_iov_len(msg->msg_iov, msg->iov_count) ==
	       ofi_total_rma_iov_len(msg->rma_iov, msg->rma_iov_count));

	send_entry = tcpx_alloc_tx(ep);
	if (!send_entry)
		return -FI_EAGAIN;

	recv_entry = tcpx_alloc_xfer(cq);
	if (!recv_entry) {
		tcpx_free_xfer(cq, send_entry);
		return -FI_EAGAIN;
	}
	tcpx_rma_read_send_entry_fill(send_entry, recv_entry, ep, msg);
	tcpx_rma_read_recv_entry_fill(recv_entry, ep, msg, flags);

	ofi_mutex_lock(&ep->lock);
	slist_insert_tail(&recv_entry->entry, &ep->rma_read_queue);
	tcpx_tx_queue_insert(ep, send_entry);
	ofi_mutex_unlock(&ep->lock);
	return FI_SUCCESS;
}

static ssize_t
tcpx_rma_read(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
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

	return tcpx_rma_readmsg(ep_fid, &msg, 0);
}

static ssize_t
tcpx_rma_readv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
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

	return tcpx_rma_readmsg(ep_fid, &msg, 0);
}

static ssize_t
tcpx_rma_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *send_entry;
	struct ofi_rma_iov *rma_iov;
	uint64_t data_len;
	size_t offset;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	send_entry = tcpx_alloc_tx(ep);
	if (!send_entry)
		return -FI_EAGAIN;

	assert(msg->iov_count <= TCPX_IOV_LIMIT);
	assert(msg->rma_iov_count <= TCPX_IOV_LIMIT);

	data_len = ofi_total_iov_len(msg->msg_iov, msg->iov_count);

	assert(ofi_total_rma_iov_len(msg->rma_iov, msg->rma_iov_count) ==
	       data_len);
	assert(!(flags & FI_INJECT) || (data_len <= TCPX_MAX_INJECT));

	send_entry->hdr.base_hdr.op = ofi_op_write;

	if (flags & FI_REMOTE_CQ_DATA) {
		send_entry->hdr.base_hdr.flags = TCPX_REMOTE_CQ_DATA;
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

	send_entry->cq_flags = tcpx_tx_completion_flag(ep, flags) |
			       FI_RMA | FI_WRITE;
	tcpx_set_commit_flags(send_entry, flags);
	send_entry->context = msg->context;

	ofi_mutex_lock(&ep->lock);
	tcpx_tx_queue_insert(ep, send_entry);
	ofi_mutex_unlock(&ep->lock);
	return FI_SUCCESS;
}

static ssize_t
tcpx_rma_write(struct fid_ep *ep_fid, const void *buf, size_t len, void *desc,
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

	return tcpx_rma_writemsg(ep_fid, &msg, 0);
}

static ssize_t
tcpx_rma_writev(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
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

	return tcpx_rma_writemsg(ep_fid, &msg, 0);
}


static ssize_t
tcpx_rma_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
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

	return tcpx_rma_writemsg(ep_fid, &msg, FI_REMOTE_CQ_DATA);
}

static ssize_t
tcpx_rma_inject_common(struct fid_ep *ep_fid, const void *buf, size_t len,
		       uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		       uint64_t key, uint64_t flags)
{
	struct tcpx_ep *ep;
	struct tcpx_xfer_entry *send_entry;
	struct ofi_rma_iov *rma_iov;
	uint64_t *cq_data;
	size_t offset;

	ep = container_of(ep_fid, struct tcpx_ep, util_ep.ep_fid);
	send_entry = tcpx_alloc_tx(ep);
	if (!send_entry)
		return -FI_EAGAIN;

	assert(len <= TCPX_MAX_INJECT);
	offset = sizeof(send_entry->hdr.base_hdr);

	send_entry->hdr.base_hdr.op = ofi_op_write;

	if (flags & FI_REMOTE_CQ_DATA) {
		send_entry->hdr.base_hdr.flags = TCPX_REMOTE_CQ_DATA;
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

	ofi_mutex_lock(&ep->lock);
	tcpx_tx_queue_insert(ep, send_entry);
	ofi_mutex_unlock(&ep->lock);
	return FI_SUCCESS;
}

static ssize_t
tcpx_rma_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	return tcpx_rma_inject_common(ep_fid, buf, len, 0 ,dest_addr,
				      addr, key, FI_INJECT);
}

static ssize_t
tcpx_rma_injectdata(struct fid_ep *ep_fid, const void *buf, size_t len,
		    uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		    uint64_t key)
{
	return tcpx_rma_inject_common(ep_fid, buf, len, data, dest_addr, addr,
				      key, FI_INJECT | FI_REMOTE_CQ_DATA);
}

struct fi_ops_rma tcpx_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = tcpx_rma_read,
	.readv = tcpx_rma_readv,
	.readmsg = tcpx_rma_readmsg,
	.write = tcpx_rma_write,
	.writev = tcpx_rma_writev,
	.writemsg = tcpx_rma_writemsg,
	.inject = tcpx_rma_inject,
	.writedata = tcpx_rma_writedata,
	.injectdata = tcpx_rma_injectdata,
};
