/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_PKE_REQ_H
#define _EFA_RDM_PKE_REQ_H

#include <stdint.h>
#include <stdlib.h>

struct efa_rdm_ope;

struct efa_rdm_pke;

void efa_rdm_pke_init_req_hdr_common(struct efa_rdm_pke *pkt_entry,
			      int pkt_type,
			      struct efa_rdm_ope *txe);

void *efa_rdm_pke_get_req_raw_addr(struct efa_rdm_pke *pkt_entry);

uint64_t efa_rdm_pke_get_req_cq_data(struct efa_rdm_pke *pkt_entry);

uint32_t *efa_rdm_pke_get_req_connid_ptr(struct efa_rdm_pke *pkt_entry);

size_t efa_rdm_pke_get_req_base_hdr_size(struct efa_rdm_pke *pkt_entry);

size_t efa_rdm_pke_get_req_hdr_size(struct efa_rdm_pke *pkt_entry);

#endif
