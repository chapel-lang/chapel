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

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>

#include "shared.h"

static struct fid_ep **eps;
static char *data_bufs;
static char **send_bufs;
static char **recv_bufs;
static struct fi_context *recv_ctx;
static struct fi_context *send_ctx;
static fi_addr_t *remote_addr;
int num_eps = 3;


static int alloc_multi_ep_res()
{
	char *rx_buf_ptr;
	int i;

	eps = calloc(num_eps, sizeof(*eps));
	remote_addr = calloc(num_eps, sizeof(*remote_addr));
	send_bufs = calloc(num_eps, sizeof(*send_bufs));
	recv_bufs = calloc(num_eps, sizeof(*recv_bufs));
	send_ctx = calloc(num_eps, sizeof(*send_ctx));
	recv_ctx = calloc(num_eps, sizeof(*recv_ctx));
	data_bufs = calloc(num_eps * 2, opts.transfer_size);

	if (!eps || !remote_addr || !send_bufs || !recv_bufs ||
	    !send_ctx || !recv_ctx || !data_bufs)
		return -FI_ENOMEM;

	rx_buf_ptr = data_bufs + opts.transfer_size * num_eps;
	for (i = 0; i < num_eps; i++) {
		send_bufs[i] = data_bufs + opts.transfer_size * i;
		recv_bufs[i] = rx_buf_ptr + opts.transfer_size * i;
	}

	return 0;
}

static void free_ep_res()
{
	int i;

	for (i = 0; i < num_eps; i++)
		FT_CLOSE_FID(eps[i]);

	free(data_bufs);
	free(send_bufs);
	free(recv_bufs);
	free(send_ctx);
	free(recv_ctx);
	free(remote_addr);
	free(eps);
}

static int do_transfers(void)
{
	int i, ret;

	for (i = 0; i < num_eps; i++) {
		rx_buf = recv_bufs[i];
		ret = ft_post_rx(eps[i], opts.transfer_size, &recv_ctx[i]);
		if (ret)
			return ret;
	}

	for (i = 0; i < num_eps; i++) {
		if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
			ret = ft_fill_buf(send_bufs[i], opts.transfer_size);
			if (ret)
				return ret;
		}

		tx_buf = send_bufs[i];
		ret = ft_post_tx(eps[i], remote_addr[i], opts.transfer_size, NO_CQ_DATA, &send_ctx[i]);
		if (ret)
			return ret;
	}

	ret = ft_get_tx_comp(num_eps);
	if (ret < 0)
		return ret;

	ret = ft_get_rx_comp(num_eps);
	if (ret < 0)
		return ret;

	if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
		for (i = 0; i < num_eps; i++) {
			ret = ft_check_buf(recv_bufs[i], opts.transfer_size);
			if (ret)
				return ret;
		}
	}

	for (i = 0; i < num_eps; i++)
		ft_finalize_ep(eps[i]);

	printf("PASSED multi ep\n");
	return 0;
}

static int setup_client_ep(struct fid_ep **ep)
{
	int ret;

	ret = fi_endpoint(domain, fi, ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	ret = ft_enable_ep(*ep, eq, av, txcq, rxcq, txcntr, rxcntr);
	if (ret)
		return ret;

	ret = ft_connect_ep(*ep, eq, fi->dest_addr);
	if (ret)
		return ret;

	return 0;
}

static int setup_server_ep(struct fid_ep **ep)
{
	int ret;

	ret = ft_retrieve_conn_req(eq, &fi);
	if (ret)
		goto failed_accept;

	ret = fi_endpoint(domain, fi, ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		goto failed_accept;
	}

	ret = ft_enable_ep(*ep, eq, av, txcq, rxcq, txcntr, rxcntr);
	if (ret)
		goto failed_accept;

	ret = ft_accept_connection(*ep, eq);
	if (ret)
		goto failed_accept;

	return 0;

failed_accept:
	fi_reject(pep, fi->handle, NULL, 0);
	return ret;
}

static int setup_av_ep(struct fid_ep **ep, fi_addr_t *remote_addr)
{
	int ret;
	hints->src_addr = NULL;

	fi_freeinfo(fi);

	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		FT_PRINTERR("fi_getinfo", ret);
		return ret;
	}

	ret = fi_endpoint(domain, fi, ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	ret = ft_enable_ep(*ep, eq, av, txcq, rxcq, txcntr, rxcntr);
	if (ret)
		return ret;

	ret = ft_init_av_addr(av, *ep, remote_addr);
	if (ret)
		return ret;

	return 0;
}

static int run_test(void)
{
	int i, ret;

	ret = alloc_multi_ep_res();
	if (ret)
		return ret;

	if (hints->ep_attr->type == FI_EP_MSG) {
		ret = ft_init_fabric_cm();
		if (ret)
			return ret;
	} else {
		opts.av_size = num_eps + 1;
		ret = ft_init_fabric();
		if (ret)
			return ret;
	}

	/* Create additional endpoints. */
	for (i = 0; i < num_eps; i++) {
		if (hints->ep_attr->type == FI_EP_MSG) {
			if (opts.dst_addr) {
				ret = setup_client_ep(&eps[i]);
				if (ret)
					return ret;
			} else {
				ret = setup_server_ep(&eps[i]);
				if (ret)
					return ret;
			}
		} else {
			ret = setup_av_ep(&eps[i], &remote_addr[i]);
			if (ret)
				return ret;
		}
	}

	tx_cq_cntr = rx_cq_cntr = 0;
	tx_seq = rx_seq = 0;
	ret = do_transfers();
	if (ret)
		return ret;

	return 0;
}

int main(int argc, char **argv)
{
	int op;
	int ret = 0;

	opts = INIT_OPTS;
	opts.transfer_size = 256;
	opts.options |= FT_OPT_SKIP_REG_MR;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "c:vh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'c':
			num_eps = atoi(optarg);
			break;
		case 'v':
			opts.options |= FT_OPT_VERIFY_DATA;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "Multi endpoint test");
			FT_PRINT_OPTS_USAGE("-c <int>",
				"number of endpoints to create and test (def 3)");
			FT_PRINT_OPTS_USAGE("-v", "Enable data verification");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = FI_MSG;
	hints->mode = FI_CONTEXT;

	ret = run_test();

	free_ep_res();
	ft_free_res();
	return ft_exit_code(ret);
}
