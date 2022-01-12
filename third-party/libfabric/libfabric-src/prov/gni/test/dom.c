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
#include "common.h"

#include "common.h"

static struct fid_fabric *fabric;
static struct fi_info *fi;

static void _setup(uint32_t version)
{
	int ret;
	struct fi_info *hints;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->fabric_attr->prov_name = strdup("gni");
	if (FI_VERSION_LT(version, FI_VERSION(1, 5)))
		hints->domain_attr->mr_mode = FI_MR_BASIC;
	else
		hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fabric, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_fabric");

	fi_freeinfo(hints);
}

static void setup(void)
{
	_setup(fi_version());
}

static void setup_1_0(void)
{
	_setup(FI_VERSION(1, 0));
}

static void teardown(void)
{
	int ret;

	ret = fi_close(&fabric->fid);
	cr_assert(ret == FI_SUCCESS, "fi_close fabric");

	fi_freeinfo(fi);
}

TestSuite(domain, .init = setup, .fini = teardown);
TestSuite(domain_1_0, .init = setup_1_0, .fini = teardown);

Test(domain_1_0, no_dom_auth_key_support)
{
	int ret;
	struct fid_domain *dom;
	void *old_auth_key = fi->domain_attr->auth_key;
	int old_auth_key_size = fi->domain_attr->auth_key_size;

	fi->domain_attr->auth_key = (void *) 0xdeadbeef;
	fi->domain_attr->auth_key_size = 47;

	ret = fi_domain(fabric, fi, &dom, NULL);
	cr_assert(ret == -FI_EINVAL, "fi_domain, ret=%d expected=%d",
		ret, -FI_EINVAL);

	fi->domain_attr->auth_key = old_auth_key;
	fi->domain_attr->auth_key_size = old_auth_key_size;
}

Test(domain_1_0, no_mr_auth_key_support)
{
	int ret;
	struct fid_mr *mr;
	struct fid_domain *dom;

	struct iovec iov = {
		.iov_base = (void *) 0xabbaabba,
		.iov_len = 1024,
	};
	struct fi_mr_attr mr_attr = {
		.mr_iov = &iov,
		.iov_count = 1,
		.access = (FI_REMOTE_READ | FI_REMOTE_WRITE
				| FI_READ | FI_WRITE),
		.offset = 0,
		.requested_key = 0,
		.context = NULL,
		.auth_key = (void *) 0xdeadbeef,
		.auth_key_size = 47,
	};

	ret = fi_domain(fabric, fi, &dom, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_domain, ret=%d expected=%d",
		ret, FI_SUCCESS);

	ret = fi_mr_regattr(dom, &mr_attr, 0, &mr);
	cr_assert(ret == -FI_EINVAL, "fi_mr_regattr, ret=%d expected=%d",
		ret, -FI_EINVAL);

	ret = fi_close(&dom->fid);
	cr_assert(ret == FI_SUCCESS, "fi_close, ret=%d expected=%d",
		ret, FI_SUCCESS);
}

Test(domain, many_domains)
{
	int i, ret;
	const int num_doms = 7919;
	struct fid_domain *doms[num_doms];
	struct gnix_fid_domain *gdom;
	struct gnix_fid_fabric *gfab;

	memset(doms, 0, num_doms*sizeof(struct fid_domain *));

	gfab = container_of(fabric, struct gnix_fid_fabric, fab_fid);
	for (i = 0; i < num_doms; i++) {
		ret = fi_domain(fabric, fi, &doms[i], NULL);
		cr_assert(ret == FI_SUCCESS, "fi_domain");
		gdom = container_of(doms[i], struct gnix_fid_domain,
				    domain_fid);
		cr_assert(gdom, "domain not allcoated");
		cr_assert(gdom->fabric == gfab, "Incorrect fabric");
		cr_assert(ofi_atomic_get32(&gdom->ref_cnt.references) == 1,
				"Incorrect ref_cnt");

	}

	for (i = num_doms-1; i >= 0; i--) {
		ret = fi_close(&doms[i]->fid);
		cr_assert(ret == FI_SUCCESS, "fi_close domain");
	}

}

Test(domain, open_ops)
{
	int i, ret;
	const int num_doms = 11;
	struct fid_domain *doms[num_doms];
	struct fi_gni_ops_domain *gni_domain_ops;
	enum dom_ops_val op;
	uint32_t val;
	char *other_reg_type = "none";
	char *string_val;
	bool xpmem_toggle = false, xpmem_check;

	memset(doms, 0, num_doms*sizeof(struct fid_domain *));

	for (i = 0; i < num_doms; i++) {
		ret = fi_domain(fabric, fi, &doms[i], NULL);
		cr_assert(ret == FI_SUCCESS, "fi_domain");
		ret = fi_open_ops(&doms[i]->fid, FI_GNI_DOMAIN_OPS_1,
				  0, (void **) &gni_domain_ops, NULL);
		cr_assert(ret == FI_SUCCESS, "fi_open_ops");
		for (op = 0; op < GNI_NUM_DOM_OPS; op++) {
			val = i*op+op;
			switch (op) {
			case GNI_MR_CACHE:
				ret = gni_domain_ops->set_val(&doms[i]->fid, op,
						&other_reg_type);
				break;
			case GNI_XPMEM_ENABLE:
				ret = gni_domain_ops->set_val(&doms[i]->fid, op,
						&xpmem_toggle);
				break;
			default:
				ret = gni_domain_ops->set_val(&doms[i]->fid, op, &val);
				break;
			}
			cr_assert(ret == FI_SUCCESS, "set_val");

			switch (op) {
			case GNI_MR_CACHE:
				ret = gni_domain_ops->get_val(&doms[i]->fid, op, &string_val);
				break;
			case GNI_XPMEM_ENABLE:
				ret = gni_domain_ops->get_val(&doms[i]->fid, op,
							      &xpmem_check);
				break;
			default:
				ret = gni_domain_ops->get_val(&doms[i]->fid, op, &val);
				break;
			}
			cr_assert(ret == FI_SUCCESS, "get_val");

			switch (op) {
			case GNI_MR_CACHE:
				cr_assert_eq(strncmp(other_reg_type, string_val,
						strlen(other_reg_type)),  0, "Incorrect op value");
				break;
			case GNI_XPMEM_ENABLE:
				cr_assert(xpmem_toggle == xpmem_check,
					  "Incorrect op value");
				break;
			default:
				cr_assert(val == i*op+op, "Incorrect op value");
				break;
			}
		}
		ret = fi_close(&doms[i]->fid);
		cr_assert(ret == FI_SUCCESS, "fi_close domain");
	}
}

Test(domain, cache_flush_op)
{
	int i, ret;
	const int num_doms = 11;
	struct fid_domain *doms[num_doms];
	struct fi_gni_ops_domain *gni_domain_ops;
	struct fid_mr *mr;
	char *buf = calloc(1024, sizeof(char));
	int requested_key = 0;

	cr_assert(buf);

	memset(doms, 0, num_doms*sizeof(struct fid_domain *));

	for (i = 0; i < num_doms; i++) {
		requested_key = USING_SCALABLE(fi) ? i : 0;

		ret = fi_domain(fabric, fi, &doms[i], NULL);
		cr_assert(ret == FI_SUCCESS, "fi_domain");
		ret = fi_open_ops(&doms[i]->fid, FI_GNI_DOMAIN_OPS_1,
				  0, (void **) &gni_domain_ops, NULL);
		cr_assert(ret == FI_SUCCESS, "fi_open_ops");

		ret = fi_mr_reg(doms[i],
				  buf,
				  1024,
				  FI_READ,
				  0,
				  requested_key,
				  0,
				  &mr,
				  NULL);
		cr_assert(ret == FI_SUCCESS, "fi_reg_mr, ret=%d", ret);

		if (USING_SCALABLE(fi))
			MR_ENABLE(mr, buf, 1024);

		ret = fi_close(&mr->fid);
		cr_assert(ret == FI_SUCCESS, "fi_close mr");

		ret = gni_domain_ops->flush_cache(&doms[i]->fid);
		cr_assert(ret == FI_SUCCESS, "flush cache");

		ret = fi_close(&doms[i]->fid);
		cr_assert(ret == FI_SUCCESS, "fi_close domain");
	}

	free(buf);
}

Test(domain, invalid_open_ops)
{
	int ret;
	struct fid_domain *dom;
	struct fi_gni_ops_domain *gni_domain_ops;
	uint32_t val = 0;

	ret = fi_domain(fabric, fi, &dom, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_domain");
	ret = fi_open_ops(&dom->fid, FI_GNI_DOMAIN_OPS_1,
			  0, (void **) &gni_domain_ops, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_open_ops");

	ret = gni_domain_ops->get_val(&dom->fid, GNI_NUM_DOM_OPS, &val);
	cr_assert(ret == -FI_EINVAL, "get_val");

	ret = gni_domain_ops->set_val(&dom->fid, GNI_NUM_DOM_OPS, &val);
	cr_assert(ret == -FI_EINVAL, "set_val");

	ret = fi_close(&dom->fid);
	cr_assert(ret == FI_SUCCESS, "fi_close domain");
}
