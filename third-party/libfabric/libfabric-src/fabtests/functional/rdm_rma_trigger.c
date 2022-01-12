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
#include <rdma/fi_trigger.h>

#include <shared.h>

struct fi_triggered_context triggered_ctx;

static char *welcome_text1 = "Hello1 from Client!";
static char *welcome_text2 = "Hello2 from Client!";

static int rma_write_trigger(void *src, size_t size,
			     struct fid_cntr *cntr, size_t threshold)
{
	int ret;
	triggered_ctx.event_type = FI_TRIGGER_THRESHOLD;
	triggered_ctx.trigger.threshold.cntr = cntr;
	triggered_ctx.trigger.threshold.threshold = threshold;
	ret = fi_write(alias_ep, src, size, mr_desc,
		       remote_fi_addr, remote.addr, remote.key,
		       &triggered_ctx);
 	if (ret){
 		FT_PRINTERR("fi_write", ret);
 		return ret;
	}
	return 0;
}

static int run_test(void)
{
	int ret = 0;

	ret = ft_init_fabric();
	if (ret)
		return ret;

	ret = ft_init_alias_ep(FI_TRANSMIT | FI_TRIGGER);
	if (ret)
		return ret;

	ret = ft_exchange_keys(&remote);
	if (ret)
		return ret;

	if (opts.dst_addr) {
		sprintf(tx_buf, "%s%s", welcome_text1, welcome_text2);

		fprintf(stdout, "Triggered RMA write to server\n");
		ret = rma_write_trigger((char *) tx_buf + strlen(welcome_text1),
					strlen(welcome_text2), txcntr, 3);
		if (ret)
			goto out;

		fprintf(stdout, "RMA write to server\n");
		ret = fi_write(ep, tx_buf, strlen(welcome_text1), mr_desc,
			       remote_fi_addr, remote.addr, remote.key,
			       &tx_ctx);
 		if (ret){
 			FT_PRINTERR("fi_write", ret);
 			goto out;
		}
		/* The value of the counter is 4 including a transfer during
		 * init_av and ft_exchange_keys() */
		ret = fi_cntr_wait(txcntr, 4, -1);
		if (ret < 0) {
			FT_PRINTERR("fi_cntr_wait", ret);
			goto out;
		}

		fprintf(stdout, "Received completion events for RMA write operations\n");
	} else {
		/* The value of the counter is 4 including a transfer during
		 * init_av and ft_exchange_keys() */
		ret = fi_cntr_wait(rxcntr, 4, -1);
		if (ret < 0) {
			FT_PRINTERR("fi_cntr_wait", ret);
			goto out;
		}

		ret = check_recv_msg(welcome_text2);
		if (ret)
			return ret;
		fprintf(stdout, "Received data from Client: %s\n", (char *) rx_buf);
	}

out:
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options = FT_OPT_SIZE | FT_OPT_RX_CNTR | FT_OPT_TX_CNTR;
	opts.transfer_size = strlen(welcome_text1) + strlen(welcome_text2);

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
			ft_usage(argv[0], "A simple RDM client-server triggered RMA example with alias ep.");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_MSG | FI_RMA | FI_RMA_EVENT | FI_TRIGGER;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run_test();

	ft_free_res();
	return ft_exit_code(ret);
}
