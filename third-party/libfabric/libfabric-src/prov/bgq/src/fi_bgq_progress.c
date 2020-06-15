/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include "rdma/bgq/fi_bgq_progress.h"
#include "rdma/bgq/fi_bgq_rx.h"

/* internal */
union fi_bgq_progress_data {
	struct l2atomic_fifo_data	l2atomic;
	struct {
		uint64_t		cacheline[16];
		uint64_t		data[MAX_ENDPOINTS*2];
	};
};

/* internal function */
void update_progress_thread (struct fi_bgq_progress * progress) {


	struct l2atomic_fifo_consumer * consumer = &progress->consumer;
	uint64_t value_rsh3b = 0;

	/* Check if another endpoint should be managed by this progress thread
	 */
	if (l2atomic_fifo_consume(consumer, &value_rsh3b) == 0) {
		struct fi_bgq_ep *bgq_ep = (struct fi_bgq_ep *)(value_rsh3b << 3);

		assert(L2_AtomicLoad(&bgq_ep->async.enabled) != 0);
		assert(L2_AtomicLoad(&bgq_ep->async.active) == 0);

		progress->all_ep[(progress->all_ep_count)++] = bgq_ep;

		if (bgq_ep->rx.caps & FI_TAGGED) {
			progress->tag_ep[(progress->tag_ep_count)++] = bgq_ep;
		}
		if (bgq_ep->rx.caps & FI_MSG) {
			progress->msg_ep[(progress->msg_ep_count)++] = bgq_ep;
		}
		L2_AtomicStore(&bgq_ep->async.active, 1);
	}

	/*
	 * Advance control code path for each endpoint once and check
	 * each endpoint if async progress is disabled
	 */
	unsigned i = 0;
	while (i < progress->all_ep_count) {

		struct fi_bgq_ep *bgq_ep = progress->all_ep[i];
		poll_cfifo(bgq_ep, 0);

		if (L2_AtomicLoad(&bgq_ep->async.enabled) == 0) {
			L2_AtomicStore(&bgq_ep->async.active, 0);

			if (bgq_ep->rx.caps & FI_MSG) {
				unsigned n = 0;
				while (progress->msg_ep[n] != bgq_ep) ++n;
				progress->msg_ep[n] = progress->msg_ep[--(progress->msg_ep_count)];
			}

			if (bgq_ep->rx.caps & FI_TAGGED) {
				unsigned n = 0;
				while (progress->tag_ep[n] != bgq_ep) ++n;
				progress->tag_ep[n] = progress->tag_ep[--(progress->tag_ep_count)];
			}

			progress->all_ep[i] = progress->all_ep[--(progress->all_ep_count)];
		} else {
			++i;
		}
	}

	return;
}

/* internal function */
void poll_noinline (struct fi_bgq_ep *bgq_ep, const unsigned poll_msg, const uint64_t cancel_context) {

	poll_mfifo(bgq_ep, poll_msg, cancel_context, 0);
	poll_rfifo(bgq_ep, 0);
}

/* internal function */
void * progress_fn (void *arg) {

	struct fi_bgq_progress * progress = (struct fi_bgq_progress *)arg;

	struct fi_bgq_ep ** tag_ep = progress->tag_ep;
	struct fi_bgq_ep ** msg_ep = progress->msg_ep;
	struct fi_bgq_ep ** all_ep = progress->all_ep;

	struct l2atomic_fifo_consumer * consumer = &progress->consumer;
	struct l2atomic_fifo_producer * producer = &progress->producer;
	uint64_t value_rsh3b = 0;

	const unsigned tag_loop = 16;
	const unsigned msg_loop = 4;

	unsigned m, j, i;

	/* first, enable the progress thread control fifo by setting the
	 * HEAD and TAIL to zero and setting the BOUNDS to FIFO_SIZE-1
	 */
	l2atomic_fifo_enable(consumer, producer);


	progress->active = 1;
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	while (progress->enabled) {

		/* Advance performance critical code path for each endpoint multiple times */

		const unsigned tag_ep_count = progress->tag_ep_count;
		const unsigned msg_ep_count = progress->msg_ep_count;

		for (m=0; m<msg_loop; ++m) {
			for (j=0; j<tag_loop; ++j) {
				for (i=0; i<tag_ep_count; ++i) {
					poll_mfifo(tag_ep[i], 0, 0, 0);
					poll_rfifo(tag_ep[i], 0);
				}
			}
			for (i=0; i<msg_ep_count; ++i) {
				poll_noinline(msg_ep[i], 1, 0);
			}
		}

		update_progress_thread(progress);		
	}

	/*
	 * This progress thread has been disabled. Before setting the thread to inactive:
	 * 1. disable the progress thread control fifo by setting the BOUNDS to zero
	 * 2. drain the progress thread control fifo of endpoints
	 * 3. Attempt to transfer any endpoints managed by this progress thread to another progress thread
	 * 4. If no active progress threads, disable and deactivate remaining endpoints
	 */
	l2atomic_fifo_disable(consumer, producer);

	while (0 == l2atomic_fifo_drain(consumer, producer, &value_rsh3b)) {
		progress->all_ep[(progress->all_ep_count)++] = (struct fi_bgq_ep *)(value_rsh3b << 3);
	}

	struct fi_bgq_domain *bgq_domain = progress->bgq_domain;
	const unsigned max_threads = bgq_domain->progress.max_threads;

	for (i=0; i<progress->all_ep_count; ++i) {

		value_rsh3b = ((uint64_t)(all_ep[i])) >> 3;

		unsigned p;
		for (p=0; p<max_threads; ++p) {
			if (0 == l2atomic_fifo_produce(&bgq_domain->progress.thread[p].producer, value_rsh3b)) {
				all_ep[i] = NULL;
				break;
			}
		}

		if (all_ep[i] != NULL) {
			/* No active progress threads; disable async progress on this endpoint */
			L2_AtomicStore(&all_ep[i]->async.enabled, 0);
			L2_AtomicStore(&all_ep[i]->async.active, 0);
			all_ep[i] = NULL;
			/* TODO - is this an error or something? */
		}
	}

	/* Deactivate this progress thread and exit */
	progress->active = 0;
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	return NULL;
};

int fi_bgq_progress_init (struct fi_bgq_domain *bgq_domain, const uint64_t max_threads) {

	assert(max_threads < (64/Kernel_ProcessCount()));

	bgq_domain->progress.max_threads = max_threads;
	bgq_domain->progress.num_threads_active = 0;
	bgq_domain->progress.memptr = NULL;

	if (0 == max_threads) return 0;

	size_t i, j;

	const size_t bytes = sizeof(union fi_bgq_progress_data) * max_threads;
	const size_t alignment = 128;

	void * memptr = malloc(bytes+alignment);
	uint32_t cnk_rc = 0;
	cnk_rc = Kernel_L2AtomicsAllocate(memptr, bytes+alignment);
	assert(0==cnk_rc);
	if (cnk_rc != 0) {
		/* Error allocating l2atomic memory */
		free(memptr);
		bgq_domain->progress.memptr = NULL;
		return -1;
	}

	union fi_bgq_progress_data *data = (union fi_bgq_progress_data *)(((uintptr_t)memptr + alignment) & (~(alignment-1)));
	const uint64_t npackets = sizeof(data[0].data) / sizeof(uint64_t);
	const size_t nep = sizeof(bgq_domain->progress.thread[0].tag_ep) / sizeof(struct fi_bgq_ep *);
	for (i=0; i<max_threads; ++i) {
		l2atomic_fifo_initialize(&bgq_domain->progress.thread[i].consumer,
			&bgq_domain->progress.thread[i].producer,
			&data[i].l2atomic, npackets);

		bgq_domain->progress.thread[i].tag_ep_count = 0;
		bgq_domain->progress.thread[i].msg_ep_count = 0;
		bgq_domain->progress.thread[i].all_ep_count = 0;
		bgq_domain->progress.thread[i].pthread = 0;
		bgq_domain->progress.thread[i].enabled = 0;
		bgq_domain->progress.thread[i].active = 0;

		fi_bgq_ref_inc(&bgq_domain->ref_cnt, "domain");
		bgq_domain->progress.thread[i].bgq_domain = bgq_domain;

		for (j=0; j<nep; ++j) {
			bgq_domain->progress.thread[i].tag_ep[j] = NULL;
			bgq_domain->progress.thread[i].msg_ep[j] = NULL;
			bgq_domain->progress.thread[i].all_ep[j] = NULL;
		}
	}

	bgq_domain->progress.memptr = memptr;

	return 0;
}

int fi_bgq_progress_enable (struct fi_bgq_domain *bgq_domain, const unsigned id) {

	assert(id < (64/Kernel_ProcessCount()-1));
	assert(id < bgq_domain->progress.max_threads);

	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_RO);
	if (bgq_domain->progress.thread[id].enabled) {
		assert(bgq_domain->progress.thread[id].active);
		return 0;
	}

	bgq_domain->progress.thread[id].enabled = 1;
	bgq_domain->progress.thread[id].active = 0;
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	int rc = 0;
	rc = pthread_create(&bgq_domain->progress.thread[id].pthread, NULL, progress_fn, (void *)&bgq_domain->progress.thread[id]);
	if (rc) {
		/* Error starting this progress thread */
		bgq_domain->progress.thread[id].enabled = 0;
		bgq_domain->progress.thread[id].active = 0;
		return -1;
	}

	/* Wait until the progress thread is active */
	while (0 == bgq_domain->progress.thread[id].active) {
		fi_bgq_msync(FI_BGQ_MSYNC_TYPE_RO);
	}

	++(bgq_domain->progress.num_threads_active);
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	return 0;
}

int fi_bgq_progress_disable (struct fi_bgq_domain *bgq_domain, const unsigned id) {

	assert(id < (64/Kernel_ProcessCount()-1));
	assert(id < bgq_domain->progress.max_threads);

	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_RO);
	if (0 == bgq_domain->progress.thread[id].enabled) {
		assert(0 == bgq_domain->progress.thread[id].active);
		return 0;
	}

	bgq_domain->progress.thread[id].enabled = 0;
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	/* Wait until the progress thread is active */
	while (bgq_domain->progress.thread[id].active) {
		fi_bgq_msync(FI_BGQ_MSYNC_TYPE_RO);
	}

	int rc __attribute__ ((unused));
	void *retval = NULL;
	rc = pthread_join(bgq_domain->progress.thread[id].pthread, &retval);
	assert(0 == rc);
	bgq_domain->progress.thread[id].pthread = 0;

	l2atomic_fifo_disable(&bgq_domain->progress.thread[id].consumer,
		&bgq_domain->progress.thread[id].producer);

	--(bgq_domain->progress.num_threads_active);

	return 0;
}

int fi_bgq_progress_fini (struct fi_bgq_domain *bgq_domain) {

	assert(0==bgq_domain->progress.num_threads_active);

	int i;
	for (i=0; i<bgq_domain->progress.max_threads; ++i) {
		assert(0 == bgq_domain->progress.thread[i].enabled);
		assert(0 == bgq_domain->progress.thread[i].active);
		//l2atomic_fifo_finalize(&bgq_domain->progress.thread[i].consumer,
		//	&bgq_domain->progress.thread[i].producer);
		bgq_domain->progress.thread[i].bgq_domain = NULL;
		fi_bgq_ref_dec(&bgq_domain->ref_cnt, "domain");
	}

	free(bgq_domain->progress.memptr);
	bgq_domain->progress.memptr = NULL;

	return 0;
}

int fi_bgq_progress_ep_enable (struct fi_bgq_progress *thread, struct fi_bgq_ep *bgq_ep) {

	bgq_ep->async.active = 0;
	bgq_ep->async.enabled = 1;
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	uint64_t value_rsh3b = ((uint64_t)bgq_ep) >> 3;
	if (0 == l2atomic_fifo_produce(&thread->producer, value_rsh3b)) {

		/* Wait until async progress on the endpoint is activated */
		while (0 == bgq_ep->async.active) {
			fi_bgq_msync(FI_BGQ_MSYNC_TYPE_RW);
		}

	} else {

		/* "fifo is full" means that the progress thread has been disabled */
		assert(0);
		return -1;
	}

	return 0;
}

int fi_bgq_progress_ep_disable (struct fi_bgq_ep *bgq_ep) {

	bgq_ep->async.enabled = 0;
	fi_bgq_msync(FI_BGQ_MSYNC_TYPE_WO);

	/* Wait until async progress on the endpoint is deactivated */
	while (0 != bgq_ep->async.active) {
		fi_bgq_msync(FI_BGQ_MSYNC_TYPE_RO);
	}

	return 0;
}
