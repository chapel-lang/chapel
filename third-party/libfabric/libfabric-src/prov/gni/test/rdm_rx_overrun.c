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

#define NUM_EPS 61
const int num_msgs = 10;

/*
 * Note that even tho we will use a min RX CQ size of 1, ugni seems to
 * have an internal minimum that is around 511, so in order for this
 * test to exercise the overrun code, it must send at least 511
 * messages (i.e., NUM_EPS*num_msgs > 511)
 */
const int min_rx_cq_size = 1;

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom[NUM_EPS];
static struct fid_ep *ep[NUM_EPS];
static struct fid_av *av[NUM_EPS];
static struct fi_info *hints;
static struct fi_info *fi;
static void *ep_name[NUM_EPS];
static fi_addr_t gni_addr[NUM_EPS];
static struct fid_cq *msg_cq[NUM_EPS];
static struct fi_cq_attr cq_attr;

static int target[NUM_EPS];
static int source[NUM_EPS];
static struct fid_mr *rem_mr[NUM_EPS], *loc_mr[NUM_EPS];
static uint64_t mr_key[NUM_EPS];

static int max_eps = NUM_EPS;

static void __setup(uint32_t version, int mr_mode)
{
	int i, j;
	int ret = 0;
	struct fi_av_attr attr;
	size_t addrlen = 0;
	struct fi_gni_ops_domain *gni_domain_ops;
	uint32_t rx_cq_size;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = mr_mode;
	hints->domain_attr->cq_data_size = 4;
	hints->domain_attr->data_progress = FI_PROGRESS_MANUAL;

	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	if (USING_SCALABLE(fi)) {
		struct fi_gni_ops_fab *ops;
		int in;

		/* nic test opens many nics and exhausts reserved keys */
		in = 256;

		ret = fi_open_ops(&fab->fid,
			FI_GNI_FAB_OPS_1, 0, (void **) &ops, NULL);
		cr_assert_eq(ret, FI_SUCCESS);
		cr_assert(ops);

		ret = ops->set_val(&fab->fid,
			GNI_DEFAULT_PROV_REGISTRATION_LIMIT,
			&in);
		cr_assert_eq(ret, FI_SUCCESS);
	}

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_TABLE;
	attr.count = NUM_EPS;

	cq_attr.format = FI_CQ_FORMAT_CONTEXT;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	for (i = 0; i < NUM_EPS; i++) {
		ret = fi_domain(fab, fi, &dom[i], NULL);
		cr_assert(!ret, "fi_domain (%d)", i);

		ret = fi_open_ops(&dom[i]->fid, FI_GNI_DOMAIN_OPS_1, 0,
				  (void **) &gni_domain_ops, NULL);
		cr_assert(ret == FI_SUCCESS, "fi_open_ops (%d)", i);

		rx_cq_size = min_rx_cq_size;

		ret = gni_domain_ops->set_val(&dom[i]->fid, GNI_RX_CQ_SIZE,
					      &rx_cq_size);
		cr_assert(ret == FI_SUCCESS, "set_val (%d)", i);

		ret = fi_endpoint(dom[i], fi, &ep[i], NULL);
		if (ret != FI_SUCCESS) {
			/* ran out of resources */
			max_eps = i;
			break;
		}
		cr_assert(ep[i]);

		ret = fi_av_open(dom[i], &attr, &av[i], NULL);
		cr_assert(!ret, "fi_av_open (%d)", i);

		ret = fi_cq_open(dom[i], &cq_attr, &msg_cq[i], 0);
		cr_assert(!ret, "fi_cq_open (%d)", i);

		ret = fi_ep_bind(ep[i], &msg_cq[i]->fid, FI_SEND | FI_RECV);
		cr_assert(!ret, "fi_ep_bind (%d)", i);
	}

	ret = fi_getname(&ep[0]->fid, NULL, &addrlen);
	cr_assert_eq(ret, -FI_ETOOSMALL);
	cr_assert(addrlen > 0);

	for (i = 0; i < max_eps; i++) {
		ep_name[i] = malloc(addrlen);
		cr_assert(ep_name[i] != NULL);
		ret = fi_getname(&ep[i]->fid, ep_name[i], &addrlen);
		cr_assert(ret == FI_SUCCESS);
		for (j = 0; j < max_eps; j++) {
			ret = fi_av_insert(av[j], ep_name[i],
					1, &gni_addr[i], 0, NULL);
			cr_assert(ret == 1);
		}
	}

	for (i = 0; i < max_eps; i++) {
		int requested_rem_key = (USING_SCALABLE(fi)) ? (i * 2) : 0;
		int requested_loc_key = (USING_SCALABLE(fi)) ? (i * 2) + 1 : 0;

		ret = fi_ep_bind(ep[i], &av[i]->fid, 0);
		cr_assert(!ret, "fi_ep_bind");
		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_ep_enable");

		ret = fi_mr_reg(dom[i],
				  target,
				  max_eps*sizeof(int),
				  FI_RECV,
				  0,
				  requested_rem_key,
				  0,
				  &rem_mr[i],
				  &target);
		cr_assert_eq(ret, 0);

		ret = fi_mr_reg(dom[i],
				  source,
				  max_eps*sizeof(int),
				  FI_SEND,
				  0,
				  requested_loc_key,
				  0,
				  &loc_mr[i],
				  &source);
		cr_assert_eq(ret, 0);

		if (USING_SCALABLE(fi)) {
			MR_ENABLE(rem_mr[i], target, max_eps*sizeof(int));
			MR_ENABLE(loc_mr[i], source, max_eps*sizeof(int));
		}

		mr_key[i] = fi_mr_key(rem_mr[i]);
	}
}

static void setup_basic(void)
{
	__setup(fi_version(), GNIX_MR_BASIC);
}

static void setup_scalable(void)
{
	__setup(fi_version(), GNIX_MR_SCALABLE);
}

static void teardown(void)
{
	int i;
	int ret = 0;

	for (i = 0; i < max_eps; i++) {
		fi_close(&loc_mr[i]->fid);
		fi_close(&rem_mr[i]->fid);

		ret = fi_close(&ep[i]->fid);
		cr_assert(!ret, "failure in closing ep %d.", i);
		ret = fi_close(&msg_cq[i]->fid);
		cr_assert(!ret, "failure in msg cq %d.", i);
		free(ep_name[i]);

		ret = fi_close(&av[i]->fid);
		cr_assert(!ret, "failure in closing av %d.", i);

		ret = fi_close(&dom[i]->fid);
		cr_assert(!ret, "failure in closing domain %d.", i);
	}

	if (max_eps != NUM_EPS) {
		/* clean up the last domain */
		ret = fi_close(&dom[max_eps]->fid);
		cr_assert(!ret, "failure in closing domain %d.", max_eps);
	}

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

TestSuite(rdm_rx_overrun_basic,
	  .init = setup_basic,
	  .fini = teardown,
	  .disabled = false);
TestSuite(rdm_rx_overrun_scalable,
	  .init = setup_scalable,
	  .fini = teardown,
	  .disabled = false);

static inline void __all_to_one(void)
{
	int i, j;
	int source_done = 0, dest_done = 0;
	struct fi_cq_entry s_cqe, d_cqe;
	ssize_t sz;
	int ctx[max_eps];

	for (i = 0; i < max_eps; i++) {
		source[i] = i;
		target[i] = -1;
		ctx[i] = -1;
	}

	for (i = 1; i < max_eps; i++) {
		for (j = 0; j < num_msgs; j++) {
			sz = fi_send(ep[i], &source[i], sizeof(int), loc_mr,
				     gni_addr[0], ctx+i);
			cr_assert_eq(sz, 0);
		}
	}

	do {
		for (i = 1; i < max_eps; i++) {
			for (j = 0; j < num_msgs; j++) {
				if (fi_cq_read(msg_cq[i], &s_cqe, 1) == 1) {
					cr_assert(((uint64_t) s_cqe.op_context
						   >= (uint64_t) ctx) &&
						  ((uint64_t) s_cqe.op_context
						   <=
						   (uint64_t) (ctx+max_eps-1)));
					source_done += 1;
				}
			}
		}
	} while (source_done != num_msgs*(max_eps-1));

	for (i = 1; i < max_eps; i++) {
		for (j = 0; j < num_msgs; j++) {
			sz = fi_recv(ep[0], &target[i], sizeof(int), rem_mr,
				     gni_addr[i], ctx+i);
			cr_assert_eq(sz, 0);
		}
	}

	do {
		for (i = 1; i < max_eps; i++) {
			for (j = 0; j < num_msgs; j++) {
				if (fi_cq_read(msg_cq[0], &d_cqe, 1) == 1) {
					cr_assert(((uint64_t) d_cqe.op_context
						   >= (uint64_t) ctx) &&
						  ((uint64_t) d_cqe.op_context
						   <=
						   (uint64_t) (ctx+max_eps-1)));
					dest_done += 1;
				}
			}
		}
	} while (dest_done != num_msgs*(max_eps-1));


	/* good enough error checking (only checks the last send) */
	for (i = 1; i < max_eps; i++) {
		cr_assert(target[i] < max_eps);
		ctx[target[i]] = target[i];
	}

	for (i = 1; i < max_eps; i++) {
		cr_assert(ctx[i] == i);
	}

}

Test(rdm_rx_overrun_basic, all_to_one)
{
	__all_to_one();
}

Test(rdm_rx_overrun_scalable, all_to_one)
{
	__all_to_one();
}
