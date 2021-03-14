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

#ifndef _RXR_PKT_TYPE_REQ_H
#define _RXR_PKT_TYPE_REQ_H

/*
 * This file contain REQ packet type related struct and functions
 * REQ packets can be classifed into 4 categories:
 *    RTM (Request To Message) is used by message
 *    RTW (Request To Write) is used by RMA write
 *    RTR (Request To Read) is used by RMA read
 *    RTA (Request To Atomic) is used by Atomic
 *
 * For each REQ packet type need to have the following:
 *
 *     1. a header struct
 *     2. an init() function called by rxr_pkt_init_ctrl()
 *     3. a handle_sent() function called by rxr_pkt_post_ctrl()
 *     4. a handle_send_completion() function called by
 *               rxr_pkt_handle_send_completion()
 *     5. a proc() function called by
 *               rxr_pkt_proc_req()
 *
 * Some req packet types are so similar that they can share
 * some functions.
 */

/*
 * Utilities shared by all REQ packets
 *
 *     Packet Header Flags
 */
#define RXR_REQ_OPT_RAW_ADDR_HDR	BIT_ULL(0)
#define RXR_REQ_OPT_CQ_DATA_HDR		BIT_ULL(1)
#define RXR_REQ_MSG			BIT_ULL(2)
#define RXR_REQ_TAGGED			BIT_ULL(3)
#define RXR_REQ_RMA			BIT_ULL(4)
#define RXR_REQ_ATOMIC			BIT_ULL(5)

/*
 *     Extra Feature Flags
 */
#define RXR_REQ_FEATURE_RDMA_READ	BIT_ULL(0)

/*
 *     Utility struct and functions for
 *             REQ packet types
 */
struct rxr_req_opt_raw_addr_hdr {
	uint32_t addr_len;
	char raw_addr[0];
};

struct rxr_req_opt_cq_data_hdr {
	int64_t cq_data;
};

void rxr_pkt_proc_req_common_hdr(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_base_hdr_size(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_max_data_size(struct rxr_ep *ep, fi_addr_t addr, int pkt_type);

/*
 * Structs and funcitons for RTM (Message) packet types
 * There are 4 message protocols
 *         Eager message protocol,
 *         Medium message protocol,
 *         Long message protocol,
 *         Read message protocol (message by read)
 * Each protocol employes two packet types: non-tagged and tagged.
 * Thus altogether there are 8 RTM packet types.
 */

/*
 *   Utility structs and functions shared by all
 *   RTM packet types
 */
struct rxr_rtm_base_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	uint32_t msg_id;
};

static inline
struct rxr_rtm_base_hdr *rxr_get_rtm_base_hdr(void *pkt)
{
	return (struct rxr_rtm_base_hdr *)pkt;
}

static inline
uint32_t rxr_pkt_msg_id(struct rxr_pkt_entry *pkt_entry)
{
	struct rxr_rtm_base_hdr *rtm_hdr;

	rtm_hdr = rxr_get_rtm_base_hdr(pkt_entry->pkt);
	/* only msg and atomic request has msg_id */
	assert(rtm_hdr->flags & (RXR_REQ_MSG | RXR_REQ_ATOMIC));
	return rtm_hdr->msg_id;
}

size_t rxr_pkt_rtm_total_len(struct rxr_pkt_entry *pkt_entry);

static inline
uint64_t rxr_pkt_rtm_tag(struct rxr_pkt_entry *pkt_entry)
{
	size_t offset;
	uint64_t *tagptr;

	/*
	 * In consideration of performance, this function did not cast header
	 * into different header types to get tag, but assume tag is always
	 * the last member of header.
	 */
	offset = rxr_pkt_req_base_hdr_size(pkt_entry) - sizeof(uint64_t);
	tagptr = (uint64_t *)((char *)pkt_entry->pkt + offset);
	return *tagptr;
}

static inline
void rxr_pkt_rtm_settag(struct rxr_pkt_entry *pkt_entry, uint64_t tag)
{
	size_t offset;
	uint64_t *tagptr;

	offset = rxr_pkt_req_base_hdr_size(pkt_entry) - sizeof(uint64_t);
	/* tag is always the last member */
	tagptr = (uint64_t *)((char *)pkt_entry->pkt + offset);
	*tagptr = tag;
}

/*
 *   Header structs for each REQ packe type
 */
struct rxr_eager_msgrtm_hdr {
	struct rxr_rtm_base_hdr hdr;
};

struct rxr_eager_tagrtm_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t tag;
};

struct rxr_medium_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t data_len;
	uint64_t offset;
};

struct rxr_medium_msgrtm_hdr {
	struct rxr_medium_rtm_base_hdr hdr;
};

struct rxr_medium_tagrtm_hdr {
	struct rxr_medium_rtm_base_hdr hdr;
	uint64_t tag;
};

static inline
struct rxr_medium_rtm_base_hdr *rxr_get_medium_rtm_base_hdr(void *pkt)
{
	return (struct rxr_medium_rtm_base_hdr *)pkt;
}

struct rxr_long_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t data_len;
	uint32_t tx_id;
	uint32_t credit_request;
};

static inline
struct rxr_long_rtm_base_hdr *rxr_get_long_rtm_base_hdr(void *pkt)
{
	return (struct rxr_long_rtm_base_hdr *)pkt;
}

struct rxr_long_msgrtm_hdr {
	struct rxr_long_rtm_base_hdr hdr;
};

struct rxr_long_tagrtm_hdr {
	struct rxr_long_rtm_base_hdr hdr;
	uint64_t tag;
};

struct rxr_read_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t data_len;
	uint32_t tx_id;
	uint32_t read_iov_count;
};

static inline
struct rxr_read_rtm_base_hdr *rxr_get_read_rtm_base_hdr(void *pkt)
{
	return (struct rxr_read_rtm_base_hdr *)pkt;
}

struct rxr_read_msgrtm_hdr {
	struct rxr_read_rtm_base_hdr hdr;
};

struct rxr_read_tagrtm_hdr {
	struct rxr_read_rtm_base_hdr hdr;
	uint64_t tag;
};

static inline
int rxr_read_rtm_pkt_type(int op)
{
	assert(op == ofi_op_tagged || op == ofi_op_msg);
	return (op == ofi_op_tagged) ? RXR_READ_TAGRTM_PKT
				     : RXR_READ_MSGRTM_PKT;
}

/*
 *  init() functions for RTM packets
 */
ssize_t rxr_pkt_init_eager_msgrtm(struct rxr_ep *ep,
				  struct rxr_tx_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_eager_tagrtm(struct rxr_ep *ep,
				  struct rxr_tx_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_medium_msgrtm(struct rxr_ep *ep,
				   struct rxr_tx_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_medium_tagrtm(struct rxr_ep *ep,
				   struct rxr_tx_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_long_msgrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_long_tagrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_read_msgrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_read_tagrtm(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);
/*
 *   handle_sent() functions for RTM packets
 */
static inline
void rxr_pkt_handle_eager_rtm_sent(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	/* there is nothing to be done for eager RTM */
	return;
}

void rxr_pkt_handle_medium_rtm_sent(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_long_rtm_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_read_rtm_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
}

/*
 *   handle_send_completion() functions for RTM packet types
 */
void rxr_pkt_handle_eager_rtm_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_medium_rtm_send_completion(struct rxr_ep *ep,
					       struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_long_rtm_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_read_rtm_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
}

/*
 *   proc() functions for RTM packet types
 */
void rxr_pkt_rtm_init_rx_entry(struct rxr_pkt_entry *pkt_entry,
			       struct rxr_rx_entry *rx_entry);

/*         This function is called by both
 *            rxr_pkt_handle_rtm_recv() and
 *            rxr_msg_handle_unexp_match()
 */
ssize_t rxr_pkt_proc_matched_rtm(struct rxr_ep *ep,
				 struct rxr_rx_entry *rx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_proc_rtm_rta(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);
/*
 *         This function is shared by all RTM packet types which handle
 *         reordering
 */
void rxr_pkt_handle_rtm_rta_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry);

/* Structs and functions for RTW packet types
 * There are 3 write protocols
 *         Eager write protocol,
 *         Long write protocol and
 *         Read write protocol (write by read)
 * Each protocol correspond to a packet type
 */

/*
 *     Header structs
 */
struct rxr_rtw_base_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
};

static inline
struct rxr_rtw_base_hdr *rxr_get_rtw_base_hdr(void *pkt)
{
	return (struct rxr_rtw_base_hdr *)pkt;
}

struct rxr_eager_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	struct fi_rma_iov rma_iov[0];
};

struct rxr_long_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t data_len;
	uint32_t tx_id;
	uint32_t credit_request;
	struct fi_rma_iov rma_iov[0];
};

struct rxr_read_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t data_len;
	uint32_t tx_id;
	uint32_t read_iov_count;
	struct fi_rma_iov rma_iov[0];
};

/*
 *     init() functions for each RTW packet types
 */
ssize_t rxr_pkt_init_eager_rtw(struct rxr_ep *ep,
			       struct rxr_tx_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_long_rtw(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_read_rtw(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry);
/*
 *     handle_sent() functions
 */
static inline
void rxr_pkt_handle_eager_rtw_sent(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	/* For eager RTW, there is nothing to be done here */
	return;
}

void rxr_pkt_handle_long_rtw_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_read_rtw_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
}

/*
 *     handle_send_completion() functions
 */
void rxr_pkt_handle_eager_rtw_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_long_rtw_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_read_rtw_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
}

/*
 *     handle_recv() functions
 */
void rxr_pkt_handle_eager_rtw_recv(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_long_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_read_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

/* Structs and functions for RTR packet types
 * There are 3 read protocols
 *         Short protocol,
 *         Long read protocol and
 *         RDMA read protocol
 * Each protocol correspond to a packet type
 */

/*
 *     Header structs
 */
struct rxr_rtr_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t data_len;
	uint32_t read_req_rx_id;
	uint32_t read_req_window;
	struct fi_rma_iov rma_iov[0];
};

static inline
struct rxr_rtr_hdr *rxr_get_rtr_hdr(void *pkt)
{
	return (struct rxr_rtr_hdr *)pkt;
}

/*
 *     init() functions for each RTW packet types
 */
ssize_t rxr_pkt_init_short_rtr(struct rxr_ep *ep,
			       struct rxr_tx_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_long_rtr(struct rxr_ep *ep,
			      struct rxr_tx_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry);

/*
 *     handle_sent() functions
 */
void rxr_pkt_handle_rtr_sent(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

/*
 *     handle_send_completion() functions
 */
void rxr_pkt_handle_rtr_send_completion(struct rxr_ep *ep,
					struct rxr_pkt_entry *pkt_entry);
/*
 *     handle_recv() functions
 */
void rxr_pkt_handle_rtr_recv(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

/* Structs and functions for RTW packet types
 * There are 2 atomic protocols
 *         write atomic protocol and, 
 *         read/compare atomic protocol and
 * Each protocol correspond to a packet type
 */
struct rxr_rta_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	uint32_t msg_id;
	/* end of rtm_base_hdr, atomic packet need msg_id for reordering */
	uint32_t rma_iov_count;
	uint32_t atomic_datatype;
	uint32_t atomic_op;
	uint32_t tx_id;
	struct fi_rma_iov rma_iov[0];
};

static inline
struct rxr_rta_hdr *rxr_get_rta_hdr(void *pkt)
{
	return (struct rxr_rta_hdr *)pkt;
}

ssize_t rxr_pkt_init_write_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry, struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_fetch_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry, struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_compare_rta(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry, struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_rta_sent(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_write_rta_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry);

/* no action to be taken for compare_rta and fetch rta's send completion therefore
 * there are not functions named rxr_pkt_handle_compare/fetch_rta_send_completion()
 */

int rxr_pkt_proc_write_rta(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry);

int rxr_pkt_proc_fetch_rta(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry);

int rxr_pkt_proc_compare_rta(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_rta_recv(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);
#endif
