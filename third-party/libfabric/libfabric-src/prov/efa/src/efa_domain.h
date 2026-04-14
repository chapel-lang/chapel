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
#include "ofi_lock.h"

enum efa_domain_info_type {
	EFA_INFO_RDM,
	EFA_INFO_DIRECT,
	EFA_INFO_DGRAM,
};

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
	size_t			mtu_size;
	size_t			addrlen;
	bool 			mr_local;
	struct dlist_entry	list_entry; /* linked to g_efa_domain_list */
	struct ofi_genlock	srx_lock; /* shared among peer providers */
	struct efa_ah		*ah_map;
	/* Total count of ibv memory registrations */
	size_t ibv_mr_reg_ct;
	/* Total size of memory registrations (in bytes) */
	size_t ibv_mr_reg_sz;
	/* info_type is used to distinguish between the rdm, dgram and
	 * efa-direct paths */
	enum efa_domain_info_type info_type;

	size_t			rdm_cq_size;
	/* number of rdma-read messages in flight */
	uint64_t		num_read_msg_in_flight;
	/* queued op entries */
	struct dlist_entry ope_queued_list;
	/* tx/rx_entries used by long CTS msg/write/read protocol
         * which have data to be sent */
	struct dlist_entry ope_longcts_send_list;
	/* list of #efa_rdm_peer that are in backoff due to RNR */
	struct dlist_entry peer_backoff_list;
	/* list of #efa_rdm_peer that will retry posting handshake pkt */
	struct dlist_entry handshake_queued_peer_list;
	/* Function pointer for internal buffer memory registration */
	int (*internal_buf_mr_regv)(struct fid_domain *domain_fid,
				    const struct iovec *iov, size_t count,
				    uint64_t access, uint64_t offset,
				    uint64_t requested_key, uint64_t flags,
				    struct fid_mr **mr_fid, void *context);
};

extern struct dlist_entry g_efa_domain_list;
extern ofi_mutex_t g_efa_domain_list_lock;

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

void efa_domain_progress_rdm_peers_and_queues(struct efa_domain *domain);

#endif
