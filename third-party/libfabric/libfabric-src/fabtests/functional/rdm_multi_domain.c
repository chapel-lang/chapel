/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
 * Copyright (c) 2017, Cisco Systems, Inc. All rights reserved.
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
#include <unistd.h>

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>

#include "shared.h"

struct test_domain {
	uint8_t *buf;
	struct fid_domain *dom;
	struct fid_ep *ep;
	struct fid_cntr *rx_cntr;
	uint64_t rma_op_cnt;
	struct fid_cntr *tx_cntr;
	struct fid_av *av;
	struct fid_mr *mr;
	struct fid_cq *tx_cq;
	struct fi_context *rma_ctx;
};

struct test_domain *domain_res_array;
uint8_t *mr_buf_array;
fi_addr_t *peer_addrs;
int domain_cnt;
int verbose;

static int alloc_domain_res()
{
	domain_res_array = calloc(domain_cnt, sizeof(*domain_res_array));
	if (!domain_res_array)
		return -FI_ENOMEM;

	mr_buf_array = calloc(domain_cnt, opts.transfer_size);
	if (!mr_buf_array)
		return -FI_ENOMEM;

	peer_addrs = calloc(domain_cnt, sizeof(*peer_addrs));
	if (!peer_addrs)
		return -FI_ENOMEM;

	return 0;
}

static int open_domain_res(struct test_domain *domain)
{
	int ret;

	av_attr.type = FI_AV_MAP;
	av_attr.count = domain_cnt;
	av_attr.name = NULL;

	ret = fi_cntr_open(domain->dom,
			&cntr_attr, &domain->rx_cntr, NULL);
	if (ret) {
		FT_PRINTERR("fi_cntr_open", ret);
		return ret;
	}

	ret = fi_cntr_open(domain->dom,
			&cntr_attr, &domain->tx_cntr, NULL);
	if (ret) {
		FT_PRINTERR("fi_cntr_open", ret);
		return ret;
	}

	cq_attr.size = domain_cnt;
	ret = fi_cq_open(domain->dom,
			&cq_attr, &domain->tx_cq, NULL);
	if (ret) {
		FT_PRINTERR("fi_cq_open", ret);
		return ret;
	}

	ret = fi_av_open(domain->dom,
			&av_attr, &domain->av, NULL);
	if (ret) {
		FT_PRINTERR("fi_av_open", ret);
		return ret;
	}

	domain->rma_ctx = calloc(domain_cnt, sizeof(*domain->rma_ctx));
	if (!domain->rma_ctx)
		return -FI_ENOMEM;

	return 0;
}

static int init_ep_mr_res(struct test_domain *domain, struct fi_info *info)
{
	int ret;

	domain->rma_op_cnt = 0;

	ret = fi_endpoint(domain->dom, info, &domain->ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	ret = fi_ep_bind(domain->ep,
			&domain->av->fid, 0);
	if (ret) {
		FT_PRINTERR("fi_ep_bind", ret);
		return ret;
	}

	ret = fi_ep_bind(domain->ep,
			&domain->tx_cntr->fid, FI_WRITE);
	if (ret) {
		FT_PRINTERR("fi_ep_bind", ret);
		return ret;
	}

	ret = fi_ep_bind(domain->ep,
			&domain->tx_cq->fid, FI_TRANSMIT);
	if (ret) {
		FT_PRINTERR("fi_ep_bind", ret);
		return ret;
	}

	ret = fi_enable(domain->ep);
	if (ret) {
		FT_PRINTERR("fi_enable", ret);
		return ret;
	}

	ret = fi_mr_reg(domain->dom, domain->buf,
			opts.transfer_size, FI_WRITE | FI_REMOTE_WRITE,
			0, FT_MR_KEY, 0, &domain->mr, NULL);
	if (ret) {
		FT_PRINTERR("fi_mr_reg", ret);
		return ret;
	}

	if (verbose) {
		printf("fi_mr_reg(fi_domain = %p,buffer = %p,size = %zu,",
				domain->dom, domain->buf, opts.transfer_size);
		printf("memory_region = %p,mr_key %ld)\n",
				domain->mr, (unsigned long)fi_mr_key(domain->mr));
	}

	ret = fi_mr_bind(domain->mr,
			&domain->rx_cntr->fid,
			FI_REMOTE_WRITE);
	if (ret) {
		FT_PRINTERR("fi_mr_bind", ret);
		return ret;
	}

	return 0;
}

static int init_peer_addresses()
{
	int dom_idx, ret;
	int ep_idx;

	/*
	Initializes the Address Vectors for each domain
	with the addresses of the remote hosts' endpoints.
	The fi_addr_t array peer_addrs gets rewritten
	in this loop with the same set of addresses for each domain
	init, but the peer addresses to the remote domains
	are the same so this is not an issue.
	The address vectors for each domain must be init even if
	the peer addresses returned is the same after the first loop.
	*/
	for (dom_idx = 0; dom_idx < domain_cnt; dom_idx++) {
		for (ep_idx = 0; ep_idx < domain_cnt; ep_idx++) {
			ret = ft_init_av_addr(domain_res_array[dom_idx].av,
					domain_res_array[ep_idx].ep, &peer_addrs[ep_idx]);
			if (ret)
				return ret;
		}
	}

	return 0;
}

static int init_domain_res()
{
	int dom_idx, ret;

	for (dom_idx = 0; dom_idx < domain_cnt; dom_idx++) {
		struct fi_info *info;
		hints->src_addr = NULL;
		ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &info);
		if (ret) {
			FT_PRINTERR("fi_getinfo", ret);
			return ret;
		}

		ret = fi_domain(fabric, info,
				&domain_res_array[dom_idx].dom,
				NULL);
		if (ret) {
			FT_PRINTERR("fi_domain", ret);
			return ret;
		}

		domain_res_array[dom_idx].buf = &mr_buf_array[dom_idx];

		ret = open_domain_res(&domain_res_array[dom_idx]);
		if (ret)
			return ret;

		ret = init_ep_mr_res(&domain_res_array[dom_idx], info);
		if (ret)
			return ret;

		fi_freeinfo(info);
	}

	return 0;
}

static void free_domain_res()
{
	int dom_idx;

	for (dom_idx = 0; dom_idx < domain_cnt; dom_idx++) {
		FT_CLOSE_FID(domain_res_array[dom_idx].ep);
		FT_CLOSE_FID(domain_res_array[dom_idx].av);
		FT_CLOSE_FID(domain_res_array[dom_idx].mr);
		FT_CLOSE_FID(domain_res_array[dom_idx].tx_cntr);
		FT_CLOSE_FID(domain_res_array[dom_idx].rx_cntr);
		FT_CLOSE_FID(domain_res_array[dom_idx].tx_cq);
		FT_CLOSE_FID(domain_res_array[dom_idx].dom);
		free(domain_res_array[dom_idx].rma_ctx);
	}
	free(peer_addrs);
	free(mr_buf_array);
	free(domain_res_array);
}

static int write_data(void *buffer, size_t size, int dom_idx,
		int remote_dom_idx, struct fi_context *rma_ctx)
{
	int ret = -FI_EAGAIN;

	while (ret == -FI_EAGAIN) {
		ret = fi_write(domain_res_array[dom_idx].ep, buffer, size,
				fi_mr_desc(domain_res_array[dom_idx].mr), peer_addrs[remote_dom_idx],
				0, fi_mr_key(domain_res_array[remote_dom_idx].mr), rma_ctx);
	}

	if (ret)
		return ret;

	return 0;
}

static int wait_write_cq(struct fid_cq *cq, size_t rma_op_count)
{
	struct fi_cq_tagged_entry entry;
	int ret;
	int op_counter = 0;

	do {
		ret = fi_cq_read(cq, &entry, 1);
		if (ret > 0)
			op_counter++;
	} while ((ret == -FI_EAGAIN) && (op_counter < rma_op_count));

	if (ret < 0) {
		if (ret == -FI_EAVAIL) {
			ret = ft_cq_readerr(cq);
		} else {
			FT_PRINTERR("fi_cq_read", ret);
		}
	}

	return ret;
}

static int multi_domain_test()
{
	int ret, remote_dom_idx, dom_idx;

	ft_sync();

	for (dom_idx = 0; dom_idx < domain_cnt; dom_idx++) {
		for (remote_dom_idx = 0; remote_dom_idx < domain_cnt; remote_dom_idx++) {
			if (verbose)
				printf("write to host's domain idx %d, memory region\n", remote_dom_idx);

			ret = write_data(tx_buf, opts.transfer_size, dom_idx,
				remote_dom_idx, &domain_res_array[dom_idx].rma_ctx[remote_dom_idx]);
			if (ret)
				return ret;

			domain_res_array[dom_idx].rma_op_cnt++;
		}
	}

	for (dom_idx = 0; dom_idx < domain_cnt; dom_idx++) {
		ret = fi_cntr_wait(domain_res_array[dom_idx].tx_cntr,
				domain_res_array[dom_idx].rma_op_cnt, 30 * 1000);
		if (ret < 0)
			return ret;

		ret = wait_write_cq(domain_res_array[dom_idx].tx_cq,
				domain_res_array[dom_idx].rma_op_cnt);
		if (ret < 1)
			return ret;

		if (verbose)
			printf("writes from domain idx %d completed\n", dom_idx);
	}

	for (dom_idx = 0; dom_idx < domain_cnt; dom_idx++) {
		if (verbose)
			printf("check for writes to domain idx %d\n", dom_idx);

		ret = fi_cntr_wait(domain_res_array[dom_idx].rx_cntr,
				domain_res_array[dom_idx].rma_op_cnt, 30 * 1000);
		if (ret < 0)
			return ret;

		if (verbose)
			printf("got all writes to domain idx %d\n", dom_idx);
	}
	printf("GOOD, multi domain test complete\n");
	return 0;
}

static int run_test(void)
{
	int ret;

	ret = ft_init_fabric();
	if (ret)
		return ret;

	ret = alloc_domain_res();
	if (ret)
		return ret;
	ret = init_domain_res();
	if (ret)
		return ret;

	ret = init_peer_addresses();
	if (ret)
		return ret;

	ret = multi_domain_test();

	return 0;
}

int main(int argc, char **argv)
{
	int op, ret;

	domain_cnt = 2;
	opts = INIT_OPTS;
	opts.transfer_size = 4096;
	verbose = 0;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "c:Vh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'c':
			domain_cnt = strtoull(optarg, NULL, 10);
			break;
		case 'V':
			verbose = 1;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "Ping-pong multi-domain test");
			FT_PRINT_OPTS_USAGE("-c <int>", "number of domains to create and test");
			FT_PRINT_OPTS_USAGE("-V", "Enable verbose printing");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_RMA | FI_RMA_EVENT | FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run_test();

	if (domain_res_array)
		free_domain_res();
	ft_free_res();

	return ft_exit_code(ret);
}
