/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_PKE_RTW_H
#define EFA_RDM_PKE_RTW_H

#include "efa_rdm_pke.h"
#include "efa_rdm_protocol.h"

static inline
struct efa_rdm_rtw_base_hdr *efa_rdm_pke_get_rtw_base_hdr(struct efa_rdm_pke *pkt_entry)
{
	return (struct efa_rdm_rtw_base_hdr *)pkt_entry->wiredata;
}

static inline
struct efa_rdm_dc_eager_rtw_hdr *efa_rdm_pke_dc_eager_rtw_hdr(struct efa_rdm_pke *pkt_entry)
{
	return (struct efa_rdm_dc_eager_rtw_hdr *)pkt_entry->wiredata;
}

ssize_t efa_rdm_pke_init_eager_rtw(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_eager_rtw_send_completion(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_eager_rtw_recv(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_dc_eager_rtw(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_dc_eager_rtw_recv(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_longcts_rtw(struct efa_rdm_pke *pkt_entry,
				     struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_longcts_rtw_sent(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_handle_longcts_rtw_send_completion(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_dc_longcts_rtw(struct efa_rdm_pke *pkt_entry,
					struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_longcts_rtw_recv(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_longread_rtw(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_longread_rtw_recv(struct efa_rdm_pke *pkt_entry);

#endif
