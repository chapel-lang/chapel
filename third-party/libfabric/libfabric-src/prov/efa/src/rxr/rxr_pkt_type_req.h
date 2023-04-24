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

#define RXR_MSG_PREFIX_SIZE (sizeof(struct rxr_pkt_entry) + sizeof(struct rxr_eager_msgrtm_hdr) + RXR_REQ_OPT_RAW_ADDR_HDR_SIZE)

#if defined(static_assert) && defined(__x86_64__)
static_assert(RXR_MSG_PREFIX_SIZE % 8 == 0, "message prefix size alignment check");
#endif

bool rxr_pkt_req_supported_by_peer(int req_type, struct rdm_peer *peer);

void *rxr_pkt_req_raw_addr(struct rxr_pkt_entry *pkt_entry);

int64_t rxr_pkt_req_cq_data(struct rxr_pkt_entry *pkt_entry);

uint32_t *rxr_pkt_req_connid_ptr(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_hdr_size_from_pkt_entry(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_base_hdr_size(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_data_size(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_hdr_size(int pkt_type, uint16_t flags, size_t rma_iov_count);

uint32_t rxr_pkt_hdr_rma_iov_count(struct rxr_pkt_entry *pkt_entry);

size_t rxr_pkt_req_max_hdr_size(int pkt_type);

size_t rxr_pkt_max_hdr_size(void);

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

static inline
struct rxr_dc_eager_rtm_base_hdr *rxr_get_dc_eager_rtm_base_hdr(void *pkt)
{
	return (struct rxr_dc_eager_rtm_base_hdr *)pkt;
}

static inline
struct rxr_dc_eager_msgrtm_hdr *rxr_get_dc_eager_msgrtm_hdr(void *pkt)
{
	return (struct rxr_dc_eager_msgrtm_hdr *)pkt;
}

static inline
struct rxr_dc_eager_tagrtm_hdr *rxr_get_dc_eager_tagrtm_hdr(void *pkt)
{
	return (struct rxr_dc_eager_tagrtm_hdr *)pkt;
}

static inline
struct rxr_medium_rtm_base_hdr *rxr_get_medium_rtm_base_hdr(void *pkt)
{
	return (struct rxr_medium_rtm_base_hdr *)pkt;
}

static inline
struct rxr_dc_medium_rtm_base_hdr *rxr_get_dc_medium_rtm_base_hdr(void *pkt)
{
	return (struct rxr_dc_medium_rtm_base_hdr *)pkt;
}

static inline
struct rxr_dc_medium_msgrtm_hdr *rxr_get_dc_medium_msgrtm_hdr(void *pkt)
{
	return (struct rxr_dc_medium_msgrtm_hdr *)pkt;
}

static inline
struct rxr_dc_medium_tagrtm_hdr *rxr_get_dc_medium_tagrtm_hdr(void *pkt)
{
	return (struct rxr_dc_medium_tagrtm_hdr *)pkt;
}

static inline
struct rxr_longcts_rtm_base_hdr *rxr_get_longcts_rtm_base_hdr(void *pkt)
{
	return (struct rxr_longcts_rtm_base_hdr *)pkt;
}

static inline
struct rxr_longread_rtm_base_hdr *rxr_get_longread_rtm_base_hdr(void *pkt)
{
	return (struct rxr_longread_rtm_base_hdr *)pkt;
}

static inline
struct rxr_runtread_rtm_base_hdr *rxr_get_runtread_rtm_base_hdr(void *pkt)
{
	return (struct rxr_runtread_rtm_base_hdr *)pkt;
}

ssize_t rxr_pkt_init_eager_msgrtm(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_eager_msgrtm(struct rxr_ep *ep,
				     struct rxr_op_entry *tx_entry,
				     struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_eager_tagrtm(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_medium_msgrtm(struct rxr_ep *ep,
				   struct rxr_op_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_eager_tagrtm(struct rxr_ep *ep,
				     struct rxr_op_entry *tx_entry,
				     struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_medium_msgrtm(struct rxr_ep *ep,
				      struct rxr_op_entry *tx_entry,
				      struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_medium_tagrtm(struct rxr_ep *ep,
				   struct rxr_op_entry *tx_entry,
				   struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_medium_tagrtm(struct rxr_ep *ep,
				      struct rxr_op_entry *tx_entry,
				      struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longcts_msgrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_longcts_msgrtm(struct rxr_ep *ep,
				    struct rxr_op_entry *tx_entry,
				    struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longcts_tagrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_longcts_tagrtm(struct rxr_ep *ep,
				    struct rxr_op_entry *tx_entry,
				    struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longread_msgrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longread_tagrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_runtread_msgrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_runtread_tagrtm(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_eager_rtm_sent(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	/* there is nothing to be done for eager RTM */
	return;
}

void rxr_pkt_handle_medium_rtm_sent(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_longcts_rtm_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_longread_rtm_sent(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_runtread_rtm_sent(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_eager_rtm_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_medium_rtm_send_completion(struct rxr_ep *ep,
					       struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_longcts_rtm_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_dc_longcts_rtm_send_completion(struct rxr_ep *ep,
						struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_longread_rtm_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_runtread_rtm_send_completion(struct rxr_ep *ep,
						 struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_rtm_update_rx_entry(struct rxr_pkt_entry *pkt_entry,
				 struct rxr_op_entry *rx_entry);

/*         This function is called by both
 *            rxr_pkt_handle_rtm_recv() and
 *            rxr_msg_handle_unexp_match()
 */
ssize_t rxr_pkt_proc_matched_rtm(struct rxr_ep *ep,
				 struct rxr_op_entry *rx_entry,
				 struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_proc_rtm_rta(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);
/*
 *         This function handles zero-copy receives that do not require ordering
 */
void rxr_pkt_handle_zcpy_recv(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry);
/*
 *         This function is shared by all RTM packet types which handle
 *         reordering
 */
void rxr_pkt_handle_rtm_rta_recv(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry);

static inline
struct rxr_rtw_base_hdr *rxr_get_rtw_base_hdr(void *pkt)
{
	return (struct rxr_rtw_base_hdr *)pkt;
}

static inline
struct rxr_dc_eager_rtw_hdr *rxr_get_dc_eager_rtw_hdr(void *pkt)
{
	return (struct rxr_dc_eager_rtw_hdr *)pkt;
}

ssize_t rxr_pkt_init_eager_rtw(struct rxr_ep *ep,
			       struct rxr_op_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longcts_rtw(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longread_rtw(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_eager_rtw(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_longcts_rtw(struct rxr_ep *ep,
				 struct rxr_op_entry *tx_entry,
				 struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_eager_rtw_sent(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry)
{
	/* For eager RTW, there is nothing to be done here */
	return;
}

void rxr_pkt_handle_longcts_rtw_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_longread_rtw_sent(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_eager_rtw_send_completion(struct rxr_ep *ep,
					      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_longcts_rtw_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_dc_longcts_rtw_send_completion(struct rxr_ep *ep,
						struct rxr_pkt_entry *pkt_entry);

static inline
void rxr_pkt_handle_longread_rtw_send_completion(struct rxr_ep *ep,
					     struct rxr_pkt_entry *pkt_entry)
{
}

void rxr_pkt_handle_eager_rtw_recv(struct rxr_ep *ep,
				   struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_dc_eager_rtw_recv(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_longcts_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_longread_rtw_recv(struct rxr_ep *ep,
				  struct rxr_pkt_entry *pkt_entry);
static inline
struct rxr_rtr_hdr *rxr_get_rtr_hdr(void *pkt)
{
	return (struct rxr_rtr_hdr *)pkt;
}

ssize_t rxr_pkt_init_short_rtr(struct rxr_ep *ep,
			       struct rxr_op_entry *tx_entry,
			       struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_longcts_rtr(struct rxr_ep *ep,
			      struct rxr_op_entry *tx_entry,
			      struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_rtr_sent(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_rtr_recv(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

static inline
struct rxr_rta_hdr *rxr_get_rta_hdr(void *pkt)
{
	return (struct rxr_rta_hdr *)pkt;
}

ssize_t rxr_pkt_init_write_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry, struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_dc_write_rta(struct rxr_ep *ep,
				  struct rxr_op_entry *tx_entry,
				  struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_fetch_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry, struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_init_compare_rta(struct rxr_ep *ep, struct rxr_op_entry *tx_entry, struct rxr_pkt_entry *pkt_entry);

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

int rxr_pkt_proc_dc_write_rta(struct rxr_ep *ep,
			      struct rxr_pkt_entry *pkt_entry);

int rxr_pkt_proc_fetch_rta(struct rxr_ep *ep,
			   struct rxr_pkt_entry *pkt_entry);

int rxr_pkt_proc_compare_rta(struct rxr_ep *ep,
			     struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_rta_recv(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

#endif
