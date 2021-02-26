/*
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
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

/*
 * Local/remote peer detection by comparing peer GID with stored local GIDs
 */
static bool efa_is_local_peer(struct efa_av *av, const void *addr)
{
	struct efa_ep_addr *cur_efa_addr = local_efa_addr;

#if ENABLE_DEBUG
	char peer_gid[INET6_ADDRSTRLEN] = { 0 };

	if (!inet_ntop(AF_INET6, ((struct efa_ep_addr *)addr)->raw, peer_gid, INET6_ADDRSTRLEN)) {
		EFA_WARN(FI_LOG_AV, "Failed to get current EFA's GID, errno: %d\n", errno);
		return 0;
	}
	EFA_INFO(FI_LOG_AV, "The peer's GID is %s.\n", peer_gid);
#endif
	while (cur_efa_addr) {
		if (!memcmp(((struct efa_ep_addr *)addr)->raw, cur_efa_addr->raw, 16)) {
			EFA_INFO(FI_LOG_AV, "The peer is local.\n");
			return 1;
		}
		cur_efa_addr = cur_efa_addr->next;
	}

	return 0;
}

static bool efa_is_same_addr(struct efa_ep_addr *lhs, struct efa_ep_addr *rhs)
{
	return !memcmp(lhs->raw, rhs->raw, sizeof(lhs->raw)) &&
	       lhs->qpn == rhs->qpn && lhs->qkey == rhs->qkey;
}

static inline struct efa_conn *efa_av_tbl_idx_to_conn(struct efa_av *av, fi_addr_t addr)
{
	if (OFI_UNLIKELY(addr == FI_ADDR_UNSPEC))
		return NULL;
	return av->conn_table[addr];
}

static inline struct efa_conn *efa_av_map_addr_to_conn(struct efa_av *av, fi_addr_t addr)
{
	if (OFI_UNLIKELY(addr == FI_ADDR_UNSPEC))
		return NULL;
	return (struct efa_conn *)(void *)addr;
}

fi_addr_t efa_ahn_qpn_to_addr(struct efa_av *av, uint16_t ahn, uint16_t qpn)
{
	struct efa_reverse_av *reverse_av;
	struct efa_ah_qpn key = {
		.ahn = ahn,
		.qpn = qpn,
	};

	HASH_FIND(hh, av->reverse_av, &key, sizeof(key), reverse_av);

	return OFI_LIKELY(!!reverse_av) ? reverse_av->fi_addr : FI_ADDR_NOTAVAIL;
}

static inline int efa_av_is_valid_address(struct efa_ep_addr *addr)
{
	struct efa_ep_addr all_zeros = {};

	return memcmp(addr->raw, all_zeros.raw, sizeof(addr->raw));
}

/* Returns the first NULL index in av connection table, starting from @hint */
static size_t efa_av_tbl_find_first_empty(struct efa_av *av, size_t hint)
{
	struct efa_conn **conn_table;

	assert(av->type == FI_AV_TABLE);

	conn_table = av->conn_table;
	for (; hint < av->util_av.count; hint++) {
		if (!conn_table[hint])
			return hint;
	}

	return -1;
}

static int efa_av_resize(struct efa_av *av, size_t new_av_count)
{
	if (av->type == FI_AV_TABLE) {
		void *p = realloc(av->conn_table,
				  (new_av_count *
				  sizeof(*av->conn_table)));

		if (p)
			av->conn_table = p;
		else
			return -FI_ENOMEM;

		memset(av->conn_table + av->util_av.count, 0,
		       (new_av_count - av->util_av.count) * sizeof(*av->conn_table));
	}

	av->util_av.count = new_av_count;

	return 0;
}

/* Inserts a single AH to AV. */
static int efa_av_insert_ah(struct efa_av *av, struct efa_ep_addr *addr,
				fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct ibv_pd *ibv_pd = av->domain->ibv_pd;
	struct ibv_ah_attr ah_attr = { 0 };

	char str[INET6_ADDRSTRLEN] = { 0 };
	struct efadv_ah_attr attr = { 0 };
	struct efa_reverse_av *reverse_av;
	struct efa_ah_qpn key;
	struct efa_conn *conn;
	int err;

	if (av->util_av.flags & FI_EVENT)
		return -FI_ENOEQ;
	if ((flags & FI_SYNC_ERR) && (!context || (flags & FI_EVENT)))
		return -FI_EINVAL;
	else if (flags & FI_SYNC_ERR)
		memset(context, 0, sizeof(int));

	memset(&ah_attr, 0, sizeof(struct ibv_ah_attr));
	inet_ntop(AF_INET6, addr->raw, str, INET6_ADDRSTRLEN);
	EFA_INFO(FI_LOG_AV, "Insert address: GID[%s] QP[%u] QKEY[%u]\n", str, addr->qpn, addr->qkey);
	if (!efa_av_is_valid_address(addr)) {
		EFA_WARN(FI_LOG_AV, "Failed to insert bad addr");
		err = -FI_EADDRNOTAVAIL;
		goto err_invalid;
	}

	err = ofi_memalign((void **)&conn, EFA_MEM_ALIGNMENT, sizeof(*conn));
	if (err) {
		err = -FI_ENOMEM;
		goto err_invalid;
	}

	ah_attr.port_num = 1;
	ah_attr.is_global = 1;
	memcpy(ah_attr.grh.dgid.raw, addr->raw, sizeof(addr->raw));
	conn->ah.ibv_ah = ibv_create_ah(ibv_pd, &ah_attr);
	if (!conn->ah.ibv_ah) {
		err = -FI_EINVAL;
		goto err_free_conn;
	}
	memcpy((void *)&conn->ep_addr, addr, sizeof(*addr));

	switch (av->type) {
	case FI_AV_MAP:
		*fi_addr = (uintptr_t)(void *)conn;

		break;
	case FI_AV_TABLE:
		av->next = efa_av_tbl_find_first_empty(av, av->next);
		assert(av->next != -1);
		*fi_addr = av->next;

		av->conn_table[av->next] = conn;
		av->next++;
		break;
	default:
		assert(0);
		break;
	}

	err = -efadv_query_ah(conn->ah.ibv_ah, &attr, sizeof(attr));
	if (err)
		goto err_destroy_ah;

	conn->ah.ahn = attr.ahn;
	key.ahn = conn->ah.ahn;
	key.qpn = addr->qpn;
	/* This is correct since the same address should be mapped to the same ah. */
	HASH_FIND(hh, av->reverse_av, &key, sizeof(key), reverse_av);
	if (!reverse_av) {
		reverse_av = malloc(sizeof(*reverse_av));
		if (!reverse_av) {
			err = -FI_ENOMEM;
			goto err_destroy_ah;
		}

		memcpy(&reverse_av->key, &key, sizeof(key));
		reverse_av->fi_addr = *fi_addr;
		HASH_ADD(hh, av->reverse_av, key,
			 sizeof(reverse_av->key), reverse_av);
	}

	EFA_INFO(FI_LOG_AV, "av successfully inserted conn[%p] fi_addr[%" PRIu64 "]\n",
		 conn, *fi_addr);

	av->used++;
	return FI_SUCCESS;

err_destroy_ah:
	ibv_destroy_ah(conn->ah.ibv_ah);
err_free_conn:
	ofi_freealign(conn);
err_invalid:
	*fi_addr = FI_ADDR_NOTAVAIL;
	return err;
}

/*
 * Insert address translation in core av & in hash.
 *
 * If shm transfer is enabled and the addr comes from local peer,
 * 1. convert addr to format 'gid_qpn', which will be set as shm's ep name later.
 * 2. insert gid_qpn into shm's av
 * 3. store returned fi_addr from shm into the hash table
 */
int efa_av_insert_addr(struct efa_av *av, struct efa_ep_addr *addr,
			   fi_addr_t *fi_addr, uint64_t flags,
			   void *context)
{
	struct efa_av_entry *av_entry;
	int ret = 0;
	struct rxr_peer *peer;
	struct rxr_ep *rxr_ep;
	struct util_ep *util_ep;
	struct dlist_entry *ep_list_entry;
	fi_addr_t shm_fiaddr;
	char smr_name[RXR_MAX_NAME_LENGTH];

	fastlock_acquire(&av->util_av.lock);

	HASH_FIND(hh, av->av_map, addr, EFA_EP_ADDR_LEN, av_entry);
	if (av_entry) {
		*fi_addr = av_entry->rdm_addr;
		goto find_out;
	}
	if (av->used + 1 > av->util_av.count) {
		ret = efa_av_resize(av, av->used + 1);
		if (ret)
			goto out;
	}
	ret = efa_av_insert_ah(av, addr, fi_addr,
				flags, context);
	if (ret) {
		EFA_WARN(FI_LOG_AV, "Error in inserting address: %s\n",
			 fi_strerror(ret));
		goto out;
	}
	av_entry = calloc(1, sizeof(*av_entry));
	if (OFI_UNLIKELY(!av_entry)) {
		ret = -FI_ENOMEM;
		EFA_WARN(FI_LOG_AV, "Failed to allocate memory for av_entry\n");
		goto out;
	}
	memcpy((void *)&av_entry->ep_addr, addr, EFA_EP_ADDR_LEN);
	av_entry->rdm_addr = *fi_addr;

	/*
	 * Walk through all the EPs that bound to the AV,
	 * update is_self flag corresponding peer structure
	 */
	dlist_foreach(&av->util_av.ep_list, ep_list_entry) {
		util_ep = container_of(ep_list_entry, struct util_ep, av_entry);
		rxr_ep = container_of(util_ep, struct rxr_ep, util_ep);
		peer = rxr_ep_get_peer(rxr_ep, *fi_addr);
		assert(peer);
		peer->is_self = efa_is_same_addr((struct efa_ep_addr *)rxr_ep->core_addr,
						 addr);
	}

	/* If peer is local, insert the address into shm provider's av */
	if (rxr_env.enable_shm_transfer && efa_is_local_peer(av, addr)) {
		if (av->shm_used >= rxr_env.shm_av_size) {
			ret = -FI_ENOMEM;
			EFA_WARN(FI_LOG_AV,
				 "Max number of shm AV entry %d has been reached.\n",
				 rxr_env.shm_av_size);
			goto err_free_av_entry;
		}
		ret = rxr_ep_efa_addr_to_str(addr, smr_name);
		if (ret != FI_SUCCESS)
			goto err_free_av_entry;

		ret = fi_av_insert(av->shm_rdm_av, smr_name, 1, &shm_fiaddr,
					flags, context);
		if (OFI_UNLIKELY(ret != 1)) {
			EFA_WARN(FI_LOG_AV,
				 "Failed to insert address to shm provider's av: %s\n",
				 fi_strerror(-ret));
			goto err_free_av_entry;
		} else {
			ret = 0;
		}
		EFA_INFO(FI_LOG_AV,
			"Insert %s to shm provider's av. addr = %" PRIu64
			" rdm_fiaddr = %" PRIu64 " shm_rdm_fiaddr = %" PRIu64
			"\n", smr_name, *(uint64_t *)addr, *fi_addr, shm_fiaddr);

		assert(shm_fiaddr < rxr_env.shm_av_size);
		av->shm_used++;
		av_entry->local_mapping = 1;
		av_entry->shm_rdm_addr = shm_fiaddr;
		av->shm_rdm_addr_map[shm_fiaddr] = av_entry->rdm_addr;

		/*
		 * Walk through all the EPs that bound to the AV,
		 * update is_local flag and shm fi_addr_t in corresponding peer structure
		 */
		dlist_foreach(&av->util_av.ep_list, ep_list_entry) {
			util_ep = container_of(ep_list_entry, struct util_ep, av_entry);
			rxr_ep = container_of(util_ep, struct rxr_ep, util_ep);
			if (rxr_ep->use_shm) {
				peer = rxr_ep_get_peer(rxr_ep, *fi_addr);
				peer->shm_fiaddr = shm_fiaddr;
				peer->is_local = 1;
			}
		}
	}
	HASH_ADD(hh, av->av_map, ep_addr,
			EFA_EP_ADDR_LEN, av_entry);

find_out:
	EFA_INFO(FI_LOG_AV,
			"addr = %" PRIu64 " rdm_fiaddr =  %" PRIu64 "\n",
			*(uint64_t *)addr, *fi_addr);
	goto out;
err_free_av_entry:
	free(av_entry);
out:
	fastlock_release(&av->util_av.lock);
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

	/*
	 * Providers are allowed to ignore FI_MORE.
	 */

	flags &= ~FI_MORE;
	if (flags)
		return -FI_ENOSYS;

	if (av->ep_type == FI_EP_RDM) {
		if (av->used + count > av->util_av.count) {
			EFA_WARN(FI_LOG_AV,
				"AV insert failed. Expect inserting %zu AV entries, but only %zu available\n",
				count, av->util_av.count - av->used);
			if (av->util_av.eq)
				ofi_av_write_event(&av->util_av, i, FI_ENOMEM,
					context);
			goto out;
		}
		for (i = 0; i < count; i++) {
			addr_i = (struct efa_ep_addr *) ((uint8_t *)addr + i * EFA_EP_ADDR_LEN);
			ret = efa_av_insert_addr(av, addr_i, &fi_addr_res,
					flags, context);
			if (ret)
				break;
			if (fi_addr)
				fi_addr[i] = fi_addr_res;
			success_cnt++;
		}
	} else {
		if (av->used + count > av->util_av.count) {
			ret = efa_av_resize(av, av->used + count);
			if (ret)
				goto out;
		}
		for (i = 0; i < count; i++) {
			addr_i = (struct efa_ep_addr *) ((uint8_t *)addr + i * EFA_EP_ADDR_LEN);
			ret = efa_av_insert_ah(av, addr_i, &fi_addr_res,
					     flags, context);
			if (ret)
				break;
			if (fi_addr)
				fi_addr[i] = fi_addr_res;
			success_cnt++;
		}
	}
out:
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

	if (av->type == FI_AV_MAP) {
		conn = (struct efa_conn *)fi_addr;
	} else { /* (av->type == FI_AV_TABLE) */
		if (fi_addr >= av->util_av.count)
			return -FI_EINVAL;

		conn = av->conn_table[fi_addr];
	}
	if (!conn)
		return -FI_EINVAL;

	memcpy(addr, (void *)&conn->ep_addr, MIN(sizeof(conn->ep_addr), *addrlen));
	*addrlen = sizeof(conn->ep_addr);
	return 0;
}

static int efa_av_remove_ah(struct fid_av *av_fid, fi_addr_t *fi_addr,
			    size_t count, uint64_t flags)
{
	struct efa_av *av = container_of(av_fid, struct efa_av, util_av.av_fid);
	struct efa_conn *conn = NULL;
	struct efa_reverse_av *reverse_av;
	struct efa_ah_qpn key;
	char str[INET6_ADDRSTRLEN];
	int ret = 0;

	if (!fi_addr || (av->type != FI_AV_MAP && av->type != FI_AV_TABLE))
		return -FI_EINVAL;

	if (*fi_addr == FI_ADDR_NOTAVAIL)
		return ret;

	if (av->type == FI_AV_MAP) {
		conn = (struct efa_conn *)fi_addr;
	} else { /* (av->type == FI_AV_TABLE) */
		conn = av->conn_table[*fi_addr];
		av->conn_table[*fi_addr] = NULL;
		av->next = MIN(av->next, *fi_addr);
	}
	if (!conn)
		return ret;

	key.ahn = conn->ah.ahn;
	key.qpn = conn->ep_addr.qpn;
	HASH_FIND(hh, av->reverse_av, &key, sizeof(key), reverse_av);
	if (OFI_LIKELY(!!reverse_av)) {
		HASH_DEL(av->reverse_av, reverse_av);
		free(reverse_av);
	}

	ret = -ibv_destroy_ah(conn->ah.ibv_ah);
	if (ret)
		goto err_free_conn;

	memset(str, 0, sizeof(str));
	inet_ntop(AF_INET6, conn->ep_addr.raw, str, INET6_ADDRSTRLEN);
	EFA_INFO(FI_LOG_AV, "av_remove conn[%p] with GID[%s] QP[%u]\n", conn,
			str, conn->ep_addr.qpn);
	av->used--;

err_free_conn:
	ofi_freealign(conn);
	return ret;
}

static int efa_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	int ret = 0;
	size_t i;
	struct efa_av *av;
	struct efa_av_entry *av_entry;
	struct efa_ep_addr addr;

	av = container_of(av_fid, struct efa_av, util_av.av_fid);
	if (av->ep_type == FI_EP_RDM) {
		fastlock_acquire(&av->util_av.lock);
		for (i = 0; i < count; i++) {
			ret = efa_av_lookup(&av->util_av.av_fid, fi_addr[i],
						&addr, &av->util_av.addrlen);
			if (ret)
				goto release_lock;

			ret = efa_av_remove_ah(&av->util_av.av_fid, &fi_addr[i], 1, flags);
			if (ret)
				goto release_lock;
			HASH_FIND(hh, av->av_map, &addr, av->util_av.addrlen, av_entry);
			if (!av_entry) {
				ret = -FI_EINVAL;
				goto release_lock;
			}
			/* remove an address from shm provider's av */
			if (rxr_env.enable_shm_transfer && av_entry->local_mapping) {
				ret = fi_av_remove(av->shm_rdm_av, &av_entry->shm_rdm_addr, 1, flags);
				if (ret)
					goto err_free_av_entry;

				av->shm_used--;
				assert(av_entry->shm_rdm_addr < rxr_env.shm_av_size);
				av->shm_rdm_addr_map[av_entry->shm_rdm_addr] = FI_ADDR_UNSPEC;
			}
			HASH_DEL(av->av_map, av_entry);
			free(av_entry);
		}
		fastlock_release(&av->util_av.lock);
	} else {
		for (i = 0; i < count; i++) {
			ret = efa_av_remove_ah(&av->util_av.av_fid, &fi_addr[i], 1, flags);
			if (ret)
				goto out;
		}
	}
	goto out;
err_free_av_entry:
	free(av_entry);
release_lock:
	fastlock_release(&av->util_av.lock);
out:
	return ret;
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

static int efa_av_close(struct fid *fid)
{
	struct efa_av *av;
	struct efa_av_entry *current_av_entry, *tmp;
	int ret = 0;
	int err = 0;
	int i;

	av = container_of(fid, struct efa_av, util_av.av_fid.fid);
	for (i = 0; i < av->util_av.count; i++) {
		fi_addr_t addr = i;

		ret = efa_av_remove_ah(&av->util_av.av_fid, &addr, 1, 0);
		if (ret) {
			err = ret;
			EFA_WARN(FI_LOG_AV, "Failed to remove ah: %s\n",
				fi_strerror(ret));
		}
	}
	free(av->conn_table);
	if (av->ep_type == FI_EP_RDM) {
		if (rxr_env.enable_shm_transfer && av->shm_rdm_av &&
		    &av->shm_rdm_av->fid) {
			ret = fi_close(&av->shm_rdm_av->fid);
			if (ret) {
				err = ret;
				EFA_WARN(FI_LOG_AV, "Failed to close shm av: %s\n",
					fi_strerror(ret));
			}
		}
		ret = ofi_av_close(&av->util_av);
		if (ret) {
			err = ret;
			EFA_WARN(FI_LOG_AV, "Failed to close av: %s\n",
				fi_strerror(ret));
		}
		HASH_ITER(hh, av->av_map, current_av_entry, tmp) {
			HASH_DEL(av->av_map, current_av_entry);
			free(current_av_entry);
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

int efa_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av_fid, void *context)
{
	struct efa_domain *efa_domain;
	struct util_domain *util_domain;
	struct rxr_domain *rxr_domain;
	struct efa_domain_base *efa_domain_base;
	struct efa_av *av;
	struct util_av_attr util_attr;
	size_t universe_size;
	struct fi_av_attr av_attr;
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

	util_domain = container_of(domain_fid, struct util_domain,
				   domain_fid);
	efa_domain_base = container_of(util_domain, struct efa_domain_base,
				       util_domain.domain_fid);
	attr->type = FI_AV_TABLE;
	/*
	 * An rxr_domain fid was passed to the user if this is an RDM
	 * endpoint, otherwise it is an efa_domain fid.  This will be
	 * removed once the rxr and efa domain structures are combined.
	 */
	if (efa_domain_base->type == EFA_DOMAIN_RDM) {
		rxr_domain = (struct rxr_domain *)efa_domain_base;
		efa_domain = container_of(rxr_domain->rdm_domain, struct efa_domain,
						util_domain.domain_fid);
		av->ep_type = FI_EP_RDM;

		if (fi_param_get_size_t(NULL, "universe_size",
					&universe_size) == FI_SUCCESS)
			attr->count = MAX(attr->count, universe_size);

		util_attr.addrlen = EFA_EP_ADDR_LEN;
		util_attr.flags = 0;
		ret = ofi_av_init(&efa_domain->util_domain, attr, &util_attr,
					&av->util_av, context);
		if (ret)
			goto err;
		av_attr = *attr;
		if (rxr_env.enable_shm_transfer) {
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
		efa_domain = (struct efa_domain *)efa_domain_base;
		av->ep_type = FI_EP_DGRAM;
	}

	EFA_INFO(FI_LOG_AV, "fi_av_attr:%" PRId64 "\n",
			av_attr.flags);

	av->domain = efa_domain;
	av->type = attr->type;
	av->used = 0;
	av->next = 0;
	av->shm_used = 0;

	if (av->type == FI_AV_TABLE && av->util_av.count > 0) {
		av->conn_table = calloc(av->util_av.count, sizeof(*av->conn_table));
		if (!av->conn_table) {
			ret = -FI_ENOMEM;
			if (av->ep_type == FI_EP_DGRAM)
				goto err_close_util_av;
			else
				goto err_close_shm_av;
		}
	}

	if (av->type == FI_AV_MAP)
		av->addr_to_conn = efa_av_map_addr_to_conn;
	else /* if (av->type == FI_AV_TABLE) */
		av->addr_to_conn = efa_av_tbl_idx_to_conn;

	*av_fid = &av->util_av.av_fid;
	(*av_fid)->fid.fclass = FI_CLASS_AV;
	(*av_fid)->fid.context = context;
	(*av_fid)->fid.ops = &efa_av_fi_ops;
	(*av_fid)->ops = &efa_av_ops;

	return 0;

err_close_shm_av:
	if (rxr_env.enable_shm_transfer) {
		retv = fi_close(&av->shm_rdm_av->fid);
		if (retv)
			EFA_WARN(FI_LOG_AV, "Unable to close shm av: %s\n",
				fi_strerror(ret));
	}
err_close_util_av:
	retv = ofi_av_close(&av->util_av);
	if (retv)
		EFA_WARN(FI_LOG_AV,
			 "Unable to close util_av: %s\n", fi_strerror(-retv));
err:
	free(av);
	return ret;
}
