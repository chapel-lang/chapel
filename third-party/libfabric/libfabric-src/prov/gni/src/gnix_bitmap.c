/*
 * Copyright (c) 2015,2017 Cray Inc. All rights reserved.
 *
 *  Created on: Apr 16, 2015
 *      Author: jswaro
 */

#include <stdlib.h>
#include <rdma/fi_errno.h>

#include "gnix_bitmap.h"

#ifdef HAVE_ATOMICS

#define __gnix_init_block(block) atomic_init(block, 0)
#define __gnix_set_block(bitmap, index, value) \
	atomic_store(&(bitmap)->arr[(index)], (value))
#define __gnix_load_block(bitmap, index) atomic_load(&(bitmap->arr[(index)]))
#define __gnix_set_bit(bitmap, bit) \
	atomic_fetch_or(&(bitmap)->arr[GNIX_BUCKET_INDEX(bit)], \
			GNIX_BIT_VALUE(bit))
#define __gnix_clear_bit(bitmap, bit) \
	atomic_fetch_and(&(bitmap)->arr[GNIX_BUCKET_INDEX(bit)], \
			~GNIX_BIT_VALUE(bit))
#define __gnix_test_bit(bitmap, bit) \
	((atomic_load(&(bitmap)->arr[GNIX_BUCKET_INDEX(bit)]) \
			& GNIX_BIT_VALUE(bit)) != 0)
#else

static inline void __gnix_init_block(gnix_bitmap_block_t *block)
{
	fastlock_init(&block->lock);
	block->val = 0llu;
}

static inline void __gnix_set_block(gnix_bitmap_t *bitmap, int index,
		uint64_t value)
{
	gnix_bitmap_block_t *block = &bitmap->arr[index];

	fastlock_acquire(&block->lock);
	block->val = value;
	fastlock_release(&block->lock);
}

static inline uint64_t __gnix_load_block(gnix_bitmap_t *bitmap, int index)
{
	gnix_bitmap_block_t *block = &bitmap->arr[index];
	uint64_t ret;

	fastlock_acquire(&block->lock);
	ret = block->val;
	fastlock_release(&block->lock);

	return ret;
}

static inline uint64_t __gnix_set_bit(gnix_bitmap_t *bitmap, int bit)
{
	gnix_bitmap_block_t *block = &bitmap->arr[GNIX_BUCKET_INDEX(bit)];
	uint64_t ret;

	fastlock_acquire(&block->lock);
	ret = block->val;
	block->val |= GNIX_BIT_VALUE(bit);
	fastlock_release(&block->lock);

	return ret;
}

static inline uint64_t __gnix_clear_bit(gnix_bitmap_t *bitmap, int bit)
{
	gnix_bitmap_block_t *block = &bitmap->arr[GNIX_BUCKET_INDEX(bit)];
	uint64_t ret;

	fastlock_acquire(&block->lock);
	ret = block->val;
	block->val &= ~GNIX_BIT_VALUE(bit);
	fastlock_release(&block->lock);

	return ret;
}

static inline int __gnix_test_bit(gnix_bitmap_t *bitmap, int bit)
{
	gnix_bitmap_block_t *block = &bitmap->arr[GNIX_BUCKET_INDEX(bit)];
	int ret;

	fastlock_acquire(&block->lock);
	ret = (block->val & GNIX_BIT_VALUE(bit)) != 0;
	fastlock_release(&block->lock);

	return ret;
}
#endif

int _gnix_test_bit(gnix_bitmap_t *bitmap, uint32_t index)
{
	return __gnix_test_bit(bitmap, index);
}

void _gnix_set_bit(gnix_bitmap_t *bitmap, uint32_t index)
{
	__gnix_set_bit(bitmap, index);
}

void _gnix_clear_bit(gnix_bitmap_t *bitmap, uint32_t index)
{
	__gnix_clear_bit(bitmap, index);
}

int _gnix_test_and_set_bit(gnix_bitmap_t *bitmap, uint32_t index)
{
	return (__gnix_set_bit(bitmap, index) & GNIX_BIT_VALUE(index)) != 0;
}

int _gnix_test_and_clear_bit(gnix_bitmap_t *bitmap, uint32_t index)
{
	return (__gnix_clear_bit(bitmap, index) & GNIX_BIT_VALUE(index)) != 0;
}

int _gnix_bitmap_full(gnix_bitmap_t *bitmap)
{
	return _gnix_find_first_zero_bit(bitmap) == -EAGAIN;
}

int _gnix_bitmap_empty(gnix_bitmap_t *bitmap)
{
	return _gnix_find_first_set_bit(bitmap) == -FI_EAGAIN;
}

int _gnix_find_first_zero_bit(gnix_bitmap_t *bitmap)
{
	int i, pos;
	gnix_bitmap_value_t value;

	for (i = 0, pos = 0;
			i < GNIX_BITMAP_BLOCKS(bitmap->length);
			++i, pos += GNIX_BITMAP_BUCKET_LENGTH) {
		/* invert the bits to check for first zero bit */
		value = ~(__gnix_load_block(bitmap, i));

		if (value != 0) {
			/* no need to check for errors because we have
			   established there is an unset bit */
			pos += ffsll(value) - 1;

			if (pos < bitmap->length)
				return pos;
			else
				return -FI_EAGAIN;
		}
	}

	return -FI_EAGAIN;
}

int _gnix_find_first_set_bit(gnix_bitmap_t *bitmap)
{
	int i, pos;
	gnix_bitmap_value_t value;

	for (i = 0, pos = 0;
			i < GNIX_BITMAP_BLOCKS(bitmap->length);
			++i, pos += GNIX_BITMAP_BUCKET_LENGTH) {
		value = __gnix_load_block(bitmap, i);

		if (value != 0) {
			/* no need to check for errors because we have
			   established there is a set bit */
			pos += ffsll(value) - 1;

			if (pos < bitmap->length)
				return pos;
			else
				return -FI_EAGAIN;		}
	}

	return -FI_EAGAIN;
}

void _gnix_fill_bitmap(gnix_bitmap_t *bitmap, uint64_t value)
{
	int i;
	gnix_bitmap_value_t fill_value = (value != 0) ? ~0 : 0;

	for (i = 0; i < GNIX_BITMAP_BLOCKS(bitmap->length); ++i) {
		__gnix_set_block(bitmap, i, fill_value);
	}
}

int _gnix_alloc_bitmap(gnix_bitmap_t *bitmap, uint32_t nbits, void *addr)
{
	int i;

	if (bitmap->state == GNIX_BITMAP_STATE_READY)
		return -FI_EINVAL;

	if (bitmap->length != 0 || nbits == 0)
		return -FI_EINVAL;

	if (!addr) {
		bitmap->arr = calloc(GNIX_BITMAP_BLOCKS(nbits),
				sizeof(gnix_bitmap_block_t));
		bitmap->internal_buffer_allocation = 1;
	} else {
		bitmap->arr = addr;
		bitmap->internal_buffer_allocation = 0;
	}

	if (!bitmap->arr)
		return -FI_ENOMEM;

	bitmap->length = nbits;

	for (i = 0; i < GNIX_BITMAP_BLOCKS(bitmap->length); ++i)
		__gnix_init_block(&bitmap->arr[i]);

	bitmap->state = GNIX_BITMAP_STATE_READY;

	return 0;
}

int _gnix_realloc_bitmap(gnix_bitmap_t *bitmap, uint32_t nbits)
{
	gnix_bitmap_block_t *new_allocation;
	int blocks_to_allocate = GNIX_BITMAP_BLOCKS(nbits);
	int i;

	if (bitmap->state != GNIX_BITMAP_STATE_READY)
		return -FI_EINVAL;

	if (nbits == 0 || bitmap->arr == NULL)
		return -FI_EINVAL;

	if (!bitmap->internal_buffer_allocation)
		return -FI_EINVAL;

	new_allocation = realloc(bitmap->arr,
			(blocks_to_allocate *
					sizeof(gnix_bitmap_block_t)));

	if (!new_allocation)
		return -FI_ENOMEM;

	bitmap->arr = new_allocation;

	/* Did we increase the size of the bitmap?
	 * If so, initialize new blocks */
	if (blocks_to_allocate > GNIX_BITMAP_BLOCKS(bitmap->length)) {
		for (i = GNIX_BITMAP_BLOCKS(bitmap->length);
				i < blocks_to_allocate;
				++i) {
			__gnix_init_block(&bitmap->arr[i]);
		}
	}

	bitmap->length = nbits;

	return 0;
}

int _gnix_free_bitmap(gnix_bitmap_t *bitmap)
{
	if (bitmap->state != GNIX_BITMAP_STATE_READY)
		return -FI_EINVAL;

	bitmap->length = 0;
	if (bitmap->arr && bitmap->internal_buffer_allocation) {
		free(bitmap->arr);
		bitmap->arr = NULL;
	}

	bitmap->state = GNIX_BITMAP_STATE_FREE;

	return 0;
}
