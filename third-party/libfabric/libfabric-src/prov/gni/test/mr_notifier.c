/*
 * Copyright (c) 2016-2018 Cray Inc. All rights reserved.
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
#include "gnix_mr_notifier.h"

#include "common.h"
#include <criterion/criterion.h>
#include "common.h"

static struct gnix_mr_notifier *mr_notifier;
static void mr_notifier_setup(void)
{
	int ret;

	ret = _gnix_notifier_init();
	cr_assert(ret == 0, "_gnix_notifier_init failed");

	ret = _gnix_notifier_open(&mr_notifier);
	cr_assert(ret == 0, "_gnix_notifier_open failed");
}

static void mr_notifier_teardown(void)
{
	int ret;

	ret = _gnix_notifier_close(mr_notifier);
	cr_assert(ret == 0, "_gnix_notifier_close failed");
}

TestSuite(mr_notifier,
	  .init = mr_notifier_setup,
	  .fini = mr_notifier_teardown);

static void
monitor_single(size_t len) {
	int ret;
	uint64_t cookie;
	char *mem = mmap(NULL, len, PROT_READ | PROT_WRITE,
			 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	cr_assert_neq(mem, MAP_FAILED, "Could not allocate %ld bytes\n", len);

	ret = _gnix_notifier_monitor(mr_notifier, mem, len, (uint64_t) mem);
	cr_assert(ret == 0, "_gnix_notifier_monitor failed");

	munmap(mem, len);

	ret = _gnix_notifier_get_event(mr_notifier, &cookie, sizeof(cookie));
	if (ret >= 0) {
		cr_assert(cookie == (uint64_t) mem,
			  "Unexpected cookie (got %lu, expected %lu)",
			  cookie, (uint64_t) mem);
		cr_assert(ret == sizeof(cookie),
			  "Unexpected number of bytes (got %d, expected %lu)",
			  ret, sizeof(cookie));
	} else if (ret == -FI_EAGAIN) {
		/* Nothing to read, ok */
		ret = _gnix_notifier_unmonitor(mr_notifier, (uint64_t) mem);
		cr_assert(ret == 0, "_gnix_notifier_unmonitor failed");
	} else {
		cr_assert(0, "Unexpected error");
	}
}

Test(mr_notifier, single)
{
	monitor_single(131);
	monitor_single(4099);
	monitor_single((1<<22) + 1);
	monitor_single((1<<26) - 1);
}

static void
monitor_multiple(size_t *lens, size_t num_lens)
{
	int i, ret;
	uint64_t cookie;
	char *mems[num_lens];
	bool unmapped[num_lens];

	for (i = 0; i < num_lens; i++) {
		mems[i] = mmap(NULL, lens[i], PROT_READ | PROT_WRITE,
			       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		cr_assert_neq(mems[i], MAP_FAILED,
			      "Could not allocate %ld bytes\n", lens[i]);

		ret = _gnix_notifier_monitor(mr_notifier, mems[i], lens[i],
					     (uint64_t) &mems[i]);
		cr_assert(ret == 0, "_gnix_notifier_monitor failed");
		unmapped[i] = false;
	}

	for (i = 0; i < num_lens; i++) {
		munmap(mems[i], lens[i]);
	}

	for (ret = _gnix_notifier_get_event(mr_notifier,
					    &cookie, sizeof(cookie));
	     ret > 0;
	     ret = _gnix_notifier_get_event(mr_notifier,
					    &cookie, sizeof(cookie))) {
		i = (int) (cookie - (uint64_t) mems)/8;
		cr_assert(cookie == (uint64_t) &mems[i],
			  "Unexpected cookie (got %lu, expected %lu)",
			  cookie, (uint64_t) &mems[i]);
		cr_assert(ret == sizeof(cookie),
			  "Unexpected number of bytes (got %d, expected %lu)",
			  ret, sizeof(cookie));
		unmapped[i] = true;
	}

	for (i = 0; i < num_lens; i++) {
		if (unmapped[i] == false) {
			ret = _gnix_notifier_unmonitor(mr_notifier,
						       (uint64_t) &mems[i]);
			cr_assert(ret == 0, "_gnix_notifier_unmonitor failed");
		}
	}
}

Test(mr_notifier, multiple)
{
	const int num_lens = 11;
	size_t lens[num_lens];

	lens[0] = 131;
	lens[1] = 4099;
	lens[2] = 8096;
	lens[3] = (1<<15)-1;
	lens[4] = 777;
	lens[5] = (1<<19)+1;
	lens[6] = 1<<20;
	lens[7] = 1<<20;
	lens[8] = 42;
	lens[9] = (1<<14)-1;
	lens[10] = (1<<21)+1;

	monitor_multiple(lens+5, 2);
	monitor_multiple(lens, num_lens);

}

#include <pthread.h>
#include "gnix_rdma_headers.h"
static struct fi_info *fi;
/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static uint64_t default_access = (FI_REMOTE_READ | FI_REMOTE_WRITE |
			   FI_READ | FI_WRITE);
static uint64_t default_flags;
static uint64_t default_req_key;
static uint64_t default_offset;

static pthread_t freer;

/* simple bounded buffer for 2 threads */
#include "ofi_atom.h"
#define buflen 23
static void *to_free_buf[buflen];
static ofi_atomic32_t head, tail;

static inline int next_head(void)
{
	int val = ofi_atomic_inc32(&head);

	while (ofi_atomic_get32(&tail)-buflen >= val) {
		pthread_yield();
	}
	return val%buflen;
}

static inline int next_tail(void)
{
	int val = ofi_atomic_inc32(&tail);

	while (ofi_atomic_get32(&head) <= val) {
		pthread_yield();
	}
	return val%buflen;
}

static void *do_free(void *data)
{
	int loc = next_tail();
	void *addr = to_free_buf[loc];

	while (addr != MAP_FAILED) {
		if (addr) {
			free(addr);
		}

		pthread_yield();

		loc = next_tail();
		addr = to_free_buf[loc];
	}
	return NULL;
}

static void mr_stressor_setup_common(void)
{
	int ret = 0;
	struct fi_info *hints;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	fi_freeinfo(hints);

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

}

static void mr_notifier_stressor_setup(void)
{
	int ret = 0;

	mr_stressor_setup_common();

	ofi_atomic_initialize32(&head, 0);
	ofi_atomic_initialize32(&tail, -1);

	ret = pthread_create(&freer, NULL, do_free, NULL);
	cr_assert_eq(ret, 0, "Could not create pthread");

	srand(0); /* want repeatable sequence, I think */
}

static void mr_stressor_teardown_common(void)
{
	int ret = 0;

	ret = fi_close(&dom->fid);
	cr_assert_eq(ret, 0, "Could not close domain");

	ret = fi_close(&fab->fid);
	cr_assert_eq(ret, 0, "Could not close fabric");

	fi_freeinfo(fi);
}

static void mr_notifier_stressor_teardown(void)
{
	int ret = 0;

	ret = pthread_join(freer, NULL);
	cr_assert_eq(ret, 0, "Could not join pthread");

	mr_stressor_teardown_common();
}

TestSuite(mr_notifier_stressor,
	  .init = mr_notifier_stressor_setup,
	  .fini = mr_notifier_stressor_teardown,
	  .disabled = true);

/* good 'nuff */
static int get_len(int min_len, int max_len)
{
	int r;
	int m = max_len - min_len;

	cr_assert(m > 0);
	r = rand()%m;

	return min_len+r;

}

static void do_notifier_stressor(int num_allocs, int min_len, int max_len,
				 bool free_imm, bool close_imm)
{
	int i, len, ret;
	char **r = calloc(num_allocs, sizeof(char *));
	struct fid_mr **mr = calloc(num_allocs, sizeof(struct fid_mr));
	int loc = ofi_atomic_get32(&head);

	for (i = 0; i < num_allocs; i++) {
		len = get_len(min_len, max_len);
		r[i] = calloc(len, sizeof(char));
		cr_assert_neq(r[i], NULL,
			      "Could not allocate %d bytes\n", len);

		ret = fi_mr_reg(dom, (void *) r[i], len, default_access,
				default_offset, default_req_key,
				default_flags, &mr[i], NULL);
		cr_assert_eq(ret, FI_SUCCESS);

		MR_ENABLE(mr[i], r[i], len);

		to_free_buf[loc] = free_imm ? r[i] : 0x0;

		loc = next_head();
		pthread_yield();

		if (close_imm) {
			fi_close(&mr[i]->fid);
		}
	}

	to_free_buf[loc] = MAP_FAILED;
	loc = next_head();

	for (i = 0; i < num_allocs; i++) {
		if (!free_imm) {
			free(r[i]);
		}
		if (!close_imm) {
			fi_close(&mr[i]->fid);
		}
	}

	free(r);
	free(mr);
}

Test(mr_notifier_stressor, free_and_close)
{
	do_notifier_stressor(300, 1<<10, 1<<25, true, true);
}

Test(mr_notifier_stressor, close_only)
{
	do_notifier_stressor(300, 1<<10, 1<<25, false, true);
}

Test(mr_notifier_stressor, free_only)
{
	do_notifier_stressor(300, 1<<10, 1<<25, true, false);
}

Test(mr_notifier_stressor, no_free_no_close)
{
	do_notifier_stressor(300, 1<<10, 1<<25, false, false);
}

Test(mr_notifier_stressor, small_free_and_close)
{
	do_notifier_stressor(3000, 1<<4, 1<<14, true, true);
}

Test(mr_notifier_stressor, small_close_only)
{
	do_notifier_stressor(3000, 1<<4, 1<<14, false, true);
}

Test(mr_notifier_stressor, small_free_only)
{
	do_notifier_stressor(3000, 1<<4, 1<<14, true, false);
}

Test(mr_notifier_stressor, small_no_free_no_close)
{
	do_notifier_stressor(3000, 1<<4, 1<<14, false, false);
}


static void mr_reuse_stressor_setup(void)
{
	mr_stressor_setup_common();
}

static void mr_reuse_stressor_teardown(void)
{
	mr_stressor_teardown_common();
}

TestSuite(mr_reuse_stressor,
	  .init = mr_reuse_stressor_setup,
	  .fini = mr_reuse_stressor_teardown,
	  .disabled = true);

static void do_reuse(int num_allocs, int num_reuse,
		     int min_len, int max_len)
{
	int i, a, len, ret;

	cr_assert(num_allocs > 2*num_reuse);

	char **r = calloc(num_reuse, sizeof(char *));

	cr_assert_neq(r, NULL);

	int *r_len = calloc(num_reuse, sizeof(int));

	cr_assert_neq(r_len, NULL);

	for (i = 0; i < num_reuse; i++) {
		len = get_len(min_len, max_len);
		r[i] = calloc(len, sizeof(char));
		cr_assert_neq(r[i], NULL,
			      "Could not allocate %d bytes\n", len);
		r_len[i] = len;
	}

	int nr = 0;

	for (a = 0; a < num_allocs; a++) {
		char *buf;
		struct fid_mr *buf_mr;
		bool free_me;

		if ((nr < num_reuse) && (a % 2)) {
			buf = r[nr];
			len = r_len[nr];
			nr++;
			free_me = false;
		} else {
			len = get_len(min_len, max_len);
			buf = calloc(len, sizeof(char));
			cr_assert_neq(buf, NULL,
				      "Could not allocate %d bytes\n", len);
			free_me = true;
		}
		ret = fi_mr_reg(dom, (void *) buf, len, default_access,
				default_offset, default_req_key,
				default_flags, &buf_mr, NULL);
		cr_assert_eq(ret, FI_SUCCESS);

		MR_ENABLE(buf_mr, buf, len);

		fi_close(&buf_mr->fid);

		if (free_me) {
			free(buf);
		}
	}

	for (i = 0; i < num_reuse; i++) {
		free(r[i]);
	}

	free(r);
	free(r_len);
}

Test(mr_reuse_stressor, few_small)
{
	do_reuse(2347, 11, 1<<8, 1<<12);
}

Test(mr_reuse_stressor, few_large)
{
	do_reuse(2347, 11, 1<<18, 1<<24);
}

Test(mr_reuse_stressor, lots_small)
{
	do_reuse(2347, 491, 1<<8, 1<<12);
}

Test(mr_reuse_stressor, lots_large)
{
	do_reuse(2347, 491, 1<<18, 1<<24);
}

