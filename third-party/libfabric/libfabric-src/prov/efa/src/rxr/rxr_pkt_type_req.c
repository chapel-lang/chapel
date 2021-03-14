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

#include <ofi_atomic.h>
#include "efa.h"
#include "rxr.h"
#include "rxr_rma.h"
#include "rxr_msg.h"
#include "rxr_pkt_cmd.h"
#include "rxr_read.h"
#include "efa_cuda.h"

/*
 * Utility constants and funnctions shared by all REQ packe
 * types.
 */
struct rxr_req_inf {
	uint64_t protover;
	uint64_t base_hdr_size;
	uint64_t ex_feature_flag;
};

/*
 * starting from protocol version 4, each REQ packet type will be assigned a
 * version number, and once assigned, the version number will not change.
 *
 * Baseline features will always be version 4 features, baseline and
 * not have a ex_feature_flag.
 *
 * Each extra feature will be assign a version and an ex_feature_flag.
 * Each extra feature will correspond to 1 or more REQ packet types.
 */
static const
struct rxr_req_inf REQ_INF_LIST[] = {
	/* rtm header */
	[RXR_EAGER_MSGRTM_PKT] = {4, sizeof(struct rxr_eager_msgrtm_hdr), 0},
	[RXR_EAGER_TAGRTM_PKT] = {4, sizeof(struct rxr_eager_tagrtm_hdr), 0},
	[RXR_MEDIUM_MSGRTM_PKT] = {4, sizeof(struct rxr_medium_msgrtm_hdr), 0},
	[RXR_MEDIUM_TAGRTM_PKT] = {4, sizeof(struct rxr_medium_tagrtm_hdr), 0},
	[RXR_LONG_MSGRTM_PKT] = {4, sizeof(struct rxr_long_msgrtm_hdr), 0},
	[RXR_LONG_TAGRTM_PKT] = {4, sizeof(struct rxr_long_tagrtm_hdr), 0},
	[RXR_READ_MSGRTM_PKT] = {4, sizeof(struct rxr_read_msgrtm_hdr), RXR_REQ_FEATURE_RDMA_READ},
	[RXR_READ_TAGRTM_PKT] = {4, sizeof(struct rxr_read_tagrtm_hdr), RXR_REQ_FEATURE_RDMA_READ},
	/* rtw header */
	[RXR_EAGER_RTW_PKT] = {4, sizeof(struct rxr_eager_rtw_hdr), 0},
	[RXR_LONG_RTW_PKT] = {4, sizeof(struct rxr_long_rtw_hdr), 0},
	[RXR_READ_RTW_PKT] = {4, sizeof(struct rxr_read_rtw_hdr), RXR_REQ_FEATURE_RDMA_READ},
	/* rtr header */
	[RXR_SHORT_RTR_PKT] = {4, sizeof(struct rxr_rtr_hdr), 0},
	[RXR_LONG_RTR_PKT] = {4, sizeof(struct rxr_rtr_hdr), 0},
	[RXR_READ_RTR_PKT] = {4, sizeof(struct rxr_base_hdr), RXR_REQ_FEATURE_RDMA_READ},
	/* rta header */
	[RXR_WRITE_RTA_PKT] = {4, sizeof(struct rxr_rta_hdr), 0},
	[RXR_FETCH_RTA_PKT] = {4, sizeof(struct rxr_rta_hdr), 0},
	[RXR_COMPARE_RTA_PKT] = {4, sizeof(struct rxr_rta_hdr), 0},
};

size_t rxr_pkt_req_data_size(struct rxr_pkt_entry *pkt_entry)
{
	assert(pkt_entry->hdr_size > 0);
	return pkt_entry->pkt_size - pkt_entry->hdr_size;
}

void rxr_pkt_init_req_hdr(struct rxr_ep *ep,
			  struct rxr_tx_entry *tx_entry,
			  int pkt_type,
			  struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rxr_peer *peer;
	struct rxr_base_hdr *base_hdr;

	/* init the base header */
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->type = pkt_type;
	base_hdr->version = REQ_INF_LIST[pkt_type].protover;
	base_hdr->flags = 0;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(peer);
	if (OFI_UNLIKELY(!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED))) {
		/*
		 * This is the first communication with this peer on this
		 * endpoint, so send the core's address for this EP in the REQ
		 * so the remote side can insert it into its address vector.
		 */
		base_hdr->flags |= RXR_REQ_OPT_RAW_ADDR_HDR;
	}

	if (tx_entry->fi_flags & FI_REMOTE_CQ_DATA) {
		base_hdr->flags |= RXR_REQ_OPT_CQ_DATA_HDR;
	}

	/* init the opt header */
	opt_hdr = (char *)base_hdr + rxr_pkt_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;

		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		raw_addr_hdr->addr_len = ep->core_addrlen;
		memcpy(raw_addr_hdr->raw_addr, ep->core_addr, raw_addr_hdr->addr_len);
		opt_hdr += sizeof(*raw_addr_hdr) + raw_addr_hdr->addr_len;
	}

	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		struct rxr_req_opt_cq_data_hdr *cq_data_hdr;

		cq_data_hdr = (struct rxr_req_opt_cq_data_hdr *)opt_hdr;
		cq_data_hdr->cq_data = tx_entry->cq_entry.data;
		opt_hdr += sizeof(*cq_data_hdr);
	}

	pkt_entry->addr = tx_entry->addr;
	pkt_entry->hdr_size = opt_hdr - (char *)pkt_entry->pkt;
}

size_t rxr_pkt_req_base_hdr_size(struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	size_t hdr_size;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	assert(base_hdr->type >= RXR_REQ_PKT_BEGIN);

	hdr_size = REQ_INF_LIST[base_hdr->type].base_hdr_size;
	if (base_hdr->type == RXR_EAGER_RTW_PKT ||
	    base_hdr->type == RXR_LONG_RTW_PKT ||
	    base_hdr->type == RXR_READ_RTW_PKT)
		hdr_size += rxr_get_rtw_base_hdr(pkt_entry->pkt)->rma_iov_count * sizeof(struct fi_rma_iov);
	else if (base_hdr->type == RXR_SHORT_RTR_PKT ||
		 base_hdr->type == RXR_LONG_RTR_PKT)
		hdr_size += rxr_get_rtr_hdr(pkt_entry->pkt)->rma_iov_count * sizeof(struct fi_rma_iov);
	else if (base_hdr->type == RXR_WRITE_RTA_PKT ||
		 base_hdr->type == RXR_FETCH_RTA_PKT ||
		 base_hdr->type == RXR_COMPARE_RTA_PKT)
		hdr_size += rxr_get_rta_hdr(pkt_entry->pkt)->rma_iov_count * sizeof(struct fi_rma_iov);

	return hdr_size;
}

void rxr_pkt_proc_req_common_hdr(struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);

	opt_hdr = (char *)pkt_entry->pkt + rxr_pkt_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;

		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		pkt_entry->raw_addr = raw_addr_hdr->raw_addr;
		opt_hdr += sizeof(*raw_addr_hdr) + raw_addr_hdr->addr_len;
	} else {
		pkt_entry->raw_addr = NULL;
	}

	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		struct rxr_req_opt_cq_data_hdr *cq_data_hdr;

		cq_data_hdr = (struct rxr_req_opt_cq_data_hdr *)opt_hdr;
		pkt_entry->cq_data = cq_data_hdr->cq_data;
		opt_hdr += sizeof(struct rxr_req_opt_cq_data_hdr);
	}

	pkt_entry->hdr_size = opt_hdr - (char *)pkt_entry->pkt;
}

size_t rxr_pkt_req_max_data_size(struct rxr_ep *ep, fi_addr_t addr, int pkt_type)
{
	struct rxr_peer *peer;

	peer = rxr_ep_get_peer(ep, addr);
	assert(peer);

	if (peer->is_local) {
		assert(ep->use_shm);
		return rxr_env.shm_max_medium_size;
	}

	int max_hdr_size = REQ_INF_LIST[pkt_type].base_hdr_size
		+ sizeof(struct rxr_req_opt_raw_addr_hdr)
		+ sizeof(struct rxr_req_opt_cq_data_hdr);

	if (pkt_type == RXR_EAGER_RTW_PKT || pkt_type == RXR_LONG_RTW_PKT)
		max_hdr_size += RXR_IOV_LIMIT * sizeof(struct fi_rma_iov);

	return ep->mtu_size - max_hdr_size;
}

static
size_t rxr_pkt_req_copy_data(struct rxr_rx_entry *rx_entry,
			     struct rxr_pkt_entry *pkt_entry,
			     char *data, size_t data_size)
{
	size_t bytes_copied;
	int bytes_left;

	bytes_copied = rxr_copy_to_rx(data, data_size, rx_entry, 0);

	if (OFI_UNLIKELY(bytes_copied < data_size)) {
		/* recv buffer is not big enough to hold req, this must be a truncated message */
		assert(bytes_copied == rx_entry->cq_entry.len &&
		       rx_entry->cq_entry.len < rx_entry->total_len);
		rx_entry->bytes_done = bytes_copied;
		bytes_left = 0;
	} else {
		assert(bytes_copied == data_size);
		rx_entry->bytes_done = data_size;
		bytes_left = rx_entry->total_len - rx_entry->bytes_done;
	}

	assert(bytes_left >= 0);
	return bytes_left;
}

/*
 * REQ packet type functions
 *
 *     init() functions
 */

/*
 * this function is called after you have set header in pkt_entry->pkt and
 * pkt_entry->hdr_size
 */
void rxr_pkt_data_from_tx(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry,
			  struct rxr_tx_entry *tx_entry, size_t data_offset,
			  size_t data_size)
{
	int tx_iov_index;
	size_t tx_iov_offset;
	char *data;

	if (data_size == 0) {
		pkt_entry->iov_count = 0;
		pkt_entry->pkt_size = pkt_entry->hdr_size;
		return;
	}

	rxr_locate_iov_pos(tx_entry->iov, tx_entry->iov_count, data_offset,
			   &tx_iov_index, &tx_iov_offset);
	assert(tx_iov_index < tx_entry->iov_count);
	assert(tx_iov_offset < tx_entry->iov[tx_iov_index].iov_len);
	assert(pkt_entry->hdr_size > 0);
	if (!tx_entry->desc[tx_iov_index]) {
		data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
		data_size = rxr_copy_from_tx(data, data_size, tx_entry, data_offset);
		pkt_entry->iov_count = 0;
		pkt_entry->pkt_size = pkt_entry->hdr_size + data_size;
		return;
	}

	/* when desc is available, we use it instead of copying */
	assert(ep->core_iov_limit >= 2);
	pkt_entry->iov[0].iov_base = pkt_entry->pkt;
	pkt_entry->iov[0].iov_len = pkt_entry->hdr_size;
	pkt_entry->desc[0] = fi_mr_desc(pkt_entry->mr);

	pkt_entry->iov[1].iov_base = (char *)tx_entry->iov[tx_iov_index].iov_base + tx_iov_offset;
	pkt_entry->iov[1].iov_len = MIN(data_size,
					tx_entry->iov[tx_iov_index].iov_len - tx_iov_offset);
	pkt_entry->desc[1] = tx_entry->desc[tx_iov_index];
	pkt_entry->iov_count = 2;
	pkt_entry->pkt_size = pkt_entry->hdr_size + pkt_entry->iov[1].iov_len;
}

void rxr_pkt_init_rtm(struct rxr_ep *ep,
		      struct rxr_tx_entry *tx_entry,
		      int pkt_type, uint64_t data_offset,
		      struct rxr_pkt_entry *pkt_entry)
{
	size_t data_size;
	struct rxr_rtm_base_hdr *rtm_hdr;
	/* this function set pkt_entry->hdr_size */
	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);

	rtm_hdr = (struct rxr_rtm_base_hdr *)pkt_entry->pkt;
	rtm_hdr->flags |= RXR_REQ_MSG;
	rtm_hdr->msg_id = tx_entry->msg_id;

	data_size = MIN(tx_entry->total_len - data_offset, ep->mtu_size - pkt_entry->hdr_size);
	rxr_pkt_data_from_tx(ep, pkt_entry, tx_entry, data_offset, data_size);
	pkt_entry->x_entry = tx_entry;
}

ssize_t rxr_pkt_init_eager_msgrtm(struct rxr_ep *ep,
				  struct rxr_tx_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rtm(ep, tx_entry, RXR_EAGER_MSGRTM_PKT, 0, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_eager_tagrtm(struct rxr_ep *ep,
				  struct rxr_tx_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;

	rxr_pkt_init_rtm(ep, tx_entry, RXR_EAGER_TAGRTM_PKT, 0, pkt_entry);
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

ssize_t rxr_pkt_init_medium_msgrtm(struct rxr_ep *ep,
				   struct rxr_tx_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_medium_rtm_base_hdr *rtm_hdr;

	rxr_pkt_init_rtm(ep, tx_entry, RXR_MEDIUM_MSGRTM_PKT,
			 tx_entry->bytes_sent, pkt_entry);
	rtm_hdr = rxr_get_medium_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->data_len = tx_entry->total_len;
	rtm_hdr->offset = tx_entry->bytes_sent;
	return 0;
}

ssize_t rxr_pkt_init_medium_tagrtm(struct rxr_ep *ep,
				   struct rxr_tx_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_medium_rtm_base_hdr *rtm_hdr;

	rxr_pkt_init_rtm(ep, tx_entry, RXR_MEDIUM_TAGRTM_PKT,
			 tx_entry->bytes_sent, pkt_entry);
	rtm_hdr = rxr_get_medium_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->data_len = tx_entry->total_len;
	rtm_hdr->offset = tx_entry->bytes_sent;
	rtm_hdr->hdr.flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

void rxr_pkt_init_long_rtm(struct rxr_ep *ep,
			   struct rxr_tx_entry *tx_entry,
			   int pkt_type,
			   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_long_rtm_base_hdr *rtm_hdr;

	rxr_pkt_init_rtm(ep, tx_entry, pkt_type, 0, pkt_entry);
	rtm_hdr = rxr_get_long_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->data_len = tx_entry->total_len;
	rtm_hdr->tx_id = tx_entry->tx_id;
	rtm_hdr->credit_request = tx_entry->credit_request;
}

ssize_t rxr_pkt_init_long_msgrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_long_rtm(ep, tx_entry, RXR_LONG_MSGRTM_PKT, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_long_tagrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;

	rxr_pkt_init_long_rtm(ep, tx_entry, RXR_LONG_TAGRTM_PKT, pkt_entry);
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

ssize_t rxr_pkt_init_read_rtm(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      int pkt_type,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_read_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;
	int err;

	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);

	rtm_hdr = rxr_get_read_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->hdr.flags |= RXR_REQ_MSG;
	rtm_hdr->hdr.msg_id = tx_entry->msg_id;
	rtm_hdr->data_len = tx_entry->total_len;
	rtm_hdr->tx_id = tx_entry->tx_id;
	rtm_hdr->read_iov_count = tx_entry->iov_count;

	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + pkt_entry->hdr_size);
	err = rxr_read_init_iov(ep, tx_entry, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_entry->pkt_size = pkt_entry->hdr_size + tx_entry->iov_count * sizeof(struct fi_rma_iov);
	return 0;
}

ssize_t rxr_pkt_init_read_msgrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	return rxr_pkt_init_read_rtm(ep, tx_entry, RXR_READ_MSGRTM_PKT, pkt_entry);
}

ssize_t rxr_pkt_init_read_tagrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	struct rxr_base_hdr *base_hdr;

	err = rxr_pkt_init_read_rtm(ep, tx_entry, RXR_READ_TAGRTM_PKT, pkt_entry);
	if (err)
		return err;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

/*
 *     handle_sent() functions
 */

/*
 *         rxr_pkt_handle_eager_rtm_sent() is empty and is defined in rxr_pkt_type_req.h
 */
void rxr_pkt_handle_medium_rtm_sent(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += rxr_pkt_req_data_size(pkt_entry);
}

void rxr_pkt_handle_long_rtm_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += rxr_pkt_req_data_size(pkt_entry);
	assert(tx_entry->bytes_sent < tx_entry->total_len);

	if (efa_mr_cache_enable || rxr_ep_is_cuda_mr(tx_entry->desc[0]))
		rxr_prepare_desc_send(rxr_ep_domain(ep), tx_entry);
}

/*
 *     handle_send_completion() functions
 */
void rxr_pkt_handle_eager_rtm_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	assert(tx_entry->total_len == rxr_pkt_req_data_size(pkt_entry));
	rxr_cq_handle_tx_completion(ep, tx_entry);
}

void rxr_pkt_handle_medium_rtm_send_completion(struct rxr_ep *ep,
					       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_tx_completion(ep, tx_entry);
}

void rxr_pkt_handle_long_rtm_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_tx_completion(ep, tx_entry);
}

/*
 *     proc() functions
 */
size_t rxr_pkt_rtm_total_len(struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	switch (base_hdr->type) {
	case RXR_EAGER_MSGRTM_PKT:
	case RXR_EAGER_TAGRTM_PKT:
		return rxr_pkt_req_data_size(pkt_entry);
	case RXR_MEDIUM_MSGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
		return rxr_get_medium_rtm_base_hdr(pkt_entry->pkt)->data_len;
	case RXR_LONG_MSGRTM_PKT:
	case RXR_LONG_TAGRTM_PKT:
		return rxr_get_long_rtm_base_hdr(pkt_entry->pkt)->data_len;
	case RXR_READ_MSGRTM_PKT:
	case RXR_READ_TAGRTM_PKT:
		return rxr_get_read_rtm_base_hdr(pkt_entry->pkt)->data_len;
	default:
		assert(0 && "Unknown REQ packet type\n");
	}

	return 0;
}

void rxr_pkt_rtm_init_rx_entry(struct rxr_pkt_entry *pkt_entry,
			       struct rxr_rx_entry *rx_entry)
{
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		rx_entry->rxr_flags |= RXR_REMOTE_CQ_DATA;
		rx_entry->cq_entry.flags |= FI_REMOTE_CQ_DATA;
		rx_entry->cq_entry.data = pkt_entry->cq_data;
	}

	rx_entry->addr = pkt_entry->addr;
	rx_entry->msg_id = rxr_pkt_msg_id(pkt_entry);
	rx_entry->total_len = rxr_pkt_rtm_total_len(pkt_entry);
	rx_entry->tag = rxr_pkt_rtm_tag(pkt_entry);
	rx_entry->cq_entry.tag = rx_entry->tag;
}

struct rxr_rx_entry *rxr_pkt_get_rtm_matched_rx_entry(struct rxr_ep *ep,
						      struct dlist_entry *match,
						      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;

	assert(match);
	rx_entry = container_of(match, struct rxr_rx_entry, entry);
	if (rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED) {
		rx_entry = rxr_ep_split_rx_entry(ep, rx_entry,
						 NULL, pkt_entry);
		if (OFI_UNLIKELY(!rx_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"RX entries exhausted.\n");
			efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
			return NULL;
		}
	} else {
		rxr_pkt_rtm_init_rx_entry(pkt_entry, rx_entry);
	}

	rx_entry->state = RXR_RX_MATCHED;

	if (!(rx_entry->fi_flags & FI_MULTI_RECV) ||
	    !rxr_msg_multi_recv_buffer_available(ep, rx_entry->master_entry))
		dlist_remove(match);

	return rx_entry;
}

static
int rxr_pkt_rtm_match_recv_anyaddr(struct dlist_entry *item, const void *arg)
{
	return 1;
}

static
int rxr_pkt_rtm_match_recv(struct dlist_entry *item, const void *arg)
{
	const struct rxr_pkt_entry *pkt_entry = arg;
	struct rxr_rx_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_rx_entry, entry);
	return rxr_match_addr(rx_entry->addr, pkt_entry->addr);
}

static
int rxr_pkt_rtm_match_trecv_anyaddr(struct dlist_entry *item, const void *arg)
{
	struct rxr_pkt_entry *pkt_entry = (struct rxr_pkt_entry *)arg;
	struct rxr_rx_entry *rx_entry;
	uint64_t match_tag;

	rx_entry = container_of(item, struct rxr_rx_entry, entry);
	match_tag = rxr_pkt_rtm_tag(pkt_entry);

	return rxr_match_tag(rx_entry->cq_entry.tag, rx_entry->ignore,
			     match_tag);
}

static
int rxr_pkt_rtm_match_trecv(struct dlist_entry *item, const void *arg)
{
	struct rxr_pkt_entry *pkt_entry = (struct rxr_pkt_entry *)arg;
	struct rxr_rx_entry *rx_entry;
	uint64_t match_tag;

	rx_entry = container_of(item, struct rxr_rx_entry, entry);
	match_tag = rxr_pkt_rtm_tag(pkt_entry);

	return rxr_match_addr(rx_entry->addr, pkt_entry->addr) &&
	       rxr_match_tag(rx_entry->cq_entry.tag, rx_entry->ignore,
			     match_tag);
}

static
struct rxr_rx_entry *rxr_pkt_get_msgrtm_rx_entry(struct rxr_ep *ep,
						 struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rxr_rx_entry *rx_entry;
	struct dlist_entry *match;
	dlist_func_t *match_func;
	int pkt_type;

	if (ep->util_ep.caps & FI_DIRECTED_RECV)
		match_func = &rxr_pkt_rtm_match_recv;
	else
		match_func = &rxr_pkt_rtm_match_recv_anyaddr;

	match = dlist_find_first_match(&ep->rx_list, match_func,
	                               *pkt_entry_ptr);
	if (OFI_UNLIKELY(!match)) {
		/*
		 * rxr_ep_alloc_unexp_rx_entry_for_msgrtm() might release pkt_entry,
		 * thus we have to use pkt_entry_ptr here
		 */
		rx_entry = rxr_ep_alloc_unexp_rx_entry_for_msgrtm(ep, pkt_entry_ptr);
		if (OFI_UNLIKELY(!rx_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"RX entries exhausted.\n");
			efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
			return NULL;
		}

	} else {
		rx_entry = rxr_pkt_get_rtm_matched_rx_entry(ep, match, *pkt_entry_ptr);
	}

	pkt_type = rxr_get_base_hdr((*pkt_entry_ptr)->pkt)->type;
	if (pkt_type == RXR_MEDIUM_MSGRTM_PKT)
		rxr_pkt_rx_map_insert(ep, *pkt_entry_ptr, rx_entry);

	return rx_entry;
}

static
struct rxr_rx_entry *rxr_pkt_get_tagrtm_rx_entry(struct rxr_ep *ep,
						 struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rxr_rx_entry *rx_entry;
	struct dlist_entry *match;
	dlist_func_t *match_func;
	int pkt_type;

	if (ep->util_ep.caps & FI_DIRECTED_RECV)
		match_func = &rxr_pkt_rtm_match_trecv;
	else
		match_func = &rxr_pkt_rtm_match_trecv_anyaddr;

	match = dlist_find_first_match(&ep->rx_tagged_list, match_func,
	                               *pkt_entry_ptr);
	if (OFI_UNLIKELY(!match)) {
		/*
		 * rxr_ep_alloc_unexp_rx_entry_for_tagrtm() might release pkt_entry,
		 * thus we have to use pkt_entry_ptr here
		 */
		rx_entry = rxr_ep_alloc_unexp_rx_entry_for_tagrtm(ep, pkt_entry_ptr);
		if (OFI_UNLIKELY(!rx_entry)) {
			efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
			return NULL;
		}
	} else {
		rx_entry = rxr_pkt_get_rtm_matched_rx_entry(ep, match, *pkt_entry_ptr);
	}

	pkt_type = rxr_get_base_hdr((*pkt_entry_ptr)->pkt)->type;
	if (pkt_type == RXR_MEDIUM_TAGRTM_PKT)
		rxr_pkt_rx_map_insert(ep, *pkt_entry_ptr, rx_entry);

	return rx_entry;
}

ssize_t rxr_pkt_proc_matched_read_rtm(struct rxr_ep *ep,
				      struct rxr_rx_entry *rx_entry,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_read_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;

	rtm_hdr = rxr_get_read_rtm_base_hdr(pkt_entry->pkt);
	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + pkt_entry->hdr_size);

	rx_entry->tx_id = rtm_hdr->tx_id;
	rx_entry->rma_iov_count = rtm_hdr->read_iov_count;
	memcpy(rx_entry->rma_iov, read_iov,
	       rx_entry->rma_iov_count * sizeof(struct fi_rma_iov));

	rxr_pkt_entry_release_rx(ep, pkt_entry);

	/* truncate rx_entry->iov to save memory registration pages because we
	 * need to do memory registration for the receiving buffer.
	 */
	ofi_truncate_iov(rx_entry->iov, &rx_entry->iov_count, rx_entry->total_len);
	return rxr_read_post_or_queue(ep, RXR_RX_ENTRY, rx_entry);
}

ssize_t rxr_pkt_proc_matched_medium_rtm(struct rxr_ep *ep,
					struct rxr_rx_entry *rx_entry,
					struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_pkt_entry *cur;
	char *data;
	size_t offset, data_size;

	cur = pkt_entry;
	while (cur) {
		data = (char *)cur->pkt + cur->hdr_size;
		offset = rxr_get_medium_rtm_base_hdr(cur->pkt)->offset;
		data_size = cur->pkt_size - cur->hdr_size;
		rxr_copy_to_rx(data, data_size, rx_entry, offset);
		rx_entry->bytes_done += data_size;
		cur = cur->next;
	}

	if (rx_entry->total_len == rx_entry->bytes_done) {
		rxr_pkt_rx_map_remove(ep, pkt_entry, rx_entry);
		/*
		 * rxr_cq_handle_rx_completion() releases pkt_entry, thus
		 * we do not release it here.
		 */
		rxr_cq_handle_rx_completion(ep, pkt_entry, rx_entry);
		rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);
		rxr_release_rx_entry(ep, rx_entry);
		return 0;
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_proc_matched_rtm(struct rxr_ep *ep,
				 struct rxr_rx_entry *rx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	int pkt_type;
	char *data;
	size_t data_size, bytes_left;
	ssize_t ret;

	assert(rx_entry->state == RXR_RX_MATCHED);

	/* Adjust rx_entry->cq_entry.len as needed.
	 * Initialy rx_entry->cq_entry.len is total recv buffer size.
	 * rx_entry->total_len is from REQ packet and is total send buffer size.
	 * if send buffer size < recv buffer size, we adjust value of rx_entry->cq_entry.len
	 * if send buffer size > recv buffer size, we have a truncated message and will
	 * write error CQ entry.
	 */
	if (rx_entry->cq_entry.len > rx_entry->total_len)
		rx_entry->cq_entry.len = rx_entry->total_len;

	pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;
	if (pkt_type == RXR_READ_MSGRTM_PKT || pkt_type == RXR_READ_TAGRTM_PKT)
		return rxr_pkt_proc_matched_read_rtm(ep, rx_entry, pkt_entry);

	if (pkt_type == RXR_MEDIUM_MSGRTM_PKT || pkt_type == RXR_MEDIUM_TAGRTM_PKT)
		return rxr_pkt_proc_matched_medium_rtm(ep, rx_entry, pkt_entry);

	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	data_size = pkt_entry->pkt_size - pkt_entry->hdr_size;
	bytes_left = rxr_pkt_req_copy_data(rx_entry, pkt_entry,
					   data, data_size);
	if (!bytes_left) {
		/*
		 * rxr_cq_handle_rx_completion() releases pkt_entry, thus
		 * we do not release it here.
		 */
		rxr_cq_handle_rx_completion(ep, pkt_entry, rx_entry);
		rxr_msg_multi_recv_free_posted_entry(ep, rx_entry);
		rxr_release_rx_entry(ep, rx_entry);
		ret = 0;
	} else {
		/*
		 * long message protocol
		 */
#if ENABLE_DEBUG
		dlist_insert_tail(&rx_entry->rx_pending_entry, &ep->rx_pending_list);
		ep->rx_pending++;
#endif
		rx_entry->state = RXR_RX_RECV;
		rx_entry->tx_id = rxr_get_long_rtm_base_hdr(pkt_entry->pkt)->tx_id;
		/* we have noticed using the default value achieve better bandwidth */
		rx_entry->credit_request = rxr_env.tx_min_credits;
		ret = rxr_pkt_post_ctrl_or_queue(ep, RXR_RX_ENTRY, rx_entry, RXR_CTS_PKT, 0);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	}

	return ret;
}

ssize_t rxr_pkt_proc_msgrtm(struct rxr_ep *ep,
			    struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	struct rxr_rx_entry *rx_entry;

	rx_entry = rxr_pkt_get_msgrtm_rx_entry(ep, &pkt_entry);
	if (OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	if (rx_entry->state == RXR_RX_MATCHED) {
		err = rxr_pkt_proc_matched_rtm(ep, rx_entry, pkt_entry);
		if (OFI_UNLIKELY(err)) {
			if (rxr_cq_handle_rx_error(ep, rx_entry, err)) {
				assert(0 && "cannot write cq error entry");
				efa_eq_write_error(&ep->util_ep, -err, err);
			}
			rxr_pkt_entry_release_rx(ep, pkt_entry);
			rxr_release_rx_entry(ep, rx_entry);
			return err;
		}
	}

	return 0;
}

ssize_t rxr_pkt_proc_tagrtm(struct rxr_ep *ep,
			    struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	struct rxr_rx_entry *rx_entry;

	rx_entry = rxr_pkt_get_tagrtm_rx_entry(ep, &pkt_entry);
	if (OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	if (rx_entry->state == RXR_RX_MATCHED) {
		err = rxr_pkt_proc_matched_rtm(ep, rx_entry, pkt_entry);
		if (OFI_UNLIKELY(err)) {
			if (rxr_cq_handle_rx_error(ep, rx_entry, err)) {
				assert(0 && "cannot write error cq entry");
				efa_eq_write_error(&ep->util_ep, -err, err);
			}
			rxr_pkt_entry_release_rx(ep, pkt_entry);
			rxr_release_rx_entry(ep, rx_entry);
			return err;
		}
	}

	return 0;
}

/*
 * proc() functions called by rxr_pkt_handle_recv_completion()
 */
ssize_t rxr_pkt_proc_rtm_rta(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	assert(base_hdr->type >= RXR_BASELINE_REQ_PKT_BEGIN);

	switch (base_hdr->type) {
	case RXR_EAGER_MSGRTM_PKT:
	case RXR_MEDIUM_MSGRTM_PKT:
	case RXR_LONG_MSGRTM_PKT:
	case RXR_READ_MSGRTM_PKT:
		return rxr_pkt_proc_msgrtm(ep, pkt_entry);
	case RXR_EAGER_TAGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
	case RXR_LONG_TAGRTM_PKT:
	case RXR_READ_TAGRTM_PKT:
		return rxr_pkt_proc_tagrtm(ep, pkt_entry);
	case RXR_WRITE_RTA_PKT:
		return rxr_pkt_proc_write_rta(ep, pkt_entry);
	case RXR_FETCH_RTA_PKT:
		return rxr_pkt_proc_fetch_rta(ep, pkt_entry);
	case RXR_COMPARE_RTA_PKT:
		return rxr_pkt_proc_compare_rta(ep, pkt_entry);
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown packet type ID: %d\n",
		       base_hdr->type);
		if (rxr_cq_handle_cq_error(ep, -FI_EINVAL))
			assert(0 && "failed to write err cq entry");
	}

	return -FI_EINVAL;
}

void rxr_pkt_handle_rtm_rta_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	struct rxr_peer *peer;
	bool need_ordering;
	int ret, msg_id;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	assert(base_hdr->type >= RXR_BASELINE_REQ_PKT_BEGIN);

	if (base_hdr->type == RXR_MEDIUM_MSGRTM_PKT || base_hdr->type == RXR_MEDIUM_TAGRTM_PKT) {
		struct rxr_rx_entry *rx_entry;
		struct rxr_pkt_entry *unexp_pkt_entry;

		rx_entry = rxr_pkt_rx_map_lookup(ep, pkt_entry);
		if (rx_entry) {
			if (rx_entry->state == RXR_RX_MATCHED) {
				rxr_pkt_proc_matched_medium_rtm(ep, rx_entry, pkt_entry);
			} else {
				assert(rx_entry->unexp_pkt);
				unexp_pkt_entry = rxr_pkt_get_unexp(ep, &pkt_entry);
				rxr_pkt_entry_append(rx_entry->unexp_pkt, unexp_pkt_entry);
			}

			return;
		}
	}

	need_ordering = false;
	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	if (!peer->is_local) {
		/*
 		 * only need to reorder msg for efa_ep
		 */
		base_hdr = (struct rxr_base_hdr *)pkt_entry->pkt;
		if ((base_hdr->flags & RXR_REQ_MSG) && rxr_need_sas_ordering(ep))
			need_ordering = true;
		else if (base_hdr->flags & RXR_REQ_ATOMIC)
			need_ordering = true;
	}

	if (!need_ordering) {
		/* rxr_pkt_proc_rtm will write error cq entry if needed */
		rxr_pkt_proc_rtm_rta(ep, pkt_entry);
		return;
	}

	msg_id = rxr_pkt_msg_id(pkt_entry);
	ret = rxr_cq_reorder_msg(ep, peer, pkt_entry);
	if (ret == 1) {
		/* Packet was queued */
		return;
	}

	if (OFI_UNLIKELY(ret == -FI_EALREADY)) {
		/* Packet with same msg_id has been processed before */
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Invalid msg_id: %" PRIu32
			" robuf->exp_msg_id: %" PRIu32 "\n",
		       msg_id, peer->robuf->exp_msg_id);
		efa_eq_write_error(&ep->util_ep, FI_EIO, ret);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	if (OFI_UNLIKELY(ret == -FI_ENOMEM)) {
		/* running out of memory while copy packet */
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		return;
	}

	if (OFI_UNLIKELY(ret < 0)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown error %d processing REQ packet msg_id: %"
			PRIu32 "\n", ret, msg_id);
		efa_eq_write_error(&ep->util_ep, FI_EIO, ret);
		return;
	}


	/*
	 * rxr_pkt_proc_rtm_rta() will write error cq entry if needed,
	 * thus we do not write error cq entry
	 */
	ret = rxr_pkt_proc_rtm_rta(ep, pkt_entry);
	if (OFI_UNLIKELY(ret))
		return;

	ofi_recvwin_slide(peer->robuf);
	/* process pending items in reorder buff */
	rxr_cq_proc_pending_items_in_recvwin(ep, peer);
}

/*
 * RTW pakcet type functions
 */
void rxr_pkt_init_rtw_data(struct rxr_ep *ep,
			   struct rxr_tx_entry *tx_entry,
			   struct rxr_pkt_entry *pkt_entry,
			   struct fi_rma_iov *rma_iov)
{
	char *data;
	size_t data_size;
	int i;

	for (i = 0; i < tx_entry->rma_iov_count; ++i) {
		rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rma_iov[i].len = tx_entry->rma_iov[i].len;
		rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	data_size = ofi_copy_from_iov(data, ep->mtu_size - pkt_entry->hdr_size,
				      tx_entry->iov, tx_entry->iov_count, 0);

	pkt_entry->pkt_size = pkt_entry->hdr_size + data_size;
	pkt_entry->x_entry = tx_entry;
}

ssize_t rxr_pkt_init_eager_rtw(struct rxr_ep *ep,
			       struct rxr_tx_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_eager_rtw_hdr *rtw_hdr;

	assert(tx_entry->op == ofi_op_write);

	rtw_hdr = (struct rxr_eager_rtw_hdr *)pkt_entry->pkt;
	rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, RXR_EAGER_RTW_PKT, pkt_entry);
	rxr_pkt_init_rtw_data(ep, tx_entry, pkt_entry, rtw_hdr->rma_iov);
	return 0;
}

ssize_t rxr_pkt_init_long_rtw(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_long_rtw_hdr *rtw_hdr;

	assert(tx_entry->op == ofi_op_write);

	rtw_hdr = (struct rxr_long_rtw_hdr *)pkt_entry->pkt;
	rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rtw_hdr->data_len = tx_entry->total_len;
	rtw_hdr->tx_id = tx_entry->tx_id;
	rtw_hdr->credit_request = tx_entry->credit_request;
	rxr_pkt_init_req_hdr(ep, tx_entry, RXR_LONG_RTW_PKT, pkt_entry);
	rxr_pkt_init_rtw_data(ep, tx_entry, pkt_entry, rtw_hdr->rma_iov);
	return 0;
}

ssize_t rxr_pkt_init_read_rtw(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_read_rtw_hdr *rtw_hdr;
	struct fi_rma_iov *rma_iov, *read_iov;
	int i, err;

	assert(tx_entry->op == ofi_op_write);

	rtw_hdr = (struct rxr_read_rtw_hdr *)pkt_entry->pkt;
	rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rtw_hdr->data_len = tx_entry->total_len;
	rtw_hdr->tx_id = tx_entry->tx_id;
	rtw_hdr->read_iov_count = tx_entry->iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, RXR_READ_RTW_PKT, pkt_entry);

	rma_iov = rtw_hdr->rma_iov;
	for (i = 0; i < tx_entry->rma_iov_count; ++i) {
		rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rma_iov[i].len = tx_entry->rma_iov[i].len;
		rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + pkt_entry->hdr_size);
	err = rxr_read_init_iov(ep, tx_entry, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_entry->pkt_size = pkt_entry->hdr_size + tx_entry->iov_count * sizeof(struct fi_rma_iov);
	return 0;
}

/*
 *     handle_sent() functions for RTW packet types
 *
 *         rxr_pkt_handle_long_rtw_sent() is empty and is defined in rxr_pkt_type_req.h
 */
void rxr_pkt_handle_long_rtw_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += rxr_pkt_req_data_size(pkt_entry);
	assert(tx_entry->bytes_sent < tx_entry->total_len);
	if (efa_mr_cache_enable || rxr_ep_is_cuda_mr(tx_entry->desc[0]))
		rxr_prepare_desc_send(rxr_ep_domain(ep), tx_entry);
}

/*
 *     handle_send_completion() functions
 */
void rxr_pkt_handle_eager_rtw_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	assert(tx_entry->total_len == rxr_pkt_req_data_size(pkt_entry));
	rxr_cq_handle_tx_completion(ep, tx_entry);
}

void rxr_pkt_handle_long_rtw_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_tx_completion(ep, tx_entry);
}

/*
 *     handle_recv() functions
 */

static
struct rxr_rx_entry *rxr_pkt_alloc_rtw_rx_entry(struct rxr_ep *ep,
						struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	struct rxr_base_hdr *base_hdr;
	struct fi_msg msg = {0};

	msg.addr = pkt_entry->addr;
	rx_entry = rxr_ep_get_rx_entry(ep, &msg, 0, ~0, ofi_op_write, 0);
	if (OFI_UNLIKELY(!rx_entry))
		return NULL;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		rx_entry->rxr_flags |= RXR_REMOTE_CQ_DATA;
		rx_entry->cq_entry.flags |= FI_REMOTE_CQ_DATA;
		rx_entry->cq_entry.data = pkt_entry->cq_data;
	}

	rx_entry->addr = pkt_entry->addr;
	rx_entry->bytes_done = 0;
	return rx_entry;
}

void rxr_pkt_handle_eager_rtw_recv(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	struct rxr_eager_rtw_hdr *rtw_hdr;
	char *data;
	size_t data_size;
	ssize_t err, bytes_left;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);
	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rtw_hdr = (struct rxr_eager_rtw_hdr *)pkt_entry->pkt;
	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtw_hdr->rma_iov, rtw_hdr->rma_iov_count,
					FI_REMOTE_WRITE, rx_entry->iov);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verify failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EIO, err);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_WRITE);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	data_size = pkt_entry->pkt_size - pkt_entry->hdr_size;
	bytes_left = rxr_pkt_req_copy_data(rx_entry, pkt_entry, data, data_size);
	if (bytes_left != 0) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Eager RTM bytes_left is %ld, which should be 0.",
			bytes_left);
		FI_WARN(&rxr_prov, FI_LOG_CQ, "target buffer: %p length: %ld", rx_entry->iov[0].iov_base,
			rx_entry->iov[0].iov_len);
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, -FI_EINVAL);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	if (rx_entry->cq_entry.flags & FI_REMOTE_CQ_DATA)
		rxr_cq_write_rx_completion(ep, rx_entry);

	rxr_release_rx_entry(ep, rx_entry);
	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

void rxr_pkt_handle_long_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	struct rxr_long_rtw_hdr *rtw_hdr;
	char *data;
	size_t data_size;
	ssize_t err, bytes_left;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);
	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rtw_hdr = (struct rxr_long_rtw_hdr *)pkt_entry->pkt;
	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtw_hdr->rma_iov, rtw_hdr->rma_iov_count,
					FI_REMOTE_WRITE, rx_entry->iov);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verify failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EIO, err);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_WRITE);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	data_size = pkt_entry->pkt_size - pkt_entry->hdr_size;
	bytes_left = rxr_pkt_req_copy_data(rx_entry, pkt_entry, data, data_size);
	if (OFI_UNLIKELY(bytes_left <= 0)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Long RTM bytes_left is %ld, which should be > 0.",
			bytes_left);
		FI_WARN(&rxr_prov, FI_LOG_CQ, "target buffer: %p length: %ld", rx_entry->iov[0].iov_base,
			rx_entry->iov[0].iov_len);
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, -FI_EINVAL);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

#if ENABLE_DEBUG
	dlist_insert_tail(&rx_entry->rx_pending_entry, &ep->rx_pending_list);
	ep->rx_pending++;
#endif
	rx_entry->state = RXR_RX_RECV;
	rx_entry->tx_id = rtw_hdr->tx_id;
	rx_entry->credit_request = rxr_env.tx_min_credits;
	err = rxr_pkt_post_ctrl_or_queue(ep, RXR_RX_ENTRY, rx_entry, RXR_CTS_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Cannot post CTS packet\n");
		rxr_cq_handle_rx_error(ep, rx_entry, err);
		rxr_release_rx_entry(ep, rx_entry);
	}
	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

void rxr_pkt_handle_read_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	struct rxr_read_rtw_hdr *rtw_hdr;
	struct fi_rma_iov *read_iov;
	ssize_t err;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);
	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rtw_hdr = (struct rxr_read_rtw_hdr *)pkt_entry->pkt;
	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtw_hdr->rma_iov, rtw_hdr->rma_iov_count,
					FI_REMOTE_WRITE, rx_entry->iov);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verify failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, -FI_EINVAL);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_WRITE);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + pkt_entry->hdr_size);
	rx_entry->addr = pkt_entry->addr;
	rx_entry->tx_id = rtw_hdr->tx_id;
	rx_entry->rma_iov_count = rtw_hdr->read_iov_count;
	memcpy(rx_entry->rma_iov, read_iov,
	       rx_entry->rma_iov_count * sizeof(struct fi_rma_iov));

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	err = rxr_read_post_or_queue(ep, RXR_RX_ENTRY, rx_entry);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RDMA post read or queue failed.\n");
		efa_eq_write_error(&ep->util_ep, err, err);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	}
}

/*
 * RTR packet functions
 *     init() functions for RTR packets
 */
void rxr_pkt_init_rtr(struct rxr_ep *ep,
		      struct rxr_tx_entry *tx_entry,
		      int pkt_type, int window,
		      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rtr_hdr *rtr_hdr;
	int i;

	assert(tx_entry->op == ofi_op_read_req);
	rtr_hdr = (struct rxr_rtr_hdr *)pkt_entry->pkt;
	rtr_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);
	rtr_hdr->data_len = tx_entry->total_len;
	rtr_hdr->read_req_rx_id = tx_entry->rma_loc_rx_id;
	rtr_hdr->read_req_window = window;
	for (i = 0; i < tx_entry->rma_iov_count; ++i) {
		rtr_hdr->rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rtr_hdr->rma_iov[i].len = tx_entry->rma_iov[i].len;
		rtr_hdr->rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	pkt_entry->pkt_size = pkt_entry->hdr_size;
	pkt_entry->x_entry = tx_entry;
}

ssize_t rxr_pkt_init_short_rtr(struct rxr_ep *ep,
			       struct rxr_tx_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rtr(ep, tx_entry, RXR_SHORT_RTR_PKT, tx_entry->total_len, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_long_rtr(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rtr(ep, tx_entry, RXR_LONG_RTR_PKT, tx_entry->rma_window, pkt_entry);
	return 0;
}

/*
 *     handle_sent() functions for RTR packet types
 */
void rxr_pkt_handle_rtr_sent(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent = 0;
	tx_entry->state = RXR_TX_WAIT_READ_FINISH;
}

/*
 *     handle_send_completion() funciton for RTR packet
 */
void rxr_pkt_handle_rtr_send_completion(struct rxr_ep *ep,
					struct rxr_pkt_entry *pkt_entry)
{
	/*
	 * Unlike other protocol, for emulated read, tx_entry
	 * is release in rxr_cq_handle_rx_completion().
	 * therefore there is nothing to be done here.
	 */
	return;
}

/*
 *     handle_recv() functions for RTR packet
 */
void rxr_pkt_handle_rtr_recv(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rtr_hdr *rtr_hdr;
	struct rxr_rx_entry *rx_entry;
	struct rxr_tx_entry *tx_entry;
	ssize_t err;
	struct fi_msg msg = {0};

	msg.addr = pkt_entry->addr;
	rx_entry = rxr_ep_get_rx_entry(ep, &msg, 0, ~0, ofi_op_read_rsp, 0);
	if (OFI_UNLIKELY(!rx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->addr = pkt_entry->addr;
	rx_entry->bytes_done = 0;
	rx_entry->cq_entry.flags |= (FI_RMA | FI_READ);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	rtr_hdr = (struct rxr_rtr_hdr *)pkt_entry->pkt;
	rx_entry->rma_initiator_rx_id = rtr_hdr->read_req_rx_id;
	rx_entry->window = rtr_hdr->read_req_window;
	rx_entry->iov_count = rtr_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtr_hdr->rma_iov, rtr_hdr->rma_iov_count,
					FI_REMOTE_READ, rx_entry->iov);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verification failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, -FI_EINVAL);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	tx_entry = rxr_rma_alloc_readrsp_tx_entry(ep, rx_entry);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Readrsp tx entry exhausted!\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	err = rxr_pkt_post_ctrl_or_queue(ep, RXR_TX_ENTRY, tx_entry, RXR_READRSP_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Posting of readrsp packet failed! err=%ld\n", err);
		efa_eq_write_error(&ep->util_ep, FI_EIO, err);
		rxr_release_tx_entry(ep, tx_entry);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->state = RXR_RX_WAIT_READ_FINISH;
	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

/*
 * RTA packet functions
 */
ssize_t rxr_pkt_init_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry,
			 int pkt_type, struct rxr_pkt_entry *pkt_entry)
{
	struct fi_rma_iov *rma_iov;
	struct rxr_rta_hdr *rta_hdr;
	char *data;
	size_t data_size;
	int i;

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	rta_hdr->msg_id = tx_entry->msg_id;
	rta_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rta_hdr->atomic_datatype = tx_entry->atomic_hdr.datatype;
	rta_hdr->atomic_op = tx_entry->atomic_hdr.atomic_op;
	rta_hdr->tx_id = tx_entry->tx_id;
	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);
	rta_hdr->flags |= RXR_REQ_ATOMIC;
	rma_iov = rta_hdr->rma_iov;
	for (i=0; i < tx_entry->rma_iov_count; ++i) {
		rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rma_iov[i].len = tx_entry->rma_iov[i].len;
		rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	data_size = ofi_copy_from_iov(data, ep->mtu_size - pkt_entry->hdr_size,
				      tx_entry->iov, tx_entry->iov_count, 0);

	pkt_entry->pkt_size = pkt_entry->hdr_size + data_size;
	pkt_entry->x_entry = tx_entry;
	return 0;
}

ssize_t rxr_pkt_init_write_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry,
			    struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rta(ep, tx_entry, RXR_WRITE_RTA_PKT, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_fetch_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rta(ep, tx_entry, RXR_FETCH_RTA_PKT, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_compare_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	char *data;
	size_t data_size;

	rxr_pkt_init_rta(ep, tx_entry, RXR_COMPARE_RTA_PKT, pkt_entry);

	/* rxr_pkt_init_rta() will copy data from tx_entry->iov to pkt entry
	 * the following append the data to be compared
	 */
	data = (char *)pkt_entry->pkt + pkt_entry->pkt_size;
	data_size = ofi_copy_from_iov(data, ep->mtu_size - pkt_entry->pkt_size,
				      tx_entry->atomic_ex.comp_iov,
				      tx_entry->atomic_ex.comp_iov_count, 0);
	assert(data_size == tx_entry->total_len);
	pkt_entry->pkt_size += data_size;
	return 0;
}

void rxr_pkt_handle_write_rta_send_completion(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_tx_entry *tx_entry;

	tx_entry = (struct rxr_tx_entry *)pkt_entry->x_entry;
	rxr_cq_handle_tx_completion(ep, tx_entry);
}

int rxr_pkt_proc_write_rta(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct iovec iov[RXR_IOV_LIMIT];
	struct rxr_rta_hdr *rta_hdr;
	char *data;
	int iov_count, op, dt, i;
	size_t dtsize, offset;

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	op = rta_hdr->atomic_op;
	dt = rta_hdr->atomic_datatype;
	dtsize = ofi_datatype_size(dt);
	
	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	iov_count = rta_hdr->rma_iov_count;
	rxr_rma_verified_copy_iov(ep, rta_hdr->rma_iov, iov_count, FI_REMOTE_WRITE, iov);

	offset = 0;
	for (i = 0; i < iov_count; ++i) {
		ofi_atomic_write_handlers[op][dt](iov[i].iov_base,
						  data + offset,
						  iov[i].iov_len / dtsize);
		offset += iov[i].iov_len;
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return 0;
}

struct rxr_rx_entry *rxr_pkt_alloc_rta_rx_entry(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry, int op)
{
	struct rxr_rx_entry *rx_entry;
	struct rxr_rta_hdr *rta_hdr;
	struct fi_msg msg = {0};

	msg.addr = pkt_entry->addr;
	rx_entry = rxr_ep_get_rx_entry(ep, &msg, 0, ~0, op, 0);
	if (OFI_UNLIKELY(!rx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		return NULL;
	}

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	rx_entry->atomic_hdr.atomic_op = rta_hdr->atomic_op;
	rx_entry->atomic_hdr.datatype = rta_hdr->atomic_datatype;

	rx_entry->iov_count = rta_hdr->rma_iov_count;
	rxr_rma_verified_copy_iov(ep, rta_hdr->rma_iov, rx_entry->iov_count, FI_REMOTE_READ, rx_entry->iov);
	rx_entry->tx_id = rta_hdr->tx_id;
	rx_entry->total_len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	/*
	 * prepare a pkt entry to temporarily hold response data.
	 * Atomic_op operates on 3 data buffers:
	 *          local_data (input/output),
	 *          request_data (input),
	 *          response_data (output)
	 * The fact local data will be changed by atomic_op means
	 * response_data is not reproducible.
	 * Because sending response packet can fail due to
	 * -FI_EAGAIN, we need a temporary buffer to hold response_data.
	 * This packet entry will be release in rxr_handle_atomrsp_send_completion()
	 */
	rx_entry->atomrsp_pkt = rxr_pkt_entry_alloc(ep, ep->tx_pkt_efa_pool);
	if (!rx_entry->atomrsp_pkt) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"pkt entries exhausted.\n");
		rxr_release_rx_entry(ep, rx_entry);
		return NULL;
	}

	return rx_entry;
}

int rxr_pkt_proc_fetch_rta(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	char *data;
	int op, dt, i;
	size_t offset, dtsize;
	ssize_t err;

	rx_entry = rxr_pkt_alloc_rta_rx_entry(ep, pkt_entry, ofi_op_atomic_fetch);
	if(OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		return -FI_ENOBUFS;
	}

	op = rx_entry->atomic_hdr.atomic_op;
 	dt = rx_entry->atomic_hdr.datatype;	
	dtsize = ofi_datatype_size(rx_entry->atomic_hdr.datatype);

	data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	rx_entry->atomrsp_buf = (char *)rx_entry->atomrsp_pkt->pkt + sizeof(struct rxr_atomrsp_hdr);

	offset = 0;
	for (i = 0; i < rx_entry->iov_count; ++i) {
		ofi_atomic_readwrite_handlers[op][dt](rx_entry->iov[i].iov_base,
						      data + offset,
						      rx_entry->atomrsp_buf + offset,
						      rx_entry->iov[i].iov_len / dtsize);
		offset += rx_entry->iov[i].iov_len;
	}

	err = rxr_pkt_post_ctrl_or_queue(ep, RXR_RX_ENTRY, rx_entry, RXR_ATOMRSP_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		if (rxr_cq_handle_rx_error(ep, rx_entry, err))
			assert(0 && "Cannot handle rx error");
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return 0;
}

int rxr_pkt_proc_compare_rta(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rx_entry *rx_entry;
	char *src_data, *cmp_data;
	int op, dt, i;
	size_t offset, dtsize;
	ssize_t err;

	rx_entry = rxr_pkt_alloc_rta_rx_entry(ep, pkt_entry, ofi_op_atomic_compare);
	if(OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, -FI_ENOBUFS);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	op = rx_entry->atomic_hdr.atomic_op;
	dt = rx_entry->atomic_hdr.datatype;
       	dtsize = ofi_datatype_size(rx_entry->atomic_hdr.datatype);

	src_data = (char *)pkt_entry->pkt + pkt_entry->hdr_size;
	cmp_data = src_data + rx_entry->total_len;
	rx_entry->atomrsp_buf = (char *)rx_entry->atomrsp_pkt->pkt + sizeof(struct rxr_atomrsp_hdr);

	offset = 0;
	for (i = 0; i < rx_entry->iov_count; ++i) {
		ofi_atomic_swap_handlers[op - FI_CSWAP][dt](rx_entry->iov[i].iov_base,
							    src_data + offset,
							    cmp_data + offset,
							    rx_entry->atomrsp_buf + offset,
							    rx_entry->iov[i].iov_len / dtsize);
		offset += rx_entry->iov[i].iov_len;
	}

	err = rxr_pkt_post_ctrl_or_queue(ep, RXR_RX_ENTRY, rx_entry, RXR_ATOMRSP_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		efa_eq_write_error(&ep->util_ep, FI_EIO, err);
		rxr_pkt_entry_release_tx(ep, rx_entry->atomrsp_pkt);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return err;
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return 0;
}
