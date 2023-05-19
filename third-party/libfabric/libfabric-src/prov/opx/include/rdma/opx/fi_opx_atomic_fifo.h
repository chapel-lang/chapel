/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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
#ifndef _FI_OPX_ATOMIC_FIFO_H_
#define _FI_OPX_ATOMIC_FIFO_H_


#include "rdma/opx/fi_opx_compiler.h"

#include <assert.h>

/*#define ATOMIC_CONSUMER*/

struct fi_opx_atomic_fifo {

	struct {
		uint64_t		head_seq;
		uint64_t		mask;
		volatile uint64_t *	data;
		uint64_t		unused[5];

	} consumer;

	struct {
		volatile uint64_t	tail_seq;	/* only updated by producers; not read by consumer */
		uint64_t		unused[7];
	} producer;

	struct {
		volatile uint64_t	bounds;		/* only updated by consumer; only read by producers */
		uint64_t		unused[7];
	} shared;

	/* not critical */

	uint64_t			size;
	void *				mem;
	volatile uint64_t		ref_cnt;

} __attribute__((__aligned__(64)));


struct fi_opx_atomic_fifo_producer {

	/* pointers to shared variables */
	volatile uint64_t *		bounds;		/* shared between producers and consumer; updated by consumer */
	volatile uint64_t *		tail_seq;	/* shared between producers; not read by consumer */

	/* private to each producer */
	uint64_t			bounds_cache;
	uint64_t			mask;
	volatile uint64_t *		data;

	/* not critical */
	struct fi_opx_atomic_fifo *	fifo;
};

static inline
void fi_opx_atomic_fifo_init (struct fi_opx_atomic_fifo * fifo, const uint64_t size) {

	switch (size) {
		case (0x01 << 2):
		case (0x01 << 3):
		case (0x01 << 4):
		case (0x01 << 5):
		case (0x01 << 6):
		case (0x01 << 7):
		case (0x01 << 8):
		case (0x01 << 9):
		case (0x01 << 10):
		case (0x01 << 11):
		case (0x01 << 12):
		case (0x01 << 13):
		case (0x01 << 14):
		case (0x01 << 15):
			break;
		default:
			fprintf(stderr, "%s:%s():%d fifo size not power-of-two\n", __FILE__, __func__, __LINE__);
			abort();
			break;
	}

	fifo->size = size;
	fifo->mem = malloc(sizeof(uint64_t) * size + 128);
	fifo->ref_cnt = 0;

	fifo->producer.tail_seq = 0;

	fifo->consumer.head_seq = 0;
	fifo->consumer.mask = size - 1;
	fifo->consumer.data = (uint64_t *)(((uintptr_t)fifo->mem + 128) & (~127));

	unsigned i;
	for (i=0; i<size; ++i)
		fifo->consumer.data[i] = 0;

	fifo->shared.bounds = size-1;


	return;
}


static inline
void fi_opx_atomic_fifo_fini (struct fi_opx_atomic_fifo * fifo) {

	if (fifo->ref_cnt != 0) {
		fprintf(stderr, "%s:%s():%d atomic fifo reference count not zero! (%lu)\n", __FILE__, __func__, __LINE__, fifo->ref_cnt);
		abort();
	}

	free(fifo->mem);
	fifo->mem = NULL;
	fifo->consumer.data = NULL;

	return;
}


static inline
void fi_opx_atomic_fifo_producer_init (struct fi_opx_atomic_fifo_producer * producer,
		struct fi_opx_atomic_fifo * fifo) {

	producer->bounds = &fifo->shared.bounds;
	producer->tail_seq = &fifo->producer.tail_seq;

	producer->bounds_cache = fifo->shared.bounds;
	producer->mask = fifo->consumer.mask;
	producer->data = fifo->consumer.data;
	producer->fifo = fifo;

	fi_opx_compiler_inc_u64(&fifo->ref_cnt);

	return;
}


static inline
void fi_opx_atomic_fifo_producer_fini (struct fi_opx_atomic_fifo_producer * producer) {

	fi_opx_compiler_dec_u64(&producer->fifo->ref_cnt);
	producer->fifo = NULL;

	return;
}



/* produce at the 'tail' */
static inline
void fi_opx_atomic_fifo_produce (struct fi_opx_atomic_fifo_producer * producer, const uint64_t data) {

	assert((data & 0x07u) == 0);

	const uint64_t tail_seq = fi_opx_compiler_fetch_and_inc_u64(producer->tail_seq);
	uint64_t bounds_cache = producer->bounds_cache;

	if (tail_seq >= bounds_cache) {

		volatile uint64_t * bounds = producer->bounds;
		do {
			bounds_cache = *bounds;
		} while (tail_seq >= bounds_cache);

		producer->bounds_cache = bounds_cache;
	}

	const uint64_t fifo_mask = producer->mask;
	volatile uint64_t * tail = producer->data + (tail_seq & fifo_mask);
	*tail = data | 0x01ul;

	/* write memory barrier ? */

	return;
}

static inline
void fi_opx_atomic_fifo_produce_unsafe (struct fi_opx_atomic_fifo_producer * producer, const uint64_t data) {

	assert((data & 0x07u) == 0);

	const uint64_t tail_seq = (*(producer->tail_seq))++;	/* unsafe! only for single-producer fifos */

	uint64_t bounds_cache = producer->bounds_cache;

	if (tail_seq >= bounds_cache) {

		volatile uint64_t * bounds = producer->bounds;
		do {
			bounds_cache = *bounds;
		} while (tail_seq >= bounds_cache);

		producer->bounds_cache = bounds_cache;
	}

	const uint64_t fifo_mask = producer->mask;
	volatile uint64_t * tail = producer->data + (tail_seq & fifo_mask);
	*tail = data | 0x01ul;

	/* write memory barrier ? */

	return;
}


/* consume at the 'head' */
static inline
unsigned fi_opx_atomic_fifo_consume (struct fi_opx_atomic_fifo * fifo, uint64_t * data) {

	const uint64_t fifo_mask = fifo->consumer.mask;
	const uint64_t head_seq = fifo->consumer.head_seq;
	volatile uint64_t * head = fifo->consumer.data + (head_seq & fifo_mask);

	/* read memory barrier ? */

	const uint64_t value = *head;

	if (value & 0x01ul) {

		*head = 0;

		/* write memory barrier ? */

		fifo->consumer.head_seq = head_seq + 1;
#ifdef ATOMIC_CONSUMER
		fi_opx_compiler_inc_u64(&fifo->shared.bounds);
#else
		++(fifo->shared.bounds);
#endif

		*data = value & 0xFFFFFFFFFFFFFFF8ul;
		return 0;
	}

	return 1;
}


static inline
void fi_opx_atomic_fifo_consume_wait (struct fi_opx_atomic_fifo * fifo, uint64_t * data) {

	const uint64_t fifo_mask = fifo->consumer.mask;
	const uint64_t head_seq = fifo->consumer.head_seq;
	volatile uint64_t * head = fifo->consumer.data + (head_seq & fifo_mask);

	uint64_t value = *head;
	while ((value & 0x01ul) == 0) {
		value = *head;		/* read memory barrier ? */
	}

	*head = 0;

	/* write memory barrier ? */

	fifo->consumer.head_seq = head_seq + 1;
#ifdef ATOMIC_CONSUMER
	fi_opx_compiler_inc_u64(&fifo->shared.bounds);
#else
	++(fifo->shared.bounds);
#endif

	*data = value & 0xFFFFFFFFFFFFFFF8ul;
	return;
}


#endif /* _FI_OPX_ATOMIC_FIFO_H_ */
