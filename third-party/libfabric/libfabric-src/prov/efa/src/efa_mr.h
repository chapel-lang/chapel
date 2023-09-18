/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EFA_MR_H
#define EFA_MR_H

#include "efa.h"

/*
 * Descriptor returned for FI_HMEM peer memory registrations
 */
struct efa_mr_peer {
	enum fi_hmem_iface      iface;
	union {
		uint64_t        reserved;
		/* this field is gdrcopy handle when gdrcopy is enabled,
		 * otherwise it is cuda device id.
		 */
		uint64_t        cuda;
		int             neuron;
		int             synapseai;
	} device;
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
};

extern int efa_mr_cache_enable;
extern size_t efa_mr_max_cached_count;
extern size_t efa_mr_max_cached_size;

int efa_mr_cache_open(struct ofi_mr_cache **cache, struct efa_domain *domain);

extern struct fi_ops_mr efa_domain_mr_ops;
extern struct fi_ops_mr efa_domain_mr_cache_ops;

int efa_mr_cache_entry_reg(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry);

void efa_mr_cache_entry_dereg(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry);

int efa_mr_reg_shm(struct fid_domain *domain_fid, struct iovec *iov,
		   uint64_t access, struct fid_mr **mr_fid);

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

#endif
