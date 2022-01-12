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
#include "common.h"

static struct fid_fabric *fabric;
static struct fi_info *fi;

static void setup(void)
{
	int ret;
	struct fi_info *hints;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = FI_MR_BASIC;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 0), NULL, 0, 0, hints, &fi);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fabric, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_fabric");

	fi_freeinfo(hints);
}

static void teardown(void)
{
	int ret;

	ret = fi_close(&fabric->fid);
	cr_assert(ret == FI_SUCCESS, "fi_close fabric");

	fi_freeinfo(fi);
}
TestSuite(fabric_bare);
TestSuite(fabric, .init = setup, .fini = teardown);

Test(fabric, simple)
{
	cr_assert(fabric != NULL);
}

Test(fabric, open_ops_1)
{
	int ret;
	struct fi_gni_ops_fab *ops;

	ret = fi_open_ops(&fabric->fid,
		FI_GNI_FAB_OPS_1, 0, (void **) &ops, NULL);
	cr_assert_eq(ret, FI_SUCCESS);

	cr_assert(ops);
}

Test(fabric, set_wait_timeout)
{
	int ret;
	struct fi_gni_ops_fab *ops;
	int old_val = 0, new_val, current_val;

	ret = fi_open_ops(&fabric->fid,
		FI_GNI_FAB_OPS_1, 0, (void **) &ops, NULL);
	cr_assert_eq(ret, FI_SUCCESS);

	cr_assert(ops);

	ret = ops->get_val(&fabric->fid,
		GNI_WAIT_SHARED_MEMORY_TIMEOUT, &old_val);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_neq(old_val, 0);

	new_val = old_val * 2;
	ret = ops->set_val(&fabric->fid,
		GNI_WAIT_SHARED_MEMORY_TIMEOUT, &new_val);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = ops->get_val(&fabric->fid,
		GNI_WAIT_SHARED_MEMORY_TIMEOUT, &current_val);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(current_val, new_val);
}

Test(fabric, open_ops_2)
{
	int ret;
	struct fi_gni_auth_key_ops_fab *auth_ops;

	ret = fi_open_ops(&fabric->fid,
		FI_GNI_FAB_OPS_2, 0, (void **) &auth_ops, NULL);
	cr_assert_eq(ret, FI_SUCCESS);

	cr_assert(auth_ops);
}

Test(fabric, set_limits)
{
	int ret;
	int in;
	int total;

	struct gnix_auth_key_attr default_attr = { 0 };
	struct gnix_auth_key_attr expected_attr = { 0 };
	struct gnix_auth_key_attr actual_attr = { 0 };

	struct fi_gni_ops_fab *ops;
	struct fi_gni_auth_key_ops_fab *auth_ops;

	ret = fi_open_ops(&fabric->fid,
		FI_GNI_FAB_OPS_1, 0, (void **) &ops, NULL);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_open_ops(&fabric->fid,
		FI_GNI_FAB_OPS_2, 0, (void **) &auth_ops, NULL);
	cr_assert_eq(ret, FI_SUCCESS);

	cr_assert(ops);
	cr_assert(auth_ops);

	ret = ops->get_val(&fabric->fid,
		GNI_DEFAULT_USER_REGISTRATION_LIMIT,
		&default_attr.user_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);

	in = (default_attr.user_key_limit == 1) ?
		2 : default_attr.user_key_limit - 1;
	cr_assert(in > 0);
	expected_attr.user_key_limit = in;

	ret = ops->get_val(&fabric->fid,
		GNI_DEFAULT_PROV_REGISTRATION_LIMIT,
		&default_attr.prov_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);

	in = (default_attr.prov_key_limit == 1) ?
		2 : default_attr.prov_key_limit - 1;
	cr_assert(in > 0);
	expected_attr.prov_key_limit = in;

	/* set defaults */
	ret = ops->set_val(&fabric->fid,
		GNI_DEFAULT_USER_REGISTRATION_LIMIT,
		&expected_attr.user_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = ops->set_val(&fabric->fid,
		GNI_DEFAULT_PROV_REGISTRATION_LIMIT,
		&expected_attr.prov_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);

	/* get defaults */
	ret = ops->get_val(&fabric->fid,
		GNI_DEFAULT_USER_REGISTRATION_LIMIT,
		&actual_attr.user_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(actual_attr.user_key_limit, expected_attr.user_key_limit);

	ret = ops->get_val(&fabric->fid,
		GNI_DEFAULT_PROV_REGISTRATION_LIMIT,
		&actual_attr.prov_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(actual_attr.prov_key_limit, expected_attr.prov_key_limit);

	/* clear result buffer */
	memset(&actual_attr, 0x0, sizeof(actual_attr));

	/* ensure defaults were propogated to default key */
	ret = auth_ops->get_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_USER_KEY_LIMIT,
		&actual_attr.user_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(actual_attr.user_key_limit, expected_attr.user_key_limit);

	ret = auth_ops->get_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_PROV_KEY_LIMIT,
		&actual_attr.prov_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(actual_attr.prov_key_limit, expected_attr.prov_key_limit);

	ret = auth_ops->get_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_TOTAL_KEYS_NEEDED,
		&total);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(total,
		expected_attr.prov_key_limit + expected_attr.user_key_limit);

	/* set default auth key limits to something else */
	expected_attr.user_key_limit >>= 1;
	expected_attr.prov_key_limit >>= 1;

	ret = auth_ops->set_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_USER_KEY_LIMIT,
		&expected_attr.user_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = auth_ops->set_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_PROV_KEY_LIMIT,
		&expected_attr.prov_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = auth_ops->get_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_USER_KEY_LIMIT,
		&actual_attr.user_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(actual_attr.user_key_limit, expected_attr.user_key_limit);

	ret = auth_ops->get_val(GNIX_PROV_DEFAULT_AUTH_KEY,
		GNIX_PROV_DEFAULT_AUTH_KEYLEN,
		GNIX_PROV_KEY_LIMIT,
		&actual_attr.prov_key_limit);
	cr_assert_eq(ret, FI_SUCCESS);
	cr_assert_eq(actual_attr.prov_key_limit, expected_attr.prov_key_limit);
}

Test(fabric_bare, fi_mr_basic_1_0)
{
	int ret;
	struct fi_info *hints;
	struct fi_info *info;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = FI_MR_BASIC;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 0), NULL, 0, 0, hints, &info);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo");
	cr_assert(info->domain_attr->mr_mode == FI_MR_BASIC);

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(fabric_bare, fi_mr_scalable_1_0)
{
	int ret;
	struct fi_info *hints;
	struct fi_info *info = NULL;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = FI_MR_SCALABLE;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 0), NULL, 0, 0, hints, &info);
	if (info) {
		cr_assert(strcmp(info->fabric_attr->prov_name,hints->fabric_attr->prov_name),
			  "fi_getinfo");
	} else {
		cr_assert(ret == -FI_ENODATA, "fi_getinfo");
	}

	fi_freeinfo(hints);
}

Test(fabric_bare, fi_mr_basic_1_5)
{
	int ret;
	struct fi_info *hints;
	struct fi_info *info;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = FI_MR_BASIC;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 5), NULL, 0, 0, hints, &info);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo");
	cr_assert(info->domain_attr->mr_mode == FI_MR_BASIC);

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(fabric_bare, fi_mr_scalable_1_5_fail)
{
	int ret;
	struct fi_info *hints;
	struct fi_info *info = NULL;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = FI_MR_SCALABLE;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 5), NULL, 0, 0, hints, &info);
	if (info) {
		cr_assert(strcmp(info->fabric_attr->prov_name,hints->fabric_attr->prov_name),
			  "fi_getinfo");
	} else {
		cr_assert(ret == -FI_ENODATA, "fi_getinfo");
	}

	fi_freeinfo(hints);
}

Test(fabric_bare, fi_mr_scalable_1_5_pass)
{
	int ret;
	struct fi_info *hints;
	struct fi_info *info;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = FI_MR_MMU_NOTIFY;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 5), NULL, 0, 0, hints, &info);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo");
	cr_assert(info->domain_attr->mr_mode == FI_MR_MMU_NOTIFY);

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(fabric_bare, fi_mr_basic_1_5_ofi_map)
{
	int ret;
	struct fi_info *hints;
	struct fi_info *info;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = OFI_MR_BASIC_MAP;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(FI_VERSION(1, 5), NULL, 0, 0, hints, &info);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo");
	cr_assert(info->domain_attr->mr_mode == OFI_MR_BASIC_MAP);

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}
