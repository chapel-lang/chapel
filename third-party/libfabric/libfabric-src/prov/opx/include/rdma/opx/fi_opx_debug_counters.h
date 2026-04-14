/*
 * Copyright (C) 2021-2025 Cornelis Networks.
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

#include <locale.h>
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
#define OPX_DEBUG_COUNTERS_RHF
#define OPX_DEBUG_COUNTERS_HMEM
#endif

#if !defined(OPX_DEBUG_COUNTERS_RELIABILITY_PING) && defined(OPX_DUMP_PINGS)
#define OPX_DEBUG_COUNTERS_RELIABILITY_PING
#endif

#define OPX_COUNTERS_WRITEV_MAX (129)
#define OPX_COUNTERS_NS_PER_SEC (1000000000ul)
#define OPX_COUNTERS_MB		(1000000.0)
#define OPX_COUNTERS_MIB	(1048756.0)

enum opx_counters_unit {
	OPX_COUNTERS_UNIT_NONE = 0,
	OPX_COUNTERS_UNIT_NS,
	OPX_COUNTERS_UNIT_BW_MB,
	OPX_COUNTERS_UNIT_LAST
};

static const char *OPX_COUNTERS_UNIT_SFX[] = {
	[OPX_COUNTERS_UNIT_NONE]  = "",
	[OPX_COUNTERS_UNIT_NS]	  = "ns",
	[OPX_COUNTERS_UNIT_BW_MB] = "MB/s",
};

struct opx_counters_measurement {
	uint64_t n;
	uint64_t sum;
	uint64_t min;
	uint64_t max;
	uint64_t avg;
};

static inline void opx_counters_print_unit(pid_t pid, char *name, uint64_t value, enum opx_counters_unit unit)
{
#ifdef OPX_DEBUG_COUNTERS_NONZERO
	if (value)
#endif
		fprintf(stderr, "(%d) ### %-50s %'15lu %s\n", pid, name, value, OPX_COUNTERS_UNIT_SFX[unit]);
}

#define OPX_COUNTERS_PRINT_UNIT(pid, name, unit) opx_counters_print_unit(pid, #name, counters->name, unit)
#define FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, name) \
	opx_counters_print_unit(pid, #name, counters->name, OPX_COUNTERS_UNIT_NONE)
#define FI_OPX_DEBUG_COUNTERS_PRINT_VAL(pid, name) opx_counters_print_unit(pid, #name, name, OPX_COUNTERS_UNIT_NONE)

#define FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER_ARR(pid, name, size)                                    \
	{                                                                                           \
		char n[32];                                                                         \
		for (int i = 0; i < (size); ++i) {                                                  \
			sprintf(n, "%s[%d]", #name, i);                                             \
			opx_counters_print_unit(pid, n, counters->name[i], OPX_COUNTERS_UNIT_NONE); \
		}                                                                                   \
	}

#define OPX_COUNTERS_PRINT_MEASURE_BW(pid, name)                                                                      \
	do {                                                                                                          \
		fprintf(stderr, "(%d) ### %-50s %'15.2f MB/s (%'.2f MiB/s)\n", pid, #name ".min",                     \
			(double) ((counters->name.min == UINT64_MAX) ? 0.0 : (counters->name.min / OPX_COUNTERS_MB)), \
			(double) ((counters->name.min == UINT64_MAX) ? 0.0 :                                          \
								       (counters->name.min / OPX_COUNTERS_MIB)));     \
		fprintf(stderr, "(%d) ### %-50s %'15.2f MB/s (%'.2f MiB/s)\n", pid, #name ".max",                     \
			(counters->name.max / OPX_COUNTERS_MB), (counters->name.max / OPX_COUNTERS_MIB));             \
		fprintf(stderr, "(%d) ### %-50s %'15.2f MB/s (%'.2f MiB/s)\n", pid, #name ".avg",                     \
			(counters->name.avg / OPX_COUNTERS_MB), (counters->name.avg / OPX_COUNTERS_MIB));             \
	} while (0)

#define OPX_COUNTERS_PRINT_MEASURE(pid, name, unit)                                 \
	do {                                                                        \
		if (counters->name.n == 0) {                                        \
			counters->name.min = 0;                                     \
			counters->name.avg = 0;                                     \
		} else {                                                            \
			counters->name.avg = counters->name.sum / counters->name.n; \
		}                                                                   \
		OPX_COUNTERS_PRINT_UNIT(pid, name.n, OPX_COUNTERS_UNIT_NONE);       \
		OPX_COUNTERS_PRINT_UNIT(pid, name.min, unit);                       \
		OPX_COUNTERS_PRINT_UNIT(pid, name.max, unit);                       \
		OPX_COUNTERS_PRINT_UNIT(pid, name.avg, unit);                       \
		OPX_COUNTERS_PRINT_UNIT(pid, name.sum, unit);                       \
	} while (0)

#define OPX_COUNTERS_PRINT_MEASURE_ARR(pid, name, size, unit)                                                    \
	do {                                                                                                     \
		char indexed_name[32];                                                                           \
		for (int i = 0; i < (size); ++i) {                                                               \
			if (counters->name[i].n == 0) {                                                          \
				counters->name[i].min = 0;                                                       \
				counters->name[i].avg = 0;                                                       \
			} else {                                                                                 \
				counters->name[i].avg = counters->name[i].sum / counters->name[i].n;             \
			}                                                                                        \
			sprintf(indexed_name, "%s[%d].n", #name, i);                                             \
			opx_counters_print_unit(pid, indexed_name, counters->name[i].n, OPX_COUNTERS_UNIT_NONE); \
			sprintf(indexed_name, "%s[%d].min", #name, i);                                           \
			opx_counters_print_unit(pid, indexed_name, counters->name[i].min, unit);                 \
			sprintf(indexed_name, "%s[%d].max", #name, i);                                           \
			opx_counters_print_unit(pid, indexed_name, counters->name[i].max, unit);                 \
			sprintf(indexed_name, "%s[%d].avg", #name, i);                                           \
			opx_counters_print_unit(pid, indexed_name, counters->name[i].avg, unit);                 \
			sprintf(indexed_name, "%s[%d].sum", #name, i);                                           \
			opx_counters_print_unit(pid, indexed_name, counters->name[i].sum, unit);                 \
		}                                                                                                \
	} while (0)

struct fi_opx_debug_counters_send {
	uint64_t inject;
	uint64_t eager;
	uint64_t eager_noncontig;
	uint64_t mp_eager;
	uint64_t rzv;
	uint64_t rzv_noncontig;
};

struct fi_opx_debug_counters_recv {
	uint64_t inject;
	uint64_t eager;
	uint64_t mp_eager;
	uint64_t rzv;
};

struct fi_opx_debug_counters_txr {
	struct fi_opx_debug_counters_send send;
	struct fi_opx_debug_counters_recv recv;
};

struct fi_opx_debug_counters_typed_txr {
	/* 0 == tag, 1 == msg */
	struct fi_opx_debug_counters_txr kind[2];
};

#define FI_OPX_DEBUG_COUNTERS_PRINT_TXR(pid, prefix)                                   \
	do {                                                                           \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.inject);          \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.eager);           \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.eager_noncontig); \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.mp_eager);        \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.rzv);             \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.send.rzv_noncontig);   \
                                                                                       \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.inject);          \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.eager);           \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.mp_eager);        \
		FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, prefix.recv.rzv);             \
	} while (0)

struct fi_opx_debug_counters {
	union fi_opx_timer_stamp timestamp; /*  2 qws =  16 bytes */
	union fi_opx_timer_state timer;	    /*  5 bytes           */
	uint8_t			 padding[3];

	struct {
		uint64_t send_first_packets;
		uint64_t send_first_eagain_credits;
		uint64_t send_first_eagain_reliability;
		uint64_t send_nth_packets;
		uint64_t send_nth_eagain_credits;
		uint64_t send_nth_eagain_reliability;

		uint64_t recv_max_ue_queue_length;
		uint64_t recv_max_mq_queue_length;
		uint64_t recv_first_packets;
		uint64_t recv_nth_packets;
		uint64_t recv_completed_process_context;
		uint64_t recv_completed_eager_first;
		uint64_t recv_completed_eager_nth;
		uint64_t recv_truncation;
		uint64_t recv_nth_no_match;
		uint64_t recv_nth_match;
	} mp_eager;

	struct {
		uint64_t acks_sent;
		uint64_t acks_preemptive_sent;
		uint64_t acks_received;
		uint64_t acks_ignored;

		uint64_t nacks_sent;
		uint64_t nacks_preemptive_sent;
		uint64_t nacks_received;
		uint64_t nacks_ignored;

		uint64_t pings_sent;
		uint64_t pings_received;

		uint64_t uepkt_received;
		uint64_t throttled_max_outstanding;
		uint64_t throttled_nacks;
		uint64_t throttled;
	} reliability_ping;

	struct {
		uint64_t writev_count;
		uint64_t writev_time_ns_total;
		uint64_t writev_time_ns_min;
		uint64_t writev_time_ns_max;
		uint64_t writev_time_ns_avg;
		uint64_t nontid_requests;
		uint64_t tid_requests;
		uint64_t replay_requests;
		uint64_t proc_reqs_calls;
		uint64_t proc_reqs_no_fill_slots;
		uint64_t proc_reqs_more_avail;
		uint64_t eagain_fill_index;
		uint64_t eagain_iov_limit;
		uint64_t eagain_psn;
		uint64_t eagain_replay;
		uint64_t eagain_sdma_we_none_free;
		uint64_t eagain_sdma_we_max_used;
		uint64_t eagain_pending_writev;
		uint64_t eagain_pending_sdma_completion;
		uint64_t eagain_pending_dc;

		/**
		 * @brief Record instances where we receive an ACK for a packet
		 * before the driver has marked the SDMA send as complete.
		 * This will also record the elapsed time between receiving the
		 * ACK and when we first see the SDMA send marked complete.
		 */
		struct opx_counters_measurement early_acks_delta;

		/**
		 * @brief The calculated send bandwidth we get via SDMA.
		 * Note that this calculation is an estimate only.
		 */
		struct opx_counters_measurement send_bw;

		/**
		 * @brief The number of calls to and time spent in writev(),
		 * based on the number of IOVs we passed to writev().
		 */
		struct opx_counters_measurement writev[OPX_COUNTERS_WRITEV_MAX];

		/**
		 * @brief The aggregate number of calls to and time spent in writev(),
		 * regardless of the number of IOVs we passed to writev().
		 */
		struct opx_counters_measurement writev_all;

		/**
		 * @brief The number of SDMA requests made, and time taken
		 * from when we initiated the send to when we see the SDMA
		 * send as marked complete.
		 */
		struct opx_counters_measurement completion;
	} sdma;

	struct {
		uint64_t tid_updates;
		uint64_t tid_update_fail;
		uint64_t tid_update_success;
		uint64_t tid_update_success_partial;
		uint64_t tid_rcv_pkts;
		uint64_t tid_rcv_pkts_replays;
		uint64_t rts_tid_ineligible;
		uint64_t rts_tid_eligible;
		uint64_t rts_fallback_eager_immediate;
		uint64_t rts_fallback_eager_reg_rzv;
		uint64_t rts_tid_setup_retries;
		uint64_t rts_tid_setup_success;
		uint64_t tid_buckets[4];
		uint64_t first_tidpair_minlen;
		uint64_t first_tidpair_maxlen;
		uint64_t first_tidpair_minoffset;
		uint64_t first_tidpair_maxoffset;
		uint64_t generation_wrap;
		uint64_t tid_cache_flush_not_lru;
		uint64_t tid_cache_flush_not_lru_helped;
		uint64_t tid_cache_flush_lru;
		uint64_t tid_cache_flush_lru_helped;
		uint64_t tid_cache_full;
		uint64_t tid_cache_miss;
		uint64_t tid_cache_hit;
		uint64_t tid_cache_found_entry_in_use;
		uint64_t tid_cache_found_entry_invalid;
		uint64_t tid_cache_overlap_left;
		uint64_t tid_cache_overlap_right;
		uint64_t reg_for_rzv_get_initial;
		uint64_t reg_for_rzv_get_remaining;
	} expected_receive;

	struct {
		uint64_t replay_rts;
		uint64_t replay_cts;
		uint64_t replay_rzv;
		uint64_t rzv_data_ooo;
	} reliability;

	struct {
		uint64_t total_searches;
		uint64_t total_misses;
		uint64_t total_hits;
		uint64_t total_not_found;

		uint64_t default_searches;
		uint64_t default_misses;
		uint64_t default_hits;
		uint64_t default_not_found;
		uint64_t default_max_length;

		uint64_t ue_hash_linear_searches;
		uint64_t ue_hash_linear_misses;
		uint64_t ue_hash_linear_hits;
		uint64_t ue_hash_linear_not_found;
		uint64_t ue_hash_linear_max_length;

		uint64_t ue_hash_tag_searches;
		uint64_t ue_hash_tag_misses;
		uint64_t ue_hash_tag_hits;
		uint64_t ue_hash_tag_not_found;
		uint64_t ue_hash_tag_max_length;
	} match;

	struct {
		uint64_t posted_recv_msg;
		uint64_t posted_recv_tag;
		uint64_t posted_multi_recv;

		uint64_t multi_recv_inject;
		uint64_t multi_recv_eager;
		uint64_t multi_recv_rzv_noncontig;
		uint64_t multi_recv_rzv_contig;

		/**
		 * @brief The number of Rendezvous RTS packets received that
		 * did not match to a posted receive, and were added to the
		 * unexpected packed (ue) queue for later matching, and the
		 * time spent waiting to be matched to a posted receive.
		 */
		struct opx_counters_measurement rzv_rts_pkt_early;

		/**
		 * @brief The number of posted receives that did not match
		 * any rendezvous RTS packets on the unexpected queue, and
		 * the time spent waiting for the RTS packet to arrive and match.
		 */
		struct opx_counters_measurement rzv_rts_rcv_early;
	} recv;

	struct {
		/* Packet type on error */
		uint64_t rcvtypeexp;
		uint64_t rcvtypeegr;
		uint64_t rcvtypeoth;

		/* Errors */
		uint64_t error;
		uint64_t icrcerr;
		uint64_t lenerr;
		uint64_t eccerr;
		uint64_t tiderr;
		uint64_t dcerr;
		uint64_t dcuncerr;
		uint64_t khdrlenerr;
		uint64_t rcvtypeerr;
		uint64_t tidbypasserr;
		uint64_t crkerr;
		uint64_t crkuncerr;
		uint64_t flowgenerr;
		uint64_t flowseqerr;
	} rhf;

	struct {
		struct fi_opx_debug_counters_typed_txr hfi;
		struct fi_opx_debug_counters_typed_txr intranode;
		// hmem - leaving "intranode" until we design hmem sr-iov
		// and understand sr-iov+hmem/shm/intranode/gdrcopy/ipc

		uint64_t dput_rzv_intranode;
		uint64_t dput_rzv_pio;
		uint64_t dput_rzv_sdma;
		uint64_t dput_rzv_tid;

		uint64_t posted_recv_msg;
		uint64_t posted_recv_tag;

		uint64_t rma_read_intranode;
		uint64_t rma_read_hfi;
		uint64_t rma_write_intranode;
		uint64_t rma_write_hfi;
		uint64_t rma_atomic_fetch_hfi;
		uint64_t rma_atomic_fetch_intranode;
		uint64_t rma_atomic_cmp_fetch_hfi;
		uint64_t rma_atomic_cmp_fetch_intranode;

		uint64_t tid_update;
		uint64_t tid_recv;
	} hmem;
};

static inline void fi_opx_dump_mem(void *address, uint64_t lenth)
{
	fprintf(stderr, "### Dumping %lu bytes of memory at addr %p...\n", lenth, address);

	fprintf(stderr, "Address                   QW0              QW1              QW2            QW3\n");
	uint64_t *mem = (uint64_t *) address;
	for (uint64_t i = 0; i < lenth; i += 32) {
		fprintf(stderr, "%016lX    %016lX  %016lX  %016lX  %016lX\n", *mem, mem[0], mem[1], mem[2], mem[3]);
		mem += 4;
	}
	fprintf(stderr, "#############################################\n");
}

static inline void fi_opx_debug_counters_print(struct fi_opx_debug_counters *counters)
{
	pid_t pid = getpid();
	sleep(pid % 10);
	fprintf(stderr, "(%d) ### DEBUG COUNTERS ###\n", pid);

#ifdef OPX_DEBUG_COUNTERS_MP_EAGER
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_first_packets);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_first_eagain_credits);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_first_eagain_reliability);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_nth_packets);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_nth_eagain_credits);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, mp_eager.send_nth_eagain_reliability);

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
	fprintf(stderr, "(%d) ### mp_eager_recv_truncation                   %lu\n", pid,
		counters->mp_eager.recv_truncation);

	if (counters->mp_eager.recv_first_packets != total_completed) {
		fprintf(stderr, "!!!!! TOTAL COMPLETED %lu != mp_eager.first %lu!!!!! \n", total_completed,
			counters->mp_eager.recv_first_packets);
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
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.nontid_requests);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.tid_requests);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.replay_requests);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.proc_reqs_calls);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.proc_reqs_no_fill_slots);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.proc_reqs_more_avail);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_fill_index);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_iov_limit);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_psn);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_replay);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_sdma_we_none_free);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_sdma_we_max_used);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_pending_writev);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_pending_sdma_completion);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, sdma.eagain_pending_dc);
	OPX_COUNTERS_PRINT_MEASURE_ARR(pid, sdma.writev, OPX_COUNTERS_WRITEV_MAX, OPX_COUNTERS_UNIT_NS);
	OPX_COUNTERS_PRINT_MEASURE(pid, sdma.writev_all, OPX_COUNTERS_UNIT_NS);
	OPX_COUNTERS_PRINT_MEASURE(pid, sdma.completion, OPX_COUNTERS_UNIT_NS);
	OPX_COUNTERS_PRINT_MEASURE_BW(pid, sdma.send_bw);
	OPX_COUNTERS_PRINT_MEASURE(pid, sdma.early_acks_delta, OPX_COUNTERS_UNIT_NS);
#endif

#ifdef OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_updates);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_update_fail);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_update_success);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_update_success_partial);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_rcv_pkts);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_rcv_pkts_replays);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_ineligible);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_eligible);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_fallback_eager_immediate);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_fallback_eager_reg_rzv);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_setup_retries);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.rts_tid_setup_success);
	uint64_t rts_sum = counters->expected_receive.rts_fallback_eager_immediate +
			   counters->expected_receive.rts_fallback_eager_reg_rzv +
			   counters->expected_receive.rts_tid_setup_success;
	if (rts_sum != counters->expected_receive.rts_tid_eligible) {
		fprintf(stderr,
			"(%d) ### WARN: rts_tid_eligible (%lu) != SUM(rts_tid_setup_success + rts_fallback*) (%lu)! Accounting error?\n\n",
			pid, counters->expected_receive.rts_tid_eligible, rts_sum);
	}
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER_ARR(pid, expected_receive.tid_buckets, 4);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_minlen);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_maxlen);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_minoffset);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.first_tidpair_maxoffset);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.generation_wrap);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_flush_not_lru);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_flush_not_lru_helped);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_flush_lru);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_flush_lru_helped);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_full);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_hit);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_miss);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_found_entry_in_use);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_found_entry_invalid);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_overlap_left);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.tid_cache_overlap_right);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.reg_for_rzv_get_initial);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, expected_receive.reg_for_rzv_get_remaining);
#endif

#ifdef OPX_DEBUG_COUNTERS_RECV
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.posted_recv_msg);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.posted_recv_tag);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.posted_multi_recv);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_inject);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_eager);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_rzv_noncontig);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, recv.multi_recv_rzv_contig);
	OPX_COUNTERS_PRINT_MEASURE(pid, recv.rzv_rts_rcv_early, OPX_COUNTERS_UNIT_NS);
	OPX_COUNTERS_PRINT_MEASURE(pid, recv.rzv_rts_pkt_early, OPX_COUNTERS_UNIT_NS);
#endif

#ifdef OPX_DEBUG_COUNTERS_RHF
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.rcvtypeexp);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.rcvtypeegr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.rcvtypeoth);

	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.error);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.icrcerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.lenerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.eccerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.tiderr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.dcerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.dcuncerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.khdrlenerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.rcvtypeerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.tidbypasserr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.rcvtypeoth);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.crkerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.crkuncerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.flowgenerr);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, rhf.flowseqerr);
#endif

#ifdef OPX_DEBUG_COUNTERS_RELIABILITY
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.replay_rts);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.replay_cts);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.replay_rzv);
	FI_OPX_DEBUG_COUNTERS_PRINT_COUNTER(pid, reliability.rzv_data_ooo);
#endif

#ifdef OPX_DEBUG_COUNTERS_MATCH
	uint64_t total_searches = counters->match.default_searches + counters->match.ue_hash_linear_searches +
				  counters->match.ue_hash_tag_searches;

	uint64_t total_misses = counters->match.default_misses + counters->match.ue_hash_linear_misses +
				counters->match.ue_hash_tag_misses;

	uint64_t total_hits =
		counters->match.default_hits + counters->match.ue_hash_linear_hits + counters->match.ue_hash_tag_hits;

	uint64_t total_not_found = counters->match.default_not_found + counters->match.ue_hash_linear_not_found +
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

static inline void fi_opx_debug_counters_init(struct fi_opx_debug_counters *counters)
{
	setlocale(LC_NUMERIC, "en_US.UTF-8");
	memset(counters, 0, sizeof(struct fi_opx_debug_counters));

	/* Set min values to UINT64_MAX so they'll correctly pick up new min values */
	for (int i = 0; i < OPX_COUNTERS_WRITEV_MAX; i++) {
		counters->sdma.writev[i].min = UINT64_MAX;
	}
	counters->sdma.writev_all.min	     = UINT64_MAX;
	counters->sdma.send_bw.min	     = UINT64_MAX;
	counters->sdma.early_acks_delta.min  = UINT64_MAX;
	counters->sdma.completion.min	     = UINT64_MAX;
	counters->recv.rzv_rts_rcv_early.min = UINT64_MAX;
	counters->recv.rzv_rts_pkt_early.min = UINT64_MAX;

	fi_opx_timer_init(&counters->timer);
	opx_debug_sig_counters = counters;
	signal(SIGUSR1, opx_debug_counters_handle_sig);
}

#if defined(OPX_DEBUG_COUNTERS_MP_EAGER) || defined(OPX_DEBUG_COUNTERS_RELIABILITY_PING) || \
	defined(OPX_DEBUG_COUNTERS_RELIABILITY) || defined(OPX_DEBUG_COUNTERS_SDMA) ||      \
	defined(OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE) || defined(OPX_DEBUG_COUNTERS_RECV) || \
	defined(OPX_DEBUG_COUNTERS_RHF) || defined(OPX_DEBUG_COUNTERS_HMEM) || defined(OPX_DEBUG_COUNTERS_MATCH)

#define FI_OPX_DEBUG_COUNTERS_DECLARE_COUNTERS struct fi_opx_debug_counters debug_counters
#define FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(x)   uint64_t x = 0
#define FI_OPX_DEBUG_COUNTERS_INIT(x)	       fi_opx_debug_counters_init(&(x))
#define FI_OPX_DEBUG_COUNTERS_PRINT(x)	       fi_opx_debug_counters_print(&(x))
#define FI_OPX_DEBUG_COUNTERS_INC(x)	       ++(x)
#define FI_OPX_DEBUG_COUNTERS_INC_N(n, x)      (x += n)
#define FI_OPX_DEBUG_COUNTERS_MAX_OF(x, y)     ((x) = MAX((x), (y)))
#define FI_OPX_DEBUG_COUNTERS_MIN_OF(x, y)     ((x) = MIN((x), (y)))
#define FI_OPX_DEBUG_COUNTERS_INC_COND(cond, x)         \
	do {                                            \
		if (cond) {                             \
			FI_OPX_DEBUG_COUNTERS_INC((x)); \
		}                                       \
                                                        \
	} while (0)
#define FI_OPX_DEBUG_COUNTERS_INC_COND_N(cond, n, x)           \
	do {                                                   \
		if (cond) {                                    \
			FI_OPX_DEBUG_COUNTERS_INC_N((n), (x)); \
		}                                              \
                                                               \
	} while (0)

#define FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep) (&((opx_ep)->debug_counters))
#define OPX_COUNTERS_RECORD_MEASURE(_val, _target)     \
	do {                                           \
		_target.n++;                           \
		_target.sum += _val;                   \
		_target.min = MIN(_target.min, _val);  \
		_target.max = MAX(_target.max, _val);  \
		_target.avg = _target.sum / _target.n; \
	} while (0)

#define OPX_COUNTERS_TIME_NS(_target, _counters)                                                 \
	do {                                                                                     \
		_target = fi_opx_timer_now_ns(&((_counters)->timestamp), &((_counters)->timer)); \
                                                                                                 \
	} while (0)

#define OPX_COUNTERS_STORE_VAL(_target, _val) \
	do {                                  \
		_target = _val;               \
                                              \
	} while (0)
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
#define FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep) (NULL)
#define OPX_COUNTERS_RECORD_MEASURE(_val, _target)
#define OPX_COUNTERS_TIME_NS(_target, _counters)
#define OPX_COUNTERS_STORE_VAL(_target, _val)
#endif

#endif
