/* SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note */
/*
 * Kernel Libfabric PCIe Provider (KLPP)
 *
 * Copyright (c) 2018-2024 GigaIO, Inc.
 */

#ifndef __KLPP_SHARED_RINGBUF_H_
#define __KLPP_SHARED_RINGBUF_H_

/* Ignore this macro in userland. */
#define BUG_ON(x)
#include <stdint.h>
#include <stdbool.h>

#ifndef READ_ONCE
#define READ_ONCE(x) (*(const __volatile__ __typeof__(x) *)&(x))
#endif

#ifndef WRITE_ONCE
#define WRITE_ONCE(x, val)				\
do {							\
	*(__volatile__ __typeof__(x) *)&(x) = (val);	\
} while (0)
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((__aligned__(x)))
#endif

#define cmpxchg(ptr, old_val, new_val) __sync_val_compare_and_swap(ptr, old_val, new_val)

#ifndef barrier
#define barrier __sync_synchronize
#endif


/*
 * This is a circular buffer implementation suitable for use in user/kernel
 * shared memory. Its size must be a power of two.
 *
 * This is a multiple producer, single consumer, lock-free implementation.
 * A single dequeue and multiple enqueues may happen concurrently.
 * Consumers must protect the dequeue operation with a lock.
 */
struct klpp_ringbuf_header {
	uint32_t producer;
	uint32_t consumer;
} __attribute__((packed, aligned(4)));

/*
 * NAME is appended to all function names. The ringbuf stores entries of
 * ENTRY_TYPE. Each slot in the ringbuf is aligned to ALIGNMENT bytes.
 */
#define KLPP_RINGBUF_SLOT(NAME, TYPE, ALIGNMENT)		\
struct klpp_ringbuf_slot_##NAME {				\
	TYPE entry;						\
	uint8_t valid;						\
	uint8_t	pad[ALIGNMENT - ((sizeof(TYPE) + sizeof(uint8_t)) % ALIGNMENT)]; \
} __attribute__((packed));

#define KLPP_RINGBUF_STRUCT(NAME)			\
struct klpp_ringbuf_##NAME  {				\
	struct klpp_ringbuf_header header;		\
	struct klpp_ringbuf_slot_##NAME slots[0];	\
} __attribute__((packed, aligned(4)));

#define KLPP_RINGBUF_SIZE(NAME)					\
static __inline__ size_t klpp_ringbuf_bytes_##NAME(size_t nentries)	\
{								\
	return sizeof(struct klpp_ringbuf_##NAME) +		\
	sizeof(struct klpp_ringbuf_slot_##NAME) * nentries;	\
}

#define KLPP_RINGBUF_INIT(NAME)					\
static __inline__ void						\
klpp_ringbuf_init_##NAME(struct klpp_ringbuf_##NAME *ringbuf, size_t nentries) \
{								\
	size_t i;						\
								\
	BUG_ON(nentries != roundup_pow_of_two(nentries));	\
	WRITE_ONCE(ringbuf->header.producer, 0);		\
	WRITE_ONCE(ringbuf->header.consumer, 0);		\
	for (i = 0; i < nentries; i++)				\
		WRITE_ONCE(ringbuf->slots[i].valid, 0);		\
}

#define KLPP_RINGBUF_EMPTY(NAME)				\
static __inline__ int						\
klpp_ringbuf_empty_##NAME(struct klpp_ringbuf_##NAME *ringbuf,	\
			  size_t nentries)			\
{								\
	struct klpp_ringbuf_slot_##NAME *slot;			\
								\
	barrier();						\
	slot = &ringbuf->slots[ringbuf->header.consumer % nentries]; \
	return !READ_ONCE(slot->valid);				\
}

#define KLPP_RINGBUF_NUM_FREE(NAME)				\
static __inline__ int						\
klpp_ringbuf_num_free_##NAME(struct klpp_ringbuf_##NAME *ringbuf, \
			     size_t nentries)			\
{								\
	struct klpp_ringbuf_header header;			\
								\
	header = READ_ONCE(ringbuf->header);			\
								\
	if (header.producer > header.consumer)			\
		return nentries - (header.producer - header.consumer); \
	else if (header.producer < header.consumer)		\
		return (header.consumer % nentries) - header.producer; \
								\
	return nentries;					\
}

/* Returns 1 on failure, 0 on success.
 * Multiple concurrent producers may run this function without locking.
 */
#define KLPP_RINGBUF_ENQUEUE(NAME, TYPE)			\
static __inline__ int						\
klpp_ringbuf_enqueue_##NAME(struct klpp_ringbuf_##NAME *ringbuf,\
			    size_t nentries, TYPE *entry)	\
{								\
	struct klpp_ringbuf_header header;			\
	struct klpp_ringbuf_slot_##NAME *slot;			\
	uint32_t new_producer;					\
								\
	while (true) {						\
		header = READ_ONCE(ringbuf->header);		\
		if (header.producer == header.consumer ||	\
		    (header.producer % nentries != header.consumer % nentries)) { \
			new_producer = header.producer + 1;	\
								\
			if (cmpxchg(&ringbuf->header.producer,	\
				    header.producer, new_producer) == header.producer) { \
				break;				\
			}					\
		} else {					\
			/* Ring buffer is full. */		\
			return 1;				\
		}						\
	}							\
	slot = &ringbuf->slots[header.producer % nentries];	\
	slot->entry = *entry;					\
	WRITE_ONCE(slot->valid, 1);				\
	barrier();						\
	return 0;						\
}

/* Returns 1 on empty queue, 0 on success. "entry" will be updated to the
 * entry at the head of the queue on success.
 */
#define KLPP_RINGBUF_PICK(NAME, TYPE)				\
static __inline__ int						\
klpp_ringbuf_peek_##NAME(struct klpp_ringbuf_##NAME *ringbuf,	\
			 size_t nentries, TYPE **entry)		\
{								\
	struct klpp_ringbuf_slot_##NAME *slot;			\
								\
	barrier();						\
	slot = &ringbuf->slots[ringbuf->header.consumer % nentries]; \
								\
	if (READ_ONCE(slot->valid) == 0)			\
		return 1;					\
								\
	*entry = &slot->entry;					\
	return 0;						\
}

/* Returns 1 on failure, 0 on success.
 * Multiple concurrent consumers must protect calls to this function with a lock.
 */
#define KLPP_RINGBUF_DEQUEUE(NAME, TYPE)			\
static __inline__ int						\
klpp_ringbuf_dequeue_##NAME(struct klpp_ringbuf_##NAME *ringbuf,\
			    size_t nentries, TYPE *entry)	\
{								\
	struct klpp_ringbuf_slot_##NAME *slot;			\
								\
	barrier();						\
	slot = &ringbuf->slots[ringbuf->header.consumer % nentries]; \
								\
	/* A producer may be writing this entry, but hasn't finished yet. \
	 * We treat it as if the entry was not present. */	\
	if (READ_ONCE(slot->valid) == 0)			\
		return 1;					\
								\
	*entry = slot->entry;					\
	WRITE_ONCE(slot->valid, 0);				\
								\
	/* Ensure we have the entry stored before marking it safe for */ \
	/* overwriting, and ensure our consumer update is visible ASAP */ \
	/* afterwards. */					\
	ringbuf->header.consumer++;				\
	barrier();						\
	return 0;						\
}
/* Returns !=0 on failure, 0 on success. */
/* Multiple concurrent consumers must protect calls to this function with a lock. */
#define KLPP_RINGBUF_DEQUEUE_DISCARD(NAME)			\
static __inline__ int						\
klpp_ringbuf_dequeue_discard_##NAME(struct klpp_ringbuf_##NAME *ringbuf, \
				    size_t nentries)		\
{								\
	struct klpp_ringbuf_slot_##NAME *slot;			\
								\
	barrier();						\
	slot = &ringbuf->slots[ringbuf->header.consumer % nentries]; \
								\
	/* A producer may be writing this entry, but hasn't finished yet. */ \
	/* We treat it as if the entry was not present. */	\
	if (READ_ONCE(slot->valid) == 0)			\
		return 1;					\
								\
	WRITE_ONCE(slot->valid, 0);				\
	ringbuf->header.consumer++;				\
	barrier();						\
	return 0;						\
}

#define KLPP_RINGBUF_DEFINE(NAME, TYPE, ALIGNMENT)		\
	KLPP_RINGBUF_SLOT(NAME, TYPE, ALIGNMENT)		\
	KLPP_RINGBUF_STRUCT(NAME)				\
	KLPP_RINGBUF_SIZE(NAME)					\
	KLPP_RINGBUF_INIT(NAME)					\
	KLPP_RINGBUF_EMPTY(NAME)				\
	KLPP_RINGBUF_NUM_FREE(NAME)				\
	KLPP_RINGBUF_ENQUEUE(NAME, TYPE)			\
	KLPP_RINGBUF_PICK(NAME, TYPE)				\
	KLPP_RINGBUF_DEQUEUE(NAME, TYPE)			\
	KLPP_RINGBUF_DEQUEUE_DISCARD(NAME)

#endif /* __KLPP_SHARED_RINGBUF_H_ */
