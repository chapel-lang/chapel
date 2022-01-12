/*
 * Copyright (c) 2017 Intel Corporation.  All rights reserved.
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
#include <rdma/fi_cm.h>

#include <shared.h>


static int listener;


static int run(void)
{
	int ret;

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	if (!listener)
		fi->tx_attr->op_flags = FI_MULTICAST;

	ret = ft_alloc_active_res(fi);
	if (ret)
		return ret;

	ret = ft_enable_ep_recv();
	if (ret)
		return ret;

	ret = ft_join_mc();
	if (ret)
		return ret;

	remote_fi_addr = fi_mc_addr(mc);
	return listener ? ft_recv_greeting(ep) : ft_send_greeting(ep);
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "Mh" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'M':
			listener = 1;
			break;
		case '?':
		case 'h':
			goto usage;
		}
	}

	if (optind == argc)
		goto usage;

	opts.dst_addr = argv[optind];

	hints->ep_attr->type		= FI_EP_DGRAM;
	hints->caps			= FI_MSG | FI_MULTICAST;
	hints->mode			= FI_CONTEXT;
	hints->domain_attr->mr_mode 	= opts.mr_mode;

	ret = run();

	ft_free_res();
	return ft_exit_code(ret);
usage:
	ft_mcusage(argv[0], "A simple multicast example.");
	return EXIT_FAILURE;
}
