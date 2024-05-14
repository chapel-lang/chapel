/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
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
#include <ofi_mem.h>
#include <ofi_iov.h>
#include "rxd.h"

static struct rxd_x_entry *rxd_tx_entry_init_rma(struct rxd_ep *ep, fi_addr_t addr,
				uint32_t op, const struct iovec *iov, size_t iov_count,
				uint64_t data, uint32_t flags, void *context,
				const struct fi_rma_iov *rma_iov, size_t rma_count)
{
	struct rxd_x_entry *tx_entry;
	struct rxd_domain *rxd_domain = rxd_ep_domain(ep);
	size_t max_inline;
	struct rxd_base_hdr *base_hdr;
	void *ptr;

	tx_entry = rxd_tx_entry_init_common(ep, addr, op, iov, iov_count, 0,
					    data, flags, context, &base_hdr, &ptr);
	if (!tx_entry)
		return NULL;

	if (tx_entry->cq_entry.flags & FI_READ) {
		tx_entry->num_segs = ofi_div_ceil(tx_entry->cq_entry.len,
						  rxd_domain->max_seg_sz);
		rxd_init_sar_hdr(&ptr, tx_entry, rma_count);
		rxd_init_rma_hdr(&ptr, rma_iov, rma_count);
	} else {
		max_inline = rxd_domain->max_inline_msg;
		max_inline -= sizeof(struct ofi_rma_iov) * rma_count;
		rxd_check_init_cq_data(&ptr, tx_entry, &max_inline);

		if (rma_count > 1 || tx_entry->cq_entry.len > max_inline) {
			max_inline -= sizeof(struct rxd_sar_hdr);
			tx_entry->num_segs = ofi_div_ceil(tx_entry->cq_entry.len -
				max_inline, rxd_domain->max_seg_sz) + 1;
			rxd_init_sar_hdr(&ptr, tx_entry, rma_count);
		} else {
			tx_entry->flags |= RXD_INLINE;
			base_hdr->flags = (uint16_t) tx_entry->flags;
			tx_entry->num_segs = 1;
		}
		rxd_init_rma_hdr(&ptr, rma_iov, rma_count);
		tx_entry->bytes_done = rxd_init_msg(&ptr, tx_entry->iov,
			tx_entry->iov_count, tx_entry->cq_entry.len,
			max_inline);
	}

	tx_entry->pkt->pkt_size = rxd_pkt_size(ep, base_hdr, ptr);

	return tx_entry;
}

static ssize_t rxd_generic_write_inject(struct rxd_ep *rxd_ep,
		const struct iovec *iov, size_t iov_count,
		const struct fi_rma_iov *rma_iov, size_t rma_count,
		fi_addr_t addr, void *context, uint32_t op, uint64_t data,
		uint32_t rxd_flags)
{
	struct rxd_x_entry *tx_entry;
	fi_addr_t rxd_addr;
	ssize_t ret = -FI_EAGAIN;

	assert(iov_count <= RXD_IOV_LIMIT && rma_count <= RXD_IOV_LIMIT);
	assert(ofi_total_iov_len(iov, iov_count) <= (size_t) rxd_ep_domain(rxd_ep)->max_inline_rma);

	ofi_genlock_lock(&rxd_ep->util_ep.lock);

	if (ofi_cirque_isfull(rxd_ep->util_ep.tx_cq->cirq))
		goto out;

	rxd_addr = (intptr_t) ofi_idx_lookup(&(rxd_ep_av(rxd_ep)->fi_addr_idx),
					      RXD_IDX_OFFSET((int) addr));
	if (!rxd_addr)
		goto out;
	ret = rxd_send_rts_if_needed(rxd_ep, rxd_addr);
	if (ret)
		goto out;

	tx_entry = rxd_tx_entry_init_rma(rxd_ep, rxd_addr, op, iov, iov_count,
					 data, rxd_flags, context, rma_iov,
					 rma_count);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto out;
	}

	if (rxd_peer(rxd_ep, rxd_addr)->peer_addr == RXD_ADDR_INVALID)
		goto out;

	ret = rxd_start_xfer(rxd_ep, tx_entry);
	if (ret && tx_entry->num_segs > 1)
		(void) rxd_ep_post_data_pkts(rxd_ep, tx_entry);
	ret = 0;

out:
	ofi_genlock_unlock(&rxd_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxd_generic_rma(struct rxd_ep *rxd_ep, const struct iovec *iov,
	size_t iov_count, const struct fi_rma_iov *rma_iov, size_t rma_count,
	void **desc, fi_addr_t addr, void *context, uint32_t op, uint64_t data,
	uint32_t rxd_flags)
{
	struct rxd_x_entry *tx_entry;
	fi_addr_t rxd_addr;
	ssize_t ret = -FI_EAGAIN;

	if (rxd_flags & RXD_INJECT)
		return rxd_generic_write_inject(rxd_ep, iov, iov_count, rma_iov,
						rma_count, addr, context, op,
						data, rxd_flags);

	assert(iov_count <= RXD_IOV_LIMIT && rma_count <= RXD_IOV_LIMIT);

	ofi_genlock_lock(&rxd_ep->util_ep.lock);

	if (ofi_cirque_isfull(rxd_ep->util_ep.tx_cq->cirq))
		goto out;
	rxd_addr = (intptr_t) ofi_idx_lookup(&(rxd_ep_av(rxd_ep)->fi_addr_idx),
					     RXD_IDX_OFFSET((int) addr));
	if (!rxd_addr)
		goto out;

	ret = rxd_send_rts_if_needed(rxd_ep, rxd_addr);
	if (ret)
		goto out;

	tx_entry = rxd_tx_entry_init_rma(rxd_ep, rxd_addr, op, iov, iov_count,
					 data, rxd_flags, context, rma_iov,
					 rma_count);
	if (!tx_entry) {
		ret = -FI_EAGAIN;
		goto out;
	}

	if (rxd_peer(rxd_ep, rxd_addr)->peer_addr == RXD_ADDR_INVALID)
		goto out;

	ret = rxd_start_xfer(rxd_ep, tx_entry);
	if (ret && (tx_entry->cq_entry.flags & FI_WRITE) && tx_entry->num_segs > 1)
		(void) rxd_ep_post_data_pkts(rxd_ep, tx_entry);
	ret = 0;

out:
	ofi_genlock_unlock(&rxd_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxd_read(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
	fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
{
	struct rxd_ep *ep;
	struct iovec msg_iov;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return rxd_generic_rma(ep, &msg_iov, 1, &rma_iov, 1, &desc,
			       src_addr, context, RXD_READ_REQ, 0,
			       ep->tx_flags);
}

static ssize_t
rxd_readv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
	size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	void *context)
{
	struct rxd_ep *ep;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.len  = ofi_total_iov_len(iov, count);
	rma_iov.key = key;

	return rxd_generic_rma(ep, iov, count, &rma_iov, 1, desc,
			       src_addr, context, RXD_READ_REQ, 0,
			       ep->tx_flags);
}

static ssize_t
rxd_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
	uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_generic_rma(ep, msg->msg_iov, msg->iov_count,
			       msg->rma_iov, msg->rma_iov_count,
			       msg->desc, msg->addr, msg->context,
			       RXD_READ_REQ, msg->data, rxd_tx_flags(flags |
			       ep->util_ep.tx_msg_flags));
}

static ssize_t
rxd_write(struct fid_ep *ep_fid, const void *buf, size_t len, void *desc,
	fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	struct rxd_ep *ep;
	struct iovec msg_iov;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return rxd_generic_rma(ep, &msg_iov, 1, &rma_iov, 1, &desc,
			       dest_addr, context, RXD_WRITE, 0,
			       ep->tx_flags);
}

static ssize_t
rxd_writev(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		void *context)
{
	struct rxd_ep *ep;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.len  = ofi_total_iov_len(iov, count);
	rma_iov.key = key;

	return rxd_generic_rma(ep, iov, count, &rma_iov, 1, desc,
			       dest_addr, context, RXD_WRITE, 0,
			       ep->tx_flags);
}

static ssize_t
rxd_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
	uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_generic_rma(ep, msg->msg_iov, msg->iov_count,
			       msg->rma_iov, msg->rma_iov_count,
			       msg->desc, msg->addr, msg->context,
			       RXD_WRITE, msg->data, rxd_tx_flags(flags |
			       ep->util_ep.tx_msg_flags));
}

static ssize_t
rxd_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
		      void *desc, uint64_t data, fi_addr_t dest_addr,
		      uint64_t addr, uint64_t key, void *context)
{
	struct rxd_ep *ep;
	struct iovec iov;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len  = len;
	rma_iov.key = key;

	return rxd_generic_rma(ep, &iov, 1, &rma_iov, 1, &desc,
			       dest_addr, context, RXD_WRITE, data,
			       ep->tx_flags | RXD_REMOTE_CQ_DATA);
}

static ssize_t
rxd_inject_write(struct fid_ep *ep_fid, const void *buf,
	size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct rxd_ep *rxd_ep;
	struct iovec iov;
	struct fi_rma_iov rma_iov;

	rxd_ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return rxd_generic_write_inject(rxd_ep, &iov, 1, &rma_iov, 1,
					dest_addr, NULL, RXD_WRITE, 0,
					RXD_NO_TX_COMP | RXD_INJECT);
}

static ssize_t
rxd_inject_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
		     uint64_t data, fi_addr_t dest_addr, uint64_t addr,
		     uint64_t key)
{
	struct rxd_ep *rxd_ep;
	struct iovec iov;
	struct fi_rma_iov rma_iov;

	rxd_ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return rxd_generic_write_inject(rxd_ep, &iov, 1, &rma_iov, 1,
					dest_addr, NULL, RXD_WRITE,
					data, RXD_NO_TX_COMP | RXD_INJECT |
					RXD_REMOTE_CQ_DATA);
}

struct fi_ops_rma rxd_ops_rma = {
	.size = sizeof(struct fi_ops_rma),
	.read = rxd_read,
	.readv = rxd_readv,
	.readmsg = rxd_readmsg,
	.write = rxd_write,
	.writev = rxd_writev,
	.writemsg = rxd_writemsg,
	.inject = rxd_inject_write,
	.writedata = rxd_writedata,
	.injectdata = rxd_inject_writedata,

};
