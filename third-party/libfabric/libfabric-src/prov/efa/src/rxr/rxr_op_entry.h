/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
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

#ifndef _RXR_OP_ENTRY_H
#define _RXR_OP_ENTRY_H

#include "rxr_pkt_entry.h"

#define RXR_IOV_LIMIT		(4)

enum rxr_x_entry_type {
	RXR_TX_ENTRY = 1,
	RXR_RX_ENTRY,
	RXR_READ_ENTRY,
};

enum rxr_op_comm_type {
	RXR_OP_FREE = 0,	/* tx_entry/rx_entry free state */
	RXR_TX_REQ,		/* tx_entry sending REQ packet */
	RXR_TX_SEND,		/* tx_entry sending data in progress */
	RXR_RX_INIT,		/* rx_entry ready to recv RTM */
	RXR_RX_UNEXP,		/* rx_entry unexp msg waiting for post recv */
	RXR_RX_MATCHED,		/* rx_entry matched with RTM */
	RXR_RX_RECV,		/* rx_entry large msg recv data pkts */
	RXR_RX_QUEUED_CTRL,	/* rx_entry encountered error when sending control
				   it is in rxr_ep->rx_queued_entry_list, progress
				   engine will resend the ctrl packet */
	RXR_RX_WAIT_READ_FINISH, /* rx_entry wait for send to finish, FI_READ */
	RXR_RX_WAIT_ATOMRSP_SENT, /* rx_entry wait for atomrsp packet sent completion */
};

struct rxr_queued_ctrl_info {
	int type;
	int inject;
};

struct rxr_atomic_hdr {
	/* atomic_op is different from tx_op */
	uint32_t atomic_op;
	uint32_t datatype;
};

/* extra information that is not included in fi_msg_atomic
 * used by fetch atomic and compare atomic.
 *     resp stands for response
 *     comp stands for compare
 */
struct rxr_atomic_ex {
	struct iovec resp_iov[RXR_IOV_LIMIT];
	int resp_iov_count;
	struct iovec comp_iov[RXR_IOV_LIMIT];
	int comp_iov_count;
};

enum rxr_cuda_copy_method {
	RXR_CUDA_COPY_UNSPEC = 0,
	RXR_CUDA_COPY_GDRCOPY,
	RXR_CUDA_COPY_LOCALREAD
};

struct rxr_op_entry {
	/* type must remain at the top, can be RXR_TX_ENTRY or RXR_RX_ENTRY */
	enum rxr_x_entry_type type;

	fi_addr_t addr;
	struct rdm_peer *peer;

	uint32_t tx_id;
	uint32_t rx_id;
	uint32_t op;


	struct rxr_atomic_hdr atomic_hdr;
	struct rxr_atomic_ex atomic_ex;

	uint32_t msg_id;

	uint64_t tag;
	uint64_t ignore;

	int64_t window;

	uint64_t total_len;

	enum rxr_op_comm_type state;
	struct rxr_queued_ctrl_info queued_ctrl;

	uint64_t fi_flags;
	uint16_t rxr_flags;

	size_t iov_count;
	struct iovec iov[RXR_IOV_LIMIT];
	void *desc[RXR_IOV_LIMIT];
	struct fid_mr *mr[RXR_IOV_LIMIT];

	size_t rma_iov_count;
	struct fi_rma_iov rma_iov[RXR_IOV_LIMIT];

	struct fi_cq_tagged_entry cq_entry;

	struct rxr_read_entry *read_entry;

	/* For tx_entry, entry is linked with tx_pending_list in rxr_ep.
	 * For rx_entry, entry is linked with one of the receive lists: rx_list, rx_tagged_list,
	 * rx_unexp_list and rxr_unexp_tagged_list in rxr_ep.
	 */
	struct dlist_entry entry;

	/* ep_entry is linked to tx/rx_entry_list in rxr_ep */
	struct dlist_entry ep_entry;

	/* queued_ctrl_entry is linked with tx/rx_queued_ctrl_list in rxr_ep */
	struct dlist_entry queued_ctrl_entry;

	/* queued_rnr_entry is linked with tx/rx_queued_rnr_list in rxr_ep */
	struct dlist_entry queued_rnr_entry;

	/* Queued packets due to TX queue full or RNR backoff */
	struct dlist_entry queued_pkts;

	/* linked with tx/rx_entry_list in rdm_peer */
	struct dlist_entry peer_entry;

	uint64_t bytes_runt;

	/* the following variables are for RX operation only */
	uint64_t bytes_received;
	uint64_t bytes_received_via_mulreq;
	uint64_t bytes_copied;
	uint64_t bytes_queued_blocking_copy;

	/* linked to peer->rx_unexp_list or peer->rx_unexp_tagged_list */
	struct dlist_entry peer_unexp_entry;
#if ENABLE_DEBUG
	/* linked with op_entry_recv_list in rxr_ep */
	struct dlist_entry pending_recv_entry;
#endif

	size_t efa_outstanding_tx_ops; 
	size_t shm_outstanding_tx_ops;

	/*
	 * A list of rx_entries tracking FI_MULTI_RECV buffers. An rx_entry of
	 * type RXR_MULTI_RECV_POSTED that was created when the multi-recv
	 * buffer was posted is the list head, and the rx_entries of type
	 * RXR_MULTI_RECV_CONSUMER get added to the list as they consume the
	 * buffer.
	 */
	struct dlist_entry multi_recv_consumers;
	struct dlist_entry multi_recv_entry;
	struct rxr_op_entry *master_entry;
	struct fi_msg *posted_recv;
	struct rxr_pkt_entry *unexp_pkt;
	char *atomrsp_data;
	enum rxr_cuda_copy_method cuda_copy_method;
	/* end of RX related variables */

	/* the following variables are for TX operation only */
	uint64_t bytes_acked;
	uint64_t bytes_sent;
	uint64_t max_req_data_size;
	/* end of TX only variables */
};


#define RXR_GET_X_ENTRY_TYPE(pkt_entry)	\
	(*((enum rxr_x_entry_type *)	\
	 ((unsigned char *)((pkt_entry)->x_entry))))

/**
 * @brief return the op_entry of a packet entry
 *
 * If a packet entry is associate with a TX/RX entry,
 * this function return the op_entry for the packet entry.
 *
 * Note that not every packet entry are associated with an op entry.
 * For example:
 *     A HANDSHAKE packet is not associated with any operation.
 *     A RMA_CONTEX packet can be associated with a rxr_read_entry.
 *
 * @param[in]		pk_entry		packet entry
 * @return		pointer to the op_entry if the input packet entry is associated with an op
 * 			NULL otherwise
 */
static inline
struct rxr_op_entry *rxr_op_entry_of_pkt_entry(struct rxr_pkt_entry *pkt_entry)
{
	enum rxr_x_entry_type x_entry_type;
	/*
	 * pkt_entry->x_entry can be NULL when the packet is a HANDSHAKE packet
	 */
	if (!pkt_entry->x_entry)
		return NULL;

	x_entry_type = RXR_GET_X_ENTRY_TYPE(pkt_entry);
	return (x_entry_type == RXR_TX_ENTRY || x_entry_type == RXR_RX_ENTRY) ? pkt_entry->x_entry : NULL;
}

void rxr_tx_entry_try_fill_desc(struct rxr_op_entry *tx_entry,
				struct efa_domain *efa_domain,
				int mr_iov_start, uint64_t access);

struct rxr_ep;

void rxr_tx_entry_set_runt_size(struct rxr_ep *ep, struct rxr_op_entry *tx_entry);

size_t rxr_op_entry_mulreq_total_data_size(struct rxr_op_entry *op_entry, int pkt_type);

size_t rxr_tx_entry_max_req_data_capacity(struct rxr_ep *ep, struct rxr_op_entry *tx_entry, int pkt_type);

void rxr_tx_entry_set_max_req_data_size(struct rxr_ep *ep, struct rxr_op_entry *tx_entry, int pkt_type);

size_t rxr_tx_entry_num_req(struct rxr_op_entry *tx_entry, int pkt_type);

#endif
