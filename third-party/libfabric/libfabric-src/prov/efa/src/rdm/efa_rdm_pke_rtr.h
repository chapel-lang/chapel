/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_PKE_RTR_H
#define EFA_RDM_PKE_RTR_H

#include "efa_rdm_pke.h"
#include "efa_rdm_protocol.h"

static inline
struct efa_rdm_rtr_hdr *efa_rdm_pke_get_rtr_hdr(struct efa_rdm_pke *pkt_entry)
{
	return (struct efa_rdm_rtr_hdr *)pkt_entry->wiredata;
}

ssize_t efa_rdm_pke_init_short_rtr(struct efa_rdm_pke *pkt_entry,
				   struct efa_rdm_ope *txe);

ssize_t efa_rdm_pke_init_longcts_rtr(struct efa_rdm_pke *pkt_entry,
				     struct efa_rdm_ope *txe);

void efa_rdm_pke_handle_rtr_recv(struct efa_rdm_pke *pkt_entry);

#endif