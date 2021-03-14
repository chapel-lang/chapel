/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC. All
 * rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
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

/*
 * The buddy allocator splits the "base" block being managed into smaller
 * blocks.  Each block is a "power-of-two length".  These subblocks are kept
 * track of in a doubly linked list, or free list.  Here are the structures
 * and format of the data structures used in the buddy allocator.  For
 * a description of each field please see gnix_buddy_allocator.h.
 *
 * Handle structure:
 * ┌──────┬──────┬─────┬─────┬────────┬───────┐
 * │ BASE │ len  │ min │ max │ nlists │ LISTS │
 * └──────┴──────┴─────┴─────┴────────┴───────┘
 * The LISTS pointer points to an array of dlist structures, each containing a
 * head pointer to the beginning of a free list.  Note that the first element of
 * LISTS is a pointer to the head of the "min block size" free list, the second
 * element is the head of the "min * 2 block size" free list and so on.
 *
 * Node format as stored in a free block:
 * ┌──────┬──────┬──────────────────────┐
 * │ NEXT │ PREV │ Remaining free bytes │
 * └──────┴──────┴──────────────────────┘
 * Each NEXT and PREV pointer is stored in the first 16 bytes of the free block.
 * This means that there is a hard limit of 16 bytes on the minimum block size.
 *
 * Bitmap layout with a min block size of 16:
 * ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
 * │16│16│16│16│..│32│32│32│32│..│64│64│64│64│..│
 * └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
 * All the blocks that the buddy allocator allocates from the base block are of
 * size X, where X = MBS * 2^Z, MBS is the minimum block size and Z is a
 * non-negative integer.
 *
 * The bitmap has 2 * (Len / MBS) bits and it's setup so that the first
 * Len / MBS bits in the bitmap flag each block of size MBS as free or
 * allocated.
 *
 * Len is the number of bytes in the base block.
 * The base block is pointed to by void *base.
 *
 * The first bit in the bitmap flags the first block of size MBS.
 * The first block of size MBS uses the address range:
 * base to (base + MBS - 1).
 *
 * The second bit in the bitmap flags the second block of size MBS.
 * The second block of size MBS uses the address range:
 * (base + MBS) to (base + 2 * MBS - 1)
 *
 * The third bit in the bitmap flags the third block of size MBS.
 * The third block of size MBS uses the address range:
 * (base + 2 * MBS) to (base + 3 * MBS - 1)
 *
 * And so on until we reach the Len / MBS bit in the bitmap.
 *
 * The second Len / MBS bits in the bitmap flag the remaining blocks of size X
 * as free allocated, or split where X > MBS.
 *
 * So, the first bit in the second Len / MBS bits in the bitmap flags the first
 * block of size MBS * 2.  The first block of size MBS * 2 uses the address
 * range:
 * base to (base + MBS * 2 - 1)
 *
 * And so on until we reach the next block size.
 *
 * A bit is set to 1 when a block is allocated, or when the block is split into
 * two smaller blocks.
 *
 * A bit is reset to 0 when a block is freed, or when a free block is coalesced
 * with another already free and unsplit block.
 *
 * The bitmap is only read for coalescing blocks. When a block Y is freed we
 * look at the bit in the bitmap for the buddy block of Y, if that bit is set
 * then the buddy of Y is allocated, split, or both in which case we cannot
 * coalesce Y with its buddy block.  However, if the bitmap bit for the buddy of
 * Y is reset, then the buddy block of Y is free and not split, so we coalesce Y
 * with the buddy of block of Y and continue to coalesce this new larger block
 * with its buddy block until we reach the max block size or a buddy block that
 * is allocated, split, or both.
 *
 * TODO: dlist_insert_sorted for fragmentation reduction.
 * TODO: Lock in __gnix_buddy_split and allow __gnix_buddy_find_block to run
 * concurrently.
 * TODO: Allow __gnix_buddy_coalesce to run concurrently and return to
 * caller of _gnix_buddy_free sooner.  __gnix_buddy_coalesce is spending ~23%
 * of the time on top of the call stack compared to other functions when running
 * random_alloc_free.
 * TODO: Find a better solution for finding the address of a buddy block.
 */

#include "gnix_buddy_allocator.h"

static inline int __gnix_buddy_create_lists(gnix_buddy_alloc_handle_t
					    *alloc_handle)
{
	uint32_t i, offset = 0;

	alloc_handle->nlists = (uint32_t) __gnix_buddy_log2(alloc_handle->max /
							    MIN_BLOCK_SIZE) + 1;
	alloc_handle->lists = calloc(1, sizeof(struct dlist_entry) *
				     alloc_handle->nlists);

	if (OFI_UNLIKELY(!alloc_handle->lists)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Could not create buddy allocator lists.\n");
		return -FI_ENOMEM;
	}

	for (i = 0; i < alloc_handle->nlists; i++) {
		dlist_init(alloc_handle->lists + i);
	}

	/* Insert free blocks of size max in sorted order into last list */
	for (i = 0; i < alloc_handle->len / alloc_handle->max; i++) {
		dlist_insert_tail((void *) ((uint8_t *) alloc_handle->base +
					    offset),
				  alloc_handle->lists +
				  alloc_handle->nlists - 1);
		offset += alloc_handle->max;
	}

	return FI_SUCCESS;
}

/**
 * Split a block in list "j" until list "i" is reached.
 */
static inline void __gnix_buddy_split(gnix_buddy_alloc_handle_t *alloc_handle,
				      uint32_t j, uint32_t i, void **ptr)
{
	void *tmp = alloc_handle->lists[j].next;

	dlist_remove(tmp);

	/* Split the block until we reach list "i" */
	for (; j > i; j--) {
		_gnix_set_bit(&alloc_handle->bitmap,
			      __gnix_buddy_bitmap_index(tmp,
							OFFSET(MIN_BLOCK_SIZE, j),
							alloc_handle->base,
							alloc_handle->len,
							MIN_BLOCK_SIZE));

		dlist_insert_tail((void *) ((uint8_t *) tmp +
					    OFFSET(MIN_BLOCK_SIZE, j - 1)),
				  alloc_handle->lists + j - 1);
	}

	/* Allocate the block */
	*ptr = tmp;
}

/**
 * Find the first free block in list i.
 *
 * @return 1  if the block cannot be found.
 *
 * @return 0 if the block is found.
 */
static inline int __gnix_buddy_find_block(gnix_buddy_alloc_handle_t
					  *alloc_handle, uint32_t i, void **ptr)
{
	uint32_t j;

	for (j = i; j < alloc_handle->nlists; j++) {
		if (!dlist_empty(alloc_handle->lists + j)) {
			__gnix_buddy_split(alloc_handle, j, i, ptr);
			return 0;
		}
	}

	return 1;
}


/**
 * If the buddy block is on the free list then coalesce and insert into the next
 * list until we reach an allocated or split buddy block, or the max list size.
 */
static inline uint32_t __gnix_buddy_coalesce(gnix_buddy_alloc_handle_t *alloc_handle
					 , void **ptr, uint32_t block_size)
{
	void *buddy;

	for (buddy = __gnix_buddy_address(*ptr, block_size, alloc_handle->base);
	     block_size < alloc_handle->max &&
		     !_gnix_test_bit(&alloc_handle->bitmap,
				     __gnix_buddy_bitmap_index(buddy,
							       block_size,
							       alloc_handle->base,
							       alloc_handle->len,
							       MIN_BLOCK_SIZE));
	     buddy = __gnix_buddy_address(*ptr, block_size, alloc_handle->base)) {

		dlist_remove(buddy);

		/* Ensure ptr is the beginning of the new block */
		if (*ptr > buddy)
			*ptr = buddy;

		block_size *= 2;

		_gnix_clear_bit(&alloc_handle->bitmap,
				__gnix_buddy_bitmap_index(*ptr, block_size,
							  alloc_handle->base,
							  alloc_handle->len,
							  MIN_BLOCK_SIZE));
	}
	return block_size;
}

int _gnix_buddy_allocator_create(void *base, uint32_t len, uint32_t max,
				 gnix_buddy_alloc_handle_t **alloc_handle)
{
	char err_buf[256] = {0}, *error = NULL;
	int fi_errno;
	uint32_t size_check = len / MIN_BLOCK_SIZE * 2;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	/* Ensure parameters are valid */
	if (OFI_UNLIKELY(!base || !len || !max || max > len || !alloc_handle ||
			 IS_NOT_POW_TWO(max) || (len % max) ||
			 !size_check)) {

		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid parameter to _gnix_buddy_allocator_create."
			  "\n");
		return -FI_EINVAL;
	}

	*alloc_handle = calloc(1, sizeof(gnix_buddy_alloc_handle_t));

	if (OFI_UNLIKELY(!alloc_handle)) {
		error = strerror_r(errno, err_buf, sizeof(err_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Could not create buddy allocator handle.\n",
			  error);
		return -FI_ENOMEM;
	}

	fastlock_init(&alloc_handle[0]->lock);
	alloc_handle[0]->base = base;
	alloc_handle[0]->len = len;
	alloc_handle[0]->max = max;

	if (__gnix_buddy_create_lists(alloc_handle[0])) {
		free(*alloc_handle);
		return -FI_ENOMEM;
	}

	/* The bitmap needs len / MIN_BLOCK_SIZE * 2 bits to flag every possible
	 * block of size: min, min * 2, min * 4, ... , max that fits in the
	 * base. block.  The maximum number of bits used would be if max = len.
	 */
	if ((fi_errno = _gnix_alloc_bitmap(&alloc_handle[0]->bitmap,
					   len / MIN_BLOCK_SIZE * 2, NULL))) {

		free(&alloc_handle[0]->lists);
		free(*alloc_handle);
	}

	return fi_errno;
}

int _gnix_buddy_allocator_destroy(gnix_buddy_alloc_handle_t *alloc_handle)
{
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (OFI_UNLIKELY(!alloc_handle)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid parameter to _gnix_buddy_allocator_destroy."
			  "\n");
		return -FI_EINVAL;
	}

	fastlock_acquire(&alloc_handle->lock);

	free(alloc_handle->lists);

	while (_gnix_free_bitmap(&alloc_handle->bitmap)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Trying to free buddy allocator handle bitmap.\n");
		sleep(1);
	}

	fastlock_release(&alloc_handle->lock);
	fastlock_destroy(&alloc_handle->lock);

	free(alloc_handle);

	return FI_SUCCESS;
}

int _gnix_buddy_alloc(gnix_buddy_alloc_handle_t *alloc_handle, void **ptr,
		      uint32_t len)
{
	uint32_t block_size, i = 0;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (OFI_UNLIKELY(!alloc_handle || !ptr || !len ||
			 len > alloc_handle->max)) {

		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid parameter to _gnix_buddy_alloc.\n");
		return -FI_EINVAL;
	}

	block_size = BLOCK_SIZE(len, MIN_BLOCK_SIZE);
	i = (uint32_t) LIST_INDEX(block_size, MIN_BLOCK_SIZE);

	fastlock_acquire(&alloc_handle->lock);

	if (__gnix_buddy_find_block(alloc_handle, i, ptr)) {
		fastlock_release(&alloc_handle->lock);
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Could not allocate buddy block.\n");
		return -FI_ENOMEM;
	}

	fastlock_release(&alloc_handle->lock);

	_gnix_set_bit(&alloc_handle->bitmap,
		      __gnix_buddy_bitmap_index(*ptr, block_size,
						alloc_handle->base,
						alloc_handle->len,
						MIN_BLOCK_SIZE));

	return FI_SUCCESS;
}

int _gnix_buddy_free(gnix_buddy_alloc_handle_t *alloc_handle, void *ptr,
		     uint32_t len)
{
	uint32_t block_size;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (OFI_UNLIKELY(!alloc_handle || !len || len > alloc_handle->max ||
			 ptr >= (void *) ((uint8_t *) alloc_handle->base +
				      alloc_handle->len) ||
		     ptr < alloc_handle->base)) {

		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid parameter to _gnix_buddy_free.\n");
		return -FI_EINVAL;
	}

	block_size = BLOCK_SIZE(len, MIN_BLOCK_SIZE);

	_gnix_clear_bit(&alloc_handle->bitmap,
			__gnix_buddy_bitmap_index(ptr, block_size,
						  alloc_handle->base,
						  alloc_handle->len,
						  MIN_BLOCK_SIZE));

	fastlock_acquire(&alloc_handle->lock);

	block_size = __gnix_buddy_coalesce(alloc_handle, &ptr, block_size);

	dlist_insert_tail(ptr, alloc_handle->lists +
			  LIST_INDEX(block_size, MIN_BLOCK_SIZE));

	fastlock_release(&alloc_handle->lock);

	return FI_SUCCESS;
}
