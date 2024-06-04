/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_PKE_RTM_H
#define EFA_RDM_PKE_RTM_H

#include "efa_rdm_pke.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pke_req.h"

/**
 * @brief get the RTM base_hdr of a RTM pakcet entry
 *
 * @param[in]	pke	RTM packet entry
 *
 * @returns
 * pointer to #efa_rdm_rtm_base_hdr
 */
static inline
struct efa_rdm_rtm_base_hdr *efa_rdm_pke_get_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_rtm_base_hdr *)pke->wiredata;
}

/**
 * @brief get the message ID (msg_id) of a RTM packet entry
 *
 * Each RTM packet has a message ID, which is to used by
 * EFA provider to support in-order transfer
 *
 * @param[in]	pkt_entry	RTM packet entry
 * @returns
 * a 32 bit integer
 */
static inline
uint32_t efa_rdm_pke_get_rtm_msg_id(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_rtm_base_hdr *rtm_hdr;

	rtm_hdr = efa_rdm_pke_get_rtm_base_hdr(pkt_entry);
	/* only msg and atomic request has msg_id */
	assert(rtm_hdr->flags & (EFA_RDM_REQ_MSG | EFA_RDM_REQ_ATOMIC));
	return rtm_hdr->msg_id;
}

size_t efa_rdm_pke_get_rtm_msg_length(struct efa_rdm_pke *pkt_entry);

/**
 * @brief the tag of a tagged RTM packet entry
 *
 * @param[in]		pkt_entry	tagged RTM packet entry
 * @return
 * a 64-bits integer
 */
static inline
uint64_t efa_rdm_pke_get_rtm_tag(struct efa_rdm_pke *pkt_entry)
{
	size_t offset;
	uint64_t *tagptr;

	/*
	 * In consideration of performance, this function did not cast header
	 * into different header types to get tag, but assume tag is always
	 * the last member of header.
	 */
	offset = efa_rdm_pke_get_req_base_hdr_size(pkt_entry) - sizeof(uint64_t);
	tagptr = (uint64_t *)(pkt_entry->wiredata + offset);
	return *tagptr;
}

/**
 * @brief set the "tag" field of the packet header of tagged RTM packet entry
 *
 * @param[in,out]	pkt_entry	tagged RTM packet entry
 * @param[in]		tag		user assigned 64 bits tag
 *
 */
static inline
void efa_rdm_pke_set_rtm_tag(struct efa_rdm_pke *pkt_entry, uint64_t tag)
{
	size_t offset;
	uint64_t *tagptr;

	offset = efa_rdm_pke_get_req_base_hdr_size(pkt_entry) - sizeof(uint64_t);
	/* tag is always the last member */
	tagptr = (uint64_t *)(pkt_entry->wiredata + offset);
	*tagptr = tag;
}

void efa_rdm_pke_rtm_update_rxe(struct efa_rdm_pke *pkt_entry,
				struct efa_rdm_ope *rxe);

ssize_t efa_rdm_pke_proc_matched_rtm(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_proc_rtm_rta(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_rtm_rta_recv(struct efa_rdm_pke *pkt_entry);

static inline
struct efa_rdm_dc_eager_rtm_base_hdr *efa_rdm_pke_get_dc_eager_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_dc_eager_rtm_base_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_dc_eager_msgrtm_hdr *efa_rdm_pke_get_dc_eager_msgrtm_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_dc_eager_msgrtm_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_dc_eager_tagrtm_hdr *efa_rdm_pke_get_dc_eager_tagrtm_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_dc_eager_tagrtm_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_medium_rtm_base_hdr *efa_rdm_pke_get_medium_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_medium_rtm_base_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_dc_medium_rtm_base_hdr *efa_rdm_pke_get_dc_medium_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_dc_medium_rtm_base_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_dc_medium_msgrtm_hdr *efa_rdm_pke_get_dc_medium_msgrtm_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_dc_medium_msgrtm_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_dc_medium_tagrtm_hdr *efa_rdm_pke_get_dc_medium_tagrtm_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_dc_medium_tagrtm_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_longcts_rtm_base_hdr *efa_rdm_pke_get_longcts_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_longcts_rtm_base_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_longread_rtm_base_hdr *efa_rdm_pke_get_longread_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_longread_rtm_base_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_runtread_rtm_base_hdr *efa_rdm_pke_get_runtread_rtm_base_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_runtread_rtm_base_hdr *)pke->wiredata;
}

ssize_t efa_rdm_pke_init_eager_msgrtm(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_eager_tagrtm(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_dc_eager_msgrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_dc_eager_tagrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_eager_rtm_send_completion(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_proc_matched_eager_rtm(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_medium_msgrtm(struct efa_rdm_pke *pkt_entry,
				       struct efa_rdm_ope *txe,
				       size_t data_offset,
				       int data_size);

ssize_t efa_rdm_pke_init_medium_tagrtm(struct efa_rdm_pke *pkt_entry,
				       struct efa_rdm_ope *txe,
				       size_t data_offset,
				       int data_size);

ssize_t efa_rdm_pke_init_dc_medium_msgrtm(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe,
				      size_t data_offset,
				      int data_size);

ssize_t efa_rdm_pke_init_dc_medium_tagrtm(struct efa_rdm_pke *pkt_entry,
					  struct efa_rdm_ope *txe,
					  size_t data_offset,
					  int data_size);

void efa_rdm_pke_handle_medium_rtm_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_medium_rtm_send_completion(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_proc_matched_mulreq_rtm(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_longcts_msgrtm(struct efa_rdm_pke *pkt_entry,
					struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_longcts_tagrtm(struct efa_rdm_pke *pkt_entry,
					struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_dc_longcts_msgrtm(struct efa_rdm_pke *pkt_entry,
					   struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_dc_longcts_tagrtm(struct efa_rdm_pke *pkt_entry,
					   struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_longcts_rtm_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_longcts_rtm_sent(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_longread_msgrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_longread_tagrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_proc_matched_longread_rtm(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_longread_rtm_sent(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_runtread_msgrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe,
					 size_t data_offset,
					 int data_size);

ssize_t efa_rdm_pke_init_runtread_tagrtm(struct efa_rdm_pke *pkt_entry,
					 struct efa_rdm_ope *txe,
					 size_t data_offset,
					 int data_size);

void efa_rdm_pke_handle_runtread_rtm_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_runtread_rtm_send_completion(struct efa_rdm_pke *pkt_entry);

#endif