/* Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */

#ifndef _EFA_RDM_PKE_NONREQ_H
#define _EFA_RDM_PKE_NONREQ_H

#include "efa_rdm_ope.h"
#include "efa_rdm_protocol.h"
#include "efa_rdm_pke_utils.h"

struct efa_rdm_ep;
struct efa_rdm_peer;

/* HANDSHAKE packet related functions */
static inline
struct efa_rdm_handshake_hdr *efa_rdm_pke_get_handshake_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_handshake_hdr *)pke->wiredata;
}

static inline
struct efa_rdm_handshake_opt_connid_hdr *efa_rdm_pke_get_handshake_opt_connid_hdr(struct efa_rdm_pke *pke)
{
	struct efa_rdm_handshake_hdr *handshake_hdr;
	size_t base_hdr_size;

	handshake_hdr = (struct efa_rdm_handshake_hdr *)pke->wiredata;
	assert(handshake_hdr->type == EFA_RDM_HANDSHAKE_PKT);
	assert(handshake_hdr->flags & EFA_RDM_PKT_CONNID_HDR);
	base_hdr_size = sizeof(struct efa_rdm_handshake_hdr) +
			(handshake_hdr->nextra_p3 - 3) * sizeof(uint64_t);
	return (struct efa_rdm_handshake_opt_connid_hdr *)((char *)pke->wiredata + base_hdr_size);
}

/**
 * @brief Return a pointer to the optional host id header in a handshake packet
 *
 * @param[in]	pkt_entry	A packet entry
 * @return	If the input has the optional host id header, return the pointer to
 *	host id value; otherwise, return NULL
 */
static inline
uint64_t *efa_rdm_pke_get_handshake_opt_host_id_ptr(struct efa_rdm_pke *pke)
{
	struct efa_rdm_base_hdr *base_hdr = efa_rdm_pke_get_base_hdr(pke);
	struct efa_rdm_handshake_hdr *handshake_hdr;
	struct efa_rdm_handshake_opt_host_id_hdr *handshake_opt_host_id_hdr;
	size_t offset = 0;

	if (base_hdr->type != EFA_RDM_HANDSHAKE_PKT || !(base_hdr->flags & EFA_RDM_HANDSHAKE_HOST_ID_HDR))
		return NULL;

	handshake_hdr = (struct efa_rdm_handshake_hdr *)pke->wiredata;
	assert(handshake_hdr->type == EFA_RDM_HANDSHAKE_PKT);

	offset += sizeof(struct efa_rdm_handshake_hdr) +
		  (handshake_hdr->nextra_p3 - 3) * sizeof(uint64_t);

	assert(handshake_hdr->flags & EFA_RDM_HANDSHAKE_HOST_ID_HDR);

	if (handshake_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		/* HOST_ID_HDR is always immediately after CONNID_HDR(if present) */
		offset += sizeof(struct efa_rdm_handshake_opt_connid_hdr);
	}

	handshake_opt_host_id_hdr = (struct efa_rdm_handshake_opt_host_id_hdr *)(pke->wiredata + offset);
	return &handshake_opt_host_id_hdr->host_id;
}

static inline
uint32_t efa_rdm_pke_get_handshake_opt_device_version(struct efa_rdm_pke *pke)
{
	struct efa_rdm_handshake_hdr *handshake_hdr;
	struct efa_rdm_handshake_opt_device_version_hdr *device_version_hdr;
	size_t offset;

	handshake_hdr = efa_rdm_pke_get_handshake_hdr(pke);
	assert(handshake_hdr->type == EFA_RDM_HANDSHAKE_PKT);
	assert(handshake_hdr->flags & EFA_RDM_HANDSHAKE_DEVICE_VERSION_HDR);

	offset = sizeof (struct efa_rdm_handshake_hdr)
		+ ((handshake_hdr->nextra_p3 - 3) * sizeof handshake_hdr->extra_info[0]);

	if (handshake_hdr->flags & EFA_RDM_PKT_CONNID_HDR)
		offset += sizeof (struct efa_rdm_handshake_opt_connid_hdr);
	if (handshake_hdr->flags & EFA_RDM_HANDSHAKE_HOST_ID_HDR)
		offset += sizeof (struct efa_rdm_handshake_opt_host_id_hdr);

	device_version_hdr = (struct efa_rdm_handshake_opt_device_version_hdr *) (pke->wiredata + offset);

	return device_version_hdr->device_version;
}

static inline
struct efa_rdm_handshake_opt_user_recv_qp_hdr *efa_rdm_pke_get_handshake_opt_user_recv_qp_ptr(struct efa_rdm_pke *pke)
{
	struct efa_rdm_handshake_hdr *handshake_hdr;
	size_t offset;

	handshake_hdr = efa_rdm_pke_get_handshake_hdr(pke);
	assert(handshake_hdr->type == EFA_RDM_HANDSHAKE_PKT);
	assert(handshake_hdr->flags & EFA_RDM_HANDSHAKE_USER_RECV_QP_HDR);

	offset = sizeof (struct efa_rdm_handshake_hdr)
		+ ((handshake_hdr->nextra_p3 - 3) * sizeof handshake_hdr->extra_info[0]);

	if (handshake_hdr->flags & EFA_RDM_PKT_CONNID_HDR)
		offset += sizeof (struct efa_rdm_handshake_opt_connid_hdr);
	if (handshake_hdr->flags & EFA_RDM_HANDSHAKE_HOST_ID_HDR)
		offset += sizeof (struct efa_rdm_handshake_opt_host_id_hdr);
	if (handshake_hdr->flags & EFA_RDM_HANDSHAKE_DEVICE_VERSION_HDR)
		offset += sizeof (struct efa_rdm_handshake_opt_device_version_hdr);

	return (struct efa_rdm_handshake_opt_user_recv_qp_hdr *) (pke->wiredata + offset);
}


ssize_t efa_rdm_pke_init_handshake(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_peer *peer);

void efa_rdm_pke_handle_handshake_recv(struct efa_rdm_pke *pkt_entry);

/* CTS packet related functions */
static inline
struct efa_rdm_cts_hdr *efa_rdm_pke_get_cts_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_cts_hdr *)pke->wiredata;
}

void efa_rdm_pke_calc_cts_window_credits(struct efa_rdm_peer *peer,
				     uint64_t size, int request,
				     int *window, int *credits);

ssize_t efa_rdm_pke_init_cts(struct efa_rdm_pke *pkt_entry,
			     struct efa_rdm_ope *ope);

void efa_rdm_pke_handle_cts_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_cts_recv(struct efa_rdm_pke *pkt_entry);

static inline
struct efa_rdm_ctsdata_hdr *efa_rdm_pke_get_ctsdata_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_ctsdata_hdr *)pke->wiredata;
}

int efa_rdm_pke_init_ctsdata(struct efa_rdm_pke *pkt_entry,
			     struct efa_rdm_ope *ope,
			     size_t data_offset,
			     int data_size);

void efa_rdm_pke_handle_ctsdata_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_ctsdata_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_ctsdata_recv(struct efa_rdm_pke *pkt_entry);

/* READRSP packet related functions */
static inline struct efa_rdm_readrsp_hdr *efa_rdm_pke_get_readrsp_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_readrsp_hdr *)pke->wiredata;
}

int efa_rdm_pke_init_readrsp(struct efa_rdm_pke *pkt_entry,
			     struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_readrsp_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_readrsp_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_readrsp_recv(struct efa_rdm_pke *pkt_entry);

/*
 *  RMA context packet, used to differentiate the normal RMA read, normal RMA
 *  write, and the RMA read in two-sided large message transfer
 *  Implementation of the function is in efa_rdm_pke_type_misc.c
 */
struct efa_rdm_rma_context_pkt {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of efa_rdm_base_hdr */
	uint32_t context_type;

	/* Used by write context */
	uint32_t tx_id;
	void *local_buf;
	void *desc;
	uint64_t remote_buf;
	size_t remote_key;

	/* used by read context */
	uint32_t read_id;
	size_t seg_size;
};

enum efa_rdm_rma_context_pkt_type {
	EFA_RDM_RDMA_READ_CONTEXT = 1,
	EFA_RDM_RDMA_WRITE_CONTEXT,
};

void efa_rdm_pke_init_write_context(struct efa_rdm_pke *pkt_entry,
				    struct efa_rdm_ope *txe, void *local_buf,
				    size_t seg_size, void *desc,
				    uint64_t remote_buf, size_t remote_key);

void efa_rdm_pke_init_read_context(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *ope,
				   int read_id,
				   size_t seg_size);

void efa_rdm_pke_handle_rma_completion(struct efa_rdm_pke *pkt_entry);

/* EOR packet related functions */
static inline
struct efa_rdm_eor_hdr *efa_rdm_pke_get_eor_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_eor_hdr *)pke->wiredata;
}

int efa_rdm_pke_init_eor(struct efa_rdm_pke *pkt_entry,
			 struct efa_rdm_ope *rxe);

void efa_rdm_pke_handle_eor_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_eor_recv(struct efa_rdm_pke *pkt_entry);

/* Read NACK packet functions */

static inline
struct efa_rdm_read_nack_hdr *efa_rdm_pke_get_read_nack_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_read_nack_hdr *)pke->wiredata;
}

int efa_rdm_pke_init_read_nack(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe);

void efa_rdm_pke_handle_read_nack_recv(struct efa_rdm_pke *pkt_entry);

/* ATOMRSP packet related functions */
static inline struct efa_rdm_atomrsp_hdr *efa_rdm_pke_get_atomrsp_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_atomrsp_hdr *)pke->wiredata;
}

int efa_rdm_pke_init_atomrsp(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe);

void efa_rdm_pke_handle_atomrsp_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_atomrsp_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_atomrsp_recv(struct efa_rdm_pke *pkt_entry);

/* RECEIPT packet related functions */
static inline
struct efa_rdm_receipt_hdr *efa_rdm_pke_get_receipt_hdr(struct efa_rdm_pke *pke)
{
	return (struct efa_rdm_receipt_hdr *)pke->wiredata;
}

int efa_rdm_pke_init_receipt(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *rxe);

void efa_rdm_pke_handle_receipt_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_receipt_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_receipt_recv(struct efa_rdm_pke *pkt_entry);
#endif
