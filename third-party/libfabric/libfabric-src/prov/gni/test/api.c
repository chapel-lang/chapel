/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
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
#include "gnix_util.h"
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

static struct fid_cntr *send_cntr[NUMEPS], *recv_cntr[NUMEPS];
static struct fi_cntr_attr cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.flags = 0
};
static uint64_t sends[NUMEPS] = {0}, recvs[NUMEPS] = {0},
	send_errs[NUMEPS] = {0}, recv_errs[NUMEPS] = {0};

static void rdm_api_setup_ep(uint32_t version, int mr_mode)
{
	int ret, i, j;
	struct fi_av_attr attr;
	size_t addrlen = 0;

	/* Get info about fabric services with the provided hints */
	for (i = 0; i < NUMEPS; i++) {
		hints[i]->domain_attr->mr_mode = mr_mode;

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
		/* Insert all gni addresses into each av */
		for (j = 0; j < NUMEPS; j++) {
			ret = fi_av_insert(av[i], ep_name[j], 1, &gni_addr[j],
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

		ret = fi_enable(ep[i]);
		cr_assert_eq(ret, -FI_EOPBADSTATE);


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

void __rdm_api_setup(uint32_t version, int mr_mode)
{
	int i;

	SKIP_IF_SCALABLE_LT_1_5(version, mr_mode);

	for (i = 0; i < NUMEPS; i++) {
		hints[i] = fi_allocinfo();
		cr_assert(hints[i], "fi_allocinfo");

		hints[i]->domain_attr->cq_data_size = NUMEPS * 2;
		hints[i]->domain_attr->data_progress = FI_PROGRESS_AUTO;
		hints[i]->mode = mode_bits;
		hints[i]->fabric_attr->prov_name = strdup("gni");
	}
}

static void rdm_api_setup_basic(void)
{
	__rdm_api_setup(fi_version(), GNIX_MR_BASIC);
}

static void rdm_api_setup_scalable(void)
{
	__rdm_api_setup(fi_version(), GNIX_MR_SCALABLE);
}

void api_setup(void)
{
}

void api_teardown(void)
{
}

static void rdm_api_teardown_common(bool unreg)
{
	int ret = 0, i = 0;

	for (; i < NUMEPS; i++) {
		fi_close(&recv_cntr[i]->fid);
		fi_close(&send_cntr[i]->fid);

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

static void rdm_api_teardown(void)
{
	rdm_api_teardown_common(true);
}

void rdm_api_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = seed++;
}

int rdm_api_check_data(char *buf1, char *buf2, int len)
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

void rdm_api_check_cqe(struct fi_cq_tagged_entry *cqe, void *ctx,
		      uint64_t flags, void *addr, size_t len,
		      uint64_t data, struct fid_ep *fid_ep)
{
	struct gnix_fid_ep *gnix_ep = get_gnix_ep(fid_ep);

	cr_assert(cqe->op_context == ctx, "CQE Context mismatch");
	cr_assert(cqe->flags == flags, "CQE flags mismatch");

	if (flags & FI_RECV) {
		cr_assert(cqe->len == len, "CQE length mismatch");
		cr_assert(cqe->buf == addr, "CQE address mismatch");

	/* TODO: Remove GNIX_ALLOW_FI_REMOTE_CQ_DATA and only check
	 *       flags for FI_RMA_EVENT */
	if (GNIX_ALLOW_FI_REMOTE_CQ_DATA(flags, gnix_ep->caps))
			cr_assert(cqe->data == data, "CQE data mismatch");
	} else {
		cr_assert(cqe->len == 0, "Invalid CQE length");
		cr_assert(cqe->buf == 0, "Invalid CQE address");
		cr_assert(cqe->data == 0, "Invalid CQE data");
	}

	cr_assert(cqe->tag == 0, "Invalid CQE tag");
}

void rdm_api_check_cntrs(uint64_t s[], uint64_t r[], uint64_t s_e[],
			uint64_t r_e[])
{
	int i = 0;

	for (; i < NUMEPS; i++) {
		sends[i] += s[i];
		recvs[i] += r[i];
		send_errs[i] += s_e[i];
		recv_errs[i] += r_e[i];

		cr_assert(fi_cntr_read(send_cntr[i]) == sends[i],
			  "Bad send count");
		cr_assert(fi_cntr_read(recv_cntr[i]) == recvs[i],
			  "Bad recv count");
		cr_assert(fi_cntr_readerr(send_cntr[i]) == send_errs[i],
			  "Bad send err count");
		cr_assert(fi_cntr_readerr(recv_cntr[i]) == recv_errs[i],
			  "Bad recv err count");
	}
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

#define MSG_SEND_ALLOWED(caps) \
	((caps & FI_MSG) && ((caps & FI_SEND) || !(caps & FI_RECV)))
#define MSG_RECV_ALLOWED(caps) \
	((caps & FI_MSG) && ((caps & FI_RECV) || !(caps & FI_SEND)))
#define TAG_SEND_ALLOWED(caps) \
	((caps & FI_TAGGED) && ((caps & FI_SEND) || !(caps & FI_RECV)))
#define TAG_RECV_ALLOWED(caps) \
	((caps & FI_TAGGED) && ((caps & FI_RECV) || !(caps & FI_SEND)))
#define WRITE_ALLOWED(caps, rcaps)                       \
	((caps & FI_RMA) &&                              \
	 ((caps & FI_WRITE) || !(caps & FI_READ)) &&     \
	 ((rcaps & FI_RMA) || (rcaps & FI_REMOTE_WRITE)) \
	)
#define READ_ALLOWED(caps, rcaps)                                \
	((caps & FI_RMA) &&                                      \
	 ((caps & FI_READ) || !(caps & FI_WRITE)) &&             \
	 (((rcaps & FI_RMA) &&                                   \
	   !(rcaps & (FI_READ | FI_WRITE | FI_REMOTE_WRITE))) || \
	  (rcaps & FI_REMOTE_READ)                               \
	 )                                                       \
	)
static int write_allowed(uint64_t rma_amo, uint64_t caps, uint64_t rcaps)
{
	dbg_printf("write %s caps:%s, rcaps:%s\n",
		   fi_tostr(&rma_amo, FI_TYPE_CAPS),
		   fi_tostr(&caps, FI_TYPE_CAPS),
		   fi_tostr(&rcaps, FI_TYPE_CAPS));
	if ((caps & rma_amo) &&
	    ((caps & FI_WRITE) || !(caps & FI_READ))) {
		if ((rcaps & rma_amo) &&
		    ((rcaps & FI_REMOTE_WRITE) ||
		      (!(rcaps & (FI_READ | FI_WRITE | FI_REMOTE_READ)))
		    )
		   ) {
			return 1;
		}
	}
	return 0;
}

static int read_allowed(uint64_t rma_amo, uint64_t caps, uint64_t rcaps)
{
	dbg_printf("read %s caps:%s, rcaps:%s\n",
		   fi_tostr(&rma_amo, FI_TYPE_CAPS),
		   fi_tostr(&caps, FI_TYPE_CAPS),
		   fi_tostr(&rcaps, FI_TYPE_CAPS));
	if ((caps & rma_amo) &&
	    ((caps & FI_READ) || !(caps & FI_WRITE))) {
		if ((rcaps & rma_amo) &&
		    ((rcaps & FI_REMOTE_READ) ||
		     (!(rcaps & (FI_READ | FI_WRITE | FI_REMOTE_WRITE)))
		    )
		   ) {
			return 1;
		}
	}
	return 0;
}

TestSuite(rdm_api_basic,
	  .init = rdm_api_setup_basic,
	  .fini = rdm_api_teardown);

TestSuite(rdm_api_scalable,
	  .init = rdm_api_setup_scalable,
	 .fini = rdm_api_teardown);

/*
 * ssize_t fi_send(struct fid_ep *ep, void *buf, size_t len,
 *		void *desc, fi_addr_t dest_addr, void *context);
 *
 * ssize_t fi_recv(struct fid_ep *ep, void * buf, size_t len,
 *		void *desc, fi_addr_t src_addr, void *context);
 */
void api_send_recv(int len)
{
	ssize_t sz;
	uint64_t caps = fi[0]->caps;

	rdm_api_init_data(source, len, 0xab);
	rdm_api_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	if (MSG_SEND_ALLOWED(caps)) {
		cr_assert(sz == 0, "fi_send failed caps:0x%lx err:%ld",
			  caps, sz);
	} else {
		cr_assert(sz < 0, "fi_send should fail caps:0x%lx err:%ld",
			  caps, sz);
	}

	sz = fi_recv(ep[1], target, len, rem_mr[1], gni_addr[0], source);
	if (MSG_RECV_ALLOWED(caps)) {
		cr_assert(sz == 0, "fi_recv failed caps:0x%lx err:%ld",
			  caps, sz);
	} else {
		cr_assert(sz < 0, "fi_recv should fail caps:0x%lx err:%ld",
			  caps, sz);
	}
}

Test(api, dom_caps)
{
	int ret;

	hints[0] = fi_allocinfo();
	cr_assert(hints[0], "fi_allocinfo");

	hints[0]->mode = mode_bits;
	hints[0]->fabric_attr->prov_name = strdup("gni");
	hints[0]->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;

	/* we only support REMOTE_COMM */
	hints[0]->domain_attr->caps = FI_LOCAL_COMM;
	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints[0], &fi[0]);
	cr_assert_eq(ret, -FI_ENODATA, "fi_getinfo");

	hints[0]->domain_attr->caps = FI_REMOTE_COMM;
	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints[0], &fi[0]);
	cr_assert_eq(ret, 0, "fi_getinfo");

	fi_freeinfo(fi[0]);

	hints[0]->domain_attr->mr_mode = FI_MR_UNSPEC;
	ret = fi_getinfo(FI_VERSION(1, 0), NULL, 0, 0, hints[0], &fi[0]);
	cr_assert_eq(ret, 0, "fi_getinfo");

	fi_freeinfo(hints[0]);
	fi_freeinfo(fi[0]);
}

static inline void __msg_no_caps(uint32_t version, int mr_mode)
{
	hints[0]->caps = 0;
	hints[1]->caps = 0;
	rdm_api_setup_ep(version, mr_mode);
	api_send_recv(BUF_SZ);
}

Test(rdm_api_basic, msg_no_caps)
{
	__msg_no_caps(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, msg_no_caps)
{
	__msg_no_caps(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __msg_send_rcv(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_MSG;
	hints[1]->caps = FI_MSG;
	rdm_api_setup_ep(version, mr_mode);
	api_send_recv(BUF_SZ);
}

Test(rdm_api_basic, msg_send_rcv)
{
	__msg_send_rcv(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, msg_send_rcv)
{
	__msg_send_rcv(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __msg_send_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_MSG | FI_SEND;
	hints[1]->caps = FI_MSG | FI_SEND;
	rdm_api_setup_ep(version, mr_mode);
	api_send_recv(BUF_SZ);
}

Test(rdm_api_basic, msg_send_only)
{
	__msg_send_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, msg_send_only)
{
	__msg_send_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __msg_recv_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_MSG | FI_RECV;
	hints[1]->caps = FI_MSG | FI_RECV;
	rdm_api_setup_ep(version, mr_mode);
	api_send_recv(BUF_SZ);
}

Test(rdm_api_basic, msg_recv_only)
{
	__msg_recv_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, msg_recv_only)
{
	__msg_recv_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __msg_send_rcv_w_tagged(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_TAGGED;
	hints[1]->caps = FI_TAGGED;
	rdm_api_setup_ep(version, mr_mode);
	api_send_recv(BUF_SZ);
}

Test(rdm_api_basic, msg_send_rcv_w_tagged)
{
	__msg_send_rcv_w_tagged(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, msg_send_rcv_w_tagged)
{
	__msg_send_rcv_w_tagged(fi_version(), GNIX_MR_SCALABLE);
}

void api_tagged_send_recv(int len)
{
	ssize_t sz;
	uint64_t caps = fi[0]->caps;

	rdm_api_init_data(source, len, 0xab);
	rdm_api_init_data(target, len, 0);

	sz = fi_tsend(ep[0], source, len, loc_mr, gni_addr[1], len, target);
	if (TAG_SEND_ALLOWED(caps)) {
		cr_assert(sz == 0, "fi_tsend failed caps:0x%lx err:%ld",
			  caps, sz);
	} else {
		cr_assert(sz < 0, "fi_tsend should fail caps:0x%lx err:%ld",
			  caps, sz);
	}

	sz = fi_trecv(ep[1], target, len, rem_mr, gni_addr[0], len, 0, source);
	if (TAG_RECV_ALLOWED(caps)) {
		cr_assert(sz == 0, "fi_trecv failed caps:0x%lx err:%ld",
			  caps, sz);
	} else {
		cr_assert(sz < 0, "fi_trecv should fail caps:0x%lx err:%ld",
			  caps, sz);
	}
}

static inline void __tsend(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_TAGGED;
	hints[1]->caps = FI_TAGGED;
	rdm_api_setup_ep(version, mr_mode);
	api_tagged_send_recv(BUF_SZ);
}

Test(rdm_api_basic, tsend)
{
	__tsend(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, tsend)
{
	__tsend(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __tsend_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_TAGGED | FI_SEND;
	hints[1]->caps = FI_TAGGED | FI_SEND;
	rdm_api_setup_ep(version, mr_mode);
	api_tagged_send_recv(BUF_SZ);
}

Test(rdm_api_basic, tsend_only)
{
	__tsend_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, tsend_only)
{
	__tsend_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __trecv_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_TAGGED | FI_RECV;
	hints[1]->caps = FI_TAGGED | FI_RECV;
	rdm_api_setup_ep(version, mr_mode);
	api_tagged_send_recv(BUF_SZ);
}

Test(rdm_api_basic, trecv_only)
{
	__trecv_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, trecv_only)
{
	__trecv_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __tsend_rcv_w_msg(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_MSG;
	hints[1]->caps = FI_MSG;
	rdm_api_setup_ep(version, mr_mode);
	api_tagged_send_recv(BUF_SZ);
}

Test(rdm_api_basic, tsend_rcv_w_msg)
{
	__tsend_rcv_w_msg(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, tsend_rcv_w_msg)
{
	__tsend_rcv_w_msg(fi_version(), GNIX_MR_SCALABLE);
}

#define READ_CTX 0x4e3dda1aULL
void api_write_read(int len)
{
	int ret;
	struct fi_cq_tagged_entry cqe;
	struct fi_cq_err_entry err_cqe = {0};

	rdm_api_init_data(source, len, 0xab);
	rdm_api_init_data(target, len, 0);

	fi_write(ep[0], source, len,
		 loc_mr[0], gni_addr[1],
		 _REM_ADDR(fi[0], target, target), mr_key[1],
		 target);

	while ((ret = fi_cq_read(msg_cq[0], &cqe, 1)) == -FI_EAGAIN)
		pthread_yield();

	if (ret == -FI_EAVAIL) {
		fi_cq_readerr(msg_cq[0], &err_cqe, 0);
		dbg_printf("fi_cq_readerr err:%d\n", err_cqe.err);
	}

	if (write_allowed(FI_RMA, fi[0]->caps, fi[1]->caps)) {
		cr_assert(ret == 1,
			  "fi_write failed caps:0x%lx ret:%d",
			  fi[0]->caps, ret);
	} else {
		cr_assert(err_cqe.err == FI_EOPNOTSUPP,
			  "fi_write should fail caps:0x%lx err:%d",
			  fi[0]->caps, err_cqe.err);
	}

	fi_read(ep[0], source, len,
		loc_mr[0], gni_addr[1],
		_REM_ADDR(fi[0], target, target), mr_key[1],
		(void *)READ_CTX);

	while ((ret = fi_cq_read(msg_cq[0], &cqe, 1)) == -FI_EAGAIN)
		pthread_yield();

	if (ret == -FI_EAVAIL) {
		fi_cq_readerr(msg_cq[0], &err_cqe, 0);
		dbg_printf("fi_cq_readerr err:%d\n", err_cqe.err);
	}

	if (read_allowed(FI_RMA, fi[0]->caps, fi[1]->caps)) {
		cr_assert(ret == 1,
			  "fi_read failed caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	} else {
		cr_assert(err_cqe.err == FI_EOPNOTSUPP,
			  "fi_read should fail caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	}
}

static inline void __rma_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA;
	hints[1]->caps = FI_RMA;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, rma_only)
{
	__rma_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, rma_only)
{
	__rma_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __rma_write_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_WRITE;
	hints[1]->caps = FI_RMA | FI_REMOTE_WRITE;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, rma_write_only, .disabled = true)
{
	__rma_write_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, rma_write_only, .disabled = true)
{
	__rma_write_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __rma_write_no_remote(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_WRITE;
	hints[1]->caps = FI_RMA | FI_WRITE;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, rma_write_no_remote)
{
	__rma_write_no_remote(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, rma_write_no_remote)
{
	__rma_write_no_remote(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __rma_read_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_READ;
	hints[1]->caps = FI_RMA | FI_REMOTE_READ;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, rma_read_only, .disabled = true)
{
	__rma_read_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, rma_read_only)
{
	__rma_read_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __rma_read_no_remote(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_READ;
	hints[1]->caps = FI_RMA | FI_READ;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, rma_read_no_remote)
{
	__rma_read_no_remote(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, rma_read_no_remote)
{
	__rma_read_no_remote(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __rma_write_read_w_msg(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_MSG;
	hints[1]->caps = FI_MSG;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, rma_write_read_w_msg)
{
	__rma_write_read_w_msg(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, rma_write_read_w_msg)
{
	__rma_write_read_w_msg(fi_version(), GNIX_MR_SCALABLE);
}

void api_do_read_buf(void)
{
	int ret;
	int len = 8*1024;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe;
	struct fi_cq_err_entry err_cqe;

	rdm_api_init_data(source, BUF_SZ, 0);
	rdm_api_init_data(target, BUF_SZ, 0xad);

	/* cause a chained transaction */
	sz = fi_read(ep[0], source+6, len,
			 loc_mr[0], gni_addr[1],
			 _REM_ADDR(fi[0], target, target+6), mr_key[1],
			 (void *)READ_CTX);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[0], &cqe, 1)) == -FI_EAGAIN)
		pthread_yield();

	if (ret == -FI_EAVAIL) {
		fi_cq_readerr(msg_cq[0], &err_cqe, 0);
		dbg_printf("fi_cq_readerr err:%d\n", err_cqe.err);
	}

	if (read_allowed(FI_RMA, fi[0]->caps, fi[1]->caps)) {
		cr_assert(ret == 1,
			  "fi_read failed caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	} else {
		cr_assert(err_cqe.err == FI_EOPNOTSUPP,
			  "fi_read should fail caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	}
}

static inline void __read_chained(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA;
	hints[1]->caps = FI_RMA;
	rdm_api_setup_ep(version, mr_mode);
	api_do_read_buf();
}

Test(rdm_api_basic, read_chained)
{
	__read_chained(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, read_chained)
{
	__read_chained(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __read_chained_remote(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_READ;
	hints[1]->caps = FI_RMA | FI_REMOTE_READ;
	rdm_api_setup_ep(version, mr_mode);
	api_do_read_buf();
}

Test(rdm_api_basic, read_chained_remote, .disabled = true)
{
	__read_chained_remote(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, read_chained_remote, .disabled = true)
{
	__read_chained_remote(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __read_chained_w_write(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_WRITE;
	hints[1]->caps = FI_RMA | FI_REMOTE_READ;
	rdm_api_setup_ep(version, mr_mode);
	api_do_read_buf();
}

Test(rdm_api_basic, read_chained_w_write, .disabled = true)
{
	__read_chained_w_write(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, read_chained_w_write)
{
	__read_chained_w_write(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __read_chained_no_remote(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_RMA | FI_READ;
	hints[1]->caps = FI_RMA | FI_READ;
	rdm_api_setup_ep(version, mr_mode);
	api_do_read_buf();
}

Test(rdm_api_basic, read_chained_no_remote)
{
	__read_chained_no_remote(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, read_chained_no_remote)
{
	__read_chained_no_remote(fi_version(), GNIX_MR_SCALABLE);
}

#define SOURCE_DATA	0xBBBB0000CCCCULL
#define TARGET_DATA	0xAAAA0000DDDDULL
#define FETCH_SOURCE_DATA	0xACEDACEDULL

void do_atomic_write_fetch(void)
{
	int ret;
	ssize_t sz;
	uint64_t operand;
	struct fi_cq_tagged_entry cqe;
	struct fi_cq_err_entry err_cqe;

	/* u64 */
	*((uint64_t *)source) = SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;

	sz = fi_atomic(ep[0], source, 1,
			   loc_mr[0], gni_addr[1],
			   _REM_ADDR(fi[0], target, target), mr_key[1],
			   FI_UINT64, FI_ATOMIC_WRITE, target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[0], &cqe, 1)) == -FI_EAGAIN)
		pthread_yield();

	if (ret == -FI_EAVAIL) {
		fi_cq_readerr(msg_cq[0], &err_cqe, 0);
		dbg_printf("fi_cq_readerr err:%d\n", err_cqe.err);
	}

	if (write_allowed(FI_ATOMIC, fi[0]->caps, fi[1]->caps)) {
		cr_assert(ret == 1,
			  "fi_atomic failed caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	} else {
		cr_assert(err_cqe.err == FI_EOPNOTSUPP,
			  "fi_atomic should fail caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	}

	/* u64 */
	operand = SOURCE_DATA;
	*((uint64_t *)source) = FETCH_SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_fetch_atomic(ep[0], &operand, 1, NULL,
				 source, loc_mr[0], gni_addr[1],
				 _REM_ADDR(fi[0], target, target),
				 mr_key[1], FI_UINT64, FI_ATOMIC_READ, target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[0], &cqe, 1)) == -FI_EAGAIN)
		pthread_yield();

	if (ret == -FI_EAVAIL) {
		fi_cq_readerr(msg_cq[0], &err_cqe, 0);
		dbg_printf("fi_cq_readerr err:%d\n", err_cqe.err);
		}

	if (read_allowed(FI_ATOMIC, fi[0]->caps, fi[1]->caps)) {
		cr_assert(ret == 1,
			  "fi_fetch_atomic failed caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	} else {
		cr_assert(err_cqe.err == FI_EOPNOTSUPP,
			  "fi_fetch_atomic should fail caps:0x%lx rcaps:0x%lx",
			  fi[0]->caps, fi[1]->caps);
	}
}

static inline void __amo_write_read(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_ATOMIC;
	hints[1]->caps = FI_ATOMIC;
	rdm_api_setup_ep(version, mr_mode);
	do_atomic_write_fetch();
}

Test(rdm_api_basic, amo_write_read)
{
	__amo_write_read(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, amo_write_read)
{
	__amo_write_read(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __amo_write_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_ATOMIC | FI_WRITE;
	hints[1]->caps = FI_ATOMIC | FI_REMOTE_WRITE;
	rdm_api_setup_ep(version, mr_mode);
	do_atomic_write_fetch();
}

Test(rdm_api_basic, amo_write_only, .disabled = true)
{
	__amo_write_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, amo_write_only)
{
	__amo_write_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __amo_write_no_remote(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_ATOMIC | FI_WRITE;
	hints[1]->caps = FI_ATOMIC | FI_WRITE;
	rdm_api_setup_ep(version, mr_mode);
	do_atomic_write_fetch();
}

Test(rdm_api_basic, amo_write_no_remote)
{
	__amo_write_no_remote(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, amo_write_no_remote)
{
	__amo_write_no_remote(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __amo_read_only(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_ATOMIC | FI_READ;
	hints[1]->caps = FI_ATOMIC | FI_REMOTE_READ;
	rdm_api_setup_ep(version, mr_mode);
	do_atomic_write_fetch();
}

Test(rdm_api_basic, amo_read_only, .disabled = true)
{
	__amo_read_only(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, amo_read_only)
{
	__amo_read_only(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __amo_read_no_remote(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_ATOMIC | FI_READ;
	hints[1]->caps = FI_ATOMIC | FI_READ;
	rdm_api_setup_ep(version, mr_mode);
	do_atomic_write_fetch();
}

Test(rdm_api_basic, amo_read_no_remote)
{
	__amo_read_no_remote(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, amo_read_no_remote)
{
	__amo_read_no_remote(fi_version(), GNIX_MR_SCALABLE);
}

static inline void __amo_write_read_w_msg(uint32_t version, int mr_mode)
{
	hints[0]->caps = FI_MSG;
	hints[1]->caps = FI_MSG;
	rdm_api_setup_ep(version, mr_mode);
	api_write_read(BUF_SZ);
}

Test(rdm_api_basic, amo_write_read_w_msg)
{
	__amo_write_read_w_msg(fi_version(), GNIX_MR_BASIC);
}

Test(rdm_api_scalable, amo_write_read_w_msg)
{
	__amo_write_read_w_msg(fi_version(), GNIX_MR_SCALABLE);
}

TestSuite(api, .init = api_setup, .fini = api_teardown, .disabled = false);

Test(api, getinfo_w_null_hints)
{
	int ret;

	ret = fi_getinfo(fi_version(), NULL, 0, 0, NULL, &fi[0]);
	cr_assert(ret == FI_SUCCESS, "fi_getinfo returned: %s",
		  fi_strerror(-ret));
}
