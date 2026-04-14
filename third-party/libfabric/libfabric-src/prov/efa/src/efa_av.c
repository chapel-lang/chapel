/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2016, Cisco Systems, Inc. All rights reserved. */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <malloc.h>
#include <stdio.h>

#include <infiniband/efadv.h>
#include <ofi_enosys.h>

#include "efa.h"
#include "efa_av.h"
#include "rdm/efa_rdm_pke_utils.h"

static void efa_conn_release(struct efa_av *av, struct efa_conn *conn,
			     bool release_from_implicit_av);

static void efa_conn_release_unsafe(struct efa_av *av, struct efa_conn *conn,
			     bool release_from_implicit_av);

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

static inline struct efa_conn *efa_av_addr_to_conn_impl(struct util_av *util_av,
							fi_addr_t fi_addr)
{
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *efa_av_entry;

	if (OFI_UNLIKELY(fi_addr == FI_ADDR_UNSPEC || fi_addr == FI_ADDR_NOTAVAIL))
		return NULL;

	if (OFI_LIKELY(ofi_bufpool_ibuf_is_valid(util_av->av_entry_pool, fi_addr)))
		util_av_entry = ofi_bufpool_get_ibuf(util_av->av_entry_pool, fi_addr);
	else
		return NULL;

	efa_av_entry = (struct efa_av_entry *)util_av_entry->data;
	return efa_av_entry->conn.ep_addr ? &efa_av_entry->conn : NULL;
}

/**
 * @brief find efa_conn struct using fi_addr in the explicit AV
 *
 * @param[in]	av	efa av
 * @param[in]	addr	fi_addr
 * @return	if address is valid, return pointer to efa_conn struct
 * 		otherwise, return NULL
 */
struct efa_conn *efa_av_addr_to_conn(struct efa_av *av, fi_addr_t fi_addr)
{
	return efa_av_addr_to_conn_impl(&av->util_av, fi_addr);
}

/**
 * @brief find efa_conn struct using fi_addr in the implicit AV
 *
 * @param[in]	av	efa av
 * @param[in]	addr	fi_addr
 * @return	if address is valid, return pointer to efa_conn struct
 * 		otherwise, return NULL
 */
struct efa_conn *efa_av_addr_to_conn_implicit(struct efa_av *av, fi_addr_t fi_addr)
{
	return efa_av_addr_to_conn_impl(&av->util_av_implicit, fi_addr);
}

/**
 * @brief find fi_addr for efa endpoint
 *
 * @param[in]	av	address vector
 * @param[in]	ahn	address handle number
 * @param[in]	qpn	QP number
 * @return	On success, return fi_addr to the peer who send the packet
 * 		If no such peer exist, return FI_ADDR_NOTAVAIL
 */
fi_addr_t efa_av_reverse_lookup(struct efa_av *av, uint16_t ahn, uint16_t qpn)
{
	struct efa_cur_reverse_av *cur_entry;
	struct efa_cur_reverse_av_key cur_key;

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = ahn;
	cur_key.qpn = qpn;
	HASH_FIND(hh, av->cur_reverse_av, &cur_key, sizeof(cur_key), cur_entry);

	return (OFI_LIKELY(!!cur_entry)) ? cur_entry->conn->fi_addr : FI_ADDR_NOTAVAIL;
}

static inline struct efa_conn *
efa_av_reverse_lookup_rdm_conn(struct efa_cur_reverse_av **cur_reverse_av,
			       struct efa_prv_reverse_av **prv_reverse_av,
			       uint16_t ahn, uint16_t qpn,
			       struct efa_rdm_pke *pkt_entry)
{
	uint32_t *connid;
	struct efa_cur_reverse_av *cur_entry;
	struct efa_prv_reverse_av *prv_entry;
	struct efa_cur_reverse_av_key cur_key;
	struct efa_prv_reverse_av_key prv_key;

	cur_key.ahn = ahn;
	cur_key.qpn = qpn;

	HASH_FIND(hh, *cur_reverse_av, &cur_key, sizeof(cur_key), cur_entry);

	if (OFI_UNLIKELY(!cur_entry))
		return NULL;

	if (!pkt_entry ||
	    (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_USER_RX_POOL)) {
		/**
		 * There is no packet entry to extract connid from when we get
		 * an IBV_WC_RECV_RDMA_WITH_IMM completion from rdma-core. Or
		 * the pkt_entry is allocated from a buffer user posted that
		 * doesn't expect any pkt hdr.
		 */
		return cur_entry->conn;
	}

	connid = efa_rdm_pke_connid_ptr(pkt_entry);
	if (!connid) {
		EFA_WARN_ONCE(FI_LOG_EP_CTRL,
			      "An incoming packet does NOT have connection ID "
			      "in its header.\n"
			      "This means the peer is using an older version "
			      "of libfabric.\n"
			      "The communication can continue but it is "
			      "encouraged to use\n"
			      "a newer version of libfabric\n");
		return cur_entry->conn;
	}

	if (OFI_LIKELY(*connid == cur_entry->conn->ep_addr->qkey))
		return cur_entry->conn;

	/* the packet is from a previous peer, look for its address from the
	 * prv_reverse_av */
	prv_key.ahn = ahn;
	prv_key.qpn = qpn;
	prv_key.connid = *connid;
	HASH_FIND(hh, *prv_reverse_av, &prv_key, sizeof(prv_key), prv_entry);

	return OFI_LIKELY(!!prv_entry) ? prv_entry->conn : NULL;
};

/**
 * @brief find fi_addr for rdm endpoint in the explicit AV
 *
 * @param[in]	av	address vector
 * @param[in]	ahn	address handle number
 * @param[in]	qpn	QP number
 * @param[in]   pkt_entry	NULL or rdm packet entry, used to extract connid
 * @return	On success, return fi_addr to the peer who send the packet
 * 		If no such peer exist, return FI_ADDR_NOTAVAIL
 */
fi_addr_t efa_av_reverse_lookup_rdm(struct efa_av *av, uint16_t ahn,
				    uint16_t qpn, struct efa_rdm_pke *pkt_entry)
{
	struct efa_conn *conn;

	conn = efa_av_reverse_lookup_rdm_conn(
		&av->cur_reverse_av, &av->prv_reverse_av, ahn, qpn, pkt_entry);

	if (OFI_LIKELY(!!conn))
		return conn->fi_addr;

	return FI_ADDR_NOTAVAIL;
}

/**
 * @brief find fi_addr for rdm endpoint in the implicit AV
 *
 * @param[in]	av	address vector
 * @param[in]	ahn	address handle number
 * @param[in]	qpn	QP number
 * @param[in]   pkt_entry	NULL or rdm packet entry, used to extract connid
 * @return	On success, return fi_addr to the peer who send the packet
 * 		If no such peer exist, return FI_ADDR_NOTAVAIL
 */
fi_addr_t efa_av_reverse_lookup_rdm_implicit(struct efa_av *av, uint16_t ahn,
					     uint16_t qpn,
					     struct efa_rdm_pke *pkt_entry)
{
	struct efa_conn *conn;

	conn = efa_av_reverse_lookup_rdm_conn(&av->cur_reverse_av_implicit,
					      &av->prv_reverse_av_implicit, ahn,
					      qpn, pkt_entry);

	if (OFI_LIKELY(!!conn)) {
		efa_av_implicit_av_lru_move(av, conn);
		return conn->implicit_fi_addr;
	}

	return FI_ADDR_NOTAVAIL;
}

static inline int efa_av_is_valid_address(struct efa_ep_addr *addr)
{
	struct efa_ep_addr all_zeros = { 0 };

	return memcmp(addr->raw, all_zeros.raw, sizeof(addr->raw));
}

/**
 * @brief allocate an ibv_ah object from GID.
 * This function use a hash map to store GID to ibv_ah map,
 * and re-use ibv_ah for same GID
 *
 * @param[in]	domain	efa_domain
 * @param[in]	gid	GID
 */
struct efa_ah *efa_ah_alloc(struct efa_domain *domain, const uint8_t *gid)
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
		efa_ah->refcnt += 1;
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
		EFA_WARN(FI_LOG_AV, "ibv_create_ah failed! errno: %d\n", errno);
		goto err_free_efa_ah;
	}

	err = efadv_query_ah(efa_ah->ibv_ah, &efa_ah_attr, sizeof(efa_ah_attr));
	if (err) {
		errno = err;
		EFA_WARN(FI_LOG_AV, "efadv_query_ah failed! err: %d\n", err);
		goto err_destroy_ibv_ah;
	}

	efa_ah->refcnt = 1;
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

/**
 * @brief release an efa_ah object
 *
 * @param[in]	domain	efa_domain
 * @param[in]	ah	efa_ah object pointer
 */
void efa_ah_release(struct efa_domain *domain, struct efa_ah *ah)
{
	int err;
	ofi_genlock_lock(&domain->util_domain.lock);
#if ENABLE_DEBUG
	struct efa_ah *tmp;

	HASH_FIND(hh, domain->ah_map, ah->gid, EFA_GID_LEN, tmp);
	assert(tmp == ah);
#endif
	assert(ah->refcnt > 0);
	ah->refcnt -= 1;
	if (ah->refcnt == 0) {
		EFA_INFO(FI_LOG_AV, "Destroying AH for ahn %d\n", ah->ahn);
		HASH_DEL(domain->ah_map, ah);
		err = ibv_destroy_ah(ah->ibv_ah);
		if (err)
			EFA_WARN(FI_LOG_AV, "ibv_destroy_ah failed! err=%d\n", err);
		free(ah);
	}
	ofi_genlock_unlock(&domain->util_domain.lock);
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
static
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
static void efa_conn_rdm_deinit(struct efa_av *av, struct efa_conn *conn)
{
	int err;
	struct efa_rdm_peer *peer;
	struct efa_rdm_ep *ep;
	struct dlist_entry *entry, *tmp;
	fi_addr_t fi_addr;
	struct efa_rdm_ep_peer_map_entry **peer_map;

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
	dlist_foreach_safe (&av->util_av.ep_list, entry, tmp) {
		ep = container_of(entry, struct efa_rdm_ep,
				  base_ep.util_ep.av_entry);

		if (conn->fi_addr != FI_ADDR_NOTAVAIL) {
			peer_map = &ep->fi_addr_to_peer_map;
			fi_addr = conn->fi_addr;
		} else {
			peer_map = &ep->fi_addr_to_peer_map_implicit;
			fi_addr = conn->implicit_fi_addr;
		}

		peer = efa_rdm_ep_peer_map_lookup(peer_map, fi_addr);
		if (peer) {
			efa_rdm_peer_destruct(peer, ep);
			efa_rdm_ep_peer_map_remove(peer_map, fi_addr);
		}
	}
}

/*
 * @brief Add newly insert address to the reverse AVs
 *
 * @param[in]		av		EFA AV object
 * @param[in,out]	cur_reverse_av	Reverse AV with AHN and QPN as key
 * @param[in,out]	prv_reverse_av	Reverse AV with AHN, QPN and QKEY as key
 * @param[in]		conn		efa_conn object
 * @return		On success, return 0.
 * 			Otherwise, return a negative libfabric error code
 */
int efa_av_reverse_av_add(struct efa_av *av,
				 struct efa_cur_reverse_av **cur_reverse_av,
				 struct efa_prv_reverse_av **prv_reverse_av,
				 struct efa_conn *conn)
{
	struct efa_cur_reverse_av *cur_entry;
	struct efa_prv_reverse_av *prv_entry;
	struct efa_cur_reverse_av_key cur_key;

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = conn->ah->ahn;
	cur_key.qpn = conn->ep_addr->qpn;
	cur_entry = NULL;

	HASH_FIND(hh, *cur_reverse_av, &cur_key, sizeof(cur_key), cur_entry);
	if (!cur_entry) {
		cur_entry = malloc(sizeof(*cur_entry));
		if (!cur_entry) {
			EFA_WARN(FI_LOG_AV, "Cannot allocate memory for cur_reverse_av entry\n");
			return -FI_ENOMEM;
		}

		cur_entry->key.ahn = cur_key.ahn;
		cur_entry->key.qpn = cur_key.qpn;
		cur_entry->conn = conn;
		HASH_ADD(hh, *cur_reverse_av, key, sizeof(cur_key), cur_entry);

		return 0;
	}

	/* We used a static connid for all dgram endpoints, therefore cur_entry should always be NULL,
	 * and only RDM endpoint can reach here. hence the following assertion
	 */
	assert(av->domain->info_type == EFA_INFO_RDM);
	prv_entry = malloc(sizeof(*prv_entry));
	if (!prv_entry) {
		EFA_WARN(FI_LOG_AV, "Cannot allocate memory for prv_reverse_av entry\n");
		return -FI_ENOMEM;
	}

	prv_entry->key.ahn = cur_key.ahn;
	prv_entry->key.qpn = cur_key.qpn;
	prv_entry->key.connid = cur_entry->conn->ep_addr->qkey;
	prv_entry->conn = cur_entry->conn;
	HASH_ADD(hh, *prv_reverse_av, key, sizeof(prv_entry->key), prv_entry);

	cur_entry->conn = conn;
	return 0;
}

/*
 * @brief Remove an address from the reverse AVs during fi_av_remove
 *
 * The address is not removed from the prv_reverse_av if it is found in
 * cur_reverse_av. Keeping the address in prv_reverse_av helps avoid QPN
 * collisions.
 *
 * @param[in]		av		EFA AV object
 * @param[in,out]	cur_reverse_av	Reverse AV with AHN and QPN as key
 * @param[in,out]	prv_reverse_av	Reverse AV with AHN, QPN and QKEY as key
 * @param[in]		conn		efa_conn object
 * @return		On success, return 0.
 * 			Otherwise, return a negative libfabric error code
 */
static void efa_av_reverse_av_remove(struct efa_cur_reverse_av **cur_reverse_av,
				    struct efa_prv_reverse_av **prv_reverse_av,
				    struct efa_conn *conn)
{
	struct efa_cur_reverse_av *cur_reverse_av_entry;
	struct efa_prv_reverse_av *prv_reverse_av_entry;
	struct efa_cur_reverse_av_key cur_key;
	struct efa_prv_reverse_av_key prv_key;

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = conn->ah->ahn;
	cur_key.qpn = conn->ep_addr->qpn;
	HASH_FIND(hh, *cur_reverse_av, &cur_key, sizeof(cur_key),
		  cur_reverse_av_entry);
	if (cur_reverse_av_entry) {
		HASH_DEL(*cur_reverse_av, cur_reverse_av_entry);
		free(cur_reverse_av_entry);
	} else {
		memset(&prv_key, 0, sizeof(prv_key));
		prv_key.ahn = conn->ah->ahn;
		prv_key.qpn = conn->ep_addr->qpn;
		prv_key.connid = conn->ep_addr->qkey;
		HASH_FIND(hh, *prv_reverse_av, &prv_key, sizeof(prv_key),
			  prv_reverse_av_entry);
		assert(prv_reverse_av_entry);
		HASH_DEL(*prv_reverse_av, prv_reverse_av_entry);
		free(prv_reverse_av_entry);
	}
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
	efa_conn_release_unsafe(av, conn_to_release, true);

	assert(HASH_CNT(hh, av->util_av_implicit.hash) == av->implicit_av_size);

out:
	dlist_insert_tail(&conn->implicit_av_lru_entry,
			  &av->implicit_av_lru_list);
	return FI_SUCCESS;
}

/**
 * @brief Move the conn to the end of the LRU list to indicate that it is the
 * most recently used entry
 *
 * @param[in]	av	efa address vector
 * @param[in]	conn	efa conn to be added to the LRU list
 */
void efa_av_implicit_av_lru_move(struct efa_av *av,
					struct efa_conn *conn)
{
	assert(av->implicit_av_size == 0 ||
	       HASH_CNT(hh, av->util_av_implicit.hash) <= av->implicit_av_size);
	assert(dlist_entry_in_list(&av->implicit_av_lru_list,
				   &conn->implicit_av_lru_entry));

	dlist_remove(&conn->implicit_av_lru_entry);
	dlist_insert_tail(&conn->implicit_av_lru_entry,
			  &av->implicit_av_lru_list);
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
static
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

	conn->ah = efa_ah_alloc(av->domain, raw_addr->raw);
	if (!conn->ah)
		goto err_release;

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
		efa_ah_release(av->domain, conn->ah);

	conn->ep_addr = NULL;
	err = ofi_av_remove_addr(util_av, fi_addr);
	if (err)
		EFA_WARN(FI_LOG_AV, "While processing previous failure, ofi_av_remove_addr failed! err=%d\n",
			 err);

	return NULL;
}

static inline void efa_conn_release_reverse_av(struct efa_av *av,
					       struct efa_conn *conn,
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

static inline void efa_conn_release_util_av(struct efa_av *av,
					    struct efa_conn *conn, bool release_from_implicit_av)
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

	efa_ah_release(av->domain, conn->ah);

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
static void efa_conn_release(struct efa_av *av, struct efa_conn *conn,
			     bool release_from_implicit_av)
{

	efa_conn_release_reverse_av(av, conn, release_from_implicit_av);

	if (av->domain->info_type == EFA_INFO_RDM) {
		ofi_genlock_lock(&av->domain->srx_lock);
		efa_conn_rdm_deinit(av, conn);
		ofi_genlock_unlock(&av->domain->srx_lock);
	}

	efa_conn_release_util_av(av, conn, release_from_implicit_av);

	release_from_implicit_av ? av->used_implicit-- : av->used_explicit--;
}

/**
 * @brief release an efa conn object
 * Caller of this function must obtain av->util_av.lock or
 * av->util_av_implicit.lock and the SRX lock. This function is called from the
 * CQ read path which already has the SRX lock.
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object pointer
 * @param[in]	release_from_implicit_av		whether to release conn
 * from implicit AV
 * @param[in]	grab_srx_lock		whether to get the SRX lock before
 * destroying the peer struct
 */
static void efa_conn_release_unsafe(struct efa_av *av, struct efa_conn *conn,
			     bool release_from_implicit_av)
{

	efa_conn_release_reverse_av(av, conn, release_from_implicit_av);

	if (av->domain->info_type == EFA_INFO_RDM)
		efa_conn_rdm_deinit(av, conn);

	efa_conn_release_util_av(av, conn, release_from_implicit_av);

	release_from_implicit_av ? av->used_implicit-- : av->used_explicit--;
}

static fi_addr_t
efa_av_get_addr_from_peer_rx_entry(struct fi_peer_rx_entry *rx_entry)
{
	struct efa_rdm_pke *pke;

	pke = (struct efa_rdm_pke *) rx_entry->peer_context;

	return pke->peer->conn->fi_addr;
}

static int efa_conn_implicit_to_explicit(struct efa_av *av,
					 struct efa_ep_addr *raw_addr,
					 fi_addr_t implicit_fi_addr,
					 fi_addr_t *fi_addr)
{
	int err;
	struct efa_ah *ah;
	fi_addr_t shm_fi_addr;
	struct efa_conn *conn;
	struct efa_rdm_ep *ep;
	struct efa_rdm_peer *peer;
	struct dlist_entry *entry;
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *implicit_av_entry, *explicit_av_entry;
	struct fid_peer_srx *peer_srx;

	EFA_INFO(FI_LOG_AV,
		 "Moving peer with implicit fi_addr %" PRIu64
		 " to explicit AV\n",
		 implicit_fi_addr);

	assert(ofi_genlock_held(&av->util_av.lock));
	assert(ofi_genlock_held(&av->util_av_implicit.lock));

	util_av_entry =
	ofi_bufpool_get_ibuf(av->util_av_implicit.av_entry_pool, implicit_fi_addr);

	implicit_av_entry = (struct efa_av_entry *) util_av_entry->data;

	assert(implicit_av_entry);
	assert(efa_is_same_addr(
		raw_addr, (struct efa_ep_addr *) implicit_av_entry->ep_addr));

	conn = &implicit_av_entry->conn;
	assert(conn->fi_addr == FI_ADDR_NOTAVAIL &&
	       conn->implicit_fi_addr == implicit_fi_addr);

	ah = conn->ah;
	assert(ah);
	shm_fi_addr = implicit_av_entry->conn.shm_fi_addr;

	efa_av_reverse_av_remove(&av->cur_reverse_av_implicit,
				 &av->prv_reverse_av_implicit, conn);

	err = ofi_av_remove_addr(&av->util_av_implicit, implicit_fi_addr);
	if (err) {
		EFA_WARN(FI_LOG_AV,
			 "ofi_av_remove_addr from implicit AV failed! Error "
			 "message: %s\n",
			 fi_strerror(err));
		return err;
	}

	dlist_remove(&implicit_av_entry->conn.implicit_av_lru_entry);

	av->used_implicit--;

	err = ofi_av_insert_addr(&av->util_av, raw_addr, fi_addr);
	if (err) {
		EFA_WARN(FI_LOG_AV,
			 "ofi_av_insert_addr into explicit AV failed! Error "
			 "message: %s\n",
			 fi_strerror(err));
		return err;
	}

	util_av_entry =
		ofi_bufpool_get_ibuf(av->util_av.av_entry_pool, *fi_addr);
	explicit_av_entry = (struct efa_av_entry *) util_av_entry->data;
	assert(efa_is_same_addr(
		raw_addr, (struct efa_ep_addr *) explicit_av_entry->ep_addr));

	conn = &explicit_av_entry->conn;
	memset(conn, 0, sizeof(*conn));
	conn->ep_addr = (struct efa_ep_addr *) explicit_av_entry->ep_addr;
	assert(av->type == FI_AV_TABLE);
	conn->ah = ah;
	conn->fi_addr = *fi_addr;
	conn->shm_fi_addr = shm_fi_addr;
	conn->implicit_fi_addr = FI_ADDR_NOTAVAIL;

	err = efa_av_reverse_av_add(av, &av->cur_reverse_av, &av->prv_reverse_av,
				    conn);
	if (err)
		return err;

	av->used_explicit++;

	EFA_INFO(FI_LOG_AV,
		 "Peer with implicit fi_addr %" PRIu64
		 " moved to explicit AV. Explicit fi_addr: %" PRIu64 "\n",
		 implicit_fi_addr, *fi_addr);

	/* Move peer from implicit peer map to explicit peer map for all
	 * endpoints. Also call foreach_unspec_addr to move unexpected messages
	 * from the unspecified queue to the specified queues
	 *
	 * util_ep is bound to the explicit util_av, so the explicit util_av's
	 * ep_list contains all of the endpoints bound to this AV */
	ofi_genlock_lock(&av->util_av.ep_list_lock);
	dlist_foreach(&av->util_av.ep_list, entry) {
		ep = container_of(entry, struct efa_rdm_ep, base_ep.util_ep.av_entry);
		peer = efa_rdm_ep_peer_map_lookup(&ep->fi_addr_to_peer_map_implicit, implicit_fi_addr);
		if (peer) {
			peer->conn = conn;
			EFA_INFO(FI_LOG_AV, "Moving peer from implicit to explicit peer map for endpoint %p\n", ep);
			efa_rdm_ep_peer_map_implicit_to_explicit(ep, peer, implicit_fi_addr, *fi_addr);
		}

		peer_srx = util_get_peer_srx(ep->peer_srx_ep);
		peer_srx->owner_ops->foreach_unspec_addr(peer_srx, &efa_av_get_addr_from_peer_rx_entry);
	}
	ofi_genlock_unlock(&av->util_av.ep_list_lock);

	return FI_SUCCESS;
}

/**
 * @brief insert one address into address vector (AV)
 *
 * @param[in]	av	address vector
 * @param[in]	addr	raw address, in the format of gid:qpn:qkey
 * @param[out]	fi_addr pointer to the output fi address. This address is used by fi_send
 * @param[in]	flags	flags user passed to fi_av_insert.
 * @param[in]	context	context user passed to fi_av_insert
 * @param[in]	insert_shm_av	whether insert address to shm av
 * @param[in]	insert_implicit_av	whether insert address to implicit AV
 * @return	0 on success, a negative error code on failure
 */
int efa_av_insert_one(struct efa_av *av, struct efa_ep_addr *addr,
		      fi_addr_t *fi_addr, uint64_t flags, void *context,
		      bool insert_shm_av, bool insert_implicit_av)
{
	struct efa_conn *conn;
	char raw_gid_str[INET6_ADDRSTRLEN];
	fi_addr_t efa_fiaddr;
	fi_addr_t implicit_fi_addr;
	int ret = 0;

	if (!efa_av_is_valid_address(addr)) {
		EFA_WARN(FI_LOG_AV, "Failed to insert bad addr\n");
		*fi_addr = FI_ADDR_NOTAVAIL;
		ret = -FI_EADDRNOTAVAIL;
		goto out;
	}

	if (av->domain->info_type == EFA_INFO_DGRAM)
		addr->qkey = EFA_DGRAM_CONNID;

	ofi_genlock_lock(&av->util_av_implicit.lock);
	ofi_genlock_lock(&av->util_av.lock);

	memset(raw_gid_str, 0, sizeof(raw_gid_str));
	if (!inet_ntop(AF_INET6, addr->raw, raw_gid_str, INET6_ADDRSTRLEN)) {
		EFA_WARN(FI_LOG_AV, "cannot convert address to string. errno: %d\n", errno);
		ret = -FI_EINVAL;
		*fi_addr = FI_ADDR_NOTAVAIL;
		goto out;
	}

	EFA_INFO(FI_LOG_AV,
		 "Inserting address GID[%s] QP[%u] QKEY[%u] to %s AV ....\n",
		 raw_gid_str, addr->qpn, addr->qkey,
		 insert_implicit_av ? "implicit" : "explicit");

	/*
	 * Check if this address already has been inserted, if so set *fi_addr
	 * to existing address, and return 0 for success.
	 */
	efa_fiaddr = ofi_av_lookup_fi_addr_unsafe(&av->util_av, addr);
	if (efa_fiaddr != FI_ADDR_NOTAVAIL) {
		/* We should never try to insert into the implicit AV an address
		 * that's already in the explicit AV */
		assert(!insert_implicit_av);

		EFA_INFO(FI_LOG_AV, "Found existing AV entry pointing to this address! fi_addr: %ld\n", efa_fiaddr);
		*fi_addr = efa_fiaddr;
		ret = 0;
		goto out;
	}

	implicit_fi_addr =
		ofi_av_lookup_fi_addr_unsafe(&av->util_av_implicit, addr);
	if (implicit_fi_addr != FI_ADDR_NOTAVAIL) {
		EFA_INFO(FI_LOG_AV,
			 "Found implicit AV entry id %ld for the same "
			 "address\n",
			 implicit_fi_addr);

		if (insert_implicit_av) {
			/* Move to the end of the LRU list */
			conn = efa_av_addr_to_conn_implicit(av,
							    implicit_fi_addr);
			efa_av_implicit_av_lru_move(av, conn);

			*fi_addr = implicit_fi_addr;
			goto out;
		}

		ret = efa_conn_implicit_to_explicit(av, addr, implicit_fi_addr,
						    fi_addr);
		if (ret)
			*fi_addr = FI_ADDR_NOTAVAIL;
		goto out;
	}

	conn = efa_conn_alloc(av, addr, flags, context, insert_shm_av, insert_implicit_av);
	if (!conn) {
		*fi_addr = FI_ADDR_NOTAVAIL;
		ret = -FI_EADDRNOTAVAIL;
		goto out;
	}

	if (insert_implicit_av) {
		*fi_addr = conn->implicit_fi_addr;
		EFA_INFO(FI_LOG_AV,
			 "Successfully inserted address GID[%s] QP[%u] "
			 "QKEY[%u] to implicit AV. fi_addr: %ld\n",
			 raw_gid_str, addr->qpn, addr->qkey, *fi_addr);
	} else {
		*fi_addr = conn->fi_addr;
		EFA_INFO(FI_LOG_AV,
			 "Successfully inserted address GID[%s] QP[%u] "
			 "QKEY[%u] to explicit AV. fi_addr: %ld\n",
			 raw_gid_str, addr->qpn, addr->qkey, *fi_addr);
	}
	ret = 0;

out:
	ofi_genlock_unlock(&av->util_av.lock);
	ofi_genlock_unlock(&av->util_av_implicit.lock);
	return ret;
}

int efa_av_insert(struct fid_av *av_fid, const void *addr,
			 size_t count, fi_addr_t *fi_addr,
			 uint64_t flags, void *context)
{
	struct efa_av *av = container_of(av_fid, struct efa_av, util_av.av_fid);
	int ret = 0, success_cnt = 0;
	size_t i = 0;
	struct efa_ep_addr *addr_i;
	fi_addr_t fi_addr_res;

	if (av->util_av.flags & FI_EVENT)
		return -FI_ENOEQ;

	if ((flags & FI_SYNC_ERR) && (!context || (flags & FI_EVENT)))
		return -FI_EINVAL;

	/*
	 * Providers are allowed to ignore FI_MORE.
	 */
	flags &= ~FI_MORE;
	if (flags)
		return -FI_ENOSYS;

	for (i = 0; i < count; i++) {
		addr_i = (struct efa_ep_addr *) ((uint8_t *)addr + i * EFA_EP_ADDR_LEN);

		ofi_genlock_lock(&av->domain->srx_lock);
		ret = efa_av_insert_one(av, addr_i, &fi_addr_res, flags, context, true, false);
		ofi_genlock_unlock(&av->domain->srx_lock);

		if (ret) {
			EFA_WARN(FI_LOG_AV, "insert raw_addr to av failed! ret=%d\n",
				 ret);
			break;
		}

		if (fi_addr)
			fi_addr[i] = fi_addr_res;
		success_cnt++;
	}

	/* cancel remaining request and log to event queue */
	for (; i < count ; i++) {
		if (fi_addr)
			fi_addr[i] = FI_ADDR_NOTAVAIL;
	}

	return success_cnt;
}

static int efa_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
			 void *addr, size_t *addrlen)
{
	struct efa_av *av = container_of(av_fid, struct efa_av, util_av.av_fid);
	struct efa_conn *conn = NULL;

	if (av->type != FI_AV_TABLE)
		return -FI_EINVAL;

	if (fi_addr == FI_ADDR_NOTAVAIL)
		return -FI_EINVAL;

	ofi_genlock_lock(&av->util_av.lock);
	conn = efa_av_addr_to_conn(av, fi_addr);
	ofi_genlock_unlock(&av->util_av.lock);
	if (!conn)
		return -FI_EINVAL;

	memcpy(addr, (void *)conn->ep_addr, MIN(EFA_EP_ADDR_LEN, *addrlen));
	if (*addrlen > EFA_EP_ADDR_LEN)
		*addrlen = EFA_EP_ADDR_LEN;
	return 0;
}

/*
 * @brief remove a set of addresses from AV and release its resources
 *
 * This function implements fi_av_remove() for EFA provider.
 *
 * Note that even after an address was removed from AV, it is still
 * possible to get TX and RX completion for the address. Per libfabric
 * standard, these completions should be ignored.
 *
 * To help TX completion handler to identify such a TX completion,
 * when removing an address, all its outstanding TX packet's addr
 * was set to FI_ADDR_NOTAVAIL. The TX completion handler will
 * ignore TX packet whose address is FI_ADDR_NOTAVAIL.
 *
 * Meanwhile, lower provider  will set a packet's address to
 * FI_ADDR_NOTAVAIL from it is from a removed address. RX completion
 * handler will ignore such packets.
 *
 * @param[in]	av_fid	fid of AV (address vector)
 * @param[in]	fi_addr pointer to an array of libfabric addresses
 * @param[in]	counter	number of libfabric addresses in the array
 * @param[in]	flags	flags
 * @return	0 if all addresses have been removed successfully,
 * 		negative libfabric error code if error was encoutnered.
 */
static int efa_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	int err = 0;
	size_t i;
	struct efa_av *av;
	struct efa_conn *conn;

	if (!fi_addr)
		return -FI_EINVAL;

	av = container_of(av_fid, struct efa_av, util_av.av_fid);
	if (av->type != FI_AV_TABLE)
		return -FI_EINVAL;

	ofi_genlock_lock(&av->util_av.lock);
	for (i = 0; i < count; i++) {
		conn = efa_av_addr_to_conn(av, fi_addr[i]);
		if (!conn) {
			err = -FI_EINVAL;
			break;
		}

		efa_conn_release(av, conn, false);
	}

	if (i < count) {
		/* something went wrong, so err cannot be zero */
		assert(err);
	}

	ofi_genlock_unlock(&av->util_av.lock);
	return err;
}

static const char *efa_av_straddr(struct fid_av *av_fid, const void *addr,
				  char *buf, size_t *len)
{
	return ofi_straddr(buf, len, FI_ADDR_EFA, addr);
}

static struct fi_ops_av efa_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = efa_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = efa_av_remove,
	.lookup = efa_av_lookup,
	.straddr = efa_av_straddr
};

static void efa_av_close_reverse_av(struct efa_av *av)
{
	struct efa_cur_reverse_av *cur_entry, *curtmp;
	struct efa_prv_reverse_av *prv_entry, *prvtmp;

	ofi_genlock_lock(&av->util_av.lock);

	HASH_ITER(hh, av->cur_reverse_av, cur_entry, curtmp) {
		efa_conn_release(av, cur_entry->conn, false);
	}

	HASH_ITER(hh, av->prv_reverse_av, prv_entry, prvtmp) {
		efa_conn_release(av, prv_entry->conn, false);
	}

	ofi_genlock_unlock(&av->util_av.lock);

	ofi_genlock_lock(&av->util_av_implicit.lock);

	HASH_ITER(hh, av->cur_reverse_av_implicit, cur_entry, curtmp) {
		efa_conn_release(av, cur_entry->conn, true);
	}

	HASH_ITER(hh, av->prv_reverse_av_implicit, prv_entry, prvtmp) {
		efa_conn_release(av, prv_entry->conn, true);
	}

	ofi_genlock_unlock(&av->util_av_implicit.lock);
}

static int efa_av_close(struct fid *fid)
{
	struct efa_av *av;
	int err = 0;
	struct efa_ep_addr_hashable *ep_addr_hashable, *tmp;

	av = container_of(fid, struct efa_av, util_av.av_fid.fid);

	efa_av_close_reverse_av(av);

	err = ofi_av_close(&av->util_av);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_AV, "Failed to close util av: %s\n",
			fi_strerror(err));
	}

	err = ofi_av_close(&av->util_av_implicit);
	if (OFI_UNLIKELY(err)) {
		EFA_WARN(FI_LOG_AV, "Failed to close implicit util av: %s\n",
			fi_strerror(err));
	}

	if (av->domain->info_type == EFA_INFO_RDM) {
		if (av->shm_rdm_av) {
			err = fi_close(&av->shm_rdm_av->fid);
			if (OFI_UNLIKELY(err)) {
				EFA_WARN(FI_LOG_AV, "Failed to close shm av: %s\n",
					fi_strerror(err));
			}
		}
	}

	HASH_ITER(hh, av->evicted_peers_hashset, ep_addr_hashable, tmp) {
		HASH_DEL(av->evicted_peers_hashset, ep_addr_hashable);
		free(ep_addr_hashable);
	}

	free(av);
	return err;
}

static struct fi_ops efa_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_av_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

/**
 * @brief initialize the util_av field in efa_av
 *
 * @param[in]	util_domain	util_domain which is part of efa_domain_base
 * @param[in]	attr		AV attr application passed to fi_av_open
 * @param[out]	util_av		util_av field in efa_av
 * @param[in]	context		contexted application passed to fi_av_open
 * @return	On success, return 0.
 *		On failure, return a negative libfabric error code.
 */
int efa_av_init_util_av(struct efa_domain *efa_domain,
			struct fi_av_attr *attr,
			struct util_av *util_av,
			void *context)
{
	struct util_av_attr util_attr;

	util_attr.addrlen = EFA_EP_ADDR_LEN;
	util_attr.context_len = sizeof(struct efa_av_entry) - EFA_EP_ADDR_LEN;
	util_attr.flags = 0;
	return ofi_av_init(&efa_domain->util_domain, attr, &util_attr,
			   util_av, context);
}

int efa_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av_fid, void *context)
{
	struct efa_domain *efa_domain;
	struct efa_av *av;
	struct fi_av_attr av_attr = { 0 };
	int ret, retv;
	size_t universe_size;

	if (!attr)
		return -FI_EINVAL;

	if (attr->name)
		return -FI_ENOSYS;

	/* FI_EVENT, FI_READ, and FI_SYMMETRIC are not supported */
	if (attr->flags)
		return -FI_ENOSYS;

	/*
	 * TODO: remove me once EFA RDM endpoint supports resizing members tied to the AV
	 * size.
	 */
	if (!attr->count)
		attr->count = EFA_MIN_AV_SIZE;
	else
		attr->count = MAX(attr->count, EFA_MIN_AV_SIZE);

	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;

	if (attr->type == FI_AV_MAP) {
		EFA_INFO(FI_LOG_AV, "FI_AV_MAP is deprecated in Libfabric 2.x. Please use FI_AV_TABLE. "
					"EFA provider will now switch to using FI_AV_TABLE.\n");
	}
	attr->type = FI_AV_TABLE;

	efa_domain = container_of(domain_fid, struct efa_domain, util_domain.domain_fid);

	if (fi_param_get_size_t(NULL, "universe_size",
				&universe_size) == FI_SUCCESS)
		attr->count = MAX(attr->count, universe_size);

	ret = efa_av_init_util_av(efa_domain, attr, &av->util_av_implicit, context);
	if (ret)
		goto err;

	ret = efa_av_init_util_av(efa_domain, attr, &av->util_av, context);
	if (ret)
		goto err_close_util_av_implicit;

	if (efa_domain->info_type == EFA_INFO_RDM && efa_domain->fabric &&
	    efa_domain->fabric->shm_fabric) {
		/*
		 * shm av supports maximum 256 entries
		 * Reset the count to 128 to reduce memory footprint and satisfy
		 * the need of the instances with more CPUs.
		 */
		av_attr = *attr;
		if (efa_env.shm_av_size > EFA_SHM_MAX_AV_COUNT) {
			ret = -FI_ENOSYS;
			EFA_WARN(FI_LOG_AV,
				 "The requested av size is beyond"
				 " shm supported maximum av size: %s\n",
				 fi_strerror(-ret));
			goto err_close_util_av;
		}
		av_attr.count = efa_env.shm_av_size;
		assert(av_attr.type == FI_AV_TABLE);
		ret = fi_av_open(efa_domain->shm_domain, &av_attr,
				 &av->shm_rdm_av, context);
		if (ret)
			goto err_close_util_av;
	}

	EFA_INFO(FI_LOG_AV, "fi_av_attr:%" PRId64 "\n",
			attr->flags);

	av->domain = efa_domain;
	av->type = attr->type;
	av->implicit_av_size = efa_env.implicit_av_size;
	av->used_implicit = 0;
	av->used_explicit = 0;
	av->shm_used = 0;

	*av_fid = &av->util_av.av_fid;
	(*av_fid)->fid.fclass = FI_CLASS_AV;
	(*av_fid)->fid.context = context;
	(*av_fid)->fid.ops = &efa_av_fi_ops;
	(*av_fid)->ops = &efa_av_ops;

	dlist_init(&av->implicit_av_lru_list);

	return 0;

err_close_util_av:
	retv = ofi_av_close(&av->util_av);
	if (retv)
		EFA_WARN(FI_LOG_AV,
			 "Unable to close util_av: %s\n", fi_strerror(-retv));

err_close_util_av_implicit:
	retv = ofi_av_close(&av->util_av_implicit);
	if (retv)
		EFA_WARN(FI_LOG_AV,
			 "Unable to close util_av_implicit: %s\n", fi_strerror(-retv));

err:
	free(av);
	return ret;
}
