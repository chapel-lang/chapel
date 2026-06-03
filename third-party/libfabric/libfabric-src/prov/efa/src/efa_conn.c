
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2016, Cisco Systems, Inc. All rights reserved. */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <infiniband/efadv.h>

#include "efa.h"

/*
 * Local/remote peer detection by comparing peer GID with stored local GIDs
 */
static bool efa_is_local_peer(struct efa_av *av, const void *addr)
{
	int i;
	uint8_t *raw_gid = ((struct efa_ep_addr *)addr)->raw;

#if ENABLE_DEBUG
	char raw_gid_str[INET6_ADDRSTRLEN] = { 0 };

	if (!inet_ntop(AF_INET6, raw_gid, raw_gid_str, INET6_ADDRSTRLEN)) {
		EFA_WARN(FI_LOG_AV, "Failed to get current EFA's GID, errno: %d\n", errno);
		return 0;
	}
	EFA_INFO(FI_LOG_AV, "The peer's GID is %s.\n", raw_gid_str);
#endif
	for (i = 0; i < g_efa_ibv_gid_cnt; ++i) {
		if (!memcmp(raw_gid, g_efa_ibv_gid_list[i].raw, EFA_GID_LEN)) {
			EFA_INFO(FI_LOG_AV, "The peer is local.\n");
			return 1;
		}
	}

	return 0;
}

/**
 * @brief Add the conn to the LRU list. If the list is full, evict the least
 * recently used entry at the front of the LRU list and add the latest one
 *
 * @param[in]	av	efa address vector
 * @param[in]	conn	efa conn to be added to the LRU list
 */
static inline int efa_av_implicit_av_lru_insert(struct efa_av *av,
						 struct efa_conn *conn)
{
	size_t cur_size;
	struct efa_ep_addr_hashable *ep_addr_hashable;
	struct efa_conn *conn_to_release;

	/* Implicit AV size of 0 means we allow the implicit AV to grow without
	 * bound */
	if (av->implicit_av_size == 0)
		goto out;

	cur_size = HASH_CNT(hh, av->util_av_implicit.hash);
	if (cur_size <= av->implicit_av_size)
		goto out;

	assert(ofi_genlock_held(&av->domain->srx_lock));

	dlist_pop_front(&av->implicit_av_lru_list, struct efa_conn,
			conn_to_release, implicit_av_lru_entry);
	EFA_INFO(FI_LOG_AV,
		 "Evicting AV entry for peer implicit fi_addr %" PRIu64
		 " AHN %" PRIu16 " QPN %" PRIu16 " QKEY %" PRIu32 " from "
		 "implicit AV\n",
		 conn_to_release->implicit_fi_addr, conn_to_release->ah->ahn,
		 conn_to_release->ep_addr->qpn, conn_to_release->ep_addr->qkey);

	/* Add to hashset with list of evicted peers */
	ep_addr_hashable = malloc(sizeof(struct efa_ep_addr_hashable));
	if (!ep_addr_hashable) {
		EFA_WARN(FI_LOG_AV, "Could not allocate memory for LRU AV entry hashset entry\n");
		return FI_ENOMEM;
	}
	memcpy(ep_addr_hashable, conn->ep_addr, sizeof(struct efa_ep_addr));
	HASH_ADD(hh, av->evicted_peers_hashset, addr, sizeof(struct efa_ep_addr), ep_addr_hashable);

	assert(ofi_genlock_held(&av->domain->srx_lock));
	efa_conn_release(av, conn_to_release, true);

	assert(HASH_CNT(hh, av->util_av_implicit.hash) == av->implicit_av_size);

out:
	dlist_insert_tail(&conn->implicit_av_lru_entry,
			  &av->implicit_av_lru_list);
	return FI_SUCCESS;
}

/**
 * @brief Insert the address into SHM provider's AV for RDM endpoints
 *
 * If shm transfer is enabled and the addr comes from local peer,
 *  1. convert addr to format 'gid_qpn', which will be set as shm's ep name later.
 *  2. insert gid_qpn into shm's av
 *  3. store returned fi_addr from shm into the hash table
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object
 * @return	On success return 0, otherwise return a negative error code
 */
int efa_conn_rdm_insert_shm_av(struct efa_av *av, struct efa_conn *conn)
{
	int err, ret;
	char smr_name[EFA_SHM_NAME_MAX];
	size_t smr_name_len;


	assert(av->domain->info_type == EFA_INFO_RDM);
	assert(conn->ep_addr);

	if (efa_is_local_peer(av, conn->ep_addr) && av->shm_rdm_av) {
		if (av->shm_used >= efa_env.shm_av_size) {
			EFA_WARN(FI_LOG_AV,
				 "Max number of shm AV entry (%d) has been reached.\n",
				 efa_env.shm_av_size);
			return -FI_ENOMEM;
		}

		smr_name_len = EFA_SHM_NAME_MAX;
		err = efa_shm_ep_name_construct(smr_name, &smr_name_len, conn->ep_addr);
		if (err != FI_SUCCESS) {
			EFA_WARN(FI_LOG_AV,
				 "efa_rdm_ep_efa_addr_to_str() failed! err=%d\n", err);
			return err;
		}

		/*
		 * The shm provider supports FI_AV_USER_ID flag. This flag
		 * associates a user-assigned identifier with each av entry that is
		 * returned with any completion entry in place of the AV's address.
		 * In the fi_av_insert call below, the &conn->shm_fi_addr is both an input
		 * and an output. conn->shm_fi_addr is passed in the function with value as
		 * conn->fi_addr, which is the address of peer in efa provider's av. shm
		 * records this value as user id in its internal hashmap for the use of cq
		 * write, and then overwrite conn->shm_fi_addr as the actual fi_addr in shm's
		 * av. The efa provider should still use conn->shm_fi_addr for transmissions
		 * through shm ep.
		 */
		conn->shm_fi_addr = conn->fi_addr;
		ret = fi_av_insert(av->shm_rdm_av, smr_name, 1, &conn->shm_fi_addr, FI_AV_USER_ID, NULL);
		if (OFI_UNLIKELY(ret != 1)) {
			EFA_WARN(FI_LOG_AV,
				 "Failed to insert address to shm provider's av: %s\n",
				 fi_strerror(-ret));
			return ret;
		}

		EFA_INFO(FI_LOG_AV,
			"Successfully inserted %s to shm provider's av. efa_fiaddr: %ld shm_fiaddr = %ld\n",
			smr_name, conn->fi_addr, conn->shm_fi_addr);

		assert(conn->shm_fi_addr < efa_env.shm_av_size);
		av->shm_used++;
	}

	return 0;
}

/**
 * @brief release the rdm related resources of an efa_conn object. This function
 * requires the caller to take the SRX lock because this function modifies the
 * peer map and destroys peers which are accessed and modified in the CQ read
 * path.
 *
 * this function release the shm av entry and rdm peer;
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object
 * peer
 */
void efa_conn_rdm_deinit(struct efa_av *av, struct efa_conn *conn)
{
	int err;
	struct efa_conn_ep_peer_map_entry *peer_map_entry, *tmp;

	assert(av->domain->info_type == EFA_INFO_RDM);

	assert((conn->fi_addr != FI_ADDR_NOTAVAIL &&
		conn->implicit_fi_addr == FI_ADDR_NOTAVAIL) ||
	       (conn->implicit_fi_addr != FI_ADDR_NOTAVAIL &&
		conn->fi_addr == FI_ADDR_NOTAVAIL));

	if (conn->shm_fi_addr != FI_ADDR_NOTAVAIL && av->shm_rdm_av) {
		err = fi_av_remove(av->shm_rdm_av, &conn->shm_fi_addr, 1, 0);
		if (err) {
			EFA_WARN(FI_LOG_AV,
				 "remove address from shm av failed! err=%d\n",
				 err);
		} else {
			av->shm_used--;
			assert(conn->shm_fi_addr < efa_env.shm_av_size);
		}
	}

	assert(ofi_genlock_held(&av->domain->srx_lock));
	HASH_ITER(hh, conn->ep_peer_map, peer_map_entry, tmp) {
		dlist_remove(&peer_map_entry->peer.ep_peer_list_entry);
		efa_rdm_peer_destruct(&peer_map_entry->peer, peer_map_entry->ep_ptr);
		HASH_DEL(conn->ep_peer_map, peer_map_entry);
		ofi_buf_free(peer_map_entry);
	}
	assert(HASH_CNT(hh, conn->ep_peer_map) == 0);
}

/**
 * @brief allocate an efa_conn object
 * caller of this function must obtain av->util_av.lock or av->util_av_implicit.lock
 *
 * @param[in]	av		efa address vector
 * @param[in]	raw_addr	raw efa address
 * @param[in]	flags		flags application passed to fi_av_insert
 * @param[in]	context		context application passed to fi_av_insert
 * @param[in]	insert_shm_av	whether insert address to shm av
 * @param[in]	insert_implicit_av	whether insert address to implicit AV
 * @return	on success, return a pointer to an efa_conn object
 *		otherwise, return NULL. errno will be set to a positive error code.
 */
struct efa_conn *efa_conn_alloc(struct efa_av *av, struct efa_ep_addr *raw_addr,
				uint64_t flags, void *context, bool insert_shm_av, bool insert_implicit_av)
{
	struct util_av *util_av;
	struct efa_cur_reverse_av **cur_reverse_av;
	struct efa_prv_reverse_av **prv_reverse_av;
	struct util_av_entry *util_av_entry = NULL;
	struct efa_av_entry *efa_av_entry = NULL;
	struct efa_conn *conn;
	fi_addr_t fi_addr;
	int err;

	if (flags & FI_SYNC_ERR)
		memset(context, 0, sizeof(int));

	if (insert_implicit_av) {
		assert(ofi_genlock_held(&av->util_av_implicit.lock));
		util_av = &av->util_av_implicit;
		cur_reverse_av = &av->cur_reverse_av_implicit;
		prv_reverse_av = &av->prv_reverse_av_implicit;
	} else {
		assert(ofi_genlock_held(&av->util_av.lock));
		util_av = &av->util_av;
		cur_reverse_av = &av->cur_reverse_av;
		prv_reverse_av = &av->prv_reverse_av;
	}

	err = ofi_av_insert_addr(util_av, raw_addr, &fi_addr);
	if (err) {
		EFA_WARN(FI_LOG_AV, "ofi_av_insert_addr failed! Error message: %s\n",
			 fi_strerror(err));
		return NULL;
	}

	util_av_entry = ofi_bufpool_get_ibuf(util_av->av_entry_pool,
					     fi_addr);
	efa_av_entry = (struct efa_av_entry *)util_av_entry->data;
	assert(efa_is_same_addr(raw_addr, (struct efa_ep_addr *)efa_av_entry->ep_addr));

	conn = &efa_av_entry->conn;
	memset(conn, 0, sizeof(*conn));
	conn->ep_addr = (struct efa_ep_addr *)efa_av_entry->ep_addr;
	assert(av->type == FI_AV_TABLE);

	conn->av = av;

	if (insert_implicit_av) {
		conn->fi_addr = FI_ADDR_NOTAVAIL;
		conn->implicit_fi_addr = fi_addr;
		err = efa_av_implicit_av_lru_insert(av, conn);
		if (err)
			return NULL;
	} else {
		conn->fi_addr = fi_addr;
		conn->implicit_fi_addr = FI_ADDR_NOTAVAIL;
	}

	conn->ah = efa_ah_alloc(av->domain, raw_addr->raw, insert_implicit_av);
	if (!conn->ah)
		goto err_release;

	if (insert_implicit_av)
		dlist_insert_tail(&conn->ah_implicit_conn_list_entry,
				  &conn->ah->implicit_conn_list);

	conn->shm_fi_addr = FI_ADDR_NOTAVAIL;
	/*
	 * The efa_conn_alloc() call can be made in two situations:
	 * 1. application calls fi_av_insert API
	 * 2. efa progress engine get a message from unknown peer through efa device,
	 *    which means peer is not local or shm is disabled for transmission.
	 * For situation 1, the shm av insertion should happen when the peer is local (insert_shm_av=1)
	 * For situation 2, the shm av insertion shouldn't happen anyway (insert_shm_av=0).
	 */
	if (av->domain->info_type == EFA_INFO_RDM && insert_shm_av) {
		err = efa_conn_rdm_insert_shm_av(av, conn);
		if (err) {
			errno = -err;
			goto err_release;
		}
	}

	err = efa_av_reverse_av_add(av, cur_reverse_av, prv_reverse_av, conn);
	if (err) {
		if (av->domain->info_type == EFA_INFO_RDM) {
			/* insert_implicit_av is only true for the CQ read path
			 * which already has the SRX lock */
			if (insert_implicit_av)
				ofi_genlock_lock(&av->domain->srx_lock);
			efa_conn_rdm_deinit(av, conn);
			if (insert_implicit_av)
				ofi_genlock_unlock(&av->domain->srx_lock);
		}
		goto err_release;
	}

	insert_implicit_av ? av->used_implicit++ : av->used_explicit++;

	return conn;

err_release:
	if (conn->ah)
		efa_ah_release(av->domain, conn->ah, insert_implicit_av);

	conn->ep_addr = NULL;
	err = ofi_av_remove_addr(util_av, fi_addr);
	if (err)
		EFA_WARN(FI_LOG_AV, "While processing previous failure, ofi_av_remove_addr failed! err=%d\n",
			 err);

	return NULL;
}

void efa_conn_release_reverse_av(struct efa_av *av, struct efa_conn *conn,
				 bool release_from_implicit_av)
{
	if (release_from_implicit_av) {
		assert(ofi_genlock_held(&av->util_av_implicit.lock));
		efa_av_reverse_av_remove(&av->cur_reverse_av_implicit,
					 &av->prv_reverse_av_implicit, conn);
	} else {
		assert(ofi_genlock_held(&av->util_av.lock));
		efa_av_reverse_av_remove(&av->cur_reverse_av,
					 &av->prv_reverse_av, conn);
	}
}

void efa_conn_release_util_av(struct efa_av *av, struct efa_conn *conn,
			      bool release_from_implicit_av)
{
	struct util_av *util_av;
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *efa_av_entry;
	char gidstr[INET6_ADDRSTRLEN];
	fi_addr_t fi_addr;
	int err;

	if (release_from_implicit_av) {
		assert(ofi_genlock_held(&av->util_av_implicit.lock));
		util_av = &av->util_av_implicit;
		fi_addr = conn->implicit_fi_addr;
	} else {
		assert(ofi_genlock_held(&av->util_av.lock));
		util_av = &av->util_av;
		fi_addr = conn->fi_addr;
	}

	util_av_entry = ofi_bufpool_get_ibuf(util_av->av_entry_pool, fi_addr);
	assert(util_av_entry);
	efa_av_entry = (struct efa_av_entry *) util_av_entry->data;

	err = ofi_av_remove_addr(util_av, fi_addr);
	if (err) {
		EFA_WARN(FI_LOG_AV, "ofi_av_remove_addr failed! err=%d\n", err);
	}

	inet_ntop(AF_INET6, conn->ep_addr->raw, gidstr, INET6_ADDRSTRLEN);
	EFA_INFO(FI_LOG_AV, "efa_conn released! conn[%p] GID[%s] QP[%u]\n",
		 conn, gidstr, conn->ep_addr->qpn);

	conn->ep_addr = NULL;
	memset(efa_av_entry->ep_addr, 0, EFA_EP_ADDR_LEN);
}

/**
 * @brief release an efa conn object
 * Caller of this function must obtain av->util_av.lock or
 * av->util_av_implicit.lock. This function obtains the SRX lock and is called
 * from the AV removal path.
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object pointer
 * @param[in]	release_from_implicit_av		whether to release conn
 * from implicit AV
 * @param[in]	grab_srx_lock		whether to get the SRX lock before
 * destroying the peer struct
 */
void efa_conn_release(struct efa_av *av, struct efa_conn *conn,
		      bool release_from_implicit_av)
{
	assert(av->domain->info_type != EFA_INFO_RDM ||
	       ofi_genlock_held(&av->domain->srx_lock));

	efa_conn_release_reverse_av(av, conn, release_from_implicit_av);
	if (av->domain->info_type == EFA_INFO_RDM)
		efa_conn_rdm_deinit(av, conn);

	if (release_from_implicit_av)
		dlist_remove(&conn->ah_implicit_conn_list_entry);

	efa_ah_release(av->domain, conn->ah, release_from_implicit_av);

	efa_conn_release_util_av(av, conn, release_from_implicit_av);

	release_from_implicit_av ? av->used_implicit-- : av->used_explicit--;
}

/**
 * @brief release an efa conn object
 * Caller of this function must obtain av->util_av.lock or
 * av->util_av_implicit.lock and the SRX lock. It also calls
 * efa_ah_release_unsafe which does not acquire the util_domain lock the
 * protects the AH map. This function is called when evicting an AH entry in the
 * CQ read path which already has the SRX lock and the util_domain lock.
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object pointer
 * @param[in]	release_from_implicit_av		whether to release conn
 * from implicit AV
 * @param[in]	grab_srx_lock		whether to get the SRX lock before
 * destroying the peer struct
 */
void efa_conn_release_ah_unsafe(struct efa_av *av, struct efa_conn *conn,
				bool release_from_implicit_av)
{
	assert(av->domain->info_type != EFA_INFO_RDM ||
	       ofi_genlock_held(&av->domain->srx_lock));

	assert(ofi_genlock_held(&av->domain->util_domain.lock));

	efa_conn_release_reverse_av(av, conn, release_from_implicit_av);
	if (av->domain->info_type == EFA_INFO_RDM)
		efa_conn_rdm_deinit(av, conn);

	if (release_from_implicit_av)
		dlist_remove(&conn->ah_implicit_conn_list_entry);

	efa_conn_release_util_av(av, conn, release_from_implicit_av);

	release_from_implicit_av ? conn->ah->implicit_refcnt-- :
				   conn->ah->explicit_refcnt--;
	release_from_implicit_av ? av->used_implicit-- : av->used_explicit--;
}

void efa_conn_ep_peer_map_insert(struct efa_conn *conn, struct efa_conn_ep_peer_map_entry *map_entry)
{
	HASH_ADD_PTR(conn->ep_peer_map, ep_ptr, map_entry);
}

struct efa_rdm_peer *efa_conn_ep_peer_map_lookup(struct efa_conn *conn,
						 struct efa_rdm_ep *ep)
{
	struct efa_conn_ep_peer_map_entry *map_entry;

	HASH_FIND_PTR(conn->ep_peer_map, &ep, map_entry);

	return map_entry ? &map_entry->peer : NULL;
}

void efa_conn_ep_peer_map_remove(struct efa_conn *conn, struct efa_rdm_ep *ep)
{
	struct efa_conn_ep_peer_map_entry *map_entry;

	HASH_FIND_PTR(conn->ep_peer_map, &ep, map_entry);
	assert(map_entry);
	HASH_DELETE(hh, conn->ep_peer_map, map_entry);
	ofi_buf_free(map_entry);
}
