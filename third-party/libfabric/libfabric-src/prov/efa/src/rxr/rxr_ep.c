/*
 * Copyright (c) 2019-2022 Amazon.com, Inc. or its affiliates.
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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "ofi.h"
#include <ofi_util.h>
#include <ofi_iov.h>
#include "efa.h"
#include "efa_cq.h"
#include "rxr_msg.h"
#include "rxr_rma.h"
#include "rxr_pkt_cmd.h"
#include "rxr_pkt_type_base.h"
#include "rxr_read.h"
#include "rxr_atomic.h"
#include <infiniband/verbs.h>

struct efa_ep_addr *rxr_ep_raw_addr(struct rxr_ep *ep)
{
	return (struct efa_ep_addr *)ep->core_addr;
}

const char *rxr_ep_raw_addr_str(struct rxr_ep *ep, char *buf, size_t *buflen)
{
	return ofi_straddr(buf, buflen, FI_ADDR_EFA, rxr_ep_raw_addr(ep));
}

struct efa_ep_addr *rxr_peer_raw_addr(struct rxr_ep *ep, fi_addr_t addr)
{
	struct efa_ep *efa_ep;
	struct efa_av *efa_av;
	struct efa_conn *efa_conn;

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	efa_av = efa_ep->av;
	efa_conn = efa_av_addr_to_conn(efa_av, addr);
	return efa_conn ? efa_conn->ep_addr : NULL;
}

const char *rxr_peer_raw_addr_str(struct rxr_ep *ep, fi_addr_t addr, char *buf, size_t *buflen)
{
	return ofi_straddr(buf, buflen, FI_ADDR_EFA, rxr_peer_raw_addr(ep, addr));
}

/**
 * @brief allocate an rx entry for an operation
 *
 * @param ep[in]	end point
 * @param addr[in]	fi address of the sender/requester.
 * @param op[in]	operation type (ofi_op_msg/ofi_op_tagged/ofi_op_read/ofi_op_write/ofi_op_atomic_xxx)
 * @return		if allocation succeeded, return pointer to rx_entry
 * 			if allocation failed, return NULL
 */
struct rxr_op_entry *rxr_ep_alloc_rx_entry(struct rxr_ep *ep, fi_addr_t addr, uint32_t op)
{
	struct rxr_op_entry *rx_entry;

	rx_entry = ofi_buf_alloc(ep->op_entry_pool);
	if (OFI_UNLIKELY(!rx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "RX entries exhausted\n");
		return NULL;
	}
	memset(rx_entry, 0, sizeof(struct rxr_op_entry));

	dlist_insert_tail(&rx_entry->ep_entry, &ep->rx_entry_list);
	rx_entry->type = RXR_RX_ENTRY;
	rx_entry->rx_id = ofi_buf_index(rx_entry);
	dlist_init(&rx_entry->queued_pkts);

	rx_entry->state = RXR_RX_INIT;
	rx_entry->addr = addr;
	if (addr != FI_ADDR_UNSPEC) {
		rx_entry->peer = rxr_ep_get_peer(ep, addr);
		assert(rx_entry->peer);
		dlist_insert_tail(&rx_entry->peer_entry, &rx_entry->peer->rx_entry_list);
	} else {
		/*
		 * If msg->addr is not provided, rx_entry->peer will be set
		 * after it is matched with a message.
		 */
		assert(op == ofi_op_msg || op == ofi_op_tagged);
		rx_entry->peer = NULL;
	}

	rx_entry->bytes_runt = 0;
	rx_entry->bytes_received_via_mulreq = 0;
	rx_entry->cuda_copy_method = RXR_CUDA_COPY_UNSPEC;
	rx_entry->efa_outstanding_tx_ops = 0;
	rx_entry->shm_outstanding_tx_ops = 0;
	rx_entry->op = op;
	dlist_init(&rx_entry->entry);
	switch (op) {
	case ofi_op_tagged:
		rx_entry->cq_entry.flags = (FI_RECV | FI_MSG | FI_TAGGED);
		break;
	case ofi_op_msg:
		rx_entry->cq_entry.flags = (FI_RECV | FI_MSG);
		break;
	case ofi_op_read_rsp:
		rx_entry->cq_entry.flags = (FI_REMOTE_READ | FI_RMA);
		break;
	case ofi_op_write:
		rx_entry->cq_entry.flags = (FI_REMOTE_WRITE | FI_RMA);
		break;
	case ofi_op_atomic:
		rx_entry->cq_entry.flags = (FI_REMOTE_WRITE | FI_ATOMIC);
		break;
	case ofi_op_atomic_fetch:
	case ofi_op_atomic_compare:
		rx_entry->cq_entry.flags = (FI_REMOTE_READ | FI_ATOMIC);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown operation while %s\n", __func__);
		assert(0 && "Unknown operation");
	}

	return rx_entry;
}

/**
 * @brief post user provided receiving buffer to the device.
 *
 * The user receive buffer was converted to an RX packet, then posted to the device.
 *
 * @param[in]	ep		endpint
 * @param[in]	rx_entry	rx_entry that contain user buffer information
 * @param[in]	flags		user supplied flags passed to fi_recv
 */
int rxr_ep_post_user_recv_buf(struct rxr_ep *ep, struct rxr_op_entry *rx_entry, uint64_t flags)
{
	struct rxr_pkt_entry *pkt_entry;
	struct efa_mr *mr;
	struct iovec msg_iov;
	struct fi_msg msg = {0};
	int err;

	assert(rx_entry->iov_count == 1);
	assert(rx_entry->iov[0].iov_len >= ep->msg_prefix_size);
	pkt_entry = (struct rxr_pkt_entry *)rx_entry->iov[0].iov_base;
	assert(pkt_entry);

	/*
	 * The ownership of the prefix buffer lies with the application, do not
	 * put it on the dbg list for cleanup during shutdown or poison it. The
	 * provider loses jurisdiction over it soon after writing the rx
	 * completion.
	 */
	dlist_init(&pkt_entry->entry);
	mr = (struct efa_mr *)rx_entry->desc[0];
	pkt_entry->mr = &mr->mr_fid;
	pkt_entry->alloc_type = RXR_PKT_FROM_USER_BUFFER;
	pkt_entry->flags = RXR_PKT_ENTRY_IN_USE;
	pkt_entry->next = NULL;
	/*
	 * The actual receiving buffer size (pkt_size) is
	 *    rx_entry->total_len - sizeof(struct rxr_pkt_entry)
	 * because the first part of user buffer was used to
	 * construct pkt_entry. The actual receiving buffer
	 * posted to device starts from pkt_entry->pkt.
	 */
	pkt_entry->pkt_size = rx_entry->iov[0].iov_len - sizeof(struct rxr_pkt_entry);

	pkt_entry->x_entry = rx_entry;
	rx_entry->state = RXR_RX_MATCHED;

	msg_iov.iov_base = pkt_entry->pkt;
	msg_iov.iov_len = pkt_entry->pkt_size;
	assert(msg_iov.iov_len <= ep->mtu_size);

	msg.iov_count = 1;
	msg.msg_iov = &msg_iov;
	msg.desc = rx_entry->desc;
	msg.addr = FI_ADDR_UNSPEC;
	msg.context = pkt_entry;
	msg.data = 0;

	err = fi_recvmsg(ep->rdm_ep, &msg, flags);
	if (OFI_UNLIKELY(err)) {
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"failed to post user supplied buffer %d (%s)\n", -err,
			fi_strerror(-err));
		return err;
	}

	ep->efa_rx_pkts_posted++;
	return 0;
}

/**
 * @brief post an internal receive buffer to lower endpoint
 *
 * The buffer was posted as undirected recv, (address was set to FI_ADDR_UNSPEC)
 *
 * @param[in]	ep		endpoint
 * @param[in]	flags		flags passed to lower provider, can have FI_MORE
 * @param[in]	lower_ep_type	lower endpoint type, can be either SHM_EP or EFA_EP
 * @return	On success, return 0
 * 		On failure, return a negative error code.
 */
int rxr_ep_post_internal_rx_pkt(struct rxr_ep *ep, uint64_t flags, enum rxr_lower_ep_type lower_ep_type)
{
	struct fi_msg msg = {0};
	struct iovec msg_iov;
	void *desc;
	struct rxr_pkt_entry *rx_pkt_entry = NULL;
	int ret = 0;

	switch (lower_ep_type) {
	case SHM_EP:
		rx_pkt_entry = rxr_pkt_entry_alloc(ep, ep->shm_rx_pkt_pool, RXR_PKT_FROM_SHM_RX_POOL);
		break;
	case EFA_EP:
		rx_pkt_entry = rxr_pkt_entry_alloc(ep, ep->efa_rx_pkt_pool, RXR_PKT_FROM_EFA_RX_POOL);
		break;
	default:
		/* Coverity will complain about this being a dead code segment,
		 * but it is useful for future proofing.
		 */
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"invalid lower EP type %d\n", lower_ep_type);
		assert(0 && "invalid lower EP type\n");
	}
	if (OFI_UNLIKELY(!rx_pkt_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unable to allocate rx_pkt_entry\n");
		return -FI_ENOMEM;
	}

	rx_pkt_entry->x_entry = NULL;

	msg_iov.iov_base = (void *)rxr_pkt_start(rx_pkt_entry);
	msg_iov.iov_len = ep->mtu_size;
	rxr_msg_construct(&msg, &msg_iov, NULL, 1, FI_ADDR_UNSPEC, rx_pkt_entry, 0);

	switch (lower_ep_type) {
	case SHM_EP:
		/* pre-post buffer with shm */
#if ENABLE_DEBUG
		dlist_insert_tail(&rx_pkt_entry->dbg_entry,
				  &ep->rx_posted_buf_shm_list);
#endif
		desc = NULL;
		msg.desc = &desc;
		ret = fi_recvmsg(ep->shm_ep, &msg, flags);
		if (OFI_UNLIKELY(ret)) {
			rxr_pkt_entry_release_rx(ep, rx_pkt_entry);
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"failed to post buf for shm  %d (%s)\n", -ret,
				fi_strerror(-ret));
			return ret;
		}
		ep->shm_rx_pkts_posted++;
		break;
	case EFA_EP:
#if ENABLE_DEBUG
		dlist_insert_tail(&rx_pkt_entry->dbg_entry,
				  &ep->rx_posted_buf_list);
#endif
		desc = fi_mr_desc(rx_pkt_entry->mr);
		msg.desc = &desc;
		ret = fi_recvmsg(ep->rdm_ep, &msg, flags);
		if (OFI_UNLIKELY(ret)) {
			rxr_pkt_entry_release_rx(ep, rx_pkt_entry);
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"failed to post buf %d (%s)\n", -ret,
				fi_strerror(-ret));
			return ret;
		}
		ep->efa_rx_pkts_posted++;
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"invalid lower EP type %d\n", lower_ep_type);
		assert(0 && "invalid lower EP type\n");
	}

	return 0;
}

/**
 * @brief bulk post internal receive buffer(s) to device
 *
 * When posting multiple buffers, this function will use
 * FI_MORE flag to achieve better performance.
 *
 * @param[in]	ep		endpint
 * @param[in]	nrecv		number of receive buffers to post
 * @param[in]	lower_ep_type	device type, can be SHM_EP or EFA_EP
 * @return	On success, return 0
 * 		On failure, return negative libfabric error code
 */
static inline
ssize_t rxr_ep_bulk_post_internal_rx_pkts(struct rxr_ep *ep, int nrecv,
					  enum rxr_lower_ep_type lower_ep_type)
{
	int i;
	ssize_t err;
	uint64_t flags;

	flags = FI_MORE;
	for (i = 0; i < nrecv; ++i) {
		if (i == nrecv - 1)
			flags = 0;

		err = rxr_ep_post_internal_rx_pkt(ep, flags, lower_ep_type);
		if (OFI_UNLIKELY(err))
			return err;
	}

	return 0;
}

void rxr_tx_entry_init(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
		       const struct fi_msg *msg, uint32_t op, uint64_t flags)
{
	uint64_t tx_op_flags;

	tx_entry->type = RXR_TX_ENTRY;
	tx_entry->op = op;
	tx_entry->tx_id = ofi_buf_index(tx_entry);
	tx_entry->state = RXR_TX_REQ;
	tx_entry->addr = msg->addr;
	tx_entry->peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(tx_entry->peer);
	dlist_insert_tail(&tx_entry->peer_entry, &tx_entry->peer->tx_entry_list);

	tx_entry->rxr_flags = 0;
	tx_entry->bytes_received = 0;
	tx_entry->bytes_copied = 0;
	tx_entry->bytes_acked = 0;
	tx_entry->bytes_sent = 0;
	tx_entry->window = 0;
	tx_entry->iov_count = msg->iov_count;
	tx_entry->msg_id = 0;
	tx_entry->efa_outstanding_tx_ops = 0;
	tx_entry->shm_outstanding_tx_ops = 0;
	dlist_init(&tx_entry->queued_pkts);

	memcpy(tx_entry->iov, msg->msg_iov, sizeof(struct iovec) * msg->iov_count);
	memset(tx_entry->mr, 0, sizeof(*tx_entry->mr) * msg->iov_count);
	if (msg->desc)
		memcpy(tx_entry->desc, msg->desc, sizeof(*msg->desc) * msg->iov_count);
	else
		memset(tx_entry->desc, 0, sizeof(tx_entry->desc));

	/* cq_entry on completion */
	tx_entry->cq_entry.op_context = msg->context;
	tx_entry->cq_entry.data = msg->data;
	tx_entry->cq_entry.len = ofi_total_iov_len(tx_entry->iov, tx_entry->iov_count);
	tx_entry->cq_entry.buf = OFI_LIKELY(tx_entry->cq_entry.len > 0) ? tx_entry->iov[0].iov_base : NULL;

	if (ep->msg_prefix_size > 0) {
		assert(tx_entry->iov[0].iov_len >= ep->msg_prefix_size);
		tx_entry->iov[0].iov_base = (char *)tx_entry->iov[0].iov_base + ep->msg_prefix_size;
		tx_entry->iov[0].iov_len -= ep->msg_prefix_size;
	}
	tx_entry->total_len = ofi_total_iov_len(tx_entry->iov, tx_entry->iov_count);

	/* set flags */
	assert(ep->util_ep.tx_msg_flags == 0 ||
	       ep->util_ep.tx_msg_flags == FI_COMPLETION);
	tx_op_flags = ep->util_ep.tx_op_flags;
	if (ep->util_ep.tx_msg_flags == 0)
		tx_op_flags &= ~FI_COMPLETION;
	tx_entry->fi_flags = flags | tx_op_flags;
	tx_entry->bytes_runt = 0;
	tx_entry->max_req_data_size = 0;
	dlist_init(&tx_entry->entry);

	switch (op) {
	case ofi_op_tagged:
		tx_entry->cq_entry.flags = FI_TRANSMIT | FI_MSG | FI_TAGGED;
		break;
	case ofi_op_write:
		tx_entry->cq_entry.flags = FI_RMA | FI_WRITE;
		break;
	case ofi_op_read_req:
		tx_entry->cq_entry.flags = FI_RMA | FI_READ;
		break;
	case ofi_op_msg:
		tx_entry->cq_entry.flags = FI_TRANSMIT | FI_MSG;
		break;
	case ofi_op_atomic:
		tx_entry->cq_entry.flags = (FI_WRITE | FI_ATOMIC);
		break;
	case ofi_op_atomic_fetch:
	case ofi_op_atomic_compare:
		tx_entry->cq_entry.flags = (FI_READ | FI_ATOMIC);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_CQ, "invalid operation type\n");
		assert(0);
	}
}

/* create a new tx entry */
struct rxr_op_entry *rxr_ep_alloc_tx_entry(struct rxr_ep *rxr_ep,
					   const struct fi_msg *msg,
					   uint32_t op,
					   uint64_t tag,
					   uint64_t flags)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = ofi_buf_alloc(rxr_ep->op_entry_pool);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return NULL;
	}

	rxr_tx_entry_init(rxr_ep, tx_entry, msg, op, flags);
	if (op == ofi_op_tagged) {
		tx_entry->cq_entry.tag = tag;
		tx_entry->tag = tag;
	}

	dlist_insert_tail(&tx_entry->ep_entry, &rxr_ep->tx_entry_list);
	return tx_entry;
}

void rxr_release_tx_entry(struct rxr_ep *ep, struct rxr_op_entry *tx_entry)
{
	int i, err = 0;
	struct dlist_entry *tmp;
	struct rxr_pkt_entry *pkt_entry;

	assert(tx_entry->peer);
	dlist_remove(&tx_entry->peer_entry);

	for (i = 0; i < tx_entry->iov_count; i++) {
		if (tx_entry->mr[i]) {
			err = fi_close((struct fid *)tx_entry->mr[i]);
			if (OFI_UNLIKELY(err)) {
				FI_WARN(&rxr_prov, FI_LOG_CQ, "mr dereg failed. err=%d\n", err);
				efa_eq_write_error(&ep->util_ep, err, FI_EFA_ERR_MR_DEREG);
			}

			tx_entry->mr[i] = NULL;
		}
	}

	dlist_remove(&tx_entry->ep_entry);

	dlist_foreach_container_safe(&tx_entry->queued_pkts,
				     struct rxr_pkt_entry,
				     pkt_entry, entry, tmp) {
		rxr_pkt_entry_release_tx(ep, pkt_entry);
	}

	if (tx_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR)
		dlist_remove(&tx_entry->queued_rnr_entry);

	if (tx_entry->rxr_flags & RXR_OP_ENTRY_QUEUED_CTRL)
		dlist_remove(&tx_entry->queued_ctrl_entry);

#ifdef ENABLE_EFA_POISONING
	rxr_poison_mem_region((uint32_t *)tx_entry,
			      sizeof(struct rxr_op_entry));
#endif
	tx_entry->state = RXR_OP_FREE;
	ofi_buf_free(tx_entry);
}

/**
 * @brief convert EFA descriptors to shm descriptors.
 *
 * Each provider defines its descriptors format. The descriptor for
 * EFA provider is of struct efa_mr *, which shm provider cannot
 * understand. This function convert EFA descriptors to descriptors
 * shm can use.
 *
 * @param numdesc[in]       number of descriptors in the array
 * @param desc[in,out]      descriptors input is EFA descriptor, output
 *                          is shm descriptor.
 */
void rxr_convert_desc_for_shm(int numdesc, void **desc)
{
	int i;
	struct efa_mr *efa_mr;

	for (i = 0; i < numdesc; ++i) {
		efa_mr = desc[i];
		if (efa_mr)
			desc[i] = fi_mr_desc(efa_mr->shm_mr);
	}
}

void rxr_prepare_desc_send(struct efa_domain *efa_domain,
			   struct rxr_op_entry *tx_entry)
{
	int tx_iov_index;
	size_t tx_iov_offset;

	rxr_locate_iov_pos(tx_entry->iov,
			   tx_entry->iov_count,
			   tx_entry->bytes_sent,
			   &tx_iov_index,
			   &tx_iov_offset);

	rxr_tx_entry_try_fill_desc(tx_entry, efa_domain, tx_iov_index, FI_SEND);
}

/* Generic send */
static void rxr_ep_free_res(struct rxr_ep *rxr_ep)
{
	struct dlist_entry *entry, *tmp;
	struct rxr_op_entry *rx_entry;
	struct rxr_op_entry *tx_entry;
	struct rxr_op_entry *op_entry;
#if ENABLE_DEBUG
	struct rxr_pkt_entry *pkt;
#endif

	dlist_foreach_safe(&rxr_ep->rx_unexp_list, entry, tmp) {
		rx_entry = container_of(entry, struct rxr_op_entry, entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with unmatched unexpected rx_entry: %p pkt_entry %p\n",
			rx_entry, rx_entry->unexp_pkt);
		rxr_pkt_entry_release_rx(rxr_ep, rx_entry->unexp_pkt);
		rxr_release_rx_entry(rxr_ep, rx_entry);
	}

	dlist_foreach_safe(&rxr_ep->rx_unexp_tagged_list, entry, tmp) {
		rx_entry = container_of(entry, struct rxr_op_entry, entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with unmatched unexpected tagged rx_entry: %p pkt_entry %p\n",
			rx_entry, rx_entry->unexp_pkt);
		rxr_pkt_entry_release_rx(rxr_ep, rx_entry->unexp_pkt);
		rxr_release_rx_entry(rxr_ep, rx_entry);
	}

	dlist_foreach_safe(&rxr_ep->rx_entry_queued_rnr_list, entry, tmp) {
		rx_entry = container_of(entry, struct rxr_op_entry,
					queued_rnr_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with queued rnr rx_entry: %p\n",
			rx_entry);
		rxr_release_rx_entry(rxr_ep, rx_entry);
	}

	dlist_foreach_safe(&rxr_ep->tx_entry_queued_rnr_list, entry, tmp) {
		tx_entry = container_of(entry, struct rxr_op_entry,
					queued_rnr_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with queued rnr tx_entry: %p\n",
			tx_entry);
		rxr_release_tx_entry(rxr_ep, tx_entry);
	}

	dlist_foreach_safe(&rxr_ep->op_entry_queued_ctrl_list, entry, tmp) {
		op_entry = container_of(entry, struct rxr_op_entry,
					queued_ctrl_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with queued ctrl op_entry: %p\n",
			op_entry);
		if (op_entry->type == RXR_TX_ENTRY) {
			rxr_release_tx_entry(rxr_ep, op_entry);
		} else {
			assert(op_entry->type == RXR_RX_ENTRY);
			rxr_release_rx_entry(rxr_ep, op_entry);
		}
	}

#if ENABLE_DEBUG
	dlist_foreach_safe(&rxr_ep->rx_posted_buf_list, entry, tmp) {
		pkt = container_of(entry, struct rxr_pkt_entry, dbg_entry);
		ofi_buf_free(pkt);
	}

	if (rxr_ep->shm_ep) {
		dlist_foreach_safe(&rxr_ep->rx_posted_buf_shm_list, entry, tmp) {
			pkt = container_of(entry, struct rxr_pkt_entry, dbg_entry);
			ofi_buf_free(pkt);
		}
	}

	dlist_foreach_safe(&rxr_ep->rx_pkt_list, entry, tmp) {
		pkt = container_of(entry, struct rxr_pkt_entry, dbg_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with unreleased RX pkt_entry: %p\n",
			pkt);
		rxr_pkt_entry_release_rx(rxr_ep, pkt);
	}

	dlist_foreach_safe(&rxr_ep->tx_pkt_list, entry, tmp) {
		pkt = container_of(entry, struct rxr_pkt_entry, dbg_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with unreleased TX pkt_entry: %p\n",
			pkt);
		rxr_pkt_entry_release_tx(rxr_ep, pkt);
	}
#endif

	dlist_foreach_safe(&rxr_ep->rx_entry_list, entry, tmp) {
		rx_entry = container_of(entry, struct rxr_op_entry,
					ep_entry);
		if (!(rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED))
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Closing ep with unreleased rx_entry\n");
		rxr_release_rx_entry(rxr_ep, rx_entry);
	}

	dlist_foreach_safe(&rxr_ep->tx_entry_list, entry, tmp) {
		tx_entry = container_of(entry, struct rxr_op_entry,
					ep_entry);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Closing ep with unreleased tx_entry: %p\n",
			tx_entry);
		rxr_release_tx_entry(rxr_ep, tx_entry);
	}

	if (rxr_ep->op_entry_pool)
		ofi_bufpool_destroy(rxr_ep->op_entry_pool);

	if (rxr_ep->map_entry_pool)
		ofi_bufpool_destroy(rxr_ep->map_entry_pool);

	if (rxr_ep->read_entry_pool)
		ofi_bufpool_destroy(rxr_ep->read_entry_pool);

	if (rxr_ep->rx_readcopy_pkt_pool) {
		FI_INFO(&rxr_prov, FI_LOG_EP_CTRL, "current usage of read copy packet pool is %d\n",
			rxr_ep->rx_readcopy_pkt_pool_used);
		FI_INFO(&rxr_prov, FI_LOG_EP_CTRL, "maximum usage of read copy packet pool is %d\n",
			rxr_ep->rx_readcopy_pkt_pool_max_used);
		assert(!rxr_ep->rx_readcopy_pkt_pool_used);
		ofi_bufpool_destroy(rxr_ep->rx_readcopy_pkt_pool);
	}

	if (rxr_ep->rx_ooo_pkt_pool)
		ofi_bufpool_destroy(rxr_ep->rx_ooo_pkt_pool);

	if (rxr_ep->rx_unexp_pkt_pool)
		ofi_bufpool_destroy(rxr_ep->rx_unexp_pkt_pool);

	if (rxr_ep->efa_rx_pkt_pool)
		ofi_bufpool_destroy(rxr_ep->efa_rx_pkt_pool);

	if (rxr_ep->efa_tx_pkt_pool)
		ofi_bufpool_destroy(rxr_ep->efa_tx_pkt_pool);

	if (rxr_ep->pkt_sendv_pool)
		ofi_bufpool_destroy(rxr_ep->pkt_sendv_pool);

	if (rxr_ep->shm_rx_pkt_pool)
		ofi_bufpool_destroy(rxr_ep->shm_rx_pkt_pool);

	if (rxr_ep->shm_tx_pkt_pool)
		ofi_bufpool_destroy(rxr_ep->shm_tx_pkt_pool);
}

/*
 * @brief determine whether an endpoint has unfinished send
 *
 * Unfinished send includes queued ctrl packets, queued
 * RNR packets and inflight TX packets.
 *
 * @param[in]	rxr_ep	endpoint
 * @return	a boolean
 */
static
bool rxr_ep_has_unfinished_send(struct rxr_ep *rxr_ep)
{
	return !dlist_empty(&rxr_ep->rx_entry_queued_rnr_list) ||
	       !dlist_empty(&rxr_ep->tx_entry_queued_rnr_list) ||
	       !dlist_empty(&rxr_ep->op_entry_queued_ctrl_list) ||
	       (rxr_ep->efa_outstanding_tx_ops > 0) ||
	       (rxr_ep->shm_outstanding_tx_ops > 0);
}

/*
 * @brief wait for send to finish
 *
 * Wait for queued packet to be sent, and inflight send to
 * complete.
 *
 * @param[in]	rxr_ep		endpoint
 * @return 	no return
 */
static inline
void rxr_ep_wait_send(struct rxr_ep *rxr_ep)
{
	ofi_mutex_lock(&rxr_ep->util_ep.lock);

	while (rxr_ep_has_unfinished_send(rxr_ep)) {
		rxr_ep_progress_internal(rxr_ep);
	}

	ofi_mutex_unlock(&rxr_ep->util_ep.lock);
}

/**
 * @brief Destroy the ibv_cq_ex in rxr_ep
 * 	Will be removed when rxr_ep and efa_ep are fully separated
 *
 * @param[in] rxr_ep Pointer to the rxr_ep.
 * @return 0 on success, error code on failure
 */
static int rxr_ep_release_device_cq(struct rxr_ep *rxr_ep)
{
	struct efa_ep *efa_ep = container_of(rxr_ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	struct ibv_cq *ibv_cq = ibv_cq_ex_to_cq(rxr_ep->ibv_cq_ex);

	assert(efa_ep->scq);
	free(efa_ep->scq);

	return -ibv_destroy_cq(ibv_cq);
}

static int rxr_ep_close(struct fid *fid)
{
	int ret, retv = 0;
	struct rxr_ep *rxr_ep;

	rxr_ep = container_of(fid, struct rxr_ep, util_ep.ep_fid.fid);

	rxr_ep_wait_send(rxr_ep);

	ret = fi_close(&rxr_ep->rdm_ep->fid);
	if (ret) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close EP\n");
		retv = ret;
	}

	ret = rxr_ep_release_device_cq(rxr_ep);
	if (ret) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close ibv_cq_ex\n");
		retv = ret;
	}

	if (rxr_ep->shm_ep) {
		ret = fi_close(&rxr_ep->shm_ep->fid);
		if (ret) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close shm EP\n");
			retv = ret;
		}
	}

	if (rxr_ep->shm_cq) {
		ret = fi_close(&rxr_ep->shm_cq->fid);
		if (ret) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close shm CQ\n");
			retv = ret;
		}
	}

	ret = ofi_endpoint_close(&rxr_ep->util_ep);
	if (ret) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close util EP\n");
		retv = ret;
	}
	rxr_ep_free_res(rxr_ep);
	free(rxr_ep);
	return retv;
}

static int rxr_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct rxr_ep *rxr_ep =
		container_of(ep_fid, struct rxr_ep, util_ep.ep_fid.fid);
	struct util_cq *cq;
	struct efa_av *av;
	struct util_cntr *cntr;
	struct util_eq *eq;
	int ret = 0;

	switch (bfid->fclass) {
	case FI_CLASS_AV:
		av = container_of(bfid, struct efa_av, util_av.av_fid.fid);
		/*
		 * Binding multiple endpoints to a single AV is currently not
		 * supported.
		 */
		if (av->ep) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "Address vector already has endpoint bound to it.\n");
			return -FI_ENOSYS;
		}

		/* Bind util provider endpoint and av */
		ret = ofi_ep_bind_av(&rxr_ep->util_ep, &av->util_av);
		if (ret)
			return ret;

		ret = fi_ep_bind(rxr_ep->rdm_ep, &av->util_av.av_fid.fid, flags);
		if (ret)
			return ret;

		/* Bind shm provider endpoint & shm av */
		if (rxr_ep->shm_ep) {
			assert(av->shm_rdm_av);
			ret = fi_ep_bind(rxr_ep->shm_ep, &av->shm_rdm_av->fid, flags);
			if (ret)
				return ret;
		}
		break;
	case FI_CLASS_CQ:
		cq = container_of(bfid, struct util_cq, cq_fid.fid);

		ret = ofi_ep_bind_cq(&rxr_ep->util_ep, cq, flags);
		if (ret)
			return ret;
		break;
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct util_cntr, cntr_fid.fid);

		ret = ofi_ep_bind_cntr(&rxr_ep->util_ep, cntr, flags);
		if (ret)
			return ret;
		break;
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct util_eq, eq_fid.fid);

		ret = ofi_ep_bind_eq(&rxr_ep->util_ep, eq);
		if (ret)
			return ret;
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

/*
 * For a given peer, trigger a handshake packet and determine if both peers
 * support rdma read.
 *
 * @param[in]	ep	rxr_ep
 * @param[in]	addr	remote address
 * @param[in]	peer	remote peer
 * @return 	1 if supported, 0 if not, negative errno on error
 */
int rxr_ep_determine_rdma_support(struct rxr_ep *ep, fi_addr_t addr,
				  struct rdm_peer *peer)
{
	int ret;

	if (!peer->is_local) {
		ret = rxr_pkt_trigger_handshake(ep, addr, peer);
		if (OFI_UNLIKELY(ret))
			return ret;

		if (!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED))
			return -FI_EAGAIN;
	}

	if (!efa_both_support_rdma_read(ep, peer))
		return 0;

	return 1;
}

static
void rxr_ep_set_extra_info(struct rxr_ep *ep)
{
	memset(ep->extra_info, 0, sizeof(ep->extra_info));

	/* RDMA read is an extra feature defined in protocol version 4 (the base version) */
	if (efa_ep_support_rdma_read(ep->rdm_ep))
		ep->extra_info[0] |= RXR_EXTRA_FEATURE_RDMA_READ;

	ep->extra_info[0] |= RXR_EXTRA_FEATURE_DELIVERY_COMPLETE;

	if (ep->use_zcpy_rx) {
		/*
		 * zero copy receive requires the packet header length remains
		 * constant, so the application receive buffer is match with
		 * incoming application data.
		 */
		ep->extra_info[0] |= RXR_EXTRA_REQUEST_CONSTANT_HEADER_LENGTH;
	}

	ep->extra_info[0] |= RXR_EXTRA_REQUEST_CONNID_HEADER;

	ep->extra_info[0] |= RXR_EXTRA_FEATURE_RUNT;
}

/*
 * Set the efa_domain hmem_info state based on what device
 * capabilities are available. Return whether hmem is supported or not.
 *
 * @param[in]	efa_domain efa domain
 * @return 	1 if we have any devices that support hmem, 0 if not, negative
 * 		errno on error.
 */
static int efa_ep_hmem_check(struct efa_domain *efa_domain)
{
	int have_hmem = 0;

	if (!(efa_domain->util_domain.info_domain_caps & FI_HMEM))
		return 0;

	/*
	 * TODO: once we support other FI_HMEM p2p modes, check the setopt
	 * option first. For now, require p2p from at least one device type.
	 */
	if (efa_domain->hmem_info[FI_HMEM_CUDA].initialized &&
	    efa_domain->hmem_info[FI_HMEM_CUDA].p2p_supported)
		have_hmem = 1;
	else
		FI_INFO(&rxr_prov, FI_LOG_EP_CTRL,
			"NVIDIA GPUDirect support is not available.\n");

	if (efa_domain->hmem_info[FI_HMEM_NEURON].initialized &&
	    efa_domain->hmem_info[FI_HMEM_NEURON].p2p_supported)
		have_hmem = 1;
	else
		FI_INFO(&rxr_prov, FI_LOG_EP_CTRL,
			"AWS Neuron peer to peer support is not available.\n");

	if (efa_domain->hmem_info[FI_HMEM_SYNAPSEAI].initialized &&
	    efa_domain->hmem_info[FI_HMEM_SYNAPSEAI].p2p_supported)
		have_hmem = 1;
	else
		FI_INFO(&rxr_prov, FI_LOG_EP_CTRL,
			"AWS SynapseAI peer to peer support is not available, but FI_HMEM was requested.\n");

	if (!have_hmem) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"EFA FI_HMEM support requested but unavailable.\n");
		return -FI_EOPNOTSUPP;
	}

	return have_hmem;
}

static int rxr_ep_ctrl(struct fid *fid, int command, void *arg)
{
	ssize_t ret;
	struct rxr_ep *ep;
	struct efa_domain *efa_domain;
	char shm_ep_name[EFA_SHM_NAME_MAX], ep_addr_str[OFI_ADDRSTRLEN];
	size_t shm_ep_name_len, ep_addr_strlen;

	switch (command) {
	case FI_ENABLE:
		ep = container_of(fid, struct rxr_ep, util_ep.ep_fid.fid);
		efa_domain = rxr_ep_domain(ep);

		ret = efa_ep_hmem_check(efa_domain);
		if (ret < 0)
			return ret;

		ret = fi_enable(ep->rdm_ep);
		if (ret)
			return ret;

		ofi_mutex_lock(&ep->util_ep.lock);

		rxr_ep_set_extra_info(ep);

		ep->core_addrlen = RXR_MAX_NAME_LENGTH;
		ret = fi_getname(&ep->rdm_ep->fid,
				 ep->core_addr,
				 &ep->core_addrlen);
		assert(ret != -FI_ETOOSMALL);
		FI_DBG(&rxr_prov, FI_LOG_EP_CTRL, "core_addrlen = %ld\n",
		       ep->core_addrlen);

		ep_addr_strlen = sizeof(ep_addr_str);
		rxr_ep_raw_addr_str(ep, ep_addr_str, &ep_addr_strlen);
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "libfabric %s efa endpoint created! address: %s\n",
			fi_tostr("1", FI_TYPE_VERSION), ep_addr_str);

		/* Enable shm provider endpoint & post recv buff.
		 * Once core ep enabled, 18 bytes efa_addr (16 bytes raw + 2 bytes qpn) is set.
		 * We convert the address to 'gid_qpn' format, and set it as shm ep name, so
		 * that shm ep can create shared memory region with it when enabling.
		 * In this way, each peer is able to open and map to other local peers'
		 * shared memory region.
		 */
		if (ep->shm_ep) {
			shm_ep_name_len = EFA_SHM_NAME_MAX;
			ret = efa_shm_ep_name_construct(shm_ep_name, &shm_ep_name_len, (struct efa_ep_addr *)ep->core_addr);
			if (ret < 0)
				goto out;
			fi_setname(&ep->shm_ep->fid, shm_ep_name, shm_ep_name_len);
			ret = fi_enable(ep->shm_ep);
			if (ret)
				goto out;
		}
out:
		ofi_mutex_unlock(&ep->util_ep.lock);
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

static struct fi_ops rxr_ep_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxr_ep_close,
	.bind = rxr_ep_bind,
	.control = rxr_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

static int rxr_ep_cancel_match_recv(struct dlist_entry *item,
				    const void *context)
{
	struct rxr_op_entry *rx_entry = container_of(item,
						     struct rxr_op_entry,
						     entry);
	return rx_entry->cq_entry.op_context == context;
}

static ssize_t rxr_ep_cancel_recv(struct rxr_ep *ep,
				  struct dlist_entry *recv_list,
				  void *context)
{
	struct efa_domain *domain;
	struct dlist_entry *entry;
	struct rxr_op_entry *rx_entry;
	struct fi_cq_err_entry err_entry;
	uint32_t api_version;

	ofi_mutex_lock(&ep->util_ep.lock);
	entry = dlist_remove_first_match(recv_list,
					 &rxr_ep_cancel_match_recv,
					 context);
	if (!entry) {
		ofi_mutex_unlock(&ep->util_ep.lock);
		return 0;
	}

	rx_entry = container_of(entry, struct rxr_op_entry, entry);
	rx_entry->rxr_flags |= RXR_RECV_CANCEL;
	if (rx_entry->fi_flags & FI_MULTI_RECV &&
	    rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED) {
		if (dlist_empty(&rx_entry->multi_recv_consumers)) {
			/*
			 * No pending messages for the buffer,
			 * release it back to the app.
			 */
			rx_entry->cq_entry.flags |= FI_MULTI_RECV;
		} else {
			rx_entry = container_of(rx_entry->multi_recv_consumers.next,
						struct rxr_op_entry,
						multi_recv_entry);
			rxr_msg_multi_recv_handle_completion(ep, rx_entry);
		}
	} else if (rx_entry->fi_flags & FI_MULTI_RECV &&
		   rx_entry->rxr_flags & RXR_MULTI_RECV_CONSUMER) {
		rxr_msg_multi_recv_handle_completion(ep, rx_entry);
	}
	ofi_mutex_unlock(&ep->util_ep.lock);
	memset(&err_entry, 0, sizeof(err_entry));
	err_entry.op_context = rx_entry->cq_entry.op_context;
	err_entry.flags |= rx_entry->cq_entry.flags;
	err_entry.tag = rx_entry->tag;
	err_entry.err = FI_ECANCELED;
	err_entry.prov_errno = -FI_ECANCELED;

	domain = rxr_ep_domain(ep);
	api_version =
		 domain->util_domain.fabric->fabric_fid.api_version;
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
		err_entry.err_data_size = 0;
	/*
	 * Other states are currently receiving data. Subsequent messages will
	 * be sunk (via RXR_RECV_CANCEL flag) and the completion suppressed.
	 */
	if (rx_entry->state & (RXR_RX_INIT | RXR_RX_UNEXP | RXR_RX_MATCHED))
		rxr_release_rx_entry(ep, rx_entry);
	return ofi_cq_write_error(ep->util_ep.rx_cq, &err_entry);
}

static ssize_t rxr_ep_cancel(fid_t fid_ep, void *context)
{
	struct rxr_ep *ep;
	int ret;

	ep = container_of(fid_ep, struct rxr_ep, util_ep.ep_fid.fid);

	ret = rxr_ep_cancel_recv(ep, &ep->rx_list, context);
	if (ret)
		return ret;

	ret = rxr_ep_cancel_recv(ep, &ep->rx_tagged_list, context);
	return ret;
}

/*
 * Validate p2p opt passed by the user and set the endpoint option if it is
 * valid. If the option chosen is invalid or not supported, return an error.
 *
 * @param[in]	efa_ep	efa endpoint
 * @return 	0 on success, negative errno on error
 */
static int efa_set_fi_hmem_p2p_opt(struct efa_ep *efa_ep, int opt)
{
	struct efa_hmem_info *hmem_info;

	hmem_info = efa_ep->domain->hmem_info;

	switch (opt) {
	/*
	 * TODO: support the other options. We can only support ENABLED
	 * and PREFERRED when p2p is available. DISABLED is not
	 * supported yet.
	 */
	case FI_HMEM_P2P_REQUIRED:
	case FI_HMEM_P2P_ENABLED:
	case FI_HMEM_P2P_PREFERRED:
		if (hmem_info[FI_HMEM_CUDA].initialized &&
		    hmem_info[FI_HMEM_CUDA].p2p_supported) {
			efa_ep->hmem_p2p_opt = opt;
		} else if (hmem_info[FI_HMEM_NEURON].initialized &&
			   hmem_info[FI_HMEM_NEURON].p2p_supported) {
			/*
			 * Neuron requires p2p support and supports no
			 * other modes.
			 */
			if (opt != FI_HMEM_P2P_REQUIRED)
				return -FI_EOPNOTSUPP;
			efa_ep->hmem_p2p_opt = FI_HMEM_P2P_REQUIRED;
		} else {
			return -FI_EOPNOTSUPP;
		}

		break;
	case FI_HMEM_P2P_DISABLED:
		return -FI_EOPNOTSUPP;
		break;
	default:
		return -FI_EINVAL;
	}

	return 0;
}

static int rxr_ep_getopt(fid_t fid, int level, int optname, void *optval,
			 size_t *optlen)
{
	struct rxr_ep *rxr_ep;
	struct efa_ep *efa_ep;

	rxr_ep = container_of(fid, struct rxr_ep, util_ep.ep_fid.fid);
	efa_ep = container_of(rxr_ep->rdm_ep, struct efa_ep, util_ep.ep_fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		*(size_t *)optval = rxr_ep->min_multi_recv_size;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_EFA_RNR_RETRY:
		*(size_t *)optval = efa_ep->rnr_retry;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_FI_HMEM_P2P:
		*(int *)optval = efa_ep->hmem_p2p_opt;
		*optlen = sizeof(int);
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown endpoint option %s\n", __func__);
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

static int rxr_ep_setopt(fid_t fid, int level, int optname,
			 const void *optval, size_t optlen)
{
	struct rxr_ep *rxr_ep;
	struct efa_ep *efa_ep;
	int intval, ret;

	rxr_ep = container_of(fid, struct rxr_ep, util_ep.ep_fid.fid);
	efa_ep = container_of(rxr_ep->rdm_ep, struct efa_ep, util_ep.ep_fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		rxr_ep->min_multi_recv_size = *(size_t *)optval;
		break;
	case FI_OPT_EFA_RNR_RETRY:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		/*
		 * Application is required to call to fi_setopt before EP
		 * enabled. If it's calling to fi_setopt after EP enabled,
		 * fail the call.
		 *
		 * efa_ep->qp will be NULL before EP enabled, use it to check
		 * if the call to fi_setopt is before or after EP enabled for
		 * convience, instead of calling to ibv_query_qp
		 */
		if (efa_ep->qp) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"The option FI_OPT_EFA_RNR_RETRY is required \
				to be set before EP enabled %s\n", __func__);
			return -FI_EINVAL;
		}

		if (!efa_ep_support_rnr_retry_modify(rxr_ep->rdm_ep)) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"RNR capability is not supported %s\n", __func__);
			return -FI_ENOSYS;
		}
		efa_ep->rnr_retry = *(size_t *)optval;
		break;
	case FI_OPT_FI_HMEM_P2P:
		if (optlen != sizeof(int))
			return -FI_EINVAL;

		intval = *(int *)optval;

		ret = efa_set_fi_hmem_p2p_opt(efa_ep, intval);
		if (ret)
			return ret;
		break;
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown endpoint option %s\n", __func__);
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

static struct fi_ops_ep rxr_ops_ep = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = rxr_ep_cancel,
	.getopt = rxr_ep_getopt,
	.setopt = rxr_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static int rxr_buf_region_alloc_hndlr(struct ofi_bufpool_region *region)
{
	size_t ret;
	struct fid_mr *mr;
	struct efa_domain *domain = region->pool->attr.context;

	ret = fi_mr_reg(&domain->util_domain.domain_fid, region->alloc_region,
			region->pool->alloc_size,
			FI_SEND | FI_RECV, 0, 0, 0, &mr, NULL);

	region->context = mr;
	return ret;
}

static void rxr_buf_region_free_hndlr(struct ofi_bufpool_region *region)
{
	ssize_t ret;

	ret = fi_close((struct fid *)region->context);
	if (ret)
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unable to deregister memory in a buf pool: %s\n",
			fi_strerror(-ret));
}

/*
 * rxr_create_pkt_pool create a packet pool. The size of pool is fixed
 * and the memory is registered with device.
 *
 * Important arguments:
 *      size: packet entry size
 *      flags: caller can specify OFI_BUFPOOL_HUGEPAGES so the pool
 *             will be backed by huge pages.
 */
static int rxr_create_pkt_pool(struct rxr_ep *ep, size_t size,
			       size_t chunk_count,
			       size_t flags,
			       struct ofi_bufpool **buf_pool)
{
	struct ofi_bufpool_attr attr = {
		.size		= size,
		.alignment	= RXR_BUF_POOL_ALIGNMENT,
		.max_cnt	= chunk_count,
		.chunk_cnt	= chunk_count,
		.alloc_fn	= rxr_buf_region_alloc_hndlr,
		.free_fn	= rxr_buf_region_free_hndlr,
		.init_fn	= NULL,
		.context	= rxr_ep_domain(ep),
		.flags		= flags,
	};

	return ofi_bufpool_create_attr(&attr, buf_pool);
}

/** @brief Initializes the endpoint.
 *
 * This function allocates the various buffer pools for the EFA and SHM
 * provider and does other endpoint initialization.
 *
 * @param ep rxr_ep struct to initialize.
 * @return 0 on success, fi_errno on error.
 */
int rxr_ep_init(struct rxr_ep *ep)
{
	size_t entry_sz, sendv_pool_size;
	int pkt_pool_flags;
	int ret;

	entry_sz = ep->mtu_size + sizeof(struct rxr_pkt_entry);
#ifdef ENABLE_EFA_POISONING
	ep->tx_pkt_pool_entry_sz = entry_sz;
	ep->rx_pkt_pool_entry_sz = entry_sz;
#endif

	if (g_efa_fork_status == EFA_FORK_SUPPORT_ON) {
		/*
		 * Make sure that no data structures can share the memory pages used
		 * for this buffer pool.
		 * When fork support is on, registering a buffer with ibv_reg_mr will
		 * set MADV_DONTFORK on the underlying pages.  After fork() the child
		 * process will not have a page mapping at that address.
		 */
		pkt_pool_flags = OFI_BUFPOOL_NONSHARED;
	} else {
		pkt_pool_flags = OFI_BUFPOOL_HUGEPAGES;
	}

	ret = rxr_create_pkt_pool(ep, entry_sz, rxr_get_tx_pool_chunk_cnt(ep),
				  pkt_pool_flags,
				  &ep->efa_tx_pkt_pool);
	if (ret)
		goto err_free;

	ret = rxr_create_pkt_pool(ep, entry_sz, rxr_get_rx_pool_chunk_cnt(ep),
				  pkt_pool_flags,
				  &ep->efa_rx_pkt_pool);
	if (ret)
		goto err_free;

	if (rxr_env.rx_copy_unexp) {
		ret = ofi_bufpool_create(&ep->rx_unexp_pkt_pool, entry_sz,
					 RXR_BUF_POOL_ALIGNMENT, 0,
					 rxr_env.unexp_pool_chunk_size, 0);

		if (ret)
			goto err_free;
	}

	if (rxr_env.rx_copy_ooo) {
		ret = ofi_bufpool_create(&ep->rx_ooo_pkt_pool, entry_sz,
					 RXR_BUF_POOL_ALIGNMENT, 0,
					 rxr_env.ooo_pool_chunk_size, 0);

		if (ret)
			goto err_free;

	}

	if ((rxr_env.rx_copy_unexp || rxr_env.rx_copy_ooo) &&
	    (rxr_ep_domain(ep)->util_domain.mr_mode & FI_MR_HMEM)) {
		/* this pool is only needed when application requested FI_HMEM
		 * capability
		 */
		ret = rxr_create_pkt_pool(ep, entry_sz,
					  rxr_env.readcopy_pool_size,
					  pkt_pool_flags,
					  &ep->rx_readcopy_pkt_pool);

		if (ret)
			goto err_free;
		ep->rx_readcopy_pkt_pool_used = 0;
		ep->rx_readcopy_pkt_pool_max_used = 0;
	}

	ret = ofi_bufpool_create(&ep->read_entry_pool,
				 sizeof(struct rxr_read_entry),
				 RXR_BUF_POOL_ALIGNMENT,
				 ep->tx_size + RXR_MAX_RX_QUEUE_SIZE,
				 ep->tx_size + ep->rx_size, 0);
	if (ret)
		goto err_free;

	ret = ofi_bufpool_create(&ep->map_entry_pool,
				 sizeof(struct rxr_pkt_rx_map),
				 RXR_BUF_POOL_ALIGNMENT,
				 RXR_MAX_RX_QUEUE_SIZE,
				 ep->rx_size, 0);

	if (ret)
		goto err_free;

	ret = ofi_bufpool_create(&ep->rx_atomrsp_pool,
				 ep->mtu_size,
				 RXR_BUF_POOL_ALIGNMENT,
				 RXR_MAX_RX_QUEUE_SIZE,
				 rxr_env.atomrsp_pool_size, 0);
	if (ret)
		goto err_free;

	ret = ofi_bufpool_create(&ep->op_entry_pool,
				 sizeof(struct rxr_op_entry),
				 RXR_BUF_POOL_ALIGNMENT,
				 RXR_MAX_RX_QUEUE_SIZE,
				 ep->tx_size + ep->rx_size, 0);
	if (ret)
		goto err_free;

	sendv_pool_size = rxr_get_tx_pool_chunk_cnt(ep);
	if (ep->use_shm_for_tx)
		sendv_pool_size += g_shm_info->tx_attr->size;
	ret = ofi_bufpool_create(&ep->pkt_sendv_pool,
				 sizeof(struct rxr_pkt_sendv),
				 RXR_BUF_POOL_ALIGNMENT,
				 sendv_pool_size,
				 sendv_pool_size, 0);
	if (ret)
		goto err_free;

	/* create pkt pool for shm */
	if (ep->use_shm_for_tx) {
		ret = ofi_bufpool_create(&ep->shm_tx_pkt_pool,
					 entry_sz,
					 RXR_BUF_POOL_ALIGNMENT,
					 g_shm_info->tx_attr->size,
					 g_shm_info->tx_attr->size, 0);
		if (ret)
			goto err_free;
	}

	if (ep->shm_ep) {
		ret = ofi_bufpool_create(&ep->shm_rx_pkt_pool,
					 entry_sz,
					 RXR_BUF_POOL_ALIGNMENT,
					 g_shm_info->rx_attr->size,
					 g_shm_info->rx_attr->size, 0);
		if (ret)
			goto err_free;

		dlist_init(&ep->rx_posted_buf_shm_list);
	}

	/* Initialize entry list */
	dlist_init(&ep->rx_list);
	dlist_init(&ep->rx_unexp_list);
	dlist_init(&ep->rx_tagged_list);
	dlist_init(&ep->rx_unexp_tagged_list);
	dlist_init(&ep->rx_posted_buf_list);
	dlist_init(&ep->rx_entry_queued_rnr_list);
	dlist_init(&ep->tx_entry_queued_rnr_list);
	dlist_init(&ep->op_entry_queued_ctrl_list);
	dlist_init(&ep->op_entry_longcts_send_list);
	dlist_init(&ep->read_pending_list);
	dlist_init(&ep->peer_backoff_list);
	dlist_init(&ep->handshake_queued_peer_list);
#if ENABLE_DEBUG
	dlist_init(&ep->op_entry_recv_list);
	dlist_init(&ep->rx_pkt_list);
	dlist_init(&ep->tx_pkt_list);
#endif
	dlist_init(&ep->rx_entry_list);
	dlist_init(&ep->tx_entry_list);

	/* Initialize pkt to rx map */
	ep->pkt_rx_map = NULL;
	return 0;

err_free:
	if (ep->shm_tx_pkt_pool)
		ofi_bufpool_destroy(ep->shm_tx_pkt_pool);

	if (ep->pkt_sendv_pool)
		ofi_bufpool_destroy(ep->pkt_sendv_pool);

	if (ep->rx_atomrsp_pool)
		ofi_bufpool_destroy(ep->rx_atomrsp_pool);

	if (ep->map_entry_pool)
		ofi_bufpool_destroy(ep->map_entry_pool);

	if (ep->read_entry_pool)
		ofi_bufpool_destroy(ep->read_entry_pool);

	if (ep->op_entry_pool)
		ofi_bufpool_destroy(ep->op_entry_pool);

	if (ep->rx_readcopy_pkt_pool)
		ofi_bufpool_destroy(ep->rx_readcopy_pkt_pool);

	if (rxr_env.rx_copy_ooo && ep->rx_ooo_pkt_pool)
		ofi_bufpool_destroy(ep->rx_ooo_pkt_pool);

	if (rxr_env.rx_copy_unexp && ep->rx_unexp_pkt_pool)
		ofi_bufpool_destroy(ep->rx_unexp_pkt_pool);

	if (ep->efa_rx_pkt_pool)
		ofi_bufpool_destroy(ep->efa_rx_pkt_pool);

	if (ep->efa_tx_pkt_pool)
		ofi_bufpool_destroy(ep->efa_tx_pkt_pool);

	return ret;
}

static int rxr_ep_rdm_setname(fid_t fid, void *addr, size_t addrlen)
{
	struct rxr_ep *ep;

	ep = container_of(fid, struct rxr_ep, util_ep.ep_fid.fid);
	return fi_setname(&ep->rdm_ep->fid, addr, addrlen);
}

static int rxr_ep_rdm_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct rxr_ep *ep;

	ep = container_of(fid, struct rxr_ep, util_ep.ep_fid.fid);
	return fi_getname(&ep->rdm_ep->fid, addr, addrlen);
}

struct fi_ops_cm rxr_ep_cm = {
	.size = sizeof(struct fi_ops_cm),
	.setname = rxr_ep_rdm_setname,
	.getname = rxr_ep_rdm_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

/*
 * @brief explicitly allocate a chunk of memory for 5 packet pools on RX side:
 *     efa's receive packet pool (efa_rx_pkt_pool)
 *     shm's receive packet pool (shm_rx_pkt_pool)
 *     unexpected packet pool (rx_unexp_pkt_pool),
 *     out-of-order packet pool (rx_ooo_pkt_pool), and
 *     local read-copy packet pool (rx_readcopy_pkt_pool).
 *
 * @param ep[in]	endpoint
 * @return		On success, return 0
 * 			On failure, return a negative error code.
 */
int rxr_ep_grow_rx_pkt_pools(struct rxr_ep *ep)
{
	int err;

	assert(ep->efa_rx_pkt_pool);
	err = ofi_bufpool_grow(ep->efa_rx_pkt_pool);
	if (err) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"cannot allocate memory for EFA's RX packet pool. error: %s\n",
			strerror(-err));
		return err;
	}

	if (ep->shm_rx_pkt_pool) {
		err = ofi_bufpool_grow(ep->shm_rx_pkt_pool);
		if (err) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"cannot allocate memory for SHM's RX packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	if (ep->rx_unexp_pkt_pool) {
		assert(ep->rx_unexp_pkt_pool);
		err = ofi_bufpool_grow(ep->rx_unexp_pkt_pool);
		if (err) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"cannot allocate memory for unexpected packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	if (ep->rx_ooo_pkt_pool) {
		assert(ep->rx_ooo_pkt_pool);
		err = ofi_bufpool_grow(ep->rx_ooo_pkt_pool);
		if (err) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"cannot allocate memory for out-of-order packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	if (ep->rx_readcopy_pkt_pool) {
		err = ofi_bufpool_grow(ep->rx_readcopy_pkt_pool);
		if (err) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"cannot allocate and register memory for readcopy packet pool. error: %s\n",
				strerror(-err));
			return err;
		}
	}

	return 0;
}

/**
 * @brief post internal receive buffers for progress engine.
 *
 * It is more efficient to post multiple receive buffers
 * to the device at once than to post each receive buffer
 * individually.
 *
 * Therefore, after an internal receive buffer (a packet
 * entry) was processed, it is not posted to the device
 * right away.
 *
 * Instead, we increase counter
 *      ep->efa/shm_rx_pkts_to_post
 * by one.
 *
 * Later, progress engine calls this function to
 * bulk post internal receive buffers (according to
 * the counter).
 *
 * This function also control number of internal
 * buffers posted to the device in zero copy receive
 * mode.
 *
 * param[in]	ep	endpoint
 */
static inline
void rxr_ep_progress_post_internal_rx_pkts(struct rxr_ep *ep)
{
	int err;

	if (ep->use_zcpy_rx) {
		/*
		 * In zero copy receive mode,
		 *
		 * If application did not post any receive buffer,
		 * we post one internal buffer so endpoint can
		 * receive RxR control packets such as handshake.
		 *
		 * If buffers have posted to the device, we do NOT
		 * repost internal buffers to maximize the chance
		 * user buffer is used to receive data.
		 */
		if (ep->efa_rx_pkts_posted == 0 && ep->efa_rx_pkts_to_post == 0) {
			ep->efa_rx_pkts_to_post = 1;
		} else if (ep->efa_rx_pkts_posted > 0 && ep->efa_rx_pkts_to_post > 0){
			ep->efa_rx_pkts_to_post = 0;
		}
	} else {
		if (ep->efa_rx_pkts_posted == 0 && ep->efa_rx_pkts_to_post == 0) {
			/* Both efa_rx_pkts_posted and efa_rx_pkts_to_post equal to 0 means
			 * this is the first call of the progress engine on this endpoint.
			 *
			 * In this case, we explictly allocate the 1st chunk of memory
			 * for unexp/ooo/readcopy RX packet pool.
			 *
			 * The reason to explicitly allocate the memory for RX packet
			 * pool is to improve efficiency.
			 *
			 * Without explicit memory allocation, a pkt pools's memory
			 * is allocated when 1st packet is allocated from it.
			 * During the computation, different processes got their 1st
			 * unexp/ooo/read-copy packet at different time. Therefore,
			 * if we do not explicitly allocate memory at the beginning,
			 * memory will be allocated at different time.
			 *
			 * When one process is allocating memory, other processes
			 * have to wait. When each process allocate memory at different
			 * time, the accumulated waiting time became significant.
			 *
			 * By explicitly allocating memory at 1st call to progress
			 * engine, the memory allocation is parallelized.
			 * (This assumes the 1st call to the progress engine on
			 * all processes happen at roughly the same time, which
			 * is a valid assumption according to our knowledge of
			 * the workflow of most application)
			 *
			 * The memory was not allocated during endpoint initialization
			 * because some applications will initialize some endpoints
			 * but never uses it, thus allocating memory initialization
			 * causes waste.
			 */
			err = rxr_ep_grow_rx_pkt_pools(ep);
			if (err)
				goto err_exit;

			ep->efa_rx_pkts_to_post = rxr_get_rx_pool_chunk_cnt(ep);

			if (ep->shm_ep) {
				assert(ep->shm_rx_pkts_posted == 0 && ep->shm_rx_pkts_to_post == 0);
				ep->shm_rx_pkts_to_post = g_shm_info->rx_attr->size;
			}
		}
	}

	err = rxr_ep_bulk_post_internal_rx_pkts(ep, ep->efa_rx_pkts_to_post, EFA_EP);
	if (err)
		goto err_exit;

	ep->efa_rx_pkts_to_post = 0;

	if (ep->shm_ep) {
		err = rxr_ep_bulk_post_internal_rx_pkts(ep, ep->shm_rx_pkts_to_post, SHM_EP);
		if (err)
			goto err_exit;

		ep->shm_rx_pkts_to_post = 0;
	}

	return;

err_exit:

	efa_eq_write_error(&ep->util_ep, err, FI_EFA_ERR_INTERNAL_RX_BUF_POST);
}

static inline ssize_t rxr_ep_send_queued_pkts(struct rxr_ep *ep,
					      struct dlist_entry *pkts)
{
	struct dlist_entry *tmp;
	struct rxr_pkt_entry *pkt_entry;
	ssize_t ret;

	dlist_foreach_container_safe(pkts, struct rxr_pkt_entry,
				     pkt_entry, entry, tmp) {
		if (ep->use_shm_for_tx && rxr_ep_get_peer(ep, pkt_entry->addr)->is_local) {
			dlist_remove(&pkt_entry->entry);
			continue;
		}

		/* If send succeeded, pkt_entry->entry will be added
		 * to peer->outstanding_tx_pkts. Therefore, it must
		 * be removed from the list before send.
		 */
		dlist_remove(&pkt_entry->entry);

		ret = rxr_pkt_entry_send(ep, pkt_entry, 0);
		if (ret) {
			if (ret == -FI_EAGAIN) {
				/* add the pkt back to pkts, so it can be resent again */
				dlist_insert_tail(&pkt_entry->entry, pkts);
			}

			return ret;
		}
	}
	return 0;
}

static inline void rxr_ep_check_peer_backoff_timer(struct rxr_ep *ep)
{
	struct rdm_peer *peer;
	struct dlist_entry *tmp;

	if (OFI_LIKELY(dlist_empty(&ep->peer_backoff_list)))
		return;

	dlist_foreach_container_safe(&ep->peer_backoff_list, struct rdm_peer,
				     peer, rnr_backoff_entry, tmp) {
		if (ofi_gettime_us() >= peer->rnr_backoff_begin_ts +
					peer->rnr_backoff_wait_time) {
			peer->flags &= ~RXR_PEER_IN_BACKOFF;
			dlist_remove(&peer->rnr_backoff_entry);
		}
	}
}

#if HAVE_EFADV_CQ_EX
/**
 * @brief Read peer raw address from EFA device and look up the peer address in AV.
 * This function should only be called if the peer AH is unknown.
 * @return Peer address, or FI_ADDR_NOTAVAIL if unavailable.
 */
static inline fi_addr_t rdm_ep_determine_peer_address_from_efadv(struct rxr_ep *ep,
																 struct ibv_cq_ex *ibv_cqx)
{
	struct rxr_pkt_entry *pkt_entry;
	struct efa_ep *efa_ep;
	struct efa_ep_addr efa_ep_addr = {0};
	fi_addr_t addr;
	union ibv_gid gid = {0};
	uint32_t *connid = NULL;

	if (ep->ibv_cq_ex_type != EFADV_CQ) {
		/* EFA DV CQ is not supported. This could be due to old EFA kernel module versions. */
		return FI_ADDR_NOTAVAIL;
	}

	/* Attempt to read sgid from EFA firmware */
	if (efadv_wc_read_sgid(efadv_cq_from_ibv_cq_ex(ibv_cqx), &gid) < 0) {
		/* Return code is negative if the peer AH is known */
		return FI_ADDR_NOTAVAIL;
	}

	pkt_entry = (void *)(uintptr_t)ibv_cqx->wr_id;

	connid = rxr_pkt_connid_ptr(pkt_entry);
	if (!connid) {
		return FI_ADDR_NOTAVAIL;
	}

	/*
	 * Use raw:qpn:connid as the key to lookup AV for peer's fi_addr
	 */
	memcpy(efa_ep_addr.raw, gid.raw, sizeof(efa_ep_addr.raw));
	efa_ep_addr.qpn = ibv_wc_read_src_qp(ibv_cqx);
	efa_ep_addr.qkey = *connid;
	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	addr = ofi_av_lookup_fi_addr(&efa_ep->av->util_av, &efa_ep_addr);
	if (addr != FI_ADDR_NOTAVAIL) {
		char gid_str_cdesc[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, gid.raw, gid_str_cdesc, INET6_ADDRSTRLEN);
		FI_WARN(&rxr_prov, FI_LOG_AV,
				"Recovered peer fi_addr. [Raw]:[QPN]:[QKey] = [%s]:[%" PRIu16 "]:[%" PRIu32 "]\n",
				gid_str_cdesc, efa_ep_addr.qpn, efa_ep_addr.qkey);
	}

	return addr;
}

/**
 * @brief Determine peer address from ibv_cq_ex
 * Attempt to inject or determine peer address if not available. This usually
 * happens when the endpoint receives the first packet from a new peer.
 * There is an edge case for EFA endpoint - the device might lose the address
 * handle of a known peer due to a firmware bug and return FI_ADDR_NOTAVAIL.
 * The provider needs to look up the address using Raw address:QPN:QKey.
 * Note: This function introduces addtional overhead. It should only be called if
 * efa_av_lookup_address_rdm fails to find the peer address.
 * @param ep Pointer to RDM endpoint
 * @param ibv_cqx Pointer to CQ
 * @returns Peer address, or FI_ADDR_NOTAVAIL if unsuccessful.
 */
static inline fi_addr_t rdm_ep_determine_addr_from_ibv_cq_ex(struct rxr_ep *ep, struct ibv_cq_ex *ibv_cqx)
{
	struct rxr_pkt_entry *pkt_entry;
	fi_addr_t addr = FI_ADDR_NOTAVAIL;

	pkt_entry = (void *)(uintptr_t)ibv_cqx->wr_id;

	addr = rxr_pkt_determine_addr(ep, pkt_entry);

	if (addr == FI_ADDR_NOTAVAIL) {
		addr = rdm_ep_determine_peer_address_from_efadv(ep, ibv_cqx);
	}

	return addr;
}
#else
/**
 * @brief Determine peer address from ibv_cq_ex
 * Attempt to inject peer address if not available. This usually
 * happens when the endpoint receives the first packet from a new peer.
 * Note: This function introduces addtional overhead. It should only be called if
 * efa_av_lookup_address_rdm fails to find the peer address.
 * @param ep Pointer to RDM endpoint
 * @param ibv_cqx Pointer to CQ
 * @returns Peer address, or FI_ADDR_NOTAVAIL if unsuccessful.
 */
static inline fi_addr_t rdm_ep_determine_addr_from_ibv_cq_ex(struct rxr_ep *ep, struct ibv_cq_ex *ibv_cqx)
{
	struct rxr_pkt_entry *pkt_entry;

	pkt_entry = (void *)(uintptr_t)ibv_cqx->wr_id;

	return rxr_pkt_determine_addr(ep, pkt_entry);
}
#endif

/**
 * @brief poll rdma-core cq and process the cq entry
 *
 * @param[in]	ep	RDM endpoint
 * @param[in]	cqe_to_process	Max number of cq entry to poll and process. Must be positive.
 */
static inline void rdm_ep_poll_ibv_cq_ex(struct rxr_ep *ep, size_t cqe_to_process)
{
	bool should_end_poll = false;
	/* Initialize an empty ibv_poll_cq_attr struct for ibv_start_poll.
	 * EFA expects .comp_mask = 0, or otherwise returns EINVAL.
	 */
	struct ibv_poll_cq_attr poll_cq_attr = {.comp_mask = 0};
	struct efa_av *efa_av;
	struct efa_ep *efa_ep;
	struct rxr_pkt_entry *pkt_entry;
	ssize_t err;
	size_t i = 0;
	int prov_errno;

	assert(cqe_to_process > 0);

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	efa_av = efa_ep->av;

	/* Call ibv_start_poll only once */
	err = ibv_start_poll(ep->ibv_cq_ex, &poll_cq_attr);
	should_end_poll = !err;

	while (!err) {
		if (ep->ibv_cq_ex->status) {
			pkt_entry = (void *)(uintptr_t)ep->ibv_cq_ex->wr_id;
			prov_errno = ibv_wc_read_vendor_err(ep->ibv_cq_ex);
			if (ibv_wc_read_opcode(ep->ibv_cq_ex) == IBV_WC_SEND) {
#if ENABLE_DEBUG
				ep->failed_send_comps++;
#endif
				rxr_pkt_handle_send_error(ep, pkt_entry, FI_EIO, prov_errno);
			} else {
				assert(ibv_wc_read_opcode(ep->ibv_cq_ex) == IBV_WC_RECV);
				rxr_pkt_handle_recv_error(ep, pkt_entry, FI_EIO, prov_errno);
			}
			break;
		}

		pkt_entry = (void *)(uintptr_t)ep->ibv_cq_ex->wr_id;

		switch (ibv_wc_read_opcode(ep->ibv_cq_ex)) {
		case IBV_WC_SEND:
#if ENABLE_DEBUG
			ep->send_comps++;
#endif
			rxr_pkt_handle_send_completion(ep, pkt_entry);
			break;
		case IBV_WC_RECV:
			pkt_entry->addr = efa_av_reverse_lookup_rdm(efa_av, ibv_wc_read_slid(ep->ibv_cq_ex),
								ibv_wc_read_src_qp(ep->ibv_cq_ex), pkt_entry);

			if (pkt_entry->addr == FI_ADDR_NOTAVAIL) {
				pkt_entry->addr = rdm_ep_determine_addr_from_ibv_cq_ex(ep, ep->ibv_cq_ex);
			}

			pkt_entry->pkt_size = ibv_wc_read_byte_len(ep->ibv_cq_ex);
			assert(pkt_entry->pkt_size > 0);
			rxr_pkt_handle_recv_completion(ep, pkt_entry, EFA_EP);
#if ENABLE_DEBUG
			ep->recv_comps++;
#endif
			break;
		default:
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Unhandled cq type\n");
			assert(0 && "Unhandled cq type");
		}

		i++;
		if (i == cqe_to_process) {
			break;
		}

		/*
		 * ibv_next_poll MUST be call after the current WC is fully processed,
		 * which prevents later calls on ibv_cq_ex from reading the wrong WC.
		 */
		err = ibv_next_poll(ep->ibv_cq_ex);
	}

	if (err && err != ENOENT) {
		err = err > 0 ? err : -err;
		prov_errno = ibv_wc_read_vendor_err(ep->ibv_cq_ex);
		efa_eq_write_error(&ep->util_ep, err, prov_errno);
	}

	if (should_end_poll)
		ibv_end_poll(ep->ibv_cq_ex);
}

static inline
void rdm_ep_poll_shm_err_cq(struct fid_cq *shm_cq, struct fi_cq_err_entry *cq_err_entry)
{
	int ret;

	ret = fi_cq_readerr(shm_cq, cq_err_entry, 0);
	if (ret == 1)
		return;

	if (ret < 0) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "encountered error when fi_cq_readerr: %s\n",
			fi_strerror(-ret));
		cq_err_entry->err = -ret;
		cq_err_entry->prov_errno = FI_EFA_ERR_SHM_INTERNAL_ERROR;
		return;
	}

	FI_WARN(&rxr_prov, FI_LOG_CQ, "fi_cq_readerr got expected return: %d\n", ret);
	cq_err_entry->err = FI_EIO;
	cq_err_entry->prov_errno = FI_EFA_ERR_SHM_INTERNAL_ERROR;
}

static inline void rdm_ep_poll_shm_cq(struct rxr_ep *ep,
				      size_t cqe_to_process)
{
	struct fi_cq_data_entry cq_entry;
	struct fi_cq_err_entry cq_err_entry = { 0 };
	struct rxr_pkt_entry *pkt_entry;
	fi_addr_t src_addr;
	ssize_t ret;
	struct efa_ep *efa_ep;
	struct efa_av *efa_av;
	int i;

	VALGRIND_MAKE_MEM_DEFINED(&cq_entry, sizeof(struct fi_cq_data_entry));

	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	efa_av = efa_ep->av;
	for (i = 0; i < cqe_to_process; i++) {
		ret = fi_cq_readfrom(ep->shm_cq, &cq_entry, 1, &src_addr);

		if (ret == -FI_EAGAIN)
			return;

		if (OFI_UNLIKELY(ret < 0)) {
			if (ret != -FI_EAVAIL) {
				efa_eq_write_error(&ep->util_ep, -ret, FI_EFA_ERR_SHM_INTERNAL_ERROR);
				return;
			}

			rdm_ep_poll_shm_err_cq(ep->shm_cq, &cq_err_entry);
			if (cq_err_entry.flags & (FI_SEND | FI_READ | FI_WRITE)) {
				assert(cq_entry.op_context);
				rxr_pkt_handle_send_error(ep, cq_entry.op_context, cq_err_entry.err, cq_err_entry.prov_errno);
			} else if (cq_err_entry.flags & FI_RECV) {
				assert(cq_entry.op_context);
				rxr_pkt_handle_recv_error(ep, cq_entry.op_context, cq_err_entry.err, cq_err_entry.prov_errno);
			} else {
				efa_eq_write_error(&ep->util_ep, cq_err_entry.err, cq_err_entry.prov_errno);
			}

			return;
		}

		if (OFI_UNLIKELY(ret == 0))
			return;

		pkt_entry = cq_entry.op_context;
		if (src_addr != FI_ADDR_UNSPEC) {
			/* convert SHM address to EFA address */
			assert(src_addr < EFA_SHM_MAX_AV_COUNT);
			src_addr = efa_av->shm_rdm_addr_map[src_addr];
		}

		if (cq_entry.flags & (FI_ATOMIC | FI_REMOTE_CQ_DATA)) {
			rxr_cq_handle_shm_completion(ep, &cq_entry, src_addr);
		} else if (cq_entry.flags & (FI_SEND | FI_READ | FI_WRITE)) {
			rxr_pkt_handle_send_completion(ep, pkt_entry);
		} else if (cq_entry.flags & (FI_RECV | FI_REMOTE_CQ_DATA)) {
			pkt_entry->addr = src_addr;

			if (pkt_entry->addr == FI_ADDR_NOTAVAIL) {
				/*
				 * Attempt to inject or determine peer address if not available. This usually
				 * happens when the endpoint receives the first packet from a new peer.
				 */
				pkt_entry->addr = rxr_pkt_determine_addr(ep, pkt_entry);
			}

			pkt_entry->pkt_size = cq_entry.len;
			assert(pkt_entry->pkt_size > 0);
			rxr_pkt_handle_recv_completion(ep, pkt_entry, SHM_EP);
		} else {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Unhandled cq type\n");
			assert(0 && "Unhandled cq type");
		}
	}
}

void rxr_ep_progress_internal(struct rxr_ep *ep)
{
	struct ibv_send_wr *bad_wr;
	struct efa_ep *efa_ep;
	struct rxr_op_entry *rx_entry;
	struct rxr_op_entry *tx_entry;
	struct rxr_op_entry *op_entry;
	struct rxr_read_entry *read_entry;
	struct rdm_peer *peer;
	struct dlist_entry *tmp;
	ssize_t ret;
	uint64_t flags;

	/* Poll the EFA completion queue. Restrict poll size
	 * to avoid CQE flooding and thereby blocking user thread. */
	rdm_ep_poll_ibv_cq_ex(ep, rxr_env.efa_cq_read_size);

	if (ep->shm_cq) {
		// Poll the SHM completion queue
		rdm_ep_poll_shm_cq(ep, rxr_env.shm_cq_read_size);
	}

	rxr_ep_progress_post_internal_rx_pkts(ep);

	rxr_ep_check_peer_backoff_timer(ep);
	/*
	 * Resend handshake packet for any peers where the first
	 * handshake send failed.
	 */
	dlist_foreach_container_safe(&ep->handshake_queued_peer_list,
				     struct rdm_peer, peer,
				     handshake_queued_entry, tmp) {
		if (peer->flags & RXR_PEER_IN_BACKOFF)
			continue;

		ret = rxr_pkt_post_handshake(ep, peer);
		if (ret == -FI_EAGAIN)
			break;

		if (OFI_UNLIKELY(ret)) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Failed to post HANDSHAKE to peer %ld: %s\n",
				peer->efa_fiaddr, fi_strerror(-ret));
			efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_PEER_HANDSHAKE);
			return;
		}

		dlist_remove(&peer->handshake_queued_entry);
		peer->flags &= ~RXR_PEER_HANDSHAKE_QUEUED;
		peer->flags |= RXR_PEER_HANDSHAKE_SENT;
	}

	/*
	 * Send any queued ctrl packets.
	 */
	dlist_foreach_container_safe(&ep->rx_entry_queued_rnr_list,
				     struct rxr_op_entry,
				     rx_entry, queued_rnr_entry, tmp) {
		peer = rxr_ep_get_peer(ep, rx_entry->addr);
		assert(peer);

		if (peer->flags & RXR_PEER_IN_BACKOFF)
			continue;

		assert(rx_entry->rxr_flags & RXR_RX_ENTRY_QUEUED_RNR);
		assert(!dlist_empty(&rx_entry->queued_pkts));
		ret = rxr_ep_send_queued_pkts(ep, &rx_entry->queued_pkts);

		if (ret == -FI_EAGAIN)
			break;

		if (OFI_UNLIKELY(ret)) {
			rxr_cq_write_rx_error(ep, rx_entry, -ret, FI_EFA_ERR_PKT_SEND);
			return;
		}

		dlist_remove(&rx_entry->queued_rnr_entry);
		rx_entry->rxr_flags &= ~RXR_RX_ENTRY_QUEUED_RNR;
	}

	dlist_foreach_container_safe(&ep->op_entry_queued_ctrl_list,
				     struct rxr_op_entry,
				     op_entry, queued_ctrl_entry, tmp) {
		peer = rxr_ep_get_peer(ep, rx_entry->addr);
		assert(peer);

		if (peer->flags & RXR_PEER_IN_BACKOFF)
			continue;

		assert(op_entry->rxr_flags & RXR_OP_ENTRY_QUEUED_CTRL);
		ret = rxr_pkt_post(ep, op_entry, op_entry->queued_ctrl.type,
				   op_entry->queued_ctrl.inject, 0);
		if (ret == -FI_EAGAIN)
			break;

		if (OFI_UNLIKELY(ret)) {
			rxr_cq_write_rx_error(ep, op_entry, -ret, FI_EFA_ERR_PKT_POST);
			return;
		}

		/* it can happen that rxr_pkt_post() released op_entry
		 * (if the op_entry is rx_entry and packet type is EOR and inject is used). In
		 * that case rx_entry's state has been set to RXR_OP_FREE and
		 * it has been removed from ep->op_queued_entry_list, so nothing
		 * is left to do.
		 */
		if (op_entry->state == RXR_OP_FREE)
			continue;

		op_entry->rxr_flags &= ~RXR_OP_ENTRY_QUEUED_CTRL;
		dlist_remove(&op_entry->queued_ctrl_entry);
	}

	dlist_foreach_container_safe(&ep->tx_entry_queued_rnr_list,
				     struct rxr_op_entry,
				     tx_entry, queued_rnr_entry, tmp) {
		peer = rxr_ep_get_peer(ep, tx_entry->addr);
		assert(peer);

		if (peer->flags & RXR_PEER_IN_BACKOFF)
			continue;

		assert(tx_entry->rxr_flags & RXR_TX_ENTRY_QUEUED_RNR);
		ret = rxr_ep_send_queued_pkts(ep, &tx_entry->queued_pkts);
		if (ret == -FI_EAGAIN)
			break;

		if (OFI_UNLIKELY(ret)) {
			rxr_cq_write_tx_error(ep, tx_entry, -ret, FI_EFA_ERR_PKT_SEND);
			return;
		}

		dlist_remove(&tx_entry->queued_rnr_entry);
		tx_entry->rxr_flags &= ~RXR_TX_ENTRY_QUEUED_RNR;
	}

	/*
	 * Send data packets until window or data queue is exhausted.
	 */
	dlist_foreach_container(&ep->op_entry_longcts_send_list, struct rxr_op_entry,
				op_entry, entry) {
		peer = rxr_ep_get_peer(ep, op_entry->addr);
		assert(peer);
		if (peer->flags & RXR_PEER_IN_BACKOFF)
			continue;

		/*
		 * Do not send DATA packet until we received HANDSHAKE packet from the peer,
		 * this is because endpoint does not know whether peer need connid in header
		 * until it get the HANDSHAKE packet.
		 *
		 * We only do this for DATA packet because other types of packets always
		 * has connid in there packet header. If peer does not make use of the connid,
		 * the connid can be safely ignored.
		 *
		 * DATA packet is different because for DATA packet connid is an optional
		 * header inserted between the mandatory header and the application data.
		 * Therefore if peer does not use/understand connid, it will take connid
		 * as application data thus cause data corruption.
		 *
		 * This will not cause deadlock because peer will send a HANDSHAKE packet
		 * back upon receiving 1st packet from the endpoint, and in all 3 sub0protocols
		 * (long-CTS message, emulated long-CTS write and emulated long-CTS read)
		 * where DATA packet is used, endpoint will send other types of packet to
		 * peer before sending DATA packets. The workflow of the 3 sub-protocol
		 * can be found in protocol v4 document chapter 3.
		 */
		if (!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED))
			continue;
		while (op_entry->window > 0) {
			flags = FI_MORE;
			if (ep->efa_max_outstanding_tx_ops - ep->efa_outstanding_tx_ops <= 1 ||
			    op_entry->window <= ep->max_data_payload_size)
				flags = 0;
			/*
			 * The core's TX queue is full so we can't do any
			 * additional work.
			 */
			if (ep->efa_outstanding_tx_ops == ep->efa_max_outstanding_tx_ops)
				goto out;

			if (peer->flags & RXR_PEER_IN_BACKOFF)
				break;
			ret = rxr_pkt_post(ep, op_entry, RXR_DATA_PKT, false, flags);
			if (OFI_UNLIKELY(ret)) {
				if (ret == -FI_EAGAIN)
					goto out;

				rxr_cq_write_tx_error(ep, op_entry, -ret, FI_EFA_ERR_PKT_POST);
				return;
			}
		}
	}

	/*
	 * Send read requests until finish or error encoutered
	 */
	dlist_foreach_container_safe(&ep->read_pending_list, struct rxr_read_entry,
				     read_entry, pending_entry, tmp) {
		peer = rxr_ep_get_peer(ep, read_entry->addr);
		/*
		 * Here peer can be NULL, when the read request is a
		 * local read request. Local read request is used to copy
		 * data from host memory to device memory on same process.
		 */
		if (peer && (peer->flags & RXR_PEER_IN_BACKOFF))
			continue;

		/*
		 * The core's TX queue is full so we can't do any
		 * additional work.
		 */
		if (ep->efa_outstanding_tx_ops == ep->efa_max_outstanding_tx_ops)
			goto out;

		ret = rxr_read_post(ep, read_entry);
		if (ret == -FI_EAGAIN)
			break;

		if (OFI_UNLIKELY(ret)) {
			rxr_read_write_error(ep, read_entry, -ret, FI_EFA_ERR_READ_POST);
			return;
		}

		read_entry->state = RXR_RDMA_ENTRY_SUBMITTED;
		dlist_remove(&read_entry->pending_entry);
	}

out:
	efa_ep = container_of(ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	if (efa_ep->xmit_more_wr_tail != &efa_ep->xmit_more_wr_head) {
		ret = efa_post_flush(efa_ep, &bad_wr);
		if (OFI_UNLIKELY(ret))
			efa_eq_write_error(&ep->util_ep, -ret, FI_EFA_ERR_WR_POST_SEND);
	}

	return;
}

void rxr_ep_progress(struct util_ep *util_ep)
{
	struct rxr_ep *ep;

	ep = container_of(util_ep, struct rxr_ep, util_ep);

	ofi_mutex_lock(&ep->util_ep.lock);
	rxr_ep_progress_internal(ep);
	ofi_mutex_unlock(&ep->util_ep.lock);
}

static
bool rxr_ep_use_shm_for_tx(struct fi_info *info)
{
	/* App provided hints supercede environmental variables.
	 *
	 * Using the shm provider comes with some overheads, particularly in the
	 * progress engine when polling an empty completion queue, so avoid
	 * initializing the provider if the app provides a hint that it does not
	 * require node-local communication. We can still loopback over the EFA
	 * device in cases where the app violates the hint and continues
	 * communicating with node-local peers.
	 */
	if (info
	    /* If the app requires explicitly remote communication */
	    && (info->caps & FI_REMOTE_COMM)
	    /* but not local communication */
	    && !(info->caps & FI_LOCAL_COMM))
		return 0;

	return rxr_env.enable_shm_transfer;
}

/**
 * @brief allocate a fi_info that can be used to open a device endpoint
 *
 * @param	app_device_info[out]		output
 * @param	device_prov_info[in]		info from efa_prov_info_alloc(FI_EP_RDM)
 * @param	api_version			API version of the output
 * @return	0 on success.
 * 		-FI_ENOMEM if memory allocation of fi_info object failed.
 */
static
int rxr_ep_alloc_app_device_info(struct fi_info **app_device_info,
				 struct fi_info *device_prov_info,
				 uint32_t api_version)
{
	struct fi_info *result;

	*app_device_info = NULL;

	result = fi_dupinfo(device_prov_info);
	if (!result)
		return -FI_ENOMEM;

	/* the MR mode in device_prov_info contains both FI_MR_BASIC and
	 * OFI_MR_BASIC_MAP (OFI_MR_BASIC_MAP equals to FI_MR_VIRT_ADDR |
	 * FI_MR_ALLOCATED | FI_MR_PROV_KEY).
	 *
	 * According to document:
	 * https://ofiwg.github.io/libfabric/main/man/fi_domain.3.html,
	 *
	 * FI_MR_BASIC and OFI_MR_BASIC_MAP means the same functionality,
	 * FI_MR_BASIC has been deprecated since libfabric 1.5 and defined
	 * only for backward compatibility.
	 *
	 * For applications that are still using FI_MR_BASIC to be able
	 * to pick up EFA, we kept FI_MR_BASIC in device_prov_info.
	 *
	 * When opening a device endpoint, the info object cannot have both
	 * FI_MR_BASIC and OFI_MR_BASIC_MAP, because FI_MR_BASIC cannot be
	 * used with other MR modes.
	 */
	if (FI_VERSION_LT(api_version, FI_VERSION(1,5))) {
		EFA_INFO(FI_LOG_EP_CTRL, "libfabric API version: %d.%d was used to construct device info.\n",
			 FI_MAJOR(api_version), FI_MINOR(api_version));
		/*
		 * For older API(< 1.5), keep using FI_MR_BASIC.
		 * and we need to set the FI_LOCAL_MR flag in mode to
		 * indicate that device need memory registration.
		 */
		result->domain_attr->mr_mode = FI_MR_BASIC;
		result->mode |= FI_LOCAL_MR | FI_MR_ALLOCATED;
	} else {
		/* For newer API(>= 1.5), unset FI_MR_BASIC because it has been deprecated.
		 */
		result->domain_attr->mr_mode &= ~FI_MR_BASIC;
	}

	*app_device_info = result;
	return 0;
}

/**
 * @brief Bind rxr_ep->ibv_cq_ex to efa_ep->scq and efa_ep->rcq
 * 	Will be removed when rxr_ep and efa_ep are fully separated
 *
 * @param[in] rxr_ep Pointer to the rxr_ep.
 * @param[in] ibv_cq_ex Pointer to ibv_cq_ex.
 * @return 0 on success, error code otherwise
 */
static int rxr_ep_setup_device_cq(struct rxr_ep *rxr_ep)
{
	struct efa_ep *efa_ep = container_of(rxr_ep->rdm_ep, struct efa_ep, util_ep.ep_fid);
	struct ibv_cq_ex *ibv_cq_ex = rxr_ep->ibv_cq_ex;

	efa_ep->scq = malloc(sizeof(struct efa_cq));
	if (!efa_ep->scq)
		return -FI_ENOMEM;
	efa_ep->scq->ibv_cq_ex = ibv_cq_ex;
	efa_ep->scq->domain = rxr_ep_domain(rxr_ep);

	efa_ep->rcq = efa_ep->scq;

	return 0;
}

int rxr_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep, void *context)
{
	struct fi_info *rdm_info = NULL;
	struct efa_domain *efa_domain = NULL;
	struct rxr_ep *rxr_ep = NULL;
	struct fi_cq_attr cq_attr;
	int ret, retv;

	rxr_ep = calloc(1, sizeof(*rxr_ep));
	if (!rxr_ep)
		return -FI_ENOMEM;

	efa_domain = container_of(domain, struct efa_domain,
				  util_domain.domain_fid);
	memset(&cq_attr, 0, sizeof(cq_attr));
	cq_attr.format = FI_CQ_FORMAT_DATA;
	cq_attr.wait_obj = FI_WAIT_NONE;

	ret = ofi_endpoint_init(domain, &rxr_util_prov, info, &rxr_ep->util_ep,
				context, rxr_ep_progress);
	if (ret)
		goto err_free_ep;

	ret = rxr_ep_alloc_app_device_info(&rdm_info,
					   efa_domain->device->rdm_info,
					   efa_domain->util_domain.fabric->fabric_fid.api_version
					   );
	if (ret)
		goto err_close_ofi_ep;

	ret = efa_ep_open(&efa_domain->util_domain.domain_fid, rdm_info,
			  &rxr_ep->rdm_ep, rxr_ep);
	if (ret)
		goto err_free_rdm_info;

	if (efa_domain->shm_domain) {
		assert(!strcmp(g_shm_info->fabric_attr->name, "shm"));
		ret = fi_endpoint(efa_domain->shm_domain, g_shm_info,
				  &rxr_ep->shm_ep, rxr_ep);
		if (ret)
			goto err_close_core_ep;

		rxr_ep->use_shm_for_tx = rxr_ep_use_shm_for_tx(info);
	} else {
		rxr_ep->shm_ep = NULL;
		rxr_ep->use_shm_for_tx = false;
	}

	rxr_ep->rx_size = info->rx_attr->size;
	rxr_ep->tx_size = info->tx_attr->size;
	rxr_ep->rx_iov_limit = info->rx_attr->iov_limit;
	rxr_ep->tx_iov_limit = info->tx_attr->iov_limit;
	rxr_ep->inject_size = info->tx_attr->inject_size;
	rxr_ep->efa_max_outstanding_tx_ops = rdm_info->tx_attr->size;
	rxr_ep->core_rx_size = rdm_info->rx_attr->size;
	rxr_ep->core_iov_limit = rdm_info->tx_attr->iov_limit;
	rxr_ep->core_caps = rdm_info->caps;

	cq_attr.size = MAX(rxr_ep->rx_size + rxr_ep->tx_size,
			   rxr_env.cq_size);

	if (info->tx_attr->op_flags & FI_DELIVERY_COMPLETE)
		FI_INFO(&rxr_prov, FI_LOG_CQ, "FI_DELIVERY_COMPLETE unsupported\n");

	assert(info->tx_attr->msg_order == info->rx_attr->msg_order);
	rxr_ep->msg_order = info->rx_attr->msg_order;
	rxr_ep->core_msg_order = rdm_info->rx_attr->msg_order;
	rxr_ep->core_inject_size = rdm_info->tx_attr->inject_size;
	rxr_ep->max_msg_size = info->ep_attr->max_msg_size;
	rxr_ep->msg_prefix_size = info->ep_attr->msg_prefix_size;
	rxr_ep->max_proto_hdr_size = rxr_pkt_max_hdr_size();
	rxr_ep->mtu_size = rdm_info->ep_attr->max_msg_size;
	fi_freeinfo(rdm_info);
	rdm_info = NULL;

	if (rxr_env.mtu_size > 0 && rxr_env.mtu_size < rxr_ep->mtu_size)
		rxr_ep->mtu_size = rxr_env.mtu_size;

	if (rxr_ep->mtu_size > RXR_MTU_MAX_LIMIT)
		rxr_ep->mtu_size = RXR_MTU_MAX_LIMIT;

	rxr_ep->max_data_payload_size = rxr_ep->mtu_size - sizeof(struct rxr_data_hdr) - sizeof(struct rxr_data_opt_connid_hdr);
	rxr_ep->min_multi_recv_size = rxr_ep->mtu_size - rxr_ep->max_proto_hdr_size;

	if (rxr_env.tx_queue_size > 0 &&
	    rxr_env.tx_queue_size < rxr_ep->efa_max_outstanding_tx_ops)
		rxr_ep->efa_max_outstanding_tx_ops = rxr_env.tx_queue_size;


	rxr_ep->use_zcpy_rx = rxr_ep_use_zcpy_rx(rxr_ep, info);
	FI_INFO(&rxr_prov, FI_LOG_EP_CTRL, "rxr_ep->use_zcpy_rx = %d\n", rxr_ep->use_zcpy_rx);

	rxr_ep->handle_resource_management = info->domain_attr->resource_mgmt;
	FI_INFO(&rxr_prov, FI_LOG_EP_CTRL,
		"rxr_ep->handle_resource_management = %d\n",
		rxr_ep->handle_resource_management);

#if ENABLE_DEBUG
	rxr_ep->efa_total_posted_tx_ops = 0;
	rxr_ep->shm_total_posted_tx_ops = 0;
	rxr_ep->send_comps = 0;
	rxr_ep->failed_send_comps = 0;
	rxr_ep->recv_comps = 0;
#endif

	rxr_ep->shm_rx_pkts_posted = 0;
	rxr_ep->shm_rx_pkts_to_post = 0;
	rxr_ep->efa_rx_pkts_posted = 0;
	rxr_ep->efa_rx_pkts_to_post = 0;
	rxr_ep->efa_outstanding_tx_ops = 0;
	rxr_ep->shm_outstanding_tx_ops = 0;

	assert(!rxr_ep->ibv_cq_ex);

	ret = efa_cq_ibv_cq_ex_open(&cq_attr, efa_domain->device->ibv_ctx,
				    &rxr_ep->ibv_cq_ex, &rxr_ep->ibv_cq_ex_type);

	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %s\n", strerror(errno));
		goto err_close_shm_ep;
	}

	ret = rxr_ep_setup_device_cq(rxr_ep);
	if (ret)
		goto err_close_core_cq;

	if (efa_domain->shm_domain) {
		/* Bind ep with shm provider's cq */
		ret = fi_cq_open(efa_domain->shm_domain, &cq_attr,
				 &rxr_ep->shm_cq, rxr_ep);
		if (ret)
			goto err_close_core_cq;

		ret = fi_ep_bind(rxr_ep->shm_ep, &rxr_ep->shm_cq->fid,
				 FI_TRANSMIT | FI_RECV);
		if (ret)
			goto err_close_shm_cq;
	}

	ret = rxr_ep_init(rxr_ep);
	if (ret)
		goto err_close_shm_cq;

	*ep = &rxr_ep->util_ep.ep_fid;
	(*ep)->msg = &rxr_ops_msg;
	(*ep)->rma = &rxr_ops_rma;
	(*ep)->atomic = &rxr_ops_atomic;
	(*ep)->tagged = &rxr_ops_tagged;
	(*ep)->fid.ops = &rxr_ep_fi_ops;
	(*ep)->ops = &rxr_ops_ep;
	(*ep)->cm = &rxr_ep_cm;
	return 0;

err_close_shm_cq:
	if (rxr_ep->shm_cq) {
		retv = fi_close(&rxr_ep->shm_cq->fid);
		if (retv)
			FI_WARN(&rxr_prov, FI_LOG_CQ, "Unable to close shm cq: %s\n",
				fi_strerror(-retv));
	}
err_close_core_cq:
	retv = -ibv_destroy_cq(ibv_cq_ex_to_cq(rxr_ep->ibv_cq_ex));
	if (retv)
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Unable to close cq: %s\n",
			fi_strerror(-retv));
err_close_shm_ep:
	if (rxr_ep->shm_ep) {
		retv = fi_close(&rxr_ep->shm_ep->fid);
		if (retv)
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close shm EP: %s\n",
				fi_strerror(-retv));
	}
err_close_core_ep:
	retv = fi_close(&rxr_ep->rdm_ep->fid);
	if (retv)
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "Unable to close EP: %s\n",
			fi_strerror(-retv));
err_free_rdm_info:
	if (rdm_info)
		fi_freeinfo(rdm_info);
err_close_ofi_ep:
	retv = ofi_endpoint_close(&rxr_ep->util_ep);
	if (retv)
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unable to close util EP: %s\n",
			fi_strerror(-retv));
err_free_ep:
	if (rxr_ep)
		free(rxr_ep);
	return ret;
}

/**
 * @brief record the event that a TX op has been submitted
 *
 * This function is called after a TX operation has been posted
 * successfully. It will:
 *
 *  1. increase the outstanding tx_op counter in endpoint and
 *     in the peer structure.
 *
 *  2. add the TX packet to peer's outstanding TX packet list.
 *
 * Both send and read are considered TX operation.
 *
 * The tx_op counters used to prevent over posting the device
 * and used in flow control. They are also usefull for debugging.
 *
 * Peer's outstanding TX packet list is used when removing a peer
 * to invalidate address of these packets, so that the completion
 * of these packet is ignored.
 *
 * @param[in,out]	ep		endpoint
 * @param[in]		pkt_entry	TX pkt_entry, which contains
 * 					the info of the TX op.
 */
void rxr_ep_record_tx_op_submitted(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rdm_peer *peer;
	struct rxr_op_entry *op_entry;

	op_entry = rxr_op_entry_of_pkt_entry(pkt_entry);
	/*
	 * peer can be NULL when the pkt_entry is a RMA_CONTEXT_PKT,
	 * and the RMA is a local read toward the endpoint itself
	 */
	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	if (peer)
		dlist_insert_tail(&pkt_entry->entry, &peer->outstanding_tx_pkts);

	if (pkt_entry->alloc_type == RXR_PKT_FROM_EFA_TX_POOL) {
		ep->efa_outstanding_tx_ops++;
		if (peer)
			peer->efa_outstanding_tx_ops++;

		if (op_entry)
			op_entry->efa_outstanding_tx_ops++;
#if ENABLE_DEBUG
		ep->efa_total_posted_tx_ops++;
#endif
	} else {
		assert(pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL);
		ep->shm_outstanding_tx_ops++;
		if (peer)
			peer->shm_outstanding_tx_ops++;

		if (op_entry)
			op_entry->shm_outstanding_tx_ops++;
#if ENABLE_DEBUG
		ep->shm_total_posted_tx_ops++;
#endif
	}

}

/**
 * @brief record the event that an TX op is completed
 *
 * This function is called when the completion of
 * a TX operation is received. It will
 *
 * 1. decrease the outstanding tx_op counter in the endpoint
 *    and in the peer.
 *
 * 2. remove the TX packet from peer's outstanding
 *    TX packet list.
 *
 * Both send and read are considered TX operation.
 *
 * One may ask why this function is not integrated
 * into rxr_pkt_entry_relase_tx()?
 *
 * The reason is the action of decrease tx_op counter
 * is not tied to releasing a TX pkt_entry.
 *
 * Sometimes we need to decreate the tx_op counter
 * without releasing a TX pkt_entry. For example,
 * we handle a TX pkt_entry encountered RNR. We need
 * to decrease the tx_op counter and queue the packet.
 *
 * Sometimes we need release TX pkt_entry without
 * decreasing the tx_op counter. For example, when
 * rxr_pkt_post() failed to post a pkt entry.
 *
 * @param[in,out]	ep		endpoint
 * @param[in]		pkt_entry	TX pkt_entry, which contains
 * 					the info of the TX op
 */
void rxr_ep_record_tx_op_completed(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *op_entry = NULL;
	struct rdm_peer *peer;

	op_entry = rxr_op_entry_of_pkt_entry(pkt_entry);
	/*
	 * peer can be NULL when:
	 *
	 * 1. the pkt_entry is a RMA_CONTEXT_PKT, and the RMA op is a local read
	 *    toward the endpoint itself.
	 * 2. peer's address has been removed from address vector. Either because
	 *    a new peer has the same GID+QPN was inserted to address, or because
	 *    application removed the peer from address vector.
	 */
	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	if (peer)
		dlist_remove(&pkt_entry->entry);

	if (pkt_entry->alloc_type == RXR_PKT_FROM_EFA_TX_POOL) {
		ep->efa_outstanding_tx_ops--;
		if (peer)
			peer->efa_outstanding_tx_ops--;

		if (op_entry)
			op_entry->efa_outstanding_tx_ops--;
	} else {
		assert(pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL);
		ep->shm_outstanding_tx_ops--;
		if (peer)
			peer->shm_outstanding_tx_ops--;

		if (op_entry)
			op_entry->shm_outstanding_tx_ops--;
	}
}
