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
#include <rdma/fi_tagged.h>
#include "shared.h"

static uint16_t max_opts = 100;
int delay, tagged;

/*
 * The general strategy here is that we call send_loop and do as many sends
 * in a row as we can until we get FI_EAGAIN which prevents us from continuing and
 * we have to drain the send_cq.  Then we do it again, until we've sent
 * all the messages we were going to send.
 */
static int send_loop(size_t size) {
	int q_opts = 0;
	int ret;
	struct fi_context send_ctx[max_opts];

	while (q_opts < max_opts) {
		do {
			ft_tag = q_opts + 1;
			if (tagged)
				ret = fi_tsend(ep, tx_buf, size, NULL, remote_fi_addr,
					ft_tag, (void *) &send_ctx[q_opts]);
			else
				ret = fi_send(ep, tx_buf, size, NULL, remote_fi_addr,
					(void *) &send_ctx[q_opts]);

			if (ret == FI_SUCCESS) {
				tx_seq++;
				q_opts++;
			}
		} while (!ret && (q_opts != max_opts));

		if (ret < 0) {
			if (ret == -FI_EAGAIN) {
				ret = ft_get_tx_comp(tx_seq);
				if (ret)
					return ret;
			} else {
				FT_PRINTERR("Send OP", ret);
				return ret;
			}
		}
	}

	ret = ft_get_tx_comp(tx_seq);
	if (ret)
		return ret;

	if (opts.verbose)
		printf("Success: Completed %d queued ops\n", q_opts);

	return 0;
}

static int receive_loop(size_t size)
{
	int ret;
	int q_opts = 0;
	struct fi_context recv_ctx[max_opts];

	while (q_opts < max_opts) {
		do {
			ft_tag = q_opts + 1;
			if (tagged)
				ret = fi_trecv(ep, rx_buf, size, NULL, remote_fi_addr,
					ft_tag, 0x0, (void *) &recv_ctx[q_opts]);
			else
				ret = fi_recv(ep, rx_buf, size, NULL, remote_fi_addr,
					(void *) &recv_ctx[q_opts]);

			if (ret == FI_SUCCESS) {
				rx_seq++;
				q_opts++;
			}
		} while (!ret && (q_opts != max_opts));

		if (ret < 0) {
			if (ret == -FI_EAGAIN) {
				if (delay > 0)
					sleep(delay);

				ret = ft_get_rx_comp(rx_seq);
				if (ret)
					return ret;
			} else {
				FT_PRINTERR("Recv OP", ret);
				return ret;
			}
		}
	}

	if (delay > 0)
		sleep(delay);

	ret = ft_get_rx_comp(rx_seq);
	if (ret)
		return ret;

	if (opts.verbose)
		printf("Success: Completed %d queued ops\n", q_opts);

	return 0;
}

static int overflow_test(void)
{
	int ret = 0;

	if (opts.dst_addr) {
		printf("Start testing for fi_send operations: %d\n", (int) max_opts);
		ret = send_loop(opts.transfer_size);
		if (ret)
			printf("ERROR: calling send_master %d\n", ret);
	} else {
		printf("Start testing for fi_recv operations: %d\n", (int) max_opts);
		ret = receive_loop(opts.transfer_size);
		if (ret)
			printf("ERROR: calling receive_loop %d\n", ret);
	}
	printf("GOOD, Completed Overflow Testing\n");

	return 0;
}

static int run_test(void)
{
	int ret = 0;

	if (hints->ep_attr->type == FI_EP_MSG)
		ret = ft_init_fabric_cm();
	else
		ret = ft_init_fabric();
	if (ret)
		return ret;

	ret = overflow_test();

	ft_sync();
	ft_finalize();

	return ret;
}

int main(int argc, char **argv)
{
	int op;
	int ret = 0;

	opts = INIT_OPTS;
	opts.tx_cq_size = max_opts;
	opts.rx_cq_size = max_opts;
	delay = 0;
	tagged = 0;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "S:R:m:l:T:X:ActdjwvVh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'S':
			opts.tx_cq_size = strtoul(optarg, NULL, 0);
			printf("send cq size: %d\n", (int) opts.tx_cq_size);
			break;
		case 'R':
			opts.rx_cq_size = strtoul(optarg, NULL, 0);
			printf("recv cq size: %d\n", (int) opts.rx_cq_size);
			break;
		case 'T':
			hints->tx_attr->size = strtoul(optarg, NULL, 0);
			printf("tx context size: %d\n", (int) hints->tx_attr->size);
			break;
		case 'X':
			hints->rx_attr->size = strtoul(optarg, NULL, 0);
			printf("rx context size: %d\n", (int) hints->rx_attr->size);
			break;
		case 'm':
			max_opts = strtoul(optarg, NULL, 0);
			printf("max_opts set to %d\n", max_opts);
			break;
		case 'j':
			opts.options |= FT_OPT_CQ_SHARED;
			printf("using single shared CQ\n");
			break;
		case 'l':
			opts.transfer_size = strtoul(optarg, NULL, 0);
			printf("Testing Message Size: %zu\n", opts.transfer_size);
			break;
		case 't':
			tagged = 1;
			hints->caps |= FI_TAGGED;
			printf("tagged messaging enabled\n");
			break;
		case 'd':
			delay = 1;
			break;
		case 'A':
			hints->domain_attr->av_type = FI_AV_TABLE;
			printf("AV TABLE enabled\n");
			break;
		case 'V':
			opts.verbose = 1;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "Resource Management Functional Test");
			FT_PRINT_OPTS_USAGE("-S <int>", "Size of send CQ");
			FT_PRINT_OPTS_USAGE("-R <int>", "Size of recv CQ");
			FT_PRINT_OPTS_USAGE("-T <int>", "Number of TX Contexts");
			FT_PRINT_OPTS_USAGE("-X <int>", "Number of RX Contexts");
			FT_PRINT_OPTS_USAGE("-m <int>", "number of operations to post");
			FT_PRINT_OPTS_USAGE("-l <int>", "message length to test");
			FT_PRINT_OPTS_USAGE("-j", "Enable shared cq");
			FT_PRINT_OPTS_USAGE("-t", "Enable tagged message testing");
			FT_PRINT_OPTS_USAGE("-d", "Enable setting a delay");
			FT_PRINT_OPTS_USAGE("-A", "Enable av table testing (only RDM/DGRAM EP)");
			FT_PRINT_OPTS_USAGE("-V", "Enable verbose printing");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->resource_mgmt = FI_RM_ENABLED;

	ret = run_test();

	ft_free_res();
	return ft_exit_code(ret);
}
