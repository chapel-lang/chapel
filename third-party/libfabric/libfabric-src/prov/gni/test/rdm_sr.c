/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2018 Cray Inc. All rights reserved.
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
#include "gnix_util.h"
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
 * be careful about API-1.1 setup in rdm_sr_setup_common_eps below
 * if you increase NUMEPS beyond 2
 */
#define NUMEPS 2

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom[NUMEPS];
struct fi_gni_ops_domain *gni_domain_ops[NUMEPS];
static struct fid_ep *ep[NUMEPS];
static struct fid_av *av[NUMEPS];
static void *ep_name[NUMEPS];
static fi_addr_t gni_addr[NUMEPS];
static struct fid_cq *msg_cq[NUMEPS];
static struct fi_info *fi[NUMEPS];
static struct fi_cq_attr cq_attr;
static const char *cdm_id[NUMEPS] = { "5000", "5001" };
struct fi_info *hints;
static int using_bnd_ep = 0;
static int dgram_should_fail;
static int eager_auto = 0;
static int peer_src_known = 1;

#define BUF_SZ (1<<20)
#define BUF_RNDZV (1<<14)
#define IOV_CNT (1<<3)

static char *target, *target_base;
static char *target2, *target2_base;
static char *source, *source_base;
static char *source2, *source2_base;
static struct iovec *src_iov, *dest_iov, *s_iov, *d_iov;
static char *iov_src_buf, *iov_dest_buf, *iov_src_buf_base, *iov_dest_buf_base;
static char *uc_target;
static char *uc_source;
static struct fid_mr *rem_mr[NUMEPS], *loc_mr[NUMEPS];
static struct fid_mr *iov_dest_buf_mr[NUMEPS], *iov_src_buf_mr[NUMEPS];
static uint64_t iov_dest_buf_mr_key[NUMEPS];
static uint64_t mr_key[NUMEPS];

static struct fid_cntr *send_cntr[NUMEPS], *recv_cntr[NUMEPS];
static struct fi_cntr_attr cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.flags = 0
};
static uint64_t sends[NUMEPS] = {0}, recvs[NUMEPS] = {0},
	send_errs[NUMEPS] = {0}, recv_errs[NUMEPS] = {0};

void rdm_sr_setup_common_eps(void)
{
	int ret = 0, i = 0, j = 0;
	struct fi_av_attr attr;
	size_t addrlen = 0;
	bool is_fi_source = !!(hints->caps & FI_SOURCE);

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = NUMEPS;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	target2_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(target2_base);
	target2 = GNIT_ALIGN_BUFFER(char *, target2_base);

	dest_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	assert(dest_iov);
	d_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	assert(d_iov);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	source2_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source2_base);
	source2 = GNIT_ALIGN_BUFFER(char *, source2_base);

	src_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	assert(src_iov);
	s_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	assert(s_iov);

	for (i = 0; i < IOV_CNT; i++) {
		src_iov[i].iov_base = malloc(BUF_SZ);
		assert(src_iov[i].iov_base != NULL);

		dest_iov[i].iov_base = malloc(BUF_SZ * 3);
		assert(dest_iov[i].iov_base != NULL);
	}

	iov_src_buf_base = malloc(GNIT_ALIGN_LEN(BUF_SZ * IOV_CNT));
	assert(iov_src_buf_base);
	iov_src_buf = GNIT_ALIGN_BUFFER(char *, iov_src_buf_base);

	iov_dest_buf_base = malloc(GNIT_ALIGN_LEN(BUF_SZ * IOV_CNT));
	assert(iov_dest_buf_base);
	iov_dest_buf = GNIT_ALIGN_BUFFER(char *, iov_dest_buf_base);

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

		if (eager_auto)
			ret = gni_domain_ops[i]->set_val(&dom[i]->fid,
							 GNI_EAGER_AUTO_PROGRESS,
							 &eager_auto);

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
		/*
		 * To test API-1.1: Reporting of unknown source addresses --
		 * only insert addresses into the sender's av
		 */
		if (is_fi_source && !peer_src_known && i < (NUMEPS / 2)) {
			for (j = 0; j < NUMEPS; j++) {
				dbg_printf("Only does src EP insertions\n");
				ret = fi_av_insert(av[i], ep_name[j], 1,
						   &gni_addr[j],
						   0, NULL);
				cr_assert(ret == 1);
			}
		} else if (peer_src_known) {
			for (j = 0; j < NUMEPS; j++) {
				ret = fi_av_insert(av[i], ep_name[j], 1,
						   &gni_addr[j],
						   0, NULL);
				cr_assert(ret == 1);
			}
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

void rdm_sr_setup_common(void)
{
	int ret = 0, i = 0, j = 0;

	rdm_sr_setup_common_eps();
	int req_key[4];

	for (i = 0; i < NUMEPS; i++) {
		for (j = 0; j < 4; j++)
			req_key[j] = (USING_SCALABLE(fi[i])) ? (i * 4) + j : 0;

		ret = fi_mr_reg(dom[i],
				  target,
				  BUF_SZ,
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

		ret = fi_mr_reg(dom[i],
				  iov_dest_buf,
				  IOV_CNT * BUF_SZ,
				  FI_REMOTE_WRITE,
				  0,
				  req_key[2],
				  0,
				  iov_dest_buf_mr + i,
				  &iov_dest_buf);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i],
				  iov_src_buf,
				  IOV_CNT * BUF_SZ,
				  FI_REMOTE_WRITE,
				  0,
				  req_key[3],
				  0,
				  iov_src_buf_mr + i,
				  &iov_src_buf);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi[i])) {
			MR_ENABLE(rem_mr[i],
				  target,
				  BUF_SZ);
			MR_ENABLE(loc_mr[i],
				  source,
				  BUF_SZ);
			MR_ENABLE(iov_dest_buf_mr[i],
				  iov_dest_buf,
				  IOV_CNT * BUF_SZ);
			MR_ENABLE(iov_src_buf_mr[i],
				  iov_src_buf,
				  IOV_CNT * BUF_SZ);
		}

		mr_key[i] = fi_mr_key(rem_mr[i]);
		iov_dest_buf_mr_key[i] = fi_mr_key(iov_dest_buf_mr[i]);
	}
}

/* Note: default ep type is FI_EP_RDM (used in rdm_sr_setup) */
void rdm_sr_setup(bool is_noreg, enum fi_progress pm)
{
	int ret = 0, i = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->control_progress = pm;
	hints->domain_attr->data_progress = pm;
	hints->mode = mode_bits;
	hints->caps = is_noreg ? hints->caps : FI_SOURCE | FI_MSG;
	hints->fabric_attr->prov_name = strdup("gni");

	/* Get info about fabric services with the provided hints */
	for (; i < NUMEPS; i++) {
		ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	if (is_noreg)
		rdm_sr_setup_common_eps();
	else
		rdm_sr_setup_common();

	dgram_should_fail = 0;
}

void dgram_sr_setup(uint32_t version, bool is_noreg, enum fi_progress pm)
{
	int ret = 0, i = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->control_progress = pm;
	hints->domain_attr->data_progress = pm;
	hints->mode = mode_bits;
	hints->caps = is_noreg ? hints->caps : FI_SOURCE | FI_MSG;
	if (FI_VERSION_GE(version, FI_VERSION(1, 5))) {
		hints->caps |= FI_SOURCE_ERR;
	}
	hints->fabric_attr->prov_name = strdup("gni");
	hints->ep_attr->type = FI_EP_DGRAM;

	/* Get info about fabric services with the provided hints */
	for (; i < NUMEPS; i++) {
		ret = fi_getinfo(version, NULL, 0, 0, hints, &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	if (is_noreg)
		rdm_sr_setup_common_eps();
	else
		rdm_sr_setup_common();
}

static void rdm_sr_setup_reg_eager_auto(void)
{
	eager_auto = 1;
	rdm_sr_setup(false, FI_PROGRESS_AUTO);
}

static void rdm_sr_setup_reg(void)
{
	eager_auto = 0;
	rdm_sr_setup(false, FI_PROGRESS_AUTO);
}

static void dgram_sr_setup_reg(void)
{
	eager_auto = 0;
	dgram_sr_setup(fi_version(), false, FI_PROGRESS_AUTO);
}

static void dgram_sr_setup_reg_src_unk_api_version_old(void)
{
	eager_auto = 0;
	peer_src_known = 0;
	dgram_sr_setup(FI_VERSION(1, 0), false, FI_PROGRESS_AUTO);
}

static void dgram_sr_setup_reg_src_unk_api_version_cur(void)
{
	eager_auto = 0;
	peer_src_known = 0;
	dgram_sr_setup(fi_version(), false, FI_PROGRESS_AUTO);
}

static void rdm_sr_setup_noreg(void) {
	eager_auto = 0;
	rdm_sr_setup(true, FI_PROGRESS_AUTO);
}

void rdm_sr_bnd_ep_setup(void)
{
	int ret = 0, i = 0;
	char my_hostname[HOST_NAME_MAX];

	eager_auto = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->mode = mode_bits;
	hints->fabric_attr->prov_name = strdup("gni");
	hints->caps = FI_SOURCE | FI_MSG;

	ret = gethostname(my_hostname, sizeof(my_hostname));
	cr_assert(!ret, "gethostname");

	for (; i < NUMEPS; i++) {
		ret = fi_getinfo(fi_version(), my_hostname,
				 cdm_id[i], 0, hints, fi + i);
		cr_assert(!ret, "fi_getinfo");
	}

	using_bnd_ep = 1;

	rdm_sr_setup_common();
}

static void rdm_sr_teardown_common(bool unreg)
{
	int ret = 0, i = 0;

	for (; i < NUMEPS; i++) {
		fi_close(&recv_cntr[i]->fid);
		fi_close(&send_cntr[i]->fid);

		if (unreg) {
			fi_close(&loc_mr[i]->fid);
			fi_close(&rem_mr[i]->fid);
			fi_close(&iov_dest_buf_mr[i]->fid);
			fi_close(&iov_src_buf_mr[i]->fid);
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
	}

	free(uc_source);
	free(uc_target);

	free(iov_src_buf_base);
	free(iov_dest_buf_base);
	free(target_base);
	free(source_base);

	for (i = 0; i < IOV_CNT; i++) {
		free(src_iov[i].iov_base);
		free(dest_iov[i].iov_base);
	}

	free(src_iov);
	free(dest_iov);
	free(s_iov);
	free(d_iov);

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(hints);
}

static void rdm_sr_teardown(void)
{
	rdm_sr_teardown_common(true);
}

static void rdm_sr_teardown_nounreg(void)
{
	rdm_sr_teardown_common(false);
}

void rdm_sr_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++) {
		buf[i] = seed++;
	}
}

static inline int rdm_sr_check_data(char *buf1, char *buf2, int len)
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

static inline int rdm_sr_check_iov_data(struct iovec *iov_buf, char *buf,
					size_t cnt, size_t buf_len)
{
	size_t i, j, cum_len = 0, len, iov_idx;

	for (i = 0; i < cnt; i++) {
		cum_len += iov_buf[i].iov_len;
	}

	len = MIN(cum_len, buf_len);

	cum_len = iov_buf[0].iov_len;

	for (i = j = iov_idx = 0; j < len; j++, iov_idx++) {

		if (j == cum_len) {
			i++, iov_idx = 0;
			cum_len += iov_buf[i].iov_len;

			if (i >= cnt)
				break;
		}

		if (((char *)iov_buf[i].iov_base)[iov_idx] != buf[j]) {
			printf("data mismatch, iov_index: %lu, elem: %lu, "
			       "iov_buf_len: %lu, "
			       " iov_buf: %hhx, buf: %hhx\n", i, j, iov_buf[i].iov_len,
			       ((char *)iov_buf[i].iov_base)[iov_idx],
			       buf[j]);
			return 0;
		}
	}

	return 1;
}

void rdm_sr_xfer_for_each_size(void (*xfer)(int len), int slen, int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2) {
		xfer(i);
	}
}

static inline void rdm_sr_check_err_cqe(struct fi_cq_err_entry *cqe, void *ctx,
					uint64_t flags, void *addr, size_t len,
					uint64_t data, bool buf_is_non_null)
{
	cr_assert(cqe->op_context == ctx, "error CQE Context mismatch");
	cr_assert(cqe->flags == flags, "error CQE flags mismatch");

	if (flags & FI_RECV) {
		if (cqe->len != len) {
			cr_assert(cqe->olen == (len - cqe->len), "error CQE "
				"olen mismatch");
		} else {
			cr_assert(cqe->olen == 0, "error CQE olen mismatch");
		}

		if (buf_is_non_null)
			cr_assert(cqe->buf == addr, "error CQE address "
				"mismatch");
		else
			cr_assert(cqe->buf == NULL, "error CQE address "
				"mismatch");

		if (flags & FI_REMOTE_CQ_DATA)
			cr_assert(cqe->data == data, "error CQE data mismatch");
	} else {
		cr_assert(cqe->len == 0, "Invalid error CQE length");
		cr_assert(cqe->buf == 0, "Invalid error CQE address");
		cr_assert(cqe->data == 0, "Invalid error CQE data");
	}

	cr_assert(cqe->tag == 0, "Invalid error CQE tag");
	cr_assert(cqe->err > 0, "Invalid error CQE err code");

	/*
	 * Note: cqe->prov_errno and cqe->err_data are not necessarily set --
	 * see the fi_cq_readerr man page
	 */
}

static inline void rdm_sr_check_cqe(struct fi_cq_tagged_entry *cqe, void *ctx,
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

static inline void rdm_sr_check_cntrs(uint64_t s[], uint64_t r[],
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

void rdm_sr_err_inject_enable(void)
{
	int ret, err_count_val = 1, i = 0;

	for (; i < NUMEPS; i++) {
		ret = gni_domain_ops[i]->set_val(&dom[i]->fid,
						 GNI_ERR_INJECT_COUNT,
						 &err_count_val);
		cr_assert(!ret, "setval(GNI_ERR_INJECT_COUNT)");
	}
}

void rdm_sr_lazy_dereg_disable(void)
{
	int ret, lazy_dereg_val = 0, i = 0;

	for (; i < NUMEPS; i++) {
		ret = gni_domain_ops[i]->set_val(&dom[i]->fid,
						 GNI_MR_CACHE_LAZY_DEREG,
						 &lazy_dereg_val);
		cr_assert(!ret, "setval(GNI_MR_CACHE_LAZY_DEREG)");
	}
}

static inline struct fi_cq_err_entry rdm_sr_check_canceled(struct fid_cq *cq)
{
	int ret;
	struct fi_cq_err_entry ee;
	struct gnix_ep_name err_ep_name, ep_name_test;
	struct gnix_fid_cq *cq_priv;
	size_t name_size;
	fi_addr_t fi_addr;

	/*application provided error_data buffer and length*/
	ee.err_data_size = sizeof(struct gnix_ep_name);
	ee.err_data = &err_ep_name;

	fi_cq_readerr(cq, &ee, 0);

	/*
	 * TODO: Check for api version once we figure out how to.
	 * Note: The address of err_ep_name should be the same as ee.err_data
	 * when using api version >= 1.5.
	 */
	cq_priv = container_of(cq, struct gnix_fid_cq, cq_fid);
	if (FI_VERSION_LT(cq_priv->domain->fabric->fab_fid.api_version, FI_VERSION(1, 5))) {
		cr_assert(ee.err_data != &err_ep_name, "Invalid err_data ptr");
	} else {
		cr_assert(ee.err_data == &err_ep_name, "Invalid err_data ptr");
	}

	/* To test API-1.1: Reporting of unknown source addresses */
	if ((hints->caps & FI_SOURCE) && ee.err == FI_EADDRNOTAVAIL) {
	        if (FI_VERSION_GE(cq_priv->domain->fabric->fab_fid.api_version,
				FI_VERSION(1, 5))) {
			cr_assert(ee.err_data_size == sizeof(struct gnix_ep_name),
					"Invalid err_data_size returned");
			ret = fi_av_insert(av[1], &err_ep_name, 1, &fi_addr,
					   0, NULL);
			cr_assert(ret == 1, "fi_av_insert failed");
			name_size = sizeof(ep_name_test);
			ret = fi_av_lookup(av[1], fi_addr,
					   &ep_name_test, &name_size);
			cr_assert(ret == FI_SUCCESS, "fi_av_lookup failed");
			cr_assert(name_size == sizeof(ep_name_test));
			cr_assert(strncmp((char *)&ep_name_test,
					  (char *)&err_ep_name,
					  sizeof(ep_name_test)) == 0);
		}
	}
	return ee;
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

TestSuite(rdm_sr_eager_auto,
	  .init = rdm_sr_setup_reg_eager_auto,
	  .fini = rdm_sr_teardown,
	  .disabled = false);
TestSuite(rdm_sr,
	  .init = rdm_sr_setup_reg,
	  .fini = rdm_sr_teardown,
	  .disabled = false);

TestSuite(dgram_sr,
	  .init = dgram_sr_setup_reg,
	  .fini = rdm_sr_teardown,
	  .disabled = false);

TestSuite(dgram_sr_src_unk_api_version_old,
	  .init = dgram_sr_setup_reg_src_unk_api_version_old,
	  .fini = rdm_sr_teardown, .disabled = false);

TestSuite(dgram_sr_src_unk_api_version_cur,
	  .init = dgram_sr_setup_reg_src_unk_api_version_cur,
	  .fini = rdm_sr_teardown, .disabled = false);

TestSuite(rdm_sr_noreg,
	  .init = rdm_sr_setup_noreg,
	  .fini = rdm_sr_teardown_nounreg,
	  .disabled = false);

TestSuite(rdm_sr_bnd_ep,
	  .init = rdm_sr_bnd_ep_setup,
	  .fini = rdm_sr_teardown,
	  .disabled = false);

/* This tests cases where the head and tail length is greater or equal to the
 * receive buffer length.
 */
TestSuite(rdm_sr_alignment_edge,
	  .init = rdm_sr_setup_reg,
	  .fini = rdm_sr_teardown,
	  .disabled = true);

/*
 * ssize_t fi_send(struct fid_ep *ep, void *buf, size_t len,
 *		void *desc, fi_addr_t dest_addr, void *context);
 *
 * ssize_t fi_recv(struct fid_ep *ep, void * buf, size_t len,
 *		void *desc, fi_addr_t src_addr, void *context);
 */
void do_send(int len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	int scanceled = 0, dcanceled = 0, daddrnotavail = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_err_entry d_err_cqe;
	struct fi_cq_err_entry s_err_cqe;

	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	memset(&d_err_cqe, -1, sizeof(struct fi_cq_err_entry));
	memset(&s_err_cqe, -1, sizeof(struct fi_cq_err_entry));

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[1], gni_addr[0], source);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}
		if (ret == -FI_EAVAIL) {
			s_err_cqe = rdm_sr_check_canceled(msg_cq[0]);
			if (s_err_cqe.err == FI_ECANCELED)
				scanceled = 1;
		}
		ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
		if (ret == 1) {
			dest_done = 1;
		}
		if (ret == -FI_EAVAIL) {
			d_err_cqe = rdm_sr_check_canceled(msg_cq[1]);
			if (d_err_cqe.err == FI_ECANCELED)
				dcanceled = 1;
			else if (d_err_cqe.err == FI_EADDRNOTAVAIL &&
				!peer_src_known)
				daddrnotavail = 1;
		}
	} while (!((source_done || scanceled) &&
		(dest_done || dcanceled || daddrnotavail)));

	/* no further checking needed */
	if (dgram_should_fail && (scanceled || dcanceled))
		return;

	if (daddrnotavail || dcanceled)
		rdm_sr_check_err_cqe(&d_err_cqe, source, (FI_MSG|FI_RECV),
				     target, len, 0, false);
	else
		rdm_sr_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV), target, len,
				 0, false, ep[1]);

	if (scanceled)
		rdm_sr_check_err_cqe(&s_err_cqe, target, (FI_MSG|FI_SEND), 0,
				     0, 0, false);
	else
		rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0,
				 false, ep[0]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, send)
{
	rdm_sr_xfer_for_each_size(do_send, 1, BUF_SZ);
}

Test(rdm_sr, send_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_send, 1, BUF_SZ);
}

Test(dgram_sr, send)
{
	rdm_sr_xfer_for_each_size(do_send, 1, BUF_SZ);
}

Test(dgram_sr_src_unk_api_version_old, send)
{
	rdm_sr_xfer_for_each_size(do_send, 1, 1);
}

Test(dgram_sr_src_unk_api_version_cur, send)
{
	rdm_sr_xfer_for_each_size(do_send, 1, 1);
}

Test(dgram_sr, send_retrans)
{
	dgram_should_fail = 1;
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_send, BUF_RNDZV, BUF_SZ);
}

/*
ssize_t fi_sendv(struct fid_ep *ep, const struct iovec *iov, void **desc,
		 size_t count, fi_addr_t dest_addr, void *context);
*/
void do_sendv(int len)
{
	int i, ret, iov_cnt;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sz = fi_sendv(ep[0], src_iov, NULL, 0, gni_addr[1], iov_dest_buf);
	cr_assert_eq(sz, -FI_EINVAL);

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			rdm_sr_init_data(src_iov[i].iov_base, len, 0x25);
			src_iov[i].iov_len = len;
		}
		rdm_sr_init_data(iov_dest_buf, len * iov_cnt, 0);

		/*
		 * TODO: Register src_iov and dest_iov.
		 * Using NULL descriptor for now so that _gnix_send auto registers
		 * the buffers for rndzv messages.
		 */
		sz = fi_sendv(ep[0], src_iov, NULL, iov_cnt, gni_addr[1], iov_dest_buf);
		cr_assert_eq(sz, 0);

		sz = fi_recv(ep[1], iov_dest_buf, len * iov_cnt, iov_dest_buf_mr[1],
			     gni_addr[0], src_iov);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

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

		rdm_sr_check_cqe(&s_cqe, iov_dest_buf, (FI_MSG|FI_SEND), 0, 0, 0,
				false, ep[0]);
		rdm_sr_check_cqe(&d_cqe, src_iov, (FI_MSG|FI_RECV), iov_dest_buf,
				 len * iov_cnt, 0, false, ep[1]);

		s[0] = 1; r[1] = 1;
		rdm_sr_check_cntrs(s, r, s_e, r_e, false);

		dbg_printf("got context events!\n");

		cr_assert(rdm_sr_check_iov_data(src_iov, iov_dest_buf, iov_cnt, len * iov_cnt),
			  "Data mismatch");
		source_done = dest_done = 0;
	}
}

Test(rdm_sr, sendv)
{
	rdm_sr_xfer_for_each_size(do_sendv, 1, BUF_SZ);
}

Test(rdm_sr, sendv_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_sendv, 1, BUF_SZ);
}

Test(rdm_sr, bug_1390)
{
	ssize_t sz;
	int i, iov_cnt;
	int len = 4096;
	void *mr_descs[IOV_CNT] = {NULL};

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			rdm_sr_init_data(src_iov[i].iov_base, len, 0x25);
			src_iov[i].iov_len = len;
		}
		rdm_sr_init_data(iov_dest_buf, len * iov_cnt, 0);
	}

	sz = fi_sendv(ep[0], src_iov, (void **) &mr_descs,
			4, gni_addr[1], iov_dest_buf);
	cr_assert_eq(sz, -FI_EINVAL);
}

/*
ssize_t fi_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
		   uint64_t flags);
*/
void do_sendmsg(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_msg msg;
	struct iovec iov;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	iov.iov_base = source;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	msg.context = target;
	msg.data = (uint64_t)target;

	rdm_sr_init_data(source, len, 0xef);
	rdm_sr_init_data(target, len, 0);

	sz = fi_sendmsg(ep[0], &msg, 0);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[0], gni_addr[0], source);
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

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);
	rdm_sr_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV), target, len, 0,
			false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, sendmsg)
{
	rdm_sr_xfer_for_each_size(do_sendmsg, 1, BUF_SZ);
}

Test(rdm_sr, sendmsg_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_sendmsg, 1, BUF_SZ);
}

/*
ssize_t fi_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
		   uint64_t flags);
*/

void do_sendmsgdata(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_msg msg;
	struct iovec iov;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	iov.iov_base = source;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	msg.context = target;
	msg.data = (uint64_t)source;

	rdm_sr_init_data(source, len, 0xef);
	rdm_sr_init_data(target, len, 0);

	sz = fi_sendmsg(ep[0], &msg, FI_REMOTE_CQ_DATA);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[0], gni_addr[0], source);
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

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false, ep[0]);
	rdm_sr_check_cqe(&d_cqe, source,
			 (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)source, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, sendmsgdata)
{
	rdm_sr_xfer_for_each_size(do_sendmsgdata, 1, BUF_SZ);
}

Test(rdm_sr, sendmsgdata_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_sendmsgdata, 1, BUF_SZ);
}

/*
ssize_t fi_inject(struct fid_ep *ep, void *buf, size_t len,
		  fi_addr_t dest_addr);
*/
#define INJECT_SIZE 64
void do_inject(int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	static gnix_mr_cache_t *cache;
	struct gnix_fid_ep *ep_priv;
	int already_registered = 0;

	rdm_sr_init_data(source, len, 0x23);
	rdm_sr_init_data(target, len, 0);

	if (!USING_SCALABLE(fi[0])) {
		ep_priv = container_of(ep[0], struct gnix_fid_ep, ep_fid);
		cache = GET_DOMAIN_RW_CACHE(ep_priv->domain);
		cr_assert(cache != NULL);
		already_registered = ofi_atomic_get32(&cache->inuse.elements);
	}

	sz = fi_inject(ep[0], source, len, gni_addr[1]);
	cr_assert_eq(sz, 0);

	if (!USING_SCALABLE(fi[0])) {
		/*
		 * shouldn't have registered the source buffer,
		 * trust but verify
		 */
		cr_assert(ofi_atomic_get32(&cache->inuse.elements)
				== already_registered);
	}

	sz = fi_recv(ep[1], target, len, rem_mr[1], gni_addr[0], source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[1], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
		/* Manually progress connection to domain 1 */
		fi_cq_read(msg_cq[0], &cqe, 1);
	}

	cr_assert_eq(ret, 1);
	rdm_sr_check_cqe(&cqe, source, (FI_MSG|FI_RECV),
			 target, len, (uint64_t)source, false, ep[1]);

	dbg_printf("got recv context event!\n");

	/* do progress until send counter is updated */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}
	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, true);

	/* make sure inject does not generate a send completion */
	cr_assert_eq(fi_cq_read(msg_cq[0], &cqe, 1), -FI_EAGAIN);

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, inject, .disabled = false)
{
	rdm_sr_xfer_for_each_size(do_inject, 1, INJECT_SIZE);
}

/*
 * this test attempts to demonstrate issue ofi-cray/libfabric-cray#559.
 * For domains with control_progress AUTO, this test should not hang.
 */
Test(rdm_sr, inject_progress)
{
	int ret, len = 64;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0x23);
	rdm_sr_init_data(target, len, 0);

	sz = fi_inject(ep[0], source, len, gni_addr[1]);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[1], gni_addr[0], source);
	cr_assert_eq(sz, 0);

	/*
	 * do progress until send counter is updated.
	 * This works because we have FI_PROGRESS_AUTO for control progress
	 */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}

	while ((ret = fi_cq_read(msg_cq[1], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	rdm_sr_check_cqe(&cqe, source, (FI_MSG|FI_RECV),
			 target, len, (uint64_t)source, false, ep[1]);

	dbg_printf("got recv context event!\n");

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, true);

	/* make sure inject does not generate a send competion */
	cr_assert_eq(fi_cq_read(msg_cq[0], &cqe, 1), -FI_EAGAIN);

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, inject_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_inject, 1, INJECT_SIZE);
}

void do_senddata_eager_auto(int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe[2];
	struct fi_msg msg;
	struct iovec iov;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(source2, len, 0xdc);
	rdm_sr_init_data(target, len, 0);
	rdm_sr_init_data(target2, len, 1);

	iov.iov_base = source2;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = NULL;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	msg.context = target;
	msg.data = (uint64_t)target;

	sz = fi_sendmsg(ep[0], &msg, FI_REMOTE_CQ_DATA);
	cr_assert_eq(sz, 0);

	iov.iov_base = source;
	iov.iov_len = len;
	msg.desc = (void **)loc_mr;

	sz = fi_sendmsg(ep[0], &msg, FI_REMOTE_CQ_DATA | FI_FENCE);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target2, len, NULL, gni_addr[0], source2);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[0], gni_addr[0], source);
	cr_assert_eq(sz, 0);

	/* Wait for auto-progress threads to do all the work. */
	sleep(1);

	/* If progress works, events should be ready right away. */
	ret = fi_cq_read(msg_cq[1], &d_cqe, 2);
	cr_assert_eq(ret, 2);

	rdm_sr_check_cqe(&d_cqe[0], source2, (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)target, false, ep[1]);
	rdm_sr_check_cqe(&d_cqe[1], source, (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)target, false, ep[1]);

	ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
	cr_assert_eq(ret, 1);

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);

	ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
	cr_assert_eq(ret, 1);

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);

	s[0] = 2; r[1] = 2;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr_eager_auto, senddata_eager_auto)
{
	/* FIXME intermittent test failures */
	cr_skip_test("intermittent test failures");

	/* Try eager and rndzv sizes */
	do_senddata_eager_auto(1);
	do_senddata_eager_auto(1024);
	do_senddata_eager_auto(BUF_SZ);
}

/*
ssize_t fi_senddata(struct fid_ep *ep, void *buf, size_t len, void *desc,
		    uint64_t data, fi_addr_t dest_addr, void *context);
*/
void do_senddata(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_senddata(ep[0], source, len, loc_mr[0], (uint64_t)source,
			 gni_addr[1], target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[0], gni_addr[0], source);
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

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false, ep[0]);
	rdm_sr_check_cqe(&d_cqe, source,
			 (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)source, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, senddata)
{
	rdm_sr_xfer_for_each_size(do_senddata, 1, BUF_SZ);
}

Test(rdm_sr, senddata_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_senddata, 1, BUF_SZ);
}

/*
ssize_t fi_injectdata(struct fid_ep *ep, const void *buf, size_t len,
		      uint64_t data, fi_addr_t dest_addr);
*/
void do_injectdata(int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_injectdata(ep[0], source, len, (uint64_t)source, gni_addr[1]);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[0], gni_addr[0], source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[1], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
		/* Manually progress connection to domain 1 */
		fi_cq_read(msg_cq[0], &cqe, 1);
	}

	rdm_sr_check_cqe(&cqe, source, (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)source, false, ep[1]);

	dbg_printf("got recv context event!\n");

	/* don't progress until send counter is updated */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, true);

	/* make sure inject does not generate a send competion */
	cr_assert_eq(fi_cq_read(msg_cq[0], &cqe, 1), -FI_EAGAIN);

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, injectdata, .disabled = false)
{
	rdm_sr_xfer_for_each_size(do_injectdata, 1, INJECT_SIZE);
}

Test(rdm_sr, injectdata_retrans, .disabled = false)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_injectdata, 1, INJECT_SIZE);
}

/*
ssize_t (*recvv)(struct fid_ep *ep, const struct iovec *iov, void **desc,
		 size_t count, fi_addr_t src_addr, void *context);
*/
void do_recvv(int len)
{
	int i, ret, iov_cnt;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sz = fi_recvv(ep[1], NULL, NULL, IOV_CNT, gni_addr[0], iov_src_buf);
	cr_assert_eq(sz, -FI_EINVAL);

	sz = fi_recvv(ep[1], dest_iov, NULL, IOV_CNT + 1, gni_addr[0], iov_src_buf);
	cr_assert_eq(sz, -FI_EINVAL);

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		rdm_sr_init_data(iov_src_buf, len * iov_cnt, 0xab);

		for (i = 0; i < iov_cnt; i++) {
			rdm_sr_init_data(dest_iov[i].iov_base, len, 0);
			dest_iov[i].iov_len = len;
		}

		sz = fi_send(ep[0], iov_src_buf, len * iov_cnt, NULL, gni_addr[1],
			     dest_iov);
		cr_assert_eq(sz, 0);

		sz = fi_recvv(ep[1], dest_iov, NULL, iov_cnt, gni_addr[0], iov_src_buf);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

		/*  need to progress both CQs simultaneously for rendezvous */
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

		rdm_sr_check_cqe(&s_cqe, dest_iov, (FI_MSG|FI_SEND), 0, 0, 0,
				 false, ep[0]);
		rdm_sr_check_cqe(&d_cqe, iov_src_buf, (FI_MSG|FI_RECV), dest_iov,
				 len * iov_cnt, 0, false, ep[1]);

		s[0] = 1; r[1] = 1;
		rdm_sr_check_cntrs(s, r, s_e, r_e, false);

		dbg_printf("got context events!\n");

		cr_assert(rdm_sr_check_iov_data(dest_iov, iov_src_buf, iov_cnt, len * iov_cnt),
			  "Data mismatch");
		source_done = dest_done = 0;
	}
}

Test(rdm_sr, recvv)
{
	rdm_sr_xfer_for_each_size(do_recvv, 1, BUF_SZ);
}

Test(rdm_sr, recvv_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_recvv, 1, BUF_SZ);
}

/*
ssize_t (*recvmsg)(struct fid_ep *ep, const struct fi_msg *msg,
		   uint64_t flags);
*/
void do_recvmsg(int len)
{
	int ret;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct iovec iov;
	struct fi_msg msg;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	cr_assert_eq(sz, 0);

	iov.iov_base = target;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)rem_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[0];
	msg.context = source;
	msg.data = (uint64_t)source;

	sz = fi_recvmsg(ep[1], &msg, 0);
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

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);
	rdm_sr_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV), target, len, 0,
			false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, recvmsg)
{
	rdm_sr_xfer_for_each_size(do_recvmsg, 1, BUF_SZ);
}

Test(rdm_sr, recvmsg_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_recvmsg, 1, BUF_SZ);
}

Test(rdm_sr_bnd_ep, recvmsg)
{
	rdm_sr_xfer_for_each_size(do_recvmsg, 1, BUF_SZ);
}

void do_send_autoreg(int len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, NULL, gni_addr[1], target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, NULL, gni_addr[0], source);
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

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);
	rdm_sr_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV), target, len, 0,
			false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_sr, send_autoreg)
{
	rdm_sr_xfer_for_each_size(do_send_autoreg, 1, BUF_SZ);
}

Test(rdm_sr, send_autoreg_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_send_autoreg, 1, BUF_SZ);
}

void do_send_autoreg_uncached(int len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(uc_source, len, 0xab);
	rdm_sr_init_data(uc_target, len, 0);

	sz = fi_send(ep[0], uc_source, len, NULL, gni_addr[1], uc_target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], uc_target, len, NULL, gni_addr[0], uc_source);
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

	rdm_sr_check_cqe(&s_cqe, uc_target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);
	rdm_sr_check_cqe(&d_cqe, uc_source, (FI_MSG|FI_RECV),
			 uc_target, len, 0, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(uc_source, uc_target, len),
		  "Data mismatch");
}

Test(rdm_sr, send_autoreg_uncached)
{
	rdm_sr_xfer_for_each_size(do_send_autoreg_uncached, 1, BUF_SZ);
}

Test(rdm_sr, send_autoreg_uncached_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_send_autoreg_uncached, 1, BUF_SZ);
}

void do_send_err(int len)
{
	int ret;
	struct fi_cq_tagged_entry s_cqe;
	struct fi_cq_err_entry err_cqe = {0};
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	/* Set err_data_size to 0 to have provider allocate buffer if needed */
	err_cqe.err_data_size = 0;

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(msg_cq[0], &s_cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, -FI_EAVAIL);

	ret = fi_cq_readerr(msg_cq[0], &err_cqe, 0);
	cr_assert_eq(ret, 1);

	cr_assert((uint64_t)err_cqe.op_context == (uint64_t)target,
		  "Bad error context");
	cr_assert(err_cqe.flags == (FI_MSG | FI_SEND));
	cr_assert(err_cqe.len == 0, "Bad error len");
	cr_assert(err_cqe.buf == 0, "Bad error buf");
	cr_assert(err_cqe.data == 0, "Bad error data");
	cr_assert(err_cqe.tag == 0, "Bad error tag");
	cr_assert(err_cqe.olen == 0, "Bad error olen");
	cr_assert(err_cqe.err == FI_ECANCELED, "Bad error errno");
	cr_assert(err_cqe.prov_errno == gnixu_to_fi_errno(GNI_RC_TRANSACTION_ERROR),
		  "Bad prov errno");
	cr_assert(err_cqe.err_data == NULL, "Bad error provider data");

	s_e[0] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);
}

Test(rdm_sr, send_err)
{
	int ret, max_retrans_val = 0, i = 0; /* 0 to force SMSG failure */

	for (; i < NUMEPS; i++) {
		ret = gni_domain_ops[i]->set_val(&dom[i]->fid,
						 GNI_MAX_RETRANSMITS,
						 &max_retrans_val);
		cr_assert(!ret, "setval(GNI_MAX_RETRANSMITS)");
	}
	rdm_sr_err_inject_enable();

	rdm_sr_xfer_for_each_size(do_send_err, 1, BUF_SZ);
}

void do_send_autoreg_uncached_nolazydereg(int len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(uc_source, len, 0xab);
	rdm_sr_init_data(uc_target, len, 0);

	sz = fi_send(ep[0], uc_source, len, NULL, gni_addr[1], uc_target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], uc_target, len, NULL, gni_addr[0], uc_source);
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

	rdm_sr_check_cqe(&s_cqe, uc_target, (FI_MSG|FI_SEND), 0, 0, 0, false,
			 ep[0]);
	rdm_sr_check_cqe(&d_cqe, uc_source, (FI_MSG|FI_RECV),
			 uc_target, len, 0, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(uc_source, uc_target, len),
		  "Data mismatch");
}

Test(rdm_sr_noreg, send_autoreg_uncached_nolazydereg)
{
	rdm_sr_lazy_dereg_disable();
	rdm_sr_xfer_for_each_size(do_send_autoreg_uncached_nolazydereg,
				  1, BUF_SZ);
}

Test(rdm_sr, send_readfrom)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	ssize_t sz;
	fi_addr_t src_addr;
	int len = 64;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(source, len, 0xab);
	rdm_sr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, loc_mr[0], gni_addr[1], target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, rem_mr[0], gni_addr[0], source);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
		if (ret == 1) {
			source_done = 1;
		}
		ret = fi_cq_readfrom(msg_cq[1], &d_cqe, 1, &src_addr);
		if (ret == 1) {
			dest_done = 1;
		}
	} while (!(source_done && dest_done));

	rdm_sr_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false, ep[0]);
	rdm_sr_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV), target, len, 0, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	cr_assert(src_addr == gni_addr[0], "src_addr mismatch");

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(source, target, len), "Data mismatch");
}

void do_send_buf(void *p, void *t, int len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	rdm_sr_init_data(p, len, 0xab);
	rdm_sr_init_data(t, len, 0);

	sz = fi_send(ep[0], p, len, loc_mr[0], gni_addr[1], t);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], t, len, rem_mr[0], gni_addr[0], p);
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

	rdm_sr_check_cqe(&s_cqe, t, (FI_MSG|FI_SEND), 0, 0, 0, false, ep[0]);
	rdm_sr_check_cqe(&d_cqe, p, (FI_MSG|FI_RECV), t, len, 0, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(p, t, len), "Data mismatch");
}

void do_send_alignment(int len)
{
	int s_off, t_off, l_off;

	for (s_off = 0; s_off < 7; s_off++) {
		for (t_off = 0; t_off < 7; t_off++) {
			for (l_off = 0; l_off < 7; l_off++) {
				do_send_buf(source + s_off,
					    target + t_off,
					    len + l_off);
			}
		}
	}
}

Test(rdm_sr, send_alignment)
{
	rdm_sr_xfer_for_each_size(do_send_alignment, 1, (BUF_SZ-1));
}

Test(rdm_sr, send_alignment_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_send_alignment, 1, (BUF_SZ-1));
}

void do_sendrecv_buf(void *p, void *t, int send_len, int recv_len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	ssize_t sz;
	int xfer_len;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	/*
	 * this test can't handle truncated messages so skip if recv_len
	 * isn't big enough to receive message
	 */
	if (send_len > recv_len)
		return;

	rdm_sr_init_data(p, send_len, 0xab);
	rdm_sr_init_data(t, recv_len, 0);

	sz = fi_send(ep[0], p, send_len, loc_mr[0], gni_addr[1], t);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], t, recv_len, rem_mr[0], gni_addr[0], p);
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

	xfer_len = MIN(send_len, recv_len);
	rdm_sr_check_cqe(&s_cqe, t, (FI_MSG|FI_SEND), 0, 0, 0, false, ep[0]);
	rdm_sr_check_cqe(&d_cqe, p, (FI_MSG|FI_RECV), t, xfer_len, 0, false, ep[1]);

	s[0] = 1; r[1] = 1;
	rdm_sr_check_cntrs(s, r, s_e, r_e, false);

	dbg_printf("got context events!\n");

	cr_assert(rdm_sr_check_data(p, t, xfer_len), "Data mismatch");
}

void do_sendrecv_alignment(int len)
{
	int s_off, t_off, sl_off, rl_off;

	for (s_off = 0; s_off < 8; s_off++) {
		for (t_off = 0; t_off < 8; t_off++) {
			for (sl_off = -7; sl_off < 8; sl_off++) {
				for (rl_off = -7; rl_off < 8; rl_off++) {
					do_sendrecv_buf(source + s_off,
							target + t_off,
							len + sl_off,
							len + rl_off);
				}
			}
		}
	}
}

void do_sendvrecv_alignment(int slen, int dlen, int offset)
{
	int i, ret, iov_cnt;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	uint64_t iov_d_buf = (uint64_t) iov_dest_buf;

	iov_d_buf += offset;

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			s_iov[i].iov_base = src_iov[i].iov_base;
			s_iov[i].iov_base = (void *) ((uint64_t)s_iov[i].iov_base + offset);
			rdm_sr_init_data(s_iov[i].iov_base, slen - offset, 0x25);
			s_iov[i].iov_len = slen - offset;
		}
		rdm_sr_init_data((void *) iov_d_buf, (dlen - offset) * iov_cnt, 0);
		sz = fi_sendv(ep[0], s_iov, NULL, iov_cnt, gni_addr[1], (void *) iov_d_buf);
		cr_assert_eq(sz, 0);

		sz = fi_recv(ep[1], (void *) iov_d_buf, (dlen - offset) * iov_cnt, (void *) iov_dest_buf_mr[1],
			     gni_addr[0], s_iov);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

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

		rdm_sr_check_cqe(&s_cqe, (void *) iov_d_buf, (FI_MSG|FI_SEND),
				 0, 0, 0, false, ep[0]);
		rdm_sr_check_cqe(&d_cqe, s_iov, (FI_MSG|FI_RECV),
				 (void *) iov_d_buf, (dlen - offset) * iov_cnt,
				 0, false, ep[1]);

		s[0] = 1; r[1] = 1;
		rdm_sr_check_cntrs(s, r, s_e, r_e, false);

		dbg_printf("got context events!\n");

		cr_assert(rdm_sr_check_iov_data(s_iov, (void *) iov_d_buf,
						iov_cnt, (dlen - offset) * iov_cnt),
			  "Data mismatch");
		source_done = dest_done = 0;
	}

}

void do_sendrecvv_alignment(int slen, int dlen, int offset)
{
	int i, ret, iov_cnt;
	ssize_t sz;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	uint64_t iov_s_buf = (uint64_t) iov_src_buf;

	iov_s_buf += offset;

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			d_iov[i].iov_base = dest_iov[i].iov_base;
			d_iov[i].iov_base = (void *) ((uint64_t)d_iov[i].iov_base + offset);
			rdm_sr_init_data(d_iov[i].iov_base, dlen - offset, 0);
			d_iov[i].iov_len = dlen - offset;
		}

		rdm_sr_init_data((void *) iov_s_buf, (slen - offset) * iov_cnt, 0xab);

		sz = fi_send(ep[0], (void *) iov_s_buf, (slen - offset) * iov_cnt, NULL, gni_addr[1],
			     d_iov);
		cr_assert_eq(sz, 0);

		sz = fi_recvv(ep[1], d_iov, NULL, iov_cnt, gni_addr[0], (void *) iov_s_buf);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

		/*  need to progress both CQs simultaneously for rendezvous */
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

		rdm_sr_check_cqe(&s_cqe, d_iov, (FI_MSG|FI_SEND), 0, 0, 0,
				 false, ep[0]);
		rdm_sr_check_cqe(&d_cqe, (void *) iov_s_buf, (FI_MSG|FI_RECV),
				 d_iov,
				 MIN((slen - offset) * iov_cnt, (dlen -
					 offset) * iov_cnt), 0, false, ep[1]);

		s[0] = 1; r[1] = 1;
		rdm_sr_check_cntrs(s, r, s_e, r_e, false);

		dbg_printf("got context events!\n");

		cr_assert(rdm_sr_check_iov_data(d_iov, (void *) iov_s_buf,
						iov_cnt, (slen - offset) * iov_cnt),
			  "Data mismatch");
		source_done = dest_done = 0;
	}
}

void do_sendvrecv_alignment_iter(int len)
{
	int offset;

	/* Check for alignment issues using offsets 1..3 */
	for (offset = 1; offset < GNI_READ_ALIGN; offset++) {
		/* lets assume the user passes in valid addresses */
		if (offset < len) {
			do_sendvrecv_alignment(len, len, offset);
		}
	}
}

void do_sendrecvv_alignment_iter(int len)
{
	int offset;

	/* Check for alignment issues using offsets 1..3 */
	for (offset = 1; offset < GNI_READ_ALIGN; offset++) {
		/* lets assume the user passes in valid addresses */
		if (offset < len) {
			do_sendrecvv_alignment(len, len, offset);
		}
	}
}

void do_iov_alignment_edge(int len)
{
	int offset;

	/* Check for alignment issues using offsets 1..3 */
	for (offset = 1; offset < GNI_READ_ALIGN; offset++) {
		/* lets assume the user passes in valid addresses */
		if (offset < len) {
			/* These calls trigger rendezvous cases on the sender's
			 * side but the recv buffer that's posted will be so
			 * small it will only fit a portion of the sender's data.
			 *
			 * The four byte alignment support in the current
			 * sendv/recvv implementation doesn't support a head/tail
			 * which meets or exceeds a given recv buffer.
			 */

			/* large (IOV) 1..8 x slen of 1..BUF_SZ -> dlen of 2..4 */
			do_sendvrecv_alignment(len, offset + 1, offset);

			/* large slen of 1..BUF_SZ -> (IOV) 1..8 x dlen of 2..4 */
			do_sendrecvv_alignment(len, offset + 1, offset);
		}
	}
}

Test(rdm_sr, sendrecv_alignment)
{
	rdm_sr_xfer_for_each_size(do_sendrecv_alignment, 8*1024, 16*1024);
}

Test(rdm_sr, sendrecv_alignment_retrans)
{
	rdm_sr_err_inject_enable();
	rdm_sr_xfer_for_each_size(do_sendrecv_alignment, 8*1024, 32*1024);
}

Test(rdm_sr, sendvrecv_alignment)
{
	rdm_sr_xfer_for_each_size(do_sendvrecv_alignment_iter, 1, BUF_SZ);
}

Test(rdm_sr, sendrecvv_alignment)
{
	rdm_sr_xfer_for_each_size(do_sendrecvv_alignment_iter, 1, BUF_SZ);
}

Test(rdm_sr_alignment_edge, iov_alignment_edge)
{
	rdm_sr_xfer_for_each_size(do_iov_alignment_edge, 1, BUF_SZ);
}

