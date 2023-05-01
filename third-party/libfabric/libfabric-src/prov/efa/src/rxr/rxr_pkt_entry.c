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
#include "rxr_msg.h"
#include "rxr_rma.h"
#include "rxr_op_entry.h"
#include "rxr_pkt_cmd.h"

/*
 *   General purpose utility functions
 */
struct rxr_pkt_entry *rxr_pkt_entry_alloc(struct rxr_ep *ep,
					  struct ofi_bufpool *pkt_pool,
					  enum rxr_pkt_entry_alloc_type alloc_type)
{
	struct rxr_pkt_entry *pkt_entry;
	void *mr = NULL;

	pkt_entry = ofi_buf_alloc_ex(pkt_pool, &mr);
	if (!pkt_entry)
		return NULL;

#ifdef ENABLE_EFA_POISONING
	memset(pkt_entry, 0, sizeof(*pkt_entry));
#endif
	dlist_init(&pkt_entry->entry);
#if ENABLE_DEBUG
	dlist_init(&pkt_entry->dbg_entry);
#endif
	pkt_entry->mr = (struct fid_mr *) mr;
#ifdef ENABLE_EFA_POISONING
	memset(pkt_entry->pkt, 0, ep->mtu_size);
#endif
	pkt_entry->alloc_type = alloc_type;
	pkt_entry->flags = RXR_PKT_ENTRY_IN_USE;
	pkt_entry->next = NULL;
	pkt_entry->x_entry = NULL;
	return pkt_entry;
}

/**
 * @brief release a TX packet entry
 *
 * @param[in]     ep  the end point
 * @param[in,out] pkt the pkt_entry to be released
 */
void rxr_pkt_entry_release_tx(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt)
{
	struct rdm_peer *peer;

#if ENABLE_DEBUG
	dlist_remove(&pkt->dbg_entry);
#endif
	/*
	 * Decrement rnr_queued_pkts counter and reset backoff for this peer if
	 * we get a send completion for a retransmitted packet.
	 */
	if (OFI_UNLIKELY(pkt->flags & RXR_PKT_ENTRY_RNR_RETRANSMIT)) {
		peer = rxr_ep_get_peer(ep, pkt->addr);
		assert(peer);
		peer->rnr_queued_pkt_cnt--;
		peer->rnr_backoff_wait_time = 0;
		if (peer->flags & RXR_PEER_IN_BACKOFF) {
			dlist_remove(&peer->rnr_backoff_entry);
			peer->flags &= ~RXR_PEER_IN_BACKOFF;
		}
		FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
		       "reset backoff timer for peer: %" PRIu64 "\n",
		       pkt->addr);
	}
	if (pkt->send) {
		ofi_buf_free(pkt->send);
		pkt->send = NULL;
	}
#ifdef ENABLE_EFA_POISONING
	rxr_poison_mem_region((uint32_t *)pkt, ep->tx_pkt_pool_entry_sz);
#endif
	pkt->flags = 0;
	ofi_buf_free(pkt);
}

/*
 * rxr_pkt_entry_release_rx() release a rx packet entry.
 * It requires input pkt_entry to be unlinked.
 *
 * RX packet entry can be linked when medium message protocol
 * is used.
 *
 * In that case, caller is responsible to unlink the pkt_entry
 * can call this function on next packet entry.
 */
void rxr_pkt_entry_release_rx(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry)
{
	assert(pkt_entry->next == NULL);

	if (ep->use_zcpy_rx && pkt_entry->alloc_type == RXR_PKT_FROM_USER_BUFFER)
		return;

	if (pkt_entry->alloc_type == RXR_PKT_FROM_EFA_RX_POOL) {
		ep->efa_rx_pkts_to_post++;
	} else if (pkt_entry->alloc_type == RXR_PKT_FROM_SHM_RX_POOL) {
		ep->shm_rx_pkts_to_post++;
	} else if (pkt_entry->alloc_type == RXR_PKT_FROM_READ_COPY_POOL) {
		assert(ep->rx_readcopy_pkt_pool_used > 0);
		ep->rx_readcopy_pkt_pool_used--;
	}

#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
#ifdef ENABLE_EFA_POISONING
	/* the same pool size is used for all types of rx pkt_entries */
	rxr_poison_mem_region((uint32_t *)pkt_entry, ep->rx_pkt_pool_entry_sz);
#endif
	pkt_entry->flags = 0;
	ofi_buf_free(pkt_entry);
}

void rxr_pkt_entry_copy(struct rxr_ep *ep,
			struct rxr_pkt_entry *dest,
			struct rxr_pkt_entry *src)
{
	FI_DBG(&rxr_prov, FI_LOG_EP_CTRL,
	       "Copying packet out of posted buffer! src_entry_alloc_type: %d desc_entry_alloc_type: %d\n",
		src->alloc_type, dest->alloc_type);
	dlist_init(&dest->entry);
#if ENABLE_DEBUG
	dlist_init(&dest->dbg_entry);
#endif
	/* dest->mr was set in rxr_pkt_entry_alloc(), and
	 * is tied to the memory region, therefore should
	 * not be changed.
	 */
	dest->x_entry = src->x_entry;
	dest->pkt_size = src->pkt_size;
	dest->addr = src->addr;
	dest->flags = RXR_PKT_ENTRY_IN_USE;
	dest->next = NULL;
	assert(src->pkt_size > 0);
	memcpy(dest->pkt, src->pkt, src->pkt_size);
}

/*
 * Handle copying or updating the metadata for an unexpected packet.
 *
 * Packets from the EFA RX pool will be copied into a separate buffer not
 * registered with the device (if this option is enabled) so that we can repost
 * the registered buffer again to keep the EFA RX queue full. Packets from the
 * SHM RX pool will also be copied to reuse the unexpected message pool.
 *
 * @param[in]     ep  the end point
 * @param[in,out] pkt_entry_ptr unexpected packet, if this packet is copied to
 *                a new memory region this pointer will be updated.
 *
 * @return	  struct rxr_pkt_entry of the updated or copied packet, NULL on
 * 		  allocation failure.
 */
struct rxr_pkt_entry *rxr_pkt_get_unexp(struct rxr_ep *ep,
					struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rxr_pkt_entry *unexp_pkt_entry;
	enum rxr_pkt_entry_alloc_type type;

	type = (*pkt_entry_ptr)->alloc_type;

	if (rxr_env.rx_copy_unexp && (type == RXR_PKT_FROM_EFA_RX_POOL ||
				      type == RXR_PKT_FROM_SHM_RX_POOL)) {
		unexp_pkt_entry = rxr_pkt_entry_clone(ep, ep->rx_unexp_pkt_pool,
						      RXR_PKT_FROM_UNEXP_POOL,
						      *pkt_entry_ptr);
		if (OFI_UNLIKELY(!unexp_pkt_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
				"Unable to allocate rx_pkt_entry for unexp msg\n");
			return NULL;
		}
		rxr_pkt_entry_release_rx(ep, *pkt_entry_ptr);
		*pkt_entry_ptr = unexp_pkt_entry;
	} else {
		unexp_pkt_entry = *pkt_entry_ptr;
	}

	return unexp_pkt_entry;
}

void rxr_pkt_entry_release_cloned(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_pkt_entry *next;

	while (pkt_entry) {
		assert(pkt_entry->alloc_type == RXR_PKT_FROM_OOO_POOL ||
		       pkt_entry->alloc_type == RXR_PKT_FROM_UNEXP_POOL);
#ifdef ENABLE_EFA_POISONING
		rxr_poison_mem_region((uint32_t *)pkt_entry, ep->tx_pkt_pool_entry_sz);
#endif
		pkt_entry->flags = 0;
		ofi_buf_free(pkt_entry);
		next = pkt_entry->next;
		pkt_entry = next;
	}
}

struct rxr_pkt_entry *rxr_pkt_entry_clone(struct rxr_ep *ep,
					  struct ofi_bufpool *pkt_pool,
					  enum rxr_pkt_entry_alloc_type alloc_type,
					  struct rxr_pkt_entry *src)
{
	struct rxr_pkt_entry *root = NULL;
	struct rxr_pkt_entry *dst;

	assert(src);
	assert(alloc_type == RXR_PKT_FROM_OOO_POOL ||
	       alloc_type == RXR_PKT_FROM_UNEXP_POOL ||
	       alloc_type == RXR_PKT_FROM_READ_COPY_POOL);

	dst = rxr_pkt_entry_alloc(ep, pkt_pool, alloc_type);
	if (!dst)
		return NULL;

	if (alloc_type == RXR_PKT_FROM_READ_COPY_POOL) {
		assert(pkt_pool == ep->rx_readcopy_pkt_pool);
		ep->rx_readcopy_pkt_pool_used++;
		ep->rx_readcopy_pkt_pool_max_used = MAX(ep->rx_readcopy_pkt_pool_used,
							ep->rx_readcopy_pkt_pool_max_used);
	}

	rxr_pkt_entry_copy(ep, dst, src);
	root = dst;
	while (src->next) {
		dst->next = rxr_pkt_entry_alloc(ep, pkt_pool, alloc_type);
		if (!dst->next) {
			rxr_pkt_entry_release_cloned(ep, root);
			return NULL;
		}

		rxr_pkt_entry_copy(ep, dst->next, src->next);
		src = src->next;
		dst = dst->next;
	}

	assert(dst && !dst->next);
	return root;
}

void rxr_pkt_entry_append(struct rxr_pkt_entry *dst,
			  struct rxr_pkt_entry *src)
{
	assert(dst);

	while (dst->next)
		dst = dst->next;
	assert(dst && !dst->next);
	dst->next = src;
}

/**
 * @brief send a packet using lower provider
 *
 * @param ep[in]        rxr end point
 * @param pkt_entry[in] packet entry to be sent
 * @param msg[in]       information regarding that the send operation, such as
 *                      memory buffer, remote EP address and local descriptor.
 *                      If the shm provider is to be used. Remote EP address
 *                      and local descriptor must be prepared for shm usage.
 * @param flags[in]     flags to be passed on to lower provider's send.
 */
static inline
ssize_t rxr_pkt_entry_sendmsg(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry,
			      const struct fi_msg *msg, uint64_t flags)
{
	struct rdm_peer *peer;
	ssize_t ret;

	if (pkt_entry->alloc_type == RXR_PKT_FROM_EFA_TX_POOL &&
	    ep->efa_outstanding_tx_ops == ep->efa_max_outstanding_tx_ops)
		return -FI_EAGAIN;

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);

	if (peer->flags & RXR_PEER_IN_BACKOFF)
		return -FI_EAGAIN;

#if ENABLE_DEBUG
	dlist_insert_tail(&pkt_entry->dbg_entry, &ep->tx_pkt_list);
#ifdef ENABLE_RXR_PKT_DUMP
	rxr_pkt_print("Sent", ep, (struct rxr_base_hdr *)pkt_entry->pkt);
#endif
#endif
	if (pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL) {
		assert(peer->is_local && ep->use_shm_for_tx);
		ret = fi_sendmsg(ep->shm_ep, msg, flags);
	} else {
		ret = fi_sendmsg(ep->rdm_ep, msg, flags);
	}

	if (OFI_UNLIKELY(ret))
		return ret;

	rxr_ep_record_tx_op_submitted(ep, pkt_entry);
	return 0;
}

/**
 * @brief Construct a fi_msg object with the information stored in pkt_entry,
 * and send it out
 *
 * @param[in] ep	rxr endpoint
 * @param[in] pkt_entry	packet entry used to construct the fi_msg object
 * @param[in] flags	flags to be applied to lower provider's send operation
 * @return		0 on success
 * 			On error, a negative value corresponding to fabric errno
 *
 */
ssize_t rxr_pkt_entry_send(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry,
			   uint64_t flags)
{
	struct iovec iov;
	void *desc;
	struct fi_msg msg;
	struct rdm_peer *peer;

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);

	if (pkt_entry->send && pkt_entry->send->iov_count > 0) {
		msg.msg_iov = pkt_entry->send->iov;
		msg.iov_count = pkt_entry->send->iov_count;
		msg.desc = pkt_entry->send->desc;
	} else {
		iov.iov_base = rxr_pkt_start(pkt_entry);
		iov.iov_len = pkt_entry->pkt_size;
		desc = (pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL) ? NULL : fi_mr_desc(pkt_entry->mr);
		msg.msg_iov = &iov;
		msg.iov_count = 1;
		msg.desc = &desc;
	}

	msg.addr = pkt_entry->addr;
	msg.context = pkt_entry;
	msg.data = 0;

	if (pkt_entry->alloc_type == RXR_PKT_FROM_SHM_TX_POOL) {
		msg.addr = peer->shm_fiaddr;
		rxr_convert_desc_for_shm(msg.iov_count, msg.desc);
	}

	return rxr_pkt_entry_sendmsg(ep, pkt_entry, &msg, flags);
}

ssize_t rxr_pkt_entry_inject(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry,
			     fi_addr_t addr)
{
	struct rdm_peer *peer;
	ssize_t ret;

	/* currently only EOR packet is injected using shm ep */
	peer = rxr_ep_get_peer(ep, addr);
	assert(peer);

	assert(ep->use_shm_for_tx && peer->is_local);
	ret = fi_inject(ep->shm_ep, rxr_pkt_start(pkt_entry), pkt_entry->pkt_size,
			 peer->shm_fiaddr);

	if (OFI_UNLIKELY(ret))
		return ret;

	rxr_ep_record_tx_op_submitted(ep, pkt_entry);
	return 0;
}

/*
 * Functions for pkt_rx_map
 */
struct rxr_op_entry *rxr_pkt_rx_map_lookup(struct rxr_ep *ep,
					   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_pkt_rx_map *entry = NULL;
	struct rxr_pkt_rx_key key;

	memset(&key, 0, sizeof(key));
	key.msg_id = rxr_pkt_msg_id(pkt_entry);
	key.addr = pkt_entry->addr;
	HASH_FIND(hh, ep->pkt_rx_map, &key, sizeof(struct rxr_pkt_rx_key), entry);
	return entry ? entry->rx_entry : NULL;
}

void rxr_pkt_rx_map_insert(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry,
			   struct rxr_op_entry *rx_entry)
{
	struct rxr_pkt_rx_map *entry;

	entry = ofi_buf_alloc(ep->map_entry_pool);
	if (OFI_UNLIKELY(!entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Map entries for medium size message exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		return;
	}

	memset(&entry->key, 0, sizeof(entry->key));
	entry->key.msg_id = rxr_pkt_msg_id(pkt_entry);
	entry->key.addr = pkt_entry->addr;

#if ENABLE_DEBUG
	{
		struct rxr_pkt_rx_map *existing_entry = NULL;

		HASH_FIND(hh, ep->pkt_rx_map, &entry->key, sizeof(struct rxr_pkt_rx_key), existing_entry);
		assert(!existing_entry);
	}
#endif

	entry->rx_entry = rx_entry;
	HASH_ADD(hh, ep->pkt_rx_map, key, sizeof(struct rxr_pkt_rx_key), entry);
}

void rxr_pkt_rx_map_remove(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry,
			   struct rxr_op_entry *rx_entry)
{
	struct rxr_pkt_rx_map *entry;
	struct rxr_pkt_rx_key key;

	memset(&key, 0, sizeof(key));
	key.msg_id = rxr_pkt_msg_id(pkt_entry);
	key.addr = pkt_entry->addr;

	HASH_FIND(hh, ep->pkt_rx_map, &key, sizeof(key), entry);
	assert(entry && entry->rx_entry == rx_entry);
	HASH_DEL(ep->pkt_rx_map, entry);
	ofi_buf_free(entry);
}

