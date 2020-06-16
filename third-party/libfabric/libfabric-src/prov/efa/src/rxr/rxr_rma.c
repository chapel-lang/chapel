/*
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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
#include "efa.h"
#include "rxr.h"
#include "rxr_rma.h"
#include "rxr_pkt_cmd.h"
#include "rxr_cntr.h"
#include "rxr_read.h"

int rxr_rma_verified_copy_iov(struct rxr_ep *ep, struct fi_rma_iov *rma,
			      size_t count, uint32_t flags, struct iovec *iov)
{
	struct efa_ep *efa_ep;
	int i, ret;

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);

	for (i = 0; i < count; i++) {
		ret = ofi_mr_verify(&efa_ep->domain->util_domain.mr_map,
				    rma[i].len,
				    (uintptr_t *)(&rma[i].addr),
				    rma[i].key,
				    flags);
		if (ret) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"MR verification failed (%s), addr: %lx key: %ld\n",
				fi_strerror(-ret), rma[i].addr, rma[i].key);
			return -FI_EACCES;
		}

		iov[i].iov_base = (void *)rma[i].addr;
		iov[i].iov_len = rma[i].len;
	}
	return 0;
}
/* Upon receiving a read request, Remote EP call this function to create
 * a tx entry for sending data back.
 */
struct rxr_tx_entry *
rxr_rma_alloc_readrsp_tx_entry(struct rxr_ep *rxr_ep,
			       struct rxr_rx_entry *rx_entry)
{
	struct rxr_tx_entry *tx_entry;
	struct fi_msg msg;

	tx_entry = ofi_buf_alloc(rxr_ep->readrsp_tx_entry_pool);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Read Response TX entries exhausted.\n");
		return NULL;
	}

	assert(tx_entry);
#if ENABLE_DEBUG
	dlist_insert_tail(&tx_entry->tx_entry_entry, &rxr_ep->tx_entry_list);
#endif

	msg.msg_iov = rx_entry->iov;
	msg.iov_count = rx_entry->iov_count;
	msg.addr = rx_entry->addr;
	msg.desc = NULL;
	msg.context = NULL;
	msg.data = 0;

	/*
	 * this tx_entry works similar to a send tx_entry thus its op was
	 * set to ofi_op_msg. Note this tx_entry will not write a completion
	 */
	rxr_tx_entry_init(rxr_ep, tx_entry, &msg, ofi_op_msg, 0);

	tx_entry->cq_entry.flags |= FI_READ;
	/* rma_loc_rx_id is for later retrieve of rx_entry
	 * to write rx_completion
	 */
	tx_entry->rma_loc_rx_id = rx_entry->rx_id;

	/* the following is essentially handle CTS */
	tx_entry->rx_id = rx_entry->rma_initiator_rx_id;
	tx_entry->window = rx_entry->window;

	/* this tx_entry does not send request
	 * therefore should not increase msg_id
	 */
	tx_entry->msg_id = 0;
	return tx_entry;
}

struct rxr_tx_entry *
rxr_rma_alloc_tx_entry(struct rxr_ep *rxr_ep,
		       const struct fi_msg_rma *msg_rma,
		       uint32_t op,
		       uint64_t flags)
{
	struct rxr_tx_entry *tx_entry;
	struct fi_msg msg;

	tx_entry = ofi_buf_alloc(rxr_ep->tx_entry_pool);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return NULL;
	}

	msg.addr = msg_rma->addr;
	msg.msg_iov = msg_rma->msg_iov;
	msg.context = msg_rma->context;
	msg.iov_count = msg_rma->iov_count;
	msg.data = msg_rma->data;
	msg.desc = msg_rma->desc;
	rxr_tx_entry_init(rxr_ep, tx_entry, &msg, op, flags);

	assert(msg_rma->rma_iov_count > 0);
	assert(msg_rma->rma_iov);
	tx_entry->rma_iov_count = msg_rma->rma_iov_count;
	memcpy(tx_entry->rma_iov, msg_rma->rma_iov,
	       sizeof(struct fi_rma_iov) * msg_rma->rma_iov_count);

#if ENABLE_DEBUG
	dlist_insert_tail(&tx_entry->tx_entry_entry, &rxr_ep->tx_entry_list);
#endif
	return tx_entry;
}

size_t rxr_rma_post_shm_write(struct rxr_ep *rxr_ep, struct rxr_tx_entry *tx_entry)
{
	struct rxr_pkt_entry *pkt_entry;
	struct fi_msg_rma msg;
	struct rxr_peer *peer;
	int i, err;

	assert(tx_entry->op == ofi_op_write);
	peer = rxr_ep_get_peer(rxr_ep, tx_entry->addr);
	pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->tx_pkt_shm_pool);
	if (OFI_UNLIKELY(!pkt_entry))
		return -FI_EAGAIN;

	rxr_pkt_init_write_context(tx_entry, pkt_entry);

	/* If no FI_MR_VIRT_ADDR being set, have to use 0-based offset */
	if (!(shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR)) {
		for (i = 0; i < tx_entry->iov_count; i++)
			tx_entry->rma_iov[i].addr = 0;
	}

	msg.msg_iov = tx_entry->iov;
	msg.iov_count = tx_entry->iov_count;
	msg.addr = peer->shm_fiaddr;
	msg.rma_iov = tx_entry->rma_iov;
	msg.rma_iov_count = tx_entry->rma_iov_count;
	msg.context = pkt_entry;
	msg.data = tx_entry->cq_entry.data;

	err = fi_writemsg(rxr_ep->shm_ep, &msg, tx_entry->fi_flags);
	if (err)
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);

	return err;
}

/* rma_read functions */
ssize_t rxr_rma_post_efa_emulated_read(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry)
{
	int err, window, credits;
	struct rxr_peer *peer;
	struct rxr_rx_entry *rx_entry;
	struct fi_msg msg = {0};

	/* create a rx_entry to receve data
	 * use ofi_op_msg for its op.
	 * it does not write a rx completion.
	 */
	msg.msg_iov = tx_entry->iov;
	msg.iov_count = tx_entry->iov_count;
	msg.addr = tx_entry->addr;
	rx_entry = rxr_ep_get_rx_entry(ep, &msg, 0, ~0, ofi_op_msg, 0);
	if (!rx_entry) {
		rxr_release_tx_entry(ep, tx_entry);
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted for read.\n");
		rxr_ep_progress_internal(ep);
		return -FI_EAGAIN;
	}

	/*
	 * this rx_entry does not know its tx_id, because remote
	 * tx_entry has not been created yet.
	 * set tx_id to -1, and the correct one will be filled in
	 * rxr_cq_handle_readrsp()
	 */
	assert(rx_entry);
	rx_entry->tx_id = -1;
	rx_entry->cq_entry.flags |= FI_READ;
	rx_entry->total_len = rx_entry->cq_entry.len;

	/*
	 * there will not be a CTS for fi_read, we calculate CTS
	 * window here, and send it via REQ.
	 * meanwhile set rx_entry->state to RXR_RX_RECV so that
	 * this rx_entry is ready to receive.
	 */

	/* But if there is no available buffer, we do not even proceed.
	 * call rxr_ep_progress_internal() might release some buffer
	 */
	if (ep->available_data_bufs == 0) {
		rxr_release_tx_entry(ep, tx_entry);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_ep_progress_internal(ep);
		return -FI_EAGAIN;
	}

	rx_entry->state = RXR_RX_RECV;
	/* rma_loc_tx_id is used in rxr_cq_handle_rx_completion()
	 * to locate the tx_entry for tx completion.
	 */
	rx_entry->rma_loc_tx_id = tx_entry->tx_id;
#if ENABLE_DEBUG
	dlist_insert_tail(&rx_entry->rx_pending_entry,
			  &ep->rx_pending_list);
	ep->rx_pending++;
#endif
	/*
	 * this tx_entry does not need a rx_id, because it does not
	 * send any data.
	 * the rma_loc_rx_id and rma_window will be sent to remote EP
	 * via REQ
	 */
	tx_entry->rma_loc_rx_id = rx_entry->rx_id;

	if (tx_entry->total_len < ep->mtu_size - sizeof(struct rxr_readrsp_hdr)) {
		err = rxr_pkt_post_ctrl_or_queue(ep, RXR_TX_ENTRY, tx_entry, RXR_SHORT_RTR_PKT, 0);
	} else {
		peer = rxr_ep_get_peer(ep, tx_entry->addr);
		assert(peer);
		rxr_pkt_calc_cts_window_credits(ep, peer,
						tx_entry->total_len,
						tx_entry->credit_request,
						&window,
						&credits);

		rx_entry->window = window;
		rx_entry->credit_cts = credits;
		tx_entry->rma_window = rx_entry->window;
		err = rxr_pkt_post_ctrl_or_queue(ep, RXR_TX_ENTRY, tx_entry, RXR_LONG_RTR_PKT, 0);
	}

	return err;
}

ssize_t rxr_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	ssize_t err;
	struct rxr_ep *rxr_ep;
	struct rxr_peer *peer;
	struct rxr_tx_entry *tx_entry;
	bool use_lower_ep_read;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "read iov_len: %lu flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(msg->iov_count <= rxr_ep->tx_iov_limit);

	rxr_perfset_start(rxr_ep, perf_rxr_tx);
	fastlock_acquire(&rxr_ep->util_ep.lock);

	if (OFI_UNLIKELY(is_tx_res_full(rxr_ep)))
		return -FI_EAGAIN;

	tx_entry = rxr_rma_alloc_tx_entry(rxr_ep, msg, ofi_op_read_req, flags);
	if (OFI_UNLIKELY(!tx_entry)) {
		rxr_ep_progress_internal(rxr_ep);
		return -FI_EAGAIN;
	}

	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	assert(peer);

	use_lower_ep_read = false;
	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		use_lower_ep_read = true;
	} else if (efa_both_support_rdma_read(rxr_ep, peer)) {
		/* efa_both_support_rdma_read also check rxr_env.use_device_rdma,
		 * so we do not check it here
		 */
		use_lower_ep_read = true;
	}

	if (use_lower_ep_read) {
		err = rxr_read_post_or_queue(rxr_ep, RXR_TX_ENTRY, tx_entry);
		if (OFI_UNLIKELY(err == -FI_ENOBUFS)) {
			rxr_release_tx_entry(rxr_ep, tx_entry);
			err = -FI_EAGAIN;
			rxr_ep_progress_internal(rxr_ep);
			goto out;
		}
	} else {
		err = rxr_ep_set_tx_credit_request(rxr_ep, tx_entry);
		if (OFI_UNLIKELY(err)) {
			rxr_release_tx_entry(rxr_ep, tx_entry);
			goto out;
		}

		err = rxr_rma_post_efa_emulated_read(rxr_ep, tx_entry);
	}

out:
	fastlock_release(&rxr_ep->util_ep.lock);
	rxr_perfset_end(rxr_ep, perf_rxr_tx);
	return err;
}

ssize_t rxr_rma_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		      size_t iov_count, fi_addr_t src_addr, uint64_t addr,
		      uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;

	rma_iov.addr = addr;
	rma_iov.len = ofi_total_iov_len(iov, iov_count);
	rma_iov.key = key;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.desc = desc;
	msg.iov_count = iov_count;
	msg.addr = src_addr;
	msg.context = context;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;

	return rxr_rma_readmsg(ep, &msg, 0);
}

ssize_t rxr_rma_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
		     fi_addr_t src_addr, uint64_t addr, uint64_t key,
		     void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return rxr_rma_readv(ep, &iov, &desc, 1, src_addr, addr, key, context);
}

/* rma_write functions */
ssize_t rxr_rma_post_write(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry)
{
	ssize_t err;
	struct rxr_peer *peer;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(peer);
	if (peer->is_local)
		return rxr_rma_post_shm_write(ep, tx_entry);

	/* Inter instance */
	if (tx_entry->total_len < rxr_pkt_req_max_data_size(ep, tx_entry->addr, RXR_EAGER_RTW_PKT))
		return rxr_pkt_post_ctrl_or_queue(ep, RXR_TX_ENTRY, tx_entry, RXR_EAGER_RTW_PKT, 0);

	if (tx_entry->total_len >= rxr_env.efa_min_read_write_size &&
	    efa_both_support_rdma_read(ep, peer) &&
	    (tx_entry->desc[0] || efa_mr_cache_enable)) {
		err = rxr_pkt_post_ctrl_or_queue(ep, RXR_TX_ENTRY, tx_entry, RXR_READ_RTW_PKT, 0);
		if (err != -FI_ENOMEM)
			return err;
		/*
		 * If read write protocol failed due to memory registration, fall back to use long
		 * message protocol
		 */
	}

	err = rxr_ep_set_tx_credit_request(ep, tx_entry);
	if (OFI_UNLIKELY(err))
		return err;

	return rxr_pkt_post_ctrl_or_queue(ep, RXR_TX_ENTRY, tx_entry, RXR_LONG_RTW_PKT, 0);
}

ssize_t rxr_rma_writemsg(struct fid_ep *ep,
			 const struct fi_msg_rma *msg,
			 uint64_t flags)
{
	ssize_t err;
	struct rxr_ep *rxr_ep;
	struct rxr_tx_entry *tx_entry;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "write iov_len %lu flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(msg->iov_count <= rxr_ep->tx_iov_limit);

	rxr_perfset_start(rxr_ep, perf_rxr_tx);
	fastlock_acquire(&rxr_ep->util_ep.lock);

	tx_entry = rxr_rma_alloc_tx_entry(rxr_ep, msg, ofi_op_write, flags);
	if (OFI_UNLIKELY(!tx_entry)) {
		rxr_ep_progress_internal(rxr_ep);
		err = -FI_EAGAIN;
		goto out;
	}

	err = rxr_rma_post_write(rxr_ep, tx_entry);
	if (OFI_UNLIKELY(err))
		rxr_release_tx_entry(rxr_ep, tx_entry);
out:
	fastlock_release(&rxr_ep->util_ep.lock);
	rxr_perfset_end(rxr_ep, perf_rxr_tx);
	return err;
}

ssize_t rxr_rma_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
		       size_t iov_count, fi_addr_t dest_addr, uint64_t addr,
		       uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;

	rma_iov.addr = addr;
	rma_iov.len = ofi_total_iov_len(iov, iov_count);
	rma_iov.key = key;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.desc = desc;
	msg.iov_count = iov_count;
	msg.addr = dest_addr;
	msg.context = context;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;

	return rxr_rma_writemsg(ep, &msg, 0);
}

ssize_t rxr_rma_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return rxr_rma_writev(ep, &iov, &desc, 1, dest_addr, addr, key, context);
}

ssize_t rxr_rma_writedata(struct fid_ep *ep, const void *buf, size_t len,
			  void *desc, uint64_t data, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key, void *context)
{
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	struct fi_msg_rma msg;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len  = len;
	rma_iov.key = key;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.desc = desc;
	msg.iov_count = 1;
	msg.addr = dest_addr;
	msg.context = context;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;
	msg.data = data;

	return rxr_rma_writemsg(ep, &msg, FI_REMOTE_CQ_DATA);
}

ssize_t rxr_rma_inject_write(struct fid_ep *ep, const void *buf, size_t len,
			     fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	struct fi_msg_rma msg;
	struct iovec iov;
	struct fi_rma_iov rma_iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len  = len;
	rma_iov.key = key;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;
	msg.addr = dest_addr;

	return rxr_rma_writemsg(ep, &msg, FI_INJECT | RXR_NO_COMPLETION);
}

ssize_t rxr_rma_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
				 uint64_t data, fi_addr_t dest_addr, uint64_t addr,
				 uint64_t key)
{
	struct fi_msg_rma msg;
	struct iovec iov;
	struct fi_rma_iov rma_iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len  = len;
	rma_iov.key = key;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;
	msg.addr = dest_addr;
	msg.data = data;

	return rxr_rma_writemsg(ep, &msg, FI_INJECT | RXR_NO_COMPLETION |
				FI_REMOTE_CQ_DATA);
}

struct fi_ops_rma rxr_ops_rma = {
	.size = sizeof(struct fi_ops_rma),
	.read = rxr_rma_read,
	.readv = rxr_rma_readv,
	.readmsg = rxr_rma_readmsg,
	.write = rxr_rma_write,
	.writev = rxr_rma_writev,
	.writemsg = rxr_rma_writemsg,
	.inject = rxr_rma_inject_write,
	.writedata = rxr_rma_writedata,
	.injectdata = rxr_rma_inject_writedata,
};

