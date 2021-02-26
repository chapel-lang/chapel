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

#ifndef N_PACKETS_LOG2
#define N_PACKETS_LOG2 (16)
#endif

#define N_PACKETS (1ull << N_PACKETS_LOG2)

#define N_THREADS (16)

#ifndef N_MESSAGES
#define N_MESSAGES 1000000
#endif

#include <stdlib.h>
#include <pthread.h>

#define L2ATOMIC_FIFO_NPACKETS			N_PACKETS
#define L2ATOMIC_FIFO_MSYNC_CONSUMER
#define L2ATOMIC_FIFO_PRODUCER_STORE_FAST
#define L2ATOMIC_FIFO_CONSUMER_MULTIPLE
#include "rdma/bgq/fi_bgq_l2atomic.h"

struct my_fifo {
	struct l2atomic_fifo_consumer 	consumer;
	uint64_t			pad_0[16];
	struct l2atomic_fifo_data 	data;
	uint64_t			element[N_PACKETS];
} __attribute__((__aligned__(32)));

struct global {
	struct my_fifo				fifo[N_THREADS];
	struct l2atomic_barrier_data		barrier_data;
	struct {
		struct l2atomic_fifo_producer	producer[N_THREADS];
		struct l2atomic_barrier		barrier;
	} worker[N_THREADS];
} __attribute__((__aligned__(32)));

struct results {
	double		mmps;
	uint16_t	n_consumers;
	uint16_t	n_producers;
	uint16_t	is_consumer;
	uint16_t	is_producer;
	uint64_t	count;
};

struct results result[N_THREADS][N_THREADS*N_THREADS];

struct global *global;

pthread_t info[N_THREADS];


static uint64_t producer_fn (uintptr_t pid, struct l2atomic_fifo_producer *producer, unsigned n_consumers) {

	uint64_t i, full_count = 0;
	do {
		/* choose fifo */
		const uint64_t f = (i+pid)%n_consumers;
		l2atomic_fifo_produce_wait(&producer[f], i);

	} while (++i<(n_consumers*N_MESSAGES));

	return full_count;
}

static uint64_t consumer_fn (uintptr_t pid, struct l2atomic_fifo_consumer *consumer, unsigned n_producers) {

	const unsigned t_messages = N_MESSAGES * n_producers;
	uint64_t data[32], empty_count = 0, i = 0;
	do {
		i += l2atomic_fifo_consume16(consumer, data);
	} while (i < t_messages);

	return empty_count;
}


static void * worker_fn (void * arg) {

	uintptr_t pid = (uintptr_t)arg;

	struct l2atomic_barrier *barrier = &global->worker[pid].barrier;
	struct l2atomic_fifo_consumer *consumer = &global->fifo[pid].consumer;
	struct l2atomic_fifo_producer *producer = &global->worker[pid].producer[0];

	unsigned n_consumers, n_producers, test = 0;
	for (n_consumers = 1; n_consumers <= N_THREADS; ++n_consumers) {

		const unsigned max_producers = N_THREADS - n_consumers;

		for (n_producers = 1; n_producers <= max_producers; ++n_producers) {

			uint64_t count = 0;

			l2atomic_barrier_enter(barrier);
			uint64_t start_time = GetTimeBase();

			if (pid < n_consumers) {
				count = consumer_fn(pid, consumer, n_producers);
			} else if (pid < (n_consumers + n_producers)) {
				count = producer_fn(pid, producer, n_consumers);
			}

			uint64_t end_time = GetTimeBase();
			l2atomic_barrier_enter(barrier);

			uint64_t total_time = end_time - start_time;
			double total_usec = (double)total_time / 1600.0;

			result[pid][test].n_consumers = n_consumers;
			result[pid][test].n_producers = n_producers;
			result[pid][test].count = count;

			if (pid < n_consumers) {
				result[pid][test].is_consumer = 1;
				result[pid][test].is_producer = 0;

				result[pid][test].mmps = ((double)(N_MESSAGES*n_producers)) / total_usec; /* same as million messages per second */
			} else if (pid < (n_producers+n_consumers)) {
				result[pid][test].is_consumer = 0;
				result[pid][test].is_producer = 1;

				result[pid][test].mmps = ((double)(N_MESSAGES*n_consumers)) / total_usec; /* same as million messages per second */
			} else {
				result[pid][test].is_consumer = 0;
				result[pid][test].is_producer = 0;
				result[pid][test].mmps = 0.0;
			}

			l2atomic_barrier_enter(barrier); ppc_msync();

			if (pid == 0) {
				char out[1024*10];
				char * ptr = &out[0];
				unsigned n;
				unsigned i;

				double all_consumer_mmps = 0.0;
				double all_producer_mmps = 0.0;
				for (i = 0; i < N_THREADS; ++i) {

					if (i>0 && result[i-1][test].is_consumer && !result[i][test].is_consumer) {
						n = sprintf(ptr, ":: "); ptr += n; 
					}

					if (result[i][test].is_consumer)
						all_consumer_mmps += result[i][test].mmps;

					if (result[i][test].is_producer)
						all_producer_mmps += result[i][test].mmps;
#ifdef DISPLAY_EMPTY_FULL_COUNT
					n = sprintf(ptr, "%5.2f(%5.2f) ", result[i][test].mmps, (double)result[i][test].count/10000000.0);
#else
					n = sprintf(ptr, "%5.2f ", result[i][test].mmps);
#endif
					ptr += n;
				}
				fprintf(stdout, "consumers=%-2u (%5.2f) producers=%-2u (%5.2f) :: %s\n", n_consumers, all_consumer_mmps, n_producers, all_producer_mmps, out);
			}

			++test;

			l2atomic_barrier_enter(barrier);
		}
	}

	return NULL;
}

int main (int argc, char *argv[]) {

	int lineno, retval = 0;

	const size_t bytes_to_allocate = 128 * 2 + sizeof(struct global);
	void *memptr = calloc(bytes_to_allocate, 1);
	global = (struct global *)(((uintptr_t)memptr + 128) & ~127);

	uintptr_t pid, w;
	for (pid = 0; pid < N_THREADS; ++pid) {
		l2atomic_fifo_initialize(&global->fifo[pid].consumer,
			&global->worker[0].producer[pid], &global->fifo[pid].data, N_PACKETS);

		for (w = 1; w < N_THREADS; ++w) {
			global->worker[w].producer[pid] = global->worker[0].producer[pid];
		}
	}

	l2atomic_barrier_initialize(&global->worker[0].barrier, &global->barrier_data, N_THREADS);

	for (pid=1; pid<N_THREADS; ++pid) {
		l2atomic_barrier_clone(&global->worker[pid].barrier, &global->barrier_data);
		if (pthread_create(&info[pid], NULL, &worker_fn, (void*)pid)) { lineno = __LINE__; goto err; }
	}

	worker_fn((void *)0);

ret:
	return retval;

err:
	fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, lineno); retval = -1;
	goto ret;

};
