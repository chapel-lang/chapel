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

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>

#include "shared.h"

struct test_mr {
	uint8_t *buf;
	struct fid_cntr *rcntr;
	uint64_t rcntr_next;
	struct fid_mr *mr;
	struct fi_rma_iov *remote;
};

struct test_mr *mr_res_array;
uint8_t *mr_buf_array;
struct fi_rma_iov *remote_array;
uint64_t mr_count;
int verbose;

static int wait_cntr(struct fid_cntr *cntr, uint64_t *cntr_next)
{
	/* 30 second wait timeout */
	int ret = fi_cntr_wait(cntr, *cntr_next, 30 * 1000);
	if (ret < 0)
		return ret;

	*cntr_next += 1;

	return 0;
}

static int free_mr_res()
{
	int i;

	if (!mr_res_array)
		return 0;

	for (i = 0; i < mr_count; i++) {
		FT_CLOSE_FID(mr_res_array[i].mr);
		FT_CLOSE_FID(mr_res_array[i].rcntr);
	}
	free(mr_res_array);
	free(remote_array);
	free(mr_buf_array);

	return 0;
}

static int alloc_multi_mr_res()
{
	int i = 0;

	mr_res_array = calloc(mr_count, sizeof(*mr_res_array));
	if (!mr_res_array)
		return -FI_ENOMEM;

	remote_array = calloc(mr_count, sizeof(*remote_array));
	if (!remote_array)
		return -FI_ENOMEM;

	mr_buf_array = calloc(mr_count, opts.transfer_size);
	if (!mr_buf_array)
		return -FI_ENOMEM;

	for (i = 0; i < mr_count; i++) {
		mr_res_array[i].remote = &remote_array[i];
		mr_res_array[i].buf = &mr_buf_array[i];
	}

	return 0;
}

static int init_multi_mr_res()
{
	int ret = 0, i;

	if ((1ULL << fi->domain_attr->mr_key_size) < mr_count) {
		fprintf(stderr, "ERROR: too many memory regions for unique mr keys");
		return -FI_EINVAL;
	}

	ret = alloc_multi_mr_res();
	if (ret)
		return ret;

	for (i = 0; i < mr_count; i++) {
		ret = fi_cntr_open(domain,
				&cntr_attr,
				&mr_res_array[i].rcntr,
				NULL);
		if (ret) {
			FT_PRINTERR("fi_cntr_open", ret);
			return ret;
		}

		mr_res_array[i].remote->len = opts.transfer_size;
		mr_res_array[i].remote->addr = 0;

		memset(mr_res_array[i].buf, 0, opts.transfer_size);

		mr_res_array[i].rcntr_next = 1;

		mr_res_array[i].remote->key = i + 1;

		ret = fi_mr_reg(domain, mr_res_array[i].buf,
				opts.transfer_size, FI_REMOTE_WRITE,
				0, mr_res_array[i].remote->key,
				0, &mr_res_array[i].mr, NULL);
		if (ret) {
			FT_PRINTERR("fi_mr_reg", ret);
			return ret;
		}

		if (verbose) {
			printf("MR_REG:domain_ptr, buf_ptr, mr_size, mr_ptr, mr_key)\n");
			printf("%p, %p, %zu, %p, %lu)\n",
					domain,
					mr_res_array[i].buf,
					opts.transfer_size,
					&mr_res_array[i].mr,
					(unsigned long)fi_mr_key(mr_res_array[i].mr));
		}

		ret = fi_mr_bind(mr_res_array[i].mr,
				&mr_res_array[i].rcntr->fid,
				FI_REMOTE_WRITE);
		if (ret) {
			FT_PRINTERR("fi_mr_bind", ret);
			return ret;
		}
	}

	return ret;
}

static int mr_key_test()
{
	int i, ret = 0;
	struct fi_context rma_ctx;

	for (i = 0; i < mr_count; i++) {
		tx_buf = (char *)mr_res_array[i].buf;

		if (opts.dst_addr) {
			ret = ft_fill_buf(mr_res_array[i].buf,
					  opts.transfer_size);
			if (ret)
				return ret;

			if (verbose)
				printf("write to host's key %lx\n",
						(unsigned long)fi_mr_key(mr_res_array[i].mr));

			ft_post_rma(FT_RMA_WRITE, ep, opts.transfer_size,
					mr_res_array[i].remote, &rma_ctx);

			if (verbose)
				printf("sent successfully\n");

			ret = wait_cntr(mr_res_array[i].rcntr,
					&mr_res_array[i].rcntr_next);
			if (ret)
				return ret;

			if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
				if (verbose)
					printf("checking result in buffer %p, key %lx\n",
							mr_res_array[i].buf,
							(unsigned long)fi_mr_key(mr_res_array[i].mr));

				ret = ft_check_buf(mr_res_array[i].buf,
						opts.transfer_size);
				if (ret)
					return ret;
			}
		} else {
			ret = wait_cntr(mr_res_array[i].rcntr,
					&mr_res_array[i].rcntr_next);
			if (ret)
				return ret;

			if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
				if (verbose)
					printf("checking result in buffer %p, key %lx\n",
							mr_res_array[i].buf,
							(unsigned long)fi_mr_key(mr_res_array[i].mr));

				ret = ft_check_buf(mr_res_array[i].buf,
						opts.transfer_size);
				if (ret)
					return ret;
			}

			ret = ft_fill_buf(mr_res_array[i].buf,
					  opts.transfer_size);
			if (ret)
				return ret;

			if (verbose)
				printf("write to client's key %lx\n",
						(unsigned long)fi_mr_key(mr_res_array[i].mr));

			ft_post_rma(FT_RMA_WRITE, ep, opts.transfer_size,
					mr_res_array[i].remote, &rma_ctx);

			if (verbose)
					printf("sent successfully\n");
		}
	}
	printf("GOOD, multi mr key test complete\n");

	return ret;
}

static int run_test(void)
{
	int ret = 0;

	ft_mr_alloc_func = init_multi_mr_res;

	if (hints->ep_attr->type == FI_EP_MSG)
		ret = ft_init_fabric_cm();
	else
		ret = ft_init_fabric();
	if (ret)
		return ret;

	ret = mr_key_test();

	ft_sync();
	ft_finalize();

	return ret;
}

int main(int argc, char **argv)
{
	int op;
	int ret = 0;

	opts = INIT_OPTS;
	opts.transfer_size = 4096;
	mr_count = 2;
	verbose = 0;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "c:Vvh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'c':
			mr_count = strtoull(optarg, NULL, 10);
			break;
		case 'V':
			verbose = 1;
			break;
		case 'v':
			opts.options |= FT_OPT_VERIFY_DATA;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "Ping-pong multi memory region test");
			FT_PRINT_OPTS_USAGE("-c <int>",
				"number of memory regions to create and test");
			FT_PRINT_OPTS_USAGE("-V", "Enable verbose printing");
			FT_PRINT_OPTS_USAGE("-v", "Enable data verification");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = FI_RMA | FI_RMA_EVENT | FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run_test();

	free_mr_res();
	ft_free_res();
	return ft_exit_code(ret);
}
