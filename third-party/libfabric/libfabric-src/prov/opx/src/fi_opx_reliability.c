/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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

#include "rdma/opx/fi_opx_reliability.h"
#include "rdma/opx/fi_opx_compiler.h"

#include <pthread.h>
#include <unistd.h>	/* sleep */
#include <inttypes.h>

#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_hfi1_sdma.h"
/* #define SKIP_RELIABILITY_PROTOCOL_RX_IMPL */
/* #define SKIP_RELIABILITY_PROTOCOL_TX_IMPL */

#include <execinfo.h>
#include <limits.h>

#ifdef OPX_PING_DEBUG
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

#define NUM_PING_STATS_COUNT THROTTLED + 1
#define NUM_PING_STATS_DETAILED UEPKT_RECV

static uint64_t ping_counts[NUM_PING_STATS_COUNT] = { };

#define INC_PING_COUNT(name) ++ping_counts[name]

// The length of all the ring buffers.
#define PDCCOUNT 4096

#ifdef OPX_DUMP_PINGS
typedef struct _pdc {
	uint64_t key;
	uint64_t start;
	uint64_t count;
} pdc;

typedef struct _pdc_list {
	unsigned index;
	pdc list[PDCCOUNT];
} pdc_list;

static pdc_list ping_count_pdcs[NUM_PING_STATS_DETAILED] = { };

#define INC_PDC(name, k, s, c)							\
	do {									\
		pdc_list *pdc_l = &ping_count_pdcs[name];			\
		pdc_l->list[pdc_l->index].key = k;				\
		pdc_l->list[pdc_l->index].start = s;				\
		pdc_l->list[pdc_l->index].count = c;				\
		pdc_l->index = (pdc_l->index + 1) % PDCCOUNT;			\
	} while(0)

#define DUMP_PINGS(pid, name)								\
	do {										\
		fprintf(stderr,"(%d) Last " stringize(PDCCOUNT) " " #name "s:\n", pid);	\
		pdc_list *pdc_l = &ping_count_pdcs[name];				\
		for (unsigned i=0; i < PDCCOUNT && pdc_l->list[i].count > 0; i++) {	\
			fprintf(stderr,"(%d) %5u " #name ": %016lx: %016lx-%04lx\n",	\
				pid, i,							\
				pdc_l->list[i].key,					\
				pdc_l->list[i].start,					\
				pdc_l->list[i].count);					\
		}									\
		fprintf(stderr,"------\n");						\
	} while(0)

#define INC_PING_STAT(name, k, s, c)					\
	do {								\
		INC_PING_COUNT((name));					\
		INC_PDC((name), (k), (s), (c));				\
	} while(0)
#else
#define INC_PING_STAT(name, k, s, c)					\
	do {								\
		INC_PING_COUNT((name));					\
	} while(0)

#endif

#define INC_PING_STAT_COND(cond, name, k, s, c)				\
	do {								\
		if (cond) {						\
			INC_PING_STAT((name), (k), (s), (c));		\
		}							\
	} while(0)


#define addquotes(x) #x
#define stringize(x) addquotes(x)

void dump_ping_counts()
{
	// This bit is a work around to try and prevent the ranks from dumping
	// their output at the same time. It will obviously only work when
	// only a few ranks are involved - try playing with the modulo if you
	// insist on using OPX_PING_DEBUG with more than 4 ranks...
	pid_t pid = getpid();
	unsigned long r = (pid % 4) * 3;
	fprintf(stderr,"%d delaying %lu seconds.\n",pid,r);
	sleep(r);

	fprintf(stderr,"(%d) ==== PING COUNT ====\n", pid);
	fprintf(stderr,"(%d) Pings Sent       %lu\n", pid, ping_counts[PINGS_SENT]);
	fprintf(stderr,"(%d) Pings Rcvd       %lu\n", pid, ping_counts[PINGS_RECV]);
	fprintf(stderr,"(%d) Pre Acks Sent    %lu\n", pid, ping_counts[PRE_ACKS_SENT]);
	fprintf(stderr,"(%d) Acks Sent        %lu\n", pid, ping_counts[ACKS_SENT]);
	fprintf(stderr,"(%d) Acks Recv'd      %lu\n", pid, ping_counts[ACKS_RECV]);
	fprintf(stderr,"(%d) Acks Ignored     %lu\n", pid, ping_counts[ACKS_IGNORED]);
	fprintf(stderr,"(%d) Pre-Nacks Sent   %lu\n", pid, ping_counts[PRE_NACKS_SENT]);
	fprintf(stderr,"(%d) Nacks Sent       %lu\n", pid, ping_counts[NACKS_SENT]);
	fprintf(stderr,"(%d) Nacks Recv'd     %lu\n", pid, ping_counts[NACKS_RECV]);
	fprintf(stderr,"(%d) Nacks Ignored    %lu\n", pid, ping_counts[NACKS_IGNORED]);
	fprintf(stderr,"(%d) UEPKT Recv'd     %lu\n", pid, ping_counts[UEPKT_RECV]);
	fprintf(stderr,"(%d) Throttle MaxOut  %lu\n", pid, ping_counts[THROTTLED_MAX_OUTSTANDING]);
	fprintf(stderr,"(%d) Throttle Nacks   %lu\n", pid, ping_counts[THROTTLED_NACKS]);
	fprintf(stderr,"(%d) Throttle Cnt     %lu\n", pid, ping_counts[THROTTLED]);
	fprintf(stderr,"(%d) ==== PING COUNT ====\n", pid);

#ifdef OPX_DUMP_PINGS
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

#define INC_PING_COUNT(name)
#define INC_PING_STAT(name, k, s, c)
#define INC_PING_STAT_COND(cond, name, k, s, c)

#endif

void fi_opx_reliability_inc_throttle_count()
{
	INC_PING_COUNT(THROTTLED);
}

void fi_opx_reliability_inc_throttle_nacks()
{
	INC_PING_COUNT(THROTTLED_NACKS);
}

void fi_opx_reliability_inc_throttle_maxo()
{
	INC_PING_COUNT(THROTTLED_MAX_OUTSTANDING);
}

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
void dump_flow_rx (struct fi_opx_reliability_flow * flow, const int line) {

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

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_reliability_inject_ud_opcode (struct fid_ep *ep,
						const uint64_t key,
						const uint64_t dlid,
						const uint64_t reliability_rx,
						const uint64_t opcode)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < 1)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		if (FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < 1) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			return -FI_EAGAIN;
		}
	}

	const uint64_t lrh_dlid = dlid << 16;
	const uint64_t bth_rx = reliability_rx << 56;
	struct fi_opx_hfi1_txe_scb model = opx_ep->reliability->service.tx.hfi1.ping_model;
	model.hdr.ud.opcode = opcode;

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	scb[0] = model.qw0 | (0x1 << FI_OPX_HFI1_PBC_CR_SHIFT);
	scb[1] = model.hdr.qw[0] | lrh_dlid;
	scb[2] = model.hdr.qw[1] | bth_rx;
	scb[3] = model.hdr.qw[2];
	scb[4] = model.hdr.qw[3];
	scb[5] = 0;
	scb[6] = 0;
	scb[7] = key;

	/* consume one credit for the packet header */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	return FI_SUCCESS;
}

/* This mask should always be set to 1 less than a power of 2,
 * i.e. all lower bits on */
#define OPX_RELIABILITY_HANDSHAKE_INIT_THROTTLE_MASK 0xFFFull

void opx_reliability_handshake_init(struct fid_ep *ep,
				union fi_opx_reliability_service_flow_key key,
				const uint64_t target_reliability_rx)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	void * itr = fi_opx_rbt_find(opx_ep->reliability->service.handshake_init, (void*)key.value);
	if (OFI_UNLIKELY(!itr)) {
		/* Reliability handshake not yet started, initiate it */
		fi_opx_hfi1_tx_reliability_inject_ud_init(ep,
							key.value, key.dlid,
							target_reliability_rx,
							FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT);

		uint64_t value = 1;
		rbtInsert(opx_ep->reliability->service.handshake_init, (void*)key.value, (void*)value);
		return;
	}

	/* Reliability handshake has started, but not yet completed. Only send
	 * an init packet every so often, so as not to flood the receiver. */
	uint64_t *count_ptr = (uint64_t *) fi_opx_rbt_value_ptr(opx_ep->reliability->service.handshake_init, itr);
	if (!((*count_ptr) & OPX_RELIABILITY_HANDSHAKE_INIT_THROTTLE_MASK)) {
		fi_opx_hfi1_tx_reliability_inject_ud_init(ep,
							key.value, key.dlid,
							target_reliability_rx,
							FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT);
	}

	(*count_ptr)++;
}

ssize_t fi_opx_hfi1_tx_reliability_inject_ud_init(struct fid_ep *ep,
						const uint64_t key,
						const uint64_t dlid,
						const uint64_t reliability_rx,
						const uint64_t opcode)
{
	assert(opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT || opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT_ACK);

	ssize_t rc = fi_opx_hfi1_tx_reliability_inject_ud_opcode(ep, key, dlid, reliability_rx, opcode);

	if (OFI_UNLIKELY(rc)) {
#ifdef OPX_RELIABILITY_DEBUG
		if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT) {
			fprintf(stderr, "(tx) flow__ %016lx 0x%lx inj init dropped; no credits\n", key, reliability_rx);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT_ACK) {
			fprintf(stderr, "(rx) flow__ %016lx 0x%lx inj init ack dropped; no credits\n", key, reliability_rx);
		}
#endif
		return -FI_EAGAIN;
	}
	
#ifdef OPX_RELIABILITY_DEBUG
	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT) {
		fprintf(stderr, "(tx) flow__ %016lx 0x%lx inj init\n", key, reliability_rx);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_INIT_ACK) {
		fprintf(stderr, "(rx) flow__ %016lx 0x%lx inj init ack\n", key, reliability_rx);
	}
#endif

	return FI_SUCCESS;
}


ssize_t fi_opx_hfi1_tx_reliability_inject_ud_resynch(struct fid_ep *ep,
						const uint64_t key,
						const uint64_t dlid,
						const uint64_t reliability_rx,
						const uint64_t opcode)
{
	assert(opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH || opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK);

	ssize_t rc = fi_opx_hfi1_tx_reliability_inject_ud_opcode(ep, key, dlid, reliability_rx, opcode);

	if (OFI_UNLIKELY(rc)) {
#ifdef OPX_RELIABILITY_DEBUG
		if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
			fprintf(stderr, "(tx) Client flow__ %016lx 0x%lx inj resynch dropped; no credits\n", key, reliability_rx);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
			fprintf(stderr, "(rx) Server flow__ %016lx 0x%lx inj resynch ack dropped; no credits\n", key, reliability_rx);
		}
#endif
		return -FI_EAGAIN;
	}
	
#ifdef OPX_RELIABILITY_DEBUG
	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
		fprintf(stderr, "(tx) Client flow__ %016lx 0x%lx inj resynch\n", key, reliability_rx);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
		fprintf(stderr, "(rx) Server flow__ %016lx 0x%lx inj resynch ack\n", key, reliability_rx);
	}
#endif

	return FI_SUCCESS;
}


ssize_t fi_opx_hfi1_tx_reliability_inject (struct fid_ep *ep,
		const uint64_t key, const uint64_t dlid, const uint64_t reliability_rx,
		const uint64_t psn_start, const uint64_t psn_count,
		const uint64_t opcode)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	// Prevent sending a packet that contains a PSN rollover.
	const uint64_t psn_start_24 = psn_start & MAX_PSN;
	const uint64_t psn_count_24 = MIN(psn_count, MAX_PSN-psn_start_24 + 1);

	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < 1)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		if (FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state) < 1) {

			/*
			 * no credits available
			 *
			 * DO NOT BLOCK - instead, drop this request and allow
			 * the reliability protocol to time out and retransmit
			 */
#ifdef OPX_RELIABILITY_DEBUG
			if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING) {
				fprintf(stderr, "(tx) flow__ %016lx inj ping dropped; no credits\n", key);
			} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK) {
				fprintf(stderr, "(rx) flow__ %016lx inj ack dropped; no credits\n", key);
			} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK) {
				fprintf(stderr, "(rx) flow__ %016lx inj nack dropped; no credits\n", key);
			} else {
				fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
			}
#endif
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			return -FI_EAGAIN;
		}
	}

#ifdef OPX_RELIABILITY_DEBUG
	const uint64_t psn_stop = psn_start + psn_count - 1;

	if (psn_start > psn_stop) {
		if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING) {
			fprintf(stderr, "%s:%s():%d (%016lx) invalid inject ping; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n", __FILE__, __func__, __LINE__, key, psn_start, psn_count, psn_stop);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK) {
			fprintf(stderr, "%s:%s():%d (%016lx) invalid inject ack; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n", __FILE__, __func__, __LINE__, key, psn_start, psn_count, psn_stop);
		} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK) {
			fprintf(stderr, "%s:%s():%d (%016lx) invalid inject nack; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n", __FILE__, __func__, __LINE__, key, psn_start, psn_count, psn_stop);
		} else {
			fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
		}
		abort();
	}

	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING) {
		fprintf(stderr, "(tx) flow__ %016lx inj ping %08lu..%08lu\n", key, psn_start, psn_stop);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK) {
		fprintf(stderr, "(rx) flow__ %016lx inj ack %08lu..%08lu\n", key, psn_start, psn_stop);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK) {
		fprintf(stderr, "(rx) flow__ %016lx inj nack %08lu..%08lu\n", key, psn_start, psn_stop);
	} else {
		fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
	}
#endif

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	const uint64_t lrh_dlid = dlid << 16;
	const uint64_t bth_rx = reliability_rx << 56;

	const struct fi_opx_hfi1_txe_scb * const model =
			opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING ?
				&opx_ep->reliability->service.tx.hfi1.ping_model :
				( opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK ?
					&opx_ep->reliability->service.tx.hfi1.ack_model :
					&opx_ep->reliability->service.tx.hfi1.nack_model );

	//uint64_t tmp[8];
	//tmp[0] =
		scb[0] = model->qw0 | (0x1 << FI_OPX_HFI1_PBC_CR_SHIFT);
	//tmp[1] =
		scb[1] = model->hdr.qw[0] | lrh_dlid;
	//tmp[2] =
		scb[2] = model->hdr.qw[1] | bth_rx;
	//tmp[3] =
		scb[3] = model->hdr.qw[2];
	//tmp[4] =
		scb[4] = model->hdr.qw[3];
	//tmp[5] =
		scb[5] = psn_count_24;
	//tmp[6] =
		scb[6] = psn_start_24;
	//tmp[7] =
		scb[7] = key;					/* service.key */

	//fi_opx_hfi1_dump_stl_packet_hdr((struct fi_opx_hfi1_stl_packet_hdr *)&tmp[1], __func__, __LINE__);

	/* consume one credit for the packet header */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	return FI_SUCCESS;
}

void fi_opx_hfi1_rx_reliability_send_pre_acks(struct fid_ep *ep, const uint64_t dlid,
					      const uint64_t reliability_rx, const uint64_t psn_start,
					      const uint64_t psn_count,
					      const union fi_opx_hfi1_packet_hdr *const hdr,
					      const uint8_t origin_rx)
{
	const uint64_t slid = hdr->stl.lrh.slid;

	const union fi_opx_reliability_service_flow_key key = {
		.slid = slid, 
		.tx = FI_OPX_HFI1_PACKET_ORIGIN_TX(hdr),
		.dlid = dlid,
		.rx = reliability_rx };

	ssize_t rc __attribute__ ((unused));
	rc = fi_opx_hfi1_tx_reliability_inject(ep, (uint64_t)key.value, slid,
						origin_rx,
						psn_start, psn_count,
						FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK);
	INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_ACKS_SENT, key.value, psn_start, psn_count);
}

/**
 * Send a ping response over a given range of PSNs. If the specified
 * range crosses a PSN rollover boundary, the response will be sent across
 * multiple packets such that no single packet will contain a range that
 * rolls over.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_rx_reliability_ping_response (struct fid_ep *ep,
						const uint64_t key,
						uint64_t psn_start,
						uint64_t psn_stop,
						const uint64_t slid,
						const uint64_t rx,
						const uint64_t opcode)
{
	assert(psn_stop >= psn_start);
	uint64_t psn_count = (psn_stop - psn_start) + 1;
	ssize_t rc;
	do {
		uint64_t psn_start_24 = psn_start & MAX_PSN;
		const uint64_t psn_count_24 = MIN(psn_count, (MAX_PSN - psn_start_24) + 1);

		rc = fi_opx_hfi1_tx_reliability_inject(ep,
				key, slid, rx,
				psn_start_24,
				psn_count_24,
				opcode);
		INC_PING_STAT_COND(rc == FI_SUCCESS,
			opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK ? ACKS_SENT : NACKS_SENT,
			key, ping_start_24, ack_count_24);

		psn_count -= psn_count_24;
		psn_start += psn_count_24;
	} while (psn_count > 0 && rc == FI_SUCCESS);

	return rc;
}

void fi_opx_hfi1_rx_reliability_ping (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const uint64_t key, uint64_t psn_count, uint64_t psn_start,
		const uint64_t slid, const uint64_t rx)
{


#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(rx) flow__ %016lx rcv ping %08lu..%08lu\n", key, psn_start, psn_start + psn_count - 1);
#endif
	void * itr = NULL;
	itr = fi_opx_rbt_find(service->rx.flow, (void*)key);

	assert(itr);

	INC_PING_STAT(PINGS_RECV, key, psn_start, psn_count);

	struct fi_opx_reliability_flow ** value_ptr =
		(struct fi_opx_reliability_flow **) fi_opx_rbt_value_ptr(service->rx.flow, itr);

	struct fi_opx_reliability_flow * flow = *value_ptr;

#ifndef DISABLE_FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK
	/* If our flow exists, but expected PSN is 0, it means we haven't
	   received any packets from this sender. Send a NACK for PSN 0 */
	if (OFI_UNLIKELY(flow->next_psn == 0)) {
		ssize_t rc __attribute__ ((unused));
		rc = fi_opx_hfi1_tx_reliability_inject(ep,
				key, slid, rx,
				0,	/* psn_start */
				1,	/* psn_count */
				FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK);
		INC_PING_STAT_COND(rc == FI_SUCCESS, NACKS_SENT, key, 0, 1);
		return;
	}
#endif
	const uint64_t flow_next_psn = flow->next_psn;
	const uint64_t flow_next_psn_24 = flow_next_psn & MAX_PSN;
	uint64_t ping_start_psn = psn_start;
	uint64_t ping_psn_count = psn_count;

	/* Scale the received PSN into the same window as the expected PSN.
	 *
	 * We know that the sender has a limited number of replays allocated
	 * (OPX_MAX_OUSTANDING_REPLAYS), and thus legitimate pings for PSNs we
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
	 *  !IMPORTANT! We need to avoid sending any ACKs right after we
	 *     send any NACKs. When the sender receives a NACK, they will
	 *     turn on throttling to prevent sending more packets while
	 *     they catch up on replaying. However, as soon as they receive
	 *     an ACK, they will turn the throttling off.
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
		rc = fi_opx_hfi1_rx_reliability_ping_response (ep,
						key, ping_start_psn,
						ack_stop_psn, slid, rx,
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
			uint64_t last_uepkt_psn = first_uepkt_psn;

			while (last_uepkt_psn <= ping_stop_psn &&
				uepkt->psn == (last_uepkt_psn + 1)) {

				++last_uepkt_psn;
				uepkt = uepkt->next;
			}

			rc = fi_opx_hfi1_rx_reliability_ping_response (ep,
							key, first_uepkt_psn,
							last_uepkt_psn, slid, rx,
							FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK);
			if (OFI_UNLIKELY(rc != FI_SUCCESS)) {
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
		uint64_t nack_stop_psn = flow->uepkt == NULL ?
						ping_stop_psn :
						MIN(flow->uepkt->psn, ping_stop_psn);

		if (nack_start_psn <= nack_stop_psn) {

			uint64_t nack_count = MIN((nack_stop_psn - nack_start_psn) + 1, OPX_RELIABILITY_RX_MAX_NACK);
			nack_stop_psn = nack_start_psn + nack_count;

			fi_opx_hfi1_rx_reliability_ping_response (ep,
						key, nack_start_psn,
						nack_start_psn, slid, rx,
						FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK);
		}
	}
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
int last_ack_index;
#endif

__OPX_FORCE_INLINE__
void fi_opx_hfi1_reliability_iov_payload_check(
		struct fi_opx_reliability_tx_replay *replay,
		const uint64_t key,
		const char *error_msg)
{
#ifndef NDEBUG
	if (!replay->use_iov) {
		return;
	}
	uint32_t copy_payload_qws = MIN(8, replay->iov->iov_len >> 3);
	uint64_t *replay_iov_base = (uint64_t *)replay->iov->iov_base;

	for (int i = 0; i < copy_payload_qws; ++i) {
		if (replay_iov_base[i] != replay->orig_payload[i]) {
			fprintf(stderr,
				"(%d) %s:%s():%d (tx) flow %016lX|psn=%d (0x%X)|"
				"iov_base=%p|iov_len=%lu|%s|"
				"orig_payload[%d]=%016lX current[@%p]=%016lX\n",
				getpid(), __FILE__, __func__, __LINE__,
				key,
				FI_OPX_HFI1_PACKET_PSN(&replay->scb.hdr),
				FI_OPX_HFI1_PACKET_PSN(&replay->scb.hdr),
				replay->iov->iov_base, replay->iov->iov_len,
				error_msg,
				i, replay->orig_payload[i],
				&replay_iov_base[i],
				replay_iov_base[i]);
			break;
		}
	}
#endif
}

void fi_opx_hfi1_rx_reliability_ack (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const uint64_t key, const uint64_t psn_count, const uint64_t psn_start)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	//const uint64_t stop_psn = psn_start + psn_count - 1;
	const uint64_t psn_stop = psn_start + psn_count - 1;

	INC_PING_STAT(ACKS_RECV, key, psn_start, psn_count);

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) flow__ %016lx rcv ack  %08lu..%08lu\n", key, psn_start, psn_stop);
#endif

	assert(psn_stop <= MAX_PSN);

	void * itr = NULL;

	/* search for existing unack'd flows */
	itr = fi_opx_rbt_find(service->tx.flow, (void*)key);

	assert(itr);
#ifndef NDEBUG
	if (OFI_UNLIKELY((itr == NULL))) {
		/*
		 * the flow identified by the key is invalid ...?
		 */
		fprintf(stderr, "(%d) %s:%s():%d invalid key (%016lX) psn_start = %lx, psn_count = %lx, stop_psn = %lx\n",
			getpid(), __FILE__, __func__, __LINE__, key, psn_start, psn_count, psn_stop);
		abort();
	}
#endif

	struct fi_opx_reliability_tx_replay ** value_ptr =
		(struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(service->tx.flow, itr);

	struct fi_opx_reliability_tx_replay * head = *value_ptr;

	if (OFI_UNLIKELY(head == NULL)) {

		/*
		 * there are no unack'd elements in the replay queue;
		 * do nothing and return
		 */
		INC_PING_STAT(ACKS_IGNORED, key, psn_start, psn_count);
		return;
	}

	struct fi_opx_reliability_tx_replay * tail = head->prev;

	/*
	 * check for "fast path" - if the head of the replay q is within the
	 * ACK range, and the tail of the q is within the ACK range, and the replay
	 * q doesn't contain a rollover (i.e, the tail's PSN >= the head's PSN)
	 * we can just retire all elements in the queue
	 */
	uint32_t head_psn = FI_OPX_HFI1_PACKET_PSN(&head->scb.hdr);
	uint32_t tail_psn = FI_OPX_HFI1_PACKET_PSN(&tail->scb.hdr);
	if ((head_psn >= psn_start) && (tail_psn <= psn_stop) && (tail_psn >= head_psn)) {

#ifdef OPX_RELIABILITY_DEBUG
		last_ack_index = snprintf(last_ack, LAST_ACK_LEN,
					"(tx) Retiring on the fast path: %"PRIx64", %"PRIx64", %"PRIx64", H: %d, T: %d\n",
					psn_start, psn_count, psn_stop, head_psn, tail_psn);
#endif
		/* retire all queue elements */
		*value_ptr = NULL;

		struct fi_opx_reliability_tx_replay * next = NULL;
		struct fi_opx_reliability_tx_replay * tmp = head;

		/* Clear any throttling. */
		tmp->psn_ptr->psn.throttle = 0;
		tmp->psn_ptr->psn.nack_count = 0;

		do {
#ifdef OPX_RELIABILITY_DEBUG
			if (last_ack_index < LAST_ACK_LEN)
				last_ack_index += snprintf(&last_ack[last_ack_index],
							   LAST_ACK_LEN-last_ack_index,
							   "(tx) packet %016lx %08x retired (fast path).\n",
							   key, FI_OPX_HFI1_PACKET_PSN(&tmp->scb.hdr));
#endif
			next = tmp->next;

			const uint64_t dec = tmp->cc_dec;
			struct fi_opx_completion_counter * cc_ptr = tmp->cc_ptr;
			if(cc_ptr) {
#ifndef NDEBUG
				fi_opx_hfi1_reliability_iov_payload_check(tmp, key,
					"Received ACK for packet where source buffer has changed!");
#endif
				cc_ptr->byte_counter -= dec;
				assert(cc_ptr->byte_counter >= 0);
				if(cc_ptr->byte_counter == 0) {
					cc_ptr->hit_zero(cc_ptr);
				}
			}

			const uint16_t lrh_pktlen_le = ntohs(tmp->scb.hdr.stl.lrh.pktlen);
			const size_t total_bytes = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
			tmp->psn_ptr->psn.bytes_outstanding -= total_bytes;
			assert((int32_t)tmp->psn_ptr->psn.bytes_outstanding >= 0);

			/* If the replay is pinned, then don't free it and just mark it as ACK'd. It is the
			   responsibility of the pinner to free it after it's ACK'd */
			if (!tmp->pinned) {
				fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, tmp);
			} else {
#ifdef OPX_RELIABILITY_DEBUG
				fprintf(stderr, "(tx) packet %016lx %08u ACK'd but pinned, marking as ACK'd and skipping free of replay.\n",
					key, FI_OPX_HFI1_PACKET_PSN(&tmp->scb.hdr));
#endif
				tmp->acked = true;
			}
			tmp = next;

		} while (tmp != head);

		return;
	}

	/*
	 * find the first replay to ack
	 */
#ifdef OPX_RELIABILITY_DEBUG
	last_ack_index = snprintf(last_ack, LAST_ACK_LEN, "(tx) Retiring on the slow path: %lx, %lx, %lx, H: %x, T: %x\n",
				psn_start, psn_count, psn_stop, head_psn, tail_psn);
#endif

	struct fi_opx_reliability_tx_replay * start = head;
	uint32_t start_psn = head_psn;
	while ((start_psn < psn_start) && (start != tail)) {
		start = start->next;
		start_psn = FI_OPX_HFI1_PACKET_PSN(&start->scb.hdr);
	}

	if (OFI_UNLIKELY(start_psn < psn_start)) {

		/*
		 * all elements in replay queue are 'younger' than the
		 * first psn to retire; do nothing and return
		 */
#ifdef OPX_RELIABILITY_DEBUG
		if (last_ack_index < LAST_ACK_LEN)
		last_ack_index+=snprintf(&last_ack[last_ack_index],LAST_ACK_LEN-last_ack_index,
			"(tx) All elements are younger.\n");
#endif
		INC_PING_STAT(ACKS_IGNORED, key, psn_start, psn_count);
		return;
	}

	/*
	 * find the last replay to ack. the replay psn must be contained in the
	 * range [start_psn,psn_stop] and cannot contain a rollover.
	 */

	struct fi_opx_reliability_tx_replay * stop = start;
	uint32_t stop_psn = start_psn;
	uint32_t stop_next_psn = FI_OPX_HFI1_PACKET_PSN(&stop->next->scb.hdr);
	while ((stop->next != head) && (stop_next_psn <= psn_stop) &&
			(stop_next_psn > psn_start)) {
		stop_psn = stop_next_psn;
		stop = stop->next;
		stop_next_psn = FI_OPX_HFI1_PACKET_PSN(&stop->next->scb.hdr);
	}

	if (OFI_UNLIKELY(stop_psn > psn_stop)) {

		/*
		 * all elements in the replay queue are 'older' than the
		 * last psn to retire; do nothing an return
		 */
#ifdef OPX_RELIABILITY_DEBUG
		if (last_ack_index < LAST_ACK_LEN)
		last_ack_index+=snprintf(&last_ack[last_ack_index],LAST_ACK_LEN-last_ack_index,
			"(tx) All elements are older.\n");
#endif
		INC_PING_STAT(ACKS_IGNORED, key, psn_start, psn_count);
		return;
	}


	const struct fi_opx_reliability_tx_replay * const halt = stop->next;

	if (start == head) {
		if (halt == start) {
			*value_ptr = NULL;
			/* Clear any nack throttling. */
			start->psn_ptr->psn.throttle = 0;
			start->psn_ptr->psn.nack_count = 0;
		} else {
			*value_ptr = (struct fi_opx_reliability_tx_replay *)halt;
		}
	}

#ifdef OPX_RELIABILITY_DEBUG
	if (last_ack_index < LAST_ACK_LEN)
		last_ack_index+=snprintf(&last_ack[last_ack_index],LAST_ACK_LEN-last_ack_index,
			"(tx) Start = %x, Stop = %x, Halt = %x\n",
			FI_OPX_HFI1_PACKET_PSN(&start->scb.hdr),
			FI_OPX_HFI1_PACKET_PSN(&stop->scb.hdr),
			FI_OPX_HFI1_PACKET_PSN(&halt->scb.hdr));
#endif

	/* remove the psn range to ack from the queue */
	start->prev->next = stop->next;
	stop->next->prev = start->prev;

	/*
	 * retire all replay packets between start and stop, inclusive
	 */

	struct fi_opx_reliability_tx_replay * tmp = start;
	do {
#ifdef OPX_RELIABILITY_DEBUG
		if (last_ack_index < LAST_ACK_LEN)
			last_ack_index+=snprintf(&last_ack[last_ack_index],LAST_ACK_LEN-last_ack_index,
				"(tx) packet %016lx %08x retired (slow path).\n", key,
				FI_OPX_HFI1_PACKET_PSN(&tmp->scb.hdr));
#endif
		struct fi_opx_reliability_tx_replay * next = tmp->next;

		const uint64_t dec = tmp->cc_dec;
		struct fi_opx_completion_counter * cc_ptr = tmp->cc_ptr;
		if(cc_ptr) {
#ifndef NDEBUG
			fi_opx_hfi1_reliability_iov_payload_check(tmp, key,
				"Received ACK for packet where source buffer has changed!");
#endif
			cc_ptr->byte_counter -= dec;
			assert(cc_ptr->byte_counter >= 0);
			if(cc_ptr->byte_counter == 0) {
				cc_ptr->hit_zero(cc_ptr);
			}
		}

		const uint16_t lrh_pktlen_le = ntohs(tmp->scb.hdr.stl.lrh.pktlen);
		const size_t total_bytes = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
		tmp->psn_ptr->psn.bytes_outstanding -= total_bytes;
		assert((int32_t)tmp->psn_ptr->psn.bytes_outstanding >= 0);

		/* If the replay is pinned, then don't free it and just mark it as ACK'd. It is the
		   responsibility of the pinner to free it after it's ACK'd */
		if (!tmp->pinned) {
			fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, tmp);
		} else {
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(tx) packet %016lx %08u ACK'd but pinned, marking as ACK'd and skipping free of replay.\n",
				key, FI_OPX_HFI1_PACKET_PSN(&tmp->scb.hdr));
#endif
			tmp->acked = true;
		}
		tmp = next;

	} while (tmp != halt);

	halt->psn_ptr->psn.nack_count = 0;
	halt->psn_ptr->psn.throttle = 0;

	assert ((*value_ptr == NULL) || (*value_ptr)->next != NULL);
}

ssize_t fi_opx_reliability_sdma_replay_complete (union fi_opx_reliability_deferred_work *work)
{
	struct fi_opx_reliability_tx_sdma_replay_params *params = &work->sdma_replay;
	struct fi_opx_ep *opx_ep = (struct fi_opx_ep *) params->opx_ep;

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) %016lx SDMA Replay Complete - BEGIN, params->sdma_reqs %s empty\n",
		params->flow_key, slist_empty(&params->sdma_reqs) ? "IS" : "IS NOT");
#endif

	fi_opx_hfi1_sdma_poll_completion(opx_ep);
	struct fi_opx_hfi1_sdma_replay_work_entry *we =
		(struct fi_opx_hfi1_sdma_replay_work_entry *) params->sdma_reqs.head;

	while (we) {
		enum hfi1_sdma_comp_state status = fi_opx_hfi1_sdma_replay_get_status(opx_ep, we);
		if (status != COMPLETE) {
#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) %016lx SDMA Replay Complete - we has status of %d, return -FI_EAGAIN\n", params->flow_key, status);
#endif
			return -FI_EAGAIN;
		}

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) %016lx SDMA Replay Complete - we is complete, unpinning/freeing %d replays...\n",
		params->flow_key, we->num_packets);
#endif
		for (int i = 0; i < we->num_packets; ++i) {
			struct fi_opx_reliability_tx_replay *replay = we->packets[i].replay;

			replay->pinned = false;

			/* If the replay is already marked as ACK'd, then it has
			   already been removed from the replay ring and we need
			   to free it here */
			if (replay->acked) {
#ifdef OPX_RELIABILITY_DEBUG
				fprintf(stderr, "(tx) packet %016lx %08u replay over SDMA complete and ACK'd, freeing replay\n",
					params->flow_key, (uint32_t)replay->scb.hdr.reliability.psn);
#endif
				fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
#ifdef OPX_RELIABILITY_DEBUG
			} else {
				fprintf(stderr, "(tx) packet %016lx %08u replay over SDMA complete, un-pinning replay\n",
					params->flow_key, (uint32_t)replay->scb.hdr.reliability.psn);
#endif
			}
		}

		slist_remove_head(&params->sdma_reqs);
		fi_opx_hfi1_sdma_replay_return_we(opx_ep, we);
		we = (struct fi_opx_hfi1_sdma_replay_work_entry *) params->sdma_reqs.head;
	}

	assert(slist_empty(&params->sdma_reqs));

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) %016lx SDMA Replay Complete - END - return FI_SUCCESS\n", params->flow_key);
#endif
	return FI_SUCCESS;
}

ssize_t fi_opx_reliability_service_do_replay_sdma (struct fid_ep *ep,
						struct fi_opx_reliability_service * service,
						struct fi_opx_reliability_tx_replay *start_replay,
						struct fi_opx_reliability_tx_replay *end_replay,
						uint32_t num_replays)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	union fi_opx_reliability_deferred_work *work = ofi_buf_alloc(service->work_pending_pool);
	assert(work);
	struct fi_opx_reliability_tx_sdma_replay_params *params = &work->sdma_replay;
	params->work_elem.slist_entry.next = NULL;
	params->work_elem.work_fn = fi_opx_reliability_sdma_replay_complete;

	params->opx_ep = opx_ep;
	slist_init(&params->sdma_reqs);

#if defined(OPX_RELIABILITY_DEBUG) || !defined(NDEBUG)
	union fi_opx_reliability_service_flow_key key;
	key.slid = (uint32_t)start_replay->scb.hdr.stl.lrh.slid;
	key.tx = (uint32_t)start_replay->scb.hdr.reliability.origin_tx;
	key.dlid = (uint32_t)start_replay->scb.hdr.stl.lrh.dlid;
	key.rx = (uint32_t)start_replay->scb.hdr.stl.bth.rx;
#endif
	uint32_t replayed = 0;

#ifdef OPX_RELIABILITY_DEBUG
	params->flow_key = key.value;
	uint32_t num_sdma_reqs = 0;
	struct fi_opx_reliability_tx_replay *orig_start_replay = start_replay;
#endif

	while (replayed < num_replays) {

		struct fi_opx_hfi1_sdma_replay_work_entry *sdma_we = fi_opx_hfi1_sdma_replay_get_idle_we(opx_ep);
		if (!sdma_we) {
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(tx) %016lx SDMA Replay: Couldn't allocate SDMA work entry\n", key.value);
#endif
			break;
		}

		fi_opx_hfi1_sdma_poll_completion(opx_ep);

		uint64_t max_packets = MIN(opx_ep->hfi->info.sdma.available_counter, FI_OPX_HFI1_SDMA_MAX_PACKETS);

		if (max_packets == 0) {
			fi_opx_hfi1_sdma_replay_return_we(opx_ep, sdma_we);
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(tx) %016lx SDMA Replay: No available comp index entries\n", key.value);
#endif
			break;
		}

		uint64_t packet_count = 0;

		struct fi_opx_reliability_tx_replay *replay = start_replay;
		do {
			// Skip replaying any replays that are already in progress
			if (replay->pinned) {
				replay = replay->next;
				continue;
			}
			uint64_t payload_size = fi_opx_reliability_replay_get_payload_size(replay);
#ifndef NDEBUG
			fi_opx_hfi1_reliability_iov_payload_check(replay, key.value, "Replaying packet (SDMA) where source buffer has changed!");
#endif
			fi_opx_hfi1_sdma_replay_add_packet(sdma_we, replay, payload_size);/*, replay->use_iov, frag_size);*/
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(tx) packet %016lx %08u size %ld bytes replay injected over SDMA (%ld packet in group)\n",
				key.value, (uint32_t)replay->scb.hdr.reliability.psn, payload_size, packet_count);
#endif
			replay->pinned = true;
			replay = replay->next;
			++packet_count;
			++replayed;
		} while (packet_count < max_packets && replay != end_replay);
#ifdef OPX_RELIABILITY_DEBUG
		if (!packet_count) {
			replay = start_replay;
			fprintf(stderr, "(%d) %s:%s():%d Packet count is zero! Og start replay=%p, Og start psn=%u, Start replay=%p, start psn=%u, end replay=%p, end psn=%u\n",
				getpid(), __FILE__, __func__, __LINE__,
				orig_start_replay, orig_start_replay->scb.hdr.reliability.psn,
				start_replay, start_replay->scb.hdr.reliability.psn,
				end_replay, end_replay->scb.hdr.reliability.psn);
			do {
				fprintf(stderr, "(%d) %s:%s():%d Replay %p PSN %u pinned=%d!\n",
					getpid(), __FILE__, __func__, __LINE__,
					replay, replay->scb.hdr.reliability.psn, replay->pinned ? 1 : 0);
				replay = replay->next;
			} while (replay != end_replay);
		}
#endif
		assert(packet_count);

#ifdef OPX_RELIABILITY_DEBUG
		num_sdma_reqs++;
#endif
		fi_opx_hfi1_sdma_do_sdma_replay(opx_ep, sdma_we);
		if (OFI_UNLIKELY(sdma_we->writev_rc == -1)) {
			for (int i = 0; i < sdma_we->num_packets; ++i) {
				sdma_we->packets[i].replay->pinned = false;
			}
			replayed -= sdma_we->num_packets;
			fi_opx_hfi1_sdma_replay_return_we(opx_ep, sdma_we);
			break;
		}
		slist_insert_tail((struct slist_entry *)sdma_we, &params->sdma_reqs);
		start_replay = replay;
	}

	if (OFI_LIKELY(!slist_empty(&params->sdma_reqs))) {
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) %016lx Sent %d total packets over %d SDMA requests, appending tail to work pending queue\n",
				key.value, replayed, num_sdma_reqs);
#endif
		slist_insert_tail(&work->work_elem.slist_entry, &service->work_pending);
	} else {
		assert(replayed == 0);
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) %016lx Sent %d total packets over %d SDMA requests, but now freeing work item...\n",
				key.value, replayed, num_sdma_reqs);
#endif
		OPX_BUF_FREE(work);
	}

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) %016lx Sent %d total packets over %d SDMA requests\n",
		key.value, replayed, num_sdma_reqs);
#endif
	return replayed;
}

ssize_t fi_opx_reliability_service_do_replay (struct fi_opx_reliability_service * service,
					struct fi_opx_reliability_tx_replay * replay)
{

#if defined(OPX_RELIABILITY_DEBUG) || !defined(NDEBUG)
	union fi_opx_reliability_service_flow_key key;
	key.slid = (uint32_t)replay->scb.hdr.stl.lrh.slid;
	key.tx = (uint32_t)FI_OPX_HFI1_PACKET_ORIGIN_TX(&replay->scb.hdr);
	key.dlid = (uint32_t)replay->scb.hdr.stl.lrh.dlid;
	key.rx = (uint32_t)replay->scb.hdr.stl.bth.rx;
#endif
	/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
	const uint16_t lrh_pktlen_le = ntohs(replay->scb.hdr.stl.lrh.pktlen);
	const size_t total_bytes_to_copy = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
	const size_t payload_bytes_to_copy = total_bytes_to_copy - sizeof(union fi_opx_hfi1_packet_hdr);

	uint16_t payload_credits_needed =
		(payload_bytes_to_copy >> 6) +				/* number of full 64-byte blocks of payload */
		((payload_bytes_to_copy & 0x000000000000003Ful) != 0);	/* number of partial 64-byte blocks of payload */

	union fi_opx_hfi1_pio_state pio_state = *service->tx.hfi1.pio_state;

	FI_OPX_HFI1_UPDATE_CREDITS(pio_state, service->tx.hfi1.pio_credits_addr);

	const uint16_t total_credits_needed = payload_credits_needed + 1;
	uint16_t total_credits_available = FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state);
	if (total_credits_available < total_credits_needed) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, service->tx.hfi1.pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_RELIABILITY_CREDITS(pio_state);
		if (total_credits_available < total_credits_needed) {
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(tx) packet %016lx %08u Couldn't do replay (no credits)\n",
				key.value, (uint32_t)FI_OPX_HFI1_PACKET_PSN(&replay->scb.hdr));
#endif
			service->tx.hfi1.pio_state->qw0 = pio_state.qw0;
			return -FI_EAGAIN;
		}
	}

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) packet %016lx %08u replay injected\n",
		key.value, (uint32_t)FI_OPX_HFI1_PACKET_PSN(&replay->scb.hdr));
#endif

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(service->tx.hfi1.pio_scb_sop_first, pio_state);

	scb[0] = replay->scb.qw0;
	scb[1] = replay->scb.hdr.qw[0];
	scb[2] = replay->scb.hdr.qw[1];
	scb[3] = replay->scb.hdr.qw[2];
	scb[4] = replay->scb.hdr.qw[3];
	scb[5] = replay->scb.hdr.qw[4];
	scb[6] = replay->scb.hdr.qw[5];
	scb[7] = replay->scb.hdr.qw[6];

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR((service->tx.hfi1.pio_credits_addr));

	/* consume one credit for the packet header */
	--total_credits_available;
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned consumed_credits = 1;
#endif

	uint64_t * buf_qws;
	if (replay->use_iov) {
#ifndef NDEBUG
		/* The pointer to the SDMA work entry should only be set when the
		 * SDMA WE bounce buffer is being used and the replay's IOV is
		 * pointing to it.  */
		if (replay->sdma_we) {
			struct fi_opx_hfi1_sdma_work_entry *sdma_we = (struct fi_opx_hfi1_sdma_work_entry *) replay->sdma_we;
			assert(sdma_we->pending_bounce_buf);
			assert(sdma_we->use_bounce_buf);

			/* If the use count of the SDMA WE at the time the replay
			 * was registered does not match the current use count of
			 * the SDMA WE, that means the SDMA WE was reused before
			 * it should have been, and the data we're about to
			 * replay is no longer valid. */
			if (replay->sdma_we_use_count != sdma_we->bounce_buf.use_count) {
				fprintf(stderr, "(%d) %s:%s():%d Replay pointing to potentially corrupt SDMA WE bounce buffer! "
						"replay->sdma_we_use_count=%hhu sdma_we->use_count=%hhu replay->iov=%p sdma_we->bounce_buf.buf=%p\n",
					getpid(), __FILE__, __func__, __LINE__,
					replay->sdma_we_use_count, sdma_we->bounce_buf.use_count,
					replay->iov[0].iov_base, sdma_we->bounce_buf.buf);
				assert(0);
			}
		}
		fi_opx_hfi1_reliability_iov_payload_check(replay, key.value, "Replaying packet (PIO) where source buffer has changed!");
#endif
		buf_qws = replay->iov[0].iov_base;
	} else {
		buf_qws = replay->payload;
	}

	while (payload_credits_needed > 0) {

		volatile uint64_t * scb_payload =
			FI_OPX_HFI1_PIO_SCB_HEAD(service->tx.hfi1.pio_scb_first, pio_state);

		const uint16_t contiguous_scb_until_wrap =
			(uint16_t)(pio_state.credits_total - pio_state.scb_head_index);

		const uint16_t contiguous_credits_available =
			MIN(total_credits_available, contiguous_scb_until_wrap);

		const uint16_t contiguous_full_blocks_to_write =
			MIN(payload_credits_needed, contiguous_credits_available);

		uint16_t i;
		for (i=0; i<contiguous_full_blocks_to_write; ++i) {

			scb_payload[0] = buf_qws[0];
			scb_payload[1] = buf_qws[1];
			scb_payload[2] = buf_qws[2];
			scb_payload[3] = buf_qws[3];
			scb_payload[4] = buf_qws[4];
			scb_payload[5] = buf_qws[5];
			scb_payload[6] = buf_qws[6];
			scb_payload[7] = buf_qws[7];

			scb_payload += 8;
			buf_qws += 8;
		}

		payload_credits_needed -= contiguous_full_blocks_to_write;
		total_credits_available -= contiguous_full_blocks_to_write;
		FI_OPX_HFI1_CONSUME_CREDITS(pio_state, contiguous_full_blocks_to_write);
#ifndef NDEBUG
		consumed_credits += contiguous_full_blocks_to_write;
#endif
	}

#ifndef NDEBUG
	assert(consumed_credits == total_credits_needed);
#endif

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(service->tx.hfi1.pio_credits_addr);

	/* save the updated txe state */
	service->tx.hfi1.pio_state->qw0 = pio_state.qw0;

	return FI_SUCCESS;
}

ssize_t fi_opx_reliability_pio_replay (union fi_opx_reliability_deferred_work *work)
{
	struct fi_opx_reliability_tx_pio_replay_params *params = &work->pio_replay;
	struct fi_opx_ep *opx_ep = (struct fi_opx_ep *) params->opx_ep;

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) flow %016lx Executing deferred PIO Replay (%p) with start_index=%u, num_replay=%u\n",
		params->flow_key, params, params->start_index, params->num_replays);
#endif

	for (int i = params->start_index; i < params->num_replays; ++i) {
		if (params->replays[i]->acked) {
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) packet %016lx %08u replay already ACK'd, skipping deferred replay\n",
			params->flow_key, FI_OPX_HFI1_PACKET_PSN(&params->replays[i]->scb.hdr));
#endif
			fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, params->replays[i]);
			params->replays[i] = NULL;
			continue;
		}

		ssize_t rc = fi_opx_reliability_service_do_replay(&opx_ep->reliability->service, params->replays[i]);
		if (rc == FI_SUCCESS) {
			params->replays[i]->pinned = false;
			params->replays[i] = NULL;
		} else {
			params->start_index = i;
			return -FI_EAGAIN;
		}
	}

	return FI_SUCCESS;
}

void fi_opx_hfi1_rx_reliability_nack (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const uint64_t key, const uint64_t psn_count, const uint64_t psn_start)
{
	assert(psn_count > 0);
	const uint64_t psn_stop = psn_start + psn_count - 1;

	INC_PING_STAT(NACKS_RECV, key, psn_start, psn_count);
	if (psn_start > psn_stop) {
		fprintf(stderr, "%s:%s():%d (%016lx) invalid nack received; psn_start = %lu, psn_count = %lu, psn_stop = %lu\n",
			__FILE__, __func__, __LINE__, key, psn_start, psn_count, psn_stop);
		abort();
	}

	assert(psn_stop <= MAX_PSN);

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) flow__ %016lx rcv nack %08lu..%08lu\n", key, psn_start, psn_stop);
#endif
	void * itr = NULL;

	/* search for existing unack'd flows */
	itr = fi_opx_rbt_find(service->tx.flow, (void*)key);
#ifndef NDEBUG
	if (OFI_UNLIKELY((itr == NULL))) {

		/*
		 * the flow identified by the key is invalid ...?
		 */
		fprintf(stderr, "%s:%s():%d invalid key (%016lx) psn_start = %lx, psn_count = %lx, psn_stop = %lx\n", __FILE__, __func__, __LINE__, key, psn_start, psn_count, psn_stop);
		abort();
	}
#endif

	struct fi_opx_reliability_tx_replay ** value_ptr =
		(struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(service->tx.flow, itr);

	struct fi_opx_reliability_tx_replay * head = *value_ptr;

	if (OFI_UNLIKELY(head == NULL)) {
		/*
		 * there are no unack'd elements in the replay queue;
		 * do nothing and return
		 */
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) flow__ %016lx rcv nack %08lu..%08lu No Unack'd replays in queue, ignoring\n",
			key, psn_start, psn_stop);
#endif
		INC_PING_STAT(NACKS_IGNORED, key, psn_start, psn_count);
		return;
	}

	struct fi_opx_reliability_tx_replay * tail = head->prev;

	/*
	 * find the first replay to retransmit. Skip any pinned (in progress) replays.
	 */

	struct fi_opx_reliability_tx_replay * start = head;
	uint32_t start_psn = FI_OPX_HFI1_PACKET_PSN(&start->scb.hdr);
	while ((start_psn < psn_start || start->pinned) && (start != tail)) {
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) flow__ %016lx rcv nack %lu..%lu Looking for start replay, current start->psn == %u, start->pinned == %d\n",
			key, psn_start, psn_stop,
			FI_OPX_HFI1_PACKET_PSN(&start->scb.hdr),
			start->pinned);
#endif
		start = start->next;
		start_psn = FI_OPX_HFI1_PACKET_PSN(&start->scb.hdr);
	}

	if (OFI_UNLIKELY(start_psn < psn_start || start_psn > psn_stop || start->pinned)) {

		/*
		 * There are no unpinned replays within the nack range.
		 * Do nothing and return.
		 */
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) flow__ %016lx rcv nack %lu..%lu No Unack'd, unpinned replays in range, ignoring (start->psn == %u, start->pinned == %d\n",
			key, psn_start, psn_stop, start_psn, start->pinned);
#endif
		INC_PING_STAT(NACKS_IGNORED, key, psn_start, psn_count);
		return;
	}

	/*
	 * We limit how many replays we do here (OPX_RELIABILITY_TX_MAX_REPLAYS)
	 * to limit running out of credits, although if the # of credits
	 * assigned us by the driver is low, or our packets are large, we will
	 * probably run out of credits before we hit the limit. Running out of
	 * credits here isn't an error but it results in wasted work and a
	 * delay, but given that we're already dropping packets (we wouldn't be
	 * here if we weren't) being a little inefficient in re-sending them
	 * might actually allow the receiver time to catch up.
	 */

	/*
	 * We have at least 1 replay to retransmit, now find the last.
	 */

	uint64_t replay_count = 1;
	struct fi_opx_reliability_tx_replay * stop = start;
	const uint64_t max = (uint64_t) MAX(OPX_RELIABILITY_TX_MAX_REPLAYS,OPX_RELIABILITY_RX_MAX_NACK);
	while ((stop->next != head) &&
		(FI_OPX_HFI1_PACKET_PSN(&stop->next->scb.hdr) <= psn_stop) &&
		(replay_count < max)) {

		// We won't retransmit pinned replays, so don't count those
		if (!stop->pinned) {
			++replay_count;
			++stop->nack_count;
		}
		stop = stop->next;
	}

	const struct fi_opx_reliability_tx_replay * const halt = stop->next;
	struct fi_opx_reliability_tx_replay * replay = start;

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(tx) flow__ %016lx rcv nack %08lu..%08lu Replaying PSNs %08u - %08u\n",
		key, psn_start, psn_stop, start_psn,
		(uint32_t)FI_OPX_HFI1_PACKET_PSN(&stop->scb.hdr));
#endif
	// Turn on throttling for this flow while we catch up on replays
	start->psn_ptr->psn.nack_count = 1;

	union fi_opx_reliability_deferred_work *work = NULL;
	struct fi_opx_reliability_tx_pio_replay_params *params = NULL;
	bool queing_replays = false;

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
				if (fi_opx_reliability_service_do_replay(service, replay) != FI_SUCCESS) {
					queing_replays = true;

					work = ofi_buf_alloc(service->work_pending_pool);
					assert(work);
					params = &work->pio_replay;
					params->work_elem.slist_entry.next = NULL;
					params->work_elem.work_fn = fi_opx_reliability_pio_replay;
					params->opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
					params->start_index = 0;

					/* Add this replay to the queue */
					replay->pinned = true;
					params->replays[0] = replay;
					params->num_replays = 1;
				}
			} else {
				replay->pinned = true;
				params->replays[params->num_replays] = replay;
				++params->num_replays;
			}

			replay = replay->next;
			continue;
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "SDMA replay\n");
		uint32_t sdma_count = 1;
		struct fi_opx_reliability_tx_replay *sdma_start = replay;
		while (replay->next != halt && replay->next->use_sdma) {
			replay = replay->next;
			if (!replay->pinned) {
				++sdma_count;
			}
		}
		fi_opx_reliability_service_do_replay_sdma(ep, service, sdma_start, replay->next, sdma_count);
		replay = replay->next;
	} while (replay != halt);

	if (queing_replays) {
		assert(params->num_replays <= max); /* likely OPX_RELIABILITY_TX_MAX_REPLAYS */
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(tx) flow %016lx Queuing deferred PIO Replay (%p) with num_replays=%u\n",
			params->flow_key, params, params->num_replays);
#endif
		slist_insert_tail(&work->work_elem.slist_entry, &service->work_pending);
	}
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_reliability_send_ping(struct fid_ep *ep,
				struct fi_opx_reliability_service * service,
				RbtIterator itr)
{
	struct fi_opx_reliability_tx_replay ** value_ptr =
		(struct fi_opx_reliability_tx_replay **)fi_opx_rbt_value_ptr(service->tx.flow, itr);

	struct fi_opx_reliability_tx_replay * head = *value_ptr;

	if (OFI_UNLIKELY(head == NULL)) {
		return 0;
	}

	const union fi_opx_reliability_service_flow_key key = {
		.slid = (uint32_t)head->scb.hdr.stl.lrh.slid,
		.tx = (uint32_t)FI_OPX_HFI1_PACKET_ORIGIN_TX(&head->scb.hdr),
		.dlid = (uint32_t)head->scb.hdr.stl.lrh.dlid,
		.rx = (uint32_t)head->scb.hdr.stl.bth.rx,
	};

	const uint64_t dlid = (uint64_t)head->scb.hdr.stl.lrh.dlid;
	const uint64_t rx = (uint64_t)head->target_reliability_rx;

	// psn_start will always be 24-bit max number here
	uint64_t psn_start = FI_OPX_HFI1_PACKET_PSN(&head->scb.hdr);
	uint64_t psn_stop = FI_OPX_HFI1_PACKET_PSN(&head->prev->scb.hdr);

	// if the PSN of the tail is less than the PSN of the head, the
	// PSN has rolled over. In that case, truncate the ping range
	// to avoid rollover confusion. 
	uint64_t psn_count = ((psn_start>psn_stop)?MAX_PSN:psn_stop) - psn_start + 1;

	// Send one ping to cover the entire replay range.
	ssize_t rc = fi_opx_hfi1_tx_reliability_inject(ep,
					key.value, dlid, rx,
					psn_start,
					psn_count,
					FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING);

	INC_PING_STAT_COND(rc == FI_SUCCESS, PINGS_SENT, key.value, psn_start, psn_count);

	return (rc == FI_SUCCESS) ? 0 : key.value;
}

void fi_reliability_service_ping_remote (struct fid_ep *ep,
		struct fi_opx_reliability_service * service)
{

	/* for each flow in the rbtree ... */
	RbtIterator start_key_itr;
	RbtIterator itr;

	uint64_t fail_key = 0;
	uint64_t start_key = service->tx.ping_start_key;
	if (start_key) {
		itr = fi_opx_rbt_find(service->tx.flow, (void*)start_key);
		start_key_itr = itr;
	} else {
		itr = rbtBegin(service->tx.flow);
		start_key_itr = NULL;
	}

	/* Loop until we hit the end of the tree, or we fail on a particular ping */
	while (itr && !fail_key) {

		fail_key = fi_opx_reliability_send_ping(ep, service, itr);

		/* advance to the next dlid */
		itr = rbtNext(service->tx.flow, itr);
	}

	/* We failed on a particular ping. Store the failing key to be the first to try next time, and stop */
	if (fail_key) {
		service->tx.ping_start_key = fail_key;
		return;
	}

	/* We hit the end of the tree. If there was no starting key, we've iterated through the whole tree and we're done. */
	if (!start_key) {
		return;
	}

	/* Wrap back around from the beginning of the tree and iterate until we've hit the starting key */
	itr = rbtBegin(service->tx.flow);

	while (itr && itr != start_key_itr && !fail_key) {

		fail_key = fi_opx_reliability_send_ping(ep, service, itr);

		/* advance to the next dlid */
		itr = rbtNext(service->tx.flow, itr);
	}

	if (fail_key) {
		service->tx.ping_start_key = fail_key;
	} else {
		service->tx.ping_start_key = 0;
	}
}

void fi_opx_reliability_service_process_pending (struct fi_opx_reliability_service * service)
{
	assert(!slist_empty(&service->work_pending));

	union fi_opx_reliability_deferred_work *work =
		(union fi_opx_reliability_deferred_work *) slist_remove_head(&service->work_pending);

	work->work_elem.slist_entry.next = NULL;
	ssize_t rc = work->work_elem.work_fn(work);
	if(rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
	} else {
		slist_insert_head(&work->work_elem.slist_entry, &service->work_pending);
	}
}

#if 0
/* 
 * Prototype code for off-loading the reliability service.
 */
static inline
void fi_opx_reliability_service_poll (struct fid_ep *ep, struct fi_opx_reliability_service * service) {

	/* process incoming tx replay packets */
	struct fi_opx_atomic_fifo * fifo = &service->fifo;

	double elapsed_usec;
	union fi_opx_timer_state * timer = &service->tx.timer;
	union fi_opx_timer_stamp *timestamp = &service->tx.timestamp;

	const double   usec_max = (double)((uint64_t)service->usec_max);
	const unsigned fifo_max = (unsigned) service->fifo_max;
	//const unsigned hfi1_max = (unsigned) service->hfi1_max;

	volatile uint64_t * enabled_ptr = &service->enabled;

	uint64_t spin_count = 0;

	while (*enabled_ptr) {

		elapsed_usec = fi_opx_timer_elapsed_usec(timestamp, timer);
		if (OFI_UNLIKELY(elapsed_usec > usec_max)) {

			fi_reliability_service_ping_remote(ep, service);

			/* reset the timer */
			fi_opx_timer_now(timestamp, timer);
		}

		unsigned count = 0;
		uint64_t data = 0;
		while ((count++ < fifo_max) && (0 == fi_opx_atomic_fifo_consume(fifo, &data))) {

			if (OFI_LIKELY((data & TX_CMD) != 0)) {

				/* process this replay buffer */
				struct fi_opx_reliability_tx_replay * replay =
					(struct fi_opx_reliability_tx_replay *) (data & ~TX_CMD);

				fi_reliability_service_process_command(service, replay);

			} else if (data & RX_CMD) {

				/* process this new rx flow */
				struct fi_opx_reliability_flow * flow =
					(struct fi_opx_reliability_flow *) (data & ~RX_CMD);

				rbtInsert(service->rx.flow, (void*)flow->key.value, (void*)flow);

			} else {
				fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__); abort();
			}

		}

//		count = 0;
//		while ((count++ < hfi1_max) && (0 != fi_opx_reliability_service_poll_hfi1(service)));

		if (service->is_backoff_enabled) {
			__asm__ __volatile__ ( "pause" );
			spin_count++;
			if (spin_count % service->backoff_period == 0) {
				sched_yield();
			}
		}
	}

	return;
}

void fi_opx_reliability_service_cleanup (struct fi_opx_reliability_service * service) {


	/*
	 * the application must not care about any un-acked replay packets;
	 * mark all flows as complete
	 */
    RbtIterator itr = fi_opx_rbt_begin(service->tx.flow);
	while (itr) {

		struct fi_opx_reliability_tx_replay ** value_ptr =
			(struct fi_opx_reliability_tx_replay **)fi_opx_rbt_value_ptr(service->tx.flow, itr);

		struct fi_opx_reliability_tx_replay * head = *value_ptr;

		if (OFI_LIKELY(head != NULL)) {
			struct fi_opx_reliability_tx_replay * tail = head->prev;

			tail->next = NULL;
			do {

				struct fi_opx_reliability_tx_replay * next = head->next;

				const uint64_t dec = head->cc_dec;
				struct fi_opx_completion_counter * cc_ptr = head->cc_ptr;
				if(cc_ptr) {
						cc_ptr->byte_counter -= dec;
						assert(cc_ptr->byte_counter >= 0);
						if(cc_ptr->byte_counter == 0) {
								cc_ptr->hit_zero(cc_ptr);
						}
				}

				head->next = NULL;
				head->prev = NULL;
				head->active = 0;

				head = next;

			} while (head != NULL);

			*value_ptr = NULL;
		}

		/* advance to the next dlid */
		itr = rbtNext(service->tx.flow, itr);
	}

	/*
	 * process, and respond to, any incoming packets from remote
	 * reliability services until no packets are received
	 */

	union fi_opx_timer_stamp *timestamp = &service->tx.timestamp;
	union fi_opx_timer_state * timer = &service->tx.timer;

	unsigned n = 0;
	while (fi_opx_timer_elapsed_usec(timestamp, timer) < 10000.0) {

//		n = fi_opx_reliability_service_poll_hfi1(service);
		if (n > 0) {
			/* reset the timer */
			fi_opx_timer_now(timestamp, timer);
		}
	}
}

void * pthread_start_routine (void * arg) {


	struct fi_opx_reliability_service * service =
		(struct fi_opx_reliability_service *)arg;

	service->active = 1;
	while (service->enabled > 0) {
		fi_opx_reliability_service_poll(service);
	}
	fi_opx_reliability_service_cleanup(service);
	service->active = 0;

	return NULL;
}
#endif

uint8_t fi_opx_reliability_service_init (struct fi_opx_reliability_service * service,
		uuid_t unique_job_key,
		struct fi_opx_hfi1_context * hfi1,
		const enum ofi_reliability_kind reliability_kind)
{
	uint8_t origin_reliability_rx = (uint8_t)-1;

	if (OFI_RELIABILITY_KIND_OFFLOAD == reliability_kind) {

		assert (hfi1 == NULL);

		service->reliability_kind = reliability_kind;

		service->context = fi_opx_hfi1_context_open(NULL, unique_job_key);

		assert (service->context != NULL);

		hfi1 = service->context;
		init_hfi1_rxe_state(hfi1, &service->rx.hfi1.state);

		service->lid_be = (uint32_t)htons(hfi1->lid);

		/*
		 * COPY the rx static information from the hfi context structure.
		 * This is to improve cache layout.
		 */
		service->rx.hfi1.hdrq.rhf_base = hfi1->info.rxe.hdrq.rhf_base;
		service->rx.hfi1.hdrq.head_register = hfi1->info.rxe.hdrq.head_register;
		service->rx.hfi1.egrq.base_addr = hfi1->info.rxe.egrq.base_addr;
		service->rx.hfi1.egrq.elemsz = hfi1->info.rxe.egrq.elemsz;
		service->rx.hfi1.egrq.last_egrbfr_index = 0;
		service->rx.hfi1.egrq.head_register = hfi1->info.rxe.egrq.head_register;


		/* the 'state' fields will change after every tx operation */
		service->tx.hfi1.pio_state = &hfi1->state.pio;

		/* the 'info' fields do not change; the values can be safely copied */
		service->tx.hfi1.pio_scb_sop_first = hfi1->info.pio.scb_sop_first;
		service->tx.hfi1.pio_scb_first = hfi1->info.pio.scb_first;
		service->tx.hfi1.pio_credits_addr = hfi1->info.pio.credits_addr;

		origin_reliability_rx = hfi1->info.rxe.id;

	} else if (OFI_RELIABILITY_KIND_ONLOAD == reliability_kind) {

		assert(hfi1 != NULL);

		service->reliability_kind = reliability_kind;
		service->context = hfi1;

		service->lid_be = (uint32_t)htons(hfi1->lid);
		/*
		 * COPY the rx static information from the hfi context structure.
		 * This is to improve cache layout.
		 */
		service->rx.hfi1.hdrq.rhf_base = hfi1->info.rxe.hdrq.rhf_base;
		service->rx.hfi1.hdrq.head_register = hfi1->info.rxe.hdrq.head_register;
		service->rx.hfi1.egrq.base_addr = hfi1->info.rxe.egrq.base_addr;
		service->rx.hfi1.egrq.elemsz = hfi1->info.rxe.egrq.elemsz;
		service->rx.hfi1.egrq.last_egrbfr_index = 0;
		service->rx.hfi1.egrq.head_register = hfi1->info.rxe.egrq.head_register;

		service->tx.hfi1.pio_state = &hfi1->state.pio;

		/* the 'info' fields do not change; the values can be safely copied */
		service->tx.hfi1.pio_scb_sop_first = hfi1->info.pio.scb_sop_first;
		service->tx.hfi1.pio_scb_first = hfi1->info.pio.scb_first;
		service->tx.hfi1.pio_credits_addr = hfi1->info.pio.credits_addr;

		origin_reliability_rx = hfi1->info.rxe.id;

	} else if (OFI_RELIABILITY_KIND_NONE == reliability_kind) {

		service->lid_be = (uint32_t)-1;
		service->reliability_kind = reliability_kind;
		return origin_reliability_rx;

	} else {

		/* invalid reliability kind */
		fprintf(stderr, "%s:%s():%d invalid reliability kind: %u\n", __FILE__, __func__, __LINE__, reliability_kind);
		abort();
	}

	/* 'ping' pio send model */
	{
		/* PBC */
		const uint64_t pbc_dws =
			2 +	/* pbc */
			2 +	/* lrh */
			3 +	/* bth */
			9;	/* kdeth; from "RcvHdrSize[i].HdrSize" CSR */

		service->tx.hfi1.ping_model.qw0 = (0 | pbc_dws |
			((hfi1->vl & FI_OPX_HFI1_PBC_VL_MASK) << FI_OPX_HFI1_PBC_VL_SHIFT) |
			(((hfi1->sc >> FI_OPX_HFI1_PBC_SC4_SHIFT) & FI_OPX_HFI1_PBC_SC4_MASK) << FI_OPX_HFI1_PBC_DCINFO_SHIFT));

		/* LRH */
		service->tx.hfi1.ping_model.hdr.stl.lrh.flags =
			htons(FI_OPX_HFI1_LRH_BTH |
			((hfi1->sl & FI_OPX_HFI1_LRH_SL_MASK) << FI_OPX_HFI1_LRH_SL_SHIFT) |
			((hfi1->sc & FI_OPX_HFI1_LRH_SC_MASK) << FI_OPX_HFI1_LRH_SC_SHIFT));

		service->tx.hfi1.ping_model.hdr.stl.lrh.dlid = 0;			/* set at runtime */
		service->tx.hfi1.ping_model.hdr.stl.lrh.pktlen = htons(pbc_dws-1);	/* does not include pbc (8 bytes), but does include icrc (4 bytes) */
		service->tx.hfi1.ping_model.hdr.stl.lrh.slid = htons(hfi1->lid);

		/* BTH */
		service->tx.hfi1.ping_model.hdr.stl.bth.opcode = FI_OPX_HFI_BTH_OPCODE_UD;
		service->tx.hfi1.ping_model.hdr.stl.bth.bth_1 = 0;
		service->tx.hfi1.ping_model.hdr.stl.bth.pkey = htons(FI_OPX_HFI1_DEFAULT_P_KEY);
		service->tx.hfi1.ping_model.hdr.stl.bth.ecn = 0;
		service->tx.hfi1.ping_model.hdr.stl.bth.qp = hfi1->bthqp;
		service->tx.hfi1.ping_model.hdr.stl.bth.unused = 0;
		service->tx.hfi1.ping_model.hdr.stl.bth.rx = 0;			/* set at runtime */

		/* KDETH */
		service->tx.hfi1.ping_model.hdr.stl.kdeth.offset_ver_tid = FI_OPX_HFI1_KDETH_VERSION << FI_OPX_HFI1_KDETH_VERSION_SHIFT;
		service->tx.hfi1.ping_model.hdr.stl.kdeth.jkey = hfi1->jkey;
		service->tx.hfi1.ping_model.hdr.stl.kdeth.hcrc = 0;
		service->tx.hfi1.ping_model.hdr.stl.kdeth.unused = 0;

		/* reliability service */
		union fi_opx_hfi1_packet_hdr * hdr =
			(union fi_opx_hfi1_packet_hdr *)&service->tx.hfi1.ping_model.hdr;

		hdr->ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING;

		hdr->service.origin_reliability_rx = hfi1->info.rxe.id;
		hdr->service.range_count = 0;
		hdr->service.unused = 0;
		hdr->service.psn_count = 0;
		hdr->service.psn_start = 0;
		hdr->service.key = 0;
	}

	/* 'ack' pio send model */
	{
		service->tx.hfi1.ack_model = service->tx.hfi1.ping_model;
		service->tx.hfi1.ack_model.hdr.ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK;
	}

	/* 'nack' pio send model */
	{
		service->tx.hfi1.nack_model = service->tx.hfi1.ping_model;
		service->tx.hfi1.nack_model.hdr.ud.opcode = FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK;
	}



	fi_opx_timer_init(&service->tx.timer);
	fi_opx_timer_now(&service->tx.timestamp, &service->tx.timer);

	service->tx.flow = rbtNew(fi_opx_reliability_compare);
	service->rx.flow = rbtNew(fi_opx_reliability_compare);
	service->handshake_init = rbtNew(fi_opx_reliability_compare);

	char * env;

	/*
	 * When to yeild() the reliability thread.
	 *
	 * OFFLOAD only
	 */
	env = getenv("FI_OPX_RELIABILITY_SERVICE_BACKOFF_PERIOD");
	service->is_backoff_enabled = 0;
	service->backoff_period = 1;
	if (env) {
		unsigned long period = strtoul(env, NULL, 10);
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_BACKOFF_PERIOD = '%s' (%lu)\n", env, period);
		
		service->is_backoff_enabled = 1;
		service->backoff_period=(uint64_t)period;
	}

	/*
	 * How often to preemptively acknowledge packets.
	 * The default is 64, which indicates send a 
	 * preemptive, non-solicited ACK after 64 packets received.
	 * Must be a power of 2, so that we can create an AND mask
	 * that will quickly tell us whether or not to ack
	 * Setting to 0 disables this feature
	 */
	int preemptive_ack_rate = 64;
	if (fi_param_get_int(fi_opx_global.prov, "reliability_service_pre_ack_rate", &preemptive_ack_rate) == FI_SUCCESS) {
		if (preemptive_ack_rate < 0 || preemptive_ack_rate > 32768) {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE has value %d which is outside the valid range of 0-32,768. Using default rate of 64\n", preemptive_ack_rate);
			preemptive_ack_rate = 64;
		} else if (preemptive_ack_rate & (preemptive_ack_rate - 1)) {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE has value %d which is not a power of 2. Using default rate of 64\n", preemptive_ack_rate);
			preemptive_ack_rate = 64;
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Using environment-specified FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE of %d%s\n", preemptive_ack_rate, preemptive_ack_rate ? "" : " (disabled)");
		}
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_PRE_ACK_RATE not specified, using default value of 64\n");
	}

	/* Subtract 1 from the rate to give us the AND mask we want to use for PSN comparison. */
	service->preemptive_ack_rate = (uint32_t) preemptive_ack_rate;
	service->preemptive_ack_rate_mask = ((uint32_t) preemptive_ack_rate) - 1;

	/*
	 * How often to send ping requests
	 *
	 * OFFLOAD and ONLOAD
	 */
	int usec;
	ssize_t rc = fi_param_get_int(fi_opx_global.prov, "reliability_service_usec_max", &usec);
	if (rc == FI_SUCCESS) {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_USEC_MAX set to %d\n", usec);
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_USEC_MAX not specified, using default value of 500\n");
		usec = 500;
	}
	service->usec_max = usec;

	service->usec_next = fi_opx_timer_next_event_usec(&service->tx.timer, &service->tx.timestamp, service->usec_max);

	int nack_threshold;
	rc = fi_param_get_int(fi_opx_global.prov, "reliability_service_nack_threshold", &nack_threshold);
	if (rc == FI_SUCCESS) {
		if (nack_threshold > 0 && nack_threshold <= 32767) {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_NACK_THRESHOLD set to %d\n", nack_threshold);
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Invalid value %d specified for FI_OPX_RELIABILITY_SERVICE_NACK_THRESHOLD. Valid values are 1-32767. Using default value of 1\n",
				nack_threshold);
			nack_threshold = 1;
		}
	} else {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"FI_OPX_RELIABILITY_SERVICE_NACK_THRESHOLD not specified, using default value of 1\n");
		nack_threshold = 1;
	}
	service->nack_threshold = nack_threshold;

	/*
	 * Maximum number of commands to process from atomic fifo before
	 * stopping to do something else
	 *
	 * OFFLOAD only
	 */
	env = getenv("FI_OPX_RELIABILITY_SERVICE_FIFO_MAX");
	service->fifo_max = 1;
	if (env) {
		unsigned long max = strtoul(env, NULL, 10);
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_RELIABILITY_SERVICE_FIFO_MAX = '%s' (%lu)\n", env, max);
		service->fifo_max = (uint8_t)max;
	}

	/*
	 * Maximum number of packets to process from hfi1 rx fifo before
	 * stopping to do something else
	 *
	 * OFFLOAD only
	 */
	env = getenv("FI_OPX_RELIABILITY_SERVICE_HFI1_MAX");
	service->hfi1_max = 1; // TODO: Make this a define.
	if (env) {
		unsigned long max = strtoul(env, NULL, 10);
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"FI_OPX_RELIABILITY_SERVICE_HFI1_MAX = '%s' (%lu)\n", env, max);
		service->hfi1_max = (uint8_t)max;
	}

	/*
	 * Placement of reliability service thread
	 *
	 * OFFLOAD only
	 */
	int local_ranks = 1;
	int local_rank_id = 0;
	int is_local_rank_mode = 0;

	env = getenv("FI_OPX_RELIABILITY_SERVICE_MPI_LOCALRANK_MODE");

	if (env) {
		char * local_ranks_env = getenv("MPI_LOCALNRANKS");
		char * local_rank_id_env = getenv("MPI_LOCALRANKID");

		if (local_ranks_env && local_rank_id_env) {
			is_local_rank_mode = 1;
			local_ranks = (int)strtoul(local_ranks_env, NULL, 10);
			local_rank_id = (int)strtoul(local_rank_id_env, NULL, 10);
		}
	}

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	env = getenv("FI_OPX_RELIABILITY_SERVICE_CPU");

	if (env) {
		long cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
		cpu_set_t cpu_set;

		CPU_ZERO(&cpu_set);

		unsigned long cpu_id = 0;
		unsigned long cpu_id_range_begin = 0;
		unsigned long cpu_id_range_end = 0;

		char * service_cpu_str_save = NULL;
		char * service_cpu_sub_str_save = NULL;

		char * service_cpu_sub_str = NULL;
		char * service_cpu_sub_str_iter = NULL;

		char * service_cpu_str = strdup(env);
		char * service_cpu_str_iter = strtok_r(service_cpu_str, ",", &service_cpu_str_save);

		while (service_cpu_str_iter != NULL) {
			service_cpu_sub_str = strdup(service_cpu_str_iter);
			service_cpu_sub_str_iter = strtok_r(service_cpu_sub_str, "-", &service_cpu_sub_str_save);

			cpu_id_range_begin = strtoul(service_cpu_sub_str_iter, NULL, 10);
			cpu_id_range_end = cpu_id_range_begin;

			service_cpu_sub_str_iter = strtok_r(NULL, "-", &service_cpu_sub_str_save);

			if (service_cpu_sub_str_iter) {
				cpu_id_range_end = strtoul(service_cpu_sub_str_iter, NULL, 10);
			}

			for (cpu_id = cpu_id_range_begin; cpu_id <= cpu_id_range_end; cpu_id++) {
				CPU_SET(cpu_id, &cpu_set);
			}

			service_cpu_str_iter = strtok_r(NULL, ",", &service_cpu_str_save);
			free(service_cpu_sub_str);
		}

		free(service_cpu_str);
		service_cpu_str = NULL;

		if (is_local_rank_mode) {
			int cpu_num_used_total = CPU_COUNT(&cpu_set);
			int cpu_num_used_max = 1;
			int cpu_num_used = 0;

			if (local_ranks != 0 && local_ranks < cpu_num_used_total) {
				cpu_num_used_max = cpu_num_used_total / local_ranks;
			}

			int cpu_num_used_offset = local_rank_id % cpu_num_used_total;

			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"cpu_num_used_offset = %d, cpu_num_used_max = %d, cpu_num_used_total = %d\n",
				cpu_num_used_offset, cpu_num_used_max, cpu_num_used_total);

			if(cpu_num >= 0){
				for (cpu_id = 0; cpu_id < cpu_num; cpu_id++) {
					if (CPU_ISSET(cpu_id, &cpu_set)) {
						if (cpu_num_used_offset) {
							CPU_CLR(cpu_id, &cpu_set); /* clear head */
							cpu_num_used_offset--;
						} else {
							if (cpu_num_used != cpu_num_used_max) {
								cpu_num_used++; /* leave body */
							} else {
								CPU_CLR(cpu_id, &cpu_set); /* clear tail */
							}
						}
					}
				}
			}
		}

		const int cpu_mask_chunk_bits_size = 64; /* uint64_t bits */
		const int cpu_mask_chunk_hex_size = cpu_mask_chunk_bits_size / 4;

		int cpu_mask_chunk_num  = cpu_num / cpu_mask_chunk_bits_size + (cpu_num % cpu_mask_chunk_bits_size ? 1 : 0);

		uint64_t cpu_mask[cpu_mask_chunk_num];
		memset(cpu_mask, 0, sizeof(cpu_mask));

		int i = 0;
		int j = 0;

		for (i = 0; i < cpu_mask_chunk_num; i++) {
			for (j = 0; j < cpu_mask_chunk_bits_size; j++) {
				cpu_mask[i] |= CPU_ISSET(i * cpu_mask_chunk_bits_size + j, &cpu_set) << j;
			}
		}

		char cpu_mask_str[cpu_mask_chunk_num * (cpu_mask_chunk_hex_size + 1 /* space */) + 1 /* null */];

		for (i = 0; i < cpu_mask_chunk_num; i++) {
			sprintf(cpu_mask_str + i * (cpu_mask_chunk_hex_size + 1), "%016" PRIX64 " ", cpu_mask[i]);
		}

		cpu_mask_str[cpu_mask_chunk_num * cpu_mask_chunk_hex_size] = '\0';

		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"RELIABILITY_SERVICE_CPU: (%s) & (rank_mode = %s) == (%s)\n",
			env, is_local_rank_mode ? "TRUE" : "FALSE", cpu_mask_str);

		size_t cpu_set_size = CPU_ALLOC_SIZE(cpu_num);
		pthread_attr_setaffinity_np(&attr, cpu_set_size, &cpu_set);
	}

	service->enabled = 1;

	if (reliability_kind == OFI_RELIABILITY_KIND_ONLOAD) {
		service->active = 1;

	} else if (reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {

		fprintf(stderr, "%s:%s():%d OFI_RELIABILITY_KIND_OFFLOAD is not supported any more. Perhaps this should instead be implemented as \"FI_PROGRESS_AUTO + OFI_RELIABILITY_KIND_ONLOAD\"?\n", __FILE__, __func__, __LINE__);
		abort();

		//service->active = 0;
		//fi_opx_atomic_fifo_init(&service->fifo, 1024*16);

		//int rc = pthread_create(&service->thread, &attr, pthread_start_routine, (void *)service);
		//if (rc != 0) {
		//	fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__); abort();
		//}
	}

	//PENDING_RX_RELIABLITY
	ofi_bufpool_create(&service->pending_rx_reliability_pool,
					sizeof(struct fi_opx_pending_rx_reliability_op),
					0, UINT_MAX, PENDING_RX_RELIABLITY_COUNT_MAX, 0);

	service->pending_rx_reliability_ops_hashmap = NULL;

	ofi_bufpool_create(&service->uepkt_pool,
					sizeof(struct fi_opx_reliability_rx_uepkt),
					64, UINT_MAX, 1024, 0);

	ofi_bufpool_create(&service->work_pending_pool,
					sizeof(union fi_opx_reliability_deferred_work),
					0, UINT_MAX, 1024, 0);

	slist_init(&service->work_pending);

	return origin_reliability_rx;
}


void fi_opx_reliability_service_fini (struct fi_opx_reliability_service * service) {

	service->enabled = 0;
	// fi_opx_compiler_msync_writes();
	while (service->active != 0) {
		// fi_opx_compiler_msync_reads();
	}

	if (service->reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {
		fi_opx_atomic_fifo_fini(&service->fifo);
	}

	if (service->pending_rx_reliability_pool) {
		ofi_bufpool_destroy(service->pending_rx_reliability_pool);
	}

	if (service->uepkt_pool) {
		ofi_bufpool_destroy(service->uepkt_pool);
	}

	if (service->work_pending_pool) {
		ofi_bufpool_destroy(service->work_pending_pool);
	}

	return;
}


void fi_opx_reliability_client_init (struct fi_opx_reliability_client_state * state,
		struct fi_opx_reliability_service * service,
		const uint8_t rx,
		const uint8_t tx,
		void (*process_fn)(struct fid_ep *ep,
				   const union fi_opx_hfi1_packet_hdr * const hdr,
				   const uint8_t * const payload,
				   const uint8_t origin_reliability_rx))
{

	state->reliability_kind = service->reliability_kind;

	state->service = service;

	if (service->reliability_kind == OFI_RELIABILITY_KIND_NONE)
		return;

	/* ---- rx and tx ----*/
	if (service->reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {
		fi_opx_atomic_fifo_producer_init(&state->fifo, &service->fifo);
	}
	state->lid_be = service->lid_be;


	/* ---- rx only ---- */
	state->process_fn = process_fn;
	state->rx_flow_rbtree = rbtNew(fi_opx_reliability_compare);
	state->rx = rx;


	/* ---- tx only ---- */
	state->tx = tx;
	state->tx_flow_rbtree = rbtNew(fi_opx_reliability_compare);

	/* ---- resynch only ---- */
	state->flow_rbtree_resynch = rbtNew(fi_opx_reliability_compare);


	/*
	 * The replay pools are used for the main send path. The pools have
	 * a fixed size and are not permitted to grow, in the theory that
	 * if a receiver is dropping packets, we should throttle the sender
	 * by returning an EAGAIN until the # of outstanding packets falls.
	 */
	(void)ofi_bufpool_create(&(state->replay_pool), 
		OPX_RELIABILITY_TX_REPLAY_SIZE, // element size
		sizeof(void *), // byte alignment
		FI_OPX_RELIABILITY_TX_REPLAY_BLOCKS, // max # of elements
		FI_OPX_RELIABILITY_TX_REPLAY_BLOCKS, // # of elements to allocate at once
		OFI_BUFPOOL_NO_TRACK); // flags
	(void)ofi_bufpool_create(&(state->replay_iov_pool), 
		OPX_RELIABILITY_TX_REPLAY_IOV_SIZE, // element size
		sizeof(void *), // byte alignment
		FI_OPX_RELIABILITY_TX_REPLAY_IOV_BLOCKS, // max # of elements
		FI_OPX_RELIABILITY_TX_REPLAY_IOV_BLOCKS, // # of elements to allocate at once
		OFI_BUFPOOL_NO_TRACK); // flags
#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr,"%s:%s():%d replay_pool = %p\n", __FILE__, __func__, __LINE__,
		state->replay_pool);
	fprintf(stderr,"%s:%s():%d replay_iov_pool = %p\n", __FILE__, __func__, __LINE__,
		state->replay_iov_pool);
#endif

#ifdef OPX_RELIABILITY_TEST
	/*
 	 * deliberately drop a percentage of packets in order to exercise the
 	 * reliability service.
 	 */
	state->drop_count = 0;
	state->drop_mask = 0x00FF;	/* default: drop every 256'th packet */
	char * env = getenv("FI_OPX_RELIABILITY_SERVICE_DROP_PACKET_MASK");
	if (env) {
		uint16_t mask = (uint16_t)strtoul(env, NULL, 16);
		fprintf(stderr, "%s():%d FI_OPX_RELIABILITY_SERVICE_DROP_PACKET_MASK = '%s' (0x%04hx)\n", __func__, __LINE__, env, mask);
		state->drop_mask = mask;
	}
#endif

	return;
}

void fi_opx_reliability_client_fini (struct fi_opx_reliability_client_state * state)
{

#ifdef OPX_PING_DEBUG
	dump_ping_counts();
#endif

	if (state->reliability_kind == OFI_RELIABILITY_KIND_NONE)
		return;

	if (state->reliability_kind == OFI_RELIABILITY_KIND_OFFLOAD) {
		fi_opx_atomic_fifo_producer_fini(&state->fifo);

		/* wait until all replay buffers are ack'd because the reliability
		 * service maintains pointers to unack'd reply buffers and can't
		 * free until service is finished
		 */
		while (fi_opx_reliability_client_active(state)) {
			//fi_opx_compiler_msync_reads();
		}
	}



	if (state->replay_pool) {
		ofi_bufpool_destroy(state->replay_pool);
		state->replay_pool = NULL;
	}
	if (state->replay_iov_pool) {
		ofi_bufpool_destroy(state->replay_iov_pool);
		state->replay_iov_pool = NULL;
	}


	/* TODO - delete rbtree and flows, but first have to notify
	 * reliability service of the tear-down */
}

__OPX_FORCE_INLINE__
struct fi_opx_reliability_rx_uepkt *fi_opx_reliability_allocate_uepkt(struct fi_opx_reliability_service *service,
								const union fi_opx_hfi1_packet_hdr * const hdr,
								const uint8_t * const payload,
								const size_t payload_bytes_to_copy)
{
	struct fi_opx_reliability_rx_uepkt * tmp = ofi_buf_alloc(service->uepkt_pool);
	assert(tmp);

	memcpy((void*)&tmp->hdr, hdr, sizeof(union fi_opx_hfi1_packet_hdr));

	if (payload && payload_bytes_to_copy > 0)
		memcpy((void*)&tmp->payload[0], (const void *)payload, payload_bytes_to_copy);

	return tmp;
}

void fi_opx_reliability_rx_exception (struct fi_opx_reliability_client_state * state,
		uint64_t slid, uint64_t origin_tx, uint32_t psn,
		struct fid_ep *ep, const union fi_opx_hfi1_packet_hdr * const hdr, const uint8_t * const payload)
{

	/* reported in LRH as the number of 4-byte words in the packet; header + payload + icrc */
	const uint16_t lrh_pktlen_le = ntohs(hdr->stl.lrh.pktlen);
	const size_t total_bytes_to_copy = (lrh_pktlen_le - 1) * 4;	/* do not copy the trailing icrc */
	const size_t payload_bytes_to_copy = total_bytes_to_copy - sizeof(union fi_opx_hfi1_packet_hdr);

	union fi_opx_reliability_service_flow_key key;
	key.slid = slid;
	key.tx = origin_tx;
	key.dlid = state->lid_be;
	key.rx = state->rx;

	void * itr = fi_opx_rbt_find(state->rx_flow_rbtree, (void*)key.value);

	assert(itr);

	struct fi_opx_reliability_flow ** value_ptr =
		(struct fi_opx_reliability_flow **) fi_opx_rbt_value_ptr(state->rx_flow_rbtree, itr);

	struct fi_opx_reliability_flow * flow = *value_ptr;
	uint64_t next_psn = flow->next_psn;
	uint8_t origin_rx = flow->origin_rx;

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

#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) packet %016lx %08u received (process out-of-order).\n", key.value, psn);
#endif
		state->process_fn(ep, hdr, payload, origin_rx);

		if (!(psn & state->service->preemptive_ack_rate_mask) && psn) {
			fi_opx_hfi1_rx_reliability_send_pre_acks(ep,
					state->lid_be,
					state->rx,
					psn - state->service->preemptive_ack_rate + 1, /* psn_start */
					state->service->preemptive_ack_rate, /* psn_count */
					hdr, origin_rx);
		}

		next_psn += 1;

		flow->next_psn = next_psn;

		struct fi_opx_reliability_rx_uepkt * head = flow->uepkt;
		if (head != NULL) {

			//ofi_spin_lock(&flow->lock);

			head = flow->uepkt;	/* check again now that lock is acquired */

			struct fi_opx_reliability_rx_uepkt * uepkt = head;

			while ((uepkt != NULL) && (next_psn == uepkt->psn)) {

				state->process_fn(ep, &uepkt->hdr, uepkt->payload, origin_rx);
#ifdef OPX_RELIABILITY_DEBUG
				fprintf(stderr, "(rx) packet %016lx %08lu delivered.\n", key.value, next_psn);
#endif
				psn = next_psn & MAX_PSN;
				if (!(psn & state->service->preemptive_ack_rate_mask) && psn) {
					fi_opx_hfi1_rx_reliability_send_pre_acks(ep,
							state->lid_be,
							state->rx,
							psn - state->service->preemptive_ack_rate + 1, /* psn_start */
							state->service->preemptive_ack_rate, /* psn_count */
							hdr, origin_rx);
				}

				++next_psn;

				struct fi_opx_reliability_rx_uepkt * next = uepkt->next;
				if (next == uepkt) {
					/* only one element in the list */
					assert(uepkt->prev == uepkt);
					next = NULL;
				}

				uepkt->prev->next = uepkt->next;
				uepkt->next->prev = uepkt->prev;
				OPX_BUF_FREE(uepkt);
				head = next;
				uepkt = next;
			}

			flow->uepkt = head;

			flow->next_psn = next_psn;
			//ofi_spin_unlock(&flow->lock);
		}

		return;
	}

	INC_PING_COUNT(UEPKT_RECV);

	/*
	 * Scale the received PSN up into the same window as the expected PSN.
	 * If the PSN is very close to the bottom of the window but the expected
	 * PSN is not, assume the received PSN rolled over and needs to be
	 * moved into the next, higher, window.
	 */
	uint64_t psn_64 = ( (psn + (next_psn & MAX_PSN_MASK)) +
		(((psn < PSN_LOW_WINDOW) &&
		  ((next_psn & MAX_PSN) > PSN_HIGH_WINDOW))?PSN_WINDOW_SIZE:0) );

	if (OFI_UNLIKELY((psn_64 < next_psn) || ((psn_64 - next_psn) > PSN_AGE_LIMIT))) {
		/*
		 * old packet or REALLY old packet.. drop it
		 */
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) packet %"PRIx64" Dropping%s duplicate packet. psn_24 = %u, psn_64 = %"PRIx64", next_psn = %"PRIx64"\n",
			key.value, (psn_64 < next_psn) ? "" : " really old", psn, psn_64, next_psn);
#endif
		/*
		 * Send a preemptive ACK here for the packet, since we've already received it.
		 * NOTE: We'll do this regardless of what preemptive ack rate is set to for the normal flow.
		 */
		ssize_t rc __attribute__ ((unused));
		rc = fi_opx_hfi1_tx_reliability_inject(ep, key.value, key.slid,
						origin_rx,
						psn, /* psn_start */
						1, /* psn_count */
						FI_OPX_HFI_UD_OPCODE_RELIABILITY_ACK);
		INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_ACKS_SENT, key.value, psn, 1);

		return;
	}

	/*
	 * Else This packet's PSN must be > next_psn (if they were equal, we would not be in the
	 * higher-level else leg for unexpected packet received)
	 *
	 * If applicable, we'll also send preemptive NACKs for the range of packet PSNs we were expecting.
	 * NOTE: We'll do this regardless of what preemptive ack rate is set to for the normal flow.
	 */
	ssize_t rc __attribute__ ((unused));

	if (flow->uepkt == NULL) {
		/*
		 * add the out-of-order packet to the empty unexpected queue
		 */

		struct fi_opx_reliability_rx_uepkt * uepkt =
			fi_opx_reliability_allocate_uepkt(state->service, hdr, payload, payload_bytes_to_copy);

		uepkt->prev = uepkt;
		uepkt->next = uepkt;
		uepkt->psn = psn_64;

		//ofi_spin_lock(&flow->lock);

		flow->uepkt = uepkt;

		//ofi_spin_unlock(&flow->lock);
#ifndef DISABLE_FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK
		uint64_t nack_count = MIN(psn_64 - next_psn, OPX_RELIABILITY_RX_MAX_NACK);
		rc = fi_opx_hfi1_tx_reliability_inject(ep, key.value, key.slid,
					origin_rx,
					next_psn,
					nack_count,
					FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK);
		INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_NACKS_SENT, key.value, next_psn, nack_count);
#endif
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) packet %016lx %08u queued (first).\n", key.value, psn);
#endif
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
		struct fi_opx_reliability_rx_uepkt * tmp =
			fi_opx_reliability_allocate_uepkt(state->service, hdr, payload, payload_bytes_to_copy);

		tmp->psn = psn_64;

		//ofi_spin_lock(&flow->lock);

		struct fi_opx_reliability_rx_uepkt * head = flow->uepkt;
		struct fi_opx_reliability_rx_uepkt * tail = head->prev;
		tmp->prev = tail; tmp->next = head;
		head->prev = tmp; tail->next = tmp;
		flow->uepkt = tmp;

		//ofi_spin_unlock(&flow->lock);
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) packet %016lx %08u queued as new head of uepkt list.\n",
			key.value, psn);
#endif
		return;
	}

	struct fi_opx_reliability_rx_uepkt * head = flow->uepkt;
	struct fi_opx_reliability_rx_uepkt * tail = head->prev;

	if (psn_64 > tail->psn) {
		/*
		 * This packet's PSN is > than all other PSNs currently
		 * in the unexpected queue. Make it the new tail, and
		 * NACK the packet PSNs between tail and this packet
		 * if there's a gap.
		 */

#ifndef DISABLE_FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK
		 uint64_t nack_start_psn = tail->psn + 1;
		 uint64_t nack_count = MIN(psn_64 - nack_start_psn, OPX_RELIABILITY_RX_MAX_NACK);

		 if (nack_count) {
			rc = fi_opx_hfi1_tx_reliability_inject(ep, key.value, key.slid,
					origin_rx,
					nack_start_psn,
					nack_count,
					FI_OPX_HFI_UD_OPCODE_RELIABILITY_NACK);
			INC_PING_STAT_COND(rc == FI_SUCCESS, PRE_NACKS_SENT, key.value, next_psn, nack_count);
		 }

#endif
		//ofi_spin_lock(&flow->lock);
		struct fi_opx_reliability_rx_uepkt * tmp =
			fi_opx_reliability_allocate_uepkt(state->service, hdr, payload, payload_bytes_to_copy);
		tmp->prev = tail;
		tmp->next = head;
		tmp->psn = psn_64;
		tail->next = tmp;
		head->prev = tmp;
		//ofi_spin_unlock(&flow->lock);
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) packet %016lx %08u queued (new tail).\n", key.value, psn);
#endif
		return;
	} else if (psn_64 == tail->psn) {
		/* drop this duplicate */
#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) packet %016lx %08u dropped (unexpected duplicate).\n", key.value, psn);
#endif
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
	struct fi_opx_reliability_rx_uepkt * uepkt = tail->prev;

	do {
		const uint64_t uepkt_psn = uepkt->psn;

		if (uepkt_psn < psn_64) {

			/* insert after this element */
			struct fi_opx_reliability_rx_uepkt * tmp =
				fi_opx_reliability_allocate_uepkt(state->service, hdr, payload, payload_bytes_to_copy);

			tmp->prev = uepkt;
			tmp->next = uepkt->next;
			tmp->psn = psn_64;

			//ofi_spin_lock(&flow->lock);

			uepkt->next->prev = tmp;
			uepkt->next = tmp;

			//ofi_spin_unlock(&flow->lock);

#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(rx) packet %016lx %08u queued.\n", key.value, psn);
#endif
			return;

		} else if (uepkt_psn == psn_64) {

			/* drop this duplicate */
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(rx) packet %016lx %08u dropped (unexpected duplicate).\n", key.value, psn);
#endif
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
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		 "Could not find suitable place for unexpected packet enqueue %016lx %08u head->psn=%08lu, tail->psn=%08lu\n",
		 key.value, psn, head->psn, tail->psn);
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

	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	struct fi_opx_reliability_service *service = opx_ep->reliability->state.service;
	struct fi_opx_pending_rx_reliability_op *cur_op, *tmp_op = NULL;
	int pending_op_count = 0;

	//FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=========Doing HASH_ITER\n");

	// Iterate thru the unsorted hash list and do all ops in it
	 HASH_ITER(hh, service->pending_rx_reliability_ops_hashmap, cur_op, tmp_op) {

		assert(cur_op->key.key);
		assert(cur_op->ud_opcode  == FI_OPX_HFI_UD_OPCODE_RELIABILITY_PING);  // No other opcodes supported

		// Detect if we Coalesced any packets since responding to the first ping, then respond to them here
		if (cur_op->psn_count < cur_op->psn_count_coalesce) {

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Processing Rx Ping, psn=%lu count=%lu key=%lu\n", 
			cur_op->key.psn_start, cur_op->psn_count, cur_op->key.key);

				fi_opx_hfi1_rx_reliability_ping(ep, service,
					cur_op->key.key, cur_op->psn_count_coalesce, cur_op->key.psn_start,
					cur_op->slid, cur_op->rx);
			}
			
		HASH_DEL(service->pending_rx_reliability_ops_hashmap, cur_op);
		OPX_BUF_FREE(cur_op);
		pending_op_count++;

		if (OFI_UNLIKELY(pending_op_count >= max_to_send)) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "WARNING: Should not break here pending_op_count=%i\n", pending_op_count);
			break;
		}
	}

	//FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=========Processed %d requests\n", pending_op_count);

	return;
}


__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_reliability_inject_shm (struct fid_ep *ep,
		const uint64_t key, const uint64_t dlid, const uint64_t reliability_rx,
		const uint8_t hfi1_unit, const uint64_t opcode)
{
	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	ssize_t rc;

#ifdef OPX_RELIABILITY_DEBUG
	if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH) {
		fprintf(stderr, "(tx) SHM - Client flow__ %016lx 0x%x inj resynch\n",
			key, (unsigned)reliability_rx);
	} else if (opcode == FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK) {
		fprintf(stderr, "(rx) SHM - Server flow__ %016lx 0x%x inj resynch ack\n",
			key, (unsigned)reliability_rx);
	} else {
		fprintf(stderr, "%s:%s():%d bad opcode (%lu) .. abort\n", __FILE__, __func__, __LINE__, opcode);
		abort();
	}
#endif

	/* Make sure the connection to remote EP exists. */
	rc = fi_opx_shm_dynamic_tx_connect(1, opx_ep, (unsigned)reliability_rx, hfi1_unit);
	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	/*
	 * Construct and send packet to send to remote EP
	 */
	uint64_t pos;
	/* HFI Rank Support:  Rank already set in reliability_rx */
	union fi_opx_hfi1_packet_hdr * const hdr =
		opx_shm_tx_next(&opx_ep->tx->shm, reliability_rx, &pos,
			false, opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) return rc;

	const uint64_t lrh_dlid = dlid << 16;
	const uint64_t bth_rx = reliability_rx << 56;

	struct fi_opx_hfi1_txe_scb model = opx_ep->reliability->service.tx.hfi1.ping_model;
	model.hdr.ud.opcode = opcode;

	hdr->qw[0] = model.hdr.qw[0] | lrh_dlid;

	hdr->qw[1] =  model.hdr.qw[1] | bth_rx;

	hdr->qw[2] = model.hdr.qw[2];

	hdr->qw[3] = model.hdr.qw[3]; 

	hdr->qw[4] = model.hdr.qw[4];

	hdr->qw[5] = model.hdr.qw[5];
	// hdr->qw[6]
	hdr->service.key = key;

	opx_shm_tx_advance(&opx_ep->tx->shm, (void*)hdr, pos);

	return FI_SUCCESS;
}


__OPX_FORCE_INLINE__
struct fi_opx_reliability_resynch_flow * fi_opx_reliability_resynch_flow_init (
		struct fi_opx_reliability_client_state * state,
		union fi_opx_reliability_service_flow_key rx_key,
		bool client)
{
	struct fi_opx_reliability_resynch_flow * resynch_flow = NULL;
	void *itr, *rx_key_ptr;

	itr = fi_opx_rbt_find(state->flow_rbtree_resynch, (void*)rx_key.value);
	if (itr) {
		fi_opx_rbt_key_value(state->flow_rbtree_resynch, itr, &rx_key_ptr, (void **)&resynch_flow);
		resynch_flow->resynch_counter++;

		if (client) {
			resynch_flow->remote_ep_resynch_completed = 0;

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(rx) Client flow__ %016lx is waiting for resynch ack again: %ld.\n",
				rx_key.value, resynch_flow->resynch_counter);
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(rx) Server flow__ %016lx is resynched again: %ld.\n",
				rx_key.value, resynch_flow->resynch_counter);
		}
	} else {
		int rc __attribute__ ((unused));
		rc = posix_memalign((void **)&resynch_flow, 32, sizeof(*resynch_flow));
		assert(rc==0);

		resynch_flow->key.value = rx_key.value;
		resynch_flow->resynch_counter = 1;

		if (client) {
			resynch_flow->client_ep = true;
			resynch_flow->remote_ep_resynch_completed = 0;

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(rx) Client flow__ %016lx is waiting for resynch ack: %ld.\n",
				rx_key.value, resynch_flow->resynch_counter);
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(rx) Server flow__ %016lx is resynched: %ld.\n",
				rx_key.value, resynch_flow->resynch_counter);
		}

		rbtInsert(state->flow_rbtree_resynch, (void*)rx_key.value, (void*)resynch_flow);
	}

	return resynch_flow;
}


__OPX_FORCE_INLINE__
void fi_opx_reliability_resynch_tx_flow_reset (struct fi_opx_ep *opx_ep,
		struct fi_opx_reliability_service * service,	
		struct fi_opx_reliability_client_state * state,
		union fi_opx_reliability_service_flow_key tx_key)
{
	void *itr;
	/*
	 * Reset all (tx) related reliability protocol data
	 */

	/* Delete all state related entries from flow list */
	itr = fi_opx_rbt_find(state->tx_flow_rbtree, (void*)tx_key.value);
	if (itr) {
		/* When the Server does its first transmit, this will cause the Server to */
		/* initiate a handshake with the Client.                                  */ 
		rbtErase(state->tx_flow_rbtree, itr);

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"(tx) Server state flow__ %016lx is reset.\n", tx_key.value);
	}


	/* Delete all service replay related entries from flow list */
	itr = fi_opx_rbt_find(service->tx.flow, (void*)tx_key.value);
	if (itr) {
		struct fi_opx_reliability_tx_replay ** value_ptr =
			(struct fi_opx_reliability_tx_replay **) fi_opx_rbt_value_ptr(service->tx.flow, itr);

		struct fi_opx_reliability_tx_replay * head = *value_ptr;

		if (head) {
			/* Retire all queue elements */
			*value_ptr = NULL;

			struct fi_opx_reliability_tx_replay * next = NULL;
			struct fi_opx_reliability_tx_replay * tmp = head;

			do {
#ifdef OPX_RELIABILITY_DEBUG
				fprintf(stderr, "(tx) packet %016lx %08u retired.\n", tx_key.value, FI_OPX_HFI1_PACKET_PSN(&tmp->scb.hdr));
#endif
				next = tmp->next;

				fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, tmp);
				tmp = next;
			} while (tmp != head);

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(tx) Server service flow__ %016lx is reset.\n", tx_key.value);
		}
	}

	/* Reset handshake flow rbtree */
	itr = fi_opx_rbt_find(opx_ep->reliability->service.handshake_init, (void*)tx_key.value);
	if (itr) {
		rbtErase(opx_ep->reliability->service.handshake_init, itr);

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"(tx) Server handshake init flow__ %016lx is reset.\n", tx_key.value);
	}
}


void fi_opx_hfi1_rx_reliability_resynch (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		uint32_t origin_reliability_rx,
		const union fi_opx_hfi1_packet_hdr *const hdr)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	struct fi_opx_reliability_client_state * state = &opx_ep->reliability->state;

	union fi_opx_reliability_service_flow_key rx_key = { .value = hdr->service.key };
	union fi_opx_reliability_service_flow_key tx_key = {
		.slid = rx_key.dlid,
		.tx = state->tx,
		.dlid = rx_key.slid,
		.rx = state->rx
	};
	union fi_opx_reliability_service_flow_key tx_local_client_key = {
		.slid = rx_key.dlid,
		.tx = state->tx,
		.dlid = rx_key.slid,
		.rx = origin_reliability_rx
	};

	/* 
	 * INTRA-NODE:
	 * Reset all SHM related reliability protocol data retained by this
	 * Server EP about the remote Client EP.
	 */
	if (rx_key.slid == rx_key.dlid) {

		/* Record completion of the resynch request for the remote Client EP */
		opx_ep->rx->shm.resynch_connection[origin_reliability_rx].completed = true;
		opx_ep->rx->shm.resynch_connection[origin_reliability_rx].counter++;

#ifdef OPX_RELIABILITY_DEBUG
		fprintf(stderr, "(rx) SHM - Server flow__ %016lx 0x%x rcv resynch: %ld\n",
			rx_key.value, origin_reliability_rx,
			opx_ep->rx->shm.resynch_connection[origin_reliability_rx].counter);
#endif

		/*
		 * Close connection to the remote Client EP to cause the Server EP to
		 * re-establish a connection on the next transmit operation issued
		 *  by the Server EP.
		 */
		opx_shm_tx_close(&opx_ep->tx->shm, origin_reliability_rx);

	 	/* Send ack to notify the remote ep that the resynch was completed */

		fi_opx_hfi1_tx_reliability_inject_shm(ep,
			rx_key.value, tx_key.dlid, origin_reliability_rx,
			opx_ep->hfi->hfi_unit,
			FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK);
		
		return;
	}

	/* 
	 * INTER-NODE:
	 * Reset all rx/tx related reliability protocol data retained by this
	 * Server EP about the remote Client EP.
	 */
#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(rx) Server flow__ %016lx rcv resynch\n", rx_key.value);
#endif

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"Server resynching local:\n");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"\t(rx) flow__ %016lx\n", rx_key.value);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"\t(tx) flow__ %016lx\n", tx_key.value);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"\t(tx) Local-Client flow__ %016lx\n", tx_local_client_key.value);

	/*
	 * Reset all (rx) related reliability protocol data
	 */
	struct fi_opx_reliability_flow *flow;
	void *itr, *rx_key_ptr;

	itr = fi_opx_rbt_find(state->rx_flow_rbtree, (void*)rx_key.value);
	if (itr) {
        fi_opx_rbt_key_value(state->rx_flow_rbtree, itr, &rx_key_ptr, (void **)&flow);
		//dump_flow_rx(flow, __LINE__);

		/* Reset next expected inbound packet PSN value */
		flow->next_psn = 0;

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"(rx) Server flow__ %016lx is reset.\n", rx_key.value);

		/* Delete all uepkt related entries from flow list */
		struct fi_opx_reliability_rx_uepkt * head = flow->uepkt;

		while (head != NULL) {
#ifdef OPX_RELIABILITY_DEBUG
			fprintf(stderr, "(rx) packet %016lx %08lu deleted.\n", rx_key.value, head->psn);
#endif

			struct fi_opx_reliability_rx_uepkt * next = head->next;
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
	fi_opx_reliability_resynch_tx_flow_reset(opx_ep, service, state, tx_key);

	/* 
	 * When a DAOS Server is configured for multiple Engine instances, each Engine
	 * instance can act as both a server and client EP.  If this is so, then set
	 * the resynch_client_ep flag to indicate that a reset of all (tx) related
	 * flows is necessary, when the Engine instance attempts its first transmit
	 * as a client EP.
	 */
	itr = fi_opx_rbt_find(opx_ep->reliability->state.flow_rbtree_resynch,
				(void*)tx_local_client_key.value);

	if (itr) {
		struct fi_opx_reliability_resynch_flow ** value_ptr =
			(struct fi_opx_reliability_resynch_flow **) fi_opx_rbt_value_ptr(
					opx_ep->reliability->state.flow_rbtree_resynch, itr
				);
		struct fi_opx_reliability_resynch_flow * resynch_flow = *value_ptr;

		resynch_flow->resynch_client_ep = 1;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"(tx) Local-Client flow__ %016lx found, initiate reset of local client flows.\n",
			tx_local_client_key.value);
	}

	/* 
	 * Create record of the RESYNCH operation being completed for all (rx) & (tx)
	 * related reliability protocol data.
	 */
	fi_opx_reliability_resynch_flow_init(state, tx_key, false);

	/*
	 * Send ack to notify the remote ep that the resynch was completed
	 */
	fi_opx_hfi1_tx_reliability_inject_ud_resynch(ep,
		rx_key.value, tx_key.dlid, origin_reliability_rx,
		FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH_ACK);
}


void fi_opx_hfi1_rx_reliability_ack_resynch (struct fid_ep *ep,
		struct fi_opx_reliability_service * service,
		const union fi_opx_hfi1_packet_hdr *const hdr)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	union fi_opx_reliability_service_flow_key rx_key = { .value = hdr->service.key };

#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr, "(rx) %s Client flow__ %016lx rcv resynch ack\n",
		(rx_key.slid == rx_key.dlid) ? "SHM -" : "",
		rx_key.value);
#endif

	/* No complicated linked list or queue mechanism required at this time.
	 * A simple flag to indicate reception of the ack resynch response is
	 * sufficient and quick.
	 */
	struct fi_opx_reliability_client_state * state = &opx_ep->reliability->state;
	struct fi_opx_reliability_resynch_flow * resynch_flow = NULL;
	void * itr, *rx_key_ptr;
	itr = fi_opx_rbt_find(state->flow_rbtree_resynch, (void*)rx_key.value);

	if (itr) {
		fi_opx_rbt_key_value(state->flow_rbtree_resynch, itr, &rx_key_ptr, (void **)&resynch_flow);

		resynch_flow->remote_ep_resynch_completed = 1;
	}
#ifdef OPX_RELIABILITY_DEBUG
	else {
		fprintf(stderr, "Warning, (rx) %s Client flow__ %016lx rcv resynch ack; not found.\n",
			(rx_key.slid == rx_key.dlid) ? "SHM -" : "",
			rx_key.value);
	}
#endif
}


ssize_t fi_opx_reliability_do_remote_ep_resynch(struct fid_ep *ep,
	union fi_opx_addr dest_addr,
	void *context,
	const uint64_t caps)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	ssize_t rc = FI_SUCCESS;
	bool inject_done = false;
	union fi_opx_reliability_service_flow_key tx_key = {
		.slid = opx_ep->tx->send.hdr.stl.lrh.slid,
		.tx = opx_ep->tx->send.hdr.reliability.origin_tx,
		.dlid = dest_addr.uid.lid,
		.rx = dest_addr.hfi1_rx
	};

	if (!opx_ep->reliability ||
		opx_ep->reliability->state.kind != OFI_RELIABILITY_KIND_ONLOAD) {
		/* Nothing to do */
		return FI_SUCCESS;
	}

	if (fi_opx_hfi1_tx_is_intranode(ep, dest_addr.fi, caps)) {
		/* INTRA-NODE */

		/* HFI Rank Support: Retreive extended addressing data
		 */
		struct fi_context2 * opx_context = (struct fi_context2 *) context;

		if (opx_context) {
			if (opx_context->internal[0]) {
				struct fi_opx_extended_addr * opx_addr =
					(struct fi_opx_extended_addr *) opx_context->internal[0];

				opx_ep->daos_info.rank = opx_addr->rank;
				opx_ep->daos_info.rank_inst = opx_addr->rank_inst;
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					"(tx) SHM - rank:%d, rank_inst:%d\n",
					opx_ep->daos_info.rank,
					opx_ep->daos_info.rank_inst);
			} else {
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					"(tx) SHM - Extended address not available\n");
			}
		} else {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(tx) SHM - Extended address not available\n");
		}

		if (opx_ep->daos_info.rank == opx_ep->hfi->daos_info.rank &&
			opx_ep->daos_info.rank_inst == opx_ep->hfi->daos_info.rank_inst) {
			/* Nothing to do */
			return FI_SUCCESS;
		}

		unsigned rx_index =
			(opx_ep->daos_info.hfi_rank_enabled) ?
				opx_shm_daos_rank_index(opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst) :
				dest_addr.hfi1_rx;

		/* 
		 * Check whether RESYNCH request has been received from the remote EP.
		 * If so, then this is a Server EP amd there is nothing to be done.
		 */
		if (opx_ep->rx->shm.resynch_connection[rx_index].completed) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"(rx) SHM - Server already received resynch from Client %016lx 0x%x: %ld\n",
				tx_key.value,
				rx_index,
				opx_ep->rx->shm.resynch_connection[rx_index].counter);

			return FI_SUCCESS;
		}

		/* 
		 * Check whether packets have already been sent to the dest EP.  If not,
		 * then send RESYNCH request to the dest Server EP.  This causes the dest
		 * Server EP to resynch all SHM related data that it maintains associated
		 * with this Client EP.
		 */
		if (!opx_ep->tx->shm.fifo_segment[rx_index] ||
			!opx_ep->tx->shm.connection[rx_index].inuse) {
			rc = fi_opx_shm_dynamic_tx_connect(1, opx_ep, rx_index, dest_addr.hfi1_unit);
			if (OFI_UNLIKELY(rc)) {
				return -FI_EAGAIN;
			}

			inject_done = true;
			rc = fi_opx_hfi1_tx_reliability_inject_shm(ep,
					tx_key.value, dest_addr.uid.lid, rx_index,
					dest_addr.hfi1_unit,
					FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH);
			if (rc)
				return -FI_EAGAIN;
		}
	} else {
		/* INTER-NODE */

		/* 
		 * Check whether a RESYNCH request was already received from the remote EP.
		 * If so, then this is a Server EP; otherwise this is a Client EP.
		 */
		void * rx_itr = fi_opx_rbt_find(opx_ep->reliability->state.flow_rbtree_resynch,
										(void*)tx_key.value);

		if (rx_itr) {
			struct fi_opx_reliability_resynch_flow ** value_ptr =
				(struct fi_opx_reliability_resynch_flow **) fi_opx_rbt_value_ptr(
						opx_ep->reliability->state.flow_rbtree_resynch, rx_itr
					);

			struct fi_opx_reliability_resynch_flow * resynch_flow = *value_ptr;
			if (!resynch_flow->client_ep) {
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					"(rx) Server already received resynch from Client %016lx: %ld\n",
					tx_key.value, resynch_flow->resynch_counter);
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
					FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
						"(tx) Local-Client resynch notification received for Local-Client %016lx: %ld\n",
						tx_key.value, resynch_flow->resynch_counter);

					/* Reset all (tx) related reliability protocol data */
					fi_opx_reliability_resynch_tx_flow_reset(opx_ep,
						opx_ep->reliability->state.service,
						&opx_ep->reliability->state,
						tx_key);
				}
			}
		}

		/*
		 * Query (tx) rbTree to determine whether packets have already been sent
		 * to the remote Server EP.  If not, then send RESYNCH request to the Server EP.
		 * This causes the Server EP to reset all necessary rbTree related data that
		 * it maintains associated with this Client EP.
		 */
		void * itr = fi_opx_rbt_find(opx_ep->reliability->state.tx_flow_rbtree, (void*)tx_key.value);
		if (!itr) {
			inject_done = true;

			fi_opx_hfi1_tx_reliability_inject_ud_resynch(ep,
				tx_key.value, tx_key.dlid, dest_addr.reliability_rx,
				FI_OPX_HFI_UD_OPCODE_RELIABILITY_RESYNCH);
		}
	}

	/*
	 * If necessary, wait for ACK response to the RESYNCH request.
	 */
	if (inject_done) {
		/* Create entry to monitor reception of ACK response */
		struct fi_opx_reliability_resynch_flow * resynch_flow =
			fi_opx_reliability_resynch_flow_init(&opx_ep->reliability->state, tx_key, true);

		if (!resynch_flow) {
			return -FI_EAGAIN;
		}

		struct fi_opx_reliability_service *service = opx_ep->reliability->state.service;
		union fi_opx_timer_state * timer = &service->tx.timer;
		union fi_opx_timer_stamp * timestamp = &service->tx.timestamp;
		union fi_opx_timer_stamp start;
		uint64_t compare = fi_opx_timer_now(&start, timer);
		uint64_t next =
			fi_opx_timer_next_event_usec(timer, &start, FI_OPX_TIMER_NEXT_EVENT_USEC_DEFAULT);

		while (compare < next) {
			fi_opx_ep_rx_poll(&opx_ep->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
			compare = fi_opx_timer_now(timestamp, timer);

			if (resynch_flow->remote_ep_resynch_completed) {
				return FI_SUCCESS;
			}
		}

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"(rx) Client failed to receive ack resynch from Server %016lx.\n",
			tx_key.value);
		rc = -FI_EAGAIN;
	}

	return rc;
}
