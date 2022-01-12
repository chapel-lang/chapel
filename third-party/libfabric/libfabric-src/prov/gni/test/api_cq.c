/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2020 Triad National Security, LLC. All rights reserved.
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
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "fi_ext_gni.h"
#include "common.h"

#if 1
#define dbg_printf(...)
#else
#define dbg_printf(...)				\
	do {					\
		printf(__VA_ARGS__);		\
		fflush(stdout);			\
	} while (0)
#endif

#define NUMEPS 2

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom[NUMEPS];
static struct fi_gni_ops_domain *gni_domain_ops[NUMEPS];
static struct fid_ep *ep[NUMEPS];
static struct fid_av *av[NUMEPS];
static void *ep_name[NUMEPS];
static fi_addr_t gni_addr[NUMEPS];
static struct fid_cq *msg_cq[NUMEPS];
static struct fi_info *fi[NUMEPS];
static struct fi_cq_attr cq_attr;
static struct fi_info *hints[NUMEPS];

#define BUF_SZ (1<<20)
static char *target, *target_base;
static char *source, *source_base;
static char *uc_target;
static char *uc_source;
static struct fid_mr *rem_mr[NUMEPS], *loc_mr[NUMEPS];
static uint64_t mr_key[NUMEPS];
static uint64_t cq_bind_flags;

void api_cq_bind(uint64_t flags)
{
	int ret, i;

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_ep_bind(ep[i], &msg_cq[i]->fid, flags);
		cr_assert(!ret, "fi_ep_bind");

		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_enable");
	}
}

static inline void __api_cq_setup(uint32_t version, int mr_mode)
{
	int ret, i, j;
	struct fi_av_attr attr;
	size_t addrlen = 0;

	for (i = 0; i < NUMEPS; i++) {
		hints[i] = fi_allocinfo();
		cr_assert(hints[i], "fi_allocinfo");

		hints[i]->domain_attr->cq_data_size = NUMEPS * 2;
		hints[i]->domain_attr->data_progress = FI_PROGRESS_AUTO;
		hints[i]->domain_attr->mr_mode = mr_mode;
		hints[i]->mode = mode_bits;
		hints[i]->fabric_attr->prov_name = strdup("gni");
	}

	/* Get info about fabric services with the provided hints */
	for (i = 0; i < NUMEPS; i++) {
		ret = fi_getinfo(version, NULL, 0, 0, hints[i],
				 &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = NUMEPS;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	/* 3x BUF_SZ for multi recv testing */
	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ * 3));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	uc_target = malloc(BUF_SZ);
	assert(uc_target);

	uc_source = malloc(BUF_SZ);
	assert(uc_source);

	ret = fi_fabric(fi[0]->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_domain(fab, fi[i], dom + i, NULL);
		cr_assert(!ret, "fi_domain");

		ret = fi_open_ops(&dom[i]->fid, FI_GNI_DOMAIN_OPS_1,
				  0, (void **) (gni_domain_ops + i), NULL);

		ret = fi_av_open(dom[i], &attr, av + i, NULL);
		cr_assert(!ret, "fi_av_open");

		ret = fi_endpoint(dom[i], fi[i], ep + i, NULL);
		cr_assert(!ret, "fi_endpoint");

		ret = fi_cq_open(dom[i], &cq_attr, msg_cq + i, 0);
		cr_assert(!ret, "fi_cq_open");

		ret = fi_getname(&ep[i]->fid, NULL, &addrlen);
		cr_assert(addrlen > 0);

		ep_name[i] = malloc(addrlen);
		cr_assert(ep_name[i] != NULL);

		ret = fi_getname(&ep[i]->fid, ep_name[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);
	}

	for (i = 0; i < NUMEPS; i++) {
		/* Insert all gni addresses into each av */
		for (j = 0; j < NUMEPS; j++) {
			ret = fi_av_insert(av[i], ep_name[j], 1, &gni_addr[j],
					   0, NULL);
			cr_assert(ret == 1);
		}

		ret = fi_ep_bind(ep[i], &av[i]->fid, 0);
		cr_assert(!ret, "fi_ep_bind");

	}

	for (i = 0; i < NUMEPS; i++) {
		int target_requested_key =
			USING_SCALABLE(fi[i]) ? (i * 2) : 0;
		int source_requested_key =
			USING_SCALABLE(fi[i]) ? (i * 2) + 1 : 0;

		ret = fi_mr_reg(dom[i],
				  target,
				  3 * BUF_SZ,
				  FI_REMOTE_WRITE,
				  0,
				  target_requested_key,
				  0,
				  rem_mr + i,
				  &target);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i],
				  source,
				  BUF_SZ,
				  FI_REMOTE_WRITE,
				  0,
				  source_requested_key,
				  0,
				  loc_mr + i,
				  &source);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi[i])) {
			MR_ENABLE(rem_mr[i], target, 3 * BUF_SZ);
			MR_ENABLE(loc_mr[i], source, BUF_SZ);
		}

		mr_key[i] = fi_mr_key(rem_mr[i]);
	}
}

static void api_cq_setup_basic(void)
{
	__api_cq_setup(fi_version(), GNIX_MR_BASIC);
}

static void api_cq_setup_scalable(void)
{
	__api_cq_setup(fi_version(), GNIX_MR_SCALABLE);
}

static void api_cq_teardown_common(bool unreg)
{
	int ret = 0, i = 0;

	for (; i < NUMEPS; i++) {
		if (unreg) {
			fi_close(&loc_mr[i]->fid);
			fi_close(&rem_mr[i]->fid);
		}

		ret = fi_close(&ep[i]->fid);
		cr_assert(!ret, "failure in closing ep.");

		ret = fi_close(&msg_cq[i]->fid);
		cr_assert(!ret, "failure in send cq.");

		ret = fi_close(&av[i]->fid);
		cr_assert(!ret, "failure in closing av.");

		ret = fi_close(&dom[i]->fid);
		cr_assert(!ret, "failure in closing domain.");

		fi_freeinfo(fi[i]);
		free(ep_name[i]);
		fi_freeinfo(hints[i]);
	}

	free(uc_source);
	free(uc_target);
	free(target_base);
	free(source_base);

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");
}

static void api_cq_teardown(void)
{
	api_cq_teardown_common(true);
}

void api_cq_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = seed++;
}

int api_cq_check_data(char *buf1, char *buf2, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (buf1[i] != buf2[i]) {
			printf("data mismatch, elem: %d, exp: %hhx, act: %hhx\n"
			       , i, buf1[i], buf2[i]);
			return 0;
		}
	}

	return 1;
}

#define MSG_SEND_ALLOWED(flags) \
	(flags & FI_SEND)
#define MSG_RECV_ALLOWED(flags) \
	(flags & FI_RECV)

void api_cq_send_allowed(ssize_t sz, uint64_t flags, char *fn)
{
	if (MSG_SEND_ALLOWED(flags)) {
		cr_assert(sz == 0, "%s failed flags:0x%lx sz:%ld",
			  fn, flags, sz);
	} else {
		cr_assert(sz < 0, "%s should fail flags:0x%lx sz:%ld",
			  fn, flags, sz);
	}
}

void api_cq_recv_allowed(ssize_t sz, uint64_t flags, char *fn)
{
	if (MSG_RECV_ALLOWED(cq_bind_flags)) {
		cr_assert(sz == 0, "%s failed flags:0x%lx sz:%ld",
			  fn, flags, sz);
	} else {
		cr_assert(sz < 0, "%s should fail flags:0x%lx sz:%ld",
			  fn, flags, sz);
	}
}

TestSuite(api_cq_basic,
	  .init = api_cq_setup_basic,
	  .fini = api_cq_teardown,
	  .disabled = false);

TestSuite(api_cq_scalable,
	  .init = api_cq_setup_scalable,
	  .fini = api_cq_teardown,
	  .disabled = false);


void api_cq_wait1(struct fid_cq *cq0, uint64_t cq_bind_flags)
{
	int ret;
	struct fi_cq_tagged_entry cqe;

	if (!cq_bind_flags)
		return;

	while ((ret = fi_cq_read(msg_cq[0], &cqe, 1)) == -FI_EAGAIN);
	cr_assert(ret > 0, "ret=%d", ret);
}

void api_cq_wait2(struct fid_cq *cq0, struct fid_cq *cq1,
		  uint64_t check_send, uint64_t check_rcv)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;

	if (!check_send)
		source_done = 1;

	if (!check_rcv)
		dest_done = 1;

	do {
		ret = fi_cq_read(cq0, &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}

		ret = fi_cq_read(cq1, &d_cqe, 1);
		if (ret == 1) {
			dest_done = 1;
		}
	} while (!(source_done && dest_done));
}

void api_cq_send_recv(int len)
{
	ssize_t sz;
	struct iovec iov;
	struct fi_msg_rma rma_msg;
	struct fi_rma_iov rma_iov;

	iov.iov_base = NULL;
	iov.iov_len = 0;

	api_cq_init_data(source, len, 0xab);
	api_cq_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_send");

	sz = fi_recv(ep[1], target, len, rem_mr[1], gni_addr[0], source);
	api_cq_recv_allowed(sz, cq_bind_flags, "fi_recv");

	/* don't expect a recv cq if we can't send and vice versa */
	api_cq_wait2(msg_cq[0], msg_cq[1],
		     (cq_bind_flags & FI_SEND) && (cq_bind_flags & FI_RECV),
		     (cq_bind_flags & FI_SEND) && (cq_bind_flags & FI_RECV));

	sz = fi_write(ep[0], source, len,
			  loc_mr[0], gni_addr[1],
			  _REM_ADDR(fi[0], target, target), mr_key[1],
			  target);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_write");

	api_cq_wait1(msg_cq[0], cq_bind_flags & FI_SEND);

	sz = fi_writev(ep[0], &iov, (void **)loc_mr, 1,
			   gni_addr[1],
			   _REM_ADDR(fi[0], target, target), mr_key[1],
			   target);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_writev");

	api_cq_wait1(msg_cq[0], cq_bind_flags & FI_SEND);

	iov.iov_len = len;
	iov.iov_base = source;

	rma_iov.addr = _REM_ADDR(fi[0], target, target);
	rma_iov.len = len;
	rma_iov.key = mr_key[1];
	rma_msg.msg_iov = &iov;
	rma_msg.desc = (void **)loc_mr;
	rma_msg.iov_count = 1;
	rma_msg.addr = gni_addr[1];
	rma_msg.rma_iov = &rma_iov;
	rma_msg.rma_iov_count = 1;
	rma_msg.context = target;
	rma_msg.data = (uint64_t)target;

	sz = fi_writemsg(ep[0], &rma_msg, 0);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_writemsg");

	api_cq_wait1(msg_cq[0], cq_bind_flags & FI_SEND);

#define WRITE_DATA 0x5123da1a145
	sz = fi_writedata(ep[0], source, len, loc_mr[0], WRITE_DATA,
			  gni_addr[1],
			  _REM_ADDR(fi[0], target, target), mr_key[1],
			  target);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_writedata");

#define READ_CTX 0x4e3dda1aULL
	sz = fi_read(ep[0], source, len,
			 loc_mr[0], gni_addr[1],
			 _REM_ADDR(fi[0], target, target), mr_key[1],
			 (void *)READ_CTX);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_read");

	sz = fi_readv(ep[0], &iov, (void **)loc_mr, 1,
			  gni_addr[1],
			  _REM_ADDR(fi[0], target, target), mr_key[1],
			  target);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_readv");

	sz = fi_readmsg(ep[0], &rma_msg, 0);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_readmsg");

	sz = fi_inject_write(ep[0], source, 64,
				 gni_addr[1],
				 _REM_ADDR(fi[0], target, target), mr_key[1]);
	cr_assert_eq(sz, 0);

	api_cq_wait1(msg_cq[0], cq_bind_flags & FI_SEND);
}

static inline void __msg(void)
{
	cq_bind_flags = FI_SEND | FI_RECV;
	api_cq_bind(cq_bind_flags);
	api_cq_send_recv(BUF_SZ);
}

Test(api_cq_basic, msg)
{
	__msg();
}

Test(api_cq_scalable, msg)
{
	__msg();
}

static inline void __msg_send_only(void)
{
	cq_bind_flags = FI_SEND;
	api_cq_bind(cq_bind_flags);
	api_cq_send_recv(BUF_SZ);
}

Test(api_cq_basic, msg_send_only)
{
	__msg_send_only();
}

Test(api_cq_scalable, msg_send_only)
{
	__msg_send_only();
}

static inline void __msg_recv_only(void)
{
	cq_bind_flags = FI_RECV;
	api_cq_bind(cq_bind_flags);
	api_cq_send_recv(BUF_SZ);
}

Test(api_cq_basic, msg_recv_only)
{
	__msg_recv_only();
}

Test(api_cq_scalable, msg_recv_only)
{
	__msg_recv_only();
}

static inline void __msg_no_cq(void)
{
	cq_bind_flags = 0;
	api_cq_bind(cq_bind_flags);
	api_cq_send_recv(BUF_SZ);
}

Test(api_cq_basic, msg_no_cq)
{
	__msg_no_cq();
}

Test(api_cq_scalable, msg_no_cq)
{
	__msg_no_cq();
}

#define SOURCE_DATA	0xBBBB0000CCCCULL
#define TARGET_DATA	0xAAAA0000DDDDULL
#define FETCH_SOURCE_DATA	0xACEDACEDULL

void api_cq_atomic(void)
{
	ssize_t sz;

	/* u64 */
	*((uint64_t *)source) = SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_atomic(ep[0], source, 1,
			   loc_mr[0], gni_addr[1],
			   _REM_ADDR(fi[0], target, target), mr_key[1],
			   FI_UINT64, FI_ATOMIC_WRITE, target);
	api_cq_send_allowed(sz, cq_bind_flags, "fi_atomic");

	sz = fi_inject_atomic(ep[0], source, 1,
				  gni_addr[1],
				  _REM_ADDR(fi[0], target, target), mr_key[1],
				  FI_INT64, FI_MIN);
	cr_assert_eq(sz, 0);

	api_cq_wait1(msg_cq[0], cq_bind_flags & FI_SEND);
}

static inline void __atomic(void)
{
	cq_bind_flags = FI_SEND | FI_RECV;
	api_cq_bind(cq_bind_flags);
	api_cq_atomic();
}

Test(api_cq_basic, atomic)
{
	__atomic();
}

Test(api_cq_scalable, atomic)
{
	__atomic();
}

static inline void __atomic_send_only(void)
{
	cq_bind_flags = FI_SEND;
	api_cq_bind(cq_bind_flags);
	api_cq_atomic();
}

Test(api_cq_basic, atomic_send_only)
{
	__atomic_send_only();
}

Test(api_cq_scalable, atomic_send_only)
{
	__atomic_send_only();
}

static inline void __atomic_recv_only(void)
{
	cq_bind_flags = FI_RECV;
	api_cq_bind(cq_bind_flags);
	api_cq_atomic();
}

Test(api_cq_basic, atomic_recv_only)
{
	__atomic_recv_only();
}

Test(api_cq_scalable, atomic_recv_only)
{
	__atomic_recv_only();
}
