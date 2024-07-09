/*
 * Copyright (C) 2021-2024 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DEBUG_COUNTERS_H_
#define _FI_PROV_OPX_DEBUG_COUNTERS_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#ifdef OPX_DEBUG_COUNTERS
	#define OPX_DEBUG_COUNTERS_MP_EAGER
	#define OPX_DEBUG_COUNTERS_RELIABILITY_PING
	#define OPX_DEBUG_COUNTERS_RELIABILITY
	#define OPX_DEBUG_COUNTERS_SDMA
	#define OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
	#define OPX_DEBUG_COUNTERS_MATCH
	#define OPX_DEBUG_COUNTERS_RECV
	#define OPX_DEBUG_COUNTERS_HMEM
#endif

#if !defined(OPX_DEBUG_COUNTERS_RELIABILITY_PING) && defined(OPX_DUMP_PINGS)
	#define OPX_DEBUG_COUNTERS_RELIABILITY_PING
#endif

static inline
void fi_opx_debug_counters_print_counter(pid_t pid, char *name, uint64_t value)
{
#ifdef OPX_DEBUG_COUNTERS_NONZERO
	if (value)
#endif
		fprintf(stderr, "(%d) ### %-50s %lu\n", pid, name, value);
}

#define FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, name) fi_opx_debug_counters_print_counter(pid, #name, counters->name)
#define FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, name) fi_opx_debug_counters_print_counter(pid, #name, name)

#define FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER_ARR(pid, name, size)		\
{										\
	char n[32];								\
	for (int i = 0; i < (size); ++i) {					\
		sprintf(n, "%s[%d]", #name, i);					\
		fi_opx_debug_counters_print_counter(pid, n, counters->name[i]);	\
	}									\
}

struct fi_opx_debug_counters_send {
	uint64_t	inject;
	uint64_t	eager;
	uint64_t	eager_noncontig;
	uint64_t	mp_eager;
	uint64_t	rzv;
	uint64_t	rzv_noncontig;
};

struct fi_opx_debug_counters_recv {
	uint64_t	inject;
	uint64_t	eager;
	uint64_t	mp_eager;
	uint64_t	rzv;
};

struct fi_opx_debug_counters_txr {

	struct fi_opx_debug_counters_send	send;
	struct fi_opx_debug_counters_recv	recv;
};

struct fi_opx_debug_counters_typed_txr {
	/* 0 == tag, 1 == msg */
	struct fi_opx_debug_counters_txr	kind[2];
};

#define FI_OPX_DEBUG_COUNTERS_PRINT_TXR(pid, prefix)					\
	do {										\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.inject);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.eager);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.eager_noncontig);	\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.mp_eager);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.rzv);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.rzv_noncontig);	\
											\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.inject);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.eager);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.mp_eager);		\
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.rzv);		\
	} while(0)

struct fi_opx_debug_counters {
	struct {
		uint64_t	send_first_packets;
		uint64_t	send_nth_packets;
		uint64_t	send_first_force_cr;
		uint64_t	send_nth_force_cr;
		uint64_t	send_fall_back_to_rzv;
		uint64_t	send_full_replay_buffer_rx_poll;

		uint64_t	recv_max_ue_queue_length;
		uint64_t	recv_max_mq_queue_length;
		uint64_t	recv_first_packets;
		uint64_t	recv_nth_packets;
		uint64_t	recv_completed_process_context;
		uint64_t	recv_completed_eager_first;
		uint64_t	recv_completed_eager_nth;
		uint64_t	recv_truncation;
		uint64_t	recv_nth_no_match;
		uint64_t	recv_nth_match;
	} mp_eager;

	struct {
		uint64_t	acks_sent;
		uint64_t	acks_preemptive_sent;
		uint64_t	acks_received;
		uint64_t	acks_ignored;

		uint64_t	nacks_sent;
		uint64_t	nacks_preemptive_sent;
		uint64_t	nacks_received;
		uint64_t	nacks_ignored;

		uint64_t	pings_sent;
		uint64_t	pings_received;

		uint64_t	uepkt_received;
		uint64_t	throttled_max_outstanding;
		uint64_t	throttled_nacks;
		uint64_t	throttled;
	} reliability_ping;

	struct {
		uint64_t	writev_calls[33];
		uint64_t	total_requests;
		uint64_t	eagain_fill_index;
		uint64_t	eagain_psn;
		uint64_t	eagain_replay;
		uint64_t	eagain_sdma_we_none_free;
		uint64_t	eagain_sdma_we_max_used;
		uint64_t	eagain_pending_writev;
		uint64_t	eagain_pending_dc;
	} sdma;

	struct {
		uint64_t	tid_updates;
		uint64_t	tid_resource_limit;
		uint64_t	tid_resource_limit_length_chunk_short;
		uint64_t	tid_resource_limit_length_chunk_long;
		uint64_t	tid_resource_limit_tidcnt_chunk_zero;
		uint64_t	tid_invalidate_needed;
		uint64_t	tid_rcv_pkts;
		uint64_t	tid_rcv_pkts_replays;
		uint64_t	rts_tid_ineligible;
		uint64_t	rts_tid_eligible;
		uint64_t	rts_fallback_eager_immediate;
		uint64_t	rts_fallback_eager_misaligned_thrsh;
		uint64_t	rts_fallback_eager_reg_rzv;
		uint64_t	rts_tid_setup_retries;
		uint64_t	rts_tid_setup_retry_success;
		uint64_t	rts_tid_setup_success;
		uint64_t	tid_buckets[4];
		uint64_t	first_tidpair_minlen;
		uint64_t	first_tidpair_maxlen;
		uint64_t	first_tidpair_minoffset;
		uint64_t	first_tidpair_maxoffset;
		uint64_t	generation_wrap;
	} expected_receive;

	struct {
		uint64_t	replay_rts;
		uint64_t	replay_cts;
		uint64_t	replay_rzv;
	} reliability;

	struct {
		uint64_t	total_searches;
		uint64_t	total_misses;
		uint64_t	total_hits;
		uint64_t	total_not_found;

		uint64_t	default_searches;
		uint64_t	default_misses;
		uint64_t	default_hits;
		uint64_t	default_not_found;
		uint64_t	default_max_length;

		uint64_t	ue_hash_linear_searches;
		uint64_t	ue_hash_linear_misses;
		uint64_t	ue_hash_linear_hits;
		uint64_t	ue_hash_linear_not_found;
		uint64_t	ue_hash_linear_max_length;

		uint64_t	ue_hash_tag_searches;
		uint64_t	ue_hash_tag_misses;
		uint64_t	ue_hash_tag_hits;
		uint64_t	ue_hash_tag_not_found;
		uint64_t	ue_hash_tag_max_length;
	} match;

	struct {
		uint64_t	posted_recv_msg;
		uint64_t	posted_recv_tag;
		uint64_t	posted_multi_recv;

		uint64_t	multi_recv_inject;
		uint64_t	multi_recv_eager;
		uint64_t	multi_recv_rzv_noncontig;
		uint64_t	multi_recv_rzv_contig;
	} recv;

	struct {
		struct fi_opx_debug_counters_typed_txr	hfi;
		struct fi_opx_debug_counters_typed_txr	intranode;

		uint64_t				dput_rzv_intranode;
		uint64_t				dput_rzv_pio;
		uint64_t				dput_rzv_sdma;
		uint64_t				dput_rzv_tid;

		uint64_t				posted_recv_msg;
		uint64_t				posted_recv_tag;

		uint64_t				rma_read_intranode;
		uint64_t				rma_read_hfi;
		uint64_t				rma_write_intranode;
		uint64_t				rma_write_hfi;
		uint64_t				rma_atomic_fetch_hfi;
		uint64_t				rma_atomic_fetch_intranode;
		uint64_t				rma_atomic_cmp_fetch_hfi;
		uint64_t				rma_atomic_cmp_fetch_intranode;

		uint64_t				tid_update;
		uint64_t				tid_recv;
	} hmem;
};

static inline
void fi_opx_dump_mem(void *address, uint64_t lenth) {
	fprintf(stderr, "### Dumping %lu bytes of memory at addr %p...\n", lenth, address);

	fprintf(stderr, "Address                   QW0              QW1              QW2            QW3\n");
	uint64_t *mem = (uint64_t *) address;
	for (uint64_t i = 0; i < lenth; i += 32) {
		fprintf(stderr, "%016lX    %016lX  %016lX  %016lX  %016lX\n", *mem, mem[0], mem[1], mem[2], mem[3]);
		mem += 4;
	}
	fprintf(stderr, "#############################################\n");
}

static inline
void fi_opx_debug_counters_print(struct fi_opx_debug_counters *counters)
{
	pid_t pid = getpid();
	sleep(pid % 10);
	fprintf(stderr, "(%d) ### DEBUG COUNTERS ###\n", pid);

	#ifdef OPX_DEBUG_COUNTERS_MP_EAGER
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_first_packets);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_nth_packets);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_first_force_cr);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_nth_force_cr);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_fall_back_to_rzv);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_full_replay_buffer_rx_poll);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_max_ue_queue_length);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_max_mq_queue_length);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_first_packets);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_nth_packets);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_completed_process_context);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_completed_eager_first);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_completed_eager_nth);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_truncation);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_nth_no_match);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.recv_nth_match);

		uint64_t total_completed = counters->mp_eager.recv_completed_process_context +
					counters->mp_eager.recv_completed_eager_first +
					counters->mp_eager.recv_completed_eager_nth;
		fprintf(stderr, "(%d) ### mp_eager_recv_total_completed -----------> %lu\n", pid, total_completed);
		fprintf(stderr, "(%d) ### mp_eager_recv_truncation                   %lu\n", pid, counters->mp_eager.recv_truncation);

		if (counters->mp_eager.recv_first_packets != total_completed) {
			fprintf(stderr, "!!!!! TOTAL COMPLETED %lu != mp_eager.first %lu!!!!! \n",
					total_completed, counters->mp_eager.recv_first_packets);
		}
	#endif

	#ifdef OPX_DEBUG_COUNTERS_RELIABILITY_PING
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.acks_sent);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.acks_preemptive_sent);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.acks_received);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.acks_ignored);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.nacks_sent);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.nacks_preemptive_sent);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.nacks_received);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.nacks_ignored);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.pings_sent);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.pings_received);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.uepkt_received);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.throttled_max_outstanding);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.throttled_nacks);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability_ping.throttled);
	#endif

	#ifdef OPX_DEBUG_COUNTERS_SDMA
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.total_requests);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_fill_index);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_psn);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_replay);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_sdma_we_none_free);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_sdma_we_max_used);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_pending_writev);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_pending_dc);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER_ARR(pid, sdma.writev_calls, 33);
	#endif

	#ifdef OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_updates);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_resource_limit);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_resource_limit_length_chunk_short);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_resource_limit_tidcnt_chunk_zero);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_invalidate_needed);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_rcv_pkts);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_rcv_pkts_replays);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_ineligible);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_eligible);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_fallback_eager_immediate);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_fallback_eager_misaligned_thrsh);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_fallback_eager_reg_rzv);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_setup_retries);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_setup_retry_success);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_setup_success);
		uint64_t rts_sum = counters->expected_receive.rts_fallback_eager_immediate +
				   counters->expected_receive.rts_fallback_eager_misaligned_thrsh +
				   counters->expected_receive.rts_fallback_eager_reg_rzv +
				   counters->expected_receive.rts_tid_setup_success;
		if (rts_sum != counters->expected_receive.rts_tid_eligible) {
			fprintf(stderr,
				"(%d) ### WARN: rts_tid_eligible (%lu) != SUM(rts_tid_setup_success + rts_fallback*) (%lu)! Accounting error?\n\n",
				pid,
				counters->expected_receive.rts_tid_eligible, rts_sum);
		}
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER_ARR(pid, expected_receive.tid_buckets, 4);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_minlen);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_maxlen);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_minoffset);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_maxoffset);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.generation_wrap);
	#endif

	#ifdef OPX_DEBUG_COUNTERS_RECV
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.posted_recv_msg);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.posted_recv_tag);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.posted_multi_recv);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_inject);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_eager);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_rzv_noncontig);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_rzv_contig);
	#endif

	#ifdef OPX_DEBUG_COUNTERS_RELIABILITY
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.replay_rts);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.replay_cts);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.replay_rzv);
	#endif

	#ifdef OPX_DEBUG_COUNTERS_MATCH
		uint64_t total_searches = counters->match.default_searches +
					  counters->match.ue_hash_linear_searches +
					  counters->match.ue_hash_tag_searches;

		uint64_t total_misses = counters->match.default_misses +
					counters->match.ue_hash_linear_misses +
					counters->match.ue_hash_tag_misses;

		uint64_t total_hits = counters->match.default_hits +
				      counters->match.ue_hash_linear_hits +
				      counters->match.ue_hash_tag_hits;

		uint64_t total_not_found = counters->match.default_not_found +
					   counters->match.ue_hash_linear_not_found +
					   counters->match.ue_hash_tag_not_found;

		uint64_t avg_len_per_hit = total_hits ? (total_misses / total_hits) : 0;

		FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, total_searches);
		FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, total_misses);
		FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, total_hits);
		FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, total_not_found);
		FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, avg_len_per_hit);
		fprintf(stderr, "\n");

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.default_searches);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.default_misses);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.default_hits);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.default_not_found);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.default_max_length);
		fprintf(stderr, "\n");

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_linear_searches);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_linear_misses);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_linear_hits);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_linear_not_found);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_linear_max_length);
		fprintf(stderr, "\n");

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_tag_searches);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_tag_misses);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_tag_hits);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_tag_not_found);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, match.ue_hash_tag_max_length);
		fprintf(stderr, "\n");
	#endif

	#ifdef OPX_DEBUG_COUNTERS_HMEM
		const int tag = 0;
		const int msg = 1;
		FI_OPX_DEBUG_COUNTERS_PRINT_TXR(pid, hmem.hfi.kind[tag]);
		FI_OPX_DEBUG_COUNTERS_PRINT_TXR(pid, hmem.hfi.kind[msg]);
		FI_OPX_DEBUG_COUNTERS_PRINT_TXR(pid, hmem.intranode.kind[tag]);
		FI_OPX_DEBUG_COUNTERS_PRINT_TXR(pid, hmem.intranode.kind[msg]);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.dput_rzv_intranode);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.dput_rzv_pio);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.dput_rzv_sdma);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.dput_rzv_tid);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.posted_recv_msg);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.posted_recv_tag);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_read_intranode);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_read_hfi);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_write_intranode);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_write_hfi);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_atomic_fetch_intranode);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_atomic_fetch_hfi);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_atomic_cmp_fetch_intranode);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.rma_atomic_cmp_fetch_hfi);

		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.tid_update);
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, hmem.tid_recv);
	#endif
}

static struct fi_opx_debug_counters *opx_debug_sig_counters;

static void opx_debug_counters_handle_sig(int signum)
{
	fi_opx_debug_counters_print(opx_debug_sig_counters);
}

static inline
void fi_opx_debug_counters_init(struct fi_opx_debug_counters *counters) {
	memset(counters, 0, sizeof(struct fi_opx_debug_counters));
	opx_debug_sig_counters = counters;
	signal(SIGUSR1, opx_debug_counters_handle_sig);
}

#if defined(OPX_DEBUG_COUNTERS_MP_EAGER)		||		\
	defined(OPX_DEBUG_COUNTERS_RELIABILITY_PING)	||		\
	defined(OPX_DEBUG_COUNTERS_RELIABILITY) 	||		\
	defined(OPX_DEBUG_COUNTERS_SDMA)		||		\
	defined(OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE)	||		\
	defined(OPX_DEBUG_COUNTERS_RECV)		||		\
	defined(OPX_DEBUG_COUNTERS_HMEM)		||		\
	defined(OPX_DEBUG_COUNTERS_MATCH)

#define FI_OPX_DEBUG_COUNTERS_DECLARE_COUNTERS struct fi_opx_debug_counters debug_counters
#define FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(x) uint64_t x = 0
#define FI_OPX_DEBUG_COUNTERS_INIT(x) fi_opx_debug_counters_init(&(x))
#define FI_OPX_DEBUG_COUNTERS_PRINT(x) fi_opx_debug_counters_print(&(x))
#define FI_OPX_DEBUG_COUNTERS_INC(x) ++(x)
#define FI_OPX_DEBUG_COUNTERS_INC_N(n, x) (x += n)
#define FI_OPX_DEBUG_COUNTERS_MAX_OF(x, y) ((x) = MAX((x), (y)))
#define FI_OPX_DEBUG_COUNTERS_MIN_OF(x, y) ((x) = MIN((x), (y)))
#define FI_OPX_DEBUG_COUNTERS_INC_COND(cond, x)				\
	do {								\
		if (cond) {						\
			FI_OPX_DEBUG_COUNTERS_INC((x));			\
		}							\
									\
	} while(0)
#define FI_OPX_DEBUG_COUNTERS_INC_COND_N(cond, n, x)			\
	do {								\
		if (cond) {						\
			FI_OPX_DEBUG_COUNTERS_INC_N((n),(x));		\
		}							\
									\
	} while(0)

#define FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep)	(&((opx_ep)->debug_counters))
#else

#define FI_OPX_DEBUG_COUNTERS_DECLARE_COUNTERS
#define FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(x)
#define FI_OPX_DEBUG_COUNTERS_INIT(x)
#define FI_OPX_DEBUG_COUNTERS_PRINT(x)
#define FI_OPX_DEBUG_COUNTERS_INC(x)
#define FI_OPX_DEBUG_COUNTERS_INC_N(n, x)
#define FI_OPX_DEBUG_COUNTERS_INC_COND(cond, x)
#define FI_OPX_DEBUG_COUNTERS_INC_COND_N(cond, n, x)
#define FI_OPX_DEBUG_COUNTERS_MAX_OF(x, y)
#define FI_OPX_DEBUG_COUNTERS_MIN_OF(x, y)
#define FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep)	(NULL)
#endif

#endif
