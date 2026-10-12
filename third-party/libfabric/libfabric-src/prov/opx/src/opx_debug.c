/*
 * Copyright (C) 2025 Cornelis Networks.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include "rdma/opx/opx_debug.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_eq.h"

static struct slist ep_list = {.head = NULL};

static void (*prev_sig_handler)(int) = NULL;

static inline size_t opx_debug_slist_len(struct slist_entry *head)
{
	size_t		    count = 0;
	struct slist_entry *tmp	  = head;

	while (tmp) {
		++count;
		tmp = tmp->next;
	}

	return count;
}

static inline uint8_t opx_debug_get_replay_bth_opcode(struct fi_opx_reliability_tx_replay *replay,
						      enum opx_hfi1_type		   hfi1_type)
{
	return (hfi1_type & (OPX_HFI1_MIXED_9B | OPX_HFI1_WFR)) ? replay->scb.scb_9B.hdr.bth.opcode :
								  replay->scb.scb_16B.hdr.bth.opcode;
}

static inline uint32_t opx_debug_get_replay_psn(struct fi_opx_reliability_tx_replay *replay,
						enum opx_hfi1_type		     hfi1_type)
{
	uint8_t opcode = opx_debug_get_replay_bth_opcode(replay, hfi1_type);

	if (opcode == 0xC2) {
		return (hfi1_type & (OPX_HFI1_MIXED_9B | OPX_HFI1_WFR)) ?
			       ((uint32_t) ntohl(replay->scb.scb_9B.hdr.bth.psn)) & 0x00FFFFFF :
			       ((uint32_t) ntohl(replay->scb.scb_16B.hdr.bth.psn)) & 0x00FFFFFF;
	} else {
		return (hfi1_type & (OPX_HFI1_MIXED_9B | OPX_HFI1_WFR)) ? replay->scb.scb_9B.hdr.bth.psn & 0x00FFFFFF :
									  replay->scb.scb_16B.hdr.bth.psn & 0x00FFFFFF;
	}
}

static void opx_debug_dump_rx_flow(struct fi_opx_ep *opx_ep, pid_t my_pid, FILE *output)
{
	fprintf(output,
		"(%d) ################################### RX Flow RB Tree #############################################\n",
		my_pid);
	fprintf(output,
		"(%d) #                              Expected    Unexpected Queue                                     #\n",
		my_pid);
	fprintf(output,
		"(%d) # Flow Key                     PSN         Count    First PSN   Last PSN   Ptr                  #\n",
		my_pid);
	fprintf(output,
		"(%d) # --------------------------------------------------------------------------------------------- #\n",
		my_pid);

	RbtIterator itr = (RbtIterator) rbtBegin(opx_ep->reli_service->rx.rx_flow_rbtree);

	while (itr) {
		NodeType			  *node_type  = (NodeType *) itr;
		struct fi_opx_reliability_rx_flow *flow_entry = (struct fi_opx_reliability_rx_flow *) node_type->val;

		uint32_t ue_count     = 0;
		uint32_t ue_first_psn = 0;
		uint32_t ue_last_psn  = 0;

		struct fi_opx_reliability_rx_uepkt *ue = (struct fi_opx_reliability_rx_uepkt *) flow_entry->uepkt;

		if (ue) {
			ue_first_psn = ue->psn;

			while (ue && ue_count < 5000) {
				++ue_count;
				ue_last_psn = ue->psn;
				ue	    = ue->next;
			}
		}

		fprintf(output, "(%d) # [%06X.%04X.%06X.%04X]    %08lX    %06X   %06X      %06X     %016lX     #\n",
			my_pid, flow_entry->key->slid, flow_entry->key->src_subctxt_rx, flow_entry->key->dlid,
			flow_entry->key->dst_subctxt_rx, flow_entry->next_psn, ue_count, ue_first_psn, ue_last_psn,
			(uintptr_t) flow_entry->uepkt);

		itr = (RbtIterator) rbtNext(opx_ep->reli_service->rx.rx_flow_rbtree, itr);
	}
	fprintf(output,
		"(%d) #################################################################################################\n",
		my_pid);
}

static void opx_debug_dump_tx_flow(struct fi_opx_ep *opx_ep, pid_t my_pid, FILE *output)
{
	fprintf(output,
		"(%d) ################################### TX Flow RB Tree ######################################################\n",
		my_pid);
	fprintf(output,
		"(%d) #                                                                Bytes      Replay         Replays       #\n",
		my_pid);
	fprintf(output,
		"(%d) # Flow Key                     Next PSN  Throttle  Nack_Count  Outstanding  Count   First PSN   Last PSN #\n",
		my_pid);
	fprintf(output,
		"(%d) # ------------------------------------------------------------------------------------------------------ #\n",
		my_pid);

	RbtIterator itr = (RbtIterator) rbtBegin(opx_ep->reli_service->tx.tx_flow_outstanding_pkts_rbtree);

	while (itr) {
		NodeType   *node_type  = (NodeType *) itr;
		RbtIterator itr2       = (RbtIterator) rbtFind(opx_ep->reli_service->tx.tx_flow_rbtree, node_type->key);
		NodeType   *node_type2 = (NodeType *) itr2;
		union fi_opx_reliability_tx_psn *psn_ptr = (union fi_opx_reliability_tx_psn *) &node_type2->val;
		union fi_opx_reliability_service_flow_key *flow_key =
			(union fi_opx_reliability_service_flow_key *) node_type->key;

		fprintf(output, "(%d) # [%06X.%04X.%06X.%04X]     %06X   %4u    %8u    %8u  ", my_pid, flow_key->slid,
			flow_key->src_subctxt_rx, flow_key->dlid, flow_key->dst_subctxt_rx, psn_ptr->psn.psn,
			psn_ptr->psn.throttle, psn_ptr->psn.nack_count, psn_ptr->psn.bytes_outstanding);

		if (node_type->val) {
			int32_t	 last_psn     = -1;
			uint32_t replay_count = 0;

			struct fi_opx_reliability_tx_replay *replay =
				(struct fi_opx_reliability_tx_replay *) node_type->val;
			int32_t psn	  = opx_debug_get_replay_psn(replay, OPX_SW_HFI1_TYPE);
			int32_t first_psn = psn;

			struct fi_opx_reliability_tx_replay *first_replay = NULL;
			while (replay != first_replay) {
				++replay_count;
				psn	 = opx_debug_get_replay_psn(replay, OPX_SW_HFI1_TYPE);
				last_psn = psn;

				if (!first_replay) {
					first_replay = replay;
				}
				replay = replay->next;
			}
			fprintf(output, "   %6u    %06X      %06X   #\n", replay_count, first_psn, last_psn);
		} else {
			fprintf(output, "        0       N/A         N/A   #\n");
		}

		itr = (RbtIterator) rbtNext(opx_ep->reli_service->tx.tx_flow_outstanding_pkts_rbtree, itr);
	}
	fprintf(output,
		"(%d) ##########################################################################################################\n",
		my_pid);
}

static void opx_debug_dump_opx_cq(pid_t my_pid, struct fi_opx_cq *cq, FILE *output)
{
	if (cq) {
		fprintf(output, "(%d)        progress.ep_count               : %lu\n", my_pid, cq->progress.ep_count);
		fprintf(output, "(%d)        ep_bind_count                   : %lu\n", my_pid, cq->ep_bind_count);
		fprintf(output, "(%d)        pending                         : %p (%lu)\n", my_pid, cq->pending.head,
			opx_debug_slist_len(cq->pending.head));
		fprintf(output, "(%d)        completed                       : %p (%lu)\n", my_pid, cq->completed.head,
			opx_debug_slist_len(cq->completed.head));
		fprintf(output, "(%d)        err                             : %p (%lu)\n", my_pid, cq->err.head,
			opx_debug_slist_len(cq->err.head));
	}
}

static void opx_debug_dump_opx_cntr(pid_t my_pid, struct fi_opx_cntr *cq, FILE *output)
{
	if (cq) {
		fprintf(output, "(%d)        progress.ep_count               : %lu\n", my_pid, cq->progress.ep_count);
		fprintf(output, "(%d)        ep_bind_count                   : %lu\n", my_pid, cq->ep_bind_count);
		fprintf(output, "(%d)        std                             : %ld\n", my_pid, cq->std.val);
		fprintf(output, "(%d)        err                             : %ld\n", my_pid, cq->err.val);
	}
}

static void opx_debug_dump_backtrace(FILE *output)
{
	void  *callstack[128];
	int    frames = backtrace(callstack, 128);
	char **strs   = backtrace_symbols(callstack, frames);
	for (int i = 0; i < frames; ++i) {
		fprintf(output, "[%d]: %s\n", i, strs[i]);
	}
	free(strs);
}

static void opx_debug_dump_endpoint(struct fi_opx_ep *opx_ep)
{
	char  hostname[HOST_NAME_MAX + 1];
	pid_t my_pid = getpid();

	int rc = gethostname(hostname, HOST_NAME_MAX);
	if (rc != 0) {
		fprintf(stderr, "Error: gethostname() returned %d\n", rc);
		return;
	}

	char output_filename[1024];
	sprintf(output_filename, "%s-%u-%06X-%04X.out", hostname, my_pid, opx_ep->rx->self.lid,
		opx_ep->rx->self.hfi1_subctxt_rx);

	FILE *output = fopen(output_filename, "w");

	if (output == NULL) {
		fprintf(stderr, "Error creating output file %s. Do you have write permissions?\n", output_filename);
		return;
	}

	int lock_required = fi_opx_threading_lock_required(opx_ep->threading, fi_opx_global.progress);
	fi_opx_lock_if_required(&opx_ep->lock, lock_required);

	fprintf(output, "(%d) ========================================================\n", my_pid);
	fprintf(output, "(%d) opx_ep : %p\n", my_pid, opx_ep);
	fprintf(output, "(%d) ========================================================\n", my_pid);

	fprintf(output, "(%d) ------------------------ Deferred work -----------------\n", my_pid);

	fprintf(output, "(%d) work_pending[SDMA].head                : %p (%lu)\n", my_pid,
		opx_ep->tx->work_pending[OPX_WORK_TYPE_SDMA].head,
		opx_debug_slist_len(opx_ep->tx->work_pending[OPX_WORK_TYPE_SDMA].head));

	fprintf(output, "(%d) work_pending[PIO].head                 : %p (%lu)\n", my_pid,
		opx_ep->tx->work_pending[OPX_WORK_TYPE_PIO].head,
		opx_debug_slist_len(opx_ep->tx->work_pending[OPX_WORK_TYPE_PIO].head));

	fprintf(output, "(%d) work_pending[SHM].head                 : %p (%lu)\n", my_pid,
		opx_ep->tx->work_pending[OPX_WORK_TYPE_SHM].head,
		opx_debug_slist_len(opx_ep->tx->work_pending[OPX_WORK_TYPE_SHM].head));

	fprintf(output, "(%d) work_pending[TID_SETUP].head           : %p (%lu)\n", my_pid,
		opx_ep->tx->work_pending[OPX_WORK_TYPE_TID_SETUP].head,
		opx_debug_slist_len(opx_ep->tx->work_pending[OPX_WORK_TYPE_TID_SETUP].head));

	fprintf(output, "(%d) work_pending[HFISVC].head              : %p (%lu)\n", my_pid,
		opx_ep->tx->work_pending[OPX_WORK_TYPE_HFISVC].head,
		opx_debug_slist_len(opx_ep->tx->work_pending[OPX_WORK_TYPE_HFISVC].head));

	fprintf(output, "(%d) sdma_request_queue.list.head           : %p (%lu)\n", my_pid,
		opx_ep->tx->sdma_request_queue.list.head,
		opx_debug_slist_len(opx_ep->tx->sdma_request_queue.list.head));

	fprintf(output, "(%d) sdma_pending_queue.head                : %p (%lu)\n", my_pid,
		opx_ep->tx->sdma_pending_queue.head, opx_debug_slist_len(opx_ep->tx->sdma_pending_queue.head));

	fprintf(output, "(%d) work_pending_completion.head           : %p (%lu)\n", my_pid,
		opx_ep->tx->work_pending_completion.head,
		opx_debug_slist_len(opx_ep->tx->work_pending_completion.head));

	fprintf(output, "(%d) reliability->service.work_pending.head : %p (%lu)\n", my_pid,
		opx_ep->reli_service->rx.work_pending.head,
		opx_debug_slist_len(opx_ep->reli_service->rx.work_pending.head));

	fprintf(output, "(%d) --------------- Pending/Completion Queues --------------\n", my_pid);
	fprintf(output, "(%d) tx.cq_pending_ptr                      : %p (%lu)\n", my_pid,
		opx_ep->tx->cq_pending_ptr->head, opx_debug_slist_len(opx_ep->tx->cq_pending_ptr->head));

	fprintf(output, "(%d) tx.cq_completed_ptr                    : %p (%lu)\n", my_pid,
		opx_ep->tx->cq_completed_ptr->head, opx_debug_slist_len(opx_ep->tx->cq_completed_ptr->head));

	fprintf(output, "(%d) tx.cq_err_ptr                          : %p (%lu)\n", my_pid,
		opx_ep->tx->cq_err_ptr->head, opx_debug_slist_len(opx_ep->tx->cq_err_ptr->head));

	fprintf(output, "(%d) rx.cq_pending_ptr                      : %p (%lu)\n", my_pid,
		opx_ep->rx->cq_pending_ptr->head, opx_debug_slist_len(opx_ep->rx->cq_pending_ptr->head));

	fprintf(output, "(%d) rx.cq_completed_ptr                    : %p (%lu)\n", my_pid,
		opx_ep->rx->cq_completed_ptr->head, opx_debug_slist_len(opx_ep->rx->cq_completed_ptr->head));

	fprintf(output, "(%d) rx.cq_err_ptr                          : %p (%lu)\n", my_pid,
		opx_ep->rx->cq_err_ptr->head, opx_debug_slist_len(opx_ep->rx->cq_err_ptr->head));

	fprintf(output, "(%d) ------------------------- Match Queues -----------------\n", my_pid);
	fprintf(output, "(%d) TAG     queue[0].mq.head               : %p (%lu)\n", my_pid,
		opx_ep->rx->queue[0].mq.head, opx_debug_slist_len(opx_ep->rx->queue[0].mq.head));

	fprintf(output, "(%d) TAG     queue[0].ue.head               : %p (%lu)\n", my_pid,
		opx_ep->rx->queue[0].ue.head, opx_debug_slist_len((struct slist_entry *) opx_ep->rx->queue[0].ue.head));

	fprintf(output, "(%d) MSG     queue[1].mq.head               : %p (%lu)\n", my_pid,
		opx_ep->rx->queue[1].mq.head, opx_debug_slist_len(opx_ep->rx->queue[1].mq.head));

	fprintf(output, "(%d) MSG     queue[1].ue.head               : %p (%lu)\n", my_pid,
		opx_ep->rx->queue[1].ue.head, opx_debug_slist_len((struct slist_entry *) opx_ep->rx->queue[1].ue.head));

	fprintf(output, "(%d) MP-EGR  mp_egr_queue.mq.head           : %p (%lu)\n", my_pid,
		opx_ep->rx->mp_egr_queue.mq.head, opx_debug_slist_len(opx_ep->rx->mp_egr_queue.mq.head));

	fprintf(output, "(%d) MP-EGR  mp_egr_queue.ue.head           : %p (%lu)\n", my_pid,
		opx_ep->rx->mp_egr_queue.ue.head,
		opx_debug_slist_len((struct slist_entry *) opx_ep->rx->mp_egr_queue.ue.head));

	fprintf(output, "(%d) ------------------------- Counters ---------------------\n", my_pid);

	fprintf(output, "(%d) opx_ep->read_cntr                      : %p\n", my_pid, opx_ep->read_cntr);
	opx_debug_dump_opx_cntr(my_pid, opx_ep->read_cntr, output);

	fprintf(output, "(%d) opx_ep->write_cntr                     : %p\n", my_pid, opx_ep->write_cntr);
	opx_debug_dump_opx_cntr(my_pid, opx_ep->write_cntr, output);

	fprintf(output, "(%d) opx_ep->send_cntr                      : %p\n", my_pid, opx_ep->send_cntr);
	opx_debug_dump_opx_cntr(my_pid, opx_ep->send_cntr, output);

	fprintf(output, "(%d) opx_ep->recv_cntr                      : %p\n", my_pid, opx_ep->recv_cntr);
	opx_debug_dump_opx_cntr(my_pid, opx_ep->recv_cntr, output);

	fprintf(output, "(%d) opx_ep->init_tx_cq                     : %p\n", my_pid, opx_ep->init_tx_cq);
	opx_debug_dump_opx_cq(my_pid, opx_ep->init_tx_cq, output);

	fprintf(output, "(%d) opx_ep->init_rx_cq                     : %p\n", my_pid, opx_ep->init_rx_cq);
	opx_debug_dump_opx_cq(my_pid, opx_ep->init_rx_cq, output);

	fprintf(output, "(%d) --------------------------- PIO  Info -------------------\n", my_pid);
	fprintf(output, "(%d) tx->pio_state->credits_total           : %hu\n", my_pid,
		opx_ep->tx->pio_state->credits_total);
	fprintf(output, "(%d) credits in use                         : %hu\n", my_pid,
		(uint16_t) ((opx_ep->tx->pio_state->fill_counter - opx_ep->tx->pio_state->free_counter_shadow) &
			    0x7FF));
	fprintf(output, "(%d) credits available                      : %hu\n", my_pid,
		(uint16_t) (opx_ep->tx->pio_state->credits_total -
			    ((opx_ep->tx->pio_state->fill_counter - opx_ep->tx->pio_state->free_counter_shadow) &
			     0x7FF)));
	fprintf(output, "(%d) tx->pio_max_eager_tx_bytes             : %hu\n", my_pid,
		opx_ep->tx->pio_max_eager_tx_bytes);
	fprintf(output, "(%d) --------------------------- SDMA Info -------------------\n", my_pid);
	fprintf(output, "(%d) hfi->info.sdma.queue_size              : %hu\n", my_pid,
		opx_ep->hfi->info.sdma.queue_size);
	fprintf(output, "(%d) hfi->info.sdma.fill_index              : %hu\n", my_pid,
		opx_ep->hfi->info.sdma.fill_index);
	fprintf(output, "(%d) hfi->info.sdma.done_index              : %hu\n", my_pid,
		opx_ep->hfi->info.sdma.done_index);
	fprintf(output, "(%d) hfi->info.sdma.available_counter       : %hu\n", my_pid,
		opx_ep->hfi->info.sdma.available_counter);
	fprintf(output, "(%d) tx->sdma_request_queue.num_reqs        : %hu\n", my_pid,
		opx_ep->tx->sdma_request_queue.num_reqs);
	fprintf(output, "(%d) tx->sdma_request_queue.num_iovs        : %hu\n", my_pid,
		opx_ep->tx->sdma_request_queue.num_iovs);
	fprintf(output, "(%d) tx->sdma_request_queue.slots_avail     : %hu\n", my_pid,
		opx_ep->tx->sdma_request_queue.slots_avail);
	fprintf(output, "(%d) tx->sdma_request_queue.max_iovs        : %hu\n", my_pid,
		opx_ep->tx->sdma_request_queue.max_iovs);

	fprintf(output, "(%d) ------------------------- Reliability -----------------\n", my_pid);
	fprintf(output, "(%d) opx_ep->rx->self.lid                   : %04X\n", my_pid, opx_ep->rx->self.lid);
	fprintf(output, "(%d) opx_ep->rx->self.hfi1_subctxt_rx       : %04X\n", my_pid,
		opx_ep->rx->self.hfi1_subctxt_rx);
	fprintf(output, "(%d) opx_ep->rx->self.hfi1_unit             : %02X\n", my_pid, opx_ep->rx->self.hfi1_unit);

	opx_debug_dump_rx_flow(opx_ep, my_pid, output);
	opx_debug_dump_tx_flow(opx_ep, my_pid, output);

	opx_debug_dump_backtrace(output);

	fclose(output);

	fi_opx_unlock_if_required(&opx_ep->lock, lock_required);
}

void opx_debug_ep_list_append(void *opx_ep)
{
	struct opx_debug_ep_entry *entry = (struct opx_debug_ep_entry *) ep_list.head;
	while (entry) {
		if (entry->ep == opx_ep) {
			return;
		}
		entry = entry->next;
	}

	struct opx_debug_ep_entry *new_entry = (struct opx_debug_ep_entry *) malloc(sizeof(struct opx_debug_ep_entry));
	new_entry->next			     = NULL;
	new_entry->ep			     = opx_ep;
	slist_insert_tail(&new_entry->slist_entry, &ep_list);
}

void opx_debug_ep_list_free(void *opx_ep)
{
	struct opx_debug_ep_entry *entry = (struct opx_debug_ep_entry *) ep_list.head;
	struct slist_entry	  *prev	 = NULL;

	while (entry) {
		if (entry->ep == opx_ep) {
			slist_remove(&ep_list, &entry->slist_entry, prev);
			free(entry);
			return;
		}
		prev  = &entry->slist_entry;
		entry = entry->next;
	}
}

void opx_debug_ep_list_dump()
{
	struct opx_debug_ep_entry *entry = (struct opx_debug_ep_entry *) ep_list.head;
	while (entry) {
		opx_debug_dump_endpoint((struct fi_opx_ep *) entry->ep);
		entry = entry->next;
	}
}

static void opx_debug_signal_handler(int signum, siginfo_t *info, void *ucontext)
{
	opx_debug_ep_list_dump();

	if (prev_sig_handler && prev_sig_handler != SIG_DFL && prev_sig_handler != SIG_IGN) {
		prev_sig_handler(signum);
	}
}

static int sig_handler_installed = 0;

void opx_debug_install_handler()
{
	if (sig_handler_installed) {
		return;
	}

	struct sigaction old_sa;
	struct sigaction act;

	memset(&act, 0, sizeof(act));

	act.sa_sigaction = opx_debug_signal_handler;
	act.sa_flags	 = SA_SIGINFO;

	sigaction(SIGUSR2, &act, &old_sa);

	prev_sig_handler = old_sa.sa_handler;

	sig_handler_installed = 1;
}
