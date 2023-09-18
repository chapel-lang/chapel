/*
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017-2022 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include <malloc.h>
#include <stdio.h>

#include <infiniband/efadv.h>
#include <ofi_enosys.h>

#include "efa.h"
#include "rxr.h"
#include "rxr_pkt_type_base.h"

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
	for (i = 0; i < g_device_cnt; ++i) {
		if (!memcmp(raw_gid, g_device_list[i].ibv_gid.raw, EFA_GID_LEN)) {
			EFA_INFO(FI_LOG_AV, "The peer is local.\n");
			return 1;
		}
	}

	return 0;
}

static bool efa_is_same_addr(struct efa_ep_addr *lhs, struct efa_ep_addr *rhs)
{
	return !memcmp(lhs->raw, rhs->raw, sizeof(lhs->raw)) &&
	       lhs->qpn == rhs->qpn && lhs->qkey == rhs->qkey;
}

/**
 * @brief initialize a rdm peer
 *
 * @param[in,out]	peer	rdm peer
 * @param[in]		ep	rdm endpoint
 * @param[in]		conn	efa conn object
 */
static inline
void efa_rdm_peer_init(struct rdm_peer *peer, struct rxr_ep *ep, struct efa_conn *conn)
{
	memset(peer, 0, sizeof(struct rdm_peer));

	peer->efa_fiaddr = conn->fi_addr;
	peer->is_self = efa_is_same_addr((struct efa_ep_addr *)ep->core_addr,
					 conn->ep_addr);
	peer->num_read_msg_in_flight = 0;
	peer->num_runt_bytes_in_flight = 0;
	ofi_recvwin_buf_alloc(&peer->robuf, rxr_env.recvwin_size);
	dlist_init(&peer->outstanding_tx_pkts);
	dlist_init(&peer->rx_unexp_list);
	dlist_init(&peer->rx_unexp_tagged_list);
	dlist_init(&peer->tx_entry_list);
	dlist_init(&peer->rx_entry_list);
}

/**
 * @brief clear resources accociated with a peer
 *
 * release reorder buffer, tx_entry list and rx_entry list of a peer
 *
 * @param[in,out]	peer 	rdm peer
 */
void efa_rdm_peer_clear(struct rxr_ep *ep, struct rdm_peer *peer)
{
	struct dlist_entry *tmp;
	struct rxr_op_entry *tx_entry;
	struct rxr_op_entry *rx_entry;
	struct rxr_pkt_entry *pkt_entry;
	/*
	 * TODO: Add support for wait/signal until all pending messages have
	 * been sent/received so we do not attempt to complete a data transfer
	 * or internal transfer after the EP is shutdown.
	 */
	if ((peer->flags & RXR_PEER_REQ_SENT) &&
	    !(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED))
		FI_WARN_ONCE(&rxr_prov, FI_LOG_EP_CTRL, "Closing EP with unacked CONNREQs in flight\n");

	if (peer->robuf.pending)
		ofi_recvwin_free(&peer->robuf);

	if (!ep) {
		/* ep is NULL means the endpoint has been closed.
		 * In this case there is no need to proceed because
		 * all the tx_entry, rx_entry, pkt_entry has been released.
		 */
		return;
	}

	/* we cannot release outstanding TX packets because device
	 * will report completion of these packets later. Setting
	 * the address to FI_ADDR_NOTAVAIL, so rxr_ep_get_peer()
	 * will return NULL for the address, so the completion will
	 * be ignored.
	 */
	dlist_foreach_container(&peer->outstanding_tx_pkts,
				struct rxr_pkt_entry,
				pkt_entry, entry) {
		pkt_entry->addr = FI_ADDR_NOTAVAIL;
	}

	dlist_foreach_container_safe(&peer->tx_entry_list,
				     struct rxr_op_entry,
				     tx_entry, peer_entry, tmp) {
		rxr_release_tx_entry(ep, tx_entry);
	}

	dlist_foreach_container_safe(&peer->rx_entry_list,
				     struct rxr_op_entry,
				     rx_entry, peer_entry, tmp) {
		rxr_release_rx_entry(ep, rx_entry);
	}

	if (peer->flags & RXR_PEER_HANDSHAKE_QUEUED)
		dlist_remove(&peer->handshake_queued_entry);

	if (peer->flags & RXR_PEER_IN_BACKOFF)
		dlist_remove(&peer->rnr_backoff_entry);

#ifdef ENABLE_EFA_POISONING
	rxr_poison_mem_region((uint32_t *)peer, sizeof(struct rdm_peer));
#endif
}

/**
 * @brief find efa_conn struct using fi_addr
 *
 * @param[in]	av	efa av
 * @param[in]	addr	fi_addr
 * @return	if address is valid, return pointer to efa_conn struct
 * 		otherwise, return NULL
 */
struct efa_conn *efa_av_addr_to_conn(struct efa_av *av, fi_addr_t fi_addr)
{
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *efa_av_entry;

	if (OFI_UNLIKELY(fi_addr == FI_ADDR_UNSPEC))
		return NULL;

	if (av->type == FI_AV_MAP) {
		return (struct efa_conn *)fi_addr;
	}

	assert(av->type == FI_AV_TABLE);
	util_av_entry = ofi_bufpool_get_ibuf(av->util_av.av_entry_pool, fi_addr);
	if (!util_av_entry)
		return NULL;

	efa_av_entry = (struct efa_av_entry *)util_av_entry->data;
	return efa_av_entry->conn.ep_addr ? &efa_av_entry->conn : NULL;
}

/**
 * @brief find fi_addr for dgram endpoint
 *
 * @param[in]	av	address vector
 * @param[in]	ahn	address handle number
 * @param[in]	qpn	QP number
 * @return	On success, return fi_addr to the peer who send the packet
 * 		If no such peer exist, return FI_ADDR_NOTAVAIL
 */
fi_addr_t efa_av_reverse_lookup_dgram(struct efa_av *av, uint16_t ahn, uint16_t qpn)
{
	struct efa_cur_reverse_av *cur_entry;
	struct efa_cur_reverse_av_key cur_key;

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = ahn;
	cur_key.qpn = qpn;
	HASH_FIND(hh, av->cur_reverse_av, &cur_key, sizeof(cur_key), cur_entry);

	return (OFI_LIKELY(!!cur_entry)) ? cur_entry->conn->fi_addr : FI_ADDR_NOTAVAIL;
}

/**
 * @brief find fi_addr for rdm endpoint
 *
 * @param[in]	av	address vector
 * @param[in]	ahn	address handle number
 * @param[in]	qpn	QP number
 * @param[in]   pkt_entry	rdm packet entry, used to extract connid
 * @return	On success, return fi_addr to the peer who send the packet
 * 		If no such peer exist, return FI_ADDR_NOTAVAIL
 */
fi_addr_t efa_av_reverse_lookup_rdm(struct efa_av *av, uint16_t ahn, uint16_t qpn, struct rxr_pkt_entry *pkt_entry)
{
	struct efa_cur_reverse_av *cur_entry;
	struct efa_prv_reverse_av *prv_entry;
	struct efa_cur_reverse_av_key cur_key;
	struct efa_prv_reverse_av_key prv_key;
	uint32_t *connid;

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = ahn;
	cur_key.qpn = qpn;

	HASH_FIND(hh, av->cur_reverse_av, &cur_key, sizeof(cur_key), cur_entry);

	if (OFI_UNLIKELY(!cur_entry))
		return FI_ADDR_NOTAVAIL;

	connid = rxr_pkt_connid_ptr(pkt_entry);
	if (!connid) {
		FI_WARN_ONCE(&rxr_prov, FI_LOG_EP_CTRL,
			     "An incoming packet does NOT have connection ID in its header.\n"
			     "This means the peer is using an older version of libfabric.\n"
			     "The communication can continue but it is encouraged to use\n"
			     "a newer version of libfabric\n");
		return cur_entry->conn->fi_addr;
	}

	if (OFI_LIKELY(*connid == cur_entry->conn->ep_addr->qkey))
		return cur_entry->conn->fi_addr;

	/* the packet is from a previous peer, look for its address from the prv_reverse_av */
	memset(&prv_key, 0, sizeof(prv_key));
	prv_key.ahn = ahn;
	prv_key.qpn = qpn;
	prv_key.connid = *connid;
	HASH_FIND(hh, av->prv_reverse_av, &prv_key, sizeof(prv_key), prv_entry);

	return OFI_LIKELY(!!prv_entry) ? prv_entry->conn->fi_addr : FI_ADDR_NOTAVAIL;
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
 * @param[in]	av	address vector
 * @param[in]	gid	GID
 */
static
struct efa_ah *efa_ah_alloc(struct efa_av *av, const uint8_t *gid)
{
	struct ibv_pd *ibv_pd = av->domain->ibv_pd;
	struct efa_ah *efa_ah;
	struct ibv_ah_attr ibv_ah_attr = { 0 };
	struct efadv_ah_attr efa_ah_attr = { 0 };
	int err;

	efa_ah = NULL;
	HASH_FIND(hh, av->ah_map, gid, EFA_GID_LEN, efa_ah);
	if (efa_ah) {
		efa_ah->refcnt += 1;
		return efa_ah;
	}

	efa_ah = malloc(sizeof(struct efa_ah));
	if (!efa_ah) {
		errno = FI_ENOMEM;
		EFA_WARN(FI_LOG_AV, "cannot allocate memory for efa_ah");
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
	HASH_ADD(hh, av->ah_map, gid, EFA_GID_LEN, efa_ah);
	return efa_ah;

err_destroy_ibv_ah:
	ibv_destroy_ah(efa_ah->ibv_ah);
err_free_efa_ah:
	free(efa_ah);
	return NULL;
}

/**
 * @brief release an efa_ah object
 *
 * @param[in]	av	address vector
 * @param[in]	ah	efa_ah object pointer
 */
static
void efa_ah_release(struct efa_av *av, struct efa_ah *ah)
{
	int err;
#if ENABLE_DEBUG
	struct efa_ah *tmp;

	HASH_FIND(hh, av->ah_map, ah->gid, EFA_GID_LEN, tmp);
	assert(tmp == ah);
#endif
	assert(ah->refcnt > 0);
	ah->refcnt -= 1;
	if (ah->refcnt == 0) {
		HASH_DEL(av->ah_map, ah);
		err = ibv_destroy_ah(ah->ibv_ah);
		if (err)
			EFA_WARN(FI_LOG_AV, "ibv_destroy_ah failed! err=%d\n", err);
		free(ah);
	}
}

static
void efa_conn_release(struct efa_av *av, struct efa_conn *conn);

/**
 * @brief initialize the rdm related resources of an efa_conn object
 *
 * This function setup rdm_peer and shm address for an efa_conn.
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
int efa_conn_rdm_init(struct efa_av *av, struct efa_conn *conn)
{
	int err, ret;
	char smr_name[EFA_SHM_NAME_MAX];
	size_t smr_name_len;
	struct rxr_ep *rxr_ep;
	struct rdm_peer *peer;

	assert(av->ep_type == FI_EP_RDM);
	assert(conn->ep_addr);

	/* currently multiple EP bind to same av is not supported */
	assert(!dlist_empty(&av->util_av.ep_list));
	rxr_ep = container_of(av->util_av.ep_list.next, struct rxr_ep, util_ep.av_entry);

	peer = &conn->rdm_peer;
	efa_rdm_peer_init(peer, rxr_ep, conn);

	/* If peer is local, insert the address into shm provider's av */
	if (efa_is_local_peer(av, conn->ep_addr) && av->shm_rdm_av) {
		if (av->shm_used >= rxr_env.shm_av_size) {
			EFA_WARN(FI_LOG_AV,
				 "Max number of shm AV entry (%d) has been reached.\n",
				 rxr_env.shm_av_size);
			return -FI_ENOMEM;
		}

		smr_name_len = EFA_SHM_NAME_MAX;
		err = efa_shm_ep_name_construct(smr_name, &smr_name_len, conn->ep_addr);
		if (err != FI_SUCCESS) {
			EFA_WARN(FI_LOG_AV,
				 "rxr_ep_efa_addr_to_str() failed! err=%d\n", err);
			return err;
		}

		ret = fi_av_insert(av->shm_rdm_av, smr_name, 1, &peer->shm_fiaddr, 0, NULL);
		if (OFI_UNLIKELY(ret != 1)) {
			EFA_WARN(FI_LOG_AV,
				 "Failed to insert address to shm provider's av: %s\n",
				 fi_strerror(-ret));
			return ret;
		}

		EFA_INFO(FI_LOG_AV,
			"Successfully inserted %s to shm provider's av. efa_fiaddr: %ld shm_fiaddr = %ld\n",
			smr_name, conn->fi_addr, peer->shm_fiaddr);

		assert(peer->shm_fiaddr < rxr_env.shm_av_size);
		av->shm_used++;
		av->shm_rdm_addr_map[peer->shm_fiaddr] = conn->fi_addr;
		peer->is_local = 1;
	}

	return 0;
}

/**
 * @brief release the rdm related resources of an efa_conn object
 *
 * this function release the shm av entry and rdm peer;
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object
 */
static
void efa_conn_rdm_deinit(struct efa_av *av, struct efa_conn *conn)
{
	int err;
	struct rdm_peer *peer;
	struct rxr_ep *ep;

	assert(av->ep_type == FI_EP_RDM);

	peer = &conn->rdm_peer;
	if (peer->is_local && av->shm_rdm_av) {
		err = fi_av_remove(av->shm_rdm_av, &peer->shm_fiaddr, 1, 0);
		if (err) {
			EFA_WARN(FI_LOG_AV, "remove address from shm av failed! err=%d\n", err);
		} else {
			av->shm_used--;
			assert(peer->shm_fiaddr < rxr_env.shm_av_size);
			av->shm_rdm_addr_map[peer->shm_fiaddr] = FI_ADDR_UNSPEC;
		}
	}

	/*
	 * We need peer->shm_fiaddr to remove shm address from shm av table,
	 * so efa_rdm_peer_clear must be after removing shm av table.
	 */
	ep = dlist_empty(&av->util_av.ep_list) ? NULL : container_of(av->util_av.ep_list.next, struct rxr_ep, util_ep.av_entry);
	efa_rdm_peer_clear(ep, peer);
}

/*
 * @brief update reverse_av when inserting an new address to AV
 *
 * @param[in,out]	av		efa AV
 * @param[in]		raw_addr	raw address
 * @param[in]		conn		efa_conn object
 * @return		On success, return 0.
 * 			Otherwise, return a negative libfabric error code
 */
static
int efa_av_update_reverse_av(struct efa_av *av, struct efa_ep_addr *raw_addr,
				    struct efa_conn *conn)
{
	struct efa_cur_reverse_av *cur_entry;
	struct efa_prv_reverse_av *prv_entry;
	struct efa_cur_reverse_av_key cur_key;

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = conn->ah->ahn;
	cur_key.qpn = raw_addr->qpn;
	cur_entry = NULL;

	HASH_FIND(hh, av->cur_reverse_av, &cur_key, sizeof(cur_key), cur_entry);
	if (!cur_entry) {
		cur_entry = malloc(sizeof(*cur_entry));
		if (!cur_entry) {
			FI_WARN(&rxr_prov, FI_LOG_AV, "Cannot allocate memory for cur_reverse_av entry");
			return -FI_ENOMEM;
		}

		cur_entry->key.ahn = cur_key.ahn;
		cur_entry->key.qpn = cur_key.qpn;
		cur_entry->conn = conn;
		HASH_ADD(hh, av->cur_reverse_av, key, sizeof(cur_key), cur_entry);
		return 0;
	}

	/* We used a static connid for all dgram endpoints, therefore cur_entry should always be NULL,
	 * and only RDM endpoint can reach here. hence the following assertion
	 */
	assert(av->ep_type == FI_EP_RDM);
	prv_entry = malloc(sizeof(*prv_entry));
	if (!prv_entry) {
		FI_WARN(&rxr_prov, FI_LOG_AV, "Cannot allocate memory for prv_reverse_av entry");
		return -FI_ENOMEM;
	}

	prv_entry->key.ahn = cur_key.ahn;
	prv_entry->key.qpn = cur_key.qpn;
	prv_entry->key.connid = cur_entry->conn->ep_addr->qkey;
	prv_entry->conn = cur_entry->conn;
	HASH_ADD(hh, av->prv_reverse_av, key, sizeof(prv_entry->key), prv_entry);

	cur_entry->conn = conn;
	return 0;
}

/**
 * @brief allocate an efa_conn object
 * caller of this function must obtain av->util_av.lock
 *
 * @param[in]	av		efa address vector
 * @param[in]	raw_addr	raw efa address
 * @param[in]	flags		flags application passed to fi_av_insert
 * @param[in]	context		context application passed to fi_av_insert
 * @return	on success, return a pointer to an efa_conn object
 *		otherwise, return NULL. errno will be set to a positive error code.
 */
static
struct efa_conn *efa_conn_alloc(struct efa_av *av, struct efa_ep_addr *raw_addr,
				uint64_t flags, void *context)
{
	struct util_av_entry *util_av_entry = NULL;
	struct efa_av_entry *efa_av_entry = NULL;
	struct efa_conn *conn;
	fi_addr_t util_av_fi_addr;
	int err;

	if (flags & FI_SYNC_ERR)
		memset(context, 0, sizeof(int));

	if (!efa_av_is_valid_address(raw_addr)) {
		EFA_WARN(FI_LOG_AV, "Failed to insert bad addr\n");
		errno = FI_EINVAL;
		return NULL;
	}

	err = ofi_av_insert_addr(&av->util_av, raw_addr, &util_av_fi_addr);
	if (err) {
		EFA_WARN(FI_LOG_AV, "ofi_av_insert_addr failed! Error message: %s\n",
			 fi_strerror(err));
		return NULL;
	}

	util_av_entry = ofi_bufpool_get_ibuf(av->util_av.av_entry_pool,
					     util_av_fi_addr);
	efa_av_entry = (struct efa_av_entry *)util_av_entry->data;
	assert(efa_is_same_addr(raw_addr, (struct efa_ep_addr *)efa_av_entry->ep_addr));

	conn = &efa_av_entry->conn;
	memset(conn, 0, sizeof(*conn));
	conn->ep_addr = (struct efa_ep_addr *)efa_av_entry->ep_addr;
	assert(av->type == FI_AV_MAP || av->type == FI_AV_TABLE);
	conn->fi_addr = (av->type == FI_AV_MAP) ? (uintptr_t)(void *)conn : util_av_fi_addr;
	conn->util_av_fi_addr = util_av_fi_addr;

	conn->ah = efa_ah_alloc(av, raw_addr->raw);
	if (!conn->ah)
		goto err_release;

	if (av->ep_type == FI_EP_RDM) {
		err = efa_conn_rdm_init(av, conn);
		if (err) {
			errno = -err;
			goto err_release;
		}
	}

	err = efa_av_update_reverse_av(av, raw_addr, conn);
	if (err) {
		if (av->ep_type == FI_EP_RDM)
			efa_conn_rdm_deinit(av, conn);
		goto err_release;
	}

	av->used++;
	return conn;

err_release:
	if (conn->ah)
		efa_ah_release(av, conn->ah);

	conn->ep_addr = NULL;
	err = ofi_av_remove_addr(&av->util_av, util_av_fi_addr);
	if (err)
		EFA_WARN(FI_LOG_AV, "While processing previous failure, ofi_av_remove_addr failed! err=%d\n",
			 err);

	return NULL;
}

/**
 * @brief release an efa conn object
 * Caller of this function must obtain av->util_av.lock
 *
 * @param[in]	av	address vector
 * @param[in]	conn	efa_conn object pointer
 */
static
void efa_conn_release(struct efa_av *av, struct efa_conn *conn)
{
	struct efa_cur_reverse_av *cur_reverse_av_entry;
	struct efa_prv_reverse_av *prv_reverse_av_entry;
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *efa_av_entry;
	struct efa_cur_reverse_av_key cur_key;
	struct efa_prv_reverse_av_key prv_key;
	char gidstr[INET6_ADDRSTRLEN];

	memset(&cur_key, 0, sizeof(cur_key));
	cur_key.ahn = conn->ah->ahn;
	cur_key.qpn = conn->ep_addr->qpn;
	HASH_FIND(hh, av->cur_reverse_av, &cur_key, sizeof(cur_key), cur_reverse_av_entry);
	if (cur_reverse_av_entry) {
		HASH_DEL(av->cur_reverse_av, cur_reverse_av_entry);
		free(cur_reverse_av_entry);
	} else {
		memset(&prv_key, 0, sizeof(prv_key));
		prv_key.ahn = conn->ah->ahn;
		prv_key.qpn = conn->ep_addr->qpn;
		prv_key.connid = conn->ep_addr->qkey;
		HASH_FIND(hh, av->prv_reverse_av, &prv_key, sizeof(prv_key), prv_reverse_av_entry);
		assert(prv_reverse_av_entry);
		HASH_DEL(av->prv_reverse_av, prv_reverse_av_entry);
		free(prv_reverse_av_entry);
	}

	if (av->ep_type == FI_EP_RDM)
		efa_conn_rdm_deinit(av, conn);

	efa_ah_release(av, conn->ah);

	util_av_entry = ofi_bufpool_get_ibuf(av->util_av.av_entry_pool, conn->util_av_fi_addr);
	assert(util_av_entry);
	efa_av_entry = (struct efa_av_entry *)util_av_entry->data;

	ofi_av_remove_addr(&av->util_av, conn->util_av_fi_addr);

	inet_ntop(AF_INET6, conn->ep_addr->raw, gidstr, INET6_ADDRSTRLEN);
	EFA_INFO(FI_LOG_AV, "efa_conn released! conn[%p] GID[%s] QP[%u]\n",
		 conn, gidstr, conn->ep_addr->qpn);

	conn->ep_addr = NULL;
	memset(efa_av_entry->ep_addr, 0, EFA_EP_ADDR_LEN);

	av->used--;
}

/**
 * @brief insert one address into address vector (AV)
 *
 * @param[in]	av	address vector
 * @param[in]	addr	raw address, in the format of gid:qpn:qkey
 * @param[out]	fi_addr pointer to the output fi address. This address is used by fi_send
 * @param[in]	flags	flags user passed to fi_av_insert.
 * @param[in]	context	context user passed to fi_av_insert
 * @return	0 on success, a negative error code on failure
 */
int efa_av_insert_one(struct efa_av *av, struct efa_ep_addr *addr,
		      fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct efa_conn *conn;
	char raw_gid_str[INET6_ADDRSTRLEN];
	fi_addr_t efa_fiaddr;
	int ret = 0;

	if (av->ep_type == FI_EP_DGRAM)
		addr->qkey = EFA_DGRAM_CONNID;

	ofi_mutex_lock(&av->util_av.lock);
	memset(raw_gid_str, 0, sizeof(raw_gid_str));
	if (!inet_ntop(AF_INET6, addr->raw, raw_gid_str, INET6_ADDRSTRLEN)) {
		EFA_WARN(FI_LOG_AV, "cannot convert address to string. errno: %d", errno);
		ret = -FI_EINVAL;
		*fi_addr = FI_ADDR_NOTAVAIL;
		goto out;
	}

	EFA_INFO(FI_LOG_AV, "Inserting address GID[%s] QP[%u] QKEY[%u] to AV ....\n",
		 raw_gid_str, addr->qpn, addr->qkey);

	/*
	 * Check if this address already has been inserted, if so set *fi_addr to existing address,
	 * and return 0 for success.
	 */
	efa_fiaddr = ofi_av_lookup_fi_addr_unsafe(&av->util_av, addr);
	if (efa_fiaddr != FI_ADDR_NOTAVAIL) {
		*fi_addr = efa_fiaddr;
		EFA_INFO(FI_LOG_AV, "Found existing AV entry pointing to this address! fi_addr: %ld\n", *fi_addr);
		ret = 0;
		goto out;
	}

	conn = efa_conn_alloc(av, addr, flags, context);
	if (!conn) {
		*fi_addr = FI_ADDR_NOTAVAIL;
		ret = -FI_EADDRNOTAVAIL;
		goto out;
	}

	*fi_addr = conn->fi_addr;
	EFA_INFO(FI_LOG_AV, "Successfully inserted address GID[%s] QP[%u] QKEY[%u] to AV. fi_addr: %ld\n",
		 raw_gid_str, addr->qpn, addr->qkey, *fi_addr);
	ret = 0;
out:
	ofi_mutex_unlock(&av->util_av.lock);
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

		ret = efa_av_insert_one(av, addr_i, &fi_addr_res, flags, context);
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
		if (av->util_av.eq)
			ofi_av_write_event(&av->util_av, i, FI_ECANCELED,
					context);
		if (fi_addr)
			fi_addr[i] = FI_ADDR_NOTAVAIL;
	}

	/* update success to event queue */
	if (av->util_av.eq)
		ofi_av_write_event(&av->util_av, success_cnt, 0, context);

	return success_cnt;
}

static int efa_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
			 void *addr, size_t *addrlen)
{
	struct efa_av *av = container_of(av_fid, struct efa_av, util_av.av_fid);
	struct efa_conn *conn = NULL;

	if (av->type != FI_AV_MAP && av->type != FI_AV_TABLE)
		return -FI_EINVAL;

	if (fi_addr == FI_ADDR_NOTAVAIL)
		return -FI_EINVAL;

	conn = efa_av_addr_to_conn(av, fi_addr);
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
	if (av->type != FI_AV_MAP && av->type != FI_AV_TABLE)
		return -FI_EINVAL;

	ofi_mutex_lock(&av->util_av.lock);
	for (i = 0; i < count; i++) {
		conn = efa_av_addr_to_conn(av, fi_addr[i]);
		if (!conn) {
			err = -FI_EINVAL;
			break;
		}

		efa_conn_release(av, conn);
	}

	if (i < count) {
		/* something went wrong, so err cannot be zero */
		assert(err);
		if (av->util_av.eq) {
			for (; i < count; ++i)
				ofi_av_write_event(&av->util_av, i, FI_ECANCELED, NULL);
		}
	}

	ofi_mutex_unlock(&av->util_av.lock);
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

	ofi_mutex_lock(&av->util_av.lock);

	HASH_ITER(hh, av->cur_reverse_av, cur_entry, curtmp) {
		efa_conn_release(av, cur_entry->conn);
	}

	HASH_ITER(hh, av->prv_reverse_av, prv_entry, prvtmp) {
		efa_conn_release(av, prv_entry->conn);
	}

	ofi_mutex_unlock(&av->util_av.lock);
}

static int efa_av_close(struct fid *fid)
{
	struct efa_av *av;
	int ret = 0;
	int err = 0;

	av = container_of(fid, struct efa_av, util_av.av_fid.fid);

	efa_av_close_reverse_av(av);

	ret = ofi_av_close(&av->util_av);
	if (ret) {
		err = ret;
		EFA_WARN(FI_LOG_AV, "Failed to close av: %s\n",
			fi_strerror(ret));
	}

	if (av->ep_type == FI_EP_RDM) {
		if (av->shm_rdm_av) {
			ret = fi_close(&av->shm_rdm_av->fid);
			if (ret) {
				err = ret;
				EFA_WARN(FI_LOG_AV, "Failed to close shm av: %s\n",
					fi_strerror(ret));
			}
		}
	}
	free(av);
	return err;
}

static int efa_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	return ofi_av_bind(fid, bfid, flags);
}

static struct fi_ops efa_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_av_close,
	.bind = efa_av_bind,
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
	size_t universe_size;

	if (fi_param_get_size_t(NULL, "universe_size",
				&universe_size) == FI_SUCCESS)
		attr->count = MAX(attr->count, universe_size);

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
	int i, ret, retv;

	if (!attr)
		return -FI_EINVAL;

	if (attr->name)
		return -FI_ENOSYS;

	/* FI_EVENT, FI_READ, and FI_SYMMETRIC are not supported */
	if (attr->flags)
		return -FI_ENOSYS;

	/*
	 * TODO: remove me once RxR supports resizing members tied to the AV
	 * size.
	 */
	if (!attr->count)
		attr->count = EFA_MIN_AV_SIZE;
	else
		attr->count = MAX(attr->count, EFA_MIN_AV_SIZE);

	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;

	attr->type = FI_AV_TABLE;

	efa_domain = container_of(domain_fid, struct efa_domain, util_domain.domain_fid);

	ret = efa_av_init_util_av(efa_domain, attr, &av->util_av, context);
	if (ret)
		goto err;

	if (EFA_EP_TYPE_IS_RDM(efa_domain->info)) {
		av->ep_type = FI_EP_RDM;

		av_attr = *attr;
		if (efa_domain->fabric && efa_domain->fabric->shm_fabric) {
			/*
			 * shm av supports maximum 256 entries
			 * Reset the count to 128 to reduce memory footprint and satisfy
			 * the need of the instances with more CPUs.
			 */
			if (rxr_env.shm_av_size > EFA_SHM_MAX_AV_COUNT) {
				ret = -FI_ENOSYS;
				EFA_WARN(FI_LOG_AV, "The requested av size is beyond"
					 " shm supported maximum av size: %s\n",
					 fi_strerror(-ret));
				goto err_close_util_av;
			}
			av_attr.count = rxr_env.shm_av_size;
			assert(av_attr.type == FI_AV_TABLE);
			ret = fi_av_open(efa_domain->shm_domain, &av_attr,
					&av->shm_rdm_av, context);
			if (ret)
				goto err_close_util_av;

			for (i = 0; i < EFA_SHM_MAX_AV_COUNT; ++i)
				av->shm_rdm_addr_map[i] = FI_ADDR_UNSPEC;
		}
	} else {
		av->ep_type = FI_EP_DGRAM;
	}

	EFA_INFO(FI_LOG_AV, "fi_av_attr:%" PRId64 "\n",
			av_attr.flags);

	av->domain = efa_domain;
	av->type = attr->type;
	av->used = 0;
	av->shm_used = 0;

	*av_fid = &av->util_av.av_fid;
	(*av_fid)->fid.fclass = FI_CLASS_AV;
	(*av_fid)->fid.context = context;
	(*av_fid)->fid.ops = &efa_av_fi_ops;
	(*av_fid)->ops = &efa_av_ops;

	return 0;

err_close_util_av:
	retv = ofi_av_close(&av->util_av);
	if (retv)
		EFA_WARN(FI_LOG_AV,
			 "Unable to close util_av: %s\n", fi_strerror(-retv));
err:
	free(av);
	return ret;
}

