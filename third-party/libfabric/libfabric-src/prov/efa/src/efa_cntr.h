/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef _EFA_CNTR_H_
#define _EFA_CNTR_H_

struct efa_cntr {
	struct util_cntr util_cntr;
	struct fid_cntr *shm_cntr;
	struct dlist_entry ibv_cq_poll_list;
	/* Only used by RDM EP type */
	bool need_to_scan_ep_list;
};

int efa_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context);

int efa_rdm_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		      struct fid_cntr **cntr_fid, void *context);

void efa_cntr_report_tx_completion(struct util_ep *ep, uint64_t flags);

void efa_cntr_report_rx_completion(struct util_ep *ep, uint64_t flags);

void efa_cntr_report_error(struct util_ep *ep, uint64_t flags);

#endif

