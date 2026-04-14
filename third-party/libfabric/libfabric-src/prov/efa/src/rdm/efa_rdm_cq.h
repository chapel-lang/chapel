/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_CQ_H
#define EFA_RDM_CQ_H

#include "efa_cq.h"
#include <ofi_util.h>

struct efa_rdm_cq {
	struct efa_cq efa_cq;
	struct fid_cq *shm_cq;
	struct dlist_entry ibv_cq_poll_list;
	bool need_to_scan_ep_list;
};

int efa_rdm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		    struct fid_cq **cq_fid, void *context);

void efa_rdm_cq_poll_ibv_cq_closing_ep(struct efa_ibv_cq *ibv_cq, struct efa_rdm_ep *closing_ep);
int efa_rdm_cq_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq);

void efa_rdm_cq_progress_peers_and_queues(struct efa_rdm_cq *efa_rdm_cq);
#endif
