/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_DGRAM_CQ_H
#define EFA_DGRAM_CQ_H

typedef void (*efa_dgram_cq_read_entry)(struct ibv_cq_ex *ibv_cqx, int index, void *buf);

struct efa_dgram_cq {
	struct util_cq		util_cq;
	struct efa_domain	*domain;
	size_t			entry_size;
	efa_dgram_cq_read_entry	read_entry;
	ofi_spin_t		lock;
	struct ofi_bufpool	*wce_pool;
	uint32_t	flags; /* User defined capability mask */

	struct ibv_cq_ex	*ibv_cq_ex;
};

int efa_dgram_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		      struct fid_cq **cq_fid, void *context);

ssize_t efa_dgram_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count, fi_addr_t *src_addr);

ssize_t efa_dgram_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry, uint64_t flags);

#endif