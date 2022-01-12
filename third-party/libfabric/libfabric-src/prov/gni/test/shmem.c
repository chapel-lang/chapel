/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
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
 *      Author: jswaro
 *		Created-on: 6/19/2017
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

#include <gnix_shmem.h>
#include "common.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"

struct test_structure {
	char name[128];
	int elements;
	int data[0];
};

static struct fi_info *info;

static void __gnix_shmem_test_setup(void)
{
	int ret;

	ret  = fi_getinfo(fi_version(), NULL, 0, 0, NULL, &info);
	cr_assert(ret == FI_SUCCESS); /* do this to initialize debug output */
}

static void __gnix_shmem_test_teardown(void)
{
	fi_freeinfo(info);
}

static int init_test_structure_function(const char *path,
	uint32_t size, void *region)
{
	struct test_structure *tmp = (struct test_structure *) region;
	int i;

	tmp->elements = (size - sizeof(struct test_structure)) / sizeof(int);
	for (i = 0; i < tmp->elements; i++)
		tmp->data[i] = 0;

	return 0;
}

TestSuite(gnix_shmem,
		.init = __gnix_shmem_test_setup,
		.fini = __gnix_shmem_test_teardown);

static inline void basic_test(void)
{
	int size, ret, elements, i;
	struct test_structure *region;
	struct gnix_shared_memory _gnix_shmem_region;

	elements = 256;
	size = sizeof(struct test_structure) + (sizeof(int) * elements);

	ret = _gnix_shmem_create("/tmp/libfabric_key_region",
			size, init_test_structure_function,
			&_gnix_shmem_region);
	cr_assert(ret == FI_SUCCESS, "ret=%d\n", ret);

	region = (struct test_structure *) _gnix_shmem_region.addr;

	cr_assert(region);
	cr_assert(region->elements == elements);
	for (i = 0; i < elements; i++)
		cr_assert(region->data[i] == 0);

	for (i = 0; i < elements; i++)
		region->data[i] = i;

	for (i = 0; i < elements; i++)
		cr_assert(region->data[i] == i);

	ret = _gnix_shmem_destroy(&_gnix_shmem_region);
	cr_assert(ret == FI_SUCCESS);
}

/* test is redundant to ensure that the shared memory file is removed between
 * application runs */
Test(gnix_shmem, basic_use_2)
{
	basic_test();
}

/* test is redundant to ensure that the shared memory file is removed between
 * application runs */
Test(gnix_shmem, basic_use_1)
{
	basic_test();
}


