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

#ifndef _FI_PROV_BGQ_L2ATOMIC_H_
#define _FI_PROV_BGQ_L2ATOMIC_H_


#include <unistd.h>

#include "rdma/bgq/fi_bgq_hwi.h"
#include "rdma/bgq/fi_bgq_spi.h"

/*
 * l2atomic lock access structure
 */
struct l2atomic_lock {
	uintptr_t	ticket_l2vaddr;
	uintptr_t	serving_l2vaddr;
};

/*
 * l2atomic lock data structure
 */
struct l2atomic_lock_data {
	volatile uint64_t	ticket;
	volatile uint64_t	serving;
} __attribute((aligned(32)));

static inline
void l2atomic_lock_initialize (struct l2atomic_lock * lock, struct l2atomic_lock_data * data) {

	uint32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_L2AtomicsAllocate((void*)data, sizeof(struct l2atomic_lock_data));
	assert(0==cnk_rc);

	lock->ticket_l2vaddr = (uintptr_t)&data->ticket;
	lock->serving_l2vaddr = (uintptr_t)&data->serving;

	L2_AtomicStore((volatile uint64_t *)lock->ticket_l2vaddr, 0);
	L2_AtomicStore((volatile uint64_t *)lock->serving_l2vaddr, 0);
};

static inline
uint64_t l2atomic_lock_acquire (struct l2atomic_lock * lock) {

	const uint64_t ticket = L2_AtomicLoadIncrement((volatile uint64_t *)lock->ticket_l2vaddr);
	while (L2_AtomicLoad((volatile uint64_t *)lock->serving_l2vaddr) != ticket);

	return ticket;
};

static inline
void l2atomic_lock_release (struct l2atomic_lock * lock) {
	L2_AtomicStoreAdd((volatile uint64_t *)lock->serving_l2vaddr, 1);
};

static inline
uint64_t l2atomic_lock_depth (struct l2atomic_lock * lock) {
	return L2_AtomicLoad((volatile uint64_t *)lock->ticket_l2vaddr) -
		L2_AtomicLoad((volatile uint64_t *)lock->serving_l2vaddr);
};

static inline
uint64_t l2atomic_lock_isbusy (struct l2atomic_lock * lock) {
	return (l2atomic_lock_depth(lock) != 0);
};


#define L2ATOMIC_FIFO_CONSUMER_CONTENTION_LEVEL_NONE	(0)
#define L2ATOMIC_FIFO_CONSUMER_CONTENTION_LEVEL_LOW	(1)
#define L2ATOMIC_FIFO_CONSUMER_CONTENTION_LEVEL_HIGH	(2)

#ifndef L2ATOMIC_FIFO_CONSUMER_CONTENTION_LEVEL
#define L2ATOMIC_FIFO_CONSUMER_CONTENTION_LEVEL		(L2ATOMIC_FIFO_CONSUMER_CONTENTION_LEVEL_HIGH)
#endif

#define L2ATOMIC_FIFO_CONSUMER_ALGORITHM_L2BOUNDED	(0)
#define L2ATOMIC_FIFO_CONSUMER_ALGORITHM_STBOUNDED	(1)

#ifndef L2ATOMIC_FIFO_CONSUMER_ALGORITHM
#define L2ATOMIC_FIFO_CONSUMER_ALGORITHM		(L2ATOMIC_FIFO_CONSUMER_ALGORITHM_L2BOUNDED)
#endif

// Change this default to L2ATOMIC_FIFO_CONSUMER_MULTIPLE ??
#ifndef L2ATOMIC_FIFO_CONSUMER_SINGLE
#ifndef L2ATOMIC_FIFO_CONSUMER_MULTIPLE
#define L2ATOMIC_FIFO_CONSUMER_SINGLE
#endif
#endif

#define L2ATOMIC_FIFO_PRODUCER_ALGORITHM_L2BOUNDED	(0)
#define L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE	(1)

#ifndef L2ATOMIC_FIFO_PRODUCER_ALGORITHM
#define L2ATOMIC_FIFO_PRODUCER_ALGORITHM		(L2ATOMIC_FIFO_PRODUCER_ALGORITHM_L2BOUNDED)
#endif

#define L2_CACHE_LINE_COUNT_UINT64			(L2_CACHE_LINE_SIZE >> 2)

// Change this default to L2ATOMIC_FIFO_MSYNC_PRODUCER ??
#ifndef L2ATOMIC_FIFO_MSYNC_CONSUMER
#ifndef L2ATOMIC_FIFO_MSYNC_PRODUCER
#define L2ATOMIC_FIFO_MSYNC_PRODUCER
#endif
#endif

// Change this default to L2ATOMIC_FIFO_PRODUCER_STORE_FAST ??
#ifndef L2ATOMIC_FIFO_PRODUCER_STORE_FAST
#ifndef L2ATOMIC_FIFO_PRODUCER_STORE_ATOMIC
#define L2ATOMIC_FIFO_PRODUCER_STORE_ATOMIC
#endif
#endif

#ifndef L2ATOMIC_FIFO_CONSUMER_CLEAR_FAST
#ifndef L2ATOMIC_FIFO_CONSUMER_CLEAR_ATOMIC
#define L2ATOMIC_FIFO_CONSUMER_CLEAR_ATOMIC
#endif
#endif

#ifndef L2ATOMIC_FIFO_BOUNDS_FAST
#ifndef L2ATOMIC_FIFO_BOUNDS_ATOMIC
#define L2ATOMIC_FIFO_BOUNDS_ATOMIC
#endif
#endif


/*
 * l2atomic fifo access structures
 */
struct l2atomic_fifo_consumer {
	uint64_t	head;
	uintptr_t	bounds_l2vaddr;
	uint64_t	mask;
	uintptr_t	packet_base_l2vaddr;
};
struct l2atomic_fifo_producer {
	uint64_t	mask;
	uintptr_t	packet_base_l2vaddr;
	uintptr_t	tail_l2vaddr;
#if L2ATOMIC_FIFO_PRODUCER_ALGORITHM == L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE
	uintptr_t	bounds_l2vaddr;
#ifdef L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE_CACHEBOUNDS
	uint64_t	local_bounds;
#endif
#endif
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE)));
struct l2atomic_fifo {
	struct l2atomic_fifo_consumer	consumer;
	struct l2atomic_fifo_producer	producer;
};

/*
 * l2atomic fifo data structure
 */
#if L2ATOMIC_FIFO_PRODUCER_ALGORITHM == L2ATOMIC_FIFO_PRODUCER_ALGORITHM_L2BOUNDED
struct l2atomic_fifo_data {
	volatile uint64_t	tail;
	volatile uint64_t	bounds;
	
	volatile uint64_t	packet[0];
} __attribute((aligned(32)));
#elif L2ATOMIC_FIFO_PRODUCER_ALGORITHM == L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE
struct l2atomic_fifo_data {
	volatile uint64_t	tail __attribute__((__aligned__(L2_CACHE_LINE_SIZE)));		/* producer rw, consumer na */
	uint64_t		pad0[(L2_CACHE_LINE_SIZE-sizeof(uint64_t))/sizeof(uint64_t)];

	volatile uint64_t	bounds __attribute__((__aligned__(L2_CACHE_LINE_SIZE)));	/* producer ro, consumer rw */
	uint64_t		pad1[(L2_CACHE_LINE_SIZE-sizeof(uint64_t))/sizeof(uint64_t)];
	
	volatile uint64_t	packet[0];							/* producer wo, consumer rw */

	uint64_t		pad2[L2_CACHE_LINE_COUNT_UINT64<<1];
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE)));
#endif



static inline
void l2atomic_fifo_enable (struct l2atomic_fifo_consumer * consumer,
		struct l2atomic_fifo_producer * producer) {

	uint64_t npackets = producer->mask+1;
	unsigned n;
	for (n = 0; n < npackets; ++n)
		L2_AtomicStore(&((uint64_t*)producer->packet_base_l2vaddr)[n], 0);

	consumer->head = 0;
	L2_AtomicStore((void *)producer->tail_l2vaddr, 0);
	L2_AtomicStore((void *)consumer->bounds_l2vaddr, npackets-L2_CACHE_LINE_COUNT_UINT64-1);
}

static inline
void l2atomic_fifo_disable (struct l2atomic_fifo_consumer * consumer,
		struct l2atomic_fifo_producer * producer) {

	L2_AtomicStore((void *)consumer->bounds_l2vaddr, 0);

}


static inline
void l2atomic_fifo_initialize (struct l2atomic_fifo_consumer * consumer,
	struct l2atomic_fifo_producer * producer,
	struct l2atomic_fifo_data * data,
	uint64_t npackets) {

	assert(consumer);
	assert(producer);
	assert(data);
	assert(((uintptr_t)data & 0x01F) == 0);	/* 32 byte aligned */
#ifdef L2ATOMIC_FIFO_NPACKETS
	npackets = L2ATOMIC_FIFO_NPACKETS;
#endif
	assert(npackets >= (1 << 8));		/* 256 -- see consume16() */

	assert((npackets == (1 << 3)) ||	/* 8 */
		(npackets == (1 << 4)) ||	/* 16 */
		(npackets == (1 << 5)) ||	/* 32 */
		(npackets == (1 << 6)) ||	/* 64 */
		(npackets == (1 << 7)) ||	/* 128 */
		(npackets == (1 << 8)) ||	/* 256 */
		(npackets == (1 << 9)) ||	/* 512 */
		(npackets == (1 << 10)) ||	/* 1k */
		(npackets == (1 << 11)) ||	/* 2k */
		(npackets == (1 << 12)) ||	/* 4k */
		(npackets == (1 << 13)) ||	/* 8k */
		(npackets == (1 << 14)) ||	/* 16k */
		(npackets == (1 << 15)) ||	/* 32k */
		(npackets == (1 << 16)) ||	/* 64k */
		(npackets == (1 << 17)) ||	/* 128k */
		(npackets == (1 << 18)) ||	/* 256k */
		(npackets == (1 << 19)));	/* 512k */

	consumer->mask = npackets-1;
	consumer->bounds_l2vaddr = (uintptr_t)&data->bounds;
	consumer->packet_base_l2vaddr = (uintptr_t)&data->packet[0];

	producer->mask = npackets-1;
	producer->tail_l2vaddr = (uintptr_t)&data->tail;
	producer->packet_base_l2vaddr = (uintptr_t)&data->packet[0];
#if L2ATOMIC_FIFO_PRODUCER_ALGORITHM == L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE
	producer->bounds_l2vaddr = (uintptr_t)&data->bounds;
#ifdef L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE_CACHEBOUNDS
	producer->local_bounds = npackets-L2_CACHE_LINE_COUNT_UINT64-1;
#endif
#endif

	uint32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_L2AtomicsAllocate((void*)data, sizeof(struct l2atomic_fifo_data) + sizeof(uint64_t) * npackets);
	assert(0==cnk_rc);

	l2atomic_fifo_enable(consumer, producer);

	return;
}

static inline
int l2atomic_fifo_produce (struct l2atomic_fifo_producer * fifo, const uint64_t data) {

#if L2ATOMIC_FIFO_PRODUCER_ALGORITHM != L2ATOMIC_FIFO_PRODUCER_ALGORITHM_L2BOUNDED
	assert(0);
#endif

	const uint64_t tail = L2_AtomicLoadIncrementBounded((volatile uint64_t *)fifo->tail_l2vaddr);
	if (tail != 0x8000000000000000ull) {
#ifdef L2ATOMIC_FIFO_NPACKETS
		const uint64_t mask = L2ATOMIC_FIFO_NPACKETS-1;
#else
		const uint64_t mask = fifo->mask;
#endif
		const uint64_t offset = (tail & mask) << 0x03ull;
#ifdef L2ATOMIC_FIFO_PRODUCER_STORE_FAST
		volatile uint64_t *pkt = (volatile uint64_t *)(fifo->packet_base_l2vaddr + offset);
		*pkt = 0x8000000000000000ull | data;
#endif
		{	/* this "l1p flush" hack is only needed to flush *writes* from a processor cache to the memory system */
			volatile uint64_t *mu_register =
				(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
				0x030 - PHYMAP_PRIVILEGEDOFFSET);
			*mu_register = 0;
		}
#ifdef L2ATOMIC_FIFO_MSYNC_PRODUCER
		ppc_msync();
#endif
#ifdef L2ATOMIC_FIFO_PRODUCER_STORE_ATOMIC
		L2_AtomicStore((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset),
			0x8000000000000000ull | data);
#endif
		return 0;
	}

	return -1;
}

static inline
int l2atomic_fifo_produce_wait (struct l2atomic_fifo_producer * fifo, const uint64_t data) {

#if L2ATOMIC_FIFO_PRODUCER_ALGORITHM == L2ATOMIC_FIFO_PRODUCER_ALGORITHM_L2BOUNDED
	while (0 != l2atomic_fifo_produce(fifo, data));
	return 0;


#elif L2ATOMIC_FIFO_PRODUCER_ALGORITHM == L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE
	const uint64_t tail = L2_AtomicLoadIncrement((volatile uint64_t *)fifo->tail_l2vaddr);
	uint64_t bounds = 0;
	volatile uint64_t * const bounds_l2vaddr = (volatile uint64_t * const)fifo->bounds_l2vaddr;
#ifdef L2ATOMIC_FIFO_NPACKETS
	const uint64_t mask = L2ATOMIC_FIFO_NPACKETS-1;
#else
	const uint64_t mask = fifo->mask;
#endif
#ifdef L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE_CACHEBOUNDS
	const uint64_t local_bounds = fifo->local_bounds;
	if (local_bounds < tail) {
		const uint64_t offset = (tail & mask) << 0x03ull;
#ifdef L2ATOMIC_FIFO_PRODUCER_STORE_FAST
		uint64_t *pkt = (volatile uint64_t *)(fifo->packet_base_l2vaddr + offset);
		*pkt = 0x8000000000000000ull | data;
#endif
		{	/* this "l1p flush" hack is only needed to flush *writes* from a processor cache to the memory system */
			volatile uint64_t *mu_register =
				(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
				0x030 - PHYMAP_PRIVILEGEDOFFSET);
			*mu_register = 0;
		}
#ifdef L2ATOMIC_FIFO_MSYNC_PRODUCER
		ppc_msync();
#endif
#ifdef L2ATOMIC_FIFO_PRODUCER_STORE_ATOMIC
		L2_AtomicStore((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset),
#endif			0x8000000000000000ull | data);
	} else {

#endif

#ifdef L2ATOMIC_FIFO_PRODUCER_CHECK_BOUNDS_FAST
	while ((bounds = *bounds_l2vaddr) < tail);
#endif
#ifdef L2ATOMIC_FIFO_PRODUCER_CHECK_BOUNDS_ATOMIC
	while ((bounds = L2_AtomicLoad((volatile uint64_t *)bounds_l2vaddr)) < tail);
#endif

	const uint64_t offset = (tail & mask) << 0x03ull;
#ifdef L2ATOMIC_FIFO_PRODUCER_STORE_FAST
	uint64_t *pkt = (volatile uint64_t *)(fifo->packet_base_l2vaddr + offset);
	*pkt = 0x8000000000000000ull | data;
#endif
	{	/* this "l1p flush" hack is only needed to flush *writes* from a processor cache to the memory system */
		volatile uint64_t *mu_register =
			(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
			0x030 - PHYMAP_PRIVILEGEDOFFSET);
		*mu_register = 0;
	}
#ifdef L2ATOMIC_FIFO_MSYNC_PRODUCER
	ppc_msync();
#endif
#ifdef L2ATOMIC_FIFO_PRODUCER_STORE_ATOMIC
	L2_AtomicStore((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset),
		0x8000000000000000ull | data);
#endif
#ifdef L2ATOMIC_FIFO_PRODUCER_ALGORITHM_RESERVE_CACHEBOUNDS
	fifo->local_bounds = bounds;
	}
#endif
	return 0;
#endif
}

static inline
int l2atomic_fifo_consume (struct l2atomic_fifo_consumer * fifo, uint64_t * data) {

#if L2ATOMIC_FIFO_CONSUMER_ALGORITHM == L2ATOMIC_FIFO_CONSUMER_ALGORITHM_STBOUNDED
	uint64_t * bounds_l2vaddr = (uint64_t *)fifo->bounds_l2vaddr;
	const uint64_t bounds = *bounds_l2vaddr;
#endif
	const uint64_t head = fifo->head;
#ifdef L2ATOMIC_FIFO_NPACKETS
	const uint64_t mask = L2ATOMIC_FIFO_NPACKETS-1;
#else
	const uint64_t mask = fifo->mask;
#endif
	const uint64_t offset = (head & mask) << 0x03ull;
#ifdef L2ATOMIC_FIFO_CONSUMER_CLEAR_FAST
	volatile uint64_t *pkt = (volatile uint64_t *)(fifo->packet_base_l2vaddr + offset);
	const uint64_t value = *pkt;
#else
#ifdef L2ATOMIC_FIFO_CONSUMER_CLEAR_ATOMIC
	const uint64_t value = L2_AtomicLoadClear((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset));
#endif
#endif
	if (value & 0x8000000000000000ull) {
#ifdef L2ATOMIC_FIFO_CONSUMER_CLEAR_FAST
		*pkt = 0;
#endif
		*data = value & (~0x8000000000000000ull);
		fifo->head = head + 1;
#if L2ATOMIC_FIFO_CONSUMER_ALGORITHM == L2ATOMIC_FIFO_CONSUMER_ALGORITHM_L2BOUNDED
		L2_AtomicStoreAdd((volatile uint64_t *)fifo->bounds_l2vaddr, 1);
#elif L2ATOMIC_FIFO_CONSUMER_ALGORITHM == L2ATOMIC_FIFO_CONSUMER_ALGORITHM_STBOUNDED
		*bounds_l2vaddr = bounds + 1;
#endif
#ifdef L2ATOMIC_FIFO_MSYNC_CONSUMER
		ppc_msync();
#endif
		return 0;
	}

	return -1;
}

static inline
unsigned l2atomic_fifo_consume16 (struct l2atomic_fifo_consumer * fifo, uint64_t * data) {

#ifdef L2ATOMIC_FIFO_CONSUMER_SINGLE
	return (0 == l2atomic_fifo_consume(fifo, data));

#else
#ifdef L2ATOMIC_FIFO_CONSUMER_MULTIPLE
	const uint64_t head_counter = fifo->head;
#ifdef L2ATOMIC_FIFO_NPACKETS
	const uint64_t fifo_size = L2ATOMIC_FIFO_NPACKETS;
	const uint64_t mask = L2ATOMIC_FIFO_NPACKETS-1;
#else
	const uint64_t mask = fifo->mask;
#endif
	const uint64_t head_offset = head_counter & mask;
	const uint64_t end_offset = head_offset + L2_CACHE_LINE_COUNT_UINT64;
	const uint64_t count = L2_CACHE_LINE_COUNT_UINT64 - (((~mask) & end_offset) * (end_offset - fifo_size));

	volatile uint64_t *ptr = (volatile uint64_t *)(fifo->packet_base_l2vaddr + (head_offset << 0x03ull));

	uint64_t i, num_processed = 0;
#ifdef DO_CACHE
	uint64_t cache[L2_CACHE_LINE_COUNT_UINT64*2];
	ppc_msync();
	for (i = 0; i < count; ++i) {
		cache[i] = L2_AtomicLoad(ptr + i);
	}
#endif
	for (i = 0; i < count; ++i) {
#ifdef DO_CACHE
		if (cache[i] & 0x8000000000000000ull) {
			data[i] = cache[i] & (~0x8000000000000000ull);
			++num_processed;
			L2_AtomicStore(ptr + i, 0);
		} else {
			break;
		}
#else
		const uint64_t value = ptr[i];
		//const uint64_t value = L2_AtomicLoadClear(ptr + i);
		if (value & 0x8000000000000000ull) {
			data[i] = value & (~0x8000000000000000ull);
			++num_processed;
			ptr[i] = 0;
		} else {
			break;
		}
#endif

	}

	fifo->head += num_processed;
#ifdef L2ATOMIC_FIFO_BOUNDS_ATOMIC
	L2_AtomicStoreAdd((volatile uint64_t *)fifo->bounds_l2vaddr, num_processed);
#else
#ifdef L2ATOMIC_FIFO_BOUNDS_FAST
	uint64_t *bounds = (uint64_t *)fifo->bounds_l2vaddr;
	++(*bounds);
#endif
#endif
#ifdef L2ATOMIC_FIFO_MSYNC_CONSUMER
	ppc_msync();
#endif

	return num_processed;
#endif
#endif
}

static inline
int l2atomic_fifo_drain (struct l2atomic_fifo_consumer * consumer,
		struct l2atomic_fifo_producer * producer, uint64_t * data) {

	/* The fifo must be disabled before it can be drained */
	assert(0 == L2_AtomicLoad((volatile uint64_t *)consumer->bounds_l2vaddr));

	const uint64_t head = consumer->head;
	const uint64_t tail = L2_AtomicLoad((void *)producer->tail_l2vaddr);

	if (head == tail) {
		/* The fifo is empty */
		return -1;
	}

#ifdef L2ATOMIC_FIFO_NPACKETS
	const uint64_t mask = L2ATOMIC_FIFO_NPACKETS-1;
#else
	const uint64_t mask = consumer->mask;
#endif
	const uint64_t offset = (head & mask) << 0x03ull;

	/* Spin until the next packet is ready */
	uint64_t value = 0;
	volatile uint64_t *ptr = (volatile uint64_t *)(consumer->packet_base_l2vaddr + offset);
	while (0 == (0x8000000000000000ull & (value = L2_AtomicLoadClear(ptr))));

	*data = value & (~0x8000000000000000ull);
	consumer->head = head + 1;

#ifdef L2ATOMIC_FIFO_MSYNC_CONSUMER
	ppc_msync();
#endif
	return 0;
}



static inline
int l2atomic_fifo_peek (struct l2atomic_fifo_consumer * fifo, uint64_t * data) {

	const uint64_t head = fifo->head;
	const uint64_t mask = fifo->mask;
	const uint64_t offset = (head & mask) << 0x03ull;
	const uint64_t value = L2_AtomicLoad((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset));
	if (value & 0x8000000000000000ull) {
		*data = value & (~0x8000000000000000ull);
		return 0;
	}

	return -1;
}

static inline
void l2atomic_fifo_advance (struct l2atomic_fifo_consumer * fifo) {

	const uint64_t head = fifo->head;
	const uint64_t mask = fifo->mask;
	const uint64_t offset = (head & mask) << 0x03ull;
	L2_AtomicLoadClear((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset));
	fifo->head = head + 1;
	L2_AtomicStoreAdd((volatile uint64_t *)fifo->bounds_l2vaddr, 1);

	return;
}

static inline
unsigned l2atomic_fifo_isempty (struct l2atomic_fifo_consumer * fifo) {

	const uint64_t head = fifo->head;
	const uint64_t mask = fifo->mask;
	const uint64_t offset = (head & mask) << 0x03ull;
	const uint64_t value = L2_AtomicLoad((volatile uint64_t *)(fifo->packet_base_l2vaddr + offset));

	return (value & 0x8000000000000000ull) == 0;
}


/*
 * l2atomic counter data structure
 */
struct l2atomic_counter_data {
	volatile uint64_t	value;
} __attribute((aligned(8)));

/*
 * l2atomic counter access structure
 */
struct l2atomic_counter {
	uintptr_t		value_l2vaddr;
};

static inline
void l2atomic_counter_initialize (struct l2atomic_counter * counter,
	struct l2atomic_counter_data * data) {

	assert(counter);
	assert(data);
	assert(((uintptr_t)data & 0x07) == 0);	/* 8 byte aligned */

	uint32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_L2AtomicsAllocate((void*)data, sizeof(struct l2atomic_counter_data));
	assert(0==cnk_rc);

	counter->value_l2vaddr = (uintptr_t)&data->value;

	L2_AtomicStore(&data->value, 0);
};

static inline
uint64_t l2atomic_counter_increment (struct l2atomic_counter * counter) {
	return L2_AtomicLoadIncrement((volatile uint64_t *)(counter->value_l2vaddr));
};

static inline
uint64_t l2atomic_counter_decrement (struct l2atomic_counter * counter) {
	return L2_AtomicLoadDecrement((volatile uint64_t *)(counter->value_l2vaddr));
};

static inline
uint64_t l2atomic_counter_get (struct l2atomic_counter * counter) {
	return L2_AtomicLoad((volatile uint64_t *)(counter->value_l2vaddr));
}

static inline
void l2atomic_counter_set (struct l2atomic_counter * counter, uint64_t new_value) {
	L2_AtomicStore((volatile uint64_t *)(counter->value_l2vaddr), new_value);
};

static inline
void l2atomic_counter_add (struct l2atomic_counter * counter, uint64_t add_value) {
	L2_AtomicStoreAdd((volatile uint64_t *)(counter->value_l2vaddr), add_value);
};

/*
 * l2atomic bounded counter data structure
 */
struct l2atomic_boundedcounter_data {
	volatile uint64_t	value;
	volatile uint64_t	bounds;
} __attribute((aligned(32)));

/*
 * l2atomic bounded counter access structure
 */
struct l2atomic_boundedcounter {
	uintptr_t		value_l2vaddr;
	uintptr_t		bounds_l2vaddr;
};

static inline
void l2atomic_boundedcounter_initialize (struct l2atomic_boundedcounter * counter,
	struct l2atomic_boundedcounter_data * data,
	uint64_t initial_bounds) {

	assert(counter);
	assert(data);
	assert(((uintptr_t)data & 0x01F) == 0);	/* 32 byte aligned */

	uint32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_L2AtomicsAllocate((void*)data, sizeof(struct l2atomic_boundedcounter_data));
	assert(0==cnk_rc);

	counter->value_l2vaddr = (uintptr_t)&data->value;
	counter->bounds_l2vaddr = (uintptr_t)&data->bounds;

	L2_AtomicStore(&data->value, 0);
	L2_AtomicStore(&data->bounds, initial_bounds);
};

static inline
uint64_t l2atomic_boundedcounter_increment_value (struct l2atomic_boundedcounter * counter) {
	return L2_AtomicLoadIncrementBounded((volatile uint64_t *)(counter->value_l2vaddr));
};

static inline
void l2atomic_boundedcounter_add_bounds (struct l2atomic_boundedcounter * counter, uint64_t add_value) {
	L2_AtomicStoreAdd((volatile uint64_t *)(counter->bounds_l2vaddr), add_value);
};

/*
 * l2atomic barrier data structure
 */
struct l2atomic_barrier_data {
	volatile __attribute__((aligned(L1D_CACHE_LINE_SIZE))) uint64_t	start;
	uint64_t							participants;
	volatile __attribute__((aligned(L1D_CACHE_LINE_SIZE))) uint64_t	count;
} __attribute__((aligned(L1D_CACHE_LINE_SIZE)));

/*
 * l2atomic barrier access structure
 */
struct l2atomic_barrier {
	uintptr_t	start_l2vaddr;
	uintptr_t	count_l2vaddr;
	uint64_t	participants;
};

static inline
void l2atomic_barrier_initialize (struct l2atomic_barrier * barrier,
		struct l2atomic_barrier_data * data,
		uint64_t participants) {

	assert(barrier);
	assert(data);
	assert(((uintptr_t)data & (L1D_CACHE_LINE_SIZE-1)) == 0);

	uint32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_L2AtomicsAllocate((void*)data, sizeof(struct l2atomic_barrier_data));
	assert(0==cnk_rc);

	barrier->start_l2vaddr = (uintptr_t)&data->start;
	barrier->count_l2vaddr = (uintptr_t)&data->count;
	barrier->participants = participants;

	data->participants = participants;
	L2_AtomicStore(&data->start, 0);
	L2_AtomicStore(&data->count, 0);
}

static inline
void l2atomic_barrier_clone (struct l2atomic_barrier * barrier,
		struct l2atomic_barrier_data * data) {

	assert(barrier);
	assert(data);
	assert(((uintptr_t)data & (L1D_CACHE_LINE_SIZE-1)) == 0);

	barrier->start_l2vaddr = (uintptr_t)&data->start;
	barrier->count_l2vaddr = (uintptr_t)&data->count;
	barrier->participants = data->participants;
}

static inline
void l2atomic_barrier_enter (struct l2atomic_barrier * barrier) {

	volatile uint64_t * start_l2vaddr = (volatile uint64_t *)(barrier->start_l2vaddr);

	const uint64_t start = L2_AtomicLoad(start_l2vaddr);
	const uint64_t current = L2_AtomicLoadIncrement((volatile uint64_t *)(barrier->count_l2vaddr)) + 1;
	const uint64_t target = start + barrier->participants;

	if (current == target) {
		L2_AtomicStoreAdd(start_l2vaddr, barrier->participants);
	} else {
		while (L2_AtomicLoad(start_l2vaddr) < current);
	}
}


#endif /* _FI_PROV_BGQ_L2ATOMIC_H_ */
