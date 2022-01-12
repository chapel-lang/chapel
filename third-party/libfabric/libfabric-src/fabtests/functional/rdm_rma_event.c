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

#include <shared.h>

struct fi_rma_iov local;

struct fi_context fi_ctx_write;
struct fi_context fi_ctx_read;

static int run_test(void)
{
	int ret = 0;
	const char *message = "Hello from Client!";
	size_t message_len = strlen(message) + 1;

	ret = ft_init_fabric();
	if (ret)
		return ret;

	ret = ft_exchange_keys(&remote);
	if (ret)
		return ret;

	if (opts.dst_addr) {
		fprintf(stdout, "RMA write to server\n");
		if (snprintf(tx_buf, tx_size, "%s", message) >= tx_size) {
                        fprintf(stderr, "Transmit buffer too small.\n");
                        return -FI_ETOOSMALL;
                }
		ret = fi_write(ep, tx_buf, message_len, mr_desc,
			       remote_fi_addr, remote.addr, remote.key,
			       &fi_ctx_write);
		if (ret)
			return ret;

		ret = ft_get_tx_comp(++tx_seq);
		if (ret)
			return ret;

		fprintf(stdout, "Received a completion event for RMA write\n");
	} else {
		ret = ft_get_rx_comp(rx_seq);
		if (ret)
			return ret;

		ret = check_recv_msg(message);
		if (ret)
			return ret;

		fprintf(stdout, "Received data from Client: %s\n", (char *) rx_buf);
	}

	/* TODO: need support for finalize operation to sync test */
	return 0;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options = FT_OPT_SIZE | FT_OPT_RX_CNTR | FT_OPT_TX_CNTR;

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
			ft_usage(argv[0], "A simple RDM client-server RMA example.");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_MSG | FI_RMA | FI_RMA_EVENT;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run_test();

	ft_free_res();
	return ft_exit_code(ret);
}
