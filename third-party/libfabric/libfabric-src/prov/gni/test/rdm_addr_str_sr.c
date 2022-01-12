/*
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

#define NUMEPS 2

static struct fid_fabric *fab;
static struct fid_domain *dom[NUMEPS];
static struct fid_ep *ep[NUMEPS];
static struct fid_av *av[NUMEPS];
static void *ep_name[NUMEPS];
static fi_addr_t gni_addr[NUMEPS];
static struct fid_cq *msg_cq[NUMEPS];
static struct fi_info *fi[NUMEPS];
static struct fi_cq_attr cq_attr;
static struct fi_info *hints;
static size_t addrlen = 0;

#define BUF_SZ (1<<20)
#define BUF_RNDZV (1<<14)

static char *target, *target_base;
static char *source, *source_base;

void rdm_str_addr_sr_setup_common(void)
{
	int ret = 0, i = 0, j = 0;
	struct fi_av_attr attr;

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = NUMEPS;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	ret = fi_fabric(fi[0]->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_domain(fab, fi[i], dom + i, NULL);
		cr_assert(!ret, "fi_domain");

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
		if (i < (NUMEPS / 2)) {
			for (j = 0; j < NUMEPS; j++) {
				dbg_printf("Only does src EP insertions\n");
				ret = fi_av_insert(av[i], ep_name[j], 1,
						   &gni_addr[j],
						   0, NULL);
				cr_assert(ret == 1);
			}
		}

		ret = fi_ep_bind(ep[i], &av[i]->fid, 0);
		cr_assert(!ret, "fi_ep_bind");

		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_ep_enable");

	}
}


void rdm_str_addr_sr_setup(void)
{
	int ret = 0, i = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->mode = FI_CONTEXT;
	hints->caps = FI_SOURCE | FI_MSG | FI_SOURCE | FI_SOURCE_ERR;
	hints->fabric_attr->prov_name = strdup("gni");
	hints->addr_format = FI_ADDR_STR;

	/* Get info about fabric services with the provided hints */
	for (; i < NUMEPS; i++) {
		ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi[i]);
		cr_assert(!ret, "fi_getinfo");
	}

	rdm_str_addr_sr_setup_common();
}

static void rdm_str_addr_sr_teardown(void)
{
	int ret = 0, i = 0;

	for (; i < NUMEPS; i++) {
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

void rdm_str_addr_sr_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++) {
		buf[i] = seed++;
	}
}

static inline int rdm_str_addr_sr_check_data(char *buf1, char *buf2, int len)
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

void rdm_str_addr_sr_xfer_for_each_size(void (*xfer)(int len), int slen, int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2) {
		xfer(i);
	}
}

static inline int rdm_str_addr_sr_check_err_cqe(struct fid_cq *cq)
{
	int ret = FI_SUCCESS, cnt;
	struct fi_cq_err_entry ee;
	size_t name_size;
	char *buffer;
	fi_addr_t fi_addr;

	/*application provided error_data buffer and length*/
	ee.err_data_size = addrlen;
	ee.err_data = malloc(addrlen);
	cr_assert((ee.err_data != NULL), "malloc failed");
	buffer = malloc(addrlen);
	cr_assert((buffer != NULL), "malloc failed");

	cnt = fi_cq_readerr(cq, &ee, 0);
	cr_assert((cnt == 1), "fi_cq_readerr didn't return entry");

	if ((hints->caps & FI_SOURCE_ERR) && ee.err == FI_EADDRNOTAVAIL) {
		ret = fi_av_insert(av[1], ee.err_data, 1, &fi_addr,
				   0, NULL);
		cr_assert(ret == 1, "fi_av_insert failed");
		name_size = addrlen;
		ret = fi_av_lookup(av[1], fi_addr,
				   buffer, &name_size);
		cr_assert(ret == FI_SUCCESS, "fi_av_lookup failed");
		cr_assert(name_size == addrlen);
		cr_assert(strncmp((char *)buffer,
				  (char *)ee.err_data,
				  addrlen) == 0);
	}
	return ret;
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

TestSuite(rdm_str_addr_sr,
	  .init = rdm_str_addr_sr_setup,
	  .fini = rdm_str_addr_sr_teardown,
	  .disabled = false);

/*
 * ssize_t fi_send(struct fid_ep *ep, void *buf, size_t len,
 *		void *desc, fi_addr_t dest_addr, void *context);
 *
 * ssize_t fi_recv(struct fid_ep *ep, void * buf, size_t len,
 *		void *desc, fi_addr_t src_addr, void *context);
 */
static void do_send(int len)
{
	int ret;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry s_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_tagged_entry d_cqe = { (void *) -1, UINT_MAX, UINT_MAX,
					    (void *) -1, UINT_MAX, UINT_MAX };
	struct fi_cq_err_entry d_err_cqe;
	struct fi_cq_err_entry s_err_cqe;

	ssize_t sz;

	memset(&d_err_cqe, -1, sizeof(struct fi_cq_err_entry));
	memset(&s_err_cqe, -1, sizeof(struct fi_cq_err_entry));

	rdm_str_addr_sr_init_data(source, len, 0xab);
	rdm_str_addr_sr_init_data(target, len, 0);

	sz = fi_send(ep[0], source, len, NULL, gni_addr[1], target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep[1], target, len, NULL, FI_ADDR_UNSPEC, source);
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

		if (ret == -FI_EAVAIL) {
			ret = rdm_str_addr_sr_check_err_cqe(msg_cq[1]);
			cr_assert((ret == FI_SUCCESS), "Err CQE processing failed");
			dest_done = 1;
		}
	} while (!(source_done && dest_done));

	cr_assert(rdm_str_addr_sr_check_data(source, target, len), "Data mismatch");
}

Test(rdm_str_addr_sr, send)
{
	rdm_str_addr_sr_xfer_for_each_size(do_send, 1, BUF_SZ);
}
