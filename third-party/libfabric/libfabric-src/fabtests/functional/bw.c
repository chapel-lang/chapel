/*
 * Copyright (c) 2019 Intel Corporation.  All rights reserved.
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
#include <getopt.h>
#include <unistd.h>

#include <shared.h>

int sleep_time = 0;

static ssize_t post_one_tx(struct ft_context *msg)
{
	ssize_t ret;

	if (ft_check_opts(FT_OPT_VERIFY_DATA | FT_OPT_ACTIVE)) {
		ret = ft_fill_buf(msg->buf + ft_tx_prefix_size(),
				  opts.transfer_size);
		if (ret)
			return ret;
	}

	return ft_post_tx_buf(ep, remote_fi_addr, opts.transfer_size,
			      NO_CQ_DATA, &msg->context, msg->buf,
			      msg->desc, 0);
}

static ssize_t wait_check_rx_bufs(void)
{
	ssize_t ret;
	int i;

	ret = ft_get_rx_comp(rx_seq);
	if (ret)
		return ret;

	if (!ft_check_opts(FT_OPT_VERIFY_DATA | FT_OPT_ACTIVE))
		return 0;

	for (i = 0; i < opts.window_size; i++) {
		ret = ft_check_buf((char *) rx_ctx_arr[i].buf +
				   ft_rx_prefix_size(), opts.transfer_size);
		if (ret)
			return ret;
	}

	return 0;
}

static int post_rx_sync(void)
{
	int ret;

	ret = ft_post_rx(ep, rx_size, &rx_ctx);
	if (ret)
		return ret;

	if (opts.dst_addr) {
		ret = ft_tx(ep, remote_fi_addr, 1, &tx_ctx);
		if (ret)
			return ret;

		ret = ft_get_rx_comp(rx_seq);
	} else {
		ret = ft_get_rx_comp(rx_seq);
		if (ret)
			return ret;

		ret = ft_tx(ep, remote_fi_addr, 1, &tx_ctx);
	}

	return ret;
}

static int run_loop(void)
{
	int ret, i;

	/* Receive side delay is used in order to let the sender
 	   get ahead of the receiver and post multiple sends
	   before the receiver begins processing them. */
	if (!opts.dst_addr)
		sleep(sleep_time);

	ft_start();
	if (opts.dst_addr) {
		for (i = 0; i < opts.window_size; i++) {
			ret = post_one_tx(&tx_ctx_arr[i]);
			if (ret)
				return ret;
		}

		ret = ft_get_tx_comp(tx_seq);
		if (ret)
			return ret;
	} else {
		for (i = 0; i < opts.window_size; i++) {
			ret = ft_post_rx_buf(ep, opts.transfer_size,
					     &rx_ctx_arr[i].context,
					     rx_ctx_arr[i].buf,
					     rx_ctx_arr[i].desc, 0);
			if (ret)
				return ret;
		}

		ret = wait_check_rx_bufs();
		if (ret)
			return ret;
	}
	ft_stop();

	if (opts.options & FT_OPT_OOB_SYNC)
		ret = ft_sync();
	else
		ret = post_rx_sync();
	if (ret)
		return ret;

	if (opts.machr)
		show_perf_mr(opts.transfer_size, opts.window_size, &start, &end, 1,
				opts.argc, opts.argv);
	else
		show_perf(NULL, opts.transfer_size, opts.window_size, &start, &end, 1);

	return ret;
}

static int run(void)
{
	int ret, i;

	ret = hints->ep_attr->type == FI_EP_MSG ?
		ft_init_fabric_cm() : ft_init_fabric();
	if (ret)
		return ret;
	
	ret = ft_tx(ep, remote_fi_addr, 1, &tx_ctx);
	if (ret)
		return ret;

	ret = ft_get_tx_comp(tx_seq);
	if (ret)
		return ret;

	ret = ft_get_rx_comp(rx_seq);
	if (ret)
		return ret;

	if (!(opts.options & FT_OPT_SIZE)) {
		for (i = 0; i < TEST_CNT; i++) {
			if (!ft_use_size(i, opts.sizes_enabled))
				continue;
			opts.transfer_size = test_size[i].size;
			ret = run_loop();
			if (ret)
				goto out;
		}
	} else {
		ret = run_loop();
		if (ret)
			goto out;
	}

out:
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	hints->ep_attr->type = FI_EP_RDM;

	while ((op = getopt(argc, argv, "UW:vT:h" CS_OPTS ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			ft_parsecsopts(op, optarg, &opts);
			break;
		case 'W':
			opts.window_size = atoi(optarg);
			break;
		case 'U':
			hints->tx_attr->op_flags |= FI_DELIVERY_COMPLETE;
			break;
		case 'v':
			opts.options |= FT_OPT_VERIFY_DATA;
			break;
		case 'T':
			sleep_time = atoi(optarg);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "A bandwidth test with data verification.");
			FT_PRINT_OPTS_USAGE("-T sleep_time",
				"Receive side delay before starting");
			FT_PRINT_OPTS_USAGE("-v", "Enable data verification");
			FT_PRINT_OPTS_USAGE("-W window_size",
				"Set transmit window size before waiting for completion");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	opts.options |= FT_OPT_ALLOC_MULT_MR;

	if (hints->ep_attr->type == FI_EP_DGRAM) {
		fprintf(stderr, "This test does not support DGRAM endpoints\n");
		return -FI_EINVAL;
	}

	if (opts.options & FT_OPT_VERIFY_DATA) {
		hints->tx_attr->msg_order |= FI_ORDER_SAS;
		hints->rx_attr->msg_order |= FI_ORDER_SAS;
	}

	ret = run();

	ft_free_res();

	return ft_exit_code(ret);
}
