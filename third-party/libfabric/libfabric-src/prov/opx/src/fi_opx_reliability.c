/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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

#include "rdma/opx/fi_opx_reliability.h"
#include "rdma/opx/fi_opx_compiler.h"

#include <pthread.h>
#include <unistd.h> /* sleep */
#include <inttypes.h>

#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_hfi1_sdma.h"
#include "rdma/opx/opx_tracer.h"
/* #define SKIP_RELIABILITY_PROTOCOL_RX_IMPL */
/* #define SKIP_RELIABILITY_PROTOCOL_TX_IMPL */

#include <execinfo.h>
#include <limits.h>

#ifdef OPX_DEBUG_COUNTERS_RELIABILITY_PING
/*
 * Names of ping stats for ping debug.
 */
enum ping_count_name {
	PRE_ACKS_SENT = 0,
	ACKS_SENT,
	ACKS_RECV,
	ACKS_IGNORED,
	PRE_NACKS_SENT,
	NACKS_SENT,
	NACKS_RECV,
	NACKS_IGNORED,
	PINGS_SENT,
	PINGS_RECV,
	/* ======== Stats below this line are counts only. ========
	   When adding new ones, keep UEPKT_RECV first and THROTTLED last! ===== */
	UEPKT_RECV,
	THROTTLED_MAX_OUTSTANDING,
	THROTTLED_NACKS,
	THROTTLED
};

#define NUM_PING_STATS_COUNT	THROTTLED + 1
#define NUM_PING_STATS_DETAILED UEPKT_RECV

// The length of all the ring buffers.
#define PDCCOUNT 4096

#ifdef OPX_DUMP_PINGS
typedef struct _pdc {
	union fi_opx_reliability_service_flow_key key;
	uint64_t				  start;
	uint64_t				  count;
} pdc;

typedef struct _pdc_list {
	unsigned index;
	pdc	 list[PDCCOUNT];
} pdc_list;

static pdc_list ping_count_pdcs[NUM_PING_STATS_DETAILED] = {};

#define INC_PDC(name, k, s, c)                                                   \
	do {                                                                     \
		pdc_list *pdc_l			= &ping_count_pdcs[name];        \
		pdc_l->list[pdc_l->index].key	= k;                             \
		pdc_l->list[pdc_l->index].start = s;                             \
		pdc_l->list[pdc_l->index].count = c;                             \
		pdc_l->index			= (pdc_l->index + 1) % PDCCOUNT; \
	} while (0)

#define DUMP_PINGS(pid, name)                                                                                     \
	do {                                                                                                      \
		fprintf(stderr, "(%d) Last " stringize(PDCCOUNT) " " #name "s:\n", pid);                          \
		pdc_list *pdc_l = &ping_count_pdcs[name];                                                         \
		for (unsigned i = 0; i < PDCCOUNT && pdc_l->list[i].count > 0; i++) {                             \
			fprintf(stderr, "(%d) %5u " #name ": %016lx-%04x: %016lx-%04lx\n", pid, i,                \
				pdc_l->list[i].key.qw_prefix, pdc_l->list[i].key.dw_suffix, pdc_l->list[i].start, \
				pdc_l->list[i].count);                                                            \
		}                                                                                                 \
		fprintf(stderr, "------\n");                                                                      \
	} while (0)

#define INC_PING_STAT(name, k, s, c)                                                                              \
	do {                                                                                                      \
		struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);                            \
		switch (name) {                                                                                   \
		case PRE_ACKS_SENT:                                                                               \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_preemptive_sent);  \
			break;                                                                                    \
		case ACKS_SENT:                                                                                   \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_sent);             \
			break;                                                                                    \
		case ACKS_RECV:                                                                                   \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_received);         \
			break;                                                                                    \
		case ACKS_IGNORED:                                                                                \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_ignored);          \
			break;                                                                                    \
		case PRE_NACKS_SENT:                                                                              \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_preemptive_sent); \
			break;                                                                                    \
		case NACKS_SENT:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_sent);            \
			break;                                                                                    \
		case NACKS_RECV:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_received);        \
			break;                                                                                    \
		case NACKS_IGNORED:                                                                               \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_ignored);         \
			break;                                                                                    \
		case PINGS_SENT:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.pings_sent);            \
			break;                                                                                    \
		case PINGS_RECV:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.pings_received);        \
			break;                                                                                    \
		default:                                                                                          \
			assert(0);                                                                                \
			break;                                                                                    \
		}                                                                                                 \
		INC_PDC(name, k, s, c);                                                                           \
	} while (0)
#else
#define INC_PING_STAT(name, k, s, c)                                                                              \
	do {                                                                                                      \
		struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);                            \
		switch (name) {                                                                                   \
		case PRE_ACKS_SENT:                                                                               \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_preemptive_sent);  \
			break;                                                                                    \
		case ACKS_SENT:                                                                                   \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_sent);             \
			break;                                                                                    \
		case ACKS_RECV:                                                                                   \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_received);         \
			break;                                                                                    \
		case ACKS_IGNORED:                                                                                \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.acks_ignored);          \
			break;                                                                                    \
		case PRE_NACKS_SENT:                                                                              \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_preemptive_sent); \
			break;                                                                                    \
		case NACKS_SENT:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_sent);            \
			break;                                                                                    \
		case NACKS_RECV:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_received);        \
			break;                                                                                    \
		case NACKS_IGNORED:                                                                               \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.nacks_ignored);         \
			break;                                                                                    \
		case PINGS_SENT:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.pings_sent);            \
			break;                                                                                    \
		case PINGS_RECV:                                                                                  \
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability_ping.pings_received);        \
			break;                                                                                    \
		default:                                                                                          \
			assert(0);                                                                                \
			break;                                                                                    \
		}                                                                                                 \
	} while (0)

#endif

#define INC_PING_STAT_COND(cond, name, k, s, c)               \
	do {                                                  \
		if (cond) {                                   \
			INC_PING_STAT((name), (k), (s), (c)); \
		}                                             \
	} while (0)

#define addquotes(x) #x
#define stringize(x) addquotes(x)

void dump_ping_counts()
{
#ifdef OPX_DUMP_PINGS
	// This bit is a work around to try and prevent the ranks from dumping
	// their output at the same time. It will obviously only work when
	// only a few ranks are involved.

	pid_t	      pid = getpid();
	unsigned long r	  = (pid % 4) * 3;
	fprintf(stderr, "%d delaying %lu seconds.\n", pid, r);

	DUMP_PINGS(pid, PINGS_SENT);
	DUMP_PINGS(pid, PINGS_RECV);
	DUMP_PINGS(pid, PRE_ACKS_SENT);
	DUMP_PINGS(pid, ACKS_SENT);
	DUMP_PINGS(pid, ACKS_RECV);
	DUMP_PINGS(pid, ACKS_IGNORED);
	DUMP_PINGS(pid, PRE_NACKS_SENT);
	DUMP_PINGS(pid, NACKS_SENT);
	DUMP_PINGS(pid, NACKS_RECV);
	DUMP_PINGS(pid, NACKS_IGNORED);
#endif
}

#else

#define INC_PING_STAT(name, k, s, c)
#define INC_PING_STAT_COND(cond, name, k, s, c)

#endif

void fi_opx_reliability_inc_throttle_count(struct fid_ep *ep)
{
	FI_OPX_DEBUG_COUNTERS_INC(
		container_of(ep, struct fi_opx_ep, ep_fid)->debug_counters.reliability_ping.throttled);
}

void fi_opx_reliability_inc_throttle_nacks(struct fid_ep *ep)
{
	FI_OPX_DEBUG_COUNTERS_INC(
		container_of(ep, struct fi_opx_ep, ep_fid)->debug_counters.reliability_ping.throttled_nacks);
}

void fi_opx_reliability_inc_throttle_maxo(struct fid_ep *ep)
{
	FI_OPX_DEBUG_COUNTERS_INC(
		container_of(ep, struct fi_opx_ep, ep_fid)->debug_counters.reliability_ping.throttled_max_outstanding);
}

#if 0
static inline
void dump_backtrace () {

	fprintf(stderr, "==== BACKTRACE ====\n");
	void * addr[100];
	backtrace_symbols_fd(addr, backtrace(addr, 100), 2);
	fprintf(stderr, "==== BACKTRACE ====\n");
}


struct fi_opx_reliability_service_range {
	uint64_t		begin;
	uint64_t		end;
};


/*
 * Functions for debugging reliability issues.
 */
/* NOT THREAD-SAFE */
static inline
void dump_flow_rx (struct fi_opx_reliability_rx_flow * flow, const int line) {

	const uint64_t key = flow->key.value;
	uint64_t next_psn = flow->next_psn;

	char debug[2048];
	char * str = debug;
	int size = sizeof(debug)-1;

	debug[0] = 0;
	if (flow->uepkt == NULL) {

		int c = snprintf(str, size, "(empty)");
		str += c;
		size -= c;

	} else {

		struct fi_opx_reliability_rx_uepkt * head = flow->uepkt;	/* read again now that queue is locked */

		int c = snprintf(str, size, "%08lu", head->psn);
		str += c;
		size -= c;

		uint64_t start_psn = head->psn;
		uint64_t stop_psn = start_psn;

		struct fi_opx_reliability_rx_uepkt * uepkt = head->next;
		while (uepkt != head) {
			if (uepkt->psn != (stop_psn + 1)) {

				if (start_psn != stop_psn) {
					c = snprintf(str, size, "..%08lu, %08lu", stop_psn, uepkt->psn);
				} else {
					c = snprintf(str, size, ", %08lu", uepkt->psn);
				}
				str += c;
				size -= c;

				start_psn = stop_psn = uepkt->psn;

			} else if (uepkt->next == head) {
				if (start_psn != uepkt->psn) {
					c = snprintf(str, size, "..%08lu", uepkt->psn);
				} else {
					c = snprintf(str, size, ", %08lu", uepkt->psn);
				}
				str += c;
				size -= c;

			} else {
				stop_psn++;
			}

			uepkt = uepkt->next;
		}

	}

	if (line) {
		fprintf(stderr, "flow__ %016lx (%d) next_psn = %lu, list: %s\n", key, line, next_psn, debug);
	} else {
		fprintf(stderr, "flow__ %016lx next_psn = %lu, list: %s\n", key, next_psn, debug);
	}
}


static inline
void dump_flow_list (uint64_t key, struct fi_opx_reliability_tx_replay * head, int line) {

	char debug[2048];
	char * str = debug;
	int size = sizeof(debug)-1;

	debug[0] = 0;

	if (!head) {

		int c = snprintf(str, size, "(empty)");
		str += c;
		size -= c;

	} else {

		int c = snprintf(str, size, "%08u", (uint32_t)head->scb.hdr.reliability.psn);
		str += c;
		size -= c;

		uint32_t next_psn = (uint32_t)head->scb.hdr.reliability.psn + 1;
		struct fi_opx_reliability_tx_replay * replay = head->next;
		while (replay != head) {

			if ((uint32_t)replay->scb.hdr.reliability.psn == next_psn) {
				if (replay->next == head) {

					c = snprintf(str, size, "..%08u", next_psn);
					str += c;
					size -= c;
				}
				next_psn += 1;
			} else {

				c = snprintf(str, size, "..%08u, %08u", next_psn, (uint32_t)replay->scb.hdr.reliability.psn);
				str += c;
				size -= c;
				next_psn = (uint32_t)replay->scb.hdr.reliability.psn + 1;
			}

			replay = replay->next;
		}
	}

	if (line) {
		fprintf(stderr, "flow__ %016lx (%d) list: %s\n", key, line, debug);
	} else {
		fprintf(stderr, "flow__ %016lx list: %s\n", key, debug);
	}
}


static inline
void fi_reliability_service_print_replay_ring (struct fi_opx_reliability_tx_replay * head,
		const char * func, const int line) {

	fprintf(stderr, "%s():%d == head = %p\n", func, line, head);
	if (head == NULL) return;

	struct fi_opx_reliability_tx_replay * tmp = head;

	do {
		fprintf(stderr, "%s():%d ==  ->    %p (p:%p, n:%p, psn:%u)\n", func, line, tmp, tmp->prev, tmp->next, (uint32_t)tmp->scb.hdr.reliability.psn);
		tmp = tmp->next;
	} while (tmp != head);

	fprintf(stderr, "%s():%d == tail = %p\n", func, line, head->prev);

	return;
}
#endif

#ifdef OPX_DAOS
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_reliability_inject_ud_opcode(struct fid_ep				    *ep,
						    const union fi_opx_reliability_service_flow_key *key,
						    const opx_lid_t dlid, const uint64_t reliability_rx,
						    const uint64_t opcode, const enum opx_hfi1_type hfi1_type,
						    const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const uint16_t credits_needed = (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) ? 1 : 2;
	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		if (FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	const uint64_t lrh_dlid_9B  = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(dlid);
	const uint64_t lrh_dlid_16B = dlid;
	const uint64_t bth_rx	    = reliability_rx << OPX_BTH_SUBCTXT_RX_SHIFT;

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	if ((hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B))) {
		struct fi_opx_hfi1_txe_scb_9B model_9B = opx_ep->reli_service->ping_model_9B;
		model_9B.hdr.ud.opcode		       = opcode;
		model_9B.hdr.service.key_dw_suffix     = key->dw_suffix;
		const uint64_t pbc_dlid		       = OPX_PBC_DLID(dlid, hfi1_type);

		opx_cacheline_store_qw_vol(scb,
					   model_9B.qw0 | OPX_PBC_CR(0x1, hfi1_type) | pbc_dlid |
						   OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
					   model_9B.hdr.qw_9B[0] | lrh_dlid_9B, model_9B.hdr.qw_9B[1] | bth_rx,
					   model_9B.hdr.qw_9B[2], model_9B.hdr.qw_9B[3], 0UL, 0UL, key->qw_prefix);

		/* consume one credit for the packet header */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	} else {
		struct fi_opx_hfi1_txe_scb_16B model_16B = opx_ep->reli_service->ping_model_16B;
		model_16B.hdr.ud.opcode			 = opcode;
		model_16B.hdr.service.key_dw_suffix	 = key->dw_suffix;
		const uint64_t pbc_dlid			 = OPX_PBC_DLID(dlid, hfi1_type);

		opx_cacheline_store_qw_vol(
			scb,
			model_16B.qw0 | OPX_PBC_CR(1, hfi1_type) | pbc_dlid | OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			model_16B.hdr.qw_16B[0] | ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
						   << OPX_LRH_JKR_16B_DLID_SHIFT_16B),
			model_16B.hdr.qw_16B[1] |
				((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
				 OPX_LRH_JKR_16B_DLID20_SHIFT_16B) |
				bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B,
			model_16B.hdr.qw_16B[2] | bth_subctxt_rx, model_16B.hdr.qw_16B[3], model_16B.hdr.qw_16B[4], 0UL,
			0UL);

		/* consume one credit for the packet header first cacheline */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

		volatile uint64_t *const scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		opx_cacheline_store_qw_vol(scb_payload, key->qw_prefix, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD);

		/* consume one credit for the packet header second (padded) cacheline */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	return FI_SUCCESS;
}

ssize_t fi_opx_hfi1_tx_reliability_inject_ud_resynch(struct fid_ep				     *ep,
						     const union fi_opx_reliability_service_flow_key *key,
						     const opx_lid_t dlid, const uint64_t reliability_rx,
						     const uint64_t opcode)
{
	assert(opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH ||
	       opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK);

	ssize_t rc = fi_opx_hfi1_tx_reliability_inject_ud_opcode(ep, key, dlid, reliability_rx, opcode, OPX_HFI1_TYPE,
								 OPX_IS_CTX_SHARING_ENABLED);

	if (OFI_UNLIKELY(rc)) {
#ifdef OPX_RELIABILITY_DEBUG
		if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
			fprintf(stderr, "(tx) Client flow__ %016lx %08x 0x%x inj resynch dropped; no credits\n",
				key->qw_prefix, key->dw_suffix, reliability_rx);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
			fprintf(stderr, "(rx) Server flow__ %016lx %08x 0x%x inj resynch ack dropped; no credits\n",
				key->qw_prefix, key->dw_suffix, reliability_rx);
		}
#endif
		return -FI_EAGAIN;
	}

#ifdef OPX_RELIABILITY_DEBUG
	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
		fprintf(stderr, "(tx) Client flow__ %016lx %08x 0x%x inj resynch\n", key->qw_prefix, key->dw_suffix,
			reliability_rx);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
		fprintf(stderr, "(rx) Server flow__ %016lx %08x 0x%x inj resynch ack\n", key->qw_prefix, key->dw_suffix,
			reliability_rx);
	}
#endif

	return FI_SUCCESS;
}
#endif

ssize_t fi_opx_hfi1_tx_reliability_inject(struct fid_ep *ep, const union fi_opx_reliability_service_flow_key *key,
					  const opx_lid_t dlid, const uint64_t reliability_rx, const uint64_t psn_start,
					  const uint64_t psn_count, const uint64_t opcode,
					  const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	// Prevent sending a packet that contains a PSN rollover.
	const uint64_t psn_start_24 = psn_start & MAX_PSN;
	const uint64_t psn_count_24 = MIN(psn_count, MAX_PSN - psn_start_24 + 1);

	const uint16_t credits_needed = (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) ? 1 : 2;
	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		if (FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < credits_needed) {
			/*
			 * no credits available
			 *
			 * DO NOT BLOCK - instead, drop this request and allow
			 * the reliability protocol to time out and retransmit
			 */
#ifdef OPX_RELIABILITY_DEBUG
			if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING) {
				fprintf(stderr, "(tx) flow__ %016lx %08x inj ping dropped; no credits\n",
					key->qw_prefix, key->dw_suffix);
			} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK) {
				fprintf(stderr, "(rx) flow__ %016lx %08x inj ack dropped; no credits\n", key->qw_prefix,
					key->dw_suffix);
			} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK) {
				fprintf(stderr, "(rx) flow__ %016lx %08x inj nack dropped; no credits\n",
					key->qw_prefix, key->dw_suffix);
			} else {
				fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__,
					opcode);
			}
#endif
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

#ifdef OPX_RELIABILITY_DEBUG
	const uint64_t psn_stop = psn_start + psn_count - 1;

	if (psn_start > psn_stop) {
		if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING) {
			fprintf(stderr,
				"%s:%s():%d (%016lx %08x) invalid inject ping; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n",
				__FILE__, __func__, __LINE__, key->qw_prefix, key->dw_suffix, psn_start, psn_count,
				psn_stop);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK) {
			fprintf(stderr,
				"%s:%s():%d (%016lx %08x) invalid inject ack; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n",
				__FILE__, __func__, __LINE__, key->qw_prefix, key->dw_suffix, psn_start, psn_count,
				psn_stop);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK) {
			fprintf(stderr,
				"%s:%s():%d (%016lx %08x) invalid inject nack; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n",
				__FILE__, __func__, __LINE__, key->qw_prefix, key->dw_suffix, psn_start, psn_count,
				psn_stop);
		} else {
			fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
		}
		abort();
	}

	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING) {
		fprintf(stderr,
			"(tx) [flow key: slid=%08x src_subctxt_rx=%04x dlid=%08x dst_subctxt_rx=%04x] inj ping %08lu..%08lu\n",
			key->slid, key->src_subctxt_rx, key->dlid, key->dst_subctxt_rx, psn_start, psn_stop);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK) {
		fprintf(stderr,
			"(rx) [flow key: slid=%08x src_subctxt_rx=%04x dlid=%08x dst_subctxt_rx=%04x] inj ack %08lu..%08lu\n",
			key->slid, key->src_subctxt_rx, key->dlid, key->dst_subctxt_rx, psn_start, psn_stop);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK) {
		fprintf(stderr,
			"(rx) [flow key: slid=%08x src_subctxt_rx=%04x dlid=%08x dst_subctxt_rx=%04x] inj nack %08lu..%08lu\n",
			key->slid, key->src_subctxt_rx, key->dlid, key->dst_subctxt_rx, psn_start, psn_stop);
	} else {
		fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
		abort();
	}
#endif

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	const uint64_t bth_rx	     = reliability_rx << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t key_dw_suffix = ((uint64_t) key->dw_suffix) << 32;

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		const uint64_t				   lrh_dlid_9B = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(dlid);
		const struct fi_opx_hfi1_txe_scb_9B *const model =
			opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING ?
				&opx_ep->reli_service->ping_model_9B :
				(opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK ? &opx_ep->reli_service->ack_model_9B :
										  &opx_ep->reli_service->nack_model_9B);
		const uint64_t pbc_dlid = OPX_PBC_DLID(dlid, hfi1_type);

		opx_cacheline_store_qw_vol(
			scb, model->qw0 | OPX_PBC_CR(0x1, hfi1_type) | pbc_dlid | OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			model->hdr.qw_9B[0] | lrh_dlid_9B, model->hdr.qw_9B[1] | bth_rx, model->hdr.qw_9B[2],
			model->hdr.qw_9B[3] | key_dw_suffix, psn_count_24, psn_start_24,
			key->qw_prefix); /* service.key */

		// fi_opx_hfi1_dump_stl_packet_hdr((struct fi_opx_hfi1_stl_packet_hdr_9B *)&tmp[1], __func__, __LINE__);

		/* consume one credit for the packet header */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	} else {
		const uint64_t				    lrh_dlid_16B = dlid;
		const struct fi_opx_hfi1_txe_scb_16B *const model_16B =
			opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING ?
				&opx_ep->reli_service->ping_model_16B :
				(opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK ?
					 &opx_ep->reli_service->ack_model_16B :
					 &opx_ep->reli_service->nack_model_16B);
		const uint64_t pbc_dlid = OPX_PBC_DLID(dlid, hfi1_type);

		opx_cacheline_store_qw_vol(
			scb,
			model_16B->qw0 | OPX_PBC_CR(1, hfi1_type) | pbc_dlid | OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			model_16B->hdr.qw_16B[0] | ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
						    << OPX_LRH_JKR_16B_DLID_SHIFT_16B),
			model_16B->hdr.qw_16B[1] |
				((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
				 OPX_LRH_JKR_16B_DLID20_SHIFT_16B) |
				(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
			model_16B->hdr.qw_16B[2] | bth_rx, model_16B->hdr.qw_16B[3],
			model_16B->hdr.qw_16B[4] | key_dw_suffix, psn_count_24, psn_start_24);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

		FI_INFO(fi_opx_global.prov, FI_LOG_EP_DATA, "Completed cacheline 1\n");

		volatile uint64_t *const scb2 = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		opx_cacheline_store_qw_vol(scb2, key->qw_prefix, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
	return FI_SUCCESS;
}

void fi_opx_hfi1_rx_reliability_send_pre_acks(struct fid_ep *ep, const opx_lid_t dlid,
					      const uint16_t reliability_subctxt_rx, const uint64_t psn_start,
					      const uint64_t psn_count, const union opx_hfi1_packet_hdr *const hdr,
					      const opx_lid_t slid, const enum opx_hfi1_type hfi1_type,
					      const bool ctx_sharing)
{
	OPX_TRACER_TRACE_RELI(OPX_TRACER_BEGIN, "RX_RELI_SEND_PRE_ACKS");

	const union fi_opx_reliability_service_flow_key key = {
		.slid		= slid,
		.src_subctxt_rx = FI_OPX_HFI1_PACKET_ORIGIN_RX(hdr),
		.dlid		= dlid,
		.dst_subctxt_rx = reliability_subctxt_rx,
	};

	ssize_t rc __attribute__((unused));
	rc = fi_opx_hfi1_tx_reliability_inject(ep, &key, slid, key.src_subctxt_rx, psn_start, psn_count,
					       FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK, hfi1_type, ctx_sharing);
	INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_ACKS_SENT, key, psn_start, psn_count);
	OPX_TRACER_TRACE_RELI(OPX_TRACER_END_SUCCESS, "RX_RELI_SEND_PRE_ACKS");
}

/**
 * Send a ping response over a given range of PSNs. If the specified
 * range crosses a PSN rollover boundary, the response will be sent across
 * multiple packets such that no single packet will contain a range that
 * rolls over.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_rx_reliability_ping_response(struct fid_ep					 *ep,
						 const union fi_opx_reliability_service_flow_key *key,
						 uint64_t psn_start, uint64_t psn_stop, const opx_lid_t slid,
						 const uint64_t rx, const uint64_t opcode)
{
	OPX_TRACER_TRACE_RELI(OPX_TRACER_BEGIN, "RX_RELI_PING_RESPONSE");
	assert(psn_stop >= psn_start);
	uint64_t psn_count = (psn_stop - psn_start) + 1;
	ssize_t	 rc;
	do {
		uint64_t       psn_start_24 = psn_start & MAX_PSN;
		const uint64_t psn_count_24 = MIN(psn_count, (MAX_PSN - psn_start_24) + 1);

		rc = fi_opx_hfi1_tx_reliability_inject(ep, key, slid, rx, psn_start_24, psn_count_24, opcode,
						       OPX_HFI1_TYPE, OPX_IS_CTX_SHARING_ENABLED);
		INC_PING_STAT_COND(rc == FI_SUCCESS,
				   opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK ? ACKS_SENT : NACKS_SENT, *key,
				   psn_start_24, psn_count_24);

		psn_count -= psn_count_24;
		psn_start += psn_count_24;
	} while (psn_count > 0 && rc == FI_SUCCESS);

	OPX_TRACER_TRACE_RELI(OPX_TRACER_END_SUCCESS, "RX_RELI_PING_RESPONSE");
	return rc;
}

void fi_opx_hfi1_rx_reliability_ping(struct fid_ep *ep, struct fi_opx_reliability_service *service,
				     const union fi_opx_reliability_service_flow_key *key, uint64_t psn_count,
				     uint64_t psn_start, const opx_lid_t slid, const uint16_t subctxt_rx)
{
	OPX_TRACER_TRACE_RELI(OPX_TRACER_BEGIN, "RX_RELI_PING");

	OPX_RELIABILITY_DEBUG_LOG(key, "(rx) rcv ping %08lu..%08lu\n", psn_start, psn_start + psn_count - 1);

	INC_PING_STAT(PINGS_RECV, *key, psn_start, psn_count);

	void *itr = fi_opx_rbt_find(service->rx.rx_flow_rbtree, (void *) key);

	/* Our flow doesn't exist, which means we haven't
	   received any packets from this sender. Send a NACK for PSN 0 */
	if (OFI_UNLIKELY(itr == NULL)) {
		ssize_t rc __attribute__((unused));
		rc = fi_opx_hfi1_tx_reliability_inject(ep, key, slid, subctxt_rx, 0, /* psn_start */
						       1,			     /* psn_count */
						       FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK, OPX_HFI1_TYPE,
						       OPX_IS_CTX_SHARING_ENABLED);
		INC_PING_STAT_COND(rc == FI_SUCCESS, NACKS_SENT, *key, 0, 1);
		OPX_TRACER_TRACE_RELI(OPX_TRACER_END_ERROR, "RX_RELI_PING");
		return;
	}

	struct fi_opx_reliability_rx_flow **value_ptr =
		(struct fi_opx_reliability_rx_flow **) fi_opx_rbt_value_ptr(service->rx.rx_flow_rbtree, itr);

	struct fi_opx_reliability_rx_flow *flow = *value_ptr;

	const uint64_t flow_next_psn	= flow->next_psn;
	const uint64_t flow_next_psn_24 = flow_next_psn & MAX_PSN;
	uint64_t       ping_start_psn	= psn_start;
	uint64_t       ping_psn_count	= psn_count;

	/* Scale the received PSN into the same window as the expected PSN.
	 *
	 * We know that the sender has a limited number of replays allocated
	 * (OPX_MAX_OUTSTANDING_REPLAYS), and thus legitimate pings for PSNs we
	 * have not yet received should never be further ahead than next expected
	 * PSN + OPX_MAX_OUTSTANDING_REPLAYS (max_future_psn). However, there is
	 * no limit on how far behind the current expected PSN the ping start
	 * PSN may be. The sender may have a replay from a million packets ago
	 * that never got ACK'd.
	 *
	 * Given this, a starting PSN that is further ahead than max_future_psn
	 * should be assumed to be representing packets we've already received
	 * and scaled accordingly.
	 */
	ping_start_psn |= (flow_next_psn & MAX_PSN_MASK);

	// If the PSN is very close to the bottom of the window but the expected
	// PSN is very high, assume the received PSN rolled over and needs to be
	// moved into the next, higher, window.
	if (OFI_UNLIKELY(flow_next_psn_24 >= PSN_HIGH_WINDOW)) {
		const uint64_t max_future_psn = flow_next_psn - PSN_HIGH_WINDOW;
		if (ping_start_psn <= max_future_psn) {
			ping_start_psn += PSN_WINDOW_SIZE;
			assert(ping_start_psn >= flow_next_psn);
		}
	} else {
		// Otherwise, if the PSN is further ahead than possible replays,
		// Assume it's for a past packet and scale it down.
		const uint64_t max_future_psn = flow_next_psn + OPX_MAX_OUTSTANDING_REPLAYS;
		if (ping_start_psn > max_future_psn) {
			assert(ping_start_psn >= PSN_WINDOW_SIZE);
			ping_start_psn -= PSN_WINDOW_SIZE;
		}
	}

	const uint64_t ping_stop_psn = ping_start_psn + ping_psn_count - 1;

	/*
	 * ping_start_psn                               ping_stop_psn
	 *      |                                               |
	 *      |---------------- Ping PSN Range ---------------|
	 *
	 *      |-------------------- RX Flow ------------------|
	 *      |                                               |
	 *      |           flow->          flow->              |
	 *      |            next_psn        uepkt->psn         |
	 *      |             |                |                |
	 *      |-------------+----------------+----------------|
	 *      | ACK range 1 | NACK range 1   | ACK range 2    |
	 *      |-------------+----------------+----------------|
	 *
	 *  Note that the Ping PSN Range as illustrated above is only one
	 *  possibility; The actual start and stop points may exclude any
	 *  or all of the ACK/NACK ranges.
	 *
	 * 1) ACK range 1: ACK all the packets we've received up to the
	 *    next expected PSN or the last PSN in the requested range,
	 *    whichever is smaller
	 */
	uint64_t ack_stop_psn = MIN(flow->next_psn - 1, ping_stop_psn);

	ssize_t rc;
	if (ping_start_psn <= ack_stop_psn) {
		rc = fi_opx_hfi1_rx_reliability_ping_response(ep, key, ping_start_psn, ack_stop_psn, slid, subctxt_rx,
							      FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK);

		/*
		 * If we failed to send the ack, stop now.
		 *
		 * Or, if we haven't received any unexpected packets,
		 * chances are any remaining packets in the ping range are
		 * already enroute and we simply haven't processed them yet.
		 *
		 * Return now to avoid sending NACKs for such packets.
		 * If they really did get dropped, we'll send a NACK for them
		 * as soon as we receive the first out of order packet, or on
		 * subsequent ping requests where ACK range 1 is excluded
		 * from the range.
		 */
		if (OFI_LIKELY(flow->uepkt == NULL) || rc != FI_SUCCESS) {
			OPX_TRACER_TRACE_RELI(OPX_TRACER_END_ERROR, "RX_RELI_PING");
			return;
		}
	}

	/*
	 * 2) ACK range 2: Either the ping range starts after ACK range 1,
	 *    or we've received at least some unexpected packets.
	 *    Send ACKs for the first set of consecutive unexpected packets
	 *    we've received that fall into the requested ping range
	 */
	if (flow->uepkt != NULL) {
		struct fi_opx_reliability_rx_uepkt *uepkt = flow->uepkt;

		// Find the first unexpected packet who's PSN is in the ping range
		// The unexpected packet queue is a ring, so uepkt->next
		// will never be NULL, and if we make it all the way around,
		// the psn check will fail
		while (uepkt->psn < ping_start_psn && uepkt->psn < uepkt->next->psn) {
			uepkt = uepkt->next;
		}

		if (uepkt->psn >= ping_start_psn && uepkt->psn <= ping_stop_psn) {
			uint64_t first_uepkt_psn = uepkt->psn;
			uint64_t last_uepkt_psn	 = first_uepkt_psn;

			while (last_uepkt_psn <= ping_stop_psn && uepkt->next->psn == (last_uepkt_psn + 1)) {
				++last_uepkt_psn;
				uepkt = uepkt->next;
			}

			rc = fi_opx_hfi1_rx_reliability_ping_response(ep, key, first_uepkt_psn, last_uepkt_psn, slid,
								      subctxt_rx, FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK);
			if (OFI_UNLIKELY(rc != FI_SUCCESS)) {
				OPX_TRACER_TRACE_RELI(OPX_TRACER_END_ERROR, "RX_RELI_PING");
				return;
			}
		}
	}

	/*
	 * 3) NACK Range 1: Finally, NACK the range between next expected PSN
	 *    and the first unexpected packet's PSN or stop PSN, whichever is
	 *    smaller, but only if the specified ping range starts on the next
	 *    expected PSN.
	 */
	if (ping_start_psn == flow->next_psn) {
		uint64_t nack_start_psn = MAX(flow->next_psn, ping_start_psn);
		uint64_t nack_stop_psn	= flow->uepkt == NULL ? ping_stop_psn : MIN(flow->uepkt->psn, ping_stop_psn);

		if (nack_start_psn <= nack_stop_psn) {
			uint64_t nack_count = MIN((nack_stop_psn - nack_start_psn) + 1, OPX_RELIABILITY_RX_MAX_NACK);
			nack_stop_psn	    = nack_start_psn + nack_count;

			fi_opx_hfi1_rx_reliability_ping_response(ep, key, nack_start_psn, nack_start_psn, slid,
								 subctxt_rx, FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK);
		}
	}

	OPX_TRACER_TRACE_RELI(OPX_TRACER_END_SUCCESS, "RX_RELI_PING");
}

#ifdef OPX_RELIABILITY_DEBUG
/*
 * In order to minimize the delay caused by trying to log every ack, this
 * buffer is used to save off the messages from last pass through the
 * reliability ack function. The contents can be read from a core dump or
 * a live process via gdb.
 */
#define LAST_ACK_LEN 4096
char last_ack[LAST_ACK_LEN] __attribute__((used));
int  last_ack_index;
#endif

#ifndef NDEBUG
__OPX_FORCE_INLINE__
void fi_opx_hfi1_reliability_iov_payload_check(struct fi_opx_reliability_tx_replay	       *replay,
					       const union fi_opx_reliability_service_flow_key *key,
					       const char *error_msg, const char *file, const char *func,
					       const int line)
{
	if (!replay->use_iov || replay->hmem_iface != FI_HMEM_SYSTEM) {
		return;
	}
	uint32_t  copy_payload_qws = MIN(8, replay->iov->iov_len >> 3);
	uint64_t *replay_iov_base  = (uint64_t *) replay->iov->iov_base;

	for (int i = 0; i < copy_payload_qws; ++i) {
		if (replay_iov_base[i] != replay->orig_payload[i]) {
			struct fi_opx_hfi1_sdma_work_entry *we		       = replay->sdma_we;
			void				   *bounce_buf	       = NULL;
			int				    use_bounce_buf     = 0;
			int				    pending_bounce_buf = 0;
			struct fi_opx_completion_counter   *we_cc	       = NULL;
			size_t				    we_cc_byte_counter = 0;

			if (we) {
				bounce_buf	   = we->bounce_buf.buf;
				use_bounce_buf	   = we->use_bounce_buf;
				pending_bounce_buf = we->pending_bounce_buf;
				we_cc		   = &we->bounce_buf.cc;
				we_cc_byte_counter = we_cc->byte_counter;
			}

			struct fi_opx_completion_counter *cc_next	       = replay->cc_ptr->next;
			size_t				  cc_next_byte_counter = cc_next ? cc_next->byte_counter : 0;

			fprintf(stderr,
				"(%d) %s:%s():%d (tx) flow %016lX %08x|psn=%d (0x%X)|"
				"sdma_we=%p|sdma_we->bounce_buf=%p|sdma_we->use_bb=%d|sdma_we->pending_bb=%d|"
				"bb_cc=%p|bb_cc->byte_counter=%lu|"
				"replay->cc=%p|replay->cc->byte_counter=%lu|replay->cc_dec=%lu|"
				"replay->cc->next=%p|replay->cc->next->byte_counter=%lu|"
				"opcode=%02X|dput_opcode=%02X|"
				"iov_base=%p|iov_len=%lu|%s|"
				"orig_payload[%d]=%016lX current[@%p]=%016lX\n",
				getpid(), file, func, line, key->qw_prefix, key->dw_suffix,
				FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(replay)),
				FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(replay)), replay->sdma_we, bounce_buf,
				use_bounce_buf, pending_bounce_buf, we_cc, we_cc_byte_counter, replay->cc_ptr,
				replay->cc_ptr->byte_counter, replay->cc_dec, cc_next, cc_next_byte_counter,
				OPX_REPLAY_HDR(replay)->bth.opcode,
				FI_OPX_HFI_DPUT_GET_OPCODE(OPX_REPLAY_HDR(replay)->dput.target.opcode_origin_rx),
				replay->iov->iov_base, replay->iov->iov_len, error_msg, i, replay->orig_payload[i],
				&replay_iov_base[i], replay_iov_base[i]);
			break;
		}
	}
}
#endif

void fi_opx_hfi1_rx_reliability_ack(struct fid_ep *ep, struct fi_opx_reliability_service *service,
				    const union fi_opx_reliability_service_flow_key *key, const uint64_t psn_count,
				    const uint64_t psn_start)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RX_ACK");
#ifdef OPX_DEBUG_COUNTERS_SDMA
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
#endif
	const uint64_t psn_stop = psn_start + psn_count - 1;

	INC_PING_STAT(ACKS_RECV, *key, psn_start, psn_count);

	OPX_RELIABILITY_DEBUG_LOG(key, "(tx) rcv ack  %08lu..%08lu\n", psn_start, psn_stop);

	assert(psn_stop <= MAX_PSN);

	void *itr = NULL;

	/* search for existing unack'd flows */
	itr = fi_opx_rbt_find(service->tx.tx_flow_outstanding_pkts_rbtree, (void *) key);

	ASSERT_FLOW_FOUND(itr, key, "Invalid key psn_start = %lx, psn_count = %lx, stop_psn = %lx\n", psn_start,
			  psn_count, psn_stop);

	struct fi_opx_reliability_tx_replay **value_ptr = (struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(
		service->tx.tx_flow_outstanding_pkts_rbtree, itr);

	struct fi_opx_reliability_tx_replay *head = *value_ptr;

	if (OFI_UNLIKELY(head == NULL)) {
		/*
		 * there are no unack'd elements in the replay queue;
		 * do nothing and return
		 */
		INC_PING_STAT(ACKS_IGNORED, *key, psn_start, psn_count);
		OPX_TRACER_TRACE(OPX_TRACER_END_ACK_IGNORED, "RX_ACK");
		return;
	}

	struct fi_opx_reliability_tx_replay *tail = head->prev;

	/*
	 * check for "fast path" - if the head of the replay q is within the
	 * ACK range, and the tail of the q is within the ACK range, and the replay
	 * q doesn't contain a rollover (i.e, the tail's PSN >= the head's PSN)
	 * we can just retire all elements in the queue
	 */

	uint32_t head_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(head));
	uint32_t tail_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(tail));
	if ((head_psn >= psn_start) && (tail_psn <= psn_stop) && (tail_psn >= head_psn)) {
#ifdef OPX_RELIABILITY_DEBUG
		last_ack_index = snprintf(last_ack, LAST_ACK_LEN,
					  "(tx) Retiring on the fast path: %" PRIx64 ", %" PRIx64 ", %" PRIx64
					  ", H: %d, T: %d\n",
					  psn_start, psn_count, psn_stop, head_psn, tail_psn);
#endif
		/* retire all queue elements */
		*value_ptr = NULL;

		struct fi_opx_reliability_tx_replay *next = NULL;
		struct fi_opx_reliability_tx_replay *tmp  = head;

		/* Clear any throttling. */
		tmp->psn_ptr->psn.throttle   = 0;
		tmp->psn_ptr->psn.nack_count = 0;

		do {
			assert(!tmp->acked);
#ifdef OPX_RELIABILITY_DEBUG
			if (last_ack_index < LAST_ACK_LEN) {
				last_ack_index += snprintf(&last_ack[last_ack_index], LAST_ACK_LEN - last_ack_index,
							   "(tx) packet %016lx %08x psn=%08x retired (fast path).\n",
							   key->qw_prefix, key->dw_suffix,
							   FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(tmp)));
			}
#endif
			next = tmp->next;

			const uint64_t			  dec	 = tmp->cc_dec;
			struct fi_opx_completion_counter *cc_ptr = tmp->cc_ptr;
			if (cc_ptr) {
#ifndef NDEBUG
				fi_opx_hfi1_reliability_iov_payload_check(
					tmp, key, "Received ACK for packet where source buffer has changed!", __FILE__,
					__func__, __LINE__);
#endif
#ifdef OPX_DEBUG_COUNTERS_SDMA
				if (tmp->use_iov && tmp->sdma_we) {
					struct fi_opx_hfi1_sdma_work_entry *sdma_we =
						(struct fi_opx_hfi1_sdma_work_entry *) tmp->sdma_we;
					if (sdma_we->first_ack_time_ns == 0 &&
					    sdma_we->comp_state == OPX_SDMA_COMP_QUEUED) {
						OPX_COUNTERS_TIME_NS(sdma_we->first_ack_time_ns,
								     &opx_ep->debug_counters);
					}
				}
#endif
				cc_ptr->byte_counter -= dec;
				assert(cc_ptr->byte_counter >= 0);
				if (cc_ptr->byte_counter == 0) {
					cc_ptr->hit_zero(cc_ptr);
				}
			}

			uint16_t lrh_pktlen_le;
			size_t	 total_bytes;

			/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
			if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
				lrh_pktlen_le = ntohs(tmp->scb.scb_9B.hdr.lrh_9B.pktlen);
				total_bytes   = (lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
			} else {
				lrh_pktlen_le = tmp->scb.scb_16B.hdr.lrh_16B.pktlen;
				total_bytes   = (lrh_pktlen_le - 1) * 8; /* do not copy the trailing icrc */
			}
			tmp->psn_ptr->psn.bytes_outstanding -= total_bytes;
			assert((int32_t) tmp->psn_ptr->psn.bytes_outstanding >= 0);

			/* If the replay is pinned, then don't free it and just mark it as ACK'd. It is the
			   responsibility of the pinner to free it after it's ACK'd */
			if (!tmp->pinned) {
				fi_opx_reliability_service_replay_deallocate(service, tmp);
			} else {
				OPX_RELIABILITY_DEBUG_LOG(
					key,
					"(tx) packet psn=%08u ACK'd but pinned, marking as ACK'd and skipping free of replay.\n",
					FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(tmp)));
				tmp->acked = true;
			}
			tmp = next;

		} while (tmp != head);

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RX_ACK");
		return;
	}

	/*
	 * find the first replay to ack
	 */
#ifdef OPX_RELIABILITY_DEBUG
	last_ack_index =
		snprintf(last_ack, LAST_ACK_LEN, "(tx) Retiring on the slow path: %lx, %lx, %lx, H: %x, T: %x\n",
			 psn_start, psn_count, psn_stop, head_psn, tail_psn);
#endif

	struct fi_opx_reliability_tx_replay *start     = head;
	uint32_t			     start_psn = head_psn;
	while ((start_psn < psn_start) && (start != tail)) {
		start	  = start->next;
		start_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(start));
	}

	if (OFI_UNLIKELY(start_psn < psn_start)) {
		/*
		 * all elements in replay queue are 'younger' than the
		 * first psn to retire; do nothing and return
		 */
#ifdef OPX_RELIABILITY_DEBUG
		if (last_ack_index < LAST_ACK_LEN) {
			last_ack_index += snprintf(&last_ack[last_ack_index], LAST_ACK_LEN - last_ack_index,
						   "(tx) All elements are younger.\n");
		}
#endif
		INC_PING_STAT(ACKS_IGNORED, *key, psn_start, psn_count);
		OPX_TRACER_TRACE(OPX_TRACER_END_ACK_IGNORED, "RX_ACK");
		return;
	}

	/*
	 * find the last replay to ack. the replay psn must be contained in the
	 * range [start_psn,psn_stop] and cannot contain a rollover.
	 */

	struct fi_opx_reliability_tx_replay *stop	   = start;
	uint32_t			     stop_psn	   = start_psn;
	uint32_t			     stop_next_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop->next));
	while ((stop->next != head) && (stop_next_psn <= psn_stop) && (stop_next_psn > psn_start)) {
		stop_psn      = stop_next_psn;
		stop	      = stop->next;
		stop_next_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop->next));
	}

	if (OFI_UNLIKELY(stop_psn > psn_stop)) {
		/*
		 * all elements in the replay queue are 'older' than the
		 * last psn to retire; do nothing an return
		 */
#ifdef OPX_RELIABILITY_DEBUG
		if (last_ack_index < LAST_ACK_LEN) {
			last_ack_index += snprintf(&last_ack[last_ack_index], LAST_ACK_LEN - last_ack_index,
						   "(tx) All elements are older.\n");
		}
#endif
		INC_PING_STAT(ACKS_IGNORED, *key, psn_start, psn_count);
		OPX_TRACER_TRACE(OPX_TRACER_END_ACK_IGNORED, "RX_ACK");
		return;
	}

	const struct fi_opx_reliability_tx_replay *const halt = stop->next;

	if (start == head) {
		if (halt == start) {
			*value_ptr = NULL;
			/* Clear any nack throttling. */
			start->psn_ptr->psn.throttle   = 0;
			start->psn_ptr->psn.nack_count = 0;
		} else {
			*value_ptr = (struct fi_opx_reliability_tx_replay *) halt;
		}
	}

#ifdef OPX_RELIABILITY_DEBUG
	if (last_ack_index < LAST_ACK_LEN) {
		last_ack_index += snprintf(
			&last_ack[last_ack_index], LAST_ACK_LEN - last_ack_index,
			"(tx) Start = %x, Stop = %x, Halt = %x\n", FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(start)),
			FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop)), FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(halt)));
	}
#endif

	/* remove the psn range to ack from the queue */
	start->prev->next = stop->next;
	stop->next->prev  = start->prev;

	/*
	 * retire all replay packets between start and stop, inclusive
	 */

	struct fi_opx_reliability_tx_replay *tmp = start;
	do {
		assert(!tmp->acked);
#ifdef OPX_RELIABILITY_DEBUG
		if (last_ack_index < LAST_ACK_LEN) {
			last_ack_index +=
				snprintf(&last_ack[last_ack_index], LAST_ACK_LEN - last_ack_index,
					 "(tx) packet %016lx %08x psn=%08x retired (slow path).\n", key->qw_prefix,
					 key->dw_suffix, FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(tmp)));
		}
#endif
		struct fi_opx_reliability_tx_replay *next = tmp->next;

		const uint64_t			  dec	 = tmp->cc_dec;
		struct fi_opx_completion_counter *cc_ptr = tmp->cc_ptr;
		if (cc_ptr) {
#ifndef NDEBUG
			fi_opx_hfi1_reliability_iov_payload_check(
				tmp, key, "Received ACK for packet where source buffer has changed!", __FILE__,
				__func__, __LINE__);
#endif
#ifdef OPX_DEBUG_COUNTERS_SDMA
			if (tmp->use_iov && tmp->sdma_we) {
				struct fi_opx_hfi1_sdma_work_entry *sdma_we =
					(struct fi_opx_hfi1_sdma_work_entry *) tmp->sdma_we;
				if (sdma_we->first_ack_time_ns == 0 && sdma_we->comp_state == OPX_SDMA_COMP_QUEUED) {
					OPX_COUNTERS_TIME_NS(sdma_we->first_ack_time_ns, &opx_ep->debug_counters);
				}
			}
#endif
			cc_ptr->byte_counter -= dec;
			assert(cc_ptr->byte_counter >= 0);
			if (cc_ptr->byte_counter == 0) {
				cc_ptr->hit_zero(cc_ptr);
			}
		}

		uint16_t lrh_pktlen_le;
		size_t	 total_bytes;
		/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
		if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
			lrh_pktlen_le = ntohs(tmp->scb.scb_9B.hdr.lrh_9B.pktlen);
			total_bytes   = (lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
		} else {
			lrh_pktlen_le = tmp->scb.scb_16B.hdr.lrh_16B.pktlen;
			total_bytes   = (lrh_pktlen_le - 1) * 8; /* do not copy the trailing icrc */
		}
		tmp->psn_ptr->psn.bytes_outstanding -= total_bytes;
		assert((int32_t) tmp->psn_ptr->psn.bytes_outstanding >= 0);

		/* If the replay is pinned, then don't free it and just mark it as ACK'd. It is the
		   responsibility of the pinner to free it after it's ACK'd */
		if (!tmp->pinned) {
			fi_opx_reliability_service_replay_deallocate(service, tmp);
		} else {
			OPX_RELIABILITY_DEBUG_LOG(
				key,
				"(tx) packet psn=%08u ACK'd but pinned, marking as ACK'd and skipping free of replay.\n",
				FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(tmp)));
			tmp->acked = true;
		}
		tmp = next;

	} while (tmp != halt);

	assert((*value_ptr == NULL) || (*value_ptr)->next != NULL);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RX_ACK");
}

ssize_t fi_opx_reliability_sdma_replay_complete(union fi_opx_reliability_deferred_work *work)
{
	OPX_TRACER_TRACE_SDMA(OPX_TRACER_BEGIN, "SDMA_RELI_REPLAY_COMPLETE");
	struct fi_opx_reliability_tx_sdma_replay_params *params = &work->sdma_replay;
	struct fi_opx_ep				*opx_ep = (struct fi_opx_ep *) params->opx_ep;

	struct fi_opx_hfi1_sdma_replay_work_entry *we =
		(struct fi_opx_hfi1_sdma_replay_work_entry *) params->sdma_reqs.head;

	while (we && we->comp_state != OPX_SDMA_COMP_QUEUED && we->comp_state != OPX_SDMA_COMP_PENDING_WRITEV) {
		if (OFI_UNLIKELY(we->comp_state == OPX_SDMA_COMP_ERROR)) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "Failed sending replay with PSN %u (%X) via SDMA\n",
				     (uint32_t) FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(we->replay)),
				     (uint32_t) FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(we->replay)));
			OPX_RELIABILITY_DEBUG_LOG(params->flow_key,
						  "(tx) replay packet psn=%08u failed sending via SDMA.\n",
						  FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(we->replay)));
		}

		assert(we->replay->pinned == true);
		we->replay->pinned = false;

		/* If the replay is already marked as ACK'd, then it has
		   already been removed from the replay ring and we need
		   to free it here */
		if (we->replay->acked) {
			OPX_RELIABILITY_DEBUG_LOG(
				params->flow_key,
				"(tx) packet psn=%08u replay over SDMA complete and ACK'd, freeing replay\n",
				(uint32_t) (OPX_REPLAY_HDR(we->replay)->reliability.psn));
			fi_opx_reliability_service_replay_deallocate(opx_ep->reli_service, we->replay);

#ifdef OPX_RELIABILITY_DEBUG
		} else {
			fprintf(stderr,
				"(tx) packet flow=(%016lx %08x) psn=%08u replay over SDMA complete, un-pinning replay\n",
				params->flow_key->qw_prefix, params->flow_key->dw_suffix,
				(uint32_t) (OPX_REPLAY_HDR(we->replay)->reliability.psn));
#endif
		}
		slist_remove_head(&params->sdma_reqs);
		we->next = NULL;
		OPX_BUF_FREE(we);
		we = (struct fi_opx_hfi1_sdma_replay_work_entry *) params->sdma_reqs.head;
	}

	if (!slist_empty(&params->sdma_reqs)) {
		OPX_TRACER_TRACE_SDMA(OPX_TRACER_END_EAGAIN, "SDMA_RELI_REPLAY_COMPLETE");
		return -FI_EAGAIN;
	}

	OPX_RELIABILITY_DEBUG_LOG(params->flow_key, "(tx) SDMA Replay Complete - END - return FI_SUCCESS\n");

	OPX_TRACER_TRACE_SDMA(OPX_TRACER_END_SUCCESS, "SDMA_RELI_REPLAY_COMPLETE");
	return FI_SUCCESS;
}

ssize_t fi_opx_reliability_service_do_replay_sdma(struct fid_ep *ep, struct fi_opx_reliability_service *service,
						  struct fi_opx_reliability_tx_replay *start_replay,
						  struct fi_opx_reliability_tx_replay *end_replay, uint32_t num_replays,
						  const union fi_opx_reliability_service_flow_key *flow_key)
{
	OPX_TRACER_TRACE_SDMA(OPX_TRACER_BEGIN, "SDMA_RELI_DO_REPLAY");
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	union fi_opx_reliability_deferred_work *work = ofi_buf_alloc(service->rx.work_pending_pool);
	assert(work);
	struct fi_opx_reliability_tx_sdma_replay_params *params = &work->sdma_replay;
	params->work_elem.slist_entry.next			= NULL;
	params->work_elem.work_fn				= fi_opx_reliability_sdma_replay_complete;

	params->opx_ep = opx_ep;
	slist_init(&params->sdma_reqs);

	uint32_t replayed = 0;

#ifdef OPX_RELIABILITY_DEBUG
	params->flow_key = flow_key;
#endif

	struct fi_opx_reliability_tx_replay *replay = start_replay;
	while (replay && replayed < num_replays) {
		// Skip replaying any replays that are already in progress
		if (replay->pinned) {
			replay = replay->next;
			continue;
		}

		struct fi_opx_hfi1_sdma_replay_work_entry *sdma_we =
			(struct fi_opx_hfi1_sdma_replay_work_entry *) ofi_buf_alloc(
				service->rx.sdma_replay_request_pool);

		if (!sdma_we) {
			OPX_RELIABILITY_DEBUG_LOG(flow_key, "(tx) SDMA Replay: Couldn't allocate SDMA work entry\n");
			break;
		}

		sdma_we->next	    = NULL;
		sdma_we->replay	    = replay;
		sdma_we->comp_state = OPX_SDMA_COMP_PENDING_WRITEV;

		uint64_t payload_size = fi_opx_reliability_replay_get_payload_size(replay);

#ifndef NDEBUG
		fi_opx_hfi1_reliability_iov_payload_check(replay, flow_key,
							  "Replaying packet (SDMA) where source buffer has changed!",
							  __FILE__, __func__, __LINE__);
#endif
		int rc = opx_hfi1_sdma_enqueue_replay(opx_ep, sdma_we, replay, payload_size);
		assert(rc == FI_SUCCESS);
		if (OFI_UNLIKELY(rc != FI_SUCCESS)) {
			OPX_BUF_FREE(sdma_we);
			break;
		}
		OPX_RELIABILITY_DEBUG_LOG(flow_key, "(tx) %08u size %ld bytes replay injected over SDMA\n",
					  (uint32_t) (OPX_REPLAY_HDR(replay)->reliability.psn), payload_size);
		replay->pinned = true;
		++replayed;
		slist_insert_tail((struct slist_entry *) sdma_we, &params->sdma_reqs);

		replay = (replay == end_replay) ? NULL : replay->next;
	}

	if (OFI_LIKELY(!slist_empty(&params->sdma_reqs))) {
		slist_insert_tail(&work->work_elem.slist_entry, &service->rx.work_pending);
	} else {
		assert(replayed == 0);
		OPX_BUF_FREE(work);
	}

	OPX_RELIABILITY_DEBUG_LOG(flow_key, "(tx) Queued %d replays/packets for sending via SDMA\n", replayed);

	OPX_TRACER_TRACE_SDMA(OPX_TRACER_END_SUCCESS, "SDMA_RELI_DO_REPLAY");
	return replayed;
}

// Note: The context sharing pio lock must be held when entering this function
ssize_t fi_opx_reliability_service_do_replay(struct fi_opx_ep *opx_ep, struct fi_opx_reliability_service *service,
					     struct fi_opx_reliability_tx_replay *replay)
{
#if defined(OPX_RELIABILITY_DEBUG) || !defined(NDEBUG)
	union fi_opx_reliability_service_flow_key key;
	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		key.slid = (opx_lid_t) __be16_to_cpu24((__be16) replay->scb.scb_9B.hdr.lrh_9B.slid);
		key.dlid = (opx_lid_t) __be16_to_cpu24((__be16) replay->scb.scb_9B.hdr.lrh_9B.dlid);
	} else {
		key.slid = (opx_lid_t) __le24_to_cpu(replay->scb.scb_16B.hdr.lrh_16B.slid20 << 20 |
						     replay->scb.scb_16B.hdr.lrh_16B.slid);
		key.dlid = (opx_lid_t) __le24_to_cpu(replay->scb.scb_16B.hdr.lrh_16B.dlid20 << 20 |
						     replay->scb.scb_16B.hdr.lrh_16B.dlid);
	}
	key.src_subctxt_rx = (uint32_t) FI_OPX_HFI1_PACKET_ORIGIN_RX(OPX_REPLAY_HDR(replay));
	key.dst_subctxt_rx = (uint32_t) (OPX_REPLAY_HDR(replay)->bth.subctxt_rx & OPX_BTH_SUBCTXT_RX_MASK);

#endif

	/* runtime checks for non-inlined functions */
	const enum opx_hfi1_type hfi1_type = OPX_HFI1_TYPE;

	/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
	uint16_t lrh_pktlen_le;

	size_t	 total_bytes_to_copy;
	size_t	 payload_bytes_to_copy; /* payload without (16B) icrc tail */
	uint16_t payload_credits_needed;

	uint32_t payload_qw_to_copy_with_header = 0;
	bool	 tail_block_needed		= false; /* 16B tail needed */

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		lrh_pktlen_le	       = ntohs(replay->scb.scb_9B.hdr.lrh_9B.pktlen);
		total_bytes_to_copy    = (lrh_pktlen_le - 1) * 4; /* do not copy the trailing icrc */
		payload_bytes_to_copy  = total_bytes_to_copy - sizeof(struct fi_opx_hfi1_stl_packet_hdr_9B);
		payload_credits_needed = (payload_bytes_to_copy >> 6); /* number of full 64-byte blocks of payload */
	} else {
		lrh_pktlen_le	    = replay->scb.scb_16B.hdr.lrh_16B.pktlen;
		total_bytes_to_copy = (lrh_pktlen_le) * 8; /* including trailing icrc */
		/* do not copy icrc, it is "pad" not user data */
		payload_bytes_to_copy =
			(total_bytes_to_copy - sizeof(struct fi_opx_hfi1_stl_packet_hdr_16B) - 8 /* icrc */);
		payload_qw_to_copy_with_header = MIN((7 * 8), payload_bytes_to_copy) >> 3; /* up to 7 qwords */
		assert(payload_bytes_to_copy >= payload_qw_to_copy_with_header * 8);
		payload_bytes_to_copy -= payload_qw_to_copy_with_header << 3;
		payload_credits_needed =
			(payload_bytes_to_copy >> 6);	   /* number of full 64-byte blocks of payload - icrc */
		if (payload_qw_to_copy_with_header >= 7) { /* if tail is not in with hdr/payload block */
			tail_block_needed = true;	   /* tail needed even if there's no partial payload block*/
		}
	}

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	const uint16_t credits_needed	    = (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) ? 1 : 2;
	const uint16_t last_partial_block   = (((payload_bytes_to_copy & 0x3Ful) || tail_block_needed) ? 1 : 0);
	const uint16_t total_credits_needed = credits_needed +	       /* header */
					      payload_credits_needed + /* full payload blocks */
					      last_partial_block;      /* last partial block  */
	uint16_t total_credits_available = FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state);
	if (total_credits_available < total_credits_needed) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state);
		if (total_credits_available < total_credits_needed) {
			OPX_RELIABILITY_DEBUG_LOG(&key, "(tx) packet psn=%08u Couldn't do replay (no credits)\n",
						  (uint32_t) FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(replay)));

			opx_ep->tx->pio_state->qw0 = pio_state.qw0;

			return -FI_EAGAIN;
		}
	}

	OPX_RELIABILITY_DEBUG_LOG(&key, "(tx) psn=%08u replay injected\n",
				  (uint32_t) FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(replay)));

	uint64_t *buf_qws;
	if (replay->use_iov) {
#ifndef NDEBUG
		/* The pointer to the SDMA work entry should only be set when the
		 * SDMA WE bounce buffer is being used and the replay's IOV is
		 * pointing to it.  */
		if (replay->sdma_we) {
			struct fi_opx_hfi1_sdma_work_entry *sdma_we =
				(struct fi_opx_hfi1_sdma_work_entry *) replay->sdma_we;
			assert(sdma_we->pending_bounce_buf);
			assert(sdma_we->use_bounce_buf);

			/* If the use count of the SDMA WE at the time the replay
			 * was registered does not match the current use count of
			 * the SDMA WE, that means the SDMA WE was reused before
			 * it should have been, and the data we're about to
			 * replay is no longer valid. */
			if (replay->sdma_we_use_count != sdma_we->bounce_buf.use_count) {
				fprintf(stderr,
					"(%d) %s:%s():%d Replay pointing to potentially corrupt SDMA WE bounce buffer! "
					"replay->sdma_we_use_count=%u sdma_we->use_count=%u replay->iov=%p sdma_we->bounce_buf.buf=%p\n",
					getpid(), __FILE__, __func__, __LINE__, replay->sdma_we_use_count,
					sdma_we->bounce_buf.use_count, replay->iov[0].iov_base,
					sdma_we->bounce_buf.buf);
				assert(0);
			}
		}
		fi_opx_hfi1_reliability_iov_payload_check(replay, &key,
							  "Replaying packet (PIO) where source buffer has changed!",
							  __FILE__, __func__, __LINE__);
#endif
		buf_qws = replay->iov[0].iov_base;
	} else {
		buf_qws = replay->payload;
	}

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);
	opx_cacheline_store_block_vol(scb, replay->scb.qws);

	/* consume one credit for the packet header */
	--total_credits_available;
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned consumed_credits = 1;
#endif

	if (hfi1_type & OPX_HFI1_CNX000) {
		volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		// spill from 1st cacheline (SOP)
		OPX_HFI1_BAR_PIO_STORE(&scb_payload[0], replay->scb.scb_16B.hdr.qw_16B[7]); // header

		int i;

		for (i = 1; i <= payload_qw_to_copy_with_header; ++i) {
			OPX_HFI1_BAR_PIO_STORE(&scb_payload[i], *buf_qws);
			buf_qws += 1;
		}
		for (i = payload_qw_to_copy_with_header + 1; i <= 7; ++i) {
			OPX_HFI1_BAR_PIO_STORE(&scb_payload[i], OPX_JKR_16B_PAD_QWORD);
		}

		/* consume one credit for the packet header+payload */
		--total_credits_available;
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		consumed_credits = 2;
#endif
	}
	/* Copy full blocks of payload */
	while (payload_credits_needed) {
		volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		const uint16_t contiguous_scb_until_wrap =
			(uint16_t) (pio_state.credits_total - pio_state.scb_head_index);

		const uint16_t contiguous_credits_available = MIN(total_credits_available, contiguous_scb_until_wrap);

		const uint16_t contiguous_full_blocks_to_write =
			MIN(payload_credits_needed, contiguous_credits_available);

		uint16_t i;
		for (i = 0; i < contiguous_full_blocks_to_write; ++i) {
			opx_cacheline_store_block_vol(scb_payload, buf_qws);

			scb_payload += FI_OPX_CACHE_LINE_QWS;
			buf_qws += FI_OPX_CACHE_LINE_QWS;
		}

		payload_credits_needed -= contiguous_full_blocks_to_write;
		total_credits_available -= contiguous_full_blocks_to_write;
		FI_OPX_HFI1_CONSUME_CREDITS(pio_state, contiguous_full_blocks_to_write);
#ifndef NDEBUG
		consumed_credits += contiguous_full_blocks_to_write;
#endif
	}
	/* Store last partial 64-byte block of payload */
	if (last_partial_block != 0) {
		int16_t payload_tail_bytes = (payload_bytes_to_copy & 0x3Ful); /* not icrc/pad */

		/* We have a credit so we don't have to worry about this wrapping on one block */
		volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		uint16_t i = 0;
		for (; payload_tail_bytes >= 8; payload_tail_bytes -= 8) {
			OPX_HFI1_BAR_PIO_STORE(scb_payload, *buf_qws);
			scb_payload += 1;
			buf_qws += 1;
			i++;
		}

		/* LRH packets are dword (9B) or qword (16b) aligned */
		assert((payload_tail_bytes == 4) || (payload_tail_bytes == 0));
		if (!(hfi1_type & OPX_HFI1_CNX000)) {
			if (payload_tail_bytes) {
				OPX_HFI1_BAR_PIO_STORE(scb_payload, ((*buf_qws)));
				scb_payload += 1;
				i++;
			}
		} else {
			/* QWORD aligned for 16B */
			assert(payload_tail_bytes == 0);
			/* Have not yet stored icrc/pad */
			assert(i < 8);
		}
		/* Pad out the cacheline/block */
		for (; i < 8; i++) {
			OPX_HFI1_BAR_PIO_STORE(scb_payload, OPX_JKR_16B_PAD_QWORD);
			scb_payload += 1;
		}

		/* consume one credit for the tail partial block payload */
		--total_credits_available;
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		consumed_credits += 1;
#endif
	}

#ifndef NDEBUG
	assert(consumed_credits == total_credits_needed);
#endif

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	return FI_SUCCESS;
}

ssize_t fi_opx_reliability_pio_replay(union fi_opx_reliability_deferred_work *work)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RELI_PIO_REPLAY");
	struct fi_opx_reliability_tx_pio_replay_params *params = &work->pio_replay;
	struct fi_opx_ep			       *opx_ep = (struct fi_opx_ep *) params->opx_ep;

	OPX_RELIABILITY_DEBUG_LOG(params->flow_key,
				  "(tx) Executing deferred PIO Replay (%p) with start_index=%u, num_replay=%u\n",
				  params, params->start_index, params->num_replays);

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	for (int i = params->start_index; i < params->num_replays; ++i) {
		if (params->replays[i]->acked) {
			OPX_RELIABILITY_DEBUG_LOG(params->flow_key,
						  "(tx) psn=%08u replay already ACK'd, skipping deferred replay\n",
						  FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(params->replays[i])));
			fi_opx_reliability_service_replay_deallocate(opx_ep->reli_service, params->replays[i]);
			params->replays[i] = NULL;
			continue;
		}

		ssize_t rc = fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, params->replays[i]);
		if (rc == FI_SUCCESS) {
			params->replays[i]->pinned = false;
			params->replays[i]	   = NULL;
		} else {
			params->start_index = i;
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "RELI_PIO_REPLAY");
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RELI_PIO_REPLAY");
	return FI_SUCCESS;
}

void fi_opx_hfi1_rx_reliability_nack(struct fid_ep *ep, struct fi_opx_reliability_service *service,
				     const union fi_opx_reliability_service_flow_key *key, const uint64_t psn_count,
				     const uint64_t psn_start)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RELI_RX_NACK");
	assert(psn_count > 0);
	const uint64_t psn_stop = psn_start + psn_count - 1;

	INC_PING_STAT(NACKS_RECV, *key, psn_start, psn_count);
	if (psn_start > psn_stop) {
		fprintf(stderr,
			"%s:%s():%d (%016lx %08x) invalid nack received; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n",
			__FILE__, __func__, __LINE__, key->qw_prefix, key->dw_suffix, psn_start, psn_count, psn_stop);
		abort();
	}

	assert(psn_stop <= MAX_PSN);

	OPX_RELIABILITY_DEBUG_LOG(key, "(tx) rcv nack %08lu..%08lu\n", psn_start, psn_stop);
	void *itr = NULL;

	/* search for existing unack'd flows */
	itr = fi_opx_rbt_find(service->tx.tx_flow_outstanding_pkts_rbtree, (void *) key);
	ASSERT_FLOW_FOUND(itr, key, "Invalid key psn_start = %lx, psn_count = %lx, psn_stop = %lx\n", psn_start,
			  psn_count, psn_stop);

	struct fi_opx_reliability_tx_replay **value_ptr = (struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(
		service->tx.tx_flow_outstanding_pkts_rbtree, itr);

	struct fi_opx_reliability_tx_replay *head = *value_ptr;

	if (OFI_UNLIKELY(head == NULL)) {
		/*
		 * there are no unack'd elements in the replay queue;
		 * do nothing and return
		 */
		OPX_RELIABILITY_DEBUG_LOG(key, "(tx) rcv nack %08lu..%08lu No Unack'd replays in queue, ignoring\n",
					  psn_start, psn_stop);
		INC_PING_STAT(NACKS_IGNORED, *key, psn_start, psn_count);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RELI_RX_NACK");
		return;
	}

	struct fi_opx_reliability_tx_replay *tail = head->prev;

	/*
	 * find the first replay to retransmit. Skip any pinned (in progress) replays.
	 */

	struct fi_opx_reliability_tx_replay *start     = head;
	uint32_t			     start_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(start));
	while ((start_psn < psn_start || start->pinned) && (start != tail)) {
		OPX_RELIABILITY_DEBUG_LOG(
			key,
			"(tx) rcv nack %lu..%lu Looking for start replay, current start->psn == %u, start->pinned == %d\n",
			psn_start, psn_stop, FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(start)), start->pinned);
		start	  = start->next;
		start_psn = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(start));
	}

	if (OFI_UNLIKELY(start_psn < psn_start || start_psn > psn_stop || start->pinned)) {
		/*
		 * There are no unpinned replays within the nack range.
		 * Do nothing and return.
		 */
		OPX_RELIABILITY_DEBUG_LOG(
			key,
			"(tx) rcv nack %lu..%lu No Unack'd, unpinned replays in range, ignoring (start->psn == %u, start->pinned == %d\n",
			psn_start, psn_stop, start_psn, start->pinned);
		INC_PING_STAT(NACKS_IGNORED, *key, psn_start, psn_count);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RELI_RX_NACK");
		return;
	}

	/*
	 * We limit how many replays we do here (OPX_RELIABILITY_TX_MAX_REPLAYS)
	 * to limit running out of credits, although if the # of credits
	 * assigned to us by the driver is low, or our packets are large, we will
	 * probably run out of credits before we hit the limit. Running out of
	 * credits here isn't an error but it results in wasted work and a
	 * delay, but given that we're already dropping packets (we wouldn't be
	 * here if we weren't) being a little inefficient in re-sending them
	 * might actually allow the receiver time to catch up.
	 */

	/*
	 * We have at least 1 replay to retransmit, now find the last.
	 * Note that the nack range should never contain a PSN rollover,
	 * and we've already asserted that psn_stop >= psn_start.
	 */

	uint64_t			     replay_count = 1;
	struct fi_opx_reliability_tx_replay *stop	  = start;
	const uint64_t max = (uint64_t) MIN(OPX_RELIABILITY_TX_MAX_REPLAYS, OPX_RELIABILITY_RX_MAX_NACK);
	while ((stop->next != head) &&
	       (FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop)) < FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop->next))) &&
	       (FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop->next)) <= psn_stop) && (replay_count < max)) {
		// We won't retransmit pinned replays, so don't count those
		if (!stop->pinned) {
			++replay_count;
			++stop->nack_count;
		}
		stop = stop->next;
	}

	assert(replay_count <= psn_count);

	const struct fi_opx_reliability_tx_replay *const halt	= stop->next;
	struct fi_opx_reliability_tx_replay		*replay = start;

	OPX_RELIABILITY_DEBUG_LOG(key, "(tx) rcv nack %08lu..%08lu Replaying PSNs %08u - %08u\n", psn_start, psn_stop,
				  start_psn, (uint32_t) FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(stop)));

	// Turn on throttling for this flow while we catch up on replays
	start->psn_ptr->psn.nack_count = 1;

	union fi_opx_reliability_deferred_work	       *work	       = NULL;
	struct fi_opx_reliability_tx_pio_replay_params *params	       = NULL;
	bool						queing_replays = false;
	struct fi_opx_ep			       *opx_ep	       = container_of(ep, struct fi_opx_ep, ep_fid);

	/* We'll attempt to send each replay on the spot as long as sending the replay
	   succeeds. As soon as replaying fails, we'll queue the failed replay and
	   the remaining replays for retrying later */
	do {
		if (replay->pinned) {
			replay = replay->next;
			continue;
		}

		if (!replay->use_sdma) {
			if (!queing_replays) {
#ifdef OPX_DEBUG_COUNTERS_RELIABILITY
				if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(OPX_REPLAY_HDR(replay)->bth.opcode) ==
				    FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) {
					FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.replay_rts);
				} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(OPX_REPLAY_HDR(replay)->bth.opcode) ==
					   FI_OPX_HFI_BTH_OPCODE_RZV_CTS) {
					FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.replay_cts);
				} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(OPX_REPLAY_HDR(replay)->bth.opcode) ==
					   FI_OPX_HFI_BTH_OPCODE_RZV_DATA) {
					FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.replay_rzv);
				}
#endif
				OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

				if (fi_opx_reliability_service_do_replay(opx_ep, service, replay) != FI_SUCCESS) {
					queing_replays = true;

					work = ofi_buf_alloc(service->rx.work_pending_pool);
					assert(work);
					params				   = &work->pio_replay;
					params->flow_key		   = key;
					params->work_elem.slist_entry.next = NULL;
					params->work_elem.work_fn	   = fi_opx_reliability_pio_replay;
					params->opx_ep			   = opx_ep;
					params->start_index		   = 0;

					/* Add this replay to the queue */
					replay->pinned	    = true;
					params->replays[0]  = replay;
					params->num_replays = 1;
				}

				OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

			} else {
				replay->pinned			     = true;
				params->replays[params->num_replays] = replay;
				++params->num_replays;
			}

			replay = replay->next;
			continue;
		}
		OPX_RELIABILITY_DEBUG_LOG(key, "SDMA replay\n");
		uint32_t			     sdma_count = 1;
		struct fi_opx_reliability_tx_replay *sdma_start = replay;
		while (replay->next != halt && replay->next->use_sdma) {
			replay = replay->next;
			if (!replay->pinned) {
				++sdma_count;
			}
		}
#ifdef OPX_DEBUG_COUNTERS_RELIABILITY
		struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
		if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(OPX_REPLAY_HDR(replay)->bth.opcode) ==
		    FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.replay_rts);
		} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(OPX_REPLAY_HDR(replay)->bth.opcode) ==
			   FI_OPX_HFI_BTH_OPCODE_RZV_CTS) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.replay_cts);
		} else if (FI_OPX_HFI_BTH_OPCODE_BASE_OPCODE(OPX_REPLAY_HDR(replay)->bth.opcode) ==
			   FI_OPX_HFI_BTH_OPCODE_RZV_DATA) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.replay_rzv);
		}
#endif
		fi_opx_reliability_service_do_replay_sdma(ep, service, sdma_start, replay->next, sdma_count, key);
		replay = replay->next;
	} while (replay != halt);

	if (queing_replays) {
		assert(params->num_replays <= max); /* likely OPX_RELIABILITY_TX_MAX_REPLAYS */
		OPX_RELIABILITY_DEBUG_LOG(key, "(tx) Queuing deferred PIO Replay (%p) with num_replays=%u\n", params,
					  params->num_replays);
		slist_insert_tail(&work->work_elem.slist_entry, &service->rx.work_pending);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RELI_RX_NACK");
}

enum opx_reliability_ping_result {
	OPX_RELIABILITY_PING_NO_REPLAYS = -1,
	OPX_RELIABILITY_PING_NO_CREDITS, // NO_CREDITS = 0 to make the if statements in ping_remote clean
	OPX_RELIABILITY_PING_SENT
};

__OPX_FORCE_INLINE__
ssize_t fi_opx_reliability_send_ping(struct fid_ep *ep, struct fi_opx_reliability_service *service, RbtIterator itr,
				     const union fi_opx_reliability_service_flow_key *key)
{
	OPX_TRACER_TRACE_RELI(OPX_TRACER_BEGIN, "RELI_SEND_PING");
	struct fi_opx_reliability_tx_replay **value_ptr = (struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(
		service->tx.tx_flow_outstanding_pkts_rbtree, itr);

	struct fi_opx_reliability_tx_replay *head = *value_ptr;

	if (OFI_UNLIKELY(head == NULL)) {
		OPX_TRACER_TRACE_RELI(OPX_TRACER_END_ERROR, "RELI_SEND_PING");
		return OPX_RELIABILITY_PING_NO_REPLAYS;
	}

	opx_lid_t dlid;
	/* Inlined but called from non-inlined functions with no const hfi1 type, so just use the runtime check */
	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		dlid = (opx_lid_t) __be16_to_cpu24((__be16) head->scb.scb_9B.hdr.lrh_9B.dlid);
	} else {
		dlid = (opx_lid_t) __le24_to_cpu(head->scb.scb_16B.hdr.lrh_16B.dlid20 << 20 |
						 head->scb.scb_16B.hdr.lrh_16B.dlid);
	}

	const uint64_t rx = (uint64_t) head->target_reliability_subctxt_rx;

	// psn_start will always be 24-bit max number here
	uint64_t psn_start = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(head));
	uint64_t psn_stop  = FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(head->prev));

	// if the PSN of the tail is less than the PSN of the head, the
	// PSN has rolled over. In that case, truncate the ping range
	// to avoid rollover confusion.
	uint64_t psn_count = ((psn_start > psn_stop) ? MAX_PSN : psn_stop) - psn_start + 1;

	// Send one ping to cover the entire replay range.
	ssize_t rc = fi_opx_hfi1_tx_reliability_inject(ep, key, dlid, rx, psn_start, psn_count,
						       FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING, OPX_HFI1_TYPE,
						       OPX_IS_CTX_SHARING_ENABLED);

	INC_PING_STAT_COND(rc == FI_SUCCESS, PINGS_SENT, *key, psn_start, psn_count);

	OPX_TRACER_TRACE_RELI(OPX_TRACER_END_SUCCESS, "RELI_SEND_PING");

	if (rc) {
		return OPX_RELIABILITY_PING_NO_CREDITS;
	}

	return OPX_RELIABILITY_PING_SENT;
}

void fi_reliability_service_ping_remote(struct fid_ep *ep, struct fi_opx_reliability_service *service)
{
	/* for each flow in the rbtree ... */
	RbtIterator start_key_itr;
	RbtIterator itr;

	union fi_opx_reliability_service_flow_key *key_ptr   = NULL;
	ssize_t					   rc	     = OPX_RELIABILITY_PING_SENT;
	uint16_t				   num_pings = 0;
	uint16_t max_pings = service->congested_flag ? service->max_congested_pings : service->max_uncongested_pings;

	const union fi_opx_reliability_service_flow_key *start_key = service->ping_start_key;
	if (start_key) {
		itr	      = fi_opx_rbt_find(service->tx.tx_flow_outstanding_pkts_rbtree, (void *) start_key);
		start_key_itr = itr;
	} else {
		itr	      = rbtBegin(service->tx.tx_flow_outstanding_pkts_rbtree);
		start_key_itr = NULL;
	}

	/* Loop until we hit the end of the tree, or we fail on a particular ping */
	while (itr && rc && num_pings < max_pings) {
		fi_opx_rbt_key(itr, (void **) &key_ptr);

		rc = fi_opx_reliability_send_ping(ep, service, itr, key_ptr);

		/* advance to the next dlid */
		itr = rbtNext(service->tx.tx_flow_outstanding_pkts_rbtree, itr);

		if (rc == OPX_RELIABILITY_PING_SENT) {
			++num_pings;
		}
	}

	/* We ran out of credits on a particular ping.
	 * Store the failing key to be the first to try next time,
	 * set the congested flag to limit future pings, and stop */
	if (!rc) {
		service->congested_flag = 1;
		service->ping_start_key = key_ptr;
		return;
	}

	// We sent the max number of pings this round, save the next key and stop
	if (num_pings == max_pings) {
		if (itr) {
			fi_opx_rbt_key(itr, (void **) &key_ptr);
			service->ping_start_key = key_ptr;
			return;
		}
		service->ping_start_key = NULL;
		return;
	}

	/* We hit the end of the tree. If there was no starting key, we've iterated through the whole tree and we're
	 * done. */
	if (!start_key) {
		// Unset the congested flag
		service->congested_flag = 0;
		return;
	}

	/* Wrap back around from the beginning of the tree and iterate until we've hit the starting key */
	itr = rbtBegin(service->tx.tx_flow_outstanding_pkts_rbtree);

	while (itr && itr != start_key_itr && rc && num_pings < max_pings) {
		fi_opx_rbt_key(itr, (void **) &key_ptr);

		rc = fi_opx_reliability_send_ping(ep, service, itr, key_ptr);

		/* advance to the next dlid */
		itr = rbtNext(service->tx.tx_flow_outstanding_pkts_rbtree, itr);

		if (rc == OPX_RELIABILITY_PING_SENT) {
			++num_pings;
		}
	}

	if (!rc) {
		service->congested_flag = 1;
		service->ping_start_key = key_ptr;
		return;
	}

	if (num_pings == max_pings) {
		if (itr) {
			fi_opx_rbt_key(itr, (void **) &key_ptr);
			service->ping_start_key = key_ptr;
			return;
		}
		service->ping_start_key = NULL;
		return;
	}

	service->ping_start_key = NULL;

	// We iterated through the whole tree, unset the congested flag
	service->congested_flag = 0;
}

void fi_opx_reliability_service_process_pending(struct fi_opx_reliability_service *service)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RELI_PROCESS_PENDING");
	assert(!slist_empty(&service->rx.work_pending));

	union fi_opx_reliability_deferred_work *work =
		(union fi_opx_reliability_deferred_work *) slist_remove_head(&service->rx.work_pending);

	work->work_elem.slist_entry.next = NULL;
	ssize_t rc			 = work->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
	} else {
		slist_insert_head(&work->work_elem.slist_entry, &service->rx.work_pending);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RELI_PROCESS_PENDING");
}

void fi_opx_reliability_model_init_9B(struct fi_opx_reliability_service *service, struct fi_opx_hfi1_context *hfi1)
{
	/* 'ping' pio send model */
	{
		/* PBC */
		const uint64_t pbc_dws = 2 + /* pbc */
					 2 + /* lrh */
					 3 + /* bth */
					 9;  /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */

		/* Setup the 9B models whether or not they'll be used */
		enum opx_hfi1_type __attribute__((unused)) hfi1_type =
			(OPX_HFI1_TYPE & OPX_HFI1_WFR) ? OPX_HFI1_WFR : OPX_HFI1_JKR_9B;

		service->ping_model_9B.qw0 =
			OPX_PBC_LEN(pbc_dws, hfi1_type) | OPX_PBC_VL(hfi1->vl, hfi1_type) |
			OPX_PBC_SC(hfi1->sc, hfi1_type) | OPX_PBC_L2TYPE(OPX_PBC_JKR_L2TYPE_9B, hfi1_type) |
			OPX_PBC_L2COMPRESSED(0, hfi1_type) | OPX_PBC_PORTIDX(hfi1->hfi_port, hfi1_type) |
			OPX_PBC_SCTXT(hfi1->send_ctxt, hfi1_type);

		/* LRH */
		service->ping_model_9B.hdr.lrh_9B.flags =
			htons(FI_OPX_HFI1_LRH_BTH | ((hfi1->sl & FI_OPX_HFI1_LRH_SL_MASK) << FI_OPX_HFI1_LRH_SL_SHIFT) |
			      ((hfi1->sc & FI_OPX_HFI1_LRH_SC_MASK) << FI_OPX_HFI1_LRH_SC_SHIFT));

		service->ping_model_9B.hdr.lrh_9B.dlid = 0; /* set at runtime */
		service->ping_model_9B.hdr.lrh_9B.pktlen =
			htons(pbc_dws - 2 +
			      1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
		service->ping_model_9B.hdr.lrh_9B.slid = htons(hfi1->lid);

		/* BTH */
		service->ping_model_9B.hdr.bth.opcode	  = FI_OPX_HFI_BTH_OPCODE_UD;
		service->ping_model_9B.hdr.bth.bth_1	  = 0;
		service->ping_model_9B.hdr.bth.pkey	  = htons(hfi1->pkey);
		service->ping_model_9B.hdr.bth.ecn	  = (uint8_t) ((OPX_BTH_RC2_VAL(hfi1_type, OPX_HFI1_INJECT)) |
								       OPX_BTH_CSPEC(OPX_BTH_CSPEC_DEFAULT, hfi1_type));
		service->ping_model_9B.hdr.bth.qp	  = hfi1->bthqp;
		service->ping_model_9B.hdr.bth.subctxt_rx = 0; /* set at runtime */

		/* KDETH */
		service->ping_model_9B.hdr.kdeth.offset_ver_tid = FI_OPX_HFI1_KDETH_VERSION
								  << FI_OPX_HFI1_KDETH_VERSION_SHIFT;
		service->ping_model_9B.hdr.kdeth.jkey	= hfi1->jkey;
		service->ping_model_9B.hdr.kdeth.hcrc	= 0;
		service->ping_model_9B.hdr.kdeth.unused = 0;

		/* reliability service */
		union opx_hfi1_packet_hdr *hdr = &service->ping_model_9B.hdr;

		hdr->ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING;

		hdr->service.origin_reliability_subctxt_rx = service->subctxt_rx;
		hdr->service.key_dw_suffix		   = 0;
		hdr->service.psn_count			   = 0;
		hdr->service.psn_start			   = 0;
		hdr->service.key			   = 0;

		OPX_DEBUG_PRINT_HDR((hdr), hfi1_type);

		/* 'ack' pio send model */
		service->ack_model_9B		    = service->ping_model_9B;
		service->ack_model_9B.hdr.ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK;

		OPX_DEBUG_PRINT_HDR((&(service->ack_model_9B.hdr)), hfi1_type);

		/* 'nack' pio send model */
		service->nack_model_9B		     = service->ping_model_9B;
		service->nack_model_9B.hdr.ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK;

		OPX_DEBUG_PRINT_HDR((&(service->nack_model_9B.hdr)), hfi1_type);
	}
}

void fi_opx_reliability_model_init_16B(struct fi_opx_reliability_service *service, struct fi_opx_hfi1_context *hfi1)
{
	/* Ping model */
	/* PBC */
	const uint64_t pbc_dws = 2 + /* pbc */
				 4 + /* lrh uncompressed */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 2;  /* ICRC/tail */

	/* Setup the 16B models whether or not they'll be used */
	enum opx_hfi1_type __attribute__((unused)) hfi1_type = OPX_HFI1_CYR;

	service->ping_model_16B.qw0 = OPX_PBC_LEN(pbc_dws, hfi1_type) | OPX_PBC_VL(hfi1->vl, hfi1_type) |
				      OPX_PBC_SC(hfi1->sc, hfi1_type) |
				      OPX_PBC_L2TYPE(OPX_PBC_JKR_L2TYPE_16B, hfi1_type) |
				      OPX_PBC_L2COMPRESSED(0, hfi1_type) | OPX_PBC_PORTIDX(hfi1->hfi_port, hfi1_type) |
				      OPX_PBC_SCTXT(hfi1->send_ctxt, hfi1_type) | OPX_PBC_JKR_INSERT_NON9B_ICRC;

	/* LRH */
	/* (LRH QW) does not include pbc (8 bytes) */
	const uint32_t packetLength = (pbc_dws - 2) * 4;
	const uint32_t lrh_qws	    = (packetLength >> 3) + ((packetLength & 0x07u) != 0);

	service->ping_model_16B.hdr.lrh_16B.qw[0]   = 0UL;
	service->ping_model_16B.hdr.lrh_16B.qw[1]   = 0UL;
	service->ping_model_16B.hdr.lrh_16B.pktlen  = lrh_qws;
	service->ping_model_16B.hdr.lrh_16B.sc	    = hfi1->sc;
	service->ping_model_16B.hdr.lrh_16B.entropy = hfi1->ctrl->ctxt_info.send_ctxt;
	service->ping_model_16B.hdr.lrh_16B.lt	    = 0; // need to add env variable to change
	service->ping_model_16B.hdr.lrh_16B.l2	    = OPX_PBC_JKR_L2TYPE_16B;
	service->ping_model_16B.hdr.lrh_16B.l4	    = 9;
	service->ping_model_16B.hdr.lrh_16B.rc	    = OPX_LRH_JKR_16B_RC(OPX_HFI1_INJECT);
	service->ping_model_16B.hdr.lrh_16B.cspec   = OPX_BTH_CSPEC_DEFAULT; /*NOT BTH CSPEC*/
	service->ping_model_16B.hdr.lrh_16B.pkey    = hfi1->pkey;

	service->ping_model_16B.hdr.lrh_16B.slid   = hfi1->lid & 0xFFFFF;
	service->ping_model_16B.hdr.lrh_16B.slid20 = (hfi1->lid) >> 20;

	/* BTH */
	service->ping_model_16B.hdr.bth.opcode	   = FI_OPX_HFI_BTH_OPCODE_UD;
	service->ping_model_16B.hdr.bth.bth_1	   = 0;
	service->ping_model_16B.hdr.bth.pkey	   = hfi1->pkey;
	service->ping_model_16B.hdr.bth.ecn	   = (uint8_t) ((OPX_BTH_RC2_VAL(hfi1_type, OPX_HFI1_INJECT)) |
								OPX_BTH_CSPEC(OPX_BTH_CSPEC_DEFAULT, hfi1_type));
	service->ping_model_16B.hdr.bth.qp	   = hfi1->bthqp;
	service->ping_model_16B.hdr.bth.subctxt_rx = 0; /* set at runtime */
	service->ping_model_16B.hdr.bth.psn	   = 0;

	/* KDETH */
	service->ping_model_16B.hdr.kdeth.offset_ver_tid = FI_OPX_HFI1_KDETH_VERSION << FI_OPX_HFI1_KDETH_VERSION_SHIFT;
	service->ping_model_16B.hdr.kdeth.jkey		 = hfi1->jkey;
	service->ping_model_16B.hdr.kdeth.hcrc		 = 0;
	service->ping_model_16B.hdr.kdeth.unused	 = 0;

	/* reliability service */
	union opx_hfi1_packet_hdr *hdr = &service->ping_model_16B.hdr;

	hdr->ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING;

	hdr->service.origin_reliability_subctxt_rx = service->subctxt_rx;
	hdr->service.key_dw_suffix		   = 0;
	hdr->service.psn_count			   = 0;
	hdr->service.psn_start			   = 0;
	hdr->service.key			   = 0;

	OPX_DEBUG_PRINT_HDR((hdr), hfi1_type);

	/* 'ack' pio send model */
	service->ack_model_16B		     = service->ping_model_16B;
	service->ack_model_16B.hdr.ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK;

	OPX_DEBUG_PRINT_HDR((&(service->ack_model_16B.hdr)), hfi1_type);

	/* 'nack' pio send model */
	service->nack_model_16B		      = service->ping_model_16B;
	service->nack_model_16B.hdr.ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK;

	OPX_DEBUG_PRINT_HDR((&(service->nack_model_16B.hdr)), hfi1_type);
}

void fi_opx_reliability_service_init(struct fi_opx_reliability_service *service, struct fi_opx_hfi1_context *hfi1,
				     void (*process_fn)(struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
							const uint8_t *const payload),
				     const enum ofi_reliability_kind reliability_kind)
{
	assert(hfi1 != NULL);
	assert(service != NULL);
	assert(process_fn != NULL);

	service->lid	    = hfi1->lid;
	service->kind	    = reliability_kind;
	service->process_fn = process_fn;

	service->subctxt_rx = (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR | OPX_HFI1_JKR_9B)) ?
				      __cpu_to_be16(hfi1->subctxt << 8 | hfi1->info.rxe.id) :
				      __cpu_to_be16(hfi1->subctxt << 9 | hfi1->info.rxe.id);

	service->rx.rx_flow_rbtree		    = rbtNew(fi_opx_reliability_compare);
	service->tx.tx_flow_outstanding_pkts_rbtree = rbtNew(fi_opx_reliability_compare);
	service->tx.tx_flow_rbtree		    = rbtNew(fi_opx_reliability_compare);

	/*
	 * The replay pools are used for the main send path. The pools have
	 * a fixed size and are not permitted to grow, in the theory that
	 * if a receiver is dropping packets, we should throttle the sender
	 * by returning an EAGAIN until the # of outstanding packets falls.
	 */
	(void) ofi_bufpool_create(&(service->tx.replay_pool),
				  OPX_RELIABILITY_TX_REPLAY_SIZE,	       // element size
				  FI_OPX_CACHE_LINE_SIZE,		       // byte alignment
				  FI_OPX_RELIABILITY_TX_REPLAY_BLOCKS,	       // max # of elements
				  FI_OPX_RELIABILITY_TX_REPLAY_BLOCKS,	       // # of elements to allocate at once
				  OFI_BUFPOOL_NO_TRACK | OFI_BUFPOOL_NO_ZERO); // flags
	(void) ofi_bufpool_create(&(service->tx.replay_iov_pool),
				  OPX_RELIABILITY_TX_REPLAY_IOV_SIZE,	       // element size
				  FI_OPX_CACHE_LINE_SIZE,		       // byte alignment
				  FI_OPX_RELIABILITY_TX_REPLAY_IOV_BLOCKS,     // max # of elements
				  FI_OPX_RELIABILITY_TX_REPLAY_IOV_BLOCKS,     // # of elements to allocate at once
				  OFI_BUFPOOL_NO_TRACK | OFI_BUFPOOL_NO_ZERO); // flags

	// Flow key pool for allocating reliability flow keys to be used in new rbtree entries.
	(void) ofi_bufpool_create(&(service->flow_key_pool),
				  sizeof(union fi_opx_reliability_service_flow_key), // element size
				  sizeof(void *),				     // byte alignment
				  0,						     // max # of elements
				  2048,					       // # of elements to allocate at once
				  OFI_BUFPOOL_NO_TRACK | OFI_BUFPOOL_NO_ZERO); // flags

	/*
	 * Buffer pool for allocating rx flow structures for when a new rx flow is established. These are stored
	 * in the rx_flow_rbtree and accessed with a reliability flow key.
	 */
	(void) ofi_bufpool_create(&(service->rx.rx_flow_pool),
				  sizeof(struct fi_opx_reliability_rx_flow),   // element size
				  sizeof(void *),			       // byte alignment
				  0,					       // max # of elements
				  2048,					       // # of elements to allocate at once
				  OFI_BUFPOOL_NO_TRACK | OFI_BUFPOOL_NO_ZERO); // flags

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "%s:%s():%d replay_pool = %p\n", __FILE__, __func__, __LINE__, service->tx.replay_pool);
	fprintf(stderr, "%s:%s():%d replay_iov_pool = %p\n", __FILE__, __func__, __LINE__, service->tx.replay_iov_pool);
	fprintf(stderr, "%s:%s():%d flow_key_pool = %p\n", __FILE__, __func__, __LINE__, service->flow_key_pool);
	fprintf(stderr, "%s:%s():%d rx_flow_pool = %p\n", __FILE__, __func__, __LINE__, service->rx.rx_flow_pool);
#endif

#ifdef OPX_RELIABILITY_TEST
	/*
	 * deliberately drop a percentage of packets in order to exercise the
	 * reliability service.
	 */
	service->drop_count = 0;
	service->drop_mask  = 0x00FF; /* default: drop every 256'th packet */
	char *env	    = getenv("FI_OPX_RELIABILITY_SERVICE_DROP_PACKET_MASK");
	if (env) {
		uint16_t mask = (uint16_t) strtoul(env, NULL, 16);
		fprintf(stderr, "%s():%d FI_OPX_RELIABILITY_SERVICE_DROP_PACKET_MASK = '%s' (0x%04hx)\n", __func__,
			__LINE__, env, mask);
		service->drop_mask = mask;
	}
#endif

	/*
	 * Initialize the timer that is used for checking when pings should be sent out for outstanding replay packets
	 * and also for processing any coalesced ping requests.
	 */
	fi_opx_timer_init(&service->timer);
	fi_opx_timer_now(&service->timestamp, &service->timer);

	/*
	 * How often to preemptively acknowledge packets.
	 * The default is 64, which indicates send a
	 * preemptive, non-solicited ACK after 64 packets received.
	 * Must be a power of 2, so that we can create an AND mask
	 * that will quickly tell us whether or not to ack
	 * Setting to 0 disables this feature
	 */
	int preemptive_ack_rate = 64;
	if (fi_param_get_int(fi_opx_global.prov, "reliability_service_pre_ack_rate", &preemptive_ack_rate) ==
	    FI_SUCCESS) {
		if (preemptive_ack_rate < 0 || preemptive_ack_rate > 32768) {
			FI_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE has value %d which is outside the valid range of 0-32,768. Using default rate of 64\n",
				preemptive_ack_rate);
			preemptive_ack_rate = 64;
		} else if (preemptive_ack_rate & (preemptive_ack_rate - 1)) {
			FI_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE has value %d which is not a power of 2. Using default rate of 64\n",
				preemptive_ack_rate);
			preemptive_ack_rate = 64;
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				 "Using environment-specified FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE of %d%s\n",
				 preemptive_ack_rate, preemptive_ack_rate ? "" : " (disabled)");
		}
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE not specified, using default value of 64\n");
	}

	/* Subtract 1 from the rate to give us the AND mask we want to use for PSN comparison. */
	service->preemptive_ack_rate	  = (uint32_t) preemptive_ack_rate;
	service->preemptive_ack_rate_mask = ((uint32_t) preemptive_ack_rate) - 1;

	/*
	 * How often to send ping requests
	 *
	 * ONLOAD
	 */
	int	usec;
	ssize_t rc = fi_param_get_int(fi_opx_global.prov, "reliability_service_usec_max", &usec);
	if (rc == FI_SUCCESS) {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_USEC_MAX set to %d\n", usec);
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "FI_OPX_RELIABILITY_SERVICE_USEC_MAX not specified, using default value of 500\n");
		usec = 500;
	}
	service->usec_max = usec;

	service->usec_next = fi_opx_timer_next_event_usec(&service->timer, &service->timestamp, service->usec_max);

	/*
	 * Max outstanding bytes for a given flow
	 */
	int max_outstanding;
	rc = fi_param_get_int(fi_opx_global.prov, "reliability_service_max_outstanding_bytes", &max_outstanding);
	if (rc == FI_SUCCESS) {
		if (max_outstanding < OPX_HFI1_PKT_SIZE ||
		    max_outstanding > OPX_RELIABILITY_MAX_OUTSTANDING_BYTES_MAX) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Error: FI_OPX_RELIABILITY_SERVICE_MAX_OUTSTANDING_BYTES was set but is outside min/max thresholds (%d-%d). Using default setting of %d\n",
				OPX_HFI1_PKT_SIZE, OPX_RELIABILITY_MAX_OUTSTANDING_BYTES_MAX,
				OPX_RELIABILITY_MAX_OUTSTANDING_BYTES_DEFAULT);
			service->max_outstanding_bytes = OPX_RELIABILITY_MAX_OUTSTANDING_BYTES_DEFAULT;
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				 "FI_OPX_RELIABILITY_SERVICE_MAX_OUTSTANDING_BYTES set to %d\n", max_outstanding);
			service->max_outstanding_bytes = max_outstanding;
		}
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "FI_OPX_RELIABILITY_SERVICE_MAX_OUTSTANDING_BYTES not specified, using default value of %d\n",
			 OPX_RELIABILITY_MAX_OUTSTANDING_BYTES_DEFAULT);
		service->max_outstanding_bytes = OPX_RELIABILITY_MAX_OUTSTANDING_BYTES_DEFAULT;
	}

	/*
	 * Initialize send ping flag(s)
	 *
	 * ONLOAD
	 */
	service->congested_flag = 0;

	/*
	 * Maximum number of reliability pings per timer in congested/uncongested scenarios
	 *
	 * ONLOAD
	 */
	int max_uncongested_pings;
	if (fi_param_get_int(fi_opx_global.prov, "reliability_max_uncongested_pings", &max_uncongested_pings) ==
	    FI_SUCCESS) {
		if (max_uncongested_pings < OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_MIN ||
		    max_uncongested_pings > OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_MAX) {
			FI_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_RELIABILITY_MAX_UNCONGESTED_PINGS has value %d which is outside the valid range of %d-%d. Using default rate of %d\n",
				max_uncongested_pings, OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_MIN,
				OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_MAX,
				OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_DEFAULT);
			max_uncongested_pings = OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_DEFAULT;
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				 "Using environment-specified FI_OPX_RELIABILITY_MAX_UNCONGESTED_PINGS of %d\n",
				 max_uncongested_pings);
		}
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "FI_OPX_RELIABILITY_MAX_UNCONGESTED_PINGS not specified, using default value of %d\n",
			 OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_DEFAULT);
		max_uncongested_pings = OPX_RELIABILITY_MAX_UNCONGESTED_PINGS_DEFAULT;
	}
	service->max_uncongested_pings = max_uncongested_pings;

	int max_congested_pings;
	if (fi_param_get_int(fi_opx_global.prov, "reliability_max_congested_pings", &max_congested_pings) ==
	    FI_SUCCESS) {
		if (max_congested_pings < OPX_RELIABILITY_MAX_CONGESTED_PINGS_MIN ||
		    max_congested_pings > OPX_RELIABILITY_MAX_CONGESTED_PINGS_MAX) {
			FI_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_RELIABILITY_MAX_CONGESTED_PINGS has value %d which is outside the valid range of %d-%d. Using default rate of %d\n",
				max_congested_pings, OPX_RELIABILITY_MAX_CONGESTED_PINGS_MIN,
				OPX_RELIABILITY_MAX_CONGESTED_PINGS_MAX, OPX_RELIABILITY_MAX_CONGESTED_PINGS_DEFAULT);
			max_congested_pings = OPX_RELIABILITY_MAX_CONGESTED_PINGS_DEFAULT;
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				 "Using environment-specified FI_OPX_RELIABILITY_MAX_CONGESTED_PINGS of %d\n",
				 max_congested_pings);
		}
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "FI_OPX_RELIABILITY_MAX_CONGESTED_PINGS not specified, using default value of %d\n",
			 OPX_RELIABILITY_MAX_CONGESTED_PINGS_DEFAULT);
		max_congested_pings = OPX_RELIABILITY_MAX_CONGESTED_PINGS_DEFAULT;
	}
	service->max_congested_pings = max_congested_pings;

	// PENDING_RX_RELIABLITY
	ofi_bufpool_create(&service->rx.pending_rx_reliability_pool, sizeof(struct fi_opx_pending_rx_reliability_op), 0,
			   UINT_MAX, PENDING_RX_RELIABLITY_COUNT_MAX, OFI_BUFPOOL_NO_ZERO);

	service->pending_rx_reliability_ops_hashmap = NULL;

	ofi_bufpool_create(&service->rx.uepkt_pool, sizeof(struct fi_opx_reliability_rx_uepkt), 64, UINT_MAX, 1024,
			   OFI_BUFPOOL_NO_ZERO);

	ofi_bufpool_create(&service->rx.work_pending_pool, sizeof(union fi_opx_reliability_deferred_work), 0, UINT_MAX,
			   1024, OFI_BUFPOOL_NO_ZERO);

	ofi_bufpool_create(&service->rx.sdma_replay_request_pool, sizeof(struct fi_opx_hfi1_sdma_replay_work_entry), 8,
			   UINT_MAX, 1024, OFI_BUFPOOL_NO_ZERO);

	slist_init(&service->rx.work_pending);

	fi_opx_reliability_model_init_16B(service, hfi1);
	fi_opx_reliability_model_init_9B(service, hfi1);

#ifdef OPX_DAOS
	service->flow_rbtree_resynch = rbtNew(fi_opx_reliability_compare);
#endif

	return;
}

void fi_opx_reliability_service_fini(struct fi_opx_reliability_service *service)
{
#ifdef OPX_DEBUG_COUNTERS_RELIABILITY_PING
	dump_ping_counts();
#endif

	if (service->rx.pending_rx_reliability_pool) {
		ofi_bufpool_destroy(service->rx.pending_rx_reliability_pool);
	}

	if (service->rx.uepkt_pool) {
		ofi_bufpool_destroy(service->rx.uepkt_pool);
	}

	if (service->rx.work_pending_pool) {
		ofi_bufpool_destroy(service->rx.work_pending_pool);
	}

	if (service->rx.sdma_replay_request_pool) {
		ofi_bufpool_destroy(service->rx.sdma_replay_request_pool);
	}

	if (service->tx.tx_flow_outstanding_pkts_rbtree) {
		rbtDelete(service->tx.tx_flow_outstanding_pkts_rbtree);
	}

	if (service->tx.replay_pool) {
		ofi_bufpool_destroy(service->tx.replay_pool);
		service->tx.replay_pool = NULL;
	}
	if (service->tx.replay_iov_pool) {
		ofi_bufpool_destroy(service->tx.replay_iov_pool);
		service->tx.replay_iov_pool = NULL;
	}

	if (service->rx.rx_flow_rbtree) {
		rbtDelete(service->rx.rx_flow_rbtree);
		service->rx.rx_flow_rbtree = NULL;
	}

	if (service->tx.tx_flow_rbtree) {
		rbtDelete(service->tx.tx_flow_rbtree);
		service->tx.tx_flow_rbtree = NULL;
	}

	if (service->flow_key_pool) {
		ofi_bufpool_destroy(service->flow_key_pool);
		service->flow_key_pool = NULL;
	}

	if (service->rx.rx_flow_pool) {
		ofi_bufpool_destroy(service->rx.rx_flow_pool);
		service->rx.rx_flow_pool = NULL;
	}

#ifdef OPX_DAOS
	if (service->flow_rbtree_resynch) {
		rbtDelete(service->flow_rbtree_resynch);
		service->flow_rbtree_resynch = NULL;
	}
#endif

	return;
}

__OPX_FORCE_INLINE__
bool opx_reliability_rx_process_check(struct fi_opx_reliability_service *service, const uint8_t opcode,
				      struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
				      const uint8_t *payload)
{
	const uint8_t dput_opcode = FI_OPX_HFI_DPUT_GET_OPCODE(hdr->dput.target.opcode_origin_rx);

	/* Process some RZV DATA opcodes immediately/OOO. */
	if ((opcode == FI_OPX_HFI_BTH_OPCODE_RZV_DATA) &&
	    ((dput_opcode == FI_OPX_HFI_DPUT_OPCODE_RZV) || (dput_opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID) ||
	     (dput_opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG))) {
		service->process_fn(ep, hdr, payload);

#ifdef OPX_DEBUG_COUNTERS_RELIABILITY
		struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.reliability.rzv_data_ooo);
#endif

		return true; /* processed */
	}

	return false; /* not processed */
}

__OPX_FORCE_INLINE__
struct fi_opx_reliability_rx_uepkt *fi_opx_reliability_allocate_uepkt(struct fi_opx_reliability_service	    *service,
								      const union opx_hfi1_packet_hdr *const hdr,
								      const uint8_t *const		     payload,
								      const size_t  payload_bytes_to_copy,
								      const uint8_t opcode, struct fid_ep *ep)
{
	struct fi_opx_reliability_rx_uepkt *tmp = ofi_buf_alloc(service->rx.uepkt_pool);
	assert(tmp);
	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		tmp->hdr.qw_9B[0] = hdr->qw_9B[0];
		tmp->hdr.qw_9B[1] = hdr->qw_9B[1];
		tmp->hdr.qw_9B[2] = hdr->qw_9B[2];
		tmp->hdr.qw_9B[3] = hdr->qw_9B[3];
		tmp->hdr.qw_9B[4] = hdr->qw_9B[4];
		tmp->hdr.qw_9B[5] = hdr->qw_9B[5];
		tmp->hdr.qw_9B[6] = hdr->qw_9B[6];
	} else {
		tmp->hdr.qw_16B[0] = hdr->qw_16B[0];
		tmp->hdr.qw_16B[1] = hdr->qw_16B[1];
		tmp->hdr.qw_16B[2] = hdr->qw_16B[2];
		tmp->hdr.qw_16B[3] = hdr->qw_16B[3];
		tmp->hdr.qw_16B[4] = hdr->qw_16B[4];
		tmp->hdr.qw_16B[5] = hdr->qw_16B[5];
		tmp->hdr.qw_16B[6] = hdr->qw_16B[6];
		tmp->hdr.qw_16B[7] = hdr->qw_16B[7];
	}

	/* We can process some opcodes immediately/OOO and no longer need the payload.
	 * The uepkt bool will prevent this uepkt from being processed again from the queue. */
	if (!(tmp->processed = opx_reliability_rx_process_check(service, opcode, ep, hdr, payload)) && payload &&
	    (payload_bytes_to_copy > 0)) {
		memcpy((void *) &tmp->payload[0], (const void *) payload, payload_bytes_to_copy);
	}

	return tmp;
}

void fi_opx_reliability_rx_exception(struct fi_opx_reliability_service *service,
				     struct fi_opx_reliability_rx_flow *flow, opx_lid_t slid, uint64_t src_origin_rx,
				     uint32_t psn, struct fid_ep *ep, const union opx_hfi1_packet_hdr *const hdr,
				     const uint8_t *const payload, const enum opx_hfi1_type hfi1_type,
				     const uint8_t opcode, const bool ctx_sharing)
{
	uint64_t next_psn = flow->next_psn;

	if (OFI_LIKELY((next_psn & MAX_PSN) == psn)) {
		/*
		 * the 24-bit PSN in the packet matches the low 24 bits of the
		 * expected PSN.  Deliver this packet and the next contiguous sequence
		 * of previously queued unexpected packets.
		 *
		 * NOTE: We assume that it is impossible for the 64-bit values of
		 * next_psn and psn to be more than MAX_PSN apart making it safe
		 * to use the 24-bit versions to test equality.
		 */
		OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u received (process out-of-order).\n", psn);
		service->process_fn(ep, hdr, payload);

		if (!(psn & service->preemptive_ack_rate_mask) && psn) {
			fi_opx_hfi1_rx_reliability_send_pre_acks(ep, service->lid, service->subctxt_rx,
								 psn - service->preemptive_ack_rate + 1, /* psn_start */
								 service->preemptive_ack_rate,		 /* psn_count */
								 hdr, slid, hfi1_type, ctx_sharing);
		}

		next_psn += 1;

		flow->next_psn = next_psn;

		struct fi_opx_reliability_rx_uepkt *uepkt = flow->uepkt;

		while ((uepkt != NULL) && (next_psn == uepkt->psn)) {
			if (!uepkt->processed) {
				service->process_fn(ep, &uepkt->hdr, uepkt->payload);
			}

			OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet %08lu delivered.\n", next_psn);

			psn = next_psn & MAX_PSN;
			if (!(psn & service->preemptive_ack_rate_mask) && psn) {
				fi_opx_hfi1_rx_reliability_send_pre_acks(ep, service->lid, service->subctxt_rx,
									 psn - service->preemptive_ack_rate +
										 1,		       /* psn_start */
									 service->preemptive_ack_rate, /* psn_count */
									 hdr, slid, hfi1_type, ctx_sharing);
			}

			++next_psn;

			struct fi_opx_reliability_rx_uepkt *next = uepkt->next;
			if (next == uepkt) {
				/* only one element in the list */
				assert(uepkt->prev == uepkt);
				next = NULL;
			}

			uepkt->prev->next = uepkt->next;
			uepkt->next->prev = uepkt->prev;
			OPX_BUF_FREE(uepkt);
			uepkt = next;
		}

		flow->uepkt = uepkt;

		flow->next_psn = next_psn;

		return;
	}
	FI_OPX_DEBUG_COUNTERS_INC(
		container_of(ep, struct fi_opx_ep, ep_fid)->debug_counters.reliability_ping.uepkt_received);

	/*
	 * Scale the received PSN up into the same window as the expected PSN.
	 * If the PSN is very close to the bottom of the window but the expected
	 * PSN is not, assume the received PSN rolled over and needs to be
	 * moved into the next, higher, window.
	 */
	uint64_t psn_64 =
		((psn + (next_psn & MAX_PSN_MASK)) +
		 (((psn < PSN_LOW_WINDOW) && ((next_psn & MAX_PSN) > PSN_HIGH_WINDOW)) ? PSN_WINDOW_SIZE : 0));

	if (OFI_UNLIKELY((psn_64 < next_psn) || ((psn_64 - next_psn) > PSN_AGE_LIMIT))) {
		/*
		 * old packet or REALLY old packet.. drop it
		 */
		OPX_RELIABILITY_DEBUG_LOG(flow->key,
					  "(rx) packet dropping%s duplicate packet. psn_24 = %u, psn_64 = %" PRIx64
					  ", next_psn = %" PRIx64 "\n",
					  (psn_64 < next_psn) ? "" : " really old", psn, psn_64, next_psn);
		/*
		 * Send a preemptive ACK here for the packet, since we've already received it.
		 * NOTE: We'll do this regardless of what preemptive ack rate is set to for the normal flow.
		 */
		ssize_t rc __attribute__((unused));
		rc = fi_opx_hfi1_tx_reliability_inject(ep, flow->key, flow->key->slid, src_origin_rx,
						       psn, /* psn_start */
						       1,   /* psn_count */
						       FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK, hfi1_type, ctx_sharing);
		INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_ACKS_SENT, *(flow->key), psn, 1);

		return;
	}

	/*
	 * Else This packet's PSN must be > next_psn (if they were equal, we would not be in the
	 * higher-level else leg for unexpected packet received)
	 *
	 * If applicable, we'll also send preemptive NACKs for the range of packet PSNs we were expecting.
	 * NOTE: We'll do this regardless of what preemptive ack rate is set to for the normal flow.
	 */
	ssize_t rc __attribute__((unused));

	if (flow->uepkt == NULL) {
		/*
		 * add the out-of-order packet to the empty unexpected queue
		 */

		size_t payload_bytes_to_copy = opx_hfi1_packet_hdr_payload_bytes(hdr, hfi1_type);
		struct fi_opx_reliability_rx_uepkt *uepkt =
			fi_opx_reliability_allocate_uepkt(service, hdr, payload, payload_bytes_to_copy, opcode, ep);

		uepkt->prev = uepkt;
		uepkt->next = uepkt;
		uepkt->psn  = psn_64;

		flow->uepkt = uepkt;

#ifdef OPX_RELIABILITY_ENABLE_PRE_NACK
		uint64_t nack_count = MIN(psn_64 - next_psn, OPX_RELIABILITY_RX_MAX_PRE_NACK);
		rc = fi_opx_hfi1_tx_reliability_inject(ep, flow->key, flow->key->slid, src_origin_rx, next_psn,
						       nack_count, FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK, hfi1_type,
						       ctx_sharing, ctx_sharing);
		INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_NACKS_SENT, *(flow->key), next_psn, nack_count);
#endif

		OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u queued (first).\n", psn);
		;
		return;
	}

	if (OFI_UNLIKELY(psn_64 < flow->uepkt->psn)) {
		/*
		 * Hopefully rare situation where this packet is unexpected but
		 * falls into the gap between next_psn and the head of the
		 * unexpected queue. Make this packet into the new head.
		 *
		 * No need to NACK in this case, as we would have already
		 * NACK'd all the PSNs between our expected PSN and this
		 * one when we received the first out of order packet.
		 *
		 */

		size_t payload_bytes_to_copy = opx_hfi1_packet_hdr_payload_bytes(hdr, hfi1_type);
		struct fi_opx_reliability_rx_uepkt *tmp =
			fi_opx_reliability_allocate_uepkt(service, hdr, payload, payload_bytes_to_copy, opcode, ep);

		tmp->psn = psn_64;

		struct fi_opx_reliability_rx_uepkt *head = flow->uepkt;
		struct fi_opx_reliability_rx_uepkt *tail = head->prev;
		tmp->prev				 = tail;
		tmp->next				 = head;
		head->prev				 = tmp;
		tail->next				 = tmp;
		flow->uepkt				 = tmp;

		OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u queued as new head of uepkt list.\n", psn);
		return;
	}

	struct fi_opx_reliability_rx_uepkt *head = flow->uepkt;
	struct fi_opx_reliability_rx_uepkt *tail = head->prev;

	if (psn_64 > tail->psn) {
		/*
		 * This packet's PSN is > than all other PSNs currently
		 * in the unexpected queue. Make it the new tail, and
		 * NACK the packet PSNs between tail and this packet
		 * if there's a gap.
		 */

#ifdef OPX_RELIABILITY_ENABLE_PRE_NACK
		uint64_t nack_start_psn = tail->psn + 1;
		uint64_t nack_count	= MIN(psn_64 - nack_start_psn, OPX_RELIABILITY_RX_MAX_PRE_NACK);

		if (nack_count) {
			rc = fi_opx_hfi1_tx_reliability_inject(
				ep, flow->key, flow->key->slid, src_origin_rx, nack_start_psn, nack_count,
				FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK, hfi1_type, ctx_sharing, ctx_sharing);

			INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_NACKS_SENT, *(flow->key), next_psn, nack_count);
		}
#endif
		size_t payload_bytes_to_copy = opx_hfi1_packet_hdr_payload_bytes(hdr, hfi1_type);
		struct fi_opx_reliability_rx_uepkt *tmp =
			fi_opx_reliability_allocate_uepkt(service, hdr, payload, payload_bytes_to_copy, opcode, ep);

		tmp->prev  = tail;
		tmp->next  = head;
		tmp->psn   = psn_64;
		tail->next = tmp;
		head->prev = tmp;

		OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u queued (new tail).\n", psn);

		return;
	} else if (psn_64 == tail->psn) {
		/* drop this duplicate */
		OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u dropped (unexpected duplicate).\n", psn);
		return;
	}

	/*
	 * insert the out-of-order packet into the unexpected queue;
	 * check for duplicates
	 *
	 * generally if one packet is received out-of-order with psn 'N'
	 * then the next packet received out-of-order will be psn 'N+1'.
	 *
	 * search the unexpected queue in reverse to find the insert
	 * point for this packet.
	 *
	 * No need to NACK in this case, as we would have already
	 * NACK'd all the applicable PSNs when inserting the unexpected
	 * packets on either side of this packet's insertion point.
	 */
	struct fi_opx_reliability_rx_uepkt *uepkt = tail->prev;

	do {
		const uint64_t uepkt_psn = uepkt->psn;

		if (uepkt_psn < psn_64) {
			/* insert after this element */
			size_t payload_bytes_to_copy		= opx_hfi1_packet_hdr_payload_bytes(hdr, hfi1_type);
			struct fi_opx_reliability_rx_uepkt *tmp = fi_opx_reliability_allocate_uepkt(
				service, hdr, payload, payload_bytes_to_copy, opcode, ep);

			tmp->prev = uepkt;
			tmp->next = uepkt->next;
			tmp->psn  = psn_64;

			uepkt->next->prev = tmp;
			uepkt->next	  = tmp;

			OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u queued.\n", psn);
			return;

		} else if (uepkt_psn == psn_64) {
			/* drop this duplicate */
			OPX_RELIABILITY_DEBUG_LOG(flow->key, "(rx) packet psn=%08u dropped (unexpected duplicate).\n",
						  psn);
			return;
		}

		/* move backward */
		uepkt = uepkt->prev;

	} while (uepkt != tail);

	/* We should never get here. The unexpected list was not
	   empty, this packet was not a duplicate, and we somehow
	   determined that the packet did not belong at the beginning,
	   end or middle of the existing list of unexpected packets.
	   We should have hit one of those cases.  */
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"Could not find suitable place for unexpected packet enqueue %016lx %08x %08u head->psn=%08lu, tail->psn=%08lu\n",
		flow->key->qw_prefix, flow->key->dw_suffix, psn, head->psn, tail->psn);
	assert(0);
}

/*
 * The following function will iterate thru the unsorted hashmap of
 * coalesced PING requests.  An ACK/NAK will be sent as a response to
 * the requests processed. We might not make it thru the entire hashmap,
 * so don't deallocate any requests that cannot be sent.
 *
 * This function is capable to handle an incomplete run thru the loop
 *
 * This function is optimized to only do pings, but it can easily be modfied
 * to handle all reliablity events.  If you see lots of duplicate ACK/NAK,
 * then adding those ops would be a good idea.
 */

// TODO: Should add some feedback from the amount of PIO send credits available
//       Each op processed takes one credit to send

void fi_opx_hfi_rx_reliablity_process_requests(struct fid_ep *ep, int max_to_send)
{
	struct fi_opx_ep			*opx_ep	 = container_of(ep, struct fi_opx_ep, ep_fid);
	struct fi_opx_reliability_service	*service = opx_ep->reli_service;
	struct fi_opx_pending_rx_reliability_op *cur_op, *tmp_op = NULL;
	int					 pending_op_count = 0;

	// FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=========Doing HASH_ITER\n");

	// Iterate thru the unsorted hash list and do all ops in it
	HASH_ITER(hh, service->pending_rx_reliability_ops_hashmap, cur_op, tmp_op)
	{
		assert(cur_op->key.flow_key.qw_prefix);
		assert(cur_op->ud_opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING); // No other opcodes supported

		// Detect if we Coalesced any packets since responding to the first ping, then respond to them here
		if (cur_op->psn_count < cur_op->psn_count_coalesce) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "Processing Rx Ping, psn=%lu count=%lu key=%u\n", cur_op->key.psn_start,
				     cur_op->psn_count, cur_op->key.flow_key.dw_suffix);

			fi_opx_hfi1_rx_reliability_ping(ep, service, &cur_op->key.flow_key, cur_op->psn_count_coalesce,
							cur_op->key.psn_start, cur_op->slid, cur_op->subctxt_rx);
		}

		HASH_DEL(service->pending_rx_reliability_ops_hashmap, cur_op);
		OPX_BUF_FREE(cur_op);
		pending_op_count++;

		if (OFI_UNLIKELY(pending_op_count >= max_to_send)) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "WARNING: Should not break here pending_op_count=%i\n", pending_op_count);
			break;
		}
	}

	// FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=========Processed %d requests\n", pending_op_count);

	return;
}

#ifdef OPX_DAOS

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_reliability_inject_shm(struct fid_ep *ep, union fi_opx_reliability_service_flow_key *key,
					      const opx_lid_t dlid, const uint64_t u8_reliability_rx,
					      const uint8_t hfi1_unit, const uint64_t u32_reliability_rx,
					      const uint64_t opcode)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	ssize_t		  rc;

#ifdef OPX_RELIABILITY_DEBUG
	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
		fprintf(stderr, "(tx) SHM - Client flow__ %016lx %08x 0x%x inj resynch\n", key->qw_prefix,
			key->dw_suffix, (unsigned) u32_reliability_rx);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
		fprintf(stderr, "(rx) SHM - Server flow__ %016lx %08x 0x%x inj resynch ack\n", key->qw_prefix,
			key->dw_suffix, (unsigned) u32_reliability_rx);
	} else {
		fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
		abort();
	}
#endif

	/* Make sure the connection to remote EP exists. */
	rc = fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, (unsigned) u32_reliability_rx, hfi1_unit);
	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	/*
	 * Construct and send packet to send to remote EP
	 */
	uint64_t pos;
	/* HFI Rank Support:  Rank already set in reliability_rx
	 * The rank_inst field has been depricated and will be phased out.
	 * The value is always zero.
	 */
	union opx_hfi1_packet_hdr *const hdr =
		opx_shm_tx_next(&opx_ep->tx->shm, hfi1_unit, u8_reliability_rx, &pos, true, u32_reliability_rx, 0, &rc);

	if (!hdr) {
		return rc;
	}

	const uint64_t lrh_dlid_9B = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(dlid);
	const uint64_t bth_rx	   = u8_reliability_rx << OPX_BTH_SUBCTXT_RX_SHIFT;

	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		struct fi_opx_hfi1_txe_scb_9B model = opx_ep->reliability->service.ping_model_9B;
		model.hdr.ud.opcode		    = opcode;
		hdr->qw_9B[0]			    = model.hdr.qw_9B[0] | lrh_dlid_9B;
		hdr->qw_9B[1]			    = model.hdr.qw_9B[1] | bth_rx;
		hdr->qw_9B[2]			    = model.hdr.qw_9B[2];
		hdr->qw_9B[3]			    = model.hdr.qw_9B[3];
		hdr->qw_9B[4]			    = model.hdr.qw_9B[4];
		hdr->qw_9B[5]			    = model.hdr.qw_9B[5];
		// hdr->qw[6]

		hdr->service.key = key->qw_prefix;
	} else {
		struct fi_opx_hfi1_txe_scb_16B model = opx_ep->reliability->service.ping_model_16B;
		model.hdr.ud.opcode		     = opcode;

		hdr->qw_16B[0] = model.hdr.qw_16B[0] |
				 ((uint64_t) (dlid & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B);
		hdr->qw_16B[1] =
			model.hdr.qw_16B[1] |
			((uint64_t) (dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B) |
			(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
		hdr->qw_16B[2]	 = model.hdr.qw_16B[2] | bth_rx;
		hdr->qw_16B[3]	 = model.hdr.qw_16B[3];
		hdr->qw_16B[4]	 = model.hdr.qw_16B[4];
		hdr->qw_16B[5]	 = model.hdr.qw_16B[5];
		hdr->qw_16B[6]	 = model.hdr.qw_16B[6];
		hdr->service.key = key->qw_prefix; /* qw[7] */
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
struct fi_opx_reliability_resynch_flow *
fi_opx_reliability_resynch_flow_init(struct fi_opx_reliability_client_state    *state,
				     union fi_opx_reliability_service_flow_key *rx_key, bool client)
{
	struct fi_opx_reliability_resynch_flow *resynch_flow = NULL;
	void				       *itr, *rx_key_ptr;

	itr = fi_opx_rbt_find(state->flow_rbtree_resynch, (void *) rx_key);
	if (itr) {
		fi_opx_rbt_key_value(state->flow_rbtree_resynch, itr, &rx_key_ptr, (void **) &resynch_flow);
		resynch_flow->resynch_counter++;

		if (client) {
			resynch_flow->remote_ep_resynch_completed = 0;

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(rx) Client flow__ %016lx %08x is waiting for resynch ack again: %ld.\n",
				     rx_key->qw_prefix, rx_key->dw_suffix, resynch_flow->resynch_counter);
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(rx) Server flow__ %016lx %08x is resynched again: %ld.\n", rx_key->qw_prefix,
				     rx_key->dw_suffix, resynch_flow->resynch_counter);
		}
	} else {
		int rc __attribute__((unused));
		rc = posix_memalign((void **) &resynch_flow, 32, sizeof(*resynch_flow));
		assert(rc == 0);

		resynch_flow->key	      = *rx_key;
		resynch_flow->resynch_counter = 1;

		if (client) {
			resynch_flow->client_ep			  = true;
			resynch_flow->remote_ep_resynch_completed = 0;

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(rx) Client flow__ %016lx %08x is waiting for resynch ack: %ld.\n",
				     rx_key->qw_prefix, rx_key->dw_suffix, resynch_flow->resynch_counter);
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(rx) Server flow__ %016lx %08x is resynched: %ld.\n", rx_key->qw_prefix,
				     rx_key->dw_suffix, resynch_flow->resynch_counter);
		}

		// TODO allocate from flow key pool. rx_key currently resides on the stack and has its address passed
		// in.
		rbtInsert(state->flow_rbtree_resynch, (void *) rx_key, (void *) resynch_flow);
	}

	return resynch_flow;
}

__OPX_FORCE_INLINE__
void fi_opx_reliability_resynch_tx_flow_reset(struct fi_opx_ep *opx_ep, struct fi_opx_reliability_service *service,
					      struct fi_opx_reliability_client_state	*state,
					      union fi_opx_reliability_service_flow_key *key)
{
	void *itr;
	/*
	 * Reset all (tx) related reliability protocol data
	 */

	/* Delete all state related entries from flow list */
	itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void *) key);
	if (itr) {
		/* When the Server does its first transmit, this will cause the Server to */
		/* initiate a handshake with the Client.                                  */
		rbtErase(state->tx_flow_rbtree, itr);

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(tx) Server state flow__ %016lx %08x is reset.\n",
			     key->qw_prefix, key->dw_suffix);

		// TODO: Is it safe to free flow key back to buffer pool at this point?
	}

	/* Delete all service replay related entries from flow list */
	itr = fi_opx_rbt_find(service->tx.tx_flow_outstanding_pkts_rbtree, (void *) tx_key.value);
	if (itr) {
		struct fi_opx_reliability_tx_replay **value_ptr =
			(struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(
				service->tx.tx_flow_outstanding_pkts_rbtree, itr);

		struct fi_opx_reliability_tx_replay *head = *value_ptr;

		if (head) {
			/* Retire all queue elements */
			*value_ptr = NULL;

			struct fi_opx_reliability_tx_replay *next = NULL;
			struct fi_opx_reliability_tx_replay *tmp  = head;

			do {
				OPX_RELIABILITY_DEBUG_LOG(key, "(tx) packet %08u retired.\n",
							  FI_OPX_HFI1_PACKET_PSN(OPX_REPLAY_HDR(tmp)));

				next = tmp->next;

				fi_opx_reliability_service_replay_deallocate(&opx_ep->reliability->state, tmp);
				tmp = next;
			} while (tmp != head);

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(tx) Server service flow__ %016lx %08x is reset.\n", key->qw_prefix,
				     key->dw_suffix);
		}
	}
}

void fi_opx_hfi1_rx_reliability_resynch(struct fid_ep *ep, struct fi_opx_reliability_service *service,
					uint32_t origin_reliability_rx, const union opx_hfi1_packet_hdr *const hdr,
					const bool ctx_sharing)
{
	struct fi_opx_ep		       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	struct fi_opx_reliability_client_state *state  = &opx_ep->reliability->state;

	union fi_opx_reliability_service_flow_key rx_key	      = {.value = hdr->service.key};
	union fi_opx_reliability_service_flow_key tx_key	      = {.slid	 = rx_key.dlid,
									 .src_rx = service->subctxt_rx,
									 .dlid	 = rx_key.slid,
									 .dst_rx = service->subctxt_rx};
	union fi_opx_reliability_service_flow_key tx_local_client_key = {.slid	 = rx_key.dlid,
									 .src_rx = service->subctxt_rx,
									 .dlid	 = rx_key.slid,
									 .dst_rx = origin_reliability_rx};

	/*
	 * INTRA-NODE:
	 * Reset all SHM related reliability protocol data retained by this
	 * Server EP about the remote Client EP.
	 */
	if (opx_lid_is_shm(rx_key.slid)) {
		/* Record completion of the resynch request for the remote Client EP */
		opx_ep->rx->shm.resynch_connection[origin_reliability_rx].completed = true;
		opx_ep->rx->shm.resynch_connection[origin_reliability_rx].counter++;

		OPX_RELIABILITY_DEBUG_LOG(&rx_key, "(rx) SHM - Server 0x%x rcv resynch: %ld\n", origin_reliability_rx,
					  opx_ep->rx->shm.resynch_connection[origin_reliability_rx].counter);

		int hfi_unit = fi_opx_hfi1_get_lid_local_unit(rx_key.slid);
		/*
		 * Close connection to the remote Client EP to cause the Server EP to
		 * re-establish a connection on the next transmit operation issued
		 *  by the Server EP.
		 */
		opx_shm_tx_close(&opx_ep->tx->shm, OPX_SHM_SEGMENT_INDEX(hfi_unit, origin_reliability_rx));

		/* Send ack to notify the remote ep that the resynch was completed */
		fi_opx_hfi1_tx_reliability_inject_shm(
			ep, rx_key.value, tx_key.dlid, hdr->service.origin_reliability_subctxt_rx, (uint8_t) hfi_unit,
			origin_reliability_rx, FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK);

		return;
	}

	/*
	 * INTER-NODE:
	 * Reset all rx/tx related reliability protocol data retained by this
	 * Server EP about the remote Client EP.
	 */
	OPX_RELIABILITY_DEBUG_LOG(&rx_key, "(rx) Server rcv resynch\n");

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Server resynching local:\n");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\t(rx) flow__ %016lx %08x\n", rx_key.qw_prefix,
		     rx_key.dw_suffix);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\t(tx) flow__ %016lx %08x\n", tx_key.qw_prefix,
		     tx_key.dw_suffix);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "\t(tx) Local-Client flow__ %016lx %08x\n",
		     tx_local_client_key.qw_prefix, tx_local_client_key.dw_suffix);

	/*
	 * Reset all (rx) related reliability protocol data
	 */
	struct fi_opx_reliability_rx_flow *flow;
	void				  *itr, *rx_key_ptr;

	itr = fi_opx_rbt_find(service->subctxt_rx_flow_rbtree, (void *) rx_key.value);
	if (itr) {
		fi_opx_rbt_key_value(service->subctxt_rx_flow_rbtree, itr, &rx_key_ptr, (void **) &flow);
		// dump_flow_rx(flow, __LINE__);

		/* Reset next expected inbound packet PSN value */
		flow->next_psn = 0;

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(rx) Server flow__ %016lx %08x is reset.\n",
			     rx_key.qw_prefix, rx_key.dw_suffix);

		/* Delete all uepkt related entries from flow list */
		struct fi_opx_reliability_rx_uepkt *head = flow->uepkt;

		while (head != NULL) {
			OPX_RELIABILITY_DEBUG_LOG(&rx_key, "(rx) packet %08lu deleted.\n", head->psn);

			struct fi_opx_reliability_rx_uepkt *next = head->next;
			if (next == head) {
				/* Last element in the list */
				assert(head->prev == head);
				next = NULL;
			}

			head->prev->next = head->next;
			head->next->prev = head->prev;
			OPX_BUF_FREE(head);
			head = next;
		};

		flow->uepkt = NULL;
	}

	/* Reset all (tx) related reliability protocol data */
	fi_opx_reliability_resynch_tx_flow_reset(opx_ep, service, state, &tx_key);

	/*
	 * When a DAOS Server is configured for multiple Engine instances, each Engine
	 * instance can act as both a server and client EP.  If this is so, then set
	 * the resynch_client_ep flag to indicate that a reset of all (tx) related
	 * flows is necessary, when the Engine instance attempts its first transmit
	 * as a client EP.
	 */
	itr = fi_opx_rbt_find(opx_ep->reliability->state.flow_rbtree_resynch, (void *) &tx_local_client_key);

	if (itr) {
		struct fi_opx_reliability_resynch_flow **value_ptr =
			(struct fi_opx_reliability_resynch_flow **) fi_opx_rbt_value_ptr(
				opx_ep->reliability->state.flow_rbtree_resynch, itr);
		struct fi_opx_reliability_resynch_flow *resynch_flow = *value_ptr;

		resynch_flow->resynch_client_ep = 1;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(tx) Local-Client flow__ %016lx %08x found, initiate reset of local client flows.\n",
			     tx_local_client_key.qw_prefix, tx_local_client_key.dw_suffix);
	}

	/*
	 * Create record of the RESYNCH operation being completed for all (rx) & (tx)
	 * related reliability protocol data.
	 */
	fi_opx_reliability_resynch_flow_init(state, &tx_key, false);

	/*
	 * Send ack to notify the remote ep that the resynch was completed
	 */
	fi_opx_hfi1_tx_reliability_inject_ud_resynch(ep, &rx_key, tx_key.dlid, origin_reliability_rx,
						     FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK);
}

void fi_opx_hfi1_rx_reliability_ack_resynch(struct fid_ep *ep, struct fi_opx_reliability_service *service,
					    const union opx_hfi1_packet_hdr *const hdr)
{
	struct fi_opx_ep			 *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	union fi_opx_reliability_service_flow_key rx_key = {.qw_prefix = hdr->service.key, .dw_suffix = 0};
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FLOW KEY slid %x/%x dlid %x/%x, key: %016lx %08x\n",
		     rx_key.slid, ntohs(rx_key.slid), rx_key.dlid, ntohs(rx_key.dlid), rx_key.qw_prefix,
		     rx_key.dw_suffix);

	OPX_RELIABILITY_DEBUG_LOG(&rx_key, "(rx) %s Client rcv resynch ack\n",
				  (opx_lid_is_shm(rx_key.dlid)) ? "SHM -" : "");

	/* No complicated linked list or queue mechanism required at this time.
	 * A simple flag to indicate reception of the ack resynch response is
	 * sufficient and quick.
	 */
	struct fi_opx_reliability_client_state *state	     = &opx_ep->reliability->state;
	struct fi_opx_reliability_resynch_flow *resynch_flow = NULL;
	void				       *itr, *rx_key_ptr;
	itr = fi_opx_rbt_find(state->flow_rbtree_resynch, (void *) &rx_key);

	if (itr) {
		fi_opx_rbt_key_value(state->flow_rbtree_resynch, itr, &rx_key_ptr, (void **) &resynch_flow);

		resynch_flow->remote_ep_resynch_completed = 1;
	}
#ifdef OPX_RELIABILITY_DEBUG
	else {
		fprintf(stderr, "Warning, (rx) %s Client flow__ %016lx %08x rcv resynch ack; not found.\n",
			(opx_lid_is_shm(rx_key.dlid)) ? "SHM -" : "", rx_key.qw_prefix, rx_key.dw_suffix);
	}
#endif
}

ssize_t fi_opx_reliability_do_remote_ep_resynch(struct fid_ep *ep, union fi_opx_addr dest_addr, void *context,
						const uint64_t caps)
{
	struct fi_opx_ep *opx_ep      = container_of(ep, struct fi_opx_ep, ep_fid);
	ssize_t		  rc	      = FI_SUCCESS;
	bool		  inject_done = false;
	opx_lid_t	  slid;

	/* Non-inlined functions should just use the runtime HFI1 type check, no optimizations */
	if (OPX_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		slid = (opx_lid_t) __be16_to_cpu24((__be16) opx_ep->tx->send_9B.hdr.lrh_9B.slid);
	} else {
		slid = (opx_lid_t) __le24_to_cpu(opx_ep->tx->send_9B.hdr.lrh_16B.slid20 << 20 |
						 opx_ep->tx->send_9B.hdr.lrh_16B.slid);
	}
	union fi_opx_reliability_service_flow_key tx_key = {
		.slid		= slid,
		.src_subctxt_rx = opx_ep->tx->send_9B.hdr.reliability.origin_subctxt_rx,
		.dlid		= dest_addr.lid,
		.dst_subctxt_rx = dest_addr.hfi1_subctxt_rx,
	};

	if (!opx_ep->reliability || opx_ep->reli_service->kind != OFI_RELIABILITY_KIND_ONLOAD) {
		/* Nothing to do */
		return FI_SUCCESS;
	}

	if (fi_opx_hfi1_tx_is_shm(opx_ep, dest_addr, caps)) {
		/* INTRA-NODE or SHM?  Will we use DAOS in sr-iov */

		/* HFI Rank Support: Retreive extended addressing data
		 */
		struct fi_context2 *opx_context = (struct fi_context2 *) context;

		if (opx_context) {
			if (opx_context->internal[0]) {
				struct fi_opx_extended_addr *opx_addr =
					(struct fi_opx_extended_addr *) opx_context->internal[0];

				opx_ep->daos_info.rank	    = opx_addr->rank;
				opx_ep->daos_info.rank_inst = opx_addr->rank_inst;
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "(tx) SHM - rank:%d, rank_inst:%d, hfi_rank:%d, SLID:0x%x, DLID:0x%x\n",
					     opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst,
					     opx_ep->hfi->daos_info.rank, slid, dest_addr.lid);
			} else {
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "(tx) SHM - Extended address not available\n");
			}
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "(tx) SHM - Extended address not available\n");
		}

		if ((slid == dest_addr.lid) && opx_ep->daos_info.rank == opx_ep->hfi->daos_info.rank &&
		    opx_ep->daos_info.rank_inst == opx_ep->hfi->daos_info.rank_inst) {
			/* Nothing to do */
			return FI_SUCCESS;
		}

#ifdef OPX_DAOS_DEBUG
		fi_opx_dump_daos_av_addr_rank(opx_ep, dest_addr, "SEND");
#endif

		unsigned rx_index =
			(opx_ep->daos_info.hfi_rank_enabled) ?
				opx_shm_daos_rank_index(opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst) :
				dest_addr.hfi1_subctxt_rx;

		/*
		 * Check whether RESYNCH request has been received from the remote EP.
		 * If so, then this is a Server EP amd there is nothing to be done.
		 */
		if (opx_ep->rx->shm.resynch_connection[rx_index].completed) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(rx) SHM - Server already received resynch from Client %016lx %08x 0x%x: %ld\n",
				     tx_key.qw_prefix, tx_key.dw_suffix, rx_index,
				     opx_ep->rx->shm.resynch_connection[rx_index].counter);

			return FI_SUCCESS;
		}

		/*
		 * Check whether a RESYNCH request was already received from the remote EP.
		 * If so, then this is a Server EP; otherwise this is a Client EP.
		 */
		bool  resynch_rcvd = false;
		void *rx_itr	   = fi_opx_rbt_find(opx_ep->reliability->state.flow_rbtree_resynch, (void *) &tx_key);

		if (rx_itr) {
			struct fi_opx_reliability_resynch_flow **value_ptr =
				(struct fi_opx_reliability_resynch_flow **) fi_opx_rbt_value_ptr(
					opx_ep->reliability->state.flow_rbtree_resynch, rx_itr);

			struct fi_opx_reliability_resynch_flow *resynch_flow = *value_ptr;
			if (resynch_flow->client_ep && resynch_flow->remote_ep_resynch_completed) {
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "(rx) SHM - Client has received resynch from Server %016lx %08x: %ld\n",
					     tx_key.qw_prefix, tx_key.dw_suffix, resynch_flow->resynch_counter);
				resynch_rcvd = true;
				;
			} else {
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "(rx) SHM - This is a Server %016lx %08x: %ld\n", tx_key.qw_prefix,
					     tx_key.dw_suffix, resynch_flow->resynch_counter);
			}
		}

		/*
		 * Check whether packets have already been sent to the dest EP.  If not,
		 * then send RESYNCH request to the dest Server EP.  This causes the dest
		 * Server EP to resynch all SHM related data that it maintains associated
		 * with this Client EP.
		 */
		struct opx_shm_info *shm_info = opx_shm_rbt_get_shm_info(&(opx_ep->tx->shm), segment_index);

		if (!resynch_rcvd || !shm_info || !shm_info->fifo_segment || !shm_info->connection.inuse) {
			rc = fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, rx_index, dest_addr.hfi1_unit);
			if (OFI_UNLIKELY(rc)) {
				return -FI_EAGAIN;
			}

			inject_done = true;
			rc	    = fi_opx_hfi1_tx_reliability_inject_shm(ep, &tx_key, dest_addr.lid,
									    dest_addr.hfi1_subctxt_rx, dest_addr.hfi1_unit,
									    rx_index, FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH);
			if (rc) {
				return -FI_EAGAIN;
			}
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "(tx) SHM - No resynch necessary from Client %016lx %08x 0x%x\n", tx_key.qw_prefix,
				     tx_key.dw_suffix, rx_index);
		}
	} else {
		/* INTER-NODE */

		/*
		 * Check whether a RESYNCH request was already received from the remote EP.
		 * If so, then this is a Server EP; otherwise this is a Client EP.
		 */
		void *rx_itr = fi_opx_rbt_find(opx_ep->reliability->state.flow_rbtree_resynch, (void *) &tx_key);

		if (rx_itr) {
			struct fi_opx_reliability_resynch_flow **value_ptr =
				(struct fi_opx_reliability_resynch_flow **) fi_opx_rbt_value_ptr(
					opx_ep->reliability->state.flow_rbtree_resynch, rx_itr);

			struct fi_opx_reliability_resynch_flow *resynch_flow = *value_ptr;
			if (!resynch_flow->client_ep) {
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "(rx) Server already received resynch from Client %016lx %08x: %ld\n",
					     tx_key.qw_prefix, tx_key.dw_suffix, resynch_flow->resynch_counter);
				return FI_SUCCESS;
			} else {
				/*
				 * When a DAOS Server is configured for multiple Engine instances, each
				 * Engine instance can act as both a server and client EP.  If this is so,
				 * then the resynch_client_ep flag will indicate whether a reset of all
				 * (tx) related flows is necessary, when the Engine instance attempts its
				 * first transmit as a client EP.
				 */
				if (resynch_flow->resynch_client_ep) {
					resynch_flow->resynch_client_ep = 0;
					FI_DBG_TRACE(
						fi_opx_global.prov, FI_LOG_EP_DATA,
						"(tx) Local-Client resynch notification received for Local-Client %016lx %08x: %ld\n",
						tx_key.qw_prefix, tx_key.dw_suffix, resynch_flow->resynch_counter);

					/* Reset all (tx) related reliability protocol data */
					fi_opx_reliability_resynch_tx_flow_reset(opx_ep,
										 opx_ep->reliability->state.service,
										 &opx_ep->reliability->state, &tx_key);
				}
			}
		}

		/*
		 * Query (tx) rbTree to determine whether packets have already been sent
		 * to the remote Server EP.  If not, then send RESYNCH request to the Server EP.
		 * This causes the Server EP to reset all necessary rbTree related data that
		 * it maintains associated with this Client EP.
		 */
		void *itr = fi_opx_rbt_find(opx_ep->reliability->state.tx_flow_rbtree, (void *) &tx_key);
		if (!itr) {
			inject_done = true;

			fi_opx_hfi1_tx_reliability_inject_ud_resynch(ep, &tx_key, tx_key.dlid,
								     dest_addr.hfi1_subctxt_rx,
								     FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH);
		}
	}

	/*
	 * If necessary, wait for ACK response to the RESYNCH request.
	 */
	if (inject_done) {
		/* Create entry to monitor reception of ACK response */
		struct fi_opx_reliability_resynch_flow *resynch_flow =
			fi_opx_reliability_resynch_flow_init(&opx_ep->reliability->state, &tx_key, true);

		if (!resynch_flow) {
			return -FI_EAGAIN;
		}

		struct fi_opx_reliability_service *service   = opx_ep->reliability->state.service;
		union fi_opx_timer_state	  *timer     = &service->timer;
		union fi_opx_timer_stamp	  *timestamp = &service->timestamp;
		union fi_opx_timer_stamp	   start;
		uint64_t			   compare = fi_opx_timer_now(&start, timer);
		uint64_t next = fi_opx_timer_next_event_usec(timer, &start, FI_OPX_TIMER_NEXT_EVENT_USEC_DEFAULT);

		while (compare < next) {
			fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME, OPX_HFI1_TYPE,
					  OPX_IS_CTX_SHARING_ENABLED);
			compare = fi_opx_timer_now(timestamp, timer);

			if (resynch_flow->remote_ep_resynch_completed) {
				return FI_SUCCESS;
			}
		}

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "(rx) Client failed to receive ack resynch from Server %016lx %08x.\n", tx_key.qw_prefix,
			     tx_key.dw_suffix);
		rc = -FI_EAGAIN;
	}

	return rc;
}

#endif
