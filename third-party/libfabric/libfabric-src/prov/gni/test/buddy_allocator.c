/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC. All
 * rights reserved.
 * Copyright (c) 2015-2016 Cray Inc. All rights reserved.
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

#include "gnix_buddy_allocator.h"
#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include <time.h>

#define LEN (1024 * 1024)	/* buddy_handle->len */
#define MAX_LEN (LEN / 2)	/* buddy_handle->max */
#define MIN_LEN MIN_BLOCK_SIZE

long *buf = NULL;		/* buddy_handle->base */
gnix_buddy_alloc_handle_t *buddy_handle;

struct ptrs_t {
	void *ptr;		/* ptrs alloc'd by buddy_alloc */
	uint32_t size;		/* size of the ptr */
} *ptrs;

void buddy_allocator_setup(void)
{
	int ret;

	ptrs = calloc(LEN / MIN_LEN, sizeof(struct ptrs_t));
	cr_assert(ptrs, "buddy_allocator_setup");

	buf = calloc(LEN, sizeof(long));
	cr_assert(buf, "buddy_allocator_setup");

	ret = _gnix_buddy_allocator_create(buf, LEN, MAX_LEN, &buddy_handle);
	cr_assert(!ret, "_gnix_buddy_allocator_create");
}

void buddy_allocator_teardown(void)
{
	int ret;

	ret = _gnix_buddy_allocator_destroy(buddy_handle);
	cr_assert(!ret, "_gnix_buddy_allocator_destroy");

	free(ptrs);
	free(buf);
}

/* Test invalid parameters for setup */
void buddy_allocator_setup_error(void)
{
	int ret;

	ret = _gnix_buddy_allocator_create(NULL, LEN, MAX_LEN, &buddy_handle);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_allocator_create(buf, 0, MAX_LEN, &buddy_handle);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_allocator_create(buf, LEN, LEN + 1, &buddy_handle);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_allocator_create(buf, LEN, 0, &buddy_handle);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_allocator_create(buf, LEN, MAX_LEN, NULL);
	cr_assert_eq(ret, -FI_EINVAL);
}

/* Test invalid parameters for teardown */
void buddy_allocator_teardown_error(void)
{
	int ret;

	ret = _gnix_buddy_allocator_destroy(NULL);
	cr_assert_eq(ret, -FI_EINVAL);
}

/* Sequential alloc */
void do_alloc(uint32_t len)
{
	uint32_t i = 0, ret;

	/* Allocate all the memory and write to each block */
	for (; i < LEN / len; i++) {
		ptrs[i].size = len;
		ret = _gnix_buddy_alloc(buddy_handle, &ptrs[i].ptr, len);
		cr_assert(!ret, "_gnix_buddy_alloc");
		memset(ptrs[i].ptr, 0, len);
	}

	/* Ensure that all free lists are empty */
	for (i = 0; i < buddy_handle->nlists; i++) {
		ret = dlist_empty(buddy_handle->lists + i);
		cr_assert_eq(ret, 1);
	}
}

/* Sequential free */
void do_free(uint32_t len)
{
	int i = 0, ret;

	/* Free all allocated blocks */
	for (i = 0; i < LEN / len; i++) {
		ret = _gnix_buddy_free(buddy_handle, ptrs[i].ptr, ptrs[i].size);
		cr_assert(!ret, "_gnix_buddy_free");
	}

	/* Ensure that every free list except the last is empty */
	for (i = 0; i < buddy_handle->nlists - 1; i++) {
		ret = dlist_empty(buddy_handle->lists + i);
		cr_assert_eq(ret, 1);
	}
	ret = dlist_empty(buddy_handle->lists + i);
	cr_assert_eq(ret, 0);
}

TestSuite(buddy_allocator, .init = buddy_allocator_setup,
	  .fini = buddy_allocator_teardown, .disabled = false);

/* Sequential alloc and frees */
Test(buddy_allocator, sequential_alloc_free)
{
	uint32_t i = MIN_LEN;

	for (i = MIN_LEN; i <= MAX_LEN; i *= 2) {
		do_alloc(i);
		do_free(i);
	}
}

/* Pseudo random allocs and frees */
Test(buddy_allocator, random_alloc_free)
{
	int i = 0, j = 0, ret;

	srand((unsigned) time(NULL));

	for (j = MIN_LEN; j <= MAX_LEN; j *= 2) {
		do {
			ret = rand() % 100;

			if (ret <= 49) {
				/* ~50% chance to alloc min size blocks*/
				ptrs[i].size = MIN_BLOCK_SIZE;
			} else if (ret >= 50 &&
				   ret <= 87) {
				/* ~37% chance to alloc blocks of size
				 * [MIN_BLOCK_SIZE * 2, MAX_BLOCK_SIZE / 2]
				 */
				ptrs[i].size = OFFSET(MIN_BLOCK_SIZE,
						      (rand() %
						       (buddy_handle->nlists -
							1)) + 1);
			} else {
				/* ~13% chance to alloc max size blocks */
				ptrs[i].size = buddy_handle->max;
			}

			ret = _gnix_buddy_alloc(buddy_handle, &ptrs[i].ptr,
					  ptrs[i].size);
			cr_assert_neq(ret, -FI_EINVAL);

			i++;
		} while (ret != -FI_ENOMEM);

		/* Free all allocated blocks */
		for (i -= 2; i >= 0; i--) {
			ret = _gnix_buddy_free(buddy_handle, ptrs[i].ptr,
					       ptrs[i].size);
			cr_assert(!ret, "_gnix_buddy_free");
		}

		/* Ensure that every free list except the last is empty */
		for (i = 0; i < buddy_handle->nlists - 1; i++) {
			ret = dlist_empty(buddy_handle->lists + i);
			cr_assert_eq(ret, 1);
		}
		ret = dlist_empty(buddy_handle->lists + i);
		cr_assert_eq(ret, 0);

		i = 0;
	}
}

Test(buddy_allocator, alloc_free_error)
{
	int ret;
	void *tmp;

	do_alloc(MIN_LEN);

	/* Request one additional block */
	ret = _gnix_buddy_alloc(buddy_handle, &tmp, MIN_LEN);
	cr_assert_eq(ret, -FI_ENOMEM);

	do_free(MIN_LEN);
}

/* Test invalid buddy alloc and free parameters */
Test(buddy_allocator, parameter_error)
{
	int ret;

	buddy_allocator_setup_error();
	buddy_allocator_teardown_error();

	/* BEGIN: Alloc, invalid parameters */
	ret = _gnix_buddy_alloc(NULL, ptrs->ptr, MAX_LEN);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_alloc(buddy_handle, ptrs->ptr, MAX_LEN + 1);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_alloc(buddy_handle, ptrs->ptr, 0);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_alloc(buddy_handle, NULL, MAX_LEN);
	cr_assert_eq(ret, -FI_EINVAL);
	/* END: Alloc, invalid parameters */

	/* BEGIN: Free, invalid parameters */
	ret = _gnix_buddy_free(NULL, ptrs->ptr, MAX_LEN);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_free(buddy_handle, NULL, MAX_LEN);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_free(buddy_handle, buf - 1, MAX_LEN);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_free(buddy_handle, buf + LEN, MAX_LEN);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_free(buddy_handle, buf, MAX_LEN + 1);
	cr_assert_eq(ret, -FI_EINVAL);

	ret = _gnix_buddy_free(buddy_handle, buf - 1, 0);
	cr_assert_eq(ret, -FI_EINVAL);
	/* END: Free, invalid parameters */
}
