/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2019-2020 Triad National Security, LLC.
 *                         All rights reserved.
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
#include <inttypes.h>

#include "gnix_vc.h"
#include "gnix_cm_nic.h"
#include "gnix_hashtable.h"
#include "gnix_rma.h"
#include "gnix_mr.h"
#include "common.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

#if 1
#define dbg_printf(...)
#else
#define dbg_printf(...) fprintf(stderr, __VA_ARGS__); fflush(stderr)
#endif

/*
 * The multirecv tests fail when NUMEPS are > 2 (GitHub issue #1116).
 * Increase this number when the issues is fixed.
 */
#define NUMEPS 4
#define NUM_MULTIRECVS 5

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
static struct fi_info *hints;

#define BUF_SZ (1<<20)
#define BUF_RNDZV (1<<14)
#define IOV_CNT (1<<3)

static char *target, *target_base;
static char *target2, *target2_base;
static char *source, *source_base;
static char *source2, *source2_base;
struct fid_mr *rem_mr[NUMEPS], *loc_mr[NUMEPS];

static struct fid_cntr *send_cntr[NUMEPS], *recv_cntr[NUMEPS];
static struct fi_cntr_attr cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.flags = 0
};
static uint64_t sends[NUMEPS] = {0}, recvs[NUMEPS] = {0},
	send_errs[NUMEPS] = {0}, recv_errs[NUMEPS] = {0};

static void setup_common_eps(void)
{
	int ret = 0, i = 0, j = 0;
	struct fi_av_attr attr;
	size_t addrlen = 0;

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = NUMEPS;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ * NUM_MULTIRECVS));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	target2_base = malloc(GNIT_ALIGN_LEN(BUF_SZ * NUM_MULTIRECVS));
	assert(target2_base);
	target2 = GNIT_ALIGN_BUFFER(char *, target2_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	source2_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source2_base);
	source2 = GNIT_ALIGN_BUFFER(char *, source2_base);

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

		ret = fi_ep_bind(ep[i], &msg_cq[i]->fid, FI_SEND | FI_RECV);
		cr_assert(!ret, "fi_ep_bind");

		ret = fi_getname(&ep[i]->fid, NULL, &addrlen);
		cr_assert(addrlen > 0);

		ep_name[i] = malloc(addrlen);
		cr_assert(ep_name[i] != NULL);

		ret = fi_getname(&ep[i]->fid, ep_name[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);
	}

	for (i = 0; i < NUMEPS; i++) {

		for (j = 0; j < NUMEPS; j++) {
			ret = fi_av_insert(av[i], ep_name[j], 1,
					   &gni_addr[j],
					   0, NULL);
			cr_assert(ret == 1);
		}

		ret = fi_ep_bind(ep[i], &av[i]->fid, 0);
		cr_assert(!ret, "fi_ep_bind");

		ret = fi_cntr_open(dom[i], &cntr_attr, send_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open");

		ret = fi_ep_bind(ep[i], &send_cntr[i]->fid, FI_SEND);
		cr_assert(!ret, "fi_ep_bind");

		ret = fi_cntr_open(dom[i], &cntr_attr, recv_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open");

		ret = fi_ep_bind(ep[i], &recv_cntr[i]->fid, FI_RECV);
		cr_assert(!ret, "fi_ep_bind");

		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_ep_enable");

	}
}

static void setup_common(void)
{
	int ret = 0, i = 0, j = 0;
	int req_key[4];

	setup_common_eps();

	for (i = 0; i < NUMEPS; i++) {
		for (j = 0; j < 4; j++)
			req_key[j] = (USING_SCALABLE(fi[i])) ? (i * 4) + j : 0;

		ret = fi_mr_reg(dom[i],
				  target,
				  NUM_MULTIRECVS * BUF_SZ,
				  FI_REMOTE_WRITE,
				  0,
				  req_key[0],
				  0,
				  rem_mr + i,
				  &target);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i],
				  source,
				  BUF_SZ,
				  FI_REMOTE_WRITE,
				  0,
				  req_key[1],
				  0,
				  loc_mr + i,
				  &source);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi[i])) {
			MR_ENABLE(rem_mr[i],
				  target,
				  NUM_MULTIRECVS * BUF_SZ);
			MR_ENABLE(loc_mr[i],
				  source,
				  BUF_SZ);
		}

	}
}

void rdm_multi_r_setup(void)
{
	int ret = 0, i = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->mode = mode_bits;
	hints->caps = FI_SOURCE | FI_MSG;
	hints->fabric_attr->prov_name = strdup("gni");

	/* Get info about fabric services with the provided hints */
	for (; i < NUMEPS; i++) {
		ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	setup_common();
}

void rdm_multi_r_setup_nr(void)
{
	int ret = 0, i = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->mode = mode_bits;
	hints->caps = FI_SOURCE | FI_MSG;
	hints->fabric_attr->prov_name = strdup("gni");

	/* Get info about fabric services with the provided hints */
	for (; i < NUMEPS; i++) {
		ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	setup_common_eps();

	for (i = 0; i < NUMEPS; i++) {
		rem_mr[i] = NULL;
		loc_mr[i] = NULL;
	}
}


static void rdm_multi_r_teardown(void)
{
	int ret = 0, i = 0;

	for (; i < NUMEPS; i++) {
		fi_close(&recv_cntr[i]->fid);
		fi_close(&send_cntr[i]->fid);

		if (loc_mr[i] != NULL)
			fi_close(&loc_mr[i]->fid);
		if (rem_mr[i] != NULL)
			fi_close(&rem_mr[i]->fid);

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
	}

	free(target_base);
	free(source_base);

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(hints);
}

static void init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++) {
		buf[i] = seed++;
	}
}

static inline int check_data(char *buf1, char *buf2, int len)
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

static void xfer_for_each_size(void (*xfer)(int len), int slen, int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2) {
		xfer(i);
	}
}

static inline void check_cqe(struct fi_cq_tagged_entry *cqe, void *ctx,
				    uint64_t flags, void *addr, size_t len,
				    uint64_t data, bool buf_is_non_null,
				    struct fid_ep *fid_ep)
{
	struct gnix_fid_ep *gnix_ep = get_gnix_ep(fid_ep);

	cr_assert(cqe->op_context == ctx, "CQE Context mismatch");
	cr_assert(cqe->flags == flags, "CQE flags mismatch");

	if (flags & FI_RECV) {
		cr_assert(cqe->len == len, "CQE length mismatch");

		if (buf_is_non_null)
			cr_assert(cqe->buf == addr, "CQE address mismatch");
		else
			cr_assert(cqe->buf == NULL, "CQE address mismatch");

	/* TODO: Remove GNIX_ALLOW_FI_REMOTE_CQ_DATA and only check flags for FI_RMA_EVENT */
	if (GNIX_ALLOW_FI_REMOTE_CQ_DATA(flags, gnix_ep->caps))
			cr_assert(cqe->data == data, "CQE data mismatch");
	} else {
		cr_assert(cqe->len == 0, "Invalid CQE length");
		cr_assert(cqe->buf == 0, "Invalid CQE address");
		cr_assert(cqe->data == 0, "Invalid CQE data");
	}

	cr_assert(cqe->tag == 0, "Invalid CQE tag");
}

static inline void check_cntrs(uint64_t s[], uint64_t r[],
			       uint64_t s_e[], uint64_t r_e[],
			       bool need_to_spin)
{
	int i = 0;
	for (; i < NUMEPS; i++) {
		sends[i] += s[i];
		recvs[i] += r[i];
		send_errs[i] += s_e[i];
		recv_errs[i] += r_e[i];

		if (need_to_spin) {
			while (fi_cntr_read(send_cntr[i]) != sends[i]) {
				pthread_yield();
			}
		}

		cr_assert(fi_cntr_read(send_cntr[i]) == sends[i],
			  "Bad send count");

		if (need_to_spin) {
			while (fi_cntr_read(recv_cntr[i]) != recvs[i]) {
				pthread_yield();
			}
		}

		cr_assert(fi_cntr_read(recv_cntr[i]) == recvs[i],
			  "Bad recv count");

		if (need_to_spin) {
			while (fi_cntr_readerr(send_cntr[i]) != send_errs[i]) {
				pthread_yield();
			}
		}

		cr_assert(fi_cntr_readerr(send_cntr[i]) == send_errs[i],
			  "Bad send err count");

		if (need_to_spin) {
			while (fi_cntr_readerr(recv_cntr[i]) != recv_errs[i]) {
				pthread_yield();
			}
		}
		cr_assert(fi_cntr_readerr(recv_cntr[i]) == recv_errs[i],
			  "Bad recv err count");
	}
}

static void inject_enable(void)
{
	int ret, err_count_val = 1, i = 0;

	for (; i < NUMEPS; i++) {
		ret = gni_domain_ops[i]->set_val(&dom[i]->fid,
						 GNI_ERR_INJECT_COUNT,
						 &err_count_val);
		cr_assert(!ret, "setval(GNI_ERR_INJECT_COUNT)");
	}
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

TestSuite(rdm_multi_r,
	  .init = rdm_multi_r_setup,
	  .fini = rdm_multi_r_teardown,
	  .disabled = false);

TestSuite(rdm_multi_r_nr,
	  .init = rdm_multi_r_setup_nr,
	  .fini = rdm_multi_r_teardown,
	  .disabled = false);

void do_multirecv(int len)
{
	int i, j, ret;
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	struct iovec iov;
	struct fi_msg msg = {0};
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	uint64_t flags;
	uint64_t min_multi_recv;
	size_t optlen;
	const int nrecvs = NUM_MULTIRECVS;
	const int dest_ep = NUMEPS-1;
	uint64_t *expected_addrs;
	bool *addr_recvd, found, got_fi_multi_cqe = false;
	int sends_done = 0;

	dbg_printf("do_multirecv_trunc_last() called with len = %d\n", len);

	init_data(source, len, 0xab);
	init_data(target, len, 0);

	ret = fi_getopt(&ep[dest_ep]->fid, FI_OPT_ENDPOINT,
			FI_OPT_MIN_MULTI_RECV,
			(void *)&min_multi_recv, &optlen);
	cr_assert(ret == FI_SUCCESS, "fi_getopt");

	/* Post receives first to force matching in SMSG callback. */
	iov.iov_base = target;
	iov.iov_len = len * nrecvs + (min_multi_recv-1);

	msg.msg_iov = &iov;
	msg.desc = (void **)rem_mr;
	msg.iov_count = 1;
	msg.addr = FI_ADDR_UNSPEC;
	msg.context = source;
	msg.data = (uint64_t)source;

	addr_recvd = calloc(nrecvs, sizeof(bool));
	cr_assert(addr_recvd);

	expected_addrs = calloc(nrecvs, sizeof(uint64_t));
	cr_assert(expected_addrs);

	for (i = 0; i < nrecvs; i++) {
		expected_addrs[i] = (uint64_t)target +
				(uint64_t) (i * len);
	}

	sz = fi_recvmsg(ep[dest_ep], &msg, FI_MULTI_RECV);
	cr_assert_eq(sz, 0);

	for (i = nrecvs-1; i >= 0; i--) {
		int iep = i%(NUMEPS-1);

		sz = fi_send(ep[iep], source, len,
			     loc_mr[iep], gni_addr[dest_ep], target);
		cr_assert_eq(sz, 0);
	}

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		for (i = 0; i < nrecvs; i++) {
			int iep = i%(NUMEPS-1);

			/* reset cqe */
			s_cqe.op_context = s_cqe.buf = (void *) -1;
			s_cqe.flags = s_cqe.len = UINT_MAX;
			s_cqe.data = s_cqe.tag = UINT_MAX;
			d_cqe.op_context = d_cqe.buf = (void *) -1;
			d_cqe.flags = d_cqe.len = UINT_MAX;
			d_cqe.data = d_cqe.tag = UINT_MAX;

			ret = fi_cq_read(msg_cq[iep], &s_cqe, 1);
			if (ret == 1) {
				check_cqe(&s_cqe, target,
					 (FI_MSG|FI_SEND),
					 0, 0, 0, false, ep[iep]);
				s[iep]++;
				sends_done++;
			}
		}

		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		if (ret == 1) {
			for (j = 0, found = false; j < nrecvs; j++) {
				if (expected_addrs[j] == (uint64_t)d_cqe.buf) {
					cr_assert(addr_recvd[j] == false,
						  "address already received");
					addr_recvd[j] = true;
					found = true;
					break;
				}
			}
			cr_assert(found == true, "Address not found");
			flags = FI_MSG | FI_RECV;
			check_cqe(&d_cqe, source,
				 flags,
				 (void *) expected_addrs[j],
				 len, 0, true, ep[dest_ep]);
			cr_assert(check_data(source, d_cqe.buf, len),
				  "Data mismatch");
			r[dest_ep]++;
		}
	} while (sends_done < nrecvs || r[dest_ep] < nrecvs);

	/*
	 * now check for final FI_MULTI_RECV CQE on dest CQ
	 */

	do {
		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		if (d_cqe.flags & FI_MULTI_RECV) {
			got_fi_multi_cqe = true;
			r[dest_ep]++;
		}
	} while (got_fi_multi_cqe == false);

	check_cntrs(s, r, s_e, r_e, false);

	free(addr_recvd);
	free(expected_addrs);

	dbg_printf("got context events!\n");
}

Test(rdm_multi_r, multirecv, .disabled = false)
{
	xfer_for_each_size(do_multirecv, 1, BUF_SZ);
}

Test(rdm_multi_r, multirecv_retrans, .disabled = false)
{
	inject_enable();
	xfer_for_each_size(do_multirecv, 1, BUF_SZ);
}

Test(rdm_multi_r_nr, multirecv, .disabled = false)
{
	xfer_for_each_size(do_multirecv, 1, BUF_SZ);
}

Test(rdm_multi_r_nr, multirecv_retrans, .disabled = false)
{
	inject_enable();
	xfer_for_each_size(do_multirecv, 1, BUF_SZ);
}

void do_multirecv_send_first(int len)
{
	int i, j, ret;
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	struct iovec iov;
	struct fi_msg msg;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	uint64_t flags;
	uint64_t min_multi_recv;
	size_t optlen;
	const int nrecvs = NUM_MULTIRECVS;
	const int dest_ep = NUMEPS-1;
	uint64_t *expected_addrs;
	bool *addr_recvd, found;
	int sends_done = 0;
	bool got_fi_multi_cqe = false;

	init_data(source, len, 0xab);
	init_data(target, len, 0);

	dbg_printf("do_multirecv_send_first() called with len = %d\n", len);

	ret = fi_getopt(&ep[NUMEPS-1]->fid, FI_OPT_ENDPOINT,
			FI_OPT_MIN_MULTI_RECV,
			(void *)&min_multi_recv, &optlen);
	cr_assert(ret == FI_SUCCESS, "fi_getopt");

	addr_recvd = calloc(nrecvs, sizeof(bool));
	cr_assert(addr_recvd);

	expected_addrs = calloc(nrecvs, sizeof(uint64_t));
	cr_assert(expected_addrs);

	for (i = 0; i < nrecvs; i++) {
		expected_addrs[i] = (uint64_t)target +
				(uint64_t) (i * len);
	}

	/* Post sends first to force matching in the _gnix_recv() path. */
	for (i = nrecvs-1; i >= 0; i--) {
		sz = fi_send(ep[i%(NUMEPS-1)], source, len,
			     loc_mr[i%(NUMEPS-1)], gni_addr[dest_ep], target);
		cr_assert_eq(sz, 0);
	}

	/* Progress our sends. */
	for (j = 0; j < 10000; j++) {
		for (i = 0; i < nrecvs; i++) {
			int iep = i%(NUMEPS-1);

			/* reset cqe */
			s_cqe.op_context = s_cqe.buf = (void *) -1;
			s_cqe.flags = s_cqe.len = UINT_MAX;
			s_cqe.data = s_cqe.tag = UINT_MAX;
			d_cqe.op_context = d_cqe.buf = (void *) -1;
			d_cqe.flags = d_cqe.len = UINT_MAX;
			d_cqe.data = d_cqe.tag = UINT_MAX;

			ret = fi_cq_read(msg_cq[iep], &s_cqe, 1);
			if (ret == 1) {
				check_cqe(&s_cqe, target,
					 (FI_MSG|FI_SEND),
					 0, 0, 0, false, ep[iep]);
				s[iep]++;
				sends_done++;
			}

		}
		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		cr_assert_eq(ret, -FI_EAGAIN);
	}

	iov.iov_base = target;
	iov.iov_len = len * nrecvs + (min_multi_recv-1);

	msg.msg_iov = &iov;
	msg.desc = (void **)rem_mr;
	msg.iov_count = 1;
	msg.addr = FI_ADDR_UNSPEC;
	msg.context = source;
	msg.data = (uint64_t)source;

	sz = fi_recvmsg(ep[dest_ep], &msg, FI_MULTI_RECV);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		for (i = 0; i < nrecvs; i++) {
			int iep = i%(NUMEPS-1);

			/* reset cqe */
			s_cqe.op_context = s_cqe.buf = (void *) -1;
			s_cqe.flags = s_cqe.len = UINT_MAX;
			s_cqe.data = s_cqe.tag = UINT_MAX;
			d_cqe.op_context = d_cqe.buf = (void *) -1;
			d_cqe.flags = d_cqe.len = UINT_MAX;
			d_cqe.data = d_cqe.tag = UINT_MAX;

			ret = fi_cq_read(msg_cq[iep], &s_cqe, 1);
			if (ret == 1) {
				check_cqe(&s_cqe, target,
					 (FI_MSG|FI_SEND),
					 0, 0, 0, false, ep[iep]);
				s[iep]++;
				sends_done++;
			}
		}

		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		if (ret == 1) {
			for (j = 0, found = false; j < nrecvs; j++) {
				if (expected_addrs[j] == (uint64_t)d_cqe.buf) {
					cr_assert(addr_recvd[j] == false,
						  "address already received");
					addr_recvd[j] = true;
					found = true;
					break;
				}
			}
			cr_assert(found == true, "Address not found");
			flags = FI_MSG | FI_RECV;
			check_cqe(&d_cqe, source,
				 flags,
				 (void *)expected_addrs[j],
				 len, 0, true, ep[dest_ep]);
			cr_assert(check_data(source, d_cqe.buf, len),
				  "Data mismatch");
			r[dest_ep]++;
		}
	} while (sends_done < nrecvs || r[dest_ep] < nrecvs);

	/*
	 * now check for final FI_MULTI_RECV CQE on dest CQ
	 */

	do {
		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		if (d_cqe.flags & FI_MULTI_RECV) {
			got_fi_multi_cqe = true;
			r[dest_ep]++;
		}
	} while (got_fi_multi_cqe == false);

	check_cntrs(s, r, s_e, r_e, false);

	free(addr_recvd);
	free(expected_addrs);

	dbg_printf("got context events!\n");
}

Test(rdm_multi_r, multirecv_send_first, .disabled = false)
{
	xfer_for_each_size(do_multirecv_send_first, 1, BUF_SZ);
}

Test(rdm_multi_r, multirecv_send_first_retrans, .disabled = false)
{
	inject_enable();
	xfer_for_each_size(do_multirecv_send_first, 1, BUF_SZ);
}

void do_multirecv_trunc_last(int len)
{
	int i, j, ret;
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	struct fi_cq_err_entry err_cqe = {0};
	struct iovec iov;
	struct fi_msg msg = {0};
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	uint64_t flags;
	uint64_t min_multi_recv = len-1;
	const int nrecvs = 2; /* first one will fit, second will overflow */
	const int dest_ep = NUMEPS-1;
	uint64_t *expected_addrs;
	bool *addr_recvd, found;

	init_data(source, len, 0xab);
	init_data(target, len, 0);

	/* set min multirecv length */
	ret = fi_setopt(&ep[dest_ep]->fid, FI_OPT_ENDPOINT,
			FI_OPT_MIN_MULTI_RECV,
			(void *)&min_multi_recv, sizeof(size_t));
	cr_assert(ret == FI_SUCCESS, "fi_setopt");

	iov.iov_base = target;
	iov.iov_len = len + min_multi_recv;

	msg.msg_iov = &iov;
	msg.desc = (void **)rem_mr;
	msg.iov_count = 1;
	msg.addr = FI_ADDR_UNSPEC;
	msg.context = source;
	msg.data = (uint64_t)source;

	addr_recvd = calloc(nrecvs, sizeof(bool));
	cr_assert(addr_recvd);

	expected_addrs = calloc(nrecvs, sizeof(uint64_t));
	cr_assert(expected_addrs);

	for (i = 0; i < nrecvs; i++) {
		expected_addrs[i] = (uint64_t)target +
				(uint64_t) (i * len);
	}

	sz = fi_recvmsg(ep[dest_ep], &msg, FI_MULTI_RECV);
	cr_assert_eq(sz, 0);

	/* Send first one... */
	sz = fi_send(ep[0], source, len, loc_mr[0],
		     gni_addr[dest_ep], target);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = UINT_MAX;
		s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = UINT_MAX;
		d_cqe.data = d_cqe.tag = UINT_MAX;

		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			check_cqe(&s_cqe, target,
				 (FI_MSG|FI_SEND),
				 0, 0, 0, false, ep[0]);
			s[0]++;
		}

		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		if (ret == 1) {
			for (j = 0, found = false; j < nrecvs; j++) {
				if (expected_addrs[j] == (uint64_t)d_cqe.buf) {
					cr_assert(addr_recvd[j] == false,
						  "address already received");
					addr_recvd[j] = true;
					found = true;
					break;
				}
			}
			cr_assert(found == true, "Address not found");
			flags = FI_MSG | FI_RECV; fflush(stdout);
			check_cqe(&d_cqe, source,
				 flags,
				 (void *) expected_addrs[j],
				 len, 0, true, ep[dest_ep]);
			cr_assert(check_data(source, d_cqe.buf, len),
				  "Data mismatch");
			r[dest_ep]++;
		}
	} while (s[0] != 1 || r[dest_ep] != 1);

	/* ...second one will overflow */
	sz = fi_send(ep[0], source, min_multi_recv+1, loc_mr[0],
		     gni_addr[dest_ep], target);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = UINT_MAX;
		s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = UINT_MAX;
		d_cqe.data = d_cqe.tag = UINT_MAX;

		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			check_cqe(&s_cqe, target,
				 (FI_MSG|FI_SEND),
				 0, 0, 0, false, ep[0]);
			s[0]++;
		}

		/* Should return -FI_EAVAIL */
		ret = fi_cq_read(msg_cq[dest_ep], &d_cqe, 1);
		if (ret == 1) {
			r[dest_ep]++;  /* we're counting the buffer release as a receive */
		}

		if (ret == -FI_EAVAIL) {
		ret = fi_cq_readerr(msg_cq[dest_ep], &err_cqe, 0);
		if (ret == 1) {
			cr_assert((uint64_t)err_cqe.op_context ==
				  (uint64_t)source,
				  "Bad error context");
			cr_assert(err_cqe.flags == (FI_MSG | FI_RECV));
			cr_assert(err_cqe.len == min_multi_recv,
				  "Bad error len");
			cr_assert(err_cqe.buf == (void *) expected_addrs[1],
				  "Bad error buf");
			cr_assert(err_cqe.olen == 1, "Bad error olen");
			cr_assert(err_cqe.err == FI_ETRUNC, "Bad error errno");
			cr_assert(err_cqe.prov_errno == FI_ETRUNC, "Bad prov errno");
			cr_assert(err_cqe.err_data == NULL,
				  "Bad error provider data");
			r_e[dest_ep]++;
		}
		}

	} while (s[0] != 2 || r_e[dest_ep] != 1 || r[dest_ep] != 2);

	check_cntrs(s, r, s_e, r_e, false);

	free(addr_recvd);
	free(expected_addrs);

	dbg_printf("got context events!\n");
}

/*
 * These two tests should be enabled when multirecv generates errors
 * for truncated message (GitHub issue #1119).  Also, the initial
 * message size of 1 below might change depending on whether 0 is a
 * valid value for FI_OPT_MIN_MULTI_RECV (Github issue #1120)
 */
Test(rdm_multi_r, multirecv_trunc_last, .disabled = false)
{
	xfer_for_each_size(do_multirecv_trunc_last, 2, BUF_SZ);
}

Test(rdm_multi_r, multirecv_trunc_last_retrans, .disabled = false)
{
	inject_enable();
	xfer_for_each_size(do_multirecv_trunc_last, 2, BUF_SZ);
}
