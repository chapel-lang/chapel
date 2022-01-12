/*
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

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include <gnix_hashtable.h>
#include <gnix_bitmap.h>

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"

#define __GNIX_MAGIC_VALUE 0xDEADBEEF

extern const gnix_hashtable_attr_t default_attr;

typedef struct gnix_test_element {
	uint64_t val;
	uint64_t key;
	uint64_t magic;
} gnix_test_element_t;

#define GNIX_TEST_ELEMENT_INIT(_val, _key) \
	{ .val = (_val), .key = (_key), .magic = (__GNIX_MAGIC_VALUE) }

gnix_test_element_t elements[4] = {
	GNIX_TEST_ELEMENT_INIT(1, 100),
	GNIX_TEST_ELEMENT_INIT(2, 200),
	GNIX_TEST_ELEMENT_INIT(10, 300),
	GNIX_TEST_ELEMENT_INIT(777, 500000)
};

gnix_test_element_t *simple_element = &elements[0];
gnix_hashtable_t *test_ht = NULL;

void __gnix_hashtable_test_uninitialized(void)
{
	cr_assert(test_ht->ht_state == GNIX_HT_STATE_UNINITIALIZED);
	cr_assert(test_ht->ht_size == 0);
	cr_assert(test_ht->ht_lf_tbl == NULL);
}

void __gnix_hashtable_test_setup_bare(void)
{
	cr_assert(test_ht == NULL);
	test_ht = (gnix_hashtable_t *) calloc(1, sizeof(gnix_hashtable_t));
	cr_assert(test_ht != NULL);

	__gnix_hashtable_test_uninitialized();
}


void __gnix_hashtable_test_teardown_bare(void)
{
	cr_assert(test_ht != NULL);
	free(test_ht);
	test_ht = NULL;
}

void __gnix_hashtable_test_initialized(void)
{
	cr_assert(test_ht->ht_state == GNIX_HT_STATE_READY);
	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
	cr_assert(test_ht->ht_size == test_ht->ht_attr.ht_initial_size);
	cr_assert(test_ht->ht_lf_tbl != NULL);
}

void __gnix_hashtable_test_destroyed_clean(void)
{
	cr_assert(test_ht->ht_state == GNIX_HT_STATE_DEAD);
	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
	cr_assert(test_ht->ht_size == 0);
	cr_assert(test_ht->ht_lf_tbl == NULL);
}

void __gnix_hashtable_destroy(void)
{
	int ret = _gnix_ht_destroy(test_ht);
	cr_assert(ret == 0);
	__gnix_hashtable_test_destroyed_clean();
}

void __gnix_hashtable_initialize(void)
{
	int ret;

	ret = _gnix_ht_init(test_ht, NULL);
	cr_assert(ret == 0);

	__gnix_hashtable_test_initialized();
}

void __gnix_hashtable_initialize_attr(gnix_hashtable_attr_t *attr)
{
	int ret;

	ret = _gnix_ht_init(test_ht, attr);
	cr_assert(ret == 0);

	__gnix_hashtable_test_initialized();
}

void __gnix_hashtable_test_setup(void)
{
	__gnix_hashtable_test_setup_bare();

	__gnix_hashtable_test_uninitialized();

	__gnix_hashtable_initialize();
}

void __gnix_hashtable_test_setup_locked(void)
{
	gnix_hashtable_attr_t attr = default_attr;

	__gnix_hashtable_test_setup_bare();

	__gnix_hashtable_test_uninitialized();

	attr.ht_internal_locking = 1;
	__gnix_hashtable_initialize_attr(&attr);
}

void __gnix_hashtable_test_teardown(void)
{
	__gnix_hashtable_destroy();

	__gnix_hashtable_test_teardown_bare();
}

/*
 * Basic functionality tests for the gnix_hashtable_t object
 */

TestSuite(gnix_hashtable_basic,
		.init = __gnix_hashtable_test_setup_bare,
		.fini = __gnix_hashtable_test_teardown_bare);

TestSuite(gnix_hashtable_advanced,
		.init = __gnix_hashtable_test_setup,
		.fini = __gnix_hashtable_test_teardown);

TestSuite(gnix_hashtable_locked,
		.init = __gnix_hashtable_test_setup_locked,
		.fini = __gnix_hashtable_test_teardown);

Test(gnix_hashtable_basic, uninitialized)
{
	__gnix_hashtable_test_uninitialized();
}


Test(gnix_hashtable_basic, initialize_ht)
{
	__gnix_hashtable_initialize();
	__gnix_hashtable_destroy();
}

Test(gnix_hashtable_basic, initialize_locked_ht)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_internal_locking = 1;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == 0);

	__gnix_hashtable_test_initialized();

	__gnix_hashtable_destroy();
}

Test(gnix_hashtable_basic, err_initialize_twice)
{
	int ret;

	__gnix_hashtable_initialize();

	ret = _gnix_ht_init(test_ht, NULL);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_initialized();

	__gnix_hashtable_destroy();
}

Test(gnix_hashtable_basic, err_invalid_initial_size_0)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_initial_size = 0;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_invalid_initial_size_gt_max)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_initial_size = attr.ht_maximum_size * 2;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_invalid_max_size)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_maximum_size = 0;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_invalid_increase_step_all)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_increase_step = 0;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_invalid_increase_step_mult)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_increase_step = 1;
	attr.ht_increase_type = GNIX_HT_INCREASE_MULT;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_invalid_increase_type)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_increase_type = -1;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_invalid_collision)
{
	int ret;
	gnix_hashtable_attr_t attr;

	memcpy(&attr, &default_attr, sizeof(gnix_hashtable_attr_t));

	attr.ht_collision_thresh = 0;

	ret = _gnix_ht_init(test_ht, &attr);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, err_destroy_uninitialized)
{
	int ret;

	ret = _gnix_ht_destroy(test_ht);
	cr_assert(ret == -FI_EINVAL);

	__gnix_hashtable_test_uninitialized();
}

Test(gnix_hashtable_basic, destroy)
{
	__gnix_hashtable_initialize();

	__gnix_hashtable_destroy();
}

Test(gnix_hashtable_basic, destroy_twice)
{
	int ret;

	__gnix_hashtable_initialize();

	__gnix_hashtable_destroy();

	ret = _gnix_ht_destroy(test_ht);
	cr_assert(ret == -FI_EINVAL);
	__gnix_hashtable_test_destroyed_clean();
}

Test(gnix_hashtable_advanced, insert_1)
{
	int ret;

	ret = _gnix_ht_insert(test_ht, simple_element->key, simple_element);
	cr_assert(ret == 0);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1);
}

Test(gnix_hashtable_advanced, insert_duplicate)
{
	int ret;

	ret = _gnix_ht_insert(test_ht, simple_element->key, simple_element);
	cr_assert(ret == 0);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1);

	ret = _gnix_ht_insert(test_ht, simple_element->key, simple_element);
	cr_assert(ret == -FI_ENOSPC);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1);
}

Test(gnix_hashtable_advanced, insert_1_remove_1)
{
	int ret;

	srand(time(NULL));

	ret = _gnix_ht_insert(test_ht, simple_element->key, simple_element);
	cr_assert(ret == 0);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1);

	ret = _gnix_ht_remove(test_ht, simple_element->key);
	cr_assert(ret == 0);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
}


Test(gnix_hashtable_advanced, insert_1024)
{
	int ret, i;

	gnix_test_element_t test_elements[1024];

	srand(time(NULL));

	for (i = 0; i < 1024; ++i) {
		test_elements[i].key = i;
		test_elements[i].val = rand() % (1024 * 1024);
		test_elements[i].magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < 1024; ++i) {
		ret = _gnix_ht_insert(test_ht,
				test_elements[i].key, &test_elements[i]);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1024);
}


Test(gnix_hashtable_advanced, insert_1024_remove_1024)
{
	int ret, i;

	gnix_test_element_t test_elements[1024];
	gnix_test_element_t *item;

	srand(time(NULL));

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];
		item->key = i;
		item->val = rand() % (1024 * 1024);
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];
		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	for (i = 1023; i >= 0; --i) {
		item = &test_elements[i];
		cr_assert(i == item->key);

		ret = _gnix_ht_remove(test_ht,
				item->key);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == i);
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
}

Test(gnix_hashtable_advanced, insert_2048_remove_all_resize_down)
{
	int ret, i;
	int nelem = 2048;
	gnix_test_element_t test_elements[2048];
	gnix_test_element_t *item;

	srand(time(NULL));

	for (i = 0; i < nelem; ++i) {
		item = &test_elements[i];
		item->key = i;
		item->val = rand() % (1024 * 1024);
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < nelem; ++i) {
		item = &test_elements[i];
		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	cr_assert(test_ht->ht_size > test_ht->ht_attr.ht_initial_size);

	for (i = nelem - 1; i >= 0; --i) {
		item = &test_elements[i];
		cr_assert(i == item->key);

		ret = _gnix_ht_remove(test_ht,
				item->key);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == i);
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
	/* on default settings, the hash table should resize to initial on
	 *   removal of all elements
	 */
	cr_assert(test_ht->ht_size == test_ht->ht_attr.ht_initial_size);
}


Test(gnix_hashtable_advanced, insert_1_lookup_pass)
{
	int ret;
	gnix_test_element_t *found = NULL;

	ret = _gnix_ht_insert(test_ht,
			simple_element->key, simple_element);
	cr_assert(ret == 0);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1);

	found = _gnix_ht_lookup(test_ht, simple_element->key);
	cr_assert(found == simple_element);
	cr_assert(found->magic == __GNIX_MAGIC_VALUE);
}

Test(gnix_hashtable_advanced, insert_1_lookup_fail)
{
	int ret;
	gnix_test_element_t *found = NULL;

	ret = _gnix_ht_insert(test_ht,
			simple_element->key, simple_element);
	cr_assert(ret == 0);

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1);

	found = _gnix_ht_lookup(test_ht, simple_element->key - 1);
	cr_assert(found != simple_element);
	cr_assert(found == NULL);
}

Test(gnix_hashtable_advanced, insert_1024_lookup_all)
{
	int ret, i;
	gnix_test_element_t test_elements[1024];
	gnix_test_element_t *item;
	gnix_test_element_t *found = NULL;

	srand(time(NULL));

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];

		item->key = i;
		item->val = rand() % (1024 * 1024);
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];

		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1024);

	for (i = 0; i < 1024; ++i) {
		found = _gnix_ht_lookup(test_ht, test_elements[i].key);
		cr_assert(found != NULL);
		cr_assert(found == &test_elements[i]);
		cr_assert(found->magic == __GNIX_MAGIC_VALUE);
	}
}

Test(gnix_hashtable_advanced, insert_1024_lookup_random)
{
	int ret, i;
	gnix_test_element_t test_elements[1024];
	gnix_test_element_t *found = NULL, *to_find = NULL;
	gnix_test_element_t *item;

	srand(time(NULL));

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];

		item->key = i;
		item->val = rand() % (1024 * 1024);
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];

		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 1024);

	for (i = 0; i < 1024; ++i) {
		to_find = &test_elements[rand() % 1024];
		found = _gnix_ht_lookup(test_ht, to_find->key);
		cr_assert(found != NULL);
		cr_assert(found == to_find);
		cr_assert(found->magic == __GNIX_MAGIC_VALUE);
	}
}

Test(gnix_hashtable_advanced, insert_8K_lookup_128K_random)
{
	int ret, i, index;
	gnix_test_element_t *test_elements;
	gnix_test_element_t *found = NULL, *to_find = NULL;
	gnix_test_element_t *item;
	gnix_bitmap_t allocated = {0};
	int test_size = 8 * 1024;
	int bitmap_size = 64 * test_size;
	int lookups = 128 * 1024;

	test_elements = calloc(test_size, sizeof(gnix_test_element_t));
	cr_assert(test_elements != NULL);

	ret = _gnix_alloc_bitmap(&allocated, bitmap_size, NULL);
	cr_assert(ret == 0);

	srand(time(NULL));

	for (i = 0; i < test_size; ++i) {
		do {
			index = rand() % bitmap_size;
		} while (_gnix_test_and_set_bit(&allocated, index));

		item = &test_elements[i];

		item->key = index;
		item->val = rand() % lookups;
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < test_size; ++i) {
		item = &test_elements[i];

		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == test_size);

	for (i = 0; i < lookups; ++i) {
		to_find = &test_elements[rand() % test_size];
		found = _gnix_ht_lookup(test_ht, to_find->key);
		cr_assert(found != NULL);
		cr_assert(found == to_find);
		cr_assert(found->magic == __GNIX_MAGIC_VALUE);
	}

	ret = _gnix_free_bitmap(&allocated);
	cr_expect(ret == 0);

	free(test_elements);
}

Test(gnix_hashtable_advanced, iterate)
{
	int ret, i;

	gnix_test_element_t test_elements[1024];
	gnix_test_element_t *item;
	char test_elements_found[1024] = {0};

	srand(time(NULL));

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];
		item->key = i;
		item->val = rand() % (1024 * 1024);
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];
		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	{
		GNIX_HASHTABLE_ITERATOR(test_ht, iter);

		for (i = 0; i < 1024; ++i) {
			item = (gnix_test_element_t *)
					_gnix_ht_iterator_next(&iter);
			cr_assert(item);
			cr_assert(!test_elements_found[item->key]);
			test_elements_found[item->key] = 1;
		}
	}

	for (i = 1023; i >= 0; --i) {
		item = &test_elements[i];
		cr_assert(i == item->key);

		ret = _gnix_ht_remove(test_ht,
				item->key);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == i);
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
}

Test(gnix_hashtable_locked, iterate)
{
	int ret, i;

	gnix_test_element_t test_elements[1024];
	gnix_test_element_t *item;
	char test_elements_found[1024] = {0};

	srand(time(NULL));

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];
		item->key = i;
		item->val = rand() % (1024 * 1024);
		item->magic = __GNIX_MAGIC_VALUE;
	}

	for (i = 0; i < 1024; ++i) {
		item = &test_elements[i];
		ret = _gnix_ht_insert(test_ht,
				item->key, item);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == (i + 1));
	}

	{
		GNIX_HASHTABLE_ITERATOR(test_ht, iter);

		for (i = 0; i < 1024; ++i) {
			item = (gnix_test_element_t *)
					_gnix_ht_iterator_next(&iter);
			cr_assert(item);
			cr_assert(!test_elements_found[item->key]);
			test_elements_found[item->key] = 1;
		}
	}

	for (i = 1023; i >= 0; --i) {
		item = &test_elements[i];
		cr_assert(i == item->key);

		ret = _gnix_ht_remove(test_ht,
				item->key);
		cr_assert(ret == 0);
		cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == i);
	}

	cr_assert(ofi_atomic_get32(&test_ht->ht_elements) == 0);
}
