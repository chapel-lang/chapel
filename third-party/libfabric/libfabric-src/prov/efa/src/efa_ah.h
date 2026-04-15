/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_AH_H
#define EFA_AH_H

#include "efa_domain.h"
#include "ofi_util.h"

#define EFA_GID_LEN	16

struct efa_ah {
	uint8_t		gid[EFA_GID_LEN]; /* efa device GID */
	struct ibv_ah	*ibv_ah; /* created by ibv_create_ah() using GID */
	uint16_t	ahn; /* adress handle number */
	/* Number of explicit AV entries associated with this AH */
	int explicit_refcnt;
	/* Number of implicit AV entries associated with this AH */
	int implicit_refcnt;
	/* dlist of all implicit AV entries associated with this AH entry */
	struct dlist_entry implicit_conn_list;
	/* dlist entry in domain's LRU AH list */
	struct dlist_entry domain_lru_ah_list_entry;
	UT_hash_handle	hh; /* hash map handle, link all efa_ah with efa_ep->ah_map */
};

void efa_ah_implicit_av_lru_ah_move(struct efa_domain *domain,
					struct efa_ah *ah);

struct efa_ah *efa_ah_alloc(struct efa_domain *domain, const uint8_t *gid,
			    bool insert_implicit_av);

void efa_ah_release(struct efa_domain *domain, struct efa_ah *ah,
		    bool release_from_implicit_av);

void efa_ah_release_unsafe(struct efa_domain *domain, struct efa_ah *ah,
			   bool release_from_implicit_av);

#endif