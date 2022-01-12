/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
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


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "gnix_vc.h"
#include "gnix_cm_nic.h"
#include "gnix_hashtable.h"
#include "gnix_rma.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

#if 1
#define dbg_printf(...)
#else
#define dbg_printf(...)		\
do {				\
	printf(__VA_ARGS__);	\
	fflush(stdout);		\
} while (0)
#endif

#define NUM_EPS 5
/* NUM_THREADS must be <= NUM_EPS */
#define NUM_THREADS NUM_EPS

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom[NUM_EPS];
static struct fid_ep *ep[NUM_EPS];
static struct fid_av *av[NUM_EPS];
static struct fi_info *hints;
static struct fi_info *fi;
static void *ep_name[NUM_EPS];
static size_t gni_addr[NUM_EPS][NUM_EPS];
static struct fid_cq *send_cq;
static struct fid_cq *recv_cq;
static struct fi_cq_attr cq_attr;
static struct fid_cntr *write_cntrs[NUM_EPS];
static struct fid_cntr *read_cntrs[NUM_EPS];
static struct fid_cntr *recv_cntrs[NUM_EPS];

static struct fi_cntr_attr cntr_attr = {.events = FI_CNTR_EVENTS_COMP,
					.wait_obj = FI_WAIT_UNSPEC,
					.flags = 0};

#define BUF_SZ (64*1024)
static char *target, *target_base;
static char *source, *source_base;
static struct fid_mr *rem_mr[NUM_EPS], *loc_mr[NUM_EPS];
static uint64_t mr_key[NUM_EPS];

static inline void cntr_setup_eps(const uint64_t caps,
	uint32_t version,
	int mr_mode)
{
	int i, ret;
	struct fi_av_attr attr;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = mr_mode;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	hints->caps = caps;

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = 16;

	for (i = 0; i < NUM_EPS; i++) {
		ret = fi_domain(fab, fi, &dom[i], NULL);
		cr_assert(!ret, "fi_domain");

		ret = fi_av_open(dom[i], &attr, &av[i], NULL);
		cr_assert(!ret, "fi_av_open");

		ret = fi_endpoint(dom[i], fi, &ep[i], NULL);
		cr_assert(!ret, "fi_endpoint");
	}
}

/* Only used by cntr suite for now */
static inline void cntr_setup_cqs(void)
{
	int ret;

	cq_attr.format = FI_CQ_FORMAT_CONTEXT;
	cq_attr.size = 1024;
	cq_attr.wait_obj = FI_WAIT_NONE;

	ret = fi_cq_open(dom[0], &cq_attr, &send_cq, 0);
	cr_assert(!ret, "fi_cq_open");
	ret = fi_ep_bind(ep[0], &send_cq->fid, FI_SEND);
	cr_assert(!ret, "fi_ep_bind cq");

	ret = fi_cq_open(dom[1], &cq_attr, &recv_cq, 0);
	cr_assert(!ret, "fi_cq_open");
	ret = fi_ep_bind(ep[1], &recv_cq->fid, FI_RECV);
	cr_assert(!ret, "fi_ep_bind cq");

}

static inline void cntr_setup_cntrs(uint64_t write_flags,
				    uint64_t read_flags,
				    uint64_t recv_flags)
{
	int i, ret;

	for (i = 0; i < NUM_EPS; i++) {
		if (write_flags) {
			ret = fi_cntr_open(dom[i], &cntr_attr,
					   &write_cntrs[i], 0);
			cr_assert(!ret, "fi_cntr_open");
			ret = fi_ep_bind(ep[i], &write_cntrs[i]->fid,
					 write_flags);
			cr_assert(!ret, "fi_ep_bind cntr");
		} else {
			write_cntrs[i] = NULL;
		}

		if (read_flags) {
			ret = fi_cntr_open(dom[i], &cntr_attr,
					   &read_cntrs[i], 0);
			cr_assert(!ret, "fi_cntr_open");
			ret = fi_ep_bind(ep[i], &read_cntrs[i]->fid,
					 read_flags);
			cr_assert(!ret, "fi_ep_bind cntr");
		} else {
			read_cntrs[i] = NULL;
		}

		if (recv_flags) {
			ret = fi_cntr_open(dom[i], &cntr_attr,
					   &recv_cntrs[i], 0);
			cr_assert(!ret, "fi_cntr_open");
			ret = fi_ep_bind(ep[i], &recv_cntrs[i]->fid,
					 recv_flags);
			cr_assert(!ret, "fi_ep_bind cntr");
		} else {
			recv_cntrs[i] = NULL;
		}
	}

}

static inline void cntr_setup_av(void)
{
	int i, j, ret;
	size_t addrlen = 0;

	ret = fi_getname(&ep[0]->fid, NULL, &addrlen);
	cr_assert(addrlen > 0);

	for (i = 0; i < NUM_EPS; i++) {
		ep_name[i] = malloc(addrlen);
		cr_assert(ep_name[i] != NULL);

		ret = fi_getname(&ep[i]->fid, ep_name[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);

		for (j = 0; j < NUM_EPS; j++) {
			ret = fi_av_insert(av[j], ep_name[i], 1,
					   &gni_addr[j][i], 0, NULL);
			cr_assert(ret == 1);
		}
	}

	for (i = 0; i < NUM_EPS; i++) {
		ret = fi_ep_bind(ep[i], &av[i]->fid, 0);
		cr_assert(!ret, "fi_ep_bind av");
	}
}

static inline void cntr_setup_enable_ep(void)
{
	int i, ret;

	for (i = 0; i < NUM_EPS; i++) {
		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_ep_enable");
	}
}

static inline void cntr_setup_mr(void)
{
	int i, ret;
	int source_key, target_key;

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	for (i = 0; i < NUM_EPS; i++) {
		source_key = USING_SCALABLE(fi) ? (i * 2) + 1 : 0;
		target_key = USING_SCALABLE(fi) ? (i * 2) + 2 : 0;

		ret = fi_mr_reg(dom[i], target, BUF_SZ,
				FI_REMOTE_READ | FI_REMOTE_WRITE,
				0, target_key, 0, &rem_mr[i], &target);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i], source, BUF_SZ,	FI_READ | FI_WRITE,
				0, source_key, 0, &loc_mr[i], &source);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi)) {
			MR_ENABLE(rem_mr[i], target, BUF_SZ);
			MR_ENABLE(loc_mr[i], source, BUF_SZ);
		}

		mr_key[i] = fi_mr_key(rem_mr[i]);
	}
}

static void __cntr_setup(uint32_t version, int mr_mode)
{
	cntr_setup_eps(GNIX_EP_PRIMARY_CAPS, version, mr_mode);
	cntr_setup_av();
	cntr_setup_cqs();
	cntr_setup_cntrs(FI_WRITE | FI_SEND, FI_READ, FI_RECV);
	cntr_setup_enable_ep();
	cntr_setup_mr();
}

static void cntr_setup_basic(void)
{
	__cntr_setup(fi_version(), GNIX_MR_BASIC);
}

static void cntr_setup_scalable(void)
{
	__cntr_setup(fi_version(), GNIX_MR_SCALABLE);
}

static void cntr_setup_default(void)
{
	__cntr_setup(fi_version(), GNIX_DEFAULT_MR_MODE);
}

static inline void cntr_teardown_mr(void)
{
	int i;

	for (i = 0; i < NUM_EPS; i++) {
		fi_close(&loc_mr[i]->fid);
		fi_close(&rem_mr[i]->fid);
	}

	free(target_base);
	free(source_base);
}

static inline void cntr_teardown_eps(void)
{
	int i, ret;

	for (i = 0; i < NUM_EPS; i++) {
		ret = fi_close(&ep[i]->fid);
		cr_assert(!ret, "failure in closing ep.");

	}
}

static inline void cntr_teardown_cqs(void)
{
	int ret;
	ret = fi_close(&send_cq->fid);
	cr_assert(!ret, "failure in closing send cq.");

	ret = fi_close(&recv_cq->fid);
	cr_assert(!ret, "failure in closing recv cq.");

}

static inline void cntr_teardown_cntrs(void)
{
	int i, ret;

	for (i = 0; i < NUM_EPS; i++) {
		if (write_cntrs[i]) {
			ret = fi_close(&write_cntrs[i]->fid);
			cr_assert(!ret, "failure in closing write_cntr.");
		}

		if (read_cntrs[i]) {
			ret = fi_close(&read_cntrs[i]->fid);
			cr_assert(!ret, "failure in closing read_cntr.");
		}

		if (recv_cntrs[i]) {
			ret = fi_close(&recv_cntrs[i]->fid);
			cr_assert(!ret, "failure in closing read_cntr.");
		}
	}

}

static inline void cntr_teardown_fini(void)
{
	int i, ret;

	for (i = 0; i < NUM_EPS; i++) {
		ret = fi_close(&av[i]->fid);
		cr_assert(!ret, "failure in closing av.");

		ret = fi_close(&dom[i]->fid);
		cr_assert(!ret, "failure in closing domain.");

		free(ep_name[i]);
	}

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	/* This can be moved up to set up */
	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

static void cntr_teardown(void)
{
	cntr_teardown_mr();
	cntr_teardown_eps();
	cntr_teardown_cqs();
	cntr_teardown_cntrs();
	cntr_teardown_fini();
}

static void init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = seed++;
}

static int check_data(char *buf1, char *buf2, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (buf1[i] != buf2[i]) {
			printf("data mismatch, elem: %d, exp: %x, act: %x\n",
			       i, buf1[i], buf2[i]);
			return 0;
		}
	}

	return 1;
}

static void xfer_for_each_size(void (*xfer)(int len), int slen, int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2)
		xfer(i);
}

/*******************************************************************************
 * Test RMA functions
 ******************************************************************************/
TestSuite(cntr_default, .init = cntr_setup_default, .fini = cntr_teardown,
	  .disabled = false);
TestSuite(cntr_basic, .init = cntr_setup_basic, .fini = cntr_teardown,
	  .disabled = false);
TestSuite(cntr_scalable, .init = cntr_setup_scalable, .fini = cntr_teardown,
	  .disabled = false);

static void do_write(int len)
{
	uint64_t old_w_cnt, new_w_cnt;
	uint64_t old_r_cnt, new_r_cnt;
	ssize_t sz;

	init_data(source, len, 0xab);
	init_data(target, len, 0);


	old_w_cnt = fi_cntr_read(write_cntrs[0]);
	old_r_cnt = fi_cntr_read(read_cntrs[0]);

	sz = fi_write(ep[0], source, len, loc_mr[0], gni_addr[0][1],
			  _REM_ADDR(fi, target, target), mr_key[1], target);
	cr_assert_eq(sz, 0);

	do {
		new_w_cnt = fi_cntr_read(write_cntrs[0]);
		if (new_w_cnt == (old_w_cnt + 1))
			break;
		pthread_yield();
	} while (1);

	cr_assert(check_data(source, target, len), "Data mismatch");

	new_r_cnt = fi_cntr_read(read_cntrs[0]);

	/*
	 * no fi_read called so old and new read cnts should be equal
	 */
	cr_assert(new_r_cnt == old_r_cnt);
}

Test(cntr_basic, write)
{
	xfer_for_each_size(do_write, 8, BUF_SZ);
}

Test(cntr_scalable, write)
{
	xfer_for_each_size(do_write, 8, BUF_SZ);
}

static void do_write_wait(int len)
{
	uint64_t old_w_cnt, new_w_cnt;
	uint64_t old_r_cnt, new_r_cnt;
	ssize_t sz;
	const int iters = 100;
	int i;

	init_data(source, len, 0xab);
	init_data(target, len, 0);

	old_w_cnt = fi_cntr_read(write_cntrs[0]);
	old_r_cnt = fi_cntr_read(read_cntrs[0]);

	for (i = 0; i < iters; i++) {
		sz = fi_write(ep[0], source, len, loc_mr[0],
				  gni_addr[0][1],
				  _REM_ADDR(fi, target, target), mr_key[1],
				  target);
		cr_assert_eq(sz, 0);
	}

	fi_cntr_wait(write_cntrs[0], old_w_cnt+iters, -1);
	new_w_cnt = fi_cntr_read(write_cntrs[0]);
	cr_assert(old_w_cnt + iters == new_w_cnt);

	cr_assert(check_data(source, target, len), "Data mismatch");

	new_r_cnt = fi_cntr_read(read_cntrs[0]);

	/*
	 * no fi_read called so old and new read cnts should be equal
	 */
	cr_assert(new_r_cnt == old_r_cnt);
}

Test(cntr_basic, write_wait)
{
	xfer_for_each_size(do_write_wait, 8, BUF_SZ);
}

Test(cntr_scalable, write_wait)
{
	xfer_for_each_size(do_write_wait, 8, BUF_SZ);
}

static void do_read(int len)
{
	ssize_t sz;
	uint64_t old_w_cnt, new_w_cnt;
	uint64_t old_r_cnt, new_r_cnt;

#define READ_CTX 0x4e3dda1aULL
	init_data(source, len, 0);
	init_data(target, len, 0xad);

	old_w_cnt = fi_cntr_read(write_cntrs[0]);
	old_r_cnt = fi_cntr_read(read_cntrs[0]);

	sz = fi_read(ep[0], source, len, loc_mr[0],
			gni_addr[0][1],
			_REM_ADDR(fi, target, target), mr_key[1],
			(void *)READ_CTX);
	cr_assert_eq(sz, 0);

	do {
		new_r_cnt = fi_cntr_read(read_cntrs[0]);
		if (new_r_cnt == (old_r_cnt + 1))
			break;
		pthread_yield();
	} while (1);

	cr_assert(check_data(source, target, len), "Data mismatch");

	new_w_cnt = fi_cntr_read(write_cntrs[0]);

	/*
	 * no fi_read called so old and new read cnts should be equal
	 */
	cr_assert(new_w_cnt == old_w_cnt);
}

static void do_read_wait(int len)
{
	int i, iters = 100;
	ssize_t sz;
	uint64_t old_w_cnt, new_w_cnt;
	uint64_t old_r_cnt;

#define READ_CTX 0x4e3dda1aULL
	init_data(source, len, 0);
	init_data(target, len, 0xad);

	old_w_cnt = fi_cntr_read(write_cntrs[0]);
	old_r_cnt = fi_cntr_read(read_cntrs[0]);

	for (i = 0; i < iters; i++) {
		sz = fi_read(ep[0], source, len, loc_mr[0],
				 gni_addr[0][1],
				 _REM_ADDR(fi, target, target),
				 mr_key[1], (void *)READ_CTX);
		cr_assert_eq(sz, 0);
	}

	fi_cntr_wait(read_cntrs[0], old_r_cnt + iters, -1);

	cr_assert(check_data(source, target, len), "Data mismatch");

	new_w_cnt = fi_cntr_read(write_cntrs[0]);

	/*
	 * no fi_read called so old and new read cnts should be equal
	 */
	cr_assert(new_w_cnt == old_w_cnt);
}

Test(cntr_basic, read)
{
	xfer_for_each_size(do_read, 8, BUF_SZ);
}

Test(cntr_scalable, read)
{
	xfer_for_each_size(do_read, 8, BUF_SZ);
}

Test(cntr_basic, read_wait)
{
	xfer_for_each_size(do_read_wait, 8, BUF_SZ);
}

Test(cntr_scalable, read_wait)
{
	xfer_for_each_size(do_read_wait, 8, BUF_SZ);
}

static inline void __send_recv(void)
{
	int ret, i, got_r = 0;
	struct fi_context r_context, s_context;
	struct fi_cq_entry cqe;
	uint64_t old_s_cnt, new_s_cnt;
	uint64_t old_r_cnt, new_r_cnt;
	char s_buffer[128], r_buffer[128];

	old_s_cnt = fi_cntr_read(write_cntrs[0]);
	old_r_cnt = fi_cntr_read(recv_cntrs[1]);

	for (i = 0; i < 16; i++) {
		sprintf(s_buffer, "Hello there iter=%d", i);
		memset(r_buffer, 0, 128);
		ret = fi_recv(ep[1],
			      r_buffer,
			      sizeof(r_buffer),
			      NULL,
			      gni_addr[1][0],
			      &r_context);
		cr_assert_eq(ret, FI_SUCCESS, "fi_recv");
		ret = fi_send(ep[0],
			      s_buffer,
			      strlen(s_buffer),
			      NULL,
			      gni_addr[0][1],
			      &s_context);
		cr_assert_eq(ret, FI_SUCCESS, "fi_send");

		while ((ret = fi_cq_read(send_cq, &cqe, 1)) == -FI_EAGAIN)
			pthread_yield();

		cr_assert((cqe.op_context == &r_context) ||
			(cqe.op_context == &s_context), "fi_cq_read");
		got_r = (cqe.op_context == &r_context) ? 1 : 0;

		if (got_r) {
			new_r_cnt = fi_cntr_read(recv_cntrs[1]);
			old_r_cnt++;
			cr_assert(new_r_cnt == old_r_cnt);
		} else {
			new_s_cnt = fi_cntr_read(write_cntrs[0]);
			old_s_cnt++;
			cr_assert(new_s_cnt == old_s_cnt);
		}

		while ((ret = fi_cq_read(recv_cq, &cqe, 1)) == -FI_EAGAIN)
			pthread_yield();
		if (got_r)
			cr_assert((cqe.op_context == &s_context), "fi_cq_read");
		else
			cr_assert((cqe.op_context == &r_context), "fi_cq_read");

		if (got_r) {
			new_s_cnt = fi_cntr_read(write_cntrs[0]);
			old_s_cnt++;
			cr_assert(new_s_cnt == old_s_cnt);
		} else {
			new_r_cnt = fi_cntr_read(recv_cntrs[1]);
			old_r_cnt++;
			cr_assert(new_r_cnt == old_r_cnt);
		}

		cr_assert(strcmp(s_buffer, r_buffer) == 0, "check message");

		got_r = 0;
	}

}

Test(cntr_basic, send_recv)
{
	__send_recv();
}

Test(cntr_scalable, send_recv)
{
	__send_recv();
}

/*
 * Multithreaded tests
 */

struct tinfo {
	int msg_size;
	int iters;
};

#define get_mark(i) ((char) (((i)%255)+0x31))

static ofi_atomic32_t cntr_test_next_tid;
static __thread uint32_t cntr_test_tid = ~(uint32_t) 0;
#define cntr_test_get_tid()						\
	((cntr_test_tid  == ~(uint32_t) 0) ?				\
	 ofi_atomic_inc32(&cntr_test_next_tid) :				\
	 cntr_test_tid)


static void __cntr_setup_mt(uint32_t version, int mr_mode)
{
	cr_assert(NUM_EPS >= NUM_THREADS);

	cntr_setup_eps(GNIX_EP_PRIMARY_CAPS, version, mr_mode);
	cntr_setup_av();
	cntr_setup_cntrs(FI_WRITE | FI_SEND, FI_READ, 0x0);
	cntr_setup_enable_ep();
	cntr_setup_mr();

	ofi_atomic_initialize32(&cntr_test_next_tid, 0);
}

static void cntr_setup_mt_basic(void)
{
	__cntr_setup_mt(fi_version(), GNIX_MR_BASIC);
}

static void cntr_setup_mt_scalable(void)
{
	__cntr_setup_mt(fi_version(), GNIX_MR_SCALABLE);
}

static void cntr_setup_mt_default(void)
{
	__cntr_setup_mt(fi_version(), GNIX_DEFAULT_MR_MODE);
}

static void cntr_teardown_mt(void)
{
	cntr_teardown_mr();
	cntr_teardown_eps();
	cntr_teardown_cntrs();
	cntr_teardown_fini();
}

TestSuite(cntr_mt_default,
	  .init = cntr_setup_mt_default,
	  .fini = cntr_teardown_mt,
	  .disabled = false);

TestSuite(cntr_mt_basic,
	  .init = cntr_setup_mt_basic,
	  .fini = cntr_teardown_mt,
	  .disabled = false);

TestSuite(cntr_mt_scalable,
	  .init = cntr_setup_mt_scalable,
	  .fini = cntr_teardown_mt,
	  .disabled = false);

static void *do_thread_read_wait(void *data)
{
	int i, tid, ret;
	ssize_t sz;
	struct tinfo *info = (struct tinfo *) data;
	int msg_size = info->msg_size;
	int iters = info->iters;

	tid = cntr_test_get_tid();

	dbg_printf("%d: reading\n", tid);
	for (i = 0; i < iters; i++) {
		sz = fi_read(ep[tid], &source[tid*msg_size], msg_size,
				 loc_mr[tid], gni_addr[tid][0],
				 _REM_ADDR(fi, target, &target[tid*msg_size]),
				 mr_key[0], (void *)(READ_CTX+i));
		cr_assert_eq(sz, 0);
	}

	dbg_printf("%d: waiting\n", tid);
	ret = fi_cntr_wait(read_cntrs[tid], iters, -1);
	cr_assert(ret == FI_SUCCESS);

	dbg_printf("%d: done\n", tid);
	return NULL;
}

static inline void __read_wait(void)
{
	int i, j;
	pthread_t threads[NUM_THREADS];
	const int msg_size = 128;
	struct tinfo info = { msg_size, 500 /* iters */};

	cr_assert(NUM_THREADS*msg_size <= BUF_SZ);

	memset(source, 0, NUM_THREADS*msg_size);
	for (i = 0; i < NUM_THREADS; i++) {
		memset(&target[i*msg_size], get_mark(i), msg_size);
	}

	dbg_printf("creating threads\n");
	for (i = 1; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, do_thread_read_wait, &info);
	}

	dbg_printf("joining\n");

	for (i = 1; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	/* Must wait until all threads are done, since we don't know
	 * which thread got which id */
	for (i = 1; i < NUM_THREADS; i++) {
		for (j = 0; j < msg_size; j++) {
			cr_assert(source[i*msg_size+j] == get_mark(i));
		}
	}

	dbg_printf("done\n");

}

Test(cntr_mt_basic, read_wait)
{
	__read_wait();
}

Test(cntr_mt_scalable, read_wait)
{
	__read_wait();
}

static void *do_thread_write_wait(void *data)
{
	int i, tid, ret;
	ssize_t sz;
	struct tinfo *info = (struct tinfo *) data;
	int msg_size = info->msg_size;
	int iters = info->iters;

	tid = cntr_test_get_tid();

	dbg_printf("%d: writing\n", tid);
	for (i = 0; i < iters; i++) {
		sz = fi_write(ep[tid], &source[tid*msg_size], msg_size,
				  loc_mr[tid], gni_addr[tid][0],
				  _REM_ADDR(fi, target, &target[tid*msg_size]),
				  mr_key[0], (void *)(READ_CTX+i));
		cr_assert_eq(sz, 0);
	}

	dbg_printf("%d: waiting\n", tid);
	ret = fi_cntr_wait(write_cntrs[tid], iters, -1);
	cr_assert(ret == FI_SUCCESS);

	dbg_printf("%d: done\n", tid);
	return NULL;
}

static inline void __write_wait(void)
{
	int i, j;
	pthread_t threads[NUM_THREADS];
	const int msg_size = 128;
	struct tinfo info = { msg_size, 500 /* iters */};

	cr_assert(NUM_THREADS*msg_size <= BUF_SZ);

	memset(target, 0, NUM_THREADS*msg_size);
	for (i = 0; i < NUM_THREADS; i++) {
		memset(&source[i*msg_size], get_mark(i), msg_size);
	}

	dbg_printf("creating threads\n");
	for (i = 1; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, do_thread_write_wait, &info);
	}

	dbg_printf("joining\n");

	for (i = 1; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	/* Must wait until all threads are done, since we don't know
	 * which thread got which id */
	for (i = 1; i < NUM_THREADS; i++) {
		for (j = 0; j < msg_size; j++) {
			cr_assert(target[i*msg_size+j] == get_mark(i));
		}
	}

	dbg_printf("done\n");
}

Test(cntr_mt_basic, write_wait)
{
	__write_wait();
}

Test(cntr_mt_scalable, write_wait)
{
	__write_wait();
}

void *do_add_cntr_mt(void *arg)
{
	int i = 0, ret, iters = ((int *)arg)[0];

	for (; i < iters; i++) {
		ret = fi_cntr_add(write_cntrs[0], 1);
		cr_assert(ret == FI_SUCCESS);
	}

	return NULL;
}

void *do_add_err_cntr_mt(void *arg)
{
	int i = 0, ret, iters = ((int *)arg)[0];

	for (; i < iters; i++) {
		ret = fi_cntr_adderr(write_cntrs[0], 1);
		cr_assert(ret == FI_SUCCESS);
	}

	return NULL;
}

Test(cntr_mt_default, set_add_read_cntr)
{
	int iters = 128, nthreads = 4, i, ret;
	uint64_t cntr_val;
	pthread_t threads[nthreads];
	void *pt_ret;

	ret = fi_cntr_set(write_cntrs[0], 0);
	cr_assert(ret == FI_SUCCESS);

	cntr_val = fi_cntr_read(write_cntrs[0]);
	cr_assert_eq(cntr_val, 0, "write error counter is incorrect.");

	/* Create & Spawn threads */
	for (i = 0; i < nthreads; i++) {
		cr_assert(!pthread_create(threads + i, NULL, do_add_cntr_mt,
					  (void *) &iters));
	}

	/* Wait until all threads are done */
	for (i = 0; i < nthreads; i++) {
		cr_assert(!pthread_join(threads[i], &pt_ret));
		cr_assert(!pt_ret);
	}

	cntr_val = fi_cntr_read(write_cntrs[0]);
	cr_assert_eq(cntr_val, (iters * nthreads), "write error counter "
		"is incorrect.");
}

Test(cntr_mt_default, set_add_read_err_cntr)
{
	int iters = 128, nthreads = 4, i, ret;
	uint64_t err_cntr_val;
	pthread_t threads[nthreads];
	void *pt_ret;

	ret = fi_cntr_seterr(write_cntrs[0], 0);
	cr_assert(ret == FI_SUCCESS);

	err_cntr_val = fi_cntr_readerr(write_cntrs[0]);
	cr_assert_eq(err_cntr_val, 0, "write error counter is incorrect.");

	/* Create & Spawn threads */
	for (i = 0; i < nthreads; i++) {
		cr_assert(!pthread_create(threads + i, NULL, do_add_err_cntr_mt,
					  (void *) &iters));
	}

	/* Wait until all threads are done */
	for (i = 0; i < nthreads; i++) {
		cr_assert(!pthread_join(threads[i], &pt_ret));
		cr_assert(!pt_ret);
	}

	err_cntr_val = fi_cntr_readerr(write_cntrs[0]);
	cr_assert_eq(err_cntr_val, (iters * nthreads), "write error counter "
		"is incorrect.");
}

static void *do_thread_adderr_wait(void *data)
{
	int i, ret;
	i = *((int *) data);

	dbg_printf("%d: waiting\n", i);
	ret = fi_cntr_wait(write_cntrs[i], ~0, -1);
	cr_assert(ret != FI_SUCCESS, "Bad return value from fi_cntr_wait");

	dbg_printf("%d: done\n", i);
	return NULL;
}

Test(cntr_mt_default, adderr_wait)
{
	int i, ret;
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	void *pt_ret;

	/* Each thread waits for an err cntr change on the i'th ep_write_cntr */
	dbg_printf("creating threads\n");
	for (i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		cr_assert(!pthread_create(&threads[i], NULL,
					  do_thread_adderr_wait, (void *) &thread_args[i]));
	}

	dbg_printf("Adding errors\n");
	for (i = 0; i < NUM_THREADS; i++) {
		ret = fi_cntr_adderr(write_cntrs[i], 1);
		cr_assert(ret == FI_SUCCESS, "Bad return value from "
			"fi_cntr_adderr");
	}

	for (i = 0; i < NUM_THREADS; i++) {
		cr_assert(!pthread_join(threads[i], &pt_ret));
		cr_assert(!pt_ret);
	}

	dbg_printf("done\n");
}

Test(cntr_default, adderr_wait)
{
	int ret;

	ret = fi_cntr_adderr(write_cntrs[0], 1);
	cr_assert(ret == FI_SUCCESS, "Bad return value from fi_cntr_adderr");

	ret = fi_cntr_wait(write_cntrs[0], ~0, -1);
	cr_assert(ret != FI_SUCCESS, "Bad return value from fi_cntr_wait");
}

Test(cntr_default, set_add_read_cntr)
{
	int iters = 128, ret, i = 0, init_val = 0xabcdefab;
	uint64_t cntr_val, prev_cntr_val;

	ret = fi_cntr_set(write_cntrs[0], init_val);
	cr_assert(ret == FI_SUCCESS);

	cntr_val = fi_cntr_read(write_cntrs[0]);
	cr_assert_eq(cntr_val, init_val, "write or counter is incorrect.");

	ret = fi_cntr_set(write_cntrs[0], 0);
	cr_assert(ret == FI_SUCCESS);

	for (; i < iters; i++) {
		prev_cntr_val = fi_cntr_read(write_cntrs[0]);
		ret = fi_cntr_add(write_cntrs[0], 1);
		cr_assert(ret == FI_SUCCESS);

		cntr_val = fi_cntr_read(write_cntrs[0]);
		cr_assert_eq(cntr_val, prev_cntr_val + 1, "counter is "
			"incorrect");
	}
}

Test(cntr_default, set_add_read_err_cntr)
{
	int iters = 128, ret, i = 0,  init_val = 0xabcdefab;
	uint64_t cntr_val, prev_cntr_val;

	ret = fi_cntr_seterr(write_cntrs[0], init_val);
	cr_assert(ret == FI_SUCCESS);

	cntr_val = fi_cntr_readerr(write_cntrs[0]);
	cr_assert_eq(cntr_val, init_val, "write or counter is incorrect.");

	ret = fi_cntr_seterr(write_cntrs[0], 0);
	cr_assert(ret == FI_SUCCESS);

	for (; i < iters; i++) {
		prev_cntr_val = fi_cntr_readerr(write_cntrs[0]);
		ret = fi_cntr_adderr(write_cntrs[0], 1);
		cr_assert(ret == FI_SUCCESS);

		cntr_val = fi_cntr_readerr(write_cntrs[0]);
		cr_assert_eq(cntr_val, prev_cntr_val + 1, "error counter is "
			"incorrect");
	}
}

