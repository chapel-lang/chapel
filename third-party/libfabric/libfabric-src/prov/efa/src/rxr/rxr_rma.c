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
#include "rxr_msg.h"
#include "rxr_rma.h"
#include "rxr_pkt_cmd.h"
#include "rxr_cntr.h"
#include "rxr_read.h"

int rxr_rma_verified_copy_iov(struct rxr_ep *ep, struct efa_rma_iov *rma,
			      size_t count, uint32_t flags,
			      struct iovec *iov, void **desc)
{
	void *context;
	struct efa_mr *efa_mr;
	struct efa_ep *efa_ep;
	int i, ret;

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);

	for (i = 0; i < count; i++) {
		ofi_genlock_lock(&efa_ep->domain->util_domain.lock);
		ret = ofi_mr_map_verify(&efa_ep->domain->util_domain.mr_map,
					(uintptr_t *)(&rma[i].addr),
					rma[i].len, rma[i].key, flags,
					&context);
		efa_mr = context;
		desc[i] = fi_mr_desc(&efa_mr->mr_fid);
		ofi_genlock_unlock(&efa_ep->domain->util_domain.lock);
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


struct rxr_op_entry *
rxr_rma_alloc_tx_entry(struct rxr_ep *rxr_ep,
		       const struct fi_msg_rma *msg_rma,
		       uint32_t op,
		       uint64_t flags)
{
	struct rxr_op_entry *tx_entry;
	struct fi_msg msg;

	tx_entry = ofi_buf_alloc(rxr_ep->op_entry_pool);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL, "TX entries exhausted.\n");
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

	dlist_insert_tail(&tx_entry->ep_entry, &rxr_ep->tx_entry_list);
	return tx_entry;
}

size_t rxr_rma_post_shm_write(struct rxr_ep *rxr_ep, struct rxr_op_entry *tx_entry)
{
	struct rxr_pkt_entry *pkt_entry;
	struct fi_msg_rma msg;
	struct rdm_peer *peer;
	int i, err;

	assert(tx_entry->op == ofi_op_write);
	peer = rxr_ep_get_peer(rxr_ep, tx_entry->addr);
	assert(peer);
	pkt_entry = rxr_pkt_entry_alloc(rxr_ep, rxr_ep->shm_tx_pkt_pool, RXR_PKT_FROM_SHM_TX_POOL);
	if (OFI_UNLIKELY(!pkt_entry))
		return -FI_EAGAIN;

	rxr_pkt_init_write_context(tx_entry, pkt_entry);

	/* If no FI_MR_VIRT_ADDR being set, have to use 0-based offset */
	if (!(g_shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR)) {
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
	msg.desc = tx_entry->desc;
	rxr_convert_desc_for_shm(msg.iov_count, tx_entry->desc);

	err = fi_writemsg(rxr_ep->shm_ep, &msg, tx_entry->fi_flags);
	if (err) {
		rxr_pkt_entry_release_tx(rxr_ep, pkt_entry);
		return err;
	}

#if ENABLE_DEBUG
	dlist_insert_tail(&pkt_entry->dbg_entry, &rxr_ep->tx_pkt_list);
#endif
	return 0;
}

/* rma_read functions */
ssize_t rxr_rma_post_efa_emulated_read(struct rxr_ep *ep, struct rxr_op_entry *tx_entry)
{
	int err;

#if ENABLE_DEBUG
	dlist_insert_tail(&tx_entry->pending_recv_entry,
			  &ep->op_entry_recv_list);
	ep->pending_recv_counter++;
#endif

	if (tx_entry->total_len < ep->mtu_size - sizeof(struct rxr_readrsp_hdr)) {
		err = rxr_pkt_post_req(ep, tx_entry, RXR_SHORT_RTR_PKT, 0, 0);
	} else {
		assert(rxr_env.tx_min_credits > 0);
		tx_entry->window = MIN(tx_entry->total_len,
				       rxr_env.tx_min_credits * ep->max_data_payload_size);
		err = rxr_pkt_post_req(ep, tx_entry, RXR_LONGCTS_RTR_PKT, 0, 0);
	}

	if (OFI_UNLIKELY(err)) {
#if ENABLE_DEBUG
		dlist_remove(&tx_entry->pending_recv_entry);
		ep->pending_recv_counter--;
#endif
	}

	return err;
}

ssize_t rxr_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	ssize_t err;
	struct rxr_ep *rxr_ep;
	struct rdm_peer *peer;
	struct rxr_op_entry *tx_entry = NULL;
	bool use_lower_ep_read;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "read iov_len: %lu flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(msg->iov_count <= rxr_ep->tx_iov_limit);

	efa_perfset_start(rxr_ep, perf_efa_tx);
	ofi_mutex_lock(&rxr_ep->util_ep.lock);

	if (OFI_UNLIKELY(is_tx_res_full(rxr_ep))) {
		err = -FI_EAGAIN;
		goto out;
	}

	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	assert(peer);

	if (peer->flags & RXR_PEER_IN_BACKOFF) {
		err = -FI_EAGAIN;
		goto out;
	}
	tx_entry = rxr_rma_alloc_tx_entry(rxr_ep, msg, ofi_op_read_req, flags);
	if (OFI_UNLIKELY(!tx_entry)) {
		rxr_ep_progress_internal(rxr_ep);
		err = -FI_EAGAIN;
		goto out;
	}

	use_lower_ep_read = false;
	if (peer->is_local && rxr_ep->use_shm_for_tx) {
		use_lower_ep_read = true;
	} else if (efa_both_support_rdma_read(rxr_ep, peer)) {
		/* efa_both_support_rdma_read also check rxr_env.use_device_rdma,
		 * so we do not check it here
		 */
		use_lower_ep_read = true;
	} else if (efa_mr_is_neuron(tx_entry->desc[0])) {
		err = rxr_ep_determine_rdma_support(rxr_ep, tx_entry->addr, peer);

		if (err < 0)
			goto out;

		if (err != 1) {
			err = -FI_EOPNOTSUPP;
			goto out;
		}

		use_lower_ep_read = true;
	}

	/*
	 * Not going to check efa_ep->hmem_p2p_opt here, if the remote side
	 * gave us a valid MR we should just honor the request even if p2p is
	 * disabled.
	 */
	if (use_lower_ep_read) {
		err = rxr_read_post_remote_read_or_queue(rxr_ep, tx_entry);
		if (OFI_UNLIKELY(err == -FI_ENOBUFS)) {
			err = -FI_EAGAIN;
			rxr_ep_progress_internal(rxr_ep);
			goto out;
		}
	} else {
		err = rxr_rma_post_efa_emulated_read(rxr_ep, tx_entry);
		if (OFI_UNLIKELY(err))
			rxr_ep_progress_internal(rxr_ep);
	}

out:
	if (OFI_UNLIKELY(err && tx_entry))
		rxr_release_tx_entry(rxr_ep, tx_entry);

	ofi_mutex_unlock(&rxr_ep->util_ep.lock);
	efa_perfset_end(rxr_ep, perf_efa_tx);
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
ssize_t rxr_rma_post_write(struct rxr_ep *ep, struct rxr_op_entry *tx_entry)
{
	ssize_t err;
	struct rdm_peer *peer;
	bool delivery_complete_requested;
	int ctrl_type, iface;
	size_t max_eager_rtw_data_size;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(peer);

	if (peer->is_local && ep->use_shm_for_tx)
		return rxr_rma_post_shm_write(ep, tx_entry);

	delivery_complete_requested = tx_entry->fi_flags & FI_DELIVERY_COMPLETE;
	if (delivery_complete_requested) {
		/*
		 * Because delivery complete is defined as an extra
		 * feature, the receiver might not support it.
		 *
		 * The sender cannot send with FI_DELIVERY_COMPLETE
		 * if the peer is not able to handle it.
		 *
		 * If the sender does not know whether the peer
		 * can handle it, it needs to trigger
		 * a handshake packet from the peer.
		 *
		 * The handshake packet contains
		 * the information whether the peer
		 * support it or not.
		 */
		err = rxr_pkt_trigger_handshake(ep, tx_entry->addr, peer);
		if (OFI_UNLIKELY(err))
			return err;

		if (!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED))
			return -FI_EAGAIN;
		else if (!rxr_peer_support_delivery_complete(peer))
			return -FI_EOPNOTSUPP;

		max_eager_rtw_data_size = rxr_tx_entry_max_req_data_capacity(ep, tx_entry, RXR_DC_EAGER_RTW_PKT);
	} else {
		max_eager_rtw_data_size = rxr_tx_entry_max_req_data_capacity(ep, tx_entry, RXR_EAGER_RTW_PKT);
	}

	/* Inter instance */

	iface = tx_entry->desc[0] ? ((struct efa_mr*) tx_entry->desc[0])->peer.iface : FI_HMEM_SYSTEM;

	if (tx_entry->total_len >= rxr_ep_domain(ep)->hmem_info[iface].min_read_write_size &&
		rxr_ep_determine_rdma_support(ep, tx_entry->addr, peer) &&
		(tx_entry->desc[0] || efa_is_cache_available(rxr_ep_domain(ep)))) {
		err = rxr_pkt_post_req(ep, tx_entry, RXR_LONGREAD_RTW_PKT, 0, 0);
		if (err != -FI_ENOMEM)
			return err;
		/*
		 * If read write protocol failed due to memory registration, fall back to use long
		 * message protocol
		 */
	}

	if (tx_entry->total_len <= max_eager_rtw_data_size) {
		ctrl_type = delivery_complete_requested ? RXR_DC_EAGER_RTW_PKT : RXR_EAGER_RTW_PKT;
		return rxr_pkt_post_req(ep, tx_entry, ctrl_type, 0, 0);
	}

	ctrl_type = delivery_complete_requested ? RXR_DC_LONGCTS_RTW_PKT : RXR_LONGCTS_RTW_PKT;
	return rxr_pkt_post_req(ep, tx_entry, ctrl_type, 0, 0);
}

ssize_t rxr_rma_writemsg(struct fid_ep *ep,
			 const struct fi_msg_rma *msg,
			 uint64_t flags)
{
	ssize_t err;
	struct rdm_peer *peer;
	struct rxr_ep *rxr_ep;
	struct rxr_op_entry *tx_entry;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "write iov_len %lu flags: %lx\n",
	       ofi_total_iov_len(msg->msg_iov, msg->iov_count),
	       flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	assert(msg->iov_count <= rxr_ep->tx_iov_limit);

	efa_perfset_start(rxr_ep, perf_efa_tx);
	ofi_mutex_lock(&rxr_ep->util_ep.lock);

	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	assert(peer);

	if (peer->flags & RXR_PEER_IN_BACKOFF) {
		err = -FI_EAGAIN;
		goto out;
	}

	tx_entry = rxr_rma_alloc_tx_entry(rxr_ep, msg, ofi_op_write, flags);
	if (OFI_UNLIKELY(!tx_entry)) {
		rxr_ep_progress_internal(rxr_ep);
		err = -FI_EAGAIN;
		goto out;
	}

	err = rxr_rma_post_write(rxr_ep, tx_entry);
	if (OFI_UNLIKELY(err)) {
		rxr_ep_progress_internal(rxr_ep);
		rxr_release_tx_entry(rxr_ep, tx_entry);
	}
out:
	ofi_mutex_unlock(&rxr_ep->util_ep.lock);
	efa_perfset_end(rxr_ep, perf_efa_tx);
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
	msg.desc = &desc;
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
	struct rxr_ep *rxr_ep;
	struct rdm_peer *peer;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	peer = rxr_ep_get_peer(rxr_ep, dest_addr);
	if (peer->is_local)
		return fi_inject_write(rxr_ep->shm_ep, buf, len, peer->shm_fiaddr, addr, key);

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
	struct rxr_ep *rxr_ep;
	struct rdm_peer *peer;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	peer = rxr_ep_get_peer(rxr_ep, dest_addr);
	if (peer->is_local)
		return fi_inject_writedata(rxr_ep->shm_ep, buf, len, data, peer->shm_fiaddr, addr, key);

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

