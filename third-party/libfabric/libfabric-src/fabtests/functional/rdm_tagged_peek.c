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
#include <unistd.h>

#include <rdma/fi_errno.h>
#include <rdma/fi_tagged.h>

#include <shared.h>

static struct fi_context fi_context;

static int wait_for_send_comp(int count)
{
	int ret, completions = 0;
	struct fi_cq_tagged_entry comp;

	do {
		ret = fi_cq_sread(txcq, &comp, 1, NULL, -1);
		if (ret != 1) {
			FT_PRINTERR("fi_cq_sread", ret);
			return ret;
		}
		completions++;
	} while (completions < count);

	return 0;
}

static int tag_queue_op(uint64_t tag, int recv, uint64_t flags)
{
	int ret;
	struct fi_cq_tagged_entry comp;
	struct fi_msg_tagged msg = {0};
	struct fi_cq_err_entry cq_err;
	struct iovec iov;
	void *desc;

	if (recv) {
		iov.iov_base = buf;
		iov.iov_len = rx_size;
		msg.msg_iov = &iov;
		desc = mr_desc;
		msg.desc = &desc;
		msg.iov_count = 1;
		msg.addr = remote_fi_addr;
	}
	msg.tag = tag;
	msg.context = &fi_context;

	ret = fi_trecvmsg(ep, &msg, flags);
	if (ret) {
		FT_PRINTERR("fi_trecvmsg", ret);
		return ret;
	}

	ret = fi_cq_sread(rxcq, &comp, 1, NULL, -1);
	if (ret != 1) {
		if (ret == -FI_EAVAIL) {
			ret = fi_cq_readerr(rxcq, &cq_err, 0);
			if (ret < 0)
				FT_PRINTERR("fi_cq_readerr", ret);
			else
				ret = -cq_err.err;
		} else {
			FT_PRINTERR("fi_cq_sread", ret);
		}
	}
	return ret;
}

static int run(void)
{
	int i, ret;

	ret = ft_init_fabric();
	if (ret)
		return ret;

	if (opts.dst_addr) {
		printf("Searching for a bad msg\n");
		ret = tag_queue_op(0xbad, 0, FI_PEEK);
		if (ret != -FI_ENOMSG) {
			FT_PRINTERR("FI_PEEK", ret);
			return ret;
		}

		printf("Searching for a bad msg with claim\n");
		ret = tag_queue_op(0xbad, 0, FI_PEEK | FI_CLAIM);
		if (ret != -FI_ENOMSG) {
			FT_PRINTERR("FI_PEEK", ret);
			return ret;
		}

		printf("Retrieving fifth message\n");
		ret = tag_queue_op(0x900d+4, 1, 0);
		if (ret != 1) {
			FT_PRINTERR("Receive after peek", ret);
			return ret;
		}
		printf("Messages 1-4 should be in unexpected queue\n");

		printf("Searching for first msg\n");
		ret = tag_queue_op(0x900d, 0, FI_PEEK);
		if (ret != 1) {
			FT_PRINTERR("FI_PEEK", ret);
			return ret;
		}

		printf("Receiving first msg\n");
		ret = tag_queue_op(0x900d, 1, 0);
		if (ret != 1) {
			FT_PRINTERR("Receive after peek", ret);
			return ret;
		}

		printf("Searching for second msg to claim\n");
		ret = tag_queue_op(0x900d+1, 0, FI_PEEK | FI_CLAIM);
		if (ret != 1) {
			FT_PRINTERR("FI_PEEK | FI_CLAIM", ret);
			return ret;
		}

		printf("Receiving second msg\n");
		ret = tag_queue_op(0x900d+1, 1, FI_CLAIM);
		if (ret != 1) {
			FT_PRINTERR("FI_CLAIM", ret);
			return ret;
		}

		printf("Searching for third msg to peek and discard\n");
		ret = tag_queue_op(0x900d+2, 0, FI_PEEK | FI_DISCARD);
		if (ret != 1) {
			FT_PRINTERR("FI_PEEK | FI_DISCARD", ret);
			return ret;
		}

		printf("Checking to see if third msg was discarded\n");
		ret = tag_queue_op(0x900d+2, 0, FI_PEEK);
		if (ret != -FI_ENOMSG) {
			FT_PRINTERR("FI_PEEK", ret);
			return ret;
		}

		printf("Searching for fourth msg to claim and discard\n");
		ret = tag_queue_op(0x900d+3, 0, FI_PEEK | FI_CLAIM);
		if (ret != 1) {
			FT_PRINTERR("FI_DISCARD", ret);
			return ret;
		}

		printf("Discarding fourth msg\n");
		ret = tag_queue_op(0x900d+3, 0, FI_CLAIM | FI_DISCARD);
		if (ret != 1) {
			FT_PRINTERR("FI_CLAIM", ret);
			return ret;
		}
		/* sync with sender before ft_finalize, since we sent
		 * and received messages outside of the sequence numbers
		 * maintained by common code */
		ret = fi_tsend(ep, tx_buf, 1, mr_desc,
				remote_fi_addr, 0xabc,
				&tx_ctx_arr[0].context);
		if (ret)
			return ret;
		ret = wait_for_send_comp(1);
		if (ret)
			return ret;
	} else {
		printf("Sending five tagged messages\n");
		for(i = 0; i < 5; i++) {
			ret = fi_tsend(ep, tx_buf, tx_size, mr_desc,
				       remote_fi_addr, 0x900d+i,
				       &tx_ctx_arr[i].context);
			if (ret)
				return ret;
		}
		printf("Waiting for messages to complete\n");
		ret = wait_for_send_comp(5);
		if (ret)
			return ret;
		ret = tag_queue_op(0xabc, 1, 0);
		if (ret != 1) {
			FT_PRINTERR("Receive sync", ret);
			return ret;
		}
	}

	ft_finalize();
	return 0;
}

int main(int argc, char **argv)
{
	int ret, op;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;
	opts.comp_method = FT_COMP_SREAD;
	opts.window_size = 5;

	hints = fi_allocinfo();
	if (!hints) {
		FT_PRINTERR("fi_allocinfo", -FI_ENOMEM);
		return EXIT_FAILURE;
	}

	while ((op = getopt(argc, argv, "h" CS_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parsecsopts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			ft_csusage(argv[0], "An RDM client-server example that uses tagged search.\n");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->domain_attr->resource_mgmt = FI_RM_ENABLED;
	hints->tx_attr->msg_order = FI_ORDER_SAS;
	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_TAGGED;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run();

	ft_free_res();
	return ft_exit_code(ret);
}
