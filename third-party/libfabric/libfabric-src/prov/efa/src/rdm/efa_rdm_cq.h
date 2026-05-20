/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_CQ_H
#define EFA_RDM_CQ_H

#include "efa_cq.h"
#include "efa_data_path_ops.h"
#include <ofi_util.h>

struct efa_rdm_cq {
	struct efa_cq efa_cq;
	struct fid_cq *shm_cq;
	struct dlist_entry ibv_cq_poll_list;
	size_t entry_size;
	bool need_to_scan_ep_list;
};

int efa_rdm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		    struct fid_cq **cq_fid, void *context);

int efa_rdm_cq_wait_add_ibv_cq(struct efa_rdm_cq *cq, struct efa_ibv_cq *ibv_cq);
void efa_rdm_cq_wait_del_ibv_cq(struct efa_rdm_cq *cq, struct efa_ibv_cq *ibv_cq);

void efa_rdm_cq_poll_ibv_cq_closing_ep(struct efa_ibv_cq *ibv_cq, struct efa_rdm_ep *closing_ep);
int efa_rdm_cq_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq);

static inline struct efa_rdm_pke *efa_rdm_cq_get_pke_from_wr_id_solicited(uint64_t wr_id)
{
	struct efa_rdm_pke *pkt_entry;
	uint8_t gen = wr_id & EFA_RDM_PACKET_GEN_MASK;

	wr_id &= ~((uint64_t)EFA_RDM_PACKET_GEN_MASK);
	pkt_entry = (struct efa_rdm_pke *) wr_id;
	if (OFI_UNLIKELY(pkt_entry->gen != gen)) {
		EFA_WARN(FI_LOG_CQ, "Received packet from wrong generation! pkt_entry %p expected gen %d received gen %d\n", pkt_entry, pkt_entry->gen, gen);
		assert(pkt_entry->gen == gen);
	}

	return pkt_entry;
}

static inline struct efa_rdm_pke *efa_rdm_cq_get_pke_from_wr_id(struct efa_ibv_cq *ibv_cq, uint64_t wr_id)
{
	if (efa_cq_wc_is_unsolicited(ibv_cq)) {
		return (struct efa_rdm_pke *) wr_id;
	}

	return efa_rdm_cq_get_pke_from_wr_id_solicited(wr_id);
}

#endif
