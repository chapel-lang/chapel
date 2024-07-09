/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_RDM_OPE_RECVMAP
#define EFA_RDM_OPE_RECVMAP

#include <stdint.h>
#include <rdma/fi_endpoint.h>
#include "uthash.h"


struct efa_rdm_rxe_map_key {
	uint64_t msg_id;
	fi_addr_t addr;
};


struct efa_rdm_rxe_map_entry;

/**
 * @brief a hashmap between sender address + msg_id to RX entry
 *
 * @details
 * This hash map is used on the receiver side to implement
 * medium and runting protocols. Such protocol will send
 * multiple RTM packets at the same time. The first RTM
 * will be matched with an RX entry and will be inserted
 * to this map, the later arriving RTM packet will use
 * this hashmap to find the RX entry.
 */
struct efa_rdm_rxe_map {
	struct efa_rdm_rxe_map_entry *head;
};

struct efa_rdm_ope;

struct efa_rdm_rxe_map_entry {
	struct efa_rdm_rxe_map_key key;
	struct efa_rdm_ope *rxe;
	UT_hash_handle hh;
};

static inline
void efa_rdm_rxe_map_construct(struct efa_rdm_rxe_map *rxe_map)
{
	rxe_map->head = NULL;
}

struct efa_rdm_pke;

struct efa_rdm_ope *efa_rdm_rxe_map_lookup(struct efa_rdm_rxe_map *rxe_map,
					   struct efa_rdm_pke *pkt_entry);

void efa_rdm_rxe_map_insert(struct efa_rdm_rxe_map *rxe_map,
			    struct efa_rdm_pke *pkt_entry,
			    struct efa_rdm_ope *rxe);

void efa_rdm_rxe_map_remove(struct efa_rdm_rxe_map *rxe_map, uint64_t msg_id,
			    fi_addr_t addr, struct efa_rdm_ope *rxe);
#endif