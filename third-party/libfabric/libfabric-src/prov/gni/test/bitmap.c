/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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
 *  Created on: Apr 23, 2015
 *      Author: jswaro
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#include <gnix_bitmap.h>
#include "common.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"

gnix_bitmap_t *test_bitmap = NULL;
int call_free_bitmap = 0;

#if HAVE_ATOMICS

#define __gnix_set_block(bitmap, index, value) \
	atomic_store(&(bitmap)->arr[(index)], (value))
#define __gnix_load_block(bitmap, index) atomic_load(&(bitmap->arr[(index)]))
#else
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
#endif

void __gnix_bitmap_test_setup(void)
{
	cr_assert(test_bitmap == NULL);
	test_bitmap = calloc(1, sizeof(*test_bitmap));
	cr_assert(test_bitmap != NULL);

	call_free_bitmap = 1;
}

void __gnix_bitmap_test_teardown(void)
{
	if (call_free_bitmap) {
		_gnix_free_bitmap(test_bitmap);
	} else if (test_bitmap && test_bitmap->arr) {
		free(test_bitmap->arr);
	}

	cr_assert(test_bitmap != NULL);
	free(test_bitmap);
	test_bitmap = NULL;
}


static void __test_clean_bitmap_state(gnix_bitmap_t *bitmap,
		int _length, gnix_bitmap_state_e _state)
{
	cr_assert(bitmap->arr != NULL);
	cr_assert(bitmap->length == _length);
	cr_assert(bitmap->state == _state);
}

static void __test_initialize_bitmap(gnix_bitmap_t *bitmap, int bits)
{
	int ret = _gnix_alloc_bitmap(bitmap, bits, NULL);

	cr_assert(ret == 0);
	__test_clean_bitmap_state(bitmap, bits, GNIX_BITMAP_STATE_READY);
}

static void __test_initialize_bitmap_clean(gnix_bitmap_t *bitmap, int bits)
{
	__test_initialize_bitmap(bitmap, bits);
	cr_assert(_gnix_bitmap_empty(bitmap));
}

static void __test_realloc_bitmap(gnix_bitmap_t *bitmap, int bits)
{
	int ret = _gnix_realloc_bitmap(bitmap, bits);

	cr_assert(ret == 0);
	__test_clean_bitmap_state(bitmap, bits,	GNIX_BITMAP_STATE_READY);
}

static void __test_realloc_bitmap_clean(gnix_bitmap_t *bitmap, int initial,
		int next)
{
	__test_initialize_bitmap(bitmap, initial);
	__test_realloc_bitmap(bitmap, next);
	cr_assert(_gnix_bitmap_empty(bitmap));
}

static void __test_free_bitmap_clean(gnix_bitmap_t *bitmap)
{
	int ret = _gnix_free_bitmap(bitmap);

	cr_assert(ret == 0);
	cr_assert(bitmap->arr == NULL);
	cr_assert(bitmap->length == 0);
	cr_assert(bitmap->state == GNIX_BITMAP_STATE_FREE);
}

/*
 * Basic functionality tests for the gnix_bitmap_t object
 */

TestSuite(gnix_bitmap,
		.init = __gnix_bitmap_test_setup,
		.fini = __gnix_bitmap_test_teardown);

Test(gnix_bitmap, uninitialized)
{
	cr_assert(test_bitmap->arr == NULL);
	cr_assert(test_bitmap->length == 0);
	cr_assert(test_bitmap->state == GNIX_BITMAP_STATE_UNINITIALIZED);

	call_free_bitmap = 0;
}

Test(gnix_bitmap, initialize_128)
{
	__test_initialize_bitmap(test_bitmap, 128);

	call_free_bitmap = 0;
}

Test(gnix_bitmap, initialize_1)
{
	__test_initialize_bitmap(test_bitmap, 1);

	call_free_bitmap = 0;
}

Test(gnix_bitmap, initialize_0)
{
	int ret;

	ret = _gnix_alloc_bitmap(test_bitmap, 0, NULL);
	cr_assert(ret == -FI_EINVAL);

	call_free_bitmap = 0;
}

Test(gnix_bitmap, already_initialized)
{
	int ret;

	__test_initialize_bitmap(test_bitmap, 128);

	ret = _gnix_alloc_bitmap(test_bitmap, 128, NULL);
	cr_assert(ret == -FI_EINVAL);

	call_free_bitmap = 0;
}

Test(gnix_bitmap, destroy_bitmap)
{
	__test_initialize_bitmap(test_bitmap, 128);

	__test_free_bitmap_clean(test_bitmap);
}

Test(gnix_bitmap, destroy_bitmap_uninitialized)
{
	int ret;

	ret = _gnix_free_bitmap(test_bitmap);
	cr_assert(ret == -FI_EINVAL);
	cr_expect(test_bitmap->arr == NULL);
	cr_expect(test_bitmap->length == 0);
	cr_expect(test_bitmap->state == GNIX_BITMAP_STATE_UNINITIALIZED);
}

Test(gnix_bitmap, destroy_bitmap_already_freed)
{
	int ret;

	__test_initialize_bitmap(test_bitmap, 128);

	__test_free_bitmap_clean(test_bitmap);

	ret = _gnix_free_bitmap(test_bitmap);
	cr_assert(ret == -FI_EINVAL);
	cr_expect(test_bitmap->arr == NULL);
	cr_expect(test_bitmap->length == 0);
	cr_expect(test_bitmap->state == GNIX_BITMAP_STATE_FREE);
}

Test(gnix_bitmap, provided_buffer)
{
	int ret;
	void *buffer;
	uint32_t size, elements = 128;

	size = _gnix_bitmap_get_buffer_size(elements);
	cr_assert(size > 0, "bad size returned, size=%d\n", size);

	buffer = calloc(1, size);
	cr_assert(buffer, "failed to allocate buffer, size=%d", size);

	ret = _gnix_alloc_bitmap(test_bitmap, elements, buffer);
	cr_assert(ret == FI_SUCCESS, "ret=%d\n", ret);

	_gnix_fill_bitmap(test_bitmap, 1);

	ret = _gnix_bitmap_full(test_bitmap);
	cr_assert(ret == 1,
		"bitmap was not full, ret=%d\n",
		ret);

	_gnix_fill_bitmap(test_bitmap, 0);

	ret = _gnix_bitmap_empty(test_bitmap);
	cr_assert(ret == 1,
		"bitmap was not empty, ret=%d\n",
		ret);

	ret = _gnix_realloc_bitmap(test_bitmap, elements * 2);
	cr_assert(ret == -FI_EINVAL, "succeeded unexpectedly");

	ret = _gnix_free_bitmap(test_bitmap);
	cr_assert(ret == FI_SUCCESS);

	free(buffer);
}


Test(gnix_bitmap, realloc_63)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 63);
}

Test(gnix_bitmap, realloc_64)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 64);
}

Test(gnix_bitmap, realloc_65)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 65);
}

Test(gnix_bitmap, realloc_255)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 255);
}

Test(gnix_bitmap, realloc_256)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 256);
}

Test(gnix_bitmap, realloc_257)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 257);
}

Test(gnix_bitmap, realloc_63_check_bits)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 63);
}

Test(gnix_bitmap, realloc_64_check_bits)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 64);
}

Test(gnix_bitmap, realloc_65_check_bits)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 65);
}

Test(gnix_bitmap, realloc_255_check_bits)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 255);
}

Test(gnix_bitmap, realloc_256_check_bits)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 256);
}

Test(gnix_bitmap, realloc_257_check_bits)
{
	__test_realloc_bitmap_clean(test_bitmap, 128, 257);
}

Test(gnix_bitmap, bit_set_test_pass)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 1);

	cr_assert(_gnix_test_bit(test_bitmap, 1));
}

Test(gnix_bitmap, bit_set_test_fail)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 1);

	cr_assert(!_gnix_test_bit(test_bitmap, 0));
}

Test(gnix_bitmap, bit_set_clear)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 1);

	cr_assert(_gnix_test_bit(test_bitmap, 1));

	_gnix_clear_bit(test_bitmap, 1);

	cr_assert(!_gnix_test_bit(test_bitmap, 1));
}

Test(gnix_bitmap, bit_clear)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_clear_bit(test_bitmap, 1);

	cr_assert(!_gnix_test_bit(test_bitmap, 1));
}

Test(gnix_bitmap, bit_set)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 1);
}

Test(gnix_bitmap, bit_test_and_set_unset)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	cr_assert(!_gnix_test_and_set_bit(test_bitmap, 1));
}

Test(gnix_bitmap, bit_test_and_set_already_set)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 1);
	cr_assert(_gnix_test_bit(test_bitmap, 1));

	cr_assert(_gnix_test_and_set_bit(test_bitmap, 1));
}

Test(gnix_bitmap, bit_test_and_clear_unset)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	cr_assert(!_gnix_test_and_clear_bit(test_bitmap, 1));
}

Test(gnix_bitmap, bit_test_and_clear_already_set)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 1);
	cr_assert(_gnix_test_bit(test_bitmap, 1));

	cr_assert(_gnix_test_and_clear_bit(test_bitmap, 1));
}

Test(gnix_bitmap, ffs_clean_bitmap)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	cr_assert(_gnix_find_first_set_bit(test_bitmap) == -FI_EAGAIN);
}

Test(gnix_bitmap, ffs_first_bit_set)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 0);

	cr_assert(_gnix_find_first_set_bit(test_bitmap) == 0);
}

Test(gnix_bitmap, ffs_seventeen_set)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_set_bit(test_bitmap, 17);

	cr_assert(_gnix_find_first_set_bit(test_bitmap) == 17);
}

Test(gnix_bitmap, ffz_clean_bitmap)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	cr_assert(_gnix_find_first_zero_bit(test_bitmap) == 0);
}

Test(gnix_bitmap, ffz_full_bitmap)
{
	int i;

	__test_initialize_bitmap_clean(test_bitmap, 64);

	for (i = 0; i < test_bitmap->length; ++i) {
		_gnix_set_bit(test_bitmap, i);
		cr_assert(_gnix_test_bit(test_bitmap, i));
	}

	cr_assert(_gnix_find_first_zero_bit(test_bitmap) == -FI_EAGAIN);
}

Test(gnix_bitmap, ffz_first_half_set)
{
	int i;

	__test_initialize_bitmap_clean(test_bitmap, 64);

	for (i = 0; i < 32 ; ++i) {
		_gnix_set_bit(test_bitmap, i);
		cr_assert(_gnix_test_bit(test_bitmap, i));
	}

	cr_expect(test_bitmap->length == 64);
	cr_expect(i == 32);
	cr_assert(_gnix_find_first_zero_bit(test_bitmap) == i);
}

Test(gnix_bitmap, map_fill_0)
{
	int i;

	__test_initialize_bitmap_clean(test_bitmap, 64);

	for (i = 0; i < test_bitmap->length; ++i) {
		_gnix_set_bit(test_bitmap, i);
		cr_assert(_gnix_test_bit(test_bitmap, i));
	}

	cr_assert(_gnix_bitmap_full(test_bitmap));

	_gnix_fill_bitmap(test_bitmap, 0);

	cr_assert(_gnix_bitmap_empty(test_bitmap));
}

Test(gnix_bitmap, map_fill_1)
{
	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_fill_bitmap(test_bitmap, 1);

	cr_assert(_gnix_bitmap_full(test_bitmap));
}

Test(gnix_bitmap, bitmap_load)
{
	gnix_bitmap_value_t expected = ~0;

	__test_initialize_bitmap_clean(test_bitmap, 64);

	_gnix_fill_bitmap(test_bitmap, 1);

	cr_assert(expected == __gnix_load_block(test_bitmap, 0));
}

Test(gnix_bitmap, bitmap_set)
{
	gnix_bitmap_value_t expected = ~0;

	__test_initialize_bitmap_clean(test_bitmap, 64);

	__gnix_set_block(test_bitmap, 0, expected);

	cr_assert(__gnix_load_block(test_bitmap, 0) == expected);
}


Test(gnix_bitmap, fill_bitmap_60_ffz_eagain)
{
	int i;

	__test_initialize_bitmap_clean(test_bitmap, 60);

	for (i = 0; i < 60; ++i)
		_gnix_set_bit(test_bitmap, i);

	cr_assert(_gnix_find_first_zero_bit(test_bitmap) == -FI_EAGAIN);
}

Test(gnix_bitmap, fill_bitmap_60_ffs_eagain)
{
	int i;

	__test_initialize_bitmap_clean(test_bitmap, 60);

	/* this will succeed because set_bit doesn't account for bounds of the
	 *   bitmap as the user should be responsible for handling the bitmap
	 *   properly.
	 */
	for (i = 60; i < 64; ++i)
		_gnix_set_bit(test_bitmap, i);

	cr_assert(_gnix_find_first_set_bit(test_bitmap) == -FI_EAGAIN);
}

TestSuite(perf_bitmap,
	  .init = __gnix_bitmap_test_setup,
	  .fini = __gnix_bitmap_test_teardown,
	  .disabled = true);

Test(perf_bitmap, performance_set_test)
{
	int i, j;
	int secs, usec;
	struct timeval start, end;

	__test_initialize_bitmap_clean(test_bitmap, 8192);

	gettimeofday(&start, 0);
	for (i = 0; i < 100000; ++i) {
		j = i % 8192;
		_gnix_set_bit(test_bitmap, j);
		cr_assert(_gnix_test_bit(test_bitmap, j));
		_gnix_clear_bit(test_bitmap, j);
		cr_assert(!_gnix_test_bit(test_bitmap, j));
	}
	gettimeofday(&end, 0);

	calculate_time_difference(&start, &end, &secs, &usec);

	cr_assert(_gnix_bitmap_empty(test_bitmap));

	cr_expect(secs < 1);
}

Test(perf_bitmap, performance_set_test_random)
{
	int i, j;
	int secs, usec;
	struct timeval start, end;

	srand(time(NULL));

	__test_initialize_bitmap_clean(test_bitmap, 8192);

	gettimeofday(&start, 0);
	for (i = 0; i < 100000; ++i) {
		j = rand() % 8192;
		_gnix_set_bit(test_bitmap, j);
		cr_assert(_gnix_test_bit(test_bitmap, j));
		_gnix_clear_bit(test_bitmap, j);
		cr_assert(!_gnix_test_bit(test_bitmap, j));
	}
	gettimeofday(&end, 0);

	calculate_time_difference(&start, &end, &secs, &usec);

	cr_assert(_gnix_bitmap_empty(test_bitmap));

	cr_expect(secs < 1);
}

