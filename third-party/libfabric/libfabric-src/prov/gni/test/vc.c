/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All rights reserved
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
 * Copyright (c) 2020 Triad National Security, LLC. All rights reserved.
 *
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
#include "gnix_nic.h"
#include "gnix_cm_nic.h"
#include "gnix_hashtable.h"
#include "gnix_av.h"

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
static struct fid_cq *cq[2];
static struct fi_cq_attr cq_attr;
static void *ep_name[2];
static fi_addr_t gni_addr[2];
static struct gnix_av_addr_entry gnix_addr[2];

/* Third EP with unqiue domain is used to test inter-CM connect. */
static struct fid_domain *dom3;
static struct fid_ep *ep3;
static struct fid_av *av3;
static struct fid_cq *cq3;
static void *ep_name3;

/* Register a target buffer with both domains for pings. */
static void *target_buf, *target_buf_base;
static int target_len = 64;
static struct fid_mr *rem_mr, *rem_mr3;
static uint64_t mr_key, mr_key3;

static void vc_setup_common(uint32_t version, int mr_mode);

static inline void __vc_setup(uint32_t version,
	int mr_mode,
	int control_progress)
{

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->cq_data_size = 4;
	hints->domain_attr->control_progress = control_progress;
	hints->mode = mode_bits;

	vc_setup_common(version, mr_mode);
}

static void vc_setup_manual_basic(void)
{
	__vc_setup(fi_version(), GNIX_MR_BASIC, FI_PROGRESS_MANUAL);
}

static void vc_setup_manual_scalable(void)
{
	__vc_setup(fi_version(), GNIX_MR_SCALABLE, FI_PROGRESS_MANUAL);
}

static void vc_setup_auto_basic(void)
{
	__vc_setup(fi_version(), GNIX_MR_BASIC, FI_PROGRESS_AUTO);
}

static void vc_setup_auto_scalable(void)
{
	__vc_setup(fi_version(), GNIX_MR_SCALABLE, FI_PROGRESS_AUTO);
}

static void vc_setup_common(uint32_t version, int mr_mode)
{
	int ret = 0;
	struct fi_av_attr attr = {0};
	size_t addrlen = 0;
	struct gnix_fid_av *gnix_av;

	hints->domain_attr->mr_mode = mr_mode;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
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

	gnix_av = container_of(av, struct gnix_fid_av, av_fid);

	ret = fi_endpoint(dom, fi, &ep[0], NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_getname(&ep[0]->fid, NULL, &addrlen);
	cr_assert(addrlen > 0);

	ep_name[0] = malloc(addrlen);
	cr_assert(ep_name[0] != NULL);

	ep_name[1] = malloc(addrlen);
	cr_assert(ep_name[1] != NULL);

	ret = fi_getname(&ep[0]->fid, ep_name[0], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_endpoint(dom, fi, &ep[1], NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_getname(&ep[1]->fid, ep_name[1], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_av_insert(av, ep_name[0], 1, &gni_addr[0], 0,
				NULL);
	cr_assert(ret == 1);

	ret = _gnix_av_lookup(gnix_av, gni_addr[0], &gnix_addr[0]);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_av_insert(av, ep_name[1], 1, &gni_addr[1], 0,
				NULL);
	cr_assert(ret == 1);

	ret = _gnix_av_lookup(gnix_av, gni_addr[1], &gnix_addr[1]);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_ep_bind(ep[0], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_ep_bind(ep[1], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(dom, &cq_attr, &cq[0], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep[0], &cq[0]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_cq_open(dom, &cq_attr, &cq[1], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep[1], &cq[1]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep[0]);
	cr_assert(!ret, "fi_enable");

	ret = fi_enable(ep[1]);
	cr_assert(!ret, "fi_ep_enable");
}

void vc_teardown(void)
{
	int ret = 0;

	ret = fi_close(&ep[0]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&ep[1]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&cq[0]->fid);
	cr_assert(!ret, "failure in closing cq.");

	ret = fi_close(&cq[1]->fid);
	cr_assert(!ret, "failure in closing cq.");

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

static void vc_conn_ping_setup(uint32_t version, int mr_mode)
{
	int ret = 0;
	struct fi_av_attr attr = {0};
	size_t addrlen = 0;

	hints->domain_attr->mr_mode = mr_mode;
	hints->fabric_attr->name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

	attr.type = FI_AV_TABLE;
	attr.count = 16;

	ret = fi_av_open(dom, &attr, &av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_endpoint(dom, fi, &ep[0], NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_getname(&ep[0]->fid, NULL, &addrlen);
	cr_assert(addrlen > 0);

	ep_name[0] = malloc(addrlen);
	cr_assert(ep_name[0] != NULL);

	ep_name[1] = malloc(addrlen);
	cr_assert(ep_name[1] != NULL);

	ret = fi_getname(&ep[0]->fid, ep_name[0], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_endpoint(dom, fi, &ep[1], NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_getname(&ep[1]->fid, ep_name[1], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_av_insert(av, ep_name[0], 1, NULL, 0, NULL);
	cr_assert(ret == 1);

	ret = fi_av_insert(av, ep_name[1], 1, NULL, 0, NULL);
	cr_assert(ret == 1);

	ret = fi_ep_bind(ep[0], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_ep_bind(ep[1], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(dom, &cq_attr, &cq[0], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep[0], &cq[0]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep[0]);
	cr_assert(!ret, "fi_enable");

	ret = fi_cq_open(dom, &cq_attr, &cq[1], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep[1], &cq[1]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep[1]);
	cr_assert(!ret, "fi_ep_enable");

	/* Setup third EP with separate domain to test inter-CM NIC connect. */
	ret = fi_domain(fab, fi, &dom3, NULL);
	cr_assert(!ret, "fi_domain");

	attr.type = FI_AV_TABLE;
	attr.count = 16;

	ret = fi_av_open(dom3, &attr, &av3, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_endpoint(dom3, fi, &ep3, NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_getname(&ep3->fid, NULL, &addrlen);
	cr_assert(addrlen > 0);

	ep_name3 = malloc(addrlen);
	cr_assert(ep_name3 != NULL);

	ret = fi_getname(&ep3->fid, ep_name3, &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_av_insert(av3, ep_name[0], 1, NULL, 0, NULL);
	cr_assert(ret == 1);

	ret = fi_av_insert(av3, ep_name[1], 1, NULL, 0, NULL);
	cr_assert(ret == 1);

	ret = fi_av_insert(av3, ep_name3, 1, NULL, 0, NULL);
	cr_assert(ret == 1);

	ret = fi_ep_bind(ep3, &av3->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(dom3, &cq_attr, &cq3, 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep3, &cq3->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep3);
	cr_assert(!ret, "fi_enable");

	/* Register target buffer for pings. */
	target_buf_base = malloc(GNIT_ALIGN_LEN(target_len));
	assert(target_buf_base);
	target_buf = GNIT_ALIGN_BUFFER(void *, target_buf_base);

	ret = fi_mr_reg(dom, target_buf, sizeof(target_buf),
			FI_REMOTE_WRITE, 0, (USING_SCALABLE(fi) ? 1 : 0),
			0, &rem_mr, &target_buf);
	cr_assert_eq(ret, 0);

	if (USING_SCALABLE(fi))
		MR_ENABLE(rem_mr, target_buf, sizeof(target_buf));

	mr_key = fi_mr_key(rem_mr);

	ret = fi_mr_reg(dom3, target_buf, sizeof(target_buf),
			FI_REMOTE_WRITE, 0, (USING_SCALABLE(fi) ? 2 : 0),
			0, &rem_mr3, &target_buf);
	cr_assert_eq(ret, 0);

	if (USING_SCALABLE(fi))
		MR_ENABLE(rem_mr3, target_buf, sizeof(target_buf));

	mr_key3 = fi_mr_key(rem_mr3);

	ret = fi_av_insert(av, ep_name3, 1, NULL, 0, NULL);
	cr_assert(ret == 1);
}

static void __vc_conn_ping_setup(uint32_t version,
	int mr_mode,
	int control_progress)
{

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->cq_data_size = 4;
	hints->domain_attr->control_progress = control_progress;
	hints->mode = mode_bits;

	vc_conn_ping_setup(version, mr_mode);
}

static void vc_conn_ping_setup_manual_basic(void)
{
	__vc_conn_ping_setup(fi_version(), GNIX_MR_BASIC, FI_PROGRESS_MANUAL);
}

static void vc_conn_ping_setup_manual_scalable(void)
{
	__vc_conn_ping_setup(fi_version(), GNIX_MR_SCALABLE,
		FI_PROGRESS_MANUAL);
}

static void vc_conn_ping_setup_auto_basic(void)
{
	__vc_conn_ping_setup(fi_version(), GNIX_MR_BASIC, FI_PROGRESS_AUTO);
}

static void vc_conn_ping_setup_auto_scalable(void)
{
	__vc_conn_ping_setup(fi_version(), GNIX_MR_SCALABLE, FI_PROGRESS_AUTO);
}

void vc_conn_ping_teardown(void)
{
	int ret = 0;

	ret = fi_close(&rem_mr3->fid);
	cr_assert(!ret, "failure in closing mr3.");

	ret = fi_close(&rem_mr->fid);
	cr_assert(!ret, "failure in closing mr.");

	free(target_buf_base);

	ret = fi_close(&cq3->fid);
	cr_assert(!ret, "failure in closing cq3.");

	ret = fi_close(&ep3->fid);
	cr_assert(!ret, "failure in closing ep3.");

	ret = fi_close(&av3->fid);
	cr_assert(!ret, "failure in closing av3.");

	ret = fi_close(&dom3->fid);
	cr_assert(!ret, "failure in closing domain3.");

	ret = fi_close(&ep[0]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&ep[1]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&cq[0]->fid);
	cr_assert(!ret, "failure in closing cq.");

	ret = fi_close(&cq[1]->fid);
	cr_assert(!ret, "failure in closing cq.");

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

/*******************************************************************************
 * Test vc functions.
 ******************************************************************************/

TestSuite(vc_management_auto_basic,
		.init = vc_setup_auto_basic,
		.fini = vc_teardown,
		.disabled = false);

TestSuite(vc_management_manual_basic,
		.init = vc_setup_manual_basic,
		.fini = vc_teardown,
		.disabled = false);

TestSuite(vc_management_auto_scalable,
		.init = vc_setup_auto_scalable,
		.fini = vc_teardown,
		.disabled = false);

TestSuite(vc_management_manual_scalable,
		.init = vc_setup_manual_scalable,
		.fini = vc_teardown,
		.disabled = false);


static inline void __vc_alloc_simple(void)
{
	int ret;
	struct gnix_vc *vc[2];
	struct gnix_fid_ep *ep_priv;

	ep_priv = container_of(ep[0], struct gnix_fid_ep, ep_fid);

	ret = _gnix_vc_alloc(ep_priv, &gnix_addr[0], &vc[0]);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = _gnix_vc_alloc(ep_priv, &gnix_addr[1], &vc[1]);
	cr_assert_eq(ret, FI_SUCCESS);

	/*
	 * vc_id's have to be different since the
	 * vc's were allocated using the same ep.
	 */
	cr_assert_neq(vc[0]->vc_id, vc[1]->vc_id);

	ret = _gnix_vc_destroy(vc[0]);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = _gnix_vc_destroy(vc[1]);
	cr_assert_eq(ret, FI_SUCCESS);
}

Test(vc_management_auto_basic, vc_alloc_simple)
{
	__vc_alloc_simple();
}

Test(vc_management_auto_scalable, vc_alloc_simple)
{
	__vc_alloc_simple();
}

static inline void __vc_lookup_by_id(void)
{
	int ret;
	struct gnix_vc *vc[2], *vc_chk;
	struct gnix_fid_ep *ep_priv;

	ep_priv = container_of(ep[0], struct gnix_fid_ep, ep_fid);

	ret = _gnix_vc_alloc(ep_priv, &gnix_addr[0], &vc[0]);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = _gnix_vc_alloc(ep_priv, &gnix_addr[1], &vc[1]);
	cr_assert_eq(ret, FI_SUCCESS);

	vc_chk = __gnix_nic_elem_by_rem_id(ep_priv->nic, vc[0]->vc_id);
	cr_assert_eq(vc_chk, vc[0]);

	vc_chk = __gnix_nic_elem_by_rem_id(ep_priv->nic, vc[1]->vc_id);
	cr_assert_eq(vc_chk, vc[1]);

	ret = _gnix_vc_destroy(vc[0]);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = _gnix_vc_destroy(vc[1]);
	cr_assert_eq(ret, FI_SUCCESS);
}

Test(vc_management_auto_basic, vc_lookup_by_id)
{
	__vc_lookup_by_id();
}

Test(vc_management_auto_scalable, vc_lookup_by_id)
{
	__vc_lookup_by_id();
}

static inline void __vc_connect(void)
{
	int ret;
	struct gnix_vc *vc_conn;
	struct gnix_fid_ep *ep_priv[2];
	gnix_ht_key_t key;
	enum gnix_vc_conn_state state;

	ep_priv[0] = container_of(ep[0], struct gnix_fid_ep, ep_fid);

	ep_priv[1] = container_of(ep[1], struct gnix_fid_ep, ep_fid);

	ret = _gnix_vc_alloc(ep_priv[0], &gnix_addr[1], &vc_conn);
	cr_assert_eq(ret, FI_SUCCESS);

	memcpy(&key, &gni_addr[1],
		sizeof(gnix_ht_key_t));

	ret = _gnix_ht_insert(ep_priv[0]->vc_ht, key, vc_conn);
	cr_assert_eq(ret, FI_SUCCESS);
	vc_conn->modes |= GNIX_VC_MODE_IN_HT;

	ret = _gnix_vc_connect(vc_conn);
	cr_assert_eq(ret, FI_SUCCESS);

	/*
	 * since we asked for FI_PROGRESS_AUTO for control
	 * we can just spin here.  add a yield in case the
	 * test is only being run on one cpu.
	 */

	/* We need to run CM NIC progress for an intra CM NIC connection. */
	ret = _gnix_cm_nic_progress(ep_priv[0]->domain->cm_nic);
	cr_assert_eq(ret, FI_SUCCESS);

	state = GNIX_VC_CONN_NONE;
	while (state != GNIX_VC_CONNECTED) {
		pthread_yield();
		state = _gnix_vc_state(vc_conn);
	}

	/* VC is destroyed by the EP */
}

Test(vc_management_auto_basic, vc_connect)
{
	__vc_connect();
}

Test(vc_management_auto_scalable, vc_connect)
{
	__vc_connect();
}

static inline void __vc_connect2(void)
{
	int ret;
	struct gnix_vc *vc_conn0, *vc_conn1;
	struct gnix_fid_ep *ep_priv[2];
	gnix_ht_key_t key;
	enum gnix_vc_conn_state state;

	ep_priv[0] = container_of(ep[0], struct gnix_fid_ep, ep_fid);
	ep_priv[1] = container_of(ep[1], struct gnix_fid_ep, ep_fid);

	ret = _gnix_vc_alloc(ep_priv[0], &gnix_addr[1], &vc_conn0);
	cr_assert_eq(ret, FI_SUCCESS);

	memcpy(&key, &gni_addr[1],
		sizeof(gnix_ht_key_t));

	ret = _gnix_ht_insert(ep_priv[0]->vc_ht, key, vc_conn0);
	cr_assert_eq(ret, FI_SUCCESS);

	vc_conn0->modes |= GNIX_VC_MODE_IN_HT;

	ret = _gnix_vc_alloc(ep_priv[1], &gnix_addr[0], &vc_conn1);
	cr_assert_eq(ret, FI_SUCCESS);

	memcpy(&key, &gni_addr[0],
		sizeof(gnix_ht_key_t));

	ret = _gnix_ht_insert(ep_priv[1]->vc_ht, key, vc_conn1);
	cr_assert_eq(ret, FI_SUCCESS);

	vc_conn1->modes |= GNIX_VC_MODE_IN_HT;

	ret = _gnix_vc_connect(vc_conn0);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = _gnix_vc_connect(vc_conn1);
	cr_assert_eq(ret, FI_SUCCESS);

	/*
	 * since we asked for FI_PROGRESS_AUTO for control
	 * we can just spin here.  add a yield in case the
	 * test is only being run on one cpu.
	 */

	/* We need to run CM NIC progress for an intra CM NIC connection. */
	ret = _gnix_cm_nic_progress(ep_priv[0]->domain->cm_nic);
	cr_assert_eq(ret, FI_SUCCESS);

	state = GNIX_VC_CONN_NONE;
	while (state != GNIX_VC_CONNECTED) {
		pthread_yield();
		state = _gnix_vc_state(vc_conn0);
	}

	/* We need to run CM NIC progress for an intra CM NIC connection. */
	ret = _gnix_cm_nic_progress(ep_priv[1]->domain->cm_nic);
	cr_assert_eq(ret, FI_SUCCESS);

	state = GNIX_VC_CONN_NONE;
	while (state != GNIX_VC_CONNECTED) {
		pthread_yield();
		state = _gnix_vc_state(vc_conn1);
	}

	/* VC is destroyed by the EP */
}

Test(vc_management_auto_basic, vc_connect2)
{
	__vc_connect2();
}

Test(vc_management_auto_scalable, vc_connect2)
{
	__vc_connect2();
}

static void vc_conn_ping(struct fid_ep *send_ep, struct fid_cq *send_cq,
			 fi_addr_t target_pe, void *target_addr,
			 size_t target_len, uint64_t target_key)
{
	ssize_t sz;
	int ret;
	struct fi_cq_tagged_entry cqe;
	void *context = (void *)0x65468;

	sz = fi_write(send_ep, target_buf, target_len,
			NULL, target_pe, _REM_ADDR(fi, target_buf, target_addr),
			target_key, context);
	cr_assert_eq(sz, 0);

	while ((ret = fi_cq_read(send_cq, &cqe, 1)) == -FI_EAGAIN) {
		pthread_yield();
	}

	cr_assert_eq(ret, 1);
	cr_assert(cqe.op_context == context, "CQE Context mismatch");
}

static void vc_conn_proc_cqes(struct fid_cq *cq0, void *ctx0,
			      struct fid_cq *cq1, void *ctx1)
{
	int cqe0 = 0, cqe1 = 0, ret;
	struct fi_cq_tagged_entry cqe;

	do {
		if (cq0 == cq1) {
			if ((ret = fi_cq_read(cq0, &cqe, 1)) != -FI_EAGAIN) {
				cr_assert_eq(ret, 1);
				if (cqe.op_context == ctx0) {
					cr_assert(!cqe0);
					cqe0++;
				} else if (cqe.op_context == ctx1) {
					cr_assert(!cqe1);
					cqe1++;
				} else {
					cr_assert(cqe.op_context == ctx0 ||
						  cqe.op_context == ctx1,
						  "CQE Context mismatch");
				}
			}
		} else {
			if ((ret = fi_cq_read(cq0, &cqe, 1)) != -FI_EAGAIN) {
				cr_assert(!cqe0);
				cr_assert_eq(ret, 1);
				cr_assert(cqe.op_context == ctx0,
					  "CQE Context mismatch");
				cqe0++;
			}

			if ((ret = fi_cq_read(cq1, &cqe, 1)) != -FI_EAGAIN) {
				cr_assert(!cqe1);
				cr_assert_eq(ret, 1);
				cr_assert(cqe.op_context == ctx1,
					  "CQE Context mismatch");
				cqe1++;
			}
		}

		pthread_yield();
	} while (!cqe0 || !cqe1);
}

static void vc_conn_pingpong(struct fid_ep *ep0, struct fid_cq *cq0,
			     fi_addr_t a0,
			     struct fid_ep *ep1, struct fid_cq *cq1,
			     fi_addr_t a1)
{
	int i;
	ssize_t sz;
#define DATA_LEN	64
	char b0[DATA_LEN], b1[DATA_LEN];

	for (i = 0; i < DATA_LEN; i++) {
		b0[i] = i;
	}

	sz = fi_send(ep0, b0, DATA_LEN, NULL, a0, b1);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep1, b1, DATA_LEN, NULL, a1, b0);
	cr_assert_eq(sz, 0);

	vc_conn_proc_cqes(cq0, b1, cq1, b0);

	for (i = 0; i < DATA_LEN; i++) {
		cr_assert(b1[i] == i);
		b1[i] = DATA_LEN + i;
	}

	sz = fi_send(ep1, b1, DATA_LEN, NULL, a1, b0);
	cr_assert_eq(sz, 0);

	sz = fi_recv(ep0, b0, DATA_LEN, NULL, a0, b1);
	cr_assert_eq(sz, 0);

	vc_conn_proc_cqes(cq1, b0, cq0, b1);

	for (i = 0; i < DATA_LEN; i++) {
		cr_assert(b0[i] == DATA_LEN + i);
	}
}

TestSuite(vc_conn_ping_auto_basic,
		.init = vc_conn_ping_setup_auto_basic,
		.fini = vc_conn_ping_teardown,
		.disabled = false);

TestSuite(vc_conn_ping_manual_basic,
		.init = vc_conn_ping_setup_manual_basic,
		.fini = vc_conn_ping_teardown,
		.disabled = false);

TestSuite(vc_conn_ping_auto_scalable,
		.init = vc_conn_ping_setup_auto_scalable,
		.fini = vc_conn_ping_teardown,
		.disabled = false);

TestSuite(vc_conn_ping_manual_scalable,
		.init = vc_conn_ping_setup_manual_scalable,
		.fini = vc_conn_ping_teardown,
		.disabled = false);

/* Connect EP to itself. */
Test(vc_conn_ping_manual_basic, ep_connect_self)
{
	vc_conn_ping(ep[0], cq[0], 0, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_auto_basic, ep_connect_self)
{
	vc_conn_ping(ep[0], cq[0], 0, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_manual_basic, ep_connect_self_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 0, ep[0], cq[0], 0);
}

Test(vc_conn_ping_auto_basic, ep_connect_self_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 0, ep[0], cq[0], 0);
}

Test(vc_conn_ping_manual_scalable, ep_connect_self)
{
	vc_conn_ping(ep[0], cq[0], 0, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_auto_scalable, ep_connect_self)
{
	vc_conn_ping(ep[0], cq[0], 0, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_manual_scalable, ep_connect_self_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 0, ep[0], cq[0], 0);
}

Test(vc_conn_ping_auto_scalable, ep_connect_self_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 0, ep[0], cq[0], 0);
}

/* Do intra-CM EP connect. */
Test(vc_conn_ping_manual_basic, ep_connect_intra_cm)
{
	vc_conn_ping(ep[0], cq[0], 1, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_auto_basic, ep_connect_intra_cm)
{
	vc_conn_ping(ep[0], cq[0], 1, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_manual_basic, ep_connect_intra_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 1, ep[1], cq[1], 0);
}

Test(vc_conn_ping_auto_basic, ep_connect_intra_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 1, ep[1], cq[1], 0);
}

Test(vc_conn_ping_manual_scalable, ep_connect_intra_cm)
{
	vc_conn_ping(ep[0], cq[0], 1, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_auto_scalable, ep_connect_intra_cm)
{
	vc_conn_ping(ep[0], cq[0], 1, target_buf, sizeof(target_buf), mr_key);
}

Test(vc_conn_ping_manual_scalable, ep_connect_intra_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 1, ep[1], cq[1], 0);
}

Test(vc_conn_ping_auto_scalable, ep_connect_intra_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 1, ep[1], cq[1], 0);
}

/* Do inter-CM EP connect. */
#if 0
Test(vc_conn_ping_manual, ep_connect_inter_cm)
{
	vc_conn_ping(ep[0], cq[0], 2, target_buf, sizeof(target_buf), mr_key3);
}
#endif

Test(vc_conn_ping_auto_basic, ep_connect_inter_cm)
{
	vc_conn_ping(ep[0], cq[0], 2, target_buf, sizeof(target_buf), mr_key3);
}

Test(vc_conn_ping_manual_basic, ep_connect_inter_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 2, ep3, cq3, 0);
}

Test(vc_conn_ping_auto_basic, ep_connect_inter_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 2, ep3, cq3, 0);
}

Test(vc_conn_ping_auto_scalable, ep_connect_inter_cm)
{
	vc_conn_ping(ep[0], cq[0], 2, target_buf, sizeof(target_buf), mr_key3);
}

Test(vc_conn_ping_manual_scalable, ep_connect_inter_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 2, ep3, cq3, 0);
}

Test(vc_conn_ping_auto_scalable, ep_connect_inter_cm_pp)
{
	vc_conn_pingpong(ep[0], cq[0], 2, ep3, cq3, 0);
}


