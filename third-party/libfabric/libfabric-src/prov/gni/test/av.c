/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2019-2020 Triad National Security, LLC.
 *                         All rights reserved.
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
#include <stddef.h>
#include <stdlib.h>

#include "ofi.h"
#include "rdma/fi_domain.h"

#include "gnix.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct fi_info *hints;
static struct fi_info *fi;
static struct gnix_ep_name *fake_names;
static struct fid_av *av;
static struct gnix_fid_av *gnix_av;


#define SIMPLE_EP_ENTRY(id) \
{	\
	.gnix_addr = { \
			.device_addr = id, \
			.cdm_id = id+1, \
	}, \
	.name_type = id+2, \
	.cm_nic_cdm_id = id+3, \
	.cookie = id+4, \
}

#define MT_ADDR_COUNT 59
#define SIMPLE_ADDR_COUNT 16
static struct gnix_ep_name simple_ep_names[MT_ADDR_COUNT] = {
		SIMPLE_EP_ENTRY(1),
		SIMPLE_EP_ENTRY(2),
		SIMPLE_EP_ENTRY(3),
		SIMPLE_EP_ENTRY(4),
		SIMPLE_EP_ENTRY(5),
		SIMPLE_EP_ENTRY(6),
		SIMPLE_EP_ENTRY(7),
		SIMPLE_EP_ENTRY(8),
		SIMPLE_EP_ENTRY(9),
		SIMPLE_EP_ENTRY(10),
		SIMPLE_EP_ENTRY(11),
		SIMPLE_EP_ENTRY(12),
		SIMPLE_EP_ENTRY(13),
		SIMPLE_EP_ENTRY(14),
		SIMPLE_EP_ENTRY(15),
		SIMPLE_EP_ENTRY(16),
		SIMPLE_EP_ENTRY(17),
		SIMPLE_EP_ENTRY(18),
		SIMPLE_EP_ENTRY(19),
		SIMPLE_EP_ENTRY(20),
		SIMPLE_EP_ENTRY(21),
		SIMPLE_EP_ENTRY(22),
		SIMPLE_EP_ENTRY(23),
		SIMPLE_EP_ENTRY(24),
		SIMPLE_EP_ENTRY(25),
		SIMPLE_EP_ENTRY(26),
		SIMPLE_EP_ENTRY(27),
		SIMPLE_EP_ENTRY(28),
		SIMPLE_EP_ENTRY(29),
		SIMPLE_EP_ENTRY(30),
		SIMPLE_EP_ENTRY(31),
		SIMPLE_EP_ENTRY(32),
		SIMPLE_EP_ENTRY(33),
		SIMPLE_EP_ENTRY(34),
		SIMPLE_EP_ENTRY(35),
		SIMPLE_EP_ENTRY(36),
		SIMPLE_EP_ENTRY(37),
		SIMPLE_EP_ENTRY(38),
		SIMPLE_EP_ENTRY(39),
		SIMPLE_EP_ENTRY(40),
		SIMPLE_EP_ENTRY(41),
		SIMPLE_EP_ENTRY(42),
		SIMPLE_EP_ENTRY(43),
		SIMPLE_EP_ENTRY(44),
		SIMPLE_EP_ENTRY(45),
		SIMPLE_EP_ENTRY(46),
		SIMPLE_EP_ENTRY(47),
		SIMPLE_EP_ENTRY(48),
		SIMPLE_EP_ENTRY(49),
		SIMPLE_EP_ENTRY(50),
		SIMPLE_EP_ENTRY(51),
		SIMPLE_EP_ENTRY(52),
		SIMPLE_EP_ENTRY(53),
		SIMPLE_EP_ENTRY(54),
		SIMPLE_EP_ENTRY(55),
		SIMPLE_EP_ENTRY(56),
		SIMPLE_EP_ENTRY(57),
		SIMPLE_EP_ENTRY(58),
		SIMPLE_EP_ENTRY(59),
};

static void av_setup(void)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert_eq(ret, FI_SUCCESS, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_domain");

}

static void av_teardown(void)
{
	int ret = 0;

	ret = fi_close(&dom->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failure in closing domain.");
	ret = fi_close(&fab->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failure in closing fabric.");
	fi_freeinfo(fi);
	fi_freeinfo(hints);
}


static void av_full_map_setup(void)
{
	struct fi_av_attr av_table_attr = {
		.type = FI_AV_MAP,
		.count = 16,
	};
	int ret;

	av_setup();

	ret = fi_av_open(dom, &av_table_attr, &av, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "failed to open av");

	gnix_av = container_of(av, struct gnix_fid_av, av_fid);
}

static void av_full_map_teardown(void)
{
	int ret;

	ret = fi_close(&av->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failed to close av");

	av_teardown();
}

static void av_full_table_setup(void)
{
	struct fi_av_attr av_table_attr = {
		.type = FI_AV_TABLE,
		.count = 16,
	};
	int ret;

	av_setup();

	ret = fi_av_open(dom, &av_table_attr, &av, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "failed to open av");

	gnix_av = container_of(av, struct gnix_fid_av, av_fid);
}

static void av_full_table_teardown(void)
{
	int ret;

	ret = fi_close(&av->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failed to close av");

	av_teardown();
}

TestSuite(av_bare, .init = av_setup, .fini = av_teardown, .disabled = false);

TestSuite(av_full_map, .init = av_full_map_setup,
		.fini = av_full_map_teardown, .disabled = false);

TestSuite(av_full_table, .init = av_full_table_setup,
		.fini = av_full_table_teardown, .disabled = false);

static void invalid_addrlen_pointer_test(void)
{
	int ret;
	fi_addr_t address = 0xdeadbeef;
	void *addr = (void *) 0xb00fbabe;

	/* while the pointers to address and addr aren't valid, they are
	 * acceptable as stated by the manpage. This will only test for a
	 * proper return code from fi_av_lookup()
	 */
	ret = fi_av_lookup(av, address, addr, NULL);
	cr_assert_eq(ret, -FI_EINVAL);
}

Test(av_full_map, invalid_addrlen_pointer)
{
	invalid_addrlen_pointer_test();
}

Test(av_full_table, invalid_addrlen_pointer)
{
	invalid_addrlen_pointer_test();
}

static void remove_addr_test(void)
{
	int ret;
	int i;
	fi_addr_t addresses[SIMPLE_ADDR_COUNT];
	fi_addr_t *compare;

	/* insert addresses */
	ret = fi_av_insert(av, (void *) simple_ep_names, SIMPLE_ADDR_COUNT,
			   addresses, 0, NULL);
	cr_assert_eq(ret, SIMPLE_ADDR_COUNT);

	/* check address contents */
	for (i = 0; i < SIMPLE_ADDR_COUNT; i++) {
		if (gnix_av->type == FI_AV_MAP) {
			compare = (fi_addr_t *) &simple_ep_names[i].gnix_addr;
			cr_assert_eq(*compare, addresses[i]);
		} else {
			cr_assert_eq(i, addresses[i]);
		}
	}

	/* remove addresses */
	ret = fi_av_remove(av, addresses, SIMPLE_ADDR_COUNT, 0);
	cr_assert_eq(ret, FI_SUCCESS);
}

Test(av_full_map, remove_addr)
{
	remove_addr_test();
}

Test(av_full_table, remove_addr)
{
	remove_addr_test();
}

static void addr_insert_test(void)
{
	int i, ret;
	fi_addr_t addresses[SIMPLE_ADDR_COUNT];

	int err[SIMPLE_ADDR_COUNT] = {0};

	cr_log_info("check for sync err flag but no context\n");
	ret = fi_av_insert(av, (void *) simple_ep_names, SIMPLE_ADDR_COUNT,
			   addresses, FI_SYNC_ERR, NULL);
	cr_assert_eq(ret, -FI_EINVAL, "%d", ret);

	ret = fi_av_insert(av, (void *) simple_ep_names, SIMPLE_ADDR_COUNT,
			   addresses, FI_SYNC_ERR, err);
	cr_assert_eq(ret, SIMPLE_ADDR_COUNT);

	cr_log_info("check for errors\n");
	for (i = 0; i < SIMPLE_ADDR_COUNT; i++) {
		cr_assert_eq(err[i], 0);
	}

	ret = fi_av_remove(av, addresses, SIMPLE_ADDR_COUNT, 0);
	cr_assert_eq(ret, FI_SUCCESS);
}

static void addr_insert_null_fi_addr_test(void)
{
	int ret;
	fi_addr_t *addresses = NULL;

	ret = fi_av_insert(av, (void *) simple_ep_names, SIMPLE_ADDR_COUNT,
			   addresses, 0, NULL);
	cr_assert_eq(ret, -FI_EINVAL, "%d", ret);

}

Test(av_full_map, insert_addr)
{
	addr_insert_test();
	addr_insert_null_fi_addr_test();
}

Test(av_full_table, insert_addr)
{
	addr_insert_test();
}

static void lookup_invalid_test(void)
{
	int ret;
	struct gnix_ep_name addr;
	size_t addrlen = sizeof(struct gnix_ep_name);

	/* test null addrlen */
	ret = fi_av_lookup(av, 0xdeadbeef, (void *) 0xdeadbeef, NULL);
	cr_assert_eq(ret, -FI_EINVAL);

	/* test null addr */
	ret = fi_av_lookup(av, 0xdeadbeef, NULL, &addrlen);
	cr_assert_eq(ret, -FI_EINVAL);

	/* test invalid lookup */
	if (gnix_av->type == FI_AV_TABLE) {
		ret = fi_av_lookup(av, 2000, &addr, &addrlen);
		cr_assert_eq(ret, -FI_EINVAL);

		/* test within range, but not inserted case */
		ret = fi_av_lookup(av, 1, &addr, &addrlen);
		cr_assert_eq(ret, -FI_EINVAL);
	} else {
		ret = fi_av_lookup(av, 0xdeadbeef, &addr, &addrlen);
		cr_assert_eq(ret, -FI_ENOENT);
	}
}

Test(av_full_map, lookup_invalid)
{
	lookup_invalid_test();
}

Test(av_full_table, lookup_invalid)
{
	lookup_invalid_test();
}

static void lookup_test(void)
{
	int ret;
	int i;
	fi_addr_t addresses[SIMPLE_ADDR_COUNT];
	fi_addr_t *compare;
	struct gnix_ep_name found;
	size_t addrlen = sizeof(struct gnix_ep_name);

	/* insert addresses */
	ret = fi_av_insert(av, (void *) simple_ep_names, SIMPLE_ADDR_COUNT,
			addresses, 0, NULL);
	cr_assert_eq(ret, SIMPLE_ADDR_COUNT);

	/* check address contents */
	for (i = 0; i < SIMPLE_ADDR_COUNT; i++) {
		if (gnix_av->type == FI_AV_MAP) {
			compare = (fi_addr_t *) &simple_ep_names[i].gnix_addr;
			cr_assert_eq(*compare, addresses[i]);
		} else {
			cr_assert_eq(i, addresses[i]);
		}
	}

	for (i = 0; i < SIMPLE_ADDR_COUNT; i++) {
		ret = fi_av_lookup(av, addresses[i], &found, &addrlen);
		cr_assert_eq(ret, FI_SUCCESS);
	}
}

Test(av_full_map, lookup)
{
	lookup_test();
}

Test(av_full_table, lookup)
{
	lookup_test();
}

/* Stuff for mulithreaded tests */
static pthread_barrier_t mtbar;

/* Currently the AV operations are not thread safe, so use this big
 * fat lock when calling them */
#define USE_LOCK
#ifdef USE_LOCK
static fastlock_t my_big_lock;
#define init_av_lock() fastlock_init(&my_big_lock)
#define av_lock() fastlock_acquire(&my_big_lock)
#define av_unlock() fastlock_release(&my_big_lock)
#else
#define init_av_lock()
#define av_lock()
#define av_unlock()
#endif

static void *insert_single(void *data)
{
	int ret;
	int n = (int) ((int *) data)[0];
	fi_addr_t *addr = ((fi_addr_t **) data)[1];

	ret = pthread_barrier_wait(&mtbar);
	if ((ret != PTHREAD_BARRIER_SERIAL_THREAD) && (ret != 0)) {
		pthread_exit((void *) 1UL);
	}

	av_lock();
	ret = fi_av_insert(av, (void *) &simple_ep_names[n], 1, addr, 0, NULL);
	av_unlock();

	if (ret != 1) {
		pthread_exit((void *) 2UL);
	}

	pthread_exit((void *) 0UL);
}

static void *remove_single(void *data)
{
	int ret;
	fi_addr_t *addr = (fi_addr_t *) data;

	ret = pthread_barrier_wait(&mtbar);
	if ((ret != PTHREAD_BARRIER_SERIAL_THREAD) && (ret != 0)) {
		pthread_exit((void *) 1UL);
	}

	av_lock();
	ret = fi_av_remove(av, addr, 1, 0);
	av_unlock();

	if (ret != FI_SUCCESS) {
		pthread_exit((void *) 2UL);
	}

	pthread_exit((void *) 0UL);
}

static void *lookup_single(void *data)
{
	int ret;
	struct gnix_ep_name found;
	size_t addrlen = sizeof(struct gnix_ep_name);
	fi_addr_t *addr = (fi_addr_t *) data;

	ret = pthread_barrier_wait(&mtbar);
	if ((ret != PTHREAD_BARRIER_SERIAL_THREAD) && (ret != 0)) {
		pthread_exit((void *) 1UL);
	}

	av_lock();
	ret = fi_av_lookup(av, *addr, &found, &addrlen);
	av_unlock();

	if (ret != FI_SUCCESS) {
		pthread_exit((void *) 2UL);
	}

	pthread_exit((void *) 0UL);
}

static void simple_mt_test(void)
{
	int ret;
	unsigned long pret;
	int i, j;
	fi_addr_t *compare;
	pthread_t threads[MT_ADDR_COUNT];
	fi_addr_t addresses[MT_ADDR_COUNT];
	bool found_addresses[MT_ADDR_COUNT];
	void *info[MT_ADDR_COUNT][2];

	ret = pthread_barrier_init(&mtbar, NULL, MT_ADDR_COUNT);
	cr_assert_eq(ret, 0);

	init_av_lock();

	/* insert addresses */
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		info[i][0] = (void *) (uint64_t) i;
		info[i][1] = (void *) &addresses[i];
		ret = pthread_create(&threads[i], NULL,
				     insert_single, &info[i]);
		cr_assert_eq(ret, 0);
	}
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		ret = pthread_join(threads[i], (void **) &pret);
		cr_assert_eq(ret, 0);
		cr_assert_eq(pret, 0UL);
	}

	for (i = 0; i < MT_ADDR_COUNT; i++) {
		found_addresses[i] = false;
	}

	/* check address contents */
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		if (gnix_av->type == FI_AV_MAP) {
			for (j = 0; j < MT_ADDR_COUNT; j++) {
				compare = (fi_addr_t *)
					&simple_ep_names[j].gnix_addr;
				if (addresses[i] == *compare) {
					found_addresses[j] = true;
				}
			}
		} else {
			found_addresses[addresses[i]] = true;
		}
	}
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		cr_assert_eq(found_addresses[i], true);
	}

	/* look up addresses */
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		ret = pthread_create(&threads[i], NULL, lookup_single,
				     (void *) &addresses[i]);
		cr_assert_eq(ret, 0);
	}

	for (i = 0; i < MT_ADDR_COUNT; i++) {
		ret = pthread_join(threads[i], (void **) &pret);
		cr_assert_eq(ret, 0);
		cr_assert_eq(pret, 0UL);
	}

	/* remove addresses */
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		ret = pthread_create(&threads[i], NULL, remove_single,
				     (void *) &addresses[i]);
		cr_assert_eq(ret, 0);
	}
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		ret = pthread_join(threads[i], (void **) &pret);
		cr_assert_eq(ret, 0);
		cr_assert_eq(pret, 0UL);
	}

	ret = pthread_barrier_destroy(&mtbar);
	cr_assert_eq(ret, 0);
}

Test(av_full_map, mt_simple)
{
	simple_mt_test();
}

Test(av_full_table, mt_simple)
{
	simple_mt_test();
}

#include "ofi_atom.h"
/* add a compare-and-swap */
static inline int atomic_cas_weak(ofi_atomic32_t *atomic, int *expected, int desired)
{
	ATOMIC_IS_INITIALIZED(atomic);
	return atomic_compare_exchange_weak_explicit(&atomic->val,
						     expected, desired,
						     memory_order_seq_cst,
						     memory_order_seq_cst);
}

static void *lookup_random(void *data)
{
	int n, ret;
	fi_addr_t *addresses = ((fi_addr_t **) data)[0];
	ofi_atomic32_t *done = ((ofi_atomic32_t **) data)[1];
	struct gnix_ep_name found;
	size_t addrlen = sizeof(struct gnix_ep_name);

	srand(0);

	ret = pthread_barrier_wait(&mtbar);
	if ((ret != PTHREAD_BARRIER_SERIAL_THREAD) && (ret != 0)) {
		pthread_exit((void *) 1UL);
	}

	while (!ofi_atomic_get32(done)) {
		n = rand()%MT_ADDR_COUNT;
		(void) fi_av_lookup(av, addresses[n], &found, &addrlen);
	}

	pthread_exit(NULL);
}

static void continuous_lookup(void)
{
	int i, ret;
	pthread_t thread;
	fi_addr_t addresses[MT_ADDR_COUNT];
	ofi_atomic32_t done;
	void *info[2];
	const int iters = 17;

	ret = pthread_barrier_init(&mtbar, NULL, 2);
	cr_assert_eq(ret, 0);

	init_av_lock();

	ofi_atomic_initialize32(&done, 0);

	memset(addresses, 0, MT_ADDR_COUNT*sizeof(fi_addr_t));

	info[0] = (void *) addresses;
	info[1] = (void *) &done;

	ret = pthread_create(&thread, NULL, lookup_random, info);
	cr_assert_eq(ret, 0);

	ret = pthread_barrier_wait(&mtbar);
	cr_assert((ret == PTHREAD_BARRIER_SERIAL_THREAD) || (ret == 0));

	for (i = 0; i < iters; i++) {
		for (i = 0; i < MT_ADDR_COUNT; i++) {
			ret = fi_av_insert(av, (void *) &simple_ep_names[i], 1,
					   &addresses[i], 0, NULL);
			cr_assert_eq(ret, 1);
		}
		for (i = 0; i < MT_ADDR_COUNT; i++) {
			ret = fi_av_remove(av, &addresses[i], 1, 0);
			cr_assert_eq(ret, FI_SUCCESS);
		}
	}

	ofi_atomic_set32(&done, 1);

	ret = pthread_join(thread, NULL);
	cr_assert_eq(ret, 0);

	ret = pthread_barrier_destroy(&mtbar);
	cr_assert_eq(ret, 0);
}

Test(av_full_map, mt_lookup)
{
	continuous_lookup();
}

Test(av_full_table, mt_lookup)
{
	continuous_lookup();
}

static const int state_empty = 1;
static const int state_full = 2;
static const int state_locked = 3;

static void *continuous_insert(void *data)
{
	int i, pos, n, ret;
	int expected_state;
	struct gnix_ep_name *ep_names = ((struct gnix_ep_name **) data)[0];
	fi_addr_t *addresses = ((fi_addr_t **) data)[1];
	ofi_atomic32_t *fe = ((ofi_atomic32_t **) data)[2];
	int num_insertions = (int) ((uint64_t *) data)[3];
	int num_addrs = (int) ((uint64_t *) data)[4];
	ofi_atomic32_t *done = ((ofi_atomic32_t **) data)[5];

	ret = pthread_barrier_wait(&mtbar);
	if ((ret != PTHREAD_BARRIER_SERIAL_THREAD) && (ret != 0)) {
		pthread_exit((void *) 1UL);
	}

	i = 0;
	pos = 0;
	while ((i < num_insertions) && !ofi_atomic_get32(done)) {
		n = (pos++)%num_addrs;
		expected_state = state_empty;
		if (atomic_cas_weak(&fe[n], &expected_state, state_locked)) {
			av_lock();
			ret = fi_av_insert(av, (void *) &ep_names[n], 1,
					   &addresses[n], 0, NULL);
			av_unlock();
			if (ret != 1) {
				/* flag shutdown to avoid deadlock */
				ofi_atomic_set32(done, 1);
				pthread_exit((void *) 1UL);
			}
			ofi_atomic_set32(&fe[n], state_full);
			i++;
		}
	}

	pthread_exit((void *) NULL);
}

static void *continuous_remove(void *data)
{
	int pos, n, ret;
	int expected_state;
	fi_addr_t *addresses = ((fi_addr_t **) data)[0];
	ofi_atomic32_t *fe = ((ofi_atomic32_t **) data)[1];
	int num_addrs = (int) ((uint64_t *) data)[2];
	ofi_atomic32_t *done = ((ofi_atomic32_t **) data)[3];

	ret = pthread_barrier_wait(&mtbar);
	if ((ret != PTHREAD_BARRIER_SERIAL_THREAD) && (ret != 0)) {
		pthread_exit((void *) 1UL);
	}

	pos = 0;
	while (!ofi_atomic_get32(done)) {
		n = (pos++)%num_addrs;
		expected_state = state_full;
		if (atomic_cas_weak(&fe[n], &expected_state, state_locked)) {
			av_lock();
			ret = fi_av_remove(av, &addresses[n], 1, 0);
			av_unlock();
			if (ret != FI_SUCCESS) {
				/* flag shutdown to avoid deadlock */
				ofi_atomic_set32(done, 1);
				pthread_exit((void *) 1UL);
			}
			ofi_atomic_set32(&fe[n], state_empty);
		}
	}

	pthread_exit((void *) NULL);
}

static void continuous_insert_remove(int num_inserters, int num_removers,
				     int num_insertions)
{
	int i, ret;
	unsigned long pret;
	ofi_atomic32_t done;
	fi_addr_t addresses[MT_ADDR_COUNT];
	ofi_atomic32_t fe[MT_ADDR_COUNT];
	const int addrs_per_thread = MT_ADDR_COUNT/num_inserters;
	const int num_threads = num_inserters + num_removers;
	pthread_t threads[num_threads];
	void *info[num_threads][6];

	ret = pthread_barrier_init(&mtbar, NULL, num_threads);
	cr_assert_eq(ret, 0);

	init_av_lock();

	ofi_atomic_initialize32(&done, 0);
	for (i = 0; i < MT_ADDR_COUNT; i++) {
		ofi_atomic_initialize32(&fe[i], state_empty);
	}

	for (i = 0; i < num_inserters; i++) {
		info[i][0] = (void *) &simple_ep_names[i*addrs_per_thread];
		info[i][1] = (void *) &addresses[i*addrs_per_thread];
		info[i][2] = (void *) &fe[i*addrs_per_thread];
		info[i][3] = (void *) (uint64_t) num_insertions;
		info[i][4] = (void *) (uint64_t) addrs_per_thread;
		info[i][5] = (void *) &done;
		ret = pthread_create(&threads[i], NULL,
				     continuous_insert, &info[i]);
		cr_assert_eq(ret, 0);
	}

	for (i = num_inserters; i < num_threads; i++) {
		info[i][0] = (void *) addresses;
		info[i][1] = (void *) fe;
		info[i][2] = (void *) (uint64_t)
			(num_inserters*addrs_per_thread);
		info[i][3] = (void *) &done;
		ret = pthread_create(&threads[i], NULL,
				     continuous_remove, &info[i]);
		cr_assert_eq(ret, 0);
	}

	for (i = 0; i < num_threads; i++) {
		if (i == num_inserters) {
			ofi_atomic_set32(&done, 1);
		}
		ret = pthread_join(threads[i], (void **) &pret);
		cr_assert_eq(ret, 0);
		cr_assert_eq(pret, 0UL, "thread %d failed\n", i);
	}

	ret = pthread_barrier_destroy(&mtbar);
	cr_assert_eq(ret, 0);
}

Test(av_full_map, mt_insert_remove)
{
	continuous_insert_remove(8, 1, 113);
	continuous_insert_remove(4, 3, 113);
	continuous_insert_remove(29, 13, 113);
}

Test(av_full_table, mt_insert_remove)
{
	continuous_insert_remove(8, 1, 113);
	continuous_insert_remove(4, 3, 113);
	continuous_insert_remove(29, 13, 113);
}

static void straddr_test(void)
{
	const char *buf;
#define ADDRSTR_LEN 128
	char addrstr[ADDRSTR_LEN];
	size_t addrstr_len;
	char *pend;
	long int value;

	addrstr_len = 10; /* too short */
	buf = fi_av_straddr(av, &simple_ep_names[0], addrstr, &addrstr_len);
	cr_assert_eq(buf, addrstr);
	cr_assert_eq(addrstr_len, 10);

	addrstr_len = ADDRSTR_LEN;
	buf = fi_av_straddr(av, &simple_ep_names[0], addrstr, &addrstr_len);
	cr_assert_eq(buf, addrstr);
	cr_assert_eq(addrstr_len, GNIX_AV_MAX_STR_ADDR_LEN);

	/* extract the first component */
	buf = strtok(addrstr, ":");
	cr_assert_not_null(buf, "version not found");

	value = strtol(buf, &pend, 16);

	/* verify the version has been returned. */
	cr_assert_eq(GNIX_AV_STR_ADDR_VERSION, value, "Invalid version");

	/* extract the second component */
	buf = strtok(NULL, ":");
	cr_assert_not_null(buf, "device_addr not found");

	value = strtol(buf, &pend, 16);

	/* verify the device addrstr has been returned. */
	cr_assert_eq(simple_ep_names[0].gnix_addr.device_addr, value,
		    "Invalid device_addr");

	/* extract the third component */
	buf = strtok(NULL, ":");
	cr_assert_not_null(buf, "cdm_id not found");

	value = strtol(buf, &pend, 16);

	/* verify the cdm_id has been returned. */
	cr_assert_eq(simple_ep_names[0].gnix_addr.cdm_id, value,
		     "Invalid cdm_id");

	/* extract the fourth component */
	buf = strtok(NULL, ":");
	cr_assert_not_null(buf, "name_type not found");

	value = strtol(buf, &pend, 10);

	/* verify the name_type has been returned. */
	cr_assert_eq(simple_ep_names[0].name_type, value, "Invalid name_type");

	/* extract the fifth component */
	buf = strtok(NULL, ":");
	cr_assert_not_null(buf, "cm_nic_cdm_id not found");

	value = strtol(buf, &pend, 16);

	/* verify the cm_nic_cdm_id has been returned. */
	cr_assert_eq(simple_ep_names[0].cm_nic_cdm_id, value,
		     "Invalid cm_nic_cdm_id");

	/* extract the sixth component */
	buf = strtok(NULL, ":");
	cr_assert_not_null(buf, "cookie not found");

	value = strtol(buf, &pend, 16);

	/* verify the cookie has been returned. */
	cr_assert_eq(simple_ep_names[0].cookie, value, "Invalid cookie");

	/* extract the seventh component */
	buf = strtok(NULL, ":");
	cr_assert_not_null(buf, "number of contexts not found");

	value = strtol(buf, &pend, 10);

	/* verify the rx_ctx_cnt has been returned. */
	cr_assert_eq(simple_ep_names[0].rx_ctx_cnt, value,
		     "Invalid number of contexts");

	/* check to see if additional component are specified */
	buf = strtok(NULL, ":");
	cr_assert_null(buf, "extra values specified");
}

Test(av_full_map, straddr)
{
	straddr_test();
}

Test(av_full_table, straddr)
{
	straddr_test();
}

#define TABLE_SIZE_INIT  16
#define TABLE_SIZE_FINAL 1024

Test(av_bare, test_capacity)
{
	int ret, i;
	fi_addr_t addresses[TABLE_SIZE_FINAL];
	struct fi_av_attr av_table_attr = {
		.type = FI_AV_TABLE,
		.count = TABLE_SIZE_INIT,
	};

	ret = fi_av_open(dom, &av_table_attr, &av, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "failed to open av");

	fake_names = (struct gnix_ep_name *)calloc(TABLE_SIZE_FINAL,
						   sizeof(*fake_names));
	cr_assert_neq(fake_names, NULL);

	for (i = 0; i < TABLE_SIZE_INIT; i++) {
		fake_names[i].gnix_addr.device_addr = i + 100;
		fake_names[i].gnix_addr.cdm_id = i;
		fake_names[i].cm_nic_cdm_id = 0xbeef;
		fake_names[i].cookie = 0xdeadbeef;
	}

	ret = fi_av_insert(av, fake_names, TABLE_SIZE_INIT,
			   addresses, 0, NULL);
	cr_assert_eq(ret, TABLE_SIZE_INIT, "av insert failed");

	/*
	 * now add some more
	 */

	for (i = TABLE_SIZE_INIT; i < TABLE_SIZE_FINAL; i++) {
		fake_names[i].gnix_addr.device_addr = i + 100;
		fake_names[i].gnix_addr.cdm_id = i;
		fake_names[i].cm_nic_cdm_id = 0xbeef;
		fake_names[i].cookie = 0xdeadbeef;
	}

	ret = fi_av_insert(av, &fake_names[TABLE_SIZE_INIT],
			   TABLE_SIZE_FINAL - TABLE_SIZE_INIT,
			   &addresses[TABLE_SIZE_INIT], 0, NULL);
	cr_assert_eq(ret, TABLE_SIZE_FINAL - TABLE_SIZE_INIT,
		     "av insert failed");

}
