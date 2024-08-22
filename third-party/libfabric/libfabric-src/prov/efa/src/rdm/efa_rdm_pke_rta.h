/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_PKE_RTA_H
#define EFA_RDM_PKE_RTA_H

#include "efa_rdm_protocol.h"

static inline
struct efa_rdm_rta_hdr *efa_rdm_pke_get_rta_hdr(struct efa_rdm_pke *pkt_entry)
{
	return (struct efa_rdm_rta_hdr *)pkt_entry->wiredata;
}

ssize_t efa_rdm_pke_init_write_rta(struct efa_rdm_pke *pkt_entry, struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_write_rta_send_completion(struct efa_rdm_pke *pkt_entry);

int efa_rdm_pke_proc_write_rta(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_dc_write_rta(struct efa_rdm_pke *pkt_entry,
				      struct efa_rdm_ope *txe);

int efa_rdm_pke_proc_dc_write_rta(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_fetch_rta(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *txe);

int efa_rdm_pke_proc_fetch_rta(struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_pke_init_compare_rta(struct efa_rdm_pke *pkt_entry,
				     struct efa_rdm_ope *txe);

int efa_rdm_pke_proc_compare_rta(struct efa_rdm_pke *pkt_entry);

#endif