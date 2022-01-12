/*
 * Copyright (c) 2013-2020 Intel Corporation. All rights reserved.
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
#include "shared.h"

#define CANCEL_TAG 0xC
#define STANDARD_TAG 0xA

static int recv_cancel_client(void)
{
	int ret;

	/* sync with server */
	ret = ft_rx(ep, 1);
	if (ret)
		return ret;

	ft_tag = CANCEL_TAG;
	ret = ft_post_tx(ep, remote_fi_addr, opts.transfer_size, NO_CQ_DATA,
			 &tx_ctx);
	if (ret)
		return ret;

	if (opts.verbose)
		fprintf(stdout, "CANCEL msg posted to server\n");

	ft_tag = STANDARD_TAG;
	ret = ft_post_tx(ep, remote_fi_addr, opts.transfer_size, NO_CQ_DATA,
			 &tx_ctx);
	if (ret)
		return ret;

	if (opts.verbose)
		fprintf(stdout, "STANDARD msg posted to server\n");

	ret = ft_get_tx_comp(tx_seq);
	if (ret)
		return ret;

	return 0;
}

static int recv_cancel_host(void)
{
	int ret = 0;
	int retries = 0;
	struct fi_cq_err_entry recv_completion, cancel_error_entry;
	struct fi_context cancel_recv_ctx, standard_recv_ctx;

	/* Pre-post two recvs, one of which will be cancelled */
	ft_tag = CANCEL_TAG;
	ret = ft_post_rx(ep, opts.transfer_size, &cancel_recv_ctx);
	if (ret)
		return ret;

	ft_tag = STANDARD_TAG;
	ret = ft_post_rx(ep, opts.transfer_size, &standard_recv_ctx);
	if (ret)
		return ret;

	/* Cancel the first recv*/
	ret = fi_cancel((struct fid *)ep, &cancel_recv_ctx);
	if (ret) {
		FT_PRINTERR("fi_cancel", ret);
		return ret;
	}

	/* sync with client */
	ft_tag = 0;
	ret = ft_tx(ep, remote_fi_addr, 1, &tx_ctx);
	if (ret)
		return ret;

	/* Wait for fi_cq_read to fail indicating an err entry */
	do {
		ret = fi_cq_read(rxcq, &recv_completion, 1);
		if (ret == -FI_EAVAIL)
			break;
		else
			retries++;
		usleep(1000);
	} while ((ret == -FI_EAGAIN) && (retries < 5000));
	if (retries >= 5000) {
		FT_PRINTERR("ERROR: no error CQ entry in cq_read deteceted",
			    -FI_EOTHER);
		return -FI_EOTHER;
	} else {
		if (opts.verbose)
			fprintf(stdout, "GOOD: detected error cq entry\n");
	}

	/* Verify the error CQ has been populated */
	if (fi_cq_readerr(rxcq, &cancel_error_entry, 0) != 1) {
		FT_PRINTERR("ERROR: No cancel CQ error entry was populated",
			    -FI_EOTHER);
		return -FI_EOTHER;
	}

	if (cancel_error_entry.err != FI_ECANCELED) {
		FT_PRINTERR("ERROR: error code is incorrect", -FI_EOTHER);
		return -FI_EOTHER;
	}

	if (!(cancel_error_entry.flags & FI_RECV)) {
		FT_PRINTERR("ERROR: cancelled completion flags are incorrect",
			    -FI_EOTHER);
		return -FI_EOTHER;
	}

	if (opts.verbose)
		fprintf(stdout, "GOOD: error entry has expected values\n");

	/* Verify only one CQ err entry can be read */
	if (fi_cq_readerr(rxcq, &cancel_error_entry, 0) != -FI_EAGAIN) {
		FT_PRINTERR("ERROR: Another CQ error entry was populated",
			    -FI_EOTHER);
		return -FI_EOTHER;
	}

	if (opts.verbose)
		fprintf(stdout, "GOOD: no extra error entries detected\n");

	/* Check for second recv completion*/
	do {
		ret = fi_cq_read(rxcq, &recv_completion, 1);
		if (ret > 0) {
			if (recv_completion.op_context != &standard_recv_ctx) {
				FT_PRINTERR("ERROR: op_context does not match",
					    -FI_EOTHER);
				return -FI_EOTHER;
			}
		} else if ((ret <= 0) && (ret != -FI_EAGAIN)) {
			FT_PRINTERR("fi_cq_read", ret);
		}
	} while (ret == -FI_EAGAIN);

	if (opts.verbose)
		fprintf(stdout, "GOOD: Completed uncancelled recv\n");

	/* Repost cancelled recv and get completion */
	ft_tag = CANCEL_TAG;
	ret = ft_post_rx(ep, opts.transfer_size, &cancel_recv_ctx);
	if (ret)
		return ret;

	do {
		ret = fi_cq_read(rxcq, &recv_completion, 1);
		if (ret > 0) {
			if (recv_completion.op_context != &cancel_recv_ctx) {
				FT_PRINTERR("ERROR: op_context does not match",
					    -FI_EOTHER);
				return -FI_EOTHER;
			}
		} else if ((ret <= 0) && (ret != -FI_EAGAIN)) {
			FT_PRINTERR("fi_cq_read", ret);
		}
	} while (ret == -FI_EAGAIN);

	if (opts.verbose)
		fprintf(stdout, "GOOD: Completed reposted cancelled recv\n");

	fprintf(stdout, "GOOD: Completed Recv Cancel Test\n");

	return 0;
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

	if (opts.dst_addr)
		return recv_cancel_client();
	else
		return recv_cancel_host();
}

int main(int argc, char **argv)
{
	int op;
	int ret = 0;

	opts = INIT_OPTS;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "Vh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'V':
			opts.verbose = 1;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "Recv Cancel Functional test");
			FT_PRINT_OPTS_USAGE("-V", "Enable Verbose printing");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = FI_TAGGED;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run_test();

	ft_free_res();
	return ft_exit_code(ret);
}
