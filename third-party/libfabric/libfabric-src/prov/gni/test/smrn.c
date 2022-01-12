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


#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "gnix_smrn.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "common.h"

#define GNIX_DEFAULT_RQ_CNT 4

#if HAVE_KDREG
# define KDREG_CHECK false
#else
# define KDREG_CHECK true
#endif

static struct gnix_smrn *smrn;
static struct gnix_smrn_rq *rqs[GNIX_DEFAULT_RQ_CNT];
static void **memory_regions;

static void smrn_setup(void)
{
	int ret;
	int i;
	struct fi_info *info;

	ret = fi_getinfo(FI_VERSION(1, 5), NULL, NULL, 0, NULL, &info);
	cr_assert_eq(ret, FI_SUCCESS);

	fi_freeinfo(info);

	ret = _gnix_smrn_init();
	cr_assert_eq(ret, 0, "_gnix_smrn_init failed, ret=%d\n", ret);

	ret = _gnix_smrn_open(&smrn);
	cr_assert(ret == 0, "_gnix_smrn_open failed");

	for (i = 0; i < GNIX_DEFAULT_RQ_CNT; i++) {
		rqs[i] = calloc(1, sizeof(*rqs[i]));
		cr_assert_neq(rqs[i], NULL);

		fastlock_init(&rqs[i]->lock);
		dlist_init(&rqs[i]->list);
		dlist_init(&rqs[i]->entry);
	}
}

static void smrn_teardown(void)
{
	int ret;
	int i;

	ret = _gnix_smrn_close(smrn);
	cr_assert(ret == 0, "_gnix_smrn_close failed");

	for (i = 0; i < GNIX_DEFAULT_RQ_CNT; i++) {
		free(rqs[i]);
		rqs[i] = NULL;
	}
}

TestSuite(smrn,
	  .init = smrn_setup,
	  .fini = smrn_teardown,
	  .disabled = KDREG_CHECK);

#define RQ_ENTRIES 21
#define REGIONS (GNIX_DEFAULT_RQ_CNT * RQ_ENTRIES)
struct test_structure {
	struct gnix_smrn_context context;
	int pending;
};

Test(smrn, simple)
{
	const int regions = REGIONS;
	void *addresses[REGIONS];
	int i;
	int len = 8129;
	int ret;
	struct gnix_smrn_rq *rq;
	struct test_structure contexts[REGIONS] = {0};
	struct gnix_smrn_context *current;
	struct test_structure *iter;
	int expected_events;

	for (i = 0; i < regions; i++)
		dlist_init(&contexts[i].context.entry);

	for (i = 0; i < regions; i++) {
		addresses[i] = mmap(NULL, len, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_SHARED, -1, 0);
		cr_assert_neq(addresses[i], MAP_FAILED);
	}

	for (i = 0; i < regions; i++) {
		rq = rqs[i / (regions >> 2)];

		ret = _gnix_smrn_monitor(smrn, rq,
			addresses[i], len, (uint64_t) &contexts[i],
			&contexts[i].context);
		cr_assert_eq(ret, FI_SUCCESS);
	}


	for (i = 0; i < regions; i++) {
		ret = munmap(addresses[i], len);
		cr_assert_eq(ret, 0);

		contexts[i].pending = 1;
	}

	expected_events = regions;
	while (expected_events > 0) {
		for (i = 0; i < GNIX_DEFAULT_RQ_CNT; i++) {
			rq = rqs[i];

			ret = _gnix_smrn_get_event(smrn, rq, &current);
			if (ret == -FI_EAGAIN)
				continue;

			cr_assert_neq(ret, -FI_EINVAL);

			iter = container_of(current,
				struct test_structure, context);

			cr_assert_eq(iter->pending, 1);
			iter->pending = 0;

			expected_events -= 1;
		}
	}
}


static void *thread_func(void *context)
{
	const int regions = RQ_ENTRIES;
	void **addresses;
	int i;
	int len = 8129;
	int ret;
	struct gnix_smrn_rq *rq;
	struct test_structure contexts[RQ_ENTRIES] = {0};
	struct gnix_smrn_context *current;
	struct test_structure *iter;
	int expected_events;
	int id = *(int *) context;

	addresses = &memory_regions[id * RQ_ENTRIES];
	rq = rqs[id];

	for (i = 0; i < regions; i++)
		dlist_init(&contexts[i].context.entry);

	for (i = 0; i < regions; i++) {
		ret = _gnix_smrn_monitor(smrn, rq,
			addresses[i], len, (uint64_t) &contexts[i],
			&contexts[i].context);
		cr_assert_eq(ret, FI_SUCCESS);
	}


	for (i = 0; i < regions; i++) {
		ret = munmap(addresses[i], len);
		cr_assert_eq(ret, 0);

		contexts[i].pending = 1;
	}

	expected_events = regions;
	while (expected_events > 0) {
		ret = _gnix_smrn_get_event(smrn, rq, &current);
		if (ret == -FI_EAGAIN)
			continue;
		cr_assert_neq(ret, -FI_EINVAL);

		iter = container_of(current, struct test_structure, context);

		cr_assert_eq(iter->pending, 1);
		iter->pending = 0;

		expected_events -= 1;
	}

	pthread_exit(NULL);
}


Test(smrn, threaded)
{
	const int regions = REGIONS;
	void *addresses[REGIONS];
	int i;
	int len = 8129;
	int ret;
	pthread_t threads[GNIX_DEFAULT_RQ_CNT];
	int thread_ids[GNIX_DEFAULT_RQ_CNT];

	memory_regions = (void **) &addresses;

	for (i = 0; i < GNIX_DEFAULT_RQ_CNT; i++) {
		thread_ids[i] = i;
	}

	for (i = 0; i < regions; i++) {
		addresses[i] = mmap(NULL, len, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_SHARED, -1, 0);
		cr_assert_neq(addresses[i], MAP_FAILED);
	}

	for (i = 0; i < GNIX_DEFAULT_RQ_CNT; i++) {
		ret = pthread_create(&threads[i], NULL, thread_func,
				(void *) &thread_ids[i]);
		cr_assert_eq(ret, 0);
	}

	for (i = 0; i < GNIX_DEFAULT_RQ_CNT; i++) {
		pthread_join(threads[i], NULL);
	}

	memory_regions = NULL;
}



