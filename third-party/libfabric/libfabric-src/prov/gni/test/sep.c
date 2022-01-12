/*
 * Copyright (c) 2015-2018 Los Alamos National Security, LLC.
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
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"
#include "fi_ext_gni.h"
#include "gnix.h"

#define NUMCONTEXTS 4
#define NUMEPS 2
#define EXTRAEPS 2
#define TOTALEPS (NUMEPS+EXTRAEPS)
#define BUF_SZ (1<<20)
#define IOV_CNT (4)

static struct fid_fabric *fab;
static struct fid_domain *dom[NUMEPS];
static struct fid_av *av[NUMEPS];
static struct fid_av *t_av;
static void *ep_name[TOTALEPS];
static fi_addr_t gni_addr[NUMEPS];
static struct fi_cq_attr cq_attr;
static struct fi_info *hints;
static struct fi_info *fi[NUMEPS];
static struct fid_ep *sep[TOTALEPS];

static char *target, *target_base;
static char *source, *source_base;
static struct iovec *src_iov, *dest_iov;
static char *iov_src_buf, *iov_dest_buf, *iov_src_buf_base, *iov_dest_buf_base;
static struct fid_mr *rem_mr[NUMEPS], *loc_mr[NUMEPS];
static struct fid_mr *iov_dest_buf_mr[NUMEPS], *iov_src_buf_mr[NUMEPS];
static uint64_t mr_key[NUMEPS];

static int ctx_cnt = NUMCONTEXTS;
static int rx_ctx_bits;
static struct fid_ep **tx_ep[NUMEPS], **rx_ep[NUMEPS];
static struct fid_cq **tx_cq[NUMEPS];
static struct fid_cq **rx_cq[NUMEPS];
static fi_addr_t *rx_addr;
static struct fid_cntr *send_cntr[NUMEPS], *recv_cntr[NUMEPS];
static struct fi_cntr_attr cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.flags = 0
};
static struct fi_tx_attr tx_attr;
static struct fi_rx_attr rx_attr;

static uint64_t sends[NUMEPS] = {0}, recvs[NUMEPS] = {0},
	send_errs[NUMEPS] = {0}, recv_errs[NUMEPS] = {0};

void sep_setup_common(int av_type, uint32_t version, int mr_mode)
{
	int ret, i, j;
	struct fi_av_attr av_attr = {0};
	size_t addrlen = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");
	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_RMA_EVENT | FI_ATOMIC | FI_RMA | FI_MSG |
		      FI_NAMED_RX_CTX | FI_TAGGED;
	hints->mode = FI_LOCAL_MR;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->mr_mode = mr_mode;
	hints->fabric_attr->prov_name = strdup("gni");
	hints->ep_attr->tx_ctx_cnt = ctx_cnt;
	hints->ep_attr->rx_ctx_cnt = ctx_cnt;

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_getinfo(version, NULL, 0, 0, hints, &fi[i]);
		cr_assert(!ret, "fi_getinfo");

		tx_cq[i] = calloc(ctx_cnt, sizeof(*tx_cq));
		rx_cq[i] = calloc(ctx_cnt, sizeof(*rx_cq));
		tx_ep[i] = calloc(ctx_cnt, sizeof(*tx_ep));
		rx_ep[i] = calloc(ctx_cnt, sizeof(*rx_ep));
		if (!tx_cq[i] || !tx_cq[i] ||
		    !tx_ep[i] || !rx_ep[i]) {
			cr_assert(0, "calloc");
		}
	}

	ctx_cnt = MIN(ctx_cnt, fi[0]->domain_attr->rx_ctx_cnt);
	ctx_cnt = MIN(ctx_cnt, fi[0]->domain_attr->tx_ctx_cnt);
	cr_assert(ctx_cnt, "ctx_cnt is 0");

	ret = fi_fabric(fi[0]->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	rx_ctx_bits = 0;
	while (ctx_cnt >> ++rx_ctx_bits);
	av_attr.rx_ctx_bits = rx_ctx_bits;
	av_attr.type = av_type;
	av_attr.count = NUMEPS;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = FI_WAIT_NONE;

	rx_addr = calloc(ctx_cnt, sizeof(*rx_addr));

	target_base = calloc(GNIT_ALIGN_LEN(BUF_SZ), 1);
	source_base = calloc(GNIT_ALIGN_LEN(BUF_SZ), 1);

	iov_src_buf_base = malloc(GNIT_ALIGN_LEN(BUF_SZ) * IOV_CNT);
	iov_dest_buf_base = malloc(GNIT_ALIGN_LEN(BUF_SZ) * IOV_CNT);
	src_iov = malloc(sizeof(struct iovec) * IOV_CNT);
	dest_iov = malloc(sizeof(struct iovec) * IOV_CNT);

	if (!rx_addr || !target_base || !source_base ||
		!iov_src_buf_base || !iov_dest_buf_base ||
		!src_iov || !dest_iov) {
		cr_assert(0, "allocation");
	}

	target = GNIT_ALIGN_BUFFER(char *, target_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);
	iov_src_buf = GNIT_ALIGN_BUFFER(char *, iov_src_buf_base);
	iov_dest_buf = GNIT_ALIGN_BUFFER(char *, iov_dest_buf_base);

	for (i = 0; i < IOV_CNT; i++) {
		src_iov[i].iov_base = malloc(BUF_SZ);
		assert(src_iov[i].iov_base != NULL);

		dest_iov[i].iov_base = malloc(BUF_SZ * 3);
		assert(dest_iov[i].iov_base != NULL);
	}

	for (i = 0; i < NUMEPS; i++) {
		fi[i]->ep_attr->tx_ctx_cnt = ctx_cnt;
		fi[i]->ep_attr->rx_ctx_cnt = ctx_cnt;

		ret = fi_domain(fab, fi[i], &dom[i], NULL);
		cr_assert(!ret, "fi_domain");

		ret = fi_scalable_ep(dom[i], fi[i], &sep[i], NULL);
		cr_assert(!ret, "fi_scalable_ep");

		ret = fi_av_open(dom[i], &av_attr, &av[i], NULL);
		cr_assert(!ret, "fi_av_open");

		ret = fi_cntr_open(dom[i], &cntr_attr, &send_cntr[i], 0);
		cr_assert(!ret, "fi_cntr_open");

		ret = fi_cntr_open(dom[i], &cntr_attr, &recv_cntr[i], 0);
		cr_assert(!ret, "fi_cntr_open");

		for (j = 0; j < ctx_cnt; j++) {
			ret = fi_tx_context(sep[i], j, NULL, &tx_ep[i][j],
					    NULL);
			cr_assert(!ret, "fi_tx_context");

			ret = fi_cq_open(dom[i], &cq_attr, &tx_cq[i][j],
					 NULL);
			cr_assert(!ret, "fi_cq_open");

			ret = fi_rx_context(sep[i], j, NULL, &rx_ep[i][j],
					    NULL);
			cr_assert(!ret, "fi_rx_context");

			ret = fi_cq_open(dom[i], &cq_attr, &rx_cq[i][j],
					 NULL);
			cr_assert(!ret, "fi_cq_open");
		}

		ret = fi_scalable_ep_bind(sep[i], &av[i]->fid, 0);
		cr_assert(!ret, "fi_scalable_ep_bind");

		for (j = 0; j < ctx_cnt; j++) {
			ret = fi_ep_bind(tx_ep[i][j], &tx_cq[i][j]->fid,
					 FI_TRANSMIT);
			cr_assert(!ret, "fi_ep_bind");

			ret = fi_ep_bind(tx_ep[i][j], &send_cntr[i]->fid,
					 FI_SEND | FI_WRITE);
			cr_assert(!ret, "fi_ep_bind");

			ret = fi_enable(tx_ep[i][j]);
			cr_assert(!ret, "fi_enable");

			ret = fi_ep_bind(rx_ep[i][j], &rx_cq[i][j]->fid,
					 FI_RECV);
			cr_assert(!ret, "fi_ep_bind");

			ret = fi_ep_bind(rx_ep[i][j], &recv_cntr[i]->fid,
					 FI_RECV | FI_READ);
			cr_assert(!ret, "fi_ep_bind");

			ret = fi_enable(rx_ep[i][j]);
			cr_assert(!ret, "fi_enable");

		}
	}

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_enable(sep[i]);
		cr_assert(!ret, "fi_enable");

		ret = fi_getname(&sep[i]->fid, NULL, &addrlen);
		cr_assert(addrlen > 0);

		ep_name[i] = malloc(addrlen);
		cr_assert(ep_name[i] != NULL);

		ret = fi_getname(&sep[i]->fid, ep_name[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);

		ret = fi_mr_reg(dom[i],
				target,
				BUF_SZ,
				FI_REMOTE_WRITE,
				0,
				(USING_SCALABLE(fi[i]) ? (i * 4) : 0),
				0,
				&rem_mr[i],
				&target);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i],
				source,
				BUF_SZ,
				FI_REMOTE_WRITE,
				0,
				(USING_SCALABLE(fi[i]) ? (i * 4) + 1 : 0),
				0,
				&loc_mr[i],
				&source);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi[i])) {
			MR_ENABLE(rem_mr[i], target, BUF_SZ);
			MR_ENABLE(loc_mr[i], source, BUF_SZ);
		}

		mr_key[i] = fi_mr_key(rem_mr[i]);

		ret = fi_mr_reg(dom[i],
				iov_dest_buf,
				IOV_CNT * BUF_SZ,
				FI_REMOTE_WRITE,
				0,
				(USING_SCALABLE(fi[i]) ? (i * 4) + 2 : 0),
				0,
				iov_dest_buf_mr + i,
				&iov_dest_buf);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i],
				iov_src_buf,
				IOV_CNT * BUF_SZ,
				FI_REMOTE_WRITE,
				0,
				(USING_SCALABLE(fi[i]) ? (i * 4) + 3 : 0),
				0,
				iov_src_buf_mr + i,
				&iov_src_buf);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi[i])) {
			MR_ENABLE(iov_dest_buf_mr[i],
				iov_dest_buf,
				IOV_CNT * BUF_SZ);
			MR_ENABLE(iov_src_buf_mr[i],
				iov_src_buf,
				IOV_CNT * BUF_SZ);
		}
	}

	for (i = 0; i < NUMEPS; i++) {
		for (j = 0; j < NUMEPS; j++) {
			ret = fi_av_insert(av[i], ep_name[j], 1, &gni_addr[j],
					   0, NULL);
			cr_assert(ret == 1);
		}
	}

	for (i = 0; i < ctx_cnt; i++) {
		rx_addr[i] = fi_rx_addr(gni_addr[1], i, rx_ctx_bits);
	}
}

void sep_basic_setup_map(void)
{
	sep_setup_common(FI_AV_MAP, fi_version(), GNIX_MR_BASIC);
}

void sep_scalable_setup_map(void)
{
	sep_setup_common(FI_AV_MAP, fi_version(), GNIX_MR_SCALABLE);
}

void sep_default_setup_map(void)
{
	sep_setup_common(FI_AV_MAP, fi_version(), GNIX_DEFAULT_MR_MODE);
}

void sep_basic_setup_table(void)
{
	sep_setup_common(FI_AV_TABLE, fi_version(), GNIX_MR_BASIC);
}

void sep_scalable_setup_table(void)
{
	sep_setup_common(FI_AV_TABLE, fi_version(), GNIX_MR_SCALABLE);
}

void sep_default_setup_table(void)
{
	sep_setup_common(FI_AV_TABLE, fi_version(), GNIX_DEFAULT_MR_MODE);
}

static void sep_teardown(void)
{
	int ret, i, j;

	for (i = 0; i < NUMEPS; i++) {
		fi_close(&recv_cntr[i]->fid);
		fi_close(&send_cntr[i]->fid);

		for (j = 0; j < ctx_cnt; j++) {
			ret = fi_close(&tx_ep[i][j]->fid);
			cr_assert(!ret, "failure closing tx_ep.");

			ret = fi_close(&rx_ep[i][j]->fid);
			cr_assert(!ret, "failure closing rx_ep.");

			ret = fi_close(&tx_cq[i][j]->fid);
			cr_assert(!ret, "failure closing tx cq.");

			ret = fi_close(&rx_cq[i][j]->fid);
			cr_assert(!ret, "failure closing rx cq.");
		}

		ret = fi_close(&sep[i]->fid);
		cr_assert(!ret, "failure in closing ep.");

		ret = fi_close(&av[i]->fid);
		cr_assert(!ret, "failure in closing av.");

		fi_close(&loc_mr[i]->fid);
		fi_close(&rem_mr[i]->fid);

		ret = fi_close(&dom[i]->fid);
		cr_assert(!ret, "failure in closing domain.");

		free(tx_ep[i]);
		free(rx_ep[i]);
		free(ep_name[i]);
		fi_freeinfo(fi[i]);
	}

	for (i = 0; i < IOV_CNT; i++) {
		free(src_iov[i].iov_base);
		free(dest_iov[i].iov_base);
	}

	free(src_iov);
	free(dest_iov);
	free(iov_src_buf_base);
	free(iov_dest_buf_base);

	fi_freeinfo(hints);
	free(target_base);
	free(source_base);

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");
}

void sep_setup_context(void)
{
	int ret;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");
	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_ATOMIC | FI_RMA | FI_MSG | FI_NAMED_RX_CTX | FI_TAGGED;
	hints->mode = FI_LOCAL_MR;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->fabric_attr->prov_name = strdup("gni");

	hints->ep_attr->tx_ctx_cnt = 0;
	hints->ep_attr->rx_ctx_cnt = 0;

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[0]);
	cr_assert(!ret, "fi_getinfo");
	cr_assert_eq(fi[0]->ep_attr->tx_ctx_cnt, 1, "incorrect tx_ctx_cnt");
	cr_assert_eq(fi[0]->ep_attr->rx_ctx_cnt, 1, "incorrect rx_ctx_cnt");

	hints->ep_attr->tx_ctx_cnt = ctx_cnt;
	hints->ep_attr->rx_ctx_cnt = ctx_cnt;

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[0]);
	cr_assert(!ret, "fi_getinfo");

	tx_ep[0] = calloc(ctx_cnt, sizeof(*tx_ep));
	rx_ep[0] = calloc(ctx_cnt, sizeof(*rx_ep));
	if (!tx_ep[0] || !rx_ep[0]) {
		cr_assert(0, "calloc");
	}

	ctx_cnt = MIN(ctx_cnt, fi[0]->domain_attr->rx_ctx_cnt);
	ctx_cnt = MIN(ctx_cnt, fi[0]->domain_attr->tx_ctx_cnt);
	cr_assert(ctx_cnt, "ctx_cnt is 0");

	ret = fi_fabric(fi[0]->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	fi[0]->ep_attr->tx_ctx_cnt = ctx_cnt;
	fi[0]->ep_attr->rx_ctx_cnt = ctx_cnt;

	ret = fi_domain(fab, fi[0], &dom[0], NULL);
	cr_assert(!ret, "fi_domain");

	ret = fi_scalable_ep(dom[0], fi[0], &sep[0], NULL);
	cr_assert(!ret, "fi_scalable_ep");

	/* add bits to check failure path */
	tx_attr.mode = FI_RESTRICTED_COMP;
	ret = fi_tx_context(sep[0], 0, &tx_attr, &tx_ep[0][0],
			    NULL);
	tx_attr.mode = 0;
	tx_attr.caps = FI_MULTICAST;
	ret = fi_tx_context(sep[0], 0, &tx_attr, &tx_ep[0][0],
			    NULL);
	cr_assert(-FI_EINVAL, "fi_tx_context");

	rx_attr.caps = FI_MULTICAST;
	ret = fi_rx_context(sep[0], 0, &rx_attr, &rx_ep[0][0],
			    NULL);
	cr_assert(-FI_EINVAL, "fi_rx_context");

	rx_attr.caps = 0;
	rx_attr.mode = FI_RESTRICTED_COMP;
	ret = fi_rx_context(sep[0], 0, &rx_attr, &rx_ep[0][0],
			    NULL);
	cr_assert(-FI_EINVAL, "fi_rx_context");
}

static void sep_teardown_context(void)
{
	int ret;

	ret = fi_close(&sep[0]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&dom[0]->fid);
	cr_assert(!ret, "failure in closing domain.");

	free(tx_ep[0]);
	free(rx_ep[0]);
	free(ep_name[0]);
	fi_freeinfo(fi[0]);

	fi_freeinfo(hints);

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");
}

static void
sep_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = seed++;
}

static int
sep_check_data(char *buf1, char *buf2, int len)
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

static void
wait_for_cqs(struct fid_cq *scq, struct fid_cq *dcq,
		struct fi_cq_tagged_entry *scqe,
		struct fi_cq_tagged_entry *dcqe)
{
	int ret;
	int s_done = 0, d_done = 0;

	do {
		ret = fi_cq_read(scq, scqe, 1);
		if (ret == 1) {
			s_done = 1;
		}

		ret = fi_cq_read(dcq, dcqe, 1);
		if (ret == 1) {
			d_done = 1;
		}
	} while (!(s_done && d_done));
}

static void
xfer_each_size(void (*xfer)(int index, int len), int index, int slen, int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2) {
		xfer(index, i);
	}
}

static void
sep_check_cqe(struct fi_cq_tagged_entry *cqe, void *ctx,
		uint64_t flags, void *addr, size_t len,
		uint64_t data, bool buf_is_non_null, uint64_t tag,
		struct fid_ep *fid_ep)
{
	struct gnix_fid_ep *gnix_ep = get_gnix_ep(fid_ep);
	cr_assert(cqe->op_context == ctx, "CQE Context mismatch");
	cr_assert(cqe->flags == flags,
		  "CQE flags mismatch cqe flags:0x%lx, flags:0x%lx", cqe->flags,
		  flags);

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

	cr_assert(cqe->tag == tag, "Invalid CQE tag:0x%lx, tag:0x%lx",
		  cqe->tag, tag);
}

static void
sep_check_tcqe(struct fi_cq_tagged_entry *tcqe, void *ctx,
	       uint64_t flags, uint64_t data, struct fid_ep *fid_ep)
{
	struct gnix_fid_ep *gnix_ep = get_gnix_ep(fid_ep);

	cr_assert(tcqe->op_context == ctx, "CQE Context mismatch");
	cr_assert(tcqe->flags == flags, "CQE flags mismatch");

	/* TODO: Remove GNIX_ALLOW_FI_REMOTE_CQ_DATA and only check flags for FI_RMA_EVENT */
	if (GNIX_ALLOW_FI_REMOTE_CQ_DATA(flags, gnix_ep->caps)) {
		cr_assert(tcqe->data == data, "CQE data invalid");
	} else {
		cr_assert(tcqe->data == 0, "CQE data invalid");
	}

	cr_assert(tcqe->len == 0, "CQE length mismatch");
	cr_assert(tcqe->buf == 0, "CQE address mismatch");
	cr_assert(tcqe->tag == 0, "CQE tag invalid");
}

static void
sep_check_cntrs(uint64_t s[], uint64_t r[], uint64_t s_e[],
		uint64_t r_e[], bool need_to_spin)
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
			  "Bad send count i:%d send_cntr:%ld sends:%ld",
			  i, fi_cntr_read(send_cntr[i]), sends[i]);

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

static int
sep_check_iov_data(struct iovec *iov_buf, char *buf, size_t cnt, size_t buf_len)
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
			       " iov_buf: %hhx, buf: %hhx\n", i, j,
			       iov_buf[i].iov_len,
			       ((char *)iov_buf[i].iov_base)[iov_idx],
			       buf[j]);
			return 0;
		}
	}

	return 1;
}

static int
check_iov_data(struct iovec *ib, struct iovec *ob, size_t cnt)
{
	size_t i;

	for (i = 0; i < cnt; i++) {
		if (memcmp(ib[i].iov_base, ob[i].iov_base, ib[i].iov_len)) {
			printf("data mismatch, ib:%x ob:%x\n",
			       *(char *)ib[i].iov_base,
			       *(char *)ob[i].iov_base);
			return 0;
		}
	}

	return 1;
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

static void sep_send_recv(int index, int len)
{
	ssize_t ret;
	struct fi_cq_tagged_entry cqe;

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	ret = fi_send(tx_ep[0][index], source, len, loc_mr[0],
		      rx_addr[index], target);
	cr_assert(ret == 0, "fi_send failed err:%ld", ret);

	ret = fi_recv(rx_ep[1][index], target, len, rem_mr[0],
		      FI_ADDR_UNSPEC, source);
	cr_assert(ret == 0, "fi_recv failed err:%ld", ret);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &cqe, &cqe);

	ret = sep_check_data(source, target, 8);
	cr_assert(ret == 1, "Data check failed");
}

static void sep_tsend(int index, int len)
{
	ssize_t ret;
	struct fi_cq_tagged_entry cqe;

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	ret = fi_tsend(tx_ep[0][index], source, len, loc_mr[0],
		       rx_addr[index], len, target);
	cr_assert(ret == 0, "fi_tsend failed err:%ld", ret);

	ret = fi_trecv(rx_ep[1][index], target, len, rem_mr[0],
		       FI_ADDR_UNSPEC, len, 0, source);
	cr_assert(ret == 0, "fi_trecv failed err:%ld", ret);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &cqe, &cqe);

	ret = sep_check_data(source, target, 8);
	cr_assert(ret == 1, "Data check failed");
}

static void sep_recvmsg(int index, int len)
{
	ssize_t ret;
	struct iovec iov;
	struct fi_msg msg;
	struct fi_cq_tagged_entry cqe;

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	ret = fi_send(tx_ep[0][index], source, len, loc_mr[0],
		      rx_addr[index], target);
	cr_assert(ret == 0, "fi_send failed err:%ld", ret);

	iov.iov_base = target;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)rem_mr;
	msg.iov_count = 1;
	msg.addr = FI_ADDR_UNSPEC;
	msg.context = source;
	msg.data = (uint64_t)source;

	ret = fi_recvmsg(rx_ep[1][index], &msg, 0);
	cr_assert(ret == 0, "fi_recvmsg failed err:%ld", ret);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &cqe, &cqe);

	ret = sep_check_data(source, target, 8);
	cr_assert(ret == 1, "Data check failed");
}

static void sep_trecvmsg(int index, int len)
{
	ssize_t ret;
	struct iovec iov;
	struct fi_msg_tagged tmsg;
	struct fi_cq_tagged_entry cqe;

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	ret = fi_tsend(tx_ep[0][index], source, len, loc_mr[0],
		       rx_addr[index], len, target);
	cr_assert(ret == 0, "fi_send failed err:%ld", ret);

	iov.iov_base = target;
	iov.iov_len = len;

	tmsg.msg_iov = &iov;
	tmsg.desc = (void **)rem_mr;
	tmsg.iov_count = 1;
	tmsg.addr = FI_ADDR_UNSPEC;
	tmsg.context = source;
	tmsg.data = (uint64_t)source;
	tmsg.tag = (uint64_t)len;
	tmsg.ignore = 0;

	ret = fi_trecvmsg(rx_ep[1][index], &tmsg, 0);
	cr_assert(ret == 0, "fi_recvmsg failed err:%ld", ret);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &cqe, &cqe);

	ret = sep_check_data(source, target, 8);
	cr_assert(ret == 1, "Data check failed");
}

static void sep_sendv(int index, int len)
{
	int i, iov_cnt;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			sep_init_data(src_iov[i].iov_base, len, 0x25);
			src_iov[i].iov_len = len;
		}
		sep_init_data(iov_dest_buf, len * iov_cnt, 0);

		sz = fi_sendv(tx_ep[0][index], src_iov, NULL, iov_cnt,
				rx_addr[index], iov_dest_buf);
		cr_assert_eq(sz, 0);

		sz = fi_recv(rx_ep[1][index], iov_dest_buf, len * iov_cnt,
			     iov_dest_buf_mr[0], FI_ADDR_UNSPEC, src_iov);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

		wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
		sep_check_cqe(&s_cqe, iov_dest_buf, (FI_MSG|FI_SEND),
				 0, 0, 0, false, 0, tx_ep[0][index]);
		sep_check_cqe(&d_cqe, src_iov, (FI_MSG|FI_RECV), iov_dest_buf,
				 len * iov_cnt, 0, false, 0, rx_ep[1][index]);

		s[0] = 1; r[1] = 1;
		sep_check_cntrs(s, r, s_e, r_e, false);
		cr_assert(sep_check_iov_data(src_iov, iov_dest_buf, iov_cnt,
			len * iov_cnt), "Data mismatch");
	}
}

static void sep_tsendv(int index, int len)
{
	int i, iov_cnt;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	ssize_t sz;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			sep_init_data(src_iov[i].iov_base, len, 0x25);
			src_iov[i].iov_len = len;
		}
		sep_init_data(iov_dest_buf, len * iov_cnt, 0);

		sz = fi_tsendv(tx_ep[0][index], src_iov, NULL, iov_cnt,
			       rx_addr[index], len * iov_cnt, iov_dest_buf);
		cr_assert_eq(sz, 0);

		sz = fi_trecv(rx_ep[1][index], iov_dest_buf, len * iov_cnt,
			      iov_dest_buf_mr[0], FI_ADDR_UNSPEC, len * iov_cnt,
			      0, src_iov);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

		wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
		sep_check_cqe(&s_cqe, iov_dest_buf, (FI_MSG|FI_SEND|FI_TAGGED),
				 0, 0, 0, false, 0, tx_ep[0][index]);
		sep_check_cqe(&d_cqe, src_iov, (FI_MSG|FI_RECV|FI_TAGGED),
			      iov_dest_buf, len * iov_cnt, 0, false,
			      len * iov_cnt, rx_ep[1][index]);

		s[0] = 1; r[1] = 1;
		sep_check_cntrs(s, r, s_e, r_e, false);
		cr_assert(sep_check_iov_data(src_iov, iov_dest_buf, iov_cnt,
			len * iov_cnt), "Data mismatch");
	}
}

static void sep_recvv(int index, int len)
{
	int i, iov_cnt;
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe, d_cqe;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	for (iov_cnt = 1; iov_cnt <= IOV_CNT; iov_cnt++) {
		for (i = 0; i < iov_cnt; i++) {
			sep_init_data(src_iov[i].iov_base, len, 0x25 + index);
			src_iov[i].iov_len = len;
		}

		for (i = 0; i < iov_cnt; i++) {
			sep_init_data(dest_iov[i].iov_base, len, 0);
			dest_iov[i].iov_len = len;
		}

		sz = fi_sendv(tx_ep[0][index], src_iov, NULL, iov_cnt,
				rx_addr[index], iov_dest_buf);
		cr_assert_eq(sz, 0);

		sz = fi_recvv(rx_ep[1][index], dest_iov, NULL, iov_cnt,
				FI_ADDR_UNSPEC, iov_src_buf);
		cr_assert_eq(sz, 0);

		/* reset cqe */
		s_cqe.op_context = s_cqe.buf = (void *) -1;
		s_cqe.flags = s_cqe.len = s_cqe.data = s_cqe.tag = UINT_MAX;
		d_cqe.op_context = d_cqe.buf = (void *) -1;
		d_cqe.flags = d_cqe.len = d_cqe.data = d_cqe.tag = UINT_MAX;

		wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
		sep_check_cqe(&s_cqe, iov_dest_buf, (FI_MSG|FI_SEND),
				 0, 0, 0, false, 0, tx_ep[0][index]);
		sep_check_cqe(&d_cqe, iov_src_buf, (FI_MSG|FI_RECV),
				iov_dest_buf, len * iov_cnt, 0, false, 0,
			      rx_ep[1][index]);

		s[0] = 1; r[1] = 1;
		sep_check_cntrs(s, r, s_e, r_e, false);
		cr_assert(check_iov_data(src_iov, dest_iov, iov_cnt),
			  "Data mismatch");
	}
}

static void _sendmsg(int index, int len, bool tagged)
{
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_msg msg;
	struct fi_msg_tagged tmsg;
	struct iovec iov;
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};
	uint64_t sflags = FI_MSG|FI_SEND|(tagged ? FI_TAGGED : 0);
	uint64_t dflags = FI_MSG|FI_RECV|(tagged ? FI_TAGGED : 0);

	iov.iov_base = source;
	iov.iov_len = len;

	msg.msg_iov = &iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = rx_addr[index];
	msg.context = target;
	msg.data = (uint64_t)target;

	tmsg.msg_iov = &iov;
	tmsg.desc = (void **)loc_mr;
	tmsg.iov_count = 1;
	tmsg.addr = rx_addr[index];
	tmsg.context = target;
	tmsg.data = (uint64_t)target;
	tmsg.tag = (uint64_t)len;
	tmsg.ignore = 0;

	sep_init_data(source, len, 0xd0 + index);
	sep_init_data(target, len, 0);

	if (tagged) {
		sz = fi_tsendmsg(tx_ep[0][index], &tmsg, 0);
	} else {
		sz = fi_sendmsg(tx_ep[0][index], &msg, 0);
	}
	cr_assert_eq(sz, 0);

	if (tagged) {
		sz = fi_trecv(rx_ep[1][index], target, len, rem_mr[0],
			      FI_ADDR_UNSPEC, len, 0, source);
	} else {
		sz = fi_recv(rx_ep[1][index], target, len, rem_mr[0],
			     FI_ADDR_UNSPEC, source);
	}
	cr_assert_eq(sz, 0);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
	sep_check_cqe(&s_cqe, target, sflags, 0, 0, 0, false, 0,
		      tx_ep[0][index]);
	sep_check_cqe(&d_cqe, source, dflags, target, len, 0,
			false, tagged ? len : 0, rx_ep[1][index]);

	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

static void sep_sendmsg(int index, int len)
{
	_sendmsg(index, len, false);
}

static void sep_tsendmsg(int index, int len)
{
	_sendmsg(index, len, true);
}

void sep_clear_counters(void)
{
	int i, ret;

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_cntr_set(send_cntr[i], 0);
		cr_assert(!ret, "fi_cntr_set");
		ret = fi_cntr_set(recv_cntr[i], 0);
		cr_assert(!ret, "fi_cntr_set");
		sends[i] = 0;
		recvs[i] = 0;
		send_errs[i] = 0;
		recv_errs[i] = 0;
	}
}

void sep_sendmsgdata(int index, int len)
{
	ssize_t sz;
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
	msg.addr = rx_addr[index];
	msg.context = target;
	msg.data = (uint64_t)source;

	sep_init_data(source, len, 0xe0 + index);
	sep_init_data(target, len, 0);

	sz = fi_sendmsg(tx_ep[0][index], &msg, FI_REMOTE_CQ_DATA);
	cr_assert_eq(sz, 0);

	sz = fi_recv(rx_ep[1][index], target, len, rem_mr[0],
		     FI_ADDR_UNSPEC, source);
	cr_assert_eq(sz, 0);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
	sep_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false, 0,
		      tx_ep[0][index]);
	sep_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
		      target, len, (uint64_t)source, false, 0, rx_ep[1][index]);

	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

#define INJECT_SIZE 64
void sep_inject(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sep_init_data(source, len, 0x13 + index);
	sep_init_data(target, len, 0);

	sz = fi_inject(tx_ep[0][index], source, len, rx_addr[index]);
	cr_assert_eq(sz, 0);

	sz = fi_recv(rx_ep[1][index], target, len, rem_mr[1],
		     FI_ADDR_UNSPEC, source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(rx_cq[1][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
		/* Manually progress connection to domain 1 */
		fi_cq_read(tx_cq[0][index], &cqe, 1);
	}

	cr_assert_eq(ret, 1);
	sep_check_cqe(&cqe, source, (FI_MSG|FI_RECV),
			 target, len, (uint64_t)source, false, 0,
		      rx_ep[1][index]);

	/* do progress until send counter is updated */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}
	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, true);

	/* make sure inject does not generate a send competion */
	cr_assert_eq(fi_cq_read(tx_cq[0][index], &cqe, 1), -FI_EAGAIN);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_tinject(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sep_init_data(source, len, 0x13 + index);
	sep_init_data(target, len, 0);

	sz = fi_tinject(tx_ep[0][index], source, len, rx_addr[index], len);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(rx_ep[1][index], target, len, rem_mr[1],
		     FI_ADDR_UNSPEC, len, 0, source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(rx_cq[1][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
		/* Manually progress connection to domain 1 */
		fi_cq_read(tx_cq[0][index], &cqe, 1);
	}

	cr_assert_eq(ret, 1);
	sep_check_cqe(&cqe, source, (FI_MSG|FI_RECV|FI_TAGGED),
			 target, len, (uint64_t)source, false, len,
		      rx_ep[1][index]);

	/* do progress until send counter is updated */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}
	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, true);

	/* make sure inject does not generate a send competion */
	cr_assert_eq(fi_cq_read(tx_cq[0][index], &cqe, 1), -FI_EAGAIN);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_senddata(int index, int len)
{
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	sz = fi_senddata(tx_ep[0][index], source, len, loc_mr[0],
			 (uint64_t)source, rx_addr[index], target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(rx_ep[1][index], target, len, rem_mr[0],
		     FI_ADDR_UNSPEC, source);
	cr_assert_eq(sz, 0);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
	sep_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND), 0, 0, 0, false, 0,
		      tx_ep[0][index]);
	sep_check_cqe(&d_cqe, source, (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)source, false, 0,
		      rx_ep[1][index]);

	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_tsenddata(int index, int len)
{
	ssize_t sz;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	sz = fi_tsenddata(tx_ep[0][index], source, len, loc_mr[0],
			 (uint64_t)source, rx_addr[index], len, target);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(rx_ep[1][index], target, len, rem_mr[0],
		      FI_ADDR_UNSPEC, len, 0, source);
	cr_assert_eq(sz, 0);

	wait_for_cqs(tx_cq[0][index], rx_cq[1][index], &s_cqe, &d_cqe);
	sep_check_cqe(&s_cqe, target, (FI_MSG|FI_SEND|FI_TAGGED), 0, 0, 0,
		      false, 0, tx_ep[0][index]);
	sep_check_cqe(&d_cqe, source,
		      (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA|FI_TAGGED),
		      target, len, (uint64_t)source, false, len,
		      rx_ep[1][index]);

	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_injectdata(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sep_init_data(source, len, 0x9b + index);
	sep_init_data(target, len, 0);

	sz = fi_injectdata(tx_ep[0][index], source, len, (uint64_t)source,
			   rx_addr[index]);
	cr_assert_eq(sz, 0);

	sz = fi_recv(rx_ep[1][index], target, len, rem_mr[0],
		     FI_ADDR_UNSPEC, source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(rx_cq[1][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
		/* Manually progress connection to domain 1 */
		fi_cq_read(tx_cq[0][index], &cqe, 1);
	}
	sep_check_cqe(&cqe, source, (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA),
			 target, len, (uint64_t)source, false, 0,
		      rx_ep[1][index]);

	/* don't progress until send counter is updated */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}

	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, false);

	/* make sure inject does not generate a send competion */
	cr_assert_eq(fi_cq_read(tx_cq[0][index], &cqe, 1), -FI_EAGAIN);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_tinjectdata(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t s[NUMEPS] = {0}, r[NUMEPS] = {0}, s_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	sep_init_data(source, len, 0x9b + index);
	sep_init_data(target, len, 0);

	sz = fi_tinjectdata(tx_ep[0][index], source, len, (uint64_t)source,
			   rx_addr[index], len);
	cr_assert_eq(sz, 0);

	sz = fi_trecv(rx_ep[1][index], target, len, rem_mr[0],
		     FI_ADDR_UNSPEC, len, 0, source);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(rx_cq[1][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
		/* Manually progress connection to domain 1 */
		fi_cq_read(tx_cq[0][index], &cqe, 1);
	}
	sep_check_cqe(&cqe, source,
		      (FI_MSG|FI_RECV|FI_REMOTE_CQ_DATA|FI_TAGGED),
		      target, len, (uint64_t)source, false, len,
		      rx_ep[1][index]);

	/* don't progress until send counter is updated */
	while (fi_cntr_read(send_cntr[0]) < 1) {
		pthread_yield();
	}

	s[0] = 1; r[1] = 1;
	sep_check_cntrs(s, r, s_e, r_e, true);

	/* make sure inject does not generate a send competion */
	cr_assert_eq(fi_cq_read(tx_cq[0][index], &cqe, 1), -FI_EAGAIN);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_read(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};

#define READ_CTX 0x4e3dda1aULL
	sep_init_data(source, len, 0);
	sep_init_data(target, len, 0xad);

	sz = fi_read(tx_ep[0][index], source, len,
			loc_mr[0], rx_addr[index],
			_REM_ADDR(fi[0], target, target),
			mr_key[1], (void *)READ_CTX);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, (void *)READ_CTX, FI_RMA | FI_READ, 0,
			tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_readv(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	struct iovec iov;
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};

	iov.iov_base = source;
	iov.iov_len = len;

	sep_init_data(target, len, 0x25);
	sep_init_data(source, len, 0);
	sz = fi_readv(tx_ep[0][index], &iov, (void **)loc_mr, 1,
			  rx_addr[index],
			  _REM_ADDR(fi[0], target, target), mr_key[1],
			  target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_RMA | FI_READ, 0, tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}


void sep_readmsg(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	struct iovec iov;
	struct fi_msg_rma msg;
	struct fi_rma_iov rma_iov;
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};

	iov.iov_base = source;
	iov.iov_len = len;

	rma_iov.addr = _REM_ADDR(fi[0], target, target);
	rma_iov.len = len;
	rma_iov.key = mr_key[1];

	msg.msg_iov = &iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = rx_addr[index];
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;
	msg.context = target;
	msg.data = (uint64_t)target;

	sep_init_data(target, len, 0xe0 + index);
	sep_init_data(source, len, 0);
	sz = fi_readmsg(tx_ep[0][index], &msg, 0);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_RMA | FI_READ, 0, tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_write(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};

	sep_init_data(source, len, 0xab);
	sep_init_data(target, len, 0);

	sz = fi_write(tx_ep[0][index], source, len, loc_mr[0], rx_addr[index],
			  _REM_ADDR(fi[0], target, target), mr_key[1], target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_RMA | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_writev(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	struct iovec iov;
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};

	iov.iov_base = source;
	iov.iov_len = len;

	sep_init_data(source, len, 0x25 + index);
	sep_init_data(target, len, 0);

	sz = fi_writev(tx_ep[0][index], &iov, (void **)loc_mr, 1,
			   gni_addr[1],
			   _REM_ADDR(fi[0], target, target), mr_key[1],
			   target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_RMA | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_writemsg(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	struct iovec iov;
	struct fi_msg_rma msg;
	struct fi_rma_iov rma_iov;
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};

	iov.iov_base = source;
	iov.iov_len = len;

	rma_iov.addr = _REM_ADDR(fi[0], target, target);
	rma_iov.len = len;
	rma_iov.key = mr_key[1];

	msg.msg_iov = &iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;
	msg.context = target;
	msg.data = (uint64_t)target;

	sep_init_data(source, len, 0xe4 + index);
	sep_init_data(target, len, 0);
	sz = fi_writemsg(tx_ep[0][index], &msg, 0);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_RMA | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");
}

void sep_inject_write(int index, int len)
{
	ssize_t sz;
	int ret, i;
	struct fi_cq_tagged_entry cqe;

	sep_init_data(source, len, 0x33);
	sep_init_data(target, len, 0);
	sz = fi_inject_write(tx_ep[0][index], source, len,
				 rx_addr[index],
				 _REM_ADDR(fi[0], target, target), mr_key[1]);
	cr_assert_eq(sz, 0, "fi_inject_write returned %ld (%s)", sz,
		     fi_strerror(-sz));

	for (i = 0; i < len; i++) {
		while (source[i] != target[i]) {
			/* for progress */
			ret = fi_cq_read(tx_cq[0][index], &cqe, 1);
			cr_assert(ret == -FI_EAGAIN || ret == -FI_EAVAIL,
				  "Received unexpected event\n");

			pthread_yield();
		}
	}
}

void sep_writedata(int index, int len)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry dcqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t w[2] = {0}, r[2] = {0}, w_e[2] = {0}, r_e[2] = {0};


#define WRITE_DATA 0x5123da1a145
	sep_init_data(source, len, 0x43 + index);
	sep_init_data(target, len, 0);
	sz = fi_writedata(tx_ep[0][index], source, len, loc_mr[0], WRITE_DATA,
			  rx_addr[index],
			  _REM_ADDR(fi[0], target, target), mr_key[1],
			  target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_RMA | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	cr_assert(sep_check_data(source, target, len), "Data mismatch");

	while ((ret = fi_cq_read(rx_cq[1][index], &dcqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}
	cr_assert(ret != FI_SUCCESS, "Missing remote data");

	sep_check_tcqe(&dcqe, NULL,
		       (FI_RMA | FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA),
		       WRITE_DATA, rx_ep[1][index]);
}

#define INJECTWRITE_DATA 0xdededadadeadbeaf
void sep_inject_writedata(int index, int len)
{
	ssize_t sz;
	int ret, i;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry dcqe = { (void *) -1, UINT_MAX, UINT_MAX,
					   (void *) -1, UINT_MAX, UINT_MAX };

	sep_init_data(source, len, 0x53 + index);
	sep_init_data(target, len, 0);
	sz = fi_inject_writedata(tx_ep[0][index], source, len, INJECTWRITE_DATA,
				 rx_addr[index],
				 _REM_ADDR(fi[0], target, target), mr_key[1]);
	cr_assert_eq(sz, 0);

	for (i = 0; i < len; i++) {
		while (source[i] != target[i]) {
			/* for progress */
			ret = fi_cq_read(tx_cq[0][index], &cqe, 1);
			cr_assert(ret == -FI_EAGAIN || ret == -FI_EAVAIL,
				  "Received unexpected event\n");

			pthread_yield();
		}
	}

	while ((ret = fi_cq_read(rx_cq[1][index], &dcqe, 1)) == -FI_EAGAIN) {
		ret = fi_cq_read(tx_cq[0][index], &cqe, 1); /* for progress */
		pthread_yield();
	}
	cr_assert(ret != FI_SUCCESS, "Missing remote data");

	sep_check_tcqe(&dcqe, NULL,
		      (FI_RMA | FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA),
		      INJECTWRITE_DATA, rx_ep[1][index]);
}

#define SOURCE_DATA	0xBBBB0000CCCCULL
#define TARGET_DATA	0xAAAA0000DDDDULL
void sep_atomic(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	/* u64 */
	*((uint64_t *)source) = SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_atomic(tx_ep[0][index], source, 1,
			loc_mr[0], rx_addr[index],
			_REM_ADDR(fi[0], target, target),
			mr_key[1], FI_UINT64, FI_ATOMIC_WRITE, target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	ret = *((uint64_t *)target) == SOURCE_DATA;
	cr_assert(ret, "Data mismatch");

}

void sep_atomic_v(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t min;
	struct fi_ioc iov;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	iov.addr = source;
	iov.count = 1;

	/* i64 */
	*((int64_t *)source) = SOURCE_DATA;
	*((int64_t *)target) = TARGET_DATA;
	sz = fi_atomicv(tx_ep[0][index], &iov, (void **)loc_mr, 1,
			rx_addr[index],
			_REM_ADDR(fi[0], target, target), mr_key[1],
			FI_INT64, FI_MIN, target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	min = ((int64_t)SOURCE_DATA < (int64_t)TARGET_DATA) ?
		SOURCE_DATA : TARGET_DATA;
	ret = *((int64_t *)target) == min;
	cr_assert(ret, "Data mismatch");
}

#define U32_MASK	0xFFFFFFFFULL
void sep_atomic_msg(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t min;
	struct fi_msg_atomic msg;
	struct fi_ioc msg_iov;
	struct fi_rma_ioc rma_iov;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	msg_iov.addr = source;
	msg_iov.count = 1;
	msg.msg_iov = &msg_iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	rma_iov.addr = _REM_ADDR(fi[0], target, target);
	rma_iov.count = 1;
	rma_iov.key = mr_key[1];
	msg.rma_iov = &rma_iov;
	msg.context = target;
	msg.op = FI_MIN;

	/* i32 */
	*((int64_t *)source) = SOURCE_DATA;
	*((int64_t *)target) = TARGET_DATA;
	msg.datatype = FI_INT32;
	sz = fi_atomicmsg(tx_ep[0][index], &msg, 0);
	cr_assert_eq(sz, 0);

	/* reset cqe */
	cqe.op_context = cqe.buf = (void *) -1;
	cqe.flags = cqe.len = cqe.data = cqe.tag = UINT_MAX;
	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_WRITE, 0, tx_ep[0][index]);

	w[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	min = ((int32_t)SOURCE_DATA < (int32_t)TARGET_DATA) ?
		SOURCE_DATA : TARGET_DATA;
	min = (min & U32_MASK) | (TARGET_DATA & (U32_MASK << 32));
	ret = *((int64_t *)target) == min;
	cr_assert(ret, "Data mismatch");
}

void sep_atomic_inject(int index)
{
	int ret, loops;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe;
	uint64_t min;

	/* i64 */
	*((int64_t *)source) = SOURCE_DATA;
	*((int64_t *)target) = TARGET_DATA;
	sz = fi_inject_atomic(tx_ep[0][index], source, 1,
				  gni_addr[1],
				  _REM_ADDR(fi[0], target, target), mr_key[1],
				  FI_INT64, FI_MIN);
	cr_assert_eq(sz, 0);

	min = ((int64_t)SOURCE_DATA < (int64_t)TARGET_DATA) ?
		SOURCE_DATA : TARGET_DATA;
	loops = 0;
	while (*((int64_t *)target) != min) {
		ret = fi_cq_read(tx_cq[0][index], &cqe, 1); /* for progress */
		cr_assert(ret == -FI_EAGAIN,
			  "Received unexpected event\n");

		pthread_yield();
		cr_assert(++loops < 10000, "Data mismatch");
	}
}

#define FETCH_SOURCE_DATA	0xACEDACEDULL
void sep_atomic_rw(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t operand = SOURCE_DATA;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	/* u64 */
	*((uint64_t *)source) = FETCH_SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_fetch_atomic(tx_ep[0][index], &operand, 1, NULL, source,
				 loc_mr[0], rx_addr[index],
				 _REM_ADDR(fi[0], target, target),
				 mr_key[1], FI_UINT64, FI_SUM, target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_READ, 0, tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	ret = *((uint64_t *)target) == (SOURCE_DATA + TARGET_DATA);
	cr_assert(ret, "Data mismatch");
	ret = *((uint64_t *)source) == TARGET_DATA;
	cr_assert(ret, "Fetch data mismatch");
}

void sep_atomic_rwv(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t min;
	uint64_t operand = SOURCE_DATA;
	struct fi_ioc iov, r_iov;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	iov.count = 1;
	r_iov.count = 1;

	/* i64 */
	*((int64_t *)source) = FETCH_SOURCE_DATA;
	*((int64_t *)target) = TARGET_DATA;
	iov.addr = &operand;
	r_iov.addr = source;
	sz = fi_fetch_atomicv(tx_ep[0][index], &iov, NULL, 1,
				  &r_iov, (void **)loc_mr, 1,
				  gni_addr[1],
				  _REM_ADDR(fi[0], target, target), mr_key[1],
				  FI_INT64, FI_MIN, target);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_READ, 0, tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	min = ((int64_t)SOURCE_DATA < (int64_t)TARGET_DATA) ?
		SOURCE_DATA : TARGET_DATA;
	ret = *((int64_t *)target) == min;
	cr_assert(ret, "Data mismatch");
	ret = *((int64_t *)source) == TARGET_DATA;
	cr_assert(ret, "Fetch data mismatch");
}

void sep_atomic_rwmsg(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t min;
	uint64_t operand = SOURCE_DATA;
	struct fi_msg_atomic msg;
	struct fi_ioc msg_iov, res_iov;
	struct fi_rma_ioc rma_iov;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	msg_iov.count = 1;
	msg.msg_iov = &msg_iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	rma_iov.addr = _REM_ADDR(fi[0], target, target);
	rma_iov.count = 1;
	rma_iov.key = mr_key[1];
	msg.rma_iov = &rma_iov;
	msg.context = target;
	msg.op = FI_MIN;

	res_iov.addr = source;
	res_iov.count = 1;

	/* i64 */
	*((int64_t *)source) = FETCH_SOURCE_DATA;
	*((int64_t *)target) = TARGET_DATA;
	msg_iov.addr = &operand;
	msg.datatype = FI_INT64;
	sz = fi_fetch_atomicmsg(tx_ep[0][index], &msg, &res_iov,
				(void **)loc_mr, 1, 0);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_READ, 0, tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	min = ((int64_t)SOURCE_DATA < (int64_t)TARGET_DATA) ?
		SOURCE_DATA : TARGET_DATA;
	ret = *((int64_t *)target) == min;
	cr_assert(ret, "Data mismatch");
	ret = *((int64_t *)source) == TARGET_DATA;
	cr_assert(ret, "Fetch data mismatch");
}

void sep_atomic_compwrite(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t operand = SOURCE_DATA, op2 = TARGET_DATA;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	/* u64 */
	*((uint64_t *)source) = FETCH_SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	sz = fi_compare_atomic(tx_ep[0][index], &operand, 1, NULL, &op2, NULL,
				   source, loc_mr[0], rx_addr[index],
				   _REM_ADDR(fi[0], target, target),
				   mr_key[1], FI_UINT64,
				   FI_CSWAP, target);
	cr_assert_eq(sz, 0, "fi_compare_atomic returned %ld (%s)", sz,
		     fi_strerror(-sz));

	/* reset cqe */
	cqe.op_context = cqe.buf = (void *) -1;
	cqe.flags = cqe.len = cqe.data = cqe.tag = UINT_MAX;
	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_READ, 0, tx_ep[0][index]);

	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	ret = *((uint64_t *)target) == SOURCE_DATA;
	cr_assert(ret, "Data mismatch");
	ret = *((uint64_t *)source) == TARGET_DATA;
	cr_assert(ret, "Fetch data mismatch");
}

#define SOURCE_DATA_FP	0.83203125
#define TARGET_DATA_FP	0.83984375
void sep_atomic_compwritev(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	double operand_dp, op2_dp;
	struct fi_ioc iov, r_iov, c_iov;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	iov.count = 1;
	r_iov.count = 1;
	c_iov.count = 1;

	/* double */
	*((double *)&operand_dp) = SOURCE_DATA_FP;
	*((double *)&op2_dp) = TARGET_DATA_FP;
	*((double *)source) = FETCH_SOURCE_DATA;
	*((double *)target) = TARGET_DATA_FP;
	iov.addr = &operand_dp;
	r_iov.addr = source;
	c_iov.addr = &op2_dp;
	sz = fi_compare_atomicv(tx_ep[0][index],
				&iov, NULL, 1,
				&c_iov, NULL, 1,
				&r_iov, (void **)loc_mr, 1,
				gni_addr[1],
				_REM_ADDR(fi[0], target, target), mr_key[1],
				FI_DOUBLE, FI_CSWAP, target);
	cr_assert_eq(sz, 0);

	/* reset cqe */
	cqe.op_context = cqe.buf = (void *) -1;
	cqe.flags = cqe.len = cqe.data = cqe.tag = UINT_MAX;

	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_READ, 0, tx_ep[0][index]);
	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	ret = *((double *)target) == (double)SOURCE_DATA_FP;
	cr_assert(ret, "Data mismatch");
	ret = *((double *)source) == (double)TARGET_DATA_FP;
	cr_assert(ret, "Fetch data mismatch");
}

void sep_atomic_compwritemsg(int index)
{
	int ret;
	ssize_t sz;
	struct fi_cq_tagged_entry cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					  (void *) -1, UINT_MAX, UINT_MAX };
	uint64_t operand = SOURCE_DATA, op2 = TARGET_DATA;
	struct fi_msg_atomic msg;
	struct fi_ioc msg_iov, res_iov, cmp_iov;
	struct fi_rma_ioc rma_iov;
	uint64_t w[NUMEPS] = {0}, r[NUMEPS] = {0}, w_e[NUMEPS] = {0};
	uint64_t r_e[NUMEPS] = {0};

	msg_iov.count = 1;
	msg.msg_iov = &msg_iov;
	msg.desc = (void **)loc_mr;
	msg.iov_count = 1;
	msg.addr = gni_addr[1];
	rma_iov.addr = _REM_ADDR(fi[0], target, target);
	rma_iov.count = 1;
	rma_iov.key = mr_key[1];
	msg.rma_iov = &rma_iov;
	msg.context = target;
	msg.op = FI_CSWAP;

	res_iov.count = 1;
	cmp_iov.count = 1;

	/* i64 */
	*((uint64_t *)source) = FETCH_SOURCE_DATA;
	*((uint64_t *)target) = TARGET_DATA;
	msg_iov.addr = &operand;
	msg.datatype = FI_INT64;
	res_iov.addr = source;
	cmp_iov.addr = &op2;
	sz = fi_compare_atomicmsg(tx_ep[0][index], &msg, &cmp_iov, NULL, 1,
				  &res_iov, (void **)loc_mr, 1, 0);
	cr_assert_eq(sz, 0);

	/* reset cqe */
	cqe.op_context = cqe.buf = (void *) -1;
	cqe.flags = cqe.len = cqe.data = cqe.tag = UINT_MAX;
	while ((ret = fi_cq_read(tx_cq[0][index], &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	sep_check_tcqe(&cqe, target, FI_ATOMIC | FI_READ, 0, tx_ep[0][index]);
	r[0] = 1;
	sep_check_cntrs(w, r, w_e, r_e, false);
	ret = *((uint64_t *)target) == SOURCE_DATA;
	cr_assert(ret, "Data mismatch");
	ret = *((uint64_t *)source) == TARGET_DATA;
	cr_assert(ret, "Fetch data mismatch");
}

void sep_invalid_compare_atomic(enum fi_datatype dt, enum fi_op op)
{
	ssize_t sz;
	size_t count;
	uint64_t operand, op2;

	if (!supported_compare_atomic_ops[op][dt]) {
		sz = fi_compare_atomic(tx_ep[0][0], &operand, 1, NULL,
				       &op2, NULL, source, loc_mr,
				       rx_addr[0], (uint64_t)target, mr_key[1],
				       dt, op, target);
		cr_assert(sz == -FI_EOPNOTSUPP);

		sz = fi_compare_atomicvalid(tx_ep[0][0], dt, op, &count);
		cr_assert(sz == -FI_EOPNOTSUPP, "fi_atomicvalid() succeeded\n");
	} else {
		sz = fi_compare_atomicvalid(tx_ep[0][0], dt, op, &count);
		cr_assert(!sz, "fi_atomicvalid() failed\n");
		cr_assert(count == 1, "fi_atomicvalid(): bad count\n");
	}
}

void sep_invalid_fetch_atomic(enum fi_datatype dt, enum fi_op op)
{
	ssize_t sz;
	size_t count;
	uint64_t operand;

	if (!supported_fetch_atomic_ops[op][dt]) {
		sz = fi_fetch_atomic(tx_ep[0][0],
				&operand,
				1,
				NULL,
				source, loc_mr[0],
				rx_addr[0],
				_REM_ADDR(fi[0], target, target),
				mr_key[1],
				dt, op, target);
		cr_assert(sz == -FI_EOPNOTSUPP);

		sz = fi_fetch_atomicvalid(tx_ep[0][0], dt, op, &count);
		cr_assert(sz == -FI_EOPNOTSUPP, "fi_atomicvalid() succeeded\n");
	} else {
		sz = fi_fetch_atomicvalid(tx_ep[0][0], dt, op, &count);
		cr_assert(!sz, "fi_atomicvalid() failed\n");
		cr_assert(count == 1, "fi_atomicvalid(): bad count\n");
	}
}

static void cancel_sep_send_sep(int index)
{
	ssize_t ret, len = 16;
	struct fi_cq_err_entry buf;

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	ret = fi_send(tx_ep[0][index], source, len, loc_mr[0],
		      rx_addr[index], target);
	cr_assert(ret == 0, "fi_send failed err:%ld", ret);

	ret = fi_cancel(&tx_ep[0][index]->fid, target);
	fprintf(stderr, "ret = %ld %s\n", ret, fi_strerror(-ret));
	cr_assert(ret == FI_SUCCESS, "fi_cancel failed");

	/* check for event */
	ret = fi_cq_readerr(tx_cq[0][index], &buf, FI_SEND);
	cr_assert(ret == 1, "did not find one error event");

	cr_assert(buf.buf == (void *) source, "buffer mismatch");
	cr_assert(buf.err == FI_ECANCELED, "error code mismatch");
	cr_assert(buf.prov_errno == FI_ECANCELED, "prov error code mismatch");
	cr_assert(buf.len == len, "length mismatch");
}

static void cancel_sep_recv_sep(int index)
{
	ssize_t ret, len = 16;
	struct fi_cq_err_entry buf;

	sep_init_data(source, len, 0xab + index);
	sep_init_data(target, len, 0);

	ret = fi_recv(rx_ep[1][index], target, len, rem_mr[0],
		      FI_ADDR_UNSPEC, source);
	cr_assert(ret == 0, "fi_recv failed err:%ld", ret);

	ret = fi_cancel(&rx_ep[1][index]->fid, source);
	cr_assert(ret == FI_SUCCESS, "fi_cancel failed");

	/* check for event */
	ret = fi_cq_readerr(rx_cq[1][index], &buf, FI_RECV);
	cr_assert(ret == 1, "did not find one error event");

	cr_assert(buf.buf == (void *) target, "buffer mismatch");
	cr_assert(buf.err == FI_ECANCELED, "error code mismatch");
	cr_assert(buf.prov_errno == FI_ECANCELED, "prov error code mismatch");
	cr_assert(buf.len == len, "length mismatch");
}

static void cancel_sep_no_event(int index)
{
	ssize_t ret;

	ret = fi_cancel(&tx_ep[0][index]->fid, NULL);
	cr_assert(ret == -FI_ENOENT, "fi_cancel failed");

	ret = fi_cancel(&rx_ep[0][index]->fid, NULL);
	cr_assert(ret == -FI_ENOENT, "fi_cancel failed");
}

void run_tests(void)
{
	int i, j;

	cr_log_info("sep_send_recv\n");
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_send_recv, i, 1, BUF_SZ);
	}

	cr_log_info("sep_tsend\n");
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_tsend, i, 1, BUF_SZ);
	}

	cr_log_info("sep_recvmsg\n");
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_recvmsg, i, 1, BUF_SZ);
	}

	cr_log_info("sep_trecvmsg\n");
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_trecvmsg, i, 1, BUF_SZ);
	}

	cr_log_info("sep_sendv\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_sendv, i, 1, BUF_SZ);
	}

	cr_log_info("sep_tsendv\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_tsendv, i, 1, BUF_SZ);
	}

	cr_log_info("sep_recvv\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_recvv, i, 1, BUF_SZ);
	}

	cr_log_info("sep_sendmsg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_sendmsg, i, 1, BUF_SZ);
	}

	cr_log_info("sep_tsendmsg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_tsendmsg, i, 1, BUF_SZ);
	}

	cr_log_info("sep_sendmsgdata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_sendmsgdata, i, 1, BUF_SZ);
	}

	cr_log_info("sep_inject\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_inject, i, 1, INJECT_SIZE);
	}

	cr_log_info("sep_tinject\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_tinject, i, 1, INJECT_SIZE);
	}

	cr_log_info("sep_senddata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_senddata, i, 1, INJECT_SIZE);
	}

	cr_log_info("sep_tsenddata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_tsenddata, i, 1, INJECT_SIZE);
	}

	cr_log_info("sep_injectdata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_injectdata, i, 1, INJECT_SIZE);
	}

	cr_log_info("sep_tinjectdata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_tinjectdata, i, 1, INJECT_SIZE);
	}

	cr_log_info("sep_read\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_read, i, 8, BUF_SZ);
	}

	cr_log_info("sep_readv\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_readv, i, 8, BUF_SZ);
	}

	cr_log_info("sep_readmsg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_readmsg, i, 8, BUF_SZ);
	}

	cr_log_info("sep_write\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_write, i, 8, BUF_SZ);
	}

	cr_log_info("sep_writev\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_writev, i, 8, BUF_SZ);
	}

	cr_log_info("sep_writemsg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_writemsg, i, 8, BUF_SZ);
	}

	cr_log_info("sep_writedata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_writedata, i, 8, BUF_SZ);
	}

	cr_log_info("sep_atomic\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic(i);
	}

	cr_log_info("sep_atomic_v\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_v(i);
	}

	cr_log_info("sep_atomic_msg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_msg(i);
	}

	cr_log_info("sep_atomic_rw\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_rw(i);
	}

	cr_log_info("sep_atomic_rwmsg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_rwmsg(i);
	}

	cr_log_info("sep_atomic_compwrite\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_compwrite(i);
	}

	cr_log_info("sep_atomic_compwritev\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_compwritev(i);
	}

	cr_log_info("sep_atomic_compwritemsg\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_compwritemsg(i);
	}

	cr_log_info("sep_atomic_inject\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		sep_atomic_inject(i);
	}

	cr_log_info("sep_inject_write\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_inject_write, i, 8, INJECT_SIZE);
	}

	cr_log_info("sep_inject_writedata\n");
	sep_clear_counters();
	for (i = 0; i < ctx_cnt; i++) {
		xfer_each_size(sep_inject_writedata, i, 8, INJECT_SIZE);
	}

	cr_log_info("sep_invalid_compare_atomic\n");
	for (i = 0; i < FI_ATOMIC_OP_LAST; i++) {
		for (j = 0; j < FI_DATATYPE_LAST; j++) {
			sep_invalid_compare_atomic(j, i);
		}
	}

	cr_log_info("sep_invalid_fetch_atomic\n");
	for (i = 0; i < FI_ATOMIC_OP_LAST; i++) {
		for (j = 0; j < FI_DATATYPE_LAST; j++) {
			sep_invalid_fetch_atomic(j, i);
		}
	}

}

void run_cancel_tests(void)
{
	int i;

	cr_log_info("cancel send test\n");
	for (i = 0; i < ctx_cnt; i++) {
		cancel_sep_send_sep(i);
	}

	cr_log_info("cancel recv test\n");
	for (i = 0; i < ctx_cnt; i++) {
		cancel_sep_recv_sep(i);
	}

	cr_log_info("cancel no event test\n");
	for (i = 0; i < ctx_cnt; i++) {
		cancel_sep_no_event(i);
	}
}

TestSuite(scalablea,
	.init = sep_setup_context,
	.fini = sep_teardown_context);

TestSuite(scalablem_default,
	.init = sep_default_setup_map,
	.fini = sep_teardown);

TestSuite(scalablet_default,
	.init = sep_default_setup_table,
	.fini = sep_teardown);

TestSuite(scalablem_basic,
	.init = sep_basic_setup_map,
	.fini = sep_teardown);

TestSuite(scalablet_basic,
	.init = sep_basic_setup_table,
	.fini = sep_teardown);

TestSuite(scalablem_scalable,
	.init = sep_scalable_setup_map,
	.fini = sep_teardown);

TestSuite(scalablet_scalable,
	.init = sep_scalable_setup_table,
	.fini = sep_teardown);

Test(scalablea, misc)
{
}

Test(scalablem_default, misc)
{
	int ret;
	struct fi_av_attr av_attr = {0};

	/* test if bind fails */
	ret = fi_ep_bind(tx_ep[0][0], &tx_cq[0][0]->fid,
			 FI_TRANSMIT);
	cr_assert(ret, "fi_ep_bind should fail");

	ret = fi_ep_bind(rx_ep[0][0], &rx_cq[0][0]->fid,
			 FI_TRANSMIT);
	cr_assert(ret, "fi_ep_bind should fail");

	/* test for inserting an ep_name that doesn't fit in the AV */
	av_attr.type = FI_AV_MAP;
	av_attr.count = NUMEPS;
	av_attr.rx_ctx_bits = 1;

	ret = fi_av_open(dom[0], &av_attr, &t_av, NULL);
	cr_assert(!ret, "fi_av_open");
	ret = fi_av_insert(t_av, ep_name[0], 1, &gni_addr[0], 0, NULL);
	cr_assert(ret == -FI_EINVAL);
	ret = fi_close(&t_av->fid);
	cr_assert(!ret, "failure in closing av.");

	/* test closing a scalable endpoint with open contexts */
	ret = fi_close(&sep[0]->fid);
	cr_assert_eq(ret, -FI_EBUSY, "close should have failed.");
}

Test(scalablem_basic, all)
{
	cr_log_info(BLUE "sep:basic:FI_AV_MAP tests:\n" COLOR_RESET);
	run_tests();
}

Test(scalablem_basic, cancel)
{
	cr_log_info(BLUE "sep:basic:FI_AV_MAP cancel tests:\n" COLOR_RESET);
	run_cancel_tests();
}

Test(scalablet_basic, all)
{
	cr_log_info(BLUE "sep:basic:FI_AV_TABLE tests:\n" COLOR_RESET);
	run_tests();
}

Test(scalablet_basic, cancel)
{
	cr_log_info(BLUE "sep:basic:FI_AV_TABLE cancel tests:\n" COLOR_RESET);
	run_cancel_tests();
}

Test(scalablem_scalable, all)
{
	cr_log_info(BLUE "sep:scalable:FI_AV_MAP tests:\n" COLOR_RESET);
	run_tests();
}

Test(scalablem_scalable, cancel)
{
	cr_log_info(BLUE "sep:scalable:FI_AV_MAP cancel tests:\n" COLOR_RESET);
	run_cancel_tests();
}

Test(scalablet_scalable, all)
{
	cr_log_info(BLUE "sep:scalable:FI_AV_TABLE tests:\n" COLOR_RESET);
	run_tests();
}

Test(scalablet_scalable, cancel)
{
	cr_log_info(BLUE "sep:scalable:FI_AV_TABLE cancel tests:\n" COLOR_RESET);
	run_cancel_tests();
}

#define INSERT_ADDR_COUNT (NUMCONTEXTS + 6)

/* test for inserting an ep_name that doesn't fit in the AV */
Test(scalable, av_insert)
{
	int ret, i;
	size_t addrlen = sizeof(struct gnix_ep_name);
	struct fi_av_attr av_attr = {0};
	int err[INSERT_ADDR_COUNT] = {0};
	fi_addr_t addresses[INSERT_ADDR_COUNT];
	struct gnix_ep_name epname[TOTALEPS];

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");
	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_ATOMIC | FI_RMA | FI_MSG | FI_NAMED_RX_CTX | FI_TAGGED;
	hints->mode = FI_LOCAL_MR;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->fabric_attr->prov_name = strdup("gni");
	hints->ep_attr->tx_ctx_cnt = NUMCONTEXTS;
	hints->ep_attr->rx_ctx_cnt = NUMCONTEXTS;

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[0]);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi[0]->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	fi[0]->ep_attr->tx_ctx_cnt = NUMCONTEXTS;
	fi[0]->ep_attr->rx_ctx_cnt = NUMCONTEXTS;

	ret = fi_domain(fab, fi[0], &dom[0], NULL);
	cr_assert(!ret, "fi_domain");

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_scalable_ep(dom[0], fi[0], &sep[i], NULL);
		cr_assert(!ret, "fi_scalable_ep");

		ret = fi_enable(sep[i]);
		cr_assert(!ret, "fi_enable");

		ret = fi_getname(&sep[i]->fid, &epname[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);
	}

	fi[0]->ep_attr->rx_ctx_cnt = INSERT_ADDR_COUNT;

	for (i = NUMEPS; i < TOTALEPS; i++) {
		ret = fi_scalable_ep(dom[0], fi[0], &sep[i], NULL);
		cr_assert(!ret, "fi_scalable_ep");

		ret = fi_enable(sep[i]);
		cr_assert(!ret, "fi_enable");

		ret = fi_getname(&sep[i]->fid, &epname[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);
	}

	rx_ctx_bits = 0;
	ctx_cnt = NUMCONTEXTS;
	while (ctx_cnt >> ++rx_ctx_bits);
	av_attr.type = FI_AV_TABLE;
	av_attr.count = NUMCONTEXTS;
	av_attr.rx_ctx_bits = rx_ctx_bits;

	cr_log_info("test av table error path\n");
	ret = fi_av_open(dom[0], &av_attr, &t_av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_av_insert(t_av, epname, TOTALEPS,
			   addresses, FI_SYNC_ERR, err);
	cr_assert((ret != TOTALEPS), "fi_av_insert should have failed but did not.");

	cr_log_info("check for errors\n");
	for (i = 0; i < NUMEPS; i++) {
		cr_assert_eq(err[i], 0, "err[%d]:%d", i, err[i]);
		cr_assert_neq(addresses[i], FI_ADDR_NOTAVAIL,
				"addresses[%d]:%lx", i, addresses[i]);
	}
	for (; i < TOTALEPS; i++) {
		cr_assert_eq(err[i], -FI_EINVAL, "err[%d]:%d", i, err[i]);
		cr_assert_eq(addresses[i], FI_ADDR_NOTAVAIL,
				"addresses[%d]:%lx", i, addresses[i]);
	}

	ret = fi_close(&t_av->fid);
	cr_assert(!ret, "failure in closing av.");

	cr_log_info("test av map error path\n");
	av_attr.type = FI_AV_MAP;

	ret = fi_av_open(dom[0], &av_attr, &t_av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_av_insert(t_av, epname, TOTALEPS,
			   addresses, FI_SYNC_ERR, err);
	cr_assert((ret != TOTALEPS), "fi_av_insert should have failed but did not.");

	cr_log_info("check for errors\n");

	for (i = 0; i < NUMEPS; i++) {
		cr_assert_eq(err[i], 0, "err[%d]:%d", i, err[i]);
		cr_assert_neq(addresses[i], FI_ADDR_NOTAVAIL,
				"addresses[%d]:%lx", i, addresses[i]);
	}

	for (; i < TOTALEPS; i++) {
		cr_assert_eq(err[i], -FI_EINVAL, "err[%d]:%d", i, err[i]);
		cr_assert_eq(addresses[i], FI_ADDR_NOTAVAIL,
				"addresses[%d]:%lx", i, addresses[i]);
	}

	ret = fi_close(&t_av->fid);
	cr_assert(!ret, "failure in closing av.");

	ret = fi_close(&sep[0]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&dom[0]->fid);
	cr_assert(!ret, "failure in closing domain.");
	fi_freeinfo(hints);

}
