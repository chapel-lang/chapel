/*
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
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
static struct fi_info *fi[NUMEPS];
static struct fi_info *hints[NUMEPS];

#define BUF_SZ (1<<20)
static char *target, *target_base;
static char *source, *source_base;
static char *uc_target;
static char *uc_source;
static struct fid_mr *rem_mr[NUMEPS], *loc_mr[NUMEPS];
static uint64_t mr_key[NUMEPS];
static uint64_t cntr_bind_flags;

static struct fid_cntr *send_cntr[NUMEPS], *recv_cntr[NUMEPS];
static struct fid_cntr *write_cntr[NUMEPS], *read_cntr[NUMEPS];
static struct fi_cntr_attr cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.flags = 0
};

#define RMA_WRITE_ALLOWED(flags) \
	(flags & FI_WRITE)
#define RMA_READ_ALLOWED(flags) \
	(flags & FI_READ)
#define MSG_SEND_ALLOWED(flags) \
	(flags & FI_SEND)
#define MSG_RECV_ALLOWED(flags) \
	(flags & FI_RECV)

void api_cntr_bind(uint64_t flags)
{
	int ret, i;

	for (i = 0; i < NUMEPS; i++) {
		if (RMA_WRITE_ALLOWED(flags)) {
			ret = fi_ep_bind(ep[i], &write_cntr[i]->fid, FI_WRITE);
			cr_assert(!ret, "fi_ep_bind");
		}

		if (RMA_READ_ALLOWED(flags)) {
			ret = fi_ep_bind(ep[i], &read_cntr[i]->fid, FI_READ);
			cr_assert(!ret, "fi_ep_bind");
		}

		if (MSG_SEND_ALLOWED(flags)) {
			ret = fi_ep_bind(ep[i], &send_cntr[i]->fid, FI_SEND);
			cr_assert(!ret, "fi_ep_bind");
		}

		if (MSG_RECV_ALLOWED(flags)) {
			ret = fi_ep_bind(ep[i], &recv_cntr[i]->fid, FI_RECV);
			cr_assert(!ret, "fi_ep_bind");
		}

		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_enable");
	}
}

static inline void __api_cntr_setup(uint32_t version, int mr_mode)
{
	int ret, i, j;
	struct fi_av_attr attr = {0};
	size_t addrlen = 0;

	for (i = 0; i < NUMEPS; i++) {
		hints[i] = fi_allocinfo();
		cr_assert(hints[i], "fi_allocinfo");

		hints[i]->domain_attr->data_progress = FI_PROGRESS_AUTO;
		hints[i]->mode = mode_bits;
		hints[i]->fabric_attr->prov_name = strdup("gni");
		hints[i]->domain_attr->mr_mode = mr_mode;
	}

	/* Get info about fabric services with the provided hints */
	for (i = 0; i < NUMEPS; i++) {
		ret = fi_getinfo(version, NULL, 0, 0, hints[i],
				 &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	attr.type = FI_AV_MAP;
	attr.count = NUMEPS;

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

		ret = fi_cntr_open(dom[i], &cntr_attr, write_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open");

		ret = fi_cntr_open(dom[i], &cntr_attr, read_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open");

		ret = fi_cntr_open(dom[i], &cntr_attr, send_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open");

		ret = fi_cntr_open(dom[i], &cntr_attr, recv_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open");

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

static void api_cntr_setup_basic(void)
{
	__api_cntr_setup(fi_version(), GNIX_MR_BASIC);
}

static void api_cntr_setup_scalable(void)
{
	__api_cntr_setup(fi_version(), GNIX_MR_SCALABLE);
}

static void api_cntr_teardown_common(bool unreg)
{
	int ret = 0, i = 0;

	for (; i < NUMEPS; i++) {
		fi_close(&write_cntr[i]->fid);
		fi_close(&read_cntr[i]->fid);
		fi_close(&send_cntr[i]->fid);
		fi_close(&recv_cntr[i]->fid);

		if (unreg) {
			fi_close(&loc_mr[i]->fid);
			fi_close(&rem_mr[i]->fid);
		}

		ret = fi_close(&ep[i]->fid);
		cr_assert(!ret, "failure in closing ep.");

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

static void api_cntr_teardown(void)
{
	api_cntr_teardown_common(true);
}

void api_cntr_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = seed++;
}

int api_cntr_check_data(char *buf1, char *buf2, int len)
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

void api_cntr_write_allowed(ssize_t sz, uint64_t flags, char *fn)
{
	if (RMA_WRITE_ALLOWED(flags)) {
		cr_assert(sz == 0, "%s failed flags:0x%lx sz:%ld",
			  fn, flags, sz);
	} else {
		cr_assert(sz < 0, "%s should fail flags:0x%lx sz:%ld",
			  fn, flags, sz);
	}
}

void api_cntr_read_allowed(ssize_t sz, uint64_t flags, char *fn)
{
	if (RMA_READ_ALLOWED(cntr_bind_flags)) {
		cr_assert(sz == 0, "%s failed flags:0x%lx sz:%ld",
			  fn, flags, sz);
	} else {
		cr_assert(sz < 0, "%s should fail flags:0x%lx sz:%ld",
			  fn, flags, sz);
	}
}

void api_cntr_send_allowed(ssize_t sz, uint64_t flags, char *fn)
{
	if (MSG_SEND_ALLOWED(flags)) {
		cr_assert(sz == 0, "%s failed flags:0x%lx sz:%ld",
			  fn, flags, sz);
	} else {
		cr_assert(sz < 0, "%s should fail flags:0x%lx sz:%ld",
			  fn, flags, sz);
	}
}

void api_cntr_recv_allowed(ssize_t sz, uint64_t flags, char *fn)
{
	if (MSG_RECV_ALLOWED(cntr_bind_flags)) {
		cr_assert(sz == 0, "%s failed flags:0x%lx sz:%ld",
			  fn, flags, sz);
	} else {
		cr_assert(sz < 0, "%s should fail flags:0x%lx sz:%ld",
			  fn, flags, sz);
	}
}

TestSuite(api_cntr_basic,
	  .init = api_cntr_setup_basic,
	  .fini = api_cntr_teardown,
	  .disabled = false);

TestSuite(api_cntr_scalable,
	  .init = api_cntr_setup_scalable,
	  .fini = api_cntr_teardown,
	  .disabled = false);

void api_cntr_send_recv(int len)
{
	ssize_t sz;

	api_cntr_init_data(source, len, 0xab);
	api_cntr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	api_cntr_send_allowed(sz, cntr_bind_flags, "fi_send");

	sz = fi_recv(ep[1], target, len, rem_mr[1], gni_addr[0], source);
	api_cntr_recv_allowed(sz, cntr_bind_flags, "fi_recv");
}

void api_cntr_write_read(int len)
{
	ssize_t sz;
	struct iovec iov;
	struct fi_msg_rma rma_msg;
	struct fi_rma_iov rma_iov;

	api_cntr_init_data(source, len, 0xab);
	api_cntr_init_data(target, len, 0);

	iov.iov_base = NULL;
	iov.iov_len = 0;

	sz = fi_write(ep[0], source, len,
		      loc_mr[0], gni_addr[1], (uint64_t)target, mr_key[1],
		      target);
	api_cntr_write_allowed(sz, cntr_bind_flags, "fi_write");

	sz = fi_writev(ep[0], &iov, (void **)loc_mr, 1,
		       gni_addr[1], (uint64_t)target, mr_key[1],
		       target);
	api_cntr_write_allowed(sz, cntr_bind_flags, "fi_writev");

	iov.iov_len = len;
	iov.iov_base = source;

	rma_iov.addr = (uint64_t)target;
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
	api_cntr_write_allowed(sz, cntr_bind_flags, "fi_writemsg");

#define WRITE_DATA 0x5123da1a145
	sz = fi_writedata(ep[0], source, len, loc_mr[0], WRITE_DATA,
			  gni_addr[1], (uint64_t)target, mr_key[1],
			  target);
	api_cntr_write_allowed(sz, cntr_bind_flags, "fi_writedata");

#define READ_CTX 0x4e3dda1aULL
	sz = fi_read(ep[0], source, len,
		     loc_mr[0], gni_addr[1], (uint64_t)target, mr_key[1],
		     (void *)READ_CTX);
	api_cntr_read_allowed(sz, cntr_bind_flags, "fi_read");

	sz = fi_readv(ep[0], &iov, (void **)loc_mr, 1,
		      gni_addr[1], (uint64_t)target, mr_key[1],
		      target);
	api_cntr_read_allowed(sz, cntr_bind_flags, "fi_readv");

	sz = fi_readmsg(ep[0], &rma_msg, 0);
	api_cntr_read_allowed(sz, cntr_bind_flags, "fi_readmsg");

	sz = fi_inject_write(ep[0], source, 64,
			     gni_addr[1], (uint64_t)target, mr_key[1]);
	cr_assert_eq(sz, 0);
}

static inline void __msg(void)
{
	cntr_bind_flags = FI_SEND | FI_RECV;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_basic, msg)
{
	__msg();
}

static inline void __msg_send_only(void)
{
	cntr_bind_flags = FI_SEND;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_basic, msg_send_only)
{
	__msg_send_only();
}

static inline void __msg_recv_only(void)
{
	cntr_bind_flags = FI_RECV;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_basic, msg_recv_only)
{
	__msg_recv_only();
}

static inline void __msg_no_cntr(void)
{
	cntr_bind_flags = 0;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_basic, msg_no_cntr)
{
	__msg_no_cntr();
}


static inline void __rma(void)
{
	cntr_bind_flags = FI_WRITE | FI_READ;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_write_read(BUF_SZ);
}

Test(api_cntr_basic, rma)
{
	__rma();
}

static inline void __rma_write_only(void)
{
	cntr_bind_flags = FI_WRITE;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_write_read(BUF_SZ);
}

Test(api_cntr_basic, rma_write_only)
{
	__rma_write_only();
}

static inline void __rma_read_only(void)
{
	cntr_bind_flags = FI_READ;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_write_read(BUF_SZ);
}

Test(api_cntr_basic, rma_read_only)
{
	__rma_read_only();
}

static inline void __rma_no_cntr(void)
{
	cntr_bind_flags = 0;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_basic, rma_no_cntr)
{
	__rma_no_cntr();
}

#define SOURCE_DATA	0xBBBB0000CCCCULL
#define TARGET_DATA	0xAAAA0000DDDDULL
#define FETCH_SOURCE_DATA	0xACEDACEDULL

void api_cntr_atomic(void)
{
	ssize_t sz;

	/* u64 */
	*((uint64_t *)source) = SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_atomic(ep[0], source, 1, loc_mr[0],
		       gni_addr[1], (uint64_t)target, mr_key[1],
		       FI_UINT64, FI_ATOMIC_WRITE, target);
	api_cntr_write_allowed(sz, cntr_bind_flags, "fi_atomic");

	*((uint64_t *)source) = SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_fetch_atomic(ep[0], source, 1, loc_mr[0],
			     source, loc_mr[0],
			     gni_addr[1], (uint64_t)target, mr_key[1],
			     FI_UINT64, FI_ATOMIC_WRITE, target);
	api_cntr_read_allowed(sz, cntr_bind_flags, "fi_atomic");

	sz = fi_inject_atomic(ep[0], source, 1,
			      gni_addr[1], (uint64_t)target, mr_key[1],
			      FI_INT64, FI_MIN);
	cr_assert_eq(sz, 0);
}

Test(api_cntr_basic, atomic)
{
	cntr_bind_flags = FI_WRITE | FI_READ;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_atomic();
}

Test(api_cntr_basic, atomic_send_only)
{
	cntr_bind_flags = FI_WRITE;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_atomic();
}

Test(api_cntr_basic, atomic_recv_only)
{
	cntr_bind_flags = FI_READ;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_atomic();
}

Test(api_cntr_scalable, msg)
{
	cntr_bind_flags = FI_SEND | FI_RECV;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_scalable, msg_send_only)
{
	cntr_bind_flags = FI_SEND;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_scalable, msg_recv_only)
{
	cntr_bind_flags = FI_RECV;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_send_recv(BUF_SZ);
}

Test(api_cntr_scalable, msg_no_cntr)
{
	__msg_no_cntr();
}

Test(api_cntr_scalable, atomic)
{
	cntr_bind_flags = FI_WRITE | FI_READ;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_atomic();
}

Test(api_cntr_scalable, atomic_send_only)
{
	cntr_bind_flags = FI_WRITE;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_atomic();
}

Test(api_cntr_scalable, atomic_recv_only)
{
	cntr_bind_flags = FI_READ;
	api_cntr_bind(cntr_bind_flags);
	api_cntr_atomic();
}

Test(api_cntr_scalable, rma)
{
	__rma();
}

Test(api_cntr_scalable, rma_write_only)
{
	__rma_write_only();
}

Test(api_cntr_scalable, rma_read_only)
{
	__rma_read_only();
}

Test(api_cntr_scalable, rma_no_cntr)
{
	__rma_no_cntr();
}

