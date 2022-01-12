/*
 * Copyright (c) 2017-2019 Intel Corporation. All rights reserved.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHWARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. const NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER const AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS const THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <stdarg.h>

#include <rdma/fi_errno.h>
#include <rdma/fi_domain.h>
#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_trigger.h>

#include <core.h>
#include <pattern.h>
#include <shared.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <hmem.h>

char *tx_barrier;
char *rx_barrier;
struct fid_mr *mr_barrier;
struct fi_context2 *barrier_tx_ctx, *barrier_rx_ctx;

struct pattern_ops *pattern;
struct multinode_xfer_state state;
struct multi_xfer_method method;
struct multi_xfer_method multi_xfer_methods[] = {
	{
		.name = "send/recv",
		.send = multi_msg_send,
		.recv = multi_msg_recv,
		.wait = multi_msg_wait,
	},
	{
		.name = "rma",
		.send = multi_rma_write,
		.recv = multi_rma_recv,
		.wait = multi_rma_wait,
	}
};

static int multi_setup_fabric(int argc, char **argv)
{
	char my_name[FT_MAX_CTRL_MSG];
	size_t len;
	int i, ret;
	struct fi_rma_iov remote;

	hints->ep_attr->type = FI_EP_RDM;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	if (pm_job.transfer_method == multi_msg) {
		hints->caps = FI_MSG;
	} else if (pm_job.transfer_method == multi_rma) {
		hints->caps = FI_MSG | FI_RMA;
	} else {
		printf("Not a valid cabability\n");
		return -FI_ENODATA;
	}

	method = multi_xfer_methods[pm_job.transfer_method];

	tx_seq = 0;
	rx_seq = 0;
	tx_cq_cntr = 0;
	rx_cq_cntr = 0;

	ret = ft_hmem_init(opts.iface);
	if (ret)
		return ret;

	if (pm_job.my_rank != 0)
		pm_barrier();

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	opts.av_size = pm_job.num_ranks;
	ret = ft_alloc_active_res(fi);
	if (ret)
		return ret;

	ret = ft_enable_ep(ep, eq, av, txcq, rxcq, txcntr, rxcntr);
	if (ret)
		return ret;

	len = FT_MAX_CTRL_MSG;
	ret = fi_getname(&ep->fid, (void *) my_name, &len);
	if (ret) {
		FT_PRINTERR("error determining local endpoint name\n", ret);
		goto err;
	}

	pm_job.name_len = 256;
	pm_job.names = malloc(pm_job.name_len * pm_job.num_ranks);
	if (!pm_job.names) {
		FT_ERR("error allocating memory for address exchange\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	if (pm_job.my_rank == 0)
		pm_barrier();

	ret = pm_allgather(my_name, pm_job.names, pm_job.name_len);
	if (ret) {
		FT_PRINTERR("error exchanging addresses\n", ret);
		goto err;
	}

	pm_job.fi_addrs = calloc(pm_job.num_ranks, sizeof(*pm_job.fi_addrs));
	if (!pm_job.fi_addrs) {
		FT_ERR("error allocating memory for av fi addrs\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	for (i = 0; i < pm_job.num_ranks; i++) {
		ret = fi_av_insert(av, (char*)pm_job.names + i * pm_job.name_len, 1,
			   &pm_job.fi_addrs[i], 0, NULL);
		if (ret != 1) {
			FT_ERR("unable to insert all addresses into AV table\n");
			ret = -1;
			goto err;
		}
	}

	pm_job.multi_iovs = malloc(sizeof(*(pm_job.multi_iovs)) * pm_job.num_ranks);
	if (!pm_job.multi_iovs) {
		FT_ERR("error allocation memory for rma_iovs\n");
		goto err;
	}

	if (fi->domain_attr->mr_mode & FI_MR_VIRT_ADDR)
		remote.addr = (uintptr_t) rx_buf;
	else
		remote.addr = 0;

	remote.key = fi_mr_key(mr);
	remote.len = rx_size;

	ret = pm_allgather(&remote, pm_job.multi_iovs, sizeof(remote));
	if (ret) {
		FT_ERR("error exchanging rma_iovs\n");
		goto err;
	}
	for (i = 0; i < pm_job.num_ranks; i++) {
		pm_job.multi_iovs[i].addr += (tx_size * pm_job.my_rank);
	}

	return 0;
err:
	ft_free_res();
	return ft_exit_code(ret);
}

static int ft_progress(struct fid_cq *cq, uint64_t total, uint64_t *cq_cntr)
{
	struct fi_cq_err_entry comp;
	int ret;

	ret = fi_cq_read(cq, &comp, 1);
	if (ret > 0)
		(*cq_cntr)++;

	if (ret >= 0 || ret == -FI_EAGAIN)
		return 0;

	if (ret == -FI_EAVAIL) {
		ret = ft_cq_readerr(cq);
		(*cq_cntr)++;
	} else {
		FT_PRINTERR("fi_cq_read/sread", ret);
	}
	return ret;
}

int multi_msg_recv()
{
	int ret, offset;

	/* post receives */
	while (!state.all_recvs_posted && state.rx_window) {

		ret = pattern->next_source(&state.cur_source);
		if (ret == -FI_ENODATA) {
			state.all_recvs_posted = true;
			break;
		} else if (ret < 0) {
			return ret;
		}

		offset = state.recvs_posted % opts.window_size ;
		assert(rx_ctx_arr[offset].state == OP_DONE);

		ret = ft_post_rx_buf(ep, opts.transfer_size,
				     &rx_ctx_arr[offset].context,
				     rx_ctx_arr[offset].buf,
				     rx_ctx_arr[offset].desc, 0);
		if (ret)
			return ret;

		rx_ctx_arr[offset].state = OP_PENDING;
		state.recvs_posted++;
		state.rx_window--;
	}
	return 0;
}

int multi_msg_send()
{
	int ret, offset;
	fi_addr_t dest;

	while (!state.all_sends_posted && state.tx_window) {

		ret = pattern->next_target(&state.cur_target);
		if (ret == -FI_ENODATA) {
			state.all_sends_posted = true;
			break;
		} else if (ret < 0) {
			return ret;
		}

		offset = state.sends_posted % opts.window_size;
		assert(tx_ctx_arr[offset].state == OP_DONE);

		dest = pm_job.fi_addrs[state.cur_target];
		ret = ft_post_tx_buf(ep, dest, opts.transfer_size,
				     NO_CQ_DATA,
				     &tx_ctx_arr[offset].context,
				     tx_ctx_arr[offset].buf,
				     tx_ctx_arr[offset].desc, 0);
		if (ret)
			return ret;

		tx_ctx_arr[offset].state = OP_PENDING;
		state.sends_posted++;
		state.tx_window--;
	}
	return 0;
}

int multi_msg_wait()
{
	int ret, i;

	ret = ft_get_tx_comp(tx_seq);
	if (ret)
		return ret;

	ret = ft_get_rx_comp(rx_seq);
	if (ret)
		return ret;

	for (i = 0; i < opts.window_size; i++) {
		rx_ctx_arr[i].state = OP_DONE;
		tx_ctx_arr[i].state = OP_DONE;
	}

	state.rx_window = opts.window_size;
	state.tx_window = opts.window_size;

	if (state.all_recvs_posted && state.all_sends_posted)
		state.all_completions_done = true;

	return 0;
}

int multi_rma_write()
{
	int ret, rc;

	while (!state.all_sends_posted && state.tx_window) {

		ret = pattern->next_target(&state.cur_target);
		if (ret == -FI_ENODATA) {
			state.all_sends_posted = true;
			break;
		} else if (ret < 0) {
			return ret;
		}

		snprintf((char*) tx_buf + tx_size * state.cur_target, tx_size,
		        "Hello World! from %zu to %i on the %zuth iteration, %s test",
		        pm_job.my_rank, state.cur_target,
		        (size_t) tx_seq, pattern->name);

		while (1) {
			ret = fi_write(ep,
				tx_buf + tx_size * state.cur_target,
				opts.transfer_size, mr_desc,
				pm_job.fi_addrs[state.cur_target],
				pm_job.multi_iovs[state.cur_target].addr,
				pm_job.multi_iovs[state.cur_target].key,
				&tx_ctx_arr[state.tx_window].context);
			if (!ret)
				break;

			if (ret != -FI_EAGAIN) {
				printf("RMA write failed");
				return ret;
			}

			rc = ft_progress(txcq, tx_seq, &tx_cq_cntr);
			if (rc && rc != -FI_EAGAIN) {
				printf("Failed to get rma completion");
				return rc;
			}
		}
		tx_seq++;

		state.sends_posted++;
		state.tx_window--;
	}
	return 0;
}

int multi_rma_recv()
{
	state.all_recvs_posted = true;
	return 0;
}

int multi_rma_wait()
{
	int ret;

	ret = ft_get_tx_comp(tx_seq);
	if (ret)
		return ret;

	state.rx_window = opts.window_size;
	state.tx_window = opts.window_size;

	if (state.all_recvs_posted && state.all_sends_posted)
		state.all_completions_done = true;

	return 0;
}

int send_recv_barrier(int sync)
{
	int ret, i;

	for(i = 0; i < pm_job.num_ranks; i++) {

		ret = ft_post_rx_buf(ep, opts.transfer_size,
			     &barrier_rx_ctx[i],
			     rx_buf, mr_desc, 0);
		if (ret)
			return ret;
	}

	for (i = 0; i < pm_job.num_ranks; i++) {
		ret = ft_post_tx_buf(ep, pm_job.fi_addrs[i], 0,
				     NO_CQ_DATA, &barrier_tx_ctx[i],
		                     tx_buf, mr_desc, 0);
		if (ret)
			return ret;
	}

	ret = ft_get_tx_comp(tx_seq);
	if (ret)
		return ret;

	ret = ft_get_rx_comp(rx_seq);

	return ret;
}

static inline void multi_init_state()
{
	state.cur_source = PATTERN_NO_CURRENT;
	state.cur_target = PATTERN_NO_CURRENT;

	state.all_completions_done = false;
	state.all_recvs_posted = false;
	state.all_sends_posted = false;

	state.rx_window = opts.window_size;
	state.tx_window = opts.window_size;
}

static int multi_run_test()
{
	int ret;
	int iter;

	for (iter = 0; iter < opts.iterations; iter++) {

		multi_init_state();
		while (!state.all_completions_done ||
				!state.all_recvs_posted ||
				!state.all_sends_posted) {
			ret = method.recv();
			if (ret)
				return ret;

			ret = method.send();
			if (ret)
				return ret;

			ret = method.wait();
			if (ret)
				return ret;
		}

		ret = send_recv_barrier(iter);
		if (ret)
			return ret;
	}
	return 0;
}

static void pm_job_free_res()
{
	free(pm_job.names);
	free(pm_job.fi_addrs);
	free(pm_job.multi_iovs);

	free(barrier_tx_ctx);
	free(barrier_rx_ctx);

	FT_CLOSE_FID(mr_barrier);
}

int multinode_run_tests(int argc, char **argv)
{
	int ret = FI_SUCCESS;
	int i;


	barrier_tx_ctx = malloc(sizeof(*barrier_tx_ctx) * pm_job.num_ranks);
	if (!barrier_tx_ctx)
		return -FI_ENOMEM;

	barrier_rx_ctx = malloc(sizeof(*barrier_rx_ctx) * pm_job.num_ranks);
	if (!barrier_rx_ctx)
		return -FI_ENOMEM;

	ret = multi_setup_fabric(argc, argv);
	if (ret)
		return ret;


	for (i = 0; i < NUM_TESTS && !ret; i++) {
		printf("starting %s... ", patterns[i].name);
		pattern = &patterns[i];
		ret = multi_run_test();
		if (ret)
			printf("failed\n");
		else
			printf("passed\n");

		fflush(stdout);
	}

	pm_job_free_res();
	ft_free_res();
	return ft_exit_code(ret);
}

