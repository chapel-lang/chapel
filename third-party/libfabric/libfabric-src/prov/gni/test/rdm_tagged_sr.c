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
#define dbg_printf(...) \
	do { \
		fprintf(stderr, __VA_ARGS__); \
		fflush(stderr); \
	} while(0)
#endif

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct fid_ep *ep[2];
static struct fid_av *av;
static struct fi_info *hints;
static struct fi_info *fi;
static void *ep_name[2];
static size_t gni_addr[2];
static struct fid_cq *msg_cq[2];
static struct fi_cq_attr cq_attr;

#define BUF_SZ (1<<16)
#define IOV_CNT (1<<3)

static char *target, *target_base;
static char *source, *source_base;
static struct iovec *src_iov, *dest_iov;
static char *iov_src_buf, *iov_dest_buf;
static struct fid_mr *rem_mr, *loc_mr;
static uint64_t mr_key;

static void setup_dom(enum fi_progress pm, uint32_t version, int mr_mode)
{
	int ret;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = mr_mode;
	hints->domain_attr->data_progress = pm;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

}

static void setup_ep(void)
{
	int ret;
	struct fi_av_attr attr;
	size_t addrlen = 0;

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = 16;

	ret = fi_av_open(dom, &attr, &av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_endpoint(dom, fi, &ep[0], NULL);
	cr_assert(!ret, "fi_endpoint");

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(dom, &cq_attr, &msg_cq[0], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_cq_open(dom, &cq_attr, &msg_cq[1], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep[0], &msg_cq[0]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_getname(&ep[0]->fid, NULL, &addrlen);
	cr_assert(addrlen > 0);

	ep_name[0] = malloc(addrlen);
	cr_assert(ep_name[0] != NULL);

	ret = fi_getname(&ep[0]->fid, ep_name[0], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_endpoint(dom, fi, &ep[1], NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_ep_bind(ep[1], &msg_cq[1]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ep_name[1] = malloc(addrlen);
	cr_assert(ep_name[1] != NULL);

	ret = fi_getname(&ep[1]->fid, ep_name[1], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_av_insert(av, ep_name[0], 1, &gni_addr[0], 0,
				NULL);
	cr_assert(ret == 1);

	ret = fi_av_insert(av, ep_name[1], 1, &gni_addr[1], 0,
				NULL);
	cr_assert(ret == 1);

	ret = fi_ep_bind(ep[0], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_ep_bind(ep[1], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep[0]);
	cr_assert(!ret, "fi_ep_enable");

	ret = fi_enable(ep[1]);
	cr_assert(!ret, "fi_ep_enable");
}

static void setup_mr(void)
{
	int i, ret;

	dest_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	assert(dest_iov);

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	src_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	assert(src_iov);

	for (i = 0; i < IOV_CNT; i++) {
		src_iov[i].iov_base = malloc(BUF_SZ);
		assert(src_iov[i].iov_base != NULL);

		dest_iov[i].iov_base = malloc(BUF_SZ);
		assert(dest_iov[i].iov_base != NULL);
	}

	iov_src_buf = malloc(BUF_SZ * IOV_CNT);
	assert(iov_src_buf != NULL);

	iov_dest_buf = malloc(BUF_SZ * IOV_CNT);
	assert(iov_src_buf != NULL);

	ret = fi_mr_reg(dom, target, BUF_SZ,
			FI_SEND | FI_RECV, 0, (USING_SCALABLE(fi) ? 1 : 0),
			0, &rem_mr, &target);
	cr_assert_eq(ret, 0);

	ret = fi_mr_reg(dom, source, BUF_SZ,
			FI_SEND | FI_RECV, 0, (USING_SCALABLE(fi) ? 2 : 0),
			0, &loc_mr, &source);
	cr_assert_eq(ret, 0);

	if (USING_SCALABLE(fi)) {
		MR_ENABLE(rem_mr, target, BUF_SZ);
		MR_ENABLE(loc_mr, source, BUF_SZ);
	}

	mr_key = fi_mr_key(rem_mr);
}

static void rdm_tagged_sr_basic_setup(void)
{
	/* Change this to FI_PROGRESS_AUTO when supported */
	setup_dom(FI_PROGRESS_MANUAL, fi_version(), GNIX_MR_BASIC);
	setup_ep();
	setup_mr();
}

static void rdm_tagged_sr_scalable_setup(void)
{
	/* Change this to FI_PROGRESS_AUTO when supported */
	setup_dom(FI_PROGRESS_MANUAL, fi_version(), GNIX_MR_SCALABLE);
	setup_ep();
	setup_mr();
}

static void rdm_tagged_sr_teardown(void)
{
	int i, ret = 0;

	fi_close(&loc_mr->fid);
	fi_close(&rem_mr->fid);

	free(target_base);
	free(source_base);

	for (i = 0; i < IOV_CNT; i++) {
		free(src_iov[i].iov_base);
		free(dest_iov[i].iov_base);
	}

	free(src_iov);
	free(dest_iov);
	free(iov_src_buf);
	free(iov_dest_buf);

	ret = fi_close(&ep[0]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&ep[1]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&msg_cq[0]->fid);
	cr_assert(!ret, "failure in send cq.");

	ret = fi_close(&msg_cq[1]->fid);
	cr_assert(!ret, "failure in recv cq.");

	ret = fi_close(&av->fid);
	cr_assert(!ret, "failure in closing av.");

	ret = fi_close(&dom->fid);
	cr_assert(!ret, "failure in closing domain.");

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
	free(ep_name[0]);
	free(ep_name[1]);
}

void rdm_tagged_sr_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++) {
		buf[i] = seed++;
	}
}

int rdm_tagged_sr_check_iov_data(struct iovec *iov_buf, char *buf, size_t cnt)
{
	int i, j, cum_len = 0;

	/*
	 * For these tests we assume cumulative length of the vector entries is
	 * equal to the buf size
	 */
	for (i = 0; i < cnt; i++) {
		for (j = 0; j < iov_buf[i].iov_len; j++, cum_len++) {
			if (((char *)iov_buf[i].iov_base)[j] != buf[cum_len]) {
				printf("data mismatch, iov_index: %d, elem: %d,"
				       " iov_buf: %hhx, buf: %hhx\n", i, j,
				       ((char *)iov_buf[i].iov_base)[j],
				       buf[cum_len]);
				return 0;
			}
		}
	}
	return 1;
}

int rdm_tagged_sr_check_data(char *buf1, char *buf2, int len)
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

void rdm_tagged_sr_xfer_for_each_size(void (*xfer)(int len), int slen, int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2) {
		xfer(i);
	}
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

TestSuite(rdm_tagged_sr_basic,
		.init = rdm_tagged_sr_basic_setup,
		.fini = rdm_tagged_sr_teardown,
		.disabled = false);

TestSuite(rdm_tagged_sr_scalable,
		.init = rdm_tagged_sr_scalable_setup,
		.fini = rdm_tagged_sr_teardown,
		.disabled = false);

/*
 * ssize_t fi_tsend(struct fid_ep *ep, void *buf, size_t len,
 *		void *desc, fi_addr_t dest_addr, uint64_t tag, void *context);
 *
 * ssize_t fi_trecv(struct fid_ep *ep, void * buf, size_t len,
 *		void *desc, fi_addr_t src_addr, uint64_t tag, uint64_t ignore,
 *		void *context);
 */
void do_tsend(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;

	rdm_tagged_sr_init_data(source, len, 0xab);
	rdm_tagged_sr_init_data(target, len, 0);

	sz = fi_tsend(ep[0], source, len, loc_mr, gni_addr[1], len, target);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(ep[1], target, len, rem_mr, gni_addr[0], len, 0, source);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}
		ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
		if (ret == 1) {
			dest_done = 1;
		}
	} while (!(source_done && dest_done));

	dbg_printf("got context events!\n");


	cr_assert(rdm_tagged_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_tagged_sr_basic, tsend)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsend, 1, BUF_SZ);
}

Test(rdm_tagged_sr_scalable, tsend)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsend, 1, BUF_SZ);
}

/*
ssize_t fi_tsendv(struct fid_ep *ep, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr, uint64_t tag,
		void *context);
 */
void do_tsendv(int len)
{
	int i, ret, iov_cnt;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;

	sz = fi_tsendv(ep[0], src_iov, NULL, 0, gni_addr[1],
		       len * IOV_CNT, iov_dest_buf);
	cr_assert_eq(sz, -FI_EINVAL);

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		rdm_tagged_sr_init_data(iov_dest_buf, len * iov_cnt, 0);

		for (i = 0; i < iov_cnt; i++) {
			rdm_tagged_sr_init_data(src_iov[i].iov_base, len, 0xab);
			src_iov[i].iov_len = len;
		}

		sz = fi_tsendv(ep[0], src_iov, NULL, iov_cnt, gni_addr[1],
			       len * iov_cnt, iov_dest_buf);
		cr_assert_eq(sz, 0);

		sz = fi_trecv(ep[1], iov_dest_buf, len * iov_cnt, NULL, gni_addr[0],
			      len * iov_cnt, 0, src_iov);
		cr_assert_eq(sz, 0);

		/* need to progress both CQs simultaneously for rendezvous */
		do {
			ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
			if (ret == 1) {
				source_done = 1;
			}
			ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
			if (ret == 1) {
				dest_done = 1;
			}
		} while (!(source_done && dest_done));

		dbg_printf("got recv context event!\n");

		cr_assert(rdm_tagged_sr_check_iov_data(src_iov, iov_dest_buf, iov_cnt),
			  "Data mismatch");
		source_done = dest_done = 0;
	}
}

Test(rdm_tagged_sr_basic, tsendv)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsendv, 1, BUF_SZ);
}

Test(rdm_tagged_sr_scalable, tsendv)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsendv, 1, BUF_SZ);
}

/*
ssize_t fi_tsendmsg(struct fid_ep *ep, const struct fi_msg *msg,
		uint64_t flags);
*/
void do_tsendmsg(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	struct fi_msg_tagged msg;
	struct iovec iov;

	iov.iov_base = source;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)&loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	msg.context = target;
	msg.data = (uint64_t)target;
	msg.tag = len;
	msg.ignore = 0;

	rdm_tagged_sr_init_data(source, len, 0xef);
	rdm_tagged_sr_init_data(target, len, 0);

	sz = fi_tsendmsg(ep[0], &msg, 0);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(ep[1], target, len, rem_mr, gni_addr[0], len, 0, source);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}
		ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
		if (ret == 1) {
			dest_done = 1;
		}
	} while (!(source_done && dest_done));

	dbg_printf("got context events!\n");

	cr_assert(rdm_tagged_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_tagged_sr_basic, tsendmsg)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsendmsg, 1, BUF_SZ);
}

Test(rdm_tagged_sr_scalable, tsendmsg)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsendmsg, 1, BUF_SZ);
}

/*
ssize_t fi_tinject(struct fid_ep *ep, void *buf, size_t len,
		fi_addr_t dest_addr);
*/
#define INJECT_SIZE 64
void do_tinject(int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe;

	rdm_tagged_sr_init_data(source, len, 0x23);
	rdm_tagged_sr_init_data(target, len, 0);

	sz = fi_tinject(ep[0], source, len, gni_addr[1], len);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(ep[1], target, len, rem_mr, gni_addr[0], len, 0, source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[1], &cqe, 1)) == -FI_EAGAIN) {
		ret = fi_cq_read(msg_cq[0], &cqe, 1);
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	cr_assert_eq((uint64_t)cqe.op_context, (uint64_t)source);

	dbg_printf("got recv context event!\n");

	cr_assert(rdm_tagged_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_tagged_sr_basic, tinject, .disabled = false)
{
	rdm_tagged_sr_xfer_for_each_size(do_tinject, 1, INJECT_SIZE);
}

Test(rdm_tagged_sr_scalable, tinject, .disabled = false)
{
	rdm_tagged_sr_xfer_for_each_size(do_tinject, 1, INJECT_SIZE);
}


/*
ssize_t fi_tsenddata(struct fid_ep *ep, void *buf, size_t len,
		void *desc, uint64_t data, fi_addr_t dest_addr, void *context);
*/
void do_tsenddata(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;

	rdm_tagged_sr_init_data(source, len, 0xab);
	rdm_tagged_sr_init_data(target, len, 0);

	sz = fi_tsenddata(ep[0], source, len, loc_mr, (uint64_t)source,
			 gni_addr[1], len, target);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(ep[1], target, len, rem_mr, gni_addr[0], len, 0, source);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}
		ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
		if (ret == 1) {
			dest_done = 1;
		}
	} while (!(source_done && dest_done));

	dbg_printf("got context events!\n");

	cr_assert(rdm_tagged_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_tagged_sr_basic, tsenddata)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsenddata, 1, BUF_SZ);
}

Test(rdm_tagged_sr_scalable, tsenddata)
{
	rdm_tagged_sr_xfer_for_each_size(do_tsenddata, 1, BUF_SZ);
}

/*
ssize_t (*recvv)(struct fid_ep *ep, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t src_addr, void *context);
 */
void do_trecvv(int len)
{
	int i, ret, iov_cnt;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;

	sz = fi_trecvv(ep[1], NULL, NULL, IOV_CNT, gni_addr[0],
		       len * IOV_CNT, 0, iov_src_buf);
	cr_assert_eq(sz, -FI_EINVAL);

	sz = fi_trecvv(ep[1], dest_iov, NULL, IOV_CNT + 1, gni_addr[0],
		       len * IOV_CNT, 0, iov_src_buf);
	cr_assert_eq(sz, -FI_EINVAL);

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		rdm_tagged_sr_init_data(iov_src_buf, len * iov_cnt, 0xab);

		for (i = 0; i < iov_cnt; i++) {
			rdm_tagged_sr_init_data(dest_iov[i].iov_base, len, 0);
			dest_iov[i].iov_len = len;
		}

		sz = fi_tsend(ep[0], iov_src_buf, len * iov_cnt, NULL, gni_addr[1],
			      len * iov_cnt, dest_iov);
		cr_assert_eq(sz, 0);

		sz = fi_trecvv(ep[1], dest_iov, NULL, iov_cnt, gni_addr[0],
			       len * iov_cnt, 0, iov_src_buf);
		cr_assert_eq(sz, 0);

		/* need to progress both CQs simultaneously for rendezvous */
		do {
			ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
			if (ret == 1) {
				source_done = 1;
			}
			ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
			if (ret == 1) {
				dest_done = 1;
			}
		} while (!(source_done && dest_done));

		dbg_printf("got context events!\n");

		cr_assert(rdm_tagged_sr_check_iov_data(dest_iov, iov_src_buf, iov_cnt),
			  "Data mismatch");
		source_done = dest_done = 0;
	}
}

Test(rdm_tagged_sr_basic, trecvv)
{
	rdm_tagged_sr_xfer_for_each_size(do_trecvv, 1, BUF_SZ);
}

Test(rdm_tagged_sr_scalable, trecvv)
{
	rdm_tagged_sr_xfer_for_each_size(do_trecvv, 1, BUF_SZ);
}

/*
ssize_t (*recvmsg)(struct fid_ep *ep, const struct fi_msg *msg,
		uint64_t flags);
 */
void do_trecvmsg(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	struct fi_msg_tagged msg;
	struct iovec iov;

	rdm_tagged_sr_init_data(source, len, 0xab);
	rdm_tagged_sr_init_data(target, len, 0);

	sz = fi_tsend(ep[0], source, len, loc_mr, gni_addr[1], len, target);
	cr_assert_eq(sz, 0);

	iov.iov_base = target;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)&rem_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[0];
	msg.context = source;
	msg.data = (uint64_t)source;
	msg.tag = len;
	msg.ignore = 0;

	sz = fi_trecvmsg(ep[1], &msg, 0);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}
		ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
		if (ret == 1) {
			dest_done = 1;
		}
	} while (!(source_done && dest_done));

	dbg_printf("got context events!\n");

	cr_assert(rdm_tagged_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_tagged_sr_basic, trecvmsg)
{
	rdm_tagged_sr_xfer_for_each_size(do_trecvmsg, 1, BUF_SZ);
}

Test(rdm_tagged_sr_scalable, trecvmsg)
{
	rdm_tagged_sr_xfer_for_each_size(do_trecvmsg, 1, BUF_SZ);
}

static inline void __multi_tsend_trecv(void)
{
	int i, it, ridx, ret;
	const int iters = 37;
	const int num_msgs = 17;
	const int slen = 256;
	uint64_t tags[num_msgs];
	uint64_t rtag = 0x01000000;
	uint64_t ignore = 0xf0ffffff;
	char msg[num_msgs][slen];
	struct fi_cq_tagged_entry cqe;

	srand(time(NULL));

	for (it = 0; it < iters; it++) {
		for (i = 0; i < num_msgs; i++) {
			tags[i] = 0x01010abc + it*iters + i;

			sprintf(msg[i], "%d\n", i);
			ret = fi_tsend(ep[1], msg[i], strlen(msg[i]),
				       NULL, gni_addr[0], tags[i], NULL);
			cr_assert(ret == FI_SUCCESS);

			do {
				ret = fi_cq_read(msg_cq[1], &cqe, 1);
				cr_assert((ret == 1) || (ret == -FI_EAGAIN));
			} while (ret == -FI_EAGAIN);

			cr_assert(cqe.len == 0);
			cr_assert(cqe.tag == 0);
		}

		for (i = 0; i < num_msgs; i++) {
			memset(target, 0, BUF_SZ);
			ret = fi_trecv(ep[0], target, BUF_SZ,
				       fi_mr_desc(loc_mr),
				       gni_addr[1], rtag, ignore, NULL);
			cr_assert(ret == FI_SUCCESS);

			do {
				ret = fi_cq_read(msg_cq[0], &cqe, 1);
				cr_assert((ret == 1) || (ret == -FI_EAGAIN));
			} while (ret == -FI_EAGAIN);

			cr_assert(rtag != cqe.tag);

			ret = sscanf(target, "%d", &ridx);
			cr_assert(ret == 1);
			cr_assert(cqe.len == strlen(msg[ridx]));

			/* zero out the tag for error checking below */
			tags[ridx] = 0;
		}

		/* Make sure we got everything */
		for (i = 0; i < num_msgs; i++)
			cr_assert(tags[i] == 0);
	}

}

Test(rdm_tagged_sr_basic, multi_tsend_trecv)
{
	__multi_tsend_trecv();
}

Test(rdm_tagged_sr_scalable, multi_tsend_trecv)
{
	__multi_tsend_trecv();
}

static void do_tagged_sr_pipelined(void)
{
	int i, it, s, ret;
	const int iters = 37;
	const int num_msgs = 61;
	const int msgs_per_stage = 17;
	const int num_stages = num_msgs/msgs_per_stage +
		(num_msgs%msgs_per_stage != 0);
	const int slen = 256;
	uint64_t tags[num_msgs];
	uint64_t rtag = 0x01000000;
	uint64_t ignore = 0xf0ffffff;
	char msg[num_msgs][slen];
	struct fi_cq_tagged_entry cqe;

	srand(time(NULL));

	for (it = 0; it < iters; it++) {
		dbg_printf("iter %d\n", it);
		for (s = 0; s < num_stages; s++) {
			dbg_printf("\tsending stage %d\n", s);
			for (i = s*msgs_per_stage;
			     i < (s+1)*msgs_per_stage && i < num_msgs;
			     i++) {
				tags[i] = 0x01010abc + it*iters + i;

				sprintf(msg[i], "%d\n", i%10);
				ret = fi_tsend(ep[1], msg[i], strlen(msg[i]),
					       NULL, gni_addr[0], tags[i],
					       NULL);
				cr_assert(ret == FI_SUCCESS);
			}

			for (i = s*msgs_per_stage;
			     i < (s+1)*msgs_per_stage && i < num_msgs;
			     i++) {
				do {
					ret = fi_cq_read(msg_cq[1], &cqe, 1);
					cr_assert((ret == 1) ||
						  (ret == -FI_EAGAIN));
				} while (ret == -FI_EAGAIN);

				cr_assert(cqe.tag == 0);
			}
			cr_assert(cqe.len == 0);
		}

		for (s = 0; s < num_stages; s++) {
			dbg_printf("\treceiving stage %d\n", s);
			for (i = s*msgs_per_stage;
			     i < (s+1)*msgs_per_stage && i < num_msgs;
			     i++) {
				ret = fi_trecv(ep[0], &target[i], BUF_SZ,
					       fi_mr_desc(loc_mr),
					       gni_addr[1], rtag, ignore,
					       NULL);
				cr_assert(ret == FI_SUCCESS);
			}

			for (i = s*msgs_per_stage;
			     i < (s+1)*msgs_per_stage && i < num_msgs;
			     i++) {
				do {
					ret = fi_cq_read(msg_cq[0], &cqe, 1);
					cr_assert((ret == 1) ||
						  (ret == -FI_EAGAIN));
				} while (ret == -FI_EAGAIN);

				cr_assert(rtag != cqe.tag);

				cr_assert(ret == 1);
				cr_assert(cqe.len == 2);

				/* zero out the tag for error checking below */
				tags[cqe.tag - (0x01010abc + it*iters)] = 0;
			}
		}

		/* Make sure we got everything */
		for (i = 0; i < num_msgs; i++) {
			cr_assert(tags[i] == 0);
		}
	}

}

/* Add this test when FI_PROGRESS_AUTO is implemented */
Test(rdm_tagged_sr_basic, multi_tsend_trecv_pipelined, .disabled = true) {
	do_tagged_sr_pipelined();
}

Test(rdm_tagged_sr_scalable, multi_tsend_trecv_pipelined, .disabled = true) {
	do_tagged_sr_pipelined();
}

/* Call fi_gni_domain_ops->set_val() with op and opval */
static void progress_manual_dom_ops_setup(const dom_ops_val_t op,
					  const uint32_t opval)
{
	int ret;
	uint32_t val = opval;
	struct fi_gni_ops_domain *gni_domain_ops;

	setup_dom(FI_PROGRESS_MANUAL, fi_version(), GNIX_MR_BASIC);
	ret = fi_open_ops(&dom->fid, FI_GNI_DOMAIN_OPS_1,
			  0, (void **) &gni_domain_ops, NULL);
	gni_domain_ops->set_val(&dom->fid, op, &val);
	cr_assert(ret == FI_SUCCESS, "fi_open_ops");

	setup_ep();
	setup_mr();
}

static void mbox_max_credit_setup(void)
{
	/* Use this with manual progress */
	progress_manual_dom_ops_setup(GNI_MBOX_MAX_CREDIT,
				      122-1 /* 2*num_msgs-1 above */);
}

/* Suite of tests that should work with manual progress */
TestSuite(rdm_tagged_sr_progress_manual,
	  .init = mbox_max_credit_setup,
	  .fini = rdm_tagged_sr_teardown,
	  .disabled = false);

Test(rdm_tagged_sr_progress_manual, multi_tsend_trecv_pipelined) {
	do_tagged_sr_pipelined();
}
