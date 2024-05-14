/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_DOMAIN_H
#define EFA_DOMAIN_H

#include <infiniband/verbs.h>
#include "efa_device.h"
#include "efa_hmem.h"
#include "efa_env.h"
#include "ofi_hmem.h"
#include "ofi_util.h"

struct efa_domain {
	struct util_domain	util_domain;
	struct fi_info		*shm_info;
	struct fid_domain	*shm_domain;
	struct efa_device	*device;
	struct ibv_pd		*ibv_pd;
	struct fi_info		*info;
	struct efa_fabric	*fabric;
	struct ofi_mr_cache	*cache;
	struct efa_qp		**qp_table;
	size_t			qp_table_sz_m1;
	struct efa_hmem_info	hmem_info[OFI_HMEM_MAX];
	size_t			mtu_size;
	size_t			addrlen;
	bool 			mr_local;
	uint64_t		rdm_mode;
	size_t			rdm_cq_size;
	struct dlist_entry	list_entry; /* linked to g_efa_domain_list */
	struct ofi_genlock	srx_lock; /* shared among peer providers */
};

extern struct dlist_entry g_efa_domain_list;

/*
 * efa_is_cache_available() is a check to see whether a memory registration
 * cache is available to be used by this domain.
 *
 * Return value:
 *    return true if a memory registration cache exists in this domain.
 *    return false if a memory registration cache does not exist in this domain.
 */
static inline bool efa_is_cache_available(struct efa_domain *efa_domain)
{
	return efa_domain->cache;
}

/**
 * @brief domain name suffix according to endpoint type
 *
 * @param	ep_type[in]		end point type
 * @return	a string to be append to domain name
 */
static inline
const char *efa_domain_name_suffix(enum fi_ep_type ep_type)
{
	assert(ep_type == FI_EP_RDM || ep_type == FI_EP_DGRAM);
	return (ep_type == FI_EP_RDM) ? "-rdm" : "-dgrm";
}

/**
 * @brief get prov_info according to endpoint type
 *
 * @param	efa_domain[in]		EFA domain
 * @param	ep_type[in]		end point type
 * @return	pointer to prov_info
 */
static inline
struct fi_info *efa_domain_get_prov_info(struct efa_domain *efa_domain, enum fi_ep_type ep_type)
{
	assert(ep_type == FI_EP_RDM || ep_type == FI_EP_DGRAM);
	return (ep_type == FI_EP_RDM) ? efa_domain->device->rdm_info : efa_domain->device->dgram_info;
}

static inline
bool efa_domain_support_rnr_retry_modify(struct efa_domain *domain)
{
#if HAVE_CAPS_RNR_RETRY
	return domain->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RNR_RETRY;
#else
	return false;
#endif
}

int efa_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		    struct fid_domain **domain_fid, void *context);

#endif
