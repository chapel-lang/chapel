/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_rdm_pke_print.h"
#include "efa_rdm_pke_req.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pke_nonreq.h"

#if ENABLE_DEBUG

/*
 *  Functions used to dump packets
 */

#define EFA_RDM_PKE_DUMP_DATA_LEN 64

static inline
void efa_rdm_pke_print_fi_rma_iov(char *prefix, int iov_count, struct fi_rma_iov *rma_iov)
{
	int i;

	for (i = 0; i < iov_count; ++i) {
		EFA_DBG(FI_LOG_EP_DATA,
			"%s iov[%d].iov_base: %ld, iov[%d].iov_len: %ld "
			"iov[%d].key: %lu\n",
			prefix, i, rma_iov[i].addr, i, rma_iov[i].len, i,
			rma_iov[i].key);
	}
}

static
void efa_rdm_pke_print_handshake(char *prefix,
			         struct efa_rdm_handshake_hdr *handshake_hdr)
{
	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM HANDSHAKE packet - version: %" PRIu8
	       " flags: %x\n", prefix, handshake_hdr->version,
	       handshake_hdr->flags);

	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM HANDSHAKE packet, nextra_p3: %d\n",
	       prefix, handshake_hdr->nextra_p3);
}

static
void efa_rdm_pke_print_cts(char *prefix, struct efa_rdm_cts_hdr *cts_hdr)
{
	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM CTS packet - version: %"	PRIu8
	       " flags: %x tx_id: %" PRIu32
	       " rx_id: %"	   PRIu32
	       " window: %"	   PRIu64
	       "\n", prefix, cts_hdr->version, cts_hdr->flags,
	       cts_hdr->send_id, cts_hdr->recv_id, cts_hdr->recv_length);
}

static
void efa_rdm_pke_print_data(char *prefix, struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ctsdata_hdr *data_hdr;
	char str[EFA_RDM_PKE_DUMP_DATA_LEN * 4];
	size_t str_len = EFA_RDM_PKE_DUMP_DATA_LEN * 4, l, hdr_size;
	uint8_t *data;
	int i;

	str[str_len - 1] = '\0';

	data_hdr = efa_rdm_pke_get_ctsdata_hdr(pkt_entry);

	EFA_DBG(FI_LOG_EP_DATA,
	       "%s EFA RDM CTSDATA packet -  version: %" PRIu8
	       " flags: %x rx_id: %" PRIu32
	       " seg_size: %"	     PRIu64
	       " seg_offset: %"	     PRIu64
	       "\n", prefix, data_hdr->version, data_hdr->flags,
	       data_hdr->recv_id, data_hdr->seg_length,
	       data_hdr->seg_offset);

	hdr_size = sizeof(struct efa_rdm_ctsdata_hdr);
	if (data_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		hdr_size += sizeof(struct efa_rdm_ctsdata_opt_connid_hdr);
		EFA_DBG(FI_LOG_EP_DATA,
		       "sender_connid: %d\n",
		       data_hdr->connid_hdr->connid);
	}

	data = (uint8_t *)pkt_entry->wiredata + hdr_size;

	l = snprintf(str, str_len, ("\tdata:    "));
	for (i = 0; i < MIN(data_hdr->seg_length, EFA_RDM_PKE_DUMP_DATA_LEN);
	     i++)
		l += snprintf(str + l, str_len - l, "%02x ",
			      data[i]);
	EFA_DBG(FI_LOG_EP_DATA, "%s\n", str);
}

static inline
void efa_rdm_pke_print_req_hdr(struct efa_rdm_pke *pkt_entry, struct efa_rdm_rtm_base_hdr *base_hdr, char **opt_hdr)
{
	char *hdr_ptr;
	char raw_gid_str[INET6_ADDRSTRLEN];
	struct efa_rdm_req_opt_raw_addr_hdr *raw_addr_hdr;

	/* logic copied from efa_rdm_pke_init_req_hdr_common */
	hdr_ptr = (char *) base_hdr +
		  efa_rdm_pke_get_req_base_hdr_size(pkt_entry);
	if (base_hdr->flags & EFA_RDM_REQ_OPT_RAW_ADDR_HDR) {
		raw_addr_hdr = (struct efa_rdm_req_opt_raw_addr_hdr *) hdr_ptr;
		if (!inet_ntop(AF_INET6, raw_addr_hdr->raw_addr, raw_gid_str,
			       INET6_ADDRSTRLEN)) {
			EFA_DBG(FI_LOG_EP_DATA,
				"could not parse raw address \n");
		} else {
			EFA_DBG(FI_LOG_EP_DATA, "raw addr len: %d gid %s\n",
				raw_addr_hdr->addr_len, raw_gid_str);
		}
		hdr_ptr += EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE;
	}

	if (base_hdr->flags & EFA_RDM_REQ_OPT_CQ_DATA_HDR) {
		struct efa_rdm_req_opt_cq_data_hdr *cq_data_hdr;
		cq_data_hdr = (struct efa_rdm_req_opt_cq_data_hdr *) hdr_ptr;
		EFA_DBG(FI_LOG_EP_DATA, "cq_data %lu\n", cq_data_hdr->cq_data);
		hdr_ptr += sizeof(*cq_data_hdr);
	}

	if (base_hdr->flags & EFA_RDM_PKT_CONNID_HDR) {
		struct efa_rdm_req_opt_connid_hdr *connid_hdr;
		connid_hdr = (struct efa_rdm_req_opt_connid_hdr *) hdr_ptr;
		EFA_DBG(FI_LOG_EP_DATA, "sender_connid: %d\n",
			connid_hdr->connid);
		hdr_ptr += sizeof(*connid_hdr);
	}

    *opt_hdr = hdr_ptr;
}

static void efa_rdm_pke_print_eager_tag_rtm(char *prefix,
					    struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_rtm_base_hdr *base_hdr;
	struct efa_rdm_eager_tagrtm_hdr *tag_rtm_hdr;
	char str[EFA_RDM_PKE_DUMP_DATA_LEN * 4];
	size_t str_len = EFA_RDM_PKE_DUMP_DATA_LEN * 4, l;
	char *opt_hdr;
	uint8_t *data;
	fi_addr_t fi_addr = FI_ADDR_NOTAVAIL;
	int i;

	str[str_len - 1] = '\0';

	base_hdr = (struct efa_rdm_rtm_base_hdr *) pkt_entry->wiredata;
	tag_rtm_hdr = (struct efa_rdm_eager_tagrtm_hdr *) pkt_entry->wiredata;

	if (pkt_entry->peer)
		fi_addr = pkt_entry->peer->conn->fi_addr;

	EFA_DBG(FI_LOG_EP_DATA,
		"%s EFA RDM RTM packet - type: %" PRIu32 "  version: %" PRIu8
		" flags: %x peer: %" PRIu64 " msg_id: %" PRIu32 " tag: %" PRIu64
		"\n",
		prefix, base_hdr->type, base_hdr->version, base_hdr->flags,
		fi_addr, base_hdr->msg_id, tag_rtm_hdr->tag);

    efa_rdm_pke_print_req_hdr(pkt_entry, base_hdr, &opt_hdr);

	if (pkt_entry->payload) {
		data = (uint8_t *) pkt_entry->payload;
	} else {
		data = (uint8_t *) opt_hdr;
	}

	l = snprintf(str, str_len, ("\tdata:    "));
	for (i = 0; i < MIN(pkt_entry->payload_size, EFA_RDM_PKE_DUMP_DATA_LEN);
	     i++)
		l += snprintf(str + l, str_len - l, "%02x ", data[i]);
	EFA_DBG(FI_LOG_EP_DATA, "%s\n", str);
}

static void efa_rdm_pke_print_longread_rtw(char *prefix,
					   struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_rtm_base_hdr *base_hdr;
	struct efa_rdm_longread_rtw_hdr *rtw_hdr;
	char *opt_hdr;

	base_hdr = (struct efa_rdm_rtm_base_hdr *) pkt_entry->wiredata;
	rtw_hdr = (struct efa_rdm_longread_rtw_hdr *) pkt_entry->wiredata;

	EFA_DBG(FI_LOG_EP_DATA,
		"%s EFA RDM longread RTW packet - type: %" PRIu32
		"  version: %" PRIu8 " flags: %x peer: %" PRIu64
		" msg_id: %" PRIu32 " rma_iov_count: %" PRIu32
		" msg_length: %" PRIu64 " send_id: %" PRIu32
		" read_iov_count: %" PRIu32 "\n",
		prefix, base_hdr->type, base_hdr->version, base_hdr->flags,
		pkt_entry->peer->conn->fi_addr, base_hdr->msg_id, rtw_hdr->rma_iov_count,
		rtw_hdr->msg_length, rtw_hdr->send_id, rtw_hdr->read_iov_count);

	efa_rdm_pke_print_fi_rma_iov("rma_iov", rtw_hdr->rma_iov_count,
				     (struct fi_rma_iov *) &rtw_hdr->rma_iov);

	efa_rdm_pke_print_req_hdr(pkt_entry, base_hdr, &opt_hdr);

	efa_rdm_pke_print_fi_rma_iov("read_iov", rtw_hdr->read_iov_count,
				     (struct fi_rma_iov *) opt_hdr);
}

void efa_rdm_pke_print(struct efa_rdm_pke *pkt_entry, char *prefix)
{
	struct efa_rdm_base_hdr *hdr;

	hdr = efa_rdm_pke_get_base_hdr(pkt_entry);

	switch (hdr->type) {
	case EFA_RDM_HANDSHAKE_PKT:
		efa_rdm_pke_print_handshake(prefix, efa_rdm_pke_get_handshake_hdr(pkt_entry));
		break;
	case EFA_RDM_CTS_PKT:
		efa_rdm_pke_print_cts(prefix, efa_rdm_pke_get_cts_hdr(pkt_entry));
		break;
	case EFA_RDM_CTSDATA_PKT:
		efa_rdm_pke_print_data(prefix, pkt_entry);
		break;
	case EFA_RDM_EAGER_TAGRTM_PKT:
		efa_rdm_pke_print_eager_tag_rtm(prefix, pkt_entry);
		break;
	case EFA_RDM_LONGREAD_RTW_PKT:
		efa_rdm_pke_print_longread_rtw(prefix, pkt_entry);
		break;
	default:
		EFA_WARN(FI_LOG_CQ, "Cannot print pkt type %d\n",
			 hdr->type);
		return;
	}
}
#endif
