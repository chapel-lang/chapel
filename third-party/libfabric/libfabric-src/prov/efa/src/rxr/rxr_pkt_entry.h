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

#ifndef _RXR_PKT_ENTRY_H
#define _RXR_PKT_ENTRY_H

#include <ofi_list.h>

#define RXR_PKT_ENTRY_IN_USE		BIT_ULL(0)
#define RXR_PKT_ENTRY_RNR_RETRANSMIT	BIT_ULL(1)
#define RXR_PKT_ENTRY_LOCAL_READ	BIT_ULL(2) /* this packet entry is used as context of a local read operation */

/* pkt_entry_alloc_type indicate where the packet entry is allocated from */
enum rxr_pkt_entry_alloc_type {
	RXR_PKT_FROM_EFA_TX_POOL = 1, /* packet is allocated from ep->efa_tx_pkt_pool */
	RXR_PKT_FROM_EFA_RX_POOL,     /* packet is allocated from ep->efa_rx_pkt_pool */
	RXR_PKT_FROM_SHM_TX_POOL,     /* packet is allocated from ep->shm_tx_pkt_pool */
	RXR_PKT_FROM_SHM_RX_POOL,     /* packet is allocated from ep->shm_rx_pkt_pool */
	RXR_PKT_FROM_UNEXP_POOL,      /* packet is allocated from ep->rx_unexp_pkt_pool */
	RXR_PKT_FROM_OOO_POOL,	      /* packet is allocated from ep->rx_ooo_pkt_pool */
	RXR_PKT_FROM_USER_BUFFER,     /* packet is from user provided buffer */
	RXR_PKT_FROM_READ_COPY_POOL,  /* packet is allocated from ep->rx_readcopy_pkt_pool */
};

struct rxr_pkt_sendv {
	/* Because core EP current only support 2 iov,
	 * and for the sake of code simplicity, we use 2 iov.
	 * One for header, and the other for data.
	 * iov_count here is used as an indication
	 * of whether iov is used, it is either 0 or 2.
	 */
	int iov_count;
	struct iovec iov[2];
	void *desc[2];
};

/* rxr_pkt_entry is used both for sending data to and receiving data from a peer.
 */
struct rxr_pkt_entry {
	/* entry is used for sending only.
	 * It is either linked peer->outstanding_tx_pkts (after a packet has been successfully sent, but it get a completion),
	 * or linked to tx_rx_entry->queued_pkts (after it encountered RNR error completion).
	 */
	struct dlist_entry entry;
#if ENABLE_DEBUG
	/* for tx/rx debug list or posted buf list */
	struct dlist_entry dbg_entry;
#endif
	void *x_entry; /* pointer to rxr rx/tx entry */
	size_t pkt_size;

	struct fid_mr *mr;
	/* `addr` is used for both sending data and receiving data.
	 *
	 * When sending a packet, `addr` will be provided by application and it cannot be FI_ADDR_NOTAVAIL.
	 * However, after a packet is sent, application can remove a peer by calling fi_av_remove().
	 * When removing the peering, `addr` will be set to FI_ADDR_NOTAVAIL. Later, when device report
	 * completion for such a TX packet, the TX completion will be ignored.
	 *
	 * When receiving a packet, lower device will set `addr`. If the sender's address is not in
	 * address vector (AV), `lower device will set `addr` to FI_ADDR_NOTAVAIL. This can happen in
	 * two scenarios:
	 *
	 * 1. There has been no prior communication with the peer. In this case, the packet should have
	 *    peer's raw address in the header, and progress engine will insert the raw address into
	 *    address vector, and update `addr`.
	 *
	 * 2. This packet is from a peer whose address has been removed from AV. In this case, the
	 *    recived packet will be ignored because all resources associated with peer has been released.
	 */
	fi_addr_t addr;
	enum rxr_pkt_entry_alloc_type alloc_type; /* where the memory of this packet entry reside */
	uint32_t flags;

	/*
	 * next is used on receiving end.
	 * send is used on sending end.
	 */
	union {
		struct rxr_pkt_entry *next;
		struct rxr_pkt_sendv *send;
	};

#if ENABLE_DEBUG
	/* pad to cache line size of 64 bytes */
	uint8_t pad[48];
#endif
	char pkt[0]; /* rxr_ctrl_*_pkt, or rxr_data_pkt */
};

static inline void *rxr_pkt_start(struct rxr_pkt_entry *pkt_entry)
{
	return pkt_entry->pkt;
}

#if defined(static_assert) && defined(__x86_64__)
#if ENABLE_DEBUG
static_assert(sizeof(struct rxr_pkt_entry) == 128, "rxr_pkt_entry check");
#else
static_assert(sizeof(struct rxr_pkt_entry) == 64, "rxr_pkt_entry check");
#endif
#endif

OFI_DECL_RECVWIN_BUF(struct rxr_pkt_entry*, rxr_robuf, uint32_t);
OFI_DECLARE_FREESTACK(struct rxr_robuf, rxr_robuf_fs);

struct rxr_ep;

struct rxr_op_entry;

struct rxr_pkt_entry *rxr_pkt_entry_init_prefix(struct rxr_ep *ep,
						const struct fi_msg *posted_buf,
						struct ofi_bufpool *pkt_pool);

struct rxr_pkt_entry *rxr_pkt_entry_alloc(struct rxr_ep *ep,
					  struct ofi_bufpool *pkt_pool,
					  enum rxr_pkt_entry_alloc_type alloc_type);

void rxr_pkt_entry_release_tx(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_entry_release_rx(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_entry_append(struct rxr_pkt_entry *dst,
			  struct rxr_pkt_entry *src);

struct rxr_pkt_entry *rxr_pkt_entry_clone(struct rxr_ep *ep,
					  struct ofi_bufpool *pkt_pool,
					  enum rxr_pkt_entry_alloc_type alloc_type,
					  struct rxr_pkt_entry *src);

struct rxr_pkt_entry *rxr_pkt_get_unexp(struct rxr_ep *ep,
					struct rxr_pkt_entry **pkt_entry_ptr);

ssize_t rxr_pkt_entry_send(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry, uint64_t flags);

ssize_t rxr_pkt_entry_inject(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry,
			     fi_addr_t addr);

struct rxr_pkt_rx_key {
	uint64_t msg_id;
	fi_addr_t addr;
};

struct rxr_op_entry;

struct rxr_pkt_rx_map {
	struct rxr_pkt_rx_key key;
	struct rxr_op_entry *rx_entry;
	UT_hash_handle hh;
};

struct rxr_op_entry *rxr_pkt_rx_map_lookup(struct rxr_ep *ep,
					   struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_rx_map_insert(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry,
			   struct rxr_op_entry *rx_entry);

void rxr_pkt_rx_map_remove(struct rxr_ep *pkt_rx_map,
			   struct rxr_pkt_entry *pkt_entry,
			   struct rxr_op_entry *rx_entry);

#endif
