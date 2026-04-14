/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_AV_H
#define EFA_AV_H

#include <infiniband/verbs.h>
#include "ofi_util.h"
#include "rdm/efa_rdm_protocol.h"
#include "rdm/efa_rdm_peer.h"

#define EFA_MIN_AV_SIZE (16384)
#define EFA_SHM_MAX_AV_COUNT       (256)

#define EFA_GID_LEN	16

struct efa_ep_addr {
	uint8_t			raw[EFA_GID_LEN];
	uint16_t		qpn;
	uint16_t		pad;
	uint32_t		qkey;
	struct efa_ep_addr	*next;
};

struct efa_ep_addr_hashable {
	struct efa_ep_addr addr;
	UT_hash_handle	hh;
};

#define EFA_EP_ADDR_LEN sizeof(struct efa_ep_addr)

struct efa_ah {
	uint8_t		gid[EFA_GID_LEN]; /* efa device GID */
	struct ibv_ah	*ibv_ah; /* created by ibv_create_ah() using GID */
	uint16_t	ahn; /* adress handle number */
	int		refcnt; /* reference counter. Multiple efa_conn can share an efa_ah */
	UT_hash_handle	hh; /* hash map handle, link all efa_ah with efa_ep->ah_map */
};

struct efa_conn {
	struct efa_ah *ah;
	struct efa_ep_addr *ep_addr;
	fi_addr_t		implicit_fi_addr;
	fi_addr_t		fi_addr;
	fi_addr_t		shm_fi_addr;
	struct dlist_entry	implicit_av_lru_entry;
};

/* util_av implementation requires the first element of efa_av_entry to be
 * ep_addr */
struct efa_av_entry {
	uint8_t			ep_addr[EFA_EP_ADDR_LEN];
	struct efa_conn		conn;
};

struct efa_cur_reverse_av_key {
	uint16_t ahn;
	uint16_t qpn;
};

struct efa_cur_reverse_av {
	struct efa_cur_reverse_av_key key;
	struct efa_conn *conn;
	UT_hash_handle hh;
};

struct efa_prv_reverse_av_key {
	uint16_t ahn;
	uint16_t qpn;
	uint32_t connid;
};

struct efa_prv_reverse_av {
	struct efa_prv_reverse_av_key key;
	struct efa_conn *conn;
	UT_hash_handle hh;
};

struct efa_av {
	struct fid_av *shm_rdm_av;
	struct efa_domain *domain;
	size_t used_explicit;
	size_t used_implicit;
	size_t shm_used;
	enum fi_av_type type;
	/* cur_reverse_av is a map from (ahn + qpn) to current (latest) efa_conn.
	 * prv_reverse_av is a map from (ahn + qpn + connid) to all previous efa_conns.
	 * cur_reverse_av is faster to search because its key size is smaller
	 */
	struct efa_cur_reverse_av *cur_reverse_av;
	struct efa_prv_reverse_av *prv_reverse_av;
	struct util_av util_av;

	/* implicit AV is used when receiving messages from peers not explicity
	 * inserted by the application
	 */
	struct util_av util_av_implicit;
	struct efa_cur_reverse_av *cur_reverse_av_implicit;
	struct efa_prv_reverse_av *prv_reverse_av_implicit;

	size_t implicit_av_size;
	struct dlist_entry implicit_av_lru_list;
	struct efa_ep_addr_hashable *evicted_peers_hashset;
};

int efa_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av_fid, void *context);

int efa_av_insert_one(struct efa_av *av, struct efa_ep_addr *addr,
		      fi_addr_t *fi_addr, uint64_t flags, void *context,
		      bool insert_shm_av, bool insert_implicit_av);

struct efa_conn *efa_av_addr_to_conn(struct efa_av *av, fi_addr_t fi_addr);
struct efa_conn *efa_av_addr_to_conn_implicit(struct efa_av *av,
					      fi_addr_t fi_addr);

fi_addr_t efa_av_reverse_lookup_rdm(struct efa_av *av, uint16_t ahn,
				    uint16_t qpn, struct efa_rdm_pke *pkt_entry);

fi_addr_t efa_av_reverse_lookup_rdm_implicit(struct efa_av *av, uint16_t ahn,
					     uint16_t qpn,
					     struct efa_rdm_pke *pkt_entry);

fi_addr_t efa_av_reverse_lookup(struct efa_av *av, uint16_t ahn, uint16_t qpn);

int efa_av_reverse_av_add(struct efa_av *av,
			  struct efa_cur_reverse_av **cur_reverse_av,
			  struct efa_prv_reverse_av **prv_reverse_av,
			  struct efa_conn *conn);

struct efa_ah *efa_ah_alloc(struct efa_domain *domain, const uint8_t *gid);

void efa_ah_release(struct efa_domain *domain, struct efa_ah *ah);

void efa_av_implicit_av_lru_move(struct efa_av *av,
					struct efa_conn *conn);

#endif