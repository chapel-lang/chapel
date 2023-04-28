/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm_mq_internal.h"

typedef void *(*memcpy_fn_t) (void *dst, const void *src, size_t n);
static int psmi_test_memcpy(memcpy_fn_t, const char *name);
static int psmi_test_epid_table(int numelems);

int psm3_diags(void);

#define diags_assert(x)	do {					\
	    if (!(x))  {					\
		_HFI_ERROR("Diags assertion failure: %s\n",	\
		    #x);					\
		goto fail;					\
	    }							\
	} while (0)

#define DIAGS_RETURN_PASS(str)						\
	do { _HFI_INFO("%s: PASSED %s\n", __func__, str); return 0; }	\
	    while (0)
#define DIAGS_RETURN_FAIL(str)						\
	do { _HFI_INFO("%s: FAILED %s\n", __func__, str); return 1; }	\
	    while (0)

int psm3_diags(void)
{
	int ret = 0;
	ret |= psmi_test_epid_table(2048);
	ret |= psmi_test_memcpy((memcpy_fn_t) psm3_memcpyo, "psm3_memcpyo");
	/* ret |= psmi_test_memcpy((memcpy_fn_t) psm3_mq_mtucpy, "psm3_mq_mtucpy"); */

	if (ret)
		DIAGS_RETURN_FAIL("");
	else
		DIAGS_RETURN_PASS("");
}

/*
 * Hash table test
 */
#define NALLOC	1024
static int psmi_test_epid_table(int numelems)
{
	ptl_ctl_t ctl;
	psm2_epaddr_t *ep_array, epaddr, ep_alloc;
	psm2_epid_t *epid_array, epid_tmp;
	psm2_ep_t ep = (psm2_ep_t) (uintptr_t) 0xabcdef00;
	struct psm3_epid_table *tab;
	int i, j;
	struct drand48_data drand48_data;

	ep_alloc =
	    (psm2_epaddr_t) psmi_calloc(PSMI_EP_NONE, UNDEFINED, numelems,
				       sizeof(struct psm2_epaddr));
	ep_array =
	    (psm2_epaddr_t *) psmi_calloc(PSMI_EP_NONE, UNDEFINED, numelems,
					 sizeof(struct psm2_epaddr *));
	epid_array =
	    (psm2_epid_t *) psmi_calloc(PSMI_EP_NONE, UNDEFINED, numelems,
				       sizeof(psm2_epid_t));
	diags_assert(ep_alloc != NULL);
	diags_assert(ep_array != NULL);
	diags_assert(epid_array != NULL);

	srand48_r(12345678, &drand48_data);

	psm3_epid_init();
	tab = &psm3_epid_table;
	ctl.ep = ep;

	for (i = 0; i < numelems; i++) {
		// this is a simulated entry with an invalid epid just for tests
		epid_array[i] = psm3_epid_pack_diag(i);
		ep_alloc[i].ptlctl = &ctl;
		ep_alloc[i].epid = epid_array[i];
		ep_array[i] = &ep_alloc[i];
	}
	for (i = 0; i < numelems; i++) {
		psm3_epid_add(ep, epid_array[i], ep_array[i]);
	}

	/* Randomize epid_array */
	for (i = 0; i < numelems; i++) {
		long int rand_result;
		lrand48_r(&drand48_data, &rand_result);
		j = (int)(rand_result % numelems);
		epid_tmp = epid_array[i];
		epid_array[i] = epid_array[j];
		epid_array[j] = epid_tmp;
	}
	/* Lookup. */
	for (i = 0; i < numelems; i++) {
		epaddr = psm3_epid_lookup(ep, epid_array[i]);
		diags_assert(epaddr != NULL);
		diags_assert(!psm3_epid_cmp_internal(epaddr->epid, epid_array[i]));
		diags_assert(epaddr->ptlctl->ep == ep);
	}

	/* Randomize epid_array again */
	for (i = 0; i < numelems; i++) {
		long int rand_result;
		lrand48_r(&drand48_data, &rand_result);
		j = (int)(rand_result % numelems);
		epid_tmp = epid_array[i];
		epid_array[i] = epid_array[j];
		epid_array[j] = epid_tmp;
	}
	/* Delete half */
	for (i = 0; i < numelems / 2; i++) {
		epaddr = psm3_epid_remove(ep, epid_array[i]);
		diags_assert(epaddr != NULL);
		diags_assert(!psm3_epid_cmp_internal(epaddr->epid, epid_array[i]));
		diags_assert(epaddr->ptlctl->ep == ep);
	}
	/* Lookup other half -- expect non-NULL, then delete */
	for (i = numelems / 2; i < numelems; i++) {
		epaddr = psm3_epid_lookup(ep, epid_array[i]);
		diags_assert(epaddr != NULL);
		diags_assert(!psm3_epid_cmp_internal(epaddr->epid, epid_array[i]));
		diags_assert(epaddr->ptlctl->ep == ep);
		epaddr = psm3_epid_remove(ep, epid_array[i]);
		epaddr = psm3_epid_lookup(ep, epid_array[i]);
		diags_assert(epaddr == NULL);
	}
	/* Lookup whole thing, expect done */
	for (i = 0; i < numelems; i++) {
		epaddr = psm3_epid_lookup(ep, epid_array[i]);
		diags_assert(epaddr == NULL);
	}
	for (i = 0; i < tab->tabsize; i++) {
		diags_assert(tab->table[i].entry == NULL ||
			     tab->table[i].entry == EPADDR_DELETED);
	}

	/* Make sure we're not leaking memory somewhere... */
	diags_assert(tab->tabsize > tab->tabsize_used &&
		     tab->tabsize * PSMI_EPID_TABLOAD_FACTOR >
		     tab->tabsize_used);

	/* Only free on success */
	psm3_epid_fini();
	psmi_free(epid_array);
	psmi_free(ep_array);
	psmi_free(ep_alloc);
	DIAGS_RETURN_PASS("");

fail:
	/* Klocwork scan report memory leak. */
	psm3_epid_fini();
	if (epid_array)
		psmi_free(epid_array);
	if (ep_array)
		psmi_free(ep_array);
	if (ep_alloc)
		psmi_free(ep_alloc);
	DIAGS_RETURN_FAIL("");
}

/*
 * Memcpy correctness test
 */
static int memcpy_check_size(memcpy_fn_t fn, int *p, int *f, size_t n);
static void *memcpy_check_one(memcpy_fn_t fn, void *dst, void *src, size_t n);

static int psmi_test_memcpy(memcpy_fn_t fn, const char *memcpy_name)
{
	const int CORNERS = 0;
	const long long lo = 1;
	const long long hi = 16 * 1024 * 1024;
	const long long below = 32;
	const long long above = 32;
	long long n, m;
	char buf[128];
	int ret = 0;
	int memcpy_passed;
	int memcpy_failed;

	memcpy_passed = 0;
	memcpy_failed = 0;

	ret = memcpy_check_size(fn, &memcpy_passed, &memcpy_failed, 0);
	if (ret < 0)
		DIAGS_RETURN_FAIL("no heap space");

	for (n = lo; n <= hi; n <<= 1) {
		_HFI_INFO("%s %d align=0..16\n", memcpy_name, (int)n);
		for (m = n - below; m <= n + above; m++) {
			if (m == n) {
				ret =
				    memcpy_check_size(fn, &memcpy_passed,
						      &memcpy_failed, n);
				if (ret < 0)
					DIAGS_RETURN_FAIL("no heap space");
			} else if (CORNERS && m >= lo && m <= hi && m > (n >> 1)
				   && m < max(n, ((n << 1) - below))) {
				ret =
				    memcpy_check_size(fn, &memcpy_passed,
						      &memcpy_failed,
						      (size_t) m);
				if (ret < 0)
					DIAGS_RETURN_FAIL("no heap space");
			}
		}
	}

	int total = memcpy_passed + memcpy_failed;
	if (total > 0) {
		_HFI_INFO("%d memcpy tests with %d passed (%.2f%%) "
			  "and %d failed (%.2f%%)\n",
			  total, memcpy_passed, (100.0 * memcpy_passed) / total,
			  memcpy_failed, (100.0 * memcpy_failed) / total);
	}
	if (memcpy_failed) {
		snprintf(buf, sizeof(buf), "%s %.2f%% of tests memcpy_failed",
			 memcpy_name, (100.0 * memcpy_failed) / total);
		DIAGS_RETURN_FAIL(buf);
	} else {
		DIAGS_RETURN_PASS(memcpy_name);
	}
}

void *memcpy_check_one(memcpy_fn_t fn, void *dst, void *src, size_t n)
{
	int ok = 1;
	unsigned int seed = (unsigned int)
	    ((uintptr_t) dst ^ (uintptr_t) src ^ (uintptr_t) n);
	size_t i;
	struct drand48_data drand48_data;

	if (!n)
		return dst;

	memset(src, 0x55, n);
	memset(dst, 0xaa, n);
	srand48_r(seed, &drand48_data);
	for (i = 0; i < n; i++) {
		long int rand_result;
		lrand48_r(&drand48_data, &rand_result);
		((uint8_t *) src)[i] = (((int)(rand_result % INT_MAX)) >> 16) & 0xff;
	}

	fn(dst, src, n);
	memset(src, 0, n);
	srand48_r(seed, &drand48_data);
	for (i = 0; i < n; i++) {
		long int rand_result;
		lrand48_r(&drand48_data, &rand_result);
		int value = (int)(uint8_t) (((int)(rand_result % INT_MAX)) >> 16);
		int v = (int)((uint8_t *) dst)[i];
		if (v != value) {
			_HFI_ERROR
			    ("Error on index %llu : got %d instead of %d\n",
			     (unsigned long long)i, v, value);
			ok = 0;
		}
	}
	return ok ? dst : NULL;
}

int memcpy_check_size(memcpy_fn_t fn, int *p, int *f, size_t n)
{
#define num_aligns 16
#define USE_MALLOC 0
#define DEBUG 0
	uint8_t *src;
	uint8_t *dst;
	size_t size = n * 2 + num_aligns;
	if (USE_MALLOC) {
		src = psmi_malloc(PSMI_EP_NONE, UNDEFINED, size);
		dst = psmi_malloc(PSMI_EP_NONE, UNDEFINED, size);
		if (src == NULL || dst == NULL) {
			if (src) psmi_free(src);
			if (dst) psmi_free(dst);
			return -1;
		}
	} else {
		void *src_p = NULL, *dst_p = NULL;
		if (posix_memalign(&src_p, 64, size) != 0 ||
		    posix_memalign(&dst_p, 64, size) != 0) {
			if (src_p) free(src_p);
			if (dst_p) free(dst_p);
			return -1;
		}
		src = (uint8_t *) src_p;
		dst = (uint8_t *) dst_p;
	}

	int src_align, dst_align;
	for (src_align = 0; src_align < num_aligns; src_align++) {
		for (dst_align = 0; dst_align < num_aligns; dst_align++) {
			uint8_t *d = ((uint8_t *) dst) + dst_align;
			uint8_t *s = ((uint8_t *) src) + src_align;
			int ok = (memcpy_check_one(fn, d, s, n) != NULL);
			if (DEBUG || !ok) {
				_HFI_INFO("memcpy(%p, %p, %llu) : %s\n", d, s,
					  (unsigned long long)n,
					  ok ? "passed" : "failed");
			}
			if (ok) {
				(*p)++;
			} else {
				(*f)++;
			}
		}
	}
	if (USE_MALLOC) {
		psmi_free(src);
		psmi_free(dst);
	} else {
		free(src);
		free(dst);
	}
	return 0;
}
