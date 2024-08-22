/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_MR_H
#define EFA_MR_H

#include <stdbool.h>
#include <ofi_mr.h>

/*
 * Descriptor returned for FI_HMEM peer memory registrations
 */
struct efa_mr_peer {
	enum fi_hmem_iface  iface;
	union {
	    uint64_t        reserved;
	    uint64_t        cuda;
	    int             neuron;
	    int             synapseai;
	} device;
	uint64_t            flags;
	void                *hmem_data;
};

struct efa_mr {
	struct fid_mr		mr_fid;
	struct ibv_mr		*ibv_mr;
	struct efa_domain	*domain;
	/* Used only in MR cache */
	struct ofi_mr_entry	*entry;
	/* Used only in rdm */
	struct fid_mr		*shm_mr;
	struct efa_mr_peer	peer;
	bool			inserted_to_mr_map;
	bool 			needs_sync;
};

extern int efa_mr_cache_enable;
extern size_t efa_mr_max_cached_count;
extern size_t efa_mr_max_cached_size;

struct efa_domain;

int efa_mr_cache_open(struct ofi_mr_cache **cache, struct efa_domain *domain);

extern struct fi_ops_mr efa_domain_mr_ops;
extern struct fi_ops_mr efa_domain_mr_cache_ops;

int efa_mr_cache_entry_reg(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry);

void efa_mr_cache_entry_dereg(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry);

static inline bool efa_mr_is_hmem(struct efa_mr *efa_mr)
{
	return efa_mr ? (efa_mr->peer.iface == FI_HMEM_CUDA ||
			 efa_mr->peer.iface == FI_HMEM_NEURON ||
			 efa_mr->peer.iface == FI_HMEM_SYNAPSEAI): false;
}

static inline bool efa_mr_is_cuda(struct efa_mr *efa_mr)
{
	return efa_mr ? (efa_mr->peer.iface == FI_HMEM_CUDA) : false;
}

static inline bool efa_mr_is_neuron(struct efa_mr *efa_mr)
{
	return efa_mr ? (efa_mr->peer.iface == FI_HMEM_NEURON) : false;
}

static inline bool efa_mr_is_synapseai(struct efa_mr *efa_mr)
{
	return efa_mr ? (efa_mr->peer.iface == FI_HMEM_SYNAPSEAI) : false;
}

static inline void *efa_mr_get_shm_desc(struct efa_mr *efa_mr)
{
	if (!efa_mr)
		return NULL;

	return efa_mr->shm_mr ? fi_mr_desc(efa_mr->shm_mr) : NULL;
}
#define EFA_MR_IOV_LIMIT 1
#define EFA_MR_SUPPORTED_PERMISSIONS (FI_SEND | FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE)

/*
 * Multiplier to give some room in the device memory registration limits
 * to allow processes added to a running job to bootstrap.
 */
#define EFA_MR_CACHE_LIMIT_MULT (.9)

#endif
