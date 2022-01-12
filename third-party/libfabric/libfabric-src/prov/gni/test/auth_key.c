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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <time.h>
#include <string.h>


#include "gnix.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "fi_ext_gni.h"

#include "gnix_auth_key.h"

static void setup(void)
{
	struct fi_info *info = fi_allocinfo();
	int ret;

	cr_assert(info);

	ret = fi_getinfo(fi_version(), NULL, 0, 0, NULL, &info);
	cr_assert(ret == FI_SUCCESS);

	fi_freeinfo(info);
}

static void teardown(void)
{
}

TestSuite(auth_key, .init = setup, .fini = teardown);


Test(auth_key, create)
{
	struct gnix_auth_key *ret;

	ret = _gnix_auth_key_create(NULL, 0);
	cr_assert(ret != NULL, "failed to create auth key");
}

Test(auth_key, failed_insert)
{
	struct gnix_auth_key *ret;

	ret = _gnix_auth_key_create(NULL, 0);
	cr_assert(ret != NULL, "failed to create auth_key");

	ret = _gnix_auth_key_create(NULL, 0);
	cr_assert(ret == NULL, "unexpectedly created auth_key");
}

void *race_create_func(void *context)
{
	pthread_barrier_t *barrier = (pthread_barrier_t *) context;
	struct gnix_auth_key *auth_key;
	int ret;

	/* -1 is the single thread return value for the
		thread allowed to make modifications to the barrier.
		For the version of the pthread header present on our
		systems, the value does not have a define. */
	ret = pthread_barrier_wait(barrier);
	cr_assert(ret == 0 || ret == -1, "pthread_barrier, "
		"ret=%d errno=%d strerror=%s", ret, errno, strerror(errno));

	auth_key = GNIX_GET_AUTH_KEY(NULL, 0, 0);
	cr_assert_neq(auth_key, NULL, "failed to get authorization key");

	return NULL;
}

Test(auth_key, race_create)
{
#define __AUTH_KEY_THREAD_COUNT 47
	int i;
	int thread_count = __AUTH_KEY_THREAD_COUNT;
	int ret;
	pthread_t threads[__AUTH_KEY_THREAD_COUNT];
	pthread_barrier_t barrier;
#undef __AUTH_KEY_THREAD_COUNT

	ret = pthread_barrier_init(&barrier, NULL, thread_count);
	cr_assert_eq(ret, 0, "failed to initialize barrier");

	for (i = 0; i < thread_count; i++) {
		ret = pthread_create(&threads[i], NULL,
			race_create_func, &barrier);
		cr_assert_eq(ret, 0, "failed to create pthread");
	}

	for (i = 0; i < thread_count; i++) {
		ret = pthread_join(threads[i], NULL);
		cr_assert_eq(ret, 0);
	}

	ret = pthread_barrier_destroy(&barrier);
	cr_assert_eq(ret, 0);
}

Test(auth_key, limit_four_vmdh_entries)
{
	int i, ret;
	struct gnix_auth_key *auth_key;
	struct fi_gni_auth_key _ak;

	for (i = 0; i < 4; i++) {
		auth_key = _gnix_auth_key_alloc();
		cr_assert(auth_key);

		auth_key->attr.prov_key_limit = 128;
		auth_key->attr.user_key_limit = 128;
		auth_key->ptag = i * 16;
		auth_key->cookie = i * 32;
		auth_key->using_vmdh = 1;

		_ak.type = GNIX_AKT_RAW;
		_ak.raw.protection_key = auth_key->cookie;

		ret = _gnix_auth_key_insert((uint8_t *) &_ak,
			sizeof(struct fi_gni_auth_key), auth_key);
		cr_assert(ret == FI_SUCCESS);

		ret = _gnix_auth_key_enable(auth_key);
		cr_assert(ret == FI_SUCCESS);
	}

	auth_key = _gnix_auth_key_alloc();
	cr_assert(auth_key);

	auth_key->attr.prov_key_limit = 128;
	auth_key->attr.user_key_limit = 128;
	auth_key->ptag = i * 16;
	auth_key->cookie = i * 32;
	auth_key->using_vmdh = 1;

	_ak.type = GNIX_AKT_RAW;
	_ak.raw.protection_key = auth_key->cookie;

	ret = _gnix_auth_key_insert((uint8_t *) &_ak,
		sizeof(struct fi_gni_auth_key), auth_key);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_auth_key_enable(auth_key);
	cr_assert(ret == -FI_ENOSPC,
		"ret is not correct, expected=%d actual=%d\n",
		-FI_ENOSPC, ret);
}
