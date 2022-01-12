/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>

#include "shared.h"


static size_t concurrent_msgs = 5;
static size_t num_iters = 600;
static bool send_data = false;


/* Common code will free allocated buffers and MR */
static int alloc_bufs(void)
{
	int ret;

	tx_size = opts.transfer_size + ft_tx_prefix_size();
	rx_size = opts.transfer_size + ft_rx_prefix_size();
	buf_size = (tx_size + rx_size) * concurrent_msgs;

	buf = malloc(buf_size);
	tx_ctx_arr = calloc(concurrent_msgs, sizeof(*tx_ctx_arr));
	rx_ctx_arr = calloc(concurrent_msgs, sizeof(*rx_ctx_arr));
	if (!buf || !tx_ctx_arr || !rx_ctx_arr)
		return -FI_ENOMEM;

	rx_buf = buf;
	tx_buf = (char *) buf + rx_size * concurrent_msgs;

	if (fi->domain_attr->mr_mode & FI_MR_LOCAL) {
		ret = fi_mr_reg(domain, buf, buf_size, FI_SEND | FI_RECV,
				 0, FT_MR_KEY, 0, &mr, NULL);
		if (ret)
			return ret;

		mr_desc = fi_mr_desc(mr);
	}

	return 0;
}

static char *get_tx_buf(int index)
{
	return tx_buf + tx_size * index;
}

static char *get_rx_buf(int index)
{
	return rx_buf + rx_size * index;
}

static int wait_recv(void)
{
	struct fi_cq_tagged_entry entry;
	int ret;

	if (opts.comp_method == FT_COMP_SREAD) {
		ret = fi_cq_sread(rxcq, &entry, 1, NULL, -1);
	} else {
		do {
			ret = fi_cq_read(rxcq, &entry, 1);
		} while (ret == -FI_EAGAIN);
	}

	if ((ret == 1) && send_data) {
		if (entry.data != opts.transfer_size) {
			printf("ERROR incorrect remote CQ data value. Got %lu,"
			       " expected %zu\n", (unsigned long)entry.data,
			       opts.transfer_size);
			return -FI_EOTHER;
		}
	}

	if (ret < 1)
		printf("ERROR fi_cq_(s)read returned %d %s\n", ret, fi_strerror(-ret));
	return ret;
}

static int run_test_loop(void)
{
	int ret = 0;
	uint64_t op_data = send_data ? opts.transfer_size : NO_CQ_DATA;
	uint64_t op_tag = 0x1234;
	char *op_buf;
	int i, j;

	for (i = 0; i < num_iters; i++) {
		for (j = 0; j < concurrent_msgs; j++) {
			op_buf = get_tx_buf(j);
			if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
				ret = ft_fill_buf(op_buf + ft_tx_prefix_size(),
						  opts.transfer_size);
				if (ret)
					return ret;
			}

			ret = ft_post_tx_buf(ep, remote_fi_addr,
					     opts.transfer_size,
					     op_data, &tx_ctx_arr[j].context,
					     op_buf, mr_desc, op_tag + j);
			if (ret) {
				printf("ERROR send_msg returned %d\n", ret);
				return ret;
			}

			/* Request send progress */
			(void) fi_cq_read(txcq, NULL, 0);
		}

		ret = ft_sync();
		if (ret)
			return ret;

		for (j = 0; j < concurrent_msgs; j++) {
			op_buf = get_rx_buf(j);
			ret = ft_post_rx_buf(ep, opts.transfer_size,
					     &rx_ctx_arr[j].context, op_buf,
					     mr_desc,
					     op_tag + (concurrent_msgs - 1) - j);
			if (ret) {
				printf("ERROR recv_msg returned %d\n", ret);
				return ret;
			}

			/* Progress sends */
			(void) fi_cq_read(txcq, NULL, 0);

			ret = wait_recv();
			if (ret < 1)
				return ret;
		}

		if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
			for (j = 0; j < concurrent_msgs; j++) {
				op_buf = get_rx_buf(j);
				if (ft_check_buf(op_buf + ft_rx_prefix_size(),
						 opts.transfer_size))
					return -FI_EOTHER;
			}
		}

		for (j = 0; j < concurrent_msgs; j++) {
			ret = ft_get_tx_comp(tx_seq);
			if (ret)
				return ret;
		}

		if (i % 100 == 0)
			printf("PID %d GOOD iter %d/%ld completed\n",
				getpid(), i, num_iters);
	}

	(void) ft_sync();
	printf("PID %d GOOD all done\n", getpid());
	return ret;
}

static int run_test(void)
{
	int ret;

	if (hints->ep_attr->type == FI_EP_MSG)
		ret = ft_init_fabric_cm();
	else
		ret = ft_init_fabric();
	if (ret)
		return ret;

	alloc_bufs();
	ret = run_test_loop();

	return ret;
}

int main(int argc, char **argv)
{
	int op;
	int ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_OOB_CTRL | FT_OPT_SKIP_MSG_ALLOC;
	opts.mr_mode = FI_MR_LOCAL | FI_MR_ALLOCATED;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "CM:h" CS_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parsecsopts(op, optarg, &opts);
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'C':
			send_data = true;
			break;
		case 'M':
			concurrent_msgs = strtoul(optarg, NULL, 0);
			break;
		case '?':
		case 'h':
			ft_csusage(argv[0], "Unexpected message handling test.");
			FT_PRINT_OPTS_USAGE("-C", "transfer remote CQ data");
			FT_PRINT_OPTS_USAGE("-M <count>", "number of concurrent msgs");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;
	hints->domain_attr->resource_mgmt = FI_RM_ENABLED;
	hints->rx_attr->total_buffered_recv = 0;
	hints->caps = FI_TAGGED;

	ret = run_test();

	ft_free_res();
	return ft_exit_code(ret);
}
