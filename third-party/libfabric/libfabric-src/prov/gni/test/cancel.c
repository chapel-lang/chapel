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
#include "gnix_ep.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

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

#define BUF_SZ (8*1024)
static char *target, *target_base;
static char *source, *source_base;
static struct fid_mr *rem_mr, *loc_mr;
static uint64_t mr_key;

void cancel_setup(void)
{
	int ret = 0;
	struct fi_av_attr attr;
	size_t addrlen = 0;
	int rem_requested_key, loc_requested_key;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = 16;

	ret = fi_av_open(dom, &attr, &av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_endpoint(dom, fi, &ep[0], NULL);
	cr_assert(!ret, "fi_endpoint");

	cq_attr.format = FI_CQ_FORMAT_CONTEXT;
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

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	rem_requested_key = USING_SCALABLE(fi) ? 1 : 0;
	loc_requested_key = USING_SCALABLE(fi) ? 2 : 0;

	ret = fi_mr_reg(dom,
			  target,
			  BUF_SZ,
			  FI_REMOTE_WRITE,
			  0,
			  rem_requested_key,
			  0,
			  &rem_mr,
			  &target);
	cr_assert_eq(ret, 0);

	ret = fi_mr_reg(dom,
			  source,
			  BUF_SZ,
			  FI_REMOTE_WRITE,
			  0,
			  loc_requested_key,
			  0,
			  &loc_mr,
			  &source);
	cr_assert_eq(ret, 0);

	if (USING_SCALABLE(fi)) {
		MR_ENABLE(rem_mr, target, BUF_SZ);
		MR_ENABLE(loc_mr, source, BUF_SZ);
	}

	mr_key = fi_mr_key(rem_mr);
}

void cancel_teardown(void)
{
	int ret = 0;

	fi_close(&loc_mr->fid);
	fi_close(&rem_mr->fid);

	free(target_base);
	free(source_base);

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

void cancel_init_data(char *buf, int len, char seed)
{
	int i;

	for (i = 0; i < len; i++) {
		buf[i] = seed++;
	}
}

int cancel_check_data(char *buf1, char *buf2, int len)
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

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

TestSuite(gnix_cancel, .init = cancel_setup, .fini = cancel_teardown,
	  .disabled = false);

Test(gnix_cancel, cancel_ep_send)
{
	int ret;
	struct gnix_fid_ep *gnix_ep;
	struct gnix_fab_req *req;
	struct fi_cq_err_entry buf;
	struct gnix_vc *vc;
	void *foobar_ptr = NULL;
	gnix_ht_key_t *key;

	/* simulate a posted request */
	gnix_ep = container_of(ep[0], struct gnix_fid_ep, ep_fid);
	req = _gnix_fr_alloc(gnix_ep);

	req->msg.send_info[0].send_addr = 0xdeadbeef;
	req->msg.cum_send_len = req->msg.send_info[0].send_len = 128;
	req->user_context = foobar_ptr;
	req->type = GNIX_FAB_RQ_SEND;

	/* allocate, store vc */
	ret = _gnix_vc_alloc(gnix_ep, NULL, &vc);
	cr_assert(ret == FI_SUCCESS, "_gnix_vc_alloc failed");

	key = (gnix_ht_key_t *)&gnix_ep->src_addr.gnix_addr;
	ret = _gnix_ht_insert(gnix_ep->vc_ht, *key, vc);
	cr_assert(!ret);

	/* make a dummy request */
	dlist_insert_head(&req->dlist, &vc->tx_queue);

	/* cancel simulated request */
	ret = fi_cancel(&ep[0]->fid, foobar_ptr);
	cr_assert(ret == FI_SUCCESS, "fi_cancel failed");

	/* check for event */
	ret = fi_cq_readerr(msg_cq[0], &buf, FI_SEND);
	cr_assert(ret == 1, "did not find one error event");

	cr_assert(buf.buf == (void *) 0xdeadbeef, "buffer mismatch");
	cr_assert(buf.data == 0, "data mismatch");
	cr_assert(buf.err == FI_ECANCELED, "error code mismatch");
	cr_assert(buf.prov_errno == FI_ECANCELED, "prov error code mismatch");
	cr_assert(buf.len == 128, "length mismatch");
}

Test(gnix_cancel, cancel_ep_recv)
{
	int ret;
	struct fi_cq_err_entry buf;

	/* simulate a posted request */
	ret = fi_recv(ep[0], (void *) 0xdeadbeef, 128, 0, FI_ADDR_UNSPEC,
			(void *) 0xcafebabe);
	cr_assert(ret == FI_SUCCESS, "fi_recv failed");

	/* cancel simulated request */
	ret = fi_cancel(&ep[0]->fid, (void *) 0xcafebabe);
	cr_assert(ret == FI_SUCCESS, "fi_cancel failed");

	/* check for event */
	ret = fi_cq_readerr(msg_cq[0], &buf, FI_RECV);
	cr_assert(ret == 1, "did not find one error event");

	cr_assert(buf.buf == (void *) 0xdeadbeef, "buffer mismatch");
	cr_assert(buf.data == 0, "data mismatch");
	cr_assert(buf.err == FI_ECANCELED, "error code mismatch");
	cr_assert(buf.prov_errno == FI_ECANCELED, "prov error code mismatch");
	cr_assert(buf.len == 128, "length mismatch");
}

Test(gnix_cancel, cancel_ep_no_event)
{
	int ret;

	ret = fi_cancel(&ep[0]->fid, NULL);
	cr_assert(ret == -FI_ENOENT, "fi_cancel failed");
}

Test(gnix_cancel, cancel_ep_no_domain)
{
	int ret;
	struct gnix_fid_ep *gnix_ep;
	struct gnix_fid_domain *gnix_dom;

	/* simulate a disconnected endpoint */
	gnix_ep = container_of(ep[0], struct gnix_fid_ep, ep_fid);
	gnix_dom = gnix_ep->domain;
	gnix_ep->domain = NULL;

	/* run test */
	ret = fi_cancel(&ep[0]->fid, NULL);
	cr_assert(ret == -FI_EDOMAIN, "fi_cancel failed");

	/* reconnect */
	gnix_ep->domain = gnix_dom;
}
