/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
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
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <rdma/fi_tagged.h>
#include "shared.h"

int use_sendmsg, use_recvmsg, send_inj_count;

static int send_msg(int sendmsg, size_t size)
{
	int ret;
	ft_tag = 0xabcd;

	if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
		ret = ft_fill_buf(tx_buf, size);
		if (ret)
			return ret;
	}

	if (sendmsg) {
		ret = ft_sendmsg(ep, remote_fi_addr, size,
				&tx_ctx, FI_INJECT_COMPLETE);
		if (ret) {
			FT_PRINTERR("ft_sendmsg", ret);
			return ret;
		}
	} else {
		ret = ft_post_tx(ep, remote_fi_addr, size, NO_CQ_DATA, &tx_ctx);
		if (ret) {
			FT_PRINTERR("ft_post_rx", ret);
			return ret;
		}
	}
	ret = ft_cq_read_verify(txcq, &tx_ctx);
	if (ret) {
		FT_PRINTERR("ft_cq_read_verify", ret);
		return ret;
	}
	tx_cq_cntr++;
	/*
	Alter the tx buffer contents, if the inject event
	was properly generated, then the changes to the TX buffer
	will not be sent to the target.
	*/
	memset(tx_buf, 0xb, size);

	return 0;
}

static int receive_msg(int recvmsg, size_t size)
{
	int ret;
	struct fi_context inj_ctx;
	ft_tag = 0xabcd;

	if (recvmsg) {
		ret = ft_recvmsg(ep, FI_ADDR_UNSPEC, size,
				&inj_ctx, 0);
		if (ret) {
			FT_PRINTERR("ft_recvmsg", ret);
			return ret;
		}
	} else {
		ret = ft_post_rx(ep, size, &inj_ctx);
		if (ret) {
			FT_PRINTERR("ft_post_rx", ret);
			return ret;
		}
	}

	ret = ft_cq_read_verify(rxcq, &inj_ctx);
	if (ret) {
		FT_PRINTERR("ft_cq_read_verify", ret);
		return ret;
	}
	rx_cq_cntr++;

	if (ft_check_opts(FT_OPT_VERIFY_DATA)) {
		ret = ft_check_buf(rx_buf, size);
		if (ret)
			return ret;
	}

	return 0;
}

static int run_test(void)
{
	int ret = 0, i;

	if (!use_sendmsg)
		hints->tx_attr->op_flags |= FI_INJECT_COMPLETE;

	if (hints->ep_attr->type == FI_EP_MSG) {
		ret = ft_init_fabric_cm();
		if (ret)
			return ret;
	} else {
		ret = ft_init_fabric();
		if (ret)
			return ret;
	}

	fprintf(stdout, "Start testing FI_INJECT_COMPLETIONS\n");
	for (i = 0; i < send_inj_count; i++) {
		if (opts.dst_addr) {
			ret = send_msg(use_sendmsg, opts.transfer_size);
			if (ret)
				return ret;
		} else {
			ret = receive_msg(use_recvmsg, opts.transfer_size);
			if (ret)
				return ret;
		}
	}
	fprintf(stdout, "GOOD: Completed FI_INJECT_COMPLETIONS Testing\n");

	return 0;
}

int main(int argc, char **argv)
{
	int op;
	int ret = 0;

	opts = INIT_OPTS;
	use_sendmsg = 0;
	use_recvmsg = 0;
	send_inj_count = 1;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "m:OSRvh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'O':
			send_inj_count = 100;
			break;
		case 'm':
			opts.transfer_size = strtoul(optarg, NULL, 0);
			break;
		case 'S':
			use_sendmsg = 1;
			break;
		case 'R':
			use_recvmsg = 1;
			break;
		case 'v':
			opts.options |= FT_OPT_VERIFY_DATA;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "FI_Inject_Completion Functional Test");
			FT_PRINT_OPTS_USAGE("-m <size>", "size of Injection message");
			FT_PRINT_OPTS_USAGE("-S", "enable testing with fi_sendmsg");
			FT_PRINT_OPTS_USAGE("-R", "enable testing with fi_recvmsg");
			FT_PRINT_OPTS_USAGE("-O", "enable testing injection overrun");
			FT_PRINT_OPTS_USAGE("-v", "Enable DataCheck testing");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->mode = FI_CONTEXT;
	hints->caps = FI_TAGGED;
	hints->domain_attr->resource_mgmt = FI_RM_ENABLED;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run_test();

	ft_free_res();
	return ft_exit_code(ret);
}
