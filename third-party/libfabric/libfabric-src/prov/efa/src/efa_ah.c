/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2016, Cisco Systems, Inc. All rights reserved. */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_ah.h"
#include "efa_conn.h"
#include <infiniband/efadv.h>

void efa_ah_destroy_ah(struct efa_domain *domain, struct efa_ah *ah);

/**
 * @brief Move the AH to the end of the LRU list to indicate that it is the
 * most recently used entry
 *
 * This function is not called in the efa_rdm_ep_get_peer so that we don't add
 * extra latency to the critical path with explicit AV insertion. We use the LRU
 * list to remove AH entries with only implicit AV entries, so it is OK to do
 * that.
 *
 * @param[in]	av	efa address vector
 * @param[in]	conn	efa conn to be added to the LRU list
 */
void efa_ah_implicit_av_lru_ah_move(struct efa_domain *domain,
					struct efa_ah *ah)
{
	assert(ah->implicit_refcnt > 0 || ah->explicit_refcnt > 0);
	assert(dlist_entry_in_list(&domain->ah_lru_list,
				   &ah->domain_lru_ah_list_entry));

	dlist_remove(&ah->domain_lru_ah_list_entry);
	dlist_insert_tail(&ah->domain_lru_ah_list_entry,
			  &domain->ah_lru_list);
}

static inline int efa_ah_implicit_av_evict_ah(struct efa_domain *domain) {
	struct efa_conn *conn_to_release;
	struct efa_ah *ah_tmp, *ah_to_release = NULL;
	struct dlist_entry *tmp;

	dlist_foreach_container (&domain->ah_lru_list, struct efa_ah, ah_tmp,
				 domain_lru_ah_list_entry) {
		if (ah_tmp->explicit_refcnt == 0) {
			ah_to_release = ah_tmp;
			break;
		}
	}

	if (!ah_to_release) {
		EFA_WARN(FI_LOG_AV,
			 "AH creation for implicit AV entry failed with ENOMEM "
			 "but no AH entries available to evict\n");
		return -FI_ENOMEM;
	}

	assert(ah_to_release->implicit_refcnt > 0);

	dlist_foreach_container_safe(&ah_to_release->implicit_conn_list,
				      struct efa_conn, conn_to_release,
				      ah_implicit_conn_list_entry, tmp) {

		assert(conn_to_release->implicit_fi_addr != FI_ADDR_NOTAVAIL &&
		       conn_to_release->fi_addr == FI_ADDR_NOTAVAIL);

		efa_conn_release_ah_unsafe(conn_to_release->av, conn_to_release, true);
	}

	if (ah_to_release->implicit_refcnt == 0 &&
	    ah_to_release->explicit_refcnt == 0) {
		efa_ah_destroy_ah(domain, ah_to_release);
	}

	return FI_SUCCESS;
}

/**
 * @brief allocate an ibv_ah object from GID.
 * This function use a hash map to store GID to ibv_ah map,
 * and re-use ibv_ah for same GID
 *
 * @param[in]	domain	efa_domain
 * @param[in]	gid	GID
 */
struct efa_ah *efa_ah_alloc(struct efa_domain *domain, const uint8_t *gid,
			    bool insert_implicit_av)
{
	struct ibv_pd *ibv_pd = domain->ibv_pd;
	struct efa_ah *efa_ah;
	struct ibv_ah_attr ibv_ah_attr = { 0 };
	struct efadv_ah_attr efa_ah_attr = { 0 };
	int err;

	efa_ah = NULL;

	ofi_genlock_lock(&domain->util_domain.lock);
	HASH_FIND(hh, domain->ah_map, gid, EFA_GID_LEN, efa_ah);
	if (efa_ah) {
		insert_implicit_av ? efa_ah->implicit_refcnt++ : efa_ah->explicit_refcnt++;
		efa_ah_implicit_av_lru_ah_move(domain, efa_ah);
		ofi_genlock_unlock(&domain->util_domain.lock);
		return efa_ah;
	}

	efa_ah = malloc(sizeof(struct efa_ah));
	if (!efa_ah) {
		errno = FI_ENOMEM;
		EFA_WARN(FI_LOG_AV, "cannot allocate memory for efa_ah\n");
		return NULL;
	}

	ibv_ah_attr.port_num = 1;
	ibv_ah_attr.is_global = 1;
	memcpy(ibv_ah_attr.grh.dgid.raw, gid, EFA_GID_LEN);
	efa_ah->ibv_ah = ibv_create_ah(ibv_pd, &ibv_ah_attr);
	if (!efa_ah->ibv_ah) {
		/* If the failure is because we have too many AH entries, try to
		 * evict an AH entry with no explicit AV entries and try AH
		 * creation again */
		if (errno == FI_ENOMEM) {
			EFA_INFO(
				FI_LOG_AV,
				"ibv_create_ah failed with ENOMEM for implicit "
				"AV insertion. Attempting to evict AH entry\n");

			err = efa_ah_implicit_av_evict_ah(domain);
			if (err)
				goto err_free_efa_ah;

			efa_ah->ibv_ah = ibv_create_ah(ibv_pd, &ibv_ah_attr);
			if (!efa_ah->ibv_ah) {
				EFA_WARN(FI_LOG_AV,
					 "ibv_create_ah failed for implicit AV "
					 "insertion! errno: %d\n",
					 errno);
				goto err_free_efa_ah;
			}
		} else {
			EFA_WARN(FI_LOG_AV,
				 "ibv_create_ah failed! errno: %d\n", errno);
			goto err_free_efa_ah;
		}
	}

	err = efadv_query_ah(efa_ah->ibv_ah, &efa_ah_attr, sizeof(efa_ah_attr));
	if (err) {
		errno = err;
		EFA_WARN(FI_LOG_AV, "efadv_query_ah failed! err: %d\n", err);
		goto err_destroy_ibv_ah;
	}

	dlist_init(&efa_ah->implicit_conn_list);
	dlist_insert_tail(&efa_ah->domain_lru_ah_list_entry, &domain->ah_lru_list);
	efa_ah->implicit_refcnt = 0;
	efa_ah->explicit_refcnt = 0;
	insert_implicit_av ? efa_ah->implicit_refcnt++ : efa_ah->explicit_refcnt++;
	efa_ah->ahn = efa_ah_attr.ahn;
	memcpy(efa_ah->gid, gid, EFA_GID_LEN);
	HASH_ADD(hh, domain->ah_map, gid, EFA_GID_LEN, efa_ah);
	ofi_genlock_unlock(&domain->util_domain.lock);
	return efa_ah;

err_destroy_ibv_ah:
	ibv_destroy_ah(efa_ah->ibv_ah);
err_free_efa_ah:
	free(efa_ah);
	ofi_genlock_unlock(&domain->util_domain.lock);
	return NULL;
}

void efa_ah_destroy_ah(struct efa_domain *domain, struct efa_ah *ah)
{
	int err;

	assert(ah->implicit_refcnt == 0 && ah->explicit_refcnt == 0);
	assert(dlist_empty(&ah->implicit_conn_list));

	EFA_INFO(FI_LOG_AV, "Destroying AH for ahn %d\n", ah->ahn);
	dlist_remove(&ah->domain_lru_ah_list_entry);
	HASH_DEL(domain->ah_map, ah);

	err = ibv_destroy_ah(ah->ibv_ah);
	if (err)
		EFA_WARN(FI_LOG_AV, "ibv_destroy_ah failed! err=%d\n", err);
	free(ah);
}

/**
 * @brief release an efa_ah object after acquiring the util domain lock
 *
 * @param[in]	domain	efa_domain
 * @param[in]	ah	efa_ah object pointer
 */
void efa_ah_release(struct efa_domain *domain, struct efa_ah *ah,
		    bool release_from_implicit_av)
{
	ofi_genlock_lock(&domain->util_domain.lock);
#if ENABLE_DEBUG
	struct efa_ah *tmp;

	HASH_FIND(hh, domain->ah_map, ah->gid, EFA_GID_LEN, tmp);
	assert(tmp == ah);
#endif
	assert((release_from_implicit_av && ah->implicit_refcnt > 0) ||
	       (!release_from_implicit_av && ah->explicit_refcnt > 0));

	release_from_implicit_av ? ah->implicit_refcnt-- :
				   ah->explicit_refcnt--;

	if (ah->implicit_refcnt == 0 && ah->explicit_refcnt == 0) {
		efa_ah_destroy_ah(domain, ah);
	}
	ofi_genlock_unlock(&domain->util_domain.lock);
}
