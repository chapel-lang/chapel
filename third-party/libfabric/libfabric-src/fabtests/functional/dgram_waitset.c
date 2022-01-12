/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2015-2016 Cisco Systems, Inc.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#include <shared.h>

static int alloc_ep_res(struct fi_info *fi)
{
	struct fi_wait_attr wait_attr;
	int ret;

	memset(&wait_attr, 0, sizeof wait_attr);
	wait_attr.wait_obj = FI_WAIT_UNSPEC;
	ret = fi_wait_open(fabric, &wait_attr, &waitset);
	if (ret) {
		FT_PRINTERR("fi_wait_open", ret);
		return ret;
	}

	ret = ft_alloc_active_res(fi);
	if (ret)
		return ret;

	return 0;
}

static int init_fabric(void)
{
	int ret;

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	ret = alloc_ep_res(fi);
	if (ret)
		return ret;

	ret = ft_enable_ep_recv();
	if (ret)
		return ret;

	return 0;
}

static int send_recv()
{
	struct fi_cq_entry comp;
	int ret;

	ret = fi_recv(ep, rx_buf, rx_size + ft_rx_prefix_size(),
		      mr_desc, 0, &rx_ctx);
	if (ret)
		return ret;

	ft_sync();

	fprintf(stdout, "Posting a send...\n");
	ret = ft_post_tx(ep, remote_fi_addr, tx_size, NO_CQ_DATA, &tx_ctx);
	if (ret)
		return ret;

	while ((tx_cq_cntr < tx_seq) || (rx_cq_cntr < rx_seq)) {
		/* Wait for completion events on CQs */
		ret = fi_wait(waitset, -1);
		if (ret < 0) {
			FT_PRINTERR("fi_wait", ret);
			return ret;
		}

		/* Read the send completion entry */
		ret = fi_cq_read(txcq, &comp, 1);
		if (ret > 0) {
			tx_cq_cntr++;
			fprintf(stdout, "Received send completion event!\n");
		} else if (ret < 0 && ret != -FI_EAGAIN) {
			if (ret == -FI_EAVAIL) {
				ret = ft_cq_readerr(txcq);
			} else {
				FT_PRINTERR("fi_cq_read", ret);
			}
			return ret;
		}

		/* Read the recv completion entry */
		ret = fi_cq_read(rxcq, &comp, 1);
		if (ret > 0) {
			rx_cq_cntr++;
			fprintf(stdout, "Received recv completion event!\n");
		} else if (ret < 0 && ret != -FI_EAGAIN) {
			if (ret == -FI_EAVAIL) {
				ret = ft_cq_readerr(rxcq);
			} else {
				FT_PRINTERR("fi_cq_read", ret);
			}
			return ret;
		}
	}

	return 0;
}

static int run(void)
{
	int ret;

	ret = init_fabric();
	if (ret)
		return ret;

	ret = ft_init_av();
	if (ret)
		return ret;

	return send_recv();
}

int main(int argc, char **argv)
{
	int op, ret = 0;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;
	opts.comp_method = FT_COMP_WAITSET;

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
			ft_usage(argv[0], "A DGRAM client-server example that uses waitset.\n");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type = FI_EP_DGRAM;
	hints->caps = FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run();

	ft_free_res();
	return ft_exit_code(ret);
}
