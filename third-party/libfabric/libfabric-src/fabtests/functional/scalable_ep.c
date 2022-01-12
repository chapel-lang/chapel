/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under the BSD license
 * below:
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

#include <rdma/fi_errno.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>

#include <shared.h>


static int ctx_cnt = 2;
static int rx_ctx_bits = 0;
static struct fid_ep *sep;
static struct fid_ep **tx_ep, **rx_ep;
static struct fid_cq **txcq_array;
static struct fid_cq **rxcq_array;
static fi_addr_t *remote_rx_addr;

static void free_res(void)
{
	if (rx_ep) {
		FT_CLOSEV_FID(rx_ep, ctx_cnt);
		free(rx_ep);
		rx_ep = NULL;
	}
	if (tx_ep) {
		FT_CLOSEV_FID(tx_ep, ctx_cnt);
		free(tx_ep);
		tx_ep = NULL;
	}
	if (rxcq_array) {
		FT_CLOSEV_FID(rxcq_array, ctx_cnt);
		free(rxcq_array);
		rxcq_array = NULL;
	}
	if (txcq_array) {
		FT_CLOSEV_FID(txcq_array, ctx_cnt);
		free(txcq_array);
		txcq_array = NULL;
	}
}

static int alloc_ep_res(struct fid_ep *sep)
{
	int i, ret;

	/* Get number of bits needed to represent ctx_cnt */
	while (ctx_cnt >> ++rx_ctx_bits);

	av_attr.rx_ctx_bits = rx_ctx_bits;

	ret = ft_alloc_ep_res(fi);
	if (ret)
		return ret;

	txcq_array = calloc(ctx_cnt, sizeof *txcq_array);
	rxcq_array = calloc(ctx_cnt, sizeof *rxcq_array);
	tx_ep = calloc(ctx_cnt, sizeof *tx_ep);
	rx_ep = calloc(ctx_cnt, sizeof *rx_ep);
	remote_rx_addr = calloc(ctx_cnt, sizeof *remote_rx_addr);

	if (!buf || !txcq_array || !rxcq_array || !tx_ep || !rx_ep || !remote_rx_addr) {
		perror("malloc");
		return -1;
	}

	for (i = 0; i < ctx_cnt; i++) {
		ret = fi_tx_context(sep, i, NULL, &tx_ep[i], NULL);
		if (ret) {
			FT_PRINTERR("fi_tx_context", ret);
			return ret;
		}

		ret = fi_cq_open(domain, &cq_attr, &txcq_array[i], NULL);
		if (ret) {
			FT_PRINTERR("fi_cq_open", ret);
			return ret;
		}

		ret = fi_rx_context(sep, i, NULL, &rx_ep[i], NULL);
		if (ret) {
			FT_PRINTERR("fi_rx_context", ret);
			return ret;
		}

		ret = fi_cq_open(domain, &cq_attr, &rxcq_array[i], NULL);
		if (ret) {
			FT_PRINTERR("fi_cq_open", ret);
			return ret;
		}
	}

	return 0;
}

static int bind_ep_res(void)
{
	int i, ret;

	ret = fi_scalable_ep_bind(sep, &av->fid, 0);
	if (ret) {
		FT_PRINTERR("fi_scalable_ep_bind", ret);
		return ret;
	}

	for (i = 0; i < ctx_cnt; i++) {
		ret = fi_ep_bind(tx_ep[i], &txcq_array[i]->fid, FI_SEND);
		if (ret) {
			FT_PRINTERR("fi_ep_bind", ret);
			return ret;
		}

		ret = fi_enable(tx_ep[i]);
		if (ret) {
			FT_PRINTERR("fi_enable", ret);
			return ret;
		}
	}

	for (i = 0; i < ctx_cnt; i++) {
		ret = fi_ep_bind(rx_ep[i], &rxcq_array[i]->fid, FI_RECV);
		if (ret) {
			FT_PRINTERR("fi_ep_bind", ret);
			return ret;
		}

		ret = fi_enable(rx_ep[i]);
		if (ret) {
			FT_PRINTERR("fi_enable", ret);
			return ret;
		}

		ret = fi_recv(rx_ep[i], rx_buf, MAX(rx_size, FT_MAX_CTRL_MSG),
			      mr_desc, 0, NULL);
		if (ret) {
			FT_PRINTERR("fi_recv", ret);
			return ret;
		}
	}

	ret = fi_enable(sep);
	if (ret) {
		FT_PRINTERR("fi_enable", ret);
		return ret;
	}

	return 0;
}

static int wait_for_comp(struct fid_cq *cq)
{
	struct fi_cq_entry comp;
	int ret;

	do {
		ret = fi_cq_read(cq, &comp, 1);
	} while (ret < 0 && ret == -FI_EAGAIN);

	if (ret != 1)
		FT_PRINTERR("fi_cq_read", ret);
	else
		ret = 0;

	return ret;
}

#define DATA 0x12345670

static int run_test()
{
	int ret = 0, i;
	uint32_t data;
	uint32_t *tb = (uint32_t *)tx_buf;
	uint32_t *rb = (uint32_t *)rx_buf;

	if (opts.dst_addr) {
		for (i = 0; i < ctx_cnt && !ret; i++) {
			fprintf(stdout, "Posting send for ctx: %d\n", i);
			tb[0] = DATA + i;
			ret = fi_send(tx_ep[i], tx_buf, tx_size, mr_desc,
				      remote_rx_addr[i], NULL);
			if (ret) {
				FT_PRINTERR("fi_send", ret);
				return ret;
			}

			ret = wait_for_comp(txcq_array[i]);
		}
	} else {
		for (i = 0; i < ctx_cnt && !ret; i++) {
			fprintf(stdout, "wait for recv completion for ctx: %d\n", i);
			ret = wait_for_comp(rxcq_array[i]);

			data = DATA + i;
			if (memcmp(&data, rx_buf, 4) != 0) {
				fprintf(stdout, "failed compare expected 0x%x,"
					" read 0x%x\n", data, rb[0]);
			}
		}
	}

	return ret;
}

static int init_fabric(void)
{
	int ret;

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	/* Check the optimal number of TX and RX contexts supported by the provider */
	ctx_cnt = MIN(ctx_cnt, fi->domain_attr->tx_ctx_cnt);
	ctx_cnt = MIN(ctx_cnt, fi->domain_attr->rx_ctx_cnt);
	if (!ctx_cnt) {
		fprintf(stderr, "Provider doesn't support contexts\n");
		return 1;
	}

	fi->ep_attr->tx_ctx_cnt = ctx_cnt;
	fi->ep_attr->rx_ctx_cnt = ctx_cnt;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	ret = fi_scalable_ep(domain, fi, &sep, NULL);
	if (ret) {
		FT_PRINTERR("fi_scalable_ep", ret);
		return ret;
	}

	ret = alloc_ep_res(sep);
	if (ret)
		return ret;

	ret = bind_ep_res();
	return ret;
}

static int init_av(void)
{
	size_t addrlen;
	int ret, i;

	if (opts.dst_addr) {
		ret = ft_av_insert(av, fi->dest_addr, 1, &remote_fi_addr, 0, NULL);
		if (ret)
			return ret;

		addrlen = FT_MAX_CTRL_MSG;
		ret = fi_getname(&sep->fid, tx_buf, &addrlen);
		if (ret) {
			FT_PRINTERR("fi_getname", ret);
			return ret;
		}

		ret = fi_send(tx_ep[0], tx_buf, addrlen,
			      mr_desc, remote_fi_addr, NULL);
		if (ret) {
			FT_PRINTERR("fi_send", ret);
			return ret;
		}

		ret = wait_for_comp(rxcq_array[0]);
		if (ret)
			return ret;
	} else {
		ret = wait_for_comp(rxcq_array[0]);
		if (ret)
			return ret;

		ret = ft_av_insert(av, rx_buf, 1, &remote_fi_addr, 0, NULL);
		if (ret)
			return ret;

		ret = fi_send(tx_ep[0], tx_buf, 1,
			      mr_desc, remote_fi_addr, NULL);
		if (ret) {
			FT_PRINTERR("fi_send", ret);
			return ret;
		}
	}

	for (i = 0; i < ctx_cnt; i++)
		remote_rx_addr[i] = fi_rx_addr(remote_fi_addr, i, rx_ctx_bits);

	ret = fi_recv(rx_ep[0], rx_buf, rx_size, mr_desc, 0, NULL);
	if (ret) {
		FT_PRINTERR("fi_recv", ret);
		return ret;
	}

	ret = wait_for_comp(txcq_array[0]);
	return ret;
}


static int run(void)
{
	int ret = 0;

	ret = init_fabric();
	if (ret)
		return ret;

	ret = init_av();
	if (ret)
		return ret;

	ret = run_test();

	/*TODO: Add a local finalize applicable for scalable ep */
	//ft_finalize(fi, tx_ep[0], txcq_array[0], rxcq_array[0], remote_rx_addr[0]);

	return ret;
}

int main(int argc, char **argv)
{
	int ret, op;

	opts = INIT_OPTS;
	opts.options = FT_OPT_SIZE;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "h" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "An RDM client-server example with scalable endpoints.\n");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_MSG | FI_NAMED_RX_CTX;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run();

	free_res();
	/* Closes the scalable ep that was allocated in the test */
	FT_CLOSE_FID(sep);
	ft_free_res();
	return ft_exit_code(ret);
}
