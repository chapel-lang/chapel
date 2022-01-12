/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2018      Los Alamos National Security, LLC. All
 *                         rights reserved.
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
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "gnix.h"
#include "common.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "gnix.h"
#include "gnix_mr.h"
#include "common.h"

#define CHECK_HOOK(name, args...) \
	({ \
		int __hook_return_val = 0; \
		if (hooks->name) \
			__hook_return_val = hooks->name(__func__, \
					__LINE__, ##args); \
		__hook_return_val; })
#define HOOK_PRESENT(name) (hooks->name != NULL)

#if 0
#define MR_DBG(fmt, args...) fprintf(stderr, fmt, ##args)
#define HOOK_DEBUG(message, args...) \
	MR_DBG("%s:%d - " message, func, line, ##args)
#define HOOK_ASSERT(cond, message, args...) \
	do { \
		if (!(cond)) \
			HOOK_DEBUG(message, args); \
	} while (0)
#else
#define MR_DBG(fmt, args...)
#define HOOK_DEBUG(message, args...) do { } while (0)
#define HOOK_ASSERT(cond, message, args...) do { } while (0)
#endif

#define GNIX_ASSUMED_ALIGNMENT 0xfff

static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct fid_mr *mr;
static struct fi_info *hints;
static struct fi_info *fi;

#define __BUF_LEN 4096
static unsigned char *buf;
static int buf_len = __BUF_LEN * sizeof(unsigned char);
static struct gnix_fid_domain *domain;
static uint8_t ptag;
static gnix_mr_cache_t *cache;

static uint64_t default_access = (FI_REMOTE_READ | FI_REMOTE_WRITE
				  | FI_READ | FI_WRITE);
static uint64_t ro_access = (FI_REMOTE_READ | FI_WRITE);

static uint64_t default_flags;
static uint64_t default_req_key;
static uint64_t default_offset;

static int regions;

struct timeval s1, s2;

struct _mr_test_hooks {
	int (*init_hook)(const char *func, int line);
	int (*post_reg_hook)(const char *func, int line,
			int cache_type, int inuse, int stale);
	int (*post_dereg_hook)(const char *func, int line,
			int inuse, int stale);
	int (*get_lazy_dereg_limit)(const char *func, int line);
};

#define HOOK_DECL struct _mr_test_hooks *hooks
struct _mr_test_hooks empty_hooks = {NULL};

#define DEFAULT_REGION_COUNT 1024
#define DEFAULT_SCALABLE_REGION_COUNT 128

/* this helper function doesn't work for string ops */
static void _set_check_domain_op_value(int op, int value)
{
	int ret;
	struct fi_gni_ops_domain *gni_domain_ops;
	int32_t get_val, val;

	ret = fi_open_ops(&domain->domain_fid.fid, FI_GNI_DOMAIN_OPS_1,
			  0, (void **) &gni_domain_ops, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_open_ops");

	val = value;
	ret = gni_domain_ops->set_val(&domain->domain_fid.fid,
			op, &val);
	cr_assert(ret == FI_SUCCESS, "set val");

	ret = gni_domain_ops->get_val(&domain->domain_fid.fid,
			op, &get_val);
	cr_assert(val == get_val, "get val");
}

static void _set_lazy_deregistration(int val)
{
	_set_check_domain_op_value(GNI_MR_CACHE_LAZY_DEREG, val);
}

static void _mr_setup(uint32_t version, int mr_mode)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = mr_mode;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

	buf = calloc(__BUF_LEN, sizeof(unsigned char));
	cr_assert(buf, "buffer allocation");

	domain = container_of(dom, struct gnix_fid_domain, domain_fid.fid);
	ptag = domain->auth_key->ptag;

	regions = 1024;
}

static void mr_teardown(void)
{
	int ret = 0;

	ret = fi_close(&dom->fid);
	cr_assert(!ret, "failure in closing domain.");
	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");
	fi_freeinfo(fi);
	fi_freeinfo(hints);

	domain = NULL;
	cache = NULL;

	free(buf);
}

static void udreg_setup(void)
{
	_mr_setup(fi_version(), GNIX_MR_BASIC);

	_gnix_open_cache(domain, GNIX_MR_TYPE_UDREG);

	_set_lazy_deregistration(1);
}

static void udreg_setup_nld(void)
{
	_mr_setup(fi_version(), GNIX_MR_BASIC);

	_gnix_open_cache(domain, GNIX_MR_TYPE_UDREG);

	_set_lazy_deregistration(0);
}

static void internal_mr_setup(void)
{
	_mr_setup(fi_version(), GNIX_MR_BASIC);

	_gnix_open_cache(domain, GNIX_MR_TYPE_INTERNAL);

	_set_lazy_deregistration(1);
}

static void internal_mr_setup_nld(void)
{
	_mr_setup(fi_version(), GNIX_MR_BASIC);

	_gnix_open_cache(domain, GNIX_MR_TYPE_INTERNAL);

	_set_lazy_deregistration(0);
}

static void no_cache_scalable_setup(void)
{
	_mr_setup(fi_version(), GNIX_MR_SCALABLE);

	_gnix_open_cache(domain, GNIX_MR_TYPE_NONE);
}

static void no_cache_basic_setup(void)
{
	_mr_setup(fi_version(), GNIX_MR_BASIC);

	_gnix_open_cache(domain, GNIX_MR_TYPE_NONE);
}

#if HAVE_KDREG
# define KDREG_CHECK false
#else
# define KDREG_CHECK true
#endif

/* bare tests */
TestSuite(mr_internal_bare,
	  .init = internal_mr_setup,
	  .fini = mr_teardown);

/* simple tests with lazy deregistration */
TestSuite(mr_internal_cache,
	  .init = internal_mr_setup,
	  .fini = mr_teardown,
	  .disabled = KDREG_CHECK);

#ifdef HAVE_UDREG
/*
 * mr_udreg_cache doesn't work if KDREG is enabled
 * since by the time this testsuite is run, the kdreg device
 * has been opened as part of the criterion test suite
 * run.
 * /dev/kdreg should really be fixed, but that's probably
 * not going to happen.
 */
TestSuite(mr_udreg_cache,
	  .init = udreg_setup,
	  .fini = mr_teardown,
	  .disabled = ~KDREG_CHECK);
#endif

TestSuite(mr_no_cache_basic,
	  .init = no_cache_basic_setup,
	  .fini = mr_teardown);
TestSuite(mr_no_cache_scalable,
	  .init = no_cache_scalable_setup,
	  .fini = mr_teardown);

/* simple tests without lazy deregistration */
TestSuite(mr_internal_cache_nld,
	  .init = internal_mr_setup_nld,
	  .fini = mr_teardown);


#ifdef HAVE_UDREG
TestSuite(mr_udreg_cache_nld,
	  .init = udreg_setup_nld,
	  .fini = mr_teardown);
#endif


/* performance tests */
TestSuite(perf_mr_internal,
	  .init = internal_mr_setup,
	  .fini = mr_teardown,
	  .disabled = true);

#ifdef HAVE_UDREG
TestSuite(perf_mr_udreg,
	  .init = udreg_setup,
	  .fini = mr_teardown,
	  .disabled = true);
#endif

TestSuite(perf_mr_no_cache,
	  .init = no_cache_basic_setup,
	  .fini = mr_teardown,
	  .disabled = true);

/* test hooks */

static int __simple_init_hook(const char *func, int line)
{
	cr_assert(GET_DOMAIN_RO_CACHE(domain)->state == GNIX_MRC_STATE_READY);
	cr_assert(GET_DOMAIN_RW_CACHE(domain)->state == GNIX_MRC_STATE_READY);

	return 0;
}

static int __simple_post_reg_hook(const char *func, int line, int cache_type,
		int expected_inuse,
		int expected_stale)
{
	if (cache_type == CACHE_RO)
		cache = GET_DOMAIN_RO_CACHE(domain);
	else
		cache = GET_DOMAIN_RW_CACHE(domain);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == expected_inuse,
		"%s:%d failed expected inuse condition, actual=%d expected=%d\n",
		func, line, ofi_atomic_get32(&cache->inuse.elements), expected_inuse);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == expected_stale,
		"%s:%d failed expected stale condition, actual=%d expected=%d\n",
		func, line, ofi_atomic_get32(&cache->stale.elements), expected_stale);

	return 0;
}

static int __simple_post_dereg_hook(const char *func, int line,
		int expected_inuse,
		int expected_stale)
{
	cache = GET_DOMAIN_RW_CACHE(domain);
	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == expected_inuse);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == expected_stale);

	return 0;
}


/* We won't do a very of the 'no cache' tests with nld "no lazy-dereg' as it
   just doesn't make sense */

/* Test simple init, register and deregister */
Test(mr_internal_bare, basic_init)
{
	int ret;

	/* ensure that the memory registration key is the right size */
	cr_assert_eq(sizeof(gnix_mr_key_t), 8);

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);
}

/* Test simple init, register and deregister */
Test(mr_no_cache_scalable, basic_init_update)
{
	int ret;
	struct fi_gni_ops_domain *gni_domain_ops;
	void *base, *addr;
	int len;
	uint64_t align_len = 0;
	struct iovec iov;

	ret = fi_open_ops(&dom->fid, FI_GNI_DOMAIN_OPS_1,
			0, (void **) &gni_domain_ops, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_open_ops");

	/* registrations must be made on aligned boundaries */
	base = sbrk(0);
	if ((uint64_t) base & GNIX_ASSUMED_ALIGNMENT) {
		align_len = 0x1000 - ((uint64_t) base & GNIX_ASSUMED_ALIGNMENT);
		sbrk(align_len);
		base = (void *) ((uint64_t)base + align_len);
	}

	len = 1 << 16;

	ret = fi_mr_reg(dom, base, len*4, FI_REMOTE_READ,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS, "actual=%d expected=%d", ret, FI_SUCCESS);

	/* assume we'll get the memory */
	ret = brk((void *)((uint64_t) base + (uint64_t) (len * 2)));
	cr_assert(ret == 0);

	addr = (void *) ((uint64_t) base + (uint64_t) len);

	iov.iov_base = addr;
	iov.iov_len = len;

	ret = fi_mr_refresh(mr, &iov, 1, 0);
	cr_assert_eq(ret, FI_SUCCESS, "expected=%d actual=%d", FI_SUCCESS, ret);

	/* retract memory and assume we didn't step on anyone */
	addr = sbrk(-len);
	addr = sbrk(align_len);
	cr_assert(addr >= base);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);
}


/* Test simple init, register and deregister */
Test(mr_internal_bare, basic_init_regv)
{
	int ret;
	const struct iovec iov = {
		.iov_base = buf,
		.iov_len = buf_len,
	};

	ret = fi_mr_regv(dom, &iov, 1, default_access,
		default_offset, default_req_key,
		default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);
}


/* Test simple init, register and deregister */
Test(mr_internal_bare, basic_init_regattr)
{
	int ret;
	const struct iovec iov = {
		.iov_base = buf,
		.iov_len = buf_len,
	};
	struct fi_mr_attr attr = {
		.mr_iov = &iov,
		.iov_count = 1,
		.access = default_access,
		.offset = default_offset,
		.requested_key = default_req_key,
		.context = NULL,
	};

	ret = fi_mr_regattr(dom, &attr, default_flags, &mr);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);
}


/* Test simple init, register and deregister, no NIC present */
Test(mr_internal_bare, bug_1086)
{
	struct gnix_fid_mem_desc *g_mr;
	struct gnix_nic *g_nic;
	int ret;

	_set_lazy_deregistration(0);

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
                        default_offset, default_req_key,
                        default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	g_mr = (struct gnix_fid_mem_desc *) container_of(mr, \
		struct gnix_fid_mem_desc, mr_fid);

	g_nic = g_mr->nic;
	cr_assert(ofi_atomic_get32(&g_nic->ref_cnt.references) > 0);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);
}

/* Test invalid flags to fi_mr_reg */
Test(mr_internal_bare, invalid_flags)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			~0, &mr, NULL);
	cr_assert(ret == -FI_EBADFLAGS);
}

/* Test invalid access param to fi_mr_reg */
Test(mr_internal_bare, invalid_access)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, 0,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == -FI_EINVAL);
}

/* Test invalid offset param to fi_mr_reg */
Test(mr_internal_bare, invalid_offset)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			~0, default_req_key, default_flags,
			&mr, NULL);
	cr_assert(ret == -FI_EINVAL);
}

/* Test invalid buf param to fi_mr_reg */
Test(mr_internal_bare, invalid_buf)
{
	int ret;

	ret = fi_mr_reg(dom, NULL, buf_len, default_access,
			default_offset, default_req_key, default_flags,
			&mr, NULL);
	cr_assert(ret == -FI_EINVAL);
}

/* Test invalid mr_o param to fi_mr_reg */
Test(mr_internal_bare, invalid_mr_ptr)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key, default_flags,
			NULL, NULL);
	cr_assert(ret == -FI_EINVAL);
}


Test(mr_internal_bare, invalid_attr)
{
	int ret;

	ret = fi_mr_regattr(dom, NULL, default_flags, &mr);
	cr_assert(ret == -FI_EINVAL);
}


Test(mr_internal_bare, invalid_iov_count)
{
	int ret;
	const struct iovec iov = {
		.iov_base = buf,
		.iov_len = buf_len,
	};

	ret = fi_mr_regv(dom, &iov, 0, default_access,
		default_offset, default_req_key, default_flags,
		&mr, NULL);
	cr_assert(ret == -FI_EINVAL);
}

Test(mr_internal_bare, invalid_iov)
{
	int ret;

	ret = fi_mr_regv(dom, NULL, 1, default_access,
		default_offset, default_req_key, default_flags,
		&mr, NULL);
	cr_assert(ret == -FI_EINVAL);
}

Test(mr_internal_bare, unsupported_iov_count)
{
	int ret;
	const struct iovec iov[2] = {
		{
			.iov_base = buf,
			.iov_len = buf_len >> 2,
		},
		{
			.iov_base = buf + (buf_len >> 1),
			.iov_len = buf_len >> 2,
		},
	};

	ret = fi_mr_regv(dom,(const struct iovec *) &iov, 2, default_access,
		default_offset, default_req_key, default_flags,
		&mr, NULL);
	cr_assert(ret == -FI_EOPNOTSUPP);
}

/* Test invalid fid param to fi_mr_reg */
Test(mr_internal_bare, invalid_fid_class)
{
	int ret;
	size_t old_class = dom->fid.fclass;

	dom->fid.fclass = FI_CLASS_UNSPEC;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key, default_flags,
			&mr, NULL);
	cr_assert(ret == -FI_EINVAL);

	/* restore old fclass for teardown */
	dom->fid.fclass = old_class;
}

/* Test invalid access param to fi_mr_reg */
Test(mr_internal_bare, invalid_requested_key)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, 1000,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS, "ret=%d\n", ret);

	cr_assert(fi_mr_key(mr) != 1000);
}

Test(mr_no_cache_scalable, invalid_user_requested_key)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, 1000,
			default_flags, &mr, NULL);
	cr_assert(ret == -FI_EKEYREJECTED);
}

Test(mr_no_cache_scalable, invalid_key_already_assigned)
{
	int ret;
	struct fid_mr *invalid;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, 1,
			default_flags, &mr, NULL);

	cr_assert(ret == FI_SUCCESS);

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, 1,
			default_flags, &invalid, NULL);
	cr_assert(ret == -FI_ENOKEY);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);
}

/* more advanced test setups */
static struct _mr_test_hooks __simple_test_hooks = {
	.init_hook = __simple_init_hook,
	.post_reg_hook = __simple_post_reg_hook,
	.post_dereg_hook = __simple_post_dereg_hook,
};

static void __simple_init_test(HOOK_DECL)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	if (USING_SCALABLE(fi))
		MR_ENABLE(mr, buf, buf_len);

	CHECK_HOOK(init_hook);

	CHECK_HOOK(post_reg_hook, CACHE_RW, 1, 0);
	CHECK_HOOK(post_reg_hook, CACHE_RO, 0, 0);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	CHECK_HOOK(post_dereg_hook, 0, 1);
	CHECK_HOOK(post_reg_hook, CACHE_RO, 0, 0);
}

static void __simple_init_ro_test(HOOK_DECL)
{
	int ret;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, ro_access,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	if (USING_SCALABLE(fi))
		MR_ENABLE(mr, buf, buf_len);

	CHECK_HOOK(init_hook);

	CHECK_HOOK(post_reg_hook, CACHE_RW, 0, 0);
	CHECK_HOOK(post_reg_hook, CACHE_RO, 1, 0);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	CHECK_HOOK(post_dereg_hook, 0, 0);
	CHECK_HOOK(post_reg_hook, CACHE_RO, 0, 1);
}

Test(mr_internal_cache, change_hard_soft_limits)
{
	int ret;
	struct fi_gni_ops_domain *gni_domain_ops;
	uint32_t val, get_val;

	ret = fi_open_ops(&domain->domain_fid.fid, FI_GNI_DOMAIN_OPS_1,
			  0, (void **) &gni_domain_ops, NULL);
	cr_assert(ret == FI_SUCCESS, "fi_open_ops");


	val = 8192;
	ret = gni_domain_ops->set_val(&domain->domain_fid.fid,
			GNI_MR_HARD_REG_LIMIT, &val);
	cr_assert(ret == FI_SUCCESS);

	ret = gni_domain_ops->get_val(&domain->domain_fid.fid,
			GNI_MR_HARD_REG_LIMIT, &get_val);
	cr_assert(ret == FI_SUCCESS);
	cr_assert(val == get_val);

	val = 4096;
	ret = gni_domain_ops->set_val(&domain->domain_fid.fid,
			GNI_MR_SOFT_REG_LIMIT, &val);
	cr_assert(ret == FI_SUCCESS);

	ret = gni_domain_ops->get_val(&domain->domain_fid.fid,
			GNI_MR_SOFT_REG_LIMIT, &get_val);
	cr_assert(ret == FI_SUCCESS);
	cr_assert(val == get_val);

	val = 256;
	ret = gni_domain_ops->set_val(&domain->domain_fid.fid,
			GNI_MR_HARD_STALE_REG_LIMIT, &val);
	cr_assert(ret == FI_SUCCESS);

	ret = gni_domain_ops->get_val(&domain->domain_fid.fid,
			GNI_MR_HARD_STALE_REG_LIMIT, &get_val);
	cr_assert(ret == FI_SUCCESS);
	cr_assert(val == get_val);

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	cache = GET_DOMAIN_RW_CACHE(domain);
	cr_assert(cache->state == GNIX_MRC_STATE_READY);
	cr_assert(cache->attr.hard_reg_limit == 8192);
	cr_assert(cache->attr.soft_reg_limit == 4096);
	cr_assert(cache->attr.hard_stale_limit == 256);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == 1);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == 0);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == 0);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == 1);
}

/* Test duplicate registration. Since this is a valid operation, we
 *   provide a unique fid_mr but internally, a second reference to the same
 *   entry is provided to prevent expensive calls to GNI_MemRegister
 */
static void __simple_duplicate_registration_test(HOOK_DECL)
{
	int ret;
	struct fid_mr *f_mr;

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	CHECK_HOOK(init_hook);

	CHECK_HOOK(post_reg_hook, CACHE_RW, 1, 0);
	CHECK_HOOK(post_reg_hook, CACHE_RO, 0, 0);

	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			default_flags, &f_mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	CHECK_HOOK(post_reg_hook, CACHE_RW, 1, 0);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&f_mr->fid);
	cr_assert(ret == FI_SUCCESS);

	CHECK_HOOK(post_reg_hook, CACHE_RW, 0, 1);
}

static int __post_dereg_greater_or_equal(const char *func, int line,
		int expected_inuse,
		int expected_stale)
{
	cache = GET_DOMAIN_RW_CACHE(domain);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == expected_inuse,
		"failed expected inuse test, actual=%d expected=%d\n",
		ofi_atomic_get32(&cache->inuse.elements),
		expected_inuse);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) >= expected_stale,
		"failed expected stale test, actual=%d expected=%d\n",
		ofi_atomic_get32(&cache->stale.elements),
		expected_stale);

	return 0;
}

static struct _mr_test_hooks __simple_rdr_hooks = {
		.post_reg_hook = __simple_post_reg_hook,
		.post_dereg_hook = __post_dereg_greater_or_equal,
};

/* Test registration of 1024 elements, all distinct. Cache element counts
 *   should meet expected values
 */
static void __simple_register_distinct_regions(HOOK_DECL, int regions)
{
	int ret;
	uint64_t **buffers;
	char *buffer;
	struct fid_mr **mr_arr;
	int i;

	mr_arr = calloc(regions, sizeof(struct fid_mr *));
	cr_assert(mr_arr);

	buffers = calloc(regions, sizeof(uint64_t *));
	cr_assert(buffers, "failed to allocate array of buffers");

	buffer = calloc(regions * 4 * __BUF_LEN, sizeof(char));
	cr_assert(buffer);

	for (i = 0; i < regions; ++i) {
		buffers[i] = (uint64_t *) (buffer + ((i * 4) * __BUF_LEN));
	}

	for (i = 0; i < regions; ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], __BUF_LEN,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	CHECK_HOOK(post_reg_hook, CACHE_RW, regions, 0);

	for (i = 0; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	free(buffers);
	buffers = NULL;

	free(mr_arr);
	mr_arr = NULL;

	free(buffer);
	buffer = NULL;

	CHECK_HOOK(post_dereg_hook, 0, 0);
}

static int __post_dereg_greater_than(const char *func, int line,
		int expected_inuse,
		int expected_stale)
{
	cache = GET_DOMAIN_RW_CACHE(domain);
	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == expected_inuse);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) > expected_stale);

	return 0;
}

static struct _mr_test_hooks __simple_rnur_hooks = {
		.post_reg_hook = __simple_post_reg_hook,
		.post_dereg_hook = __post_dereg_greater_than,
};

/* Test registration of 1024 registrations backed by the same initial
 *   registration. There should only be a single registration in the cache
 */
static void __simple_register_non_unique_regions_test(HOOK_DECL, int regions)
{
	int ret;
	char *hugepage;
	struct fid_mr *hugepage_mr;
	char **buffers;
	struct fid_mr **mr_arr;
	int i;

	mr_arr = calloc(regions, sizeof(struct fid_mr *));
	cr_assert(mr_arr);

	buffers = calloc(regions, sizeof(uint64_t *));
	cr_assert(buffers, "failed to allocate array of buffers");

	hugepage = calloc(regions * regions, sizeof(char));
	cr_assert(hugepage);

	for (i = 0; i < regions; ++i) {
		buffers[i] = &hugepage[i * regions];
		cr_assert(buffers[i]);
	}

	ret = fi_mr_reg(dom, (void *) hugepage,
			regions * regions * sizeof(char),
			default_access, default_offset, default_req_key,
			default_flags, &hugepage_mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	for (i = 0; i < regions; ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], regions,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	CHECK_HOOK(post_reg_hook, CACHE_RW, 1, 0);

	for (i = 0; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	ret = fi_close(&hugepage_mr->fid);
	cr_assert(ret == FI_SUCCESS);

	free(hugepage);
	hugepage = NULL;

	free(buffers);
	buffers = NULL;

	free(mr_arr);
	mr_arr = NULL;

	CHECK_HOOK(post_dereg_hook, 0, 0);
}

static int __get_lazy_dereg_limit(const char *func, int line)
{
	cache = GET_DOMAIN_RW_CACHE(domain);

	return cache->attr.hard_stale_limit;
}

static struct _mr_test_hooks __simple_lazy_hooks = {
		.post_reg_hook = __simple_post_reg_hook,
		.post_dereg_hook = __simple_post_dereg_hook,
		.get_lazy_dereg_limit = __get_lazy_dereg_limit,
};


/* Test registration of 128 regions that will be cycled in and out of the
 *   inuse and stale trees. inuse + stale should never exceed 128
 */
static void __simple_cyclic_register_distinct_regions(HOOK_DECL, int regions)
{
	int ret;
	char **buffers;
	char *hugepage;
	struct fid_mr **mr_arr;
	int i;
	int buf_size = __BUF_LEN * sizeof(char);
	int lazy_limit = 0;

	mr_arr = calloc(regions, sizeof(struct fid_mr *));
	cr_assert(mr_arr);

	buffers = calloc(regions, sizeof(char *));
	cr_assert(buffers, "failed to allocate array of buffers");

	hugepage = calloc(regions * 4 * __BUF_LEN, sizeof(char));
	cr_assert(hugepage);

	for (i = 0; i < regions; ++i) {
		buffers[i] = (char *) (hugepage + ((i * 4) * __BUF_LEN));
	}

	/* create the initial memory registrations */
	for (i = 0; i < regions; ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], buf_size,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	if (HOOK_PRESENT(get_lazy_dereg_limit)) {
		lazy_limit = CHECK_HOOK(get_lazy_dereg_limit);
	}

	/* all registrations should now be 'in-use' */
	CHECK_HOOK(post_reg_hook, CACHE_RW, regions, 0);

	for (i = 0; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	/* all registrations should now be 'stale' */
	CHECK_HOOK(post_dereg_hook, 0, lazy_limit);

	for (i = 0; i < regions; ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], buf_size,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);

		CHECK_HOOK(post_reg_hook, CACHE_RW, i + 1, regions - (i + 1));
	}

	/* all registrations should have been moved from 'stale' to 'in-use' */
	CHECK_HOOK(post_reg_hook, CACHE_RW, regions, 0);

	for (i = 0; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	/* all registrations should now be 'stale' */
	CHECK_HOOK(post_dereg_hook, 0, regions);

	free(buffers);
	buffers = NULL;

	free(mr_arr);
	mr_arr = NULL;

	free(hugepage);
	hugepage = NULL;
}

static int __test_stale_lt_or_equal(const char *func, int line,
		int cache_type,
		int expected_inuse,
		int expected_stale)
{
	if (cache_type == CACHE_RO)
		cache = GET_DOMAIN_RO_CACHE(domain);
	else
		cache = GET_DOMAIN_RW_CACHE(domain);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == expected_inuse);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) <= expected_stale);

	return 0;
}

static struct _mr_test_hooks __simple_sais_hooks = {
		.post_reg_hook = __test_stale_lt_or_equal,
		.post_dereg_hook = __simple_post_dereg_hook,
		.get_lazy_dereg_limit = __get_lazy_dereg_limit,
};


/* Test repeated registration of a memory region with the same base
 * address, increasing the size each time..  This is an explicit
 * version of what the test rdm_sr::send_autoreg_uncached does under
 * the covers (currently).
 */
static void __simple_same_addr_incr_size_test(HOOK_DECL)
{
	int ret;
	int i;

	for (i = 2; i <= buf_len; i *= 2) {
		ret = fi_mr_reg(dom, (void *) buf, i, default_access,
				default_offset, default_req_key,
				default_flags, &mr, NULL);
		cr_assert(ret == FI_SUCCESS);

		CHECK_HOOK(init_hook);

		CHECK_HOOK(post_reg_hook, CACHE_RW, 1, 1);

		ret = fi_close(&mr->fid);
		cr_assert(ret == FI_SUCCESS);

		CHECK_HOOK(post_dereg_hook, 0, 1);
	}
}

/* Same as above, except with decreasing sizes */
static void  __simple_same_addr_decr_size_test(HOOK_DECL)
{
	int ret;
	int i;

	for (i = buf_len; i >= 2; i /= 2) {
		ret = fi_mr_reg(dom, (void *) buf, i, default_access,
				default_offset, default_req_key,
				default_flags, &mr, NULL);
		cr_assert(ret == FI_SUCCESS);

		CHECK_HOOK(init_hook);

		CHECK_HOOK(post_reg_hook, CACHE_RW, 1, 0);

		ret = fi_close(&mr->fid);
		cr_assert(ret == FI_SUCCESS);

		CHECK_HOOK(post_dereg_hook, 0, 1);
	}
}

/* Test simple cache initialization */
Test(mr_internal_cache, basic_init)
{
	__simple_init_test(&__simple_test_hooks);
}

Test(mr_internal_cache, basic_init_ro)
{
	__simple_init_ro_test(&__simple_test_hooks);
}

/* Test duplicate registration. Since this is a valid operation, we
 *   provide a unique fid_mr but internally, a second reference to the same
 *   entry is provided to prevent expensive calls to GNI_MemRegister
 */
Test(mr_internal_cache, duplicate_registration)
{
	__simple_duplicate_registration_test(&__simple_test_hooks);
}

/* Test registration of 1024 elements, all distinct. Cache element counts
 *   should meet expected values
 */
Test(mr_internal_cache, register_1024_distinct_regions)
{
	__simple_register_distinct_regions(&__simple_rdr_hooks,
		DEFAULT_REGION_COUNT);
}

/* Test registration of 1024 registrations backed by the same initial
 *   registration. There should only be a single registration in the cache
 */
Test(mr_internal_cache, register_1024_non_unique_regions)
{
	__simple_register_non_unique_regions_test(&__simple_rnur_hooks,
		DEFAULT_REGION_COUNT);
}

/* Test registration of 128 regions that will be cycled in and out of the
 *   inuse and stale trees. inuse + stale should never exceed 128
 */
Test(mr_internal_cache, cyclic_register_128_distinct_regions)
{
	__simple_cyclic_register_distinct_regions(&__simple_lazy_hooks, 128);
}

/* Test repeated registration of a memory region with the same base
 * address, increasing the size each time..  This is an explicit
 * version of what the test rdm_sr::send_autoreg_uncached does under
 * the covers (currently).
 */
Test(mr_internal_cache, same_addr_incr_size)
{
	__simple_same_addr_incr_size_test(&__simple_sais_hooks);
}

/* Same as above, except with decreasing sizes */
Test(mr_internal_cache, same_addr_decr_size)
{
	__simple_same_addr_decr_size_test(&__simple_test_hooks);
}

Test(mr_internal_cache, lru_evict_first_entry)
{
	int ret;
	char **buffers;
	char *hugepage;
	struct fid_mr **mr_arr;
	int i;
	int buf_size = __BUF_LEN * sizeof(char);
	int regions;

	regions = domain->mr_cache_attr.hard_stale_limit << 1;
	cr_assert(regions > 0);
	mr_arr = calloc(regions, sizeof(struct fid_mr *));
	cr_assert(mr_arr);

	buffers = calloc(regions, sizeof(char *));
	cr_assert(buffers, "failed to allocate array of buffers");

	hugepage = calloc(regions * 4 * __BUF_LEN, sizeof(char));
	cr_assert(hugepage);

	for (i = 0; i < regions; ++i) {
		buffers[i] = (char *) (hugepage + ((i * 4) * __BUF_LEN));
	}

	/* create the initial memory registrations */
	for (i = 0; i < regions; ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], buf_size,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	/* all registrations should now be 'in-use' */
	cache = GET_DOMAIN_RW_CACHE(domain);
	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == regions);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == 0);

	/* deregister cache->stale_reg_limit + 1 to test if the first region was
	 *   deregistered
	 */
	for (i = 0; i < cache->attr.hard_stale_limit + 1; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	for (i = 1; i < MIN(cache->attr.hard_stale_limit + 1, regions); ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], buf_size,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	/* all registrations should now be 'stale' */
	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == regions - 1);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == 0);

	for (i = 1; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	/* all registrations should now be 'stale' */
	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == 0);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == MIN(regions - 1,
			cache->attr.hard_stale_limit));

	free(buffers);
	buffers = NULL;

	free(mr_arr);
	mr_arr = NULL;

	free(hugepage);
	hugepage = NULL;
}

Test(mr_internal_cache, lru_evict_middle_entry)
{
	int ret;
	char **buffers;
	char *hugepage;
	struct fid_mr **mr_arr;
	int i, limit;
	int buf_size = __BUF_LEN * sizeof(char);
	int regions;

	regions = domain->mr_cache_attr.hard_stale_limit << 1;
	cr_assert(regions > 0);
	mr_arr = calloc(regions, sizeof(struct fid_mr *));
	cr_assert(mr_arr);

	buffers = calloc(regions, sizeof(char *));
	cr_assert(buffers, "failed to allocate array of buffers");

	hugepage = calloc(regions * 4 * __BUF_LEN, sizeof(char));
	cr_assert(hugepage);

	for (i = 0; i < regions; ++i) {
		buffers[i] = (char *) (hugepage + ((i * 4) * __BUF_LEN));
	}

	/* create the initial memory registrations */
	for (i = 0; i < regions; ++i) {
		ret = fi_mr_reg(dom, (void *) buffers[i], buf_size,
				default_access,	default_offset, default_req_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	/* all registrations should now be 'in-use' */
	cache = GET_DOMAIN_RW_CACHE(domain);
	limit = cache->attr.hard_stale_limit;
	cr_assert(limit < regions);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == regions);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == 0);

	/* deregister cache->stale_reg_limit + 1 to test if the first region was
	 *   deregistered
	 */
	for (i = 0; i < limit + 1; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == (regions - (limit + 1)));
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == limit);

	/* re-register this region in the middle to test removal */
	i = (regions >> 2);
	ret = fi_mr_reg(dom, (void *) buffers[i], buf_size,
			default_access,	default_offset, default_req_key,
			default_flags, &mr_arr[i], NULL);
	cr_assert(ret == FI_SUCCESS);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == (regions - limit));
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == (limit - 1));

	for (i = limit + 1; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == 1);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == limit);

	i = (regions >> 2);
	ret = fi_close(&mr_arr[i]->fid);
	cr_assert(ret == FI_SUCCESS);

	/* all registrations should now be 'stale' */
	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == 0);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == limit);

	free(buffers);
	buffers = NULL;

	free(mr_arr);
	mr_arr = NULL;

	free(hugepage);
	hugepage = NULL;
}

static inline void _repeated_registration(const char *label)
{
	int ret, i;
	int region_len = 1 << 24;
	int registrations = 4096 * 16;
	unsigned char *region = calloc(region_len, sizeof(unsigned char));
	struct fid_mr **f_mr;
	int reg_time, dereg_time, seconds;

	cr_assert(region != NULL);

	f_mr = calloc(registrations, sizeof(*f_mr));
	cr_assert(f_mr != NULL);

	ret = fi_mr_reg(dom, (void *) region,
					region_len, default_access,
					default_offset, default_req_key,
					default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	cache = GET_DOMAIN_RW_CACHE(domain);

	gettimeofday(&s1, 0);
	for (i = 0; i < registrations; i++) {
		ret = fi_mr_reg(dom, (void *) region,
				region_len, default_access,
				default_offset, default_req_key,
				default_flags, &f_mr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &seconds, &reg_time);
	reg_time += seconds * 1000000;

	gettimeofday(&s1, 0);
	for (i = 0; i < registrations; i++) {
		ret = fi_close(&f_mr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &seconds, &dereg_time);
	dereg_time += seconds * 1000000;

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	fprintf(stderr, "[%s] best(repeated) case: reg_time=%.3f "
			"usec dereg_time=%.3f usec\n", label,
			reg_time / (registrations * 1.0),
			dereg_time / (registrations * 1.0));

	free(region);
}



static inline void _single_large_registration(const char *label)
{
	int ret, i;
	int region_len = 1 << 24;
	int registration_width = 1 << 12;
	int registrations = region_len / registration_width;
	unsigned char *region = calloc(region_len, sizeof(unsigned char));
	struct fid_mr **f_mr;
	int reg_time, dereg_time, seconds;

	cr_assert(region != NULL);

	f_mr = calloc(registrations, sizeof(*f_mr));
	cr_assert(f_mr != NULL);

	ret = fi_mr_reg(dom, (void *) region,
					region_len, default_access,
					default_offset, default_req_key,
					default_flags, &mr, NULL);

	cache = GET_DOMAIN_RW_CACHE(domain);

	gettimeofday(&s1, 0);
	for (i = 0; i < registrations; i++) {
		ret = fi_mr_reg(dom, (void *) (region +
					       (registration_width * i)),
				registration_width, default_access,
				default_offset, default_req_key,
				default_flags, &f_mr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &seconds, &reg_time);
	reg_time += seconds * 1000000;

	gettimeofday(&s1, 0);
	for (i = 0; i < registrations; i++) {
		ret = fi_close(&f_mr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &seconds, &dereg_time);
	dereg_time += seconds * 1000000;

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	fprintf(stderr, "[%s] best(overlap) case: reg_time=%.3f "
			"usec dereg_time=%.3f usec\n", label,
			reg_time / (registrations * 1.0),
			dereg_time / (registrations * 1.0));

	free(region);
}

static inline void _random_analysis(const char *label)
{
	int ret, i;
	int region_len = 1 << 24;
	int registration_width = 1 << 12;
	int registrations = region_len / registration_width;
	unsigned char *region = calloc(region_len, sizeof(unsigned char));
	struct fid_mr **f_mr;
	int reg_time, dereg_time, seconds;
	void *ptr;
	uint64_t ptr_len;

	srand(0xDEADBEEF);
	cr_assert(region != NULL);

	f_mr = calloc(registrations, sizeof(*f_mr));
	cr_assert(f_mr != NULL);

	/* prep the cache by adding and removing an entry before timing */
	ret = fi_mr_reg(dom, (void *) buf, buf_len, default_access,
			default_offset, default_req_key,
			default_flags, &mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&mr->fid);
	cr_assert(ret == FI_SUCCESS);

	gettimeofday(&s1, 0);
	for (i = 0; i < registrations; i++) {
		ptr = region + rand() % region_len;
		ptr_len = registration_width;
		if ((uint64_t) ((char *) ptr + ptr_len) >
		    (uint64_t) (region + region_len)) {
			ptr_len = ((uint64_t) region + region_len) - (uint64_t) ptr;
		}

		ret = fi_mr_reg(dom, (void *) ptr,
				ptr_len, default_access,
				default_offset, default_req_key,
				default_flags, &f_mr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &seconds, &reg_time);
	reg_time += seconds * 1000000;

	gettimeofday(&s1, 0);
	for (i = 0; i < registrations; i++) {
		ret = fi_close(&f_mr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &seconds, &dereg_time);
	dereg_time += seconds * 1000000;

	fprintf(stderr, "[%s] random case: reg_time=%.3f usec "
			"dereg_time=%.3f usec\n", label,
			reg_time / (registrations * 1.0),
			dereg_time / (registrations * 1.0));

	free(region);
}

Test(perf_mr_internal, repeated_registration)
{
	_repeated_registration("internal");
}

Test(perf_mr_internal, single_large_registration)
{
	_single_large_registration("internal");
}

Test(perf_mr_internal, random_analysis)
{
	_random_analysis("internal");
}

/*
 * This test exercises the ability of the cache to drop registrations that
 * have been subsumed by other registrations
 */
Test(mr_internal_cache, regression_615)
{
	int ret;
	struct fid_mr *f_mr;
	char *buffer = calloc(1 << 19, sizeof(char));

	cr_assert(buffer != NULL);

	/* set up stale cache */
	ret = fi_mr_reg(dom, (void *) (buffer + 0x18000), 0x8000,
			default_access, default_offset, default_req_key,
			default_flags, &f_mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&f_mr->fid);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_mr_reg(dom, (void *) (buffer + 0x0), 0x80000,
			default_access, default_offset, default_req_key,
			default_flags, &f_mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_close(&f_mr->fid);
	cr_assert(ret == FI_SUCCESS);

	/* set up inuse */
	ret = fi_mr_reg(dom, (void *) (buffer + 0x28000), 0x4000,
			default_access, default_offset, default_req_key,
			default_flags, &f_mr, NULL);
	cr_assert(ret == FI_SUCCESS);

	cache = GET_DOMAIN_RW_CACHE(domain);

	cr_assert(ofi_atomic_get32(&cache->inuse.elements) == 1);
	cr_assert(ofi_atomic_get32(&cache->stale.elements) == 0);

	ret = fi_close(&f_mr->fid);
	cr_assert(ret == FI_SUCCESS);

	free(buffer);
}

void simple_register_distinct_regions(int regions)
{
	int ret;
	uint64_t **buffers;
	char *buffer;
	struct fid_mr **mr_arr;
	int i;
	int requested_key;

	mr_arr = calloc(regions, sizeof(struct fid_mr *));
	cr_assert(mr_arr);

	buffers = calloc(regions, sizeof(uint64_t *));
	cr_assert(buffers, "failed to allocate array of buffers");

	buffer = calloc(regions * 4 * __BUF_LEN, sizeof(char));
	cr_assert(buffer);

	for (i = 0; i < regions; ++i) {
		buffers[i] = (uint64_t *) (buffer + ((i * 4) * __BUF_LEN));
	}

	for (i = 0; i < regions; ++i) {
		requested_key = USING_SCALABLE(fi) ? i : default_req_key;
		ret = fi_mr_reg(dom, (void *) buffers[i], __BUF_LEN,
				default_access,	default_offset, requested_key,
				default_flags, &mr_arr[i], NULL);
		cr_assert(ret == FI_SUCCESS);
	}

	for (i = 0; i < regions; ++i) {
		ret = fi_close(&mr_arr[i]->fid);
		cr_assert(ret == FI_SUCCESS);
	}

	free(buffers);
	buffers = NULL;

	free(mr_arr);
	mr_arr = NULL;

	free(buffer);
	buffer = NULL;
}

void simple_register_non_unique_regions(int regions)
{
	int ret;
		char *hugepage;
		struct fid_mr *hugepage_mr;
		char **buffers;
		struct fid_mr **mr_arr;
		int i;
		int requested_key;

		mr_arr = calloc(regions, sizeof(struct fid_mr *));
		cr_assert(mr_arr);

		buffers = calloc(regions, sizeof(uint64_t *));
		cr_assert(buffers, "failed to allocate array of buffers");

		hugepage = calloc(regions * regions, sizeof(char));
		cr_assert(hugepage);

		for (i = 0; i < regions; ++i) {
			buffers[i] = &hugepage[i * regions];
			cr_assert(buffers[i]);
		}

		ret = fi_mr_reg(dom, (void *) hugepage,
				regions * regions * sizeof(char),
				default_access, default_offset, default_req_key,
				default_flags, &hugepage_mr, NULL);
		cr_assert(ret == FI_SUCCESS);

		for (i = 0; i < regions; ++i) {
			requested_key = USING_SCALABLE(fi) ?
				i + 1 : default_req_key;
			ret = fi_mr_reg(dom, (void *) buffers[i], regions,
					default_access,	default_offset,
					requested_key,
					default_flags, &mr_arr[i], NULL);
			cr_assert(ret == FI_SUCCESS);
		}

		for (i = 0; i < regions; ++i) {
			ret = fi_close(&mr_arr[i]->fid);
			cr_assert(ret == FI_SUCCESS);
		}

		ret = fi_close(&hugepage_mr->fid);
		cr_assert(ret == FI_SUCCESS);

		free(hugepage);
		hugepage = NULL;

		free(buffers);
		buffers = NULL;

		free(mr_arr);
		mr_arr = NULL;
}

#ifdef HAVE_UDREG
/* Test registration of 1024 elements, all distinct. Cache element counts
 *   should meet expected values
 */
Test(mr_udreg_cache, register_1024_distinct_regions)
{
	simple_register_distinct_regions(DEFAULT_REGION_COUNT);
}

/* Test registration of 1024 registrations backed by the same initial
 *   registration. There should only be a single registration in the cache
 */
Test(mr_udreg_cache, register_1024_non_unique_regions)
{
	simple_register_non_unique_regions(DEFAULT_REGION_COUNT);
}

/* performance tests */
Test(perf_mr_udreg, repeated_registration)
{
	_repeated_registration("udreg");
}

Test(perf_mr_udreg, single_large_registration)
{
	_single_large_registration("udreg");
}

Test(perf_mr_udreg, random_analysis)
{
	_random_analysis("udreg");
}

/* no lazy dereg tests */
Test(mr_udreg_cache_nld, register_1024_distinct_regions)
{
	simple_register_distinct_regions(DEFAULT_REGION_COUNT);
}

Test(mr_udreg_cache_nld, register_1024_non_unique_regions)
{
	simple_register_non_unique_regions(DEFAULT_REGION_COUNT);
}

#endif


Test(mr_no_cache_basic, register_1024_distinct_regions)
{
	int nbufs = DEFAULT_REGION_COUNT;

	simple_register_distinct_regions(nbufs);
}

Test(mr_no_cache_scalable, register_1024_distinct_regions)
{
	int nbufs = DEFAULT_SCALABLE_REGION_COUNT;

	simple_register_distinct_regions(nbufs);
}

/* Test registration of 1024 registrations backed by the same initial
 *   registration. There should only be a single registration in the cache
 */
Test(mr_no_cache_basic, register_1024_non_unique_regions)
{
	int nbufs = DEFAULT_REGION_COUNT;

	simple_register_non_unique_regions(nbufs);
}

Test(mr_no_cache_scalable, register_1024_non_unique_regions)
{
	int nbufs = DEFAULT_SCALABLE_REGION_COUNT;

	simple_register_non_unique_regions(nbufs);
}

Test(perf_mr_no_cache, repeated_registration)
{
	_repeated_registration("no caching");
}

Test(perf_mr_no_cache, single_large_registration)
{
	_single_large_registration("no caching");
}

Test(perf_mr_no_cache, random_analysis)
{
	_random_analysis("no caching");
}


/* simple tests without lazy deregistration. Empty hooks can be used with all
 * of the tests */

/* Test simple cache initialization */
Test(mr_internal_cache_nld, basic_init)
{
	__simple_init_test(&empty_hooks);
}

/* Test duplicate registration. Since this is a valid operation, we
 *   provide a unique fid_mr but internally, a second reference to the same
 *   entry is provided to prevent expensive calls to GNI_MemRegister
 */
Test(mr_internal_cache_nld, duplicate_registration)
{
	__simple_duplicate_registration_test(&empty_hooks);
}

/* Test registration of 1024 elements, all distinct. Cache element counts
 *   should meet expected values
 */
Test(mr_internal_cache_nld, register_1024_distinct_regions)
{
	__simple_register_distinct_regions(&empty_hooks, DEFAULT_REGION_COUNT);
}

/* Test registration of 1024 registrations backed by the same initial
 *   registration. There should only be a single registration in the cache
 */
Test(mr_internal_cache_nld, register_1024_non_unique_regions)
{
	__simple_register_non_unique_regions_test(&empty_hooks,
		DEFAULT_REGION_COUNT);
}

/* Test registration of 128 regions that will be cycled in and out of the
 *   inuse and stale trees. inuse + stale should never exceed 128
 */
Test(mr_internal_cache_nld, cyclic_register_128_distinct_regions)
{
	__simple_cyclic_register_distinct_regions(&empty_hooks, 128);
}

/* Test repeated registration of a memory region with the same base
 * address, increasing the size each time..  This is an explicit
 * version of what the test rdm_sr::send_autoreg_uncached does under
 * the covers (currently).
 */
Test(mr_internal_cache_nld, same_addr_incr_size)
{
	__simple_same_addr_incr_size_test(&empty_hooks);
}

/* Same as above, except with decreasing sizes */
Test(mr_internal_cache_nld, same_addr_decr_size)
{
	__simple_same_addr_decr_size_test(&empty_hooks);
}
