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

#ifndef EFA_DOMAIN_H
#define EFA_DOAMIN_H

#include <infiniband/verbs.h>
#include "efa_hmem.h"

struct efa_domain {
	struct util_domain	util_domain;
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

#endif
