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
#include "gnix_nic.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

static struct fi_info *hints;
static struct fi_info *fi;
static struct fid_fabric *fab;
static struct fid_domain *dom;

static void __setup(uint32_t version, int mr_mode)
{
	int ret;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = mr_mode;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	if (USING_SCALABLE(fi)) {
		struct fi_gni_ops_fab *ops;
		int in;

		/* nic test opens many nics and exhausts reserved keys */
		in = 256;

		ret = fi_open_ops(&fab->fid,
			FI_GNI_FAB_OPS_1, 0, (void **) &ops, NULL);
		cr_assert_eq(ret, FI_SUCCESS);
		cr_assert(ops);

		ret = ops->set_val(&fab->fid,
			GNI_DEFAULT_PROV_REGISTRATION_LIMIT,
			&in);
		cr_assert_eq(ret, FI_SUCCESS);
	}

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");
}

static void setup_basic(void)
{
	__setup(fi_version(), GNIX_MR_BASIC);
}

static void setup_scalable(void)
{
	__setup(fi_version(), GNIX_MR_SCALABLE);
}

static void teardown(void)
{
	int ret;

	ret = fi_close(&dom->fid);
	cr_assert(!ret, "fi_close domain");

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "fi_close fabric");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

TestSuite(nic_basic, .init = setup_basic, .fini = teardown);
TestSuite(nic_scalable, .init = setup_scalable, .fini = teardown);

static inline void __alloc_free(void)
{
	int i, ret;
	const int num_nics = 79;
	struct gnix_nic *nics[num_nics];
	struct gnix_fid_domain *domain = container_of(
		dom, struct gnix_fid_domain, domain_fid);
	struct gnix_auth_key *auth_key = domain->auth_key;
	struct gnix_nic_attr nic_attr = {0};

	nic_attr.auth_key = auth_key;

	for (i = 0; i < num_nics; i++) {
		ret = gnix_nic_alloc(container_of(dom, struct gnix_fid_domain,
						  domain_fid),
						  &nic_attr,
						  &nics[i]);
		cr_assert_eq(ret, FI_SUCCESS, "Could not allocate nic");
	}

	for (i = 0; i < num_nics; i++) {
		ret = _gnix_nic_free(nics[i]);
		cr_assert_eq(ret, FI_SUCCESS, "Could not free nic");
	}
}

Test(nic_basic, alloc_free)
{
	__alloc_free();
}

Test(nic_scalable, alloc_free)
{
	__alloc_free();
}
