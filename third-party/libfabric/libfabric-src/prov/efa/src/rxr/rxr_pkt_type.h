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

#ifndef _RXR_PKT_TYPE_H
#define _RXR_PKT_TYPE_H

#include "rxr_op_entry.h"
#include "rdm_proto_v4.h"

static inline struct rxr_base_hdr *rxr_get_base_hdr(void *pkt)
{
	return (struct rxr_base_hdr *)pkt;
}

struct rxr_ep;
struct rdm_peer;
struct rxr_read_entry;

/* HANDSHAKE packet related functions */
static inline
struct rxr_handshake_hdr *rxr_get_handshake_hdr(void *pkt)
{
	return (struct rxr_handshake_hdr *)pkt;
}

static inline
struct rxr_handshake_opt_connid_hdr *rxr_get_handshake_opt_connid_hdr(void *pkt)
{
	struct rxr_handshake_hdr *handshake_hdr;
	size_t base_hdr_size;

	handshake_hdr = (struct rxr_handshake_hdr *)pkt;
	assert(handshake_hdr->type == RXR_HANDSHAKE_PKT);
	assert(handshake_hdr->flags & RXR_PKT_CONNID_HDR);
	base_hdr_size = sizeof(struct rxr_handshake_hdr) +
			(handshake_hdr->nextra_p3 - 3) * sizeof(uint64_t);
	return (struct rxr_handshake_opt_connid_hdr *)((char *)pkt + base_hdr_size);
}

ssize_t rxr_pkt_init_handshake(struct rxr_ep *ep,
			       struct rxr_pkt_entry *pkt_entry,
			       fi_addr_t addr);

ssize_t rxr_pkt_post_handshake(struct rxr_ep *ep, struct rdm_peer *peer);

void rxr_pkt_post_handshake_or_queue(struct rxr_ep *ep,
				     struct rdm_peer *peer);

void rxr_pkt_handle_handshake_recv(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry);

/* CTS packet related functions */
static inline
struct rxr_cts_hdr *rxr_get_cts_hdr(void *pkt)
{
	return (struct rxr_cts_hdr *)pkt;
}

void rxr_pkt_calc_cts_window_credits(struct rxr_ep *ep, struct rdm_peer *peer,
				     uint64_t size, int request,
				     int *window, int *credits);

ssize_t rxr_pkt_init_cts(struct rxr_ep *ep,
			 struct rxr_op_entry *op_entry,
			 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_cts_sent(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_cts_recv(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

static inline
struct rxr_data_hdr *rxr_get_data_hdr(void *pkt)
{
	return (struct rxr_data_hdr *)pkt;
}

int rxr_pkt_init_data(struct rxr_ep *ep,
		      struct rxr_op_entry *op_entry,
		      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_data_sent(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_proc_data(struct rxr_ep *ep,
		       struct rxr_op_entry *op_entry,
		       struct rxr_pkt_entry *pkt_entry,
		       char *data, size_t seg_offset,
		       size_t seg_size);

void rxr_pkt_handle_data_send_completion(struct rxr_ep *ep,
					 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_data_recv(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry);

/* READRSP packet related functions */
static inline struct rxr_readrsp_hdr *rxr_get_readrsp_hdr(void *pkt)
{
	return (struct rxr_readrsp_hdr *)pkt;
}

int rxr_pkt_init_readrsp(struct rxr_ep *ep,
			 struct rxr_op_entry *tx_entry,
			 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_readrsp_sent(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_readrsp_send_completion(struct rxr_ep *ep,
					    struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_readrsp_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry);

/*
 *  RMA context packet, used to differentiate the normal RMA read, normal RMA
 *  write, and the RMA read in two-sided large message transfer
 *  Implementation of the function is in rxr_pkt_type_misc.c
 */
struct rxr_rma_context_pkt {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t context_type;
	uint32_t tx_id; /* used by write context */
	uint32_t read_id; /* used by read context */
	size_t seg_size; /* used by read context */
};

enum rxr_rma_context_pkt_type {
	RXR_READ_CONTEXT = 1,
	RXR_WRITE_CONTEXT,
};

void rxr_pkt_init_write_context(struct rxr_op_entry *tx_entry,
				struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_init_read_context(struct rxr_ep *rxr_ep,
			       struct rxr_read_entry *read_entry,
			       size_t seg_size,
			       struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_rma_completion(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry);

/* EOR packet related functions */
static inline
struct rxr_eor_hdr *rxr_get_eor_hdr(void *pkt)
{
	return (struct rxr_eor_hdr *)pkt;
}

int rxr_pkt_init_eor(struct rxr_ep *ep,
		     struct rxr_op_entry *rx_entry,
		     struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_eor_sent(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_eor_send_completion(struct rxr_ep *ep,
					struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_eor_recv(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

/* ATOMRSP packet related functions */
static inline struct rxr_atomrsp_hdr *rxr_get_atomrsp_hdr(void *pkt)
{
	return (struct rxr_atomrsp_hdr *)pkt;
}

int rxr_pkt_init_atomrsp(struct rxr_ep *ep, struct rxr_op_entry *rx_entry,
			 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_atomrsp_sent(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_atomrsp_send_completion(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_atomrsp_recv(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

/* RECEIPT packet related functions */
static inline
struct rxr_receipt_hdr *rxr_get_receipt_hdr(void *pkt)
{
	return (struct rxr_receipt_hdr *)pkt;
}

int rxr_pkt_init_receipt(struct rxr_ep *ep, struct rxr_op_entry *rx_entry,
			 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_receipt_sent(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_receipt_send_completion(struct rxr_ep *ep,
					    struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_receipt_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry);

/* General packet type helper functions */
static inline
int rxr_pkt_type_contains_rma_iov(int pkt_type)
{
	switch (pkt_type) {
		case RXR_EAGER_RTW_PKT:
		case RXR_DC_EAGER_RTW_PKT:
		case RXR_LONGCTS_RTW_PKT:
		case RXR_DC_LONGCTS_RTW_PKT:
		case RXR_LONGREAD_RTW_PKT:
		case RXR_SHORT_RTR_PKT:
		case RXR_LONGCTS_RTR_PKT:
		case RXR_WRITE_RTA_PKT:
		case RXR_DC_WRITE_RTA_PKT:
		case RXR_FETCH_RTA_PKT:
		case RXR_COMPARE_RTA_PKT:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * @brief determine whether a req pkt type is part of a runt protocol
 *
 * A runt protocol send user data into two parts. The first part
 * was sent by multiple eagerly sent packages. The rest of the
 * data is sent regularly.
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool rxr_pkt_type_is_runt(int pkt_type)
{
	return (pkt_type >= RXR_RUNT_PKT_BEGIN && pkt_type < RXR_RUNT_PKT_END);
}

/**
 * @brief determine whether a req pkt type is part of a medium protocol
 *
 * medium protocol send user data eagerly without CTS based flow control.
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool rxr_pkt_type_is_medium(int pkt_type)
{
	return pkt_type == RXR_MEDIUM_TAGRTM_PKT || pkt_type == RXR_MEDIUM_MSGRTM_PKT ||
	       pkt_type == RXR_DC_MEDIUM_MSGRTM_PKT ||pkt_type == RXR_DC_MEDIUM_TAGRTM_PKT;
}

/**
 * @brief determine whether a req pkt type is part of a multi-req protocol
 *
 * A multi-req protocol sends multiple (>=2) data containing REQ packets.
 * This function determine whether a req pkt type is part of a multi-req
 * protocol
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool rxr_pkt_type_is_mulreq(int pkt_type)
{
	return rxr_pkt_type_is_medium(pkt_type) || rxr_pkt_type_is_runt(pkt_type);
}

/**
 * @brief determine whether a pkt type is runtread rtm
 *
 * @param[in]		pkt_type		REQ packet type
 * @return		a boolean
 */
static inline
bool rxr_pkt_type_is_runtread(int pkt_type)
{
	return pkt_type == RXR_RUNTREAD_TAGRTM_PKT || pkt_type == RXR_RUNTREAD_MSGRTM_PKT;
}

int rxr_pkt_type_readbase_rtm(struct rdm_peer *peer, int op, uint64_t fi_flags, struct efa_hmem_info *hmem_info);

#endif

#include "rxr_pkt_type_req.h"
