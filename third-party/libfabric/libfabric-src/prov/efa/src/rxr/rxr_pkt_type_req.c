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

#include <ofi_atomic.h>
#include "efa.h"
#include "rxr.h"
#include "rxr_rma.h"
#include "rxr_msg.h"
#include "rxr_pkt_cmd.h"
#include "rxr_pkt_hdr.h"
#include "rxr_pkt_type_base.h"
#include "rxr_read.h"

#include "rxr_tp.h"

/*
 * Utility constants and functions shared by all REQ packet
 * types.
 */
struct rxr_req_inf {
	uint64_t extra_info_id;
	uint64_t base_hdr_size;
	uint64_t ex_feature_flag;
};

static const
struct rxr_req_inf REQ_INF_LIST[] = {
	/* rtm header */
	[RXR_EAGER_MSGRTM_PKT] = {0, sizeof(struct rxr_eager_msgrtm_hdr), 0},
	[RXR_EAGER_TAGRTM_PKT] = {0, sizeof(struct rxr_eager_tagrtm_hdr), 0},
	[RXR_MEDIUM_MSGRTM_PKT] = {0, sizeof(struct rxr_medium_msgrtm_hdr), 0},
	[RXR_MEDIUM_TAGRTM_PKT] = {0, sizeof(struct rxr_medium_tagrtm_hdr), 0},
	[RXR_LONGCTS_MSGRTM_PKT] = {0, sizeof(struct rxr_longcts_msgrtm_hdr), 0},
	[RXR_LONGCTS_TAGRTM_PKT] = {0, sizeof(struct rxr_longcts_tagrtm_hdr), 0},
	[RXR_LONGREAD_MSGRTM_PKT] = {0, sizeof(struct rxr_longread_msgrtm_hdr), RXR_EXTRA_FEATURE_RDMA_READ},
	[RXR_LONGREAD_TAGRTM_PKT] = {0, sizeof(struct rxr_longread_tagrtm_hdr), RXR_EXTRA_FEATURE_RDMA_READ},
	[RXR_DC_EAGER_MSGRTM_PKT] = {0, sizeof(struct rxr_dc_eager_msgrtm_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_DC_EAGER_TAGRTM_PKT] = {0, sizeof(struct rxr_dc_eager_tagrtm_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_DC_MEDIUM_MSGRTM_PKT] = {0, sizeof(struct rxr_dc_medium_msgrtm_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_DC_MEDIUM_TAGRTM_PKT] = {0, sizeof(struct rxr_dc_medium_tagrtm_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_DC_LONGCTS_MSGRTM_PKT] = {0, sizeof(struct rxr_longcts_msgrtm_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_DC_LONGCTS_TAGRTM_PKT] = {0, sizeof(struct rxr_longcts_tagrtm_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_RUNTCTS_MSGRTM_PKT] = {0, sizeof(struct rxr_runtcts_msgrtm_hdr), RXR_EXTRA_FEATURE_RUNT},
	[RXR_RUNTCTS_TAGRTM_PKT] = {0, sizeof(struct rxr_runtcts_tagrtm_hdr), RXR_EXTRA_FEATURE_RUNT},
	[RXR_RUNTREAD_MSGRTM_PKT] = {0, sizeof(struct rxr_runtread_msgrtm_hdr), RXR_EXTRA_FEATURE_RUNT | RXR_EXTRA_FEATURE_RDMA_READ},
	[RXR_RUNTREAD_TAGRTM_PKT] = {0, sizeof(struct rxr_runtread_tagrtm_hdr), RXR_EXTRA_FEATURE_RUNT | RXR_EXTRA_FEATURE_RDMA_READ},
	/* rtw header */
	[RXR_EAGER_RTW_PKT] = {0, sizeof(struct rxr_eager_rtw_hdr), 0},
	[RXR_DC_EAGER_RTW_PKT] = {0, sizeof(struct rxr_dc_eager_rtw_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_LONGCTS_RTW_PKT] = {0, sizeof(struct rxr_longcts_rtw_hdr), 0},
	[RXR_DC_LONGCTS_RTW_PKT] = {0, sizeof(struct rxr_longcts_rtw_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_LONGREAD_RTW_PKT] = {0, sizeof(struct rxr_longread_rtw_hdr), RXR_EXTRA_FEATURE_RDMA_READ},
	[RXR_RUNTCTS_RTW_PKT] = {0, sizeof(struct rxr_runtcts_rtw_hdr), RXR_EXTRA_FEATURE_RUNT},
	[RXR_RUNTREAD_RTW_PKT] = {0, sizeof(struct rxr_runtread_rtw_hdr), RXR_EXTRA_FEATURE_RUNT},
	/* rtr header */
	[RXR_SHORT_RTR_PKT] = {0, sizeof(struct rxr_rtr_hdr), 0},
	[RXR_LONGCTS_RTR_PKT] = {0, sizeof(struct rxr_rtr_hdr), 0},
	[RXR_READ_RTR_PKT] = {0, sizeof(struct rxr_base_hdr), RXR_EXTRA_FEATURE_RDMA_READ},
	/* rta header */
	[RXR_WRITE_RTA_PKT] = {0, sizeof(struct rxr_rta_hdr), 0},
	[RXR_DC_WRITE_RTA_PKT] = {0, sizeof(struct rxr_rta_hdr), RXR_EXTRA_FEATURE_DELIVERY_COMPLETE},
	[RXR_FETCH_RTA_PKT] = {0, sizeof(struct rxr_rta_hdr), 0},
	[RXR_COMPARE_RTA_PKT] = {0, sizeof(struct rxr_rta_hdr), 0},
};

bool rxr_pkt_req_supported_by_peer(int req_type, struct rdm_peer *peer)
{
	assert(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED);

	int extra_info_id = REQ_INF_LIST[req_type].extra_info_id;

	return peer->extra_info[extra_info_id] & REQ_INF_LIST[req_type].ex_feature_flag;
}

size_t rxr_pkt_req_data_offset(struct rxr_pkt_entry *pkt_entry)
{
	int pkt_type, read_iov_count;
	size_t pkt_data_offset;

	pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;
	pkt_data_offset = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	assert(pkt_data_offset > 0);

	if (pkt_type == RXR_RUNTREAD_MSGRTM_PKT ||
	    pkt_type == RXR_RUNTREAD_TAGRTM_PKT) {
		read_iov_count = rxr_get_runtread_rtm_base_hdr(pkt_entry->pkt)->read_iov_count;
		pkt_data_offset +=  read_iov_count * sizeof(struct fi_rma_iov);
	}

	return pkt_data_offset;
}

size_t rxr_pkt_req_data_size(struct rxr_pkt_entry *pkt_entry)
{
	return pkt_entry->pkt_size - rxr_pkt_req_data_offset(pkt_entry);
}

/**
 * @brief Determine which Read based protocol to use
 *
 * @param[in] peer		rdm peer
 * @param[in] op		operation type
 * @param[in] flags		the flags that the application used to call fi_* functions
 * @param[in] hmem_info	configured protocol limits
 * @return The read-based protocol to use based on inputs.
 */
int rxr_pkt_type_readbase_rtm(struct rdm_peer *peer, int op, uint64_t fi_flags, struct efa_hmem_info *hmem_info)
{
	assert(op == ofi_op_tagged || op == ofi_op_msg);
	if (peer->num_read_msg_in_flight == 0 &&
	    hmem_info->runt_size > peer->num_runt_bytes_in_flight &&
	    !(fi_flags & FI_DELIVERY_COMPLETE)) {
		return (op == ofi_op_tagged) ? RXR_RUNTREAD_TAGRTM_PKT
					     : RXR_RUNTREAD_MSGRTM_PKT;
	} else {
		return (op == ofi_op_tagged) ? RXR_LONGREAD_TAGRTM_PKT
					     : RXR_LONGREAD_MSGRTM_PKT;
	}
}


void rxr_pkt_init_req_hdr(struct rxr_ep *ep,
			  struct rxr_op_entry *tx_entry,
			  int pkt_type,
			  struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rdm_peer *peer;
	struct rxr_base_hdr *base_hdr;

	/* init the base header */
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->type = pkt_type;
	base_hdr->version = RXR_PROTOCOL_VERSION;
	base_hdr->flags = 0;

	peer = rxr_ep_get_peer(ep, tx_entry->addr);
	assert(peer);

	if (rxr_peer_need_raw_addr_hdr(peer)) {
		/*
		 * This is the first communication with this peer on this
		 * endpoint, so send the core's address for this EP in the REQ
		 * so the remote side can insert it into its address vector.
		 */
		base_hdr->flags |= RXR_REQ_OPT_RAW_ADDR_HDR;
	} else if (rxr_peer_need_connid(peer)) {
		/*
		 * After receiving handshake packet, we will know the peer's capability.
		 *
		 * If the peer need connid, we will include the optional connid
		 * header in the req packet header.The peer will use it
		 * to verify my identity.
		 *
		 * This logic means that a req packet cannot have both
		 * the optional raw address header and the optional connid header.
		 */
		base_hdr->flags |= RXR_PKT_CONNID_HDR;
	}

	if (tx_entry->fi_flags & FI_REMOTE_CQ_DATA) {
		base_hdr->flags |= RXR_REQ_OPT_CQ_DATA_HDR;
	}

	/* init the opt header */
	opt_hdr = (char *)base_hdr + rxr_pkt_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;

		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		raw_addr_hdr->addr_len = RXR_REQ_OPT_RAW_ADDR_HDR_SIZE - sizeof(struct rxr_req_opt_raw_addr_hdr);
		assert(raw_addr_hdr->addr_len >= ep->core_addrlen);
		memcpy(raw_addr_hdr->raw_addr, ep->core_addr, ep->core_addrlen);
		opt_hdr += RXR_REQ_OPT_RAW_ADDR_HDR_SIZE;
	}

	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		struct rxr_req_opt_cq_data_hdr *cq_data_hdr;

		cq_data_hdr = (struct rxr_req_opt_cq_data_hdr *)opt_hdr;
		cq_data_hdr->cq_data = tx_entry->cq_entry.data;
		opt_hdr += sizeof(*cq_data_hdr);
	}

	if (base_hdr->flags & RXR_PKT_CONNID_HDR) {
		struct rxr_req_opt_connid_hdr *connid_hdr;

		connid_hdr = (struct rxr_req_opt_connid_hdr *)opt_hdr;
		connid_hdr->connid = rxr_ep_raw_addr(ep)->qkey;
		opt_hdr += sizeof(*connid_hdr);
	}

	pkt_entry->addr = tx_entry->addr;
	assert(opt_hdr - pkt_entry->pkt == rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry));
}

/**
 * @brief Get the value of rma_iov_count in pkt header.
 *
 * @param[in] pkt_entry the pkt entry
 * @return The rma_iov_count in the pkt header, if it is part of the header.
 * Otherwise return 0.
 */
uint32_t rxr_pkt_hdr_rma_iov_count(struct rxr_pkt_entry *pkt_entry)
{
	int pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;

	if (pkt_type == RXR_EAGER_RTW_PKT ||
	    pkt_type == RXR_DC_EAGER_RTW_PKT ||
	    pkt_type == RXR_LONGCTS_RTW_PKT ||
	    pkt_type == RXR_DC_LONGCTS_RTW_PKT ||
	    pkt_type == RXR_LONGREAD_RTW_PKT)
		return rxr_get_rtw_base_hdr(pkt_entry->pkt)->rma_iov_count;

	if (pkt_type == RXR_SHORT_RTR_PKT ||
		 pkt_type == RXR_LONGCTS_RTR_PKT)
		return rxr_get_rtr_hdr(pkt_entry->pkt)->rma_iov_count;

	if (pkt_type == RXR_WRITE_RTA_PKT ||
		 pkt_type == RXR_DC_WRITE_RTA_PKT ||
		 pkt_type == RXR_FETCH_RTA_PKT ||
		 pkt_type == RXR_COMPARE_RTA_PKT)
		return rxr_get_rta_hdr(pkt_entry->pkt)->rma_iov_count;

	return 0;
}

size_t rxr_pkt_req_base_hdr_size(struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	uint32_t rma_iov_count;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	assert(base_hdr->type >= RXR_REQ_PKT_BEGIN);

	rma_iov_count = rxr_pkt_hdr_rma_iov_count(pkt_entry);
	return REQ_INF_LIST[base_hdr->type].base_hdr_size +
	       rma_iov_count * sizeof(struct fi_rma_iov);
}

/**
 * @brief return the optional raw addr header pointer in a req packet
 *
 * @param[in]	pkt_entry	an REQ packet entry
 * @return	If the input has the optional raw addres header, return the pointer to it.
 *		Otherwise, return NULL
 */
void *rxr_pkt_req_raw_addr(struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rxr_base_hdr *base_hdr;
	struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	opt_hdr = (char *)pkt_entry->pkt + rxr_pkt_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		/* For req packet, the optional connid header and the optional
		 * raw address header are mutually exclusive.
		 */
		assert(!(base_hdr->flags & RXR_PKT_CONNID_HDR));
		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		return raw_addr_hdr->raw_addr;
	}

	return NULL;
}

/**
 * @brief return the pointer to connid in a req packet
 *
 * @param[in]	pkt_entry	an REQ packet entry
 * @return	If the input has the optional connid header, return the pointer to connid
 * 		Otherwise, return NULL
 */
uint32_t *rxr_pkt_req_connid_ptr(struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rxr_base_hdr *base_hdr;
	struct rxr_req_opt_connid_hdr *connid_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	opt_hdr = (char *)pkt_entry->pkt + rxr_pkt_req_base_hdr_size(pkt_entry);

	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;
		struct efa_ep_addr *raw_addr;

		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		raw_addr = (struct efa_ep_addr *)raw_addr_hdr->raw_addr;
		return &raw_addr->qkey;
	}

	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR)
		opt_hdr += sizeof(struct rxr_req_opt_cq_data_hdr);

	if (base_hdr->flags & RXR_PKT_CONNID_HDR) {
		connid_hdr = (struct rxr_req_opt_connid_hdr *)opt_hdr;
		return &connid_hdr->connid;
	}

	return NULL;
}

/**
 * @brief calculate the exact header size for a given REQ pkt_entry
 *
 * The difference between this function and rxr_pkt_req_hdr_size() is
 * the handling of the size of req opt raw address header.
 *
 * rxr_pkt_req_hdr_size() always use RXR_REQ_OPT_RAW_ADDR_HDR_SIZE, while
 * this function pull raw address from pkt_entry's size.
 *
 * The difference is because older version of libfabric EFA provider uses
 * a different opt header size.
 *
 * @param[in]	pkt_entry		packet entry
 * @return 	header size of the REQ packet entry
 */
size_t rxr_pkt_req_hdr_size_from_pkt_entry(struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rxr_base_hdr *base_hdr;
	struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	opt_hdr = (char *)pkt_entry->pkt + rxr_pkt_req_base_hdr_size(pkt_entry);

	/*
	 * It is not possible to have both optional raw addr header and optional
	 * connid header in a packet header.
	 */
	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		assert(!(base_hdr->flags & RXR_PKT_CONNID_HDR));
		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		opt_hdr += sizeof(struct rxr_req_opt_raw_addr_hdr) + raw_addr_hdr->addr_len;
	}

	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR)
		opt_hdr += sizeof(struct rxr_req_opt_cq_data_hdr);

	if (base_hdr->flags & RXR_PKT_CONNID_HDR) {
		assert(!(base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR));
		opt_hdr += sizeof(struct rxr_req_opt_connid_hdr);
	}

	return opt_hdr - (char *)pkt_entry->pkt;
}

int64_t rxr_pkt_req_cq_data(struct rxr_pkt_entry *pkt_entry)
{
	char *opt_hdr;
	struct rxr_base_hdr *base_hdr;
	struct rxr_req_opt_cq_data_hdr *cq_data_hdr;
	struct rxr_req_opt_raw_addr_hdr *raw_addr_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	opt_hdr = (char *)pkt_entry->pkt + rxr_pkt_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		raw_addr_hdr = (struct rxr_req_opt_raw_addr_hdr *)opt_hdr;
		opt_hdr += sizeof(struct rxr_req_opt_raw_addr_hdr) + raw_addr_hdr->addr_len;
	}

	assert(base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR);
	cq_data_hdr = (struct rxr_req_opt_cq_data_hdr *)opt_hdr;
	return cq_data_hdr->cq_data;
}

/**
 * @brief calculates the exact header size given a REQ packet type, flags, and IOV count.
 *
 * @param[in]	pkt_type	packet type
 * @param[in]	flags	flags from packet
 * @param[in]	rma_iov_count	number of RMA IOV structures present
 * @return	The exact size of the packet header
 */
inline
size_t rxr_pkt_req_hdr_size(int pkt_type, uint16_t flags, size_t rma_iov_count)
{
	int hdr_size = REQ_INF_LIST[pkt_type].base_hdr_size;

	if (flags & RXR_REQ_OPT_RAW_ADDR_HDR) {
		/* It is impossible to have both optional connid hdr and opt_raw_addr_hdr
		 * in the header, and length of opt raw addr hdr is larger than
		 * connid hdr (which is confirmed by the following assertion).
		 */
		assert(RXR_REQ_OPT_RAW_ADDR_HDR_SIZE >= sizeof(struct rxr_req_opt_connid_hdr));
		hdr_size += RXR_REQ_OPT_RAW_ADDR_HDR_SIZE;
	} else if (flags & RXR_PKT_CONNID_HDR) {
		hdr_size += sizeof(struct rxr_req_opt_connid_hdr);
	}

	if (flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		hdr_size += sizeof(struct rxr_req_opt_cq_data_hdr);
	}

	if (rxr_pkt_type_contains_rma_iov(pkt_type)) {
		hdr_size += rma_iov_count * sizeof(struct fi_rma_iov);
	}

	return hdr_size;
}

/**
 * @brief calculates the max header size given a REQ packet type
 *
 * @param[in]	pkt_type	packet type
 * @return	The max possible size of the packet header
 */
inline size_t rxr_pkt_req_max_hdr_size(int pkt_type)
{
	/* To calculate max REQ reader size, we should include all possible REQ opt header flags.
	 * However, because the optional connid header and optional raw address header cannot
	 * exist at the same time, and the raw address header is longer than connid header,
	 * we did not include the flag for CONNID header
	 */
	uint16_t header_flags = RXR_REQ_OPT_RAW_ADDR_HDR | RXR_REQ_OPT_CQ_DATA_HDR;

	return rxr_pkt_req_hdr_size(pkt_type, header_flags, RXR_IOV_LIMIT);
}

size_t rxr_pkt_max_hdr_size(void)
{
	size_t max_hdr_size = 0;
	size_t pkt_type = RXR_REQ_PKT_BEGIN;

	while (pkt_type < RXR_EXTRA_REQ_PKT_END) {
		max_hdr_size = MAX(max_hdr_size,
				rxr_pkt_req_max_hdr_size(pkt_type));
		if (pkt_type == RXR_BASELINE_REQ_PKT_END)
			pkt_type = RXR_EXTRA_REQ_PKT_BEGIN;
		else
			pkt_type += 1;
	}

	return max_hdr_size;
}


/*
 * REQ packet type functions
 *
 *     init() functions
 */
static inline
int rxr_pkt_init_rtm(struct rxr_ep *ep,
		     struct rxr_op_entry *tx_entry,
		     int pkt_type, uint64_t data_offset,
		     struct rxr_pkt_entry *pkt_entry)
{
	size_t data_size;
	struct rxr_rtm_base_hdr *rtm_hdr;
	int ret;
	static const size_t CUDA_MEMORY_ALIGNMENT = 64;

	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);

	rtm_hdr = (struct rxr_rtm_base_hdr *)pkt_entry->pkt;
	rtm_hdr->flags |= RXR_REQ_MSG;
	rtm_hdr->msg_id = tx_entry->msg_id;

	data_size = MIN(tx_entry->total_len - data_offset,
			ep->mtu_size - rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry));
	if (data_size + data_offset < tx_entry->total_len) {

		if (tx_entry->max_req_data_size > 0)
			data_size = MIN(data_size, tx_entry->max_req_data_size);

		if (efa_mr_is_cuda(tx_entry->desc[0]))
			data_size &= ~(CUDA_MEMORY_ALIGNMENT -1);
	}


	ret = rxr_pkt_init_data_from_op_entry(ep, pkt_entry, rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry),
					      tx_entry, data_offset, data_size);
	return ret;
}

ssize_t rxr_pkt_init_eager_msgrtm(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry)
{
	int ret;

	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_EAGER_MSGRTM_PKT, 0, pkt_entry);
	if (ret)
		return ret;

	assert(tx_entry->total_len == rxr_pkt_req_data_size(pkt_entry));
	return 0;
}

ssize_t rxr_pkt_init_dc_eager_msgrtm(struct rxr_ep *ep,
				     struct rxr_op_entry *tx_entry,
				     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_dc_eager_msgrtm_hdr *dc_eager_msgrtm_hdr;
	int ret;

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_DC_EAGER_MSGRTM_PKT, 0, pkt_entry);
	if (ret)
		return ret;
	dc_eager_msgrtm_hdr = rxr_get_dc_eager_msgrtm_hdr(pkt_entry->pkt);
	dc_eager_msgrtm_hdr->hdr.send_id = tx_entry->tx_id;
	return 0;
}

ssize_t rxr_pkt_init_eager_tagrtm(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	int ret;

	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_EAGER_TAGRTM_PKT, 0, pkt_entry);
	if (ret)
		return ret;
	assert(tx_entry->total_len == rxr_pkt_req_data_size(pkt_entry));
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

ssize_t rxr_pkt_init_dc_eager_tagrtm(struct rxr_ep *ep,
				     struct rxr_op_entry *tx_entry,
				     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	struct rxr_dc_eager_tagrtm_hdr *dc_eager_tagrtm_hdr;
	int ret;

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_DC_EAGER_TAGRTM_PKT, 0, pkt_entry);
	if (ret)
		return ret;
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);

	dc_eager_tagrtm_hdr = rxr_get_dc_eager_tagrtm_hdr(pkt_entry->pkt);
	dc_eager_tagrtm_hdr->hdr.send_id = tx_entry->tx_id;
	return 0;
}

ssize_t rxr_pkt_init_medium_msgrtm(struct rxr_ep *ep,
				   struct rxr_op_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct efa_domain *efa_domain;
	struct rxr_medium_rtm_base_hdr *rtm_hdr;
	int ret;

	efa_domain = rxr_ep_domain(ep);
	rxr_tx_entry_try_fill_desc(tx_entry, efa_domain, 0, FI_SEND);

	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_MEDIUM_MSGRTM_PKT,
			       tx_entry->bytes_sent, pkt_entry);
	if (ret)
		return ret;

	rtm_hdr = rxr_get_medium_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->msg_length = tx_entry->total_len;
	rtm_hdr->seg_offset = tx_entry->bytes_sent;
	return 0;
}

ssize_t rxr_pkt_init_dc_medium_msgrtm(struct rxr_ep *ep,
				      struct rxr_op_entry *tx_entry,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct efa_domain *efa_domain;
	struct rxr_dc_medium_msgrtm_hdr *dc_medium_msgrtm_hdr;
	int ret;

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;

	efa_domain = rxr_ep_domain(ep);
	rxr_tx_entry_try_fill_desc(tx_entry, efa_domain, 0, FI_SEND);

	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_DC_MEDIUM_MSGRTM_PKT,
			       tx_entry->bytes_sent, pkt_entry);
	if (ret)
		return ret;

	dc_medium_msgrtm_hdr = rxr_get_dc_medium_msgrtm_hdr(pkt_entry->pkt);
	dc_medium_msgrtm_hdr->hdr.msg_length = tx_entry->total_len;
	dc_medium_msgrtm_hdr->hdr.seg_offset = tx_entry->bytes_sent;
	dc_medium_msgrtm_hdr->hdr.send_id = tx_entry->tx_id;
	return 0;
}

ssize_t rxr_pkt_init_medium_tagrtm(struct rxr_ep *ep,
				   struct rxr_op_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct efa_domain *efa_domain;
	struct rxr_medium_rtm_base_hdr *rtm_hdr;
	int ret;

	efa_domain = rxr_ep_domain(ep);
	rxr_tx_entry_try_fill_desc(tx_entry, efa_domain, 0, FI_SEND);

	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_MEDIUM_TAGRTM_PKT,
			       tx_entry->bytes_sent, pkt_entry);
	if (ret)
		return ret;

	rtm_hdr = rxr_get_medium_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->msg_length = tx_entry->total_len;
	rtm_hdr->seg_offset = tx_entry->bytes_sent;
	rtm_hdr->hdr.flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

ssize_t rxr_pkt_init_dc_medium_tagrtm(struct rxr_ep *ep,
				      struct rxr_op_entry *tx_entry,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct efa_domain *efa_domain;
	struct rxr_dc_medium_tagrtm_hdr *dc_medium_tagrtm_hdr;
	int ret;

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;

	efa_domain = rxr_ep_domain(ep);
	rxr_tx_entry_try_fill_desc(tx_entry, efa_domain, 0, FI_SEND);

	ret = rxr_pkt_init_rtm(ep, tx_entry, RXR_DC_MEDIUM_TAGRTM_PKT,
			       tx_entry->bytes_sent, pkt_entry);
	if (ret)
		return ret;

	dc_medium_tagrtm_hdr = rxr_get_dc_medium_tagrtm_hdr(pkt_entry->pkt);
	dc_medium_tagrtm_hdr->hdr.msg_length = tx_entry->total_len;
	dc_medium_tagrtm_hdr->hdr.seg_offset = tx_entry->bytes_sent;
	dc_medium_tagrtm_hdr->hdr.hdr.flags |= RXR_REQ_TAGGED;
	dc_medium_tagrtm_hdr->hdr.send_id = tx_entry->tx_id;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

int rxr_pkt_init_longcts_rtm(struct rxr_ep *ep,
			     struct rxr_op_entry *tx_entry,
			     int pkt_type,
			     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_longcts_rtm_base_hdr *rtm_hdr;
	int ret;

	tx_entry->rxr_flags |= RXR_LONGCTS_PROTOCOL;
	ret = rxr_pkt_init_rtm(ep, tx_entry, pkt_type, 0, pkt_entry);
	if (ret)
		return ret;

	rtm_hdr = rxr_get_longcts_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->msg_length = tx_entry->total_len;
	rtm_hdr->send_id = tx_entry->tx_id;
	rtm_hdr->credit_request = rxr_env.tx_min_credits;
	return 0;
}

ssize_t rxr_pkt_init_longcts_msgrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	return rxr_pkt_init_longcts_rtm(ep, tx_entry, RXR_LONGCTS_MSGRTM_PKT, pkt_entry);
}

ssize_t rxr_pkt_init_dc_longcts_msgrtm(struct rxr_ep *ep,
				    struct rxr_op_entry *tx_entry,
				    struct rxr_pkt_entry *pkt_entry)
{
	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	return rxr_pkt_init_longcts_rtm(ep, tx_entry, RXR_DC_LONGCTS_MSGRTM_PKT, pkt_entry);
}

ssize_t rxr_pkt_init_longcts_tagrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	int ret;

	ret = rxr_pkt_init_longcts_rtm(ep, tx_entry, RXR_LONGCTS_TAGRTM_PKT, pkt_entry);
	if (ret)
		return ret;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

ssize_t rxr_pkt_init_dc_longcts_tagrtm(struct rxr_ep *ep,
				    struct rxr_op_entry *tx_entry,
				    struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	int ret;

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	ret = rxr_pkt_init_longcts_rtm(ep, tx_entry, RXR_DC_LONGCTS_TAGRTM_PKT, pkt_entry);
	if (ret)
		return ret;
	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

ssize_t rxr_pkt_init_longread_rtm(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      int pkt_type,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_longread_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;
	size_t hdr_size;
	int err;

	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);

	rtm_hdr = rxr_get_longread_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->hdr.flags |= RXR_REQ_MSG;
	rtm_hdr->hdr.msg_id = tx_entry->msg_id;
	rtm_hdr->msg_length = tx_entry->total_len;
	rtm_hdr->send_id = tx_entry->tx_id;
	rtm_hdr->read_iov_count = tx_entry->iov_count;

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + hdr_size);
	err = rxr_read_init_iov(ep, tx_entry, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_entry->pkt_size = hdr_size + tx_entry->iov_count * sizeof(struct fi_rma_iov);
	pkt_entry->x_entry = tx_entry;
	return 0;
}

ssize_t rxr_pkt_init_longread_msgrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	return rxr_pkt_init_longread_rtm(ep, tx_entry, RXR_LONGREAD_MSGRTM_PKT, pkt_entry);
}

ssize_t rxr_pkt_init_longread_tagrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	struct rxr_base_hdr *base_hdr;

	err = rxr_pkt_init_longread_rtm(ep, tx_entry, RXR_LONGREAD_TAGRTM_PKT, pkt_entry);
	if (err)
		return err;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	base_hdr->flags |= RXR_REQ_TAGGED;
	rxr_pkt_rtm_settag(pkt_entry, tx_entry->tag);
	return 0;
}

/**
 * @brief fill in the rxr_runtread_rtm_base_hdr and data of a RUNTREAD packet
 *
 * only thing left unset is tag
 *
 * @param[in]		ep		end point
 * @param[in]		tx_entry	contains information of the send operation
 * @param[in]		pkt_type	RXR_RUNREAD_MSGRTM or RXR_RUNTREAD_TAGRTM
 * @param[out]		pkt_entry	pkt_entry to be initialzied
 */
static
ssize_t rxr_pkt_init_runtread_rtm(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  int pkt_type,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_runtread_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;
	size_t hdr_size, pkt_data_offset, tx_data_offset, data_size;
	int err;

	assert(tx_entry->bytes_runt);
	assert(tx_entry->bytes_sent < tx_entry->bytes_runt);

	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);

	rtm_hdr = rxr_get_runtread_rtm_base_hdr(pkt_entry->pkt);
	rtm_hdr->hdr.flags |= RXR_REQ_MSG;
	rtm_hdr->hdr.msg_id = tx_entry->msg_id;
	rtm_hdr->msg_length = tx_entry->total_len;
	rtm_hdr->send_id = tx_entry->tx_id;
	rtm_hdr->seg_offset = tx_entry->bytes_sent;
	rtm_hdr->runt_length = tx_entry->bytes_runt;
	rtm_hdr->read_iov_count = tx_entry->iov_count;

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + hdr_size);
	err = rxr_read_init_iov(ep, tx_entry, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_data_offset  = hdr_size + tx_entry->iov_count * sizeof(struct fi_rma_iov);
	tx_data_offset = tx_entry->bytes_sent;
	data_size = MIN(tx_entry->bytes_runt - tx_entry->bytes_sent,
			ep->mtu_size - pkt_data_offset);

	if (tx_entry->max_req_data_size && data_size > tx_entry->max_req_data_size)
		data_size = tx_entry->max_req_data_size;

	return rxr_pkt_init_data_from_op_entry(ep, pkt_entry, pkt_data_offset, tx_entry, tx_data_offset, data_size);
}

ssize_t rxr_pkt_init_runtread_msgrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	return rxr_pkt_init_runtread_rtm(ep, tx_entry, RXR_RUNTREAD_MSGRTM_PKT, pkt_entry);
}

ssize_t rxr_pkt_init_runtread_tagrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	struct rxr_base_hdr *base_hdr;

	err = rxr_pkt_init_runtread_rtm(ep, tx_entry, RXR_RUNTREAD_TAGRTM_PKT, pkt_entry);
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
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += rxr_pkt_req_data_size(pkt_entry);
}

void rxr_pkt_handle_longcts_rtm_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;
	struct efa_domain *efa_domain;

	efa_domain = rxr_ep_domain(ep);
	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += rxr_pkt_req_data_size(pkt_entry);
	assert(tx_entry->bytes_sent < tx_entry->total_len);

	if (tx_entry->desc[0] || efa_is_cache_available(efa_domain))
		rxr_prepare_desc_send(efa_domain, tx_entry);
}


void rxr_pkt_handle_longread_rtm_sent(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct rdm_peer *peer;

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	peer->num_read_msg_in_flight += 1;
}

void rxr_pkt_handle_runtread_rtm_sent(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct rdm_peer *peer;
	struct rxr_op_entry *tx_entry;
	size_t pkt_data_size = rxr_pkt_req_data_size(pkt_entry);

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += pkt_data_size;
	peer->num_runt_bytes_in_flight += pkt_data_size;

	if (rxr_get_runtread_rtm_base_hdr(pkt_entry->pkt)->seg_offset == 0 &&
	    tx_entry->total_len > tx_entry->bytes_runt)
		peer->num_read_msg_in_flight += 1;
}

/*
 *     handle_send_completion() functions
 */
void rxr_pkt_handle_eager_rtm_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	assert(tx_entry->total_len == rxr_pkt_req_data_size(pkt_entry));
	rxr_cq_handle_send_completion(ep, tx_entry);
}

void rxr_pkt_handle_medium_rtm_send_completion(struct rxr_ep *ep,
					       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_send_completion(ep, tx_entry);
}

void rxr_pkt_handle_longcts_rtm_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_send_completion(ep, tx_entry);
}

void rxr_pkt_handle_dc_longcts_rtm_send_completion(struct rxr_ep *ep,
						struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked &&
	    tx_entry->rxr_flags & RXR_RECEIPT_RECEIVED)
		rxr_cq_handle_send_completion(ep, tx_entry);
}

void rxr_pkt_handle_runtread_rtm_send_completion(struct rxr_ep *ep,
						 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;
	struct rdm_peer *peer;
	size_t pkt_data_size;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	pkt_data_size = rxr_pkt_req_data_size(pkt_entry);
	tx_entry->bytes_acked += pkt_data_size;

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
	assert(peer->num_runt_bytes_in_flight >= pkt_data_size);
	peer->num_runt_bytes_in_flight -= pkt_data_size;
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_send_completion(ep, tx_entry);
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
	case RXR_DC_EAGER_MSGRTM_PKT:
	case RXR_DC_EAGER_TAGRTM_PKT:
		return rxr_pkt_req_data_size(pkt_entry);
	case RXR_MEDIUM_MSGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
		return rxr_get_medium_rtm_base_hdr(pkt_entry->pkt)->msg_length;
	case RXR_DC_MEDIUM_MSGRTM_PKT:
	case RXR_DC_MEDIUM_TAGRTM_PKT:
		return rxr_get_dc_medium_rtm_base_hdr(pkt_entry->pkt)->msg_length;
	case RXR_LONGCTS_MSGRTM_PKT:
	case RXR_LONGCTS_TAGRTM_PKT:
	case RXR_DC_LONGCTS_MSGRTM_PKT:
	case RXR_DC_LONGCTS_TAGRTM_PKT:
		return rxr_get_longcts_rtm_base_hdr(pkt_entry->pkt)->msg_length;
	case RXR_LONGREAD_MSGRTM_PKT:
	case RXR_LONGREAD_TAGRTM_PKT:
		return rxr_get_longread_rtm_base_hdr(pkt_entry->pkt)->msg_length;
	case RXR_RUNTREAD_MSGRTM_PKT:
	case RXR_RUNTREAD_TAGRTM_PKT:
		return rxr_get_runtread_rtm_base_hdr(pkt_entry->pkt)->msg_length;
	default:
		assert(0 && "Unknown REQ packet type\n");
	}

	return 0;
}

/*
 * @brief Update rx_entry with the following information in RTM packet entry.
 *            address:       this is necessary because original address in
 *                           rx_entry can be FI_ADDR_UNSPEC
 *            cq_entry.data: for FI_REMOTE_CQ_DATA
 *            msg_id:        message id
 *            total_len:     application might provide a buffer that is larger
 *                           then incoming message size.
 *            tag:           sender's tag can be different from receiver's tag
 *                           becuase match only requires
 *                           (sender_tag | ignore) == (receiver_tag | ignore)
 *        This function is applied to both unexpected rx_entry (when they are
 *        allocated) and expected rx_entry (when they are matched to a RTM)
 *
 * @param pkt_entry(input)  RTM packet entry
 * @param rx_entry(input)   rx entry to be updated
 */
void rxr_pkt_rtm_update_rx_entry(struct rxr_pkt_entry *pkt_entry,
				 struct rxr_op_entry *rx_entry)
{
	struct rxr_base_hdr *base_hdr;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		rx_entry->rxr_flags |= RXR_REMOTE_CQ_DATA;
		rx_entry->cq_entry.flags |= FI_REMOTE_CQ_DATA;
		rx_entry->cq_entry.data = rxr_pkt_req_cq_data(pkt_entry);
	}

	rx_entry->addr = pkt_entry->addr;
	rx_entry->msg_id = rxr_pkt_msg_id(pkt_entry);
	rx_entry->total_len = rxr_pkt_rtm_total_len(pkt_entry);
	rx_entry->tag = rxr_pkt_rtm_tag(pkt_entry);
	rx_entry->cq_entry.tag = rx_entry->tag;
}

struct rxr_op_entry *rxr_pkt_get_rtm_matched_rx_entry(struct rxr_ep *ep,
						      struct dlist_entry *match,
						      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;

	assert(match);
	rx_entry = container_of(match, struct rxr_op_entry, entry);
	if (rx_entry->rxr_flags & RXR_MULTI_RECV_POSTED) {
		rx_entry = rxr_msg_split_rx_entry(ep, rx_entry, NULL, pkt_entry);
		if (OFI_UNLIKELY(!rx_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"RX entries exhausted.\n");
			efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
			return NULL;
		}
	} else {
		rxr_pkt_rtm_update_rx_entry(pkt_entry, rx_entry);
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
	struct rxr_op_entry *rx_entry;

	rx_entry = container_of(item, struct rxr_op_entry, entry);
	return ofi_match_addr(rx_entry->addr, pkt_entry->addr);
}

static
int rxr_pkt_rtm_match_trecv_anyaddr(struct dlist_entry *item, const void *arg)
{
	struct rxr_pkt_entry *pkt_entry = (struct rxr_pkt_entry *)arg;
	struct rxr_op_entry *rx_entry;
	uint64_t match_tag;

	rx_entry = container_of(item, struct rxr_op_entry, entry);
	match_tag = rxr_pkt_rtm_tag(pkt_entry);

	return ofi_match_tag(rx_entry->cq_entry.tag, rx_entry->ignore,
			     match_tag);
}

static
int rxr_pkt_rtm_match_trecv(struct dlist_entry *item, const void *arg)
{
	struct rxr_pkt_entry *pkt_entry = (struct rxr_pkt_entry *)arg;
	struct rxr_op_entry *rx_entry;
	uint64_t match_tag;

	rx_entry = container_of(item, struct rxr_op_entry, entry);
	match_tag = rxr_pkt_rtm_tag(pkt_entry);

	return ofi_match_addr(rx_entry->addr, pkt_entry->addr) &&
	       ofi_match_tag(rx_entry->cq_entry.tag, rx_entry->ignore, match_tag);
}

static
struct rxr_op_entry *rxr_pkt_get_msgrtm_rx_entry(struct rxr_ep *ep,
						 struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rxr_op_entry *rx_entry;
	struct dlist_entry *match;
	dlist_func_t *match_func;
	int pkt_type;

	if ((*pkt_entry_ptr)->alloc_type == RXR_PKT_FROM_USER_BUFFER) {
		/* If a pkt_entry is constructred from user supplied buffer,
		 * the endpoint must be in zero copy receive mode.
		 */
		assert(ep->use_zcpy_rx);
		/* In this mode, an rx_entry is always created together
		 * with this pkt_entry, and pkt_entry->x_entry is pointing
		 * to it. Thus we can skip the matching process, and return
		 * pkt_entry->x_entry right away.
		 */
		assert((*pkt_entry_ptr)->x_entry);
		return (*pkt_entry_ptr)->x_entry;
	}

	if (ep->util_ep.caps & FI_DIRECTED_RECV)
		match_func = &rxr_pkt_rtm_match_recv;
	else
		match_func = &rxr_pkt_rtm_match_recv_anyaddr;

	match = dlist_find_first_match(&ep->rx_list, match_func,
	                               *pkt_entry_ptr);
	if (OFI_UNLIKELY(!match)) {
		/*
		 * rxr_msg_alloc_unexp_rx_entry_for_msgrtm() might release pkt_entry,
		 * thus we have to use pkt_entry_ptr here
		 */
		rx_entry = rxr_msg_alloc_unexp_rx_entry_for_msgrtm(ep, pkt_entry_ptr);
		if (OFI_UNLIKELY(!rx_entry)) {
			FI_WARN(&rxr_prov, FI_LOG_CQ,
				"RX entries exhausted.\n");
			efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
			return NULL;
		}

	} else {
		rx_entry = rxr_pkt_get_rtm_matched_rx_entry(ep, match, *pkt_entry_ptr);
		rxr_tracing(msg_match_expected_nontagged, rx_entry->msg_id, 
			    (size_t) rx_entry->cq_entry.op_context, rx_entry->total_len);
	}

	pkt_type = rxr_get_base_hdr((*pkt_entry_ptr)->pkt)->type;
	if (rxr_pkt_type_is_mulreq(pkt_type))
		rxr_pkt_rx_map_insert(ep, *pkt_entry_ptr, rx_entry);

	return rx_entry;
}

static
struct rxr_op_entry *rxr_pkt_get_tagrtm_rx_entry(struct rxr_ep *ep,
						 struct rxr_pkt_entry **pkt_entry_ptr)
{
	struct rxr_op_entry *rx_entry;
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
		 * rxr_msg_alloc_unexp_rx_entry_for_tagrtm() might release pkt_entry,
		 * thus we have to use pkt_entry_ptr here
		 */
		rx_entry = rxr_msg_alloc_unexp_rx_entry_for_tagrtm(ep, pkt_entry_ptr);
		if (OFI_UNLIKELY(!rx_entry)) {
			efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
			return NULL;
		}
	} else {
		rx_entry = rxr_pkt_get_rtm_matched_rx_entry(ep, match, *pkt_entry_ptr);
		rxr_tracing(msg_match_expected_tagged, rx_entry->msg_id, 
			    (size_t) rx_entry->cq_entry.op_context, rx_entry->total_len);
	}

	pkt_type = rxr_get_base_hdr((*pkt_entry_ptr)->pkt)->type;
	if (rxr_pkt_type_is_mulreq(pkt_type))
		rxr_pkt_rx_map_insert(ep, *pkt_entry_ptr, rx_entry);

	return rx_entry;
}

ssize_t rxr_pkt_proc_matched_longread_rtm(struct rxr_ep *ep,
				      struct rxr_op_entry *rx_entry,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_longread_rtm_base_hdr *rtm_hdr;
	struct fi_rma_iov *read_iov;

	rtm_hdr = rxr_get_longread_rtm_base_hdr(pkt_entry->pkt);
	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry));

	rx_entry->tx_id = rtm_hdr->send_id;
	rx_entry->rma_iov_count = rtm_hdr->read_iov_count;
	memcpy(rx_entry->rma_iov, read_iov,
	       rx_entry->rma_iov_count * sizeof(struct fi_rma_iov));

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	rxr_tracing(longread_read_posted, rx_entry->msg_id, 
		    (size_t) rx_entry->cq_entry.op_context, rx_entry->total_len);
	return rxr_read_post_remote_read_or_queue(ep, rx_entry);
}

ssize_t rxr_pkt_proc_matched_mulreq_rtm(struct rxr_ep *ep,
					struct rxr_op_entry *rx_entry,
					struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_pkt_entry *cur, *nxt;
	char *pkt_data;
	int pkt_type;
	ssize_t ret, err;
	size_t rx_data_offset, pkt_data_offset, data_size;

	pkt_type = rxr_get_base_hdr(pkt_entry->pkt)->type;

	if (rxr_pkt_type_is_runtread(pkt_type)) {
		struct rxr_runtread_rtm_base_hdr *runtread_rtm_hdr;

		runtread_rtm_hdr = rxr_get_runtread_rtm_base_hdr(pkt_entry->pkt);
		rx_entry->bytes_runt = runtread_rtm_hdr->runt_length;
		if (rx_entry->total_len > rx_entry->bytes_runt && !rx_entry->read_entry) {
			struct fi_rma_iov *read_iov;

			rx_entry->tx_id = runtread_rtm_hdr->send_id;
			read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry));
			rx_entry->rma_iov_count = runtread_rtm_hdr->read_iov_count;
			memcpy(rx_entry->rma_iov, read_iov, rx_entry->rma_iov_count * sizeof(struct fi_rma_iov));
			rxr_tracing(runtread_read_posted, rx_entry->msg_id, 
				    (size_t) rx_entry->cq_entry.op_context, rx_entry->total_len);
			err = rxr_read_post_remote_read_or_queue(ep, rx_entry);
			if (err)
				return err;
		}
	}

	ret = 0;
	cur = pkt_entry;
	while (cur) {
		pkt_data_offset = rxr_pkt_req_data_offset(cur);
		pkt_data = (char *)cur->pkt + pkt_data_offset;
		data_size = cur->pkt_size - pkt_data_offset;

		rx_data_offset = rxr_pkt_hdr_seg_offset(cur->pkt);

		/* rxr_pkt_copy_data_to_op_entry() can release rx_entry, so
		 * bytes_received must be calculated before it.
		 */
		rx_entry->bytes_received += data_size;
		rx_entry->bytes_received_via_mulreq += data_size;
		if (rxr_op_entry_mulreq_total_data_size(rx_entry, pkt_type) == rx_entry->bytes_received_via_mulreq)
			rxr_pkt_rx_map_remove(ep, cur, rx_entry);

		/* rxr_pkt_copy_data_to_op_entry() will release cur, so
		 * cur->next must be copied out before it.
		 */
		nxt = cur->next;
		cur->next = NULL;

		err = rxr_pkt_copy_data_to_op_entry(ep, rx_entry, rx_data_offset, cur, pkt_data, data_size);
		if (err) {
			rxr_pkt_entry_release_rx(ep, cur);
			ret = err;
		}

		cur = nxt;
	}

	return ret;
}

/**
 * @brief process a matched eager rtm packet entry
 *
 * For an eager message, it will write rx completion,
 * release packet entry and rx_entry.
 *
 * @param[in]	ep		endpoint
 * @param[in]	rx_entry	RX entry
 * @param[in]	pkt_entry	packet entry
 * @return	On success, return 0
 * 		On failure, return libfabric error code
 */
ssize_t rxr_pkt_proc_matched_eager_rtm(struct rxr_ep *ep,
				       struct rxr_op_entry *rx_entry,
				       struct rxr_pkt_entry *pkt_entry)
{
	int err;
	char *data;
	size_t hdr_size, data_size;

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);

	if (pkt_entry->alloc_type != RXR_PKT_FROM_USER_BUFFER) {
		data = (char *)pkt_entry->pkt + hdr_size;
		data_size = pkt_entry->pkt_size - hdr_size;

		/*
		 * On success, rxr_pkt_copy_data_to_op_entry will write rx completion,
		 * release pkt_entry and rx_entry
		 */
		err = rxr_pkt_copy_data_to_op_entry(ep, rx_entry, 0, pkt_entry, data, data_size);
		if (err)
			rxr_pkt_entry_release_rx(ep, pkt_entry);

		return err;
	}

	/* In this case, data is already in user provided buffer, so no need
	 * to copy. However, we do need to make sure the packet header length
	 * is correct. Otherwise, user will get wrong data.
	 *
	 * The expected header size is
	 * 	ep->msg_prefix_size - sizeof(struct rxr_pkt_entry)
	 * because we used the first sizeof(struct rxr_pkt_entry) to construct
	 * a pkt_entry.
	 */
	if (hdr_size != ep->msg_prefix_size - sizeof(struct rxr_pkt_entry)) {
		/* if header size is wrong, the data in user buffer is not useful.
		 * setting rx_entry->cq_entry.len here will cause an error cq entry
		 * to be written to application.
		 */
		rx_entry->cq_entry.len = 0;
	} else {
		assert(rx_entry->cq_entry.buf == pkt_entry->pkt - sizeof(struct rxr_pkt_entry));
		rx_entry->cq_entry.len = pkt_entry->pkt_size + sizeof(struct rxr_pkt_entry);
	}

	rxr_cq_write_rx_completion(ep, rx_entry);
	rxr_release_rx_entry(ep, rx_entry);

	/* no need to release packet entry because it is
	 * constructed using user supplied buffer */
	return 0;
}

ssize_t rxr_pkt_proc_matched_rtm(struct rxr_ep *ep,
				 struct rxr_op_entry *rx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	int pkt_type;
	char *data;
	size_t hdr_size, data_size;
	ssize_t ret;

	assert(rx_entry->state == RXR_RX_MATCHED);

	if (!rx_entry->peer) {
		rx_entry->addr = pkt_entry->addr;
		rx_entry->peer = rxr_ep_get_peer(ep, rx_entry->addr);
		assert(rx_entry->peer);
		dlist_insert_tail(&rx_entry->peer_entry, &rx_entry->peer->rx_entry_list);
	}

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

	if (pkt_type > RXR_DC_REQ_PKT_BEGIN &&
	    pkt_type < RXR_DC_REQ_PKT_END)
		rx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;

	if (pkt_type == RXR_LONGCTS_MSGRTM_PKT ||
	    pkt_type == RXR_LONGCTS_TAGRTM_PKT)
		rx_entry->tx_id = rxr_get_longcts_rtm_base_hdr(pkt_entry->pkt)->send_id;
	else if (pkt_type == RXR_DC_EAGER_MSGRTM_PKT ||
		 pkt_type == RXR_DC_EAGER_TAGRTM_PKT)
		rx_entry->tx_id = rxr_get_dc_eager_rtm_base_hdr(pkt_entry->pkt)->send_id;
	else if (pkt_type == RXR_DC_MEDIUM_MSGRTM_PKT ||
		 pkt_type == RXR_DC_MEDIUM_TAGRTM_PKT)
		rx_entry->tx_id = rxr_get_dc_medium_rtm_base_hdr(pkt_entry->pkt)->send_id;
	else if (pkt_type == RXR_DC_LONGCTS_MSGRTM_PKT ||
		 pkt_type == RXR_DC_LONGCTS_TAGRTM_PKT)
		rx_entry->tx_id = rxr_get_longcts_rtm_base_hdr(pkt_entry->pkt)->send_id;

	rx_entry->msg_id = rxr_get_rtm_base_hdr(pkt_entry->pkt)->msg_id;

	if (pkt_type == RXR_LONGREAD_MSGRTM_PKT || pkt_type == RXR_LONGREAD_TAGRTM_PKT)
		return rxr_pkt_proc_matched_longread_rtm(ep, rx_entry, pkt_entry);

	if (rxr_pkt_type_is_mulreq(pkt_type))
		return rxr_pkt_proc_matched_mulreq_rtm(ep, rx_entry, pkt_entry);

	if (pkt_type == RXR_EAGER_MSGRTM_PKT ||
	    pkt_type == RXR_EAGER_TAGRTM_PKT ||
	    pkt_type == RXR_DC_EAGER_MSGRTM_PKT ||
	    pkt_type == RXR_DC_EAGER_TAGRTM_PKT) {
		return rxr_pkt_proc_matched_eager_rtm(ep, rx_entry, pkt_entry);
	}

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	data = (char *)pkt_entry->pkt + hdr_size;
	data_size = pkt_entry->pkt_size - hdr_size;

	rx_entry->bytes_received += data_size;
	ret = rxr_pkt_copy_data_to_op_entry(ep, rx_entry, 0, pkt_entry, data, data_size);
	if (ret) {
		return ret;
	}
#if ENABLE_DEBUG
	dlist_insert_tail(&rx_entry->pending_recv_entry, &ep->op_entry_recv_list);
	ep->pending_recv_counter++;
#endif
	rx_entry->state = RXR_RX_RECV;
	ret = rxr_pkt_post_or_queue(ep, rx_entry, RXR_CTS_PKT, 0);

	return ret;
}

ssize_t rxr_pkt_proc_msgrtm(struct rxr_ep *ep,
			    struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	struct rxr_op_entry *rx_entry;

	rx_entry = rxr_pkt_get_msgrtm_rx_entry(ep, &pkt_entry);
	if (OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	if (rx_entry->state == RXR_RX_MATCHED) {
		err = rxr_pkt_proc_matched_rtm(ep, rx_entry, pkt_entry);
		if (OFI_UNLIKELY(err)) {
			rxr_cq_write_rx_error(ep, rx_entry, -err, FI_EFA_ERR_PKT_PROC_MSGRTM);
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
	struct rxr_op_entry *rx_entry;

	rx_entry = rxr_pkt_get_tagrtm_rx_entry(ep, &pkt_entry);
	if (OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	if (rx_entry->state == RXR_RX_MATCHED) {
		err = rxr_pkt_proc_matched_rtm(ep, rx_entry, pkt_entry);
		if (OFI_UNLIKELY(err)) {
			rxr_cq_write_rx_error(ep, rx_entry, -err, FI_EFA_ERR_PKT_PROC_TAGRTM);
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
	case RXR_LONGCTS_MSGRTM_PKT:
	case RXR_LONGREAD_MSGRTM_PKT:
	case RXR_RUNTREAD_MSGRTM_PKT:
	case RXR_DC_EAGER_MSGRTM_PKT:
	case RXR_DC_MEDIUM_MSGRTM_PKT:
	case RXR_DC_LONGCTS_MSGRTM_PKT:
		return rxr_pkt_proc_msgrtm(ep, pkt_entry);
	case RXR_EAGER_TAGRTM_PKT:
	case RXR_MEDIUM_TAGRTM_PKT:
	case RXR_LONGCTS_TAGRTM_PKT:
	case RXR_LONGREAD_TAGRTM_PKT:
	case RXR_RUNTREAD_TAGRTM_PKT:
	case RXR_DC_EAGER_TAGRTM_PKT:
	case RXR_DC_MEDIUM_TAGRTM_PKT:
	case RXR_DC_LONGCTS_TAGRTM_PKT:
		return rxr_pkt_proc_tagrtm(ep, pkt_entry);
	case RXR_WRITE_RTA_PKT:
		return rxr_pkt_proc_write_rta(ep, pkt_entry);
	case RXR_DC_WRITE_RTA_PKT:
		return rxr_pkt_proc_dc_write_rta(ep, pkt_entry);
	case RXR_FETCH_RTA_PKT:
		return rxr_pkt_proc_fetch_rta(ep, pkt_entry);
	case RXR_COMPARE_RTA_PKT:
		return rxr_pkt_proc_compare_rta(ep, pkt_entry);
	default:
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown packet type ID: %d\n",
		       base_hdr->type);
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_UNKNOWN_PKT_TYPE);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	}

	return -FI_EINVAL;
}

void rxr_pkt_handle_rtm_rta_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_base_hdr *base_hdr;
	struct rdm_peer *peer;
	int ret, msg_id;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	assert(base_hdr->type >= RXR_BASELINE_REQ_PKT_BEGIN);

	if (rxr_pkt_type_is_mulreq(base_hdr->type)) {
		struct rxr_op_entry *rx_entry;
		struct rxr_pkt_entry *unexp_pkt_entry;

		rx_entry = rxr_pkt_rx_map_lookup(ep, pkt_entry);
		if (rx_entry) {
			if (rx_entry->state == RXR_RX_MATCHED) {
				rxr_pkt_proc_matched_mulreq_rtm(ep, rx_entry, pkt_entry);
			} else {
				assert(rx_entry->unexp_pkt);
				unexp_pkt_entry = rxr_pkt_get_unexp(ep, &pkt_entry);
				rxr_pkt_entry_append(rx_entry->unexp_pkt, unexp_pkt_entry);
			}

			return;
		}
	}

	peer = rxr_ep_get_peer(ep, pkt_entry->addr);
	assert(peer);
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
		       msg_id, peer->robuf.exp_msg_id);
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_PKT_ALREADY_PROCESSED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	if (OFI_UNLIKELY(ret == -FI_ENOMEM)) {
		/* running out of memory while copy packet */
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_OOM);
		return;
	}

	if (OFI_UNLIKELY(ret < 0)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"Unknown error %d processing REQ packet msg_id: %"
			PRIu32 "\n", ret, msg_id);
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_OTHER);
		return;
	}


	/*
	 * rxr_pkt_proc_rtm_rta() will write error cq entry if needed,
	 * thus we do not write error cq entry
	 */
	ret = rxr_pkt_proc_rtm_rta(ep, pkt_entry);
	if (OFI_UNLIKELY(ret))
		return;

	ofi_recvwin_slide((&peer->robuf));
	/* process pending items in reorder buff */
	rxr_cq_proc_pending_items_in_recvwin(ep, peer);
}

/*
 * RTW packet type functions
 */
int rxr_pkt_init_rtw_data(struct rxr_ep *ep,
			  struct rxr_op_entry *tx_entry,
			  struct rxr_pkt_entry *pkt_entry,
			  struct efa_rma_iov *rma_iov)
{
	size_t hdr_size;
	size_t data_size;
	int i;

	for (i = 0; i < tx_entry->rma_iov_count; ++i) {
		rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rma_iov[i].len = tx_entry->rma_iov[i].len;
		rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	data_size = MIN(ep->mtu_size - hdr_size, tx_entry->total_len);
	return rxr_pkt_init_data_from_op_entry(ep, pkt_entry, hdr_size, tx_entry, 0, data_size);
}

ssize_t rxr_pkt_init_eager_rtw(struct rxr_ep *ep,
			       struct rxr_op_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_eager_rtw_hdr *rtw_hdr;

	assert(tx_entry->op == ofi_op_write);

	rtw_hdr = (struct rxr_eager_rtw_hdr *)pkt_entry->pkt;
	rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, RXR_EAGER_RTW_PKT, pkt_entry);
	return rxr_pkt_init_rtw_data(ep, tx_entry, pkt_entry, rtw_hdr->rma_iov);
}

ssize_t rxr_pkt_init_dc_eager_rtw(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_dc_eager_rtw_hdr *dc_eager_rtw_hdr;
	int ret;

	assert(tx_entry->op == ofi_op_write);

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	dc_eager_rtw_hdr = (struct rxr_dc_eager_rtw_hdr *)pkt_entry->pkt;
	dc_eager_rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, RXR_DC_EAGER_RTW_PKT, pkt_entry);
	ret = rxr_pkt_init_rtw_data(ep, tx_entry, pkt_entry,
				    dc_eager_rtw_hdr->rma_iov);
	dc_eager_rtw_hdr->send_id = tx_entry->tx_id;
	return ret;
}

static inline void rxr_pkt_init_longcts_rtw_hdr(struct rxr_ep *ep,
					     struct rxr_op_entry *tx_entry,
					     struct rxr_pkt_entry *pkt_entry,
					     int pkt_type)
{
	struct rxr_longcts_rtw_hdr *rtw_hdr;

	tx_entry->rxr_flags |= RXR_LONGCTS_PROTOCOL;
	rtw_hdr = (struct rxr_longcts_rtw_hdr *)pkt_entry->pkt;
	rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rtw_hdr->msg_length = tx_entry->total_len;
	rtw_hdr->send_id = tx_entry->tx_id;
	rtw_hdr->credit_request = rxr_env.tx_min_credits;
	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);
}

ssize_t rxr_pkt_init_longcts_rtw(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_longcts_rtw_hdr *rtw_hdr;

	assert(tx_entry->op == ofi_op_write);

	rtw_hdr = (struct rxr_longcts_rtw_hdr *)pkt_entry->pkt;
	rxr_pkt_init_longcts_rtw_hdr(ep, tx_entry, pkt_entry, RXR_LONGCTS_RTW_PKT);
	return rxr_pkt_init_rtw_data(ep, tx_entry, pkt_entry, rtw_hdr->rma_iov);
}

ssize_t rxr_pkt_init_dc_longcts_rtw(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_longcts_rtw_hdr *rtw_hdr;

	assert(tx_entry->op == ofi_op_write);

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	rtw_hdr = (struct rxr_longcts_rtw_hdr *)pkt_entry->pkt;
	rxr_pkt_init_longcts_rtw_hdr(ep, tx_entry, pkt_entry, RXR_DC_LONGCTS_RTW_PKT);
	return rxr_pkt_init_rtw_data(ep, tx_entry, pkt_entry, rtw_hdr->rma_iov);
}

ssize_t rxr_pkt_init_longread_rtw(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_longread_rtw_hdr *rtw_hdr;
	struct efa_rma_iov *rma_iov;
	struct fi_rma_iov *read_iov;
	size_t hdr_size;
	int i, err;

	assert(tx_entry->op == ofi_op_write);

	rtw_hdr = (struct rxr_longread_rtw_hdr *)pkt_entry->pkt;
	rtw_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rtw_hdr->msg_length = tx_entry->total_len;
	rtw_hdr->send_id = tx_entry->tx_id;
	rtw_hdr->read_iov_count = tx_entry->iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, RXR_LONGREAD_RTW_PKT, pkt_entry);

	rma_iov = rtw_hdr->rma_iov;
	for (i = 0; i < tx_entry->rma_iov_count; ++i) {
		rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rma_iov[i].len = tx_entry->rma_iov[i].len;
		rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + hdr_size);
	err = rxr_read_init_iov(ep, tx_entry, read_iov);
	if (OFI_UNLIKELY(err))
		return err;

	pkt_entry->pkt_size = hdr_size + tx_entry->iov_count * sizeof(struct efa_rma_iov);
	pkt_entry->x_entry = tx_entry;
	return 0;
}

/*
 *     handle_sent() functions for RTW packet types
 *
 *         rxr_pkt_handle_longcts_rtw_sent() is empty and is defined in rxr_pkt_type_req.h
 */
void rxr_pkt_handle_longcts_rtw_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;
	struct efa_domain *efa_domain = rxr_ep_domain(ep);

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_sent += rxr_pkt_req_data_size(pkt_entry);
	assert(tx_entry->bytes_sent < tx_entry->total_len);
	if (tx_entry->desc[0] || efa_is_cache_available(efa_domain))
		rxr_prepare_desc_send(rxr_ep_domain(ep), tx_entry);
}

/*
 *     handle_send_completion() functions
 */
void rxr_pkt_handle_eager_rtw_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	assert(tx_entry->total_len == rxr_pkt_req_data_size(pkt_entry));
	rxr_cq_handle_send_completion(ep, tx_entry);
}

void rxr_pkt_handle_longcts_rtw_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked)
		rxr_cq_handle_send_completion(ep, tx_entry);
}

void rxr_pkt_handle_dc_longcts_rtw_send_completion(struct rxr_ep *ep,
						struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	tx_entry->bytes_acked += rxr_pkt_req_data_size(pkt_entry);
	if (tx_entry->total_len == tx_entry->bytes_acked &&
	    tx_entry->rxr_flags & RXR_RECEIPT_RECEIVED)
		rxr_cq_handle_send_completion(ep, tx_entry);
}

/*
 *     handle_recv() functions
 */

static
struct rxr_op_entry *rxr_pkt_alloc_rtw_rx_entry(struct rxr_ep *ep,
						struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_base_hdr *base_hdr;

	rx_entry = rxr_ep_alloc_rx_entry(ep, pkt_entry->addr, ofi_op_write);
	if (OFI_UNLIKELY(!rx_entry))
		return NULL;

	base_hdr = rxr_get_base_hdr(pkt_entry->pkt);
	if (base_hdr->flags & RXR_REQ_OPT_CQ_DATA_HDR) {
		rx_entry->rxr_flags |= RXR_REMOTE_CQ_DATA;
		rx_entry->cq_entry.flags |= FI_REMOTE_CQ_DATA;
		rx_entry->cq_entry.data = rxr_pkt_req_cq_data(pkt_entry);
	}

	rx_entry->addr = pkt_entry->addr;
	rx_entry->bytes_received = 0;
	rx_entry->bytes_copied = 0;
	return rx_entry;
}

void rxr_pkt_proc_eager_rtw(struct rxr_ep *ep,
			    struct efa_rma_iov *rma_iov,
			    size_t rma_iov_count,
			    struct rxr_op_entry *rx_entry,
			    struct rxr_pkt_entry *pkt_entry)
{
	ssize_t err;
	char *data;
	size_t data_size, hdr_size;

	err = rxr_rma_verified_copy_iov(ep, rma_iov, rma_iov_count,
					FI_REMOTE_WRITE, rx_entry->iov, rx_entry->desc);

	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verify failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_RMA_ADDR);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_WRITE);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	data = (char *)pkt_entry->pkt + hdr_size;
	data_size = pkt_entry->pkt_size - hdr_size;

	rx_entry->bytes_received += data_size;
	if (data_size != rx_entry->total_len) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Eager RTM size mismatch! data_size: %ld total_len: %ld.",
			data_size, rx_entry->total_len);
		FI_WARN(&rxr_prov, FI_LOG_CQ, "target buffer: %p length: %ld", rx_entry->iov[0].iov_base,
			rx_entry->iov[0].iov_len);
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_RTM_MISMATCH);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		rxr_release_rx_entry(ep, rx_entry);
	} else {
		err = rxr_pkt_copy_data_to_op_entry(ep, rx_entry, 0, pkt_entry, data, data_size);
		if (OFI_UNLIKELY(err)) {
			efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_RX_ENTRY_COPY);
			rxr_pkt_entry_release_rx(ep, pkt_entry);
			rxr_release_rx_entry(ep, rx_entry);
		}
	}
}

void rxr_pkt_handle_eager_rtw_recv(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_eager_rtw_hdr *rtw_hdr;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);

	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rtw_hdr = (struct rxr_eager_rtw_hdr *)pkt_entry->pkt;
	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	rxr_pkt_proc_eager_rtw(ep,
			       rtw_hdr->rma_iov,
			       rtw_hdr->rma_iov_count,
			       rx_entry, pkt_entry);
}

void rxr_pkt_handle_dc_eager_rtw_recv(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_dc_eager_rtw_hdr *rtw_hdr;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);
	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	rtw_hdr = (struct rxr_dc_eager_rtw_hdr *)pkt_entry->pkt;
	rx_entry->tx_id = rtw_hdr->send_id;
	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	rxr_pkt_proc_eager_rtw(ep,
			       rtw_hdr->rma_iov,
			       rtw_hdr->rma_iov_count,
			       rx_entry, pkt_entry);
}

void rxr_pkt_handle_longcts_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_longcts_rtw_hdr *rtw_hdr;
	char *data;
	size_t hdr_size, data_size;
	ssize_t err;
	uint32_t tx_id;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);
	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rtw_hdr = (struct rxr_longcts_rtw_hdr *)pkt_entry->pkt;
	tx_id = rtw_hdr->send_id;
	if (rtw_hdr->type == RXR_DC_LONGCTS_RTW_PKT)
		rx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;

	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtw_hdr->rma_iov, rtw_hdr->rma_iov_count,
					FI_REMOTE_WRITE, rx_entry->iov, rx_entry->desc);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verify failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_RMA_ADDR);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_WRITE);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	data = (char *)pkt_entry->pkt + hdr_size;
	data_size = pkt_entry->pkt_size - hdr_size;

	rx_entry->bytes_received += data_size;
	if (data_size >= rx_entry->total_len) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Long RTM size mismatch! pkt_data_size: %ld total_len: %ld\n",
			data_size, rx_entry->total_len);
		FI_WARN(&rxr_prov, FI_LOG_CQ, "target buffer: %p length: %ld", rx_entry->iov[0].iov_base,
			rx_entry->iov[0].iov_len);
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_RTM_MISMATCH);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	} else {
		err = rxr_pkt_copy_data_to_op_entry(ep, rx_entry, 0, pkt_entry, data, data_size);
		if (OFI_UNLIKELY(err)) {
			efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_RX_ENTRY_COPY);
			rxr_release_rx_entry(ep, rx_entry);
			rxr_pkt_entry_release_rx(ep, pkt_entry);
			return;
		}
	}


#if ENABLE_DEBUG
	dlist_insert_tail(&rx_entry->pending_recv_entry, &ep->op_entry_recv_list);
	ep->pending_recv_counter++;
#endif
	rx_entry->state = RXR_RX_RECV;
	rx_entry->tx_id = tx_id;
	err = rxr_pkt_post_or_queue(ep, rx_entry, RXR_CTS_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Cannot post CTS packet\n");
		rxr_cq_write_rx_error(ep, rx_entry, -err, FI_EFA_ERR_PKT_POST);
		rxr_release_rx_entry(ep, rx_entry);
	}
}

void rxr_pkt_handle_longread_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_longread_rtw_hdr *rtw_hdr;
	struct fi_rma_iov *read_iov;
	size_t hdr_size;
	ssize_t err;

	rx_entry = rxr_pkt_alloc_rtw_rx_entry(ep, pkt_entry);
	if (!rx_entry) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rtw_hdr = (struct rxr_longread_rtw_hdr *)pkt_entry->pkt;
	rx_entry->iov_count = rtw_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtw_hdr->rma_iov, rtw_hdr->rma_iov_count,
					FI_REMOTE_WRITE, rx_entry->iov, rx_entry->desc);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verify failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_RMA_ADDR);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_WRITE);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	read_iov = (struct fi_rma_iov *)((char *)pkt_entry->pkt + hdr_size);
	rx_entry->addr = pkt_entry->addr;
	rx_entry->tx_id = rtw_hdr->send_id;
	rx_entry->rma_iov_count = rtw_hdr->read_iov_count;
	memcpy(rx_entry->rma_iov, read_iov,
	       rx_entry->rma_iov_count * sizeof(struct fi_rma_iov));

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	err = rxr_read_post_remote_read_or_queue(ep, rx_entry);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RDMA post read or queue failed.\n");
		efa_eq_write_error(&ep->util_ep, err, FI_EFA_ERR_RDMA_READ_POST);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
	}
}

/*
 * RTR packet functions
 *     init() functions for RTR packets
 */
void rxr_pkt_init_rtr(struct rxr_ep *ep,
		      struct rxr_op_entry *tx_entry,
		      int pkt_type, int window,
		      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rtr_hdr *rtr_hdr;
	int i;

	assert(tx_entry->op == ofi_op_read_req);
	rtr_hdr = (struct rxr_rtr_hdr *)pkt_entry->pkt;
	rtr_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);
	rtr_hdr->msg_length = tx_entry->total_len;
	rtr_hdr->recv_id = tx_entry->tx_id;
	rtr_hdr->recv_length = window;
	for (i = 0; i < tx_entry->rma_iov_count; ++i) {
		rtr_hdr->rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rtr_hdr->rma_iov[i].len = tx_entry->rma_iov[i].len;
		rtr_hdr->rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	pkt_entry->pkt_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	pkt_entry->x_entry = tx_entry;
}

ssize_t rxr_pkt_init_short_rtr(struct rxr_ep *ep,
			       struct rxr_op_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rtr(ep, tx_entry, RXR_SHORT_RTR_PKT, tx_entry->total_len, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_longcts_rtr(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rtr(ep, tx_entry, RXR_LONGCTS_RTR_PKT, tx_entry->window, pkt_entry);
	return 0;
}


/*
 *     handle_recv() functions for RTR packet
 */
void rxr_pkt_handle_rtr_recv(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rtr_hdr *rtr_hdr;
	struct rxr_op_entry *rx_entry;
	ssize_t err;

	rx_entry = rxr_ep_alloc_rx_entry(ep, pkt_entry->addr, ofi_op_read_rsp);
	if (OFI_UNLIKELY(!rx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->addr = pkt_entry->addr;
	rx_entry->bytes_received = 0;
	rx_entry->bytes_copied = 0;

	rtr_hdr = (struct rxr_rtr_hdr *)pkt_entry->pkt;
	rx_entry->tx_id = rtr_hdr->recv_id;
	rx_entry->window = rtr_hdr->recv_length;
	rx_entry->iov_count = rtr_hdr->rma_iov_count;
	err = rxr_rma_verified_copy_iov(ep, rtr_hdr->rma_iov, rtr_hdr->rma_iov_count,
					FI_REMOTE_READ, rx_entry->iov, rx_entry->desc);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "RMA address verification failed!\n");
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_RMA_ADDR);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rx_entry->cq_entry.flags |= (FI_RMA | FI_READ);
	rx_entry->cq_entry.len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	rx_entry->cq_entry.buf = rx_entry->iov[0].iov_base;
	rx_entry->total_len = rx_entry->cq_entry.len;

	err = rxr_pkt_post_or_queue(ep, rx_entry, RXR_READRSP_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Posting of readrsp packet failed! err=%ld\n", err);
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_PKT_POST);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return;
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
}

/*
 * RTA packet functions
 */
ssize_t rxr_pkt_init_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
			 int pkt_type, struct rxr_pkt_entry *pkt_entry)
{
	struct efa_rma_iov *rma_iov;
	struct rxr_rta_hdr *rta_hdr;
	char *data;
	size_t hdr_size, data_size;
	int i;

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	rta_hdr->msg_id = tx_entry->msg_id;
	rta_hdr->rma_iov_count = tx_entry->rma_iov_count;
	rta_hdr->atomic_datatype = tx_entry->atomic_hdr.datatype;
	rta_hdr->atomic_op = tx_entry->atomic_hdr.atomic_op;
	rxr_pkt_init_req_hdr(ep, tx_entry, pkt_type, pkt_entry);
	rta_hdr->flags |= RXR_REQ_ATOMIC;
	rma_iov = rta_hdr->rma_iov;
	for (i=0; i < tx_entry->rma_iov_count; ++i) {
		rma_iov[i].addr = tx_entry->rma_iov[i].addr;
		rma_iov[i].len = tx_entry->rma_iov[i].len;
		rma_iov[i].key = tx_entry->rma_iov[i].key;
	}

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	data = (char *)pkt_entry->pkt + hdr_size;
	data_size = ofi_copy_from_iov(data, ep->mtu_size - hdr_size,
				      tx_entry->iov, tx_entry->iov_count, 0);

	pkt_entry->pkt_size = hdr_size + data_size;
	pkt_entry->x_entry = tx_entry;
	return 0;
}

ssize_t rxr_pkt_init_write_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
			    struct rxr_pkt_entry *pkt_entry)
{
	rxr_pkt_init_rta(ep, tx_entry, RXR_WRITE_RTA_PKT, pkt_entry);
	return 0;
}

ssize_t rxr_pkt_init_dc_write_rta(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rta_hdr *rta_hdr;

	tx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;
	rxr_pkt_init_rta(ep, tx_entry, RXR_DC_WRITE_RTA_PKT, pkt_entry);
	rta_hdr = rxr_get_rta_hdr(pkt_entry->pkt);
	rta_hdr->send_id = tx_entry->tx_id;
	return 0;
}

ssize_t rxr_pkt_init_fetch_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rta_hdr *rta_hdr;

	rxr_pkt_init_rta(ep, tx_entry, RXR_FETCH_RTA_PKT, pkt_entry);
	rta_hdr = rxr_get_rta_hdr(pkt_entry->pkt);
	rta_hdr->recv_id = tx_entry->tx_id;
	return 0;
}

ssize_t rxr_pkt_init_compare_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry)
{
	char *data;
	size_t data_size;
	struct rxr_rta_hdr *rta_hdr;

	rxr_pkt_init_rta(ep, tx_entry, RXR_COMPARE_RTA_PKT, pkt_entry);
	rta_hdr = rxr_get_rta_hdr(pkt_entry->pkt);
	rta_hdr->recv_id = tx_entry->tx_id;
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
	struct rxr_op_entry *tx_entry;

	tx_entry = (struct rxr_op_entry *)pkt_entry->x_entry;
	rxr_cq_handle_send_completion(ep, tx_entry);
}

int rxr_pkt_proc_write_rta(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct iovec iov[RXR_IOV_LIMIT];
	struct rxr_rta_hdr *rta_hdr;
	void *desc[RXR_IOV_LIMIT];
	char *data;
	int iov_count, op, dt, i;
	size_t dtsize, offset, hdr_size;

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	op = rta_hdr->atomic_op;
	dt = rta_hdr->atomic_datatype;
	dtsize = ofi_datatype_size(dt);
	if (OFI_UNLIKELY(!dtsize)) {
		return -errno;
	}

	hdr_size = rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	data = (char *)pkt_entry->pkt + hdr_size;
	iov_count = rta_hdr->rma_iov_count;
	rxr_rma_verified_copy_iov(ep, rta_hdr->rma_iov, iov_count, FI_REMOTE_WRITE, iov, desc);

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

struct rxr_op_entry *rxr_pkt_alloc_rta_rx_entry(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry, int op)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_rta_hdr *rta_hdr;

	rx_entry = rxr_ep_alloc_rx_entry(ep, pkt_entry->addr, op);
	if (OFI_UNLIKELY(!rx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"RX entries exhausted.\n");
		return NULL;
	}

	if (op == ofi_op_atomic) {
		rx_entry->addr = pkt_entry->addr;
		return rx_entry;
	}

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	rx_entry->atomic_hdr.atomic_op = rta_hdr->atomic_op;
	rx_entry->atomic_hdr.datatype = rta_hdr->atomic_datatype;

	rx_entry->iov_count = rta_hdr->rma_iov_count;
	rxr_rma_verified_copy_iov(ep, rta_hdr->rma_iov, rx_entry->iov_count,
				  FI_REMOTE_READ, rx_entry->iov, rx_entry->desc);
	rx_entry->total_len = ofi_total_iov_len(rx_entry->iov, rx_entry->iov_count);
	/*
	 * prepare a buffer to hold response data.
	 * Atomic_op operates on 3 data buffers:
	 *          local_data (input/output),
	 *          request_data (input),
	 *          response_data (output)
	 * The fact local data will be changed by atomic_op means
	 * response_data is not reproducible.
	 * Because sending response packet can fail due to
	 * -FI_EAGAIN, we need a buffer to hold response_data.
	 * The buffer will be release in rxr_handle_atomrsp_send_completion()
	 */
	rx_entry->atomrsp_data = ofi_buf_alloc(ep->rx_atomrsp_pool);
	if (!rx_entry->atomrsp_data) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"atomic repsonse buffer pool exhausted.\n");
		rxr_release_rx_entry(ep, rx_entry);
		return NULL;
	}

	return rx_entry;
}

int rxr_pkt_proc_dc_write_rta(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	struct rxr_rta_hdr *rta_hdr;
	ssize_t err;
	int ret;

	rx_entry = rxr_pkt_alloc_rta_rx_entry(ep, pkt_entry, ofi_op_atomic);
	if (OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	rta_hdr = (struct rxr_rta_hdr *)pkt_entry->pkt;
	rx_entry->tx_id = rta_hdr->send_id;
	rx_entry->rxr_flags |= RXR_DELIVERY_COMPLETE_REQUESTED;

	ret = rxr_pkt_proc_write_rta(ep, pkt_entry);
	if (OFI_UNLIKELY(ret)) {
		FI_WARN(&rxr_prov,
			FI_LOG_CQ,
			"Error while processing the write rta packet\n");
		return ret;
	}

	err = rxr_pkt_post_or_queue(ep, rx_entry, RXR_RECEIPT_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ,
			"Posting of receipt packet failed! err=%s\n",
			fi_strerror(err));
		rxr_cq_write_rx_error(ep, rx_entry, -err, FI_EFA_ERR_PKT_POST);
		return err;
	}

	return ret;
}

int rxr_pkt_proc_fetch_rta(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	char *data;
	int op, dt, i;
	size_t offset, dtsize;
	ssize_t err;

	rx_entry = rxr_pkt_alloc_rta_rx_entry(ep, pkt_entry, ofi_op_atomic_fetch);
	if(OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		return -FI_ENOBUFS;
	}

	rx_entry->tx_id = rxr_get_rta_hdr(pkt_entry->pkt)->recv_id;
	op = rx_entry->atomic_hdr.atomic_op;
	dt = rx_entry->atomic_hdr.datatype;
	dtsize = ofi_datatype_size(rx_entry->atomic_hdr.datatype);
	if (OFI_UNLIKELY(!dtsize)) {
		return -errno;
	}

	data = (char *)pkt_entry->pkt + rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);

	offset = 0;
	for (i = 0; i < rx_entry->iov_count; ++i) {
		ofi_atomic_readwrite_handlers[op][dt](rx_entry->iov[i].iov_base,
						      data + offset,
						      rx_entry->atomrsp_data + offset,
						      rx_entry->iov[i].iov_len / dtsize);
		offset += rx_entry->iov[i].iov_len;
	}

	err = rxr_pkt_post_or_queue(ep, rx_entry, RXR_ATOMRSP_PKT, 0);
	if (OFI_UNLIKELY(err))
		rxr_cq_write_rx_error(ep, rx_entry, -err, FI_EFA_ERR_PKT_POST);

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return 0;
}

int rxr_pkt_proc_compare_rta(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_op_entry *rx_entry;
	char *src_data, *cmp_data;
	int op, dt, i;
	size_t offset, dtsize;
	ssize_t err;

	rx_entry = rxr_pkt_alloc_rta_rx_entry(ep, pkt_entry, ofi_op_atomic_compare);
	if(OFI_UNLIKELY(!rx_entry)) {
		efa_eq_write_error(&ep->util_ep, FI_ENOBUFS, FI_EFA_ERR_RX_ENTRIES_EXHAUSTED);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -FI_ENOBUFS;
	}

	rx_entry->tx_id = rxr_get_rta_hdr(pkt_entry->pkt)->recv_id;
	op = rx_entry->atomic_hdr.atomic_op;
	dt = rx_entry->atomic_hdr.datatype;
	dtsize = ofi_datatype_size(rx_entry->atomic_hdr.datatype);
	if (OFI_UNLIKELY(!dtsize)) {
		efa_eq_write_error(&ep->util_ep, FI_EINVAL, FI_EFA_ERR_INVALID_DATATYPE);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return -errno;
	}

	src_data = (char *)pkt_entry->pkt + rxr_pkt_req_hdr_size_from_pkt_entry(pkt_entry);
	cmp_data = src_data + rx_entry->total_len;
	offset = 0;

#ifdef HAVE___INT128
	/*
	 * Perform a check here on the datatype and then a copy if this is a
	 * 128-bit integer (otherwise, take the normal code path). We have to
	 * do this because of the way our buffers are laid out in memory.
	 * Unfortunately they are not aligned at 16 bytes, which is required
	 * when using optimized instructions.
	 */
	if (dt == FI_INT128) {
		for (i = 0; i < rx_entry->iov_count; ++i) {
			ofi_int128_t src, cmp;
			memcpy(&src, src_data + offset, sizeof(ofi_int128_t));
			memcpy(&cmp, cmp_data + offset, sizeof(ofi_int128_t));

			ofi_atomic_swap_handler(op, dt, rx_entry->iov[i].iov_base,
									&src,
									&cmp,
									rx_entry->atomrsp_data + offset,
									rx_entry->iov[i].iov_len / dtsize);
			offset += rx_entry->iov[i].iov_len;
		}
	} else if (dt == FI_UINT128) {
		for (i = 0; i < rx_entry->iov_count; ++i) {
			ofi_uint128_t src, cmp;
			memcpy(&src, src_data + offset, sizeof(ofi_uint128_t));
			memcpy(&cmp, cmp_data + offset, sizeof(ofi_uint128_t));
			ofi_atomic_swap_handler(op, dt, rx_entry->iov[i].iov_base,
									&src,
									&cmp,
									rx_entry->atomrsp_data + offset,
									rx_entry->iov[i].iov_len / dtsize);
			offset += rx_entry->iov[i].iov_len;
		}
	} else {
#endif
		for (i = 0; i < rx_entry->iov_count; ++i) {
			ofi_atomic_swap_handler(op, dt, rx_entry->iov[i].iov_base,
									src_data + offset,
									cmp_data + offset,
									rx_entry->atomrsp_data + offset,
									rx_entry->iov[i].iov_len / dtsize);
			offset += rx_entry->iov[i].iov_len;
		}
#ifdef HAVE___INT128
	}
#endif

	err = rxr_pkt_post_or_queue(ep, rx_entry, RXR_ATOMRSP_PKT, 0);
	if (OFI_UNLIKELY(err)) {
		efa_eq_write_error(&ep->util_ep, FI_EIO, FI_EFA_ERR_PKT_POST);
		ofi_buf_free(rx_entry->atomrsp_data);
		rxr_release_rx_entry(ep, rx_entry);
		rxr_pkt_entry_release_rx(ep, pkt_entry);
		return err;
	}

	rxr_pkt_entry_release_rx(ep, pkt_entry);
	return 0;
}
