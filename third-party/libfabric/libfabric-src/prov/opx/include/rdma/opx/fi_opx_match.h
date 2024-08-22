/*
 * Copyright (C) 2023 Cornelis Networks.
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

#ifndef _FI_PROV_OPX_MATCH_H_
#define _FI_PROV_OPX_MATCH_H_
#include <stdint.h>

#include "rdma/opx/fi_opx_debug_counters.h"
#include "rdma/opx/fi_opx_addr.h"
#include "rdma/opx/fi_opx_hfi1_packet.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_compiler.h"
#include <ofi_list.h>
#include <ofi_mem.h>

/*
 * The maximum length of the default non-hashing UE slist before new early arriving
 * packets are put on the new hashing list.
 */
#ifndef FI_OPX_MATCH_DEFAULT_UE_LIST_MAX_LENGTH
#define FI_OPX_MATCH_DEFAULT_UE_LIST_MAX_LENGTH	(1024)
#endif

/*
 * The size of the tag and addr hash tables. Must be a power of 2.
 */
#ifndef FI_OPX_MATCH_UE_HT_SIZE
#define FI_OPX_MATCH_UE_HT_SIZE		(1024ul)
#endif

#define FI_OPX_MATCH_UE_HT_MASK		(FI_OPX_MATCH_UE_HT_SIZE - 1)
OPX_COMPILE_TIME_ASSERT((FI_OPX_MATCH_UE_HT_SIZE & FI_OPX_MATCH_UE_HT_MASK) == 0,
			"FI_OPX_MATCH_UE_HT_SIZE must be a power of 2!");

#define FI_OPX_MATCH_IGNORE_ALL		(-1ul)
#define FI_OPX_MATCH_TAG_ZERO		(0ul)

struct fi_opx_match_ue_hash {
	struct fi_opx_hfi1_ue_packet_slist	ue;	/* 3 qws */
	struct fi_opx_hfi1_ue_packet_slist	tag_ht[FI_OPX_MATCH_UE_HT_SIZE];
} __attribute__((__packed__)) __attribute__((aligned(64)));

__OPX_FORCE_INLINE__
int fi_opx_match_ue_hash_init(struct fi_opx_match_ue_hash **ue_hash) {
	if (posix_memalign((void **)ue_hash, 64, sizeof(struct fi_opx_match_ue_hash))) {
		*ue_hash = NULL;
		return -FI_ENOMEM;
	}
	memset(*ue_hash, 0, sizeof(struct fi_opx_match_ue_hash));
	return 0;
}

__OPX_FORCE_INLINE__
void fi_opx_match_ue_hash_free(struct fi_opx_match_ue_hash **ue_hash) {
	if (*ue_hash) {
		free(*ue_hash);
		*ue_hash = NULL;
	}
}

__OPX_FORCE_INLINE__
void fi_opx_match_ue_hash_append(struct fi_opx_hfi1_ue_packet *uepkt,
				struct fi_opx_match_ue_hash *ue_hash,
				struct fi_opx_debug_counters *debug_counters)
{
	const uint64_t index = uepkt->tag & FI_OPX_MATCH_UE_HT_MASK;
	struct fi_opx_hfi1_ue_packet_slist *ue_ht = &ue_hash->tag_ht[index];

	uepkt->tag_ht.ht = ue_ht;
	struct fi_opx_hfi1_ue_packet *tail = (struct fi_opx_hfi1_ue_packet *) ue_ht->tail;

	++ue_ht->length;

	if (!tail) {
		assert(ue_ht->length == 1);
		ue_ht->head = uepkt;
		ue_ht->tail = uepkt;
		uepkt->tag_ht.next = NULL;
		uepkt->tag_ht.prev = NULL;
	} else {
		assert(ue_ht->length > 1);

		uepkt->tag_ht.prev = tail;
		uepkt->tag_ht.next = NULL;
		tail->tag_ht.next = uepkt;
		ue_ht->tail = uepkt;
	}

	FI_OPX_DEBUG_COUNTERS_MAX_OF(debug_counters->match.ue_hash_tag_max_length, ue_ht->length);
	FI_OPX_DEBUG_COUNTERS_MAX_OF(debug_counters->match.ue_hash_linear_max_length, ue_hash->ue.length);
}

__OPX_FORCE_INLINE__
void fi_opx_match_ue_ht_remove(struct fi_opx_hfi1_ue_packet *uepkt)
{
	/* Packet was the first */
	if (!uepkt->tag_ht.prev) {
		assert(uepkt->tag_ht.ht->head == uepkt);

		/* Packet was also the only one, hq_list now empty */
		if (!uepkt->tag_ht.next) {
			assert(uepkt->tag_ht.ht->tail == uepkt);

			uepkt->tag_ht.ht->head = NULL;
			uepkt->tag_ht.ht->tail = NULL;
		} else {
			uepkt->tag_ht.next->tag_ht.prev = NULL;
			uepkt->tag_ht.ht->head = uepkt->tag_ht.next;
		}
	} else if (!uepkt->tag_ht.next) {
		assert(uepkt->tag_ht.ht->tail == uepkt);

		uepkt->tag_ht.prev->tag_ht.next = NULL;
		uepkt->tag_ht.ht->tail = uepkt->tag_ht.prev;
	} else {
		uepkt->tag_ht.prev->tag_ht.next = uepkt->tag_ht.next;
		uepkt->tag_ht.next->tag_ht.prev = uepkt->tag_ht.prev;
	}
	assert(uepkt->tag_ht.ht->length > 0);
	--uepkt->tag_ht.ht->length;

	uepkt->tag_ht.ht = NULL;
	uepkt->tag_ht.next = NULL;
	uepkt->tag_ht.prev = NULL;
}

__OPX_FORCE_INLINE__
void fi_opx_match_ue_hash_pop(struct fi_opx_hfi1_ue_packet *uepkt,
				struct fi_opx_match_ue_hash *ue_hash)
{
	fi_opx_match_ue_ht_remove(uepkt);

	fi_opx_hfi1_ue_packet_slist_pop_item(uepkt, &ue_hash->ue);
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_ue_packet *fi_opx_match_ue_hash_remove(struct fi_opx_hfi1_ue_packet *uepkt,
							  struct fi_opx_match_ue_hash *ue_hash)
{
	struct fi_opx_hfi1_ue_packet *next = uepkt->next;
	fi_opx_match_ue_hash_pop(uepkt, ue_hash);
	OPX_BUF_FREE(uepkt);
	return next;
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_match_packet(const uint64_t origin_tag,
			     const fi_opx_uid_t origin_uid_fi,
			     const uint64_t target_tag,
			     const uint64_t ignore_bits,
			     const union fi_opx_addr src_opx_addr,
			     const fi_addr_t ctx_src_addr)
{
	const uint64_t origin_tag_and_not_ignore = origin_tag & ~ignore_bits;
	const uint64_t answer = (origin_tag_and_not_ignore == target_tag) &&
				((ctx_src_addr == FI_ADDR_UNSPEC) ||
				 (origin_uid_fi == src_opx_addr.uid.fi));
	return answer;
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_ue_packet *fi_opx_match_find_uepkt_linear(struct fi_opx_match_ue_hash *ue_hash,
								const uint64_t target_tag,
								const uint64_t ignore_bits,
								const union fi_opx_addr src_opx_addr,
								const fi_addr_t ctx_src_addr,
								struct fi_opx_debug_counters *debug_counters)
{
	struct fi_opx_hfi1_ue_packet *uepkt = ue_hash->ue.head;
	assert(uepkt);

	FI_OPX_DEBUG_COUNTERS_INC(debug_counters->match.ue_hash_linear_searches);

	while (uepkt && !fi_opx_match_packet(uepkt->tag, uepkt->origin_uid_fi,
					     target_tag, ignore_bits,
					     src_opx_addr, ctx_src_addr)) {
		FI_OPX_DEBUG_COUNTERS_INC(debug_counters->match.ue_hash_linear_misses);
		uepkt = uepkt->next;
	}

	FI_OPX_DEBUG_COUNTERS_INC_COND(uepkt, debug_counters->match.ue_hash_linear_hits);
	FI_OPX_DEBUG_COUNTERS_INC_COND(!uepkt, debug_counters->match.ue_hash_linear_not_found);
	return uepkt;
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_ue_packet *fi_opx_match_find_uepkt_by_tag(struct fi_opx_match_ue_hash *ue_hash,
								const uint64_t hash_index,
								const uint64_t target_tag,
								const uint64_t ignore_bits,
								const union fi_opx_addr src_opx_addr,
								const fi_addr_t ctx_src_addr,
								struct fi_opx_debug_counters *debug_counters)
{
	struct fi_opx_hfi1_ue_packet *uepkt = ue_hash->tag_ht[hash_index].head;
	assert(uepkt);

	FI_OPX_DEBUG_COUNTERS_INC(debug_counters->match.ue_hash_tag_searches);

	while (uepkt && !fi_opx_match_packet(uepkt->tag, uepkt->origin_uid_fi,
					     target_tag, ignore_bits,
					     src_opx_addr, ctx_src_addr)) {
		FI_OPX_DEBUG_COUNTERS_INC(debug_counters->match.ue_hash_tag_misses);
		uepkt = uepkt->tag_ht.next;
	}
	FI_OPX_DEBUG_COUNTERS_INC_COND(uepkt, debug_counters->match.ue_hash_tag_hits);
	FI_OPX_DEBUG_COUNTERS_INC_COND(!uepkt, debug_counters->match.ue_hash_tag_not_found);

	return uepkt;
}

__OPX_FORCE_INLINE__
struct fi_opx_hfi1_ue_packet *fi_opx_match_find_uepkt(struct fi_opx_match_ue_hash *ue_hash,
							const union fi_opx_context *context,
							struct fi_opx_debug_counters *debug_counters)
{
	if (!ue_hash->ue.head) {
		return NULL;
	}

	const union fi_opx_addr src_opx_addr = {.fi = context->src_addr};

	const uint64_t ignore_bits = context->ignore;
	const uint64_t target_tag = context->tag & ~ignore_bits;

	/* If ignore bits don't overlap our hash bits, we can potentially search by tag */
	if (!(ignore_bits & FI_OPX_MATCH_UE_HT_MASK)) {
		const uint64_t hashed_tag = target_tag & FI_OPX_MATCH_UE_HT_MASK;
		return fi_opx_match_find_uepkt_by_tag(ue_hash,
							hashed_tag,
							target_tag,
							ignore_bits,
							src_opx_addr,
							context->src_addr,
							debug_counters);
	}

	return fi_opx_match_find_uepkt_linear(ue_hash,
						target_tag,
						ignore_bits,
						src_opx_addr,
						FI_ADDR_UNSPEC,
						debug_counters);
}

#endif
